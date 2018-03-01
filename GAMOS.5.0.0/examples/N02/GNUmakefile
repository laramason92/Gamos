name = exampleN02
G4TARGET := $(name)
G4EXLIB := true

.PHONY: all
all: lib plugin

include $(GAMOSINSTALL)/config/binmake.gmk
include $(GAMOSINSTALL)/config/general.gmk

EXTRALIBS += -lGamosBase_Base -lGamosUtils -lGamosUserActionMgr -lGamosGeometry -lGamosGenerator -lGamosPhysics_PhysicsList -lGamosSD
