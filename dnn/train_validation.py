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
#from tqdm import tqdm

import variableAnalyzer as var

def ana(inputDir, process, outputDir, syst='', flag1=False):

    if '__' in process:
        process = process.split('__')[0]
    print("Process: "+process+"\n")
    print("Systematics: "+syst+"\n")

    if 'Filter' in inputDir: process = 'ttbbFilter'+process

    print('Save:'+process+syst+'\n')

    timer = ROOT.TStopwatch()
    timer.Start()

    ver = ""
    configDir = ""
    weightDir = ""
    modelfile = ""

    with open('var.txt', 'r') as f :
        while True :
            line = f.readline()
            if not line : break
            tmp = line.split()
            if 'ver' in tmp : ver = tmp[1]
            if 'configDir' in tmp : configDir = tmp[1]
            if 'weightDir' in tmp : weightDir = tmp[1]
            if 'modelfile' in tmp : modelfile = tmp[1]
    if not os.path.exists(outputDir+'/'+modelfile):
        os.makedirs(outputDir+'/'+modelfile)

    #selEvent = pd.read_hdf(inputDir)  
    df_data = pd.read_hdf(inputDir)
    ttbb = False
    if "ttbb" in inputDir: ttbb = True

    # should get the number automatically 
    countMatchable = True
    if countMatchable :
        df = pd.read_hdf("./rdf_array/array_dnn_TTLJ_PowhegPythia_ttbb.h5")
        df = df.filter(['signal','event','dRbb'], axis=1)
        df = df.query('signal > 0')
        tmpId = df.groupby(['event'])['signal'].transform(max) == df['signal']
        df = df[tmpId]
        df.reset_index(drop=True, inplace=True)
        nMatchable = len(df.index)
        f_tmp = open('matchable.txt','w')
        f_tmp.write(str(nMatchable))
        f_tmp.write(str(df))
        f_tmp.close()

    #Histograms of DNN input variables
#    varlist = ut.getVarlist()
#    varlist = ['dRbb','mbb','pt1','eta1','pt2','eta2']
    varlist = ['dRbb','mbb']
#    xlabel = ut.getHistXlabel()
#    xlabel = dictionary = {'dRbb':'#DeltaR_{b#bar{b}}', 'mbb':'m_{b#bar{b}}','pt1':"pT_{b1}",'eta1':"#eta_{b1}",'pt2':"pT_{b2}",'eta2':"#eta_{b2}"}
    xlabel = dictionary = {'dRbb':'#DeltaR_{b#bar{b}}', 'mbb':'m_{b#bar{b}}'}

    #f_pred = open('pred.txt','w')
    print "\nLoad modelfile : "+str(modelfile)
    model = load_model(configDir+weightDir+ver+'/'+modelfile)
    model.summary()
#    varlist = ut.getVarlist()
#    varlist = ['mbb','dRbb','pt1','eta1','pt2','eta2','bt1','bt2']
    varlist = ['mbb','dRbb','pt1','eta1','pt2','eta2']
    selEvent = df_data
    jetCombi = selEvent.filter(varlist)
    weight = selEvent.filter(['b_weight'])
    scaler = StandardScaler()
    pred = pd.DataFrame([])
    if len(jetCombi) is not 0:
        inputset = np.array(jetCombi)
        inputset_sc = scaler.fit_transform(inputset)
        pred = model.predict(inputset_sc, batch_size = 2000)
  
    pred = pd.DataFrame(pred, columns=['background','signal'])
    #pred = pd.DataFrame(pred, columns=['signal'])
    #f_pred.write('Pred\n'+str(pred)+'\n'+str(type(pred)))
    #f_pred.write('SelEvent\n'+str(selEvent))
    if 'signal' in selEvent: selEvent = selEvent.drop('signal', axis=1)
    #################selEvent = pd.concat([selEvent,pred], axis=1)
    selEvent = pd.concat([selEvent,pred], axis=1)
    #f_pred.write('SelEvent+Pred\n'+str(selEvent))
    idx = selEvent.groupby(['event'])['signal'].transform(max) == selEvent['signal']
    #f_pred.write('\n'+str(idx)+'\n'+str(selEvent[idx])+'\n')
    selEvent = selEvent[idx]
    selEvent.reset_index(drop=True, inplace=True)

    ######################
    #Matching Efficiencies
    ######################
    if ttbb:
        print "it is ttbb"
        nevt = len(df_data.drop_duplicates(subset=['event']))
        df_data = df_data.filter(['event','signal'])
        df_pred = pred.filter(['signal'])
        df_pred.rename(columns = {'signal':'pred'},inplace=True)
        df_pred = pd.concat([df_pred,df_data], axis=1)
        idx = df_pred.groupby(['event'])['pred'].transform(max) == df_pred['pred']
        df_pred = df_pred[idx]
        print("maxscore::: "+str(df_pred))
        num_matched = len(df_pred.loc[df_pred['signal']==1])
        mateff = float(num_matched)/nevt
        recoeff = float(num_matched)/nMatchable
        print('Reconstruction efficiency = ' + str(num_matched) + ' / ' + str(nMatchable) + ' = ' + str(recoeff))
        print('Matching efficiency = ' + str(num_matched) + ' / ' + str(nevt) + ' = ' + str(mateff))
        f_ratio = open('mateff_results.txt','a')
        f_ratio.write("\n"+ver+"\n")
        f_ratio.write(modelfile+"\n")
        f_ratio.write('Reconstruction Efficiency = ' + str(num_matched) + ' / ' + str(nMatchable) + ' = ' + str(recoeff)+'\n')
        f_ratio.write('Matching efficiency = ' + str(num_matched) + ' / ' + str(nevt) + ' = ' + str(mateff)+'\n')
        f_ratio.close()
        sys.exit()
  
    #draw a prediction
    outfile = TFile.Open('rdf_hist/hist_'+process[:-3]+'.root','RECREATE')
    h_mbb = TH1D('h_mbb','',20,0,400)
    h_dRbb = TH1D('h_dRbb','',20,0,4)

    for index, event in selEvent.iterrows():
      mbb = event['mbb']
      dRbb = event['dRbb']
      b_weight = event['b_weight']

      h_mbb.Fill(mbb, b_weight)
      h_dRbb.Fill(dRbb, b_weight)

    outfile.Write()
    outfile.Close()
    print ("Saved")
    keras.backend.clear_session()

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

    parser.add_option("-t", "--test", dest="test",
                      action = 'store_true',
                      default=False,
                      help='Test single root file')

    parser.add_option("-s", "--syst", dest="syst",
                      action = 'store_true',
                      default=False,
                      help='all root fiels in input directory')

    (options,args) = parser.parse_args()
   
 
    start_time = time.time()

    ntupleDir = '/data/users/seohyun/ntuple/Run2018/V10_2/split/'
    arrayDir = './rdf_array/'
    histDir = './hist/'

    systlist = [
        '__puup', '__pudown',
        '__musfup', '__musfdown', '__mutrgup', '__mutrgdown',
        '__elsfup', '__elsfdown', '__eltrgup', '__eltrgdown',
        '__lfup', '__lfdown', '__hfup', '__hfdown',
        '__hfstat1up', '__hfstat1down', '__hfstat2up', '__hfstat2down',
        '__lfstat1up', '__lfstat1down', '__lfstat2up', '__lfstat2down',
        '__cferr1up', '__cferr1down', '__cferr2up', '__cferr2down',
        '__scale0', '__scale1', '__scale2', '__scale3', '__scale4', '__scale5',
        '__pdfup', '__pdfdown'
    ]

    if options.test:
        process = 'TTLJ_PowhegPythia_ttbb'
        ana(arrayDir+process, process, histDir, flag1=True)

    else:
        for process in os.listdir(arrayDir):
            if "split" in process: continue
            if "2018" in process: continue
            if "2017" in process: continue
            if "2016" in process: continue
            #if not "ttbbFilter" in process: continue
            #if process.endswith(".h5"): continue

            
            if not ('SYS' in process or '__' in process or 'Herwig' in process or 'Evtgen' in process or 'TT_aMCatNLO' in process):
                print("Start "+str(process))

                ana(arrayDir+process, process, histDir)

                if options.syst and not 'Data' in process:
                    for syst in systlist:
                        if 'scale' in syst and not 'tt' in process: continue
                        ana(arrayDir+process, process, histDir, syst)
            else:
                if   'FSRup'   in process: syst = '__fsrup'
                elif 'FSRdown' in process: syst = '__fsrdown'
                elif 'ISRup'   in process: syst = '__isrup'
                elif 'ISRdown' in process: syst = '__isrdown'
                elif 'hdampUp' in process: syst = '__hdampup'
                elif 'hdampDown' in process: syst = '__hdampdown'
                elif 'TuneUp'  in process: syst = '__tuneup'
                elif 'TuneDown'in process: syst = '__tunedown'
                elif 'jecdown' in process: syst = '__jecdown'
                elif 'jecup'   in process: syst = '__jecup'
                elif 'jerdown' in process: syst = '__jerdown'
                elif 'jerup'   in process: syst = '__jerup'
                elif 'Herwig'  in process: syst = '__powhegherwig'
                elif 'TT_aMCatNLO'in process: syst = '__amcatnlopythia'
                elif 'Evtgen'  in process: syst = '__powhegpythiaevtgen'
                else:                      syst = ''
                ana(arrayDir+process, process, histDir, syst)

    print("Total running time :%s " %(time.time() - start_time))
