#!/bin/bash

skimedtopNano="/data1/common/skimmed_NanoAOD/skim_ttbb/v1/"
targetmc18_dir="/data1/common/skimmed_NanoAOD/ttbb_ntuplev1/2018/"

./skimnanoaod.py -F --split 25 -Y 2018 $skimedtopNano/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8 $targetmc18_dir/TTTo2L2Nu  > TTTo2L2Nu.out
./skimnanoaod.py -F --split 25 -Y 2018 $skimedtopNano/TTToHadronic_TuneCP5_13TeV-powheg-pythia8 $targetmc18_dir/TTToHadronic  > TTToHadronic.out
./skimnanoaod.py -F --split 25 -Y 2018 $skimedtopNano/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8 $targetmc18_dir/TTToSemiLeptonic/nominal  > TTToSemiLeptonic.out
./skimnanoaod.py -F --split 25 -Y 2018 $skimedtopNano/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8 $targetmc18_dir/TTToSemiLeptonic/ext  > TTToSemiLeptonic_ext.out
