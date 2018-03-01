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
#include "Det1stHitByDistanceToOther.hh"
#include "DetRecHitCone.hh"
#include "GamosCore/GamosSD/include/GmRecHit.hh"
#include "GamosCore/GamosSD/include/GmHitsEventMgr.hh"
#include "DetVerbosity.hh"
#include "CLHEP/Random/RandFlat.h"


//----------------------------------------------------------------------
Det1stHitByDistanceToOther::Det1stHitByDistanceToOther()
{
  theName = "DistanceToOther";
  InitVariables();
}

//----------------------------------------------------------------------
GmRecHit* Det1stHitByDistanceToOther::Get1stRecHit( const DetRecHitSet& set1, const DetRecHitSet& set2 )
{
 //----- Try all possible combinations of distances between pair of hits

  std::set<GmRecHit*>::iterator ites1,ites2, selectItes;
  selectItes = set1.begin(); // just in case no one is found
  G4double minDist = 9.E99;
  for( ites1 = set1.begin(); ites1 != set1.end(); ites1++ ) {
    //--- Distance to rec hit of second set
    if( theSelectionOrder != 0 ) {
      G4double dist = ((*ites1)->GetPosition()-set2.GetPosition()).mag();
      //	G4cout << " check ites2 dist " << dist << G4endl;
      if(dist < minDist ){
	if( DetVerb(debugVerb) ) G4cout << " Det1stHitByXYPos::GetRecHitComptonCone minDist " << minDist << " new min dist " << dist << (*ites1)->GetPosition() << G4endl;
	minDist = dist;
	selectItes = ites1;
      }
    } else {
      //	G4cout << " check ites " << set2.size() << G4endl;
      for( ites2 = set2.begin(); ites2 != set2.end(); ites2++ ) {
	G4double dist = ((*ites1)->GetPosition()-(*ites2)->GetPosition()).mag();
	//  G4cout << " check ites2 " << dist << G4endl;
	if(dist < minDist ){
	  minDist = dist;
	  selectItes = ites1;
	}
      }
    }
  }

  return *selectItes;
}

