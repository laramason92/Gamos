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
// $Id: G4ParticleHPDiscreteTwoBody.hh,v 1.1 2012/10/02 17:20:40 arce Exp $
// GEANT4 tag $Name:  $
//
#ifndef G4ParticleHPDiscreteTwoBody_h
#define G4ParticleHPDiscreteTwoBody_h 1

//101110 Bug fix in MF=6, LAW=2 case; contribution from E. Mendoza, D. Cano-Ott (CIEMAT)

#include "G4ios.hh"
#include <fstream>
#include "globals.hh"
#include "G4VParticleHPEnergyAngular.hh"
#include "G4ParticleHPLegendreTable.hh"
#include "G4ParticleHPInterpolator.hh"
#include "G4InterpolationManager.hh"

class G4ParticleHPDiscreteTwoBody : public G4VParticleHPEnergyAngular
{
  public:
  
  G4ParticleHPDiscreteTwoBody()
  {
    theCoeff = 0;
  }
  ~G4ParticleHPDiscreteTwoBody()
  {
    if(theCoeff!=0) delete [] theCoeff;
  }
  
  void Init(std::ifstream & aDataFile)
  {
    aDataFile >> nEnergy;
    theManager.Init(aDataFile);
    theCoeff = new G4ParticleHPLegendreTable[nEnergy];
    for(G4int i=0; i<nEnergy; i++)
    {
      G4double energy;
      G4int aRep, nCoeff;
      aDataFile >> energy >> aRep >> nCoeff;
      energy*=eV;
      G4int nPoints=nCoeff;
      if(aRep>0) nPoints*=2;
      //theCoeff[i].Init(energy, nPoints);

      theCoeff[i].Init(energy, nPoints-1);
      theCoeff[i].SetRepresentation(aRep);
      for(G4int ii=0; ii<nPoints; ii++)
      {
        G4double y;
        aDataFile >> y;
        theCoeff[i].SetCoeff(ii, y);
      }
    }
  }
    
  G4ReactionProduct * Sample(G4double anEnergy, G4double massCode, G4double mass);
  G4double MeanEnergyOfThisInteraction() { return -1; }
  
  private:
  
  G4int nEnergy;
  G4InterpolationManager theManager; // knows the interpolation between stores
  G4ParticleHPLegendreTable * theCoeff;
    
  private:
  
  G4ParticleHPInterpolator theInt;

};
#endif
