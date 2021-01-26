#include "MyAnalysisExample/MyAnalysisAlg.h"
#include "xAODEventInfo/EventInfo.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODTau/TauJetContainer.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODJet/JetContainer.h"
#include "xAODEgamma/PhotonContainer.h"
#include "xAODMissingET/MissingETContainer.h"
#include "xAODCore/ShallowAuxContainer.h"
#include "xAODCutFlow/CutBookkeeperContainer.h"
#include "xAODCore/ShallowCopy.h"
#include "xAODBase/IParticleHelpers.h"
#include "AsgTools/AnaToolHandle.h"
#include "xAODMetaData/FileMetaData.h"
#include "GaudiKernel/ITHistSvc.h"
#include "JetInterface/IJetUpdateJvt.h"
#include "TH1.h"
#include "TH2.h"
#include <TLorentzVector.h>
#include <vector>
#include "TTree.h"

//Jet Uncertainties tool
//JetUncertaintiesTool jesProv("JESProvider");
///////////////////////////////////////////////////////////////////

//muon, truth muon, electron
const double muon_mass = 0.1056583745; //GeV
const double crosssection = 1000*1.2342*1;
MyAnalysisAlg::MyAnalysisAlg(const std::string& name, ISvcLocator* pSvcLocator)
  : AthAnalysisAlgorithm(name, pSvcLocator)
  , m_thistSvc("THistSvc/THistSvc", name)
  , inputMetaStore("StoreGateSvc/InputMetaDataStore","")
  , m_selTool("CP::MuonSelectionTool/MyMuonSelectionTool", this)
  , eg_selTool("AsgElectronLikelihoodTool/MyAsgElectronLikelihoodTool", this)
  , m_acc_eleId("")
  , ph_selTool("AsgPhotonIsEMSelector/MyAsgPhotonIsEMSelectorTool",this)
  , tight_acc("")
  , m_jetCalitool("JetCalibrationTool/myCalibTool", this)
  , btag_selTool("BTaggingSelectionTool/mybtagSelTool", this)
  , t_selTool("TauAnalysisTools::TauSelectionTool/MyTauSelectionTool", this)
  , h_updatejvt("JetVertexTaggerTool/UpdateJVT")
{
  declareProperty("message", m_message="You cannot always get what you want.");
  declareProperty("MuonSelTool", m_selTool, "The private muon selection tool");
  declareProperty("ElectronSelTool", eg_selTool, "The private Electron selection tool");
  declareProperty("PhotonSelTool", ph_selTool, "The private photon selection");
  declareProperty("JetCalibTool",m_jetCalitool, "The private jet calibration");
  declareProperty("BtagSelTool",btag_selTool, "The private btag selection tool");
  declareProperty("TauSelTool",t_selTool, "The private tau Truth Matching tool");
}

StatusCode MyAnalysisAlg::initialize() {
  ATH_MSG_INFO("initialize()");
  ATH_MSG_INFO("My message: " << m_message);

  ATH_CHECK(m_thistSvc.retrieve());
  ATH_CHECK(m_selTool.retrieve());
  ATH_CHECK(eg_selTool.retrieve());
  ATH_CHECK(ph_selTool.retrieve());
  //Jet calibration
  //ATH_CHECK(m_jetCalitool.retrieve());
  //btagselectiontool
  ATH_CHECK(btag_selTool.retrieve());
  //tau truthmatchingtool
  ATH_CHECK(t_selTool.retrieve());

 
  //std::cout << std::endl << std::endl  <<"here is ratio :" << h_ratio  << std::endl << std::endl;
  
  //m_acc_eleId = std::string("DFCommonElectronsLHLoose"); // electron selector
  m_acc_eleId = std::string("DFCommonElectronsLHMedium");
  // m_acc_eleId = std::string("DFCommonElectronsLHTight");
  //DFCommonElectronsLHVeryLoose, DFCommonElectronsLHLoose, DFCommonElectronsLHLooseBL, DFCommonElectronsLHMedium, DFCommonElectronsLHTight
  tight_acc = std::string("DFCommonPhotonsIsEMTight");

  //muon plot
  m_h_nMuons = new TH1D("h_nMuons", "Number of muons", 11, -0.5, 10.5);
  m_thistSvc->regHist("/MyAnalysisAlg/Muon/h_nMuons", m_h_nMuons).setChecked();
  m_h_muon_e = new TH1D("h_muon_e", "Muon energy [GeV]", 100, 0., 250.);
  m_thistSvc->regHist("/MyAnalysisAlg/Muon/h_muon_e", m_h_muon_e).setChecked();
  m_h_muon_pt = new TH1D("h_muon_pt", "Muon p_{T} [GeV]", 100, 0., 250.);
  m_thistSvc->regHist("/MyAnalysisAlg/Muon/h_muon_pt", m_h_muon_pt).setChecked();
  m_h_muon_eta = new TH1D("h_muon_eta", "Muon #eta", 100, -3.0, 3.0);
  m_thistSvc->regHist("/MyAnalysisAlg/Muon/h_muon_eta", m_h_muon_eta).setChecked();
  m_h_muon_phi = new TH1D("h_muon_phi", "Muon #phi [radian]", 100, -M_PI, M_PI);
  m_thistSvc->regHist("/MyAnalysisAlg/Muon/h_muon_phi", m_h_muon_phi).setChecked();
  m_h2_muon_eta_phi = new TH2D("h2_muon_eta_phi", "Muon #eta vs #phi", 100, -3.0, 3.0, 100, -M_PI, M_PI);
  m_thistSvc->regHist("/MyAnalysisAlg/Muon/h2_muon_eta_phi", m_h2_muon_eta_phi).setChecked();
  m_two_muon_plus = new TH1D("two_muon_plus_mass","two_muon_plus_mass",60,61.,120.);
  m_thistSvc->regHist("/MyAnalysisAlg/Muon/two_muon_plus_mass",m_two_muon_plus).setChecked();
  m_two_muon_plus_pt = new TH1D("two_muon_plus_pt","two_muon_plus_pt",500,1.,500.);
  m_thistSvc->regHist("/MyAnalysisAlg/Muon/two_muon_plus_pt",m_two_muon_plus_pt).setChecked();
  m_two_muon_ns_plus = new TH1D("two_muon_ns_plus_mass","two_muon_ns_plus_mass",130,1.,130.);
  m_thistSvc->regHist("/MyAnalysisAlg/Muon_Ns/two_muon_ns_plus_mass",m_two_muon_ns_plus).setChecked();
  m_four_muon_ns_plus = new TH1D("four_muon_ns_plus_mass","four_muon_ns_plus_mass",500,1.,500.);
  m_thistSvc->regHist("/MyAnalysisAlg/Muon_Ns/four_muon_ns_plus_mass",m_four_muon_ns_plus).setChecked();
  //m_four_muon_plus->Sumw2();
  //particle higgs
  m_h_Zboson = new TH1D("h_Zboson", "Mass of Zboson", 60, 61, 120);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/Zboson/h_Zboson", m_h_Zboson).setChecked();
  m_h_Zboson_pt = new TH1D("h_Zboson_pt", "Zboson p_{T} [GeV]", 1200, 1., 1200.);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/Zboson/h_Zboson_pt", m_h_Zboson_pt).setChecked();
  m_h_Zmumu = new TH1D("h_Zmumu", "Mass of Zmumu", 60, 61, 120);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/Zmumu/h_Zmumu", m_h_Zmumu).setChecked();
  m_h_Zmumu->Sumw2();
  m_h_Zmumu_pt = new TH1D("h_Zmumu_pt", "Zmumu p_{T} [GeV]", 1200, 1., 1200.);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/Zmumu/h_Zmumu_pt", m_h_Zmumu_pt).setChecked();
  m_h_Zmumu_pt->Sumw2();
  m_h_Zmumu_event_nocut = new TH1D("h_Zmumu_event_nocut", "Zmumu p_{T} [GeV]", 10, 1., 10.);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/Zmumu/h_Zmumu_event_nocut", m_h_Zmumu_event_nocut).setChecked();
  m_h_Zmumu_event_ptcut = new TH1D("h_Zmumu_event_ptcut", "Zmumu p_{T} [GeV]", 10, 1., 10.);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/Zmumu/h_Zmumu_event_ptcut", m_h_Zmumu_event_ptcut).setChecked();
  m_h_Zmumu_event_etacut = new TH1D("h_Zmumu_event_etacut", "Zmumu p_{T} [GeV]", 10, 1., 10.);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/Zmumu/h_Zmumu_event_etacut", m_h_Zmumu_event_etacut).setChecked();
  m_h_Zmumu_event = new TH1D("h_Zmumu_event", "Zmumu p_{T} [GeV]", 10, 1., 10.);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/Zmumu/h_Zmumu_event", m_h_Zmumu_event).setChecked();
  m_h_Zmumu_event_ns = new TH1D("h_Zmumu_event_ns", "Zmumu p_{T} [GeV]", 10, 1., 10.);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/Zmumu/h_Zmumu_event_ns", m_h_Zmumu_event_ns).setChecked();

  m_h_Zee = new TH1D("h_Zee", "Mass of Zee", 60, 61, 120);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/Zee/h_Zee", m_h_Zee).setChecked();
  m_h_Zee->Sumw2();
  m_h_Zee_pt = new TH1D("h_Zee_pt", "Zee p_{T} [GeV]", 1200, 1., 1200.);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/Zee/h_Zee_pt", m_h_Zee_pt).setChecked();
  m_h_Zee_pt->Sumw2();

  m_h_two_Zboson_mass = new TH1D("h_two_Zboson_mass", "Mass of two Zboson_mass", 500, 1, 500);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/Zboson/h_two_Zboson_mass", m_h_two_Zboson_mass).setChecked();
  m_h_Wboson = new TH1D("h_Wboson", "Mass of Wboson", 60, 51, 110);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/Wboson/h_Wboson", m_h_Wboson).setChecked();
  m_h_Wboson_pt = new TH1D("h_Wboson_pt", "Wboson p_{T} [GeV]", 500, 1., 500.);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/Wboson/h_Wboson_pt", m_h_Wboson_pt).setChecked();
  m_h_Tquark = new TH1D("h_tquark", "Mass of tquark", 431, 70, 500);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/tquark/h_tquark", m_h_Tquark).setChecked();
  m_h_Higgs = new TH1D("h_Higgs", "Mass of Higgs", 430, 70., 500.);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/Higgs/h_Higgs", m_h_Higgs).setChecked();
  m_h_Higgs_pt = new TH1D("h_Higgs_pt", "pt of Higgs", 500, 1., 500);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/Higgs/h_Higgs_pt", m_h_Higgs_pt).setChecked();
  m_h_two_Zboson = new TH1D("h_two_Zboson", "Mass of two_Zboson", 500, 251, 500);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/two_Zboson/h_two_Zboson", m_h_two_Zboson).setChecked();
  m_h_two_Wboson = new TH1D("h_two_Wboson", "Mass of two_Wboson", 500, 251, 500);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/two_Wboson/h_two_Wboson", m_h_two_Wboson).setChecked();
  m_h_two_Tquark = new TH1D("h_two_tquark", "Mass of two_tquark", 250, 251, 500);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/two_tquark/h_two_tquark", m_h_two_Tquark).setChecked();
  m_h_two_Tquark_pt = new TH1D("h_two_tquark_pt", "pt of two_tquark", 500, 1., 500.);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/two_tquark/h_two_tquark_pt", m_h_two_Tquark_pt).setChecked();
  m_h_two_Bquark = new TH1D("h_two_bquark", "Mass of two_bquark", 500, 251, 500);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/two_bquark/h_two_bquark", m_h_two_Bquark).setChecked();
  m_h_two_photon = new TH1D("h_two_photon", "Mass of two_photon", 500, 251, 500);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/two_photon/h_two_photon", m_h_two_photon).setChecked();
  //////////////////////truth particle/////////////////////////
  /////////////////////////////////////////////////////////////
  m_h_truthZboson = new TH1D("h_truthZboson", "Mass of truthZboson", 60, 61, 120);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Zboson/h_truthZboson", m_h_truthZboson).setChecked();
  m_h_truthZboson_pt = new TH1D("h_truthZboson_pt", "truthZboson p_{T} [GeV]", 250, 1., 500.);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Zboson/h_truthZboson_pt", m_h_truthZboson_pt).setChecked();
  m_h_two_truthZboson_pt = new TH1D("h_two_truthZboson_pt", "two truthZboson p_{T} [GeV]", 500, 1., 500.);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Zboson/h_two_truthZboson_pt", m_h_two_truthZboson_pt).setChecked();
  m_h_two_truthZboson_mass = new TH1D("h_two_truthZboson_mass", "Mass of two truthZboson", 500, 1, 500);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Zboson/h_two_truthZboson_mass", m_h_two_truthZboson_mass).setChecked();

  m_h_truthWboson = new TH1D("h_truthWboson", "Mass of truthWboson", 60, 51, 110);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Wboson/h_truthWboson", m_h_truthWboson).setChecked();
  m_h_truthWboson_pt = new TH1D("h_truthWboson_pt", "truthWboson p_{T} [GeV]", 500, 1., 500.);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Wboson/h_truthWboson_pt", m_h_truthWboson_pt).setChecked();
  m_h_truthTquark = new TH1D("h_truthtquark", "Mass of truthtquark", 400, 101, 500);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/tquark/h_truthtquark", m_h_truthTquark).setChecked();
  m_h_truthHiggs = new TH1D("h_truthHiggs", "Mass of truthHiggs", 431, 70, 500);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Higgs/h_truthHiggs", m_h_truthHiggs).setChecked();
  m_h_truthHiggs_pt = new TH1D("h_truthHiggs_pt", "pt of truthHiggs", 400, 1., 500);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Higgs/h_truthHiggs_pt", m_h_truthHiggs_pt).setChecked();
  /////////////////////truthmuon////////////////////////
  m_h_ntruthMuons = new TH1D("h_ntruthMuons", "Number of truthmuons", 11, -0.5, 10.5);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Muon/h_ntruthMuons", m_h_ntruthMuons).setChecked();
  m_h_truthmuon_e = new TH1D("h_truthmuon_e", "truthMuon energy [GeV]", 100, 0., 250.);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Muon/h_truthmuon_e", m_h_truthmuon_e).setChecked();
  m_h_truthmuon_pt = new TH1D("h_truthmuon_pt", "truthMuon p_{T} [GeV]", 100, 0., 250.);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Muon/h_truthmuon_pt", m_h_truthmuon_pt).setChecked();
  m_h_truthmuon_eta = new TH1D("h_truthmuon_eta", "truthMuon #eta", 100, -3.0, 3.0);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Muon/h_truthmuon_eta", m_h_truthmuon_eta).setChecked();
  m_h_truthmuon_phi = new TH1D("h_truthmuon_phi", "truthMuon #phi [radian]", 100, -M_PI, M_PI);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Muon/h_truthmuon_phi", m_h_truthmuon_phi).setChecked();
  m_h2_truthmuon_eta_phi = new TH2D("h2_truthmuon_eta_phi", "truthMuon #eta vs #phi", 100, -3.0, 3.0, 100, -M_PI, M_PI);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Muon/h2_truthmuon_eta_phi", m_h2_truthmuon_eta_phi).setChecked();
  m_two_truthmuon_plus = new TH1D("two_truthmuon_plus_mass","two_truthmuon_plus_mass",60,61.,120.);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Muon/two_truthmuon_plus_mass",m_two_truthmuon_plus).setChecked();
  m_two_truthmuon_plus_pt = new TH1D("two_truthmuon_plus_pt","two_truthmuon_plus_pt",500,1.,500.);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Muon/two_truthmuon_plus_pt",m_two_truthmuon_plus_pt).setChecked();
  m_four_truthmuon_plus = new TH1D("four_truthmuon_plus_mass","four_truthmuon_plus_mass",500,1.,500.);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Muon/four_truthmuon_plus_mass",m_four_truthmuon_plus).setChecked();
  m_four_truthmuon_plus_pt = new TH1D("four_truthmuon_plus_pt","four_truthmuon_plus_pt",500,1.,500.);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Muon/four_truthmuon_plus_pt",m_four_truthmuon_plus_pt).setChecked();
  //////////////////////TruthElectrons//////////////////////////
  m_h_ntruthElectrons = new TH1D("h_ntruthElectrons", "Number of truthelectrons", 11, -0.5, 10.5);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Electron/h_ntruthElectrons", m_h_ntruthElectrons).setChecked();
  m_h_truthelectron_e = new TH1D("h_truthelectron_e", "truthElectron energy [GeV]", 100, 0., 250.);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Electron/h_truthelectron_e", m_h_truthelectron_e).setChecked();
  m_h_truthelectron_pt = new TH1D("h_truthelectron_pt", "truthElectron p_{T} [GeV]", 100, 0., 250.);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Electron/h_truthelectron_pt", m_h_truthelectron_pt).setChecked();
  m_h_truthelectron_eta = new TH1D("h_truthelectron_eta", "truthElectron #eta", 100, -3.0, 3.0);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Electron/h_truthelectron_eta", m_h_truthelectron_eta).setChecked();
  m_h_truthelectron_phi = new TH1D("h_truthelectron_phi", "truthElectron #phi [radian]", 100, -M_PI, M_PI);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Electron/h_truthelectron_phi", m_h_truthelectron_phi).setChecked();
  m_h2_truthelectron_eta_phi = new TH2D("h2_truthelectron_eta_phi", "truthElectron #eta vs #phi", 100, -3.0, 3.0, 100, -M_PI, M_PI);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Electron/h2_truthelectron_eta_phi", m_h2_truthelectron_eta_phi).setChecked();
  m_two_truthelectron_plus = new TH1D("two_truthelectron_plus_mass","two_truthelectron_plus_mass",60,61.,120.);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Electron/two_truthelectron_plus_mass",m_two_truthelectron_plus).setChecked();
  m_two_truthelectron_plus_pt = new TH1D("two_truthelectron_plus_pt","two_truthelectron_plus_pt",500,1.,500.);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Electron/two_truthelectron_plus_pt",m_two_truthelectron_plus_pt).setChecked();
  m_four_truthelectron_plus = new TH1D("four_truthelectron_plus_mass","four_truthelectron_plus_mass",500,1.,500.);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Electron/four_truthelectron_plus_mass",m_four_truthelectron_plus).setChecked();
  m_four_truthelectron_plus_pt = new TH1D("four_truthelectron_plus_pt","four_truthelectron_plus_pt",500,1.,500.);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Electron/four_truthelectron_plus_pt",m_four_truthelectron_plus_pt).setChecked();
  ////////////////////TruthNeutrinos////////////////////////////
  m_h_ntruthNeutrinos = new TH1D("h_ntruthNeutrinos", "Number of truthneutrinos", 11, -0.5, 10.5);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Neutrino/h_ntruthNeutrinos", m_h_ntruthNeutrinos).setChecked();
  m_h_truthneutrino_e = new TH1D("h_truthneutrino_e", "truthNeutrino energy [GeV]", 100, 0., 250.);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Neutrino/h_truthneutrino_e", m_h_truthneutrino_e).setChecked();
  m_h_truthneutrino_pt = new TH1D("h_truthneutrino_pt", "truthNeutrino p_{T} [GeV]", 100, 0., 250.);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Neutrino/h_truthneutrino_pt", m_h_truthneutrino_pt).setChecked();
  m_h_truthneutrino_eta = new TH1D("h_truthneutrino_eta", "truthNeutrino #eta", 100, -3.0, 3.0);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Neutrino/h_truthneutrino_eta", m_h_truthneutrino_eta).setChecked();
  m_h_truthneutrino_phi = new TH1D("h_truthneutrino_phi", "truthNeutrino #phi [radian]", 100, -M_PI, M_PI);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Neutrino/h_truthneutrino_phi", m_h_truthneutrino_phi).setChecked();
  m_h2_truthneutrino_eta_phi = new TH2D("h2_truthneutrino_eta_phi", "truthNeutrino #eta vs #phi", 100, -3.0, 3.0, 100, -M_PI, M_PI);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Neutrino/h2_truthneutrino_eta_phi", m_h2_truthneutrino_eta_phi).setChecked();
  ////////////////////TruthPhotons/////////////////////////////
  m_h_ntruthPhotons = new TH1D("h_ntruthPhotons", "Number of truthphotons", 11, -0.5, 10.5);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Photon/h_ntruthPhotons", m_h_ntruthPhotons).setChecked();
  m_h_truthphoton_e = new TH1D("h_truthphoton_e", "truthPhoton energy [GeV]", 100, 0., 250.);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Photon/h_truthphoton_e", m_h_truthphoton_e).setChecked();
  m_h_truthphoton_pt = new TH1D("h_truthphoton_pt", "truthPhoton p_{T} [GeV]", 100, 0., 250.);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Photon/h_truthphoton_pt", m_h_truthphoton_pt).setChecked();
  m_h_truthphoton_eta = new TH1D("h_truthphoton_eta", "truthPhoton #eta", 100, -3.0, 3.0);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Photon/h_truthphoton_eta", m_h_truthphoton_eta).setChecked();
  m_h_truthphoton_phi = new TH1D("h_truthphoton_phi", "truthPhoton #phi [radian]", 100, -M_PI, M_PI);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Photon/h_truthphoton_phi", m_h_truthphoton_phi).setChecked();
  m_h2_truthphoton_eta_phi = new TH2D("h2_truthphoton_eta_phi", "truthPhoton #eta vs #phi", 100, -3.0, 3.0, 100, -M_PI, M_PI);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Photon/h2_truthphoton_eta_phi", m_h2_truthphoton_eta_phi).setChecked();
  ////////////////////TruthTaus///////////////////////////////
  m_h_ntruthTaus = new TH1D("h_ntruthTaus", "Number of truthtaus", 11, -0.5, 10.5);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Tau/h_ntruthTaus", m_h_ntruthTaus).setChecked();
  m_h_truthtau_e = new TH1D("h_truthtau_e", "truthTau energy [GeV]", 100, 0., 250.);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Tau/h_truthtau_e", m_h_truthtau_e).setChecked();
  m_h_truthtau_pt = new TH1D("h_truthtau_pt", "truthTau p_{T} [GeV]", 100, 0., 250.);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Tau/h_truthtau_pt", m_h_truthtau_pt).setChecked();
  m_h_truthtau_eta = new TH1D("h_truthtau_eta", "truthTau #eta", 100, -3.0, 3.0);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Tau/h_truthtau_eta", m_h_truthtau_eta).setChecked();
  m_h_truthtau_phi = new TH1D("h_truthtau_phi", "truthTau #phi [radian]", 100, -M_PI, M_PI);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Tau/h_truthtau_phi", m_h_truthtau_phi).setChecked();
  m_h2_truthtau_eta_phi = new TH2D("h2_truthtau_eta_phi", "truthTau #eta vs #phi", 100, -3.0, 3.0, 100, -M_PI, M_PI);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Tau/h2_truthtau_eta_phi", m_h2_truthtau_eta_phi).setChecked();
  m_two_truthtau_plus = new TH1D("two_truthtau_plus_mass","two_truthtau_plus_mass",500,1.,500.);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Tau/two_truthtau_plus_mass",m_two_truthtau_plus).setChecked();
  m_two_truthtau_plus_pt = new TH1D("two_truthtau_plus_pt","two_truthtau_plus_pt",500,1.,500.);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Tau/two_truthtau_plus_pt",m_two_truthtau_plus_pt).setChecked();
  m_four_truthtau_plus = new TH1D("four_truthtau_plus_mass","four_truthtau_plus_mass",500,1.,500.);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Tau/four_truthtau_plus_mass",m_four_truthtau_plus).setChecked();
  /////////////AntiKt10TruthTrimmedPtFrac5SmallR20Jets///////////
  m_h_ntruthLjets = new TH1D("h_ntruthLjets", "Number of truthljets", 11, -0.5, 10.5);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Ljet/h_ntruthLjets", m_h_ntruthLjets).setChecked();
  m_h_truthljet_e = new TH1D("h_truthljet_e", "truthLjet energy [GeV]", 100, 0., 250.);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Ljet/h_truthljet_e", m_h_truthljet_e).setChecked();
  m_h_truthljet_pt = new TH1D("h_truthljet_pt", "truthLjet p_{T} [GeV]", 100, 0., 250.);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Ljet/h_truthljet_pt", m_h_truthljet_pt).setChecked();
  m_h_truthljet_eta = new TH1D("h_truthljet_eta", "truthLjet #eta", 100, -3.0, 3.0);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Ljet/h_truthljet_eta", m_h_truthljet_eta).setChecked();
  m_h_truthljet_phi = new TH1D("h_truthljet_phi", "truthLjet #phi [radian]", 100, -M_PI, M_PI);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Ljet/h_truthljet_phi", m_h_truthljet_phi).setChecked();
  m_h2_truthljet_eta_phi = new TH2D("h2_truthljet_eta_phi", "truthLjet #eta vs #phi", 100, -3.0, 3.0, 100, -M_PI, M_PI);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Ljet/h2_truthljet_eta_phi", m_h2_truthljet_eta_phi).setChecked();
  m_one_ljet = new TH1D("h_truthljet_plus_mass","one_ljet_mass",151,0.,150.);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Ljet/h_truthljet_plus_mass",m_one_ljet).setChecked();
  m_two_ljet_plus = new TH1D("h_two_truthljet_plus_mass","two_ljet_plus_mass",401,100.,400.);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Ljet/h_two_truthljet_plus_mass",m_two_ljet_plus).setChecked();
  ///////////////AntiKt4TruthJets////////////////////////////
  m_h_ntruthSjets = new TH1D("h_ntruthSjets", "Number of truthsjets", 11, -0.5, 10.5);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Sjet/h_ntruthSjets", m_h_ntruthSjets).setChecked();
  m_h_truthsjet_e = new TH1D("h_truthsjet_e", "truthSjet energy [GeV]", 100, 0., 250.);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Sjet/h_truthsjet_e", m_h_truthsjet_e).setChecked();
  m_h_truthsjet_pt = new TH1D("h_truthsjet_pt", "truthSjet p_{T} [GeV]", 100, 0., 250.);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Sjet/h_truthsjet_pt", m_h_truthsjet_pt).setChecked();
  m_h_truthsjet_eta = new TH1D("h_truthsjet_eta", "truthSjet #eta", 100, -3.0, 3.0);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Sjet/h_truthsjet_eta", m_h_truthsjet_eta).setChecked();
  m_h_truthsjet_phi = new TH1D("h_truthsjet_phi", "truthSjet #phi [radian]", 100, -M_PI, M_PI);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Sjet/h_truthsjet_phi", m_h_truthsjet_phi).setChecked();
  m_h2_truthsjet_eta_phi = new TH2D("h2_truthsjet_eta_phi", "truthSjet #eta vs #phi", 100, -3.0, 3.0, 100, -M_PI, M_PI);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Sjet/h2_truthsjet_eta_phi", m_h2_truthsjet_eta_phi).setChecked();
  ////////////////////b-jet///////////////////////////////
  m_h_ntruthBjets = new TH1D("h_ntruthBjets", "Number of truthbjets", 11, -0.5, 10.5);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Bjet/h_ntruthBjets", m_h_ntruthBjets).setChecked();
  m_h_truthbjet_e = new TH1D("h_truthbjet_e", "truthBjet energy [GeV]", 100, 0., 250.);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Bjet/h_truthbjet_e", m_h_truthbjet_e).setChecked();
  m_h_truthbjet_pt = new TH1D("h_truthbjet_pt", "truthBjet p_{T} [GeV]", 100, 0., 250.);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Bjet/h_truthbjet_pt", m_h_truthbjet_pt).setChecked();
  m_h_truthbjet_eta = new TH1D("h_truthbjet_eta", "truthBjet #eta", 100, -3.0, 3.0);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Bjet/h_truthbjet_eta", m_h_truthbjet_eta).setChecked();
  m_h_truthbjet_phi = new TH1D("h_truthbjet_phi", "truthBjet #phi [radian]", 100, -M_PI, M_PI);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Bjet/h_truthbjet_phi", m_h_truthbjet_phi).setChecked();
  m_h2_truthbjet_eta_phi = new TH2D("h2_truthbjet_eta_phi", "truthBjet #eta vs #phi", 100, -3.0, 3.0, 100, -M_PI, M_PI);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Bjet/h2_truthbjet_eta_phi", m_h2_truthbjet_eta_phi).setChecked();
  m_one_bjet = new TH1D("h_one_truthbjet_mass","one_bjet_mass",100,1.,10.);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Bjet/h_one_truthbjet_mass",m_one_bjet).setChecked();
  m_two_bjet_plus = new TH1D("h_two_truthbjet_plus_mass","two_bjet_plus_mass",150,301.,450.);
  m_thistSvc->regHist("/MyAnalysisAlg/Truthparticle/Bjet/h_two_truthbjet_plus_mass",m_two_bjet_plus).setChecked();
  ///////////////////MET_Truth////////////////////////////

  ///////////////////////////////////////////////////////////////////////////////////

  //electron plot
  m_h_nElectrons = new TH1D("h_nElectron", "Number of electron", 11, -0.5, 10.5);
  m_thistSvc->regHist("/MyAnalysisAlg/Electron/h_nElectron", m_h_nElectrons).setChecked();
  m_h_electron_e = new TH1D("h_electron_e", "Electron energy [GeV]", 100, 0., 250.);
  m_thistSvc->regHist("/MyAnalysisAlg/Electron/h_electron_e", m_h_electron_e).setChecked();
  m_h_electron_pt = new TH1D("h_electron_pt", "Electron p_{T} [GeV]", 100, 0., 250.);
  m_thistSvc->regHist("/MyAnalysisAlg/Electron/h_electron_pt", m_h_electron_pt).setChecked();
  m_h_electron_eta = new TH1D("h_electron_eta", "Electron #eta", 100, -3.0, 3.0);
  m_thistSvc->regHist("/MyAnalysisAlg/Electron/h_electron_eta", m_h_electron_eta).setChecked();
  m_h_electron_phi = new TH1D("h_electron_phi", "Electron #phi [radian]", 100, -M_PI, M_PI);
  m_thistSvc->regHist("/MyAnalysisAlg/Electron/h_electron_phi", m_h_electron_phi).setChecked();
  m_h2_electron_eta_phi = new TH2D("h2_electron_eta_phi", "Electron #eta vs #phi", 100, -3.0, 3.0, 100, -M_PI, M_PI);
  m_thistSvc->regHist("/MyAnalysisAlg/Electron/h2_electron_eta_phi", m_h2_electron_eta_phi).setChecked();
  m_two_electron_plus = new TH1D("two_electron_plus_mass","two_electron_plus_mass",60,61.,120.);
  m_thistSvc->regHist("/MyAnalysisAlg/Electron/two_electron_plus_mass",m_two_electron_plus).setChecked();
  m_two_electron_plus_pt = new TH1D("two_electron_plus_pt","two_electron_plus_pt",500,1.,500.);
  m_thistSvc->regHist("/MyAnalysisAlg/Electron/two_electron_plus_pt",m_two_electron_plus_pt).setChecked();
  //m_four_electron_plus->Sumw2();
  m_two_electron_ns_plus = new TH1D("two_electron_ns_plus_mass","two_electron_ns_plus_mass",130,1.,130.);
  m_thistSvc->regHist("/MyAnalysisAlg/Electron_Ns/two_electron_ns_plus_mass",m_two_electron_ns_plus).setChecked();
  //tau
  m_h_nTaus = new TH1D("h_nTau", "Number of tau", 11, -0.5, 10.5);
  m_thistSvc->regHist("/MyAnalysisAlg/Tau/h_nTau", m_h_nTaus).setChecked();
  m_h_tau_e = new TH1D("h_tau_e", "Tau energy [GeV]", 100, 0., 250.);
  m_thistSvc->regHist("/MyAnalysisAlg/Tau/h_tau_e", m_h_tau_e).setChecked();
  m_h_tau_pt = new TH1D("h_tau_pt", "Tau p_{T} [GeV]", 100, 0., 250.);
  m_thistSvc->regHist("/MyAnalysisAlg/Tau/h_tau_pt", m_h_tau_pt).setChecked();
  m_h_tau_eta = new TH1D("h_tau_eta", "Tau #eta", 100, -3.0, 3.0);
  m_thistSvc->regHist("/MyAnalysisAlg/Tau/h_tau_eta", m_h_tau_eta).setChecked();
  m_h_tau_phi = new TH1D("h_tau_phi", "Tau #phi [radian]", 100, -M_PI, M_PI);
  m_thistSvc->regHist("/MyAnalysisAlg/Tau/h_tau_phi", m_h_tau_phi).setChecked();
  m_h2_tau_eta_phi = new TH2D("h2_tau_eta_phi", "Tau #eta vs #phi", 100, -3.0, 3.0, 100, -M_PI, M_PI);
  m_thistSvc->regHist("/MyAnalysisAlg/Tau/h2_tau_eta_phi", m_h2_tau_eta_phi).setChecked();
  m_two_tau_plus = new TH1D("two_tau_plus_mass","two_tau_plus_mass",60,61.,120.);
  m_thistSvc->regHist("/MyAnalysisAlg/Tau/two_tau_plus_mass",m_two_tau_plus).setChecked();
  m_two_tau_plus_pt = new TH1D("two_tau_plus_pt","two_tau_plus_pt",500,1.,500.);
  m_thistSvc->regHist("/MyAnalysisAlg/Tau/two_tau_plus_pt",m_two_tau_plus_pt).setChecked();
  m_two_tau_ns_plus = new TH1D("two_tau_ns_plus_mass","two_tau_ns_plus_mass",130,1.,130.);
  m_thistSvc->regHist("/MyAnalysisAlg/Tau_Ns/two_tau_ns_plus_mass",m_two_tau_ns_plus).setChecked();
  //photon
  m_h_nPhotons = new TH1D("h_nPhoton", "Number of photon", 11, -0.5, 10.5);
  m_thistSvc->regHist("/MyAnalysisAlg/Photon/h_nPhoton", m_h_nPhotons).setChecked();
  m_h_photon_e = new TH1D("h_photon_e", "Photon energy [GeV]", 100, 0., 250.);
  m_thistSvc->regHist("/MyAnalysisAlg/Photon/h_photon_e", m_h_photon_e).setChecked();
  m_h_photon_pt = new TH1D("h_photon_pt", "Photon p_{T} [GeV]", 100, 0., 250.);
  m_thistSvc->regHist("/MyAnalysisAlg/Photon/h_photon_pt", m_h_photon_pt).setChecked();
  m_h_photon_eta = new TH1D("h_photon_eta", "Photon #eta", 100, -3.0, 3.0);
  m_thistSvc->regHist("/MyAnalysisAlg/Photon/h_photon_eta", m_h_photon_eta).setChecked();
  m_h_photon_phi = new TH1D("h_photon_phi", "Photon #phi [radian]", 100, -M_PI, M_PI);
  m_thistSvc->regHist("/MyAnalysisAlg/Photon/h_photon_phi", m_h_photon_phi).setChecked();
  m_h2_photon_eta_phi = new TH2D("h2_photon_eta_phi", "Photon #eta vs #phi", 100, -3.0, 3.0, 100, -M_PI, M_PI);
  m_thistSvc->regHist("/MyAnalysisAlg/Photon/h2_photon_eta_phi", m_h2_photon_eta_phi).setChecked();
  m_two_photon_plus = new TH1D("two_photon_plus_mass","two_photon_plus_mass",200,301.,500.);
  m_thistSvc->regHist("/MyAnalysisAlg/Photon/two_photon_plus_mass",m_two_photon_plus).setChecked();
  m_two_photon_ns_plus = new TH1D("two_photon_ns_plus_mass","two_photon_ns_plus_mass",131,0.,130.);
  m_thistSvc->regHist("/MyAnalysisAlg/Photon_Ns/two_photon_ns_plus_mass",m_two_photon_ns_plus).setChecked();
  //neutrino
  m_h_nNeutrinos = new TH1D("h_nNeutrino", "Number of neutrino", 11, -0.5, 10.5);
  m_thistSvc->regHist("/MyAnalysisAlg/Neutrino/h_nNeutrino", m_h_nNeutrinos).setChecked();
  m_h_neutrino_e = new TH1D("h_neutrino_e", "Neutrino energy [GeV]", 100, 0., 250.);
  m_thistSvc->regHist("/MyAnalysisAlg/Neutrino/h_neutrino_e", m_h_neutrino_e).setChecked();
  m_h_neutrino_pt = new TH1D("h_neutrino_pt", "Neutrino p_{T} [GeV]", 100, 0., 250.);
  m_thistSvc->regHist("/MyAnalysisAlg/Neutrino/h_neutrino_pt", m_h_neutrino_pt).setChecked();
  m_h_neutrino_eta = new TH1D("h_neutrino_eta", "Neutrino #eta", 100, -3.0, 3.0);
  m_thistSvc->regHist("/MyAnalysisAlg/Neutrino/h_neutrino_eta", m_h_neutrino_eta).setChecked();
  m_h_neutrino_phi = new TH1D("h_neutrino_phi", "Neutrino #phi [radian]", 100, -M_PI, M_PI);
  m_thistSvc->regHist("/MyAnalysisAlg/Neutrino/h_neutrino_phi", m_h_neutrino_phi).setChecked();
  m_h2_neutrino_eta_phi = new TH2D("h2_neutrino_eta_phi", "Neutrino #eta vs #phi", 100, -3.0, 3.0, 100, -M_PI, M_PI);
  m_thistSvc->regHist("/MyAnalysisAlg/Neutrino/h2_neutrino_eta_phi", m_h2_neutrino_eta_phi).setChecked();
 

  //Jet plot
  m_h_nJets = new TH1D("h_nJet", "Number of jet", 11, -0.5, 10.5);
  m_thistSvc->regHist("/MyAnalysisAlg/Jet/h_nJet", m_h_nJets).setChecked();
  m_h_jet_e = new TH1D("h_Jet_e", "Jet energy [GeV]", 100, 0., 250.);
  m_thistSvc->regHist("/MyAnalysisAlg/Jet/h_jet_e", m_h_jet_e).setChecked();
  m_h_jet_pt = new TH1D("h_jet_pt", "Jet p_{T} [GeV]", 100, 0., 250.);
  m_thistSvc->regHist("/MyAnalysisAlg/Jet/h_jet_pt", m_h_jet_pt).setChecked();
  m_h_jet_eta = new TH1D("h_jet_eta", "Jet #eta", 100, -3.0, 3.0);
  m_thistSvc->regHist("/MyAnalysisAlg/Jet/h_jet_eta", m_h_jet_eta).setChecked();
  m_h_jet_phi = new TH1D("h_jet_phi", "Jet #phi [radian]", 100, -M_PI, M_PI);
  m_thistSvc->regHist("/MyAnalysisAlg/Jet/h_jet_phi", m_h_jet_phi).setChecked();
  m_h2_jet_eta_phi = new TH2D("h2_jetn_eta_phi", "Jet #eta vs #phi", 100, -3.0, 3.0, 100, -M_PI, M_PI);
  m_thistSvc->regHist("/MyAnalysisAlg/Jet/h2_jet_eta_phi", m_h2_jet_eta_phi).setChecked();
  m_jet_mass = new TH1D("jet_mass","jet_mass",100,1.,10.);
  m_thistSvc->regHist("/MyAnalysisAlg/Jet/jet_mass",m_jet_mass).setChecked();
  m_two_jet_plus = new TH1D("two_jet_plus_mass","two_jet_plus_mass",401,200.,600.);
  m_thistSvc->regHist("/MyAnalysisAlg/Jet/two_jet_plus_mass",m_two_jet_plus).setChecked();
  //m_four_jet_plus->Sumw2();

  //missingET
  m_h_nMETs = new TH1D("h_nMETs", "Number of METs", 11, -0.5, 10.5);
  m_thistSvc->regHist("/MyAnalysisAlg/MET/h_nMETs", m_h_nMETs).setChecked();
  m_h_MET_e = new TH1D("h_MET_e", "MET energy [GeV]", 100, 0., 250.);
  m_thistSvc->regHist("/MyAnalysisAlg/MET/h_MET_e", m_h_MET_e).setChecked();
  m_h_MET_pt = new TH1D("h_MET_pt", "MET p_{T} [GeV]", 100, 1., 250.);
  m_thistSvc->regHist("/MyAnalysisAlg/MET/h_MET_pt", m_h_MET_pt).setChecked();
  m_h_MET_eta = new TH1D("h_MET_eta", "MET #eta", 100, -3.0, 3.0);
  m_thistSvc->regHist("/MyAnalysisAlg/MET/h_MET_eta", m_h_MET_eta).setChecked();
  m_h_MET_phi = new TH1D("h_MET_phi", "MET #phi [radian]", 100, -M_PI, M_PI);
  m_thistSvc->regHist("/MyAnalysisAlg/MET/h_MET_phi", m_h_MET_phi).setChecked();
  // m_h2_MET_eta_phi = new TH2D("h2_MET_eta_phi", "MET #eta vs #phi", 100, -3.0, 3.0, 100, -M_PI, M_PI);
  //m_thistSvc->regHist("/MyAnalysisAlg/MET/h2_MET_eta_phi", m_h2_MET_eta_phi).setChecked();


  //4e+4mu+(2e+2mu)
  m_4l_plus = new TH1D("4l_plus_mass","4l_plus_mass",500,1.,500.);
  m_thistSvc->regHist("/MyAnalysisAlg/Total/4l_plus_mass",m_4l_plus).setChecked();
  m_4l_plus_pt = new TH1D("4l_plus_pt","4l_plus_pt",500,1.,500.);
  m_thistSvc->regHist("/MyAnalysisAlg/Total/4l_plus_pt",m_4l_plus_pt).setChecked();
 
  //TTree
  four_tree = new TTree("fourTree", "Tree of four lepton");
  four_tree->Branch("m_four_phi", &m_four_phi, "m_four_phi/F");
  four_tree->Branch("m_four_eta", &m_four_eta, "m_four_eta/F");
  four_tree->Branch("m_four_pt", &m_four_pt, "m_four_pt/F");
  four_tree->Branch("m_four_e", &m_four_e, "m_four_e/F");
  four_tree->Branch("m_four_m", &m_four_m, "m_four_m/F");
  four_tree->Branch("m_mcEventWeight", &weight_four, "m_mcEventWeight/D");

  Zmumu_tree = new TTree("ZmumuTree", "Tree of Zboson and met");
  Zmumu_tree->Branch("m_Zboson_met_dphi", &m_Zboson_met_dphi, "m_Zboson_met_phi/F");
  Zmumu_tree->Branch("m_Zboson_eta", &m_Zboson_eta, "m_Zboson_eta/F");
  Zmumu_tree->Branch("m_Zboson_pt", &m_Zboson_pt, "m_Zboson_pt/F");
  Zmumu_tree->Branch("m_Zboson_m", &m_Zboson_m, "m_Zboson_m/F");
  Zmumu_tree->Branch("m_met_pt", &m_met_pt, "m_met_pt/F");
  Zmumu_tree->Branch("Zmumu_met_mass",&Zmumu_met_mass, "Zmumu_met_mass/F");
  Zmumu_tree->Branch("m_mcEventWeight", &weight_Zmumumet, "m_mcEventWeight/D");  
  
  ljet_tree = new TTree("ljetTree", "Tree of two Large-R Jet");
  ljet_tree->Branch("m_ljet_dphi", &m_ljet_dphi, "m_ljet_dphi/F");
  ljet_tree->Branch("m_ljet_eta_1st", &m_ljet_eta_1st, "m_ljet_eta_1st/F");
  ljet_tree->Branch("m_ljet_pt_1st", &m_ljet_pt_1st, "m_ljet_pt_1st/F");
  ljet_tree->Branch("m_ljet_m_1st", &m_ljet_m_1st, "m_ljet_m_1st/F");
  ljet_tree->Branch("m_ljet_eta_2nd", &m_ljet_eta_2nd, "m_ljet_eta_2nd/F");
  ljet_tree->Branch("m_ljet_pt_2nd", &m_ljet_pt_2nd, "m_ljet_pt_2nd/F");
  ljet_tree->Branch("m_ljet_m_2nd", &m_ljet_m_2nd, "m_ljet_m_2nd/F");
  ljet_tree->Branch("diljet_mass",&diljet_mass, "diljet_mass/F");
  ljet_tree->Branch("m_mcEventWeight", &weight_ljet, "m_mcEventWeight/D");

  //Wboson
  //lvlv
  Wlnu_tree = new TTree("WlnuTree", "Tree of Wlnulnu");
  Wlnu_tree->Branch("m_Wlnu_eta_1st", &m_Wlnu_eta_1st, "m_Wlnu_eta_1st/F");
  Wlnu_tree->Branch("m_Wlnu_pt_1st", &m_Wlnu_pt_1st, "m_Wlnu_pt_1st/F");
  Wlnu_tree->Branch("m_Wlnu_m_1st", &m_Wlnu_m_1st, "m_Wlnu_m_1st/F");
  Wlnu_tree->Branch("m_Wlnu_eta_2nd", &m_Wlnu_eta_2nd, "m_Wlnu_eta_2nd/F");
  Wlnu_tree->Branch("m_Wlnu_pt_2nd", &m_Wlnu_pt_2nd, "m_Wlnu_pt_2nd/F");
  Wlnu_tree->Branch("m_Wlnu_m_2nd", &m_Wlnu_m_2nd, "m_Wlnu_m_2nd/F");
  Wlnu_tree->Branch("m_Wlnu_met_pt",&m_Wlnu_met_pt, "m_Wlnu_met_pt/F");
  Wlnu_tree->Branch("m_Wlnu_mass",&m_Wlnu_mass, "m_Wlnu_mass/F");
  Wlnu_tree->Branch("m_mcEventWeight", &weight_Wlnu, "m_mcEventWeight/D");

  //lvqq
  Wl_ljet_tree = new TTree("Wl_ljetTree", "Tree of Wlnuljet");
  Wl_ljet_tree->Branch("m_Wl_ljet_ljet_eta", &m_Wl_ljet_ljet_eta, "m_Wl_ljet_ljet_eta/F");
  Wl_ljet_tree->Branch("m_Wl_ljet_ljet_pt", &m_Wl_ljet_ljet_pt, "m_Wl_ljet_ljet_pt/F");
  Wl_ljet_tree->Branch("m_Wl_ljet_ljet_m", &m_Wl_ljet_ljet_m, "m_Wl_ljet_ljet_m/F");
  Wl_ljet_tree->Branch("m_Wl_ljet_l_eta", &m_Wl_ljet_l_eta, "m_Wl_ljet_l_eta/F");
  Wl_ljet_tree->Branch("m_Wl_ljet_l_pt", &m_Wl_ljet_l_pt, "m_Wl_ljet_l_pt/F");
  Wl_ljet_tree->Branch("m_Wl_ljet_l_m", &m_Wl_ljet_l_m, "m_Wl_ljet_l_m/F");
  Wl_ljet_tree->Branch("m_Wl_ljet_met_pt",&m_Wl_ljet_met_pt, "m_Wl_ljet_met_pt/F");
  Wl_ljet_tree->Branch("m_Wl_ljet_lnu_m",&m_Wl_ljet_lnu_m, "m_Wl_ljet_lnu_m/F");
  Wl_ljet_tree->Branch("m_Wl_ljet_mass",&m_Wl_ljet_mass, "m_Wl_ljet_mass/F");
  Wl_ljet_tree->Branch("m_mcEventWeight", &weight_Wl_ljet, "m_mcEventWeight/D");

  Wl_sjet_tree = new TTree("Wl_sjetTree", "Tree of Wlnusjet");
  Wl_sjet_tree->Branch("m_Wl_sjet_sjet_eta_1st", &m_Wl_sjet_sjet_eta_1st, "m_Wl_sjet_sjet_eta_1st/F");
  Wl_sjet_tree->Branch("m_Wl_sjet_sjet_pt_1st", &m_Wl_sjet_sjet_pt_1st, "m_Wl_sjet_sjet_pt_1st/F");
  Wl_sjet_tree->Branch("m_Wl_sjet_sjet_m_1st", &m_Wl_sjet_sjet_m_1st, "m_Wl_sjet_sjet_m_1st/F");
  Wl_sjet_tree->Branch("m_Wl_sjet_sjet_eta_2nd", &m_Wl_sjet_sjet_eta_2nd, "m_Wl_sjet_sjet_eta_2nd/F");
  Wl_sjet_tree->Branch("m_Wl_sjet_sjet_pt_2nd", &m_Wl_sjet_sjet_pt_2nd, "m_Wl_sjet_sjet_pt_2nd/F");
  Wl_sjet_tree->Branch("m_Wl_sjet_sjet_m_2nd", &m_Wl_sjet_sjet_m_2nd, "m_Wl_sjet_sjet_m_2nd/F");
  Wl_sjet_tree->Branch("m_Wl_sjet_disjet_m", &m_Wl_sjet_disjet_m, "m_Wl_sjet_disjet_m/F");
  Wl_sjet_tree->Branch("m_Wl_sjet_l_eta", &m_Wl_sjet_l_eta, "m_Wl_sjet_l_eta/F");
  Wl_sjet_tree->Branch("m_Wl_sjet_l_pt", &m_Wl_sjet_l_pt, "m_Wl_sjet_l_pt/F");
  Wl_sjet_tree->Branch("m_Wl_sjet_l_m", &m_Wl_sjet_l_m, "m_Wl_sjet_l_m/F");
  Wl_sjet_tree->Branch("m_Wl_sjet_met_pt",&m_Wl_sjet_met_pt, "m_Wl_sjet_met_pt/F");
  Wl_sjet_tree->Branch("m_Wl_sjet_lnu_m",&m_Wl_sjet_lnu_m, "m_Wl_sjet_lnu_m/F");
  Wl_sjet_tree->Branch("m_Wl_sjet_mass",&m_Wl_sjet_mass, "m_Wl_sjet_mass/F");
  Wl_sjet_tree->Branch("m_mcEventWeight", &weight_Wl_sjet, "m_mcEventWeight/D");

  //tquark 
  tll_tree = new TTree("tllTTree", "Tree of ttbar");
  tll_tree->Branch("m_tll_dphi", &m_tll_dphi, "m_tll_dphi/F");
  tll_tree->Branch("m_tll_ljet_eta_1st", &m_tll_ljet_eta_1st, "m_tll_ljet_eta_1st/F");
  tll_tree->Branch("m_tll_ljet_pt_1st", &m_tll_ljet_pt_1st, "m_tll_ljet_pt_1st/F");
  tll_tree->Branch("m_tll_ljet_m_1st", &m_tll_ljet_m_1st, "m_tll_ljet_m_1st/F");
  tll_tree->Branch("m_tll_ljet_eta_2nd", &m_tll_ljet_eta_2nd, "m_tll_ljet_eta_2nd/F");
  tll_tree->Branch("m_tll_ljet_p_2ndt", &m_tll_ljet_pt_2nd, "m_tll_ljet_pt_2nd/F");
  tll_tree->Branch("m_tll_ljet_m_2nd", &m_tll_ljet_m_2nd, "m_tll_ljet_m_2nd/F");
  tll_tree->Branch("m_tll_mass",&m_tll_mass, "m_tll_mass/F");
  tll_tree->Branch("m_mcEventWeight", &weight_tll, "m_mcEventWeight/D");

  tWb_tree = new TTree("tWbTree", "Tree of tWb");
  tWb_tree->Branch("m_tWb_l_eta_1st", &m_tWb_l_eta_1st, "m_tWb_l_eta_1st/F");
  tWb_tree->Branch("m_tWb_l_pt_1st", &m_tWb_l_pt_1st, "m_tWb_l_pt_1st/F");
  tWb_tree->Branch("m_tWb_l_m_1st", &m_tWb_l_m_1st, "m_tWb_l_m_1st/F");
  tWb_tree->Branch("m_tWb_l_eta_2nd", &m_tWb_l_eta_2nd, "m_tWb_l_eta_2nd/F");
  tWb_tree->Branch("m_tWb_l_pt_2nd", &m_tWb_l_pt_2nd, "m_tWb_l_pt_2nd/F");
  tWb_tree->Branch("m_tWb_l_m_2nd", &m_tWb_l_m_2nd, "m_tWb_l_m_2nd/F");
  tWb_tree->Branch("m_tWb_met_pt", &m_tWb_met_pt, "m_tWb_met_pt/F");
  tWb_tree->Branch("m_tWb_b_eta_1st", &m_tWb_b_eta_1st, "m_tWb_b_eta_1st/F");
  tWb_tree->Branch("m_tWb_b_pt_1st", &m_tWb_b_pt_1st, "m_tWb_b_pt_1st/F");
  tWb_tree->Branch("m_tWb_b_m_1st", &m_tWb_b_m_1st, "m_tWb_b_m_1st/F");
  tWb_tree->Branch("m_tWb_b_eta_2nd", &m_tWb_b_eta_2nd, "m_tWb_b_eta_2nd/F");
  tWb_tree->Branch("m_tWb_b_pt_2nd", &m_tWb_b_pt_2nd, "m_tWb_b_pt_2nd/F");
  tWb_tree->Branch("m_tWb_b_m_2nd", &m_tWb_b_m_2nd, "m_tWb_b_m_2nd/F");
  tWb_tree->Branch("m_tWb_mass",&m_tWb_mass, "m_tWb_mass/F");
  tWb_tree->Branch("m_mcEventWeight", &weight_tWb, "m_mcEventWeight/D");

  tljet_b_tree = new TTree("tljet_bTree", "Tree of L-Rjet + 2b");
  tljet_b_tree->Branch("m_tljet_b_l_eta_1st", &m_tljet_b_l_eta_1st, "m_tljet_b_l_eta_1st/F");
  tljet_b_tree->Branch("m_tljet_b_l_pt_1st", &m_tljet_b_l_pt_1st, "m_tljet_b_l_pt_1st/F");
  tljet_b_tree->Branch("m_tljet_b_l_m_1st", &m_tljet_b_l_m_1st, "m_tljet_b_l_m_1st/F");
  tljet_b_tree->Branch("m_tljet_b_met_pt", &m_tljet_b_met_pt, "m_tljet_b_met_pt/F");
  tljet_b_tree->Branch("m_tljet_b_Wlnu_m", &m_tljet_b_lnu_m, "m_tljet_b_lnu_m/F");
  tljet_b_tree->Branch("m_tljet_b_ljet_eta", &m_tljet_b_ljet_eta, "m_tljet_b_ljet_eta/F");
  tljet_b_tree->Branch("m_tljet_b_ljet_pt", &m_tljet_b_ljet_pt, "m_tljet_b_ljet_pt/F");
  tljet_b_tree->Branch("m_tljet_b_ljet_m", &m_tljet_b_ljet_m, "m_tljet_b_ljet_m/F");
  tljet_b_tree->Branch("m_tljet_b_b_eta_1st", &m_tljet_b_b_eta_1st, "m_tljet_b_b_eta_1st/F");
  tljet_b_tree->Branch("m_tljet_b_b_pt_1st", &m_tljet_b_b_pt_1st, "m_tljet_b_b_pt_1st/F");
  tljet_b_tree->Branch("m_tljet_b_b_m_1st", &m_tljet_b_b_m_1st, "m_tljet_b_b_m_1st/F");
  tljet_b_tree->Branch("m_tljet_b_b_eta_2nd", &m_tljet_b_b_eta_2nd, "m_tljet_b_b_eta_2nd/F");
  tljet_b_tree->Branch("m_tljet_b_b_pt_2nd", &m_tljet_b_b_pt_2nd, "m_tljet_b_b_pt_2nd/F");
  tljet_b_tree->Branch("m_tljet_b_b_m_2nd", &m_tljet_b_b_m_2nd, "m_tljet_b_b_m_2nd/F");
  tljet_b_tree->Branch("m_tljet_b_mass",&m_tljet_b_mass, "m_tljet_b_mass/F");
  tljet_b_tree->Branch("m_mcEventWeight", &weight_tljet_b, "m_mcEventWeight/D");
  
  ttwoljet_b_tree = new TTree("ttwoLjet_bTree", "Tree of 2 L-Rjet + 2b");
  ttwoljet_b_tree->Branch("m_ttwoljet_b_ljet_eta_1st", &m_ttwoljet_b_ljet_eta_1st, "m_ttwoljet_b_ljet_eta_1st/F");
  ttwoljet_b_tree->Branch("m_ttwoljet_b_ljet_pt_1st", &m_ttwoljet_b_ljet_pt_1st, "m_ttwoljet_b_ljet_pt_1st/F");
  ttwoljet_b_tree->Branch("m_ttwoljet_b_ljet_m_1st", &m_ttwoljet_b_ljet_m_1st, "m_ttwoljet_b_ljet_m_1st/F");
  ttwoljet_b_tree->Branch("m_ttwoljet_b_ljet_eta_2nd", &m_ttwoljet_b_ljet_eta_2nd, "m_ttwoljet_b_ljet_eta_2nd/F");
  ttwoljet_b_tree->Branch("m_ttwoljet_b_ljet_pt_2nd", &m_ttwoljet_b_ljet_pt_2nd, "m_ttwoljet_b_ljet_pt_2nd/F");
  ttwoljet_b_tree->Branch("m_ttwoljet_b_ljet_m_2nd", &m_ttwoljet_b_ljet_m_2nd, "m_ttwoljet_b_ljet_m_2nd/F");
  ttwoljet_b_tree->Branch("m_ttwoljet_b_b_eta_1st", &m_ttwoljet_b_b_eta_1st, "m_ttwoljet_b_b_eta_1st/F");
  ttwoljet_b_tree->Branch("m_ttwoljet_b_b_pt_1st", &m_ttwoljet_b_b_pt_1st, "m_ttwoljet_b_b_pt_1st/F");
  ttwoljet_b_tree->Branch("m_ttwoljet_b_b_m_1st", &m_ttwoljet_b_b_m_1st, "m_ttwoljet_b_b_m_1st/F");
  ttwoljet_b_tree->Branch("m_ttwoljet_b_b_eta_2nd", &m_ttwoljet_b_b_eta_2nd, "m_ttwoljet_b_b_eta_2nd/F");
  ttwoljet_b_tree->Branch("m_ttwoljet_b_b_pt_2nd", &m_ttwoljet_b_b_pt_2nd, "m_ttwoljet_b_b_pt_2nd/F");
  ttwoljet_b_tree->Branch("m_ttwoljet_b_b_m_2nd", &m_ttwoljet_b_b_m_2nd, "m_ttwoljet_b_b_m_2nd/F");
  ttwoljet_b_tree->Branch("m_ttwoljet_b_mass",&m_ttwoljet_b_mass, "m_ttwoljet_b_mass/F");
  ttwoljet_b_tree->Branch("m_mcEventWeight", &weight_ttwoljet_b, "m_mcEventWeight/D");

  ttwoljet_oneb_tree = new TTree("ttwoljet_oneb_tree", "Tree of 2 L-Rjet + b");
  ttwoljet_oneb_tree->Branch("m_ttwoljet_oneb_tljet_eta", &m_ttwoljet_oneb_tljet_eta, "m_ttwoljet_oneb_tljet_eta/F");
  ttwoljet_oneb_tree->Branch("m_ttwoljet_oneb_tljet_pt", &m_ttwoljet_oneb_tljet_pt, "m_ttwoljet_oneb_tljet_pt/F");
  ttwoljet_oneb_tree->Branch("m_ttwoljet_oneb_tljet_m", &m_ttwoljet_oneb_tljet_m, "m_ttwoljet_oneb_tljet_m/F");
  ttwoljet_oneb_tree->Branch("m_ttwoljet_oneb_Wljet_eta", &m_ttwoljet_oneb_Wljet_eta, "m_ttwoljet_oneb_Wljet_eta/F");
  ttwoljet_oneb_tree->Branch("m_ttwoljet_oneb_Wljet_pt", &m_ttwoljet_oneb_Wljet_pt, "m_ttwoljet_oneb_Wljet_pt/F");
  ttwoljet_oneb_tree->Branch("m_ttwoljet_oneb_Wljet_m", &m_ttwoljet_oneb_Wljet_m, "m_ttwoljet_oneb_Wljet_m/F");
  ttwoljet_oneb_tree->Branch("m_ttwoljet_oneb_b_eta", &m_ttwoljet_oneb_b_eta, "m_ttwoljet_oneb_b_eta/F");
  ttwoljet_oneb_tree->Branch("m_ttwoljet_oneb_b_pt", &m_ttwoljet_oneb_b_pt, "m_ttwoljet_oneb_b_pt/F");
  ttwoljet_oneb_tree->Branch("m_ttwoljet_oneb_b_m", &m_ttwoljet_oneb_b_m, "m_ttwoljet_oneb_b_m/F");
  ttwoljet_oneb_tree->Branch("m_ttwoljet_oneb_mass", &m_ttwoljet_oneb_mass, "m_ttwoljet_oneb_mass/F");
  ttwoljet_oneb_tree->Branch("m_mcEventWeight", &weight_ttwoljet_oneb, "m_mcEventWeight/D");

  tljetWb_tree = new TTree("tljetWb_tree", "Tree of L-Rjet + Wb");
  tljetWb_tree->Branch("m_tljetWb_ljet_eta", &m_tljetWb_ljet_eta, "m_tljetWb_ljet_eta/F");
  tljetWb_tree->Branch("m_tljetWb_ljet_pt", &m_tljetWb_ljet_pt, "m_tljetWb_ljet_pt/F");
  tljetWb_tree->Branch("m_tljetWb_ljet_m", &m_tljetWb_ljet_m, "m_tljetWb_ljet_m/F");
  tljetWb_tree->Branch("m_tljetWb_l_eta", &m_tljetWb_l_eta, "m_tljetWb_l_eta/F");
  tljetWb_tree->Branch("m_tljetWb_l_pt", &m_tljetWb_l_pt, "m_tljetWb_l_pt/F");
  tljetWb_tree->Branch("m_tljetWb_l_m", &m_tljetWb_l_m, "m_tljetWb_l_m/F");
  tljetWb_tree->Branch("m_tljetWb_met_pt", &m_tljetWb_met_pt, "m_tljetWb_met_pt/F");
  tljetWb_tree->Branch("m_tljetWb_b_eta", &m_tljetWb_b_eta, "m_tljetWb_b_eta/F");
  tljetWb_tree->Branch("m_tljetWb_b_pt", &m_tljetWb_b_pt, "m_tljetWb_b_pt/F");
  tljetWb_tree->Branch("m_tljetWb_b_m", &m_tljetWb_b_m, "m_tljetWb_b_m/F");
  tljetWb_tree->Branch("m_tljetWb_mass", &m_tljetWb_mass, "m_tljetWb_mass/F");
  tljetWb_tree->Branch("m_mcEventWeight", &weight_tljetWb, "m_mcEventWeight/D");

  //higgs
  Hhh_tree = new TTree("HhhTree", "Tree of Hhh");
  Hhh_tree->Branch("m_Hhh_dphi", &m_Hhh_dphi, "m_Hhh_dphi/F");
  Hhh_tree->Branch("m_Hhh_b_eta_1st", &m_Hhh_b_eta_1st, "m_Hhh_b_eta_1st/F");
  Hhh_tree->Branch("m_Hhh_b_pt_1st", &m_Hhh_b_pt_1st, "m_Hhh_b_pt_1st/F");
  Hhh_tree->Branch("m_Hhh_b_m_1st", &m_Hhh_b_m_1st, "m_Hhh_b_m_1st/F");
  Hhh_tree->Branch("m_Hhh_b_eta_2nd", &m_Hhh_b_eta_2nd, "m_Hhh_b_eta_2nd/F");
  Hhh_tree->Branch("m_Hhh_b_pt_2nd", &m_Hhh_b_pt_2nd, "m_Hhh_b_pt_2nd/F");
  Hhh_tree->Branch("m_Hhh_b_m_2nd", &m_Hhh_b_m_2nd, "m_Hhh_b_m_2nd/F");
  Hhh_tree->Branch("m_Hhh_b_eta_3rd", &m_Hhh_b_eta_3rd, "m_Hhh_b_eta_3rd/F");
  Hhh_tree->Branch("m_Hhh_b_pt_3rd", &m_Hhh_b_pt_3rd, "m_Hhh_b_pt_3rd/F");
  Hhh_tree->Branch("m_Hhh_b_m_3rd", &m_Hhh_b_m_3rd, "m_Hhh_b_m_3rd/F");
  Hhh_tree->Branch("m_Hhh_b_eta_4th", &m_Hhh_b_eta_4th, "m_Hhh_b_eta_4th/F");
  Hhh_tree->Branch("m_Hhh_b_pt_4th", &m_Hhh_b_pt_4th, "m_Hhh_b_pt_4th/F");
  Hhh_tree->Branch("m_Hhh_b_m_4th", &m_Hhh_b_m_4th, "m_Hhh_b_m_4th/F");
  Hhh_tree->Branch("m_Hhh_h_m_1st", &m_Hhh_h_m_1st, "m_Hhh_h_m_1st/F");
  Hhh_tree->Branch("m_Hhh_h_m_2nd", &m_Hhh_h_m_2nd, "m_Hhh_h_m_2nd/F");
  Hhh_tree->Branch("m_Hhh_mass",&m_Hhh_mass, "m_Hhh_mass/F");
  Hhh_tree->Branch("m_mcEventWeight", &weight_Hhh, "m_mcEventWeight/D");


  truthljet_tree = new TTree("truthljetTree", "Tree of two Large-R Jet");
  truthljet_tree->Branch("m_truthljet_dphi", &m_truthljet_dphi, "m_truthljet_dphi/F");
  truthljet_tree->Branch("m_truthljet_eta_1st", &m_truthljet_eta_1st, "m_truthljet_eta_1st/F");
  truthljet_tree->Branch("m_truthljet_pt_1st", &m_truthljet_pt_1st, "m_truthljet_pt_1st/F");
  truthljet_tree->Branch("m_truthljet_m_1st", &m_truthljet_m_1st, "m_truthljet_m_1st/F");
  truthljet_tree->Branch("m_truthljet_eta_2nd", &m_truthljet_eta_2nd, "m_truthljet_eta_2nd/F");
  truthljet_tree->Branch("m_truthljet_pt_2nd", &m_truthljet_pt_2nd, "m_truthljet_pt_2nd/F");
  truthljet_tree->Branch("m_truthljet_m_2nd", &m_truthljet_m_2nd, "m_truthljet_m_2nd/F");
  truthljet_tree->Branch("ditruthljet_mass",&ditruthljet_mass, "ditruthljet_mass/F");
  truthljet_tree->Branch("m_mcEventWeight", &weight_truthljet, "m_mcEventWeight/D");

  bjet_tree = new TTree("bjetTree", "Tree of di-bJet");
  bjet_tree->Branch("m_bjet_dphi", &m_bjet_dphi, "m_bjet_dphi/F");
  bjet_tree->Branch("m_bjet_eta_1st", &m_bjet_eta_1st, "m_bjet_eta_1st/F");
  bjet_tree->Branch("m_bjet_pt_1st", &m_bjet_pt_1st, "m_bjet_pt_1st/F");
  bjet_tree->Branch("m_bjet_m_1st", &m_bjet_m_1st, "m_bjet_m_1st/F");
  bjet_tree->Branch("m_bjet_eta_2nd", &m_bjet_eta_2nd, "m_bjet_eta_2nd/F");
  bjet_tree->Branch("m_bjet_pt_2nd", &m_bjet_pt_2nd, "m_bjet_pt_2nd/F");
  bjet_tree->Branch("m_bjet_m_2nd", &m_bjet_m_2nd, "m_bjet_m_2nd/F");
  bjet_tree->Branch("m_mcEventWeight", &weight_bjet, "m_mcEventWeight/D");
  bjet_tree->Branch("dibjet_mass",&dibjet_mass, "dibjet_mass/F");

  truthbjet_tree = new TTree("truthbjetTree", "Tree of di-bJet");
  truthbjet_tree->Branch("m_truthbjet_dphi", &m_truthbjet_dphi, "m_truthbjet_dphi/F");
  truthbjet_tree->Branch("m_truthbjet_eta_1st", &m_truthbjet_eta_1st, "m_truthbjet_eta_1st/F");
  truthbjet_tree->Branch("m_truthbjet_pt_1st", &m_truthbjet_pt_1st, "m_truthbjet_pt_1st/F");
  truthbjet_tree->Branch("m_truthbjet_m_1st", &m_truthbjet_m_1st, "m_truthbjet_m_1st/F");
  truthbjet_tree->Branch("m_truthbjet_eta_2nd", &m_truthbjet_eta_2nd, "m_truthbjet_eta_2nd/F");
  truthbjet_tree->Branch("m_truthbjet_pt_2nd", &m_truthbjet_pt_2nd, "m_truthbjet_pt_2nd/F");
  truthbjet_tree->Branch("m_truthbjet_m_2nd", &m_truthbjet_m_2nd, "m_truthbjet_m_2nd/F");
  truthbjet_tree->Branch("m_mcEventWeight", &weight_truthbjet, "m_mcEventWeight/D"); 
  truthbjet_tree->Branch("ditruthbjet_mass",&ditruthbjet_mass, "ditruthbjet_mass/F");

  tau_tree = new TTree("tauTree", "Tree of di-tau");
  tau_tree->Branch("m_tau_dphi", &m_tau_dphi, "m_tau_dphi/F");
  tau_tree->Branch("m_tau_eta_1st", &m_tau_eta_1st, "m_tau_eta_1st/F");
  tau_tree->Branch("m_tau_pt_1st", &m_tau_pt_1st, "m_tau_pt_1st/F");
  tau_tree->Branch("m_tau_m_1st", &m_tau_m_1st, "m_tau_m_1st/F");
  tau_tree->Branch("m_tau_eta_2nd", &m_tau_eta_2nd, "m_tau_eta_2nd/F");
  tau_tree->Branch("m_tau_pt_2nd", &m_tau_pt_2nd, "m_tau_pt_2nd/F");
  tau_tree->Branch("m_tau_m_2nd", &m_tau_m_2nd, "m_tau_m_2nd/F");
  tau_tree->Branch("m_tau_met_e" ,&m_tau_met_e, "m_tau_met_e");
  tau_tree->Branch("m_mcEventWeight", &weight_tau, "m_mcEventWeight/D");
  tau_tree->Branch("ditau_mass",&ditau_mass, "ditau_mass/F");

  ltau_htau_tree = new TTree("ltau_htauTree", "Tree of leptionic and hadronic tau");
  ltau_htau_tree->Branch("m_ltau_htau_dphi", &m_ltau_htau_dphi, "m_ltau_htau_dphi/F");
  ltau_htau_tree->Branch("m_ltau_lepton_pt", &m_ltau_lepton_pt, "m_ltau_lepton_pt/F");
  ltau_htau_tree->Branch("m_ltau_lepton_m", &m_ltau_lepton_m, "m_ltau_lepton_m/F");
  ltau_htau_tree->Branch("m_ltau_lepton_eta", &m_ltau_lepton_eta, "m_ltau_lepton_eta/F");
  ltau_htau_tree->Branch("m_htau_eta", &m_htau_eta, "m_htau_eta/F");
  ltau_htau_tree->Branch("m_htau_pt", &m_htau_pt, "m_htau_pt/F");
  ltau_htau_tree->Branch("m_ltau_htau_met_e" ,&m_ltau_htau_met_e, "m_ltau_htau_met_e");
  ltau_htau_tree->Branch("m_mcEventWeight", &weight_ltau_htau, "m_mcEventWeight/D");
  ltau_htau_tree->Branch("ltau_htau_mass",&ltau_htau_mass, "ltau_htau_mass/F");


  truthtau_tree = new TTree("truthtauTree", "Tree of di-tau");
  truthtau_tree->Branch("m_truthtau_dphi", &m_truthtau_dphi, "m_truthtau_dphi/F");
  truthtau_tree->Branch("m_truthtau_eta_1st", &m_truthtau_eta_1st, "m_truthtau_eta_1st/F");
  truthtau_tree->Branch("m_truthtau_pt_1st", &m_truthtau_pt_1st, "m_truthtau_pt_1st/F");
  truthtau_tree->Branch("m_truthtau_m_1st", &m_truthtau_m_1st, "m_truthtau_m_1st/F");
  truthtau_tree->Branch("m_truthtau_eta_2nd", &m_truthtau_eta_2nd, "m_truthtau_eta_2nd/F");
  truthtau_tree->Branch("m_truthtau_pt_2nd", &m_truthtau_pt_2nd, "m_truthtau_pt_2nd/F");
  truthtau_tree->Branch("m_truthtau_m_2nd", &m_truthtau_m_2nd, "m_truthtau_m_2nd/F");
  truthtau_tree->Branch("m_mcEventWeight", &weight_truthtau, "m_mcEventWeight/D");
  truthtau_tree->Branch("ditruthtau_mass",&ditruthtau_mass, "ditruthtau_mass/F");

  photon_tree = new TTree("photonTree", "Tree of di-photon");
  photon_tree->Branch("m_photon_dphi", &m_photon_dphi, "m_photon_dphi/F");
  photon_tree->Branch("m_photon_eta_1st", &m_photon_eta_1st, "m_photon_eta_1st/F");
  photon_tree->Branch("m_photon_pt_1st", &m_photon_pt_1st, "m_photon_pt_1st/F");
  photon_tree->Branch("m_photon_m_1st", &m_photon_m_1st, "m_photon_m_1st/F");
  photon_tree->Branch("m_photon_eta_2nd", &m_photon_eta_2nd, "m_photon_eta_2nd/F");
  photon_tree->Branch("m_photon_pt_2nd", &m_photon_pt_2nd, "m_photon_pt_2nd/F");
  photon_tree->Branch("m_photon_m_2nd", &m_photon_m_2nd, "m_photon_m_2nd/F");
  photon_tree->Branch("m_mcEventWeight", &weight_photon, "m_mcEventWeight/D");
  photon_tree->Branch("diphoton_mass",&diphoton_mass, "diphoton_mass/F");

  truthphoton_tree = new TTree("truthphotonTree", "Tree of di-photon");
  truthphoton_tree->Branch("m_truthphoton_dphi", &m_truthphoton_dphi, "m_truthphoton_dphi/F");
  truthphoton_tree->Branch("m_truthphoton_eta_1st", &m_truthphoton_eta_1st, "m_truthphoton_eta_1st/F");
  truthphoton_tree->Branch("m_truthphoton_pt_1st", &m_truthphoton_pt_1st, "m_truthphoton_pt_1st/F");
  truthphoton_tree->Branch("m_truthphoton_m_1st", &m_truthphoton_m_1st, "m_truthphoton_m_1st/F");
  truthphoton_tree->Branch("m_truthphoton_eta_2nd", &m_truthphoton_eta_2nd, "m_truthphoton_eta_2nd/F");
  truthphoton_tree->Branch("m_truthphoton_pt_2nd", &m_truthphoton_pt_2nd, "m_truthphoton_pt_2nd/F");
  truthphoton_tree->Branch("m_truthphoton_m_2nd", &m_truthphoton_m_2nd, "m_truthphoton_m_2nd/F");
  truthphoton_tree->Branch("m_mcEventWeight", &weight_truthphoton, "m_mcEventWeight/D");
  truthphoton_tree->Branch("ditruthphoton_mass",&ditruthphoton_mass, "ditruthphoton_mass/F");
  
  twol_ljet_tree = new TTree("Twolep_ljetTree", "Tree of two lepton + Large-R Jet");
  twol_ljet_tree->Branch("m_twol_ljet_dphi", &m_twol_ljet_dphi, "m_twol_ljet_dphi/F");
  twol_ljet_tree->Branch("m_twol_ljet_eta", &m_twol_ljet_eta, "m_twol_ljet_eta/F");
  twol_ljet_tree->Branch("m_twol_ljet_pt", &m_twol_ljet_pt, "m_twol_ljet_pt/F");
  twol_ljet_tree->Branch("m_twol_ljet_m", &m_twol_ljet_m, "m_twol_ljet_m/F");
  twol_ljet_tree->Branch("m_twol_Zboson_eta", &m_twol_Zboson_eta, "m_twol_Zboson_eta/F");
  twol_ljet_tree->Branch("m_twol_Zboson_pt", &m_twol_Zboson_pt, "m_twol_Zboson_pt/F");
  twol_ljet_tree->Branch("m_twol_Zboson_m", &m_twol_Zboson_m, "m_twol_Zboson_m/F");
  twol_ljet_tree->Branch("twol_ljet_mass",&twol_ljet_mass, "twol_ljet_mass/F");
  twol_ljet_tree->Branch("m_mcEventWeight", &weight_twol_ljet, "m_mcEventWeight/D");

  twol_truthljet_tree = new TTree("Twolep_truthljetTree", "Tree of two lepton + Large-R Jet");
  twol_truthljet_tree->Branch("m_twol_truthljet_dphi", &m_twol_truthljet_dphi, "m_twol_truthljet_dphi/F");
  twol_truthljet_tree->Branch("m_twol_truthljet_eta", &m_twol_truthljet_eta, "m_twol_truthljet_eta/F");
  twol_truthljet_tree->Branch("m_twol_truthljet_pt", &m_twol_truthljet_pt, "m_twol_truthljet_pt/F");
  twol_truthljet_tree->Branch("m_twol_truthljet_m", &m_twol_truthljet_m, "m_twol_truthljet_m/F");
  twol_truthljet_tree->Branch("m_twol_truthZboson_eta", &m_twol_truthZboson_eta, "m_twol_truthZboson_eta/F");
  twol_truthljet_tree->Branch("m_twol_truthZboson_pt", &m_twol_truthZboson_pt, "m_twol_truthZboson_pt/F");
  twol_truthljet_tree->Branch("m_twol_truthZboson_m", &m_twol_truthZboson_m, "m_twol_truthZboson_m/F");
  twol_truthljet_tree->Branch("twol_truthljet_mass",&twol_truthljet_mass, "twol_truthljet_mass/F");
  twol_truthljet_tree->Branch("m_mcEventWeight", &weight_twol_truthljet, "m_mcEventWeight/D");

  //2l + 2q
  twol_sjet_tree = new TTree("Twolep_sjetTree", "Tree of two lepton + 2quark");
  twol_sjet_tree->Branch("m_twol_sjet_dphi", &m_twol_sjet_dphi, "m_twol_sjet_dphi/F");
  twol_sjet_tree->Branch("m_twol_sjet_first_eta", &m_twol_sjet_first_eta, "m_twol_sjet_first_eta/F");
  twol_sjet_tree->Branch("m_twol_sjet_first_pt", &m_twol_sjet_first_pt, "m_twol_sjet_first_pt/F");
  twol_sjet_tree->Branch("m_twol_sjet_first_m", &m_twol_sjet_first_m, "m_twol_sjet_first_m/F");
  twol_sjet_tree->Branch("m_twol_sjet_second_eta", &m_twol_sjet_second_eta, "m_twol_sjet_second_eta/F");
  twol_sjet_tree->Branch("m_twol_sjet_second_pt", &m_twol_sjet_second_pt, "m_twol_sjet_second_pt/F");
  twol_sjet_tree->Branch("m_twol_sjet_second_m", &m_twol_sjet_second_m, "m_twol_sjet_second_m/F");
  twol_sjet_tree->Branch("m_twol_sjet_mass", &m_twol_sjet_m, "m_twol_sjet_mass/F");
  twol_sjet_tree->Branch("m_Zboson_eta", &m_twol_sjet_Zboson_eta, "m_Zboson_eta/F");
  twol_sjet_tree->Branch("m_Zboson_pt", &m_twol_sjet_Zboson_pt, "m_Zboson_pt/F");
  twol_sjet_tree->Branch("m_Zboson_m", &m_twol_sjet_Zboson_m, "m_Zboson_m/F");
  twol_sjet_tree->Branch("twol_sjet_mass",&twol_sjet_mass, "twol_sjet_mass/F");
  twol_sjet_tree->Branch("m_mcEventWeight", &weight_twol_sjet, "m_mcEventWeight/D");

  twol_truthsjet_tree = new TTree("Twolep_truthsjetTree", "Tree of two lepton + 2quark");
  twol_truthsjet_tree->Branch("m_twol_truthsjet_dphi", &m_twol_truthsjet_dphi, "m_twol_truthsjet_dphi/F");
  twol_truthsjet_tree->Branch("m_twol_truthsjet_first_eta", &m_twol_truthsjet_first_eta, "m_twol_truthsjet_first_eta/F");
  twol_truthsjet_tree->Branch("m_twol_truthsjet_first_pt", &m_twol_truthsjet_first_pt, "m_twol_truthsjet_first_pt/F");
  twol_truthsjet_tree->Branch("m_twol_truthsjet_first_m", &m_twol_truthsjet_first_m, "m_twol_truthsjet_first_m/F");
  twol_truthsjet_tree->Branch("m_twol_truthsjet_second_eta", &m_twol_truthsjet_second_eta, "m_twol_truthsjet_second_eta/F");
  twol_truthsjet_tree->Branch("m_twol_truthsjet_second_pt", &m_twol_truthsjet_second_pt, "m_twol_truthsjet_second_pt/F");
  twol_truthsjet_tree->Branch("m_twol_truthsjet_second_m", &m_twol_truthsjet_second_m, "m_twol_truthsjet_second_m/F");
  twol_truthsjet_tree->Branch("m_twol_truthsjet_mass", &m_twol_truthsjet_m, "m_twol_truthsjet_mass/F");
  twol_truthsjet_tree->Branch("m_truthZboson_eta", &m_twol_truthsjet_Zboson_eta, "m_truthZboson_eta/F");
  twol_truthsjet_tree->Branch("m_truthZboson_pt", &m_twol_truthsjet_Zboson_pt, "m_truthZboson_pt/F");
  twol_truthsjet_tree->Branch("m_truthZboson_m", &m_twol_truthsjet_Zboson_m, "m_truthZboson_m/F");
  twol_truthsjet_tree->Branch("twol_truthsjet_mass",&twol_truthsjet_mass, "twol_truthsjet_mass/F");
  twol_truthsjet_tree->Branch("m_mcEventWeight", &weight_twol_truthsjet, "m_mcEventWeight/D");

  //4q
  four_sjet_tree = new TTree("four_sjettree", "Tree of 2quark + 2quark");
  four_sjet_tree->Branch("m_four_sjet_mass", &m_four_sjet_mass, "m_four_sjet_mass/F");
  four_sjet_tree->Branch("m_mcEventWeight", &weight_four_sjet, "m_mcEventWeight/D");

  four_truthsjet_tree = new TTree("four_truthsjettree", "truthTree of 2quark + 2quark");
  four_truthsjet_tree->Branch("m_four_truthsjet_mass", &m_four_truthsjet_mass, "m_four_truthsjet_mass/F");
  four_truthsjet_tree->Branch("m_two_truthsjet_mass_1st", &m_two_truthsjet_mass_1st, "m_two_truthsjet_mass_1st/F");
  four_truthsjet_tree->Branch("m_two_truthsjet_mass_2nd", &m_two_truthsjet_mass_2nd, "m_two_truthsjet_mass_2nd/F"); 
  four_truthsjet_tree->Branch("m_mcEventWeight", &weight_four_truthsjet, "m_mcEventWeight/D");

  //

  ratio_tree = new TTree("RatioTree","Hist Ratio magnitude");
  ratio_tree->Branch("m_ratio", &h_ratio, "m_ratio/D");
  ratio_tree->Branch("m_sumOfWeights_tlv", &m_sumOfWeights, "m_sumOfWeights_tlv/D");

  check_tree = new TTree("CheckTree", "Check Tree error");
  check_tree->Branch("check_tau_mass", &check_tau_mass, "check_tau_mass");

  m_tree = new TTree("MyTree", "Ntuple variables collection");
  m_tree->Branch("runNumber", &m_runNumber, "runNumber/I");
  m_tree->Branch("lumiBlock", &m_lumiBlock, "lumiBlock/I");
  m_tree->Branch("eventNumber", &m_eventNumber, "eventNumber/I");
  m_tree->Branch("nMuons", &m_nMuons, "nMuons/I");
  m_muon_author = new std::vector<int>;
  m_tree->Branch("muon_author", &m_muon_author);
  m_muon_phi = new std::vector<double>;
  m_tree->Branch("muon_phi", &m_muon_phi);
  m_muon_eta = new std::vector<double>;
  m_tree->Branch("muon_eta", &m_muon_eta);
  m_muon_pt = new std::vector<double>;
  m_tree->Branch("muon_pt", &m_muon_pt);
  m_muon_e = new std::vector<double>;
  m_tree->Branch("muon_e", &m_muon_e);
  m_muon_charge = new std::vector<double>;
  m_tree->Branch("muon_charge", &m_muon_charge);

  //cut seperating

  //all two
  m_h_Zboson_pt_ptcut = new TH1D("h_Zboson_pt_ptcut", "Zboson p_{T} [GeV]", 500, 1., 500.);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/Zboson/h_Zboson_pt_ptcut", m_h_Zboson_pt_ptcut).setChecked();
  m_h_Zboson_mass_ptcut = new TH1D("h_Zboson_mass_ptcut", "Mass of Zboson", 500, 1, 500);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/Zboson/h_Zboson_mass_ptcut", m_h_Zboson_mass_ptcut).setChecked();
  m_h_Zboson_pt_masscut = new TH1D("h_Zboson_pt_masscut", "Zboson p_{T} [GeV]", 500, 1., 500.);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/Zboson/h_Zboson_pt_masscut", m_h_Zboson_pt_masscut).setChecked();
  m_h_Zboson_mass_masscut = new TH1D("h_Zboson_mass_masscut", "Mass of Zboson", 500, 1, 500);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/Zboson/h_Zboson_mass_masscut", m_h_Zboson_mass_masscut).setChecked();
  
  // all four
  m_h_two_Zboson_pt_allptcut = new TH1D("h_two_Zboson_pt_allptcut", "two Zboson p_{T} [GeV]", 500, 1., 500.);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/Zboson/h_two_Zboson_pt_allptcutt", m_h_two_Zboson_pt_allptcut).setChecked();
  m_h_two_Zboson_pt_muonptmasscut = new TH1D("h_two_Zboson_pt_muonptmasscut", "two Zboson p_{T} [GeV]", 500, 1., 500.);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/Zboson/h_two_Zboson_pt_muonptmasscutt", m_h_two_Zboson_pt_muonptmasscut).setChecked();
  m_h_two_Zboson_pt_electronptmasscut = new TH1D("h_two_Zboson_pt_electronptmasscut", "two Zboson p_{T} [GeV]", 500, 1., 500.);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/Zboson/h_two_Zboson_pt_electronptmasscutt", m_h_two_Zboson_pt_electronptmasscut).setChecked();

  m_h_two_Zboson_100_electroncut = new TH1D("h_two_Zboson_100_electroncut", "two Zboson Mass [GeV]", 500, 1., 500.);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/Zboson/h_two_Zboson_100_electroncut", m_h_two_Zboson_100_electroncut).setChecked();
  m_h_two_Zboson_75_electroncut = new TH1D("h_two_Zboson_75_electroncut", "two Zboson Mass [GeV]", 500, 1., 500.);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/Zboson/h_two_Zboson_75_electroncut", m_h_two_Zboson_75_electroncut).setChecked();
  m_h_two_Zboson_50_electroncut = new TH1D("h_two_Zboson_50_electroncut", "two Zboson Mass [GeV]", 500, 1., 500.);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/Zboson/h_two_Zboson_50_electroncut", m_h_two_Zboson_50_electroncut).setChecked();

  m_h_two_Zboson_100_allcut = new TH1D("h_two_Zboson_100_allcut", "two Zboson Mass [GeV]", 500, 1., 500.);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/Zboson/h_two_Zboson_100_allcut", m_h_two_Zboson_100_allcut).setChecked();
  m_h_two_Zboson_75_allcut = new TH1D("h_two_Zboson_75_allcut", "two Zboson Mass [GeV]", 500, 1., 500.);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/Zboson/h_two_Zboson_75_allcut", m_h_two_Zboson_75_allcut).setChecked();
  m_h_two_Zboson_50_allcut = new TH1D("h_two_Zboson_50_allcut", "two Zboson Mass [GeV]", 500, 1., 500.);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/Zboson/h_two_Zboson_50_allcut", m_h_two_Zboson_50_allcut).setChecked();

  m_h_two_Zboson_100_muoncut = new TH1D("h_two_Zboson_100_muoncut", "two Zboson Mass [GeV]", 500, 1., 500.);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/Zboson/h_two_Zboson_100_muoncut", m_h_two_Zboson_100_muoncut).setChecked();
  m_h_two_Zboson_75_muoncut = new TH1D("h_two_Zboson_75_muoncut", "two Zboson Mass [GeV]", 500, 1., 500.);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/Zboson/h_two_Zboson_75_muoncut", m_h_two_Zboson_75_muoncut).setChecked();
  m_h_two_Zboson_50_muoncut = new TH1D("h_two_Zboson_50_muoncut", "two Zboson Mass [GeV]", 500, 1., 500.);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/Zboson/h_two_Zboson_50_muoncut", m_h_two_Zboson_50_muoncut).setChecked();
  

  //signal
  m_4l_plus_allptcut = new TH1D("4l_plus_mass_allptcut","4l_plus_mass_allptcut",500,1.,500.);
  m_thistSvc->regHist("/MyAnalysisAlg/Total/4l_plus_mass_allptcut",m_4l_plus_allptcut).setChecked();
  m_4l_plus_pt_allptcut = new TH1D("4l_plus_pt_allptcut","4l_plus_pt_allptcut",500,1.,500.);
  m_thistSvc->regHist("/MyAnalysisAlg/Total/4l_plus_pt_allptcut",m_4l_plus_pt_allptcut).setChecked();
  m_4l_plus_muonptmasscut = new TH1D("4l_plus_mass_muonptmasscut","4l_plus_mass_muonptmasscut",500,1.,500.);
  m_thistSvc->regHist("/MyAnalysisAlg/Total/4l_plus_mass_muonptmasscut",m_4l_plus_muonptmasscut).setChecked();
  m_4l_plus_pt_muonptmasscut = new TH1D("4l_plus_pt_muonptmasscut","4l_plus_pt_muonptmasscut",500,1.,500.);
  m_thistSvc->regHist("/MyAnalysisAlg/Total/4l_plus_pt_muonptmasscut",m_4l_plus_pt_muonptmasscut).setChecked();
  m_4l_plus_electronptmasscut = new TH1D("4l_plus_mass_electronptmasscut","4l_plus_mass_electronptmasscut",500,1.,500.);
  m_thistSvc->regHist("/MyAnalysisAlg/Total/4l_plus_masselectronptmasscut",m_4l_plus_electronptmasscut).setChecked();
  m_4l_plus_pt_electronptmasscut = new TH1D("4l_plus_ptelectronptmasscut","4l_plus_ptelectronptmasscut",500,1.,500.);
  m_thistSvc->regHist("/MyAnalysisAlg/Total/4l_plus_ptelectronptmasscut",m_4l_plus_pt_electronptmasscut).setChecked();
  m_4l_plus_nocut = new TH1D("4l_plus_mass_nocut","4l_plus_mass_nocut",500,1.,500.);
  m_thistSvc->regHist("/MyAnalysisAlg/Total/4l_plus_mass_nocut",m_4l_plus_nocut).setChecked();
  m_4l_plus_pt_nocut = new TH1D("4l_plus_pt_nocut","4l_plus_pt_nocut",500,1.,500.);
  m_thistSvc->regHist("/MyAnalysisAlg/Total/4l_plus_pt_nocut",m_4l_plus_pt_nocut).setChecked();
  
  m_4l_plus_eta_nocut = new TH1D("m_4l_plus_eta_nocut", "m_4l_plus_eta_nocut #eta", 100, -3.0, 3.0);
  m_thistSvc->regHist("/MyAnalysisAlg/Total/4l_plus_eta_nocut", m_4l_plus_eta_nocut).setChecked();
  m_2l_plus_eta_ptcut = new TH1D("m_2l_plus_eta_ptcut", "m_2l_plus_eta_ptcut #eta", 100, -3.0, 3.0);
  m_thistSvc->regHist("/MyAnalysisAlg/Total/2l_plus_eta_ptcut", m_2l_plus_eta_ptcut).setChecked();
 
  m_Zboson_met = new TH1D("m_Zboson_met","m_Zboson_met",500,1.,500.);
  m_thistSvc->regHist("/MyAnalysisAlg/Total/m_Zboson_met",m_Zboson_met).setChecked();
  m_Zboson_met_pt = new TH1D("m_Zboson_met_pt","m_Zboson_met_pt",1000,1.,1000.);
  m_thistSvc->regHist("/MyAnalysisAlg/Total/m_Zboson_met_pt",m_Zboson_met_pt).setChecked();
  m_Zboson_ljet = new TH1D("m_Zboson_ljet","m_Zboson_ljet",500,1.,500.);
  m_thistSvc->regHist("/MyAnalysisAlg/Total/m_Zboson_ljet",m_Zboson_ljet).setChecked();
  m_Zboson_ljet_pt = new TH1D("m_Zboson_ljet_pt","m_Zboson_ljet_pt",1000,1.,1000.);
  m_thistSvc->regHist("/MyAnalysisAlg/Total/m_Zboson_ljet_pt",m_Zboson_ljet_pt).setChecked();
  m_Zboson_bjet = new TH1D("m_Zboson_bjet","m_Zboson_bjet",500,1.,500.);
  m_thistSvc->regHist("/MyAnalysisAlg/Total/m_Zboson_bjet",m_Zboson_bjet).setChecked();
  m_Zboson_bjet_pt = new TH1D("m_Zboson_bjet_pt","m_Zboson_bjet_pt",1000,1.,1000.);
  m_thistSvc->regHist("/MyAnalysisAlg/Total/m_Zboson_bjet_pt",m_Zboson_bjet_pt).setChecked();

  //HT
  m_HT = new TH1D("HT", "HT", 1000, 1., 1000.);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/Zmumu/h_HT", m_HT).setChecked();
  m_HT_150GeV = new TH1D("HT_150GeV", "HT_150GeV", 1000, 1., 1000.);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/Zmumu/h_HT_150GeV", m_HT_150GeV).setChecked();

  //event
  m_h_one_Wmunu_event = new TH1D("h_one_Wmunu_event", "one_Wmunu event", 10, 1., 10.);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/Wmunu/h_one_Wmunu_event", m_h_one_Wmunu_event).setChecked();
  m_h_one_Wmunu_event_nocut = new TH1D("h_one_Wmunu_event_nocut", "one_Wmunu event_nocut", 10, 1., 10.);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/Wmunu/h_one_Wmunu_event_nocut", m_h_one_Wmunu_event_nocut).setChecked();
  m_h_one_Wmunu_event_ptcut = new TH1D("h_one_Wmunu_event_ptcut", "one_Wmunu event_ptcut", 10, 1., 10.);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/Wmunu/h_one_Wmunu_event_ptcut", m_h_one_Wmunu_event_ptcut).setChecked();
  
  m_h_one_Wmunu_event_etacut = new TH1D("h_one_Wmunu_event_etacut", "one_Wmunu event_etacut", 10, 1., 10.);
  m_thistSvc->regHist("/MyAnalysisAlg/particle/Wmunu/h_one_Wmunu_event_etacut", m_h_one_Wmunu_event_etacut).setChecked();

  //2h
  h2_Zboson_HT_pt = new TH2D("h2_Zboson_HT_pt", "Zboson pT vs HT", 1000, 1., 1000., 1000, 1., 1000.);
  m_thistSvc->regHist("/MyAnalysisAlg/h2/h2_Zboson_HT_pt", h2_Zboson_HT_pt).setChecked();
  

  all_event = new TH1D("all_event", "all_event", 10, 1., 10.);
  m_thistSvc->regHist("/MyAnalysisAlg/Total/all_event", all_event).setChecked();
  
 
  ATH_CHECK(m_thistSvc->regTree("/MyAnalysisAlg/MyTree", m_tree));
  m_tree->Print();
  ATH_CHECK(m_thistSvc->regTree("/MyAnalysisAlg/fourTree", four_tree));
  four_tree->Print();
  ATH_CHECK(m_thistSvc->regTree("/MyAnalysisAlg/ZmumuTree", Zmumu_tree));
  Zmumu_tree->Print();
  ATH_CHECK(m_thistSvc->regTree("/MyAnalysisAlg/ljetTree", ljet_tree));
  ljet_tree->Print();
  ATH_CHECK(m_thistSvc->regTree("/MyAnalysisAlg/truthljetTree", truthljet_tree));
  truthljet_tree->Print();
  ATH_CHECK(m_thistSvc->regTree("/MyAnalysisAlg/bjetTree", bjet_tree));
  bjet_tree->Print();
  ATH_CHECK(m_thistSvc->regTree("/MyAnalysisAlg/truthbjetTree", truthbjet_tree));
  truthbjet_tree->Print();
  ATH_CHECK(m_thistSvc->regTree("/MyAnalysisAlg/tauTree", tau_tree));
  tau_tree->Print();
  ATH_CHECK(m_thistSvc->regTree("/MyAnalysisAlg/ltau_htauTree", ltau_htau_tree));
  ltau_htau_tree->Print();
  ATH_CHECK(m_thistSvc->regTree("/MyAnalysisAlg/truthtauTree", truthtau_tree));
  truthtau_tree->Print();
  ATH_CHECK(m_thistSvc->regTree("/MyAnalysisAlg/photonTree", photon_tree));
  photon_tree->Print();
  ATH_CHECK(m_thistSvc->regTree("/MyAnalysisAlg/truthphotonTree", truthphoton_tree));
  truthphoton_tree->Print();
 
  
  ATH_CHECK(m_thistSvc->regTree("/MyAnalysisAlg/Twolep_ljetTree", twol_ljet_tree));
  twol_ljet_tree->Print();
  ATH_CHECK(m_thistSvc->regTree("/MyAnalysisAlg/Twolep_truthljetTree", twol_truthljet_tree));
  twol_truthljet_tree->Print();

  ATH_CHECK(m_thistSvc->regTree("/MyAnalysisAlg/twol_sjet_tree",  twol_sjet_tree));
  twol_sjet_tree->Print();
  ATH_CHECK(m_thistSvc->regTree("/MyAnalysisAlg/twol_truthsjet_tree",  twol_truthsjet_tree));
  twol_truthsjet_tree->Print();

  ATH_CHECK(m_thistSvc->regTree("/MyAnalysisAlg/four_sjetTree",  four_sjet_tree));
  four_sjet_tree->Print();
  ATH_CHECK(m_thistSvc->regTree("/MyAnalysisAlg/four_truthsjetTree",  four_truthsjet_tree));
  four_truthsjet_tree->Print();
   
  //WW
  ATH_CHECK(m_thistSvc->regTree("/MyAnalysisAlg/WlnuTree",  Wlnu_tree));
  Wlnu_tree->Print();
  ATH_CHECK(m_thistSvc->regTree("/MyAnalysisAlg/Wl_ljetTree",  Wl_ljet_tree));
  Wl_ljet_tree->Print();
  ATH_CHECK(m_thistSvc->regTree("/MyAnalysisAlg/Wl_sjetTree",  Wl_sjet_tree));
  Wl_sjet_tree->Print();

  //tt
  ATH_CHECK(m_thistSvc->regTree("/MyAnalysisAlg/tllTTree",  tll_tree));
  tll_tree->Print();
  ATH_CHECK(m_thistSvc->regTree("/MyAnalysisAlg/tWbTree",  tWb_tree));
  tWb_tree->Print();

  ATH_CHECK(m_thistSvc->regTree("/MyAnalysisAlg/tljet_bTree", tljet_b_tree));
  tljet_b_tree->Print();
  ATH_CHECK(m_thistSvc->regTree("/MyAnalysisAlg/ttwoljet_bTree", ttwoljet_b_tree));
  ttwoljet_b_tree->Print();

  ATH_CHECK(m_thistSvc->regTree("/MyAnalysisAlg/ttwoljet_onebTree", ttwoljet_oneb_tree));
  ttwoljet_oneb_tree->Print();
  ATH_CHECK(m_thistSvc->regTree("/MyAnalysisAlg/tljetWbTree", tljetWb_tree));
  tljetWb_tree->Print();
  

  //hh
  ATH_CHECK(m_thistSvc->regTree("/MyAnalysisAlg/HhhTree",  Hhh_tree));
  Hhh_tree->Print();
  

  ATH_CHECK(m_thistSvc->regTree("/MyAnalysisAlg/RatioTree", ratio_tree));
  ratio_tree->Print();

  ATH_CHECK(m_thistSvc->regTree("/MyAnalysisAlg/CheckTree", check_tree));
  check_tree->Print();

  //cutflow
  h_allevent = new TH1D("allevent","allevent",10,0,10);
  m_thistSvc->regHist("/MyAnalysisAlg/cutflow/h_allevent",h_allevent).setChecked();
  h_dibjet = new TH1D("dibjet","dibjet",10,0,10);
  m_thistSvc->regHist("/MyAnalysisAlg/cutflow/h_dibjet",h_dibjet).setChecked();
  h_dibjetcut = new TH1D("dibjet_eventcut","dibjetcut",10,0,10);
  m_thistSvc->regHist("/MyAnalysisAlg/cutflow/h_dibjetcut",h_dibjetcut).setChecked();
  h_diljet = new TH1D("diljet","diljet",10,0,10);
  m_thistSvc->regHist("/MyAnalysisAlg/cutflow/h_diljet",h_diljet).setChecked();
  h_diljetcut = new TH1D("diljetcut","diljetcut",10,0,10);
  m_thistSvc->regHist("/MyAnalysisAlg/cutflow/h_diljetcut",h_diljetcut).setChecked();
  h_twol_ljet = new TH1D("twol_ljet","twol_ljet",10,0,10);
  m_thistSvc->regHist("/MyAnalysisAlg/cutflow/h_twol_ljet",h_twol_ljet).setChecked();
  h_twol_ljetcut = new TH1D("twol_ljetcut","twol_ljetcut",10,0,10);
  m_thistSvc->regHist("/MyAnalysisAlg/cutflow/h_twol_ljetcut",h_twol_ljetcut).setChecked();
  h_twol_sjet = new TH1D("twol_sjet","twol_sjet",10,0,10);
  m_thistSvc->regHist("/MyAnalysisAlg/cutflow/h_twol_sjet",h_twol_sjet).setChecked();
  h_twol_sjetcut = new TH1D("twol_sjetcut","twol_sjetcut",10,0,10);
  m_thistSvc->regHist("/MyAnalysisAlg/cutflow/h_twol_sjetcut",h_twol_sjetcut).setChecked();
  h_twol_sjetcut = new TH1D("twol_sjetcut","twol_sjetcut",10,0,10);
  m_thistSvc->regHist("/MyAnalysisAlg/cutflow/h_twol_sjetcut",h_twol_sjetcut).setChecked();
  h_ditau = new TH1D("ditau","ditau",10,0,10);
  m_thistSvc->regHist("/MyAnalysisAlg/cutflow/h_ditau",h_ditau).setChecked();
  h_ditaucut = new TH1D("ditaucut","ditaucut",10,0,10);
  m_thistSvc->regHist("/MyAnalysisAlg/cutflow/h_ditaucut",h_ditaucut).setChecked();
  h_ttbar = new TH1D("ttbar","ttbar",10,0,10);
  m_thistSvc->regHist("/MyAnalysisAlg/cutflow/h_ttbar",h_ttbar).setChecked();

  return StatusCode::SUCCESS;
}

StatusCode MyAnalysisAlg::beginInputFile(){
    
  //SumOfweight
  const xAOD::CutBookkeeperContainer* completeCBC = 0;
  ATH_CHECK(inputMetaStore->retrieve(completeCBC, "CutBookkeepers"));
  const xAOD::CutBookkeeper* allEventsCBK = 0;
  int maxcycle=-1;
  for ( auto cbk : *completeCBC ) {
    if ( cbk->name() == "AllExecutedEvents" && cbk->inputStream() == "StreamAOD" && cbk->cycle() > maxcycle){
      maxcycle = cbk->cycle();
      allEventsCBK = cbk;
    }
  }
  sumOfWeights_tlv = allEventsCBK->sumOfEventWeights();
  sumOfWeights = sumOfWeights + sumOfWeights_tlv;
  //std::cout << std::endl << std::endl  <<"here is sumofweight :" << sumOfWeights  << std::endl << std::endl;
  
  
  
  return StatusCode::SUCCESS;
}

StatusCode MyAnalysisAlg::finalize() {
  ATH_MSG_INFO("finalize()");
  //h_ratio = (10/sumOfWeights);
  m_sumOfWeights = sumOfWeights;
  //std::cout << h_ratio << std::endl;
  h_ratio = 1.;
  int ratio_nbytes = ratio_tree->Fill();
  ATH_MSG_DEBUG("Number of Ratio bytes written = " << ratio_nbytes);

    
    
  delete m_muon_author; m_muon_author = nullptr;
  delete m_muon_phi; m_muon_phi = nullptr;
  delete m_muon_eta; m_muon_eta = nullptr;
  delete m_muon_pt; m_muon_pt = nullptr;
  delete m_muon_e; m_muon_e = nullptr;
  delete m_muon_charge; m_muon_charge = nullptr;
  //ratio
  /*  m_two_truthmuon_plus->Scale(h_ratio);
  m_two_truthmuon_plus_pt->Scale(h_ratio);
  m_two_truthelectron_plus->Scale(h_ratio);
  m_two_truthelectron_plus_pt->Scale(h_ratio);
  m_two_truthtau_plus->Scale(h_ratio);
  m_two_truthtau_plus_pt->Scale(h_ratio);
  m_four_truthelectron_plus->Scale(h_ratio);
  m_four_truthelectron_plus_pt->Scale(h_ratio);
  m_four_truthmuon_plus->Scale(h_ratio);
  m_four_truthmuon_plus_pt->Scale(h_ratio);
  
  m_4l_plus_pt->Scale(h_ratio);
  m_h_Zboson->Scale(h_ratio);
  
  m_h_truthZboson->Scale(h_ratio);
  m_h_truthZboson_pt->Scale(h_ratio);
  m_h_two_Zboson->Scale(h_ratio);
  m_h_Wboson->Scale(h_ratio);
  m_h_Wboson_pt->Scale(h_ratio);
  m_h_two_Tquark->Scale(h_ratio);
  m_h_two_Tquark_pt->Scale(h_ratio);
  m_h_two_truthTquark->Scale(h_ratio);
  m_h_two_truthTquark_pt->Scale(h_ratio);
  m_h_Higgs->Scale(h_ratio);
  m_h_Higgs_pt->Scale(h_ratio);
  m_h_truthHiggs->Scale(h_ratio);
  m_h_truthHiggs_pt->Scale(h_ratio);

  
  m_two_muon_plus->Scale(h_ratio);
  m_two_muon_plus_pt->Scale(h_ratio);;
  m_two_electron_plus->Scale(h_ratio);
  m_two_electron_plus_pt->Scale(h_ratio);
  m_two_tau_plus->Scale(h_ratio);
  m_two_tau_plus_pt->Scale(h_ratio);

  m_h_two_Zboson_pt->Scale(h_ratio);
  m_h_two_Zboson_mass->Scale(h_ratio);
  m_h_two_truthZboson_pt->Scale(h_ratio);
  m_h_two_truthZboson_mass->Scale(h_ratio);
  
  //all two
  m_h_Zboson_pt_ptcut->Scale(h_ratio);
  m_h_Zboson_mass_ptcut->Scale(h_ratio);
  m_h_Zboson_pt_masscut->Scale(h_ratio);
  m_h_Zboson_mass_masscut->Scale(h_ratio);
  
  // all four
  m_h_two_Zboson_pt_allptcut->Scale(h_ratio);
  m_h_two_Zboson_pt_muonptmasscut->Scale(h_ratio);
  m_h_two_Zboson_pt_electronptmasscut ->Scale(h_ratio);
  
 
  
  m_h_MET_pt->Scale(h_ratio);

  //background
  m_h_two_Zboson_100_electroncut->Scale(h_ratio);
  m_h_two_Zboson_75_electroncut->Scale(h_ratio);
  m_h_two_Zboson_50_electroncut->Scale(h_ratio);
  
  m_h_two_Zboson_100_allcut->Scale(h_ratio);
  m_h_two_Zboson_75_allcut->Scale(h_ratio);
  m_h_two_Zboson_50_allcut->Scale(h_ratio);

  m_h_two_Zboson_100_muoncut->Scale(h_ratio);
  m_h_two_Zboson_75_muoncut->Scale(h_ratio);
  m_h_two_Zboson_50_muoncut->Scale(h_ratio);

 

  m_Zboson_met->Scale(h_ratio);
  m_Zboson_met_pt->Scale(h_ratio);
  m_Zboson_ljet->Scale(h_ratio);
  m_Zboson_ljet_pt->Scale(h_ratio);
  m_Zboson_bjet->Scale(h_ratio);
  m_Zboson_bjet_pt->Scale(h_ratio);
  */

  //signal
  m_4l_plus->Scale(h_ratio);
  m_4l_plus_allptcut->Scale(h_ratio);
  //m_4l_plus_pt_allptcut->Scale(h_ratio);
  m_4l_plus_muonptmasscut->Scale(h_ratio);
  //m_4l_plus_pt_muonptmasscut->Scale(h_ratio);
  m_4l_plus_electronptmasscut->Scale(h_ratio);
  //m_4l_plus_pt_electronptmasscut->Scale(h_ratio);
  m_4l_plus_nocut->Scale(h_ratio);
  //m_4l_plus_pt_nocut->Scale(h_ratio);
  //m_4l_plus_eta_nocut->Scale(h_ratio);
  //m_2l_plus_eta_ptcut->Scale(h_ratio);
   //Zmumu
  m_h_Zmumu_pt->Scale(h_ratio);
  m_h_Zmumu->Scale(h_ratio);
  m_h_Zboson_pt->Scale(h_ratio);
  
  //Zee
  m_h_Zee_pt->Scale(h_ratio);
  m_h_Zee->Scale(h_ratio);

  //event
  m_h_one_Wmunu_event->Scale(h_ratio);
  m_h_one_Wmunu_event_nocut->Scale(h_ratio);
  m_h_one_Wmunu_event_ptcut->Scale(h_ratio);
  all_event->Scale(h_ratio);
  m_h_one_Wmunu_event_etacut->Scale(h_ratio);
  
  m_h_Zmumu_event_ns->Scale(h_ratio); 
  m_h_Zmumu_event_nocut->Scale(h_ratio); 
  m_h_Zmumu_event_ptcut->Scale(h_ratio);
  m_h_Zmumu_event_etacut->Scale(h_ratio);
  m_h_Zmumu_event->Scale(h_ratio);
  h2_Zboson_HT_pt->Scale(h_ratio);

  m_HT->Scale(h_ratio);
  m_HT_150GeV->Scale(h_ratio);

  return StatusCode::SUCCESS;
}

StatusCode MyAnalysisAlg::execute() {
  ATH_MSG_INFO("execute()");
//init in each event_TTree
 m_muon_author->clear();
 m_muon_phi->clear();
 m_muon_eta->clear();
 m_muon_pt->clear();
 m_muon_e->clear();
 m_muon_charge->clear();


 mcEventWeight = 0;
 m_sumOfWeights = 0;


 //2l + met
 m_Zboson_met_dphi = 0;
 m_Zboson_eta = 0;
 m_Zboson_pt = 0;
 m_Zboson_m = 0;
 m_met_pt = 0;
 Zmumu_met_mass = 0;

 //4l
 m_four_phi = 0;
 m_four_eta = 0;
 m_four_pt = 0;
 m_four_e = 0;
 m_four_m = 0;
 weight_four = 0;
 four_number = 0;

 m_ljet_dphi = 0;
 m_ljet_eta_1st = 0;
 m_ljet_pt_1st = 0;
 m_ljet_m_1st = 0;
 m_ljet_eta_2nd = 0;
 m_ljet_pt_2nd = 0;
 m_ljet_m_2nd = 0;
 diljet_mass = 0;

 weight_ljet = 0;
 ljet_number = 0;

 m_truthljet_dphi = 0;
 m_truthljet_eta_1st = 0;
 m_truthljet_pt_1st = 0;
 m_truthljet_m_1st = 0;
 m_truthljet_eta_2nd = 0;
 m_truthljet_pt_2nd = 0;
 m_truthljet_m_2nd = 0;
 ditruthljet_mass = 0;

 weight_truthljet = 0;


 m_bjet_dphi = 0;
 m_bjet_eta_1st = 0;
 m_bjet_pt_1st = 0;
 m_bjet_m_1st = 0;
 m_bjet_eta_2nd = 0;
 m_bjet_pt_2nd = 0;
 m_bjet_m_2nd = 0;

 weight_bjet = 0;
 bjet_number = 0;

 m_truthbjet_dphi = 0;
 m_truthbjet_eta_1st = 0;
 m_truthbjet_pt_1st = 0;
 m_truthbjet_m_1st = 0;
 m_truthbjet_eta_2nd = 0;
 m_truthbjet_pt_2nd = 0;
 m_truthbjet_m_2nd = 0;

 weight_truthbjet = 0;

 m_tau_dphi = 0;
 m_tau_eta_1st = 0;
 m_tau_pt_1st = 0;
 m_tau_m_1st = 0;
 m_tau_eta_2nd = 0;
 m_tau_pt_2nd = 0;
 m_tau_m_2nd = 0;
 m_tau_met_e = 0;
 
 weight_tau = 0;
 
 m_ltau_htau_dphi = 0;
 m_ltau_lepton_pt = 0;
 m_ltau_lepton_m = 0;
 m_ltau_lepton_eta = 0;
 m_htau_eta = 0;
 m_htau_pt = 0;
 m_ltau_htau_met_e = 0;
 weight_ltau_htau = 0;
 ltau_htau_mass = 0;


 m_truthtau_dphi = 0;
 m_truthtau_eta_1st = 0;
 m_truthtau_pt_1st = 0;
 m_truthtau_m_1st = 0;
 m_truthtau_eta_2nd = 0;
 m_truthtau_pt_2nd = 0;
 m_truthtau_m_2nd = 0;

 weight_truthtau = 0;

 
 m_photon_dphi = 0;
 m_photon_eta_1st = 0;
 m_photon_pt_1st = 0;
 m_photon_m_1st = 0;
 m_photon_eta_2nd = 0;
 m_photon_pt_2nd = 0;
 m_photon_m_2nd = 0;

 weight_photon = 0;

 m_truthphoton_dphi = 0;
 m_truthphoton_eta_1st = 0;
 m_truthphoton_pt_1st = 0;
 m_truthphoton_m_1st = 0;
 m_truthphoton_eta_2nd = 0;
 m_truthphoton_pt_2nd = 0;
 m_truthphoton_m_2nd = 0;

 weight_truthphoton = 0;

 //ljet + 2lepton
 m_twol_ljet_dphi = 0;
 m_twol_ljet_eta = 0;
 m_twol_ljet_pt = 0;
 m_twol_ljet_m = 0;
 m_twol_Zboson_eta = 0;
 m_twol_Zboson_pt = 0;
 m_twol_Zboson_m = 0;
 twol_ljet_mass = 0;

 weight_twol_ljet = 0;

 m_twol_truthljet_dphi = 0;
 m_twol_truthljet_eta = 0;
 m_twol_truthljet_pt = 0;
 m_twol_truthljet_m = 0;
 m_twol_Zboson_eta = 0;
 m_twol_Zboson_pt = 0;
 m_twol_Zboson_m = 0;
 twol_truthljet_mass = 0;

 weight_twol_truthljet = 0;

 m_twol_sjet_dphi = 0;
 m_twol_sjet_first_eta = 0;
 m_twol_sjet_first_pt = 0;
 m_twol_sjet_first_m = 0;
 m_twol_sjet_second_eta = 0;
 m_twol_sjet_second_pt = 0;
 m_twol_sjet_second_m = 0;
 m_twol_sjet_m = 0;
 m_twol_sjet_Zboson_eta = 0;
 m_twol_sjet_Zboson_pt = 0;
 m_twol_sjet_Zboson_m = 0;
 twol_sjet_mass = 0;

 weight_twol_sjet = 0;

 m_twol_truthsjet_dphi = 0;
 m_twol_truthsjet_first_eta = 0;
 m_twol_truthsjet_first_pt = 0;
 m_twol_truthsjet_first_m = 0;
 m_twol_truthsjet_second_eta = 0;
 m_twol_truthsjet_second_pt = 0;
 m_twol_truthsjet_second_m = 0;
 m_twol_truthsjet_m = 0;
 m_twol_truthsjet_Zboson_eta = 0;
 m_twol_truthsjet_Zboson_pt = 0;
 m_twol_truthsjet_Zboson_m = 0;
 twol_truthsjet_mass = 0;
 
 //4q
 m_four_sjet_mass = 0;
 weight_four_sjet = 0;
 
 m_four_truthsjet_mass = 0;
 weight_four_truthsjet = 0;
 m_two_truthsjet_mass_1st = 0;
 m_two_truthsjet_mass_2nd = 0;
 
 weight_twol_truthsjet = 0;
 
 //W
 m_Wlnu_eta_1st = 0;
 m_Wlnu_pt_1st = 0;
 m_Wlnu_m_1st = 0;
 m_Wlnu_eta_2nd = 0;
 m_Wlnu_pt_2nd = 0;
 m_Wlnu_m_2nd = 0;
 m_Wlnu_met_pt = 0;
 m_Wlnu_mass = 0;
 weight_Wlnu = 0;
  
 m_Wl_ljet_ljet_eta = 0;
 m_Wl_ljet_ljet_pt = 0;
 m_Wl_ljet_ljet_m = 0;
 m_Wl_ljet_l_eta = 0;
 m_Wl_ljet_l_pt = 0;
 m_Wl_ljet_l_m = 0;
 m_Wl_ljet_met_pt = 0;
 m_Wl_ljet_lnu_m = 0;
 m_Wl_ljet_mass = 0;
 weight_Wl_ljet = 0;

 m_Wl_sjet_sjet_eta_1st = 0;
 m_Wl_sjet_sjet_pt_1st = 0;
 m_Wl_sjet_sjet_m_1st = 0;
 m_Wl_sjet_sjet_eta_2nd = 0;
 m_Wl_sjet_sjet_pt_2nd = 0;
 m_Wl_sjet_sjet_m_2nd = 0;
 m_Wl_sjet_disjet_m = 0;
 m_Wl_sjet_l_eta = 0;
 m_Wl_sjet_l_pt = 0;
 m_Wl_sjet_l_m = 0;
 m_Wl_sjet_met_pt = 0;
 m_Wl_sjet_lnu_m = 0;
 m_Wl_sjet_mass = 0;
 weight_Wl_sjet = 0;
 //tt
 m_tll_dphi = 0;
 m_tll_ljet_eta_1st = 0;
 m_tll_ljet_pt_1st = 0;
 m_tll_ljet_m_1st = 0;
 m_tll_ljet_eta_2nd = 0;
 m_tll_ljet_pt_2nd = 0;
 m_tll_ljet_m_2nd = 0;
 m_tll_mass = 0;
 weight_tll = 0;

 m_tWb_l_eta_1st = 0;
 m_tWb_l_pt_1st = 0;
 m_tWb_l_m_1st = 0;
 m_tWb_l_eta_2nd = 0;
 m_tWb_l_pt_2nd = 0;
 m_tWb_l_m_2nd = 0;
 m_tWb_met_pt = 0;
 m_tWb_b_eta_1st = 0;
 m_tWb_b_pt_1st = 0;
 m_tWb_b_m_1st = 0;
 m_tWb_b_eta_2nd = 0;
 m_tWb_b_pt_2nd = 0;
 m_tWb_b_m_2nd = 0;
 m_tWb_mass = 0;
 weight_tWb = 0;




 m_Hhh_dphi = 0;
 m_Hhh_b_eta_1st = 0;
 m_Hhh_b_pt_1st = 0;
 m_Hhh_b_m_1st = 0;
 m_Hhh_b_eta_2nd = 0;
 m_Hhh_b_pt_2nd = 0;
 m_Hhh_b_m_2nd = 0;
 m_Hhh_b_eta_3rd = 0;
 m_Hhh_b_pt_3rd = 0;
 m_Hhh_b_m_3rd = 0;
 m_Hhh_b_eta_4th = 0;
 m_Hhh_b_pt_4th = 0;
 m_Hhh_b_m_4th = 0;
 m_Hhh_h_m_1st = 0;
 m_Hhh_h_m_2nd = 0;
 m_Hhh_mass = 0;
 weight_Hhh = 0;

 m_tljet_b_l_eta_1st = 0;
 m_tljet_b_l_pt_1st = 0;
 m_tljet_b_l_m_1st = 0;
 m_tljet_b_met_pt = 0;
 m_tljet_b_lnu_m = 0;
 m_tljet_b_ljet_eta = 0;
 m_tljet_b_ljet_pt = 0;
 m_tljet_b_ljet_m = 0;
 m_tljet_b_b_eta_1st = 0;
 m_tljet_b_b_pt_1st = 0;
 m_tljet_b_b_m_1st = 0;
 m_tljet_b_b_eta_2nd = 0;
 m_tljet_b_b_pt_2nd = 0;
 m_tljet_b_b_m_2nd = 0;
 m_tljet_b_mass = 0;
 weight_tljet_b = 0;

 m_ttwoljet_b_ljet_eta_1st = 0;
 m_ttwoljet_b_ljet_pt_1st = 0;
 m_ttwoljet_b_ljet_m_1st = 0;
 m_ttwoljet_b_ljet_eta_2nd = 0;
 m_ttwoljet_b_ljet_pt_2nd = 0;
 m_ttwoljet_b_ljet_m_2nd = 0;
 m_ttwoljet_b_b_eta_1st = 0;
 m_ttwoljet_b_b_pt_1st = 0;
 m_ttwoljet_b_b_m_1st = 0;
 m_ttwoljet_b_b_eta_2nd = 0;
 m_ttwoljet_b_b_pt_2nd = 0;
 m_ttwoljet_b_b_m_2nd = 0;
 m_ttwoljet_b_mass = 0;
 weight_ttwoljet_b = 0;
 
 m_ttwoljet_oneb_tljet_eta = 0;
 m_ttwoljet_oneb_tljet_pt = 0;
 m_ttwoljet_oneb_tljet_m = 0;
 m_ttwoljet_oneb_Wljet_eta = 0;
 m_ttwoljet_oneb_Wljet_pt = 0;
 m_ttwoljet_oneb_Wljet_m = 0;
 m_ttwoljet_oneb_b_eta = 0;
 m_ttwoljet_oneb_b_pt = 0;
 m_ttwoljet_oneb_b_m = 0;
 m_ttwoljet_oneb_mass = 0;
 weight_ttwoljet_oneb = 0;

 m_tljetWb_ljet_eta = 0;
 m_tljetWb_ljet_pt = 0;
 m_tljetWb_ljet_m = 0;
 m_tljetWb_l_eta = 0;
 m_tljetWb_l_pt = 0;
 m_tljetWb_l_m = 0;
 m_tljetWb_met_pt = 0;
 m_tljetWb_b_eta = 0;
 m_tljetWb_b_pt = 0;
 m_tljetWb_b_m = 0;
 m_tljetWb_mass = 0;
 weight_tljetWb = 0;

 
 //check
 check_tau_mass = 0;

 // Event information
 const xAOD::EventInfo* eventInfo = nullptr;
 ATH_CHECK(evtStore()->retrieve(eventInfo));
 uint32_t runNumber = eventInfo->runNumber();
 uint32_t lumiBlock = eventInfo->lumiBlock();
 unsigned long long eventNumber = eventInfo->eventNumber();
 ATH_MSG_INFO("Run = " << runNumber << " : LB " << lumiBlock << " : Event = " << eventNumber);

 //save the runNumber,LB and event number in new tree
 m_runNumber = runNumber;
 m_lumiBlock = lumiBlock;
 m_eventNumber = eventNumber;


 mcEventWeight = eventInfo->mcEventWeights().at(0);
 //mcEventWeight = 1;
 //std::cout << crosssection << std::endl;

 h_allevent->Fill(1,mcEventWeight);

 // Muon
 const xAOD::MuonContainer* muonContainer = nullptr;
 ATH_CHECK(evtStore()->retrieve(muonContainer, "Muons"));
 ATH_MSG_DEBUG("Muons successfully retrieved");
 ATH_MSG_DEBUG("# of muons = " << muonContainer->size());
 unsigned int nMuons = 0;
 
 std::vector<TLorentzVector> muons;  //important
 std::vector<TLorentzVector> muons_ns;
 for (const xAOD::Muon* muon : *muonContainer) {
   TLorentzVector muon_ns_tlv;
   muon_ns_tlv.SetPtEtaPhiM(muon->pt()/1000, muon->eta(), muon->phi(), muon->m()/1000.);
   muons_ns.push_back(muon_ns_tlv);
   if (not m_selTool->accept(*muon)) {
     ATH_MSG_INFO("The muon is rejected");
     continue;
   }
   ATH_MSG_INFO(nMuons << "th muon :" <<
        " Pt : " << muon->pt() <<
        " Eta : " << muon->eta() <<
        " Phi : " << muon->phi() <<
        " E : " << muon->e() <<
        " M : " << muon->m());
   m_h_muon_e->Fill(muon->e()/1000.); // Energy in GeV
   m_h_muon_pt->Fill(muon->pt()/1000.); // Transverse momentum in GeV
   m_h_muon_eta->Fill(muon->eta()); // Pseudorapidity
   m_h_muon_phi->Fill(muon->phi()); // Azimuthal angle
   m_h2_muon_eta_phi->Fill(muon->eta(), muon->phi()); // 2D distribution, X:eta, Y:phi
     
   ++nMuons;

   TLorentzVector muon_tlv;
   muon_tlv.SetPtEtaPhiM(muon->pt()/1000, muon->eta(), muon->phi(), muon->m()/1000.);
   muons.push_back(muon_tlv);
   
   //muon_TTree
   m_muon_author->push_back(muon->author());
   m_muon_phi->push_back(muon->phi());
   m_muon_eta->push_back(muon->eta());
   m_muon_pt->push_back(muon->pt()/1000.);
   m_muon_e->push_back(muon->e()/1000.);
   if (muon->primaryTrackParticle()) {
     m_muon_charge->push_back(muon->primaryTrackParticle()->charge());
   } else {
     m_muon_charge->push_back(0.);
   }

 }

 

 m_h_nMuons->Fill(nMuons); // The number of muons

  //muon TTree
 // m_nMuons = static_cast<int>(nMuons);
 //int nbytes = m_tree->Fill();
 //ATH_MSG_DEBUG("Number of bytes written = " << nbytes);

 //fill two muon mass
 /* for (size_t i=0; i<muons.size(); i++){
   for (size_t j=i+1; j<muons.size(); j++){
     auto m1 = muons.at(i);
     auto m2 = muons.at(j);
     if(((m1 + m2).M()<121) && ((m1 + m2).M()>61)){
   m_h_Zboson_mass_masscut->Fill((m1 + m2).M(),mcEventWeight);
   m_h_Zboson_pt_masscut->Fill((m1 + m2).Pt(),mcEventWeight);
     }
     if((m1.Pt() > 10 )&& (m2.Pt() > 10)){
   m_h_Zboson_mass_ptcut->Fill((m1 + m2).M(),mcEventWeight);
   m_h_Zboson_pt_ptcut->Fill((m1 + m2).Pt(),mcEventWeight);
   m_2l_plus_eta_ptcut->Fill((m1 + m2).Eta(),mcEventWeight);
   if(((m1 + m2).M()<121) && ((m1 + m2).M()>61)){
     m_two_muon_plus->Fill((m1 + m2).M(),mcEventWeight);
     m_two_muon_plus_pt->Fill((m1 + m2).Pt(),mcEventWeight);
   }
     }
   }
 }
 for (size_t i=0; i<muons_ns.size(); i++){
   for (size_t j=i+1; j<muons_ns.size(); j++){
     auto m1 = muons_ns.at(i);
     auto m2 = muons_ns.at(j);
     m_two_muon_ns_plus->Fill((m1 + m2).M());
   }
 }



 for (size_t i=0; i<muons_ns.size(); i++){
   for (size_t j=i+1; j<muons_ns.size(); j++){
     for(size_t k=j+1; k<muons_ns.size(); k++){
   for(size_t l=k+1; l<muons_ns.size(); l++){
     auto m1 = muons_ns.at(i);
     auto m2 = muons_ns.at(j);
     auto m3 = muons_ns.at(k);
     auto m4 = muons_ns.at(l);
     m_four_muon_ns_plus->Fill((m1 + m2 + m3 + m4).M());
   }
     }
   }
   }*/
  

 //Electron
 const xAOD::ElectronContainer* electronContainer = nullptr;
 ATH_CHECK(evtStore()->retrieve(electronContainer, "Electrons"));
 unsigned int nElectrons = 0;
 ATH_MSG_DEBUG("Electrons successfully retrieved");
 ATH_MSG_DEBUG("# of electrons = " << electronContainer->size());

 std::vector<TLorentzVector> electrons;  //important
 std::vector<TLorentzVector> electrons_ns;
 for (const xAOD::Electron* eg : *electronContainer) {
   TLorentzVector electron_ns_tlv;
   electron_ns_tlv.SetPtEtaPhiM(eg->pt()/1000, eg->eta(), eg->phi(), eg->m()/1000.);
   electrons_ns.push_back(electron_ns_tlv);
   if (m_acc_eleId.isAvailable(*eg)){
     if(not m_acc_eleId(*eg)){
   ATH_MSG_INFO("The Electron is rejected");
   continue;
     }
   }
   ATH_MSG_INFO(nElectrons << " th electron :" <<
        " Pt : " << eg->pt() <<
        " Eta : " << eg->eta() <<
        " Phi : " << eg->phi() <<
        " E : " << eg->e() <<
        " M : " << eg->m()
        );
   m_h_electron_e->Fill(eg->e()/1000.); // Energy in GeV
   m_h_electron_pt->Fill(eg->pt()/1000.); // Transverse momentum in GeV
   m_h_electron_eta->Fill(eg->eta()); // Pseudorapidity
   m_h_electron_phi->Fill(eg->phi()); // Azimuthal angle
   m_h2_electron_eta_phi->Fill(eg->eta(), eg->phi()); // 2D distribution, X:eta, Y:phi

   ++nElectrons;

   TLorentzVector electron_tlv;
   electron_tlv.SetPtEtaPhiM(eg->pt()/1000, eg->eta(), eg->phi(), eg->m()/1000.);
   electrons.push_back(electron_tlv);
 }
 
 m_h_nElectrons->Fill(nElectrons);

 //tau
 const xAOD::TauJetContainer* taujetContainer = nullptr;
 ATH_CHECK(evtStore()->retrieve(taujetContainer, "TauJets"));
 ATH_MSG_DEBUG("Taus successfully retrieved");
 ATH_MSG_DEBUG("# of taus = " << taujetContainer->size());
 unsigned int nTaus = 0;
 std::vector<TLorentzVector> taus;  //important
 std::vector<TLorentzVector> taus_ns;
 for (const xAOD::TauJet* tu : *taujetContainer) {
   if (not t_selTool->accept(*tu)) {
     ATH_MSG_INFO("The tau is rejected");
     continue;
   }
   ATH_MSG_INFO(nTaus << " th tau :" <<
        " Pt : " << tu->pt() <<
        " Eta : " << tu->eta() <<
        " Phi : " << tu->phi() <<
        " E : " << tu->e() <<
        " M : " << tu->m()
        );
   m_h_tau_e->Fill(tu->e()/1000.); // Energy in GeV
   m_h_tau_pt->Fill(tu->pt()/1000.); // Transverse momentum in GeV
   m_h_tau_eta->Fill(tu->eta()); // Pseudorapidity
   m_h_tau_phi->Fill(tu->phi()); // Azimuthal angle
   m_h2_tau_eta_phi->Fill(tu->eta(), tu->phi()); // 2D distribution, X:eta, Y:phi

   ++nTaus;

   TLorentzVector tau_tlv;
   tau_tlv.SetPtEtaPhiM(tu->pt()/1000, tu->eta(), tu->phi(), tu->m()/1000.);
   taus.push_back(tau_tlv);
   
   check_tau_mass = tau_tlv.M();
   int check_nbytes = check_tree->Fill();
   ATH_MSG_DEBUG("Number of Check bytes written = " << check_nbytes);

 }
 m_h_nTaus->Fill(nTaus);
 for (size_t i=0; i<taus.size(); i++){
   for (size_t j=i+1; j<taus.size(); j++){
     auto t1 = taus.at(i);
     auto t2 = taus.at(j);
     m_two_tau_plus->Fill((t1 + t2).M(),mcEventWeight);
   }
 }

 //fill two electron mass
 for (size_t i=0; i<electrons.size(); i++){
   for (size_t j=i+1; j<electrons.size(); j++){
     auto e1 = electrons.at(i);
     auto e2 = electrons.at(j);
     if(((e1 + e2).M()<121) && ((e1 + e2).M()>61)){
   m_h_Zboson_mass_masscut->Fill((e1 + e2).M(),mcEventWeight);
   m_h_Zboson_pt_masscut->Fill((e1 + e2).Pt(),mcEventWeight);
     }
     if((e1.Pt() > 10 )&& (e2.Pt() > 10)){
   m_h_Zboson_mass_ptcut->Fill((e1 + e2).M(),mcEventWeight);
   m_h_Zboson_pt_ptcut->Fill((e1 + e2).Pt(),mcEventWeight);
   m_2l_plus_eta_ptcut->Fill((e1 + e2).Eta(),mcEventWeight);
   if(((e1 + e2).M()<121) && ((e1 + e2).M()>61)){
     m_two_electron_plus->Fill((e1 + e2).M(),mcEventWeight);
     m_two_electron_plus_pt->Fill((e1 + e2).Pt(),mcEventWeight);
   }
     }
   }
 }
 for (size_t i=0; i<electrons_ns.size(); i++){
   for (size_t j=i+1; j<electrons_ns.size(); j++){
     auto e1 = electrons_ns.at(i);
     auto e2 = electrons_ns.at(j);
     m_two_electron_ns_plus->Fill((e1 + e2).M());
   }
 }

 //photon container
 const xAOD::PhotonContainer* photonContainer = nullptr;
 ATH_CHECK(evtStore()->retrieve(photonContainer, "Photons"));
 unsigned int nPhotons = 0;
 ATH_MSG_DEBUG("Photons successfully retrieved");
 ATH_MSG_DEBUG("# of photons = " << photonContainer->size());

 std::vector<TLorentzVector> photons;  //important
 std::vector<TLorentzVector> photons_ns;
 for (const xAOD::Photon* ph : *photonContainer) {
   TLorentzVector photon_ns_tlv;
   photon_ns_tlv.SetPtEtaPhiM(ph->pt()/1000, ph->eta(), ph->phi(), ph->m()/1000.);
   photons_ns.push_back(photon_ns_tlv);
   if(not tight_acc(*ph)){
    ATH_MSG_INFO("The Photon is rejected");
   continue;
   }
   ATH_MSG_INFO(nPhotons << " th photon :" <<
        " Pt : " << ph->pt() <<
        " Eta : " << ph->eta() <<
        " Phi : " << ph->phi() <<
        " E : " << ph->e() <<
        " M : " << ph->m()
        );
   m_h_photon_e->Fill(ph->e()/1000.); // Energy in GeV
   m_h_photon_pt->Fill(ph->pt()/1000.); // Transverse momentum in GeV
   m_h_photon_eta->Fill(ph->eta()); // Pseudorapidity
   m_h_photon_phi->Fill(ph->phi()); // Azimuthal angle
   m_h2_photon_eta_phi->Fill(ph->eta(), ph->phi()); // 2D distribution, X:eta, Y:phi

   ++nPhotons;

   TLorentzVector photon_tlv;
   photon_tlv.SetPtEtaPhiE(ph->pt()/1000, ph->eta(), ph->phi(), ph->e()/1000.);
   photons.push_back(photon_tlv);
 }
 
 m_h_nPhotons->Fill(nPhotons);
 //fill two photon mass
 for (size_t i=0; i<photons.size(); i++){
   for (size_t j=i+1; j<photons.size(); j++){
     auto p1 = photons.at(i);
     auto p2 = photons.at(j);
     m_two_photon_plus->Fill((p1 + p2).M());
   }
   }
//// L-jet
 const xAOD::JetContainer* ljetContainer = nullptr;
 ATH_CHECK(evtStore()->retrieve(ljetContainer, "AntiKt10LCTopoTrimmedPtFrac5SmallR20Jets")); //AntiKt4EMPFlowJets, sjet  //AntiKt10LCTopoTrimmedPtFrac5SmallR20Jets, l-jet
 ATH_MSG_DEBUG("L-jets successfully retrieved");
 ATH_MSG_DEBUG("# of L-jets = " << ljetContainer->size());
 std::vector<TLorentzVector> ljets;  //important
 unsigned int nlJets = 0;
 for (const xAOD::Jet* ljt : *ljetContainer) {
   ATH_MSG_INFO(nlJets << " th jet :" <<
        " Pt : " << ljt->pt() <<
        " Eta : " << ljt->eta() <<
        " Phi : " << ljt->phi() <<
        " E : " << ljt->e() <<
        " M : " << ljt->m()
        );
   m_h_jet_e->Fill(ljt->e()/1000.); // Energy in GeV
   m_h_jet_pt->Fill(ljt->pt()/1000.); // Transverse momentum in GeV
   m_h_jet_eta->Fill(ljt->eta()); // Pseudorapidity
   m_h_jet_phi->Fill(ljt->phi()); // Azimuthal angle
   m_h2_jet_eta_phi->Fill(ljt->eta(), ljt->phi()); // 2D distribution, X:eta, Y:phi

   ++nlJets;

   TLorentzVector ljet_tlv;
   ljet_tlv.SetPtEtaPhiM(ljt->pt()/1000, ljt->eta(), ljt->phi(), ljt->m()/1000.);
   ljets.push_back(ljet_tlv);
 }

 //Small-R jet
 const xAOD::JetContainer* sjetContainer = nullptr;
 // ATH_CHECK(evtStore()->retrieve(sjetContainer, "AntiKt4EMPFlowJets")); //AntiKt4EMPFlowJets, sjet  //AntiKt10LCTopoTrimmedPtFrac5SmallR20Jets, l-jet
 ATH_CHECK(evtStore()->retrieve(sjetContainer, "AntiKt4EMPFlowJets_BTagging201903"));
 ATH_MSG_DEBUG("jets successfully retrieved");
 ATH_MSG_DEBUG("# of jets = " << sjetContainer->size());
 std::vector<TLorentzVector> sjets;
 unsigned int nsJets = 0;
 for (const xAOD::Jet* sjt : *sjetContainer) {
   TLorentzVector sjet_tlv;
   sjet_tlv.SetPtEtaPhiM(sjt->pt()/1000, sjt->eta(), sjt->phi(), sjt->m()/1000.);
   sjets.push_back(sjet_tlv);
   //jtv
   //float newjvt = h_updatejvt->updateJvt(*sjt); 
   ATH_MSG_INFO(nsJets << " th jet :" <<
        " Pt : " << sjt->pt() <<
        " Eta : " << sjt->eta() <<
        " Phi : " << sjt->phi() <<
        " E : " << sjt->e() <<
        " M : " << sjt->m()
        );
   m_h_jet_e->Fill(sjt->e()/1000.); // Energy in GeV
   m_h_jet_pt->Fill(sjt->pt()/1000.); // Transverse momentum in GeV
   m_h_jet_eta->Fill(sjt->eta()); // Pseudorapidity
   m_h_jet_phi->Fill(sjt->phi()); // Azimuthal angle
   m_h2_jet_eta_phi->Fill(sjt->eta(), sjt->phi()); // 2D distribution, X:eta, Y:phi

   ++nsJets;
 }
 
 

 ////B-jet
 const xAOD::JetContainer* jetContainer = nullptr;
 ATH_CHECK(evtStore()->retrieve(jetContainer, "AntiKt4EMPFlowJets_BTagging201903")); //AntiKt4EMPFlowJets, sjet  //AntiKt10LCTopoTrimmedPtFrac5SmallR20Jets, l-jet
 ATH_MSG_DEBUG("jets successfully retrieved");
 ATH_MSG_DEBUG("# of jets = " << jetContainer->size());
 std::vector<TLorentzVector> bjets;  //important
 unsigned int nJets = 0;
 for (const xAOD::Jet* jt : *jetContainer) {
   if(not btag_selTool->accept(*jt)) {
     ATH_MSG_INFO("The bjet is rejected");
     continue;
   }
   ATH_MSG_INFO(nJets << " th jet :" <<
        " Pt : " << jt->pt() <<
        " Eta : " << jt->eta() <<
        " Phi : " << jt->phi() <<
        " E : " << jt->e() <<
        " M : " << jt->m()
        );
   m_h_jet_e->Fill(jt->e()/1000.); // Energy in GeV
   m_h_jet_pt->Fill(jt->pt()/1000.); // Transverse momentum in GeV
   m_h_jet_eta->Fill(jt->eta()); // Pseudorapidity
   m_h_jet_phi->Fill(jt->phi()); // Azimuthal angle
   m_h2_jet_eta_phi->Fill(jt->eta(), jt->phi()); // 2D distribution, X:eta, Y:phi

   ++nJets;

   TLorentzVector bjet_tlv;
   bjet_tlv.SetPtEtaPhiM(jt->pt()/1000, jt->eta(), jt->phi(), jt->m()/1000.);
   bjets.push_back(bjet_tlv);
 }
 
 m_h_nJets->Fill(nJets);
 
 for (size_t i=0; i<bjets.size(); i++){
   auto j1 = bjets.at(i);
   m_jet_mass->Fill(j1.M());
 }

 //MissingET
 const xAOD::MissingETContainer* METContainer = nullptr;
 ATH_CHECK(evtStore()->retrieve(METContainer, "MET_Reference_AntiKt4EMTopo"));
 ATH_MSG_DEBUG("METs successfully retrieved");
 ATH_MSG_DEBUG("# of METs = " << METContainer->size());
 std::vector<TLorentzVector> mets;  //important
 unsigned int nMETs = 0;

 for (const xAOD::MissingET* met : *METContainer) {
   if(met->name() != "FinalTrk"){
     ATH_MSG_INFO("Not FinalTrk condidate");
     continue;
   }
   auto px = met->mpx()/1000.;
   auto py = met->mpy()/1000.;
   auto E = met->met()/1000.;
   auto pz = 0;
   ATH_MSG_INFO(nMETs << " th MET :" <<
        " E : " << met->met() <<
        " Px : " << met->mpx() <<
        " Py : " << met->mpy() <<
        " Phi : " << met->phi()
        );
   ++nMETs;
   TLorentzVector met_tlv;
   met_tlv.SetPxPyPzE(px, py, pz, E);
   mets.push_back(met_tlv);
   m_h_MET_pt->Fill(met_tlv.E(),mcEventWeight);
 }
 m_h_nMETs->Fill(nMETs);



 

 ////////////higgs
 //Zboson
 std::vector<TLorentzVector> Zbosons;
 std::vector<TLorentzVector> Zbosons_nocut;
 std::vector<TLorentzVector> Zbosons_ptcut;
 std::vector<TLorentzVector> Zbosons_muoncut;
 std::vector<TLorentzVector> Zbosons_electroncut;
 std::vector<TLorentzVector> Zbosons_etacut;
 /*for (size_t i=0; i<electrons.size(); i++){
   for (size_t j=i+1; j<electrons.size(); j++){
     auto e1 = electrons.at(i);
     auto e2 = electrons.at(j);
     
     TLorentzVector Zboson_tlv_nocut;
     Zboson_tlv_nocut = e1 + e2;
     Zbosons_nocut.push_back(Zboson_tlv_nocut);
     Zbosons_muoncut.push_back(Zboson_tlv_nocut);
     
     if(e1.Pt() < 20) continue;
     if(e2.Pt() < 20) continue;
     TLorentzVector Zboson_tlv_ptcut;
     Zboson_tlv_ptcut = e1 + e2;
     Zbosons_ptcut.push_back(Zboson_tlv_ptcut);
     
     if((e1+e2).M() > 121) continue;
     if((e1+e2).M() < 61) continue;
     TLorentzVector Zboson_tlv;
     Zboson_tlv = e1 + e2;
     Zbosons.push_back(Zboson_tlv);
     Zbosons_electroncut.push_back(Zboson_tlv);
     
     m_h_Zboson_pt->Fill((e1 + e2).Pt(),mcEventWeight);
     m_h_Zboson->Fill((e1 + e2).M(),mcEventWeight);
     m_h_Zee_pt->Fill((e1 + e2).Pt(),mcEventWeight);
     m_h_Zee->Fill((e1 + e2).M(),mcEventWeight);
   }
 }
 */ 
 ///////
 for (size_t i=0; i<muons.size(); i++){
   for (size_t j=i+1; j<muons.size(); j++){
     auto m1 = muons.at(i);
     auto m2 = muons.at(j);
     TLorentzVector Zboson_tlv_nocut;
     Zboson_tlv_nocut = m1 + m2;
     Zbosons_nocut.push_back(Zboson_tlv_nocut);
     Zbosons_electroncut.push_back(Zboson_tlv_nocut);
     
     if(m1.Pt() < 20) continue;
     if(m2.Pt() < 20) continue;
     TLorentzVector Zboson_tlv_ptcut;
     Zboson_tlv_ptcut = m1 + m2;
     Zbosons_ptcut.push_back(Zboson_tlv_ptcut);

     if(std::abs(m1.Eta()) > 2.5) continue;
     if(std::abs(m2.Eta()) > 2.5) continue;
     TLorentzVector Zboson_tlv_etacut;
     Zboson_tlv_etacut = m1 + m2;
     Zbosons_etacut.push_back(Zboson_tlv_etacut);

     if((m1+m2).M() > 121) continue;
     if((m1+m2).M() < 61) continue;
     //if((m1+m2).Pt() < 100) continue;
     TLorentzVector Zboson_tlv;
     Zboson_tlv = m1 + m2;
     Zbosons.push_back(Zboson_tlv);
     Zbosons_muoncut.push_back(Zboson_tlv);

   }
 }

 std::vector<TLorentzVector> Zbosons_ns;
  for (size_t i=0; i<muons_ns.size(); i++){
   for (size_t j=i+1; j<muons_ns.size(); j++){
     auto m1 = muons_ns.at(i);
     auto m2 = muons_ns.at(j);
     TLorentzVector Zboson_tlv_ns;
     Zbosons_ns.push_back(Zboson_tlv_ns);
   }
 }
 

  //h2
  //h2_Zboson_HT_pt
  double Z_pt = 0;
  if(Zbosons.size()>0){
    double sjet_HT = 0;
    for(size_t i=0; i<sjets.size(); i++){
      auto s1 = sjets.at(i);
      if(s1.Pt()<50) continue;
      double sjet_HT_tlv;
      sjet_HT_tlv = std::sqrt(s1.Px()*s1.Px() +s1.Py()*s1.Py());
      sjet_HT = sjet_HT + sjet_HT_tlv;
    }
    for(size_t i=0; i<Zbosons.size(); i++){
      auto z1 = Zbosons.at(i);
      double Z_pt_tlv = 0;
      Z_pt_tlv = z1.Pt();
      if (Z_pt_tlv > Z_pt){
	Z_pt = Z_pt_tlv;
      }
    }
    h2_Zboson_HT_pt->Fill(Z_pt,sjet_HT,mcEventWeight);
  }

  //HT
  double HT = 0;
  for(size_t i=0; i<sjets.size(); i++){
    auto s1 = sjets.at(i);
    if(s1.Pt()<20) continue;
    if(std::abs(s1.Eta())>4.9) continue;
    double HT_tlv;
    HT_tlv = std::sqrt(s1.Px()*s1.Px() +s1.Py()*s1.Py());
    HT = HT + HT_tlv;
  }
  m_HT->Fill(HT,mcEventWeight);
  if(HT>150){
    m_HT_150GeV->Fill(HT,mcEventWeight);
  }
  
  for(size_t i=0; i<Zbosons.size(); i++){
    if(HT>200){
      auto z1 = Zbosons.at(i);
      m_h_Zmumu_pt->Fill((z1).Pt(),mcEventWeight);
    }
  }


 int event = 1;
 
 all_event->Fill(event,mcEventWeight);
 /*if((electrons.size()>0) || (muons.size()>0)){
   m_h_one_Wmunu_event_nocut->Fill(event,mcEventWeight);
 }
 
 std::vector<TLorentzVector> mu_10GeV;
 std::vector<TLorentzVector> e_10GeV;
 for(size_t i=0; i<muons.size(); i++){
   auto m1 = muons.at(i);
   if (m1.Pt() < 10) continue;
   mu_10GeV.push_back(m1);
 }
 for(size_t i=0; i<electrons.size(); i++){
   auto e1 = electrons.at(i);
   if (e1.Pt() < 10) continue;
   e_10GeV.push_back(e1);
 }
 if((mu_10GeV.size()>0) || (e_10GeV.size()>0)){
   m_h_one_Wmunu_event_ptcut->Fill(event,mcEventWeight);
 }

 std::vector<TLorentzVector> mu_20GeV;
 std::vector<TLorentzVector> e_20GeV;
 for(size_t i=0; i<muons.size(); i++){
   auto m1 = muons.at(i);
   if (m1.Pt() < 20) continue;
   mu_20GeV.push_back(m1);
 }
 for(size_t i=0; i<electrons.size(); i++){
   auto e1 = electrons.at(i);
   if (e1.Pt() < 20) continue;
   e_20GeV.push_back(e1);
 }
 
 if((mu_20GeV.size()>0) || (e_20GeV.size()>0)){
   m_h_one_Wmunu_event->Fill(event,mcEventWeight);
 }

 std::vector<TLorentzVector> mu_eta;
 std::vector<TLorentzVector> e_eta;
 for(size_t i=0; i<muons.size(); i++){
   auto m1 = muons.at(i);
   if (m1.Pt() < 20) continue;
   if(std::abs(m1.Eta()) > 2.5) continue;
   mu_eta.push_back(m1);
 }
 for(size_t i=0; i<electrons.size(); i++){
   auto e1 = electrons.at(i);
   if (e1.Pt() < 20) continue;
   if(std::abs(e1.Eta()) > 2.47) continue;
   if((e1.Eta() > 1.37) && (e1.Eta() < 1.52)) continue;
   mu_eta.push_back(e1);
 }

 if((mu_eta.size()>0) || (e_eta.size()>0)){
   m_h_one_Wmunu_event_etacut->Fill(event,mcEventWeight);
 }
 */

 
 /////
 if(Zbosons_ns.size()>0){
   m_h_Zmumu_event_ns->Fill(event,mcEventWeight);
 }

 if(Zbosons_nocut.size()>0){
   m_h_Zmumu_event_nocut->Fill(event,mcEventWeight);
 }

 if(Zbosons_ptcut.size()>0){
   m_h_Zmumu_event_ptcut->Fill(event,mcEventWeight);
 }

 if(Zbosons_etacut.size()>0){
   m_h_Zmumu_event_etacut->Fill(event,mcEventWeight);
 }

  if(Zbosons.size()>0){
    m_h_Zmumu_event->Fill(event,mcEventWeight);
 }

  
  /*
  for (size_t i=0; i<taus.size(); i++){
   for (size_t j=i+1; j<taus.size(); j++){
     auto t1 = taus.at(i);
     auto t2 = taus.at(j);
     if((t1.Pt() > 20 )&& (t2.Pt() > 20)){
   if(((t1 + t2).M()<121) && ((t1 + t2).M()>61)){
     // m_h_Zboson_pt->Fill((t1 + t2).Pt(),mcEventWeight);
     //m_h_Zboson->Fill((t1 + t2).M(),mcEventWeight); //wrong onexs
     m_two_tau_plus_pt->Fill((t1 + t2).Pt(),mcEventWeight);
     TLorentzVector Zboson_tlv;
     Zboson_tlv = t1 + t2;
     // Zbosons.push_back(Zboson_tlv);
   }
     }
   }
 }
  for (size_t i=0; i<mets.size(); i++){
   for (size_t j=i+1; j<mets.size(); j++){
     auto n1 = mets.at(i);
     auto n2 = mets.at(j);
     //m_h_Zboson->Fill((n1 + n2).M());
     // m_h_Zboson_pt->Fill((n1 + n2).pt(),mcEventWeight);
     TLorentzVector Zboson_tlv;
     Zboson_tlv = n1 + n2;
     //Zbosons.push_back(Zboson_tlv);
   }
  }
  //Wboson
  std::vector<TLorentzVector> Wbosons;
  for (size_t i=0; i<mets.size(); i++){
    for (size_t j=0; j<electrons.size(); j++){
      auto n1 = mets.at(i);
      auto e1 = electrons.at(j);
      if((n1.Pt() > 20 )&& (e1.Pt() > 10)){
    m_h_Wboson->Fill((n1 + e1).M());
    m_h_Wboson_pt->Fill((n1 + e1).Pt());
    TLorentzVector Wboson_tlv;
    Wboson_tlv = n1 + e1;
    Wbosons.push_back(Wboson_tlv);
      }
    }
  }
  for (size_t i=0; i<mets.size(); i++){
    for (size_t j=0; j<muons.size(); j++){
      auto n1 = mets.at(i);
      auto m1 = muons.at(j);
      if((n1.Pt() > 20 )&& (m1.Pt() > 10)){
    m_h_Wboson->Fill((n1 + m1).M());
    m_h_Wboson_pt->Fill((n1 + m1).Pt());
    TLorentzVector Wboson_tlv;
    Wboson_tlv = n1 + m1;
    Wbosons.push_back(Wboson_tlv);
      }
    }
  }
  for (size_t i=0; i<mets.size(); i++){
    for (size_t j=0; j<taus.size(); j++){
      auto n1 = mets.at(i);
      auto t1 = taus.at(j);
      if((n1.Pt() > 20 )&& (t1.Pt() > 20)){
    m_h_Wboson->Fill((n1 + t1).M());
    m_h_Wboson_pt->Fill((n1 + t1).Pt());
    TLorentzVector Wboson_tlv;
    Wboson_tlv = n1 + t1;
    Wbosons.push_back(Wboson_tlv);
      }
    }
  }
  //tquark
  std::vector<TLorentzVector> tquarks;
  for (size_t i=0; i<Wbosons.size(); i++){
    for (size_t j=0; j<bjets.size(); j++){
      auto w1 = Wbosons.at(i);
      auto b1 = bjets.at(j);
      m_h_Tquark->Fill((w1 + b1).M());
      TLorentzVector tquark_tlv;
      tquark_tlv = w1 + b1;
      tquarks.push_back(tquark_tlv);
    }
    }*/
  //higgs from Zboson
  for (size_t i=0; i<Zbosons.size(); i++){
   for (size_t j=i+1; j<Zbosons.size(); j++){
     auto z1 = Zbosons.at(i);
     auto z2 = Zbosons.at(j);
     m_4l_plus->Fill((z1 + z2).M(),mcEventWeight);
   }
 }
  for (size_t i=0; i<Zbosons_nocut.size(); i++){
    for (size_t j=i+1; j<Zbosons_nocut.size(); j++){
      auto z1 = Zbosons_nocut.at(i);
      auto z2 = Zbosons_nocut.at(j);
      m_4l_plus_nocut->Fill((z1 + z2).M(),mcEventWeight);
    }
  }
  for (size_t i=0; i<Zbosons_ptcut.size(); i++){
    for (size_t j=i+1; j<Zbosons_ptcut.size(); j++){
      auto z1 = Zbosons_ptcut.at(i);
      auto z2 = Zbosons_ptcut.at(j);
      m_4l_plus_allptcut->Fill((z1 + z2).M(),mcEventWeight);
    }
  }
  

  //higgs
  /*for (size_t i=0; i<Zbosons.size(); i++){
   for (size_t j=i+1; j<Zbosons.size(); j++){
     auto z1 = Zbosons.at(i);
     auto z2 = Zbosons.at(j);
     m_h_two_Zboson->Fill((z1 + z2).M(),mcEventWeight);
     m_h_Higgs->Fill((z1 + z2).M(),mcEventWeight);
     m_h_Higgs_pt->Fill((z1 + z2).Pt(),mcEventWeight);
   }
 }
  for (size_t i=0; i<photons.size(); i++){
   for (size_t j=i+1; j<photons.size(); j++){
     auto p1 = photons.at(i);
     auto p2 = photons.at(j);
     m_h_two_photon->Fill((p1 + p2).M(),mcEventWeight);
     m_h_Higgs->Fill((p1 + p2).M(),mcEventWeight);
     m_h_Higgs_pt->Fill((p1 + p2).Pt(),mcEventWeight);
   }
  }
  for (size_t i=0; i<Wbosons.size(); i++){
   for (size_t j=i+1; j<Wbosons.size(); j++){
     auto w1 = Wbosons.at(i);
     auto w2 = Wbosons.at(j);
     m_h_two_Wboson->Fill((w1 + w2).M(),mcEventWeight);
     m_h_Higgs->Fill((w1 + w2).M(),mcEventWeight);
     m_h_Higgs_pt->Fill((w1 + w2).Pt(),mcEventWeight);
   }
  }
  for (size_t i=0; i< tquarks.size(); i++){
    for (size_t j=i+1; j< tquarks.size(); j++){
      auto t1 =  tquarks.at(i);
      auto t2 =  tquarks.at(j);
      if((t1.Pt() > 20 )&& (t2.Pt() > 20)){
    m_h_two_Tquark->Fill((t1 + t2).M(),mcEventWeight);
    m_h_two_Tquark_pt->Fill((t1 + t2).Pt(),mcEventWeight);
    m_h_Higgs->Fill((t1 + t2).M(),mcEventWeight);
    m_h_Higgs_pt->Fill((t1 + t2).Pt(),mcEventWeight);
      }
    }
  }
  for (size_t i=0; i<bjets.size(); i++){
    for (size_t j=i+1; j<bjets.size(); j++){
      auto b1 = bjets.at(i);
      auto b2 = bjets.at(j);
      m_h_two_Bquark->Fill((b1 + b2).M(),mcEventWeight);
      m_h_Higgs->Fill((b1 + b2).M(),mcEventWeight);
      m_h_Higgs_pt->Fill((b1 + b2).Pt(),mcEventWeight);
    }
  }
 
  //zboson + others
  for(size_t i=0; i<Zbosons.size(); i++){
    for(size_t j=0; j<mets.size(); j++){
      auto z1 = Zbosons.at(i);
      auto m1 = mets.at(j);
      if(m1.Pt() < 30) continue;
      m_Zboson_met->Fill((z1+m1).M(),mcEventWeight);
      m_Zboson_met_pt->Fill((z1+m1).Pt(),mcEventWeight);
    }
  }
  for(size_t i=0; i<Zbosons.size(); i++){
    for(size_t j=0; j<ljets.size(); j++){
      auto z1 = Zbosons.at(i);
      auto l1 = ljets.at(j);
      m_Zboson_ljet->Fill((z1+l1).M(),mcEventWeight);
      m_Zboson_ljet_pt->Fill((z1+l1).Pt(),mcEventWeight);
    }
  }
  for(size_t i=0; i<Zbosons.size(); i++){
    for(size_t j=0; j<bjets.size(); j++){
      auto z1 = Zbosons.at(i);
      auto b1 = bjets.at(j);
      m_Zboson_bjet->Fill((z1+b1).M(),mcEventWeight);
      m_Zboson_bjet_pt->Fill((z1+b1).Pt(),mcEventWeight);
    }
    }*/

  
  
  //TTreetest
    
  //4l case
  std::vector<TLorentzVector> Zbosons_mumu;
  std::vector<TLorentzVector> Zbosons_ee;
  //mumu
  for (size_t i=0; i<muons.size(); i++){
    for (size_t j=i+1; j<muons.size(); j++){
      auto m1 = muons.at(i);
      auto m2 = muons.at(j);
      
      if(m1.Pt() < 20) continue;
      if(m2.Pt() < 20) continue;
      
      if(std::abs(m1.Eta()) > 2.5) continue;
      if(std::abs(m2.Eta()) > 2.5) continue;
      
      if((m1+m2).M() > 111) continue;
      if((m1+m2).M() < 71) continue;
      TLorentzVector Zboson_mumu_tlv;
      Zboson_mumu_tlv = m1 + m2;
      Zbosons_mumu.push_back(Zboson_mumu_tlv);
    }
  }
  //ee
  for (size_t i=0; i<electrons.size(); i++){
    for (size_t j=i+1; j<electrons.size(); j++){
      auto e1 = electrons.at(i);
      auto e2 = electrons.at(j);
	
      if(e1.Pt() < 20) continue;
      if(e2.Pt() < 20) continue;

      if(std::abs(e1.Eta()) > 2.47) continue;
      if(std::abs(e2.Eta()) > 2.47) continue;
      if(std::abs(e1.Eta()) > 1.37 && std::abs(e1.Eta()) < 1.52) continue;
      if(std::abs(e2.Eta()) > 1.37 && std::abs(e2.Eta()) < 1.52) continue;
	
      if((e1+e2).M() > 111) continue;
      if((e1+e2).M() < 71) continue;
      TLorentzVector Zboson_ee_tlv;
      Zboson_ee_tlv = e1 + e2;
      Zbosons_ee.push_back(Zboson_ee_tlv);
    }
  }
  //4l
  std::vector<TLorentzVector> four_lepton;
  for(size_t i=0; i<Zbosons_mumu.size(); i++){
    for(size_t j=0; j<Zbosons_ee.size(); j++){
      auto z_mumu = Zbosons_mumu.at(i);
      auto z_ee = Zbosons_ee.at(j);
      TLorentzVector four_lepton_tlv;
      four_lepton_tlv = z_mumu + z_ee;
      four_lepton.push_back(four_lepton_tlv);
    }
  }
  four_number = four_lepton.size();
  for(size_t i=0; i<four_lepton.size(); i++){
    auto f1 = four_lepton.at(i);
    m_four_phi = f1.Phi();
    m_four_eta = f1.Eta();
    m_four_pt = f1.Pt();
    m_four_e = f1.E();
    m_four_m = f1.M();
    weight_four = mcEventWeight;
      
    int four_nbytes = four_tree->Fill();
    ATH_MSG_DEBUG("Number of four_Data bytes written = " << four_nbytes);
  }

  //two L jet
  if(ljets.size()>1){
    h_diljet->Fill(1,mcEventWeight);
  }
  ljet_number = ljets.size();
  std::vector<TLorentzVector> ljets_100;
  for(size_t i=0; i<ljets.size(); i++){
    auto l1 = ljets.at(i);
    if(l1.Pt()<50) continue;
    if(l1.M()>111) continue;
    if(l1.M()<71) continue;
    ljets_100.push_back(l1);
  }
    
  Float_t ljet_pt_tlv1 = 0;
  Float_t ljet_pt_tlv2 = 0;
  TLorentzVector ljets_100_tlv1;
  TLorentzVector ljets_100_tlv2;
  size_t ljet_int = 0;
  if(ljets_100.size()>1){
    for(size_t i=0; i<ljets_100.size(); i++){
      auto l1 = ljets_100.at(i);
      if(l1.Pt()>ljet_pt_tlv1){
	ljet_pt_tlv1 = l1.Pt();
	ljets_100_tlv1 = l1;
	ljet_int = i;
      }
    }

    for(size_t i=0; i<ljets_100.size(); i++){
      if(i!=ljet_int){
	auto l2 = ljets_100.at(i);	  
	if(l2.Pt()>ljet_pt_tlv2){
	  ljet_pt_tlv2 = l2.Pt();
	  ljets_100_tlv2 = l2;
	}
      }	
    }
      
  }
  
    
    
  if(ljets_100.size()>1){
    Float_t ljet_phi_tlv1 = ljets_100_tlv1.Phi();
    Float_t ljet_phi_tlv2 = ljets_100_tlv2.Phi();
    if(ljet_phi_tlv1<0){
      ljet_phi_tlv1 = ljet_phi_tlv1 + 2*M_PI;
    }
    if(ljet_phi_tlv2<0){
      ljet_phi_tlv2 = ljet_phi_tlv2 + 2*M_PI;
    }
    if(ljet_phi_tlv1>=ljet_phi_tlv2){
      m_ljet_dphi = ljet_phi_tlv1 - ljet_phi_tlv2;
    }
    if(ljet_phi_tlv1<ljet_phi_tlv2){
      m_ljet_dphi = ljet_phi_tlv2 - ljet_phi_tlv1;
    }
    if(m_ljet_dphi>M_PI){
      m_ljet_dphi = 2*M_PI - m_ljet_dphi;
    }
  
    m_ljet_eta_1st = ljets_100_tlv1.Eta();
    m_ljet_pt_1st = ljets_100_tlv1.Pt();
    m_ljet_m_1st = ljets_100_tlv1.M();
    m_ljet_eta_2nd = ljets_100_tlv2.Eta();
    m_ljet_pt_2nd = ljets_100_tlv2.Pt();
    m_ljet_m_2nd = ljets_100_tlv2.M();
    auto dijet = ljets_100_tlv1 + ljets_100_tlv2;
    diljet_mass = dijet.M();
      
    weight_ljet = mcEventWeight;
    int ljet_nbytes = ljet_tree->Fill();
    ATH_MSG_DEBUG("Number of ljet_Data bytes written = " << ljet_nbytes);
    h_diljetcut->Fill(1,mcEventWeight);
  }
    
  //2l Zboson + large R jet
  if(ljets.size()>0){
    if((muons.size()>1) || (electrons.size()>1)){
      h_twol_ljet->Fill(1,mcEventWeight);
    }
  }
  std::vector<TLorentzVector> met_nonu;
  if(mets.at(0).Pt()<30){
    auto m1 = mets.at(0);
    met_nonu.push_back(m1);
  }


  Float_t two_ljet_pt_tlv1 = 0;
  TLorentzVector two_ljets_100_tlv1;
  if(ljets_100.size()>0){
    for(size_t i=0; i<ljets_100.size(); i++){
      auto l1 = ljets_100.at(i);
      if(l1.Pt()>two_ljet_pt_tlv1){
	two_ljet_pt_tlv1 = l1.Pt();
	two_ljets_100_tlv1 = l1;
      }
    }
  }

  TLorentzVector Zmumu_tlv;
  TLorentzVector Zee_tlv;
  Float_t Zmumu_pt_tlv = 0;
  Float_t Zee_pt_tlv = 0;
  if(Zbosons_mumu.size()>0){
    for(size_t i=0; i<Zbosons_mumu.size(); i++){
      auto m1 = Zbosons_mumu.at(i);
      if(m1.Pt()>Zmumu_pt_tlv){
	Zmumu_pt_tlv = m1.Pt();
	Zmumu_tlv = m1;
      }
    }
  }
  if(Zbosons_ee.size()>0){
    for(size_t i=0; i<Zbosons_ee.size(); i++){
      auto e1 = Zbosons_ee.at(i);
      if(e1.Pt()>Zee_pt_tlv){
	Zee_pt_tlv = e1.Pt();
	Zee_tlv = e1;
      }
    }
  }
  //if(met_nonu.size()>0){
  if(Zbosons_mumu.size()>0){
    if(ljets_100.size()>0){
      if(Zmumu_pt_tlv > Zee_pt_tlv){
	Float_t ljet_phi_tlv = two_ljets_100_tlv1.Phi();
	Float_t Zmumu_phi_tlv = Zmumu_tlv.Phi();
	if(ljet_phi_tlv<0){
	  ljet_phi_tlv = ljet_phi_tlv + 2*M_PI;
	}
	if(Zmumu_phi_tlv<0){
	  Zmumu_phi_tlv = Zmumu_phi_tlv + 2*M_PI;
	}
	if(ljet_phi_tlv>=Zmumu_phi_tlv){
	  m_twol_ljet_dphi = ljet_phi_tlv - Zmumu_phi_tlv;
	}
	if(ljet_phi_tlv<Zmumu_phi_tlv){
	  m_twol_ljet_dphi = Zmumu_phi_tlv - ljet_phi_tlv;
	}
	if(m_twol_ljet_dphi>M_PI){
	  m_twol_ljet_dphi = 2*M_PI - m_twol_ljet_dphi;
	}
	  
	m_twol_ljet_eta = two_ljets_100_tlv1.Eta();
	m_twol_ljet_pt = two_ljets_100_tlv1.Pt();
	m_twol_ljet_m = two_ljets_100_tlv1.M();
	m_twol_Zboson_eta = Zmumu_tlv.Eta();
	m_twol_Zboson_pt = Zmumu_tlv.Pt();
	m_twol_Zboson_m = Zmumu_tlv.M();
	auto twol_ljet = two_ljets_100_tlv1 + Zmumu_tlv;
	twol_ljet_mass = twol_ljet.M();
	
	weight_twol_ljet = mcEventWeight;
	if(two_ljets_100_tlv1.DeltaR(Zmumu_tlv) > 1){
	  int twol_ljet_nbytes = twol_ljet_tree->Fill();
	  ATH_MSG_DEBUG("Number of twol_ljet_Data bytes written = " << twol_ljet_nbytes);
	  h_twol_ljetcut->Fill(1,mcEventWeight);
	}
      }
    }
  }

  if(Zbosons_ee.size()>0){
    if(ljets_100.size()>0){
      if(Zee_pt_tlv > Zmumu_pt_tlv){
	  
	Float_t ljet_phi_tlv = two_ljets_100_tlv1.Phi();
	Float_t Zee_phi_tlv = Zee_tlv.Phi();
	if(ljet_phi_tlv<0){
	  ljet_phi_tlv = ljet_phi_tlv + 2*M_PI;
	}
	if(Zee_phi_tlv<0){
	  Zee_phi_tlv = Zee_phi_tlv + 2*M_PI;
	}
	if(ljet_phi_tlv>=Zee_phi_tlv){
	  m_twol_ljet_dphi = ljet_phi_tlv - Zee_phi_tlv;
	}
	if(ljet_phi_tlv<Zee_phi_tlv){
	  m_twol_ljet_dphi = Zee_phi_tlv - ljet_phi_tlv;
	}
	if(m_twol_ljet_dphi>M_PI){
	  m_twol_ljet_dphi = 2*M_PI - m_twol_ljet_dphi;
	}
	  
	m_twol_ljet_eta = two_ljets_100_tlv1.Eta();
	m_twol_ljet_pt = two_ljets_100_tlv1.Pt();
	m_twol_ljet_m = two_ljets_100_tlv1.M();
	m_twol_Zboson_eta = Zee_tlv.Eta();
	m_twol_Zboson_pt = Zee_tlv.Pt();
	m_twol_Zboson_m = Zee_tlv.M();
	auto twol_ljet = two_ljets_100_tlv1 + Zee_tlv;
	twol_ljet_mass = twol_ljet.M();

	weight_twol_ljet = mcEventWeight;
	if(two_ljets_100_tlv1.DeltaR(Zee_tlv) > 1){
	int twol_ljet_nbytes = twol_ljet_tree->Fill();
	ATH_MSG_DEBUG("Number of twol_ljet_Data bytes written = " << twol_ljet_nbytes);
	h_twol_ljetcut->Fill(1,mcEventWeight);
	}
      }
    }
  }
  // }
  
//sjet 
  std::vector<TLorentzVector> sjets_50;
  for(size_t i=0; i<sjets.size(); i++){
    auto s1 = sjets.at(i);
    if(s1.Pt()<30) continue;
    //if(s1.M()>111) continue;
    //if(s1.M()<71) continue;
    sjets_50.push_back(s1);
  }

  //2l+2q
  if(sjets.size()>1){
    if((muons.size()>1) || (electrons.size()>1)){
      h_twol_sjet->Fill(1,mcEventWeight);
    }
  }
  Float_t sjet_pt_tlv1 = 0;
  Float_t sjet_pt_tlv2 = 0;
  TLorentzVector sjets_50_tlv1;
  TLorentzVector sjets_50_tlv2;
  TLorentzVector two_sjets_50;
  size_t sjet_int = 0;
  if(sjets_50.size()>1){
    for(size_t i=0; i<sjets_50.size(); i++){
      auto l1 = sjets_50.at(i);
      if(l1.Pt()>sjet_pt_tlv1){
	sjet_pt_tlv1 = l1.Pt();
	sjets_50_tlv1 = l1;
	sjet_int = i;
      }
    }

    for(size_t i=0; i<sjets_50.size(); i++){
      if(i!=sjet_int){
	auto l2 = sjets_50.at(i);	  
	if(l2.Pt()>sjet_pt_tlv2){
	  sjet_pt_tlv2 = l2.Pt();
	  sjets_50_tlv2 = l2;
	}
      }	
    }
    two_sjets_50 =  sjets_50_tlv1 + sjets_50_tlv2;
  }

  
if(Zbosons_mumu.size()>0){
    if(sjets_50.size()>1){
      if(Zmumu_pt_tlv > Zee_pt_tlv){
	Float_t sjet_phi_tlv = two_sjets_50.Phi();
	Float_t Zmumu_phi_tlv = Zmumu_tlv.Phi();
	if(sjet_phi_tlv<0){
	  sjet_phi_tlv = sjet_phi_tlv + 2*M_PI;
	}
	if(Zmumu_phi_tlv<0){
	  Zmumu_phi_tlv = Zmumu_phi_tlv + 2*M_PI;
	}
	if(sjet_phi_tlv>=Zmumu_phi_tlv){
	  m_twol_sjet_dphi = sjet_phi_tlv - Zmumu_phi_tlv;
	}
	if(sjet_phi_tlv<Zmumu_phi_tlv){
	  m_twol_sjet_dphi = Zmumu_phi_tlv - sjet_phi_tlv;
	}
	if(m_twol_sjet_dphi>M_PI){
	  m_twol_sjet_dphi = 2*M_PI - m_twol_sjet_dphi;
	}
	  
	m_twol_sjet_first_eta = sjets_50_tlv1.Eta();
	m_twol_sjet_first_pt = sjets_50_tlv1.Pt();
	m_twol_sjet_first_m = sjets_50_tlv1.M();
	m_twol_sjet_second_eta = sjets_50_tlv2.Eta();
	m_twol_sjet_second_pt = sjets_50_tlv2.Pt();
	m_twol_sjet_second_m = sjets_50_tlv2.M();
	m_twol_sjet_Zboson_eta = Zmumu_tlv.Eta();
	m_twol_sjet_Zboson_pt = Zmumu_tlv.Pt();
	m_twol_sjet_Zboson_m = Zmumu_tlv.M();
	m_twol_sjet_m = two_sjets_50.M();
	auto twol_sjet = two_sjets_50 + Zmumu_tlv;
	twol_sjet_mass = twol_sjet.M();
	
	weight_twol_sjet = mcEventWeight;
	if((two_sjets_50.M()<111) && (two_sjets_50.M()>71) && (two_sjets_50.DeltaR(Zmumu_tlv) > 1)){
	int twol_sjet_nbytes = twol_sjet_tree->Fill();
	ATH_MSG_DEBUG("Number of twol_sjet_Data bytes written = " << twol_sjet_nbytes);
	h_twol_sjetcut->Fill(1,mcEventWeight);
	}
      }
    }
  }

  if(Zbosons_ee.size()>0){
    if(sjets_50.size()>1){
      if(Zee_pt_tlv > Zmumu_pt_tlv){
	  
	Float_t sjet_phi_tlv = two_sjets_50.Phi();
	Float_t Zee_phi_tlv = Zee_tlv.Phi();
	if(sjet_phi_tlv<0){
	  sjet_phi_tlv = sjet_phi_tlv + 2*M_PI;
	}
	if(Zee_phi_tlv<0){
	  Zee_phi_tlv = Zee_phi_tlv + 2*M_PI;
	}
	if(sjet_phi_tlv>=Zee_phi_tlv){
	  m_twol_sjet_dphi = sjet_phi_tlv - Zee_phi_tlv;
	}
	if(sjet_phi_tlv<Zee_phi_tlv){
	  m_twol_sjet_dphi = Zee_phi_tlv - sjet_phi_tlv;
	}
	if(m_twol_sjet_dphi>M_PI){
	  m_twol_sjet_dphi = 2*M_PI - m_twol_sjet_dphi;
	}
	  
	m_twol_sjet_first_eta = sjets_50_tlv1.Eta();
	m_twol_sjet_first_pt = sjets_50_tlv1.Pt();
	m_twol_sjet_first_m = sjets_50_tlv1.M();
	m_twol_sjet_second_eta = sjets_50_tlv2.Eta();
	m_twol_sjet_second_pt = sjets_50_tlv2.Pt();
	m_twol_sjet_second_m = sjets_50_tlv2.M();
	m_twol_Zboson_eta = Zee_tlv.Eta();
	m_twol_Zboson_pt = Zee_tlv.Pt();
	m_twol_Zboson_m = Zee_tlv.M();
	m_twol_sjet_m = two_sjets_50.M();
	auto twol_sjet = two_sjets_50 + Zee_tlv;
	twol_sjet_mass = twol_sjet.M();

	weight_twol_sjet = mcEventWeight;
        if((two_sjets_50.M()<121) && (two_sjets_50.M()>61) && (two_sjets_50.DeltaR(Zee_tlv) > 1)){
	int twol_sjet_nbytes = twol_sjet_tree->Fill();
	ATH_MSG_DEBUG("Number of twol_sjet_Data bytes written = " << twol_sjet_nbytes);
	h_twol_sjetcut->Fill(1,mcEventWeight);
	}
      }
    }
  }
  
  //four quark(sjet)
  std::vector<TLorentzVector> two_sjets;
  for(size_t i=0; i<sjets_50.size(); i++){
    for(size_t j=i+1; j<sjets_50.size(); j++){
      auto s1 = sjets_50.at(i);
      auto s2 = sjets_50.at(j);
      auto plus = s1+s2;
      if(plus.M()>121) continue;
      if(plus.M()<61) continue;
      two_sjets.push_back(plus);
    }
  }
  

  Float_t four_sjet_pt_tlv1 = 0;
  Float_t four_sjet_pt_tlv2 = 0;
  TLorentzVector four_sjets_50_tlv1;
  TLorentzVector four_sjets_50_tlv2;
  TLorentzVector four_sjets;
  size_t four_sjet_int = 0;
  if(two_sjets.size()>1){
    for(size_t i=0; i<two_sjets.size(); i++){
      auto s1 = two_sjets.at(i);
      if(s1.Pt()>four_sjet_pt_tlv1){
	four_sjet_pt_tlv1 = s1.Pt();
	four_sjets_50_tlv1 = s1;
	four_sjet_int = i;
      }
    }

    for(size_t i=0; i<two_sjets.size(); i++){
      if(i!=four_sjet_int){
	auto s2 = two_sjets.at(i);	  
	if(s2.Pt()>four_sjet_pt_tlv2){
	  four_sjet_pt_tlv2 = s2.Pt();
	  four_sjets_50_tlv2 = s2;
	}
      }	
    }
  }


  if(two_sjets.size()>1){
    
    four_sjets = four_sjets_50_tlv1 + four_sjets_50_tlv2;
    m_four_sjet_mass = four_sjets.M();
    weight_four_sjet = mcEventWeight;
    if((four_sjets_50_tlv1.DeltaR(four_sjets_50_tlv2) > 1)){
      int four_sjet_nbytes = four_sjet_tree->Fill();
      ATH_MSG_DEBUG("Number of four_sjet_Data bytes written = " << four_sjet_nbytes);
    }

  }
  
  //two lepton + met
  std::vector<TLorentzVector> mets_30;
  if(mets.at(0).Pt()>20){
    auto m1 = mets.at(0);
    mets_30.push_back(m1);
  }
  
  if(mets_30.size()>0){
    auto m1 = mets_30.at(0);
    if(Zbosons_mumu.size()>0){
      if(Zmumu_pt_tlv > Zee_pt_tlv){
	Float_t met_phi_tlv = m1.Phi();
	Float_t Zmumu_phi_tlv = Zmumu_tlv.Phi();
	if(met_phi_tlv<0){
	  met_phi_tlv = met_phi_tlv + 2*M_PI;
	}
	if(Zmumu_phi_tlv<0){
	  Zmumu_phi_tlv = Zmumu_phi_tlv + 2*M_PI;
	}
	if(met_phi_tlv>=Zmumu_phi_tlv){
	  m_Zboson_met_dphi = met_phi_tlv - Zmumu_phi_tlv;
	}
	if(met_phi_tlv<Zmumu_phi_tlv){
	  m_Zboson_met_dphi = Zmumu_phi_tlv - met_phi_tlv;
	}
	if(m_Zboson_met_dphi>M_PI){
	  m_Zboson_met_dphi = 2*M_PI - m_Zboson_met_dphi;
	}
      
	m_Zboson_eta = Zmumu_tlv.Eta();
	m_Zboson_pt = Zmumu_tlv.Pt();
	m_Zboson_m = Zmumu_tlv.M();
	m_met_pt = m1.Pt();
	auto Zmumu_met = Zmumu_tlv + m1;
	Zmumu_met_mass = Zmumu_met.M();
	if(Zmumu_tlv.DeltaR(m1) > 1){
	weight_Zmumumet = mcEventWeight;
	int Zmumu_nbytes = Zmumu_tree->Fill();
	ATH_MSG_DEBUG("Number of Zmumumet bytes written = " << Zmumu_nbytes);
	}
	 
      }
    }
    
    if(Zbosons_ee.size()>0){
      if(Zmumu_pt_tlv < Zee_pt_tlv){
	Float_t met_phi_tlv = m1.Phi();
	Float_t Zee_phi_tlv = Zee_tlv.Phi();
	if(met_phi_tlv<0){
	  met_phi_tlv = met_phi_tlv + 2*M_PI;
	}
	if(Zee_phi_tlv<0){
	  Zee_phi_tlv = Zee_phi_tlv + 2*M_PI;
	}
	if(met_phi_tlv>=Zee_phi_tlv){
	  m_Zboson_met_dphi = met_phi_tlv - Zee_phi_tlv;
	}
	if(met_phi_tlv<Zee_phi_tlv){
	  m_Zboson_met_dphi = Zee_phi_tlv - met_phi_tlv;
	}
	if(m_Zboson_met_dphi>M_PI){
	  m_Zboson_met_dphi = 2*M_PI - m_Zboson_met_dphi;
	}
      
	m_Zboson_eta = Zee_tlv.Eta();
	m_Zboson_pt = Zee_tlv.Pt();
	m_Zboson_m = Zee_tlv.M();
	m_met_pt = m1.Pt();
	auto Zee_met = Zee_tlv + m1;
	Zmumu_met_mass = Zee_met.M();
	if(Zee_tlv.DeltaR(m1) > 1){
	weight_Zmumumet = mcEventWeight;
	int Zmumu_nbytes = Zmumu_tree->Fill();
	ATH_MSG_DEBUG("Number of Zmumumet bytes written = " << Zmumu_nbytes);
	}
      }

    }
  }
  
  //two Bjet
  if(bjets.size()>1){
    h_dibjet->Fill(1,mcEventWeight);
  }
  bjet_number = bjets.size();
  std::vector<TLorentzVector> bjets_100;
  for(size_t i=0; i<bjets.size(); i++){
    auto b1 = bjets.at(i);
    if(b1.Pt()<30) continue;
    //if(b1.M()>20) continue;
    //if(b1.M()<2) continue;
    bjets_100.push_back(b1);
  }
    
  Float_t bjet_pt_tlv1 = 0;
  Float_t bjet_pt_tlv2 = 0;
  TLorentzVector bjets_100_tlv1;
  TLorentzVector bjets_100_tlv2;
  size_t bjet_int = 0;
  if(bjets_100.size()>1){
    for(size_t i=0; i<bjets_100.size(); i++){
      auto b1 = bjets_100.at(i);
      if(b1.Pt()>bjet_pt_tlv1){
	bjet_pt_tlv1 = b1.Pt();
	bjets_100_tlv1 = b1;
	bjet_int = i;
      }
    }

    for(size_t i=0; i<bjets_100.size(); i++){
      if(i!=bjet_int){
	auto b2 = bjets_100.at(i);	  
	if(b2.Pt()>bjet_pt_tlv2){
	  bjet_pt_tlv2 = b2.Pt();
	  bjets_100_tlv2 = b2;
	}
      }	
    }
    
  }
  
  if(bjets_100.size()>1){
    Float_t bjet_phi_tlv1 = bjets_100_tlv1.Phi();
    Float_t bjet_phi_tlv2 = bjets_100_tlv2.Phi();
    if(bjet_phi_tlv1<0){
      bjet_phi_tlv1 = bjet_phi_tlv1 + 2*M_PI;
    }
    if(bjet_phi_tlv2<0){
      bjet_phi_tlv2 = bjet_phi_tlv2 + 2*M_PI;
    }
    if(bjet_phi_tlv1>=bjet_phi_tlv2){
      m_bjet_dphi = bjet_phi_tlv1 - bjet_phi_tlv2;
    }
    if(bjet_phi_tlv1<bjet_phi_tlv2){
      m_bjet_dphi = bjet_phi_tlv2 - bjet_phi_tlv1;
    }
    if(m_bjet_dphi>M_PI){
      m_bjet_dphi = 2*M_PI - m_bjet_dphi;
    }

    m_bjet_eta_1st = bjets_100_tlv1.Eta();
    m_bjet_pt_1st = bjets_100_tlv1.Pt();
    m_bjet_m_1st = bjets_100_tlv1.M();
    m_bjet_eta_2nd = bjets_100_tlv2.Eta();
    m_bjet_pt_2nd = bjets_100_tlv2.Pt();
    m_bjet_m_2nd = bjets_100_tlv2.M();
    auto dibjet = bjets_100_tlv1 + bjets_100_tlv2;
    dibjet_mass = dibjet.M();
    weight_bjet = mcEventWeight;
    int bjet_nbytes = bjet_tree->Fill();
    ATH_MSG_DEBUG("Number of bjet_Data bytes written = " << bjet_nbytes);
    h_dibjetcut->Fill(1,mcEventWeight);
  }
  
  //tau
  if(taus.size()>1){
    h_ditau->Fill(1,mcEventWeight);
  }
  std::vector<TLorentzVector> taus_100;
  for(size_t i=0; i<taus.size(); i++){
    auto t1 = taus.at(i);
    if(t1.Pt()<20) continue;
    taus_100.push_back(t1);
  }

  //htau + ltau
  Float_t htau_pt = 0;
  TLorentzVector htaus_tlv;
  if(taus_100.size()>0){
    for(size_t i=0; i<taus_100.size(); i++){
      auto t1 = taus_100.at(i);
      if(t1.Pt()>htau_pt){
	htau_pt = t1.Pt();
	htaus_tlv = t1;
      }
    }
  }

  //
  if(mets_30.size()>0){
    if(taus_100.size()>0){
      auto m1 = mets_30.at(0);
      if(Zbosons_mumu.size()>0){
	if(Zmumu_pt_tlv > Zee_pt_tlv){
	  auto ltaus = Zmumu_tlv + m1;
	  m_ltau_htau_dphi = ltaus.DeltaPhi(htaus_tlv);
	  m_ltau_lepton_pt = Zmumu_tlv.Pt();
	  m_ltau_lepton_m = Zmumu_tlv.M();
	  m_ltau_lepton_eta = Zmumu_tlv.Eta();
	  m_htau_eta = htaus_tlv.Eta();
	  m_htau_pt = htaus_tlv.Pt();
	  m_ltau_htau_met_e = m1.E();
	  auto ditaus = ltaus + htaus_tlv;
	  ltau_htau_mass = ditaus.M();
	  if(ltaus.DeltaR(htaus_tlv) > 1){
	    weight_ltau_htau = mcEventWeight;
	    int ltau_htau_nbytes = ltau_htau_tree->Fill();
	    ATH_MSG_DEBUG("Number of ltau_htau bytes written = " << ltau_htau_nbytes);
	  }
	 
	}
      }
    
      if(Zbosons_ee.size()>0){
	if(Zmumu_pt_tlv < Zee_pt_tlv){
	  auto ltaus = Zee_tlv + m1;
	  m_ltau_htau_dphi = ltaus.DeltaPhi(htaus_tlv);
	  m_ltau_lepton_pt = Zee_tlv.Pt();
	  m_ltau_lepton_m = Zee_tlv.M();
	  m_ltau_lepton_eta = Zee_tlv.Eta();
	  m_htau_eta = htaus_tlv.Eta();
	  m_htau_pt = htaus_tlv.Pt();
	  m_ltau_htau_met_e = m1.E();
	  auto ditaus = ltaus + htaus_tlv;
	  ltau_htau_mass = ditaus.M();
	  if(ltaus.DeltaR(htaus_tlv) > 1){
	    weight_ltau_htau = mcEventWeight;
	    int ltau_htau_nbytes = ltau_htau_tree->Fill();
	    ATH_MSG_DEBUG("Number of ltau_htau bytes written = " << ltau_htau_nbytes);
	  }
	}

      }
    }
  }

  
    
  Float_t tau_pt_tlv1 = 0;
  Float_t tau_pt_tlv2 = 0;
  TLorentzVector taus_100_tlv1;
  TLorentzVector taus_100_tlv2;
  size_t tau_int = 0;
  if(taus_100.size()>1){
    for(size_t i=0; i<taus_100.size(); i++){
      auto t1 = taus_100.at(i);
      if(t1.Pt()>tau_pt_tlv1){
	tau_pt_tlv1 = t1.Pt();
	taus_100_tlv1 = t1;
	tau_int = i;
      }
    }

    for(size_t i=0; i<taus_100.size(); i++){
      if(i!=tau_int){
	auto t2 = taus_100.at(i);	  
	if(t2.Pt()>tau_pt_tlv2){
	  tau_pt_tlv2 = t2.Pt();
	  taus_100_tlv2 = t2;
	}
      }	
    }
    
  }
  if(mets_30.size()>0){
    if(taus_100.size()>1){
      Float_t tau_phi_tlv1 = taus_100_tlv1.Phi();
      Float_t tau_phi_tlv2 = taus_100_tlv2.Phi();
      if(tau_phi_tlv1<0){
	tau_phi_tlv1 = tau_phi_tlv1 + 2*M_PI;
      }
      if(tau_phi_tlv2<0){
	tau_phi_tlv2 = tau_phi_tlv2 + 2*M_PI;
      }
      if(tau_phi_tlv1>=tau_phi_tlv2){
	m_tau_dphi = tau_phi_tlv1 - tau_phi_tlv2;
      }
      if(tau_phi_tlv1<tau_phi_tlv2){
	m_tau_dphi = tau_phi_tlv2 - tau_phi_tlv1;
      }
      if(m_tau_dphi>M_PI){
	m_tau_dphi = 2*M_PI - m_tau_dphi;
      }
      
      m_tau_eta_1st = taus_100_tlv1.Eta();
      m_tau_pt_1st = taus_100_tlv1.Pt();
      //m_tau_m_1st = taus_100_tlv1.M();
      m_tau_eta_2nd = taus_100_tlv2.Eta();
      m_tau_pt_2nd = taus_100_tlv2.Pt();
      //m_tau_m_2nd = taus_100_tlv2.M();
      auto m1 = mets_30.at(0);
      // auto ditau = taus_100_tlv1 + taus_100_tlv2 + mets_30.at(0);
      m_tau_met_e = m1.E();
      auto ditau = taus_100_tlv1 + taus_100_tlv2 + m1;
      ditau_mass = ditau.M();
      weight_tau = mcEventWeight;
      int tau_nbytes = tau_tree->Fill();
      ATH_MSG_DEBUG("Number of tau_Data bytes written = " << tau_nbytes);
      h_ditaucut->Fill(1,mcEventWeight);
    }
  }

  
  //2photons
  std::vector<TLorentzVector> photons_100;
  for(size_t i=0; i<photons.size(); i++){
    auto l1 = photons.at(i);
    if(l1.Pt()<20) continue;
    photons_100.push_back(l1);
  }
    
  Float_t photon_pt_tlv1 = 0;
  Float_t photon_pt_tlv2 = 0;
  TLorentzVector photons_100_tlv1;
  TLorentzVector photons_100_tlv2;
  size_t photon_int = 0;
  if(photons_100.size()>1){
    for(size_t i=0; i<photons_100.size(); i++){
      auto l1 = photons_100.at(i);
      if(l1.Pt()>photon_pt_tlv1){
	photon_pt_tlv1 = l1.Pt();
	photons_100_tlv1 = l1;
	photon_int = i;
      }
    }

    for(size_t i=0; i<photons_100.size(); i++){
      if(i!=photon_int){
	auto l2 = photons_100.at(i);	  
	if(l2.Pt()>photon_pt_tlv2){
	  photon_pt_tlv2 = l2.Pt();
	  photons_100_tlv2 = l2;
	}
      }	
    }
      
  }
  
  if(photons_100.size()>1){
    Float_t photon_phi_tlv1 = photons_100_tlv1.Phi();
    Float_t photon_phi_tlv2 = photons_100_tlv2.Phi();
    if(photon_phi_tlv1<0){
      photon_phi_tlv1 = photon_phi_tlv1 + 2*M_PI;
    }
    if(photon_phi_tlv2<0){
      photon_phi_tlv2 = photon_phi_tlv2 + 2*M_PI;
    }
    if(photon_phi_tlv1>=photon_phi_tlv2){
      m_photon_dphi = photon_phi_tlv1 - photon_phi_tlv2;
    }
    if(photon_phi_tlv1<photon_phi_tlv2){
      m_photon_dphi = photon_phi_tlv2 - photon_phi_tlv1;
    }
    if(m_photon_dphi>M_PI){
      m_photon_dphi = 2*M_PI - m_photon_dphi;
    }
  
    m_photon_eta_1st = photons_100_tlv1.Eta();
    m_photon_pt_1st = photons_100_tlv1.Pt();
    m_photon_m_1st = photons_100_tlv1.M();
    m_photon_eta_2nd = photons_100_tlv2.Eta();
    m_photon_pt_2nd = photons_100_tlv2.Pt();
    m_photon_m_2nd = photons_100_tlv2.M();
      
    weight_photon = mcEventWeight;
    int photon_nbytes = photon_tree->Fill();
    ATH_MSG_DEBUG("Number of photon_Data bytes written = " << photon_nbytes);
  }

  //Wlnulnu
  //Wmumu
  std::vector<TLorentzVector> Wlnu_leptons;
  for (size_t i=0; i<muons.size(); i++){
      auto m1 = muons.at(i); 
      if(m1.Pt() < 20) continue; 
      if(std::abs(m1.Eta()) > 2.5) continue;
      Wlnu_leptons.push_back(m1);
  }
  //ee
  for (size_t i=0; i<electrons.size(); i++){
    auto e1 = electrons.at(i);
    if(e1.Pt() < 20) continue;
    if(std::abs(e1.Eta()) > 2.47) continue;
    if(std::abs(e1.Eta()) > 1.37 && std::abs(e1.Eta()) < 1.52) continue;
    Wlnu_leptons.push_back(e1);
  }
  
  Float_t Wlnu_lepton_pt_tlv1 = 0;
  Float_t Wlnu_lepton_pt_tlv2 = 0;
  TLorentzVector Wlnu_leptons_tlv1;
  TLorentzVector Wlnu_leptons_tlv2;
  size_t Wlnu_lepton_int = 0;
  if(Wlnu_leptons.size()>1){
    for(size_t i=0; i<Wlnu_leptons.size(); i++){
      auto l1 = Wlnu_leptons.at(i);
      if(l1.Pt()>Wlnu_lepton_pt_tlv1){
	Wlnu_lepton_pt_tlv1 = l1.Pt();
	Wlnu_leptons_tlv1 = l1;
        Wlnu_lepton_int = i;
      }
    }

    for(size_t i=0; i<Wlnu_leptons.size(); i++){
      if(i!=Wlnu_lepton_int){
	auto l2 = Wlnu_leptons.at(i);	  
	if(l2.Pt()>Wlnu_lepton_pt_tlv2){
	  Wlnu_lepton_pt_tlv2 = l2.Pt();
	  Wlnu_leptons_tlv2 = l2;
	}
      }	
    }
      
  }

  
  if(Wlnu_leptons.size()>1){
    if(mets_30.size()>0){
      m_Wlnu_eta_1st = Wlnu_leptons_tlv1.Eta();
      m_Wlnu_pt_1st = Wlnu_leptons_tlv1.Pt();
      m_Wlnu_m_1st = Wlnu_leptons_tlv1.M();
      m_Wlnu_eta_2nd = Wlnu_leptons_tlv2.Eta();
      m_Wlnu_pt_2nd = Wlnu_leptons_tlv2.Pt();
      m_Wlnu_m_2nd = Wlnu_leptons_tlv2.M();
      m_Wlnu_met_pt = mets_30.at(0).Pt();
      auto Wlnu = Wlnu_leptons_tlv1 + Wlnu_leptons_tlv2 + mets_30.at(0);
      m_Wlnu_mass = Wlnu.Pt();
      weight_Wlnu = mcEventWeight;
      int Wlnu_nbytes = Wlnu_tree->Fill();
      ATH_MSG_DEBUG("Number of WlnuTree bytes written = " << Wlnu_nbytes);
      
    }
  }

  //Wlnuqq
  std::vector<TLorentzVector> Wl_ljets;
  for(size_t i=0; i<ljets.size(); i++){
    auto l1 = ljets.at(i);
    if(l1.Pt()<20) continue;
    if(l1.M()>100) continue;
    if(l1.M()<60) continue;
    Wl_ljets.push_back(l1);
  }
  Float_t Wl_ljet_pt_tlv = 0;
  TLorentzVector Wl_ljets_tlv;
  if(Wl_ljets.size()>0){
    for(size_t i=0; i<Wl_ljets.size(); i++){
      auto l1 = Wl_ljets.at(i);
      if(l1.Pt()>Wl_ljet_pt_tlv){
	Wl_ljet_pt_tlv = l1.Pt();
	Wl_ljets_tlv = l1;
      }
    }
  }

  Float_t Wlnu_onelepton_pt = 0;
  TLorentzVector Wlnu_oneleptons;
  if(Wlnu_leptons.size()>0){
    for(size_t i=0; i<Wlnu_leptons.size(); i++){
      auto l1 = Wlnu_leptons.at(i);
      if(l1.Pt()>Wlnu_onelepton_pt){
	Wlnu_onelepton_pt = l1.Pt();
        Wlnu_oneleptons = l1;
      }
    } 
  }

  if(Wlnu_leptons.size()>0){
    if(Wl_ljets.size()>0){
      if(mets_30.size()>0){
	m_Wl_ljet_ljet_eta = Wl_ljets_tlv.Eta();
	m_Wl_ljet_ljet_pt = Wl_ljets_tlv.Pt();
	m_Wl_ljet_ljet_m = Wl_ljets_tlv.M();
	m_Wl_ljet_l_eta = Wlnu_oneleptons.Eta();
	m_Wl_ljet_l_pt = Wlnu_oneleptons.Pt();
	m_Wl_ljet_l_m = Wlnu_oneleptons.M();
	m_Wl_ljet_met_pt = mets_30.at(0).Pt();
	auto Wlnu = Wlnu_leptons_tlv1 + mets_30.at(0);
	m_Wl_ljet_lnu_m = Wlnu.M();
	auto Wlnu_ljet = Wl_ljets_tlv + Wlnu_oneleptons + mets_30.at(0);
	m_Wl_ljet_mass = Wlnu_ljet.M();
	weight_Wl_ljet = mcEventWeight;
	if(Wlnu.M()>5){
	int Wl_ljet_nbytes = Wl_ljet_tree->Fill();
	ATH_MSG_DEBUG("Number of Wl_ljetTree bytes written = " << Wl_ljet_nbytes);
	}
      }
    }
  }

  //Wl_sjet
  std::vector<TLorentzVector> Wl_sjets;
  for(size_t i=0; i<sjets.size(); i++){
    auto l1 = sjets.at(i);
    if(l1.Pt()<20) continue;
    Wl_sjets.push_back(l1);
  }
    
  Float_t Wl_sjet_pt_tlv1 = 0;
  Float_t Wl_sjet_pt_tlv2 = 0;
  TLorentzVector Wl_sjets_tlv1;
  TLorentzVector Wl_sjets_tlv2;
  size_t Wl_sjet_int = 0;
  if(Wl_sjets.size()>1){
    for(size_t i=0; i<Wl_sjets.size(); i++){
      auto l1 = Wl_sjets.at(i);
      if(l1.Pt()>Wl_sjet_pt_tlv1){
	Wl_sjet_pt_tlv1 = l1.Pt();
	Wl_sjets_tlv1 = l1;
	Wl_sjet_int = i;
      }
    }

    for(size_t i=0; i<Wl_sjets.size(); i++){
      if(i!=Wl_sjet_int){
	auto l2 = Wl_sjets.at(i);	  
	if(l2.Pt()>Wl_sjet_pt_tlv2){
	  Wl_sjet_pt_tlv2 = l2.Pt();
	  Wl_sjets_tlv2 = l2;
	}
      }	
    }
  }

  if(Wlnu_leptons.size()>0){
    if(Wl_sjets.size()>1){
      if(mets_30.size()>0){
	m_Wl_sjet_sjet_eta_1st = Wl_sjets_tlv1.Eta();
	m_Wl_sjet_sjet_pt_1st = Wl_sjets_tlv1.Pt();
	m_Wl_sjet_sjet_m_1st = Wl_sjets_tlv1.M();
	m_Wl_sjet_sjet_eta_2nd = Wl_sjets_tlv2.Eta();
	m_Wl_sjet_sjet_pt_2nd = Wl_sjets_tlv2.Pt();
	m_Wl_sjet_sjet_m_2nd = Wl_sjets_tlv2.M();
	auto W_disjet = Wl_sjets_tlv1 + Wl_sjets_tlv2;
	m_Wl_sjet_disjet_m = W_disjet.M();
	m_Wl_sjet_l_eta = Wlnu_leptons_tlv1.Eta();
	m_Wl_sjet_l_pt = Wlnu_leptons_tlv1.Pt();
	m_Wl_sjet_l_m = Wlnu_leptons_tlv1.M();
	m_Wl_sjet_met_pt = mets_30.at(0).Pt();
	auto Wlnu = Wlnu_leptons_tlv1 + mets_30.at(0);
	m_Wl_sjet_lnu_m = Wlnu.M();
	auto Wlnu_sjet = Wl_sjets_tlv1 + Wl_sjets_tlv2 + Wlnu_leptons_tlv1 + mets_30.at(0);
	m_Wl_sjet_mass = Wlnu_sjet.M();
	weight_Wl_sjet = mcEventWeight;
	if((W_disjet.M()>60) && (W_disjet.M()<100)){
	  if((Wlnu.M()>5)){
	  int Wl_sjet_nbytes = Wl_sjet_tree->Fill();
	  ATH_MSG_DEBUG("Number of Wl_sjetTree bytes written = " << Wl_sjet_nbytes);
	  }
	}
      }
    }
  }


  //Htt_tree
  std::vector<TLorentzVector> tll_ljets;
  for(size_t i=0; i<ljets.size(); i++){
    auto l1 = ljets.at(i);
    if(l1.Pt()<20) continue;
    if(l1.M()>193) continue;
    if(l1.M()<153) continue;
    tll_ljets.push_back(l1);
  }
  if(tll_ljets.size()>1){
    h_ttbar->Fill(1);
  }
    
  Float_t tll_ljet_pt_tlv1 = 0;
  Float_t tll_ljet_pt_tlv2 = 0;
  TLorentzVector tll_ljets_tlv1;
  TLorentzVector tll_ljets_tlv2;
  size_t tll_ljet_int = 0;
  if(tll_ljets.size()>1){
    for(size_t i=0; i<tll_ljets.size(); i++){
      auto l1 = tll_ljets.at(i);
      if(l1.Pt()>tll_ljet_pt_tlv1){
	tll_ljet_pt_tlv1 = l1.Pt();
	tll_ljets_tlv1 = l1;
	tll_ljet_int = i;
      }
    }
    for(size_t i=0; i<tll_ljets.size(); i++){
      if(i!=tll_ljet_int){
	auto l2 = tll_ljets.at(i);	  
	if(l2.Pt()>tll_ljet_pt_tlv2){
	  tll_ljet_pt_tlv2 = l2.Pt();
	  tll_ljets_tlv2 = l2;
	}
      }	
    }
      
  } 
    
  if(tll_ljets.size()>1){
    
    m_tll_dphi = tll_ljets_tlv1.DeltaPhi(tll_ljets_tlv2);
    m_tll_ljet_eta_1st = tll_ljets_tlv1.Eta();
    m_tll_ljet_pt_1st = tll_ljets_tlv1.Pt();
    m_tll_ljet_m_1st = tll_ljets_tlv1.M();
    m_tll_ljet_eta_2nd = tll_ljets_tlv2.Eta();
    m_tll_ljet_pt_2nd = tll_ljets_tlv2.Pt();
    m_tll_ljet_m_2nd = tll_ljets_tlv2.M();
    auto tll = tll_ljets_tlv1 + tll_ljets_tlv2;
    m_tll_mass = tll.M();
    
    weight_tll = mcEventWeight;
    //if(tll_ljets_tlv1.DeltaR(tll_ljets_tlv2) > 1){
      int tll_nbytes = tll_tree->Fill();
      ATH_MSG_DEBUG("Number of tllTree bytes written = " << tll_nbytes);
      //}
  }

  //HtWb
  std::vector<TLorentzVector> tWb_bjets;
  for(size_t i=0; i<bjets.size(); i++){
    auto b1 = bjets.at(i);
    if(b1.Pt()<20) continue;
    //if(b1.M()>20) continue;
    //if(b1.M()<2) continue;
    tWb_bjets.push_back(b1);
  }
    
  Float_t tWb_bjet_pt_tlv1 = 0;
  Float_t tWb_bjet_pt_tlv2 = 0;
  TLorentzVector tWb_bjets_tlv1;
  TLorentzVector tWb_bjets_tlv2;
  size_t tWb_bjet_int = 0;
  if(tWb_bjets.size()>1){
    for(size_t i=0; i<tWb_bjets.size(); i++){
      auto b1 = tWb_bjets.at(i);
      if(b1.Pt()>tWb_bjet_pt_tlv1){
	tWb_bjet_pt_tlv1 = b1.Pt();
	tWb_bjets_tlv1 = b1;
	tWb_bjet_int = i;
      }
    }

    for(size_t i=0; i<tWb_bjets.size(); i++){
      if(i!=tWb_bjet_int){
	auto b2 = tWb_bjets.at(i);	  
	if(b2.Pt()>tWb_bjet_pt_tlv2){
	  tWb_bjet_pt_tlv2 = b2.Pt();
	  tWb_bjets_tlv2 = b2;
	}
      }	
    }
  }
  if(Wlnu_leptons.size()>1){
    if(tWb_bjets.size()>1){
      if(mets_30.size()>0){
	m_tWb_l_eta_1st = Wlnu_leptons_tlv1.Eta();
	m_tWb_l_pt_1st = Wlnu_leptons_tlv1.Pt();
	m_tWb_l_m_1st = Wlnu_leptons_tlv1.M();
	m_tWb_l_eta_2nd = Wlnu_leptons_tlv2.Eta();
	m_tWb_l_pt_2nd = Wlnu_leptons_tlv2.Pt();
	m_tWb_l_m_2nd = Wlnu_leptons_tlv2.M();
	m_tWb_met_pt = mets_30.at(0).Pt();
	m_tWb_b_eta_1st = tWb_bjets_tlv1.Eta();
	m_tWb_b_pt_1st = tWb_bjets_tlv1.Pt();
	m_tWb_b_m_1st = tWb_bjets_tlv1.M();
	m_tWb_b_eta_2nd = tWb_bjets_tlv2.Eta();
	m_tWb_b_pt_2nd = tWb_bjets_tlv2.Pt();
	m_tWb_b_m_2nd = tWb_bjets_tlv2.M();
	auto Htt = Wlnu_leptons_tlv1 + Wlnu_leptons_tlv2 + mets_30.at(0) + tWb_bjets_tlv1 + tWb_bjets_tlv2;
	m_tWb_mass = Htt.M();
	weight_tWb = mcEventWeight;

	int tWb_nbytes = tWb_tree->Fill();
	ATH_MSG_DEBUG("Number of tWbTree bytes written = " << tWb_nbytes);
	
      }
    }
  }

  std::vector<TLorentzVector> tWl_ljets;
  for(size_t i=0; i<ljets.size(); i++){
    auto l1 = ljets.at(i);
    if(l1.Pt()<20) continue;
    if(l1.M()>100) continue;
    if(l1.M()<60) continue;
    tWl_ljets.push_back(l1);
  }
 
  Float_t tWl_ljet_pt_tlv1 = 0;
  Float_t tWl_ljet_pt_tlv2 = 0;
  TLorentzVector tWl_ljets_tlv1;
  TLorentzVector tWl_ljets_tlv2;
  size_t tWl_ljet_int = 0;
  if(tWl_ljets.size()>1){
    for(size_t i=0; i<tWl_ljets.size(); i++){
      auto l1 = tWl_ljets.at(i);
      if(l1.Pt()>tWl_ljet_pt_tlv1){
	tWl_ljet_pt_tlv1 = l1.Pt();
	tWl_ljets_tlv1 = l1;
	tWl_ljet_int = i;
      }
    }

    for(size_t i=0; i<tWl_ljets.size(); i++){
      if(i!=tWl_ljet_int){
	auto l2 = tWl_ljets.at(i);	  
	if(l2.Pt()>tWl_ljet_pt_tlv2){
	  tWl_ljet_pt_tlv2 = l2.Pt();
	  tWl_ljets_tlv2 = l2;
	}
      }	
    }
      
  }


  Float_t tWl_ljet_pt_tlv = 0;
  TLorentzVector tWl_ljets_tlv;
  if(tWl_ljets.size()>0){
    for(size_t i=0; i<tWl_ljets.size(); i++){
      auto l1 = tWl_ljets.at(i);
      if(l1.Pt()>tWl_ljet_pt_tlv){
	tWl_ljet_pt_tlv = l1.Pt();
	tWl_ljets_tlv = l1;
      }
    } 
  }

  //tlnub+tqqb
  if(mets_30.size()>0){
    if(tWb_bjets.size()>1){
      if(Wlnu_leptons.size()>0){
	if(tWl_ljets.size()>0){
	  m_tljet_b_l_eta_1st = Wlnu_oneleptons.Eta();
	  m_tljet_b_l_pt_1st = Wlnu_oneleptons.Pt();
	  m_tljet_b_l_m_1st = Wlnu_oneleptons.M();
	  m_tljet_b_met_pt = mets_30.at(0).Pt();
	  m_tljet_b_ljet_eta = tWl_ljets_tlv.Eta();
	  m_tljet_b_ljet_pt = tWl_ljets_tlv.Pt();
	  m_tljet_b_ljet_m = tWl_ljets_tlv.M();
	  m_tljet_b_b_eta_1st = tWb_bjets_tlv1.Eta();
	  m_tljet_b_b_pt_1st = tWb_bjets_tlv1.Pt();
	  m_tljet_b_b_m_1st = tWb_bjets_tlv1.M();
	  m_tljet_b_b_eta_2nd = tWb_bjets_tlv2.Eta();
	  m_tljet_b_b_pt_2nd = tWb_bjets_tlv2.Pt();
	  m_tljet_b_b_m_2nd = tWb_bjets_tlv2.M();
	  auto Htt = Wlnu_oneleptons + mets_30.at(0) + tWl_ljets_tlv + tWb_bjets_tlv1 + tWb_bjets_tlv2;
	  m_tljet_b_mass = Htt.M();
	  weight_tljet_b = mcEventWeight;
	  auto Wlnu = tWl_ljets_tlv + mets_30.at(0);
	  //if(Wlnu.M()>5){
	  int tljet_b_nbytes = tljet_b_tree->Fill();
	  ATH_MSG_DEBUG("Number of tljet_bTree bytes written = " << tljet_b_nbytes);
	  //}
	}
      } 
    }
  }
  //tLLbb
  if(tWb_bjets.size()>1){
    if(tWl_ljets.size()>1){
      m_ttwoljet_b_ljet_eta_1st = tWl_ljets_tlv1.Eta();
      m_ttwoljet_b_ljet_pt_1st = tWl_ljets_tlv1.Pt();
      m_ttwoljet_b_ljet_m_1st = tWl_ljets_tlv1.M();
      m_ttwoljet_b_ljet_eta_2nd = tWl_ljets_tlv2.Eta();
      m_ttwoljet_b_ljet_pt_2nd = tWl_ljets_tlv2.Pt();
      m_ttwoljet_b_ljet_m_2nd = tWl_ljets_tlv2.M();
      m_ttwoljet_b_b_eta_1st = tWb_bjets_tlv1.Eta();
      m_ttwoljet_b_b_pt_1st = tWb_bjets_tlv1.Pt();
      m_ttwoljet_b_b_m_1st = tWb_bjets_tlv1.M();
      m_ttwoljet_b_b_eta_2nd = tWb_bjets_tlv2.Eta();
      m_ttwoljet_b_b_pt_2nd = tWb_bjets_tlv2.Pt();
      m_ttwoljet_b_b_m_2nd = tWb_bjets_tlv2.M();
      auto Htt = tWl_ljets_tlv1 + tWl_ljets_tlv2 + tWb_bjets_tlv1 + tWb_bjets_tlv2;
      m_ttwoljet_b_mass = Htt.M();
      weight_ttwoljet_b = mcEventWeight;
      if(tWb_bjets_tlv1.DeltaR(tWl_ljets_tlv1) < 1){
	if(tWb_bjets_tlv2.DeltaR(tWl_ljets_tlv2) < 1){
	  int ttwoljet_b_nbytes = ttwoljet_b_tree->Fill();
	  ATH_MSG_DEBUG("Number of ttwoljet_bTree bytes written = " << ttwoljet_b_nbytes);
	}
      }
      else if(tWb_bjets_tlv1.DeltaR(tWl_ljets_tlv2) < 1){
	if(tWb_bjets_tlv2.DeltaR(tWl_ljets_tlv2) < 1){
	  int ttwoljet_b_nbytes = ttwoljet_b_tree->Fill();
	  ATH_MSG_DEBUG("Number of ttwoljet_bTree bytes written = " << ttwoljet_b_nbytes);
	}
      }
    }
  }
  //tL+Wlnub
  Float_t tll_ljet_pt_tlv = 0;
  TLorentzVector tll_ljets_tlv;
  if(tll_ljets.size()>0){
    for(size_t i=0; i<tll_ljets.size(); i++){
      auto l1 = tll_ljets.at(i);
      if(l1.Pt()>tll_ljet_pt_tlv){
	tll_ljet_pt_tlv = l1.Pt();
	tll_ljets_tlv = l1;
      }
    }
  }
  
  Float_t tWb_bjet_pt_tlv = 0;
  TLorentzVector tWb_bjets_tlv;
  if(tWb_bjets.size()>0){
    for(size_t i=0; i<tWb_bjets.size(); i++){
      auto b1 = tWb_bjets.at(i);
      if(b1.Pt()>tWb_bjet_pt_tlv){
	tWb_bjet_pt_tlv = b1.Pt();
	tWb_bjets_tlv = b1;
      }
    }
  }

  if(tll_ljets.size()>0){
    if(Wlnu_leptons.size()>0){
      if(mets_30.size()>0){
	if(tWb_bjets.size()>0){
	  m_tljetWb_ljet_eta = tll_ljets_tlv.Eta();
	  m_tljetWb_ljet_pt = tll_ljets_tlv.Pt();
	  m_tljetWb_ljet_m = tll_ljets_tlv.M();
	  m_tljetWb_l_eta = Wlnu_oneleptons.Eta();
	  m_tljetWb_l_pt = Wlnu_oneleptons.Pt();
	  m_tljetWb_l_m = Wlnu_oneleptons.M();
	  auto m1 = mets_30.at(0);
	  m_tljetWb_met_pt = m1.Pt();
	  m_tljetWb_b_eta = tWb_bjets_tlv.Eta();
	  m_tljetWb_b_pt = tWb_bjets_tlv.Pt();
	  m_tljetWb_b_m = tWb_bjets_tlv.M();
	  auto Htt = tll_ljets_tlv + Wlnu_oneleptons + m1 + tWb_bjets_tlv;
	  m_tljetWb_mass = Htt.M();
	  weight_tljetWb = mcEventWeight;
	  int tljetWb_nbytes = tljetWb_tree->Fill();
	  ATH_MSG_DEBUG("Number of tljetWbTree bytes written = " << tljetWb_nbytes);
	}
      }
    }
  }

  //tl + wlb
  if(tll_ljets.size()>0){
    if(tWl_ljets.size()>0){
      if(tWb_bjets.size()>0){
	m_ttwoljet_oneb_tljet_eta = tll_ljets_tlv.Eta();
	m_ttwoljet_oneb_tljet_pt = tll_ljets_tlv.Pt();
	m_ttwoljet_oneb_tljet_m = tll_ljets_tlv.M();
	m_ttwoljet_oneb_Wljet_eta = tWl_ljets_tlv.Eta();
	m_ttwoljet_oneb_Wljet_pt = tWl_ljets_tlv.Pt();
	m_ttwoljet_oneb_Wljet_m = tWl_ljets_tlv.M();
	m_ttwoljet_oneb_b_eta = tWb_bjets_tlv.Eta();
	m_ttwoljet_oneb_b_pt = tWb_bjets_tlv.Pt();
	m_ttwoljet_oneb_b_m = tWb_bjets_tlv.M();
	auto Htt = tll_ljets_tlv + tWl_ljets_tlv + tWb_bjets_tlv;
	m_ttwoljet_oneb_mass = Htt.M();
	weight_ttwoljet_oneb = mcEventWeight;
	if(tWb_bjets_tlv.DeltaR(tWl_ljets_tlv) < 1){
	  int ttwoljet_oneb_nbytes = ttwoljet_oneb_tree->Fill();
	  ATH_MSG_DEBUG("Number of ttwoljet_onebTree bytes written = " << ttwoljet_oneb_nbytes);
	}
      }
    }
  }

  

  //Hhh
  std::vector<TLorentzVector> hbb_bjets;
  for(size_t i=0; i<bjets.size(); i++){
    auto b1 = bjets.at(i);
    if(b1.Pt()<50) continue;
    //if(b1.M()>20) continue;
    if(b1.M()<2) continue;
    hbb_bjets.push_back(b1);
  }
    
  Float_t hbb_bjet_pt_tlv1 = 0;
  Float_t hbb_bjet_pt_tlv2 = 0;
  Float_t hbb_bjet_pt_tlv3 = 0;
  Float_t hbb_bjet_pt_tlv4 = 0;
  TLorentzVector hbb_bjets_tlv1;
  TLorentzVector hbb_bjets_tlv2;
  TLorentzVector hbb_bjets_tlv3;
  TLorentzVector hbb_bjets_tlv4;
  size_t hbb_bjet_int1 = 0;
  size_t hbb_bjet_int2 = 0;
  size_t hbb_bjet_int3 = 0;
  if(hbb_bjets.size()>3){
    for(size_t i=0; i<hbb_bjets.size(); i++){
      auto b1 = hbb_bjets.at(i);
      if(b1.Pt()>hbb_bjet_pt_tlv1){
	hbb_bjet_pt_tlv1 = b1.Pt();
	hbb_bjets_tlv1 = b1;
	hbb_bjet_int1 = i;
      }
    }

    for(size_t i=0; i<hbb_bjets.size(); i++){
      if(i!=hbb_bjet_int1){
	auto b2 = hbb_bjets.at(i);	  
	if(b2.Pt()>hbb_bjet_pt_tlv2){
	  hbb_bjet_pt_tlv2 = b2.Pt();
	  hbb_bjets_tlv2 = b2;
	  hbb_bjet_int2 = i;
	}
      }	
    }

    for(size_t i=0; i<hbb_bjets.size(); i++){
      if((i!=hbb_bjet_int1) &&(i!=hbb_bjet_int2)){
	auto b3 = hbb_bjets.at(i);	  
	if(b3.Pt()>hbb_bjet_pt_tlv3){
	  hbb_bjet_pt_tlv3 = b3.Pt();
	  hbb_bjets_tlv3 = b3;
	}
      }	
    }
    
    for(size_t i=0; i<hbb_bjets.size(); i++){
      if((i!=hbb_bjet_int1) && (i!=hbb_bjet_int2) && (i!=hbb_bjet_int3)){
	auto b4 = hbb_bjets.at(i);	  
	if(b4.Pt()>hbb_bjet_pt_tlv4){
	  hbb_bjet_pt_tlv4 = b4.Pt();
	  hbb_bjets_tlv4 = b4;
	}
      }	
    }
  }

  if(hbb_bjets.size()>3){
    m_Hhh_b_eta_1st = hbb_bjets_tlv1.Eta();
    m_Hhh_b_pt_1st = hbb_bjets_tlv1.Pt();
    m_Hhh_b_m_1st = hbb_bjets_tlv1.M();
    m_Hhh_b_eta_2nd = hbb_bjets_tlv2.Eta();
    m_Hhh_b_pt_2nd = hbb_bjets_tlv2.Pt();
    m_Hhh_b_m_2nd = hbb_bjets_tlv2.M();
    m_Hhh_b_eta_3rd = hbb_bjets_tlv3.Eta();
    m_Hhh_b_pt_3rd = hbb_bjets_tlv3.Pt();
    m_Hhh_b_m_3rd = hbb_bjets_tlv3.M();
    m_Hhh_b_eta_4th = hbb_bjets_tlv4.Eta();
    m_Hhh_b_pt_4th = hbb_bjets_tlv4.Pt();
    m_Hhh_b_m_4th = hbb_bjets_tlv4.M();
    std::vector<float> hbb_dRs;
    hbb_dRs.push_back(hbb_bjets_tlv1.DeltaR(hbb_bjets_tlv2));
    hbb_dRs.push_back(hbb_bjets_tlv1.DeltaR(hbb_bjets_tlv3));
    hbb_dRs.push_back(hbb_bjets_tlv1.DeltaR(hbb_bjets_tlv4));
    hbb_dRs.push_back(hbb_bjets_tlv2.DeltaR(hbb_bjets_tlv3));
    hbb_dRs.push_back(hbb_bjets_tlv2.DeltaR(hbb_bjets_tlv4));
    hbb_dRs.push_back(hbb_bjets_tlv3.DeltaR(hbb_bjets_tlv4));
    float dR = 0;
    int dR_int = 0;
    for(size_t i=0; i<hbb_dRs.size(); i++){
      if(dR < hbb_dRs.at(i)){
	dR = hbb_dRs.at(i);
	dR_int = i;
      }
    }
    TLorentzVector hbb1;
    TLorentzVector hbb2;
    switch(dR_int){
    case 0 :
      hbb1 = hbb_bjets_tlv1 + hbb_bjets_tlv2;
      hbb2 = hbb_bjets_tlv3 + hbb_bjets_tlv4;
      m_Hhh_dphi = hbb_dRs.at(0);
      break;
    case 1 :
      hbb1 = hbb_bjets_tlv1 + hbb_bjets_tlv3;
      hbb2 = hbb_bjets_tlv2 + hbb_bjets_tlv4;
      m_Hhh_dphi = hbb_dRs.at(1);
      break;
    case 2 :
      hbb1 = hbb_bjets_tlv1 + hbb_bjets_tlv4;
      hbb2 = hbb_bjets_tlv2 + hbb_bjets_tlv3;
      m_Hhh_dphi = hbb_dRs.at(2);
      break;
    case 3 :
      hbb1 = hbb_bjets_tlv2 + hbb_bjets_tlv3;
      hbb2 = hbb_bjets_tlv1 + hbb_bjets_tlv4;
      m_Hhh_dphi = hbb_dRs.at(3);
      break;
    case 4 :
      hbb1 = hbb_bjets_tlv2 + hbb_bjets_tlv4;
      hbb2 = hbb_bjets_tlv1 + hbb_bjets_tlv3;
      m_Hhh_dphi = hbb_dRs.at(4);
      break;
    case 5 :
      hbb1 = hbb_bjets_tlv3 + hbb_bjets_tlv4;
      hbb2 = hbb_bjets_tlv1 + hbb_bjets_tlv1;
      m_Hhh_dphi = hbb_dRs.at(5);
      break;
    }
    m_Hhh_h_m_1st =  hbb1.M();
    m_Hhh_h_m_2nd = hbb2.M();
    auto Hhh = hbb1 + hbb2;
    m_Hhh_mass = Hhh.M();
    
    weight_Hhh = mcEventWeight;

    int Hhh_nbytes = Hhh_tree->Fill();
    ATH_MSG_DEBUG("Number of HhhTree bytes written = " << Hhh_nbytes);

  }

  //truth particle
  
  ////////////////////////TruthAntiKt4TruthJets&b-jet//////////////////////////
  /*const xAOD::JetContainer* truthSjetContainer = nullptr;
  ATH_CHECK(evtStore()->retrieve(truthSjetContainer, "AntiKt4TruthDressedWZJets"));
  ATH_MSG_DEBUG("truthSjets successfully retrieved");
  ATH_MSG_DEBUG("# of truthsjets = " << truthSjetContainer->size());
    
  unsigned int ntruthSjets = 0;
    
  std::vector<TLorentzVector> truthsjets;  //important
  std::vector<TLorentzVector> truthbjets;
  for (const xAOD::Jet* sjet : *truthSjetContainer) {
    m_h_truthsjet_e->Fill(sjet->e()/1000.); // Energy in GeV
    m_h_truthsjet_pt->Fill(sjet->pt()/1000.); // Transverse momentum in GeV
    m_h_truthsjet_eta->Fill(sjet->eta()); // Pseudorapidity
    m_h_truthsjet_phi->Fill(sjet->phi()); // Azimuthal angle
    m_h2_truthsjet_eta_phi->Fill(sjet->eta(), sjet->phi()); // 2D distribution, X:eta, Y:phi

    ++ntruthSjets;
   
    TLorentzVector truthsjet_tlv;
    truthsjet_tlv.SetPtEtaPhiM(sjet->pt()/1000, sjet->eta(), sjet->phi(), sjet->m()/1000.);
    truthsjets.push_back(truthsjet_tlv);
    bool isBjet = false;
    int truthlabel(-1);
    sjet->getAttribute("HadronConeExclTruthLabelID", truthlabel);
    isBjet = std::abs(truthlabel) == 5;
    if (isBjet){
      m_h_truthbjet_e->Fill(sjet->e()/1000.); // Energy in GeV
      m_h_truthbjet_pt->Fill(sjet->pt()/1000.); // Transverse momentum in GeV
      m_h_truthbjet_eta->Fill(sjet->eta()); // Pseudorapidity
      m_h_truthbjet_phi->Fill(sjet->phi()); // Azimuthal angle
      m_h2_truthbjet_eta_phi->Fill(sjet->eta(), sjet->phi()); // 2D distribution, X:eta, Y:phi
      truthbjets.push_back(truthsjet_tlv);
    }
  }

   //////////////////////////TruthTaus////////////////////////////
  const xAOD::TruthParticleContainer* truthTauContainer = nullptr;
  ATH_CHECK(evtStore()->retrieve(truthTauContainer, "TruthTaus"));
  ATH_MSG_DEBUG("truthTaus successfully retrieved");
  ATH_MSG_DEBUG("# of truthtaus = " << truthTauContainer->size());
  
  unsigned int ntruthTaus = 0;
  std::vector<TLorentzVector> truthtaus;  //important
  for (const xAOD::TruthParticle* tau : *truthTauContainer) {
    m_h_truthtau_e->Fill(tau->e()/1000.); // Energy in GeV
    m_h_truthtau_pt->Fill(tau->pt()/1000.); // Transverse momentum in GeV
    m_h_truthtau_eta->Fill(tau->eta()); // Pseudorapidity
    m_h_truthtau_phi->Fill(tau->phi()); // Azimuthal angle
    m_h2_truthtau_eta_phi->Fill(tau->eta(), tau->phi()); // 2D distribution, X:eta, Y:phi
    
    ++ntruthTaus;
    
    TLorentzVector truthtau_tlv;
    truthtau_tlv.SetPtEtaPhiM(tau->pt()/1000, tau->eta(), tau->phi(), tau->m()/1000.);
    truthtaus.push_back(truthtau_tlv);
  }

  const xAOD::TruthParticleContainer* truthPhotonContainer = nullptr;
  ATH_CHECK(evtStore()->retrieve(truthPhotonContainer, "TruthPhotons"));
  ATH_MSG_DEBUG("truthPhotons successfully retrieved");
  ATH_MSG_DEBUG("# of truthphotons = " << truthPhotonContainer->size());
  
  unsigned int ntruthPhotons = 0;
  std::vector<TLorentzVector> truthphotons;  //important
  for (const xAOD::TruthParticle* photon : *truthPhotonContainer) {
    m_h_truthphoton_e->Fill(photon->e()/1000.); // Energy in GeV
    m_h_truthphoton_pt->Fill(photon->pt()/1000.); // Transverse momentum in GeV
    m_h_truthphoton_eta->Fill(photon->eta()); // Pseudorapidity
    m_h_truthphoton_phi->Fill(photon->phi()); // Azimuthal angle
    m_h2_truthphoton_eta_phi->Fill(photon->eta(), photon->phi()); // 2D distribution, X:eta, Y:phi
    
    ++ntruthPhotons;
    
    TLorentzVector truthphoton_tlv;
    truthphoton_tlv.SetPtEtaPhiE(photon->pt()/1000, photon->eta(), photon->phi(), photon->e()/1000.);
    truthphotons.push_back(truthphoton_tlv);
  }

  //truthljets
   ///////////////TruthAntiKt10TruthTrimmedPtFrac5SmallR20Jets////////////
  const xAOD::JetContainer* truthLjetContainer = nullptr;
  ATH_CHECK(evtStore()->retrieve(truthLjetContainer, "AntiKt10TruthTrimmedPtFrac5SmallR20Jets"));
  ATH_MSG_DEBUG("truthLjets successfully retrieved");
  ATH_MSG_DEBUG("# of truthljets = " << truthLjetContainer->size());

  unsigned int ntruthLjets = 0;
  std::vector<TLorentzVector> truthljets;  //important
  for (const xAOD::Jet* ljet : *truthLjetContainer) {
    m_h_truthljet_e->Fill(ljet->e()/1000.); // Energy in GeV
    m_h_truthljet_pt->Fill(ljet->pt()/1000.); // Transverse momentum in GeV
    m_h_truthljet_eta->Fill(ljet->eta()); // Pseudorapidity
    m_h_truthljet_phi->Fill(ljet->phi()); // Azimuthal angle
    m_h2_truthljet_eta_phi->Fill(ljet->eta(), ljet->phi()); // 2D distribution, X:eta, Y:phi

    ++ntruthLjets;
   
    TLorentzVector truthljet_tlv;
    truthljet_tlv.SetPtEtaPhiM(ljet->pt()/1000, ljet->eta(), ljet->phi(), ljet->m()/1000.);
    truthljets.push_back(truthljet_tlv);
  }

  //truthmuons
  const xAOD::TruthParticleContainer* truthMuonContainer = nullptr;
  ATH_CHECK(evtStore()->retrieve(truthMuonContainer, "TruthMuons"));
  ATH_MSG_DEBUG("truthMuons successfully retrieved");
  ATH_MSG_DEBUG("# of truthmuons = " << truthMuonContainer->size());
  
  std::vector<TLorentzVector> truthmuons;  //important
  for (const xAOD::TruthParticle* muon : *truthMuonContainer) {
    m_h_truthmuon_e->Fill(muon->e()/1000.); // Energy in GeV
    m_h_truthmuon_pt->Fill(muon->pt()/1000.); // Transverse momentum in GeV
    m_h_truthmuon_eta->Fill(muon->eta()); // Pseudorapidity
    m_h_truthmuon_phi->Fill(muon->phi()); // Azimuthal angle
    m_h2_truthmuon_eta_phi->Fill(muon->eta(), muon->phi()); // 2D distribution, X:eta, Y:phi
    
    
    TLorentzVector truthmuon_tlv;
    truthmuon_tlv.SetPtEtaPhiM(muon->pt()/1000, muon->eta(), muon->phi(), muon->m()/1000.);
    truthmuons.push_back(truthmuon_tlv);
  
  }
  
  //TruthElectrons
  const xAOD::TruthParticleContainer* truthElectronContainer = nullptr;
  ATH_CHECK(evtStore()->retrieve(truthElectronContainer, "TruthElectrons"));
  ATH_MSG_DEBUG("truthElectrons successfully retrieved");
  ATH_MSG_DEBUG("# of truthelectrons = " << truthElectronContainer->size());
  std::vector<TLorentzVector> truthelectrons;  //important
  for (const xAOD::TruthParticle* electron : *truthElectronContainer) {
    m_h_truthelectron_e->Fill(electron->e()/1000.); // Energy in GeV
    m_h_truthelectron_pt->Fill(electron->pt()/1000.); // Transverse momentum in GeV
    m_h_truthelectron_eta->Fill(electron->eta()); // Pseudorapidity
    m_h_truthelectron_phi->Fill(electron->phi()); // Azimuthal angle
    m_h2_truthelectron_eta_phi->Fill(electron->eta(), electron->phi()); // 2D distribution, X:eta, Y:phi
    
    TLorentzVector truthelectron_tlv;
    truthelectron_tlv.SetPtEtaPhiM(electron->pt()/1000, electron->eta(), electron->phi(), electron->m()/1000.);
    truthelectrons.push_back(truthelectron_tlv);
  }


  //ee mumu setup
  std::vector<TLorentzVector> truthZbosons_mumu;
  std::vector<TLorentzVector> truthZbosons_ee;
  //mumu
  for (size_t i=0; i<truthmuons.size(); i++){
    for (size_t j=i+1; j<truthmuons.size(); j++){
      auto m1 = truthmuons.at(i);
      auto m2 = truthmuons.at(j);
      
      if(m1.Pt() < 20) continue;
      if(m2.Pt() < 20) continue;
      
      if(std::abs(m1.Eta()) > 2.5) continue;
      if(std::abs(m2.Eta()) > 2.5) continue;
      
      if((m1+m2).M() > 121) continue;
      if((m1+m2).M() < 61) continue;
      TLorentzVector truthZboson_mumu_tlv;
      truthZboson_mumu_tlv = m1 + m2;
      truthZbosons_mumu.push_back(truthZboson_mumu_tlv);
    }
  }
  //ee
  for (size_t i=0; i<truthelectrons.size(); i++){
    for (size_t j=i+1; j<truthelectrons.size(); j++){
      auto e1 = truthelectrons.at(i);
      auto e2 = truthelectrons.at(j);
	
      if(e1.Pt() < 20) continue;
      if(e2.Pt() < 20) continue;

      if(std::abs(e1.Eta()) > 2.47) continue;
      if(std::abs(e2.Eta()) > 2.47) continue;
      if(std::abs(e1.Eta()) > 1.37 && std::abs(e1.Eta()) < 1.52) continue;
      if(std::abs(e2.Eta()) > 1.37 && std::abs(e2.Eta()) < 1.52) continue;
	
      if((e1+e2).M() > 121) continue;
      if((e1+e2).M() < 61) continue;
      TLorentzVector truthZboson_ee_tlv;
      truthZboson_ee_tlv = e1 + e2;
      truthZbosons_ee.push_back(truthZboson_ee_tlv);
    }
  }

  //truthljet
  std::vector<TLorentzVector> truthljets_100;
  for(size_t i=0; i<truthljets.size(); i++){
    auto l1 = truthljets.at(i);
    if(l1.Pt()<50) continue;
    if(l1.M()>121) continue;
    if(l1.M()<61) continue;
    truthljets_100.push_back(l1);
  }
    
  Float_t truthljet_pt_tlv1 = 0;
  Float_t truthljet_pt_tlv2 = 0;
  TLorentzVector truthljets_100_tlv1;
  TLorentzVector truthljets_100_tlv2;
  size_t truthljet_int = 0;
  if(truthljets_100.size()>1){
    for(size_t i=0; i<truthljets_100.size(); i++){
      auto l1 = truthljets_100.at(i);
      if(l1.Pt()>truthljet_pt_tlv1){
	truthljet_pt_tlv1 = l1.Pt();
	truthljets_100_tlv1 = l1;
	truthljet_int = i;
      }
    }

    for(size_t i=0; i<truthljets_100.size(); i++){
      if(i!=truthljet_int){
	auto l2 = truthljets_100.at(i);	  
	if(l2.Pt()>truthljet_pt_tlv2){
	  truthljet_pt_tlv2 = l2.Pt();
	  truthljets_100_tlv2 = l2;
	}
      }	
    }
      
  }
  //truth diljet
  if(truthljets_100.size()>1){
    Float_t truthljet_phi_tlv1 = truthljets_100_tlv1.Phi();
    Float_t truthljet_phi_tlv2 = truthljets_100_tlv2.Phi();
    if(truthljet_phi_tlv1<0){
      truthljet_phi_tlv1 = truthljet_phi_tlv1 + 2*M_PI;
    }
    if(truthljet_phi_tlv2<0){
      truthljet_phi_tlv2 = truthljet_phi_tlv2 + 2*M_PI;
    }
    if(truthljet_phi_tlv1>=truthljet_phi_tlv2){
      m_truthljet_dphi = truthljet_phi_tlv1 - truthljet_phi_tlv2;
    }
    if(truthljet_phi_tlv1<truthljet_phi_tlv2){
      m_truthljet_dphi = truthljet_phi_tlv2 - truthljet_phi_tlv1;
    }
    if(m_truthljet_dphi>M_PI){
      m_truthljet_dphi = 2*M_PI - m_truthljet_dphi;
    }
  
    m_truthljet_eta_1st = truthljets_100_tlv1.Eta();
    m_truthljet_pt_1st = truthljets_100_tlv1.Pt();
    m_truthljet_m_1st = truthljets_100_tlv1.M();
    m_truthljet_eta_2nd = truthljets_100_tlv2.Eta();
    m_truthljet_pt_2nd = truthljets_100_tlv2.Pt();
    m_truthljet_m_2nd = truthljets_100_tlv2.M();
    auto dijet = truthljets_100_tlv1 + truthljets_100_tlv2;
    ditruthljet_mass = dijet.M();
    weight_truthljet = mcEventWeight;
    int truthljet_nbytes = truthljet_tree->Fill();
    ATH_MSG_DEBUG("Number of truthljet_Data bytes written = " << truthljet_nbytes); 
  }

  //truth two lepton + ljet
  Float_t two_truthljet_pt_tlv1 = 0;
  TLorentzVector two_truthljets_100_tlv1;
  if(truthljets_100.size()>0){
    for(size_t i=0; i<truthljets_100.size(); i++){
      auto l1 = truthljets_100.at(i);
      if(l1.Pt()>two_truthljet_pt_tlv1){
	two_truthljet_pt_tlv1 = l1.Pt();
	two_truthljets_100_tlv1 = l1;
      }
    }
  }

  TLorentzVector truthZmumu_tlv;
  TLorentzVector truthZee_tlv;
  Float_t truthZmumu_pt_tlv = 0;
  Float_t truthZee_pt_tlv = 0;
  if(truthZbosons_mumu.size()>0){
    for(size_t i=0; i<truthZbosons_mumu.size(); i++){
      auto m1 = truthZbosons_mumu.at(i);
      if(m1.Pt()>truthZmumu_pt_tlv){
	truthZmumu_pt_tlv = m1.Pt();
	truthZmumu_tlv = m1;
      }
    }
  }
  if(truthZbosons_ee.size()>0){
    for(size_t i=0; i<truthZbosons_ee.size(); i++){
      auto e1 = truthZbosons_ee.at(i);
      if(e1.Pt()>truthZee_pt_tlv){
	truthZee_pt_tlv = e1.Pt();
	truthZee_tlv = e1;
      }
    }
  }
  if(truthZbosons_mumu.size()>0){
    if(truthljets_100.size()>0){
      if(truthZmumu_pt_tlv > truthZee_pt_tlv){
	Float_t truthljet_phi_tlv = two_truthljets_100_tlv1.Phi();
	Float_t truthZmumu_phi_tlv = truthZmumu_tlv.Phi();
	if(truthljet_phi_tlv<0){
	  truthljet_phi_tlv = truthljet_phi_tlv + 2*M_PI;
	}
	if(truthZmumu_phi_tlv<0){
	  truthZmumu_phi_tlv = truthZmumu_phi_tlv + 2*M_PI;
	}
	if(truthljet_phi_tlv>=truthZmumu_phi_tlv){
	  m_twol_truthljet_dphi = truthljet_phi_tlv - truthZmumu_phi_tlv;
	}
	if(truthljet_phi_tlv<truthZmumu_phi_tlv){
	  m_twol_truthljet_dphi = truthZmumu_phi_tlv - truthljet_phi_tlv;
	}
	if(m_twol_truthljet_dphi>M_PI){
	  m_twol_truthljet_dphi = 2*M_PI - m_twol_truthljet_dphi;
	}
	  
	m_twol_truthljet_eta = two_truthljets_100_tlv1.Eta();
	m_twol_truthljet_pt = two_truthljets_100_tlv1.Pt();
	m_twol_truthljet_m = two_truthljets_100_tlv1.M();
	m_twol_truthZboson_eta = truthZmumu_tlv.Eta();
	m_twol_truthZboson_pt = truthZmumu_tlv.Pt();
	m_twol_truthZboson_m = truthZmumu_tlv.M();
	auto twol_truthljet = two_truthljets_100_tlv1 + truthZmumu_tlv;
	//auto twol_truthljet = two_truthljets_100_tlv1;
	twol_truthljet_mass = twol_truthljet.M();
        
	weight_twol_truthljet = mcEventWeight;
	if(two_truthljets_100_tlv1.DeltaR(truthZmumu_tlv) > 1){
	int twol_truthljet_nbytes = twol_truthljet_tree->Fill();
	ATH_MSG_DEBUG("Number of twol_truthljet_Data bytes written = " << twol_truthljet_nbytes);
	}
      }
    }
  }

  if(truthZbosons_ee.size()>0){
    if(truthljets_100.size()>0){
      if(truthZee_pt_tlv > truthZmumu_pt_tlv){
	  
	Float_t truthljet_phi_tlv = two_truthljets_100_tlv1.Phi();
	Float_t truthZee_phi_tlv = truthZee_tlv.Phi();
	if(truthljet_phi_tlv<0){
	  truthljet_phi_tlv = truthljet_phi_tlv + 2*M_PI;
	}
	if(truthZee_phi_tlv<0){
	  truthZee_phi_tlv = truthZee_phi_tlv + 2*M_PI;
	}
	if(truthljet_phi_tlv>=truthZee_phi_tlv){
	  m_twol_truthljet_dphi = truthljet_phi_tlv - truthZee_phi_tlv;
	}
	if(truthljet_phi_tlv<truthZee_phi_tlv){
	  m_twol_truthljet_dphi = truthZee_phi_tlv - truthljet_phi_tlv;
	}
	if(m_twol_truthljet_dphi>M_PI){
	  m_twol_truthljet_dphi = 2*M_PI - m_twol_truthljet_dphi;
	}
	  
	m_twol_truthljet_eta = two_truthljets_100_tlv1.Eta();
	m_twol_truthljet_pt = two_truthljets_100_tlv1.Pt();
	m_twol_truthljet_m = two_truthljets_100_tlv1.M();
	m_twol_truthZboson_eta = truthZee_tlv.Eta();
	m_twol_truthZboson_pt = truthZee_tlv.Pt();
	m_twol_truthZboson_m = truthZee_tlv.M();
	auto twol_truthljet = two_truthljets_100_tlv1 + truthZee_tlv;
	//auto twol_truthljet = two_truthljets_100_tlv1;
	twol_truthljet_mass = twol_truthljet.M();
	
	weight_twol_truthljet = mcEventWeight;
	if(two_truthljets_100_tlv1.DeltaR(truthZee_tlv) > 1){
	int twol_truthljet_nbytes = twol_truthljet_tree->Fill();
	ATH_MSG_DEBUG("Number of twol_truthljet_Data bytes written = " << twol_truthljet_nbytes);
	}
      }
    }
  }
  
  //2truthbjet
  std::vector<TLorentzVector> truthbjets_100;
  for(size_t i=0; i<truthbjets.size(); i++){
    auto b1 = truthbjets.at(i);
    if(b1.Pt()<50) continue;
    truthbjets_100.push_back(b1);
  }
    
  Float_t truthbjet_pt_tlv1 = 0;
  Float_t truthbjet_pt_tlv2 = 0;
  TLorentzVector truthbjets_100_tlv1;
  TLorentzVector truthbjets_100_tlv2;
  size_t truthbjet_int = 0;
  if(truthbjets_100.size()>1){
    for(size_t i=0; i<truthbjets_100.size(); i++){
      auto b1 = truthbjets_100.at(i);
      if(b1.Pt()>truthbjet_pt_tlv1){
	truthbjet_pt_tlv1 = b1.Pt();
	truthbjets_100_tlv1 = b1;
	truthbjet_int = i;
      }
    }

    for(size_t i=0; i<truthbjets_100.size(); i++){
      if(i!=truthbjet_int){
	auto b2 = truthbjets_100.at(i);	  
	if(b2.Pt()>truthbjet_pt_tlv2){
	  truthbjet_pt_tlv2 = b2.Pt();
	  truthbjets_100_tlv2 = b2;
	}
      }	
    }
    
  }
  
  if(truthbjets_100.size()>1){
    Float_t truthbjet_phi_tlv1 = truthbjets_100_tlv1.Phi();
    Float_t truthbjet_phi_tlv2 = truthbjets_100_tlv2.Phi();
    if(truthbjet_phi_tlv1<0){
      truthbjet_phi_tlv1 = truthbjet_phi_tlv1 + 2*M_PI;
    }
    if(truthbjet_phi_tlv2<0){
      truthbjet_phi_tlv2 = truthbjet_phi_tlv2 + 2*M_PI;
    }
    if(truthbjet_phi_tlv1>=truthbjet_phi_tlv2){
      m_truthbjet_dphi = truthbjet_phi_tlv1 - truthbjet_phi_tlv2;
    }
    if(truthbjet_phi_tlv1<truthbjet_phi_tlv2){
      m_truthbjet_dphi = truthbjet_phi_tlv2 - truthbjet_phi_tlv1;
    }
    if(m_truthbjet_dphi>M_PI){
      m_truthbjet_dphi = 2*M_PI - m_truthbjet_dphi;
    }

    m_truthbjet_eta_1st = truthbjets_100_tlv1.Eta();
    m_truthbjet_pt_1st = truthbjets_100_tlv1.Pt();
    m_truthbjet_m_1st = truthbjets_100_tlv1.M();
    m_truthbjet_eta_2nd = truthbjets_100_tlv2.Eta();
    m_truthbjet_pt_2nd = truthbjets_100_tlv2.Pt();
    m_truthbjet_m_2nd = truthbjets_100_tlv2.M();
    auto ditruthbjet = truthbjets_100_tlv1 + truthbjets_100_tlv2;
    ditruthbjet_mass = ditruthbjet.M();
    weight_truthbjet = mcEventWeight;
    int truthbjet_nbytes = truthbjet_tree->Fill();
    ATH_MSG_DEBUG("Number of truthbjet_Data bytes written = " << truthbjet_nbytes);
  }
  //truthtau
  std::vector<TLorentzVector> truthtaus_100;
  for(size_t i=0; i<truthtaus.size(); i++){
    auto t1 = truthtaus.at(i);
    //if(t1.Pt()<100) continue;
    truthtaus_100.push_back(t1);
  }
    
  Float_t truthtau_pt_tlv1 = 0;
  Float_t truthtau_pt_tlv2 = 0;
  TLorentzVector truthtaus_100_tlv1;
  TLorentzVector truthtaus_100_tlv2;
  size_t truthtau_int = 0;
  if(truthtaus_100.size()>1){
    for(size_t i=0; i<truthtaus_100.size(); i++){
      auto t1 = truthtaus_100.at(i);
      if(t1.Pt()>truthtau_pt_tlv1){
	truthtau_pt_tlv1 = t1.Pt();
	truthtaus_100_tlv1 = t1;
	truthtau_int = i;
      }
    }

    for(size_t i=0; i<truthtaus_100.size(); i++){
      if(i!=truthtau_int){
	auto t2 = truthtaus_100.at(i);	  
	if(t2.Pt()>truthtau_pt_tlv2){
	  truthtau_pt_tlv2 = t2.Pt();
	  truthtaus_100_tlv2 = t2;
	}
      }	
    }
    
  }
  
  if(truthtaus_100.size()>1){
    Float_t truthtau_phi_tlv1 = truthtaus_100_tlv1.Phi();
    Float_t truthtau_phi_tlv2 = truthtaus_100_tlv2.Phi();
    if(truthtau_phi_tlv1<0){
      truthtau_phi_tlv1 = truthtau_phi_tlv1 + 2*M_PI;
    }
    if(truthtau_phi_tlv2<0){
      truthtau_phi_tlv2 = truthtau_phi_tlv2 + 2*M_PI;
    }
    if(truthtau_phi_tlv1>=truthtau_phi_tlv2){
      m_truthtau_dphi = truthtau_phi_tlv1 - truthtau_phi_tlv2;
    }
    if(truthtau_phi_tlv1<truthtau_phi_tlv2){
      m_truthtau_dphi = truthtau_phi_tlv2 - truthtau_phi_tlv1;
    }
    if(m_truthtau_dphi>M_PI){
      m_truthtau_dphi = 2*M_PI - m_truthtau_dphi;
    }

    m_truthtau_eta_1st = truthtaus_100_tlv1.Eta();
    m_truthtau_pt_1st = truthtaus_100_tlv1.Pt();
    m_truthtau_m_1st = truthtaus_100_tlv1.M();
    m_truthtau_eta_2nd = truthtaus_100_tlv2.Eta();
    m_truthtau_pt_2nd = truthtaus_100_tlv2.Pt();
    m_truthtau_m_2nd = truthtaus_100_tlv2.M();
    auto ditruthtau = truthtaus_100_tlv1 + truthtaus_100_tlv2;
    ditruthtau_mass = ditruthtau.M();
    weight_truthtau = mcEventWeight;
    int truthtau_nbytes = truthtau_tree->Fill();
    ATH_MSG_DEBUG("Number of truthtau_Data bytes written = " << truthtau_nbytes);
  }

  //truthphoton
  std::vector<TLorentzVector> truthphotons_100;
  for(size_t i=0; i<truthphotons.size(); i++){
    auto b1 = truthphotons.at(i);
    //if(b1.Pt()<20) continue;
    truthphotons_100.push_back(b1);
  }
    
  Float_t truthphoton_pt_tlv1 = 0;
  Float_t truthphoton_pt_tlv2 = 0;
  TLorentzVector truthphotons_100_tlv1;
  TLorentzVector truthphotons_100_tlv2;
  size_t truthphoton_int = 0;
  if(truthphotons_100.size()>1){
    for(size_t i=0; i<truthphotons_100.size(); i++){
      auto b1 = truthphotons_100.at(i);
      if(b1.Pt()>truthphoton_pt_tlv1){
	truthphoton_pt_tlv1 = b1.Pt();
	truthphotons_100_tlv1 = b1;
	truthphoton_int = i;
      }
    }

    for(size_t i=0; i<truthphotons_100.size(); i++){
      if(i!=truthphoton_int){
	auto b2 = truthphotons_100.at(i);	  
	if(b2.Pt()>truthphoton_pt_tlv2){
	  truthphoton_pt_tlv2 = b2.Pt();
	  truthphotons_100_tlv2 = b2;
	}
      }	
    }
    
  }
  
  if(truthphotons_100.size()>1){
    Float_t truthphoton_phi_tlv1 = truthphotons_100_tlv1.Phi();
    Float_t truthphoton_phi_tlv2 = truthphotons_100_tlv2.Phi();
    if(truthphoton_phi_tlv1<0){
      truthphoton_phi_tlv1 = truthphoton_phi_tlv1 + 2*M_PI;
    }
    if(truthphoton_phi_tlv2<0){
      truthphoton_phi_tlv2 = truthphoton_phi_tlv2 + 2*M_PI;
    }
    if(truthphoton_phi_tlv1>=truthphoton_phi_tlv2){
      m_truthphoton_dphi = truthphoton_phi_tlv1 - truthphoton_phi_tlv2;
    }
    if(truthphoton_phi_tlv1<truthphoton_phi_tlv2){
      m_truthphoton_dphi = truthphoton_phi_tlv2 - truthphoton_phi_tlv1;
    }
    if(m_truthphoton_dphi>M_PI){
      m_truthphoton_dphi = 2*M_PI - m_truthphoton_dphi;
    }

    m_truthphoton_eta_1st = truthphotons_100_tlv1.Eta();
    m_truthphoton_pt_1st = truthphotons_100_tlv1.Pt();
    m_truthphoton_m_1st = truthphotons_100_tlv1.M();
    m_truthphoton_eta_2nd = truthphotons_100_tlv2.Eta();
    m_truthphoton_pt_2nd = truthphotons_100_tlv2.Pt();
    m_truthphoton_m_2nd = truthphotons_100_tlv2.M();
    auto ditruthphoton = truthphotons_100_tlv1 + truthphotons_100_tlv2;
    ditruthphoton_mass = ditruthphoton.M();
    weight_truthphoton = mcEventWeight;
    int truthphoton_nbytes = truthphoton_tree->Fill();
    ATH_MSG_DEBUG("Number of truthphoton_Data bytes written = " << truthphoton_nbytes);
  }

  
  //sjet 
  std::vector<TLorentzVector> truthsjets_50;
  for(size_t i=0; i<truthsjets.size(); i++){
    auto s1 = truthsjets.at(i);
    if(s1.Pt()<20) continue;
    //if(l1.M()>121) continue;
    //if(l1.M()<61) continue;
    truthsjets_50.push_back(s1);
    }
  
  //2l+2q
  
  Float_t truthsjet_pt_tlv1 = 0;
  Float_t truthsjet_pt_tlv2 = 0;
  TLorentzVector truthsjets_50_tlv1;
  TLorentzVector truthsjets_50_tlv2;
  TLorentzVector two_truthsjets_50;
  size_t truthsjet_int = 0;
  if(truthsjets_50.size()>1){
    for(size_t i=0; i<truthsjets_50.size(); i++){
      auto l1 = truthsjets_50.at(i);
      if(l1.Pt()>truthsjet_pt_tlv1){
	truthsjet_pt_tlv1 = l1.Pt();
	truthsjets_50_tlv1 = l1;
	truthsjet_int = i;
      }
    }

    for(size_t i=0; i<truthsjets_50.size(); i++){
      if(i!=truthsjet_int){
	auto l2 = truthsjets_50.at(i);	  
	if(l2.Pt()>truthsjet_pt_tlv2){
	  truthsjet_pt_tlv2 = l2.Pt();
	  truthsjets_50_tlv2 = l2;
	}
      }	
    }
    two_truthsjets_50 =  truthsjets_50_tlv1 + truthsjets_50_tlv2;
  }


if(truthZbosons_mumu.size()>0){
    if(truthsjets_50.size()>1){
      if(truthZmumu_pt_tlv > truthZee_pt_tlv){
	Float_t truthsjet_phi_tlv = two_truthsjets_50.Phi();
	Float_t truthZmumu_phi_tlv = truthZmumu_tlv.Phi();
	if(truthsjet_phi_tlv<0){
	  truthsjet_phi_tlv = truthsjet_phi_tlv + 2*M_PI;
	}
	if(truthZmumu_phi_tlv<0){
	  truthZmumu_phi_tlv = truthZmumu_phi_tlv + 2*M_PI;
	}
	if(truthsjet_phi_tlv>=truthZmumu_phi_tlv){
	  m_twol_truthsjet_dphi = truthsjet_phi_tlv - truthZmumu_phi_tlv;
	}
	if(truthsjet_phi_tlv<truthZmumu_phi_tlv){
	  m_twol_truthsjet_dphi = truthZmumu_phi_tlv - truthsjet_phi_tlv;
	}
	if(m_twol_truthsjet_dphi>M_PI){
	  m_twol_truthsjet_dphi = 2*M_PI - m_twol_truthsjet_dphi;
	}
	  
	m_twol_truthsjet_first_eta = truthsjets_50_tlv1.Eta();
	m_twol_truthsjet_first_pt = truthsjets_50_tlv1.Pt();
	m_twol_truthsjet_first_m = truthsjets_50_tlv1.M();
	m_twol_truthsjet_second_eta = truthsjets_50_tlv2.Eta();
	m_twol_truthsjet_second_pt = truthsjets_50_tlv2.Pt();
	m_twol_truthsjet_second_m = truthsjets_50_tlv2.M();
	m_twol_truthsjet_Zboson_eta = truthZmumu_tlv.Eta();
	m_twol_truthsjet_Zboson_pt = truthZmumu_tlv.Pt();
	m_twol_truthsjet_Zboson_m = truthZmumu_tlv.M();
	m_twol_truthsjet_m = two_truthsjets_50.M();
	auto twol_truthsjet = two_truthsjets_50 + truthZmumu_tlv;
	twol_truthsjet_mass = twol_truthsjet.M();
	
	weight_twol_truthsjet = mcEventWeight;
        if((two_truthsjets_50.M()<121) && (two_truthsjets_50.M()>61) && (two_truthsjets_50.DeltaR(truthZmumu_tlv) > 1)){
	int twol_truthsjet_nbytes = twol_truthsjet_tree->Fill();
	ATH_MSG_DEBUG("Number of twol_truthsjet_Data bytes written = " << twol_truthsjet_nbytes);
	}
      }
    }
  }

  if(truthZbosons_ee.size()>0){
    if(truthsjets_50.size()>1){
      if(truthZee_pt_tlv > truthZmumu_pt_tlv){
	  
	Float_t truthsjet_phi_tlv = two_truthsjets_50.Phi();
	Float_t truthZee_phi_tlv = truthZee_tlv.Phi();
	if(truthsjet_phi_tlv<0){
	  truthsjet_phi_tlv = truthsjet_phi_tlv + 2*M_PI;
	}
	if(truthZee_phi_tlv<0){
	  truthZee_phi_tlv = truthZee_phi_tlv + 2*M_PI;
	}
	if(truthsjet_phi_tlv>=truthZee_phi_tlv){
	  m_twol_truthsjet_dphi = truthsjet_phi_tlv - truthZee_phi_tlv;
	}
	if(truthsjet_phi_tlv<truthZee_phi_tlv){
	  m_twol_truthsjet_dphi = truthZee_phi_tlv - truthsjet_phi_tlv;
	}
	if(m_twol_truthsjet_dphi>M_PI){
	  m_twol_truthsjet_dphi = 2*M_PI - m_twol_truthsjet_dphi;
	}
	  
	m_twol_truthsjet_first_eta = truthsjets_50_tlv1.Eta();
	m_twol_truthsjet_first_pt = truthsjets_50_tlv1.Pt();
	m_twol_truthsjet_first_m = truthsjets_50_tlv1.M();
	m_twol_truthsjet_second_eta = truthsjets_50_tlv2.Eta();
	m_twol_truthsjet_second_pt = truthsjets_50_tlv2.Pt();
	m_twol_truthsjet_second_m = truthsjets_50_tlv2.M();
	m_twol_truthsjet_Zboson_eta = truthZee_tlv.Eta();
	m_twol_truthsjet_Zboson_pt = truthZee_tlv.Pt();
	m_twol_truthsjet_Zboson_m = truthZee_tlv.M();
	m_twol_truthsjet_m = two_truthsjets_50.M();
	auto twol_truthsjet = two_truthsjets_50 + truthZee_tlv;
	twol_truthsjet_mass = twol_truthsjet.M();

	weight_twol_truthsjet = mcEventWeight;
	if((two_truthsjets_50.M()<121) && (two_truthsjets_50.M()>61) && (two_truthsjets_50.DeltaR(truthZee_tlv) > 1)){
	int twol_truthsjet_nbytes = twol_truthsjet_tree->Fill();
	ATH_MSG_DEBUG("Number of twol_truthsjet_Data bytes written = " << twol_truthsjet_nbytes);
	}
      }
    }
    }

  //4q

  std::vector<TLorentzVector> two_truthsjets;
  for(size_t i=0; i<truthsjets_50.size(); i++){
    for(size_t j=i+1; j<truthsjets_50.size(); j++){
      auto s1 = truthsjets_50.at(i);
      auto s2 = truthsjets_50.at(j);
      auto plus = s1+s2;
      if(plus.Pt()<50) continue;
      if(plus.M()>121) continue;
      if(plus.M()<61) continue;
      two_truthsjets.push_back(plus);
    }
  }
  

  Float_t four_truthsjet_pt_tlv1 = 0;
  Float_t four_truthsjet_pt_tlv2 = 0;
  TLorentzVector four_truthsjets_50_tlv1;
  TLorentzVector four_truthsjets_50_tlv2;
  TLorentzVector four_truthsjets;
  size_t four_truthsjet_int = 0;
  if(two_truthsjets.size()>1){
    for(size_t i=0; i<two_truthsjets.size(); i++){
      auto s1 = two_truthsjets.at(i);
      if(s1.Pt()>four_truthsjet_pt_tlv1){
	four_truthsjet_pt_tlv1 = s1.Pt();
	four_truthsjets_50_tlv1 = s1;
	four_truthsjet_int = i;
      }
    }

    for(size_t i=0; i<two_truthsjets.size(); i++){
      if(i!=four_truthsjet_int){
	auto s2 = two_truthsjets.at(i);	  
	if(s2.Pt()>four_truthsjet_pt_tlv2){
	  four_truthsjet_pt_tlv2 = s2.Pt();
	  four_truthsjets_50_tlv2 = s2;
	}
      }	
    }
  }


  if(two_truthsjets.size()>1){
    
    four_truthsjets = four_truthsjets_50_tlv1 + four_truthsjets_50_tlv2;
    m_two_truthsjet_mass_1st = four_truthsjets_50_tlv1.M();
    m_two_truthsjet_mass_2nd = four_truthsjets_50_tlv2.M();
    m_four_truthsjet_mass = four_truthsjets.M();
    weight_four_truthsjet = mcEventWeight;
    if((four_truthsjets_50_tlv1.DeltaR(four_truthsjets_50_tlv2) > 1)){
      int four_truthsjet_nbytes = four_truthsjet_tree->Fill();
      ATH_MSG_DEBUG("Number of four_truthsjet_Data bytes written = " << four_truthsjet_nbytes);
    }

    }
  */
 /////////////////////////////////////////////////////tree

 //fill two elcron mass and two muon mass
  /*  for (size_t i=0; i<electrons.size(); i++){
   for (size_t j=i+1; j<electrons.size(); j++){
     for(size_t k=0; k<muons.size(); k++){
   for(size_t l=k+1; l<muons.size(); l++){
     auto e1 = electrons.at(i);
     auto e2 = electrons.at(j);
     auto m1 = muons.at(k);
     auto m2 = muons.at(l);
     auto Z1 = e1 + e2;
     auto Z2 = m1 + m2;
     //&& ((m1+m2).Pt() > 100)
     if(((m1+m2).Pt() > 100)){
       m_h_two_Zboson_100_muoncut->Fill((e1 + e2 + m1 + m2).M(),mcEventWeight);
     }
     if(((m1+m2).Pt() > 75)){
       m_h_two_Zboson_75_muoncut->Fill((e1 + e2 + m1 + m2).M(),mcEventWeight);
     }
     if(((m1+m2).Pt() > 50)){
       m_h_two_Zboson_50_muoncut->Fill((e1 + e2 + m1 + m2).M(),mcEventWeight);
     }
     if(((e1+e2).Pt() > 100)){
       m_h_two_Zboson_100_electroncut->Fill((e1 + e2 + m1 + m2).M(),mcEventWeight);
     }
     if(((e1+e2).Pt() > 75)){
       m_h_two_Zboson_75_electroncut->Fill((e1 + e2 + m1 + m2).M(),mcEventWeight);
     }
     if(((e1+e2).Pt() > 50)){
       m_h_two_Zboson_50_electroncut->Fill((e1 + e2 + m1 + m2).M(),mcEventWeight);
     }
     if(((e1+e2).Pt() > 100) && ((m1+m2).Pt() > 100)){
       m_h_two_Zboson_100_allcut->Fill((e1 + e2 + m1 + m2).M(),mcEventWeight);
     }
     if(((e1+e2).Pt() > 75) && ((m1+m2).Pt() > 75)){
       m_h_two_Zboson_75_allcut->Fill((e1 + e2 + m1 + m2).M(),mcEventWeight);
     }
     if(((e1+e2).Pt() > 50) && ((m1+m2).Pt() > 50)){
       m_h_two_Zboson_50_allcut->Fill((e1 + e2 + m1 + m2).M(),mcEventWeight);
     }
       m_4l_plus_nocut->Fill((e1 + e2 + m1 + m2).M(),mcEventWeight);
       m_4l_plus_pt_nocut->Fill((e1 + e2 + m1 + m2).Pt(),mcEventWeight);
       m_4l_plus_eta_nocut->Fill((e1 + e2 + m1 + m2).Eta(),mcEventWeight);
       if((Z1.M()>61) && (Z1.M()<121)){
         m_two_electron_two_muon_plus_electron_masscut->Fill((e1 + e2 + m1 + m2).M(),mcEventWeight);
         m_two_electron_two_muon_plus_pt_electron_masscut->Fill((e1 + e2 + m1 + m2).Pt(),mcEventWeight);
       }
       if((Z2.M()>61) && (Z2.M()<121)){
         m_two_electron_two_muon_plus_muon_masscut->Fill((e1 + e2 + m1 + m2).M(),mcEventWeight);
         m_two_electron_two_muon_plus_pt_muon_masscut->Fill((e1 + e2 + m1 + m2).Pt(),mcEventWeight);
       }
       if((m1.Pt() > 10) && (m2.Pt() > 10)){
         m_two_electron_two_muon_plus_muon_ptcut->Fill((e1 + e2 + m1 + m2).M(),mcEventWeight);
         m_two_electron_two_muon_plus_pt_muon_ptcut->Fill((e1 + e2 + m1 + m2).Pt(),mcEventWeight);
         if((Z2.M()>61) && (Z2.M()<121)){
       m_two_electron_two_muon_plus_muonptmasscut->Fill((e1 + e2 + m1 + m2).M(),mcEventWeight);
       m_two_electron_two_muon_plus_pt_muonptmasscut->Fill((e1 + e2 + m1 + m2).Pt(),mcEventWeight);
       m_h_two_Zboson_pt_muonptmasscut->Fill((e1 + e2 + m1 + m2).Pt(),mcEventWeight);
       m_4l_plus_muonptmasscut->Fill((e1 + e2 + m1 + m2).M(),mcEventWeight);
       m_4l_plus_pt_muonptmasscut->Fill((e1 + e2 + m1 + m2).Pt(),mcEventWeight);
         }
       }
       if((e1.Pt() > 10) && (e2.Pt() > 10)){
         m_two_electron_two_muon_plus_electron_ptcut->Fill((e1 + e2 + m1 + m2).M(),mcEventWeight);
         m_two_electron_two_muon_plus_pt_electron_ptcut->Fill((e1 + e2 + m1 + m2).Pt(),mcEventWeight);
         if((Z1.M()>61) && (Z1.M()<121)){
       m_two_electron_two_muon_plus_electronptmasscut->Fill((e1 + e2 + m1 + m2).M(),mcEventWeight);
       m_two_electron_two_muon_plus_pt_electronptmasscut->Fill((e1 + e2 + m1 + m2).Pt(),mcEventWeight);
       m_h_two_Zboson_pt_electronptmasscut->Fill((e1 + e2 + m1 + m2).Pt(),mcEventWeight);
       m_4l_plus_electronptmasscut->Fill((e1 + e2 + m1 + m1).M(),mcEventWeight);
       m_4l_plus_pt_electronptmasscut->Fill((e1 + e2 + m1 + m2).Pt(),mcEventWeight);
         }
       }
       if((e1.Pt() > 10 ) && (e2.Pt() > 10) && (m1.Pt() > 10) && (m2.Pt() > 10)){
         m_two_electron_two_muon_plus_allptcut->Fill((e1 + e2 + m1 + m2).M(),mcEventWeight);
         m_two_electron_two_muon_plus_pt_allptcut->Fill((e1 + e2 + m1 + m2).Pt(),mcEventWeight);
         m_h_two_Zboson_pt_allptcut->Fill((e1 + e2 + m1 + m2).Pt(),mcEventWeight);
         m_4l_plus_allptcut->Fill((e1 + e2 + m1 + m2).M(),mcEventWeight);
         m_4l_plus_pt_allptcut->Fill((e1 + e2 + m1 + m2).Pt(),mcEventWeight);
         //(Z1.M()>60) && (Z1.M()<120) &&   (e1.Pt() > 10 ) && (e2.Pt() > 10) &&
         if((Z1.M()>61) && (Z1.M()<121) && (Z2.M()>61) && (Z2.M()<121)){
       m_h_two_Zboson_pt->Fill((e1 + e2 + m1 + m2).Pt(),mcEventWeight);
       m_h_two_Zboson_mass->Fill((e1 + e2 + m1 + m2).M(),mcEventWeight);
       m_two_electron_two_muon_plus->Fill((e1 + e2 + m1 + m2).M(),mcEventWeight);
       m_two_electron_two_muon_plus_pt->Fill((e1 + e2 + m1 + m2).Pt(),mcEventWeight);
       m_4l_plus->Fill((e1 + e2 + m1 + m2).M(),mcEventWeight);
       m_4l_plus_pt->Fill((e1 + e2 + m1 + m2).Pt(),mcEventWeight);
         }
       }
     }
   }
     }
   }
  */

 //4l plus case
 /*
 //Jet and  Calibration
 const xAOD::JetContainer* jetContainer = nullptr;
 std::string jetCollectionName = "AntiKt4EMPFlowJets";
 ATH_CHECK(evtStore->retrieve(jetContainer, jetCollectionName));
 for(auto *ijet: *jetContainer){
   xAOD::Jet *jet = 0;
   m_jetCalibration->calibratedCopy(*ijet, jet);
   if(jet->pt() < 20000.) cintinue;
   delete jet;
   }*/
 //jet
 /*
 const xAOD::JetContainer* jetContainer = nullptr;
 ATH_CHECK(evtStore()->retrieve(jetContainer, "AntiKt4EMPFlowJets"));
 ATH_MSG_DEBUG("jets successfully retrieved");
 ATH_MSG_DEBUG("# of jets = " << jetContainer->size());
 std::vector<TLorentzVector> jets;  //important
 unsigned int nJets = 0;
 for (const xAOD::Jet* ijt : *jetContainer) {
   xAOD::Jet* jt = 0;
   m_jetCalitool->calibratedCopy(*ijt,jt);
   ATH_MSG_INFO(nJets << " th jet :" <<
        " Pt : " << jt->pt() <<
        " Eta : " << jt->eta() <<
        " Phi : " << jt->phi() <<
        " E : " << jt->e() <<
        " M : " << jt->m()
        );
   m_h_jet_e->Fill(jt->e()/1000.); // Energy in GeV
   m_h_jet_pt->Fill(jt->pt()/1000.); // Transverse momentum in GeV
   m_h_jet_eta->Fill(jt->eta()); // Pseudorapidity
   m_h_jet_phi->Fill(jt->phi()); // Azimuthal angle
   m_h2_jet_eta_phi->Fill(jt->eta(), jt->phi()); // 2D distribution, X:eta, Y:phi

   ++nJets;

   TLorentzVector jet_tlv;
   jet_tlv.SetPtEtaPhiM(jt->pt()/1000, jt->eta(), jt->phi(), jt->m()/1000.);
   jets.push_back(jet_tlv);
   if(jt->pt() < 20000.) continue;
   delete jt;
 }
 for (size_t i=0; i<jets.size(); i++){
   auto j1 = jets.at(i);
   m_jet_mass->Fill(j1.M());
 }
 
 for (size_t i=0; i<jets.size(); i++){
   for (size_t j=i+1; j<jets.size(); j++){
     auto j1 = jets.at(i);
     auto j2 = jets.at(j);
     m_two_jet_plus->Fill((j1 + j2).M());
   }
   }*/


  //////////////////////////TruthParticle
/////////////////////////TruthParticle/////////////////////////////
 ///////////////////////////////////////////////////////////////////
 ////////////////truthMuons///////////////
 /* const xAOD::TruthParticleContainer* truthMuonContainer = nullptr;
  ATH_CHECK(evtStore()->retrieve(truthMuonContainer, "TruthMuons"));
  ATH_MSG_DEBUG("truthMuons successfully retrieved");
  ATH_MSG_DEBUG("# of truthmuons = " << truthMuonContainer->size());
  
  unsigned int ntruthMuons = 0;
  std::vector<TLorentzVector> truthmuons;  //important
  for (const xAOD::TruthParticle* muon : *truthMuonContainer) {
    ATH_MSG_INFO(ntruthMuons << "th muon :" <<
		 " Pt : " << muon->pt() <<
		 " Eta : " << muon->eta() <<
		 " Phi : " << muon->phi() <<
		 " E : " << muon->e() <<
		 " M : " << muon->m() <<
		 " Parent Type : " << muon->auxdata<unsigned int>("classifierParticleOrigin")
		 );
    m_h_truthmuon_e->Fill(muon->e()/1000.); // Energy in GeV
    m_h_truthmuon_pt->Fill(muon->pt()/1000.); // Transverse momentum in GeV
    m_h_truthmuon_eta->Fill(muon->eta()); // Pseudorapidity
    m_h_truthmuon_phi->Fill(muon->phi()); // Azimuthal angle
    m_h2_truthmuon_eta_phi->Fill(muon->eta(), muon->phi()); // 2D distribution, X:eta, Y:phi
    
    ++ntruthMuons;
    
    TLorentzVector truthmuon_tlv;
    truthmuon_tlv.SetPtEtaPhiM(muon->pt()/1000, muon->eta(), muon->phi(), muon->m()/1000.);
    truthmuons.push_back(truthmuon_tlv);
  
  }
  
  //////////////////////////TruthElectrons/////////////////////////
  const xAOD::TruthParticleContainer* truthElectronContainer = nullptr;
  ATH_CHECK(evtStore()->retrieve(truthElectronContainer, "TruthElectrons"));
  ATH_MSG_DEBUG("truthElectrons successfully retrieved");
  ATH_MSG_DEBUG("# of truthelectrons = " << truthElectronContainer->size());
  
  unsigned int ntruthElectrons = 0;
  std::vector<TLorentzVector> truthelectrons;  //important
  for (const xAOD::TruthParticle* electron : *truthElectronContainer) {
    m_h_truthelectron_e->Fill(electron->e()/1000.); // Energy in GeV
    m_h_truthelectron_pt->Fill(electron->pt()/1000.); // Transverse momentum in GeV
    m_h_truthelectron_eta->Fill(electron->eta()); // Pseudorapidity
    m_h_truthelectron_phi->Fill(electron->phi()); // Azimuthal angle
    m_h2_truthelectron_eta_phi->Fill(electron->eta(), electron->phi()); // 2D distribution, X:eta, Y:phi
    
    ++ntruthElectrons;
    
    TLorentzVector truthelectron_tlv;
    truthelectron_tlv.SetPtEtaPhiM(electron->pt()/1000, electron->eta(), electron->phi(), electron->m()/1000.);
    truthelectrons.push_back(truthelectron_tlv);
  }
  //////////////////////////TruthNeutrinos/////////////////////////
  const xAOD::TruthParticleContainer* truthNeutrinoContainer = nullptr;
  ATH_CHECK(evtStore()->retrieve(truthNeutrinoContainer, "TruthNeutrinos"));
  ATH_MSG_DEBUG("truthNeutrinos successfully retrieved");
  ATH_MSG_DEBUG("# of truthneutrinos = " << truthNeutrinoContainer->size());
  
  unsigned int ntruthNeutrinos = 0;
  std::vector<TLorentzVector> truthneutrinos;  //important
  for (const xAOD::TruthParticle* neutrino : *truthNeutrinoContainer) {
    m_h_truthneutrino_e->Fill(neutrino->e()/1000.); // Energy in GeV
    m_h_truthneutrino_pt->Fill(neutrino->pt()/1000.); // Transverse momentum in GeV
    m_h_truthneutrino_eta->Fill(neutrino->eta()); // Pseudorapidity
    m_h_truthneutrino_phi->Fill(neutrino->phi()); // Azimuthal angle
    m_h2_truthneutrino_eta_phi->Fill(neutrino->eta(), neutrino->phi()); // 2D distribution, X:eta, Y:phi
    
    ++ntruthNeutrinos;
    
    TLorentzVector truthneutrino_tlv;
    truthneutrino_tlv.SetPtEtaPhiM(neutrino->pt()/1000, neutrino->eta(), neutrino->phi(), neutrino->m()/1000.);
    truthneutrinos.push_back(truthneutrino_tlv);
 }
  //////////////////////////TruthPhotons//////////////////////////
  const xAOD::TruthParticleContainer* truthPhotonContainer = nullptr;
  ATH_CHECK(evtStore()->retrieve(truthPhotonContainer, "TruthPhotons"));
  ATH_MSG_DEBUG("truthPhotons successfully retrieved");
  ATH_MSG_DEBUG("# of truthphotons = " << truthPhotonContainer->size());
  
  unsigned int ntruthPhotons = 0;
  std::vector<TLorentzVector> truthphotons;  //important
  for (const xAOD::TruthParticle* photon : *truthPhotonContainer) {
    m_h_truthphoton_e->Fill(photon->e()/1000.); // Energy in GeV
    m_h_truthphoton_pt->Fill(photon->pt()/1000.); // Transverse momentum in GeV
    m_h_truthphoton_eta->Fill(photon->eta()); // Pseudorapidity
    m_h_truthphoton_phi->Fill(photon->phi()); // Azimuthal angle
    m_h2_truthphoton_eta_phi->Fill(photon->eta(), photon->phi()); // 2D distribution, X:eta, Y:phi
    
    ++ntruthPhotons;
    
    TLorentzVector truthphoton_tlv;
   truthphoton_tlv.SetPtEtaPhiM(photon->pt()/1000, photon->eta(), photon->phi(), photon->m()/1000.);
   truthphotons.push_back(truthphoton_tlv);
  }
  for (size_t i=0; i<truthphotons.size(); i++){
    for (size_t j=i+1; j<truthphotons.size(); j++){
      auto p1 = truthphotons.at(i);
      auto p2 = truthphotons.at(j);
      m_h_truthHiggs->Fill((p1 + p2).M());
   }
  }
  
  //////////////////////////TruthTaus////////////////////////////
  const xAOD::TruthParticleContainer* truthTauContainer = nullptr;
  ATH_CHECK(evtStore()->retrieve(truthTauContainer, "TruthTaus"));
  ATH_MSG_DEBUG("truthTaus successfully retrieved");
  ATH_MSG_DEBUG("# of truthtaus = " << truthTauContainer->size());
  
  unsigned int ntruthTaus = 0;
  std::vector<TLorentzVector> truthtaus;  //important
  for (const xAOD::TruthParticle* tau : *truthTauContainer) {
    m_h_truthtau_e->Fill(tau->e()/1000.); // Energy in GeV
    m_h_truthtau_pt->Fill(tau->pt()/1000.); // Transverse momentum in GeV
    m_h_truthtau_eta->Fill(tau->eta()); // Pseudorapidity
    m_h_truthtau_phi->Fill(tau->phi()); // Azimuthal angle
    m_h2_truthtau_eta_phi->Fill(tau->eta(), tau->phi()); // 2D distribution, X:eta, Y:phi
    
    ++ntruthTaus;
    
    TLorentzVector truthtau_tlv;
   truthtau_tlv.SetPtEtaPhiM(tau->pt()/1000, tau->eta(), tau->phi(), tau->m()/1000.);
   truthtaus.push_back(truthtau_tlv);
 }
 ///////////////TruthAntiKt10TruthTrimmedPtFrac5SmallR20Jets////////////
 const xAOD::JetContainer* truthLjetContainer = nullptr;
 ATH_CHECK(evtStore()->retrieve(truthLjetContainer, "AntiKt10TruthTrimmedPtFrac5SmallR20Jets"));
 ATH_MSG_DEBUG("truthLjets successfully retrieved");
 ATH_MSG_DEBUG("# of truthljets = " << truthLjetContainer->size());

 unsigned int ntruthLjets = 0;
 std::vector<TLorentzVector> truthljets;  //important
 for (const xAOD::Jet* ljet : *truthLjetContainer) {
   m_h_truthljet_e->Fill(ljet->e()/1000.); // Energy in GeV
   m_h_truthljet_pt->Fill(ljet->pt()/1000.); // Transverse momentum in GeV
   m_h_truthljet_eta->Fill(ljet->eta()); // Pseudorapidity
   m_h_truthljet_phi->Fill(ljet->phi()); // Azimuthal angle
   m_h2_truthljet_eta_phi->Fill(ljet->eta(), ljet->phi()); // 2D distribution, X:eta, Y:phi

   ++ntruthLjets;
   
   TLorentzVector truthljet_tlv;
   truthljet_tlv.SetPtEtaPhiM(ljet->pt()/1000, ljet->eta(), ljet->phi(), ljet->m()/1000.);
   truthljets.push_back(truthljet_tlv);
   }
 ////////////////////////TruthAntiKt4TruthJets&b-jet//////////////////////////
 const xAOD::JetContainer* truthSjetContainer = nullptr;
 ATH_CHECK(evtStore()->retrieve(truthSjetContainer, "AntiKt4TruthDressedWZJets"));
 ATH_MSG_DEBUG("truthSjets successfully retrieved");
 ATH_MSG_DEBUG("# of truthsjets = " << truthSjetContainer->size());

 unsigned int ntruthSjets = 0;
 
 std::vector<TLorentzVector> truthsjets;  //important
 std::vector<TLorentzVector> truthbjets;
 for (const xAOD::Jet* sjet : *truthSjetContainer) {
   m_h_truthsjet_e->Fill(sjet->e()/1000.); // Energy in GeV
   m_h_truthsjet_pt->Fill(sjet->pt()/1000.); // Transverse momentum in GeV
   m_h_truthsjet_eta->Fill(sjet->eta()); // Pseudorapidity
   m_h_truthsjet_phi->Fill(sjet->phi()); // Azimuthal angle
   m_h2_truthsjet_eta_phi->Fill(sjet->eta(), sjet->phi()); // 2D distribution, X:eta, Y:phi

   ++ntruthSjets;
   
   TLorentzVector truthsjet_tlv;
   truthsjet_tlv.SetPtEtaPhiM(sjet->pt()/1000, sjet->eta(), sjet->phi(), sjet->m()/1000.);
   truthsjets.push_back(truthsjet_tlv);
   bool isBjet = false;
   int truthlabel(-1);
   sjet->getAttribute("HadronConeExclTruthLabelID", truthlabel);
   isBjet = std::abs(truthlabel) == 5;
   if (isBjet){
     m_h_truthbjet_e->Fill(sjet->e()/1000.); // Energy in GeV
     m_h_truthbjet_pt->Fill(sjet->pt()/1000.); // Transverse momentum in GeV
     m_h_truthbjet_eta->Fill(sjet->eta()); // Pseudorapidity
     m_h_truthbjet_phi->Fill(sjet->phi()); // Azimuthal angle
     m_h2_truthbjet_eta_phi->Fill(sjet->eta(), sjet->phi()); // 2D distribution, X:eta, Y:phi
     truthbjets.push_back(truthsjet_tlv);
   }
 }
 //////////////////////b-jet//////////////////////////
 for(size_t i=0; i<truthbjets.size(); i++){
   auto b1 =truthbjets.at(i);
   m_one_bjet->Fill(b1.M());
   }
 for (size_t i=0; i<truthbjets.size(); i++){
   for (size_t j=i+1; j<truthbjets.size(); j++){
     auto b1 = truthbjets.at(i);
     auto b2 = truthbjets.at(j);
     m_two_bjet_plus->Fill((b1 + b2).M());
   }
 }
 //////////////////////////MET_Truth////////////////////////////
 
 //////////////////////////Zboson////////////////////////////////
 std::vector<TLorentzVector> truthZbosons;
 std::vector<TLorentzVector> truthZbosons_nocut;
 std::vector<TLorentzVector> truthZbosons_ptcut;
 std::vector<TLorentzVector> truthZbosons_etacut;
 
 for (size_t i=0; i<truthmuons.size(); i++){
   for (size_t j=i+1; j<truthmuons.size(); j++){
     auto m1 = truthmuons.at(i);
     auto m2 = truthmuons.at(j);
     TLorentzVector Zboson_tlv_nocut;
     Zboson_tlv_nocut = m1 + m2;
     truthZbosons_nocut.push_back(Zboson_tlv_nocut);
     
     if(m1.Pt() < 20) continue;
     if(m2.Pt() < 20) continue;
     TLorentzVector Zboson_tlv_ptcut;
     Zboson_tlv_ptcut = m1 + m2;
     truthZbosons_ptcut.push_back(Zboson_tlv_ptcut);

     if(std::abs(m1.Eta()) > 2.5) continue;
     if(std::abs(m2.Eta()) > 2.5) continue;
     TLorentzVector Zboson_tlv_etacut;
     Zboson_tlv_etacut = m1 + m2;
     truthZbosons_etacut.push_back(Zboson_tlv_etacut);

     if((m1+m2).M() > 121) continue;
     if((m1+m2).M() < 61) continue;
     TLorentzVector Zboson_tlv;
     Zboson_tlv = m1 + m2;
     truthZbosons.push_back(Zboson_tlv);
   }
 }
 
 int event = 1;

 if(truthZbosons_nocut.size()>0){
   m_h_Zmumu_event_nocut->Fill(event,mcEventWeight);
 }
 
 if(truthZbosons_ptcut.size()>0){
   m_h_Zmumu_event_ptcut->Fill(event,mcEventWeight);
 }

 if(truthZbosons_etacut.size()>0){
   m_h_Zmumu_event_etacut->Fill(event,mcEventWeight);
 }
 
 if(truthZbosons.size()>0){
   m_h_Zmumu_event->Fill(event,mcEventWeight);
 }


 
 //////////////////////Wboson//////////////////////
 std::vector<TLorentzVector> truthWbosons;
 for (size_t i=0; i<truthneutrinos.size(); i++){
   for (size_t j=0; j<truthelectrons.size(); j++){
     auto n1 = truthneutrinos.at(i);
     auto e1 = truthelectrons.at(j);
     if((n1.Pt() > 20 )&& (e1.Pt() > 10)){
       m_h_truthWboson->Fill((n1 + e1).M());
       m_h_truthWboson_pt->Fill((n1 + e1).Pt());
       TLorentzVector truthWboson_tlv;
       truthWboson_tlv = n1 + e1;
       truthWbosons.push_back(truthWboson_tlv);
     }
   }
 }
 for (size_t i=0; i<truthneutrinos.size(); i++){
   for (size_t j=0; j<truthmuons.size(); j++){
     auto n1 = truthneutrinos.at(i);
     auto m1 = truthmuons.at(j);
     if((n1.Pt() > 20 )&& (m1.Pt() > 10)){
       m_h_truthWboson->Fill((n1 + m1).M());
       m_h_truthWboson_pt->Fill((n1 + m1).Pt());
       TLorentzVector truthWboson_tlv;
       truthWboson_tlv = n1 + m1;
       truthWbosons.push_back(truthWboson_tlv);
     }
   }
 }
 for (size_t i=0; i<truthneutrinos.size(); i++){
   for (size_t j=0; j<truthtaus.size(); j++){
     auto n1 = truthneutrinos.at(i);
     auto t1 = truthtaus.at(j);
     if((n1.Pt() > 20 )&& (t1.Pt() > 20)){
       m_h_truthWboson->Fill((n1 + t1).M());
       m_h_truthWboson_pt->Fill((n1 + t1).Pt());
       TLorentzVector truthWboson_tlv;
       truthWboson_tlv = n1 + t1;
       truthWbosons.push_back(truthWboson_tlv);
     }
   }
 }
 ////////////////////tquark/////////////////////////
 std::vector<TLorentzVector> truthtquarks;
 for (size_t i=0; i<truthWbosons.size(); i++){
   for (size_t j=0; j<truthbjets.size(); j++){
     auto w1 = truthWbosons.at(i);
     auto b1 = truthbjets.at(j);
     m_h_truthTquark->Fill((w1 + b1).M());
     TLorentzVector truthtquark_tlv;
     truthtquark_tlv = w1 + b1;
     truthtquarks.push_back(truthtquark_tlv);
   }
 }
 //////////////////ljet//////////////////////

 for (size_t i=0; i<truthljets.size(); i++){
   auto j1 = truthljets.at(i);
   m_one_ljet->Fill(j1.M());
 }
 for (size_t i=0; i<truthljets.size(); i++){
   for (size_t j=i+1; j<truthljets.size(); j++){
     auto j1 = truthljets.at(i);
     auto j2 = truthljets.at(j);
     m_two_ljet_plus->Fill((j1 + j2).M());
   }
 }
 /////////////////////Higgs//////////////////////////
 
 for (size_t i=0; i<truthZbosons.size(); i++){
   for (size_t j=i+1; j<truthZbosons.size(); j++){
     auto z1 = truthZbosons.at(i);
     auto z2 = truthZbosons.at(j);
     m_h_truthHiggs->Fill((z1 + z2).M(),mcEventWeight);
     m_h_truthHiggs_pt->Fill((z1 + z2).Pt(),mcEventWeight);
   }
 }
 for (size_t i=0; i<truthphotons.size(); i++){
   for (size_t j=i+1; j<truthphotons.size(); j++){
     auto p1 = truthphotons.at(i);
     auto p2 = truthphotons.at(j);
     m_h_truthHiggs->Fill((p1 + p2).M(),mcEventWeight);
     m_h_truthHiggs_pt->Fill((p1 + p2).Pt(),mcEventWeight);
   }
 }
 for (size_t i=0; i<truthWbosons.size(); i++){
   for (size_t j=i+1; j<truthWbosons.size(); j++){
     auto w1 = truthWbosons.at(i);
     auto w2 = truthWbosons.at(j);
     m_h_truthHiggs->Fill((w1 + w2).M(),mcEventWeight);
     m_h_truthHiggs_pt->Fill((w1 + w2).Pt(),mcEventWeight);
   }
 }
 for (size_t i=0; i< truthtquarks.size(); i++){
   for (size_t j=i+1; j< truthtquarks.size(); j++){
     auto t1 =  truthtquarks.at(i);
     auto t2 =  truthtquarks.at(j);
     if((t1.Pt() > 20 )&& (t2.Pt() > 20)){
       m_h_two_truthTquark->Fill((t1 + t2).M(),mcEventWeight);
       m_h_two_truthTquark_pt->Fill((t1 + t2).Pt(),mcEventWeight);
       m_h_truthHiggs->Fill((t1 + t2).M(),mcEventWeight);
       m_h_truthHiggs_pt->Fill((t1 + t2).Pt(),mcEventWeight);
     }
   }
 }
 for (size_t i=0; i<truthbjets.size(); i++){
   for (size_t j=i+1; j<truthbjets.size(); j++){
     auto b1 = truthbjets.at(i);
     auto b2 = truthbjets.at(j);
     m_h_truthHiggs->Fill((b1 + b2).M());
     m_h_truthHiggs_pt->Fill((b1 + b2).Pt(),mcEventWeight);
   }
 }
  
 //seperate truthhiggs
 for (size_t i=0; i<truthmuons.size(); i++){
   for (size_t j=i+1; j<truthmuons.size(); j++){
     for(size_t k=j+1; k<truthmuons.size(); k++){
       for(size_t l=k+1; l<truthmuons.size(); l++){
	 auto m1 = truthmuons.at(i);
	 auto m2 = truthmuons.at(j);
	 auto m3 = truthmuons.at(k);
	 auto m4 = truthmuons.at(l);
	 if((m1.Pt() >10) && (m2.Pt() > 10) && (m3.Pt() > 10) && (m4.Pt() > 10)){
	   // if(((m1+m2).M() > 61) && ((m1+m2).M() < 121) && ((m3+m4).M() > 61) && ((m3+m4).M() < 121)){
	   m_four_truthmuon_plus->Fill((m1 + m2 + m3 + m4).M(),mcEventWeight);
	   m_four_truthmuon_plus_pt->Fill((m1 + m2 + m3 + m4).Pt(),mcEventWeight);
	   //}
	 }
       }
     }
   }
 }

 for (size_t i=0; i<truthelectrons.size(); i++){
   for (size_t j=i+1; j<truthelectrons.size(); j++){
     for(size_t k=0; k<truthmuons.size(); k++){
       for(size_t l=k+1; l<truthmuons.size(); l++){
	 auto e1 = truthelectrons.at(i);
	 auto e2 = truthelectrons.at(j);
	 auto m1 = truthmuons.at(k);
	 auto m2 = truthmuons.at(l);
	 if( (m1.Pt() > 10) && (m2.Pt() > 10) && (e1.Pt() > 10) && (e2.Pt() > 10)){
	   auto Z1 = e1 + e2;
	   auto Z2 = m1 + m2;
	   //(Z2.M()>60) && (Z2.M()<120) &&     &&(e1.Pt() > 10) && (e2.Pt() > 10)
	   //if((Z2.M()>60) && (Z2.M()<120) && (Z1.M()>60) && (Z1.M()<120)){
	   m_two_truthelectron_two_truthmuon_plus->Fill((e1 + e2 + m1 + m2).M(),mcEventWeight);
	   m_two_truthelectron_two_truthmuon_plus_pt->Fill((e1 + e2 + m1 + m2).Pt(),mcEventWeight);
	   m_h_two_truthZboson_pt->Fill((e1 + e2 + m1 + m2).Pt(),mcEventWeight);
	   m_h_two_truthZboson_mass->Fill((e1 + e2 + m1 + m2).M(),mcEventWeight);
	   // }
	 }
       }
     }
   }
 }
 for (size_t i=0; i<truthtaus.size(); i++){
   for (size_t j=i+1; j<truthtaus.size(); j++){
     for(size_t k=j+1; k<truthtaus.size(); k++){
       for(size_t l=k+1; l<truthtaus.size(); l++){
	 auto t1 = truthtaus.at(i);
	 auto t2 = truthtaus.at(j);
	 auto t3 = truthtaus.at(k);
	 auto t4 = truthtaus.at(l);
	 m_four_truthtau_plus->Fill((t1 + t2 + t3 + t4).M(),mcEventWeight);
       }
     }
   }
   }*/
 
 ////////////////////////////////////////////////////////////////////////////////////////

   
 return StatusCode::SUCCESS;
}


