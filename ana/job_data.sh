#Producing histogram
#options
#root -l -b -q ana_rdf.C'("year","sample","channel","dnntree saving")' 

#2016
#SingleElectron
root -l -b -q ana_rdf.C'("2016","SingleElectron2016B_ver2","1","1")' & 
root -l -b -q ana_rdf.C'("2016","SingleElectron2016C","1","1")' & 
root -l -b -q ana_rdf.C'("2016","SingleElectron2016D","1","1")' & 
root -l -b -q ana_rdf.C'("2016","SingleElectron2016E","1","1")' & 
root -l -b -q ana_rdf.C'("2016","SingleElectron2016F","1","1")' & 
root -l -b -q ana_rdf.C'("2016","SingleElectron2016G","1","1")' & 
root -l -b -q ana_rdf.C'("2016","SingleElectron2016H","1","1")' & 
root -l -b -q ana_rdf.C'("2016","SingleMuon2016B_ver2","0","1")' &
#SingleMuon
root -l -b -q ana_rdf.C'("2016","SingleMuon2016C","0","1")' & 
root -l -b -q ana_rdf.C'("2016","SingleMuon2016D","0","1")' & 
root -l -b -q ana_rdf.C'("2016","SingleMuon2016E","0","1")' & 
root -l -b -q ana_rdf.C'("2016","SingleMuon2016F","0","1")' & 
root -l -b -q ana_rdf.C'("2016","SingleMuon2016G","0","1")' & 
root -l -b -q ana_rdf.C'("2016","SingleMuon2016H","0","1")' & 

#2017
#SingleElectron
root -l -b -q ana_rdf.C'("2017","SingleElectron2017B","1","1")' & 
root -l -b -q ana_rdf.C'("2017","SingleElectron2017C","1","1")' & 
root -l -b -q ana_rdf.C'("2017","SingleElectron2017D","1","1")' & 
root -l -b -q ana_rdf.C'("2017","SingleElectron2017E","1","1")' & 
root -l -b -q ana_rdf.C'("2017","SingleElectron2017F","1","1")' & 
#SingleMuon
root -l -b -q ana_rdf.C'("2017","SingleMuon2017B","0","1")' & 
root -l -b -q ana_rdf.C'("2017","SingleMuon2017C","0","1")' & 
root -l -b -q ana_rdf.C'("2017","SingleMuon2017D","0","1")' & 
root -l -b -q ana_rdf.C'("2017","SingleMuon2017E","0","1")' & 
root -l -b -q ana_rdf.C'("2017","SingleMuon2017F","0","1")' & 

#2018
#EGamma
root -l -b -q ana_rdf.C'("2018","EGamma2018A","1","1")' & 
root -l -b -q ana_rdf.C'("2018","EGamma2018B","1","1")' & 
root -l -b -q ana_rdf.C'("2018","EGamma2018C","1","1")' & 
root -l -b -q ana_rdf.C'("2018","EGamma2018D","1","1")' &
#SingleMuon
root -l -b -q ana_rdf.C'("2018","SingleMuon2018A","0","1")' & 
root -l -b -q ana_rdf.C'("2018","SingleMuon2018B","0","1")' & 
root -l -b -q ana_rdf.C'("2018","SingleMuon2018C","0","1")' & 
root -l -b -q ana_rdf.C'("2018","SingleMuon2018D","0","1")' & 
