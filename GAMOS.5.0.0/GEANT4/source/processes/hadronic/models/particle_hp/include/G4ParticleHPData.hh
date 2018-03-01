//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: G4ParticleHPData.hh,v 1.1 2012/10/02 17:20:39 arce Exp $
// GEANT4 tag $Name:  $
//
 // Hadronic Process: Very Low Energy Neutron X-Sections
 // original by H.P. Wellisch, TRIUMF, 14-Feb-97
 // Has the Cross-section data for all materials.
 
#ifndef G4ParticleHPData_h
#define G4ParticleHPData_h 1
#include "globals.hh"
#include "G4Element.hh"
#include "G4ParticleHPElasticData.hh"
#include "G4ParticleHPInelasticData.hh"
#include "G4ParticleHPFissionData.hh"
#include "G4ParticleHPCaptureData.hh"
#include "G4ParticleHPElementData.hh"

class G4ParticleHPData
{
public:

  G4ParticleHPData(G4ParticleDefinition* projectile, const char* dataDirVariable);

  ~G4ParticleHPData();

  inline G4PhysicsVector * MakePhysicsVector(G4Element * thE, G4ParticleHPFissionData * theP)
  {
     return DoPhysicsVector(theData[thE->GetIndex()].GetData(theP));
  }
  inline G4PhysicsVector * MakePhysicsVector(G4Element * thE, G4ParticleHPCaptureData * theP)
  {
     return DoPhysicsVector(theData[thE->GetIndex()].GetData(theP));
  }
  inline G4PhysicsVector * MakePhysicsVector(G4Element * thE, G4ParticleHPElasticData * theP)
  {
     return DoPhysicsVector(theData[thE->GetIndex()].GetData(theP));
  }
  inline G4PhysicsVector * MakePhysicsVector(G4Element * thE, G4ParticleHPInelasticData * theP)
  {
//     G4cout << "entered G4ParticleHPData::MakePhysicsVector!!!"<<G4endl;
//     G4cout << "thE->GetIndex()="<<thE->GetIndex()<<G4endl;
     return DoPhysicsVector(theData[thE->GetIndex()].GetData(theP));
  }

  G4PhysicsVector * DoPhysicsVector(G4ParticleHPVector * theVector);
  
  static G4ParticleHPData * Instance(G4ParticleDefinition* projectile, const char* dataDirVariable);
  
private:

  G4ParticleHPElementData * theData;
  G4int numEle;
  
};

#endif
