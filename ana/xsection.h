#include <string>
#include <iostream>
#include <map>
#include "TString.h"

double lumi = 59740;

const std::map<TString, double> Xsections = {

  {"TTLJ_PowhegPythia_ttbb",    365.34},
  {"TTLJ_PowhegPythia_ttbj",    365.34},
  {"TTLJ_PowhegPythia_ttcc",    365.34},
  {"TTLJ_PowhegPythia_ttLF",    365.34},
  {"TTLJ_PowhegPythia_ttother", 365.34},
  {"SingleTbar_tW_PowhegPythia", 35.85};
  {"SingleTbar_t_PowhegPythia",  80.95};
  {"SingleTop_s_aMCatNLOPythia", 3.36 };
  {"SingleTop_tW_PowhegPythia",  35.85};
  {"SingleTop_t_PowhegPythia",   36.02};
  {"TTJJ_PowhegPythiaBkg",      377.96};
  {"TTLJ_PowhegPythiaBkg",      365.34};
  {"TTLJ_PowhegPythia_ttLF",    365.34};
  {"TTLJ_PowhegPythia_ttbb",   
  {"TTLJ_PowhegPythia_ttbj",
  {"TTLJ_PowhegPythia_ttcc",
  {"TTLJ_PowhegPythia_ttother",
  {"TTLL_PowhegPythiaBkg
  {"W1JetsToLNu_MadgraphPythia
  {"W2JetsToLNu_MadgraphPythia
  {"W3JetsToLNu_MadgraphPythia
  {"W4JetsToLNu_MadgraphPythia
  {"WJetsToLNu_MadgraphPythia
  {"WW_Pythia
  {"WZ_Pythia
  {"ZJets_M50_aMCatNLOPythia
  {"ZZ_Pythia
  {"ttHToNonbb_PowhegPythia
  {"ttHTobb_PowhegPythia
  {"ttWToQQ_aMCatNLOMadspinPythia
  {"ttZToLLNuNu_aMCatNLOMadspinPythia
  {"ttZToQQ_aMCatNLOMadspinPythia
  {"ZJets_M10to50_MadgraphPythia
  {"ttWToLNu_aMCatNLOMadspinPythia
  {"QCD_Pt-1000toInf_MuEnriched
  {"QCD_Pt-120to170_EMEnriched
  {"QCD_Pt-120to170_MuEnriched
  {"QCD_Pt-15to20_EMEnriched
  {"QCD_Pt-15to20_MuEnriched
  {"QCD_Pt-170to300_EMEnriched
  {"QCD_Pt-170to300_MuEnriched
  {"QCD_Pt-20to30_EMEnriched
  {"QCD_Pt-20to30_MuEnriched
  {"QCD_Pt-300to470_MuEnriched
  {"QCD_Pt-300toInf_EMEnriched
  {"QCD_Pt-30to50_EMEnriched
  {"QCD_Pt-470to600_MuEnriched
  {"QCD_Pt-50to80_EMEnriched
  {"QCD_Pt-50to80_MuEnriched
  {"QCD_Pt-600to800_MuEnriched
  {"QCD_Pt-80to120_EMEnriched
  {"QCD_Pt-80to120_MuEnriched  

 

};

//double n_event= ;

/*
DYJets_10to50                                    
DYJets
WJetsToLNu
W1JetsToLNu
W2JetsToLNu
W3JetsToLNu
W4JetsToLNu
TTTo2L2Nu
TTTo2L2Nu_hdampUP
TTTo2L2Nu_hdampDOWN
TTTo2L2Nu_TuneCP5up
TTTo2L2Nu_TuneCP5down
TTLJ_powheg
TTLJ_powheg_hdampUP
TTLJ_powheg_hdampDOWN
TTLJ_powheg_TuneCP5up
TTLJ_powheg_TuneCP5down
TTToHadronic
TTToHadronic_hdampUP
TTToHadronic_hdampDOWN
TTToHadronic_TuneCP5up
TTToHadronic_TuneCP5down
TTLL_HEM
TTLJ_HEM
SingleTop_s
SingleTop_t
SingleTbar_t
SingleTop_tW
SingleTbar_tW
WW
WZ
ZZ
TTWJetsToLNu
TTWJetsToQQ
TTZToLLNuNu
TTZToQQ
ttHTobb
ttHToNonbb
ST_TH_1L3B_Hut
ST_TH_1L3B_Hct
TT_TH_1L3B_aTLep_Hut
TT_TH_1L3B_TLep_Hut
TT_TH_1L3B_aTLep_Hct
TT_TH_1L3B_TLep_Hct
QCD_Pt-15to20_MuEnriched
QCD_Pt-20to30_MuEnriched
QCD_Pt-30to50_MuEnriched
QCD_Pt-50to80_MuEnriched
QCD_Pt-80to120_MuEnriched
QCD_Pt-120to170_MuEnriched
QCD_Pt-170to300_MuEnriched
QCD_Pt-300to470_MuEnriched
QCD_Pt-470to600_MuEnriched
QCD_Pt-600to800_MuEnriched
QCD_Pt-800to1000_MuEnriched
QCD_Pt-1000toInf_MuEnriched
QCD_Pt-15to20_EMEnriched
QCD_Pt-20to30_EMEnriched
QCD_Pt-30to50_EMEnriched
QCD_Pt-50to80_EMEnriched
QCD_Pt-80to120_EMEnriched
QCD_Pt-120to170_EMEnriched
QCD_Pt-170to300_EMEnriched
QCD_Pt-300toInf_EMEnriched
*/
