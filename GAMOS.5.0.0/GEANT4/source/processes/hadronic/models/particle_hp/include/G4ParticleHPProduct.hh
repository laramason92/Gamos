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
// $Id: G4ParticleHPProduct.hh,v 1.1 2012/10/02 17:20:53 arce Exp $
// GEANT4 tag $Name:  $
//
#ifndef G4ParticleHPProduct_h
#define G4ParticleHPProduct_h 1

#include "G4HadronicException.hh"
#include "globals.hh"
#include "G4ParticleHPVector.hh"
#include "Randomize.hh"
#include "G4ios.hh"
#include <fstream>
#include "globals.hh"
#include "G4VParticleHPEnergyAngular.hh"
#include "G4ReactionProductVector.hh"

#include "G4ParticleHPContEnergyAngular.hh"
#include "G4ParticleHPDiscreteTwoBody.hh"
#include "G4ParticleHPIsotropic.hh"
#include "G4ParticleHPNBodyPhaseSpace.hh"
#include "G4ParticleHPLabAngularEnergy.hh"
class G4ParticleDefinition;

enum G4HPMultiMethod { G4HPMultiPoisson, G4HPMultiAverage };

class G4ParticleHPProduct
{
  public:
  G4ParticleHPProduct()
  {
    theDist = 0;
    char * method = getenv( "G4PARTICLEHP_MULTIPLICITY_METHOD" );
    if( method )  {
      if( G4String(method) == "Poisson" ) {
	theMultiplicityMethod = G4HPMultiPoisson;
      } else if( G4String(method) == "Average" ) {
	theMultiplicityMethod = G4HPMultiAverage;
      } else {
	throw G4HadronicException(__FILE__, __LINE__, ("multiplicity method unknown to G4ParticleHPProduct" + G4String(method)).c_str());
      }
    } else {
      theMultiplicityMethod = G4HPMultiPoisson;
    }
  }
  ~G4ParticleHPProduct()
  {
    if(theDist != 0) delete theDist;
  }
  
  inline void Init(std::ifstream & aDataFile, G4ParticleDefinition* projectile)
  {
    aDataFile >> theMassCode>>theMass>>theIsomerFlag>>theDistLaw
              >> theGroundStateQValue>>theActualStateQValue;
    theGroundStateQValue*= eV;
    theActualStateQValue*= eV;
    theYield.Init(aDataFile, eV);
    if(theDistLaw==0)
    {
      // distribution not known, use E-independent, isotropic angular distribution
      theDist = new G4ParticleHPIsotropic;
    }
    else if(theDistLaw == 1)
    {
      // Continuum energy-angular distribution
      theDist = new G4ParticleHPContEnergyAngular(projectile);
    }
    else if(theDistLaw == 2)
    {
      // Discrete 2-body scattering
      theDist = new G4ParticleHPDiscreteTwoBody;
    }
    else if(theDistLaw == 3)
    {
      // Isotropic emission
      theDist = new G4ParticleHPIsotropic;
    }
    else if(theDistLaw == 4)
    {
      // Discrete 2-body recoil modification
      // not used for now. @@@@
      theDist = new G4ParticleHPDiscreteTwoBody; 
      // the above is only temporary;
      // recoils need to be addressed
      // properly
      delete theDist;
      theDist = 0;
    }
    else if(theDistLaw == 5)
    {
      // charged particles only, to be used in a later stage. @@@@
    }
    else if(theDistLaw == 6)
    {
      // N-Body phase space
      theDist = new G4ParticleHPNBodyPhaseSpace;
    }
    else if(theDistLaw == 7)
    {
      // Laboratory angular energy paraetrisation
      theDist = new G4ParticleHPLabAngularEnergy;
    }
    else
    {
      throw G4HadronicException(__FILE__, __LINE__, "distribution law unknown to G4ParticleHPProduct");
    }
    if(theDist!=0)
    {
      theDist->SetQValue(theActualStateQValue);      
      theDist->Init(aDataFile);
    }
  }
  
  G4int GetMultiplicity(G4double anEnergy, G4bool bForceAverage = 0);
  G4ReactionProductVector * Sample(G4double anEnergy, G4int nParticles);
  
  G4double GetMeanYield(G4double anEnergy)
  {
    return theYield.GetY(anEnergy);
  }
  
  void SetIncidentPart(G4ReactionProduct * aIncidentPart) 
  { 
    theIncidentPart = aIncidentPart; 
  }
  
  void SetTarget(G4ReactionProduct * aTarget)
  { 
    theTarget = aTarget; 
  }
  
  inline G4ReactionProduct * GetTarget() { return theTarget; }
  
  inline G4ReactionProduct * GetIncidentPart() { return theIncidentPart; }
  
  inline G4double MeanEnergyOfThisInteraction() 
  { 
    G4double result;
    if(theDist == 0)
    {
      result = 0;
    }
    else
    {
      result=theDist->MeanEnergyOfThisInteraction();
      result *= theCurrentMultiplicity;
    }
    return result;
  }
  
  inline G4double GetQValue() { return theActualStateQValue; }

  inline G4double GetMassCode() const { return theMassCode; }

  private:
   
   // data members

   G4double theMassCode;
   G4double theMass;
   G4int theIsomerFlag;
   G4double theGroundStateQValue;
   G4double theActualStateQValue;
   G4int theDistLaw;  // redundant
   G4ParticleHPVector theYield;
   G4VParticleHPEnergyAngular *  theDist;
   
   // Utility quantities
   
   G4ReactionProduct * theTarget;
   G4ReactionProduct * theIncidentPart;

   // cashed values
   
   G4int theCurrentMultiplicity;

  G4HPMultiMethod theMultiplicityMethod;  
};

#endif
