#!/bin/bash
centraldata_dir="/data1/common/NanoAOD/data/"
centralmc16_dir="/data1/common/NanoAOD/mc/RunIISummer16NanoAODv6/"
centralmc17_dir="/data1/common/NanoAOD/mc/RunIIFall17NanoAODv6/"
centralmc18_dir="/data1/common/NanoAOD/mc/RunIIAutumn18NanoAODv6/"
topNano18_dir="/data1/common/NanoAOD/topNanoAOD/v6p1/2018/"
topNano18ext_dir="/data1/common/NanoAOD/topNanoAOD/v6-1-1/2018/"

target_dir="/data1/common/skimmed_NanoAOD/ttbb_ntuple_v3/"

#EGamma
#./skimnanoaod.py -F --split 25 -Y 2018 $centraldata_dir/Run2018A/EGamma/NANOAOD/Nano25Oct2019-v1 $target_dir/2018/EGamma2018A > EGamma2018A.out
#./skimnanoaod.py -F --split 25 -Y 2018 $centraldata_dir/Run2018B/EGamma/NANOAOD/Nano25Oct2019-v1 $target_dir/2018/EGamma2018B > EGamma2018B.out
#./skimnanoaod.py -F --split 25 -Y 2018 $centraldata_dir/Run2018C/EGamma/NANOAOD/Nano25Oct2019-v1 $target_dir/2018/EGamma2018C > EGamma2018C.out
#./skimnanoaod.py -F --split 25 -Y 2018 $centraldata_dir/Run2018D/EGamma/NANOAOD/Nano25Oct2019-v1 $target_dir/2018/EGamma2018D > EGamma2018D.out

#./skimnanoaod.py -F --split 25 -Y 2017 $centraldata_dir/Run2017B/SingleElectron/NANOAOD/Nano25Oct2019-v1 $target_dir/2017/SingleElectron2017B > SingleElectron2017B.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centraldata_dir/Run2017C/SingleElectron/NANOAOD/Nano25Oct2019-v1 $target_dir/2017/SingleElectron2017C > SingleElectron2017C.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centraldata_dir/Run2017D/SingleElectron/NANOAOD/Nano25Oct2019-v1 $target_dir/2017/SingleElectron2017D > SingleElectron2017D.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centraldata_dir/Run2017E/SingleElectron/NANOAOD/Nano25Oct2019-v1 $target_dir/2017/SingleElectron2017E > SingleElectron2017E.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centraldata_dir/Run2017F/SingleElectron/NANOAOD/Nano25Oct2019-v1 $target_dir/2017/SingleElectron2017F > SingleElectron2017F.out

./skimnanoaod.py -F --split 25 -Y 2016 $centraldata_dir/Run2016B_ver2/SingleElectron/NANOAOD/Nano25Oct2019_ver2-v1 $target_dir/2016/SingleElectron2016B_ver2 > SingleElectron2016B_ver2.out
./skimnanoaod.py -F --split 25 -Y 2016 $centraldata_dir/Run2016C/SingleElectron/NANOAOD/Nano25Oct2019-v1 $target_dir/2016/SingleElectron2016C > SingleElectron2016C.out
./skimnanoaod.py -F --split 25 -Y 2016 $centraldata_dir/Run2016D/SingleElectron/NANOAOD/Nano25Oct2019-v1 $target_dir/2016/SingleElectron2016D > SingleElectron2016D.out
./skimnanoaod.py -F --split 25 -Y 2016 $centraldata_dir/Run2016E/SingleElectron/NANOAOD/Nano25Oct2019-v1 $target_dir/2016/SingleElectron2016E > SingleElectron2016E.out
./skimnanoaod.py -F --split 25 -Y 2016 $centraldata_dir/Run2016F/SingleElectron/NANOAOD/Nano25Oct2019-v1 $target_dir/2016/SingleElectron2016F > SingleElectron2016F.out
./skimnanoaod.py -F --split 25 -Y 2016 $centraldata_dir/Run2016G/SingleElectron/NANOAOD/Nano25Oct2019-v1 $target_dir/2016/SingleElectron2016G > SingleElectron2016G.out
./skimnanoaod.py -F --split 25 -Y 2016 $centraldata_dir/Run2016H/SingleElectron/NANOAOD/Nano25Oct2019-v1 $target_dir/2016/SingleElectron2016H > SingleElectron2016H.out
