{
  // Open an input file for training and test:
  // NB: To avoid basket's WriteBuffer full problem, the input file pointer of TFile should be obtained before opening outputFile.
  //TFile* sig_input = TFile::Open("DataSample_bjet.root");
  TFile* sig_input = TFile::Open("./TMVATree/SignalSample.root");
  //std::cout << "Zmumu1" << std::endl;
  TFile* bkg_input0_70_ZmumuA = TFile::Open("TMVATree/Zmumu/Zmumu_MAXHTPTV0_70_CVetoBVeto.root");
  std::cout << "Zmumu2" << std::endl;
  TFile* bkg_input0_70_ZmumuB = TFile::Open("TMVATree/Zmumu/Zmumu_MAXHTPTV0_70_CFilterBVeto.root");
  std::cout << "Zmumu3" << std::endl;
  TFile* bkg_input0_70_ZmumuC = TFile::Open("TMVATree/Zmumu/Zmumu_MAXHTPTV0_70_BFilter.root");
  std::cout << "Zmumu4" << std::endl;
  TFile* bkg_input70_140_ZmumuA = TFile::Open("TMVATree/Zmumu/Zmumu_MAXHTPTV70_140_CVetoBVeto.root");
  std::cout << "Zmumu5" << std::endl;
  TFile* bkg_input70_140_ZmumuB = TFile::Open("TMVATree/Zmumu/Zmumu_MAXHTPTV70_140_CFilterBVeto.root");
  std::cout << "Zmumu6" << std::endl;
  TFile* bkg_input70_140_ZmumuC = TFile::Open("TMVATree/Zmumu/Zmumu_MAXHTPTV70_140_BFilter.root");
  std::cout << "Zmumu7" << std::endl;
  TFile* bkg_input140_280_ZmumuA = TFile::Open("TMVATree/Zmumu/Zmumu_MAXHTPTV140_280_CVetoBVeto.root");
  std::cout << "Zmumu8" << std::endl;
  TFile* bkg_input140_280_ZmumuB = TFile::Open("TMVATree/Zmumu/Zmumu_MAXHTPTV140_280_CFilterBVeto.root");
  std::cout << "Zmumu9" << std::endl;
  TFile* bkg_input140_280_ZmumuC = TFile::Open("TMVATree/Zmumu/Zmumu_MAXHTPTV140_280_BFilter.root");
  std::cout << "Zmumu10" << std::endl;
  TFile* bkg_input280_500_ZmumuA = TFile::Open("TMVATree/Zmumu/Zmumu_MAXHTPTV280_500_CVetoBVeto.root");
  std::cout << "Zmumu11" << std::endl;
  TFile* bkg_input280_500_ZmumuB = TFile::Open("TMVATree/Zmumu/Zmumu_MAXHTPTV280_500_CFilterBVeto.root");
  std::cout << "Zmumu12" << std::endl;
  TFile* bkg_input280_500_ZmumuC = TFile::Open("TMVATree/Zmumu/Zmumu_MAXHTPTV280_500_BFilter.root");
  std::cout << "Zmumu13" << std::endl;
  TFile* bkg_input500_1000_Zmumu = TFile::Open("TMVATree/Zmumu/Zmumu_MAXHTPTV500_1000.root");
  std::cout << "Zmumu14" << std::endl;
  TFile* bkg_inputE_CMS_Zmumu = TFile::Open("TMVATree/Zmumu/Zmumu_MAXHTPTV1000_E_CMS.root");

  //std::cout << "Zee1" << std::endl;
  TFile* bkg_input0_70_ZeeA = TFile::Open("TMVATree/Zee/Zee_MAXHTPTV0_70_CVetoBVeto.root");
  std::cout << "Zee2" << std::endl;
  TFile* bkg_input0_70_ZeeB = TFile::Open("TMVATree/Zee/Zee_MAXHTPTV0_70_CFilterBVeto.root");
  std::cout << "Zee3" << std::endl;
  TFile* bkg_input0_70_ZeeC = TFile::Open("TMVATree/Zee/Zee_MAXHTPTV0_70_BFilter.root");
  std::cout << "Zee4" << std::endl;
  TFile* bkg_input70_140_ZeeA = TFile::Open("TMVATree/Zee/Zee_MAXHTPTV70_140_CVetoBVeto.root");
  std::cout << "Zee5" << std::endl;
  TFile* bkg_input70_140_ZeeB = TFile::Open("TMVATree/Zee/Zee_MAXHTPTV70_140_CFilterBVeto.root");
  std::cout << "Zee6" << std::endl;
  TFile* bkg_input70_140_ZeeC = TFile::Open("TMVATree/Zee/Zee_MAXHTPTV70_140_BFilter.root");
  std::cout << "Zee7" << std::endl;
  TFile* bkg_input140_280_ZeeA = TFile::Open("TMVATree/Zee/Zee_MAXHTPTV140_280_CVetoBVeto.root");
  std::cout << "Zee8" << std::endl;
  TFile* bkg_input140_280_ZeeB = TFile::Open("TMVATree/Zee/Zee_MAXHTPTV140_280_CFilterBVeto.root");
  std::cout << "Zee9" << std::endl;
  TFile* bkg_input140_280_ZeeC = TFile::Open("TMVATree/Zee/Zee_MAXHTPTV140_280_BFilter.root");
  std::cout << "Zee10" << std::endl;
  TFile* bkg_input280_500_ZeeA = TFile::Open("TMVATree/Zee/Zee_MAXHTPTV280_500_CVetoBVeto.root");
  std::cout << "Zee11" << std::endl;
  TFile* bkg_input280_500_ZeeB = TFile::Open("TMVATree/Zee/Zee_MAXHTPTV280_500_CFilterBVeto.root");
  std::cout << "Zee12" << std::endl;
  TFile* bkg_input280_500_ZeeC = TFile::Open("TMVATree/Zee/Zee_MAXHTPTV280_500_BFilter.root");
  std::cout << "Zee13" << std::endl;
  TFile* bkg_input500_1000_Zee = TFile::Open("TMVATree/Zee/Zee_MAXHTPTV500_1000.root");
  std::cout << "Zee14" << std::endl;
  TFile* bkg_inputE_CMS_Zee = TFile::Open("TMVATree/Zee/Zee_MAXHTPTV1000_E_CMS.root");

  std::cout << "Ztautau1" << std::endl;
  TFile* bkg_input0_70_ZtautauA = TFile::Open("TMVATree/Ztautau/Ztautau_MAXHTPTV0_70_CVetoBVeto.root");
  std::cout << "Ztautau2" << std::endl;
  TFile* bkg_input0_70_ZtautauB = TFile::Open("TMVATree/Ztautau/Ztautau_MAXHTPTV0_70_CFilterBVeto.root");
  std::cout << "Ztautau3" << std::endl;
  TFile* bkg_input0_70_ZtautauC = TFile::Open("TMVATree/Ztautau/Ztautau_MAXHTPTV0_70_BFilter.root");
  std::cout << "Ztautau4" << std::endl;
  TFile* bkg_input70_140_ZtautauA = TFile::Open("TMVATree/Ztautau/Ztautau_MAXHTPTV70_140_CVetoBVeto.root");
  std::cout << "Ztautau5" << std::endl;
  TFile* bkg_input70_140_ZtautauB = TFile::Open("TMVATree/Ztautau/Ztautau_MAXHTPTV70_140_CFilterBVeto.root");
  std::cout << "Ztautau6" << std::endl;
  TFile* bkg_input70_140_ZtautauC = TFile::Open("TMVATree/Ztautau/Ztautau_MAXHTPTV70_140_BFilter.root");
  std::cout << "Ztautau7" << std::endl;
  TFile* bkg_input140_280_ZtautauA = TFile::Open("TMVATree/Ztautau/Ztautau_MAXHTPTV140_280_CVetoBVeto.root");
  std::cout << "Ztautau8" << std::endl;
  TFile* bkg_input140_280_ZtautauB = TFile::Open("TMVATree/Ztautau/Ztautau_MAXHTPTV140_280_CFilterBVeto.root");
  std::cout << "Ztautau9" << std::endl;
  TFile* bkg_input140_280_ZtautauC = TFile::Open("TMVATree/Ztautau/Ztautau_MAXHTPTV140_280_BFilter.root");
  std::cout << "Ztautau10" << std::endl;
  TFile* bkg_input280_500_ZtautauA = TFile::Open("TMVATree/Ztautau/Ztautau_MAXHTPTV280_500_CVetoBVeto.root");
  std::cout << "Ztautau11" << std::endl;
  TFile* bkg_input280_500_ZtautauB = TFile::Open("TMVATree/Ztautau/Ztautau_MAXHTPTV280_500_CFilterBVeto.root");
  std::cout << "Ztautau12" << std::endl;
  TFile* bkg_input280_500_ZtautauC = TFile::Open("TMVATree/Ztautau/Ztautau_MAXHTPTV280_500_BFilter.root");
  std::cout << "Ztautau13" << std::endl;
  TFile* bkg_input500_1000_Ztautau = TFile::Open("TMVATree/Ztautau/Ztautau_MAXHTPTV500_1000.root");
  std::cout << "Ztautau14" << std::endl;
  TFile* bkg_inputE_CMS_Ztautau = TFile::Open("TMVATree/Ztautau/Ztautau_MAXHTPTV1000_E_CMS.root");

  std::cout << "Wmunu1" << std::endl;
  TFile* bkg_input0_70_WmunuA = TFile::Open("TMVATree/Wmunu/Wmunu_MAXHTPTV0_70_CVetoBVeto.root");
  std::cout << "Wmunu2" << std::endl;
  TFile* bkg_input0_70_WmunuB = TFile::Open("TMVATree/Wmunu/Wmunu_MAXHTPTV0_70_CFilterBVeto.root");
  std::cout << "Wmunu3" << std::endl;
  TFile* bkg_input0_70_WmunuC = TFile::Open("TMVATree/Wmunu/Wmunu_MAXHTPTV0_70_BFilter.root");
  std::cout << "Wmunu4" << std::endl;
  TFile* bkg_input70_140_WmunuA = TFile::Open("TMVATree/Wmunu/Wmunu_MAXHTPTV70_140_CVetoBVeto.root");
  std::cout << "Wmunu5" << std::endl;
  TFile* bkg_input70_140_WmunuB = TFile::Open("TMVATree/Wmunu/Wmunu_MAXHTPTV70_140_CFilterBVeto.root");
  std::cout << "Wmunu6" << std::endl;
  TFile* bkg_input70_140_WmunuC = TFile::Open("TMVATree/Wmunu/Wmunu_MAXHTPTV70_140_BFilter.root");
  std::cout << "Wmunu7" << std::endl;
  TFile* bkg_input140_280_WmunuA = TFile::Open("TMVATree/Wmunu/Wmunu_MAXHTPTV140_280_CVetoBVeto.root");
  std::cout << "Wmunu8" << std::endl;
  TFile* bkg_input140_280_WmunuB = TFile::Open("TMVATree/Wmunu/Wmunu_MAXHTPTV140_280_CFilterBVeto.root");
  std::cout << "Wmunu9" << std::endl;
  TFile* bkg_input140_280_WmunuC = TFile::Open("TMVATree/Wmunu/Wmunu_MAXHTPTV140_280_BFilter.root");
  std::cout << "Wmunu10" << std::endl;
  TFile* bkg_input280_500_WmunuA = TFile::Open("TMVATree/Wmunu/Wmunu_MAXHTPTV280_500_CVetoBVeto.root");
  std::cout << "Wmunu11" << std::endl;
  TFile* bkg_input280_500_WmunuB = TFile::Open("TMVATree/Wmunu/Wmunu_MAXHTPTV280_500_CFilterBVeto.root");
  std::cout << "Wmunu12" << std::endl;
  TFile* bkg_input280_500_WmunuC = TFile::Open("TMVATree/Wmunu/Wmunu_MAXHTPTV280_500_BFilter.root");
  std::cout << "Wmunu13" << std::endl;
  TFile* bkg_input500_1000_Wmunu = TFile::Open("TMVATree/Wmunu/Wmunu_MAXHTPTV500_1000.root");
  std::cout << "Wmunu14" << std::endl;
  TFile* bkg_inputE_CMS_Wmunu = TFile::Open("TMVATree/Wmunu/Wmunu_MAXHTPTV1000_E_CMS.root");
  
  std::cout << "Wenu1" << std::endl;
  TFile* bkg_input0_70_WenuA = TFile::Open("TMVATree/Wenu/Wenu_MAXHTPTV0_70_CVetoBVeto.root");
  std::cout << "Wenu2" << std::endl;
  TFile* bkg_input0_70_WenuB = TFile::Open("TMVATree/Wenu/Wenu_MAXHTPTV0_70_CFilterBVeto.root");
  std::cout << "Wenu3" << std::endl;
  TFile* bkg_input0_70_WenuC = TFile::Open("TMVATree/Wenu/Wenu_MAXHTPTV0_70_BFilter.root");
  std::cout << "Wenu4" << std::endl;
  TFile* bkg_input70_140_WenuA = TFile::Open("TMVATree/Wenu/Wenu_MAXHTPTV70_140_CVetoBVeto.root");
  std::cout << "Wenu5" << std::endl;
  TFile* bkg_input70_140_WenuB = TFile::Open("TMVATree/Wenu/Wenu_MAXHTPTV70_140_CFilterBVeto.root");
  std::cout << "Wenu6" << std::endl;
  TFile* bkg_input70_140_WenuC = TFile::Open("TMVATree/Wenu/Wenu_MAXHTPTV70_140_BFilter.root");
  std::cout << "Wenu7" << std::endl;
  TFile* bkg_input140_280_WenuA = TFile::Open("TMVATree/Wenu/Wenu_MAXHTPTV140_280_CVetoBVeto.root");
  std::cout << "Wenu8" << std::endl;
  TFile* bkg_input140_280_WenuB = TFile::Open("TMVATree/Wenu/Wenu_MAXHTPTV140_280_CFilterBVeto.root");
  std::cout << "Wenu9" << std::endl;
  TFile* bkg_input140_280_WenuC = TFile::Open("TMVATree/Wenu/Wenu_MAXHTPTV140_280_BFilter.root");
  std::cout << "Wenu10" << std::endl;
  TFile* bkg_input280_500_WenuA = TFile::Open("TMVATree/Wenu/Wenu_MAXHTPTV280_500_CVetoBVeto.root");
  std::cout << "Wenu11" << std::endl;
  TFile* bkg_input280_500_WenuB = TFile::Open("TMVATree/Wenu/Wenu_MAXHTPTV280_500_CFilterBVeto.root");
  std::cout << "Wenu12" << std::endl;
  TFile* bkg_input280_500_WenuC = TFile::Open("TMVATree/Wenu/Wenu_MAXHTPTV280_500_BFilter.root");
  std::cout << "Wenu13" << std::endl;
  TFile* bkg_input500_1000_Wenu = TFile::Open("TMVATree/Wenu/Wenu_MAXHTPTV500_1000.root");
  std::cout << "Wenu14" << std::endl;
  TFile* bkg_inputE_CMS_Wenu = TFile::Open("TMVATree/Wenu/Wenu_MAXHTPTV1000_E_CMS.root");

  std::cout << "Wtaunu1" << std::endl;
  TFile* bkg_input0_70_WtaunuA = TFile::Open("TMVATree/Wtaunu/Wtaunu_MAXHTPTV0_70_CVetoBVeto.root");
  std::cout << "Wtaunu2" << std::endl;
  TFile* bkg_input0_70_WtaunuB = TFile::Open("TMVATree/Wtaunu/Wtaunu_MAXHTPTV0_70_CFilterBVeto.root");
  std::cout << "Wtaunu3" << std::endl;
  TFile* bkg_input0_70_WtaunuC = TFile::Open("TMVATree/Wtaunu/Wtaunu_MAXHTPTV0_70_BFilter.root");
  std::cout << "Wtaunu4" << std::endl;
  TFile* bkg_input70_140_WtaunuA = TFile::Open("TMVATree/Wtaunu/Wtaunu_MAXHTPTV70_140_CVetoBVeto.root");
  std::cout << "Wtaunu5" << std::endl;
  TFile* bkg_input70_140_WtaunuB = TFile::Open("TMVATree/Wtaunu/Wtaunu_MAXHTPTV70_140_CFilterBVeto.root");
  std::cout << "Wtaunu6" << std::endl;
  TFile* bkg_input70_140_WtaunuC = TFile::Open("TMVATree/Wtaunu/Wtaunu_MAXHTPTV70_140_BFilter.root");
  std::cout << "Wtaunu7" << std::endl;
  TFile* bkg_input140_280_WtaunuA = TFile::Open("TMVATree/Wtaunu/Wtaunu_MAXHTPTV140_280_CVetoBVeto.root");
  std::cout << "Wtaunu8" << std::endl;
  TFile* bkg_input140_280_WtaunuB = TFile::Open("TMVATree/Wtaunu/Wtaunu_MAXHTPTV140_280_CFilterBVeto.root");
  std::cout << "Wtaunu9" << std::endl;
  TFile* bkg_input140_280_WtaunuC = TFile::Open("TMVATree/Wtaunu/Wtaunu_MAXHTPTV140_280_BFilter.root");
  std::cout << "Wtaunu10" << std::endl;
  TFile* bkg_input280_500_WtaunuA = TFile::Open("TMVATree/Wtaunu/Wtaunu_MAXHTPTV280_500_CVetoBVeto.root");
  std::cout << "Wtaunu11" << std::endl;
  TFile* bkg_input280_500_WtaunuB = TFile::Open("TMVATree/Wtaunu/Wtaunu_MAXHTPTV280_500_CFilterBVeto.root");
  std::cout << "Wtaunu12" << std::endl;
  TFile* bkg_input280_500_WtaunuC = TFile::Open("TMVATree/Wtaunu/Wtaunu_MAXHTPTV280_500_BFilter.root");
  std::cout << "Wtaunu13" << std::endl;
  TFile* bkg_input500_1000_Wtaunu = TFile::Open("TMVATree/Wtaunu/Wtaunu_MAXHTPTV500_1000.root");
  std::cout << "Wtaunu14" << std::endl;
  TFile* bkg_inputE_CMS_Wtaunu = TFile::Open("TMVATree/Wtaunu/Wtaunu_MAXHTPTV1000_E_CMS.root");

  std::cout << "ttbar" << std::endl;
  TFile* bkg_input_ttbar = TFile::Open("TMVATree/ttbar/ttbar_bkg.root");

  std::cout << "fullttbar" << std::endl;
  TFile* bkg_input_fullttbar = TFile::Open("TMVATree/fullttbar/fullttbar_bkg.root");
  

  std::cout << "--- TMVAClassification   : Using input signal file: " << sig_input->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input0_70_ZmumuA->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input0_70_ZmumuB->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input0_70_ZmumuC->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input70_140_ZmumuA->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input70_140_ZmumuB->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input70_140_ZmumuC->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input140_280_ZmumuA->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input140_280_ZmumuB->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input140_280_ZmumuC->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input280_500_ZmumuA->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input280_500_ZmumuB->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input280_500_ZmumuC->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input500_1000_Zmumu->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_inputE_CMS_Zmumu->GetName() << std::endl;

  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input0_70_ZeeA->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input0_70_ZeeB->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input0_70_ZeeC->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input70_140_ZeeA->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input70_140_ZeeB->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input70_140_ZeeC->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input140_280_ZeeA->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input140_280_ZeeB->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input140_280_ZeeC->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input280_500_ZeeA->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input280_500_ZeeB->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input280_500_ZeeC->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input500_1000_Zee->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_inputE_CMS_Zee->GetName() << std::endl;

  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input0_70_ZtautauA->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input0_70_ZtautauB->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input0_70_ZtautauC->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input70_140_ZtautauA->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input70_140_ZtautauB->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input70_140_ZtautauC->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input140_280_ZtautauA->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input140_280_ZtautauB->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input140_280_ZtautauC->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input280_500_ZtautauA->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input280_500_ZtautauB->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input280_500_ZtautauC->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input500_1000_Ztautau->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_inputE_CMS_Ztautau->GetName() << std::endl;

  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input0_70_WmunuA->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input0_70_WmunuB->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input0_70_WmunuC->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input70_140_WmunuA->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input70_140_WmunuB->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input70_140_WmunuC->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input140_280_WmunuA->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input140_280_WmunuB->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input140_280_WmunuC->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input280_500_WmunuA->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input280_500_WmunuB->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input280_500_WmunuC->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input500_1000_Wmunu->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_inputE_CMS_Wmunu->GetName() << std::endl;

  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input0_70_WenuA->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input0_70_WenuB->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input0_70_WenuC->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input70_140_WenuA->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input70_140_WenuB->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input70_140_WenuC->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input140_280_WenuA->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input140_280_WenuB->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input140_280_WenuC->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input280_500_WenuA->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input280_500_WenuB->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input280_500_WenuC->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input500_1000_Wenu->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_inputE_CMS_Wenu->GetName() << std::endl;

  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input0_70_WtaunuA->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input0_70_WtaunuB->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input0_70_WtaunuC->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input70_140_WtaunuA->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input70_140_WtaunuB->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input70_140_WtaunuC->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input140_280_WtaunuA->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input140_280_WtaunuB->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input140_280_WtaunuC->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input280_500_WtaunuA->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input280_500_WtaunuB->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input280_500_WtaunuC->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input500_1000_Wtaunu->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_inputE_CMS_Wtaunu->GetName() << std::endl;

  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input_ttbar->GetName() << std::endl;
  std::cout << "--- TMVAClassification   : Using input Background file: " << bkg_input_fullttbar->GetName() << std::endl;
  
  // Create a ROOT output file where TMVA will store ntuples, histograms, etc.
  TString outfileName("TMVA_bjet_BDT_data.root ");
  TFile* outputFile = TFile::Open(outfileName, "RECREATE");
  
  // Create the factory object
  TMVA::Factory* factory = new TMVA::Factory("TMVAClassification", outputFile,
                                              "!V:!Silent:Color:!DrawProgressBar:AnalysisType=Classification");
  
  // Define the input variables that shall be used for the MVA training
  // note that you may also use variable expressions, such as: "3*var1/var2*abs(var3)"
  TMVA::DataLoader* dataloader = new TMVA::DataLoader("Dataset");
  dataloader->AddVariable("m_bjet_dphi", 'F' );
  dataloader->AddVariable("m_bjet_eta_1st", 'F');
  dataloader->AddVariable("m_bjet_pt_1st", 'F');
  dataloader->AddVariable("m_bjet_m_1st",  'F');
  dataloader->AddVariable("m_bjet_eta_2nd", 'F');
  dataloader->AddVariable("m_bjet_pt_2nd", 'F');
  dataloader->AddVariable("m_bjet_m_2nd",  'F');
  //dataloader->AddVariable("dibjet_mass", 'F');
     
  // --- Register the training and test trees
  TTree* t_sig = (TTree*)sig_input->Get("bjetTree");
  TTree* bkg_0_70_ZmumuA = (TTree*)bkg_input0_70_ZmumuA->Get("bjetTree");
  TTree* bkg_0_70_ZmumuB = (TTree*)bkg_input0_70_ZmumuB->Get("bjetTree");
  TTree* bkg_0_70_ZmumuC = (TTree*)bkg_input0_70_ZmumuC->Get("bjetTree");
  TTree* bkg_70_140_ZmumuA = (TTree*)bkg_input70_140_ZmumuA->Get("bjetTree");
  TTree* bkg_70_140_ZmumuB = (TTree*)bkg_input70_140_ZmumuB->Get("bjetTree");
  TTree* bkg_70_140_ZmumuC = (TTree*)bkg_input70_140_ZmumuC->Get("bjetTree");
  TTree* bkg_140_280_ZmumuA = (TTree*)bkg_input140_280_ZmumuA->Get("bjetTree");
  TTree* bkg_140_280_ZmumuB = (TTree*)bkg_input140_280_ZmumuB->Get("bjetTree");
  TTree* bkg_140_280_ZmumuC = (TTree*)bkg_input140_280_ZmumuC->Get("bjetTree");
  TTree* bkg_280_500_ZmumuA = (TTree*)bkg_input280_500_ZmumuA->Get("bjetTree");
  TTree* bkg_280_500_ZmumuB = (TTree*)bkg_input280_500_ZmumuB->Get("bjetTree");
  TTree* bkg_280_500_ZmumuC = (TTree*)bkg_input280_500_ZmumuC->Get("bjetTree");
  TTree* bkg_500_1000_Zmumu = (TTree*)bkg_input500_1000_Zmumu->Get("bjetTree");
  TTree* bkg_E_CMS_Zmumu = (TTree*)bkg_inputE_CMS_Zmumu->Get("bjetTree");

  TTree* bkg_0_70_ZeeA = (TTree*)bkg_input0_70_ZeeA->Get("bjetTree");
  TTree* bkg_0_70_ZeeB = (TTree*)bkg_input0_70_ZeeB->Get("bjetTree");
  TTree* bkg_0_70_ZeeC = (TTree*)bkg_input0_70_ZeeC->Get("bjetTree");
  TTree* bkg_70_140_ZeeA = (TTree*)bkg_input70_140_ZeeA->Get("bjetTree");
  TTree* bkg_70_140_ZeeB = (TTree*)bkg_input70_140_ZeeB->Get("bjetTree");
  TTree* bkg_70_140_ZeeC = (TTree*)bkg_input70_140_ZeeC->Get("bjetTree");
  TTree* bkg_140_280_ZeeA = (TTree*)bkg_input140_280_ZeeA->Get("bjetTree");
  TTree* bkg_140_280_ZeeB = (TTree*)bkg_input140_280_ZeeB->Get("bjetTree");
  TTree* bkg_140_280_ZeeC = (TTree*)bkg_input140_280_ZeeC->Get("bjetTree");
  TTree* bkg_280_500_ZeeA = (TTree*)bkg_input280_500_ZeeA->Get("bjetTree");
  TTree* bkg_280_500_ZeeB = (TTree*)bkg_input280_500_ZeeB->Get("bjetTree");
  TTree* bkg_280_500_ZeeC = (TTree*)bkg_input280_500_ZeeC->Get("bjetTree");
  TTree* bkg_500_1000_Zee = (TTree*)bkg_input500_1000_Zee->Get("bjetTree");
  TTree* bkg_E_CMS_Zee = (TTree*)bkg_inputE_CMS_Zee->Get("bjetTree");

  TTree* bkg_0_70_ZtautauA = (TTree*)bkg_input0_70_ZtautauA->Get("bjetTree");
  TTree* bkg_0_70_ZtautauB = (TTree*)bkg_input0_70_ZtautauB->Get("bjetTree");
  TTree* bkg_0_70_ZtautauC = (TTree*)bkg_input0_70_ZtautauC->Get("bjetTree");
  TTree* bkg_70_140_ZtautauA = (TTree*)bkg_input70_140_ZtautauA->Get("bjetTree");
  TTree* bkg_70_140_ZtautauB = (TTree*)bkg_input70_140_ZtautauB->Get("bjetTree");
  TTree* bkg_70_140_ZtautauC = (TTree*)bkg_input70_140_ZtautauC->Get("bjetTree");
  TTree* bkg_140_280_ZtautauA = (TTree*)bkg_input140_280_ZtautauA->Get("bjetTree");
  TTree* bkg_140_280_ZtautauB = (TTree*)bkg_input140_280_ZtautauB->Get("bjetTree");
  TTree* bkg_140_280_ZtautauC = (TTree*)bkg_input140_280_ZtautauC->Get("bjetTree");
  TTree* bkg_280_500_ZtautauA = (TTree*)bkg_input280_500_ZtautauA->Get("bjetTree");
  TTree* bkg_280_500_ZtautauB = (TTree*)bkg_input280_500_ZtautauB->Get("bjetTree");
  TTree* bkg_280_500_ZtautauC = (TTree*)bkg_input280_500_ZtautauC->Get("bjetTree");
  TTree* bkg_500_1000_Ztautau = (TTree*)bkg_input500_1000_Ztautau->Get("bjetTree");
  TTree* bkg_E_CMS_Ztautau = (TTree*)bkg_inputE_CMS_Ztautau->Get("bjetTree");
 
  TTree* bkg_0_70_WmunuA = (TTree*)bkg_input0_70_WmunuA->Get("bjetTree");
  TTree* bkg_0_70_WmunuB = (TTree*)bkg_input0_70_WmunuB->Get("bjetTree");
  TTree* bkg_0_70_WmunuC = (TTree*)bkg_input0_70_WmunuC->Get("bjetTree");
  TTree* bkg_70_140_WmunuA = (TTree*)bkg_input70_140_WmunuA->Get("bjetTree");
  TTree* bkg_70_140_WmunuB = (TTree*)bkg_input70_140_WmunuB->Get("bjetTree");
  TTree* bkg_70_140_WmunuC = (TTree*)bkg_input70_140_WmunuC->Get("bjetTree");
  TTree* bkg_140_280_WmunuA = (TTree*)bkg_input140_280_WmunuA->Get("bjetTree");
  TTree* bkg_140_280_WmunuB = (TTree*)bkg_input140_280_WmunuB->Get("bjetTree");
  TTree* bkg_140_280_WmunuC = (TTree*)bkg_input140_280_WmunuC->Get("bjetTree");
  TTree* bkg_280_500_WmunuA = (TTree*)bkg_input280_500_WmunuA->Get("bjetTree");
  TTree* bkg_280_500_WmunuB = (TTree*)bkg_input280_500_WmunuB->Get("bjetTree");
  TTree* bkg_280_500_WmunuC = (TTree*)bkg_input280_500_WmunuC->Get("bjetTree");
  TTree* bkg_500_1000_Wmunu = (TTree*)bkg_input500_1000_Wmunu->Get("bjetTree");
  TTree* bkg_E_CMS_Wmunu = (TTree*)bkg_inputE_CMS_Wmunu->Get("bjetTree");

  TTree* bkg_0_70_WenuA = (TTree*)bkg_input0_70_WenuA->Get("bjetTree");
  TTree* bkg_0_70_WenuB = (TTree*)bkg_input0_70_WenuB->Get("bjetTree");
  TTree* bkg_0_70_WenuC = (TTree*)bkg_input0_70_WenuC->Get("bjetTree");
  TTree* bkg_70_140_WenuA = (TTree*)bkg_input70_140_WenuA->Get("bjetTree");
  TTree* bkg_70_140_WenuB = (TTree*)bkg_input70_140_WenuB->Get("bjetTree");
  TTree* bkg_70_140_WenuC = (TTree*)bkg_input70_140_WenuC->Get("bjetTree");
  TTree* bkg_140_280_WenuA = (TTree*)bkg_input140_280_WenuA->Get("bjetTree");
  TTree* bkg_140_280_WenuB = (TTree*)bkg_input140_280_WenuB->Get("bjetTree");
  TTree* bkg_140_280_WenuC = (TTree*)bkg_input140_280_WenuC->Get("bjetTree");
  TTree* bkg_280_500_WenuA = (TTree*)bkg_input280_500_WenuA->Get("bjetTree");
  TTree* bkg_280_500_WenuB = (TTree*)bkg_input280_500_WenuB->Get("bjetTree");
  TTree* bkg_280_500_WenuC = (TTree*)bkg_input280_500_WenuC->Get("bjetTree");
  TTree* bkg_500_1000_Wenu = (TTree*)bkg_input500_1000_Wenu->Get("bjetTree");
  TTree* bkg_E_CMS_Wenu = (TTree*)bkg_inputE_CMS_Wenu->Get("bjetTree");

  TTree* bkg_0_70_WtaunuA = (TTree*)bkg_input0_70_WtaunuA->Get("bjetTree");
  TTree* bkg_0_70_WtaunuB = (TTree*)bkg_input0_70_WtaunuB->Get("bjetTree");
  TTree* bkg_0_70_WtaunuC = (TTree*)bkg_input0_70_WtaunuC->Get("bjetTree");
  TTree* bkg_70_140_WtaunuA = (TTree*)bkg_input70_140_WtaunuA->Get("bjetTree");
  TTree* bkg_70_140_WtaunuB = (TTree*)bkg_input70_140_WtaunuB->Get("bjetTree");
  TTree* bkg_70_140_WtaunuC = (TTree*)bkg_input70_140_WtaunuC->Get("bjetTree");
  TTree* bkg_140_280_WtaunuA = (TTree*)bkg_input140_280_WtaunuA->Get("bjetTree");
  TTree* bkg_140_280_WtaunuB = (TTree*)bkg_input140_280_WtaunuB->Get("bjetTree");
  TTree* bkg_140_280_WtaunuC = (TTree*)bkg_input140_280_WtaunuC->Get("bjetTree");
  TTree* bkg_280_500_WtaunuA = (TTree*)bkg_input280_500_WtaunuA->Get("bjetTree");
  TTree* bkg_280_500_WtaunuB = (TTree*)bkg_input280_500_WtaunuB->Get("bjetTree");
  TTree* bkg_280_500_WtaunuC = (TTree*)bkg_input280_500_WtaunuC->Get("bjetTree");
  TTree* bkg_500_1000_Wtaunu = (TTree*)bkg_input500_1000_Wtaunu->Get("bjetTree");
  TTree* bkg_E_CMS_Wtaunu = (TTree*)bkg_inputE_CMS_Wtaunu->Get("bjetTree");

  TTree* bkg_ttbar = (TTree*)bkg_input_ttbar->Get("bjetTree");
  TTree* bkg_fullttbar = (TTree*)bkg_input_ttbar->Get("bjetTree");
     
  // global event weights per tree (see below for setting event-wise weights)
  Double_t signalWeight     = 1.0;
  Double_t backgroundWeight = 1.0;
   
     
  // You can add an arbitrary number of signal or background trees
  //weight = crossection/sumOfweight
  dataloader->AddSignalTree(t_sig, signalWeight);
  std::cout << endl << "------------------------------" << endl;
  //std::cout << "bkg_0_70_ZmumuA" << endl;
  //dataloader->AddBackgroundTree(bkg_0_70_ZmumuA, (1981.7*0.82152)/752857.02);
  //std::cout << "bkg_0_70_ZmumuB" << endl;
  //dataloader->AddBackgroundTree(bkg_0_70_ZmumuB, (1982.6*0.11357)/195606.96);
  //std::cout << "bkg_0_70_ZmumuC" << endl;
  //dataloader->AddBackgroundTree(bkg_0_70_ZmumuC, (1981.8*0.065853)/259453.19);
  //std::cout << "bkg_70_140_ZmumuA" << endl;
  //dataloader->AddBackgroundTree(bkg_70_140_ZmumuA, (109.07*0.68831)/240752.05);
  //std::cout << "bkg_70_140_ZmumuB" << endl;
  //dataloader->AddBackgroundTree(bkg_70_140_ZmumuB, (108.99*0.19451)/164083.04);
  //std::cout << "bkg_70_140_ZmumuC" << endl;
  //dataloader->AddBackgroundTree(bkg_70_140_ZmumuC, (109.03*0.11735)/304791.85);
  //std::cout << "bkg_140_280_ZmumuA" << endl;
  //dataloader->AddBackgroundTree(bkg_140_280_ZmumuA, (39.892*0.61205)/469776.51);
  std::cout << "bkg_140_280_ZmumuB" << endl;
  dataloader->AddBackgroundTree(bkg_140_280_ZmumuB, (39.847*0.23347)/398033.98);
  std::cout << "bkg_140_280_ZmumuC" << endl;
  dataloader->AddBackgroundTree(bkg_140_280_ZmumuC, (39.877*0.15579)/837636.17);
  //std::cout << "bkg_280_500_ZmumuA" << endl;
  //dataloader->AddBackgroundTree(bkg_280_500_ZmumuA, (8.522*0.56046)/345232.04);
  std::cout << "bkg_280_500_ZmumuB" << endl;
  dataloader->AddBackgroundTree(bkg_280_500_ZmumuB, (8.5194*0.26621)/321686.56);
  std::cout << "bkg_280_500_ZmumuC" << endl;
  dataloader->AddBackgroundTree(bkg_280_500_ZmumuC, (8.5301*0.17666)/584064.73);
  std::cout << "bkg_500_1000_Zmumu" << endl;
  dataloader->AddBackgroundTree(bkg_500_1000_Zmumu, (1.7876*1)/467399.85);
  std::cout << "bkg_E_CMS_Zmumu" << endl;
  dataloader->AddBackgroundTree(bkg_E_CMS_Zmumu, (0.1476*1)/244809.70);

  //std::cout << "bkg_0_70_ZeeA" << endl;
  //dataloader->AddBackgroundTree(bkg_0_70_ZeeA, (1982.8*0.82120)/765948.72);
  std::cout << "bkg_0_70_ZeeB" << endl;
  dataloader->AddBackgroundTree(bkg_0_70_ZeeB, (1981.7*0.11367)/339726.00);
  //std::cout << "bkg_0_70_ZeeC" << endl;
  //dataloader->AddBackgroundTree(bkg_0_70_ZeeC, (1981.9*0.065697)/658105.07);
  //std::cout << "bkg_70_140_ZeeA" << endl;
  //dataloader->AddBackgroundTree(bkg_70_140_ZeeA, (110.71*0.69362)/435203.02);
  std::cout << "bkg_70_140_ZeeB" << endl;
  dataloader->AddBackgroundTree(bkg_70_140_ZeeB, (110.47*0.19130)/102304.86);
  std::cout << "bkg_70_140_ZeeC" << endl;
  //dataloader->AddBackgroundTree(bkg_70_140_ZeeC, (110.53*0.11587)/130454.55);
  std::cout << "bkg_140_280_ZeeA" << endl;
  dataloader->AddBackgroundTree(bkg_140_280_ZeeA, (40.645*0.61591)/561512.11);
  //std::cout << "bkg_140_280_ZeeB" << endl;
  //dataloader->AddBackgroundTree(bkg_140_280_ZeeB, (40.674*0.23268)/439364.95);
  std::cout << "bkg_140_280_ZeeC" << endl;
  dataloader->AddBackgroundTree(bkg_140_280_ZeeC, (40.68*0.15352)/423872.54);
  std::cout << "bkg_280_500_ZeeA" << endl;
  dataloader->AddBackgroundTree(bkg_280_500_ZeeA, (8.6716*0.56429)/318851.10);
  std::cout << "bkg_280_500_ZeeB" << endl;
  dataloader->AddBackgroundTree(bkg_280_500_ZeeB, (8.6698*0.26616)/128307.31);
  std::cout << "bkg_280_500_ZeeC" << endl;
  dataloader->AddBackgroundTree(bkg_280_500_ZeeC, (8.6801*0.17632)/440470.94);
  std::cout << "bkg_500_1000_Zee" << endl;
  dataloader->AddBackgroundTree(bkg_500_1000_Zee, (1.8089*1)/475318.09);
  std::cout << "bkg_E_CMS_Zee" << endl;
  dataloader->AddBackgroundTree(bkg_E_CMS_Zee, (0.14881*1)/182816.89);

  std::cout << "bkg_0_70_ZtautauA" << endl;
  dataloader->AddBackgroundTree(bkg_0_70_ZtautauA, (1982.6*0.82105)/868284.77);
  //std::cout << "bkg_0_70_ZtautauB" << endl;
  //dataloader->AddBackgroundTree(bkg_0_70_ZtautauB, (1981.9*0.11359)/714316.37);
  std::cout << "bkg_0_70_ZtautauC" << endl;
  dataloader->AddBackgroundTree(bkg_0_70_ZtautauC, (1982*0.065773)/543116.44);
  std::cout << "bkg_70_140_ZtautauA" << endl;
  dataloader->AddBackgroundTree(bkg_70_140_ZtautauA, (110.72*0.69313)/198600.69);
  //std::cout << "bkg_70_140_ZtautauB" << endl;
  //dataloader->AddBackgroundTree(bkg_70_140_ZtautauB, (110.46*0.18905)/371530.16);
  std::cout << "bkg_70_140_ZtautauC" << endl;
  dataloader->AddBackgroundTree(bkg_70_140_ZtautauC, (110.68*0.12001)/356630.99);
  //std::cout << "bkg_140_280_ZtautauA" << endl;
  //dataloader->AddBackgroundTree(bkg_140_280_ZtautauA, (40.756*0.61697)/514074.63);
  std::cout << "bkg_140_280_ZtautauB" << endl;
  dataloader->AddBackgroundTree(bkg_140_280_ZtautauB, (40.71*0.23420)/520893.21);
  std::cout << "bkg_140_280_ZtautauC" << endl;
  dataloader->AddBackgroundTree(bkg_140_280_ZtautauC, (40.741*0.15494)/717258.67);
  std::cout << "bkg_280_500_ZtautauA" << endl;
  dataloader->AddBackgroundTree(bkg_280_500_ZtautauA, (8.522*0.56430)/404149.03);
  std::cout << "bkg_280_500_ZtautauB" << endl;
  dataloader->AddBackgroundTree(bkg_280_500_ZtautauB, (8.6758*0.26759)/242368.17);
  std::cout << "bkg_280_500_ZtautauC" << endl;
  dataloader->AddBackgroundTree(bkg_280_500_ZtautauC, (8.6788*0.17615)/392295.20);
  std::cout << "bkg_500_1000_Ztautau" << endl;
  dataloader->AddBackgroundTree(bkg_500_1000_Ztautau, (1.8075*1)/754718.01);
  std::cout << "bkg_E_CMS_Ztautau" << endl;
  dataloader->AddBackgroundTree(bkg_E_CMS_Ztautau, (0.14821*1)/325903.21);

  //std::cout << "bkg_0_70_WmunuA" << endl;
  //dataloader->AddBackgroundTree(bkg_0_70_WmunuA, (19151*0.82460)/322490.07);
  //std::cout << "bkg_0_70_WmunuB" << endl;
  //dataloader->AddBackgroundTree(bkg_0_70_WmunuB, (19145*0.13031)/516098.86);
  std::cout << "bkg_0_70_WmunuC" << endl;
  dataloader->AddBackgroundTree(bkg_0_70_WmunuC, (19143*0.044209)/931959.04);
  //std::cout << "bkg_70_140_WmunuA" << endl;
  //dataloader->AddBackgroundTree(bkg_70_140_WmunuA, (945.89*0.68150)/427078.94);
  //std::cout << "bkg_70_140_WmunuB" << endl;
  //dataloader->AddBackgroundTree(bkg_70_140_WmunuB, (946.12*0.23841)/297331.49);
  //std::cout << "bkg_70_140_WmunuC" << endl;
  //dataloader->AddBackgroundTree(bkg_70_140_WmunuC, (944.95*0.084077)/325011.85);
  //std::cout << "bkg_140_280_WmunuA" << endl;
  //dataloader->AddBackgroundTree(bkg_140_280_WmunuA, (339.73*0.60009)/394176.85);
  std::cout << "bkg_140_280_WmunuB" << endl;
  dataloader->AddBackgroundTree(bkg_140_280_WmunuB, (339.8*0.29172)/591339.09);
  std::cout << "bkg_140_280_WmunuC" << endl;
  dataloader->AddBackgroundTree(bkg_140_280_WmunuC, (339.68*0.11111)/513568.21);
  //std::cout << "bkg_280_500_WmunuA" << endl;
  //dataloader->AddBackgroundTree(bkg_280_500_WmunuA, (72.084*0.54738)/727297.13);
  std::cout << "bkg_280_500_WmunuB" << endl;
  dataloader->AddBackgroundTree(bkg_280_500_WmunuB, (72.103*0.32005)/786500.66);
  std::cout << "bkg_280_500_WmunuC" << endl;
  dataloader->AddBackgroundTree(bkg_280_500_WmunuC, (72.063*0.13720)/525780.09);
  std::cout << "bkg_500_1000_Wmunu" << endl;
  dataloader->AddBackgroundTree(bkg_500_1000_Wmunu, (15.008*1)/572383.01);
  std::cout << "bkg_E_CMS_WmunuA" << endl;
  dataloader->AddBackgroundTree(bkg_E_CMS_Wmunu, (1.2349*1)/379841.47);

  //std::cout << "bkg_0_70_WenuA" << endl;
  //dataloader->AddBackgroundTree(bkg_0_70_WenuA, (19153*0.82457)/330821.82);
  //std::cout << "bkg_0_70_WenuB" << endl;
  //dataloader->AddBackgroundTree(bkg_0_70_WenuB, (19145*0.13102)/888480.07);
  //std::cout << "bkg_0_70_WenuC" << endl;
  //dataloader->AddBackgroundTree(bkg_0_70_WenuC, (19143*0.044183)/762949.79);
  //std::cout << "bkg_70_140_WenuA" << endl;
  //dataloader->AddBackgroundTree(bkg_70_140_WenuA, (945.69*0.68025)/444473.34);
  //std::cout << "bkg_70_140_WenuB" << endl;
  //dataloader->AddBackgroundTree(bkg_70_140_WenuB, (946.37*0.24282)/404069.66);
  //std::cout << "bkg_70_140_WenuC" << endl;
  //dataloader->AddBackgroundTree(bkg_70_140_WenuC, (945.63*0.083353)/255850.86);
  //std::cout << "bkg_140_280_WenuA" << endl;
  //dataloader->AddBackgroundTree(bkg_140_280_WenuA, (339.75*0.59652)/458066.69);
  std::cout << "bkg_140_280_WenuB" << endl;
  dataloader->AddBackgroundTree(bkg_140_280_WenuB, (339.8*0.28804)/503995.11);
  std::cout << "bkg_140_280_WenuC" << endl;
  dataloader->AddBackgroundTree(bkg_140_280_WenuC, (339.7*0.11088)/645545.68);
  std::cout << "bkg_280_500_WenuA" << endl;
  dataloader->AddBackgroundTree(bkg_280_500_WenuA, (72.077*0.54800)/428081.89);
  std::cout << "bkg_280_500_WenuB" << endl;
  dataloader->AddBackgroundTree(bkg_280_500_WenuB, (72.105*0.32017)/368294.60);
  std::cout << "bkg_280_500_WenuC" << endl;
  dataloader->AddBackgroundTree(bkg_280_500_WenuC, (72.077*0.13755)/383015.61);
  std::cout << "bkg_500_1000_Wenu" << endl;
  dataloader->AddBackgroundTree(bkg_500_1000_Wenu, (15.05*1)/340976.13);
  std::cout << "bkg_E_CMS_WenuA" << endl;
  dataloader->AddBackgroundTree(bkg_E_CMS_Wenu, (1.2344*1)/402956.30);

  //std::cout << "bkg_0_70_WtaunuA" << endl;
  //dataloader->AddBackgroundTree(bkg_0_70_WtaunuA, (19155*0.82462)/1148703.7);
  std::cout << "bkg_0_70_WtaunuB" << endl;
  dataloader->AddBackgroundTree(bkg_0_70_WtaunuB, (19154*0.12957)/597405.37);
  std::cout << "bkg_0_70_WtaunuC" << endl;
  dataloader->AddBackgroundTree(bkg_0_70_WtaunuC, (19152*0.045076)/1101023.9);
  //std::cout << "bkg_70_140_WtaunuA" << endl;
  //dataloader->AddBackgroundTree(bkg_70_140_WtaunuA, (945.58*0.68329)/267289.10);
  //std::cout << "bkg_70_140_WtaunuB" << endl;
  //dataloader->AddBackgroundTree(bkg_70_140_WtaunuB, (946.49*0.24245)/250910.21);
  //std::cout << "bkg_70_140_WtaunuC" << endl;
  //dataloader->AddBackgroundTree(bkg_70_140_WtaunuC, (945.87*0.086159)/454828.87);
  std::cout << "bkg_140_280_WtaunuA" << endl;
  dataloader->AddBackgroundTree(bkg_140_280_WtaunuA, (339.69*0.59821)/880153.91);
  std::cout << "bkg_140_280_WtaunuB" << endl;
  dataloader->AddBackgroundTree(bkg_140_280_WtaunuB, (339.84*0.29032)/868948.72);
  std::cout << "bkg_140_280_WtaunuC" << endl;
  dataloader->AddBackgroundTree(bkg_140_280_WtaunuC, (339.68*0.11153)/1111621.1);
  //std::cout << "bkg_280_500_WtaunuA" << endl;
  //dataloader->AddBackgroundTree(bkg_280_500_WtaunuA, (72.078*0.56172)/411578.62);
  std::cout << "bkg_280_500_WtaunuB" << endl;
  dataloader->AddBackgroundTree(bkg_280_500_WtaunuB, (71.99*0.31984)/271234.18);
  std::cout << "bkg_280_500_WtaunuC" << endl;
  dataloader->AddBackgroundTree(bkg_280_500_WtaunuC, (71.944*0.13604)/249039.21);
  std::cout << "bkg_500_1000_Wtaunu" << endl;
  dataloader->AddBackgroundTree(bkg_500_1000_Wtaunu, (15.052*1)/975508.77);
  std::cout << "bkg_E_CMS_Wtaunu" << endl;
  dataloader->AddBackgroundTree(bkg_E_CMS_Wtaunu, (1.2342*1)/367636.65);

  std::cout << "bkg_ttbar" << endl;
  dataloader->AddBackgroundTree(bkg_ttbar, (729.77*0.54382)/3.190e+09);
  std::cout << "bkg_fullttbar" << endl;
  dataloader->AddBackgroundTree(bkg_fullttbar, (729.77*0.45621)/963878654);

  
  // Set individual event weights (the variables must exist in the original TTree)
  dataloader->SetSignalWeightExpression("m_mcEventWeight");
  dataloader->SetBackgroundWeightExpression("m_mcEventWeight");
  
  // Apply additional cuts on the signal and background samples (can be different)
  TCut mycuts = ""; // for example: TCut mycuts = "abs(var1)<0.5 && abs(var2-0.5)<1";
  TCut mycutb = ""; // for example: TCut mycutb = "abs(var1)<0.5";
  
  // Tell the factory how to use the training and testing events
  dataloader->PrepareTrainingAndTestTree(mycuts, mycutb,
					 "nTrain_Signal=3000:nTrain_Background=79906:SplitMode=Random:NormMode=NumEvents:!V" );
  
  // ---- Book MVA methods
  //Cut optimisation
  // factory->BookMethod(dataloader, TMVA::Types::kCuts, "Cuts",
  //		      "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart" );
  
  // Likelihood ("naive Bayes estimator")
  //factory->BookMethod(dataloader, TMVA::Types::kLikelihood, "Likelihood",
  //		      "H:!V:TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmoothBkg[1]=10:NSmooth=1:NAvEvtPerBin=50");
  
  // Test the multi-dimensional probability density estimator
  // here are the options strings for the MinMax and RMS methods, respectively:
  //factory->BookMethod(dataloader, TMVA::Types::kPDERS, "PDERS",
  //		      "!H:!V:NormTree=T:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600");
  
  // Boosted Decision Trees
  factory->BookMethod(dataloader, TMVA::Types::kBDT, "BDT",
  		      "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20" );
  
  //rule fit
  //factory->BookMethod(dataloader, TMVA::Types::kRuleFit, "RuleFit",
  //		      "H:!V:RuleFitModule=RFTMVA:Model=ModRuleLinear:MinImp=0.001:RuleMinDist=0.001:NTrees=20:fEventsMin=0.01:fEventsMax=0.5:GDTau=-1.0:GDTauPrec=0.01:GDStep=0.01:GDNSteps=10000:GDErrScale=1.02" );
  
  // ---- Now you can tell the factory to train, test, and evaluate the MVAs
  // Train MVAs using the set of training events
  factory->TrainAllMethods();
  // ---- Evaluate all MVAs using the set of test events
  factory->TestAllMethods();
  // ----- Evaluate and compare performance of all configured MVAs
  factory->EvaluateAllMethods();
  
  // Save the output
  outputFile->Close();
  
  std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
  std::cout << "==> TMVAClassification is done!" << std::endl;
  
  delete factory;
}
