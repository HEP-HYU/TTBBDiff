#!/bin/bash
centraldata_dir="/data1/common/NanoAOD/v6/data/"
centralmc18_dir="/data1/common/NanoAOD/v6/mc/RunIIAutumn18NanoAODv6/"
centralmc17_dir="/data1/common/NanoAOD/v6/mc/RunIIFall17NanoAODv6/"

topnanoAODv6p1="/data1/common/topNanoAOD/v6p1/"
topnanoAODv611="/data1/common/topNanoAOD/v6-1-1/"

debug_dir="./debug/"

SECONDS=0

#debug
#./skimnanoaod.py -F --split 10 -Y 2018 $centraldata_dir/Run2018A/EGamma/NANOAOD/Nano25Oct2019-v1 $debug_dir/EGamma2018A > EGamma2018A.out
#./skimnanoaod.py -F --split 10 -Y 2017 $centraldata_dir/Run2017B/SingleElectron/NANOAOD/Nano25Oct2019-v1 $target_dir/SingleElectron2017B > SingleElectron2017B.out

./skimnanoaod.py -F --split 10 -Y 2018 -C ttbb $topnanoAODv6p1/2018/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8 $debug_dir/TTToSemiLeptonic2018_v6p1  > TTToSemiLeptonic2018_v6p1.out      #normal file
./skimnanoaod.py -F --split 10 -Y 2018 -C ttbb $topnanoAODv611/2018/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8 $debug_dir/TTToSemiLeptonic2018_v611  > TTToSemiLeptonic2018_v611.out    #extension file

./skimnanoaod.py -F --split 10 -Y 2017 -C ttbb $topnanoAODv6p1/2017/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8 $debug_dir/TTToSemiLeptonic2017  > TTToSemiLeptonic2017.out
./skimnanoaod.py -F --split 10 -Y 2017 -C ttbb $topnanoAODv6p1/2017/TTToSemiLeptonic_TuneCP5_PSweights_13TeV-powheg-pythia8 $debug_dir/TTToSemiLeptonic2017_PSweights  > TTToSemiLeptonic2017_PSweights.out

./skimnanoaod.py -F --split 10 -Y 2016 -C ttbb $topnanoAODv611/2016/TTToSemiLeptonic_TuneCP5_PSweights_13TeV-powheg-pythia8 $debug_dir/TTToSemiLeptonic2016  > TTToSemiLeptonic2016.out      #PSweights only in 2016

duration=$SECONDS
echo "$(($duration / 60)) minutes and $(($duration % 60)) seconds elapsed."
