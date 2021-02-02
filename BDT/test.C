#include "TMVA/Reader.h"
#include <iostream>
#include "TTree.h"

void test()
{
  // --- Create the Reader object
  TMVA::Reader* reader = new TMVA::Reader( "!Color:!Silent" );   
  
  // Create a set of variables and declare them to the reader
  // - the variable names MUST corresponds in name and type to those given in the weight file(s) used
  Float_t dphi;
  Float_t eta_1st;
  Float_t pt_1st;
  Float_t m_1st;
  Float_t eta_2nd;
  Float_t pt_2nd;
  Float_t m_2nd;
  
  reader->AddVariable("m_bjet_dphi", &dphi);
  reader->AddVariable("m_bjet_eta_1st", &eta_1st);
  reader->AddVariable("m_bjet_pt_1st", &pt_1st);
  reader->AddVariable("m_bjet_m_1st", &m_1st);
  reader->AddVariable("m_bjet_eta_2nd", &eta_2nd);
  reader->AddVariable("m_bjet_pt_2nd", &pt_2nd);
  reader->AddVariable("m_bjet_m_2nd", &m_2nd);
  
  // --- Book the MVA methods
  TString dir    = "./Dataset_bkg/Dataset/weights/";
  TString prefix = "TMVAClassification";
  
  // Book method(s)
  TString methodName("BDT method");
  TString weightfile = dir + prefix + TString("_BDT.weights.xml");
  reader->BookMVA(methodName, weightfile);
  
  // Book output histograms
  UInt_t nbin = 100;
  TH1F* histBdt  = new TH1F("MVA_BDT",        "MVA_BDT",        nbin, -0.8, 0.8 );
  
  // Prepare input tree (this must be replaced by your data source)
  // in this example, there is a toy tree with signal and one with background events
  // we'll later on use only the "signal" events for the test in this example.
  //  
  TFile *input(0);
  //TString fname = "./TMVATree/SignalSample.root";
  TString fname = "./DataSample_bjet.root";
  input = TFile::Open( fname ); // check if file in local directory exists
     
  if (!input) {
     std::cout << "ERROR: could not open data file" << std::endl;
     exit(1);
  }
  std::cout << "--- TMVAClassificationApp    : Using input file: " << input->GetName() << std::endl;

  //outputfile
  TFile *target  = new TFile( "./bjet_BDT_apply_data.root","RECREATE" );
  //TTree
  TTree *Zmumu_tree;
  Float_t BDTop;
  Float_t m_weight;
  Zmumu_tree = new TTree("bjetTree", "Tree of bjet");
  Zmumu_tree->Branch("BDToutput", &BDTop, "BDToutput/F");
  Zmumu_tree->Branch("mcEventWeight", &m_weight, "mcEventWeight/F");
  
  // --- Event loop
  // Prepare the event tree
  // - here the variable names have to corresponds to your tree
  // - you can use the same variables as above which is slightly faster,
  //   but of course you can use different ones and copy the values inside the event loop
  std::cout << "--- Select signal sample" << std::endl;
  TTree* theTree = (TTree*)input->Get("bjetTree");

  Float_t m_dphi;
  Float_t m_eta_1st;
  Float_t m_pt_1st;
  Float_t m_m_1st;
  Float_t m_eta_2nd;
  Float_t m_pt_2nd;
  Float_t m_m_2nd;
  theTree->SetBranchAddress("m_bjet_dphi", &m_dphi);
  theTree->SetBranchAddress("m_bjet_eta_1st", &m_eta_1st);
  theTree->SetBranchAddress("m_bjet_pt_1st", &m_pt_1st);
  theTree->SetBranchAddress("m_bjet_m_1st", &m_m_1st);
  theTree->SetBranchAddress("m_bjet_eta_2nd", &m_eta_2nd);
  theTree->SetBranchAddress("m_bjet_pt_2nd", &m_pt_2nd);
  theTree->SetBranchAddress("m_bjet_m_2nd", &m_m_2nd);

  double mcEventWeight;
  theTree->SetBranchAddress("m_mcEventWeight", &mcEventWeight);
   
  std::cout << "--- Processing: " << theTree->GetEntries() << " events" << std::endl;
  
  for (Long64_t ievt=0; ievt<theTree->GetEntries();ievt++) {
     if (ievt%1000 == 0) std::cout << "--- ... Processing event: " << ievt << std::endl;
     theTree->GetEntry(ievt);
     
     dphi = (Float_t)m_dphi;
     eta_1st = (Float_t)m_eta_1st;
     pt_1st = (Float_t)m_pt_1st;
     m_1st = (Float_t)m_m_1st;
     eta_2nd = (Float_t)m_eta_2nd;
     pt_2nd = (Float_t)m_pt_2nd;
     m_2nd = (Float_t)m_m_2nd;
     // --- Return the MVA outputs and fill into histograms
     histBdt ->Fill(reader->EvaluateMVA("BDT method"), mcEventWeight);
     BDTop = 0;
     m_weight = 0;
     BDTop = reader->EvaluateMVA("BDT method");
     m_weight = mcEventWeight;
     Zmumu_tree->Fill();
  }
  
  // --- Write histograms
  
  histBdt ->Write();
  Zmumu_tree->Write();
  target->Close();

  //Zmumu_tree->Print();
  
  std::cout << "--- Created root file:\"Zmumu_BDT_apply_data.root\" containing the MVA output histograms" << std::endl;
  delete reader;
  std::cout << "==> TMVAClassificationApplication is done!" << std::endl << std::endl;
}
