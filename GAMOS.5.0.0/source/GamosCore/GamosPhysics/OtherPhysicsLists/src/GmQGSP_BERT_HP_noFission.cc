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
#include "GmQGSP_BERT_HP_noFission.hh"
#include "GmHadronPhysicsQGSP_BERT_HP_noFission.hh"

#include "globals.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleWithCuts.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4ios.hh"
#include <iomanip>   

#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4QStoppingPhysics.hh"
#include "G4HadronElasticPhysics.hh"

#include "G4DataQuestionaire.hh"

GmQGSP_BERT_HP_noFission::GmQGSP_BERT_HP_noFission():  G4VModularPhysicsList()
{
  // default cut value  (1.0mm) 
  // defaultCutValue = 1.0*mm;
  G4DataQuestionaire it(photon, neutron);
  //  G4cout << "<<< Geant4 Physics List simulation engine: GmQGSP_BERT_HP_noFission 3.0"<<G4endl;
  G4cout <<G4endl;
  this->defaultCutValue = 0.7*mm;  

  G4int ver = 1; // verbosity
  // EM Physics
  this->RegisterPhysics( new G4EmStandardPhysics(ver,"standard EM"));

  // Synchroton Radiation & GN Physics
  this->RegisterPhysics( new G4EmExtraPhysics("extra EM"));

  // Decays
  this->RegisterPhysics( new G4DecayPhysics("decay",ver) );

    // Hadron Elastic scattering
  this-> RegisterPhysics( new G4HadronElasticPhysics("elastic",ver,true));

  // Hadron Physics
  //  G4cout << " call GmHadronPhysicsQGSP_BERT_HP_noFission " << G4endl;
  this->RegisterPhysics( new GmHadronPhysicsQGSP_BERT_HP_noFission("hadron"));

  // Stopping Physics
  this->RegisterPhysics( new G4QStoppingPhysics("stopping"));

  // Ion Physics
  this->RegisterPhysics( new G4IonPhysics("ion"));
}

GmQGSP_BERT_HP_noFission::~GmQGSP_BERT_HP_noFission()
{
}

void GmQGSP_BERT_HP_noFission::SetCuts()
{
  if (this->verboseLevel >1){
    G4cout << "GmQGSP_BERT_HP_noFission::SetCuts:";
  }  
  //  " G4VUserPhysicsList::SetCutsWithDefault" method sets 
  //   the default cut value for all particle types 

  this->SetCutsWithDefault();   
 
//  if (this->verboseLevel >0)
//    G4VUserPhysicsList::DumpCutValuesTable();  
  
}



// 2002 by J.P. Wellisch
