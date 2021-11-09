#!/bin/bash
topNano_dir="/data1/common/topNanoAOD/v6p1/"
topNanoext_dir="/data1/common/topNanoAOD/v6-1-1/"

source_dir="/data1/common/skimmed_NanoAOD/skim_ttbb/v3/"i
target_dir="/data1/common/skimmed_NanoAOD/ttbb_ntuple_v3/"

##2016
#./skimnanoaod.py -F --split 25 -Y 2016 -C ttbb $topNanoext_dir/2016/TTToSemiLeptonic_TuneCP5_PSweights_13TeV-powheg-pythia8 $target_dir/2016/TTToSemiLeptonic_ttbb  > TTToSemiLeptonic_2016_ttbb.out 
#./skimnanoaod.py -F --split 25 -Y 2016 -C ttbj $topNanoext_dir/2016/TTToSemiLeptonic_TuneCP5_PSweights_13TeV-powheg-pythia8 $target_dir/2016/TTToSemiLeptonic_ttbj  > TTToSemiLeptonic_2016_ttbj.out 
#./skimnanoaod.py -F --split 25 -Y 2016 -C ttcc $topNanoext_dir/2016/TTToSemiLeptonic_TuneCP5_PSweights_13TeV-powheg-pythia8 $target_dir/2016/TTToSemiLeptonic_ttcc  > TTToSemiLeptonic_2016_ttcc.out 
#./skimnanoaod.py -F --split 25 -Y 2016 -C ttother $topNanoext_dir/2016/TTToSemiLeptonic_TuneCP5_PSweights_13TeV-powheg-pythia8 $target_dir/2016/TTToSemiLeptonic_ttother  > TTToSemiLeptonic_2016_ttother.out 
#
##2017
#./skimnanoaod.py -F --split 25 -Y 2017 -C ttbb $topNano_dir/2017/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8 $target_dir/2017/TTToSemiLeptonic_ttbb  > TTToSemiLeptonic_2017_ttbb.out
#./skimnanoaod.py -F --split 25 -Y 2017 -C ttbj $topNano_dir/2017/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8 $target_dir/2017/TTToSemiLeptonic_ttbj  > TTToSemiLeptonic_2017_ttbj.out
#./skimnanoaod.py -F --split 25 -Y 2017 -C ttcc $topNano_dir/2017/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8 $target_dir/2017/TTToSemiLeptonic_ttcc  > TTToSemiLeptonic_2017_ttcc.out
#./skimnanoaod.py -F --split 25 -Y 2017 -C ttother $topNano_dir/2017/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8 $target_dir/2017/TTToSemiLeptonic_ttother  > TTToSemiLeptonic_2017_ttother.out
#
#./skimnanoaod.py -F --split 25 -Y 2017 -C ttbb $topNano_dir/2017/TTToSemiLeptonic_TuneCP5_PSweights_13TeV-powheg-pythia8 $target_dir/2017/TTToSemiLeptonic_PSweight_ttbb  > TTToSemiLeptonic_2017_PSweight_ttbb.out 
#./skimnanoaod.py -F --split 25 -Y 2017 -C ttbj $topNano_dir/2017/TTToSemiLeptonic_TuneCP5_PSweights_13TeV-powheg-pythia8 $target_dir/2017/TTToSemiLeptonic_PSweight_ttbj  > TTToSemiLeptonic_2017_PSweight_ttbj.out 
#./skimnanoaod.py -F --split 25 -Y 2017 -C ttcc $topNano_dir/2017/TTToSemiLeptonic_TuneCP5_PSweights_13TeV-powheg-pythia8 $target_dir/2017/TTToSemiLeptonic_PSweight_ttcc  > TTToSemiLeptonic_2017_PSweight_ttcc.out 
#./skimnanoaod.py -F --split 25 -Y 2017 -C ttother $topNano_dir/2017/TTToSemiLeptonic_TuneCP5_PSweights_13TeV-powheg-pythia8 $target_dir/2017/TTToSemiLeptonic_PSweight_ttother  > TTToSemiLeptonic_2017_PSweight_ttother.out
#
#2018
#./skimnanoaod.py -F --split 25 -Y 2018 -C ttbb $source_dir/2018/TTToSemiLeptonic $target_dir/2018/TTToSemiLeptonic_ttbb  > TTToSemiLeptonic_2018_ttbb.out
#./skimnanoaod.py -F --split 25 -Y 2018 -C ttbj $source_dir/2018/TTToSemiLeptonic $target_dir/2018/TTToSemiLeptonic_ttbj  > TTToSemiLeptonic_2018_ttbj.out
#./skimnanoaod.py -F --split 25 -Y 2018 -C ttcc $source_dir/2018/TTToSemiLeptonic $target_dir/2018/TTToSemiLeptonic_ttcc  > TTToSemiLeptonic_2018_ttcc.out &
#./skimnanoaod.py -F --split 25 -Y 2018 -C ttother $source_dir/2018/TTToSemiLeptonic $target_dir/2018/TTToSemiLeptonic_ttother  > TTToSemiLeptonic_2018_ttother.out &

start_time="$(date -u +%s)"

./skimnanoaod.py -F --split 25 -Y 2018 -C ttbb /data1/users/tjkim/topNanoAOD/v9-1-1/2018UL/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8 /data1/common/skimmed_NanoAOD/ttbb_ntuple_ULv0/2018/TTToSemiLeptonic_ttbb > TTToSemiLeptonic_2018_ttbb.out 

end_time="$(date -u +%s)"

elapsed="$(($end_time-$start_time))"
echo "Total of $elapsed seconds elapsed for process"
