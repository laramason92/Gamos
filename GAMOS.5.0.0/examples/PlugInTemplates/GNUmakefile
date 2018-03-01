name := PlugInTemplates
G4TARGET := $(name)
G4EXLIB := true


ifndef G4INSTALL
  G4INSTALL = ../../..
endif

.PHONY: all
all: lib plugin

include $(GAMOSINSTALL)/config/binmake.gmk
include $(GAMOSINSTALL)/config/general.gmk

EXTRALIBS += -lGamosAnalysis -lGamosBase -lGamosFactories -lGamosGenerator -lGamosGeometry -lGamosMovement -lGamosPhysics_OtherPhysicsLists -lGamosPhysics_PhysicsList -lGamosPhysics_Cuts -lGamosPhysics_VarianceReduction -lGamosReadDICOM -lGamosScoring -lGamosSD -lGamosUserActionMgr -lGamosUtils -lGamosUtilsUA
