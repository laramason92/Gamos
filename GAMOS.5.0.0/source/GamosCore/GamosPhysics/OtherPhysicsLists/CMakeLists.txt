#------------------------------------------------------------------------------
# 
# Module : GamosCore
# Package: GamosPhysics_OtherPhysicsLists
#
#------------------------------------------------------------------------------
include(Geant4MacroLibraryTargets)

if(GAMOS_BUILD_GRANULAR_LIBS)
   GAMOS_GRANULAR_LIBRARY_TARGET(COMPONENT sources.cmake)
else()
   GEANT4_GLOBAL_LIBRARY_TARGET(COMPONENTS sources.cmake)
endif()
