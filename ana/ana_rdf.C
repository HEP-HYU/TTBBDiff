#include "ROOT/RDataFrame.hxx"
#include "ROOT/RVec.hxx"
#include "ROOT/RDF/RInterface.hxx"
#include "TCanvas.h"
#include "TH1D.h"
#include "TLatex.h"
#include "Math/Vector4D.h"
#include "TStyle.h"
#include "TLegend.h"
#include "Math/Vector4Dfwd.h"
#include <vector>
//#include "xsection.h"

using namespace ROOT::VecOps;
//using rvec_f = const RVec<float> &;
using rvec_f = RVec<float>;

//selected bjets (temporal)
vector<int> bjet_idx(rvec_f pt, rvec_f eta, rvec_f scale, rvec_f btag)
{
  vector<int> out;
  for( int i = 0; i < pt.size(); i++){
    if( pt[i]*scale[i] > 30 && abs(eta[i]) < 2.4 && btag[i] > 0.7527){
      out.push_back(i);
    } 
  }
  return out;
}

//selected jets
vector<int> jet_idx(rvec_f pt, rvec_f eta, rvec_f scale)
{
  vector<int> out;
  for( int i = 0; i < pt.size(); i++){
    if( pt[i]*scale[i] > 30 && abs(eta[i]) < 2.4){
      out.push_back(i);
    } 
  }
  return out;
}

// Compute mass of Higgs from four leptons of the same kind
float compute_mass(rvec_f pt, rvec_f eta, rvec_f phi, rvec_f energy, rvec_f btag)
{
    float dR = 999;
    float mbb = 999;
    for( int i = 0; i < pt.size()-1 ; i++){
      for( int j = i+1; j < pt.size() ; j++){
        if( btag[i] <=  0.7527 || btag[j] <= 0.7527) continue;
        float tmp = DeltaR(eta[i], eta[j], phi[i], phi[j]);
        if(tmp < dR) {
          ROOT::Math::PtEtaPhiEVector p1(pt[i], eta[i], phi[i], energy[i]);
          ROOT::Math::PtEtaPhiEVector p2(pt[j], eta[j], phi[j], energy[j]);
          mbb = (p1 + p2).M();
          dR = tmp;
        }    
      } 
    }

    return mbb;
}

float compute_dR(rvec_f pt, rvec_f eta, rvec_f phi, rvec_f energy, rvec_f btag)
{

    float dR = 999;
    float mbb = 999;
    for( int i = 0; i < pt.size()-1 ; i++){
      for( int j = i+1; j < pt.size() ; j++){
        if( btag[i] <=  0.7527 || btag[j] <= 0.7527) continue;
        float tmp = DeltaR(eta[i], eta[j], phi[i], phi[j]);
        if(tmp < dR) {
          dR = tmp;
        }
      }
    }
    return dR;
}

template <typename T>
void plot(T sig, const std::string &name, const std::string &filename){

  auto h = *sig;
  auto c = new TCanvas( name.c_str(), "c", 1);
  h.DrawClone("Hist");
  c->SaveAs(filename.c_str());
}

void ana_rdf(TString name = "TTLJ_PowhegPythia_ttbb", TString ch = "0"){
  // Enable multi-threading
  ROOT::EnableImplicitMT();

  ROOT::RDataFrame df("ttbbLepJets/tree",Form("/cms/ldap_home/sarakm0704/WORK/ttbb/ntuple/Run2018/V10_2/%s.root", name.Data()) );
//  ROOT::RDataFrame df("ttbbLepJets/tree","/cms/ldap_home/sarakm0704/WORK/ttbb/ntuple/Run2017/V9_6/TTLJ_PowhegPythia_ttbb.root"); //2017
//  ROOT::RDataFrame df("ttbbLepJets/tree","/cms/ldap_home/sarakm0704/WORK/ttbb/ntuple/Run2016/v808/TTLJ_PowhegPythia_ttbb.root"); //2016

  TFile *f_info = new TFile(Form("/cms/ldap_home/sarakm0704/WORK/ttbb/ntuple/Run2018/V10_2/%s.root", name.Data()));
  TH1D * EventInfo = (TH1D*) f_info->Get("ttbbLepJets/EventInfo");
  double n = EventInfo->GetBinContent(2);
  double Xsection = 1.0;
/*
  if(  Xsections.find(name.Data()) != Xsections.end() ){
    Xsection = Xsections.find(name)->second ;
  } 
*/
  auto fileName_dnn = Form("dnn_tree_%s_Ch%s.root", name.Data(), ch.Data());
  auto treeName = "tree";
  auto df_ch = df.Filter(Form("channel == %s", ch.Data()) );

  if( name.Contains("Data") ){
    df_ch = df_ch.Define("jet_scale","rvec_f(jet_pt.size(), 1.0f);");
    df_ch = df_ch.Define("weight","1");
  }else{
    df_ch = df_ch.Define("jet_scale","jet_JER_Nom");
    //event weight = genweight * puweight * lepton scale * b jet scale 
    if ( ch.Atoi() == 0 ){
      df_ch = df_ch.Define("weight","genweight*PUWeight[0]*lepton_SF[0]*lepton_SF[3]*lepton_SF[6]*jet_SF_deepCSV_30[0]");
    }else if ( ch.Atoi() == 1 ){
      df_ch = df_ch.Define("weight","genweight*PUWeight[0]*lepton_SF[0]*lepton_SF[3]*lepton_SF[6]*lepton_SF[9]*jet_SF_deepCSV_30[0]");
    }else{ cout << "goes wrong ... channel is " << ch.Data() << endl; }
  }

  //event selection
  //step1
  auto df_S1 = df_ch.Filter("lepton_pt > 30 && abs(lepton_eta) < 2.4", "Events with one lepton");
//  auto df_S1 = df.Filter("lepton_pT > 30 && abs(lepton_eta) < 2.1", "Events with one leptons"); //2016
  //step2
  auto df_S2 = df_S1.Filter("Sum( jet_pt*jet_scale > 30 && abs(jet_eta) < 2.4) >= 6", "Events with at least six jets");
  //auto df_S2 = df_S1.Filter("Sum( jet_pT*jet_JER_Nom > 30 && abs(jet_eta) < 2.4) >= 6", "Events with at least six jets"); //2016
  //step3
  //select b-tagged jets with DeepCSV tagging at tight working point of 0.7527(2018), 0.8001(2017), 0.9535(2016)
  //https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation 
  auto df_S3 = df_S2.Filter("Sum( jet_pt*jet_scale > 30 && abs(jet_eta) < 2.4 && jet_deepCSV >  0.7527) >=3","Require 3 jets to be b-tagged");  

  //Histograms
  //Step 1: Events with one lepton
  auto h_njets = df_S1.Define("njets","Sum(jet_pt*jet_scale > 30 && abs(jet_eta) < 2.4)").Histo1D({"h_njets", "", 10, 0, 10}, "njets","weight");
  auto h_lepton_pt_S1 = df_S1.Define("lepton_pT","lepton_pt").Histo1D({"h_lepton_pt_S1","",20,0,400},"lepton_pT","weight");
  auto h_lepton_eta_S1 = df_S1.Define("lepton_Eta","abs(lepton_eta)").Histo1D({"h_lepton_eta_S1","",20,0,2.5},"lepton_Eta","weight");
  auto h_lepton_phi_S1 = df_S1.Define("lepton_Phi","lepton_phi").Histo1D({"h_lepton_phi_S1","",40,-3.5,3.5},"lepton_Phi","weight");

  //Step 2: Events with at least six jets 
  auto h_nbjets = df_S2.Define("nbjets","Sum(jet_pt*jet_scale > 30 && abs(jet_eta) < 2.4 && jet_deepCSV > 0.7527)").Histo1D({"h_nbjets", "", 5, 0, 5}, "nbjets","weight");

  df_S2 = df_S2.Define("jet_idx",jet_idx,{"jet_pt","jet_eta","jet_scale"});
  
  auto h_jet1_pt_S2 = df_S2.Define("jet1_pT","jet_pt[jet_idx[0]]*jet_scale[jet_idx[0]]").Histo1D({"h_jet1_pt_S2","",20,0,400},"jet1_pT","weight");
  auto h_jet1_eta_S2 = df_S2.Define("jet1_Eta","abs(jet_eta[jet_idx[0]])").Histo1D({"h_jet1_eta_S2","",20,0,2.5},"jet1_Eta","weight");
  auto h_jet2_pt_S2 = df_S2.Define("jet2_pT","jet_pt[jet_idx[1]]*jet_scale[jet_idx[1]]").Histo1D({"h_jet2_pt_S2","",20,0,400},"jet2_pT","weight");
  auto h_jet2_eta_S2 = df_S2.Define("jet2_Eta","abs(jet_eta[jet_idx[1]])").Histo1D({"h_jet2_eta_S2","",20,0,2.5},"jet2_Eta","weight");
  auto h_jet3_pt_S2 = df_S2.Define("jet3_pT","jet_pt[jet_idx[2]]*jet_scale[jet_idx[2]]").Histo1D({"h_jet3_pt_S2","",20,0,400},"jet3_pT","weight");
  auto h_jet3_eta_S2 = df_S2.Define("jet3_Eta","abs(jet_eta[jet_idx[2]])").Histo1D({"h_jet3_eta_S2","",20,0,2.5},"jet3_Eta","weight");
  auto h_jet4_pt_S2 = df_S2.Define("jet4_pT","jet_pt[jet_idx[3]]*jet_scale[jet_idx[3]]").Histo1D({"h_jet4_pt_S2","",20,0,400},"jet4_pT","weight");
  auto h_jet4_eta_S2 = df_S2.Define("jet4_Eta","abs(jet_eta[jet_idx[3]])").Histo1D({"h_jet4_eta_S2","",20,0,2.5},"jet4_Eta","weight");

//  //scale histogram
//  h_njets->Scale();
//  h_lepton_pt_S1->Scale();
//  h_lepton_eta_S1->Scale();
//  h_lepton_phi_S1->Scale();
//  h_nbjets->Scale();
//  h_jet_pt_S2->Scale();
//  h_jet_eta_S2->Scale();
//  h_jet_phi_S2->Scale();

  //Step 3: Require 3 jets to be b-tagged 
  //Define variables
  df_S3 = df_S3.Define("bjet_idx",bjet_idx,{"jet_pt","jet_eta","jet_scale","jet_deepCSV"})
               .Define("mbb", compute_mass, {"jet_pt","jet_eta","jet_phi","jet_e","jet_deepCSV"})
               .Define("dRbb", compute_dR, {"jet_pt","jet_eta","jet_phi","jet_e","jet_deepCSV"});

  auto h_mbb = df_S3.Histo1D({"h_mbb", "", 10, 70, 170}, "mbb");
  auto h_dRbb = df_S3.Histo1D({"h_dRbb", "", 20 , 0, 4}, "dRbb");

  auto h_bjet1_pt_S3 = df_S3.Define("bjet1_pT","jet_pt[bjet_idx[0]]*jet_scale[bjet_idx[0]]").Histo1D({"h_bjet1_pt_S3","",20,0,400},"bjet1_pT","weight");
  auto h_bjet2_pt_S3 = df_S3.Define("bjet2_pT","jet_pt[bjet_idx[1]]*jet_scale[bjet_idx[1]]").Histo1D({"h_bjet2_pt_S3","",20,0,400},"bjet2_pT","weight");


  //create ntuple 
  df_S3.Snapshot(treeName, fileName_dnn, {"mbb","dRbb"});
  TFile f(Form("hist/hist_%s_Ch%s.root", name.Data(), ch.Data()),"recreate");
  h_njets->Write();
  h_lepton_pt_S1->Write();
  h_lepton_eta_S1->Write();
  h_lepton_phi_S1->Write();
  h_nbjets->Write();
  h_jet1_pt_S2->Write();
  h_jet1_eta_S2->Write();
  h_jet2_pt_S2->Write();
  h_jet2_eta_S2->Write();
  h_jet3_pt_S2->Write();
  h_jet3_eta_S2->Write();
  h_jet4_pt_S2->Write();
  h_jet4_eta_S2->Write();
  h_mbb->Write();
  h_dRbb->Write();
  h_bjet1_pt_S3->Write();
  h_bjet2_pt_S3->Write();
  f.Close();

  //count number of events
  auto n_df = df.Count();
  auto n_df_S1 = df_S1.Count();
  auto n_df_S2 = df_S2.Count();
  auto n_df_S3 = df_S3.Count();

  std::cout << Form("%s : number of events = ", name.Data()) << *n_df << " , preselection(S1) = " << *n_df_S1 <<  " , S2 = " << *n_df_S2 << ", final selection(S3) = " << *n_df_S3 << std::endl;

  //plotting 
 // plot(h_nbtag, "ttbb_nbtag", "h_nbtag.pdf");
 // plot(h_njets, "ttbb_njets", "h_njets.pdf");

}
