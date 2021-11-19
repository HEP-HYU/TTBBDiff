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

using namespace ROOT::VecOps;
using rvec_f = RVec<float>;

vector<int> signal_label(const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f phi, rvec_f scale, rvec_f btag, rvec_f btag_wp, rvec_f ad1_eta, rvec_f ad1_phi, rvec_f ad2_eta, rvec_f ad2_phi){

//    rvec_f label;
    vector<int> label;
    for( size_t i = 0; i < idx.size(); i++ ){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      //need to be simpler
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
//      if (!pass_jet) continue;
      bool pass_btag = btag[i1] > btag_wp[0] && btag[i2] > btag_wp[0];
      if( !(pass_jet && pass_btag) ) continue;

      bool matched_1 = ( DeltaR(eta[i1], ad1_eta[0], phi[i1], ad1_phi[0]) < 0.4 ) && ( DeltaR(eta[i2], ad2_eta[0], phi[i2], ad2_phi[0]) < 0.4 );
      bool matched_2 = ( DeltaR(eta[i1], ad2_eta[0], phi[i1], ad2_phi[0]) < 0.4 ) && ( DeltaR(eta[i2], ad1_eta[0], phi[i2], ad1_phi[0]) < 0.4 );

      if(matched_1 || matched_2) label.push_back(1);
      else label.push_back(0);
    }

    return label;

}

RVec<RVec<size_t>> jet_combi_border(rvec_f pt, rvec_f eta, rvec_f phi, rvec_f energy, rvec_f scale, rvec_f btag, rvec_f btag_wp){
 
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
RVec<RVec<size_t>> jet_combi(rvec_f pt, rvec_f eta, rvec_f phi, rvec_f energy, rvec_f scale, rvec_f btag, rvec_f btag_wp){

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
vector<int> bjet_idx(rvec_f pt, rvec_f eta, rvec_f scale, rvec_f btag, rvec_f btag_wp)
{
  vector<int> out;
  for( int i = 0; i < pt.size(); i++){
    if( pt[i]*scale[i] > 30 && abs(eta[i]) < 2.4 && btag[i] > btag_wp[0]){
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

vector<float> compute_dRlnub2( const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f phi, rvec_f scale, rvec_f btag, rvec_f btag_wp, float lep_pt, float lep_eta, float lep_phi, float lep_e, float met_pt, float met_phi )
{
    vector<float> var;
    ROOT::Math::PtEtaPhiMVector nu(met_pt, 0.0f, met_phi, met_pt);
    ROOT::Math::PtEtaPhiMVector lep(lep_pt, lep_eta, lep_phi, lep_e);
    float Wleta = (nu+lep).Eta();
    float Wlphi = (nu+lep).Phi();
    for( size_t i = 0; i < idx.size() ; i++){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
//      if (!pass_jet) continue;
      bool pass_btag = btag[i1] > btag_wp[0] && btag[i2] > btag_wp[0];
      if( !(pass_jet && pass_btag) ) continue;
      var.push_back( DeltaR( Wleta, eta[i2], Wlphi, phi[i2]) );
    }
    return var;
}

vector<float> compute_dRlnub1( const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f phi, rvec_f scale, rvec_f btag, rvec_f btag_wp, float lep_pt, float lep_eta, float lep_phi, float lep_e, float met_pt, float met_phi )
{
    vector<float> var;
    ROOT::Math::PtEtaPhiMVector nu(met_pt, 0.0f, met_phi, met_pt);
    ROOT::Math::PtEtaPhiMVector lep(lep_pt, lep_eta, lep_phi, lep_e);
    float Wleta = (nu+lep).Eta();
    float Wlphi = (nu+lep).Phi();
    for( size_t i = 0; i < idx.size() ; i++){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
//      if (!pass_jet) continue;
      bool pass_btag = btag[i1] > btag_wp[0] && btag[i2] > btag_wp[0];
      if( !(pass_jet && pass_btag) ) continue;
      var.push_back( DeltaR( Wleta, eta[i1], Wlphi, phi[i1]) );
    }
    return var;
}

vector<float> compute_dRlnubb( const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f phi, rvec_f energy, rvec_f scale, rvec_f btag, rvec_f btag_wp, float lep_pt, float lep_eta, float lep_phi, float lep_e, float met_pt, float met_phi )
{
    vector<float> var;
    ROOT::Math::PtEtaPhiMVector nu(met_pt, 0.0f, met_phi, met_pt);
    ROOT::Math::PtEtaPhiMVector lep(lep_pt, lep_eta, lep_phi, lep_e);
    float Wleta = (nu+lep).Eta();
    float Wlphi = (nu+lep).Phi();
    for( size_t i = 0; i < idx.size() ; i++){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
//      if (!pass_jet) continue;
      bool pass_btag = btag[i1] > btag_wp[0] && btag[i2] > btag_wp[0];
      if( !(pass_jet && pass_btag) ) continue;
      ROOT::Math::PtEtaPhiMVector p1(pt[i1], eta[i1], phi[i1], energy[i1]);
      ROOT::Math::PtEtaPhiMVector p2(pt[i2], eta[i2], phi[i2], energy[i2]);
      float teta = (p1+p2).Eta();
      float tphi = (p1+p2).Phi();
      var.push_back( DeltaR( Wleta, teta, Wlphi, tphi) );
    }
    return var;
}

vector<float> compute_massnubb(const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f phi, rvec_f energy, rvec_f scale, rvec_f btag, rvec_f btag_wp, float met_pt, float met_phi)
{
    vector<float> var;
    ROOT::Math::PtEtaPhiMVector nu(met_pt, 0.0f, met_phi, met_pt);
    for( size_t i = 0; i < idx.size() ; i++){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
//      if (!pass_jet) continue;
      bool pass_btag = btag[i1] > btag_wp[0] && btag[i2] > btag_wp[0];
      if( !(pass_jet && pass_btag) ) continue;
      ROOT::Math::PtEtaPhiMVector p1(pt[i1], eta[i1], phi[i1], energy[i1]);
      ROOT::Math::PtEtaPhiMVector p2(pt[i2], eta[i2], phi[i2], energy[i2]);
      var.push_back((p1 + p2 + nu).M());
    }
    return var;
}

vector<float> compute_dRnubb( const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f phi, rvec_f energy, rvec_f scale, rvec_f btag, rvec_f btag_wp, float met_phi )
{
    vector<float> var;
    for( size_t i = 0; i < idx.size() ; i++){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
      //if (!pass_jet) continue;
      bool pass_btag = btag[i1] > btag_wp[0] && btag[i2] > btag_wp[0];
      if( !(pass_jet && pass_btag) ) continue;
      ROOT::Math::PtEtaPhiMVector p1(pt[i1], eta[i1], phi[i1], energy[i1]);
      ROOT::Math::PtEtaPhiMVector p2(pt[i2], eta[i2], phi[i2], energy[i2]);
      float teta = (p1+p2).Eta();
      float tphi = (p1+p2).Phi();
      var.push_back( DeltaR( teta, 0.0f, tphi, met_phi) );
    }
    return var;
}

vector<float> compute_massnub2( const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f phi, rvec_f energy, rvec_f scale, rvec_f btag, rvec_f btag_wp, float met_pt, float met_phi )
{
    vector<float> var;
    ROOT::Math::PtEtaPhiMVector nu(met_pt, 0.0f, met_phi, met_pt);
    for( size_t i = 0; i < idx.size() ; i++){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
      //if (!pass_jet) continue;
      bool pass_btag = btag[i1] > btag_wp[0] && btag[i2] > btag_wp[0];
      if( !(pass_jet && pass_btag) ) continue;
      ROOT::Math::PtEtaPhiMVector p2(pt[i2], eta[i2], phi[i2], energy[i2]);
      var.push_back((p2 + nu).M());
    }
    return var;
}

vector<float> compute_massnub1( const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f phi, rvec_f energy, rvec_f scale, rvec_f btag, rvec_f btag_wp, float met_pt, float met_phi )
{
    vector<float> var;
    ROOT::Math::PtEtaPhiMVector nu(met_pt, 0.0f, met_phi, met_pt);
    for( size_t i = 0; i < idx.size() ; i++){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
      //if (!pass_jet) continue;
      bool pass_btag = btag[i1] > btag_wp[0] && btag[i2] > btag_wp[0];
      if( !(pass_jet && pass_btag) ) continue;
      ROOT::Math::PtEtaPhiMVector p1(pt[i1], eta[i1], phi[i1], energy[i1]);
      var.push_back((p1 + nu).M());
    }
    return var;
}

vector<float> compute_dRnub2( const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f phi, rvec_f scale, rvec_f btag, rvec_f btag_wp, float met_phi )
{
    vector<float> var;
    for( size_t i = 0; i < idx.size() ; i++){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
      //if (!pass_jet) continue;
      bool pass_btag = btag[i1] > btag_wp[0] && btag[i2] > btag_wp[0];
      if( !(pass_jet && pass_btag) ) continue;
      var.push_back( DeltaR( eta[i2], 0.0f, phi[i2], met_phi) );
    }
    return var;
}

vector<float> compute_dRnub1( const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f phi, rvec_f scale, rvec_f btag, rvec_f btag_wp, float met_phi )
{
    vector<float> var;
    for( size_t i = 0; i < idx.size() ; i++){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
      //if (!pass_jet) continue;
      bool pass_btag = btag[i1] > btag_wp[0] && btag[i2] > btag_wp[0];
      if( !(pass_jet && pass_btag) ) continue;
      var.push_back( DeltaR( eta[i1], 0.0f, phi[i1], met_phi) );
    }
    return var;
}

vector<float> compute_masslbb(const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f phi, rvec_f energy, rvec_f scale, rvec_f btag, rvec_f btag_wp, float lep_pt, float lep_eta, float lep_phi, float lep_e)
{
    vector<float> var;
    ROOT::Math::PtEtaPhiMVector lep(lep_pt, lep_eta, lep_phi, lep_e);
    for( size_t i = 0; i < idx.size() ; i++){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
      //if (!pass_jet) continue;
      bool pass_btag = btag[i1] > btag_wp[0] && btag[i2] > btag_wp[0];
      if( !(pass_jet && pass_btag) ) continue;
      ROOT::Math::PtEtaPhiMVector p1(pt[i1], eta[i1], phi[i1], energy[i1]);
      ROOT::Math::PtEtaPhiMVector p2(pt[i2], eta[i2], phi[i2], energy[i2]);
      var.push_back((p1 + p2 + lep).M());
    }
    return var;
}

vector<float> compute_dRlbb( const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f phi, rvec_f energy, rvec_f scale, rvec_f btag, rvec_f btag_wp, float lep_eta, float lep_phi )
{
    vector<float> var;
    for( size_t i = 0; i < idx.size() ; i++){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
      //if (!pass_jet) continue;
      bool pass_btag = btag[i1] > btag_wp[0] && btag[i2] > btag_wp[0];
      if( !(pass_jet && pass_btag) ) continue;
      ROOT::Math::PtEtaPhiMVector p1(pt[i1], eta[i1], phi[i1], energy[i1]);
      ROOT::Math::PtEtaPhiMVector p2(pt[i2], eta[i2], phi[i2], energy[i2]);
      float teta = (p1+p2).Eta();
      float tphi = (p1+p2).Phi();
      var.push_back( DeltaR( teta, lep_eta, tphi, lep_phi) );
    }
    return var;
}

vector<float> compute_masslb2( const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f phi, rvec_f energy, rvec_f scale, rvec_f btag, rvec_f btag_wp, float lep_pt, float lep_eta, float lep_phi, float lep_e )
{
    vector<float> var;
    ROOT::Math::PtEtaPhiMVector lep(lep_pt, lep_eta, lep_phi, lep_e);
    for( size_t i = 0; i < idx.size() ; i++){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
      //if (!pass_jet) continue;
      bool pass_btag = btag[i1] > btag_wp[0] && btag[i2] > btag_wp[0];
      if( !(pass_jet && pass_btag) ) continue;   
      ROOT::Math::PtEtaPhiMVector p2(pt[i2], eta[i2], phi[i2], energy[i2]);
      var.push_back( (p2 + lep).M() );
    }
    return var;
}

vector<float> compute_masslb1( const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f phi, rvec_f energy, rvec_f scale, rvec_f btag, rvec_f btag_wp, float lep_pt, float lep_eta, float lep_phi, float lep_e )
{
    vector<float> var;
    ROOT::Math::PtEtaPhiMVector lep(lep_pt, lep_eta, lep_phi, lep_e);
    for( size_t i = 0; i < idx.size() ; i++){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
      //if (!pass_jet) continue;
      bool pass_btag = btag[i1] > btag_wp[0] && btag[i2] > btag_wp[0];
      if( !(pass_jet && pass_btag) ) continue;   
      ROOT::Math::PtEtaPhiMVector p1(pt[i1], eta[i1], phi[i1], energy[i1]);
      var.push_back( (p1 + lep).M() );
    }
    return var;
}

vector<float> compute_dRlb2( const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f phi, rvec_f scale, rvec_f btag, rvec_f btag_wp, float lep_eta, float lep_phi )
{
    vector<float> var;
    for( size_t i = 0; i < idx.size() ; i++){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
      //if (!pass_jet) continue;
      bool pass_btag = btag[i1] > btag_wp[0] && btag[i2] > btag_wp[0];
      if( !(pass_jet && pass_btag) ) continue;
      var.push_back( DeltaR( eta[i2], lep_eta, phi[i2], lep_phi) );
    }
    return var;
}

vector<float> compute_dRlb1( const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f phi, rvec_f scale, rvec_f btag, rvec_f btag_wp, float lep_eta, float lep_phi )
{
    vector<float> var;
    for( size_t i = 0; i < idx.size() ; i++){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
      //if (!pass_jet) continue;
      bool pass_btag = btag[i1] > btag_wp[0] && btag[i2] > btag_wp[0];
      if( !(pass_jet && pass_btag) ) continue;
      var.push_back( DeltaR( eta[i1], lep_eta, phi[i1], lep_phi) );
    }
    return var;
}

vector<float> compute_dPhi(const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f phi, rvec_f scale, rvec_f btag, rvec_f btag_wp)
{
    vector<float> var;
    for( size_t i = 0; i < idx.size() ; i++){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
      //if (!pass_jet) continue;
      bool pass_btag = btag[i1] > btag_wp[0] && btag[i2] > btag_wp[0];
      if( !(pass_jet && pass_btag) ) continue;
      var.push_back(phi[i1]-phi[i2]);
    }
    return var;
}


vector<float> compute_dEta(const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f scale, rvec_f btag, rvec_f btag_wp)
{
    vector<float> var;
    for( size_t i = 0; i < idx.size() ; i++){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
      //if (!pass_jet) continue;
      bool pass_btag = btag[i1] > btag_wp[0] && btag[i2] > btag_wp[0];
      if( !(pass_jet && pass_btag) ) continue;
      var.push_back(eta[i1]-eta[i2]);
    }
    return var;
}

vector<float> compute_Eta(const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f phi, rvec_f energy, rvec_f scale, rvec_f btag, rvec_f btag_wp)
{
    vector<float> var;
    for( size_t i = 0; i < idx.size() ; i++){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
      //if (!pass_jet) continue;
      bool pass_btag = btag[i1] > btag_wp[0] && btag[i2] > btag_wp[0];
      if( !(pass_jet && pass_btag) ) continue;
      ROOT::Math::PtEtaPhiMVector p1(pt[i1], eta[i1], phi[i1], energy[i1]);
      ROOT::Math::PtEtaPhiMVector p2(pt[i2], eta[i2], phi[i2], energy[i2]);
      var.push_back((p1 + p2).Eta());
    }
    return var;
}

vector<float> compute_Phi(const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f phi, rvec_f energy, rvec_f scale, rvec_f btag, rvec_f btag_wp)
{
    vector<float> var;
    for( size_t i = 0; i < idx.size() ; i++){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
      //if (!pass_jet) continue;
      bool pass_btag = btag[i1] > btag_wp[0] && btag[i2] > btag_wp[0];
      if( !(pass_jet && pass_btag) ) continue;
      ROOT::Math::PtEtaPhiMVector p1(pt[i1], eta[i1], phi[i1], energy[i1]);
      ROOT::Math::PtEtaPhiMVector p2(pt[i2], eta[i2], phi[i2], energy[i2]);
      var.push_back((p1 + p2).Phi());
    }
    return var;
}

vector<float> compute_mass(const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f phi, rvec_f energy, rvec_f scale, rvec_f btag, rvec_f btag_wp)
{
    vector<float> var;
    for( size_t i = 0; i < idx.size() ; i++){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
      //if (!pass_jet) continue;
      bool pass_btag = btag[i1] > btag_wp[0] && btag[i2] > btag_wp[0];
      if( !(pass_jet && pass_btag) ) continue;
      ROOT::Math::PtEtaPhiMVector p1(pt[i1], eta[i1], phi[i1], energy[i1]);
      ROOT::Math::PtEtaPhiMVector p2(pt[i2], eta[i2], phi[i2], energy[i2]);
      var.push_back((p1 + p2).M());
    }
    return var;
}

vector<float> compute_dR(const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f phi, rvec_f scale, rvec_f btag, rvec_f btag_wp)
{   
    vector<float> var;
    for( size_t i = 0; i < idx.size() ; i++){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
      //if (!pass_jet) continue;
      bool pass_btag = btag[i1] > btag_wp[0] && btag[i2] > btag_wp[0];
      if( !(pass_jet && pass_btag) ) continue;
      var.push_back( DeltaR(eta[i1], eta[i2], phi[i1], phi[i2]) );
    }   
    return var;
}

vector<float> compute_d1(const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f scale, rvec_f btag, rvec_f btag_wp)
{
    vector<float> var;
    for( size_t i = 0; i < idx.size() ; i++){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
      //if (!pass_jet) continue;
      bool pass_btag = btag[i1] > btag_wp[0] && btag[i2] > btag_wp[0];
      if( !(pass_jet && pass_btag) ) continue;
      var.push_back(btag[i1]);
    }
    return var;
}

vector<float> compute_d2(const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f scale, rvec_f btag, rvec_f btag_wp)
{
    vector<float> var;
    for( size_t i = 0; i < idx.size() ; i++){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
      bool pass_btag = btag[i1] > btag_wp[0] && btag[i2] > btag_wp[0];
      //if (!pass_jet) continue;
      if( !(pass_jet && pass_btag) ) continue;
      var.push_back(btag[i2]);
    }
    return var;
}

vector<float> compute_bpt1(const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f scale, rvec_f btag, rvec_f btag_wp)
{
    vector<float> var;
    for( size_t i = 0; i < idx.size() ; i++){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
      bool pass_btag = btag[i1] > btag_wp[0] && btag[i2] > btag_wp[0];
      //if (!pass_jet) continue;
      if( !(pass_jet && pass_btag) ) continue;
      var.push_back(pt[i1]);
    }
    return var;
}

vector<float> compute_bpt2(const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f scale, rvec_f btag, rvec_f btag_wp)
{
    vector<float> var;
    for( size_t i = 0; i < idx.size() ; i++){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
      bool pass_btag = btag[i1] > btag_wp[0] && btag[i2] > btag_wp[0];
      //if (!pass_jet) continue;
      if( !(pass_jet && pass_btag) ) continue;
      var.push_back(pt[i2]);
    }
    return var;
}

vector<float> compute_beta1(const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f scale, rvec_f btag, rvec_f btag_wp)
{
    vector<float> var;
    for( size_t i = 0; i < idx.size() ; i++){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
      bool pass_btag = btag[i1] > btag_wp[0] && btag[i2] > btag_wp[0];
      //if (!pass_jet) continue;
      if( !(pass_jet && pass_btag) ) continue;
      var.push_back(eta[i1]);
    }
    return var;
}

vector<float> compute_beta2(const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f scale, rvec_f btag, rvec_f btag_wp)
{
    vector<float> var;
    for( size_t i = 0; i < idx.size() ; i++){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
      bool pass_btag = btag[i1] > btag_wp[0] && btag[i2] > btag_wp[0];
      //if (!pass_jet) continue;
      if( !(pass_jet && pass_btag) ) continue;
      var.push_back(eta[i2]);
    }
    return var;
}

vector<float> compute_bphi1(const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f phi, rvec_f scale, rvec_f btag, rvec_f btag_wp)
{
    vector<float> var;
    for( size_t i = 0; i < idx.size() ; i++){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
      bool pass_btag = btag[i1] > btag_wp[0] && btag[i2] > btag_wp[0];
      //if (!pass_jet) continue;
      if( !(pass_jet && pass_btag) ) continue;
      var.push_back(phi[i1]);
    }
    return var;
}

vector<float> compute_bphi2(const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f phi, rvec_f scale, rvec_f btag, rvec_f btag_wp)
{
    vector<float> var;
    for( size_t i = 0; i < idx.size() ; i++){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
      bool pass_btag = btag[i1] > btag_wp[0] && btag[i2] > btag_wp[0];
      //if (!pass_jet) continue;
      if( !(pass_jet && pass_btag) ) continue;
      var.push_back(phi[i2]);
    }
    return var;
}

vector<float> compute_be1(const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f scale, rvec_f btag, rvec_f btag_wp, rvec_f e)
{
    vector<float> var;
    for( size_t i = 0; i < idx.size() ; i++){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
      bool pass_btag = btag[i1] > btag_wp[0] && btag[i2] > btag_wp[0];
      //if (!pass_jet) continue;
      if( !(pass_jet && pass_btag) ) continue;
      var.push_back(e[i1]);
    }
    return var;
}

vector<float> compute_be2(const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f scale, rvec_f btag, rvec_f btag_wp, rvec_f e)
{
    vector<float> var;
    for( size_t i = 0; i < idx.size() ; i++){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
      bool pass_btag = btag[i1] > btag_wp[0] && btag[i2] > btag_wp[0];
      //if (!pass_jet) continue;
      if( !(pass_jet && pass_btag) ) continue;
      var.push_back(e[i2]);
    }
    return var;
}

vector<float> compute_bm1(const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f phi, rvec_f energy, rvec_f scale, rvec_f btag, rvec_f btag_wp)
{
    vector<float> var;
    for( size_t i = 0; i < idx.size() ; i++){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
      bool pass_btag = btag[i1] > btag_wp[0] && btag[i2] > btag_wp[0];
      //if (!pass_jet) continue;
      if( !(pass_jet && pass_btag) ) continue;
      ROOT::Math::PtEtaPhiMVector p1(pt[i1], eta[i1], phi[i1], energy[i1]);
      var.push_back(p1.M());
    }
    return var;
}

vector<float> compute_bm2(const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f phi, rvec_f energy, rvec_f scale, rvec_f btag, rvec_f btag_wp)
{
    vector<float> var;
    for( size_t i = 0; i < idx.size() ; i++){
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      bool pass_jet = pt[i1]*scale[i1] > 30 && abs(eta[i1]) < 2.4 && pt[i2]*scale[i2] > 30 && abs(eta[i2]) < 2.4;
      bool pass_btag = btag[i1] > btag_wp[0] && btag[i2] > btag_wp[0];
      //if (!pass_jet) continue;
      if( !(pass_jet && pass_btag) ) continue;
      ROOT::Math::PtEtaPhiMVector p2(pt[i2], eta[i2], phi[i2], energy[i2]);
      var.push_back(p2.M());
    }
    return var;
}

float draw_mindR(rvec_f pt, rvec_f scale, rvec_f eta, rvec_f phi, rvec_f btag, rvec_f btag_wp)
{

    float dR = 999;
    float mbb = 999;
    for( int i = 0; i < pt.size()-1 ; i++){
      for( int j = i+1; j < pt.size() ; j++){
        bool pass_jet = pt[i]*scale[i] > 30 && abs(eta[i]) < 2.4 && pt[j]*scale[j] > 30 && abs(eta[j]) < 2.4;
        bool pass_btag = btag[i] > btag_wp[0] && btag[j] > btag_wp[0];
        if( !(pass_jet && pass_btag) ) continue;
        float tmp = DeltaR(eta[i], eta[j], phi[i], phi[j]);
        if(tmp < dR) {
          dR = tmp;
        }
      }
    }
    return dR;
}

float draw_mass(rvec_f pt, rvec_f scale, rvec_f eta, rvec_f phi, rvec_f energy, rvec_f btag, rvec_f btag_wp)
{
    float dR = 999;
    float mbb = 999;
    for( int i = 0; i < pt.size()-1 ; i++){
      for( int j = i+1; j < pt.size() ; j++){
        bool pass_jet = pt[i]*scale[i] > 30 && abs(eta[i]) < 2.4 && pt[j]*scale[j] > 30 && abs(eta[j]) < 2.4;
        bool pass_btag = btag[i] > btag_wp[0] && btag[j] > btag_wp[0];
        if( !(pass_jet && pass_btag) ) continue;
        float tmp = DeltaR(eta[i], eta[j], phi[i], phi[j]);
        if(tmp < dR) {
          ROOT::Math::PtEtaPhiMVector p1(pt[i], eta[i], phi[i], energy[i]);
          ROOT::Math::PtEtaPhiMVector p2(pt[j], eta[j], phi[j], energy[j]);
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

void ana_rdf(TString year = "2018", TString name = "test_ttbb", TString ch = "0", TString dnn = "1"){

  ROOT::RDataFrame df("outputTree",Form("/data1/common/skimmed_NanoAOD/ttbb_ntuple_ULv2/%s/%s/*.root", year.Data(), name.Data()) ); //from local

  auto df_ch = df.Filter(Form("channel == %s", ch.Data()),"lepton channel");


  if( name.Contains("Single") || name.Contains("EGamma")){
    df_ch = df_ch.Define("jet_scale","rvec_f(jet_pt.size(), 1.0f);");
    df_ch = df_ch.Define("weight","1");
    df_ch = df_ch.Define("b_weight","1");
  }else{
    df_ch = df_ch.Define("jet_scale","rvec_f(jet_pt.size(), 1.0f);");
    //df_ch = df_ch.Define("weight","pugenWeight");
    //df_ch = df_ch.Define("b_weight","pugenWeight*btagWeight_DeepFlavBrecalc*evWeight_leptonSF");
    df_ch = df_ch.Define("weight","evWeight/btagWeight_DeepFlavBrecalc");
    df_ch = df_ch.Define("b_weight","evWeight");

    //MC TODO
    //df_ch = df_ch.Define("jet_scale","jet_JER_Nom");
    ////event weight = genweight * puweight * lepton SF * b-tagging SF
    //if ( ch.Atoi() == 0 ){
    //  if ( year.Atoi() == 2016 ){ 
    //    df_ch = df_ch.Define("weight","genweight*PUWeight[0]*prefireweigt[0]*lepton_SF[0]*lepton_SF[3]*lepton_SF[6]");// analyzer typo! need to be fixed
    //    df_ch = df_ch.Define("b_weight","genweight*PUWeight[0]*prefireweigt[0]*lepton_SF[0]*lepton_SF[3]*lepton_SF[6]*jet_SF_deepJet_30[0]");
    //  }else if ( year.Atoi() == 2017 ){
    //    df_ch = df_ch.Define("weight","genweight*PUWeight[0]*prefireweight[0]*lepton_SF[0]*lepton_SF[3]*lepton_SF[6]");
    //    df_ch = df_ch.Define("b_weight","genweight*PUWeight[0]*prefireweight[0]*lepton_SF[0]*lepton_SF[3]*lepton_SF[6]*jet_SF_deepJet_30[0]");    
    //  }else if (year.Atoi() == 2018 ){
    //    df_ch = df_ch.Define("weight","genweight*PUWeight[0]*lepton_SF[0]*lepton_SF[3]*lepton_SF[6]");
    //    df_ch = df_ch.Define("b_weight","genweight*PUWeight[0]*lepton_SF[0]*lepton_SF[3]*lepton_SF[6]*jet_SF_deepJet_30[0]");
    //  }else cout << "Invalid year:: " << year.Data() << endl;

    //}else if ( ch.Atoi() == 1 ){
    //  if ( year.Atoi() == 2016 ){
    //    df_ch = df_ch.Define("weight","genweight*PUWeight[0]*prefireweight[0]*lepton_SF[0]*lepton_SF[3]*lepton_SF[6]*lepton_SF[9]");// analyzer typo! need to be fixed
    //    df_ch = df_ch.Define("b_weight","genweight*PUWeight[0]*prefireweight[0]*lepton_SF[0]*lepton_SF[3]*lepton_SF[6]*lepton_SF[9]*jet_SF_deepJet_30[0]");    
    //  }else if ( year.Atoi() == 2017 ){
    //    df_ch = df_ch.Define("weight","genweight*PUWeight[0]*prefireweight[0]*lepton_SF[0]*lepton_SF[3]*lepton_SF[6]*lepton_SF[9]");
    //    df_ch = df_ch.Define("b_weight","genweight*PUWeight[0]*prefireweight[0]*lepton_SF[0]*lepton_SF[3]*lepton_SF[6]*lepton_SF[9]*jet_SF_deepJet_30[0]");    
    //  }else if (year.Atoi() == 2018 ){
    //    df_ch = df_ch.Define("weight","genweight*PUWeight[0]*lepton_SF[0]*lepton_SF[3]*lepton_SF[6]");
    //    df_ch = df_ch.Define("b_weight","genweight*PUWeight[0]*lepton_SF[0]*lepton_SF[3]*lepton_SF[6]*jet_SF_deepJet_30[0]");
    //  }else cout << "Invalid year:: " << year.Data() << endl;
    //}else cout << "Invalid channel:: " << ch.Data() << endl;
  }

  //event selection
  //step1
  auto df_S0 = df_ch;

  if( year.Contains("2016")){
      if (ch.Atoi() == 0) df_S0 = df_S0.Filter("lepton_pt > 26 && abs(lepton_eta) < 2.4");
      else if (ch.Atoi() == 1) df_S0 = df_S0.Filter("lepton_pt > 29 && abs(lepton_eta) < 2.4");
      df_S0 = df_S0.Define("BTAG_deepJet_M","rvec_f(jet_pt.size(), 0.3093f);");

  }else if (year.Contains("2017")){ 
      if (ch.Atoi() == 0) df_S0 = df_S0.Filter("lepton_pt > 29 && abs(lepton_eta) < 2.4");
      else if (ch.Atoi() == 1) df_S0 = df_S0.Filter("(lepton_pt > 34 && abs(lepton_eta) < 2.4) || (lepton_pt > 30 && abs(lepton_eta) < 2.1)");
      df_S0 = df_S0.Define("BTAG_deepJet_M","rvec_f(jet_pt.size(), 0.3033f);");

  }else if (year.Contains("2018")){ 
      if (ch.Atoi() == 0) df_S0 = df_S0.Filter("lepton_pt > 26 && abs(lepton_eta) < 2.4");
      else if (ch.Atoi() == 1) df_S0 = df_S0.Filter("(lepton_pt > 34 && abs(lepton_eta) < 2.4) || (lepton_pt > 30 && abs(lepton_eta) < 2.1)");
      df_S0 = df_S0.Define("BTAG_deepJet_M","rvec_f(jet_pt.size(), 0.2770f);");
  }

  df_S0 = df_S0.Define("njets","Sum( jet_pt*jet_scale > 30 && abs(jet_eta) < 2.4 )");

  auto h_njets_S0 = df_S0.Histo1D({"h_njets_S0", "", 10, 0, 10}, "njets","weight");
  auto h_lepton_pt_S0 = df_S0.Define("Lepton_pT","lepton_pt").Histo1D({"h_lepton_pt_S0","",20,0,400},"Lepton_pT","weight");
  auto h_lepton_eta_S0 = df_S0.Define("Lepton_Eta","abs(lepton_eta)").Histo1D({"h_lepton_eta_S0","",20,0,2.5},"Lepton_Eta","weight");
  auto h_lepton_phi_S0 = df_S0.Define("Lepton_Phi","lepton_phi").Histo1D({"h_lepton_phi_S0","",40,-3.5,3.5},"Lepton_Phi","weight");

  //step1
  // 1 lep 1 jet
  auto df_S1 = df_S0.Filter("flags && Sum( jet_pt*jet_scale > 30 && abs(jet_eta) < 2.4 ) >= 1", "Events with one lepton and one jet");
//               .Define("njets","Sum( jet_pt*jet_scale > 30 && abs(jet_eta) < 2.4 )");
  
  auto h_njets_S1 = df_S1.Histo1D({"h_njets_S1", "", 10, 0, 10}, "njets","weight");
  auto h_lepton_pt_S1 = df_S1.Define("Lepton_pT","lepton_pt").Histo1D({"h_lepton_pt_S1","",20,0,400},"Lepton_pT","weight");
  auto h_lepton_eta_S1 = df_S1.Define("Lepton_Eta","abs(lepton_eta)").Histo1D({"h_lepton_eta_S1","",20,0,2.5},"Lepton_Eta","weight");
  auto h_lepton_phi_S1 = df_S1.Define("Lepton_Phi","lepton_phi").Histo1D({"h_lepton_phi_S1","",40,-3.5,3.5},"Lepton_Phi","weight");

  //step2
  // 1 lep 4 jet 2 b-tagged
  auto df_S2 = df_S1.Filter("Sum( jet_pt*jet_scale > 30 && abs(jet_eta) < 2.4 ) >= 4", "Events with at least 4 jets")
                    .Filter("Sum( jet_pt*jet_scale > 30 && abs(jet_eta) < 2.4 && jet_deepJet > BTAG_deepJet_M ) >= 2","Require 2 jets to be b-tagged");   
  df_S2 = df_S2.Define("jet_idx",jet_idx,{"jet_pt","jet_eta","jet_scale"})
               .Define("nbjets","Sum( jet_pt*jet_scale > 30 && abs(jet_eta) < 2.4 && jet_deepJet > BTAG_deepJet_M )");

  // b_weight from here
  auto h_njets_S2 = df_S2.Histo1D({"h_njets_S2", "", 6, 4, 10}, "njets","b_weight");
  auto h_nbjets_S2 = df_S2.Histo1D({"h_nbjets_S2", "", 3, 2, 5}, "nbjets","b_weight");
  auto h_lepton_pt_S2 = df_S2.Define("Lepton_pT","lepton_pt").Histo1D({"h_lepton_pt_S2","",20,0,400},"Lepton_pT","b_weight");
  auto h_lepton_eta_S2 = df_S2.Define("Lepton_Eta","abs(lepton_eta)").Histo1D({"h_lepton_eta_S2","",20,0,2.5},"Lepton_Eta","b_weight");
  auto h_lepton_phi_S2 = df_S2.Define("Lepton_Phi","lepton_phi").Histo1D({"h_lepton_phi_S2","",40,-3.5,3.5},"Lepton_Phi","b_weight");
  auto h_jet1_pt_S2 = df_S2.Define("jet1_pT","jet_pt[jet_idx[0]]*jet_scale[jet_idx[0]]").Histo1D({"h_jet1_pt_S2","",20,0,400},"jet1_pT","b_weight");
  auto h_jet1_eta_S2 = df_S2.Define("jet1_Eta","abs(jet_eta[jet_idx[0]])").Histo1D({"h_jet1_eta_S2","",20,0,2.5},"jet1_Eta","b_weight");
  auto h_jet2_pt_S2 = df_S2.Define("jet2_pT","jet_pt[jet_idx[1]]*jet_scale[jet_idx[1]]").Histo1D({"h_jet2_pt_S2","",20,0,400},"jet2_pT","b_weight");
  auto h_jet2_eta_S2 = df_S2.Define("jet2_Eta","abs(jet_eta[jet_idx[1]])").Histo1D({"h_jet2_eta_S2","",20,0,2.5},"jet2_Eta","b_weight");
  auto h_jet3_pt_S2 = df_S2.Define("jet3_pT","jet_pt[jet_idx[2]]*jet_scale[jet_idx[2]]").Histo1D({"h_jet3_pt_S2","",20,0,400},"jet3_pT","b_weight");
  auto h_jet3_eta_S2 = df_S2.Define("jet3_Eta","abs(jet_eta[jet_idx[2]])").Histo1D({"h_jet3_eta_S2","",20,0,2.5},"jet3_Eta","b_weight");
  auto h_jet4_pt_S2 = df_S2.Define("jet4_pT","jet_pt[jet_idx[3]]*jet_scale[jet_idx[3]]").Histo1D({"h_jet4_pt_S2","",20,0,400},"jet4_pT","b_weight");
  auto h_jet4_eta_S2 = df_S2.Define("jet4_Eta","abs(jet_eta[jet_idx[3]])").Histo1D({"h_jet4_eta_S2","",20,0,2.5},"jet4_Eta","b_weight");

  //step3
  // 1 lep 6 jet 2 b-tagged
  auto df_S3 = df_S2.Filter("Sum( jet_pt*jet_scale > 30 && abs(jet_eta) < 2.4 ) >= 6","Require 6 jets");

  auto h_nbjets_S3 = df_S3.Histo1D({"h_nbjets_S3", "", 3, 2, 5}, "nbjets","b_weight");
  auto h_lepton_pt_S3 = df_S3.Define("Lepton_pT","lepton_pt").Histo1D({"h_lepton_pt_S3","",20,0,400},"Lepton_pT","b_weight");
  auto h_lepton_eta_S3 = df_S3.Define("Lepton_Eta","abs(lepton_eta)").Histo1D({"h_lepton_eta_S3","",20,0,2.5},"Lepton_Eta","b_weight");
  auto h_lepton_phi_S3 = df_S3.Define("Lepton_Phi","lepton_phi").Histo1D({"h_lepton_phi_S3","",40,-3.5,3.5},"Lepton_Phi","b_weight");
  auto h_jet1_pt_S3 = df_S3.Define("jet1_pT","jet_pt[jet_idx[0]]*jet_scale[jet_idx[0]]").Histo1D({"h_jet1_pt_S3","",20,0,400},"jet1_pT","b_weight");
  auto h_jet1_eta_S3 = df_S3.Define("jet1_Eta","abs(jet_eta[jet_idx[0]])").Histo1D({"h_jet1_eta_S3","",20,0,2.5},"jet1_Eta","b_weight");
  auto h_jet2_pt_S3 = df_S3.Define("jet2_pT","jet_pt[jet_idx[1]]*jet_scale[jet_idx[1]]").Histo1D({"h_jet2_pt_S3","",20,0,400},"jet2_pT","b_weight");
  auto h_jet2_eta_S3 = df_S3.Define("jet2_Eta","abs(jet_eta[jet_idx[1]])").Histo1D({"h_jet2_eta_S3","",20,0,2.5},"jet2_Eta","b_weight");
  auto h_jet3_pt_S3 = df_S3.Define("jet3_pT","jet_pt[jet_idx[2]]*jet_scale[jet_idx[2]]").Histo1D({"h_jet3_pt_S3","",20,0,400},"jet3_pT","b_weight");
  auto h_jet3_eta_S3 = df_S3.Define("jet3_Eta","abs(jet_eta[jet_idx[2]])").Histo1D({"h_jet3_eta_S3","",20,0,2.5},"jet3_Eta","b_weight");
  auto h_jet4_pt_S3 = df_S3.Define("jet4_pT","jet_pt[jet_idx[3]]*jet_scale[jet_idx[3]]").Histo1D({"h_jet4_pt_S3","",20,0,400},"jet4_pT","b_weight");
  auto h_jet4_eta_S3 = df_S3.Define("jet4_Eta","abs(jet_eta[jet_idx[3]])").Histo1D({"h_jet4_eta_S3","",20,0,2.5},"jet4_Eta","b_weight");

  //step4
  // 1 lep 6 jet 4 b-tagged

  auto df_S4 = df_S3.Filter("Sum( jet_pt*jet_scale > 30 && abs(jet_eta) < 2.4 && jet_deepJet > BTAG_deepJet_M ) >= 4","Require 4 jets to be b-tagged");
  df_S4 = df_S4.Define("bjet_idx",bjet_idx,{"jet_pt","jet_eta","jet_scale","jet_deepJet","BTAG_deepJet_M"})
               .Define("min_Mass", draw_mass, {"jet_pt","jet_scale","jet_eta","jet_phi","jet_m","jet_deepJet","BTAG_deepJet_M"})
               .Define("min_dR", draw_mindR, {"jet_pt","jet_scale","jet_eta","jet_phi","jet_deepJet","BTAG_deepJet_M"});

  auto h_lepton_pt_S4 = df_S4.Define("Lepton_pT","lepton_pt").Histo1D({"h_lepton_pt_S4","",20,0,400},"Lepton_pT","b_weight");
  auto h_lepton_eta_S4 = df_S4.Define("Lepton_Eta","abs(lepton_eta)").Histo1D({"h_lepton_eta_S4","",20,0,2.5},"Lepton_Eta","b_weight");
  auto h_lepton_phi_S4 = df_S4.Define("Lepton_Phi","lepton_phi").Histo1D({"h_lepton_phi_S4","",40,-3.5,3.5},"Lepton_Phi","b_weight");
  //auto h_minMass_S4 = df_S4.Histo1D({"h_minMass_S4","",20,0,400},"min_Mass","b_weight");
  //auto h_mindR_S4 = df_S4.Histo1D({"h_mindR_S4","",20,0,4},"min_dR","b_weight");
  
  //const double dR_width[7] = {0.4,0.6,1.0,1.5,2.0,3.0,4.0};
  //const double M_width[9] = {0.0,30.0,60.0,80.0,100.0,135.0,170.0,285.0,400.0};
  const double dR_width[5] = {0.4,0.6,1.0,2.0,4.0};
  const double M_width[5] = {0.0,60.0,100.0,170.0,400.0};

  auto h_minMass_S4 = df_S4.Histo1D({"h_minMass_S4","",4,M_width},"min_Mass","b_weight");
  auto h_mindR_S4 = df_S4.Histo1D({"h_mindR_S4","",4,dR_width},"min_dR","b_weight");
  auto h_jet1_pt_S4 = df_S4.Define("jet1_pT","jet_pt[jet_idx[0]]*jet_scale[jet_idx[0]]").Histo1D({"h_jet1_pt_S4","",20,0,400},"jet1_pT","b_weight");
  auto h_jet1_eta_S4 = df_S4.Define("jet1_Eta","abs(jet_eta[jet_idx[0]])").Histo1D({"h_jet1_eta_S4","",20,0,2.5},"jet1_Eta","b_weight");
  auto h_bjet1_pt_S4 = df_S4.Define("bjet1_pT","jet_pt[bjet_idx[0]]*jet_scale[bjet_idx[0]]").Histo1D({"h_bjet1_pt_S4","",20,0,400},"bjet1_pT","b_weight");
  auto h_bjet2_pt_S4 = df_S4.Define("bjet2_pT","jet_pt[bjet_idx[1]]*jet_scale[bjet_idx[1]]").Histo1D({"h_bjet2_pt_S4","",20,0,400},"bjet2_pT","b_weight");

  //for DNN
  if( dnn.Atoi() == true ){
      df_S4 = df_S4.Define("jet_combi_idx", jet_combi, {"jet_pt","jet_eta","jet_phi","jet_m","jet_scale","jet_deepJet","BTAG_deepJet_M"})
                   .Define("mbb", compute_mass, {"jet_combi_idx","jet_pt","jet_eta","jet_phi","jet_m","jet_scale","jet_deepJet","BTAG_deepJet_M"})
                   .Define("dRbb", compute_dR, {"jet_combi_idx","jet_pt","jet_eta","jet_phi","jet_scale","jet_deepJet","BTAG_deepJet_M"})
                   .Define("dPhibb", compute_dPhi, {"jet_combi_idx","jet_pt","jet_eta","jet_phi","jet_scale","jet_deepJet","BTAG_deepJet_M"})
                   .Define("dEtabb", compute_dEta, {"jet_combi_idx","jet_pt","jet_eta","jet_scale","jet_deepJet","BTAG_deepJet_M"})
                   .Define("Etabb", compute_Eta, {"jet_combi_idx","jet_pt","jet_eta","jet_phi","jet_m","jet_scale","jet_deepJet","BTAG_deepJet_M"})
                   .Define("Phibb", compute_Phi, {"jet_combi_idx","jet_pt","jet_eta","jet_phi","jet_m","jet_scale","jet_deepJet","BTAG_deepJet_M"})

                   .Define("mlb1", compute_masslb1, {"jet_combi_idx","jet_pt","jet_eta","jet_phi","jet_m","jet_scale","jet_deepJet","BTAG_deepJet_M","lepton_pt","lepton_eta","lepton_phi","lepton_m"})
                   .Define("mlb2", compute_masslb2, {"jet_combi_idx","jet_pt","jet_eta","jet_phi","jet_m","jet_scale","jet_deepJet","BTAG_deepJet_M","lepton_pt","lepton_eta","lepton_phi","lepton_m"})
                   .Define("dRlb1", compute_dRlb1, {"jet_combi_idx","jet_pt","jet_eta","jet_phi","jet_scale","jet_deepJet","BTAG_deepJet_M","lepton_eta","lepton_phi"})
                   .Define("dRlb2", compute_dRlb2, {"jet_combi_idx","jet_pt","jet_eta","jet_phi","jet_scale","jet_deepJet","BTAG_deepJet_M","lepton_eta","lepton_phi"})

                   .Define("mlbb", compute_masslbb, {"jet_combi_idx","jet_pt","jet_eta","jet_phi","jet_m","jet_scale","jet_deepJet","BTAG_deepJet_M","lepton_pt","lepton_eta","lepton_phi","lepton_m"})
                   .Define("dRlbb", compute_dRlbb, {"jet_combi_idx","jet_pt","jet_eta","jet_phi","jet_m","jet_scale","jet_deepJet","BTAG_deepJet_M","lepton_eta","lepton_phi"})
//
                   .Define("mnub1", compute_massnub1, {"jet_combi_idx","jet_pt","jet_eta","jet_phi","jet_m","jet_scale","jet_deepJet","BTAG_deepJet_M","MET_pt","MET_phi"})
                   .Define("mnub2", compute_massnub2, {"jet_combi_idx","jet_pt","jet_eta","jet_phi","jet_m","jet_scale","jet_deepJet","BTAG_deepJet_M","MET_pt","MET_phi"})
                   .Define("dRnub1", compute_dRnub1, {"jet_combi_idx","jet_pt","jet_eta","jet_phi","jet_scale","jet_deepJet","BTAG_deepJet_M","MET_phi"})
                   .Define("dRnub2", compute_dRnub2, {"jet_combi_idx","jet_pt","jet_eta","jet_phi","jet_scale","jet_deepJet","BTAG_deepJet_M","MET_phi"})

                   .Define("mnubb", compute_massnubb, {"jet_combi_idx","jet_pt","jet_eta","jet_phi","jet_m","jet_scale","jet_deepJet","BTAG_deepJet_M","MET_pt","MET_phi"})
                   .Define("dRnubb", compute_dRnubb, {"jet_combi_idx","jet_pt","jet_eta","jet_phi","jet_m","jet_scale","jet_deepJet","BTAG_deepJet_M","MET_phi"})

                   .Define("dRlnubb", compute_dRlnubb, {"jet_combi_idx","jet_pt","jet_eta","jet_phi","jet_m","jet_scale","jet_deepJet","BTAG_deepJet_M","lepton_pt","lepton_eta","lepton_phi","lepton_m","MET_pt","MET_phi"})
                   .Define("dRlnub1", compute_dRlnub1, {"jet_combi_idx","jet_pt","jet_eta","jet_phi","jet_scale","jet_deepJet","BTAG_deepJet_M","lepton_pt","lepton_eta","lepton_phi","lepton_m","MET_pt","MET_phi"})
                   .Define("dRlnub2", compute_dRlnub2, {"jet_combi_idx","jet_pt","jet_eta","jet_phi","jet_scale","jet_deepJet","BTAG_deepJet_M","lepton_pt","lepton_eta","lepton_phi","lepton_m","MET_pt","MET_phi"})
//
                   .Define("pt1", compute_bpt1, {"jet_combi_idx","jet_pt","jet_eta","jet_scale","jet_deepJet","BTAG_deepJet_M"})
                   .Define("pt2", compute_bpt2, {"jet_combi_idx","jet_pt","jet_eta","jet_scale","jet_deepJet","BTAG_deepJet_M"})

                   .Define("d1", compute_d1, {"jet_combi_idx","jet_pt","jet_eta","jet_scale","jet_deepJet","BTAG_deepJet_M"})
                   .Define("d2", compute_d2, {"jet_combi_idx","jet_pt","jet_eta","jet_scale","jet_deepJet","BTAG_deepJet_M"})

                   .Define("eta1", compute_beta1, {"jet_combi_idx","jet_pt","jet_eta","jet_scale","jet_deepJet","BTAG_deepJet_M"})
                   .Define("eta2", compute_beta2, {"jet_combi_idx","jet_pt","jet_eta","jet_scale","jet_deepJet","BTAG_deepJet_M"})
                   .Define("phi1", compute_bphi1, {"jet_combi_idx","jet_pt","jet_eta","jet_phi","jet_scale","jet_deepJet","BTAG_deepJet_M"})
                   .Define("phi2", compute_bphi2, {"jet_combi_idx","jet_pt","jet_eta","jet_phi","jet_scale","jet_deepJet","BTAG_deepJet_M"})

                   .Define("e1", compute_be1, {"jet_combi_idx","jet_pt","jet_eta","jet_scale","jet_deepJet","BTAG_deepJet_M", "jet_m"})
                   .Define("e2", compute_be2, {"jet_combi_idx","jet_pt","jet_eta","jet_scale","jet_deepJet","BTAG_deepJet_M", "jet_m"})

                   .Define("m1", compute_bm1, {"jet_combi_idx","jet_pt","jet_eta","jet_phi","jet_m","jet_scale","jet_deepJet","BTAG_deepJet_M"})
                   .Define("m2", compute_bm2, {"jet_combi_idx","jet_pt","jet_eta","jet_phi","jet_m","jet_scale","jet_deepJet","BTAG_deepJet_M"})
                   .Define("btag3rd", "jet_deepJet[bjet_idx[2]]")
                   .Define("btag4th", "jet_deepJet[bjet_idx[3]]");

    df_S4.Snapshot("dnn_tree", Form("dnn_tree/dnn_%s_Ch%s.root", name.Data(), ch.Data()), {"event","mbb","dRbb","dPhibb","dEtabb","Etabb","Phibb","mlbb","dRlbb","mlb1","dRlb1","mlb2","dRlb2","mnubb","dRnubb","mnub1","dRnub1","mnub2","dRnub2","dRlnubb","dRlnub1","dRlnub2","b_weight","nbjets","pt1","eta1","phi1","e1","pt2","eta2","phi2","e2","d1","d2","m1","m2","lepton_pt","lepton_eta","lepton_phi","lepton_m","MET_pt","MET_phi","btag3rd","btag4th"});
//    df_S3.Snapshot("dnn_tree", Form("dnn_tree/dnn_%s_Ch%s.root", name.Data(), ch.Data()), {"event","mbb","dRbb","dPhibb","dEtabb","Etabb","Phibb","mlbb","dRlbb","mlb1","dRlb1","mlb2","dRlb2","b_weight","nbjets","pt1","eta1","phi1","e1","pt2","eta2","phi2","e2","d1","d2","m1","m2","lepton_pt","lepton_eta","lepton_phi","lepton_m"});

    if( name.Contains("ttbb") ){
      df_S4 = df_S4.Define("signal", signal_label, {"jet_combi_idx","jet_pt","jet_eta","jet_phi","jet_scale","jet_deepJet","BTAG_deepJet_M","addbjet1_eta","addbjet1_phi","addbjet2_eta","addbjet2_phi"});
      df_S4.Snapshot("dnn_tree", Form("dnn_tree/dnn_%s_Ch%s.root", name.Data(), ch.Data()), {"event","signal","mbb","dRbb","dPhibb","dEtabb","Etabb","Phibb","mlbb","dRlbb","mlb1","dRlb1","mlb2","dRlb2","mnubb","dRnubb","mnub1","dRnub1","mnub2","dRnub2","dRlnubb","dRlnub1","dRlnub2","b_weight","nbjets","pt1","eta1","phi1","e1","pt2","eta2","phi2","e2","d1","d2","m1","m2","lepton_pt","lepton_eta","lepton_phi","lepton_m","MET_pt","MET_phi","btag3rd","btag4th","addbjet1_pt","addbjet1_eta","addbjet1_phi","addbjet1_mass","addbjet2_pt","addbjet2_eta","addbjet2_phi","addbjet2_mass"});
//      df_S3.Snapshot("dnn_tree", Form("dnn_tree/dnn_%s_Ch%s.root", name.Data(), ch.Data()), {"event","signal","mbb","dRbb","dPhibb","dEtabb","Etabb","Phibb","mlbb","dRlbb","mlb1","dRlb1","mlb2","dRlb2","b_weight","nbjets","pt1","eta1","phi1","e1","pt2","eta2","phi2","e2","d1","d2","m1","m2","lepton_pt","lepton_eta","lepton_phi","lepton_m","addbjet1_pt","addbjet1_eta","addbjet1_phi","addbjet1_mass","addbjet2_pt","addbjet2_eta","addbjet2_phi","addbjet2_mass"});
    }
  }

  //create ntuple
  TFile f(Form("hist/hist_%s_%s_Ch%s.root", year.Data(), name.Data(), ch.Data()),"recreate");

  h_njets_S0->Write();
  h_lepton_pt_S0->Write();
  h_lepton_eta_S0->Write();
  h_lepton_phi_S0->Write();

  h_njets_S1->Write();
  h_lepton_pt_S1->Write();
  h_lepton_eta_S1->Write();
  h_lepton_phi_S1->Write();

  h_njets_S2->Write();
  h_nbjets_S2->Write();
  h_lepton_pt_S2->Write();
  h_lepton_eta_S2->Write();
  h_lepton_phi_S2->Write();
  h_jet1_pt_S2->Write();
  h_jet1_eta_S2->Write();
  h_jet2_pt_S2->Write();
  h_jet2_eta_S2->Write();
  h_jet3_pt_S2->Write();
  h_jet3_eta_S2->Write();
  h_jet4_pt_S2->Write();
  h_jet4_eta_S2->Write();

  h_nbjets_S3->Write();
  h_lepton_pt_S3->Write();
  h_lepton_eta_S3->Write();
  h_lepton_phi_S3->Write();
  h_jet1_pt_S3->Write();
  h_jet1_eta_S3->Write();
  h_jet2_pt_S3->Write();
  h_jet2_eta_S3->Write();
  h_jet3_pt_S3->Write();
  h_jet3_eta_S3->Write();
  h_jet4_pt_S3->Write();
  h_jet4_eta_S3->Write();

  h_lepton_pt_S4->Write();
  h_lepton_eta_S4->Write();
  h_lepton_phi_S4->Write();
  h_jet1_pt_S4->Write();
  h_jet1_eta_S4->Write();
  h_bjet1_pt_S4->Write();
  h_bjet2_pt_S4->Write();
  h_mindR_S4->Write();
  h_minMass_S4->Write();

  f.Close();

  //count number of events
  auto n_df = df.Count();
  auto n_df_S1 = df_S1.Count();
  auto n_df_S2 = df_S2.Count();
  auto n_df_S3 = df_S3.Count();
  auto n_df_S4 = df_S4.Count();

  std::cout << Form("Sample = %s, Ch=%s : number of events = ", name.Data(), ch.Data()) << *n_df << " , preselection(S1) = " << *n_df_S1 <<  " , S2 = " << *n_df_S2 << " , S3 = "  << *n_df_S3 << " , final selection(S4) = " << *n_df_S4 << std::endl;

}
