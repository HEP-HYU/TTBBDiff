/*
 * utility.cpp
 *
 *  Created on: Dec 4, 2018
 *      Author: suyong
 */
#include "utility.h"
#include "TMatrixDSym.h"
#include "TVectorT.h"
#include "Math/SpecFuncMathMore.h"
#include "Math/GenVector/VectorUtil.h"
#include "Math/GenVector/Rotation3D.h"
#include "Math/Math.h"

// Utility function to generate fourvector objects for thigs that pass selections

FourVectorVec gen4vec(floats &pt, floats &eta, floats &phi, floats &mass)
{
	const int nsize = pt.size();
	FourVectorVec fourvecs;
	fourvecs.reserve(nsize);
	for (auto i=0; i<nsize; i++)
	{
		fourvecs.emplace_back(pt[i], eta[i], phi[i], fabs(mass[i]));
	}

	return fourvecs;
}

FourVectorVec genmet4vec(float met_pt, float met_phi)
{
        FourVectorVec vecs;
        float met_px = met_pt*cos(met_phi);
        float met_py = met_pt*sin(met_phi);
        for(int i = -500; i <= 500; i+=50){
                FourVector metfourvec;
                metfourvec.SetPxPyPzE(met_px, met_py, i, met_pt);
                vecs.emplace_back(metfourvec);
        }
        return vecs;
}

floats weightv(floats &x, float evWeight)
{
	const int nsize = x.size();
	floats weightvector(nsize, evWeight);
	return weightvector;
}

floats sphericity(FourVectorVec &p)
{
	TMatrixDSym NormMomTensor(3);

	NormMomTensor = 0.0;
	double p2sum = 0.0;
	for (auto x: p)
	{
		p2sum += x.P2();
		double mom[3] = {x.Px(), x.Py(), x.Pz()};
		for (int irow=0; irow<3; irow++)
		{
			for (int icol=irow; icol<3; icol++)
			{
				NormMomTensor(irow, icol) += mom[irow] * mom[icol];
			}
		}
	}
	NormMomTensor *= (1.0/p2sum);
	TVectorT<double> Qrev;
	NormMomTensor.EigenVectors(Qrev);
	floats Q(3);
	for (auto i=0; i<3; i++) Q[i] = Qrev[2-i];

	return Q;
}


double foxwolframmoment(int l, FourVectorVec &p, int minj, int maxj)
{   // PRD 87, 073014 (2013)
	double answer = 0.0;

	double ptsum=0.0;

	if (maxj==-1) // process everything
	{
		maxj = p.size();
	}
	//for (auto x: p)
	for (auto i=minj; i<maxj; i++)
	{
		auto x = p[i];
		ptsum += x.Pt();
		//for (auto y: p)
		for (auto j=minj; j<maxj; j++)
		{
			auto y = p[j];
			double wij = x.Pt() * y.Pt();
			double cosdOmega = x.Vect().Dot(y.Vect()) / (x.P() * y.P());
			if (cosdOmega>1.0) cosdOmega=1.0;
			if (cosdOmega<-1.0) cosdOmega=-1.0;
			answer += wij * ROOT::Math::legendre(l, cosdOmega);
		}
	}
	answer /= ptsum*ptsum;
	if (fabs(answer)>1.0) std::cout << "FW>1 " << answer << std::endl;
	return answer;
}


ints good_idx(ints good)
{
        ints out;
        for(unsigned int i = 0; i < good.size(); i++){
                if( good[i] ){
                        out.emplace_back(i);
                }
        }
        return out;
}


floats chi2(float smtop_mass, float smw_mass, float lqtop_mass)
{
	floats out;

	const float MT = 172.5;
        const float MW = 80.4;
        const float WT = 1.41;
        const float WW = 2.085;
	
	float chi2_SMTop = pow((MT-smtop_mass)/WT, 2);
        float chi2_SMW = pow((MW-smw_mass)/WW, 2);
        float chi2_LQTop = pow((MT-lqtop_mass)/WT, 2);
        float chi2 = chi2_SMTop + chi2_SMW + chi2_LQTop;

	out.emplace_back(chi2);
	out.emplace_back(chi2_SMTop);
	out.emplace_back(chi2_SMW);
	out.emplace_back(chi2_LQTop);
	
	return out;
}


floats lqtop_reconstruction( FourVectorVec &cjet, FourVectorVec &mu, FourVectorVec &tau)
{
        floats results;
        float tmp_LQTop_mass = 999;
        float tmp_chi2 = 99999;
        for( unsigned int i = 0; i < cjet.size(); i++){
                for( unsigned int j = 0; j < mu.size(); j++){
                        for( unsigned int k = 0; k < tau.size(); k++){
                                tmp_LQTop_mass = (cjet[i] + mu[j] + tau[k]).M();
                                if( pow((tmp_LQTop_mass - 172.5)/1.41,2) < tmp_chi2){
                                        tmp_chi2 = pow((tmp_LQTop_mass - 172.5)/1.41,2);
                                        results.clear();
                                        results.emplace_back(tmp_chi2); //Chi2 value
                                        results.emplace_back(tmp_LQTop_mass); //LQ Top mass
                                }   
                        }
                }
        }

        return results; 
}


floats top_reconstruction_whad(FourVectorVec &jets, FourVectorVec &bjets, FourVectorVec &muons, FourVectorVec &taus){
        
        floats out;

        float LFVtop_mass, SMW_mass, SMtop_mass;
        float X_LFVtop, X_SMW, X_SMtop;
        float X_min=999999999999999, X_min_LFVtop_mass=-1, X_min_SMW_mass=-1, X_min_SMtop_mass=-1;
        float c_idx=-1, wj1_idx=-1, wj2_idx=-1, tau_idx=-1;
        float MT = 172.5;   // Top mass
        float WT = 1.482;   // Top width
        float MW = 79.8;    // W mass
        float WW = 2.085;   // W width
        
        // U or C jet
        for(unsigned int j1 = 0; j1<jets.size(); j1++){
                // Tau
                for(unsigned int ta = 0; ta<taus.size(); ta++){
                        LFVtop_mass = (jets[j1]+taus[ta]+muons[0]).M();
                        X_LFVtop = std::pow((MT-LFVtop_mass)/WT,2);
                        // Jets from W-1
                        for(unsigned int j2 = 0; j2<jets.size(); j2++){
                                if(jets[j2].Pt() == bjets[0].Pt() || jets[j2].Pt() == jets[j1].Pt()) continue;
                                // Jets from W-2
                                for(unsigned int j3 = 0; j3<jets.size(); j3++){
                                        if(jets[j3].Pt() == jets[j2].Pt() || jets[j3].Pt() == bjets[0].Pt() || jets[j3].Pt() == jets[j1].Pt()) continue;
                                        SMW_mass = (jets[j2]+jets[j3]).M();
                                        X_SMW = std::pow((MW-SMW_mass)/WW,2);
                                        SMtop_mass = (bjets[0]+jets[j2]+jets[j3]).M();
                                        X_SMtop = std::pow((MT-SMtop_mass)/WT,2);
                                        if (X_LFVtop + X_SMW + X_SMtop < X_min){
                                                X_min = X_LFVtop + X_SMW + X_SMtop;
                                                X_min_LFVtop_mass = LFVtop_mass;
                                                X_min_SMW_mass = SMW_mass;
                                                X_min_SMtop_mass = SMtop_mass;
                                                c_idx = float(j1);
                                                wj1_idx = float(j2);
                                                wj2_idx = float(j3);
                                                tau_idx = float(ta);
                                        }
                                }
                        }
                }
        }
        out.push_back(X_min);               // 0
        out.push_back(X_min_LFVtop_mass);    // 1
        out.push_back(X_min_SMW_mass);      // 2
        out.push_back(X_min_SMtop_mass);    // 3
        out.push_back(c_idx);               // 4
        out.push_back(wj1_idx);             // 5
        out.push_back(wj2_idx);             // 6
        out.push_back(tau_idx);             // 7

        return out;
}

floats top_reco_products(FourVectorVec &jets, FourVectorVec &muons, FourVectorVec &taus, floats topreco){
        floats out;
        int j_idx = topreco[4];
        int wjet1_idx = topreco[5];
        int wjet2_idx = topreco[6];
        int tau_idx = topreco[7];

        FourVector lfvjet = jets[j_idx];
        FourVector wjet1 = jets[wjet1_idx];
        FourVector wjet2 = jets[wjet2_idx];
        FourVector tau = taus[tau_idx];
        FourVector muon = muons[0];

        float wqq_dEta = wjet1.Eta() - wjet2.Eta();
        float wqq_dPhi = ROOT::Math::VectorUtil::DeltaPhi(wjet1, wjet2);
        float wqq_dR = ROOT::Math::VectorUtil::DeltaR(wjet1, wjet2);

        float lfvjmu_dEta = lfvjet.Eta() - muon.Eta();
        float lfvjmu_dPhi = ROOT::Math::VectorUtil::DeltaPhi(lfvjet, muon);
        float lfvjmu_dR = ROOT::Math::VectorUtil::DeltaR(lfvjet, muon);
        float lfvjmu_mass = ROOT::Math::VectorUtil::InvariantMass(lfvjet, muon);

        float lfvjtau_dEta = lfvjet.Eta() - tau.Eta();
        float lfvjtau_dPhi = ROOT::Math::VectorUtil::DeltaPhi(lfvjet, tau);
        float lfvjtau_dR = ROOT::Math::VectorUtil::DeltaR(lfvjet, tau);
        float lfvjtau_mass = ROOT::Math::VectorUtil::InvariantMass(lfvjet, tau);
        
        float mutau_dEta = muon.Eta() - tau.Eta();
        float mutau_dPhi = ROOT::Math::VectorUtil::DeltaPhi(muon, tau);
        float mutau_dR = ROOT::Math::VectorUtil::DeltaR(muon, tau);
        float mutau_mass = ROOT::Math::VectorUtil::InvariantMass(muon, tau);

        FourVector mutau = muon + tau;
        float lfvjmutau_dEta = lfvjet.Eta() - mutau.Eta();
        float lfvjmutau_dPhi = ROOT::Math::VectorUtil::DeltaPhi(lfvjet, mutau);
        float lfvjmutau_dR = ROOT::Math::VectorUtil::DeltaR(lfvjet, mutau);
        float lfvjmutau_mass = ROOT::Math::VectorUtil::InvariantMass(lfvjet, mutau);

        out.emplace_back(wqq_dEta);
        out.emplace_back(wqq_dPhi);
        out.emplace_back(wqq_dR);
        out.emplace_back(lfvjmu_dEta);
        out.emplace_back(lfvjmu_dPhi);
        out.emplace_back(lfvjmu_dR);
        out.emplace_back(lfvjmu_mass);
        out.emplace_back(lfvjtau_dEta);
        out.emplace_back(lfvjtau_dPhi);
        out.emplace_back(lfvjtau_dR);
        out.emplace_back(lfvjtau_mass);
        out.emplace_back(mutau_dEta);
        out.emplace_back(mutau_dPhi);
        out.emplace_back(mutau_dR);
        out.emplace_back(mutau_mass);
        out.emplace_back(lfvjmutau_dEta);
        out.emplace_back(lfvjmutau_dPhi);
        out.emplace_back(lfvjmutau_dR);
        out.emplace_back(lfvjmutau_mass);

        return out;
}


float calculate_deltaEta( FourVector &p1, FourVector &p2){
        return p1.Eta() - p2.Eta();
}

float calculate_deltaPhi( FourVector &p1, FourVector &p2){
        return ROOT::Math::VectorUtil::DeltaPhi(p1, p2);
}

float calculate_deltaR( FourVector &p1, FourVector &p2){
        return ROOT::Math::VectorUtil::DeltaR(p1, p2);
}

float calculate_invMass( FourVector &p1, FourVector &p2){
        return ROOT::Math::VectorUtil::InvariantMass(p1, p2);
}

FourVector sum_4vec( FourVector &p1, FourVector &p2){
        return p1+p2;
}

floats sort_discriminant( floats discr, floats obj ){
        auto sorted_discr = Reverse(Argsort(discr));
        floats out;
        for (auto idx : sorted_discr){
                out.emplace_back(obj[idx]);
        }
        return out;
}

ints find_element( ints vec, int a ){
        ints out;
        for(unsigned int i = 0; i < vec.size(); i++){
            if( vec[i] == a ) out.emplace_back(i);
        }
        return out;
}
