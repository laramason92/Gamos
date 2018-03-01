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
#ifndef SHRadDecayScorerFromFlux_h
#define SHRadDecayScorerFromFlux_h 1

#include "G4THitsMap.hh"
class G4ParticleHPProduct;
class G4Ions;
class G4IonTable;
class G4Step;
class G4Isotope;

///////////////////////////////////////////////////////////////////////////////
class SHRadDecayScorerFromFlux 
{
 
public: // with description
  SHRadDecayScorerFromFlux();
  ~SHRadDecayScorerFromFlux();
  void ProcessHits(const G4Step*);

  G4bool FillScorer(const G4Step* aStep, G4int index, G4double val, G4double wei);
  void ScoreNewEvent();
  void CalculateErrors(G4THitsMap<G4double>* RunMap);
  G4double GetError( G4int index );
  G4double GetError( G4int index, G4double sumWX, G4double nEvents );
  G4double GetErrorRelative( G4int index, G4double sumWX, G4double nEvents );

  G4int GetIndexFromProduct(G4ParticleHPProduct* product);
  G4int GetIndexAdjustingIon( G4int AZ, const G4Isotope* isotope );

  G4Ions* GetIon(G4int index) const;

  G4THitsMap<G4double>* GetRunMap() const {
    return theRunMap;
  }
  

private:
  G4bool bAdjustPHP;
  G4bool bAdjustHP;
  G4bool bInitialised;
  G4double theEnergyInterpStep;
  G4int theNInterpStep;

  G4IonTable* theIonTable;
  std::map<G4ParticleHPProduct*,G4int> theHPProductIndices;
  std::map<G4int,G4int> theAZindices;
  std::map<G4int,G4Ions*> theIonIndices;

  // use hits as if it were a scorer
  G4THitsMap<G4double>* theEvtMap;
  G4THitsMap<G4double>* theRunMap;

  G4bool bNewEvent;
  //--- Error calculations
  std::map<G4int,G4double> theSumV_tmp;
  std::map<G4int,G4double> theSumV2;
  std::map<G4int,G4double> theError;
  G4double sumALL; 

};

#endif
