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
#ifndef DetV1stHitAlgorithm_HH
#define DetV1stHitAlgorithm_HH

#include "globals.hh"
#include "G4ThreeVector.hh"
#include <set>
#include <map>
#include "DetRecHitSet.hh" 

typedef std::multimap<G4double,GmRecHit*> mmdrh;

class DetV1stHitAlgorithm
{

public:
  DetV1stHitAlgorithm();
  virtual ~DetV1stHitAlgorithm(){};

  virtual GmRecHit* Get1stRecHit( const DetRecHitSet& set1, const DetRecHitSet& set2 ) = 0;

protected:
  void InitVariables();

protected:
  GmRecHit* GetRecHitByX( mmdrh& recHitMap, G4bool startByLast );

  G4String theName;
  G4int theSelectionOrder;

};

#endif

