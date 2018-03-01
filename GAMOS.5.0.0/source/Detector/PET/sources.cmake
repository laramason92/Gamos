#------------------------------------------------------------------------------
# 
# Module : Detector
# Package: PET
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
GEANT4_DEFINE_MODULE(NAME Detector_PET

   HEADERS
       PETVerbosity.hh
       PETVComptClassificationVariable.hh
       PETPrintPositron.hh
       PETHistosPositron.hh
       PETIOMgr.hh
       PETProjDataMgr.hh
       PETEventClassifierUA.hh
       PETComptonStudyHistosUA-.hh
       PETVComptAlgorithmVariable.hh

   SOURCES
       plugin.cc
       PETIOMgr.cc
       PETPrintPositron.cc
       PETEventClassifierUA.cc
       PETProjDataMgr.cc
       PETHistosPositron.cc
       PETVerbosity.cc
 
   GRANULAR_DEPENDENCIES

   GLOBAL_DEPENDENCIES
    ${Geant4_LIBRARIES}
    ${Root_LIBRARIES}
  GamosCore_GamosUserActionMgr
  GamosCore_GamosBase_Base
  GamosCore_GamosUtils
  GamosCore_GamosSD
  GamosCore_GamosAnalysis
  Detector_Base

   LINK_LIBRARIES
)

# List any source specific properties here
