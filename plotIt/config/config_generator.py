import ROOT
from ROOT import *
import os,sys

#inputDir = '/data/users/sarakm0704/ntuple/ttbb/V10_3/sync/merged/'
#inputDir = '/data/users/sarakm0704/ntuple/ttbb/V9_7/merged/'
inputDir = '/data/users/sarakm0704/ntuple/ttbb/V8_1/merged/'

def writeConfig(inputDir, process):

    evt = -1
    xsec = -1
    fillcolor = 'poo'
    group = 'poo'
    order = 'poo'

    #### TTbar ####
    if "TTLJ" in process:
      xsec = 365.34

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

    elif "TTLL" in process:
      xsec = 88.29
      fillcolor = '#ff6666'
      group = "GttBkg"
      order = 5

 
    elif "TTJJ" in process:
      xsec = 377.96
      fillcolor = '#ff6666'
      group = "GttBkg"
      order = 7

    #### SingleTop ####
    elif "SingleT" in process:
      fillcolor = '#990099'
      group = "GSingleT"
      
      if "SingleTop_s_" in process:
        xsec = 3.36
        order = 16

      elif "SingleTop_t_" in process:
        xsec = 136.02
        order = 14

      elif "SingleTbar_t_" in process:
        xsec = 80.95
        order = 17

      elif "SingleTop_tW_" in process:
        xsec = 35.85
        order = 15

      elif "SingleTbar_tW_" in process:
        xsec = 35.85
        order = 18

    #### W+Jet ####
#    elif "WJet" in process:
#      xsec = 61526

    elif "W1Jet" in process:
      xsec = 9625
      group = "GWJets"
      fillcolor = '#ff9933'
      order = 24

    elif "W2Jet" in process:
      xsec = 2793
      group = "GWJets"
      fillcolor = '#ff9933'
      order = 25

    elif "W3Jet" in process:
      xsec = 992.5
      group = "GWJets"
      fillcolor = '#ff9933'
      order = 26

    elif "W4Jet" in process:
      xsec = 544.3
      group = "GWJets"
      fillcolor = '#ff9933'
      order = 27

    elif "WW" in process:
      xsec = 118.7
      fillcolor = '#00cccc'
      group = "GVV"
      order = 19

    elif "WZ" in process:
      xsec = 47.13
      fillcolor = '#00cccc'
      group = "GVV"
      order = 20

    elif "ZJets_M10to50" in process:
      xsec = 18610.0
      fillcolor = '#000099'
      group = "GZJets"
      order = 23

    elif "ZJets_M50" in process:
      xsec = 6077.22
      fillcolor = '#000099'
      group = "GZJets"
      order = 22

    elif "ZZ" in process:
      xsec = 16.523
      fillcolor = '#00cccc'
      group = "GVV"
      order = 21

    elif "ttHToNonbb" in process:
      xsec = 0.2151
      fillcolor = '#ff66ff'
      group = "GttX"
      order = 9

    elif "ttHTobb" in process:
      xsec = 0.2934
      fillcolor = '#ff66ff'
      group = "GttX"
      order = 8

    elif "ttWToLNu" in process:
      xsec = 0.2043
      fillcolor = '#ff66ff'
      group = "GttX"
      order = 11

    elif "ttWToQQ" in process:
      xsec = 0.4062
      fillcolor = '#ff66ff'
      group = "GttX"
      order = 10

    elif "ttZToLLNuNu" in process:
      xsec = 0.2529
      fillcolor = '#ff66ff'
      group = "GttX"
      order = 13

    elif "ttZToQQ" in process:
      xsec = 0.5297
      fillcolor = '#ff66ff'
      group = "GttX"
      order = 12

    elif "QCD" in process:
      fillcolor = '#d0cfd4'
      group = "GQCD"
      
      if "15to20_MuEnriched" in process:
        xsec = 3819570.0
        order = 29
      elif "20to30_MuEnriched" in process:
        xsec = 2960198.4
        order = 30
      elif "50to80_MuEnriched" in process:
        xsec = 437504.1
        order = 31
      elif "80to120_MuEnriched" in process:
        xsec = 106033.6648
        order = 32
      elif "120to170_MuEnriched" in process:
        xsec = 25190.51514
        order = 33
      elif "170to300_MuEnriched" in process:
        xsec = 8654.49315
        order = 34
      elif "300to470_MuEnriched" in process:
        xsec = 797.35269
        order = 35
      elif "470to600_MuEnriched" in process:
        xsec = 79.02553776
        order = 36
      elif "600to800_MuEnriched" in process:
        xsec = 25.09505908
        order = 37
      elif "800to1000_MuEnriched" in process:
        xsec = 4.707368272
        order = 37
      elif "1000toInf_MuEnriched" in process:
        xsec = 1.62131692
        order = 38

      elif "15to20_EMEnriched" in process:
        xsec = 2302200.0
        order = 39
      elif "20to30_EMEnriched" in process:
        xsec = 5352960.0
        order = 40
      elif "30to50_EMEnriched" in process:
        xsec = 9928000.0
        order = 41
      elif "50to80_EMEnriched" in process:
        xsec = 2890800.0
        order = 42
      elif "80to120_EMEnriched" in process:
        xsec = 350000.0
        order = 43
      elif "120to170_EMEnriched" in process:
        xsec = 62964.0
        order = 44
      elif "170to300_EMEnriched" in process:
        xsec = 18810.0
        order = 45
      elif "300toInf_EMEnriched" in process:
        xsec = 1350.0
        order = 46

    else: print("RAISE "+str(process))

    f = TFile.Open(inputDir+process)

    h_eventInfo = f.Get('ttbbLepJets/EventInfo')
    evt = h_eventInfo.GetBinContent(2)
    
    f.Close()

    print("  pretty-name: '"+str(process[:-5])+"'")
    print("  cross-section: "+str(xsec))
    print("  generated-events: "+str(evt))
    print("  fill-color: '"+str(fillcolor)+"'")
    print("  group: "+str(group))
    print("  order: "+str(order))

for process in os.listdir(inputDir):   

    if "WJet" in process: continue

    if not "root" in process: continue
    print("\n'root16/hist_"+str(process)+"':")
    if "Data" in process:
      print("  type: data")
      print("  pretty-name: '"+str(process[:-5])+"'")
      print("  marker-size: 0.6")
      print("  group: GData")
    else: 
      print("  type: mc")
      writeConfig(inputDir,process)
    
print ("DONE")
