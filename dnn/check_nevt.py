import pandas as pd
import deepdish.io as io 

df = pd.read_hdf("array/split/array_dnn_TTToSemiLeptonic_ttbb.h5")

df = df.sort_values(by=['event']).reset_index()

all_combi = df.filter(['event','signal'], axis=1)
print("all combination = "+str(len(all_combi)))

groupped_event = all_combi.drop_duplicates(subset=['event'])
#groupped_event = all_combi.drop_duplicates('event')
nevt = len(groupped_event)
print("number of total event = "+str(nevt))

#split_nevt = groupped_event[:int(nevt*0.9)].iloc[-1]
#print split_nevt
#
#n = -1
#split_point = -1
#bef = 0
#for idx, row in all_combi.iterrows():
#  if (n <= row['event']): n = row['event']
#  else: 
#    print
#    print n, idx, "\n", row
#    print bef
#    print
#  bef = (idx, row)
##  if (row['event'] == split_nevt['event']):
##    if split_point < 0: split_point = idx
#train_event = all_combi[:split_point]
#valid_event = all_combi[split_point:]
#split_point = 299876


split_nevt = groupped_event[:int(nevt*0.9)].iloc[-1]
split_point = -1
for idx, row in all_combi.iterrows():
  if (row['event'] == split_nevt['event']):
    if split_point < 0: split_point = idx

train_event = all_combi[:split_point]
valid_event = all_combi[split_point:]

print train_event[len(train_event)-10:]
print valid_event[:10]

print("train combination ? : "+str(len(train_event)))
print("valid combination ? : "+str(len(valid_event)))

len_train_nevt =  len(train_event.drop_duplicates(subset=['event']))
print ("number of train event = "+str(len_train_nevt))

len_val_nevt =  len(valid_event.drop_duplicates(subset=['event']))
print ("number of valid event = "+str(len_val_nevt))

train_matchable_idx = train_event.groupby(['event'])['signal'].transform(sum) >= 1
train_matchable = train_event[train_matchable_idx]
len_train_matchable = len(train_matchable.drop_duplicates(subset=['event']))
print ("number of matchable event = "+str(len_train_matchable))

train_nomatchable_idx = train_event.groupby(['event'])['signal'].transform(max) == 0
train_nomatchable = train_event[train_nomatchable_idx]
len_train_nomatchable = len(train_nomatchable.drop_duplicates(subset=['event']))
print ("number of event with no signal= "+str(len_train_nomatchable))

train_remove0 = pd.concat([train_event,train_nomatchable]).drop_duplicates(keep=False).reset_index()
print ("number of train event with signal ="+str(len(train_remove0)))
