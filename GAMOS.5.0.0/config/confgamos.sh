#!/bin/bash
export G4SYSTEM=Darwin-g++

#--- GAMOS variables
export GAMOSDIR=/Users/laramason/Downloads/GAMOS
export GAMOS_VER=GAMOS.5.0.0
export GAMOSINSTALL=$GAMOSDIR/$GAMOS_VER
export GAMOSEXTERNAL=$GAMOSDIR/external
export GAMOS_SEARCH_PATH=.:$GAMOSINSTALL/data

export PATH="${GAMOSINSTALL}/bin/${G4SYSTEM}:${PATH}"

#--- GEANT4 variables
export G4INSTALL=${GAMOSEXTERNAL}/geant4/geant4.9.6.p02.gamos
export G4TMP=$GAMOSINSTALL/tmp
export G4BIN=$GAMOSINSTALL/bin
export G4NEUTRONHPDATA=$G4INSTALL/../data/G4NDL4.2
export G4NEUTRONXSDATA=$G4INSTALL/../data/G4NEUTRONXS1.3
export G4LEVELGAMMADATA=$G4INSTALL/../data/PhotonEvaporation2.4
export G4RADIOACTIVEDATA=$G4INSTALL/../data/RadioactiveDecay3.6
export G4LEDATA=$G4INSTALL/../data/G4EMLOW6.32
export G4ABLADATA=$G4INSTALL/../data/G4ABLA3.0
export G4PIIDATA=$G4INSTALL/../data/G4PII1.3
export G4REALSURFACEDATA=$G4INSTALL/../data/RealSurface1.0
export G4SAIDXSDATA=$G4INSTALL/../data/G4SAIDDATA1.1
export G4PROTONHPDATA=$G4INSTALL/../data/TENDL-2012_proton
export G4DEUTERONHPDATA=$G4INSTALL/../data/TENDL-2012_deuteron
export G4TRITONHPDATA=$G4INSTALL/../data/TENDL-2012_triton
export G4HE3HPDATA=$G4INSTALL/../data/TENDL-2012_He3
export G4ALPHAHPDATA=$G4INSTALL/../data/TENDL-2012_alpha
export G4PARTICLEHP_DO_NOT_ADJUST_FINAL_STATE=1
export G4NEUTRONHP_DO_NOT_ADJUST_FINAL_STATE=1

export G4LIB_BUILD_SHARED=1
 
#--- SEAL variables
export SEAL_BASE_DIR=${GAMOSINSTALL}
export SEAL_PLUGIN_DIR=${GAMOSINSTALL}/module/$G4SYSTEM
export SEAL_KEEP_MODULES=true
export SEAL_PLUGINS="${GAMOSINSTALL}/module/${G4SYSTEM}:${SEAL_BASE_DIR}/${G4SYSTEM}/lib/modules"

#--- ROOT variables
export ROOT_BASE_DIR=${GAMOSEXTERNAL}/root/5.34.34/root
export PATH="${ROOT_BASE_DIR}/bin:${PATH}"
export ROOTSYS=${ROOT_BASE_DIR}

#--- FFTW variables
export FFTW_BASE_DIR=${GAMOSEXTERNAL}/fftw/fftw-3.2.2
# by default do not use it
export GAMOS_NO_FFTW=1

#export G4DEBUG=1
#export G4NEUTRONHP_NEGLECT_DOPPLER=1

source ${GAMOSINSTALL}/config/gamosvis.sh

export AllowForHeavyElements=1


#export NeutronHPNames=1

if [ "$G4SYSTEM" == "Linux-g++"  ] ;  then
#--- Initialize LD_LIBRARY_PATH
  if [ "$LD_LIBRARY_PATH" == "" ] ; then
    export LD_LIBRARY_PATH=""
  fi
  export LD_LIBRARY_PATH="${GAMOSINSTALL}/lib/${G4SYSTEM}:${LD_LIBRARY_PATH}"

  export LD_LIBRARY_PATH="${G4INSTALL}/lib/${G4SYSTEM}:${LD_LIBRARY_PATH}"
  export LD_LIBRARY_PATH="${ROOT_BASE_DIR}/lib:${LD_LIBRARY_PATH}"
  export LD_LIBRARY_PATH="/usr/lib:/usr/local/lib:/usr/local/lib/X11:/usr/explorer/lib:/usr/lib/Motif1.2:/usr/X11R6/include/X11:${LD_LIBRARY_PATH}"
elif [ "$G4SYSTEM" == "Darwin-g++"  ] ;  then
#--- Initialize DYLD_LIBRARY_PATH
  if [ "$DYLD_LIBRARY_PATH" == "" ] ; then
    export DYLD_LIBRARY_PATH=""
  fi
  export DYLD_LIBRARY_PATH="${GAMOSINSTALL}/lib/${G4SYSTEM}:${DYLD_LIBRARY_PATH}"

  export DYLD_LIBRARY_PATH="${G4INSTALL}/lib/${G4SYSTEM}:${DYLD_LIBRARY_PATH}"
  export DYLD_LIBRARY_PATH="${ROOT_BASE_DIR}/lib:${DYLD_LIBRARY_PATH}"
  export DYLD_LIBRARY_PATH="/usr/lib:/usr/local/lib:/usr/local/lib/X11:/usr/explorer/lib:/usr/lib/Motif1.2:/usr/X11R6/include/X11:${DYLD_LIBRARY_PATH}"
fi
