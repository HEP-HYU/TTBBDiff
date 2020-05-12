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

import ROOT
from sys import exit
import numpy as np

import csv
from sklearn.utils import shuffle
import os

from sklearn.preprocessing import StandardScaler, label_binarize

trainInput = "rdf_array/array_dnn_TTLJ_PowhegPythia_ttbb.h5"
name_inputvar = ['mbb','dRbb','pt1','pt2','eta1','eta2']

model_name = '2018_nobsel_epoch250'

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

split_nevt = groupped_event[:int(nevt*0.8)].iloc[-1]
split_point = -1
for idx, row in all_event.iterrows():
  if (row['event'] == split_nevt['event']):
    if split_point < 0: split_point = idx
#need to check
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
a = 300
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
                        epochs=250, batch_size=1024,
                        validation_data=(valid_data,valid_label),
                        )

    os.mkdir("models/"+model_name)
    model.save('models/'+model_name+'/model.h5')


    import matplotlib.pyplot as plt
    
    plt.plot(history.history['loss'])
    plt.plot(history.history['val_loss'])
    plt.title('Binary crossentropy')
    plt.ylabel('Loss')
    plt.xlabel('Epoch')
    plt.legend(['Train','Validation'],loc='upper right')
    plt.savefig(os.path.join("models/"+model_name+'/','fig_score_loss.pdf'))
    plt.gcf().clear()

    print "Training complete!"

###################################
#   prediction on validation set  #
###################################
pred_val = pd.DataFrame(model.predict(valid_data, batch_size=1024)).set_index(valid_event.index)
pred_val = pd.concat([pred_val,valid_event], axis=1)
pred_val.columns = pred_val.columns.map(str)
idx = pred_val.groupby(['event'])['0'].transform(max) == pred_val['0']
pred_val = pred_val[idx]
val_nevt = len(valid_event.drop_duplicates(subset=['event']))
val_nevt_matchable = len(valid_event.loc[valid_event['signal']==1].drop_duplicates(subset=['event']))
val_matched = len(pred_val.loc[pred_val['signal']==1])
val_match_eff = float(val_matched)/val_nevt
val_recon_eff = float(val_matched)/val_nevt_matchable
print('\nMatching efficiency on validation set = ' + str(val_matched) + ' / ' + str(val_nevt) + ' = ' + str(val_match_eff))
print('Reconstruction efficiency on validation set = ' + str(val_matched) + ' / ' + str(val_nevt_matchable) + ' = ' + str(val_recon_eff))

###what is going on with all event
pred_all = pd.DataFrame(model.predict(all_data, batch_size=1024)).set_index(all_event.index)
pred_all = pd.concat([pred_all,all_event], axis=1)
pred_all.columns = pred_all.columns.map(str)
idx = pred_all.groupby(['event'])['0'].transform(max) == pred_all['0']
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
f_ratio.write('\nMatching efficiency on validation set = ' + str(val_matched) + ' / ' + str(val_nevt) + ' = ' + str(val_match_eff))
f_ratio.write('\nReconstruction efficiency on validation set = ' + str(val_matched) + ' / ' + str(val_nevt_matchable) + ' = ' + str(val_recon_eff))
f_ratio.write('\nMatching efficiency on all set = ' + str(all_matched) + ' / ' + str(all_nevt) + ' = ' + str(all_match_eff))
f_ratio.write('\nReconstruction efficiency on all set = ' + str(all_matched) + ' / ' + str(all_nevt_matchable) + ' = ' + str(all_recon_eff))
f_ratio.close()
