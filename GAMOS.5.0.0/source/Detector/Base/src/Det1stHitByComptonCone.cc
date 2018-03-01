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
#include "Det1stHitByComptonCone.hh"
#include "DetRecHitCone.hh"
#include "GamosCore/GamosSD/include/GmRecHit.hh"
#include "GamosCore/GamosSD/include/GmHitsEventMgr.hh"
#include "DetVerbosity.hh"
#include "CLHEP/Random/RandFlat.h"


//----------------------------------------------------------------------
Det1stHitByComptonCone::Det1stHitByComptonCone()
{
  theName = "ComptonCone";
}

//----------------------------------------------------------------------
GmRecHit* Det1stHitByComptonCone::Get1stRecHit( const DetRecHitSet& set1, const DetRecHitSet& set2 )
{	
  //----- Try all possible combinations of compton cone extrapolation to rec hits od other 511 rec hit set

  std::set<GmRecHit*>::iterator ites11,ites12,ites2, selectItes;
  selectItes = set1.begin(); // just in case no one is found
  G4double minDist = 9.E99;
  selectItes = set1.end();

  for( ites11 = set1.begin(); ites11 != set1.end(); ites11++ ) {
    for( ites12 = set1.begin(); ites12 != set1.end(); ites12++ ) {
      if( ites11 == ites12 ) continue;
      //      if( electron_mass_c2 / (*ites12)->GetEnergy() > 3 ) continue;

      if( ((*ites11)->GetPosition() - (*ites12)->GetPosition()).mag() < 1.e-9 ) continue;
     DetRecHitCone* rhc = new DetRecHitCone( *ites11, *ites12 );

      //--- Distance to rec hit of second set
      if( theSelectionOrder != 0 ) {
	G4double dist = rhc->GetDist(set2.GetPosition());
	if(dist < minDist ){
	  if( DetVerb(debugVerb) ) G4cout << " Det1stHitByXYPos::GetRecHitComptonCone minDist " << minDist << " new min dist " << dist << (*ites11)->GetPosition() << G4endl;
	  minDist = dist;
	  selectItes = ites11;
	}
      } else {
	//	G4cout << " check ites " << set2.size() << G4endl;
	for( ites2 = set2.begin(); ites2 != set2.end(); ites2++ ) {
	  G4double dist = rhc->GetDist((*ites2)->GetPosition());
	  //	  G4cout << " check ites2 " << dist << G4endl;
	  if(dist < minDist ){
	    minDist = dist;
	    selectItes = ites11;
	  }
	}
      }
      delete rhc;
    }
  }

  if( selectItes == set1.end() ) {
    // no good Compton cone found, try at least the one closer to a cone
    for( ites11 = set1.begin(); ites11 != set1.end(); ites11++ ) {
      for( ites12 = set1.begin(); ites12 != set1.end(); ites12++ ) {
	G4double dist = electron_mass_c2 / (*ites12)->GetEnergy();
	if( dist < minDist ) {
	  selectItes = ites11;
	  minDist = dist;
	}
      }
    }
  }

  return *selectItes;
}

