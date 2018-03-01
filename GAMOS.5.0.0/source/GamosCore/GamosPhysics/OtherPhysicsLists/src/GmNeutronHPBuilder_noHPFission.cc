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
#include "GmNeutronHPBuilder_noHPFission.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4ProcessManager.hh"

GmNeutronHPBuilder_noHPFission::
GmNeutronHPBuilder_noHPFission() 
{
  theHPElastic = 0;
  theHPElasticData = 0;
  
  //  theHPFission = 0;
  //  theHPFissionData = 0;
  
  theHPCapture = 0;
  theHPCaptureData = 0;
  
  theHPInelastic = 0;
  theHPInelasticData = 0;
}

GmNeutronHPBuilder_noHPFission::
~GmNeutronHPBuilder_noHPFission() 
{
  delete theHPElasticData;
  //  delete theHPFissionData;
  delete theHPCaptureData;
  delete theHPInelasticData;
}

void GmNeutronHPBuilder_noHPFission::
Build(G4HadronElasticProcess * aP)
{
  if(theHPElastic==0) theHPElastic = new G4NeutronHPElastic;
  if(theHPElasticData == 0) theHPElasticData = new G4NeutronHPElasticData;
  aP->AddDataSet(theHPElasticData);
  aP->RegisterMe(theHPElastic);
}

void GmNeutronHPBuilder_noHPFission::
Build(G4HadronFissionProcess * )
{
  G4cout << " GmNeutronHPBuilder_noHPFission " << G4endl;
  return; //pad
  /*  if(theHPFission == 0) theHPFission = new G4NeutronHPFission;
  if(theHPFissionData==0) theHPFissionData=new G4NeutronHPFissionData;
  aP->AddDataSet(theHPFissionData);
  aP->RegisterMe(theHPFission);
  */
}

void GmNeutronHPBuilder_noHPFission::
Build(G4HadronCaptureProcess * aP)
{
  if(theHPCapture==0) theHPCapture = new G4NeutronHPCapture;
  if(theHPCaptureData==0) theHPCaptureData = new G4NeutronHPCaptureData;
  aP->AddDataSet(theHPCaptureData);
  aP->RegisterMe(theHPCapture);
}

void GmNeutronHPBuilder_noHPFission::
Build(G4NeutronInelasticProcess * aP)
{
  if(theHPInelastic==0) theHPInelastic = new G4NeutronHPInelastic;
  if(theHPInelasticData==0) theHPInelasticData = new G4NeutronHPInelasticData;
  aP->AddDataSet(theHPInelasticData);
  aP->RegisterMe(theHPInelastic);
}
// 2002 by J.P. Wellisch
