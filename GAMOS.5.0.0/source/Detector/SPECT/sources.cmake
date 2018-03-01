#------------------------------------------------------------------------------
# 
# Module : Detector
# Package: SPECT
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
GEANT4_DEFINE_MODULE(NAME Detector_SPECT

   HEADERS
       SPECTVerbosity.hh
       SPECTEventClassifierUA.hh
       SPECTIOMgr.hh

   SOURCES
       plugin.cc
       SPECTVerbosity.cc
       SPECTEventClassifierUA.cc
       SPECTIOMgr.cc
 
   GRANULAR_DEPENDENCIES

   GLOBAL_DEPENDENCIES
    ${Geant4_LIBRARIES}
    ${Root_LIBRARIES}
  GamosCore_GamosAnalysis
  GamosCore_GamosGeometry
  GamosCore_GamosUtils
  GamosCore_GamosMovement
  GamosCore_GamosUserActionMgr
  GamosCore_GamosBase_Base
  GamosCore_GamosSD
  Detector_Base

   LINK_LIBRARIES
)

# List any source specific properties here
