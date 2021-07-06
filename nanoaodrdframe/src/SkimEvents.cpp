/*
 * SkimEvents.cpp
 *
 *  Created on: Dec 9, 2018
 *      Author: suyong
 */

#include "SkimEvents.h"
#include "utility.h"

SkimEvents::SkimEvents(TTree *t, std::string outfilename, std::string cat, std::string year, std::string jsonfname, string globaltag, int nthreads)
:NanoAODAnalyzerrdframe(t, outfilename, cat, year, jsonfname, globaltag, nthreads),_cat(cat),_year(year)
{

}

// Define your cuts here
void SkimEvents::defineCuts()
{
	// Cuts to be applied in order
	// These will be passed to Filter method of RDF
	// check for good json event is defined earlier
        std::cout<<"YEAR "<<_year<<std::endl;

        //TODO
        // one Lepton Selection(without Trigger)
        //addCuts("( (nmuonpass == 1 && nvetomuons == 1 && (nelepass + nvetoeles) == 0 ) || (nelepass == 1 && nvetoeles == 1 && (nmuonpass + nvetomuons) == 0 ) )","0");
        //MET Filter
        //sync flag

        bool isWJetincl = false;
        if(isWJetincl){
                addCuts("LHE_HT <= 100","0");
        }
        else{ 
                if(_year.find("2016") != std::string::npos){
                        //if(_sync){
                            //addCuts("run == 274335 && (luminosityBlock >= 90 && luminosityBlock <= 94)","0"); // syncDATA
                            //addCuts("luminosityBlock <= 79","0"); // syncMC
                            //addCuts("(is_mu || is_e)","00");
                        //}
                        addCuts("( (HLT_IsoMu24 || HLT_IsoTkMu24) && (nmuonpass == 1 && nvetomuons == 1 && (nelepass + nvetoeles) == 0) ) || ( HLT_Ele27_WPTight_Gsf && (nelepass == 1 && nvetoeles == 1 && (nmuonpass + nvetomuons) == 0) )","0"); // MC
                        //addCuts("HLT_IsoMu24 || HLT_IsoTkMu24","0"); // SingleMuon
                        //addCuts("nmuonpass == 1 && nvetomuons == 1 && (nelepass + nvetoeles == 0)","00"); // SingleMuon
                        //addCuts("HLT_Ele27_WPTight_Gsf","0"); // SingleElectron
                        //addCuts("nelepass == 1 && nvetoeles == 1 && (nmuonpass + nvetomuons == 0)","00"); // SingleElectron
                }
                else if(_year == "2017"){
                        //if(_sync){
                            //addCuts("run == 297503 && (luminosityBlock >= 5 && luminosityBlock <= 8)","0"); // syncDATA
                            //addCuts("luminosityBlock <= 19","0"); // syncMC
                            //addCuts("(is_mu || is_e)","00");
                        //}
                        addCuts("( nmuonpass == 1 && nvetomuons == 1 && (nelepass + nvetoeles) == 0 ) || ( nelepass == 1 && nvetoeles == 1 && (nmuonpass + nvetomuons) == 0 )","0"); // MC
                        addCuts("(channel == 0 && HLT_IsoMu27) || ( channel == 1 && ((HLT_Ele32_WPTight_Gsf_L1DoubleEG && flag_Trig) || HLT_Ele28_eta2p1_WPTight_Gsf_HT150) )","00"); // MC
                        //addCuts("HLT_IsoMu27","0"); // SingleMuon
                        //addCuts("nmuonpass == 1 && nvetomuons == 1 && (nelepass + nvetoeles == 0)","00"); // SingleMuon
                        //addCuts("nelepass == 1 && nvetoeles == 1 && (nmuonpass + nvetomuons == 0)","0"); // SingleElectron
                        //addCuts("( HLT_Ele32_WPTight_Gsf_L1DoubleEG && flag_Trig ) || HLT_Ele28_eta2p1_WPTight_Gsf_HT150","00"); // SingleElectron
                }
                else if(_year == "2018"){
                        //if(_sync){
                            //addCuts("run == 316060 && (luminosityBlock >= 1 && luminosityBlock <= 6)","0"); // syncDATA
                            //addCuts("luminosityBlock <= 4","0"); // syncMC
                            //addCuts("(is_mu || is_e)","00");
                        //}
                        //addCuts("( nmuonpass == 1 && nvetomuons == 1 && (nelepass + nvetoeles) == 0 ) || ( nelepass == 1 && nvetoeles == 1 && (nmuonpass + nvetomuons) == 0 )","0"); // MC
                        //addCuts("( channel == 0 && HLT_IsoMu24 ) || ( channel == 1 && (HLT_Ele32_WPTight_Gsf || HLT_Ele28_eta2p1_WPTight_Gsf_HT150) )","00"); // MC
                        addCuts("( HLT_IsoMu24 && (nmuonpass == 1 && nvetomuons == 1 && (nelepass + nvetoeles) == 0) ) || ( (HLT_Ele32_WPTight_Gsf || HLT_Ele28_eta2p1_WPTight_Gsf_HT150) && (nelepass == 1 && nvetoeles == 1 && (nmuonpass + nvetomuons) == 0) )","0"); // MC
                        //addCuts("HLT_IsoMu24","0"); // SingleMuon
                        //addCuts("nmuonpass == 1 && nvetomuons == 1 && (nelepass + nvetoeles == 0)","00"); // SingleMuon
                        //addCuts("HLT_Ele32_WPTight_Gsf || HLT_Ele28_eta2p1_WPTight_Gsf_HT150","0"); // EGamma
                        //addCuts("nelepass == 1 && nvetoeles == 1 && (nmuonpass + nvetomuons == 0)","00"); // EGamma
                }

        }
      
        if(_cat == "ttbb"){
                std::cout<<"is TTBB"<<std::endl;
                addCuts("isttbb","000");
        }
        else if(_cat == "ttbj"){
                addCuts("isttbj","000");
        }
        else if(_cat == "ttcc"){
                addCuts("isttcc","000");
        }
        else if(_cat == "ttLF"){
                addCuts("isttLF","000");
        }
        else if(_cat == "ttother"){
                addCuts("isttother","000");
        }
        else cout << "cat condition is not defined" << endl;
}

void SkimEvents::defineMoreVars()
{
	//addVar({"Sel_jetHT", "Sum(Sel_jetpt)"});

  //only for ttSL skimming
	//addVartoStore("run");
	//addVartoStore("luminosityBlock");
	//addVartoStore("event");
	//addVartoStore("gen.*");
	//addVartoStore("Pileup.*");
  //if(_year == "2016" ){ 
  //                      addVartoStore("HLT_IsoMu24"); addVartoStore("HLT_IsoTkMu24");
  //                      addVartoStore("HLT_Ele27_WPTight_Gsf");
  //}
  //if(_year == "2017"){  
  //                      addVartoStore("HLT_IsoMu27");
  //                      addVartoStore("HLT_Ele32_WPTight_Gsf_L1DoubleEG"); addVartoStore("HLT_Ele28_eta2p1_WPTight_Gsf_HT150");
  //                      addVartoStore("nTrigObj"); addVartoStore("TrigObj_.*");
  //}
  //if(_year == "2018"){
  //                      addVartoStore("HLT_IsoMu24");
  //                      addVartoStore("HLT_Ele32_WPTight_Gsf"); addVartoStore("HLT_Ele28_eta2p1_WPTight_Gsf_HT150");
  //}
	//addVartoStore("nElectron"); 
	//addVartoStore("Electron_.*");
	//addVartoStore("nMuon");
	//addVartoStore("Muon_.*");
	//addVartoStore("nJet");
	//addVartoStore("Jet_.*");
	//addVartoStore("nGenJet");
  //addVartoStore("GenJet_.*");
  //addVartoStore("nGenDressedLepton");
  //addVartoStore("GenDressedLepton_.*");
  //addVartoStore("nGenPart");
  //addVartoStore("GenPart_.*");

	//define variables that you want to store
  //for making ntuple
	addVartoStore("run");
	addVartoStore("luminosityBlock");
	addVartoStore("event");
	addVartoStore("evWeight");
	addVartoStore("evWeight_leptonSF");
	addVartoStore("puWeight.*");
	addVartoStore("regenWeight.*");
	addVartoStore("pugenWeight.*");
	addVartoStore("btagWeight.*");
	addVartoStore("gen.*");
	addVartoStore("Pileup.*");

  addVartoStore("channel");
  addVartoStore("lepton_.*");
  addVartoStore("jet_.*");
  addVartoStore("bjet_.*");

  //of course
	addVartoStore("MET.*");
	//addVartoStore("LHE_.*"); // WJet HT sample

  //should I?
	//addVartoStore("PV.*");
	//addVartoStore("nOtherPV");
	//addVartoStore("OtherPV_z");
	//addVartoStore("flags");
  //for MET Filter
	addVartoStore("Flag_goodVertices");
	addVartoStore("Flag_globalSuperTightHalo2016Filter");
	addVartoStore("Flag_HBHENoiseFilter");
	addVartoStore("Flag_HBHENoiseIsoFilter");
	addVartoStore("Flag_EcalDeadCellTriggerPrimitiveFilter");
	addVartoStore("Flag_BadPFMuonFilter");
	addVartoStore("Flag_eeBadScFilter");
	if(_year == "2017" || _year == "2018") addVartoStore("Flag_ecalBadCalibFilterV2");

  //for ttbb
  if(_cat == "ttbb"){
      addVartoStore("p4_addbjets");
      addVartoStore("addbjet1_.*");
      addVartoStore("addbjet2_.*");
  }

  //for sync
  //if(_sync) TODO
	addVartoStore("run"); 
	addVartoStore("luminosityBlock");
	addVartoStore("event");
	addVartoStore("is_e");
	addVartoStore("is_mu");
	addVartoStore("flags");
	addVartoStore("trigger_e");
	addVartoStore("trigger_mu");
	addVartoStore("lepton_pt");
	addVartoStore("lepton_eta");
	addVartoStore("lepton_deltaEtaSC");
  addVartoStore("lepton_phi");
  addVartoStore("lepton_abs_dxy");
  addVartoStore("lepton_abs_dz");
  addVartoStore("lepton_iso");
  addVartoStore("nJets");
  addVartoStore("jet1_pt");
  addVartoStore("jet1_eta");
  addVartoStore("jet1_phi");
  addVartoStore("jet1_deepJet");
  addVartoStore("jet2_pt");
  addVartoStore("jet2_eta");
  addVartoStore("jet2_phi");
  addVartoStore("jet2_deepJet");
  addVartoStore("nBJets_deepJetM");

  //for syst
  addVartoStore("fixedGridRhoFastjet*"); //for JEC v2

}

void SkimEvents::bookHists()
{
	// _hist1dinfovector contains the information of histogram definitions (as TH1DModel)
	// the variable to be used for filling
	// and the minimum cutstep for which the histogram should be filled
	//
	// The braces are used to initalize the struct
	// TH1D
        add1DHist( {"hcounter",  "Event counter", 2, -0.5, 1.5}, "one", "one", "");
        add1DHist( {"hcounter1", "Event counter", 2, -0.5, 1.5}, "one", "regenWeight", "");
        add1DHist( {"hcounter2", "Event counter", 2, -0.5, 1.5}, "one", "pugenWeight", "");
}
