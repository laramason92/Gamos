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

#include "GmQGSP_BERT_HP_noHPFission.hh"
#include "GmQGSP_BERT_HP_noFission.hh"
#include "GmDummyPhysics.hh"
PLUGINSVC_FACTORY(GmDummyPhysics,G4VUserPhysicsList*())

//#include "GmQGSP_BIC_PHP.hh"
//PLUGINSVC_FACTORY(GmQGSP_BIC_PHP,G4VUserPhysicsList*())

#include "GmG4PhysicsLists.hh"
PLUGINSVC_FACTORY(G4CHIPS,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4CHIPS_HP,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4FTF_BIC,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4FTFP_BERT_EMV,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4FTFP_BERT_EMX,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4FTFP_BERT,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4FTFP_BERT_TRV,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4LBE,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4LHEP_EMV,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4LHEP,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QBBC,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGS_BIC,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSC_BERT,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSC_CHIPS,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSP_BERT_CHIPS,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSP_BERT_EMV,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSP_BERT_EMX,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSP_BERT,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSP_BERT_HP,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSP_BERT_NOLEP,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSP_BERT_TRV,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSP_BIC_EMY,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSP_BIC,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSP_BIC_HP,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSP_FTFP_BERT,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSP,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSP_QEL,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSP_INCLXX,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4CHIPS_HP,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4FTFP_BERT_HP,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSP_BERT_95,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSP_BERT_95XS,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSP_FTFP_BERT_95,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSP_FTFP_BERT_95XS,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4Shielding,G4VUserPhysicsList*())

PLUGINSVC_FACTORY(G4QGSP_BIC_PHP,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSP_BIC_NHP,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSP_BIC_AllHP,G4VUserPhysicsList*())


