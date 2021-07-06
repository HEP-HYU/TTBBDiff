#! /usr/bin/env python
import os
import sys
import google.protobuf

os.environ["CUDA_VISIBLE_DEVICES"] = "3"

import pandas as pd
import csv
import math
import numpy as np
from array import array
from sklearn.preprocessing import StandardScaler, label_binarize
from sklearn.utils import shuffle
from operator import itemgetter
from itertools import groupby

import ROOT
from ROOT import *

import tensorflow as tf
import keras
from keras.utils import np_utils, multi_gpu_model
from keras.models import Model, Sequential, load_model
from keras.layers import Input, Dense, Activation, Dropout, add
from keras.layers.normalization import BatchNormalization
from keras.regularizers import l2
from keras.optimizers import Adam, SGD
from keras.callbacks import Callback, ModelCheckpoint

import utils as ut
import multiprocessing as mp
import time

model_name = '2018_nobsel_epoch250'
arrayDir = './rdf_array/'
histDir = 'models/'+model_name+'/hist'

def ana(inputDir, process, histDir):

    timer = ROOT.TStopwatch()
    timer.Start()

    df_data = pd.read_hdf(inputDir)
    ttbb = False
    if "ttbb" in inputDir: ttbb = True

    name_inputvar = ['mbb','dRbb','pt1','pt2','eta1','eta2']
    
    model = load_model("models/"+model_name+'/model.h5')
    model.summary()
    
    if ttbb:
        input_event = df_data.filter(['event','signal'], axis=1)
        info = df_data.filter(['b_weight','addbjet1_pt','addbjet1_eta','addbjet1_phi','addbjet1_e','addbjet2_pt','addbjet2_eta','addbjet2_phi','addbjet2_e'])
    
    else:
        input_event = df_data.filter(['event'], axis=1)
        info = df_data.filter(['b_weight'])
    
    input_data = df_data.filter(name_inputvar)
    pred = pd.DataFrame([], columns=['pred'])
    scaler = StandardScaler()
    if len(input_data) is not 0:
        input_sc = scaler.fit_transform(input_data)
        #prediction
        pred = pd.DataFrame(model.predict(input_sc, batch_size = 1024), columns=['pred'])
    selEvent = pd.concat([info, input_data, input_event, pred], axis=1)
    idx = selEvent.groupby(['event'])['pred'].transform(max) == selEvent['pred']
    selEvent = selEvent[idx]
    selEvent.reset_index(drop=True, inplace=True)
    
    if ttbb and not "Ch" in process:
        nevt = len(input_event.drop_duplicates(subset=['event']))
        nevt_matchable = len(input_event.loc[input_event['signal']==1].drop_duplicates(subset=['event']))
        matched = len(selEvent.loc[selEvent['signal']==1])
        match_eff = float(matched)/nevt
        recon_eff = float(matched)/nevt_matchable
        print('\nMatching efficiency = ' + str(matched) + ' / ' + str(nevt) + ' = ' + str(match_eff))
        print('Reconstruction efficiency = ' + str(matched) + ' / ' + str(nevt_matchable) + ' = ' + str(recon_eff))
        f_ratio = open("models/"+model_name+'/matching_eff.txt','a')
        f_ratio.write("\n with "+model_name)
        f_ratio.write('\nMatching efficiency on all set = ' + str(matched) + ' / ' + str(nevt) + ' = ' + str(match_eff))
        f_ratio.write('\nReconstruction efficiency on all set = ' + str(matched) + ' / ' + str(nevt_matchable) + ' = ' + str(recon_eff))
        f_ratio.close()
        
    outfile = TFile.Open('models/'+model_name+'/hist_'+process[:-3]+'.root','RECREATE')
    h_mbb = TH1D('h_mbb','',20,0,400)
    h_dRbb = TH1D('h_dRbb','',20,0,4)
    h_gen_mbb = TH1D('h_gen_mbb','',20,0,400)
    h_gen_dRbb = TH1D('h_gen_dRbb','',20,0,4)
    h_responseMatrix_mbb = TH2D('h_responseMatrix_mbb','',20,0,400,20,0,400)
    h_responseMatrix_dRbb = TH2D('h_responseMatrix_dRbb','',20,0,4,20,0,4)
    
    for index, event in selEvent.iterrows():
    
      mbb = event['mbb']
      dRbb = event['dRbb']
      b_weight = event['b_weight']
    
      h_mbb.Fill(mbb, b_weight)
      h_dRbb.Fill(dRbb, b_weight)
    
      if ttbb:
        gen_addbjet1 = TLorentzVector()
        gen_addbjet2 = TLorentzVector()
        gen_addbjet1.SetPtEtaPhiE(event['addbjet1_pt'],event['addbjet1_eta'],event['addbjet1_phi'],event['addbjet1_e'])
        gen_addbjet2.SetPtEtaPhiE(event['addbjet2_pt'],event['addbjet2_eta'],event['addbjet2_phi'],event['addbjet2_e'])
    
        gen_dRbb = gen_addbjet1.DeltaR(gen_addbjet2)
        gen_mbb = (gen_addbjet1+gen_addbjet2).M()
    
        h_gen_mbb.Fill(gen_mbb, b_weight)
        h_gen_dRbb.Fill(gen_dRbb, b_weight)
        h_responseMatrix_mbb.Fill(mbb, gen_mbb, b_weight)
        h_responseMatrix_dRbb.Fill(dRbb, gen_dRbb, b_weight)
    
    outfile.Write()
    outfile.Close()
    print ("Saved")

    timer.Stop()
    realtime = timer.RealTime()
    cputime = timer.CpuTime()
    print("Real Time : {0:6.2f} seconds, CPU Time : {1:6.2f} seconds").format(realtime,cputime)

#Options
if __name__ == '__main__':
    from optparse import OptionParser 

    parser = OptionParser()
    parser.usage = """
    %make histogram from array
    """
    start_time = time.time()

    for process in os.listdir(arrayDir):
        if not ".h5" in process: continue
#        if not "ttbb" in process: continue

        print("Start "+str(process))
        ana(arrayDir+process, process, histDir)

    print("Total running time :%s " %(time.time() - start_time))
