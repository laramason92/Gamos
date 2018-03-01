#------------------------------------------------------------------------------
# 
# Module : Detector
# Package: ComptonCamera
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
GEANT4_DEFINE_MODULE(NAME Detector_ComptonCamera

   HEADERS
       CCRecHitSet.hh
       CCVerbosity.hh
       CCIOMgr.hh
       CCEventClassifierUA.hh

   SOURCES
       plugin.cc
       CCRecHitSet.cc
       CCVerbosity.cc
       CCEventClassifierUA.cc
       CCIOMgr.cc
 
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
