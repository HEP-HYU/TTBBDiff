import ROOT
from ROOT import *
from array import array
import os,sys
import time

#inputDir = '/data1/common/skimmed_NanoAOD/ttbb_ntuple_v3/2016/'
#inputDir = '/data1/common/skimmed_NanoAOD/ttbb_ntuple_v3/2017/'
inputDir = '/data1/common/skimmed_NanoAOD/ttbb_ntuple_v3/2016/'

def writeConfig(inputDir, process):

    evt = -1
    xsec = -1
    fillcolor = 'poo'
    group = 'poo'
    order = 'poo'

    #### TTbar ####
    if "TTToSemiLeptonic" in process:
      xsec = 365.34
      fillcolor = '#330000'
      group = 'GttSL'
      order = 0

      if "ttbb" in process:
        fillcolor = '#330000'
        group = 'Gttbb'
        order = 0
        if "Filter" in process:
          fillcolor = '#330000'
          group = 'Gttbb'
          order = 0
        elif "openloops" in process:
          fillcolor = '#330000'
          group = 'Gttbb'
          order = 0
      elif "ttbj" in process:
        fillcolor = '#660000'
        group = 'Gttbj'
        order = 1
      elif "ttcc" in process:
        fillcolor = '#990000'
        group = "Gttcc"
        order = 2
      elif "ttLF" in process:
        fillcolor = '#cc0000'
        group = "GttLF"
        order = 3
      elif "ttother" in process:
        fillcolor = '#ff6666'
        group = "GttBkg"
        order = 4
      elif "Bkg" in process:
        fillcolor = '#ff6666'
        group = "GttBkg"
        order = 6
      else: print("RAISE "+str(process))

    elif "TTTo2L2Nu" in process:
      xsec = 88.29
      fillcolor = '#ff6666'
      group = "GttBkg"
      order = 5
 
    elif "TTToHadronic" in process:
      xsec = 377.96
      fillcolor = '#ff6666'
      group = "GttBkg"
      order = 7

    #### SingleTop ####
    elif "ST" in process:
      fillcolor = '#990099'
      group = "GSingleT"
      
      if "ST_s-" in process:
        xsec = 3.36
        order = 16

      elif "ST_t-channel_top" in process:
        xsec = 136.02
        order = 14

      elif "ST_t-channel_antitop" in process:
        xsec = 80.95
        order = 17

      elif "ST_tW_top" in process:
        xsec = 35.85
        order = 15

      elif "ST_tW_antitop" in process:
        xsec = 35.85
        order = 18

    #### W+Jet ####
#    elif "WJet" in process:
#      xsec = 61526

    elif "W1Jet" in process:
      xsec = 9625
      group = "GVJets"
      fillcolor = '#ff9933'
      order = 24

    elif "W2Jet" in process:
      xsec = 2793
      group = "GVJets"
      fillcolor = '#ff9933'
      order = 25

    elif "W3Jet" in process:
      xsec = 992.5
      group = "GVJets"
      fillcolor = '#ff9933'
      order = 26

    elif "W4Jet" in process:
      xsec = 544.3
      group = "GVJets"
      fillcolor = '#ff9933'
      order = 27

    elif "WWTo2L2Nu" in process:
      xsec = 12.178
      fillcolor = '#00cccc'
      group = "GVV"
      order = 19

    elif "WWToLNuQQ" in process:
      xsec = 49.997
      fillcolor = '#00cccc'
      group = "GVV"
      order = 19

    elif "WZTo2L2Q" in process:
      xsec = 5.595
      fillcolor = '#00cccc'
      group = "GVV"
      order = 20

    elif "WZTo3LNu" in process:
      xsec = 4.42965
      fillcolor = '#00cccc'
      group = "GVV"
      order = 20

    elif "DYJetsToLL_M-10to50" in process:
      xsec = 18610.0
      fillcolor = '#000099'
      group = "GVJets"
      order = 23

    elif "DYJetsToLL_M-50" in process:
      xsec = 6077.22
      fillcolor = '#000099'
      group = "GVJets"
      order = 22

    elif "ZZTo2L2Q" in process:
      xsec = 3.22
      fillcolor = '#00cccc'
      group = "GVV"
      order = 21

    elif "ttHToNonbb" in process:
      xsec = 0.2151
      fillcolor = '#ff66ff'
      group = "GttH"
      order = 9

    elif "ttHTobb" in process:
      xsec = 0.2934
      fillcolor = '#ff66ff'
      group = "GttH"
      order = 8

    elif "TTWJetsToLNu" in process:
      xsec = 0.2043
      fillcolor = '#ff66ff'
      group = "GttX"
      order = 11

    elif "TTWJetsToQQ" in process:
      xsec = 0.4062
      fillcolor = '#ff66ff'
      group = "GttX"
      order = 10

    elif "TTZToLLNuNu_M-10" in process:
      xsec = 0.2529
      fillcolor = '#ff66ff'
      group = "GttX"
      order = 13

    elif "TTZToQQ_TuneCP5" in process:
      xsec = 0.5297
      fillcolor = '#ff66ff'
      group = "GttX"
      order = 12

    else: print("RAISE "+str(process))

    print("  pretty-name: '"+str(process)+"'")
    print("  cross-section: "+str(xsec))

    print("  fill-color: '"+str(fillcolor)+"'")
    print("  group: "+str(group))
    print("  order: "+str(order))


def merge(inputDir, process):
    f = TFile.Open(inputDir+process)
    hist = f.Get('hcounter1_nocut')
    h_merge.Add(hist)

start_time = time.time()

print("Start processing")

for target in os.listdir(inputDir):

#    if "TTToSemi" in target: continue

    h_merge = TH1D('h_merge','Event counter',2,-0.5,1.5)

    for process in os.listdir(inputDir+target):
        #print("Start "+str(process))
        merge(inputDir+target+"/", process)

    print("")      
    print("'root17/hist_"+str(target)+".root':")
    
    if "Single" in target or "EGamma" in target:
      print("  type: data")
      print("  pretty-name: '"+str(target)+"'")
      print("  marker-size: 0.6")
      print("  group: GData")
    else:
      print("  type: mc")
      writeConfig(inputDir,target)
      print("  generated-events: "+str(h_merge.Integral()))

#print("Done! Total running time :%s " %(time.time() - start_time))
