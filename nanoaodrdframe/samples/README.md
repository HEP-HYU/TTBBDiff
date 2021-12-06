Usage
```
source makelist.sh <list.txt>
```
You may set proxy before run makelist.sh.
In the list.txt, there should be three arguments for make lists.
You can check also lfvsamplelists.txt for example.

e.g.) For data and mc,
```
data        SingleMuon18A               /SingleMuon/Run2018A-UL2018_MiniAODv1_NanoAODv2-v3/NANOAOD
mc/16pre    TTTo2L2Nu                   /TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL16NanoAODAPVv2-106X_mcRun2_asymptotic_preVFP_v9-v1/NANOAODSIM
```
1st argument : Folders that txt files are saved.
2nd argument : The name of txt files. (${arg2}.txt)
3rd argument : DAS name of sample.
