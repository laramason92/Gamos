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
// $Id: G4ParticleHPFinalState.hh,v 1.1 2012/10/02 17:20:42 arce Exp $
// GEANT4 tag $Name:  $
//
// 080721 Create adjust_final_state method by T. Koi  
// 080801 Introduce theNDLDataA,Z which has A and Z of NDL data by T. Koi
//
#ifndef G4ParticleHPFinalState_h
#define G4ParticleHPFinalState_h

#include "G4Material.hh"
#include "G4FastVector.hh"
#include "G4HadFinalState.hh"
#include "G4ParticleHPNames.hh"
#include "G4ParticleHPVector.hh"
#include "G4HadProjectile.hh"
#include "G4Neutron.hh"
class G4ParticleDefinition;

class G4ParticleHPFinalState
{
public:

  G4ParticleHPFinalState()
  { 
    hasFSData = true; 
    hasXsec = true;
    hasAnyData = true;
    theBaseZ = 0;
    theBaseA = 0;

    theNDLDataZ = 0;
    theNDLDataA = 0;

     adjustResult = true;
     if ( getenv( "G4PARTICLEHP_DO_NOT_ADJUST_FINAL_STATE" ) ) adjustResult = false;

     theProjectile = G4Neutron::Neutron();

     theDataDirVariable = "G4NEUTRONHPDATA"; // Used for Inelastic/Gammas directory only
  };
  
  virtual ~G4ParticleHPFinalState(){};

  //virtual void Init (G4double A, G4double Z, G4String & dirName, G4String & aFSType) = 0;
  void Init (G4double A, G4double Z, G4String & dirName, G4String & aFSType, G4ParticleDefinition* projectile) { G4int M = 0; Init ( A, Z, M, dirName, aFSType,const_cast<G4ParticleDefinition*>(projectile)); };
  virtual void Init (G4double A, G4double Z, G4int M, G4String & dirName, G4String & aFSType, G4ParticleDefinition* ) =0;
  virtual G4HadFinalState * ApplyYourself(const G4HadProjectile & ) 
  {
    throw G4HadronicException(__FILE__, __LINE__, "G4HadFinalState * ApplyYourself(const G4HadProjectile & theTrack) needs implementation");
    return 0;
  };
  
  // of course this would better be Done templating G4ParticleHPChannel..., 
  // but due to peculiarities of the DEC compiler, this way it
  // is easier to maintain.
  virtual G4ParticleHPFinalState * New() = 0;
  
  G4bool HasXsec() {return hasXsec;};
  G4bool HasFSData() {return hasFSData;};
  G4bool HasAnyData() {return hasAnyData;};
  
  virtual G4double GetXsec(G4double ) { return 0; };
  virtual G4ParticleHPVector * GetXsec() { return 0; };
  
  void     SetA_Z(G4double anA, G4double aZ) {theBaseA = anA; theBaseZ = aZ; };
  G4double GetZ() { return theBaseZ; };
  G4double GetN() { return theBaseA; };
  
  void SetProjectile( G4ParticleDefinition* projectile ) {
    theProjectile = projectile; }

  void SetDataDirVariable( G4String dir ) { theDataDirVariable = dir; }

  protected:
  
  G4bool hasXsec;
  G4bool hasFSData;
  G4bool hasAnyData;
  G4ParticleHPNames theNames;
  
  G4HadFinalState theResult;
  
  G4double theBaseA;
  G4double theBaseZ;


//080721
   protected:
      void adjust_final_state ( G4LorentzVector );
      G4int theNDLDataZ;
      G4int theNDLDataA;

   private:
      G4bool adjustResult;

protected:
  G4ParticleDefinition* theProjectile;
  
  G4String theDataDirVariable;

};
#endif
