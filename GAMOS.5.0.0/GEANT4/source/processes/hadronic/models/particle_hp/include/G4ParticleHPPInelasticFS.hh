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
// $Id: G4ParticleHPPInelasticFS.hh,v 1.1 2012/10/02 17:20:51 arce Exp $
// GEANT4 tag $Name:  $
//
#ifndef G4ParticleHPPInelasticFS_h
#define G4ParticleHPPInelasticFS_h 1

#include "globals.hh"
#include "G4HadProjectile.hh"
#include "G4HadFinalState.hh"
#include "G4ParticleHPInelasticCompFS.hh"
#include "G4ParticleHPAngular.hh"
#include "G4ParticleHPEnergyDistribution.hh"
#include "G4ParticleHPEnAngCorrelation.hh"
#include "G4ParticleHPPhotonDist.hh"

class G4ParticleHPPInelasticFS : public G4ParticleHPInelasticCompFS
{
  public:
  
  G4ParticleHPPInelasticFS(){}
  ~G4ParticleHPPInelasticFS(){}
  void Init (G4double A, G4double Z, G4int M, G4String & dirName, G4String & aFSType, G4ParticleDefinition*);
  G4HadFinalState * ApplyYourself(const G4HadProjectile & theTrack);
  G4ParticleHPFinalState * New() 
  {
   G4ParticleHPPInelasticFS * theNew = new G4ParticleHPPInelasticFS;
   theNew->SetProjectile( theProjectile );
   theNew->SetDataDirVariable( theDataDirVariable );
   return theNew;
  }
  
  private:
  
};
#endif