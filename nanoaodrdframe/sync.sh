#!/bin/bash
centraldata_dir="/data1/common/NanoAOD/data/"
centralmc16_dir="/data/common/NanoAOD/mc/RunIISummer16NanoAODv6/"
centralmc17_dir="/data1/common/NanoAOD/mc/RunIIFall17NanoAODv6/"
centralmc18_dir="/data/common/NanoAOD/mc/RunIIAutumn18NanoAODv6/"
topNano_dir="/data1/common/topNanoAOD/v6p1/"
#topNano18ext_dir="/data/common/NanoAOD/topNanoAOD/v6-1-1/2018/"

#target_dir="/data1/common/skimmed_NanoAOD/ttbb_ntuplev1/2018/"
target_dir="/data/users/sarakm0704/ntuple/ttbb/sync/v3/"

#skimmed TTSL
#source_dir="/data1/common/skimmed_NanoAOD/skim_ttbb/v1/"
#target_dir="/data1/common/skimmed_NanoAOD/ttbb_ntuplev1/2018/"

#SingleMuon
#./skimnanoaod.py -F --split 25 -Y 2018 $centraldata_dir/Run2018A/SingleMuon/NANOAOD/Nano25Oct2019-v1 $target_dir/SingleMuon2018A > SingleMuon2018A.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centraldata_dir/Run2017B/SingleMuon/NANOAOD/Nano25Oct2019-v1 $target_dir/SingleMuon2017A > SingleMuon2017A.out
#./skimnanoaod.py -F --split 25 -Y 2016 $centraldata_dir/Run2016B/SingleMuon/NANOAOD/Nano25Oct2019-v1 $target_dir/SingleMuon2017A > SingleMuon2017A.out
#
##EGamma / SingleElectron
#./skimnanoaod.py -F --split 25 -Y 2018 $centraldata_dir/Run2018A/EGamma/NANOAOD/Nano25Oct2019-v1 $target_dir/EGamma2018A > EGamma2018A.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centraldata_dir/Run2017B/SingleElectron/NANOAOD/Nano25Oct2019-v1 $target_dir/SingleElectron2017B > SingleElectron2017B.out
#./skimnanoaod.py -F --split 25 -Y 2016 $centraldata_dir/Run2016B_ver2/SingleElectron/NANOAOD/Nano25Oct2019_ver2-v1 $target_dir/SingleElectron2016B > SingleElectron2016B.out
#
#TTbar
./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8 $target_dir/TTToSemiLeptonic2017  > TTToSemiLeptonic2017.out
#./skimnanoaod.py -F --split 25 -Y 2018 $topNano18_dir/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8 $target_dir/TTToSemiLeptonic/normal  > TTToSemiLeptonic.out
#./skimnanoaod.py -F --split 25 -Y 2018 $centralmc18_dir/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8 $target_dir/TTToSemiLeptonic2018/normal  > TTToSemiLeptonic2018.out
#./skimnanoaod.py -F --split 25 -Y 2018 $topNano18_dir/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8 $target_dir/TTToSemiLeptonic/normal  > TTToSemiLeptonic.out
#./skimnanoaod.py -F --split 25 -Y 2018 $topNano18_dir/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8 $target_dir/TTToSemiLeptonic/normal  > TTToSemiLeptonic.out
