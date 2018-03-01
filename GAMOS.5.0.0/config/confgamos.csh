setenv G4SYSTEM Darwin-g++

#--- GAMOS variables
setenv GAMOSDIR /Users/laramason/Downloads/GAMOS
setenv GAMOS_VER GAMOS.5.0.0
setenv GAMOSINSTALL $GAMOSDIR/$GAMOS_VER
setenv GAMOSEXTERNAL $GAMOSDIR/external
setenv GAMOS_SEARCH_PATH .:$GAMOSINSTALL/data
setenv PATH "${GAMOSINSTALL}/bin/${G4SYSTEM}:${PATH}"

#--- GEANT4 variables
setenv G4INSTALL ${GAMOSEXTERNAL}/geant4/geant4.9.6.p02.gamos
setenv G4TMP $GAMOSINSTALL/tmp
setenv G4BIN $GAMOSINSTALL/bin
setenv G4NEUTRONHPDATA $G4INSTALL/../data/G4NDL4.2
setenv G4NEUTRONXSDATA $G4INSTALL/../data/G4NEUTRONXS1.3
setenv G4LEVELGAMMADATA $G4INSTALL/../data/PhotonEvaporation2.3
setenv G4RADIOACTIVEDATA $G4INSTALL/../data/RadioactiveDecay3.6
setenv G4LEDATA $G4INSTALL/../data/G4EMLOW6.32
setenv G4ABLADATA $G4INSTALL/../data/G4ABLA3.0
setenv G4PIIDATA $G4INSTALL/../data/G4PII1.3
setenv G4REALSURFACEDATA $G4INSTALL/../data/RealSurface1.0
setenv G4SAIDXSDATA $G4INSTALL/../data/G4SAIDDATA1.1

setenv G4PROTONHPDATA $G4INSTALL/../data/TENDL-2012_proton
setenv G4DEUTERONHPDATA $G4INSTALL/../data/TENDL-2012_deuteron
setenv G4TRITONHPDATA $G4INSTALL/../data/TENDL-2012_triton
setenv G4HE3HPDATA $G4INSTALL/../data/TENDL-2012_He3
setenv G4ALPHAHPDATA $G4INSTALL/../data/TENDL-2012_alpha
setenv G4PHP_DO_NOT_ADJUST_FINAL_STATE 1
setenv G4NEUTRONHP_DO_NOT_ADJUST_FINAL_STATE 1
setenv G4LIB_BUILD_SHARED 1
 
#--- SEAL variables
setenv SEAL_BASE_DIR ${GAMOSINSTALL}
setenv SEAL_PLUGIN_DIR ${GAMOSINSTALL}/module/$G4SYSTEM
setenv SEAL_KEEP_MODULES true
setenv SEAL_PLUGINS "${GAMOSINSTALL}/module/${G4SYSTEM}:${SEAL_BASE_DIR}/${G4SYSTEM}/lib/modules"

#--- ROOT variables
setenv ROOT_BASE_DIR  ${GAMOSEXTERNAL}/root/5.34.34/root
setenv PATH "${ROOT_BASE_DIR}/bin:${PATH}"
setenv ROOTSYS ${ROOT_BASE_DIR}

#--- FFTW variables
setenv FFTW_BASE_DIR ${GAMOSEXTERNAL}/fftw/fftw-3.2.2
# by default do not use it
setenv GAMOS_NO_FFTW 1

#setenv G4DEBUG 1
#setenv G4NEUTRONHP_NEGLECT_DOPPLER 1
source ${GAMOSINSTALL}/config/gamosvis.csh

setenv AllowForHeavyElements 1

#setenv NeutronHPNames 1

if ( "$G4SYSTEM" == "Linux-g++"  )  then
#--- Initialize LD_LIBRARY_PATH
  if ("$?LD_LIBRARY_PATH" == 0) then
    setenv LD_LIBRARY_PATH ""
  endif
  setenv LD_LIBRARY_PATH "${GAMOSINSTALL}/lib/${G4SYSTEM}:${LD_LIBRARY_PATH}"

  setenv LD_LIBRARY_PATH "${G4INSTALL}/lib/${G4SYSTEM}:${LD_LIBRARY_PATH}"
  setenv LD_LIBRARY_PATH "${ROOT_BASE_DIR}/lib:${LD_LIBRARY_PATH}"
  setenv LD_LIBRARY_PATH "/usr/lib:/usr/local/lib:/usr/local/lib/X11:/usr/explorer/lib:/usr/lib/Motif1.2:/usr/X11R6/include/X11:${LD_LIBRARY_PATH}"
else if ( "$G4SYSTEM" == "Darwin-g++"  )  then
#--- Initialize DYLD_LIBRARY_PATH
  if ( "$DYLD_LIBRARY_PATH" == "" ) then
    setenv DYLD_LIBRARY_PATH ""
  endif
  setenv DYLD_LIBRARY_PATH "${GAMOSINSTALL}/lib/${G4SYSTEM}:${DYLD_LIBRARY_PATH}"

  setenv DYLD_LIBRARY_PATH "${G4INSTALL}/lib/${G4SYSTEM}:${DYLD_LIBRARY_PATH}"
  setenv DYLD_LIBRARY_PATH "${ROOT_BASE_DIR}/lib:${DYLD_LIBRARY_PATH}"
  setenv DYLD_LIBRARY_PATH "/usr/lib:/usr/local/lib:/usr/local/lib/X11:/usr/explorer/lib:/usr/lib/Motif1.2:/usr/X11R6/include/X11:${DYLD_LIBRARY_PATH}"
endif

setenv G4DEBUG 1
setenv G4VRML_PV_PICKABLE 1
