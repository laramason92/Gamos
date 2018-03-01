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

#include "DetHistosGammaDist.hh"
#include "DetComptonStudyHistosUA.hh"

PLUGINSVC_FACTORY(DetHistosGammaDist,GmUserAction*())
PLUGINSVC_FACTORY(DetComptonStudyHistosUA,GmUserAction*())

#include "DetCutsStudyFilter.hh"
PLUGINSVC_FACTORY(DetCutsStudyFilter,GmVFilter*(G4String))

#include "Det1stHitByEnergy.hh"
#include "Det1stHitByXPos.hh"
#include "Det1stHitByXYPos.hh"
#include "Det1stHitByXYZPos.hh"
#include "Det1stHitByComptonCone.hh"
#include "Det1stHitByDistanceToOther.hh"

PLUGINSVC_FACTORY(Det1stHitByEnergy,DetV1stHitAlgorithm*())
PLUGINSVC_FACTORY(Det1stHitByXPos,DetV1stHitAlgorithm*())
PLUGINSVC_FACTORY(Det1stHitByXYPos,DetV1stHitAlgorithm*())
PLUGINSVC_FACTORY(Det1stHitByXYZPos,DetV1stHitAlgorithm*())
PLUGINSVC_FACTORY(Det1stHitByComptonCone,DetV1stHitAlgorithm*())
PLUGINSVC_FACTORY(Det1stHitByDistanceToOther,DetV1stHitAlgorithm*())

#include "DetCAlgoEnergy.hh" 
#include "DetCAlgoXPos.hh" 
#include "DetCAlgoYPos.hh" 
#include "DetCAlgoZPos.hh" 
#include "DetCAlgoXYPos.hh" 
#include "DetCAlgoXZPos.hh" 
#include "DetCAlgoYZPos.hh" 
#include "DetCAlgoXYZPos.hh" 

PLUGINSVC_FACTORY(DetCAlgoEnergy,DetVComptAlgorithmVariable*())
PLUGINSVC_FACTORY(DetCAlgoXPos,DetVComptAlgorithmVariable*())
PLUGINSVC_FACTORY(DetCAlgoYPos,DetVComptAlgorithmVariable*())
PLUGINSVC_FACTORY(DetCAlgoZPos,DetVComptAlgorithmVariable*())
PLUGINSVC_FACTORY(DetCAlgoXYPos,DetVComptAlgorithmVariable*())
PLUGINSVC_FACTORY(DetCAlgoXZPos,DetVComptAlgorithmVariable*())
PLUGINSVC_FACTORY(DetCAlgoYZPos,DetVComptAlgorithmVariable*())
PLUGINSVC_FACTORY(DetCAlgoXYZPos,DetVComptAlgorithmVariable*())

#include "DetCClassifNRecHit.hh" 
#include "DetCClassifEnergyMin.hh" 
#include "DetCClassifEnergyMax.hh" 
#include "DetCClassifEnergyAverage.hh" 
#include "DetCClassifXPosMin.hh" 
#include "DetCClassifYPosMin.hh" 
#include "DetCClassifZPosMin.hh" 
#include "DetCClassifXYPosMin.hh" 
#include "DetCClassifXZPosMin.hh" 
#include "DetCClassifYZPosMin.hh" 
#include "DetCClassifXYZPosMin.hh" 

PLUGINSVC_FACTORY(DetCClassifNRecHit,DetVComptClassificationVariable*())
PLUGINSVC_FACTORY(DetCClassifEnergyMin,DetVComptClassificationVariable*())
PLUGINSVC_FACTORY(DetCClassifEnergyMax,DetVComptClassificationVariable*())
PLUGINSVC_FACTORY(DetCClassifEnergyAverage,DetVComptClassificationVariable*())
PLUGINSVC_FACTORY(DetCClassifXPosMin,DetVComptClassificationVariable*())
PLUGINSVC_FACTORY(DetCClassifYPosMin,DetVComptClassificationVariable*())
PLUGINSVC_FACTORY(DetCClassifZPosMin,DetVComptClassificationVariable*())
PLUGINSVC_FACTORY(DetCClassifXYPosMin,DetVComptClassificationVariable*())
PLUGINSVC_FACTORY(DetCClassifXZPosMin,DetVComptClassificationVariable*())
PLUGINSVC_FACTORY(DetCClassifYZPosMin,DetVComptClassificationVariable*())
PLUGINSVC_FACTORY(DetCClassifXYZPosMin,DetVComptClassificationVariable*())

#include "DetVerbosity.hh"
PLUGINSVC_FACTORY(DetVerbosity,GmVVerbosity*())

