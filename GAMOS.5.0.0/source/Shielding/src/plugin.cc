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

#include "SHNthValueLayerUA.hh"
#include "SHActivationUA.hh"
#include "SHActivationFromFluxUA.hh"
#include "SHGetHadronicChannelXSUA.hh"
#include "SHPrintPHPChannelXSUA.hh"
#include "SHStudyDecayChannelsUA.hh"
#include "SHPlotPHPDBSpectraUA.hh"
#include "SHPrintEMDataUA.hh"
#include "SHPrintPHPChannelThickYieldUA.hh"
  
 PLUGINSVC_FACTORY(SHNthValueLayerUA,GmUserAction*())
PLUGINSVC_FACTORY(SHActivationUA,GmUserAction*())
PLUGINSVC_FACTORY(SHActivationFromFluxUA,GmUserAction*())
PLUGINSVC_FACTORY(SHGetHadronicChannelXSUA,GmUserAction*())
PLUGINSVC_FACTORY(SHPrintPHPChannelXSUA,GmUserAction*())
PLUGINSVC_FACTORY(SHStudyDecayChannelsUA,GmUserAction*())
PLUGINSVC_FACTORY(SHPlotPHPDBSpectraUA,GmUserAction*())
PLUGINSVC_FACTORY(SHPrintEMDataUA,GmUserAction*())
PLUGINSVC_FACTORY(SHPrintPHPChannelThickYieldUA,GmUserAction*())

#include "GmNoUseG4RadDecayTimeUA.hh"
#include "GmRadDecayGammaAngleCorrelationUA.hh"

PLUGINSVC_FACTORY(GmNoUseG4RadDecayTimeUA,GmUserAction*())
PLUGINSVC_FACTORY(GmRadDecayGammaAngleCorrelationUA,GmUserAction*())

#include "ShieldingVerbosity.hh"
PLUGINSVC_FACTORY(ShieldingVerbosity,GmVVerbosity*())
