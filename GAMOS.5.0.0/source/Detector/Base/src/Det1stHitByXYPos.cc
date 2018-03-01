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
#include "Det1stHitByXYPos.hh"
#include "DetRecHitCone.hh"
#include "GamosCore/GamosSD/include/GmRecHit.hh"
#include "GamosCore/GamosSD/include/GmHitsEventMgr.hh"
#include "DetVerbosity.hh"
#include "CLHEP/Random/RandFlat.h"


//----------------------------------------------------------------------
Det1stHitByXYPos::Det1stHitByXYPos()
{
  theName = "XYPos";
  InitVariables();
}

//----------------------------------------------------------------------
GmRecHit* Det1stHitByXYPos::Get1stRecHit( const DetRecHitSet& set1, const DetRecHitSet&  )
{
  //----- Order the rec hits by absolute X position
  std::set<GmRecHit*>::iterator ites;
  mmdrh recHitMap;
  for( ites = set1.begin(); ites != set1.end(); ites++ ) {
    G4double value = (*ites)->GetPosition().perp();
    recHitMap.insert(mmdrh::value_type(value,*ites));
    if( DetVerb(debugVerb) ) G4cout << " GetRecHitByXYRadius add" << value << " " << (*ites) << " " << (*ites)->GetPosition() << G4endl;
  }

  return GetRecHitByX( recHitMap, 0 );

}

