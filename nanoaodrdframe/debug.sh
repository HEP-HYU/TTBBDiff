#!/bin/bash
centraldata_dir="/data1/common/NanoAOD/data/"
output_dir="/data1/common/skimmed_NanoAOD/ttbb_debug/"
centralmc18_dir="/data1/common/NanoAOD/mc/RunIIAutumn18NanoAODv6/"
centralmc17_dir="/data1/common/NanoAOD/mc/RunIIFall17NanoAODv6/"

source_dir="/data/users/sarakm0704/ntuple/ttbb/nano2018/"
#source_dir="/data/users/sarakm0704/ntuple/ttbb/nano2017/"
debug_dir="./debug/"

SECONDS=0
#debug
#./skimnanoaod.py -F --split 5 -Y 2018 -C ttbb $source_dir/TTToSemiLeptonic_topNanoAOD $debug_dir/TTToSemiLeptonic_topNanoAOD_ttbb  > TTToSemiLeptonic_topNanoAOD_ttbb.out 
#./skimnanoaod.py -F --split 5 -Y 2018 -C ttbj $source_dir/TTToSemiLeptonic_topNanoAOD $debug_dir/TTToSemiLeptonic_topNanoAOD_ttbj  > TTToSemiLeptonic_topNanoAOD_ttbj.out &
#./skimnanoaod.py -F --split 5 -Y 2018 -C ttcc $source_dir/TTToSemiLeptonic_topNanoAOD $debug_dir/TTToSemiLeptonic_topNanoAOD_ttcc  > TTToSemiLeptonic_topNanoAOD_ttcc.out &
#./skimnanoaod.py -F --split 5 -Y 2018 -C ttLF $source_dir/TTToSemiLeptonic_topNanoAOD $debug_dir/TTToSemiLeptonic_topNanoAOD_ttLF  > TTToSemiLeptonic_topNanoAOD_ttLF.out &
#./skimnanoaod.py -F --split 5 -Y 2018 -C ttother $source_dir/TTToSemiLeptonic_topNanoAOD $debug_dir/TTToSemiLeptonic_topNanoAOD_ttother  > TTToSemiLeptonic_topNanoAOD_ttother.out &

#./skimnanoaod.py -F --split 25 -Y 2018 $centraldata_dir/Run2018A/EGamma/NANOAOD/Nano25Oct2019-v1 $debug_dir/EGamma2018A > EGamma2018A.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centraldata_dir/Run2017B/SingleElectron/NANOAOD/Nano25Oct2019-v1 $target_dir/SingleElectron2017B > SingleElectron2017B.out
./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8 $debug_dir/2017/TTToSemiLeptonic  > TTToSemiLeptonic.out
#./skimnanoaod.py -F --split 25 -Y 2018 $centralmc18_dir/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8 $debug_dir/TTToSemiLeptonic2018  > TTToSemiLeptonic2018.out

#./skimnanoaod.py -F --split 1 -Y 2017 $source_dir/SingleElB $debug_dir/SingleElB  > SingleElB.out 
#./skimnanoaod.py -F --split 1 -Y 2018 $source_dir/SingleMuA $debug_dir/SingleMuA  > SingleMuA.out 
#./skimnanoaod.py -F --split 1 -Y 2018 $source_dir/ST_s $debug_dir/ST_s  > ST_s.out
#./skimnanoaod.py -F --split 1 -Y 2018 -C ttbb $source_dir/test_v2/TTToSemiLeptonic_skimmed_outputTree $debug_dir/TTToSemiLeptonic_skimmed_ttbb  > TTToSemiLeptonic_skimmed_ttbb.out
 
duration=$SECONDS
echo "$(($duration / 60)) minutes and $(($duration % 60)) seconds elapsed."
