#! /usr/bin/env python
import os
import sys
#import google.protobuf

os.environ["CUDA_VISIBLE_DEVICES"] = "0"

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

model_name = 'nanoRun2_statement30'
#model_name = 'nano2017_statement_20_PSonly'
#model_name = 'nano2017_statement_30'
#model_name = 'nano2016_statement_20'
#model_name = 'nano2018_statement_50'

arrayDir = './array/split/'

histDir = 'models/'+model_name+'/hist'

def ana(inputDir, process, histDir):

    timer = ROOT.TStopwatch()
    timer.Start()

    df_data = pd.read_hdf(inputDir)
    ttbb = False
    if "ttbb" in inputDir: ttbb = True
    name_inputvar = ['mbb','dRbb','dPhibb','dEtabb','Etabb','Phibb','mlbb','dRlbb','mlb1','dRlb1','mlb2','dRlb2','pt1','pt2','eta1','eta2','d1','d2','e1','e2','m1','m2','nbjets','lepton_pt','lepton_eta','lepton_m','btag3rd','btag4th']
    #name_inputvar = ['mbb','dRbb','dPhibb','dEtabb','Etabb','Phibb','mlbb','dRlbb','mlb1','dRlb1','mlb2','dRlb2','mnubb','dRnubb','mnub1','dRnub1','mnub2','dRnub2','dRlnubb','pt1','pt2','eta1','eta2','d1','d2','e1','e2','m1','m2','nbjets','lepton_pt','lepton_eta','lepton_e','MET','MET_phi']
    #name_inputvar_add = ['mbb','dRbb','dPhibb','dEtabb','Etabb','Phibb','mlbb','dRlbb','mlb1','dRlb1','mlb2','dRlb2','mnubb','dRnubb','mnub1','dRnub1','mnub2','dRnub2','dRlnubb','pt1','pt2','eta1','eta2','d1','d2','e1','e2','m1','m2','nbjets','lepton_pt','lepton_eta','lepton_e','MET','MET_phi','btag3rd','btag4th']

    model = load_model("models/"+model_name+'/model.h5')
    model.summary()
    
    #if ttbb and not "Ch" in process:
    if ttbb:
        input_event = df_data.filter(['event','signal'], axis=1)
        info = df_data.filter(['b_weight','addbjet1_pt','addbjet1_eta','addbjet1_phi','addbjet1_mass','addbjet2_pt','addbjet2_eta','addbjet2_phi','addbjet2_mass'])
        true_event = df_data.loc[df_data['signal']==1].drop_duplicates(subset=['event'])
        true_event.reset_index(drop=True, inplace=True)
    
    else:
        input_event = df_data.filter(['event'], axis=1)
        info = df_data.filter(['b_weight'])
    
    input_data = df_data.filter(name_inputvar)
    #input_data_add = df_data.filter(name_inputvar_add)
    pred = pd.DataFrame([], columns=['pred'])
    scaler = StandardScaler()
    if len(input_data) is not 0:
        input_sc = scaler.fit_transform(input_data)
        #prediction
        pred = pd.DataFrame(model.predict(input_sc, batch_size = 2048), columns=['pred'])
    selEvent = pd.concat([info, input_data, input_event, pred], axis=1)
    #selEvent = pd.concat([info, input_data_add, input_event, pred], axis=1)
    idx = selEvent.groupby(['event'])['pred'].transform(max) == selEvent['pred']
    selEvent = selEvent[idx]
    selEvent.reset_index(drop=True, inplace=True)
    
    if ttbb and not "Ch" in process:
        nevt = len(input_event.drop_duplicates(subset=['event']))
        nevt_matchable = len(input_event.loc[input_event['signal']==1].drop_duplicates(subset=['event']))
        matched = len(selEvent.loc[selEvent['signal']==1])
        match_eff = float(matched)/nevt
        recon_eff = float(matched)/nevt_matchable
        print('\nMatching efficiency = ' + str(matched) + ' / ' + str(nevt) + ' = ' + str(round(match_eff,3)))
        print('Reconstruction efficiency = ' + str(matched) + ' / ' + str(nevt_matchable) + ' = ' + str(round(recon_eff,3)))
        f_ratio = open("models/"+model_name+'/matching_eff.txt','a')
        f_ratio.write("\n with "+model_name)
        f_ratio.write('\nMatching efficiency on all set = ' + str(matched) + ' / ' + str(nevt) + ' = ' + str(match_eff))
        f_ratio.write('\nReconstruction efficiency on all set = ' + str(matched) + ' / ' + str(nevt_matchable) + ' = ' + str(recon_eff))
        f_ratio.close()
        
    outfile = TFile.Open('models/'+model_name+'/hist_'+process[:-3]+'.root','RECREATE')
    h_mbb = TH1D('h_mbb','',4,array('d',[0.0,60.0,100.0,170.0,400.0]))
    h_mbb.GetXaxis().SetTitle("Reco. m_{b#bar{b}}(GeV)")
    h_mbb.GetYaxis().SetTitle("Entries")
    h_mbb.Sumw2()

    h_dRbb = TH1D('h_dRbb','',4,array('d',[0.4,0.6,1.0,2.0,4.0]))
    h_dRbb.GetXaxis().SetTitle("Reco. #DeltaR_{b#bar{b}}")
    h_dRbb.GetYaxis().SetTitle("Entries")
    h_dRbb.Sumw2()

    h_gen_mbb = TH1D('h_gen_mbb','',4,array('d',[0.0,60.0,100.0,170.0,400.0]))
    h_gen_mbb.GetXaxis().SetTitle("Gen. m_{b#bar{b}}(GeV)")
    h_gen_mbb.GetYaxis().SetTitle("Entries")
    h_gen_mbb.Sumw2()

    h_gen_dRbb = TH1D('h_gen_dRbb','',4,array('d',[0.4,0.6,1.0,2.0,4.0]))
    h_gen_dRbb.GetXaxis().SetTitle("Gen. #DeltaR_{b#bar{b}}")
    h_gen_dRbb.GetYaxis().SetTitle("Entries")
    h_gen_dRbb.Sumw2()

    h_mbb_csv = TH2D('h_mbb_csv','',4,array('d',[0.0,60.0,100.0,170.0,400.0]),5,array('d',[0.0,0.2,0.4,0.6,0.8,1.0]))
    h_mbb_csv.GetXaxis().SetTitle("Reco. m_{b#bar{b}}(GeV)")
    h_mbb_csv.GetYaxis().SetTitle("b discriminator(DeepJet)")
    h_mbb_csv.Sumw2()

    h_dRbb_csv = TH2D('h_dRbb_csv','',4,array('d',[0.4,0.6,1.0,2.0,4.0]),5,array('d',[0.0,0.2,0.4,0.6,0.8,1.0]))
    h_dRbb_csv.GetXaxis().SetTitle("Reco. #DeltaR_{b#bar{b}}")
    h_dRbb_csv.GetYaxis().SetTitle("b discriminator(DeepJet)")
    h_dRbb_csv.Sumw2()

    h_mbb_csv3 = TH2D('h_mbb_csv3','',4,array('d',[0.0,60.0,100.0,170.0,400.0]),5,array('d',[0.0,0.2,0.4,0.6,0.8,1.0]))
    h_mbb_csv3.GetXaxis().SetTitle("Reco. m_{b#bar{b}}(GeV)")
    h_mbb_csv3.GetYaxis().SetTitle("3rd b discriminator(DeepJet)")
    h_mbb_csv3.Sumw2()

    h_dRbb_csv3 = TH2D('h_dRbb_csv3','',4,array('d',[0.4,0.6,1.0,2.0,4.0]),5,array('d',[0.0,0.2,0.4,0.6,0.8,1.0]))
    h_dRbb_csv3.GetXaxis().SetTitle("Reco. #DeltaR_{b#bar{b}}")
    h_dRbb_csv3.GetYaxis().SetTitle("3rd b discriminator(DeepJet)")
    h_dRbb_csv3.Sumw2()

    h_mbb_csv4 = TH2D('h_mbb_csv4','',4,array('d',[0.0,60.0,100.0,170.0,400.0]),5,array('d',[0.0,0.2,0.4,0.6,0.8,1.0]))
    h_mbb_csv4.GetXaxis().SetTitle("Reco. m_{b#bar{b}}(GeV)")
    h_mbb_csv4.GetYaxis().SetTitle("4th b discriminator(DeepJet)")
    h_mbb_csv4.Sumw2()

    h_dRbb_csv4 = TH2D('h_dRbb_csv4','',4,array('d',[0.4,0.6,1.0,2.0,4.0]),5,array('d',[0.0,0.2,0.4,0.6,0.8,1.0]))
    h_dRbb_csv4.GetXaxis().SetTitle("Reco. #DeltaR_{b#bar{b}}")
    h_dRbb_csv4.GetYaxis().SetTitle("4th b discriminator(DeepJet)")
    h_dRbb_csv4.Sumw2()

    h_mbb_nbjets = TH2D('h_mbb_nbjets','',4,array('d',[0.0,60.0,100.0,170.0,400.0]),4,array('d',[4.0,5.0,6.0,7.0,8.0]))
    h_mbb_nbjets.GetXaxis().SetTitle("Reco. m_{b#bar{b}}(GeV)")
    h_mbb_nbjets.GetYaxis().SetTitle("number of b-tagged Jets")
    h_mbb_nbjets.Sumw2()

    h_dRbb_nbjets = TH2D('h_dRbb_nbjets','',4,array('d',[0.4,0.6,1.0,2.0,4.0]),4,array('d',[4.0,5.0,6.0,7.0,8.0]))
    h_dRbb_nbjets.GetXaxis().SetTitle("Reco. #DeltaR_{b#bar{b}}")
    h_dRbb_nbjets.GetYaxis().SetTitle("number of b-tagged Jets")
    h_dRbb_nbjets.Sumw2()

    h_responseMatrix_mbb = TH2D('h_responseMatrix_mbb','',4,array('d',[0.0,60.0,100.0,170.0,400.0]),4,array('d',[0.0,60.0,100.0,170.0,400.0]))
    h_responseMatrix_mbb.GetXaxis().SetTitle("Reco. m_{b#bar{b}}(GeV)")
    h_responseMatrix_mbb.GetYaxis().SetTitle("Gen. m_{b#bar{b}}(GeV)")
    h_responseMatrix_mbb.Sumw2()

    h_responseMatrix_dRbb = TH2D('h_responseMatrix_dRbb','',4,array('d',[0.4,0.6,1.0,2.0,4.0]),4,array('d',[0.4,0.6,1.0,2.0,4.0]))
    h_responseMatrix_dRbb.GetXaxis().SetTitle("Reco. #DeltaR_{b#bar{b}}")
    h_responseMatrix_dRbb.GetYaxis().SetTitle("Gen. #DeltaR_{b#bar{b}}")
    h_responseMatrix_dRbb.Sumw2()

    h_resp_dRbb_csv = TH3D('h_responseMatrix_dRbb_csv','',4,array('d',[0.4,0.6,1.0,2.0,4.0]),4,array('d',[0.4,0.6,1.0,2.0,4.0]),5,array('d',[0.0,0.2,0.4,0.6,0.8,1.0]))
    h_resp_dRbb_csv.GetXaxis().SetTitle("Reco. #DeltaR_{b#bar{b}}")
    h_resp_dRbb_csv.GetYaxis().SetTitle("Gen. #DeltaR_{b#bar{b}}")
    h_resp_dRbb_csv.GetZaxis().SetTitle("b discriminator(DeepJet)")
    h_resp_dRbb_csv.Sumw2()

    h_resp_mbb_csv = TH3D('h_responseMatrix_mbb_csv','',4,array('d',[0.0,60.0,100.0,170.0,400.0]),4,array('d',[0.0,60.0,100.0,170.0,400.0]),5,array('d',[0.0,0.2,0.4,0.6,0.8,1.0]))
    h_resp_mbb_csv.GetXaxis().SetTitle("Reco. m_{b#bar{b}}")
    h_resp_mbb_csv.GetYaxis().SetTitle("Gen. m_{b#bar{b}}")
    h_resp_mbb_csv.GetZaxis().SetTitle("b discriminator(DeepJet)")
    h_resp_mbb_csv.Sumw2()

    h_resp_dRbb_csv3 = TH3D('h_responseMatrix_dRbb_csv3','',4,array('d',[0.4,0.6,1.0,2.0,4.0]),4,array('d',[0.4,0.6,1.0,2.0,4.0]),5,array('d',[0.0,0.2,0.4,0.6,0.8,1.0]))
    h_resp_dRbb_csv3.GetXaxis().SetTitle("Reco. #DeltaR_{b#bar{b}}")
    h_resp_dRbb_csv3.GetYaxis().SetTitle("Gen. #DeltaR_{b#bar{b}}")
    h_resp_dRbb_csv3.GetZaxis().SetTitle("3rd b discriminator(DeepJet)")
    h_resp_dRbb_csv3.Sumw2()

    h_resp_mbb_csv3 = TH3D('h_responseMatrix_mbb_csv3','',4,array('d',[0.0,60.0,100.0,170.0,400.0]),4,array('d',[0.0,60.0,100.0,170.0,400.0]),5,array('d',[0.0,0.2,0.4,0.6,0.8,1.0]))
    h_resp_mbb_csv3.GetXaxis().SetTitle("Reco. m_{b#bar{b}}")
    h_resp_mbb_csv3.GetYaxis().SetTitle("Gen. m_{b#bar{b}}")
    h_resp_mbb_csv3.GetZaxis().SetTitle("3rd b discriminator(DeepJet)")
    h_resp_mbb_csv3.Sumw2()

    h_resp_dRbb_csv4 = TH3D('h_responseMatrix_dRbb_csv4','',4,array('d',[0.4,0.6,1.0,2.0,4.0]),4,array('d',[0.4,0.6,1.0,2.0,4.0]),5,array('d',[0.0,0.2,0.4,0.6,0.8,1.0]))
    h_resp_dRbb_csv4.GetXaxis().SetTitle("Reco. #DeltaR_{b#bar{b}}")
    h_resp_dRbb_csv4.GetYaxis().SetTitle("Gen. #DeltaR_{b#bar{b}}")
    h_resp_dRbb_csv4.GetZaxis().SetTitle("4th b discriminator(DeepJet)")
    h_resp_dRbb_csv4.Sumw2()

    h_resp_mbb_csv4 = TH3D('h_responseMatrix_mbb_csv4','',4,array('d',[0.0,60.0,100.0,170.0,400.0]),4,array('d',[0.0,60.0,100.0,170.0,400.0]),5,array('d',[0.0,0.2,0.4,0.6,0.8,1.0]))
    h_resp_mbb_csv4.GetXaxis().SetTitle("Reco. m_{b#bar{b}}")
    h_resp_mbb_csv4.GetYaxis().SetTitle("Gen. m_{b#bar{b}}")
    h_resp_mbb_csv4.GetZaxis().SetTitle("4th b discriminator(DeepJet)")
    h_resp_mbb_csv4.Sumw2()

    h_resp_dRbb_nbjets = TH3D('h_responseMatrix_dRbb_nbjets','',4,array('d',[0.4,0.6,1.0,2.0,4.0]),4,array('d',[0.4,0.6,1.0,2.0,4.0]),4,array('d',[4.0,5.0,6.0,7.0,8.0]))
    h_resp_dRbb_nbjets.GetXaxis().SetTitle("Reco. #DeltaR_{b#bar{b}}")
    h_resp_dRbb_nbjets.GetYaxis().SetTitle("Gen. #DeltaR_{b#bar{b}}")
    h_resp_dRbb_nbjets.GetZaxis().SetTitle("number of b-tagged Jets")
    h_resp_dRbb_nbjets.Sumw2()

    h_resp_mbb_nbjets = TH3D('h_responseMatrix_mbb_nbjets','',4,array('d',[0.0,60.0,100.0,170.0,400.0]),4,array('d',[0.0,60.0,100.0,170.0,400.0]),4,array('d',[4.0,5.0,6.0,7.0,8.0]))
    h_resp_mbb_nbjets.GetXaxis().SetTitle("Reco. m_{b#bar{b}}")
    h_resp_mbb_nbjets.GetYaxis().SetTitle("Gen. m_{b#bar{b}}")
    h_resp_mbb_nbjets.GetZaxis().SetTitle("number of b-tagged Jets")
    h_resp_mbb_nbjets.Sumw2()
    
    for index, event in selEvent.iterrows():
#    for index, event in true_event.iterrows():
    
      mbb = event['mbb']
      dRbb = event['dRbb']
      b_weight = event['b_weight']
      
      csv = event['d1']
      csv3 = event['btag3rd']
      csv4 = event['btag4th']
      nbjets = event['nbjets']
    
      h_mbb.Fill(mbb, b_weight)
      h_dRbb.Fill(dRbb, b_weight)

      h_mbb_csv.Fill(mbb, csv, b_weight)
      h_dRbb_csv.Fill(dRbb, csv, b_weight)

      h_mbb_csv3.Fill(mbb, csv3, b_weight)
      h_dRbb_csv3.Fill(dRbb, csv3, b_weight)
      h_mbb_csv4.Fill(mbb, csv4, b_weight)
      h_dRbb_csv4.Fill(dRbb, csv4, b_weight)

      h_mbb_nbjets.Fill(mbb, nbjets, b_weight)
      h_dRbb_nbjets.Fill(dRbb, nbjets, b_weight)
    
      #if ttbb and not "Ch" in process:
      if ttbb:
        gen_addbjet1 = TLorentzVector()
        gen_addbjet2 = TLorentzVector()
        gen_addbjet1.SetPtEtaPhiE(event['addbjet1_pt'],event['addbjet1_eta'],event['addbjet1_phi'],event['addbjet1_mass'])
        gen_addbjet2.SetPtEtaPhiE(event['addbjet2_pt'],event['addbjet2_eta'],event['addbjet2_phi'],event['addbjet2_mass'])
    
        gen_dRbb = gen_addbjet1.DeltaR(gen_addbjet2)
        gen_mbb = (gen_addbjet1+gen_addbjet2).M()
    
        h_gen_mbb.Fill(gen_mbb, b_weight)
        h_gen_dRbb.Fill(gen_dRbb, b_weight)
        h_responseMatrix_mbb.Fill(mbb, gen_mbb, b_weight)
        h_responseMatrix_dRbb.Fill(dRbb, gen_dRbb, b_weight)
        h_resp_dRbb_csv.Fill(dRbb, gen_dRbb, csv, b_weight)
        h_resp_mbb_csv.Fill(mbb, gen_mbb, csv, b_weight)

        h_resp_dRbb_csv3.Fill(dRbb, gen_dRbb, csv3, b_weight)
        h_resp_mbb_csv3.Fill(mbb, gen_mbb, csv3, b_weight)
        h_resp_dRbb_csv4.Fill(dRbb, gen_dRbb, csv4, b_weight)
        h_resp_mbb_csv4.Fill(mbb, gen_mbb, csv4, b_weight)
        h_resp_dRbb_nbjets.Fill(dRbb, gen_dRbb, nbjets, b_weight)
        h_resp_mbb_nbjets.Fill(mbb, gen_mbb, nbjets, b_weight)
    
    if ttbb and not "Ch" in process:
    #if ttbb in process:

      h_purity_dRbb = h_responseMatrix_dRbb.ProjectionX()
      h_purity_dRbb.SetName("h_purity_dRbb")
      h_purity_dRbb.GetXaxis().SetTitle("Reco. #DeltaR_{b#bar{b}}")
      h_purity_dRbb.GetYaxis().SetTitle("Purity (%)")
  
      h_stability_dRbb = h_responseMatrix_dRbb.ProjectionY()
      h_stability_dRbb.SetName("h_stability_dRbb")
      h_stability_dRbb.GetXaxis().SetTitle("Gen. #DeltaR_{b#bar{b}}")
      h_stability_dRbb.GetYaxis().SetTitle("Stability (%)")
  
      h_purity_mbb = h_responseMatrix_mbb.ProjectionX()
      h_purity_mbb.SetName("h_purity_mbb")
      h_purity_mbb.GetXaxis().SetTitle("Reco. m_{b#bar{b}}(GeV)")
      h_purity_mbb.GetYaxis().SetTitle("Purity (%)")
  
      h_stability_mbb = h_responseMatrix_mbb.ProjectionY()
      h_stability_mbb.SetName("h_stability_mbb")
      h_stability_mbb.GetXaxis().SetTitle("Gen. m_{b#bar{b}}(GeV)")
      h_stability_mbb.GetYaxis().SetTitle("Stability (%)")

      for i in range( h_purity_dRbb.GetNbinsX() ):
        den = h_purity_dRbb.GetBinContent(i+1)
        num = h_responseMatrix_dRbb.GetBinContent(i+1, i+1)
        if num*den > 0:
            purity = num/den*100
            h_purity_dRbb.SetBinContent(i+1, purity)
            h_purity_dRbb.SetBinError(i+1, abs(purity)*math.sqrt(pow(math.sqrt(den)/den,2)+pow(math.sqrt(num)/num,2)))
  
      for i in range( h_stability_dRbb.GetNbinsX() ):
        den = h_stability_dRbb.GetBinContent(i+1)
        num = h_responseMatrix_dRbb.GetBinContent(i+1, i+1)
        if num*den > 0:
          stability = float(num/den*100)
          h_stability_dRbb.SetBinContent(i+1, stability)
          h_stability_dRbb.SetBinError(i+1, abs(stability)*math.sqrt(pow(math.sqrt(den)/den,2)+pow(math.sqrt(num)/num,2)))
  
      for i in range( h_purity_mbb.GetNbinsX() ):
        den = h_purity_mbb.GetBinContent(i+1)
        num = h_responseMatrix_mbb.GetBinContent(i+1, i+1)
        if num*den > 0:
          purity = num/den*100
          h_purity_mbb.SetBinContent(i+1, purity)
          h_purity_mbb.SetBinError(i+1, abs(purity)*math.sqrt(pow(math.sqrt(den)/den,2)+pow(math.sqrt(num)/num,2)))
  
      for i in range( h_stability_mbb.GetNbinsX() ):
        den = h_stability_mbb.GetBinContent(i+1)
        num = h_responseMatrix_mbb.GetBinContent(i+1, i+1)
        if num*den > 0:
          stability = float(num/den*100)
          h_stability_mbb.SetBinContent(i+1, stability)
          h_stability_mbb.SetBinError(i+1, abs(stability)*math.sqrt(pow(math.sqrt(den)/den,2)+pow(math.sqrt(num)/num,2)))
  
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
