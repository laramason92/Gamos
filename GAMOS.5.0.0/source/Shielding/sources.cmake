#------------------------------------------------------------------------------
# 
# Module : Shielding
# Package: 
#
#------------------------------------------------------------------------------
# List external includes needed.

# List external includes needed.
include(GamosSetup)
include(UseRoot)

#
# Define the GAMOS Module.
#
include(Geant4MacroDefineModule)
GEANT4_DEFINE_MODULE(NAME Shielding

   HEADERS
       GmRadDecayGammaAngleCorrelationUA.hh
       SHRadDecayScorerFromFlux.hh
       SHPrintPHPChannelXSUA.hh
       GmNoUseG4RadDecayTimePrimaryUA.hh
       SHRadDecayChainSet.hh
       SHMetastableIsotMgr.hh
       GmNoUseG4RadDecayTimeUA.hh
       SHActivationUA.hh
       SHPrintEMDataUA.hh
       SHGetHadronicChannelXSUA.hh
       SHNthValueLayerUA.hh
       SHActivationFromFluxUA.hh
       SHDecayChainBuilder.hh
       SHPlotPHPDBSpectraUA.hh
       SHDecayChains.hh
       ShieldingVerbosity.hh
       SHPrintPHPChannelThickYieldUA.hh
       SHStudyDecayChannelsUA.hh
       SHUtils.hh
       SHRadDecayChain.hh

   SOURCES
       SHPrintEMDataUA.cc
       plugin.cc
       SHPlotPHPDBSpectraUA.cc
       GmNoUseG4RadDecayTimePrimaryUA.cc
       SHActivationUA.cc
       SHRadDecayChainBuilder.cc
       SHRadDecayChainSet.cc
       SHPrintPHPChannelXSUA.cc
       SHStudyDecayChannelsUA.cc
       SHRadDecayChain.cc
       SHNthValueLayerUA.cc
       GmRadDecayGammaAngleCorrelationUA.cc
       GmNoUseG4RadDecayTimeUA.cc
       SHRadDecayScorerFromFlux.cc
       SHMetastableIsotMgr.cc
       SHPrintPHPChannelThickYieldUA.cc
       ShieldingVerbosity.cc
       SHUtils.cc
       SHGetHadronicChannelXSUA.cc
       SHActivationFromFluxUA.cc
 
   GRANULAR_DEPENDENCIES

   GLOBAL_DEPENDENCIES
    ${Geant4_LIBRARIES}
    ${Root_LIBRARIES}
  GamosCore_GamosAnalysis
  GamosCore_GamosGeometry
  GamosCore_GamosUtils
  GamosCore_GamosUtilsUA
  GamosCore_GamosUserActionMgr
  GamosCore_GamosBase_Base
  GamosCore_GamosGenerator

   LINK_LIBRARIES
)

# List any source specific properties here
