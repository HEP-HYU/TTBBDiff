#!/bin/bash
centraldata_dir="/data1/common/NanoAOD/data/"
centralmc16_dir="/data1/common/NanoAOD/mc/RunIISummer16NanoAODv6/"
centralmc17_dir="/data1/common/NanoAOD/mc/RunIIFall17NanoAODv6/"
centralmc18_dir="/data1/common/NanoAOD/mc/RunIIAutumn18NanoAODv6/"
topNano18_dir="/data1/common/NanoAOD/topNanoAOD/v6p1/2018/"
topNano18ext_dir="/data1/common/NanoAOD/topNanoAOD/v6-1-1/2018/"

target_dir="/data1/common/skimmed_NanoAOD/ttbb_ntuple_v3/"

#SingleMuon
#2018
#./skimnanoaod.py -F --split 25 -Y 2018 $centraldata_dir/Run2018A/SingleMuon/NANOAOD/Nano25Oct2019-v1 $target_dir/2018/SingleMuon2018A > SingleMuon2018A.out
#./skimnanoaod.py -F --split 25 -Y 2018 $centraldata_dir/Run2018B/SingleMuon/NANOAOD/Nano25Oct2019-v1 $target_dir/2018/SingleMuon2018B > SingleMuon2018B.out
#./skimnanoaod.py -F --split 25 -Y 2018 $centraldata_dir/Run2018C/SingleMuon/NANOAOD/Nano25Oct2019-v1 $target_dir/2018/SingleMuon2018C > SingleMuon2018C.out
#./skimnanoaod.py -F --split 25 -Y 2018 $centraldata_dir/Run2018D/SingleMuon/NANOAOD/Nano25Oct2019-v1 $target_dir/2018/SingleMuon2018D > SingleMuon2018D.out

#2017
#./skimnanoaod.py -F --split 25 -Y 2017 $centraldata_dir/Run2017B/SingleMuon/NANOAOD/Nano25Oct2019-v1 $target_dir/2017/SingleMuon2017B > SingleMuon2017B.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centraldata_dir/Run2017C/SingleMuon/NANOAOD/Nano25Oct2019-v1 $target_dir/2017/SingleMuon2017C > SingleMuon2017C.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centraldata_dir/Run2017D/SingleMuon/NANOAOD/Nano25Oct2019-v1 $target_dir/2017/SingleMuon2017D > SingleMuon2017D.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centraldata_dir/Run2017E/SingleMuon/NANOAOD/Nano25Oct2019-v1 $target_dir/2017/SingleMuon2017E > SingleMuon2017E.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centraldata_dir/Run2017F/SingleMuon/NANOAOD/Nano25Oct2019-v1 $target_dir/2017/SingleMuon2017F > SingleMuon2017F.out
#
#2016
./skimnanoaod.py -F --split 25 -Y 2016 $centraldata_dir/Run2016B_ver2/SingleMuon/NANOAOD/Nano25Oct2019_ver2-v1 $target_dir/2016/SingleMuon2016B_ver2 > SingleMuon2016B_ver2.out
./skimnanoaod.py -F --split 25 -Y 2016 $centraldata_dir/Run2016C/SingleMuon/NANOAOD/Nano25Oct2019-v1 $target_dir/2016/SingleMuon2016C > SingleMuon2016C.out
./skimnanoaod.py -F --split 25 -Y 2016 $centraldata_dir/Run2016D/SingleMuon/NANOAOD/Nano25Oct2019-v1 $target_dir/2016/SingleMuon2016D > SingleMuon2016D.out
./skimnanoaod.py -F --split 25 -Y 2016 $centraldata_dir/Run2016E/SingleMuon/NANOAOD/Nano25Oct2019-v1 $target_dir/2016/SingleMuon2016E > SingleMuon2016E.out
./skimnanoaod.py -F --split 25 -Y 2016 $centraldata_dir/Run2016F/SingleMuon/NANOAOD/Nano25Oct2019-v1 $target_dir/2016/SingleMuon2016F > SingleMuon2016F.out
./skimnanoaod.py -F --split 25 -Y 2016 $centraldata_dir/Run2016G/SingleMuon/NANOAOD/Nano25Oct2019-v1 $target_dir/2016/SingleMuon2016G > SingleMuon2016G.out
./skimnanoaod.py -F --split 25 -Y 2016 $centraldata_dir/Run2016H/SingleMuon/NANOAOD/Nano25Oct2019-v1 $target_dir/2016/SingleMuon2016H > SingleMuon2016H.out
