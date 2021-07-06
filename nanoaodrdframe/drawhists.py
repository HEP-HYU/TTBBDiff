import stackhists
import CMS_lumi
import sys

#s = stackhists.Stackhists(35.9)
s = stackhists.Stackhists( 59.74 )
s.setupStyle(alpha=1)
CMS_lumi.extraText = ""
#path = sys.argv[1]
# DATA
s.addChannel('Run2018.root', '2018 data', 999, isMC=False)

# LFV
s.addChannel("LFV_TT_TToCMuTau_Tensor.root", "LFV T x10", 12, isMC=True, xsec=129 * 1E+1, counterhistogramroot="LFV_TT_TToCMuTau_Tensor.root")
s.addChannel("LFV_TT_TToCMuTau_Vector.root", "LFV V x10", 11, isMC=True, xsec=21.50 * 1E+1, counterhistogramroot="LFV_TT_TToCMuTau_Vector.root")
s.addChannel("LFV_TT_TToCMuTau_Scalar.root", "LFV S x10", 10, isMC=True, xsec=2.688 * 1E+1, counterhistogramroot="LFV_TT_TToCMuTau_Scalar.root")

# MC ( reference twiki : https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns )
s.addChannel("TTToHadronic.root", "TT-had", 2, isMC=True, xsec=377960.0, counterhistogramroot="TTToHadronic.root")
s.addChannel("TTTo2L2Nu.root", "TT-di", 0, isMC=True, xsec=88290.0, counterhistogramroot="TTTo2L2Nu.root")
s.addChannel("TTToSemiLeptonic.root", "TT-semi", 1, isMC=True, xsec=365340.0, counterhistogramroot="TTToSemiLeptonic.root")

#s.addChannel("WJetsToLNu.root", "W+jets", 3, isMC=True, xsec=61526700, counterhistogramroot="WJetsToLNu.root")
s.addChannel("WJetsToLNu_inclHT100.root", "W+jets", 3, isMC=True, xsec=59373300, counterhistogramroot="WJetsToLNu_inclHT100.root")
#s.addChannel("WJetsToLNu_HT-70To100.root", "W+jets", 3, isMC=True, xsec=1353000.0 * 1.2, counterhistogramroot="WJetsToLNu_HT-70To100.root")
s.addChannel("WJetsToLNu_HT-100To200.root", "W+jets", 3, isMC=True, xsec=1345000.0 * 1.2, counterhistogramroot="WJetsToLNu_HT-100To200.root")
s.addChannel("WJetsToLNu_HT-200To400.root", "W+jets", 3, isMC=True, xsec=359700.0 * 1.2, counterhistogramroot="WJetsToLNu_HT-200To400.root")
s.addChannel("WJetsToLNu_HT-400To600.root", "W+jets", 3, isMC=True, xsec=48910.0 * 1.2, counterhistogramroot="WJetsToLNu_HT-400To600.root")
s.addChannel("WJetsToLNu_HT-600To800.root", "W+jets", 3, isMC=True, xsec=12050.0 * 1.2, counterhistogramroot="WJetsToLNu_HT-600To800.root")
s.addChannel("WJetsToLNu_HT-800To1200.root", "W+jets", 3, isMC=True, xsec=5501.0 * 1.2, counterhistogramroot="WJetsToLNu_HT-800To1200.root")
s.addChannel("WJetsToLNu_HT-1200To2500.root", "W+jets", 3, isMC=True, xsec=1329.0 * 1.2, counterhistogramroot="WJetsToLNu_HT-1200To2500.root")
s.addChannel("WJetsToLNu_HT-2500ToInf.root", "W+jets", 3, isMC=True, xsec=32.16 * 1.2, counterhistogramroot="WJetsToLNu_HT-2500ToInf.root")

#s.addChannel("WJetsToLNu_HT-100To200.root", "W+jets", 3, isMC=True, xsec=1345000.0*0.993, counterhistogramroot="WJetsToLNu_HT-100To200.root")
#s.addChannel("WJetsToLNu_HT-200To400.root", "W+jets", 3, isMC=True, xsec=359700.0*1.002, counterhistogramroot="WJetsToLNu_HT-200To400.root")
#s.addChannel("WJetsToLNu_HT-400To600.root", "W+jets", 3, isMC=True, xsec=48910.0*1.009, counterhistogramroot="WJetsToLNu_HT-400To600.root")
#s.addChannel("WJetsToLNu_HT-600To800.root", "W+jets", 3, isMC=True, xsec=12050.0*1.120, counterhistogramroot="WJetsToLNu_HT-600To800.root")
#s.addChannel("WJetsToLNu_HT-800To1200.root", "W+jets", 3, isMC=True, xsec=5501.0*1.202, counterhistogramroot="WJetsToLNu_HT-800To1200.root")
#s.addChannel("WJetsToLNu_HT-1200To2500.root", "W+jets", 3, isMC=True, xsec=1329.0*1.332, counterhistogramroot="WJetsToLNu_HT-1200To2500.root")
#s.addChannel("WJetsToLNu_HT-2500ToInf.root", "W+jets", 3, isMC=True, xsec=32.16*4.200, counterhistogramroot="WJetsToLNu_HT-2500ToInf.root")

s.addChannel("DYJetsToLL_M-10to50.root", "Z+jets", 4, isMC=True, xsec=18610000.0, counterhistogramroot="DYJetsToLL_M-10to50.root")
s.addChannel("DYJetsToLL_M-50-amcatnloFXFX.root", "Z+jets", 4, isMC=True, xsec=6077220.0, counterhistogramroot="DYJetsToLL_M-50-amcatnloFXFX.root")

s.addChannel("ST_t-channel_top.root", "ST", 5, isMC=True, xsec=136020.0, counterhistogramroot="ST_t-channel_top.root")
s.addChannel("ST_t-channel_antitop.root", "ST", 5, isMC=True, xsec=80950.0, counterhistogramroot="ST_t-channel_antitop.root")
s.addChannel("ST_tW_top.root", "ST", 5, isMC=True, xsec=35850.0, counterhistogramroot="ST_tW_top.root")
s.addChannel("ST_tW_antitop.root", "ST", 5, isMC=True, xsec=35850.0, counterhistogramroot="ST_tW_antitop.root")

s.addChannel("WWTo2L2Nu.root", "VV", 6, isMC=True, xsec=12178.0, counterhistogramroot="WWTo2L2Nu.root")
s.addChannel("WWToLNuQQ.root", "VV", 6, isMC=True, xsec=49997.0, counterhistogramroot="WWToLNuQQ.root")
s.addChannel("WZTo2L2Q.root", "VV", 6, isMC=True, xsec=5595.0, counterhistogramroot="WZTo2L2Q.root")
s.addChannel("WZTo3LNu.root", "VV", 6, isMC=True, xsec=4429.65, counterhistogramroot="WZTo3LNu.root")
s.addChannel("ZZTo2L2Q.root", "VV", 6, isMC=True, xsec=3220.0, counterhistogramroot="ZZTo2L2Q.root")

s.addChannel("TTWJetsToLNu.root", "TTX", 7, isMC=True, xsec=204.3, counterhistogramroot="TTWJetsToLNu.root")
s.addChannel("TTWJetsToQQ.root", "TTX", 7, isMC=True, xsec=406.2, counterhistogramroot="TTWJetsToQQ.root")
s.addChannel("TTZToLLNuNu_M-10.root", "TTX", 7, isMC=True, xsec=252.9, counterhistogramroot="TTZToLLNuNu_M-10.root")
s.addChannel("TTZToQQ.root", "TTX", 7, isMC=True, xsec=529.7, counterhistogramroot="TTZToQQ.root")

s.addChannel("QCD_Pt-15to20_MuEnrichedPt5.root", "QCD", 8, isMC=True, xsec=1273190000000*0.003, counterhistogramroot="QCD_Pt-15to20_MuEnrichedPt5.root")
s.addChannel("QCD_Pt-20to30_MuEnrichedPt5.root", "QCD", 8, isMC=True, xsec=558528000000*0.0053, counterhistogramroot="QCD_Pt-20to30_MuEnrichedPt5.root")
s.addChannel("QCD_Pt-30to50_MuEnrichedPt5.root", "QCD", 8, isMC=True, xsec=139803000000*0.01182, counterhistogramroot="QCD_Pt-30to50_MuEnrichedPt5.root")
s.addChannel("QCD_Pt-50to80_MuEnrichedPt5.root", "QCD", 8, isMC=True, xsec=19222500000*0.02276, counterhistogramroot="QCD_Pt-50to80_MuEnrichedPt5.root")
s.addChannel("QCD_Pt-80to120_MuEnrichedPt5.root", "QCD", 8, isMC=True, xsec=2758420000*0.03844, counterhistogramroot="QCD_Pt-80to120_MuEnrichedPt5.root")
s.addChannel("QCD_Pt-120to170_MuEnrichedPt5.root", "QCD", 8, isMC=True, xsec=469797000*0.05362, counterhistogramroot="QCD_Pt-120to170_MuEnrichedPt5.root")
s.addChannel("QCD_Pt-170to300_MuEnrichedPt5.root", "QCD", 8, isMC=True, xsec=117989000*0.07335, counterhistogramroot="QCD_Pt-170to300_MuEnrichedPt5.root")
s.addChannel("QCD_Pt-300to470_MuEnrichedPt5.root", "QCD", 8, isMC=True, xsec=7820250*0.10196, counterhistogramroot="QCD_Pt-300to470_MuEnrichedPt5.root")
s.addChannel("QCD_Pt-470to600_MuEnrichedPt5.root", "QCD", 8, isMC=True, xsec=645528*0.12242, counterhistogramroot="QCD_Pt-470to600_MuEnrichedPt5.root")
s.addChannel("QCD_Pt-600to800_MuEnrichedPt5.root", "QCD", 8, isMC=True, xsec=187109*0.13412, counterhistogramroot="QCD_Pt-600to800_MuEnrichedPt5.root")
s.addChannel("QCD_Pt-800to1000_MuEnrichedPt5.root", "QCD", 8, isMC=True, xsec=32348.6*0.14552, counterhistogramroot="QCD_Pt-800to1000_MuEnrichedPt5.root")
s.addChannel("QCD_Pt-1000toInf_MuEnrichedPt5.root", "QCD", 8, isMC=True, xsec=10430.5*0.15544, counterhistogramroot="QCD_Pt-1000toInf_MuEnrichedPt5.root")

s.addHistogram("hnevents_nocut", "Nocut counter", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hnevents_cut0", "Cut1 nevents", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hnevents_cut00", "Cut2 nevents", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hnevents_cut000", "Cut3 nevents", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hnevents_cut0000", "Cut4 nevents", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hnevents_cut00000", "Cut5 nevents", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)

# Muon multiplicity
s.addHistogram("hnmuonpass_cut0", "Number of Muons", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hnmuonpass_cut00", "Number of Muons", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hnmuonpass_cut000", "Number of Muons", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hnmuonpass_cut0000", "Number of Muons", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hnmuonpass_cut00000", "Number of Muons", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)

# Jet multiplicity
s.addHistogram("hncleanjetspass_cut0", "Number of Jets", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hncleanjetspass_cut00", "Number of Jets", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hncleanjetspass_cut000", "Number of Jets", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hncleanjetspass_cut0000", "Number of Jets", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hncleanjetspass_cut00000", "Number of Jets", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)

# bjet multiplicity
s.addHistogram("hncleanbjetspass_cut0", "Number of b-tagged jets", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hncleanbjetspass_cut00", "Number of b-tagged jets", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hncleanbjetspass_cut000", "Number of b-tagged jets", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hncleanbjetspass_cut0000", "Number of b-tagged jets", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hncleanbjetspass_cut00000", "Number of b-tagged jets", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)

# cjet multiplicity
#s.addHistogram("hncleancjetspass_cut0", "Number of c-tagged Jets", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
#s.addHistogram("hncleancjetspass_cut00", "Number of c-tagged Jets", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
#s.addHistogram("hncleancjetspass_cut000", "Number of c-tagged Jets", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
#s.addHistogram("hncleancjetspass_cut0000", "Number of c-tagged Jets", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
#s.addHistogram("hncleancjetspass_cut00000", "Number of c-tagged Jets", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)

# Tau multiplicity
s.addHistogram("hncleantaupass_cut0", "Number of Hadronic Tau", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hncleantaupass_cut00", "Number of Hadronic Tau", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hncleantaupass_cut000", "Number of Hadronic Tau", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hncleantaupass_cut0000", "Number of Hadronic Tau", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hncleantaupass_cut00000", "Number of Hadronic Tau", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)

# Jet Histograms
s.addHistogram("hjet1pt_cut000", "p_{T} of Leading Jet (GeV)", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hjet1pt_cut0000", "p_{T} of Leading Jet (GeV)", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hjet1pt_cut00000", "p_{T} of Leading Jet (GeV)", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)

s.addHistogram("hjet1eta_cut000", "#eta of Leading Jet", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hjet1eta_cut0000", "#eta of Leading Jet", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hjet1eta_cut00000", "#eta of Leading Jet", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)

s.addHistogram("hjet2pt_cut000", "p_{T} of Sub-leading Jet (GeV)", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hjet2pt_cut0000", "p_{T} of Sub-leading Jet (GeV)", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hjet2pt_cut00000", "p_{T} of Sub-leading Jet (GeV)", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)

s.addHistogram("hjet2eta_cut000", "#eta of Sub-leading Jet", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hjet2eta_cut0000", "#eta of Sub-leading Jet", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hjet2eta_cut00000", "#eta of Sub-leading Jet", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)

# bJet Histograms
s.addHistogram("hbjet1pt_cut0000", "p_{T} of Leading b-tagged Jet (GeV)", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hbjet1pt_cut00000", "p_{T} of Leading b-tagged Jet (GeV)", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)

s.addHistogram("hbjet1eta_cut0000", "#eta of Leading b-tagged Jet", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hbjet1eta_cut00000", "#eta of Leading b-tagged Jet", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)

# Muon Histograms
s.addHistogram("hmuon1pt_cut0", "p_{T} of Leading Muon (GeV)", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hmuon1pt_cut00", "p_{T} of Leading Muon (GeV)", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hmuon1pt_cut000", "p_{T} of Leading Muon (GeV)", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hmuon1pt_cut0000", "p_{T} of Leading Muon (GeV)", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hmuon1pt_cut00000", "p_{T} of Leading Muon (GeV)", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)

s.addHistogram("hmuon1eta_cut0", "#eta of Leading Muon", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hmuon1eta_cut00", "#eta of Leading Muon", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hmuon1eta_cut000", "#eta of Leading Muon", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hmuon1eta_cut0000", "#eta of Leading Muon", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hmuon1eta_cut00000", "#eta of Leading Muon", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)

# Tau Histograms
s.addHistogram("htau1pt_cut00", "p_{T} of Leading Tau (GeV)", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("htau1pt_cut000", "p_{T} of Leading Tau (GeV)", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("htau1pt_cut0000", "p_{T} of Leading Tau (GeV)", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("htau1pt_cut00000", "p_{T} of Leading Tau (GeV)", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)

s.addHistogram("htau1eta_cut00", "#eta of Leading Tau", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("htau1eta_cut000", "#eta of Leading Tau", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("htau1eta_cut0000", "#eta of Leading Tau", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("htau1eta_cut00000", "#eta of Leading Tau", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)

# Top mass reconstruction ( W hadronic )
s.addHistogram("hchi2_cut0000", "#chi^{2} score", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hchi2_SMTop_mass_cut0000", "SM Top mass from #chi^{2} (GeV)", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hchi2_LQTop_mass_cut0000", "LQ Top mass from #chi^{2} (GeV)", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hchi2_SMW_mass_cut0000", "SM W mass from #chi^{2} (GeV)", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)

s.addHistogram("hchi2_wqq_absdEta_cut0000", "#it{#left|#Delta#eta#right|}_{wqq} from #chi^{2}", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hchi2_wqq_absdPhi_cut0000", "#it{#left|#Delta#phi#right|}_{wqq} from #chi^{2}", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hchi2_wqq_dR_cut0000", "#DeltaR_{wqq}", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)

s.addHistogram("hchi2_lfvjmu_absdEta_cut0000", "#it{#left|#Delta#eta#right|}_{j,#mu} from #chi^{2}", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hchi2_lfvjmu_absdPhi_cut0000", "#it{#left|#Delta#phi#right|}_{j,#mu} from #chi^{2}", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hchi2_lfvjmu_dR_cut0000", "#DeltaR_{j,#mu} from #chi^{2}", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hchi2_lfvjmu_mass_cut0000", "M_{j,#mu} (GeV) from #chi^{2}", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)

s.addHistogram("hchi2_lfvjtau_absdEta_cut0000", "#it{#left|#Delta#eta#right|}_{j,#tau} from #chi^{2}", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hchi2_lfvjtau_absdPhi_cut0000", "#it{#left|#Delta#phi#right|}_{j,#tau} from #chi^{2}", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hchi2_lfvjtau_dR_cut0000", "#DeltaR_{j,#tau} from #chi^{2}", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hchi2_lfvjtau_mass_cut0000", "M_{j,#tau} from #chi^{2} (GeV)", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)

s.addHistogram("hchi2_mutau_absdEta_cut0000", "#it{#left|#Delta#eta#right|}_{#mu,#tau} from #chi^{2}", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hchi2_mutau_absdPhi_cut0000", "#it{#left|#Delta#phi#right|}_{#mu,#tau} from #chi^{2}", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hchi2_mutau_dR_cut0000", "#DeltaR_{#mu,#tau} from #chi^{2}", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hchi2_mutau_mass_cut0000", "M_{#mu,#tau} from #chi^{2} (GeV)", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)

s.addHistogram("hchi2_lfvjmutau_absdEta_cut0000", "#it{#left|#Delta#eta#right|}_{j,#mu#tau} from #chi^{2}", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hchi2_lfvjmutau_absdPhi_cut0000", "#it{#left|#Delta#phi#right|}_{j,#mu#tau} from #chi^{2}", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hchi2_lfvjmutau_dR_cut0000", "#DeltaR_{j,#mu#tau} from #chi^{2}", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)

s.addHistogram("hchi2_cut00000", "#chi^{2} score", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hchi2_SMTop_mass_cut00000", "SM Top mass from #chi^{2} (GeV)", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hchi2_LQTop_mass_cut00000", "LQ Top mass from #chi^{2} (GeV)", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hchi2_SMW_mass_cut00000", "SM W mass from #chi^{2} (GeV)", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)

s.addHistogram("hchi2_wqq_absdEta_cut00000", "#it{#left|#Delta#eta#right|}_{wqq} from #chi^{2}", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hchi2_wqq_absdPhi_cut00000", "#it{#left|#Delta#phi#right|}_{wqq} from #chi^{2}", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hchi2_wqq_dR_cut00000", "#DeltaR_{wqq}", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)

s.addHistogram("hchi2_lfvjmu_absdEta_cut00000", "#it{#left|#Delta#eta#right|}_{j,#mu} from #chi^{2}", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hchi2_lfvjmu_absdPhi_cut00000", "#it{#left|#Delta#phi#right|}_{j,#mu} from #chi^{2}", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hchi2_lfvjmu_dR_cut00000", "#DeltaR_{j,#mu} from #chi^{2}", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hchi2_lfvjmu_mass_cut00000", "M_{j,#mu} (GeV) from #chi^{2}", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)

s.addHistogram("hchi2_lfvjtau_absdEta_cut00000", "#it{#left|#Delta#eta#right|}_{j,#tau} from #chi^{2}", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hchi2_lfvjtau_absdPhi_cut00000", "#it{#left|#Delta#phi#right|}_{j,#tau} from #chi^{2}", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hchi2_lfvjtau_dR_cut00000", "#DeltaR_{j,#tau} from #chi^{2}", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hchi2_lfvjtau_mass_cut00000", "M_{j,#tau} from #chi^{2} (GeV)", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)

s.addHistogram("hchi2_mutau_absdEta_cut00000", "#it{#left|#Delta#eta#right|}_{#mu,#tau} from #chi^{2}", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hchi2_mutau_absdPhi_cut00000", "#it{#left|#Delta#phi#right|}_{#mu,#tau} from #chi^{2}", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hchi2_mutau_dR_cut00000", "#DeltaR_{#mu,#tau} from #chi^{2}", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hchi2_mutau_mass_cut00000", "M_{#mu,#tau} from #chi^{2} (GeV)", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)

s.addHistogram("hchi2_lfvjmutau_absdEta_cut00000", "#it{#left|#Delta#eta#right|}_{j,#mu#tau} from #chi^{2}", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hchi2_lfvjmutau_absdPhi_cut00000", "#it{#left|#Delta#phi#right|}_{j,#mu#tau} from #chi^{2}", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hchi2_lfvjmutau_dR_cut00000", "#DeltaR_{j,#mu#tau} from #chi^{2}", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)

# MET
s.addHistogram("hmetpt_cut0", "MET p_{T} (GeV)", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hmetpt_cut00", "MET p_{T} (GeV)", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hmetpt_cut000", "MET p_{T} (GeV)", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hmetpt_cut0000", "MET p_{T} (GeV)", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hmetpt_cut00000", "MET p_{T} (GeV)", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)

s.addHistogram("hmetphi_cut0", "MET #phi", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hmetphi_cut00", "MET #phi", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hmetphi_cut000", "MET #phi", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hmetphi_cut0000", "MET #phi", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hmetphi_cut00000", "MET #phi", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)

# SumET
s.addHistogram("hsumet_cut0", "Sum E_{T} (GeV)", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hsumet_cut00", "Sum E_{T} (GeV)", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hsumet_cut000", "Sum E_{T} (GeV)", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hsumet_cut0000", "Sum E_{T} (GeV)", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)
s.addHistogram("hsumet_cut00000", "Sum E_{T} (GeV)", "Entries", drawmode=stackhists.STACKED, drawoption="hist", isLogy=False, ymin=0.1)

s.draw()
 
