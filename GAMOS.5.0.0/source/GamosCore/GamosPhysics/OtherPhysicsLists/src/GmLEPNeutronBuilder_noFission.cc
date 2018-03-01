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
#include "GmLEPNeutronBuilder_noFission.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4ProcessManager.hh"

GmLEPNeutronBuilder_noFission::
GmLEPNeutronBuilder_noFission() 
{
  theMin = 0;
  theIMin = theMin;
  theMax = 20*TeV;
  theIMax = 55*GeV;
}

GmLEPNeutronBuilder_noFission::
~GmLEPNeutronBuilder_noFission() 
{
  //  delete theNeutronFissionModel;
  delete theNeutronCaptureModel;
  delete theLENeutronModel;
}

void GmLEPNeutronBuilder_noFission::
Build(G4HadronElasticProcess *)
{
     G4cout << "Info - GmLEPNeutronBuilder_noFission::Build() not adding elastic" << G4endl;
}

void GmLEPNeutronBuilder_noFission::
Build(G4HadronFissionProcess * )
{
  /*
  theNeutronFissionModel = new G4LFission();
  // theNeutronFissionModel->SetMinEnergy(theMin);
  theNeutronFissionModel->SetMinEnergy(0.);
  theNeutronFissionModel->SetMaxEnergy(theMax);
  aP->RegisterMe(theNeutronFissionModel);
  */
}

void GmLEPNeutronBuilder_noFission::
Build(G4HadronCaptureProcess * aP)
{
  theNeutronCaptureModel = new G4LCapture();
  theNeutronCaptureModel->SetMinEnergy(theMin);
  //  theNeutronCaptureModel->SetMinEnergy(0.);
  theNeutronCaptureModel->SetMaxEnergy(theMax);
  aP->RegisterMe(theNeutronCaptureModel);
}

void GmLEPNeutronBuilder_noFission::
Build(G4NeutronInelasticProcess * aP)
{
  theLENeutronModel = new G4LENeutronInelastic();
  theLENeutronModel->SetMinEnergy(theIMin);
  theLENeutronModel->SetMaxEnergy(theIMax);
  aP->RegisterMe(theLENeutronModel);
}

// 2002 by J.P. Wellisch
