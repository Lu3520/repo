// -*- C++ -*-

#ifndef MYANALYSISALG_H // Include guard
#define MYANALYSISALG_H

//#include "AthenaBaseComps/AthAlgorithm.h" "old one"
#include "AthAnalysisBaseComps/AthAnalysisAlgorithm.h"
#include "MuonAnalysisInterfaces/IMuonSelectionTool.h"
#include "ElectronPhotonSelectorTools/AsgElectronLikelihoodTool.h"
#include "EgammaAnalysisInterfaces/IAsgElectronIsEMSelector.h"
#include "EgammaAnalysisInterfaces/IAsgPhotonIsEMSelector.h"
#include "AsgTools/AnaToolHandle.h"
#include "JetCalibTools/IJetCalibrationTool.h"
#include "JetCalibTools/JetCalibrationTool.h"
#include "FTagAnalysisInterfaces/IBTaggingSelectionTool.h"
#include "TauAnalysisTools/ITauSelectionTool.h"
#include "JetInterface/IJetUpdateJvt.h"

class ITHistSvc;
class StoreGateSvc;
class TH1D;
class TH2D;
class TTree;

class MyAnalysisAlg:public AthAnalysisAlgorithm
{
 public:
  MyAnalysisAlg(const std::string& name, ISvcLocator* pSvcLocator); // Constructor

  virtual StatusCode initialize() override;
  virtual StatusCode beginInputFile() override;
  virtual StatusCode finalize() override;
  virtual StatusCode execute() override;

 private:
  std::string m_message;
  ServiceHandle<ITHistSvc> m_thistSvc;
  ServiceHandle<StoreGateSvc> inputMetaStore;
  ToolHandle< CP::IMuonSelectionTool > m_selTool; //MuonSelector tool
  ToolHandle< AsgElectronLikelihoodTool > eg_selTool; //now still no function
  SG::AuxElement::ConstAccessor<char> m_acc_eleId; //still don't know the function
  ToolHandle< IAsgPhotonIsEMSelector > ph_selTool;
  SG::AuxElement::Accessor<char> tight_acc; //photon selection
  ToolHandle< IJetCalibrationTool > m_jetCalitool;
  ToolHandle< IBTaggingSelectionTool > btag_selTool;
  ToolHandle< TauAnalysisTools::ITauSelectionTool > t_selTool;
  asg::AnaToolHandle<IJetUpdateJvt> h_updatejvt;
  //JetCalibrationTool *m_jetCalibration;
  //asg :: AnaToolHandle<IJetCalibrationTool> *m_jetCalibration;
  //ToolHandle<IAsgElectronIsEMSelector> eg_selTool; //ElectronSelector tool
  double sumOfWeight_tlv;
  double sumOfWeights;
  double sumOfWeights_tlv;
  double weight_Zmumumet = 0;
  double m_sumOfWeights;
  double mcEventWeight;

  //h2
  TH2D* h2_Zboson_HT_pt;

  TH1D* m_h_two_Zboson_pt;
  TH1D* m_h_two_Zboson_mass;
  TH1D* m_h_two_truthZboson_pt;
  TH1D* m_h_two_truthZboson_mass;
  //higgs
  TH1D* m_h_Zboson;
  TH1D* m_h_Zboson_pt;
  TH1D* m_h_Zboson_one_pt;
  TH1D* m_h_Wboson;
  TH1D* m_h_Wboson_pt;
  TH1D* m_h_Tquark;
  TH1D* m_h_Higgs;
  TH1D* m_h_Higgs_pt;
  TH1D* m_h_two_Zboson;
  TH1D* m_h_two_Wboson;
  TH1D* m_h_two_Tquark;
  TH1D* m_h_two_Tquark_pt;
  TH1D* m_h_two_Bquark;
  TH1D* m_h_two_photon;
  //muon
  TH1D* m_h_nMuons;
  TH1D* m_h_muon_e;
  TH1D* m_h_muon_pt;
  TH1D* m_h_muon_eta;
  TH1D* m_h_muon_phi;
  TH2D* m_h2_muon_eta_phi;
  TH1D* m_two_muon_plus;
  TH1D* m_two_muon_plus_pt;
  TH1D* m_four_muon_plus;
  TH1D* m_two_muon_ns_plus;
  TH1D* m_four_muon_ns_plus;
  TH1D* m_four_muon_plus_pt;
  ////////////////////////////Truthparticle/////////////////////////
  //////////////////////////////////////////////////////////////////
  TH1D* m_h_truthZboson;
  TH1D* m_h_truthZboson_pt;
  TH1D* m_h_truthZboson_one_pt;
  TH1D* m_h_truthWboson;
  TH1D* m_h_truthWboson_pt;
  TH1D* m_h_truthTquark;
  TH1D* m_h_two_truthTquark;
  TH1D* m_h_two_truthTquark_pt;
  TH1D* m_h_truthHiggs;
  TH1D* m_h_truthHiggs_pt;
  TH1D* m_two_truthelectron_two_truthmuon_plus;
  TH1D* m_two_truthelectron_two_truthmuon_plus_pt;
  TH1D* m_four_truthelectron_plus;
  TH1D* m_four_truthelectron_plus_pt;
  TH1D* m_four_truthmuon_plus;
  TH1D* m_four_truthmuon_plus_pt;
  TH1D* m_four_truthtau_plus;
  /////////////////////////truthmuon///////////////////////////////
  TH1D* m_h_ntruthMuons;
  TH1D* m_h_truthmuon_e;
  TH1D* m_h_truthmuon_pt; 
  TH1D* m_h_truthmuon_eta;
  TH1D* m_h_truthmuon_phi;
  TH2D* m_h2_truthmuon_eta_phi;
  TH1D* m_two_truthmuon_plus;
  TH1D* m_two_truthmuon_plus_pt;
  ////////////////////////truthelectron/////////////////////////
  TH1D* m_h_ntruthElectrons;
  TH1D* m_h_truthelectron_e;
  TH1D* m_h_truthelectron_pt; 
  TH1D* m_h_truthelectron_eta;
  TH1D* m_h_truthelectron_phi;
  TH2D* m_h2_truthelectron_eta_phi;
  TH1D* m_two_truthelectron_plus;
  TH1D* m_two_truthelectron_plus_pt;
  /////////////////////truthneutrino////////////////////////
  TH1D* m_h_ntruthNeutrinos;
  TH1D* m_h_truthneutrino_e;
  TH1D* m_h_truthneutrino_pt; 
  TH1D* m_h_truthneutrino_eta;
  TH1D* m_h_truthneutrino_phi;
  TH2D* m_h2_truthneutrino_eta_phi;
  /////////////////////truthphoton//////////////////////////
  TH1D* m_h_ntruthPhotons;
  TH1D* m_h_truthphoton_e;
  TH1D* m_h_truthphoton_pt; 
  TH1D* m_h_truthphoton_eta;
  TH1D* m_h_truthphoton_phi;
  TH2D* m_h2_truthphoton_eta_phi;
  /////////////////////truthtau////////////////////////////
  TH1D* m_h_ntruthTaus;
  TH1D* m_h_truthtau_e;
  TH1D* m_h_truthtau_pt; 
  TH1D* m_h_truthtau_eta;
  TH1D* m_h_truthtau_phi;
  TH2D* m_h2_truthtau_eta_phi;
  /////////////////////truthsjet/////////////////////////
  TH1D* m_h_ntruthSjets;
  TH1D* m_h_truthsjet_e;
  TH1D* m_h_truthsjet_pt; 
  TH1D* m_h_truthsjet_eta;
  TH1D* m_h_truthsjet_phi;
  TH2D* m_h2_truthsjet_eta_phi;
  TH1D* m_two_truthtau_plus;
  TH1D* m_two_truthtau_plus_pt;
  /////////////////////truthljet////////////////////////
  TH1D* m_h_ntruthLjets;
  TH1D* m_h_truthljet_e;
  TH1D* m_h_truthljet_pt; 
  TH1D* m_h_truthljet_eta;
  TH1D* m_h_truthljet_phi;
  TH2D* m_h2_truthljet_eta_phi;
  TH1D* m_one_ljet;
  TH1D* m_two_ljet_plus;
  /////////////////truthbjet/////////////////////////
  TH1D* m_h_ntruthBjets;
  TH1D* m_h_truthbjet_e;
  TH1D* m_h_truthbjet_pt; 
  TH1D* m_h_truthbjet_eta;
  TH1D* m_h_truthbjet_phi;
  TH2D* m_h2_truthbjet_eta_phi;
  TH1D* m_one_bjet;
  TH1D* m_two_bjet_plus;
  /////////////////////////////////////////////////////
  //electron
  TH1D* m_h_nElectrons;
  TH1D* m_h_electron_e;
  TH1D* m_h_electron_pt;
  TH1D* m_h_electron_eta;
  TH1D* m_h_electron_phi;
  TH2D* m_h2_electron_eta_phi;
  TH1D* m_two_electron_plus;
  TH1D* m_two_electron_ns_plus;
  TH1D* m_two_electron_plus_pt;

  //Total 4e + 4mu +(2e+2mu)
  TH1D* m_4l_plus;
  TH1D* m_4l_plus_pt;
  //tau
  TH1D* m_h_nTaus;
  TH1D* m_h_tau_e;
  TH1D* m_h_tau_pt;
  TH1D* m_h_tau_eta;
  TH1D* m_h_tau_phi;
  TH2D* m_h2_tau_eta_phi;
  TH1D* m_two_tau_plus;
  TH1D* m_four_tau_plus;
  TH1D* m_two_tau_ns_plus;
  TH1D* m_four_tau_ns_plus;
  TH1D* m_two_tau_plus_pt;
  //photon
  TH1D* m_h_nPhotons;
  TH1D* m_h_photon_e;
  TH1D* m_h_photon_pt;
  TH1D* m_h_photon_eta;
  TH1D* m_h_photon_phi;
  TH2D* m_h2_photon_eta_phi;
  TH1D* m_two_photon_plus;
  TH1D* m_two_photon_ns_plus;
  //neutrino
  TH1D* m_h_nNeutrinos;
  TH1D* m_h_neutrino_e;
  TH1D* m_h_neutrino_pt;
  TH1D* m_h_neutrino_eta;
  TH1D* m_h_neutrino_phi;
  TH2D* m_h2_neutrino_eta_phi;
  TH1D* m_Wboson;
  //Jet
  TH1D* m_h_nJets;
  TH1D* m_h_jet_e;
  TH1D* m_h_jet_pt;
  TH1D* m_h_jet_eta;
  TH1D* m_h_jet_phi;
  TH2D* m_h2_jet_eta_phi;
  TH1D* m_jet_mass;
  TH1D* m_two_jet_plus;
  TString jetAlgo;
  TString config;
  TString calibSeq;
  bool isData;
  //MET
  TH1D* m_h_nMETs;
  TH1D* m_h_MET_e;
  TH1D* m_h_MET_pt;
  TH1D* m_h_MET_eta;
  TH1D* m_h_MET_phi;
  TH2D* m_h2_MET_eta_phi;
  TH1D* m_MET_mass;
 
  //ratio
  double h_ratio;
  
  
  //all two
  TH1D* m_h_Zboson_pt_ptcut;
  TH1D* m_h_Zboson_mass_ptcut;
  TH1D* m_h_Zboson_pt_masscut;
  TH1D* m_h_Zboson_mass_masscut;
  
  // all four
  TH1D* m_h_two_Zboson_pt_allptcut;
  TH1D* m_h_two_Zboson_pt_muonptmasscut;
  TH1D* m_h_two_Zboson_pt_electronptmasscut ;
  
  //signal
  TH1D* m_4l_plus_allptcut;
  TH1D* m_4l_plus_pt_allptcut;
  TH1D* m_4l_plus_muonptmasscut;
  TH1D* m_4l_plus_pt_muonptmasscut;
  TH1D* m_4l_plus_electronptmasscut;
  TH1D* m_4l_plus_pt_electronptmasscut;
  TH1D* m_4l_plus_nocut;
  TH1D* m_4l_plus_pt_nocut;
  
  TH1D* m_4l_plus_eta_nocut;
  TH1D* m_2l_plus_eta_ptcut;

  //bckground with bkg cut

  TH1D* m_h_two_Zboson_100_electroncut;
  TH1D* m_h_two_Zboson_75_electroncut;
  TH1D* m_h_two_Zboson_50_electroncut;
  
  TH1D* m_h_two_Zboson_100_allcut;
  TH1D* m_h_two_Zboson_75_allcut;
  TH1D* m_h_two_Zboson_50_allcut; 

  TH1D* m_h_two_Zboson_100_muoncut; 
  TH1D* m_h_two_Zboson_75_muoncut;
  TH1D* m_h_two_Zboson_50_muoncut;
  TH1D* m_four_muon_plus_nocut;

  //number
  TH1D* m_h_nAllevent;
  TH1D* m_h_nTwolepton;
  TH1D* m_h_nZboson;
  TH1D* m_h_nFourlepton;
  TH1D* m_h_nTwoZboson;
  TH1D* n_sumOfWeights;
  TH1D* n_event_noweight;
  TH1D* eventweight_sum; 
  double all_mcEventWeight = 0;
  TH1D* m_h_Zmumu;
  TH1D* m_h_Zmumu_pt;
  TH1D* m_h_Zmumu_event_nocut;
  TH1D* m_h_Zmumu_event_ptcut;
  TH1D* m_h_Zmumu_event;
  
  TH1D* m_h_Zee;
  TH1D* m_h_Zee_pt;

  TH1D* m_Zboson_met;
  TH1D* m_Zboson_met_pt;
  TH1D* m_Zboson_ljet;
  TH1D* m_Zboson_ljet_pt;
  TH1D* m_Zboson_bjet;
  TH1D* m_Zboson_bjet_pt;


  //TTree
  TTree* m_tree;
  TTree* Zmumu_tree;
  TTree* four_tree;
  TTree* ljet_tree;
  TTree* truthljet_tree;
  TTree* bjet_tree;
  TTree* truthbjet_tree;
  TTree* tau_tree;
  TTree* ltau_htau_tree;
  TTree* truthtau_tree;
  TTree* photon_tree;
  TTree* truthphoton_tree;
  TTree* twol_ljet_tree;
  TTree* twol_truthljet_tree;
  TTree* twol_sjet_tree;
  TTree* twol_truthsjet_tree;
  TTree* four_sjet_tree;
  TTree* four_truthsjet_tree;
  TTree* Wlnu_tree;
  TTree* Wl_ljet_tree;
  TTree* Wl_sjet_tree;
  TTree* tll_tree;
  TTree* tWb_tree;
  TTree* tljet_b_tree;
  TTree* ttwoljet_b_tree;
  TTree* tljetWb_tree;
  TTree* ttwoljet_oneb_tree;
  TTree* Hhh_tree;
  int m_eventNumber;
  int m_lumiBlock;
  int m_runNumber;
  int m_nMuons;
  std::vector<int>* m_muon_author;
  std::vector<double>* m_muon_phi;
  std::vector<double>* m_muon_eta;
  std::vector<double>* m_muon_pt;
  std::vector<double>* m_muon_e;
  std::vector<double>* m_muon_charge;
  //ratio_tree
  TTree* ratio_tree;
  //Zboson met Tree
  Float_t m_Zboson_met_dphi;
  Float_t m_Zboson_eta;
  Float_t m_Zboson_pt;
  Float_t m_Zboson_m;
  Float_t m_met_pt;
  Float_t Zmumu_met_mass;

  //event
  TH1D* m_h_one_Wmunu_event;
  TH1D* m_h_one_Wmunu_event_nocut;
  TH1D* m_h_one_Wmunu_event_ptcut;
  TH1D* m_h_Zmumu_event_etacut;
  TH1D* all_event;
  TH1D* m_h_one_Wmunu_event_etacut;
  TH1D* m_h_Zmumu_event_ns;

  //HT
  TH1D* m_HT;
  TH1D* m_HT_150GeV;

  //4l tree
  Float_t m_four_phi;
  Float_t m_four_eta;
  Float_t m_four_pt;
  Float_t m_four_e;
  Float_t m_four_m;
  double weight_four;
  int four_number;

  Float_t m_ljet_dphi;
  Float_t m_ljet_eta_1st;
  Float_t m_ljet_pt_1st;
  Float_t m_ljet_m_1st;
  Float_t m_ljet_eta_2nd;
  Float_t m_ljet_pt_2nd;
  Float_t m_ljet_m_2nd;
  Float_t diljet_mass;
  double weight_ljet;
  int ljet_number;
  
  Float_t m_truthljet_dphi;
  Float_t m_truthljet_eta_1st;
  Float_t m_truthljet_pt_1st;
  Float_t m_truthljet_m_1st;
  Float_t m_truthljet_eta_2nd;
  Float_t m_truthljet_pt_2nd;
  Float_t m_truthljet_m_2nd;
  Float_t ditruthljet_mass;
  double weight_truthljet;
  

  Float_t m_bjet_dphi;
  Float_t m_bjet_eta_1st;
  Float_t m_bjet_pt_1st;
  Float_t m_bjet_m_1st;
  Float_t m_bjet_eta_2nd;
  Float_t m_bjet_pt_2nd;
  Float_t m_bjet_m_2nd;
  double weight_bjet;
  Float_t dibjet_mass;
  int bjet_number;

  Float_t m_truthbjet_dphi;
  Float_t m_truthbjet_eta_1st;
  Float_t m_truthbjet_pt_1st;
  Float_t m_truthbjet_m_1st;
  Float_t m_truthbjet_eta_2nd;
  Float_t m_truthbjet_pt_2nd;
  Float_t m_truthbjet_m_2nd;
  double weight_truthbjet;
  Float_t ditruthbjet_mass;


  Float_t m_tau_dphi;
  Float_t m_tau_eta_1st;
  Float_t m_tau_pt_1st;
  Float_t m_tau_m_1st;
  Float_t m_tau_eta_2nd;
  Float_t m_tau_pt_2nd;
  Float_t m_tau_m_2nd;
  Float_t m_tau_met_e;
  double weight_tau;
  Float_t ditau_mass;
  int tau_number;

  Float_t m_truthtau_dphi;
  Float_t m_truthtau_eta_1st;
  Float_t m_truthtau_pt_1st;
  Float_t m_truthtau_m_1st;
  Float_t m_truthtau_eta_2nd;
  Float_t m_truthtau_pt_2nd;
  Float_t m_truthtau_m_2nd;
  double weight_truthtau;
  Float_t ditruthtau_mass;

  Float_t m_ltau_htau_dphi;
  Float_t m_ltau_lepton_pt;
  Float_t m_ltau_lepton_m;
  Float_t m_ltau_lepton_eta; 
  Float_t m_htau_eta;
  Float_t m_htau_pt;
  Float_t m_ltau_htau_met_e;
  double weight_ltau_htau;
  Float_t ltau_htau_mass;

  Float_t m_photon_dphi;
  Float_t m_photon_eta_1st;
  Float_t m_photon_pt_1st;
  Float_t m_photon_m_1st;
  Float_t m_photon_eta_2nd;
  Float_t m_photon_pt_2nd;
  Float_t m_photon_m_2nd;
  double weight_photon;
  Float_t diphoton_mass;
  int photon_number;

  Float_t m_truthphoton_dphi;
  Float_t m_truthphoton_eta_1st;
  Float_t m_truthphoton_pt_1st;
  Float_t m_truthphoton_m_1st;
  Float_t m_truthphoton_eta_2nd;
  Float_t m_truthphoton_pt_2nd;
  Float_t m_truthphoton_m_2nd;
  double weight_truthphoton;
  Float_t ditruthphoton_mass;
  //Large R jet
  Float_t m_twol_ljet_dphi;
  Float_t m_twol_ljet_eta;
  Float_t m_twol_ljet_pt;
  Float_t m_twol_ljet_m;
  Float_t m_twol_Zboson_eta;
  Float_t m_twol_Zboson_pt;
  Float_t m_twol_Zboson_m;
  Float_t twol_ljet_mass;
  double weight_twol_ljet;

  Float_t m_twol_truthljet_dphi;
  Float_t m_twol_truthljet_eta;
  Float_t m_twol_truthljet_pt;
  Float_t m_twol_truthljet_m;
  Float_t m_twol_truthZboson_eta;
  Float_t m_twol_truthZboson_pt;
  Float_t m_twol_truthZboson_m;
  Float_t twol_truthljet_mass;
  double weight_twol_truthljet;

  //2l+2q
  Float_t m_twol_sjet_dphi;
  Float_t m_twol_sjet_first_eta;
  Float_t m_twol_sjet_first_pt;
  Float_t m_twol_sjet_first_m;
  Float_t m_twol_sjet_second_eta;
  Float_t m_twol_sjet_second_pt;
  Float_t m_twol_sjet_second_m;
  Float_t m_twol_sjet_m;
  Float_t m_twol_sjet_Zboson_eta;
  Float_t m_twol_sjet_Zboson_pt;
  Float_t m_twol_sjet_Zboson_m;
  Float_t twol_sjet_mass;
  double weight_twol_sjet;

  Float_t m_twol_truthsjet_dphi;
  Float_t m_twol_truthsjet_first_eta;
  Float_t m_twol_truthsjet_first_pt;
  Float_t m_twol_truthsjet_first_m;
  Float_t m_twol_truthsjet_second_eta;
  Float_t m_twol_truthsjet_second_pt;
  Float_t m_twol_truthsjet_second_m;
  Float_t m_twol_truthsjet_m;
  Float_t m_twol_truthsjet_Zboson_eta;
  Float_t m_twol_truthsjet_Zboson_pt;
  Float_t m_twol_truthsjet_Zboson_m;
  Float_t twol_truthsjet_mass;
  double weight_twol_truthsjet;

  Float_t m_four_sjet_mass;
  double weight_four_sjet;

  Float_t m_four_truthsjet_mass;
  Float_t m_two_truthsjet_mass_1st;
  Float_t m_two_truthsjet_mass_2nd;
  double weight_four_truthsjet;

  //W
  Float_t m_Wlnu_eta_1st;
  Float_t m_Wlnu_pt_1st;
  Float_t m_Wlnu_m_1st;
  Float_t m_Wlnu_eta_2nd;
  Float_t m_Wlnu_pt_2nd;
  Float_t m_Wlnu_m_2nd;
  Float_t m_Wlnu_met_pt;
  Float_t m_Wlnu_mass;
  Double_t weight_Wlnu;
  
  Float_t m_Wl_ljet_ljet_eta;
  Float_t m_Wl_ljet_ljet_pt;
  Float_t m_Wl_ljet_ljet_m;
  Float_t m_Wl_ljet_l_eta;
  Float_t m_Wl_ljet_l_pt;
  Float_t m_Wl_ljet_l_m;
  Float_t m_Wl_ljet_met_pt;
  Float_t m_Wl_ljet_lnu_m;
  Float_t m_Wl_ljet_mass;
  Double_t weight_Wl_ljet;

  Float_t m_Wl_sjet_sjet_eta_1st;
  Float_t m_Wl_sjet_sjet_pt_1st;
  Float_t m_Wl_sjet_sjet_m_1st;
  Float_t m_Wl_sjet_sjet_eta_2nd;
  Float_t m_Wl_sjet_sjet_pt_2nd;
  Float_t m_Wl_sjet_sjet_m_2nd;
  Float_t m_Wl_sjet_disjet_m;
  Float_t m_Wl_sjet_l_eta;
  Float_t m_Wl_sjet_l_pt;
  Float_t m_Wl_sjet_l_m;
  Float_t m_Wl_sjet_met_pt;
  Float_t m_Wl_sjet_lnu_m;
  Float_t m_Wl_sjet_mass;
  Double_t weight_Wl_sjet;
  //tt
  Float_t m_tll_dphi;
  Float_t m_tll_ljet_eta_1st;
  Float_t m_tll_ljet_pt_1st;
  Float_t m_tll_ljet_m_1st;
  Float_t m_tll_ljet_eta_2nd;
  Float_t m_tll_ljet_pt_2nd;
  Float_t m_tll_ljet_m_2nd;
  Float_t m_tll_mass;
  Double_t weight_tll;

  Float_t m_tWb_l_eta_1st;
  Float_t m_tWb_l_pt_1st;
  Float_t m_tWb_l_m_1st;
  Float_t m_tWb_l_eta_2nd;
  Float_t m_tWb_l_pt_2nd;
  Float_t m_tWb_l_m_2nd;
  Float_t m_tWb_met_pt;
  Float_t m_tWb_b_eta_1st;
  Float_t m_tWb_b_pt_1st;
  Float_t m_tWb_b_m_1st;
  Float_t m_tWb_b_eta_2nd;
  Float_t m_tWb_b_pt_2nd;
  Float_t m_tWb_b_m_2nd;
  Float_t m_tWb_mass;
  Double_t weight_tWb;

  Float_t m_Hhh_dphi;
  Float_t m_Hhh_b_eta_1st;
  Float_t m_Hhh_b_pt_1st;
  Float_t m_Hhh_b_m_1st;
  Float_t m_Hhh_b_eta_2nd;
  Float_t m_Hhh_b_pt_2nd;
  Float_t m_Hhh_b_m_2nd;
  Float_t m_Hhh_b_eta_3rd;
  Float_t m_Hhh_b_pt_3rd;
  Float_t m_Hhh_b_m_3rd;
  Float_t m_Hhh_b_eta_4th;
  Float_t m_Hhh_b_pt_4th;
  Float_t m_Hhh_b_m_4th;
  Float_t m_Hhh_h_m_1st;
  Float_t m_Hhh_h_m_2nd;
  Float_t m_Hhh_mass;
  Double_t weight_Hhh;

  Float_t m_tljet_b_l_eta_1st;
  Float_t m_tljet_b_l_pt_1st;
  Float_t m_tljet_b_l_m_1st;
  Float_t m_tljet_b_met_pt;
  Float_t m_tljet_b_lnu_m;
  Float_t m_tljet_b_ljet_eta;
  Float_t m_tljet_b_ljet_pt;
  Float_t m_tljet_b_ljet_m;
  Float_t m_tljet_b_b_eta_1st;
  Float_t m_tljet_b_b_pt_1st;
  Float_t m_tljet_b_b_m_1st;
  Float_t m_tljet_b_b_eta_2nd;
  Float_t m_tljet_b_b_pt_2nd;
  Float_t m_tljet_b_b_m_2nd;
  Float_t m_tljet_b_mass;
  Double_t weight_tljet_b;

  Float_t m_ttwoljet_b_ljet_eta_1st;
  Float_t m_ttwoljet_b_ljet_pt_1st;
  Float_t m_ttwoljet_b_ljet_m_1st;
  Float_t m_ttwoljet_b_ljet_eta_2nd;
  Float_t m_ttwoljet_b_ljet_pt_2nd;
  Float_t m_ttwoljet_b_ljet_m_2nd;
  Float_t m_ttwoljet_b_b_eta_1st;
  Float_t m_ttwoljet_b_b_pt_1st;
  Float_t m_ttwoljet_b_b_m_1st;
  Float_t m_ttwoljet_b_b_eta_2nd;
  Float_t m_ttwoljet_b_b_pt_2nd;
  Float_t m_ttwoljet_b_b_m_2nd;
  Float_t m_ttwoljet_b_mass;
  Double_t weight_ttwoljet_b;

 Float_t m_ttwoljet_oneb_tljet_eta;
 Float_t m_ttwoljet_oneb_tljet_pt;
 Float_t m_ttwoljet_oneb_tljet_m;
 Float_t m_ttwoljet_oneb_Wljet_eta;
 Float_t m_ttwoljet_oneb_Wljet_pt;
 Float_t m_ttwoljet_oneb_Wljet_m;
 Float_t m_ttwoljet_oneb_b_eta;
 Float_t m_ttwoljet_oneb_b_pt;
 Float_t m_ttwoljet_oneb_b_m;
 Float_t m_ttwoljet_oneb_mass;
 Double_t weight_ttwoljet_oneb;

 Float_t m_tljetWb_ljet_eta;
 Float_t m_tljetWb_ljet_pt;
 Float_t m_tljetWb_ljet_m;
 Float_t m_tljetWb_l_eta;
 Float_t m_tljetWb_l_pt;
 Float_t m_tljetWb_l_m;
 Float_t m_tljetWb_met_pt;
 Float_t m_tljetWb_b_eta;
 Float_t m_tljetWb_b_pt;
 Float_t m_tljetWb_b_m;
 Float_t m_tljetWb_mass;
 Double_t weight_tljetWb;
  
  //check
  TTree* check_tree;
  Float_t check_tau_mass = 0;
  //TH1D cutflow
  TH1D* h_allevent;
  TH1D* h_dibjet;
  TH1D* h_dibjetcut;
  TH1D* h_diljet;
  TH1D* h_diljetcut;
  TH1D* h_twol_ljet;
  TH1D* h_twol_ljetcut;
  TH1D* h_twol_sjet;
  TH1D* h_twol_sjetcut;
  TH1D* h_ditau;
  TH1D* h_ditaucut;
  TH1D* h_ttbar;

};

#endif // MYANALYSISALG_H
