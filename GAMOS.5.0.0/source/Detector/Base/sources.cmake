#------------------------------------------------------------------------------
# 
# Module : Detector
# Package: Base
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
GEANT4_DEFINE_MODULE(NAME Detector_Base

   HEADERS
       DetCClassifALL.hh
       DetCClassifEnergyMin.hh
       DetCClassifYZPosMin.hh
       DetCAlgoZPos.hh
       DetRecHitSet.hh
       DetCAlgoXZPos.hh
       Det1stHitByXYPos.hh
       DetVComptAlgorithmVariable.hh
       Det1stHitByComptonCone.hh
       DetCClassifEnergyAverage.hh
       DetCClassifXPosMin.hh
       DetRecHitCone.hh
       DetCAlgoXPos.hh
       DetCutsStudyFilter.hh
       DetCountScatteringUA.hh
       DetCClassifZPosMin.hh
       DetCClassifEnergyMax.hh
       DetCAlgoYPos.hh
       DetVComptClassificationVariable.hh
       Det1stHitByEnergy.hh
       DetHistosGammaDist.hh
       DetCAlgoXYPos.hh
       Det1stHitByXYZPos.hh
       Det1stHitByXPos.hh
       DetCClassifXYPosMin.hh
       DetV1stHitAlgorithm.hh
       DetCClassifYPosMin.hh
       DetCClassifNRecHit.hh
       DetCClassifXZPosMin.hh
       DetCAlgoXYZPos.hh
       DetVerbosity.hh
       DetComptonStudyHistosUA.hh
       DetCAlgoYZPos.hh
       DetCAlgoEnergy.hh
       DetCClassifXYZPosMin.hh
       Det1stHitByDistanceToOther.hh

   SOURCES
       plugin.cc
       DetCAlgoEnergy.cc
       DetCClassifXPosMin.cc
       DetCClassifEnergyMin.cc
       DetCClassifXYZPosMin.cc
       DetRecHitSet.cc
       DetCClassifXZPosMin.cc
       DetCountScatteringUA.cc
       DetCAlgoZPos.cc
       DetCAlgoXYZPos.cc
       DetV1stHitAlgorithm.cc
       DetCClassifNRecHit.cc
       Det1stHitByEnergy.cc
       DetVComptClassificationVariable.cc
       DetCClassifALL.cc
       DetCClassifYZPosMin.cc
       Det1stHitByDistanceToOther.cc
       DetComptonStudyHistosUA.cc
       DetCClassifZPosMin.cc
       DetHistosGammaDist.cc
       DetCClassifYPosMin.cc
       DetCClassifXYPosMin.cc
       DetCClassifEnergyAverage.cc
       DetCAlgoXZPos.cc
       Det1stHitByXYPos.cc
       DetCAlgoYZPos.cc
       DetVComptAlgorithmVariable.cc
       DetCutsStudyFilter.cc
       DetCAlgoYPos.cc
       DetCAlgoXYPos.cc
       DetCClassifEnergyMax.cc
       DetCAlgoXPos.cc
       DetRecHitCone.cc
       Det1stHitByComptonCone.cc
       DetVerbosity.cc
       Det1stHitByXYZPos.cc
       Det1stHitByXPos.cc
 
   GRANULAR_DEPENDENCIES

   GLOBAL_DEPENDENCIES
    ${Geant4_LIBRARIES}
    ${Root_LIBRARIES}
  GamosCore_GamosAnalysis
  GamosCore_GamosPhysics_Cuts
  GamosCore_GamosGeometry
  GamosCore_GamosUtils
  GamosCore_GamosUserActionMgr
  GamosCore_GamosBase_Base
  GamosCore_GamosSD

   LINK_LIBRARIES
)

# List any source specific properties here
