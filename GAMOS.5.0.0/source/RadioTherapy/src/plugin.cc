//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  GAMOS software  is  copyright of the Copyright  Holders  of *
// * the GAMOS Collaboration.  It is provided  under  the  terms  and *
// * conditions of the GAMOS Software License,  included in the  file *
// * LICENSE and available at  http://fismed.ciemat.es/GAMOS/license .*
// * These include a list of copyright holders.                       *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GAMOS collaboration.                       *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the GAMOS Software license.           *
// ********************************************************************
//
#include "Reflex/PluginService.h"

#include "RTPhaseSpaceUA.hh"
#include "RTGeneratorPhaseSpace.hh"
#include "RTZRLimitsAutoUA.hh"
#include "RTZRLimitsUA.hh"
#include "RTVerbosity.hh"
#include "RTPSPDoseHistos.hh"
#include "RTReuseAtZPlaneUA.hh"

#include "RTProdCutsStudyUA.hh"
#include "RTMinRangeLimitsStudyUA.hh"
#include "RTCutsStudyFilter.hh"
#include "RTClassifierByEILong.hh"
#include "RTExtraInfoProviderOrigin.hh"
#include "RTExtraInfoProviderInteractions.hh"
#include "RTExtraInfoProviderCrossings.hh"
#include "RTExtraInfoProviderZLast.hh"
#include "RTExtraInfoProviderZOrigin.hh"
#include "RTFilterByEILong.hh"

PLUGINSVC_FACTORY(RTPhaseSpaceUA,GmUserAction*())
PLUGINSVC_FACTORY(RTGeneratorPhaseSpace,G4VUserPrimaryGeneratorAction*())
PLUGINSVC_FACTORY(RTVerbosity,GmVVerbosity*())
PLUGINSVC_FACTORY(RTZRLimitsAutoUA,GmUserAction*())
PLUGINSVC_FACTORY(RTZRLimitsUA,GmUserAction*())
PLUGINSVC_FACTORY(RTPSPDoseHistos,GmVPSPrinter*(G4String))
PLUGINSVC_FACTORY(RTReuseAtZPlaneUA,GmUserAction*())

PLUGINSVC_FACTORY(RTProdCutsStudyUA,GmUserAction*())
PLUGINSVC_FACTORY(RTMinRangeLimitsStudyUA,GmUserAction*())
PLUGINSVC_FACTORY(RTCutsStudyFilter,GmVFilter*(G4String))

PLUGINSVC_FACTORY(RTExtraInfoProviderOrigin,GmUserAction*())
PLUGINSVC_FACTORY(RTExtraInfoProviderInteractions,GmUserAction*())
PLUGINSVC_FACTORY(RTExtraInfoProviderCrossings,GmUserAction*())
PLUGINSVC_FACTORY(RTExtraInfoProviderZLast,GmUserAction*())
PLUGINSVC_FACTORY(RTExtraInfoProviderZOrigin,GmUserAction*())
PLUGINSVC_FACTORY(RTClassifierByEILong,GmVClassifier*(G4String))
PLUGINSVC_FACTORY(RTFilterByEILong,GmVFilter*(G4String))
