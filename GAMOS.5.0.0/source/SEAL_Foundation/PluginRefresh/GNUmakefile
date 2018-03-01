name := SealPluginRefresh
G4TARGET := $(name)
G4EXLIB := true

.PHONY: all
all: lib bin

include $(GAMOSINSTALL)/config/binmake.gmk
include $(GAMOSINSTALL)/config/gamos.gmk
include $(GAMOSINSTALL)/config/seal.gmk

#
EXTRALIBS += -llcg_SealBase -llcg_PluginManager

