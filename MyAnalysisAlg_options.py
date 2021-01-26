#-----------------------------------------------------------------------------
# Athena imports
#-----------------------------------------------------------------------------
from AthenaCommon.Constants import *
from AthenaCommon.AppMgr import theApp
from AthenaCommon.AppMgr import ServiceMgr
##For MC 
#import AthenaPoolCnvSvc.ReadAthenaPool
##For data
import AthenaRootComps.ReadAthenaxAODHybrid

from AthenaCommon.AlgSequence import AlgSequence
job = AlgSequence()

#-----------------------------------------------------------------------------
# Message Service
#-----------------------------------------------------------------------------
# Set output level threshold (2=DEBUG, 3=INFO, 4=WARNING, 5=ERROR, 6=FATAL )
ServiceMgr.MessageSvc.OutputLevel = WARNING
import AthenaServices
AthenaServices.AthenaServicesConf.AthenaEventLoopMgr.OutputLevel = WARNING

#-----------------------------------------------------------------------------
# Input Datasets
#-----------------------------------------------------------------------------
ServiceMgr.EventSelector.InputCollections = [
    "/gpfs/fs7001/jtanaka/AJPhysicsChallenge/Signal/mc16_13TeV.999999.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_ggH411W2_inclUniBR.DAOD_SUSY1.p3990/DAOD_SUSY1.test.pool.root"]
#We use /gpfs/fs7001/jtanaka/AJPhysicsChallenge/Signal/mc16_13TeV.999999.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_ggH411W2_inclUniBR.DAOD_SUSY1.p3990/DAOD_SUSY1.test.pool.root

#####many files
#import glob
#datasets = glob.glob("/gpfs/fs7001/jtanaka/AJPhysicsChallenge/Data/merged*")
#ServiceMgr.EventSelector.InputCollections = datasets

#/gpfs/fs7001/jtanaka/AJPhysicsChallenge/Data/merged*

##background
#/gpfs/fs7001/jtanaka/AJPhysicsChallenge/Background/mc16_13TeV.364101.Sherpa_221_NNPDF30NNLO_Zmumu_MAXHTPTV0_70_CFilterBVeto.deriv.DAOD_SUSY1.e5271_s3126_r10201_p3990/DAOD_SUSY*



theApp.EvtMax = -1 # -1 means all events

#-----------------------------------------------------------------------------
# Algorithms
#-----------------------------------------------------------------------------
from MyAnalysisExample.MyAnalysisExampleConf import *
myAnaAlg = MyAnalysisAlg("MyAnalysisAlg1", OutputLevel = DEBUG)

########set number

#crosssectrion = 1981.7

#CP tool
from MuonSelectorTools.MuonSelectorToolsConf import CP__MuonSelectionTool
myAnaAlg.MuonSelTool = CP__MuonSelectionTool("MyMuonSelectionTool",
                                             MuQuality=1)
#For MuQuality=3(very loose) For =1(Medium)

#Electron selection
from ElectronPhotonSelectorTools.ElectronPhotonSelectorToolsConf import AsgElectronLikelihoodTool
myAnaAlg.ElectronSelTool = AsgElectronLikelihoodTool("MyAsgElectronLikelihoodTool", WorkingPoint="VeryLooseLHElectron")

#VeryLooseLHElectron, LooseLHElectron, LooseBLLHElectron, MediumLHElectron, TightLHElectron

#photon selection

from ElectronPhotonSelectorTools.ElectronPhotonSelectorToolsConf import AsgPhotonIsEMSelector
myAnaAlg.PhotonSelTool = AsgPhotonIsEMSelector("MyAsgPhotonIsEMSelectorTool", WorkingPoint="LoosePhoton") #Loose Medium TightPhoton


#Btagging
from xAODBTaggingEfficiency.xAODBTaggingEfficiencyConf import BTaggingSelectionTool
myAnaAlg.BtagSelTool = BTaggingSelectionTool("mybtagSelTool",
                                             MaxEta=2.5,
                                             MinPt = 20000.,
                                             JetAuthor="AntiKt4EMPFlowJets_BTagging201903",
                                             TaggerName="DL1r",
                                             OperatingPoint="FixedCutBEff_70",
                                             FlvTagCutDefinitionsFileName="xAODBTaggingEfficiency/13TeV/2020-21-13TeV-MC16-CDI-2020-03-11_v3.root")

from TauAnalysisTools.TauAnalysisToolsConf import TauAnalysisTools__TauSelectionTool
myAnaAlg.TauSelTool = TauAnalysisTools__TauSelectionTool("MyTauSelectionTool") 


#jet calibration
#from JetCalibTools.JetCalibToolsConf import JetCalibrationTool
#myAnaAlg.JetCalibTool = JetCalibrationTool("myCalibTool",
 #                                          JetCollection="AntiKt4EMPFlowJets_BTagging201903",
  #                                         ConfigFile="JES_MC16Recommendation_Consolidated_PFlow_Apr2019_Rel21.config",
   #                                        CalibSequence="JetArea_Residual_EtaJES_GSC_Smear",
    #                                       IsData=False)


#job += MyAnalysisAlg("MyAnalysisAlg1", OutputLevel = DEBUG)
job += myAnaAlg
#job += MyAnalysisAlg("MyAnalysisAlg2", OutputLevel = FATAL)
#job += MyAnalysisAlg("MyAnalysisAlg3", OutputLevel = INFO,
#                     message = "But if you try sometimes, you just might find you get what you need.")

#-----------------------------------------------------------------------------
# save ROOT histograms and Tuple
#-----------------------------------------------------------------------------
from GaudiSvc.GaudiSvcConf import THistSvc
ServiceMgr += THistSvc()
ServiceMgr.THistSvc.Output = [ "MyAnalysisAlg DATAFILE='SignalSample.root ' OPT='RECREATE'" ]
#R_DSampleHistograms.root

##background
#Zboson/ptcut_Zmumu/Zmumu_MAXHTPTV0_70_CFilterBVeto.root


print job
#-----------------------------------------------------------------------------
