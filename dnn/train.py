import tensorflow as tf
import pandas as pd
import keras
from keras.utils import np_utils, multi_gpu_model
from keras.models import Model, Sequential, load_model
from keras.layers import Input, Dense, Activation, Dropout, add
from keras.layers.normalization import BatchNormalization
from keras.regularizers import l2
from keras.optimizers import Adam, SGD
from keras.callbacks import Callback, ModelCheckpoint

import os
os.environ["CUDA_VISIBLE_DEVICES"] = "0"

from sklearn.utils import class_weight
from sklearn.metrics import roc_auc_score, roc_curve

import matplotlib
matplotlib.use('Agg')

import ROOT
from sys import exit
import numpy as np

import csv
from sklearn.utils import shuffle
import os

from sklearn.preprocessing import StandardScaler, label_binarize

trainInput = './array/array_dnn_TTLJ_PowhegPythia_ttbb.h5'

name_inputvar = ['mbb','dRbb','dPhibb','dEtabb','Etabb','Phibb','mlbb','dRlbb','mlb1','dRlb1','mlb2','dRlb2','pt1','pt2','eta1','eta2','d1','d2','e1','e2','m1','m2','nbjets','lepton_pt','lepton_eta','lepton_m','btag3rd','btag4th']

print ("number of variables: "+str(len(list(name_inputvar))))

model_name = 'model_example'
df_data = pd.read_hdf(trainInput)
data = df_data

labels = data.filter(['signal'], axis=1)
all_event = data.filter(['event','signal'], axis=1)

data = data.filter(['signal']+name_inputvar)
data.astype('float32')

data = data.drop('signal', axis=1) #then drop label

###############
#split datasets
###############
groupped_event = all_event.drop_duplicates(subset=['event'])

nevt = len(groupped_event)
print("number of total event = "+str(len(groupped_event)))

split_nevt = groupped_event[:int(nevt*0.9)].iloc[-1]
split_point = -1
for idx, row in all_event.iterrows():
  if (row['event'] == split_nevt['event']):
    if split_point < 0:
        split_point = idx
        break

train_event = all_event[:split_point]
valid_event = all_event[split_point:]

train_sig = train_event.loc[labels['signal'] == 1]
train_bkg = train_event.loc[labels['signal'] == 0]

valid_sig = valid_event.loc[labels['signal'] == 1]
valid_bkg = valid_event.loc[labels['signal'] == 0]

train_idx = pd.concat([train_sig, train_bkg]).sort_index().index
valid_idx = pd.concat([valid_sig, valid_bkg]).sort_index().index

data_train = data.loc[train_idx,:].copy()
data_valid = data.loc[valid_idx,:].copy()

labels_train = labels.loc[train_idx,:].copy()
labels_valid = labels.loc[valid_idx,:].copy()

print('\n## NUMBER OF COMBINAITONS ##')
print('Training signal: '+str(len(train_sig))+' / validing signal: '+str(len(valid_sig))+' / training background: '+str(len(train_bkg))+' / validing background: '+str(len(valid_bkg)))
print('############################\n')

labels_train = labels_train.values
train_label = labels_train
labels_valid = labels_valid.values
valid_label = labels_valid

########################
#Standardization and PCA
########################
scaler = StandardScaler()
data_train_sc = scaler.fit_transform(data_train)
data_valid_sc = scaler.fit_transform(data_valid)
train_data = data_train_sc
valid_data = data_valid_sc

all_data = data
all_data = scaler.fit_transform(all_data)

#################################
#Keras model compile and training
#################################
nvar = len(name_inputvar)
a = 200
b = 0.08
init = 'glorot_uniform'

with tf.device("/cpu:0") :
    inputs = Input(shape=(nvar,))
    x = Dense(a, kernel_regularizer=l2(1E-2))(inputs)
    x = Dropout(b)(x)
    x = Dense(a, activation='relu', kernel_initializer=init, bias_initializer='zeros')(x)
    x = Dropout(b)(x)
    x = Dense(a, activation='relu', kernel_initializer=init, bias_initializer='zeros')(x)
    x = Dropout(b)(x)
    x = Dense(a, activation='relu', kernel_initializer=init, bias_initializer='zeros')(x)
    x = Dropout(b)(x)
    x = Dense(a, activation='relu', kernel_initializer=init, bias_initializer='zeros')(x)
    x = Dropout(b)(x)
    x = Dense(a, activation='relu', kernel_initializer=init, bias_initializer='zeros')(x)
    outputs = Dense(1, activation='sigmoid')(x)
    model = Model(inputs=inputs, outputs=outputs)

if not os.path.exists("models"): os.mkdir("models")

if os.path.exists("models/"+model_name+'/model.h5'): 
    print "\nModel exists already!\n"
    model = load_model("models/"+model_name+'/model.h5')
else:
    adam=keras.optimizers.Adam(lr=1E-3, beta_1=0.9, beta_2=0.999, epsilon=1e-08, decay=1E-3)
    model.compile(loss='binary_crossentropy', optimizer='adam', metrics=['accuracy','binary_accuracy'])
    checkpoint = ModelCheckpoint(model_name, monitor='val_binary_accuracy', verbose=1, save_best_only=False)
    history = model.fit(train_data, train_label,
                        epochs=30, batch_size=2048,
                        validation_data=(valid_data,valid_label),
#                        class_weight={0:0.2,1:0.8}
                        )

    os.mkdir("models/"+model_name)
    model.save('models/'+model_name+'/model.h5')


    ######################################
    #   prediction on train & valid set  #
    ######################################
    pred_data = pd.DataFrame(model.predict(train_data, batch_size=2048), columns=['pred']).set_index(train_event.index)
    pred = pd.concat([pred_data,train_event], axis=1)
    idx = pred.groupby(['event'])['pred'].transform(max) == pred['pred']
    pred = pred[idx]
    train_nevt = len(train_event.drop_duplicates(subset=['event']))
    train_nevt_matchable = len(train_event.loc[train_event['signal']==1].drop_duplicates(subset=['event']))
    train_matched = len(pred.loc[pred['signal']==1])
    train_match_eff = float(train_matched)/train_nevt
    train_recon_eff = float(train_matched)/train_nevt_matchable
    print('\nMatching efficiency on train set = ' + str(train_matched) + ' / ' + str(train_nevt) + ' = ' + str(train_match_eff))
    print('Reconstruction efficiency on train set = ' + str(train_matched) + ' / ' + str(train_nevt_matchable) + ' = ' + str(train_recon_eff))

    pred_val_data = pd.DataFrame(model.predict(valid_data, batch_size=2048), columns=['pred']).set_index(valid_event.index)
    pred_val = pd.concat([pred_val_data,valid_event], axis=1)
    idx = pred_val.groupby(['event'])['pred'].transform(max) == pred_val['pred']
    pred_val = pred_val[idx]
    val_nevt = len(valid_event.drop_duplicates(subset=['event']))
    val_nevt_matchable = len(valid_event.loc[valid_event['signal']==1].drop_duplicates(subset=['event']))
    val_matched = len(pred_val.loc[pred_val['signal']==1])
    val_match_eff = float(val_matched)/val_nevt
    val_recon_eff = float(val_matched)/val_nevt_matchable
    print('\nMatching efficiency on validation set = ' + str(val_matched) + ' / ' + str(val_nevt) + ' = ' + str(val_match_eff))
    print('Reconstruction efficiency on validation set = ' + str(val_matched) + ' / ' + str(val_nevt_matchable) + ' = ' + str(val_recon_eff))

    #save it
    f_ratio = open("models/"+model_name+'/matching_eff.txt','a')
    f_ratio.write("\n"+model_name)
    f_ratio.write('\nMatching efficiency on train set = ' + str(train_matched) + ' / ' + str(train_nevt) + ' = ' + str(train_match_eff))
    f_ratio.write('\nReconstruction efficiency on train set = ' + str(train_matched) + ' / ' + str(train_nevt_matchable) + ' = ' + str(train_recon_eff))
    f_ratio.write('\nMatching efficiency on validation set = ' + str(val_matched) + ' / ' + str(val_nevt) + ' = ' + str(val_match_eff))
    f_ratio.write('\nReconstruction efficiency on validation set = ' + str(val_matched) + ' / ' + str(val_nevt_matchable) + ' = ' + str(val_recon_eff))
    f_ratio.close()

    import matplotlib.pyplot as plt

    ######################################
    #          Plot loss curve           #
    ######################################  
    plt.plot(history.history['loss'])
    plt.plot(history.history['val_loss'])
    plt.title('Binary crossentropy')
    plt.ylabel('Loss')
    plt.xlabel('Epoch')
    plt.legend(['Train','Valid'],loc='upper right')
    plt.savefig(os.path.join("models/"+model_name+'/','fig_score_loss.pdf'))
    plt.gcf().clear()
    print('Loss curve is saved!')

    plt.plot(history.history['acc'])
    plt.plot(history.history['val_acc'])
    plt.title('Binary accuracy')
    plt.ylabel('Accuracy')
    plt.xlabel('Epoch')
    plt.legend(['Train','Valid'],loc='upper right')
    plt.savefig(os.path.join("models/"+model_name+'/','fig_score_acc.pdf'))
    plt.gcf().clear()
    print('Accuracy curve is saved!')

    ######################################
    #           Plot ROC curve           #
    ######################################
    fpr = dict()
    tpr = dict()
    roc_auc = dict()
    fpr[1], tpr[1], thresholds1 = roc_curve(valid_label.tolist(), pred_val_data['pred'].values.tolist(), pos_label=1)#w.r.t sig is truth in val set
    fpr[2], tpr[2], thresholds2 = roc_curve(train_label.tolist(), pred_data['pred'].values.tolist(), pos_label=1)#w.r.t sig is truth in training set, for overtraining check
    plt.plot(tpr[1], 1-fpr[1])#HEP style ROC
    plt.plot(tpr[2], 1-fpr[2])#training ROC
    plt.xlabel('Signal Efficiency')
    plt.ylabel('Background Rejection')
    plt.title('ROC Curve')
    plt.legend(['Valid', 'Train'], loc='lower left')
    plt.savefig(os.path.join("models/"+model_name+'/','fig_score_roc.pdf'))
    plt.gcf().clear()
    print('ROC curve is saved!')
    
    #########################################################
    #Overtraining Check, as well as bkg & sig discrimination#
    #########################################################
    bins = 40
    scores = [tpr[1], fpr[1], tpr[2], fpr[2]]
    low = min(np.min(d) for d in scores)
    high = max(np.max(d) for d in scores)
    low_high = (low,high)
    
    #train is filled
    plt.hist(tpr[2],
        color='b', alpha=0.5, range=low_high, bins=bins,
        histtype='stepfilled', density=True, label='S (train)')
    plt.hist(fpr[2],
        color='r', alpha=0.5, range=low_high, bins=bins,
        histtype='stepfilled', density=True, label='B (train)')
    
    #valid is dotted
    hist, bins = np.histogram(tpr[1], bins=bins, range=low_high, density=True)
    scale = len(tpr[1]) / sum(hist)
    err = np.sqrt(hist * scale) / scale
    width = (bins[1] - bins[0])
    center = (bins[:-1] + bins[1:]) / 2
    plt.errorbar(center, hist, yerr=err, fmt='o', c='b', label='S (valid)')
    hist, bins = np.histogram(fpr[1], bins=bins, range=low_high, density=True)
    scale = len(tpr[1]) / sum(hist)
    err = np.sqrt(hist * scale) / scale
    plt.errorbar(center, hist, yerr=err, fmt='o', c='r', label='B (valid)')
    
    plt.xlabel("Deep Learning Score")
    #plt.ylabel("Arbitrary units")
    plt.ylabel("Number of Combinations")
    plt.legend(loc='best')
    plt.savefig(os.path.join("models/"+model_name+'/','fig_score_overtraining.pdf'))
    plt.gcf().clear()
    print('Overtraining check plot is saved!')

    print "Training complete!"

#################################
#     Prediction on all set     #
#################################
pred_all = pd.DataFrame(model.predict(all_data, batch_size=2048), columns=['pred']).set_index(all_event.index)
pred_all = pd.concat([pred_all,all_event], axis=1)
#pred_all.columns = pred_all.columns.map(str)
#idx = pred_all.groupby(['event'])['0'].transform(max) == pred_all['0']
idx = pred_all.groupby(['event'])['pred'].transform(max) == pred_all['pred']
pred_all = pred_all[idx]
all_nevt = len(all_event.drop_duplicates(subset=['event']))
all_nevt_matchable = len(all_event.loc[all_event['signal']==1].drop_duplicates(subset=['event']))
all_matched = len(pred_all.loc[pred_all['signal']==1])
all_match_eff = float(all_matched)/all_nevt
all_recon_eff = float(all_matched)/all_nevt_matchable
print('\nMatching efficiency on all set = ' + str(all_matched) + ' / ' + str(all_nevt) + ' = ' + str(all_match_eff))
print('Reconstruction efficiency on all set = ' + str(all_matched) + ' / ' + str(all_nevt_matchable) + ' = ' + str(all_recon_eff))

f_ratio = open("models/"+model_name+'/matching_eff.txt','a')
f_ratio.write("\n"+model_name)
f_ratio.write('\nMatching efficiency on all set = ' + str(all_matched) + ' / ' + str(all_nevt) + ' = ' + str(all_match_eff))
f_ratio.write('\nReconstruction efficiency on all set = ' + str(all_matched) + ' / ' + str(all_nevt_matchable) + ' = ' + str(all_recon_eff))
f_ratio.close()
