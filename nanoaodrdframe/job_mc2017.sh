#!/bin/bash
centraldata_dir="/data1/common/NanoAOD/v6/data/"
centralmc16_dir="/data1/common/NanoAOD/v6/mc/RunIISummer16NanoAODv6/"
centralmc17_dir="/data1/common/NanoAOD/v6/mc/RunIIFall17NanoAODv6/"
centralmc18_dir="/data1/common/NanoAOD/v6/mc/RunIIAutumn18NanoAODv6/"
topNano_dir="/data1/common/topNanoAOD/v6p1/"
topNanoext_dir="/data1/common/topNanoAOD/v6-1-1/"

target_dir="/data1/common/skimmed_NanoAOD/ttbb_ntuple_v3/"

#TTbar
#./skimnanoaod.py -F --split 25 -Y 2017 $topNanoext_dir/2017/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8 $target_dir/2017/TTTo2L2Nu  > TTTo2L2Nu.out
#./skimnanoaod.py -F --split 25 -Y 2017 $topNanoext_dir/2017/TTToHadronic_TuneCP5_13TeV-powheg-pythia8 $target_dir/2017/TTToHadronic  > TTToHadronic.out
#./skimnanoaod.py -F --split 25 -Y 2017 $topNanoext_dir/2017/TTToSemiLeptonic_TuneCP5_PSweights_13TeV-powheg-pythia8 $target_dir/2017/TTToSemiLeptonic  > TTToSemiLeptonic.out
#ttSL categorization
#./skimnanoaod.py -F --split 5 -Y 2017 -C ttbb $source_dir/TTToSemiLeptonic $target_dir/2017/TTToSemiLeptonic_ttbb  > TTToSemiLeptonic_ttbb.out &
#./skimnanoaod.py -F --split 5 -Y 2017 -C ttbj $source_dir/TTToSemiLeptonic $target_dir/2017/TTToSemiLeptonic_ttbj  > TTToSemiLeptonic_ttbj.out &
#./skimnanoaod.py -F --split 5 -Y 2017 -C ttcc $source_dir/TTToSemiLeptonic $target_dir/2017/TTToSemiLeptonic_ttcc  > TTToSemiLeptonic_ttcc.out &
#./skimnanoaod.py -F --split 5 -Y 2017 -C ttLF $source_dir/TTToSemiLeptonic $target_dir/2017/TTToSemiLeptonic_ttLF  > TTToSemiLeptonic_ttLF.out &
#./skimnanoaod.py -F --split 5 -Y 2017 -C ttother $source_dir/TTToSemiLeptonic $target_dir/2017/TTToSemiLeptonic_ttother  > TTToSemiLeptonic_ttother.out &

#DY
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8 $target_dir/2017/DYJetsToLL_M-10to50  > DYJetsToLL_M-10to50.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8 $target_dir/2017/DYJetsToLL_M-50-amcatnloFXFX  > DYJetsToLL_M-50-amcatnloFXFX.out

#ST
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/ST_s-channel_4f_leptonDecays_TuneCP5_PSweights_13TeV-amcatnlo-pythia8 $target_dir/2017/ST_s-channel_4f_leptonDecays  > ST_s-channel_4f_leptonDecays.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/ST_t-channel_antitop_4f_InclusiveDecays_TuneCP5_PSweights_13TeV-powheg-pythia8 $target_dir/2017/ST_t-channel_antitop  > ST_t-channel_antitop.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/ST_t-channel_top_4f_InclusiveDecays_TuneCP5_PSweights_13TeV-powheg-pythia8 $target_dir/2017/ST_t-channel_top  > ST_t-channel_top.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8 $target_dir/2017/ST_tW_antitop  > ST_tW_antitop.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8 $target_dir/2017/ST_tW_top  > ST_tW_top.out

#ttX
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8 $target_dir/2017/TTWJetsToLNu  > TTWJetsToLNu.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/TTWJetsToQQ_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8 $target_dir/2017/TTWJetsToQQ  > TTWJetsToQQ.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8 $target_dir/2017/TTZToLLNuNu_M-10  > TTZToLLNuNu_M-10.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/TTZToQQ_TuneCP5_13TeV-amcatnlo-pythia8 $target_dir/2017/TTZToQQ_TuneCP5_13TeV  > TTZToQQ_TuneCP5_13TeV.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/ttHToNonbb_M125_TuneCP5_13TeV-powheg-pythia8 $target_dir/2017/ttHToNonbb > ttHToNonbb.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/ttHTobb_M125_TuneCP5_13TeV-powheg-pythia8 $target_dir/2017/ttHTobb > ttHTobb.out

#WJet
./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/W1JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8 $target_dir/2017/W1JetsToLNu  > W1JetsToLNu.out
./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/W2JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8 $target_dir/2017/W2JetsToLNu  > W2JetsToLNu.out
./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/W3JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8 $target_dir/2017/W3JetsToLNu  > W3JetsToLNu.out
./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/W4JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8 $target_dir/2017/W4JetsToLNu  > W4JetsToLNu.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8 $target_dir/WJetsToLNu_HT-100To200  > WJetsToLNu_HT-100To200.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8 $target_dir/WJetsToLNu_HT-200To400  > WJetsToLNu_HT-200To400.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8 $target_dir/WJetsToLNu_HT-400To600  > WJetsToLNu_HT-400To600.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8 $target_dir/WJetsToLNu_HT-600To800  > WJetsToLNu_HT-600To800.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8 $target_dir/WJetsToLNu_HT-800To1200  > WJetsToLNu_HT-800To1200.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8 $target_dir/WJetsToLNu_HT-1200To2500  > WJetsToLNu_HT-1200To2500.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM-pythia8 $target_dir/WJetsToLNu_HT-2500ToInf  > WJetsToLNu_HT-2500ToInf.out
#
##VV
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/WWTo2L2Nu_NNPDF31_TuneCP5_13TeV-powheg-pythia8 $target_dir/2017/WWTo2L2Nu  > WWTo2L2Nu.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/WWToLNuQQ_NNPDF31_TuneCP5_13TeV-powheg-pythia8 $target_dir/2017/WWToLNuQQ  > WWToLNuQQ.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/WZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8 $target_dir/2017/WZTo2L2Q  > WZTo2L2Q.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8 $target_dir/2017/WZTo3LNu  > WZTo3LNu.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/ZZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8 $target_dir/2017/ZZTo2L2Q  > ZZTo2L2Q.out
#
##QCD
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/QCD_Pt-15to20_MuEnrichedPt5_TuneCP5_13TeV_pythia8 $target_dir/QCD_Pt-15to20_MuEnrichedPt5  > QCD_Pt-15to20_MuEnrichedPt5.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/QCD_Pt-20to30_MuEnrichedPt5_TuneCP5_13TeV_pythia8 $target_dir/QCD_Pt-20to30_MuEnrichedPt5  > QCD_Pt-20to30_MuEnrichedPt5.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/QCD_Pt-30to50_MuEnrichedPt5_TuneCP5_13TeV_pythia8 $target_dir/QCD_Pt-30to50_MuEnrichedPt5  > QCD_Pt-30to50_MuEnrichedPt5.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/QCD_Pt-50to80_MuEnrichedPt5_TuneCP5_13TeV_pythia8 $target_dir/QCD_Pt-50to80_MuEnrichedPt5  > QCD_Pt-50to80_MuEnrichedPt5.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/QCD_Pt-80to120_MuEnrichedPt5_TuneCP5_13TeV_pythia8 $target_dir/QCD_Pt-80to120_MuEnrichedPt5  > QCD_Pt-80to120_MuEnrichedPt5.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/QCD_Pt-120to170_MuEnrichedPt5_TuneCP5_13TeV_pythia8 $target_dir/QCD_Pt-120to170_MuEnrichedPt5  > QCD_Pt-120to170_MuEnrichedPt5.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/QCD_Pt-170to300_MuEnrichedPt5_TuneCP5_13TeV_pythia8 $target_dir/QCD_Pt-170to300_MuEnrichedPt5  > QCD_Pt-170to300_MuEnrichedPt5.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/QCD_Pt-300to470_MuEnrichedPt5_TuneCP5_13TeV_pythia8 $target_dir/QCD_Pt-300to470_MuEnrichedPt5  > CD_Pt-300to470_MuEnrichedPt5.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/QCD_Pt-470to600_MuEnrichedPt5_TuneCP5_13TeV_pythia8 $target_dir/QCD_Pt-470to600_MuEnrichedPt5  > QCD_Pt-470to600_MuEnrichedPt5.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/QCD_Pt-600to800_MuEnrichedPt5_TuneCP5_13TeV_pythia8 $target_dir/QCD_Pt-600to800_MuEnrichedPt5  > QCD_Pt-600to800_MuEnrichedPt5.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/QCD_Pt-800to1000_MuEnrichedPt5_TuneCP5_13TeV_pythia8 $target_dir/QCD_Pt-800to1000_MuEnrichedPt5  > QCD_Pt-800to1000_MuEnrichedPt5.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/QCD_Pt-1000toInf_MuEnrichedPt5_TuneCP5_13TeV_pythia8 $target_dir/QCD_Pt-1000toInf_MuEnrichedPt5  > QCD_Pt-1000toInf_MuEnrichedPt5.out
#
##QCD
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/QCD_Pt-15to20_EMEnriched_TuneCP5_13TeV_pythia8 $target_dir/QCD_Pt-15to20_EMEnriched  > QCD_Pt-15to20_EMEnriched.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/QCD_Pt-20to30_EMEnriched_TuneCP5_13TeV_pythia8 $target_dir/QCD_Pt-20to30_EMEnriched  > QCD_Pt-20to30_EMEnriched.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/QCD_Pt-30to50_EMEnriched_TuneCP5_13TeV_pythia8 $target_dir/QCD_Pt-30to50_EMEnriched  > QCD_Pt-30to50_EMEnriched.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/QCD_Pt-50to80_EMEnriched_TuneCP5_13TeV_pythia8 $target_dir/QCD_Pt-50to80_EMEnriched  > QCD_Pt-50to80_EMEnriched.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/QCD_Pt-80to120_EMEnriched_TuneCP5_13TeV_pythia8 $target_dir/QCD_Pt-80to120_EMEnriched  > QCD_Pt-80to120_EMEnriched.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/QCD_Pt-120to170_EMEnriched_TuneCP5_13TeV_pythia8 $target_dir/QCD_Pt-120to170_EMEnriched  > QCD_Pt-120to170_EMEnriched.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/QCD_Pt-170to300_EMEnriched_TuneCP5_13TeV_pythia8 $target_dir/QCD_Pt-170to300_EMEnriched  > QCD_Pt-170to300_EMEnriched.out
#./skimnanoaod.py -F --split 25 -Y 2017 $centralmc17_dir/QCD_Pt-300toInf_EMEnriched_TuneCP5_13TeV_pythia8 $target_dir/QCD_Pt-300toInf_EMEnriched  > CD_Pt-300toInf_EMEnriched.out
