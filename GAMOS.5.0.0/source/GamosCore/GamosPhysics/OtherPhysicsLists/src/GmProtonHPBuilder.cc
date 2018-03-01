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
#include "GmProtonHPBuilder.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4ProcessManager.hh"

GmProtonHPBuilder::
GmProtonHPBuilder() 
{
  theMin = 0;
  theMax=200.*MeV;
}

GmProtonHPBuilder::
~GmProtonHPBuilder() 
{
  delete theParticleModel;
}

void GmProtonHPBuilder::
Build(G4HadronElasticProcess *)
{
  G4cout << "Info - GmProtonHPBuilder::Build() not adding elastic" << G4endl;
}

void GmProtonHPBuilder::
Build(G4ProtonInelasticProcess * aP)
{
// G4cout << "adding inelastic Proton in LHEP" << G4endl;
  theParticleModel = new G4ParticleHPInelastic(G4Proton::Proton(),"G4PROTONHPDATA");
  theParticleModel->SetMinEnergy(theMin);
  theParticleModel->SetMaxEnergy(theMax);
  aP->RegisterMe(theParticleModel);
  //  theParticleModel->DumpInfo();
}

// 2002 by J.P. Wellisch
