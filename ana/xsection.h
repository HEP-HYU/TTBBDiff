#include <string>
#include <iostream>
#include <map>
#include "TString.h"

double lumi = 59740.;

const std::map<TString, double> Xsections = {

  {"TTLJ_PowhegPythia_ttbb",           365.34},
  {"TTLJ_PowhegPythia_ttbj",           365.34},
  {"TTLJ_PowhegPythia_ttcc",           365.34},
  {"TTLJ_PowhegPythia_ttLF",           365.34},
  {"TTLJ_PowhegPythia_ttother",        365.34},
  {"SingleTbar_tW_PowhegPythia",       35.85 };
  {"SingleTbar_t_PowhegPythia",        80.95 };
  {"SingleTop_s_aMCatNLOPythia",       3.36  };
  {"SingleTop_tW_PowhegPythia",        35.85 };
  {"SingleTop_t_PowhegPythia",         36.02 };
  {"TTJJ_PowhegPythiaBkg",             377.96};
  {"TTLJ_PowhegPythiaBkg",             365.34};
  {"TTLJ_PowhegPythia_ttLF",           365.34};
  {"TTLJ_PowhegPythia_ttbb",           365.34};  
  {"TTLJ_PowhegPythia_ttbj",           365.34};
  {"TTLJ_PowhegPythia_ttcc",           365.34};
  {"TTLJ_PowhegPythia_ttother",        365.34};
  {"TTLL_PowhegPythiaBkg",             88.29 };
  {"W1JetsToLNu_MadgraphPythia",       9625  };
  {"W2JetsToLNu_MadgraphPythia",       2793  };
  {"W3JetsToLNu_MadgraphPythia",       992.5 };
  {"W4JetsToLNu_MadgraphPythia",       544.3 };
  {"WJetsToLNu_MadgraphPythia",        61526 };
  {"WW_Pythia",                        118.7 };
  {"WZ_Pythia",                        47.13 };
  {"ZJets_M50_aMCatNLOPythia",         6225.42};
  {"ZZ_Pythia",                        16.523};
  {"ttHToNonbb_PowhegPythia",          0.2151};
  {"ttHTobb_PowhegPythia",             0.2934};
  {"ttWToQQ_aMCatNLOMadspinPythia",    0.4062};
  {"ttZToLLNuNu_aMCatNLOMadspinPythia",0.2529};
  {"ttZToQQ_aMCatNLOMadspinPythia",    0.5297};
  {"ZJets_M10to50_MadgraphPythia",     18610 };
  {"ttWToLNu_aMCatNLOMadspinPythia",   0.2043};
  {"QCD_Pt-15to20_MuEnriched",         0.1697}; 
  {"QCD_Pt-20to30_MuEnriched",         1.2   }; 
  {"QCD_Pt-30to50_MuEnriched",         1.2   };
  {"QCD_Pt-50to80_MuEnriched",         0.8462};   
  {"QCD_Pt-80to120_MuEnriched",        1.1   };    
  {"QCD_Pt-120to170_MuEnriched",       0.9869};       
  {"QCD_Pt-170to300_MuEnriched",       1.8   };  
  {"QCD_Pt-300to470_MuEnriched",       1.6   };      
  {"QCD_Pt-470to600_MuEnriched",       1.2   };    
  {"QCD_Pt-600to800_MuEnriched",       0.9939};   
  {"QCD_Pt-800to1000_MuEnriched",      1     };  
  {"QCD_Pt-1000toInf_MuEnriched",      0.6838};       
  {"QCD_Pt-15to20_EMEnriched",         0.5152};           
  {"QCD_Pt-20to30_EMEnriched",         0.5147};            
  {"QCD_Pt-30to50_EMEnriched",         0.5621};           
  {"QCD_Pt-50to80_EMEnriched",         0.4273};              
  {"QCD_Pt-80to120_EMEnriched",        0.4076};              
  {"QCD_Pt-120to170_EMEnriched",       0.4424};            
  {"QCD_Pt-170to300_EMEnriched",       0.1756};            
  {"QCD_Pt-300toInf_EMEnriched",       0.1482};                     
};

//double n_event= ;

/*
TTTo2L2Nu_hdampUP
TTTo2L2Nu_hdampDOWN
TTTo2L2Nu_TuneCP5up
TTTo2L2Nu_TuneCP5down
TTLJ_powheg_hdampUP
TTLJ_powheg_hdampDOWN
TTLJ_powheg_TuneCP5up
TTLJ_powheg_TuneCP5down
TTToHadronic_hdampUP
TTToHadronic_hdampDOWN
TTToHadronic_TuneCP5up
TTToHadronic_TuneCP5down
TTLL_HEM
TTLJ_HEM
*/
