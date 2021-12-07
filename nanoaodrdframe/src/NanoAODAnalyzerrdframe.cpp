/*
 * NanoAODAnalyzerrdframe.cpp
 *
 *  Created on: Sep 30, 2018
 *      Author: suyong
 */

#include "NanoAODAnalyzerrdframe.h"
#include <iostream>
#include <algorithm>
#include <typeinfo>

#include "TCanvas.h"
#include "Math/GenVector/VectorUtil.h"
#include <vector>
#include <fstream>
#include "utility.h"
#include <regex>
#include "ROOT/RDFHelpers.hxx"


using namespace std;

NanoAODAnalyzerrdframe::NanoAODAnalyzerrdframe(TTree *atree, std::string outfilename, std::string cat, std::string year, std::string jsonfname, std::string globaltag, int nthreads)
:_rd(*atree),_jsonOK(false), _outfilename(outfilename), _cat(cat), _year(year), _jsonfname(jsonfname), _globaltag(globaltag), _inrootfile(0),_outrootfile(0), _rlm(_rd)
        , _btagcalibreader(BTagEntry::OP_RESHAPING, "central", {"up_jes", "down_jes"})
        , _rnt(&_rlm), currentnode(0), _jetCorrector(0), _jetCorrectionUncertainty(0)
{
        //
        // if genWeight column exists, then it is not real data
        //
        if (atree->GetBranch("genWeight") == nullptr) {
                _isData = true;
                cout << "input file is data" <<endl;
        }
        else
        {
                _isData = false;
                cout << "input file is MC" <<endl;
        }
        TObjArray *allbranches = atree->GetListOfBranches();
        for (int i =0; i<allbranches->GetSize(); i++)
        {
                TBranch *abranch = dynamic_cast<TBranch *>(allbranches->At(i));
                if (abranch!= nullptr){
                        cout << abranch->GetName() << endl;
                        _originalvars.push_back(abranch->GetName());
                }
        }
        
        cout<<"Run "<<_year<<endl;
        if(_year.find("2016") != std::string::npos){
                _btagcalib = {"DeepJet", "data/btag/DeepJet_2016LegacySF_V1_TuneCP5.csv"};
        }
        else if(_year == "2017"){
                _btagcalib = {"DeepJet", "data/btag/DeepJet_106XUL17SF_V2p1.csv"};
        }
        else if(_year == "2018"){
                _btagcalib = {"DeepJet", "data/btag/DeepJet_106XUL18SF.csv"};
        }

        // load the formulae b flavor tagging
        _btagcalibreader.load(_btagcalib, BTagEntry::FLAV_B, "iterativefit");
        _btagcalibreader.load(_btagcalib, BTagEntry::FLAV_C, "iterativefit");
        _btagcalibreader.load(_btagcalib, BTagEntry::FLAV_UDSG, "iterativefit");

        if(_year == "2016preVFP"){
                pumcfile = "data/pileup/PileupMC_UL16.root";
                pudatafile = "data/pileup/PileupDATA_UL16pre.root";
        }
        else if(_year == "2016postVFP"){
                pumcfile = "data/pileup/PileupMC_UL16.root";
                pudatafile = "data/pileup/PileupDATA_UL16post.root";
        }
        else if(_year == "2017"){
                pumcfile = "data/pileup/PileupMC_UL17.root";
                pudatafile = "data/pileup/PileupDATA_UL17.root";
        }
        else if(_year == "2018"){
                pumcfile = "data/pileup/PileupMC_UL18.root";
                pudatafile = "data/pileup/PileupDATA_UL18.root";
        }
        cout << "Loading Pileup profiles" << endl;
        TFile tfmc(pumcfile);
        _hpumc = dynamic_cast<TH1D *>(tfmc.Get("pu_mc"));
        _hpumc->SetDirectory(0);
        tfmc.Close();

        TFile tfdata(pudatafile);
        _hpudata = dynamic_cast<TH1D *>(tfdata.Get("pileup"));
        _hpudata_plus = dynamic_cast<TH1D *>(tfdata.Get("pileup_plus"));
        _hpudata_minus = dynamic_cast<TH1D *>(tfdata.Get("pileup_minus"));

        _hpudata->SetDirectory(0);
        _hpudata_plus->SetDirectory(0);
        _hpudata_minus->SetDirectory(0);
        tfdata.Close();


        _puweightcalc = new WeightCalculatorFromHistogram(_hpumc, _hpudata);
        _puweightcalc_plus = new WeightCalculatorFromHistogram(_hpumc, _hpudata_plus);
        _puweightcalc_minus = new WeightCalculatorFromHistogram(_hpumc, _hpudata_minus);

        setupJetMETCorrection(_globaltag);
        /*
         *  The following doesn't work yet...
        if (nthreads>1)
        {
                ROOT::EnableImplicitMT(nthreads);
        }
        */

        // Loading Lepton Scale Factor
        // Loading Muon Scale Factor
        cout<<"Loading Muon SF"<<endl;
        if(_year == "2016preVFP"){
                TFile muontrg("data/MuonSF/UL2016_preVFP/Efficiencies_muon_generalTracks_Z_Run2016_UL_HIPM_SingleMuonTriggers.root");
                _hmuontrg = dynamic_cast<TH2F *>(muontrg.Get("NUM_IsoMu24_or_IsoTkMu24_DEN_CutBasedIdTight_and_PFIsoTight_abseta_pt"));
                _hmuontrg->SetDirectory(0);
                muontrg.Close();

                TFile muonid("data/MuonSF/UL2016_preVFP/Efficiencies_muon_generalTracks_Z_Run2016_UL_HIPM_ID.root");
                _hmuonid = dynamic_cast<TH2F *>(muonid.Get("NUM_TightID_DEN_TrackerMuons_abseta_pt"));
                _hmuonid->SetDirectory(0);
                muonid.Close();

                TFile muoniso("data/MuonSF/UL2016_preVFP/Efficiencies_muon_generalTracks_Z_Run2016_UL_HIPM_ISO.root");
                _hmuoniso = dynamic_cast<TH2F *>(muoniso.Get("NUM_TightRelTkIso_DEN_HighPtIDandIPCut_abseta_pt"));
                _hmuoniso->SetDirectory(0);
                muoniso.Close();

                _muontrg = new WeightCalculatorFromHistogram(_hmuontrg);
                _muonid = new WeightCalculatorFromHistogram(_hmuonid);
                _muoniso = new WeightCalculatorFromHistogram(_hmuoniso);

        }
        else if((_year == "2016postVFP")){
                TFile muontrg("data/MuonSF/UL2016_postVFP/Efficiencies_muon_generalTracks_Z_Run2016_UL_SingleMuonTriggers.root");
                _hmuontrg = dynamic_cast<TH2F *>(muontrg.Get("NUM_IsoMu24_or_IsoTkMu24_DEN_CutBasedIdTight_and_PFIsoTight_abseta_pt"));
                _hmuontrg->SetDirectory(0);
                muontrg.Close();

                TFile muonid("data/MuonSF/UL2016_postVFP/Efficiencies_muon_generalTracks_Z_Run2016_UL_ID.root");
                _hmuonid = dynamic_cast<TH2F *>(muonid.Get("NUM_TightID_DEN_TrackerMuons_abseta_pt"));
                _hmuonid->SetDirectory(0);
                muonid.Close();

                TFile muoniso("data/MuonSF/UL2016_postVFP/Efficiencies_muon_generalTracks_Z_Run2016_UL_ISO.root");
                _hmuoniso = dynamic_cast<TH2F *>(muoniso.Get("NUM_TightRelTkIso_DEN_HighPtIDandIPCut_abseta_pt"));
                _hmuoniso->SetDirectory(0);
                muoniso.Close();

                _muontrg = new WeightCalculatorFromHistogram(_hmuontrg);
                _muonid = new WeightCalculatorFromHistogram(_hmuonid);
                _muoniso = new WeightCalculatorFromHistogram(_hmuoniso);
        
        }
        else if(_year == "2017"){
                TFile muontrg("data/MuonSF/UL2017/Efficiencies_muon_generalTracks_Z_Run2017_UL_SingleMuonTriggers.root");
                _hmuontrg = dynamic_cast<TH2F *>(muontrg.Get("NUM_IsoMu27_DEN_CutBasedIdTight_and_PFIsoTight_abseta_pt"));
                _hmuontrg->SetDirectory(0);
                muontrg.Close();

                TFile muonid("data/MuonSF/UL2017/Efficiencies_muon_generalTracks_Z_Run2017_UL_ID.root");
                _hmuonid = dynamic_cast<TH2F *>(muonid.Get("NUM_TightID_DEN_TrackerMuons_abseta_pt"));
                _hmuonid->SetDirectory(0);
                muonid.Close();

                TFile muoniso("data/MuonSF/UL2017/Efficiencies_muon_generalTracks_Z_Run2017_UL_ISO.root");
                _hmuoniso = dynamic_cast<TH2F *>(muoniso.Get("NUM_TightRelTkIso_DEN_HighPtIDandIPCut_abseta_pt"));
                _hmuoniso->SetDirectory(0);
                muoniso.Close();

                _muontrg = new WeightCalculatorFromHistogram(_hmuontrg);
                _muonid = new WeightCalculatorFromHistogram(_hmuonid);
                _muoniso = new WeightCalculatorFromHistogram(_hmuoniso);
         
                //cout<<"Loading Electron SF"<<endl;
                //Electron Zvtx / prefiring 
        }
        else if(_year == "2018"){
                
                cout<<"Loading Muon SF 2018"<<endl;

                TFile muontrg("data/MuonSF/UL2018/Efficiencies_muon_generalTracks_Z_Run2018_UL_SingleMuonTriggers.root");
                _hmuontrg = dynamic_cast<TH2F *>(muontrg.Get("NUM_IsoMu24_DEN_CutBasedIdTight_and_PFIsoTight_abseta_pt"));
                _hmuontrg->SetDirectory(0);
                muontrg.Close();

                TFile muonid("data/MuonSF/UL2018/Efficiencies_muon_generalTracks_Z_Run2018_UL_ID.root");
                _hmuonid = dynamic_cast<TH2F *>(muonid.Get("NUM_TightID_DEN_TrackerMuons_abseta_pt"));
                _hmuonid->SetDirectory(0);
                muonid.Close();

                TFile muoniso("data/MuonSF/UL2018/Efficiencies_muon_generalTracks_Z_Run2018_UL_ISO.root");
                _hmuoniso = dynamic_cast<TH2F *>(muoniso.Get("NUM_TightRelTkIso_DEN_HighPtIDandIPCut_abseta_pt"));
                _hmuoniso->SetDirectory(0);
                muoniso.Close();

                _muontrg = new WeightCalculatorFromHistogram(_hmuontrg);
                _muonid = new WeightCalculatorFromHistogram(_hmuonid);
                _muoniso = new WeightCalculatorFromHistogram(_hmuoniso);

                //cout<<"Loading Electron SF"<<endl;
                ////Electron -> Id/Reco/Zvtx/Trigger

        }

}

NanoAODAnalyzerrdframe::~NanoAODAnalyzerrdframe() {
        // TODO Auto-generated destructor stub
        // ugly...
        
        cout << "writing histograms" << endl;
        for (auto afile:_outrootfilenames)
        {
                _outrootfile = new TFile(afile.c_str(), "UPDATE");
                double nevt1 = -1;
                double nevt2 = -1;
                double nevt3 = -1;
                for (auto &h : _th1dhistos)
                {
                        if (h.second.GetPtr() != nullptr) h.second->Write();
                        if(!isDefined("regenWeight")){
                                if (h.first == "hcounter1_nocut") nevt1 = h.second->GetBinContent(2);
                                if (h.first == "hcounter2_nocut") nevt2 = h.second->GetBinContent(2);
                                if (h.first == "hcounter3_nocut") nevt3 = h.second->GetBinContent(2);
                        }
                }
                if(!isDefined("regenWeight")){
                        TH1D *hnEvents = new TH1D("hnEvents", "Number of Events", 4, 0, 4); 
                        hnEvents->Fill(0.5,nevt1);
                        hnEvents->Fill(1.5,nevt2);
                        hnEvents->Fill(2.5,nevt3);
                        hnEvents->Write();
                }
                _outrootfile->Write(0, TObject::kOverwrite);
                _outrootfile->Close();
                delete _outrootfile;
        }
}

bool NanoAODAnalyzerrdframe::isDefined(string v)
{
        auto result = std::find(_originalvars.begin(), _originalvars.end(), v);
        if (result != _originalvars.end()) return true;
        else return false;
}

void NanoAODAnalyzerrdframe::setTree(TTree *t, std::string outfilename)
{
        _rd = ROOT::RDataFrame(*t);
        _rlm = RNode(_rd);
        _outfilename = outfilename;
        _hist1dinfovector.clear();
        _th1dhistos.clear();
        _varstostore.clear();
        _hist1dinfovector.clear();
        _selections.clear();
        
        this->setupAnalysis();
}

void NanoAODAnalyzerrdframe::setupAnalysis()
{
        /* Must sequentially define objects.
         *
         */

        if (_isData) _jsonOK = readjson();
        // Event weight for data it's always one. For MC, it depends on the sign
        //bool debug= true; 
        //if(debug)
        //    _rlm.Range(0,1000);        
        _rlm = _rlm.Define("one", "1.0");
        if (_isData)
        {
                if (!isDefined("evWeight"))
                {
                        _rlm = _rlm.Define("evWeight", [](){ return 1.0; }, {} );
                }
                if (!isDefined("pugenWeight"))
                {
                        _rlm = _rlm.Define("pugenWeight", [](){ return 1.0; }, {} );
                }
                if (!isDefined("genWeight"))
                {
                        _rlm = _rlm.Define("genWeight", [](){ return 1.0; }, {} );
                }
                if (!isDefined("regenWeight"))
                {
                        _rlm = _rlm.Define("regenWeight", [](){ return 1.0; }, {} );
                }
        }
        else if (!_isData) {
                if (!isDefined("puWeight")) _rlm = _rlm.Define("puWeight",
                        [this](float x) {
                            return _puweightcalc->getWeight(x);
                        }, {"Pileup_nTrueInt"}
                    );
                if (!isDefined("puWeight_plus")) _rlm = _rlm.Define("puWeight_plus",
                        [this](float x) {
                            return _puweightcalc_plus->getWeight(x);
                        }, {"Pileup_nTrueInt"}
                    );
                if (!isDefined("puWeight_minus")) _rlm = _rlm.Define("puWeight_minus",
                        [this](float x) {
                            return _puweightcalc_minus->getWeight(x);
                        }, {"Pileup_nTrueInt"}
                    );
                if (!isDefined("pugenWeight")) _rlm = _rlm.Define("pugenWeight", [this](float x, float y){
                            return (x > 0 ? 1.0 : -1.0) *y;
                        }, {"genWeight", "puWeight"}
                    );
                if (!isDefined("regenWeight")) _rlm = _rlm.Define("regenWeight", [this](float x){
                            return (x > 0 ? 1.0 : -1.0);
                        }, {"genWeight"}
                    );
        }

        // Object selection will be defined in sequence.
        // Selected objects will be stored in new vectors.

        selectElectrons();
        selectMuons();
        selectLeptons();
        applyJetMETCorrections();
        //selectMET();
        if(_cat.find("tt*") && isDefined("GenJet_nBHadFromT")) selectGenJets(); //for ttSL categorization
        selectJets();
        //if(_sync) synchro(); //for synchronization TODO
        calculateEvWeight();
        defineMoreVars();
        defineCuts();
        bookHists();
        setupCuts_and_Hists();
        setupTree();
}

bool NanoAODAnalyzerrdframe::readjson()
{
        auto isgoodjsonevent = [this](unsigned int runnumber, unsigned int lumisection)
                {
                        auto key = std::to_string(runnumber);

                        bool goodeventflag = false;

                        if (jsonroot.isMember(key))
                        {
                                Json::Value runlumiblocks = jsonroot[key];
                                for (unsigned int i=0; i<runlumiblocks.size() && !goodeventflag; i++)
                                {
                                        auto lumirange = runlumiblocks[i];
                                        if (lumisection >= lumirange[0].asUInt() && lumisection <= lumirange[1].asUInt()) goodeventflag = true;
                                }
                                return goodeventflag;
                        }
                        else
                        {
                                //cout << "Run not in json " << runnumber << endl;
                                return false;
                        }

                };

        if (_jsonfname != "")
        {
                std::ifstream jsoninfile;
                jsoninfile.open(_jsonfname);

                if (jsoninfile.good())
                {
                        jsoninfile >> jsonroot;
                        /*
                        auto runlumiblocks =  jsonroot["276775"];
                        for (auto i=0; i<runlumiblocks.size(); i++)
                        {
                                auto lumirange = runlumiblocks[i];
                                cout << "lumi range " << lumirange[0] << " " << lumirange[1] << endl;
                        }
                        */
                        _rlm = _rlm.Define("goodjsonevent", isgoodjsonevent, {"run", "luminosityBlock"}).Filter("goodjsonevent");
                        _jsonOK = true;
                        return true;
                }
                else
                {
                        cout << "Problem reading json file " << _jsonfname << endl;
                        return false;
                }
        }
        else
        {
                cout << "no JSON file given" << endl;
                return true;
        }
}

void NanoAODAnalyzerrdframe::setupJetMETCorrection(string globaltag, string jetalgo)
{
        if (globaltag != "")
        {
                cout << "Applying new JetMET corrections. GT: "+globaltag+" on jetAlgo: "+jetalgo << endl;
                string basedirectory = "data/jme/";

                string datamcflag = "";
                if (_isData) datamcflag = "DATA";
                else datamcflag = "MC";

                // set file names that contain the parameters for corrections
                string dbfilenamel1 = basedirectory+globaltag+"_"+datamcflag+"_L1FastJet_"+jetalgo+".txt";
                string dbfilenamel2 = basedirectory+globaltag+"_"+datamcflag+"_L2Relative_"+jetalgo+".txt";
                string dbfilenamel3 = basedirectory+globaltag+"_"+datamcflag+"_L3Absolute_"+jetalgo+".txt";
                string dbfilenamel2l3 = basedirectory+globaltag+"_"+datamcflag+"_L2L3Residual_"+jetalgo+".txt";

                JetCorrectorParameters *L1JetCorrPar = new JetCorrectorParameters(dbfilenamel1);

                if (!L1JetCorrPar->isValid())
                {
                    std::cerr << "L1FastJet correction parameters not read" << std::endl;
                    exit(1);
                }
                        JetCorrectorParameters *L2JetCorrPar = new JetCorrectorParameters(dbfilenamel2);
                if (!L2JetCorrPar->isValid())
                {
                    std::cerr << "L2Relative correction parameters not read" << std::endl;
                    exit(1);
                }
                        JetCorrectorParameters *L3JetCorrPar = new JetCorrectorParameters(dbfilenamel3);
                if (!L3JetCorrPar->isValid())
                {
                    std::cerr << "L3Absolute correction parameters not read" << std::endl;
                    exit(1);
                }
                        JetCorrectorParameters *L2L3JetCorrPar = new JetCorrectorParameters(dbfilenamel2l3);
                if (!L2L3JetCorrPar->isValid())
                {
                    std::cerr << "L2L3Residual correction parameters not read" << std::endl;
                    exit(1);
                }

                // to apply all the corrections, first collect them into a vector
                std::vector<JetCorrectorParameters> jetc;
                jetc.push_back(*L1JetCorrPar);
                jetc.push_back(*L2JetCorrPar);
                jetc.push_back(*L3JetCorrPar);
                jetc.push_back(*L2L3JetCorrPar);

                // apply the various corrections
                _jetCorrector = new FactorizedJetCorrector(jetc);

                // object to calculate uncertainty
                string dbfilenameunc = basedirectory+globaltag+"_"+datamcflag+"_Uncertainty_"+jetalgo+".txt";
                _jetCorrectionUncertainty = new JetCorrectionUncertainty(dbfilenameunc);
        }
        else
        {
                cout << "Not applying new JetMET corrections. Using NanoAOD as is." << endl;
        }
}



void NanoAODAnalyzerrdframe::selectElectrons()
{
        cout << "select electrons" << endl;
        // Run II recommendation: https://twiki.cern.ch/twiki/bin/viewauth/CMS/EgammaRunIIRecommendations
        // Run II recomendation - cutbased: https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedElectronIdentificationRun2

        if(_year == "2018" || _year == "2017") _rlm = _rlm.Define("elKinematics","( Electron_pt > 34.0 && abs(Electron_eta) < 2.4 ) || ( Electron_pt > 30.0 && abs(Electron_eta) < 2.1 )");
        if(_year.find("2016") != std::string::npos) _rlm = _rlm.Define("elKinematics","Electron_pt > 29.0 && abs(Electron_eta) < 2.4");

        _rlm = _rlm.Define("elecuts", "elKinematics && ( ( abs(Electron_eta + Electron_deltaEtaSC) < 1.4442 ) || abs(Electron_eta + Electron_deltaEtaSC) > 1.566 ) && ( ( (abs(Electron_eta + Electron_deltaEtaSC) <= 1.479) && abs(Electron_dxy) < 0.05 && abs(Electron_dz) < 0.10 ) || ( (abs(Electron_eta + Electron_deltaEtaSC)>1.479) && abs(Electron_dxy) < 0.10 && abs(Electron_dz) < 0.20) ) && Electron_cutBased == 4");
        _rlm = _rlm.Define("vetoelecuts", "Electron_pt > 15.0 && abs(Electron_eta) < 2.5 && Electron_cutBased >= 1"); 
        _rlm = _rlm.Define("Sel_elept", "Electron_pt[elecuts]")
                   .Define("Sel_eleta", "Electron_eta[elecuts]")
                   .Define("Sel_elephi", "Electron_phi[elecuts]")
                   .Define("Sel_elemass", "Electron_mass[elecuts]")
                   .Define("Sel_eleidx", ::good_idx, {"elecuts"})
                   .Define("nelepass", "int(Sel_elept.size())")
                   .Define("nvetoeles","Sum(vetoelecuts)");

        _rlm = _rlm.Define("ele4vecs", ::gen4vec, {"Sel_elept", "Sel_eleta", "Sel_elephi", "Sel_elemass"});

}

void NanoAODAnalyzerrdframe::selectMuons()
{
        cout << "select muons" << endl;
        if(_year == "2018" || _year.find("2016") != std::string::npos) _rlm = _rlm.Define("muKinematics","Muon_pt > 26.0 && abs(Muon_eta) < 2.4");
        if(_year == "2017") _rlm = _rlm.Define("muKinematics","Muon_pt > 29.0 && abs(Muon_eta) < 2.4");

        _rlm = _rlm.Define("muoncuts", "muKinematics && Muon_tightId && Muon_pfRelIso04_all<0.15");
        _rlm = _rlm.Define("Sel_muonpt", "Muon_pt[muoncuts]") // define new variables
                   .Define("Sel_muoneta", "Muon_eta[muoncuts]")
                   .Define("Sel_muonphi", "Muon_phi[muoncuts]")
                   .Define("Sel_muonmass", "Muon_mass[muoncuts]")
                   .Define("Sel_muoncharge", "Muon_charge[muoncuts]")
                   .Define("Sel_muonidx", ::good_idx, {"muoncuts"})
                   .Define("nmuonpass", "int(Sel_muonpt.size())");

        _rlm = _rlm.Define("vetomuoncuts", "Muon_pt>15.0 && abs(Muon_eta)<2.4 && Muon_looseId && Muon_pfRelIso04_all<0.25")
                   .Define("nvetomuons","Sum(vetomuoncuts)");

        _rlm = _rlm.Define("muon4vecs", ::gen4vec, {"Sel_muonpt", "Sel_muoneta", "Sel_muonphi", "Sel_muonmass"});
}

void NanoAODAnalyzerrdframe::selectLeptons()
{
        // For 2017, Need to match all L1 objects within DR<0.1 of the offline electron with condition(32_L1DoubleEG_AND_L1SingleEGOr)
        auto objectMatching = [](float el_eta, float el_phi, floats &trigob_eta, floats &trigob_phi, ints &trigob_fbits){
        
            bool matched = false;
            for (size_t i = 0; i < trigob_eta.size(); i++){
                float dR = ROOT::VecOps::DeltaR(el_eta, trigob_eta[i], el_phi, trigob_phi[i]);
                if( dR < 0.1 && trigob_fbits[i] & 1024 ) matched = true;
            }
            return matched;
        
        };

        cout << "Define leptons" << endl;   

        _rlm = _rlm.Define("is_mu","(nmuonpass == 1 && nvetomuons == 1 && (nelepass + nvetoeles) == 0) ? true : false")
                   .Define("is_e","(nelepass == 1 && nvetoeles == 1 && (nmuonpass + nvetomuons) == 0) ? true : false");

        _rlm = _rlm.Define("channel","is_mu ? 0 : 1");

        _rlm = _rlm.Define("lepton_pt","is_mu ? Muon_pt[muoncuts][0] : Electron_pt[elecuts][0]")
                   .Define("lepton_eta","is_mu ? Muon_eta[muoncuts][0] : Electron_eta[elecuts][0]")
                   .Define("lepton_phi","is_mu ? Muon_phi[muoncuts][0] : Electron_phi[elecuts][0]")
                   .Define("lepton_m","is_mu ? Muon_mass[muoncuts][0] : Electron_mass[elecuts][0]");

        if(_year == "2018"){
            _rlm = _rlm.Define("flags","(Flag_goodVertices && Flag_globalSuperTightHalo2016Filter && Flag_HBHENoiseFilter && Flag_HBHENoiseIsoFilter && Flag_EcalDeadCellTriggerPrimitiveFilter && Flag_BadPFMuonFilter && Flag_eeBadScFilter && Flag_ecalBadCalibFilter) ? true : false")
                       .Define("muTrigger","HLT_IsoMu24")
                       .Define("elTrigger","HLT_Ele32_WPTight_Gsf || HLT_Ele28_eta2p1_WPTight_Gsf_HT150");
        }
        if(_year == "2017"){
            _rlm = _rlm.Define("flags","(Flag_goodVertices && Flag_globalSuperTightHalo2016Filter && Flag_HBHENoiseFilter && Flag_HBHENoiseIsoFilter && Flag_EcalDeadCellTriggerPrimitiveFilter && Flag_BadPFMuonFilter && Flag_eeBadScFilter && Flag_ecalBadCalibFilter) ? true : false")
                       .Define("flag_Trig",objectMatching,{"lepton_eta","lepton_phi","TrigObj_eta","TrigObj_phi","TrigObj_filterBits"})
                       .Define("muTrigger","HLT_IsoMu27")
                       .Define("elTrigger","(HLT_Ele32_WPTight_Gsf_L1DoubleEG && flag_Trig) || HLT_Ele28_eta2p1_WPTight_Gsf_HT150");
        }
        if(_year.find("2016") != std::string::npos){
            _rlm = _rlm.Define("flags","(Flag_goodVertices && Flag_globalSuperTightHalo2016Filter && Flag_HBHENoiseFilter && Flag_HBHENoiseIsoFilter && Flag_EcalDeadCellTriggerPrimitiveFilter && Flag_BadPFMuonFilter && Flag_eeBadScFilter) ? true : false")
                       .Define("muTrigger","HLT_IsoMu24 || HLT_IsoTkMu24")
                       .Define("elTrigger","HLT_Ele27_WPTight_Gsf");
        }


}


/*
void NanoAODAnalyzerrdframe::selectMET()
{
        _rlm = _rlm.Define("met4vec", ::genmet4vec, {"MET_pt","MET_phi"});

}*/


// Adapted from https://github.com/cms-nanoAOD/nanoAOD-tools/blob/master/python/postprocessing/modules/jme/jetRecalib.py
// and https://github.com/cms-nanoAOD/nanoAOD-tools/blob/master/python/postprocessing/modules/jme/JetRecalibrator.py
void NanoAODAnalyzerrdframe::applyJetMETCorrections()
{
        auto appcorrlambdaf = [this](floats jetpts, floats jetetas, floats jetAreas, floats jetrawf, float rho)->floats
        {
            floats corrfactors;
            corrfactors.reserve(jetpts.size());
            for (unsigned int i =0; i<jetpts.size(); i++)
            {
                float rawjetpt = jetpts[i]*(1.0-jetrawf[i]);
                _jetCorrector->setJetPt(rawjetpt);
                _jetCorrector->setJetEta(jetetas[i]);
                _jetCorrector->setJetA(jetAreas[i]);
                _jetCorrector->setRho(rho);
                float corrfactor = _jetCorrector->getCorrection();
                corrfactors.emplace_back(rawjetpt * corrfactor);
            
            }
            return corrfactors;
        };
        
        auto jecuncertaintylambdaf= [this](floats jetpts, floats jetetas, floats jetAreas, floats jetrawf, float rho)->floats
            {
                floats uncertainties;
                uncertainties.reserve(jetpts.size());
                for (unsigned int i =0; i<jetpts.size(); i++)
                {
                    float rawjetpt = jetpts[i]*(1.0-jetrawf[i]);
                    _jetCorrector->setJetPt(rawjetpt);
                    _jetCorrector->setJetEta(jetetas[i]);
                    _jetCorrector->setJetA(jetAreas[i]);
                    _jetCorrector->setRho(rho);
                    float corrfactor = _jetCorrector->getCorrection();
                    _jetCorrectionUncertainty->setJetPt(corrfactor*rawjetpt);
                    _jetCorrectionUncertainty->setJetEta(jetetas[i]);
                    float unc = _jetCorrectionUncertainty->getUncertainty(true);
                    uncertainties.emplace_back(unc);
                
                }
                return uncertainties;
            };
        
        auto metcorrlambdaf = [](float met, float metphi, floats jetptsbefore, floats jetptsafter, floats jetphis)->float
        {
            auto metx = met * cos(metphi);
            auto mety = met * sin(metphi);
            for (unsigned int i=0; i<jetphis.size(); i++)
            {
                if (jetptsafter[i]>15.0)
                {
                    metx -= (jetptsafter[i] - jetptsbefore[i])*cos(jetphis[i]);
                    mety -= (jetptsafter[i] - jetptsbefore[i])*sin(jetphis[i]);
                }
            }
            return float(sqrt(metx*metx + mety*mety));
        };
        
        auto metphicorrlambdaf = [](float met, float metphi, floats jetptsbefore, floats jetptsafter, floats jetphis)->float
        {
            auto metx = met * cos(metphi);
            auto mety = met * sin(metphi);
            for (unsigned int i=0; i<jetphis.size(); i++)
            {
                if (jetptsafter[i]>15.0)
                {
                    metx -= (jetptsafter[i] - jetptsbefore[i])*cos(jetphis[i]);
                    mety -= (jetptsafter[i] - jetptsbefore[i])*sin(jetphis[i]);
                }
            }
            return float(atan2(mety, metx));
        };
        
        if (_jetCorrector != 0)
        {
            _rlm = _rlm.Define("Jet_pt_corr", appcorrlambdaf, {"Jet_pt", "Jet_eta", "Jet_area", "Jet_rawFactor", "fixedGridRhoFastjetAll"});
            _rlm = _rlm.Define("Jet_pt_relerror", jecuncertaintylambdaf, {"Jet_pt", "Jet_eta", "Jet_area", "Jet_rawFactor", "fixedGridRhoFastjetAll"});
            _rlm = _rlm.Define("Jet_pt_corr_up", "Jet_pt_corr*(1.0f + Jet_pt_relerror)");
            _rlm = _rlm.Define("Jet_pt_corr_down", "Jet_pt_corr*(1.0f - Jet_pt_relerror)");
            _rlm = _rlm.Define("MET_pt_corr", metcorrlambdaf, {"MET_pt", "MET_phi", "Jet_pt", "Jet_pt_corr", "Jet_phi"});
            _rlm = _rlm.Define("MET_phi_corr", metphicorrlambdaf, {"MET_pt", "MET_phi", "Jet_pt", "Jet_pt_corr", "Jet_phi"});
            _rlm = _rlm.Define("MET_pt_corr_up", metcorrlambdaf, {"MET_pt", "MET_phi", "Jet_pt", "Jet_pt_corr_up", "Jet_phi"});
            _rlm = _rlm.Define("MET_phi_corr_up", metphicorrlambdaf, {"MET_pt", "MET_phi", "Jet_pt", "Jet_pt_corr_up", "Jet_phi"});
            _rlm = _rlm.Define("MET_pt_corr_down", metcorrlambdaf, {"MET_pt", "MET_phi", "Jet_pt", "Jet_pt_corr_down", "Jet_phi"});
            _rlm = _rlm.Define("MET_phi_corr_down", metphicorrlambdaf, {"MET_pt", "MET_phi", "Jet_pt", "Jet_pt_corr_down", "Jet_phi"});
        }
}

void NanoAODAnalyzerrdframe::selectGenJets(){

        //is this genjet additional jet?
        auto isaddJets = []( floats jeta, floats jphi, floats Wqeta, floats Wqphi, ints GenJet_nBHadFromT, ints GenJet_nBHadFromTbar, ints GenJet_nBHadFromW, ints GenJet_nBHadOther, ints GenJet_nCHadFromW, ints GenJet_nCHadOther ){
        
          bools out;
          bool flag = false;
        
          //GenJet loops
          for( size_t i = 0; i < jeta.size(); i++ ){
            flag = false;
        
            //additional Jets
            if( GenJet_nBHadFromTbar[i] + GenJet_nBHadFromT[i] + GenJet_nCHadFromW[i] < 1 ){
              double minDRWquarks = DBL_MAX;
        
              for( size_t j = 0; j < Wqeta.size(); j++ ){
                double dR = ROOT::VecOps::DeltaR(jeta[i], Wqeta[j], jphi[i], Wqphi[j]);
                if( dR < minDRWquarks ) minDRWquarks = dR;
              }
              if( minDRWquarks > 0.4 ) flag = true;
            }
            //additional bJets
            if( GenJet_nBHadOther[i] > 0 ) flag = true;
            //additional cJets
            if( GenJet_nCHadOther[i] > 0 ) flag = true;
        
            out.push_back(flag);
          }
        
          return out;
        };

        auto isquarkfromW = [](FourVectorVec &p, ints &pdgId, ints &midx){
        
            bools out;
            unsigned int np = p.size();
            bool flag = false;
        
            // GenParticle loop 
            for(unsigned int i = 0; i < np; i++){
                flag = false;

                //if the particle is quark {u d s c b}
                if( abs( pdgId[i] ) < 6 ){

                  //Is its mother W and Does it have top as its ancestor?
                  int moid = midx[i];
                  if ( abs( pdgId[ moid ] ) == 24 ){

                      //Make sure W has two daugthers (W->qq)
                      ints d_idx = find_element(midx, moid);
                      if( d_idx.size() == 2 ){

                          //lets find the mother of this W...
                          for( unsigned int k = 0; k < np-2; k++){
                              moid = midx[ moid ];
                              if (moid > int(pdgId.size()) || moid < -1 ) break; //segement violation arises if the index is out of range

                              //if this W has top as its ancestor 
                              if( abs( pdgId[ moid ] ) == 6) {
                                flag = true;
                                break;
                              }
                          }
                      }
                  }
                }
                out.emplace_back(flag);
            }
            return out;
        };

        //check genlepton is muon or electron and whether they're from top
        auto isleptonfromtop = [](FourVectorVec &p, ints &pdgId, ints &midx){
            bools out;
            unsigned int np = p.size();
            bool flag = false;
        
            // GenParticle loop
            for( unsigned int i = 0; i < np; i++ ){
                flag = false;
                //should be muon or electron
                if ( !( abs( pdgId[i] ) == 13 || abs( pdgId[i] ) == 11) ) {
                    out.emplace_back(flag);
                    continue;
                }
                //lets find the mother of this particle...
                int moid = midx[i];
                for( unsigned int k = 0; k < np-1; k++ ){
                  //sometimes if the index goes out of the index range segment violates 
                    if (moid > int(pdgId.size()) || moid < 0 ) break;

                    moid = midx[ moid ];
                    if( abs( pdgId[ moid ] ) == 6) {
                        flag = true;
                        break;
                    }
        
                }
                out.emplace_back(flag);
            }
            return out;
        };

        //clean jets with leptons
        auto dRlepton = [](FourVectorVec &jets, FourVectorVec &leptons){
            doubles dr_lepjet;

            for ( auto ajet: jets ){

                double mindR = FLT_MAX;
                for ( auto alepton: leptons ){

                    double dR = ROOT::Math::VectorUtil::DeltaR(ajet, alepton);
                    if( dR < mindR ) mindR = dR;
                }
                dr_lepjet.emplace_back(mindR);
            }
            return dr_lepjet;
        };
        
        //genLepton removal with GenParticle collection: muon or electron that are from top 
        _rlm = _rlm.Define("p4_GenJet",::gen4vec, {"GenJet_pt","GenJet_eta","GenJet_phi","GenJet_mass"})
                   .Define("p4_GenPart",::gen4vec, {"GenPart_pt","GenPart_eta","GenPart_phi","GenPart_mass"});
      
        _rlm = _rlm.Define("isleptonfromtop", isleptonfromtop, {"p4_GenPart","GenPart_pdgId","GenPart_genPartIdxMother"})
                   .Define("Genparts_pt","GenPart_pt[isleptonfromtop]")
                   .Define("Genparts_eta","GenPart_eta[isleptonfromtop]")
                   .Define("Genparts_phi","GenPart_phi[isleptonfromtop]")
                   .Define("Genparts_mass","GenPart_mass[isleptonfromtop]")
                   .Define("p4_Genparts",::gen4vec, {"Genparts_pt","Genparts_eta","Genparts_phi","Genparts_mass"})
                   .Define("origin_gendR",dRlepton, {"p4_GenJet","p4_Genparts"});

        //selection additional Jets
        _rlm = _rlm.Define("quarksfromW",isquarkfromW,{"p4_GenPart","GenPart_pdgId","GenPart_genPartIdxMother"})
                   .Define("quarksfromW_eta","GenPart_eta[quarksfromW]")
                   .Define("quarksfromW_phi","GenPart_phi[quarksfromW]")
                   .Define("isaddJet",isaddJets,{"GenJet_eta","GenJet_phi","quarksfromW_eta","quarksfromW_phi","GenJet_nBHadFromT","GenJet_nBHadFromTbar","GenJet_nBHadFromW","GenJet_nBHadOther","GenJet_nCHadFromW","GenJet_nCHadOther"});

        _rlm = _rlm.Define("sel_addjets","origin_gendR > 0.4 && isaddJet && GenJet_pt > 20 && abs(GenJet_eta) < 2.4")
                   .Define("sel_addbjets", "origin_gendR > 0.4 && (GenJet_nBHadFromT + GenJet_nBHadFromTbar + GenJet_nBHadFromW) == 0 && GenJet_nBHadOther > 0") //pt>20 && |eta|<2.4 selections are already applied in GenJet_n*Had*
                   .Define("sel_addcjets", "origin_gendR > 0.4 && GenJet_nCHadFromW == 0 && GenJet_nCHadOther > 0"); //pt>20 && |eta|<2.4 selections are already applied in GenJet_n*Had*

        //for ttbb
        _rlm = _rlm.Define("addbjets_pt","GenJet_pt[sel_addbjets]") 
                   .Define("addbjets_eta","GenJet_eta[sel_addbjets]") 
                   .Define("addbjets_phi","GenJet_phi[sel_addbjets]") 
                   .Define("addbjets_mass","GenJet_mass[sel_addbjets]")
                   .Define("p4_addbjets",::gen4vec, {"addbjets_pt","addbjets_eta","addbjets_phi","addbjets_mass"})

                   .Define("addbjet1_pt","addbjets_pt[0]") 
                   .Define("addbjet1_eta","addbjets_eta[0]") 
                   .Define("addbjet1_phi","addbjets_phi[0]") 
                   .Define("addbjet1_mass","addbjets_mass[0]")

                   .Define("addbjet2_pt","addbjets_pt[1]") 
                   .Define("addbjet2_eta","addbjets_eta[1]") 
                   .Define("addbjet2_phi","addbjets_phi[1]") 
                   .Define("addbjet2_mass","addbjets_mass[1]");

        //for categorization
        _rlm = _rlm.Define("NaddJets20", "Sum(sel_addjets)")  // >1:ttjj
                   .Define("NaddbJets20","Sum(sel_addbjets)") // >1:ttbb, >0:ttbj
                   .Define("NaddcJets20","Sum(sel_addcjets)"); // >1:ttcc

        //https://github.com/vallot/CATTools/blob/cat10x/CatAnalyzer/plugins/ttbbLepJetsAnalyzer.cc#L858-L866
        _rlm = _rlm.Define("isttjj","NaddJets20 > 1")
                   .Define("isttbb","isttjj && NaddbJets20 > 1")
                   .Define("isttbj","!isttbb && isttjj && NaddbJets20 > 0")
                   .Define("isttcc","!(isttbb || isttbj) && isttjj && NaddcJets20 > 1")
                   .Define("isttLF","!(isttbb || isttbj || isttcc) && isttjj && NaddJets20 > 1")
                   .Define("isttother","!(isttbb || isttbj || isttcc || isttLF)");

}

void NanoAODAnalyzerrdframe::selectJets()
{
// apparently size() returns long int, which ROOT doesn't recognized for branch types
// , so it must be cast into int if you want to save them later into a TTree
        auto DR_ljets = [](floats jeteta, floats jetphi, float lepeta, float lepphi){
        
            doubles dr;
            for (size_t i = 0; i < jeteta.size(); i++){
                dr.push_back(ROOT::VecOps::DeltaR(jeteta[i], lepeta, jetphi[i], lepphi));
            }
            return dr;
        };

        _rlm = _rlm.Define("dr_lepjets",DR_ljets,{"Jet_eta", "Jet_phi", "lepton_eta", "lepton_phi"});

        _rlm = _rlm.Define("jetcuts", "Jet_pt>30.0 && abs(Jet_eta)<2.4 && Jet_jetId & 4 && dr_lepjets > 0.4")
                   .Define("Sel_jetpt", "Jet_pt[jetcuts]")
                   .Define("Sel_jeteta", "Jet_eta[jetcuts]")
                   .Define("Sel_jetphi", "Jet_phi[jetcuts]")
                   .Define("Sel_jetmass", "Jet_mass[jetcuts]")
                   .Define("Sel_jetbtag", "Jet_btagDeepFlavB[jetcuts]")
                   .Define("Sel_jetcvsb", "Jet_btagDeepFlavCvB[jetcuts]")
                   .Define("Sel_jetcvsl", "Jet_btagDeepFlavCvL[jetcuts]")
                   .Define("njetspass", "int(Sel_jetpt.size())")
                   .Define("jet4vecs", ::gen4vec, {"Sel_jetpt", "Sel_jeteta", "Sel_jetphi", "Sel_jetmass"});

        //for making histogram convention, temporal
        _rlm = _rlm.Define("jet_pt", "Sel_jetpt")
                   .Define("jet_eta", "Sel_jeteta")
                   .Define("jet_phi", "Sel_jetphi")
                   .Define("jet_m", "Sel_jetmass")
                   .Define("jet_deepJet", "Sel_jetbtag")
                   .Define("jet_cvsb", "Sel_jetcvsb")
                   .Define("jet_cvsl", "Sel_jetcvsl")
                   .Define("jet_multiplicity", "int(jet_pt.size())")
                   .Define("cleanjet4vecs", ::gen4vec, {"jet_pt", "jet_eta", "jet_phi", "jet_m"})
                   .Define("jet_HT", "Sum(jet_pt)");
                   //.Define("Sel2_jetweight", "std::vector<double>(jet_number, evWeight)")
      
        //https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation102X
        if(_year == "2018") _rlm = _rlm.Define("wp","0.2770");
        if(_year == "2017") _rlm = _rlm.Define("wp","0.3033");
        if(_year.find("2016") != std::string::npos) _rlm = _rlm.Define("wp","0.3093");

        _rlm = _rlm.Define("btagcuts", "jet_deepJet>wp")
                   .Define("bjet_pt", "jet_pt[btagcuts]")
                   .Define("bjet_eta", "jet_eta[btagcuts]")
                   .Define("bjet_phi", "jet_phi[btagcuts]")
                   .Define("bjet_mass", "jet_m[btagcuts]")
                   .Define("bjet_multiplicity", "int(bjet_pt.size())")
                   .Define("bjet_HT", "Sum(bjet_pt)")
                   .Define("cleanbjet4vecs", ::gen4vec, {"bjet_pt", "bjet_eta", "bjet_phi", "bjet_mass"});

}

void NanoAODAnalyzerrdframe::synchro()
{

        _rlm = _rlm.Define("trigger_mu","muTrigger ? true : false")
                   .Define("trigger_e","elTrigger ? true : false");

        _rlm = _rlm.Define("lepton_deltaEtaSC","is_mu ? -1.0 : Electron_deltaEtaSC[elecuts][0]")
                   .Define("lepton_abs_dxy","is_mu ? abs(Muon_dxy[muoncuts][0]) : abs(Electron_dxy[elecuts][0])")
                   .Define("lepton_abs_dz","is_mu ? abs(Muon_dz[muoncuts][0]) : abs(Electron_dz[elecuts][0])")                        
                   .Define("lepton_iso","is_mu ? Muon_pfRelIso04_all[muoncuts][0] : Electron_pfRelIso03_all[elecuts][0]")                        
                   .Define("nJets","njetspass")
                   .Define("jet1_pt","Sum(jetcuts) > 0 ? Sel_jetpt[0] : -1.0")
                   .Define("jet1_eta","Sum(jetcuts) > 0 ? Sel_jeteta[0] : -1.0")
                   .Define("jet1_phi","Sum(jetcuts) > 0 ? Sel_jetphi[0] : -1.0")
                   .Define("jet1_deepJet","Sum(jetcuts) > 0 ? Sel_jetbtag[0] : -1.0")
                   .Define("jet2_pt","Sum(jetcuts) > 1 ? Sel_jetpt[1] : -1.0")
                   .Define("jet2_eta","Sum(jetcuts) > 1 ? Sel_jeteta[1] : -1.0")
                   .Define("jet2_phi","Sum(jetcuts) > 1 ? Sel_jetphi[1] : -1.0")
                   .Define("jet2_deepJet","Sum(jetcuts) > 1 ? Sel_jetbtag[1] : -1.0")

                   .Define("nBJets_deepJetM", "bjet_multiplicity"); 
}


// This function is newly added for getting event weight with selected objects
void NanoAODAnalyzerrdframe::calculateEvWeight()
{
        
        if (!_isData && !isDefined("evWeight")) {
                // Muon SF                
                cout<<"Getting Muon Scale Factors"<<endl;
                auto muon2018SF = [this](floats &pt, floats &eta)->float {
                        float weight = 1;
                        if(pt.size() > 0){
                                for(unsigned int i=0; i<pt.size(); i++){
                                        float trg_SF = _muontrg->getWeight(std::abs(eta[i]),pt[i]);
                                        float ID_SF = _muonid->getWeight(std::abs(eta[i]),pt[i]);
                                        float Iso_SF = _muoniso->getWeight(std::abs(eta[i]),pt[i]);
                                        weight *= trg_SF * ID_SF * Iso_SF;
                                }
                        }
                        return weight;
                };

                _rlm = _rlm.Define("evWeight_muonSF",muon2018SF,{"Sel_muonpt","Sel_muoneta"});

                // Electron SF                
                //cout<<"Getting Electron Scale Factors"<<endl;
                //_rlm = _rlm.Define("evWeight_eleSF",muon2018SF,{"Sel_elept","Sel_eleeta"});

                //_rlm = _rlm.Define("evWeight_leptonSF","evWeight_muonSF*evWeight_eleSF"); //no eleSF sofar
                _rlm = _rlm.Define("evWeight_leptonSF","evWeight_muonSF"); //no eleSF sofar

                // B tagging SF
                // calculate event weight for MC only
                _rlm = _rlm.Define("Sel_jethadflav","Jet_hadronFlavour[jetcuts]")
                           .Define("jet_hadflav","Sel_jethadflav");
        
                // function to calculate event weight for MC events based on DeepJet algorithm
                auto btagweightgenerator= [this](floats &pts, floats &etas, ints &hadflav, floats &btags)->float
                {
                    double bweight=1.0;
                    BTagEntry::JetFlavor hadfconv;
                    for (unsigned int i=0; i<pts.size(); i++)
                    {
                        if (hadflav[i]==5) hadfconv=BTagEntry::FLAV_B;
                        else if (hadflav[i]==4) hadfconv=BTagEntry::FLAV_C;
                        else hadfconv=BTagEntry::FLAV_UDSG;
                        
                        double w = _btagcalibreader.eval_auto_bounds("central", hadfconv, fabs(etas[i]), pts[i], btags[i]);
                        bweight *= w;
                    }
                    return bweight;
                };

                cout<<"Generate b-tagging weight"<<endl;
                _rlm = _rlm.Define("btagWeight_DeepFlavBrecalc", btagweightgenerator, {"jet_pt", "jet_eta", "jet_hadflav", "jet_deepJet"});
                _rlm = _rlm.Define("evWeight", "pugenWeight * btagWeight_DeepFlavBrecalc * evWeight_leptonSF");

        }
}

/*
bool NanoAODAnalyzerrdframe::helper_1DHistCreator(std::string hname, std::string title, const int nbins, const double xlow, const double xhi, std::string rdfvar, std::string evWeight)
{
        RDF1DHist histojets = _rlm.Histo1D({hname.c_str(), title.c_str(), nbins, xlow, xhi}, rdfvar, evWeight); // Fill with weight given by evWeight
        _th1dhistos[hname] = histojets;
}
*/

void NanoAODAnalyzerrdframe::helper_1DHistCreator(std::string hname, std::string title, const int nbins, const double xlow, const double xhi, std::string rdfvar, std::string evWeight, RNode *anode)
{
        RDF1DHist histojets = anode->Histo1D({hname.c_str(), title.c_str(), nbins, xlow, xhi}, rdfvar, evWeight); // Fill with weight given by evWeight
        _th1dhistos[hname] = histojets;
}
;

// Automatically loop to create
void NanoAODAnalyzerrdframe::setupCuts_and_Hists()
{
        cout << "setting up definitions, cuts, and histograms" <<endl;
        
        for ( auto &c : _varinfovector)
        {
            if (c.mincutstep.length()==0) _rlm = _rlm.Define(c.varname, c.vardefinition);
        }
        
        for (auto &x : _hist1dinfovector)
        {
            std::string hpost = "_nocut";
        
            if (x.mincutstep.length()==0)
            {
                helper_1DHistCreator(std::string(x.hmodel.fName)+hpost,  std::string(x.hmodel.fTitle)+hpost, x.hmodel.fNbinsX, x.hmodel.fXLow, x.hmodel.fXUp, x.varname, x.weightname, &_rlm);
            }
        }
        
        _rnt.setRNode(&_rlm);
        
        for (auto acut : _cutinfovector)
        {
            std::string cutname = "cut"+ acut.idx;
            std::string hpost = "_"+cutname;
            RNode *r = _rnt.getParent(acut.idx)->getRNode();
            auto rnext = new RNode(r->Define(cutname, acut.cutdefinition));
            *rnext = rnext->Filter(cutname);
        
            for ( auto &c : _varinfovector)
            {
                if (acut.idx.compare(c.mincutstep)==0) *rnext = rnext->Define(c.varname, c.vardefinition);
            }
            for (auto &x : _hist1dinfovector)
            {
                if (acut.idx.compare(0, x.mincutstep.length(), x.mincutstep)==0)
                {
                    helper_1DHistCreator(std::string(x.hmodel.fName)+hpost,  std::string(x.hmodel.fTitle)+hpost, x.hmodel.fNbinsX, x.hmodel.fXLow, x.hmodel.fXUp, x.varname, x.weightname, rnext);
                }
            }
            _rnt.addDaughter(rnext, acut.idx);

            /*
            _rlm = _rlm.Define(cutname, acut.cutdefinition);
            _rlm = _rlm.Filter(cutname);

            for ( auto &c : _varinfovector)
            {
                    if (acut.idx.compare(c.mincutstep)==0) _rlm = _rlm.Define(c.varname, c.vardefinition);
            }
            for (auto &x : _hist1dinfovector)
            {
                    if (acut.idx.compare(0, x.mincutstep.length(), x.mincutstep)==0)
                    {
                            helper_1DHistCreator(std::string(x.hmodel.fName)+hpost,  std::string(x.hmodel.fTitle)+hpost, x.hmodel.fNbinsX, x.hmodel.fXLow, x.hmodel.fXUp, x.varname, x.weightname);
                    }
            }
            _rnt.addDaughter(&_rlm, acut.idx);
            */
        }
}

void NanoAODAnalyzerrdframe::add1DHist(TH1DModel histdef, std::string variable, std::string weight,string mincutstep)
{
        _hist1dinfovector.push_back({histdef, variable, weight, mincutstep});
}


void NanoAODAnalyzerrdframe::drawHists(RNode t)
{
        cout << "processing" <<endl;
        t.Count();
}

void NanoAODAnalyzerrdframe::addVar(varinfo v)
{
        _varinfovector.push_back(v);
}

void NanoAODAnalyzerrdframe::addVartoStore(string varname)
{
        // varname is assumed to be a regular expression.
        // e.g. if varname is "Muon_eta" then "Muon_eta" will be stored
        // if varname=="Muon_.*", then any branch name that starts with "Muon_" string will
        // be saved
        _varstostore.push_back(varname);
        /*
        std::regex b(varname);
        bool foundmatch = false;
        for (auto a: _rlm.GetColumnNames())
        {
            if (std::regex_match(a, b)) {
                _varstostore.push_back(a);
                foundmatch = true;
            }
        }
        */

}

void NanoAODAnalyzerrdframe::setupTree()
{
        vector<RNodeTree *> rntends;
        _rnt.getRNodeLeafs(rntends);
        for (auto arnt: rntends)
        {
                RNode *arnode = arnt->getRNode();
                string nodename = arnt->getIndex();
                vector<string> varforthistree;
                std::map<string, int> varused;

                for (auto varname: _varstostore)
                {
                        bool foundmatch = false;
                        std::regex b(varname);
                        for (auto a: arnode->GetColumnNames())
                        {
                                if (std::regex_match(a, b) && varused[a]==0)
                                {
                                        varforthistree.push_back(a);
                                        varused[a]++;
                                        foundmatch = true;
                                }
                        }
                        if (!foundmatch)
                        {
                                cout << varname << " not found at "<< nodename << endl;
                        }

                }
                _varstostorepertree[nodename]  = varforthistree;
        }

}

void NanoAODAnalyzerrdframe::addCuts(string cut, string idx)
{
        _cutinfovector.push_back({cut, idx});
}


void NanoAODAnalyzerrdframe::run(bool saveAll, string outtreename)
{
        /*
        if (saveAll) {
                _rlm.Snapshot(outtreename, _outfilename);
        }
        else {
                // use the following if you want to store only a few variables
                _rlm.Snapshot(outtreename, _outfilename, _varstostore);
        }
        */

        vector<RNodeTree *> rntends;
        _rnt.getRNodeLeafs(rntends);
        _rnt.Print();
        cout << rntends.size() << endl;
        // on master, regex_replace doesn't work somehow
        //std::regex rootextension("\\.root");

        for (auto arnt: rntends)
        {
                string nodename = arnt->getIndex();
                //string outname = std::regex_replace(_outfilename, rootextension, "_"+nodename+".root");
                string outname = _outfilename;
                // if producing many root files due to branched selection criteria,  each root file will get a different name
                if (rntends.size()>1) outname.replace(outname.find(".root"), 5, "_"+nodename+".root");
                _outrootfilenames.push_back(outname);
                RNode *arnode = arnt->getRNode();
                cout << arnt->getIndex();
                //cout << ROOT::RDF::SaveGraph(_rlm) << endl;
                if (saveAll) {
                        arnode->Snapshot(outtreename, outname);
                }
                else {
                        // use the following if you want to store only a few variables
                        //arnode->Snapshot(outtreename, outname, _varstostore);
                        cout << " writing branches" << endl;
                        for (auto bname: _varstostorepertree[nodename])
                        {
                                cout << bname << endl;
                        }
                        arnode->Snapshot(outtreename, outname, _varstostorepertree[nodename]);
                }
        }



}
