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
//#include "selection.h"

using namespace ROOT::VecOps;
using rvec_f = RVec<float>;

vector<int> signal_label(const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f phi, rvec_f scale, rvec_f btag, rvec_f ad1_eta, rvec_f ad1_phi, rvec_f ad2_eta, rvec_f ad2_phi){

//    rvec_f label;
    vector<int> label;
    for( size_t i = 0; i < idx.size(); i++ ){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      //need to be simpler
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
      if (!pass_jet) continue;
//      bool pass_btag = btag[i1] >  0.8484 && btag[i2] > 0.8484;
//      if( !(pass_jet && pass_btag) ) continue;

      bool matched_1 = ( DeltaR(eta[i1], ad1_eta[0], phi[i1], ad1_phi[0]) < 0.4 ) && ( DeltaR(eta[i2], ad2_eta[0], phi[i2], ad2_phi[0]) < 0.4 );
      bool matched_2 = ( DeltaR(eta[i1], ad2_eta[0], phi[i1], ad2_phi[0]) < 0.4 ) && ( DeltaR(eta[i2], ad1_eta[0], phi[i2], ad1_phi[0]) < 0.4 );

      if(matched_1 || matched_2) label.push_back(1);
      else label.push_back(0);
    }

    return label;

}

RVec<RVec<size_t>> jet_combi_border(rvec_f pt, rvec_f eta, rvec_f phi, rvec_f energy, rvec_f scale, rvec_f btag){
 
  auto sorted_btag = Reverse(Argsort(btag));//sort as b-discriminator

  //size_t total = pt.size()*(pt.size()-1)/2; //total number of combination 
  RVec<RVec<size_t>> idx(6);
  for(size_t i = 0; i < 6; i++){
    idx[i].reserve(2); 
  }
  //auto idx_cmb = Combinations(pt, 2);
  rvec_f v(4);
  auto idx_cmb = Combinations(v, 2);

  for (size_t i = 0; i < idx_cmb[0].size(); i++) {
    const auto i1 = idx_cmb[0][i];
    const auto i2 = idx_cmb[1][i];
    idx[i].push_back(sorted_btag[i1]);
    idx[i].push_back(sorted_btag[i2]); 
  }
  return idx;
}

//b jet combination
RVec<RVec<size_t>> jet_combi(rvec_f pt, rvec_f eta, rvec_f phi, rvec_f energy, rvec_f scale, rvec_f btag){

  size_t total = pt.size()*(pt.size()-1)/2; //total number of combination 
  RVec<RVec<size_t>> idx(total);
  for(size_t i = 0; i < total; i++){
    idx[i].reserve(2); 
  }

  auto idx_cmb = Combinations(pt, 2);
  for (size_t i = 0; i < idx_cmb[0].size(); i++) {
    const auto i1 = idx_cmb[0][i];
    const auto i2 = idx_cmb[1][i];
    idx[i].emplace_back(i1);
    idx[i].emplace_back(i2); 
  }
  return idx;
}

//selected bjets (temporal)
vector<int> bjet_idx(rvec_f pt, rvec_f eta, rvec_f scale, rvec_f btag)
{
  vector<int> out;
  for( int i = 0; i < pt.size(); i++){
    if( pt[i]*scale[i] > 30 && abs(eta[i]) < 2.4 && btag[i] > 0.8484){
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

vector<float> compute_mass(const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f phi, rvec_f energy, rvec_f scale, rvec_f btag)
{
    vector<float> var;
    for( size_t i = 0; i < idx.size() ; i++){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
      //bool pass_btag = btag[i1] >  0.8484 && btag[i2] > 0.8484;
      if (!pass_jet) continue;
      //if( !(pass_jet && pass_btag) ) continue;
      ROOT::Math::PtEtaPhiEVector p1(pt[i1], eta[i1], phi[i1], energy[i1]);
      ROOT::Math::PtEtaPhiEVector p2(pt[i2], eta[i2], phi[i2], energy[i2]);
      var.push_back((p1 + p2).M());
    }
    return var;
}

vector<float> compute_dR(const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f phi, rvec_f scale, rvec_f btag)
{   
    vector<float> var;
    for( size_t i = 0; i < idx.size() ; i++){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
      //bool pass_btag = btag[i1] >  0.8484 && btag[i2] > 0.8484;
      if (!pass_jet) continue;
//      if( !(pass_jet && pass_btag) ) continue;
      var.push_back( DeltaR(eta[i1], eta[i2], phi[i1], phi[i2]) );
    }   
    return var;
}

vector<float> compute_bt1(const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f scale, rvec_f btag)
{
    vector<float> var;
    for( size_t i = 0; i < idx.size() ; i++){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
      //bool pass_btag = btag[i1] >  0.8484 && btag[i2] > 0.8484;
      if (!pass_jet) continue;
      //if( !(pass_jet && pass_btag) ) continue;
      var.push_back(btag[i1]);
    }
    return var;
}

vector<float> compute_bt2(const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f scale, rvec_f btag)
{
    vector<float> var;
    for( size_t i = 0; i < idx.size() ; i++){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
      //bool pass_btag = btag[i1] >  0.8484 && btag[i2] > 0.8484;
      if (!pass_jet) continue;
      //if( !(pass_jet && pass_btag) ) continue;
      var.push_back(btag[i2]);
    }
    return var;
}

vector<float> compute_bpt1(const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f scale, rvec_f btag)
{
    vector<float> var;
    for( size_t i = 0; i < idx.size() ; i++){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
      //bool pass_btag = btag[i1] >  0.8484 && btag[i2] > 0.8484;
      if (!pass_jet) continue;
      //if( !(pass_jet && pass_btag) ) continue;
      var.push_back(pt[i1]);
    }
    return var;
}

vector<float> compute_bpt2(const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f scale, rvec_f btag)
{
    vector<float> var;
    for( size_t i = 0; i < idx.size() ; i++){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
      //bool pass_btag = btag[i1] >  0.8484 && btag[i2] > 0.8484;
      if (!pass_jet) continue;
      //if( !(pass_jet && pass_btag) ) continue;
      var.push_back(pt[i2]);
    }
    return var;
}

vector<float> compute_beta1(const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f scale, rvec_f btag)
{
    vector<float> var;
    for( size_t i = 0; i < idx.size() ; i++){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
      //bool pass_btag = btag[i1] >  0.8484 && btag[i2] > 0.8484;
      if (!pass_jet) continue;
      //if( !(pass_jet && pass_btag) ) continue;
      var.push_back(eta[i1]);
    }
    return var;
}

vector<float> compute_beta2(const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f scale, rvec_f btag)
{
    vector<float> var;
    for( size_t i = 0; i < idx.size() ; i++){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
      //bool pass_btag = btag[i1] >  0.8484 && btag[i2] > 0.8484;
      if (!pass_jet) continue;
      //if( !(pass_jet && pass_btag) ) continue;
      var.push_back(eta[i2]);
    }
    return var;
}


float draw_mindR(rvec_f pt, rvec_f scale, rvec_f eta, rvec_f phi, rvec_f btag)
{

    float dR = 999;
    float mbb = 999;
    for( int i = 0; i < pt.size()-1 ; i++){
      for( int j = i+1; j < pt.size() ; j++){
        bool pass_jet = pt[i]*scale[i] > 30 && abs(eta[i]) < 2.4 && pt[j]*scale[j] > 30 && abs(eta[j]) < 2.4;
        bool pass_btag = btag[i] >  0.8484 && btag[j] > 0.8484;
        if( !(pass_jet && pass_btag) ) continue;
        float tmp = DeltaR(eta[i], eta[j], phi[i], phi[j]);
        if(tmp < dR) {
          dR = tmp;
        }
      }
    }
    return dR;
}

float draw_mass(rvec_f pt, rvec_f scale, rvec_f eta, rvec_f phi, rvec_f energy, rvec_f btag)
{
    float dR = 999;
    float mbb = 999;
    for( int i = 0; i < pt.size()-1 ; i++){
      for( int j = i+1; j < pt.size() ; j++){
        bool pass_jet = pt[i]*scale[i] > 30 && abs(eta[i]) < 2.4 && pt[j]*scale[j] > 30 && abs(eta[j]) < 2.4;
        bool pass_btag = btag[i] >  0.8484 && btag[j] > 0.8484;
        if( !(pass_jet && pass_btag) ) continue;
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


template <typename T>
void plot(T sig, const std::string &name, const std::string &filename){

  auto h = *sig;
  auto c = new TCanvas( name.c_str(), "c", 1);
  h.DrawClone("Hist");
  c->SaveAs(filename.c_str());
}

void ana_rdf(TString year = "2018", TString CATver = "V10_2", TString name = "TTLJ_PowhegPythia_ttbb", TString ch = "0", TString dnn = "1"){
  // Enable multi-threading
  //ROOT::EnableImplicitMT();

//  ROOT::RDataFrame df("ttbbLepJets/tree",Form("/cms/ldap_home/sarakm0704/public/ntuple/Run%s/%s/%s.root", year.Data(), CATver.Data(), name.Data()) ); //all
  ROOT::RDataFrame df("ttbbLepJets/tree",Form("/xrootd/store/user/sarakm0704/ntuple/Run%s/%s/%s.root", year.Data(), CATver.Data(), name.Data()) ); //from xrootd

/*
  TFile *f_info = new TFile(Form("/cms/ldap_home/sarakm0704/public/ntuple/Run2018/V10_2/%s.root", name.Data()));
  TH1D * EventInfo = (TH1D*) f_info->Get("ttbbLepJets/EventInfo");
  double n = EventInfo->GetBinContent(2);
  double Xsection = 1.0;

  if(  Xsections.find(name.Data()) != Xsections.end() ){
    Xsection = Xsections.find(name)->second ;
  } 
*/
  auto df_ch = df.Filter(Form("channel == %s", ch.Data()),"lepton channel");

  if( name.Contains("Data") ){
    if ( year.Atoi() == 2016 ) df_ch = df_ch.Define("jet_scale","rvec_f(jet_pT.size(), 1.0f);");
    else df_ch = df_ch.Define("jet_scale","rvec_f(jet_pt.size(), 1.0f);");

    df_ch = df_ch.Define("weight","1");
    df_ch = df_ch.Define("b_weight","1");
  }else{
    df_ch = df_ch.Define("jet_scale","jet_JER_Nom");

    if ( ch.Atoi() == 0 ){
      if ( year.Atoi() == 2016 ){
        df_ch = df_ch.Define("weight","genweight*PUWeight[0]*lepton_SF[0]*lepton_SF[3]");
        df_ch = df_ch.Define("b_weight","genweight*PUWeight[0]*lepton_SF[0]*lepton_SF[3]*jet_SF_CSV_30[0]");
      }else if ( year.Atoi() == 2017 ){
        df_ch = df_ch.Define("weight","genweight*PUWeight[0]*prefireweight[0]*lepton_SF[0]*lepton_SF[3]*lepton_SF[6]");
        df_ch = df_ch.Define("b_weight","genweight*PUWeight[0]*prefireweight[0]*lepton_SF[0]*lepton_SF[3]*lepton_SF[6]*jet_SF_deepCSV_30[0]");    
      }else if (year.Atoi() == 2018 ){
        df_ch = df_ch.Define("weight","genweight*PUWeight[0]*lepton_SF[0]*lepton_SF[3]*lepton_SF[6]");
        df_ch = df_ch.Define("b_weight","genweight*PUWeight[0]*lepton_SF[0]*lepton_SF[3]*lepton_SF[6]*jet_SF_deepCSV_30[0]");
      }else cout << "Invalid year:: " << year.Data() << endl;

    }else if ( ch.Atoi() == 1 ){
      if ( year.Atoi() == 2016 ){
        df_ch = df_ch.Define("weight","genweight*PUWeight[0]*lepton_SF[0]*lepton_SF[3]");
        df_ch = df_ch.Define("b_weight","genweight*PUWeight[0]*lepton_SF[0]*lepton_SF[3]*jet_SF_CSV_30[0]");
      }else if ( year.Atoi() == 2017 ){
        df_ch = df_ch.Define("weight","genweight*PUWeight[0]*prefireweight[0]*lepton_SF[0]*lepton_SF[3]*lepton_SF[6]*lepton_SF[9]");
        df_ch = df_ch.Define("b_weight","genweight*PUWeight[0]*prefireweight[0]*lepton_SF[0]*lepton_SF[3]*lepton_SF[6]*lepton_SF[9]*jet_SF_deepCSV_30[0]");    
      }else if (year.Atoi() == 2018 ){
        df_ch = df_ch.Define("weight","genweight*PUWeight[0]*lepton_SF[0]*lepton_SF[3]*lepton_SF[6]");
        df_ch = df_ch.Define("b_weight","genweight*PUWeight[0]*lepton_SF[0]*lepton_SF[3]*lepton_SF[6]*jet_SF_deepCSV_30[0]");
      }else cout << "Invalid year:: " << year.Data() << endl;
    }else cout << "Invalid channel:: " << ch.Data() << endl;
  }

  //event selection
  //step1
  auto df_S1 = df_ch;

  if( year.Atoi() == 2016 ){
    df_S1 = df_S1.Define("lepton_pt","lepton_pT")
                 .Define("jet_pt","jet_pT")
                 .Define("jet_e","jet_E")
                 .Define("jet_deepCSV","jet_CSV");

    if (ch.Atoi() == 0) df_S1 = df_S1.Filter("lepton_pt > 30 && abs(lepton_eta) < 2.1");
    else if (ch.Atoi() == 1) df_S1 = df_S1.Filter("lepton_pt > 35 && abs(lepton_eta) < 2.1");
  }else{ //2017, 2018
     df_S1 = df_S1.Filter("lepton_pt > 30 && abs(lepton_eta) < 2.4");
  }

  df_S1 = df_S1.Filter("Sum( jet_pt*jet_scale > 30 && abs(jet_eta) < 2.4) >= 1", "Events with one lepton and one jet")
               .Define("njets","Sum(jet_pt*jet_scale > 30 && abs(jet_eta) < 2.4)");
  
  auto h_njets_S1 = df_S1.Histo1D({"h_njets_S1", "", 5, 0, 5}, "njets","weight");
  auto h_lepton_pt_S1 = df_S1.Define("Lepton_pT","lepton_pt").Histo1D({"h_lepton_pt_S1","",20,0,400},"Lepton_pT","weight");
  auto h_lepton_eta_S1 = df_S1.Define("Lepton_Eta","abs(lepton_eta)").Histo1D({"h_lepton_eta_S1","",20,0,2.5},"Lepton_Eta","weight");
  auto h_lepton_phi_S1 = df_S1.Define("Lepton_Phi","lepton_phi").Histo1D({"h_lepton_phi_S1","",40,-3.5,3.5},"Lepton_Phi","weight");

  //step2
  auto df_S2 = df_S1.Filter("Sum( jet_pt*jet_scale > 30 && abs(jet_eta) < 2.4) >= 6", "Events with at least six jets");   
  df_S2 = df_S2.Define("jet_idx",jet_idx,{"jet_pt","jet_eta","jet_scale"})
               .Define("nbjets","Sum(jet_pt*jet_scale > 30 && abs(jet_eta) < 2.4 && jet_deepCSV > 0.8484)");  //2016 CSVv2M
//               .Define("nbjets","Sum(jet_pt*jet_scale > 30 && abs(jet_eta) < 2.4 && jet_deepCSV > 0.7527)"); //2018 DeepCSVT
//               .Define("nbjets","Sum(jet_pt*jet_scale > 30 && abs(jet_eta) < 2.4 && jet_deepCSV > 0.8001)"); //2017 DeepCSVT

  auto h_nbjets_S2 = df_S2.Histo1D({"h_nbjets_S2", "", 5, 0, 5}, "nbjets","weight");
  auto h_jet1_pt_S2 = df_S2.Define("jet1_pT","jet_pt[jet_idx[0]]*jet_scale[jet_idx[0]]").Histo1D({"h_jet1_pt_S2","",20,0,400},"jet1_pT","weight");
  auto h_jet1_eta_S2 = df_S2.Define("jet1_Eta","abs(jet_eta[jet_idx[0]])").Histo1D({"h_jet1_eta_S2","",20,0,2.5},"jet1_Eta","weight");
  auto h_jet2_pt_S2 = df_S2.Define("jet2_pT","jet_pt[jet_idx[1]]*jet_scale[jet_idx[1]]").Histo1D({"h_jet2_pt_S2","",20,0,400},"jet2_pT","weight");
  auto h_jet2_eta_S2 = df_S2.Define("jet2_Eta","abs(jet_eta[jet_idx[1]])").Histo1D({"h_jet2_eta_S2","",20,0,2.5},"jet2_Eta","weight");
  auto h_jet3_pt_S2 = df_S2.Define("jet3_pT","jet_pt[jet_idx[2]]*jet_scale[jet_idx[2]]").Histo1D({"h_jet3_pt_S2","",20,0,400},"jet3_pT","weight");
  auto h_jet3_eta_S2 = df_S2.Define("jet3_Eta","abs(jet_eta[jet_idx[2]])").Histo1D({"h_jet3_eta_S2","",20,0,2.5},"jet3_Eta","weight");
  auto h_jet4_pt_S2 = df_S2.Define("jet4_pT","jet_pt[jet_idx[3]]*jet_scale[jet_idx[3]]").Histo1D({"h_jet4_pt_S2","",20,0,400},"jet4_pT","weight");
  auto h_jet4_eta_S2 = df_S2.Define("jet4_Eta","abs(jet_eta[jet_idx[3]])").Histo1D({"h_jet4_eta_S2","",20,0,2.5},"jet4_Eta","weight");

  if( dnn.Atoi() == true ){
//    df_S3 = df_S3.Define("jet_combi_idx", jet_combi, {"jet_pt","jet_eta","jet_phi","jet_e","jet_scale","jet_deepCSV"})
      df_S2 = df_S2.Define("jet_combi_idx", jet_combi_border, {"jet_pt","jet_eta","jet_phi","jet_e","jet_scale","jet_deepCSV"}) //sort as b-discrimminator order
                 .Define("mbb", compute_mass, {"jet_combi_idx","jet_pt","jet_eta","jet_phi","jet_e","jet_scale","jet_deepCSV"})
                 .Define("dRbb", compute_dR, {"jet_combi_idx","jet_pt","jet_eta","jet_phi","jet_scale","jet_deepCSV"})

                 .Define("pt1", compute_bpt1, {"jet_combi_idx","jet_pt","jet_eta","jet_scale","jet_deepCSV"})
                 .Define("pt2", compute_bpt2, {"jet_combi_idx","jet_pt","jet_eta","jet_scale","jet_deepCSV"})

                 .Define("bt1", compute_bt1, {"jet_combi_idx","jet_pt","jet_eta","jet_scale","jet_deepCSV"})
                 .Define("bt2", compute_bt2, {"jet_combi_idx","jet_pt","jet_eta","jet_scale","jet_deepCSV"})

                 .Define("eta1", compute_beta1, {"jet_combi_idx","jet_pt","jet_eta","jet_scale","jet_deepCSV"})
                 .Define("eta2", compute_beta2, {"jet_combi_idx","jet_pt","jet_eta","jet_scale","jet_deepCSV"});

    df_S2.Snapshot("dnn_tree", Form("dnn_tree/dnn_%s_Ch%s.root", name.Data(), ch.Data()), {"event","mbb","dRbb","b_weight","pt1","eta1","pt2","eta2","bt1","bt2"});

    if( name.Contains("ttbb") ){
      df_S2 = df_S2.Define("signal", signal_label, {"jet_combi_idx","jet_pt","jet_eta","jet_phi","jet_scale","jet_deepCSV","addbjet1_eta","addbjet1_phi","addbjet2_eta","addbjet2_phi"});
      df_S2.Snapshot("dnn_tree", Form("dnn_tree/dnn_%s_Ch%s.root", name.Data(), ch.Data()), {"event","signal","mbb","dRbb","b_weight","pt1","eta1","pt2","eta2","bt1","bt2"});
    }
  }
  //step3
  //select b-tagged jets with DeepCSV tagging at tight working point of 0.7527(2018), 0.8001(2017), medium 0.8484(2016ReReco)
  //https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation 
//  auto df_S3 = df_S2.Filter("Sum( jet_pt*jet_scale > 30 && abs(jet_eta) < 2.4 && jet_deepCSV >  0.7527) >=3","Require 3 jets to be b-tagged");  //2018 DeepCSVT
//  auto df_S3 = df_S2.Filter("Sum( jet_pt*jet_scale > 30 && abs(jet_eta) < 2.4 && jet_deepCSV >  0.8001) >=3","Require 3 jets to be b-tagged");  //2017 DeepCSVT
  auto df_S3 = df_S2.Filter("Sum( jet_pt*jet_scale > 30 && abs(jet_eta) < 2.4 && jet_deepCSV > 0.8484) >=4","Require 4 jets to be b-tagged");  //2016 CSVv2M

  df_S3 = df_S3.Define("bjet_idx",bjet_idx,{"jet_pt","jet_eta","jet_scale","jet_deepCSV"})
               .Define("min_Mass", draw_mass, {"jet_pt","jet_scale","jet_eta","jet_phi","jet_e","jet_deepCSV"})
               .Define("min_dR", draw_mindR, {"jet_pt","jet_scale","jet_eta","jet_phi","jet_deepCSV"});

  auto h_minMass_S3 = df_S3.Histo1D({"h_minMass_S3","",20,0,400},"min_Mass","b_weight");
  auto h_mindR_S3 = df_S3.Histo1D({"h_mindR_S3","",20,0,4},"min_dR","b_weight");

  auto h_bjet1_pt_S3 = df_S3.Define("bjet1_pT","jet_pt[bjet_idx[0]]*jet_scale[bjet_idx[0]]").Histo1D({"h_bjet1_pt_S3","",20,0,400},"bjet1_pT","b_weight");
  auto h_bjet2_pt_S3 = df_S3.Define("bjet2_pT","jet_pt[bjet_idx[1]]*jet_scale[bjet_idx[1]]").Histo1D({"h_bjet2_pt_S3","",20,0,400},"bjet2_pT","b_weight");

//instant number?
//  df_var = df_S3.Define("number","rvec_f(jet_pt.size(), 1.0f);").Define("test1", compute_var, {"jet_combi_idx","jet_pt","jet_eta","jet_phi","jet_scale","jet_deepCSV","number"});

  //for DNN

//    auto h_mbb_S3 = df_S3.Histo1D({"h_mbb_S3", "", 10, 70, 170}, "mbb", "b_weight");
//    auto h_dRbb_S3 = df_S3.Histo1D({"h_dRbb_S3", "", 20 , 0, 4}, "dRbb", "b_weight");

//  //scale histogram 
//  h_njets_S1->Scale();
//  h_lepton_pt_S1->Scale();
//  h_lepton_eta_S1->Scale();
//  h_lepton_phi_S1->Scale();
//  h_nbjets_S2->Scale();
//  h_jet_pt_S2->Scale();
//  h_jet_eta_S2->Scale();
//  h_jet_phi_S2->Scale();

  //create ntuple
  TFile f(Form("hist/hist_%s_Ch%s.root", name.Data(), ch.Data()),"recreate");
  h_njets_S1->Write();
  h_lepton_pt_S1->Write();
  h_lepton_eta_S1->Write();
  h_lepton_phi_S1->Write();
  h_nbjets_S2->Write();
  h_jet1_pt_S2->Write();
  h_jet1_eta_S2->Write();
  h_jet2_pt_S2->Write();
  h_jet2_eta_S2->Write();
  h_jet3_pt_S2->Write();
  h_jet3_eta_S2->Write();
  h_jet4_pt_S2->Write();
  h_jet4_eta_S2->Write();
  h_bjet1_pt_S3->Write();
  h_bjet2_pt_S3->Write();
  h_mindR_S3->Write();
  h_minMass_S3->Write();
  f.Close();

  //count number of events
  auto n_df = df.Count();
  auto n_df_S1 = df_S1.Count();
  auto n_df_S2 = df_S2.Count();
  auto n_df_S3 = df_S3.Count();

  std::cout << Form("Sample = %s, Ch=%s : number of events = ", name.Data(), ch.Data()) << *n_df << " , preselection(S1) = " << *n_df_S1 <<  " , S2 = " << *n_df_S2 << ", final selection(S3) = " << *n_df_S3 << std::endl;

  //plotting 
 // plot(h_nbtag, "ttbb_nbtag", "h_nbtag.pdf");
 // plot(h_njets, "ttbb_njets", "h_njets.pdf");

}
