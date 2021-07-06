#Producing histogram
#options
#root -l -b -q ana_rdf.C'("year","sample","channel","dnntree saving")' 
#2016
#ttbar
root -l -b -q ana_rdf.C'("2016","TTToSemiLeptonic_ttbb","0","1")'& #
root -l -b -q ana_rdf.C'("2016","TTToSemiLeptonic_ttbj","0","1")'& #
root -l -b -q ana_rdf.C'("2016","TTToSemiLeptonic_ttcc","0","1")'& #
root -l -b -q ana_rdf.C'("2016","TTToSemiLeptonic_ttother","0","1")'& #
root -l -b -q ana_rdf.C'("2016","TTTo2L2Nu","0","1")'& #
root -l -b -q ana_rdf.C'("2016","TTToHadronic","0","1")'& #
#VV
root -l -b -q ana_rdf.C'("2016","WWTo2L2Nu","0","1")' & #
root -l -b -q ana_rdf.C'("2016","WWToLNuQQ","0","1")' & #
root -l -b -q ana_rdf.C'("2016","WZTo2L2Q","0","1")' & #
root -l -b -q ana_rdf.C'("2016","WZTo3LNu","0","1")' & #
root -l -b -q ana_rdf.C'("2016","ZZTo2L2Q","0","1")' & #
#Wjets
root -l -b -q ana_rdf.C'("2016","W1JetsToLNu","0","1")' & #
root -l -b -q ana_rdf.C'("2016","W2JetsToLNu","0","1")' & #
root -l -b -q ana_rdf.C'("2016","W3JetsToLNu","0","1")' & #
root -l -b -q ana_rdf.C'("2016","W4JetsToLNu","0","1")' & #
#Zjets
root -l -b -q ana_rdf.C'("2016","DYJetsToLL_M-10to50","0","1")' & #
root -l -b -q ana_rdf.C'("2016","DYJetsToLL_M-50-amcatnloFXFX","0","1")' & #
#single top
root -l -b -q ana_rdf.C'("2016","ST_t-channel_antitop","0","1")' & #
root -l -b -q ana_rdf.C'("2016","ST_tW_antitop","0","1")' & #
root -l -b -q ana_rdf.C'("2016","ST_s-channel_4f_leptonDecays","0","1")' & #
root -l -b -q ana_rdf.C'("2016","ST_t-channel_top","0","1")' & #
root -l -b -q ana_rdf.C'("2016","ST_tW_top","0","1")' & #
#ttX
root -l -b -q ana_rdf.C'("2016","TTWJetsToLNu","0","1")' & #
root -l -b -q ana_rdf.C'("2016","TTWJetsToQQ","0","1")' & #
root -l -b -q ana_rdf.C'("2016","TTZToLLNuNu_M-10","0","1")' & #
root -l -b -q ana_rdf.C'("2016","TTZToQQ_TuneCP5_13TeV","0","1")' & #
root -l -b -q ana_rdf.C'("2016","ttHTobb","0","1")' & #
root -l -b -q ana_rdf.C'("2016","ttHToNonbb","0","1")' & #
#ttbar
root -l -b -q ana_rdf.C'("2016","TTToSemiLeptonic_ttbb","1","1")'& #
root -l -b -q ana_rdf.C'("2016","TTToSemiLeptonic_ttbj","1","1")'& #
root -l -b -q ana_rdf.C'("2016","TTToSemiLeptonic_ttcc","1","1")'& #
root -l -b -q ana_rdf.C'("2016","TTToSemiLeptonic_ttother","1","1")'& #
root -l -b -q ana_rdf.C'("2016","TTTo2L2Nu","1","1")'& #
root -l -b -q ana_rdf.C'("2016","TTToHadronic","1","1")'& #
#VV
root -l -b -q ana_rdf.C'("2016","WWTo2L2Nu","1","1")' & #
root -l -b -q ana_rdf.C'("2016","WWToLNuQQ","1","1")' & #
root -l -b -q ana_rdf.C'("2016","WZTo2L2Q","1","1")' & #
root -l -b -q ana_rdf.C'("2016","WZTo3LNu","1","1")' & #
root -l -b -q ana_rdf.C'("2016","ZZTo2L2Q","1","1")' & #
#Wjets
root -l -b -q ana_rdf.C'("2016","W1JetsToLNu","1","1")' & #
root -l -b -q ana_rdf.C'("2016","W2JetsToLNu","1","1")' & #
root -l -b -q ana_rdf.C'("2016","W3JetsToLNu","1","1")' & #
root -l -b -q ana_rdf.C'("2016","W4JetsToLNu","1","1")' & #
#Zjets
root -l -b -q ana_rdf.C'("2016","DYJetsToLL_M-10to50","1","1")' & #
root -l -b -q ana_rdf.C'("2016","DYJetsToLL_M-50-amcatnloFXFX","1","1")' & #
#single top
root -l -b -q ana_rdf.C'("2016","ST_t-channel_antitop","1","1")' & #
root -l -b -q ana_rdf.C'("2016","ST_tW_antitop","1","1")' & #
root -l -b -q ana_rdf.C'("2016","ST_s-channel_4f_leptonDecays","1","1")' & #
root -l -b -q ana_rdf.C'("2016","ST_t-channel_top","1","1")' & #
root -l -b -q ana_rdf.C'("2016","ST_tW_top","1","1")' & #
#ttX
root -l -b -q ana_rdf.C'("2016","TTWJetsToLNu","1","1")' & #
root -l -b -q ana_rdf.C'("2016","TTWJetsToQQ","1","1")' & #
root -l -b -q ana_rdf.C'("2016","TTZToLLNuNu_M-10","1","1")' & #
root -l -b -q ana_rdf.C'("2016","TTZToQQ_TuneCP5_13TeV","1","1")' & #
root -l -b -q ana_rdf.C'("2016","ttHTobb","1","1")' & #
root -l -b -q ana_rdf.C'("2016","ttHToNonbb","1","1")' & #

#2017
##ttbar
root -l -b -q ana_rdf.C'("2017","TTToSemiLeptonic_ttbb","0","1")'& #
root -l -b -q ana_rdf.C'("2017","TTToSemiLeptonic_ttbj","0","1")'& #
root -l -b -q ana_rdf.C'("2017","TTToSemiLeptonic_ttcc","0","1")'& #
root -l -b -q ana_rdf.C'("2017","TTToSemiLeptonic_ttother","0","1")'& #
root -l -b -q ana_rdf.C'("2017","TTTo2L2Nu","0","1")'& #
root -l -b -q ana_rdf.C'("2017","TTToHadronic","0","1")'& #
#VV
root -l -b -q ana_rdf.C'("2017","WWTo2L2Nu","0","1")' & #
root -l -b -q ana_rdf.C'("2017","WWToLNuQQ","0","1")' & #
root -l -b -q ana_rdf.C'("2017","WZTo2L2Q","0","1")' & #
root -l -b -q ana_rdf.C'("2017","WZTo3LNu","0","1")' & #
root -l -b -q ana_rdf.C'("2017","ZZTo2L2Q","0","1")' & #
#Wjets
root -l -b -q ana_rdf.C'("2017","W1JetsToLNu","0","1")' & #
root -l -b -q ana_rdf.C'("2017","W2JetsToLNu","0","1")' & #
root -l -b -q ana_rdf.C'("2017","W3JetsToLNu","0","1")' & #
root -l -b -q ana_rdf.C'("2017","W4JetsToLNu","0","1")' & #
#Zjets
root -l -b -q ana_rdf.C'("2017","DYJetsToLL_M-10to50","0","1")' & #
root -l -b -q ana_rdf.C'("2017","DYJetsToLL_M-50-amcatnloFXFX","0","1")' & #
#single top
root -l -b -q ana_rdf.C'("2017","ST_t-channel_antitop","0","1")' & #
root -l -b -q ana_rdf.C'("2017","ST_tW_antitop","0","1")' & #
root -l -b -q ana_rdf.C'("2017","ST_s-channel_4f_leptonDecays","0","1")' & #
root -l -b -q ana_rdf.C'("2017","ST_t-channel_top","0","1")' & #
root -l -b -q ana_rdf.C'("2017","ST_tW_top","0","1")' & #
#ttX
root -l -b -q ana_rdf.C'("2017","TTWJetsToLNu","0","1")' & #
root -l -b -q ana_rdf.C'("2017","TTWJetsToQQ","0","1")' & #
root -l -b -q ana_rdf.C'("2017","TTZToLLNuNu_M-10","0","1")' & #
root -l -b -q ana_rdf.C'("2017","TTZToQQ_TuneCP5_13TeV","0","1")' & #
root -l -b -q ana_rdf.C'("2017","ttHTobb","0","1")' & #
root -l -b -q ana_rdf.C'("2017","ttHToNonbb","0","1")' & #
#ttbar
#something ttSL
root -l -b -q ana_rdf.C'("2017","TTToSemiLeptonic_ttbb","1","1")'& #
root -l -b -q ana_rdf.C'("2017","TTToSemiLeptonic_ttbj","1","1")'& #
root -l -b -q ana_rdf.C'("2017","TTToSemiLeptonic_ttcc","1","1")'& #
root -l -b -q ana_rdf.C'("2017","TTToSemiLeptonic_ttother","1","1")'& #
root -l -b -q ana_rdf.C'("2017","TTTo2L2Nu","1","1")'& #
root -l -b -q ana_rdf.C'("2017","TTToHadronic","1","1")'& #
#VV
root -l -b -q ana_rdf.C'("2017","WWTo2L2Nu","1","1")' & #
root -l -b -q ana_rdf.C'("2017","WWToLNuQQ","1","1")' & #
root -l -b -q ana_rdf.C'("2017","WZTo2L2Q","1","1")' & #
root -l -b -q ana_rdf.C'("2017","WZTo3LNu","1","1")' & #
root -l -b -q ana_rdf.C'("2017","ZZTo2L2Q","1","1")' & #
#Wjets
root -l -b -q ana_rdf.C'("2017","W1JetsToLNu","1","1")' & #
root -l -b -q ana_rdf.C'("2017","W2JetsToLNu","1","1")' & #
root -l -b -q ana_rdf.C'("2017","W3JetsToLNu","1","1")' & #
root -l -b -q ana_rdf.C'("2017","W4JetsToLNu","1","1")' & #
#Zjets
root -l -b -q ana_rdf.C'("2017","DYJetsToLL_M-10to50","1","1")' & #
root -l -b -q ana_rdf.C'("2017","DYJetsToLL_M-50-amcatnloFXFX","1","1")' & #
#single top
root -l -b -q ana_rdf.C'("2017","ST_t-channel_antitop","1","1")' & #
root -l -b -q ana_rdf.C'("2017","ST_tW_antitop","1","1")' & #
root -l -b -q ana_rdf.C'("2017","ST_s-channel_4f_leptonDecays","1","1")' & #
root -l -b -q ana_rdf.C'("2017","ST_t-channel_top","1","1")' & #
root -l -b -q ana_rdf.C'("2017","ST_tW_top","1","1")' & #
#ttX
root -l -b -q ana_rdf.C'("2017","TTWJetsToLNu","1","1")' & #
root -l -b -q ana_rdf.C'("2017","TTWJetsToQQ","1","1")' & #
root -l -b -q ana_rdf.C'("2017","TTZToLLNuNu_M-10","1","1")' & #
root -l -b -q ana_rdf.C'("2017","TTZToQQ_TuneCP5_13TeV","1","1")' & #
root -l -b -q ana_rdf.C'("2017","ttHTobb","1","1")' & #
root -l -b -q ana_rdf.C'("2017","ttHToNonbb","1","1")' & #

#2018
#ttbar
root -l -b -q ana_rdf.C'("2018","TTToSemiLeptonic_ttbb","0","1")'& #
root -l -b -q ana_rdf.C'("2018","TTToSemiLeptonic_ttbj","0","1")'& #
root -l -b -q ana_rdf.C'("2018","TTToSemiLeptonic_ttcc","0","1")'& #
root -l -b -q ana_rdf.C'("2018","TTToSemiLeptonic_ttother","0","1")'& #
root -l -b -q ana_rdf.C'("2018","TTTo2L2Nu","0","1")'& #
root -l -b -q ana_rdf.C'("2018","TTToHadronic","0","1")'& #
#VV
root -l -b -q ana_rdf.C'("2018","WWTo2L2Nu","0","1")' & #
root -l -b -q ana_rdf.C'("2018","WWToLNuQQ","0","1")' & #
root -l -b -q ana_rdf.C'("2018","WZTo2L2Q","0","1")' & #
root -l -b -q ana_rdf.C'("2018","WZTo3LNu","0","1")' & #
root -l -b -q ana_rdf.C'("2018","ZZTo2L2Q","0","1")' & #
#Wjets
root -l -b -q ana_rdf.C'("2018","W1JetsToLNu","0","1")' & #
root -l -b -q ana_rdf.C'("2018","W2JetsToLNu","0","1")' & #
root -l -b -q ana_rdf.C'("2018","W3JetsToLNu","0","1")' & #
root -l -b -q ana_rdf.C'("2018","W4JetsToLNu","0","1")' & #
#Zjets
root -l -b -q ana_rdf.C'("2018","DYJetsToLL_M-10to50","0","1")' & #
root -l -b -q ana_rdf.C'("2018","DYJetsToLL_M-50-amcatnloFXFX","0","1")' & #
#single top
root -l -b -q ana_rdf.C'("2018","ST_t-channel_antitop","0","1")' & #
root -l -b -q ana_rdf.C'("2018","ST_tW_antitop","0","1")' & #
root -l -b -q ana_rdf.C'("2018","ST_s-channel_4f_leptonDecays","0","1")' & #
root -l -b -q ana_rdf.C'("2018","ST_t-channel_top","0","1")' & #
root -l -b -q ana_rdf.C'("2018","ST_tW_top","0","1")' & #
#ttX
root -l -b -q ana_rdf.C'("2018","TTWJetsToLNu","0","1")' & #
root -l -b -q ana_rdf.C'("2018","TTWJetsToQQ","0","1")' & #
root -l -b -q ana_rdf.C'("2018","TTZToLLNuNu_M-10","0","1")' & #
root -l -b -q ana_rdf.C'("2018","TTZToQQ_TuneCP5_13TeV","0","1")' & #
root -l -b -q ana_rdf.C'("2018","ttHTobb","0","1")' & #
root -l -b -q ana_rdf.C'("2018","ttHToNonbb","0","1")' & #
#ttbar
#something ttSL
root -l -b -q ana_rdf.C'("2018","TTToSemiLeptonic_ttbb","1","1")'& #
root -l -b -q ana_rdf.C'("2018","TTToSemiLeptonic_ttbj","1","1")'& #
root -l -b -q ana_rdf.C'("2018","TTToSemiLeptonic_ttcc","1","1")'& #
root -l -b -q ana_rdf.C'("2018","TTToSemiLeptonic_ttother","1","1")'& #
root -l -b -q ana_rdf.C'("2018","TTTo2L2Nu","1","1")'& #
root -l -b -q ana_rdf.C'("2018","TTToHadronic","1","1")'& #
#VV
root -l -b -q ana_rdf.C'("2018","WWTo2L2Nu","1","1")' & #
root -l -b -q ana_rdf.C'("2018","WWToLNuQQ","1","1")' & #
root -l -b -q ana_rdf.C'("2018","WZTo2L2Q","1","1")' & #
root -l -b -q ana_rdf.C'("2018","WZTo3LNu","1","1")' & #
root -l -b -q ana_rdf.C'("2018","ZZTo2L2Q","1","1")' & #
#Wjets
root -l -b -q ana_rdf.C'("2018","W1JetsToLNu","1","1")' & #
root -l -b -q ana_rdf.C'("2018","W2JetsToLNu","1","1")' & #
root -l -b -q ana_rdf.C'("2018","W3JetsToLNu","1","1")' & #
root -l -b -q ana_rdf.C'("2018","W4JetsToLNu","1","1")' & #
#Zjets
root -l -b -q ana_rdf.C'("2018","DYJetsToLL_M-10to50","1","1")' & #
root -l -b -q ana_rdf.C'("2018","DYJetsToLL_M-50-amcatnloFXFX","1","1")' & #
#single top
root -l -b -q ana_rdf.C'("2018","ST_t-channel_antitop","1","1")' & #
root -l -b -q ana_rdf.C'("2018","ST_tW_antitop","1","1")' & #
root -l -b -q ana_rdf.C'("2018","ST_s-channel_4f_leptonDecays","1","1")' & #
root -l -b -q ana_rdf.C'("2018","ST_t-channel_top","1","1")' & #
root -l -b -q ana_rdf.C'("2018","ST_tW_top","1","1")' & #
#ttX
root -l -b -q ana_rdf.C'("2018","TTWJetsToLNu","1","1")' & #
root -l -b -q ana_rdf.C'("2018","TTWJetsToQQ","1","1")' & #
root -l -b -q ana_rdf.C'("2018","TTZToLLNuNu_M-10","1","1")' & #
root -l -b -q ana_rdf.C'("2018","TTZToQQ_TuneCP5_13TeV","1","1")' & #
root -l -b -q ana_rdf.C'("2018","ttHTobb","1","1")' & #
root -l -b -q ana_rdf.C'("2018","ttHToNonbb","1","1")' & #
