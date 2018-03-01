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
#ifndef CCRecHitSet_HH
#define CCRecHitSet_HH

#include "globals.hh"
#include "G4ThreeVector.hh"
#include <set>
#include <map>
#include <vector>
#include <iostream>
class GmRecHit;

class CCRecHitSet : public std::set<GmRecHit*> 
{

public:
  CCRecHitSet();
  
  virtual ~CCRecHitSet(){};

  void Initialise();

  void AddHit( GmRecHit* rhit );

  G4double GetEnergy() const{
    return theEnergy; 
  }
  G4ThreeVector GetPosition() const{
    return thePosition; 
  }

  void SetEnergy( G4double ener ) {
    theEnergy = ener; 
  }
  void SetPosition( G4ThreeVector pos ) {
    thePosition = pos;
  }

  G4double GetTime() const;
  G4double GetTimeMin() const { return theTimeMin;}
  G4double GetTimeMax() const { return theTimeMax;}
 
  GmRecHit* GetRecHitByEnergy( G4int nEnergy );
  GmRecHit* GetRecHitByPosition( G4int nPosition );
  void Print( std::ostream& os = G4cout );

private:
  GmRecHit* GetRecHitByX( G4int nEnergy, std::map<G4double,GmRecHit*>& recHitMap, G4bool bStartByLast );

private:
  //  std::set<GmRecHit*> theRecHits;
  G4double theEnergy;
  G4ThreeVector thePosition;
  G4double theTimeMin;
  G4double theTimeMax;
};

typedef std::vector<CCRecHitSet> CCRecHitSetList;

#endif
