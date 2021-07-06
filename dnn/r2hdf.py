#! /usr/bin/env python
import sys, os
import numpy as np
from numpy.lib.recfunctions import stack_arrays
from ROOT import *
#from root_numpy import tree2array
from array import array
import math
import glob
import pandas as pd
import deepdish.io as io

import os
import multiprocessing as mp
import time
#from tqdm import tqdm

import utils as ut

def merge(arrayDir):
    for process in os.listdir(arrayDir+'/split/'):
        print process
        outputFile = arrayDir + '/split/' + process[:-7]
        print "merge " + process[:-7]
        selEvent = pd.DataFrame([])
        max_nevt_num = 0
        for item in range(2):
            df = pd.read_hdf(outputFile + "_Ch" + str(item) + ".h5")
            #last = 0
            if df.size != 0: last = int(df.tail(1)['event'])+1
            #df['event'] = df['event'] + max_nevt_num
            selEvent = pd.concat([selEvent,df], axis=0)
            #max_nevt_num += last

        selEvent.reset_index(drop=True, inplace=True)
        io.save(arrayDir+process[:-7]+".h5",selEvent)

def makeCombi(inputDir, inputFile, outputDir, makeTrainingInput=False, sys=''):
    print(str(inputDir+"/"+inputFile)+" start")
    chain = TChain("dnn_tree")
    chain.Add(inputDir+"/"+inputFile)

    data = False
    if 'Data' in inputDir: data = True
    ttbb = False
    if makeTrainingInput:  ttbb = True

    jetCombi = []

    for i in xrange(chain.GetEntries()) :
        chain.GetEntry(i)
        if ttbb:
            addbjet1_pt = chain.addbjet1_pt
            addbjet1_eta = chain.addbjet1_eta
            addbjet1_phi = chain.addbjet1_phi
            addbjet1_mass = chain.addbjet1_mass
            addbjet2_pt = chain.addbjet2_pt
            addbjet2_eta = chain.addbjet2_eta
            addbjet2_phi = chain.addbjet2_phi
            addbjet2_mass = chain.addbjet2_mass
        for j in range(len(chain.mbb)):
          if ttbb: signal = chain.signal[j]

          event = chain.event
          #event = i

          mbb = chain.mbb[j]
          dRbb = chain.dRbb[j]
          dPhibb = chain.dPhibb[j]
          dEtabb = chain.dEtabb[j]
          Phibb = chain.Phibb[j]
          Etabb = chain.Etabb[j]

          mlbb = chain.mlbb[j]
          dRlbb = chain.dRlbb[j]

          mlb1 = chain.mlb1[j]
          dRlb1 = chain.dRlb1[j]
          mlb2 = chain.mlb2[j]
          dRlb2 = chain.dRlb2[j]

          #mnubb = chain.mnubb[j]
          #dRnubb = chain.dRnubb[j]

          #mnub1 = chain.mnub1[j]
          #dRnub1 = chain.dRnub1[j]
          #mnub2 = chain.mnub2[j]
          #dRnub2 = chain.dRnub2[j]

          #dRlnubb = chain.dRlnubb[j]
          #dRlnub1 = chain.dRlnub1[j]
          #dRlnub2 = chain.dRlnub2[j]

          pt1 = chain.pt1[j]
          pt2 = chain.pt2[j]
      
          eta1 = chain.eta1[j]
          eta2 = chain.eta2[j]

          d1 = chain.d1[j]
          d2 = chain.d2[j]

          phi1 = chain.phi1[j]
          phi2 = chain.phi2[j]

          e1 = chain.e1[j]
          e2 = chain.e2[j]

          m1 = chain.m1[j]
          m2 = chain.m2[j]

          b_weight = chain.b_weight

          nbjets = chain.nbjets

          lepton_pt = chain.lepton_pt
          lepton_eta = chain.lepton_eta
          lepton_phi = chain.lepton_phi
          lepton_m = chain.lepton_m

          #MET = chain.MET
          #MET_phi = chain.MET_phi

          btag3rd = chain.btag3rd
          btag4th = chain.btag4th

          #if ttbb: jetCombi.append([signal, event, mbb, dRbb, dPhibb, dEtabb, Phibb, Etabb, mlbb, dRlbb, mlb1, dRlb1, mlb2, dRlb2, mnubb, dRnubb, mnub1, dRnub1, mnub2, dRnub2, dRlnubb, dRlnub1, dRlnub2, pt1, eta1, phi1, pt2, eta2, phi2, d1, d2, e1, e2, m1, m2, nbjets, lepton_pt, lepton_eta, lepton_phi, lepton_e, MET, MET_phi, btag3rd, btag4th, b_weight, addbjet1_pt, addbjet1_eta, addbjet1_phi, addbjet1_e, addbjet2_pt, addbjet2_eta, addbjet2_phi, addbjet2_e])
          #else: jetCombi.append([event, mbb, dRbb, dPhibb, dEtabb, Phibb, Etabb, mlbb, dRlbb, mlb1, dRlb1, mlb2, dRlb2, mnubb, dRnubb, mnub1, dRnub1, mnub2, dRnub2, dRlnubb, dRlnub1, dRlnub2, pt1, eta1, phi1, pt2, eta2, phi2, d1, d2, e1, e2, m1, m2, nbjets, lepton_pt, lepton_eta, lepton_phi, lepton_e, MET, MET_phi, btag3rd, btag4th, b_weight])
          if ttbb: jetCombi.append([signal, event, mbb, dRbb, dPhibb, dEtabb, Phibb, Etabb, mlbb, dRlbb, mlb1, dRlb1, mlb2, dRlb2, pt1, eta1, phi1, pt2, eta2, phi2, d1, d2, e1, e2, m1, m2, nbjets, lepton_pt, lepton_eta, lepton_phi, lepton_m, btag3rd, btag4th, b_weight, addbjet1_pt, addbjet1_eta, addbjet1_phi, addbjet1_mass, addbjet2_pt, addbjet2_eta, addbjet2_phi, addbjet2_mass])
          else: jetCombi.append([event, mbb, dRbb, dPhibb, dEtabb, Phibb, Etabb, mlbb, dRlbb, mlb1, dRlb1, mlb2, dRlb2, pt1, eta1, phi1, pt2, eta2, phi2, d1, d2, e1, e2, m1, m2, nbjets, lepton_pt, lepton_eta, lepton_phi, lepton_m, btag3rd, btag4th, b_weight])

    tmp = inputFile[:-5]
    if makeTrainingInput:
        #combi = pd.DataFrame(jetCombi, columns=['signal', 'event', 'mbb', 'dRbb', 'dPhibb', 'dEtabb', 'Phibb', 'Etabb', 'mlbb', 'dRlbb', 'mlb1', 'dRlb1', 'mlb2', 'dRlb2', 'mnubb', 'dRnubb', 'mnub1', 'dRnub1', 'mnub2', 'dRnub2', 'dRlnubb', 'dRlnub1', 'dRlnub2', 'pt1', 'eta1', 'phi1', 'pt2', 'eta2', 'phi2', 'd1', 'd2', 'e1', 'e2', 'm1', 'm2', 'nbjets', 'lepton_pt', 'lepton_eta', 'lepton_phi', 'lepton_e', 'MET', 'MET_phi', 'btag3rd', 'btag4th', 'b_weight','addbjet1_pt','addbjet1_eta','addbjet1_phi','addbjet1_e','addbjet2_pt','addbjet2_eta','addbjet2_phi','addbjet2_e'])
        combi = pd.DataFrame(jetCombi, columns=['signal', 'event', 'mbb', 'dRbb', 'dPhibb', 'dEtabb', 'Phibb', 'Etabb', 'mlbb', 'dRlbb', 'mlb1', 'dRlb1', 'mlb2', 'dRlb2', 'pt1', 'eta1', 'phi1', 'pt2', 'eta2', 'phi2', 'd1', 'd2', 'e1', 'e2', 'm1', 'm2', 'nbjets', 'lepton_pt', 'lepton_eta', 'lepton_phi', 'lepton_m', 'btag3rd', 'btag4th', 'b_weight','addbjet1_pt','addbjet1_eta','addbjet1_phi','addbjet1_mass','addbjet2_pt','addbjet2_eta','addbjet2_phi','addbjet2_mass'])
        io.save(outputDir+"split/array_"+tmp+".h5",combi)

    else:
        #combi = pd.DataFrame(jetCombi, columns=['event', 'mbb', 'dRbb', 'dPhibb', 'dEtabb', 'Phibb', 'Etabb', 'mlbb', 'dRlbb', 'mlb1', 'dRlb1', 'mlb2', 'dRlb2', 'mnubb', 'dRnubb', 'mnub1', 'dRnub1', 'mnub2', 'dRnub2', 'dRlnubb', 'dRlnub1', 'dRlnub2', 'pt1', 'eta1', 'phi1', 'pt2', 'eta2', 'phi2', 'd1', 'd2', 'e1', 'e2', 'm1', 'm2', 'nbjets', 'lepton_pt', 'lepton_eta', 'lepton_phi', 'lepton_e', 'MET', 'MET_phi', 'btag3rd', 'btag4th', 'b_weight'])
        combi = pd.DataFrame(jetCombi, columns=['event', 'mbb', 'dRbb', 'dPhibb', 'dEtabb', 'Phibb', 'Etabb', 'mlbb', 'dRlbb', 'mlb1', 'dRlb1', 'mlb2', 'dRlb2', 'pt1', 'eta1', 'phi1', 'pt2', 'eta2', 'phi2', 'd1', 'd2', 'e1', 'e2', 'm1', 'm2', 'nbjets', 'lepton_pt', 'lepton_eta', 'lepton_phi', 'lepton_m', 'btag3rd', 'btag4th', 'b_weight'])
        io.save(outputDir+"split/array_"+tmp+".h5",combi)

    print(str(inputDir+"/"+inputFile)+" end")

if __name__ == '__main__':
    #Options
    from optparse import OptionParser

    parser = OptionParser()
    parser.usage = """
    %prog [options] option
    convert root ntuple to array 
    """

    parser.add_option("-m", "--merge", dest="merge",
                      action = 'store_true',
                      default=False,
                      help='Merge array files for each process')

    parser.add_option("-t", "--test", dest="test",
		      action = 'store_true',
		      default=False,
		      help='Test single root file')

    parser.add_option("-d", "--deep", dest="deep",
		      action = 'store_true',
		      default=False,
		      help='Run on signal sample for deep learning train')

    parser.add_option("-a", "--all", dest="all",
		      action = 'store_true',
		      default=False,
		      help='all root fiels in input directory')

    parser.add_option("-s", "--sys", dest="sys",
		      action = 'store_true',
		      default=False,
		      help='all root fiels in input directory')

    (options,args) = parser.parse_args()

    ntupleDir = './dnn_tree/'
    arrayDir = './array/'

    processes = []
    if len(args) is 1:
        f = open(args[0], "r") 
        processes = f.read().splitlines()
    else:
        processes = os.listdir(ntupleDir) 

    start_time = time.time()

    if options.merge:
        merge(arrayDir)

    if options.deep:
        #for 2017 PSweight sample
        #makeCombi(ntupleDir, 'dnn_TTToSemiLeptonic_PSweight_ttbb_Ch0.root', arrayDir, True)
        #makeCombi(ntupleDir, 'dnn_TTToSemiLeptonic_PSweight_ttbb_Ch1.root', arrayDir, True)

        makeCombi(ntupleDir, 'dnn_TTToSemiLeptonic_ttbb_Ch0.root', arrayDir, True)
        makeCombi(ntupleDir, 'dnn_TTToSemiLeptonic_ttbb_Ch1.root', arrayDir, True)

    if options.test:
        if not os.path.exists("test"):
            os.makedirs('./test')
            makeCombi(ntupleDir+'TTLJ_PowhegPythia_ttbb','Tree_ttbbLepJets_002.root' ,'./test')

    if options.all:
        for process in processes:
                os.makedirs(arrayDir+process)

                proc = process.split(".")[0]
                makeCombi(ntupleDir, process, arrayDir)

    if options.sys:
        syslist = ['jecup','jecdown','jerup','jerdown']
        for sys in syslist:
            for process in processes:
                if not ('Data' in process or 'SYS' in process or 'Herwig' in process or 'Evtgen' in process or 'aMC' in process):
                    os.makedirs(arrayDir+process+'__'+sys)
                    for item in os.listdir(ntupleDir+process):
                        proc = mp.Process(target=makeCombi, args=(ntupleDir+process,item,arrayDir+process+'__'+sys,sys))
                        proc.start()
                    for item in os.listdir(ntupleDir+process):
                        proc.join()

    print("Total running time :%s " %(time.time() - start_time))

