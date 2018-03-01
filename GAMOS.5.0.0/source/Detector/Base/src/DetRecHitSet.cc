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
#include "DetRecHitSet.hh"
#include "DetRecHitCone.hh"
#include "GamosCore/GamosSD/include/GmRecHit.hh"
#include "GamosCore/GamosSD/include/GmHitsEventMgr.hh"
#include "DetVerbosity.hh"
#include "CLHEP/Random/RandFlat.h"

//----------------------------------------------------------------------
DetRecHitSet::DetRecHitSet()
{
  Initialise();
}

//----------------------------------------------------------------------
void DetRecHitSet::Initialise()
{
  theTimeMin = DBL_MAX;
  theTimeMax = -DBL_MAX;
  clear();
}

//----------------------------------------------------------------------
void DetRecHitSet::AddHit( GmRecHit* rhit )
{
  if( size() == 0 ) {
    thePosition = rhit->GetPosition();
    theEnergy = 0;
  }

  insert( rhit );
  theEnergy += rhit->GetEnergy();

  theTimeMin = std::min( rhit->GetTimeMin(), theTimeMin );
  theTimeMax = std::max( rhit->GetTimeMin(), theTimeMax );

  //  G4cout << this << "    DetRecHitSet::AddHit( "<< GetTime() << " = " << rhit->GetTime() << " E " << rhit->GetEnergy() << G4endl;
}


//----------------------------------------------------------------------
GmRecHit* DetRecHitSet::GetRecHitByEnergy( G4int nOrder )
{
  //----- Order the rec hits by energy
  std::set<GmRecHit*>::iterator ites;
  std::map<G4double,GmRecHit*> recHitMap;
  for( ites = begin(); ites != end(); ites++ ) {
    recHitMap[(*ites)->GetEnergy()] = *ites;
  }

  return GetRecHitByX( nOrder, recHitMap, 1 );
}

//----------------------------------------------------------------------
GmRecHit* DetRecHitSet::GetRecHitByX( G4int nOrder, std::map<G4double,GmRecHit*>& recHitMap, G4bool startByLast )
{
  G4int ii;
  nOrder --;
  if( startByLast ) {
    std::map<G4double,GmRecHit*>::reverse_iterator mdrite = recHitMap.rbegin();
    std::map<G4double,GmRecHit*>::reverse_iterator mdrite2 = recHitMap.rbegin();
    mdrite2++;
    
    for( ii = 0; ii < nOrder; ii++ ){
      if( ii >= G4int(size()) ) break;
      //-      G4cout << " add " << ii << " " << nOrder << " " << size() << G4endl;
      mdrite++;
      mdrite2++;
    }
    
    //it may happen that two have the same X, and in simulation the first rec hit create is the first, while in real data it is random: randomize to properly simulate real data

    if( ii != G4int(size()) && (*mdrite).first == (*mdrite2).first ) {
      //      G4cout << " using 2nd " << ii << G4endl;
      G4double rand = CLHEP::RandFlat::shoot();
      if( rand > 0.5 ) {
	return (*mdrite).second;
      } else {
	return (*mdrite2).second;
      }
    } else {
      return (*mdrite).second;
    }
  } else {
    std::map<G4double,GmRecHit*>::iterator mdrite = recHitMap.begin();
    std::map<G4double,GmRecHit*>::iterator mdrite2 = recHitMap.begin();
    mdrite2++;
    
    for(ii = 0; ii < nOrder; ii++ ){
      if( ii >= G4int(size()) ) break;
      //-      G4cout << " add " << ii << " " << nOrder << " " << size() << G4endl;
      mdrite++;
      mdrite2++;
    }
    
    //it may happen that two have the same X, and in simulation the first rec hit create is the first, while in real data it is random: randomize to properly simulate real data
    if( ii != G4int(size()) && (*mdrite).first == (*mdrite2).first ) {
      G4double rand = CLHEP::RandFlat::shoot();
      if( rand > 0.5 ) {
	return (*mdrite).second;
      } else {
	return (*mdrite2).second;
      }
    } else {
      return (*mdrite).second;
    }
  }

}


//----------------------------------------------------------------------
void DetRecHitSet::Print( std::ostream& os )
{
  std::set<GmRecHit*>::iterator ite;
  G4int ii = 0;
  os << "DetRecHitSet:: POS= " << thePosition.perp() << " " << thePosition.phi() << " " << thePosition.z() << " E= " << theEnergy << G4endl;
  for( ite = begin(); ite != end(); ite++ ){
    os << "DetRecHitSet:: rec hit " << ii << " POS= " << (*ite)->GetPosition().perp() << " " << (*ite)->GetPosition().phi() << " " << (*ite)->GetPosition().z()  << " E= " << (*ite)->GetEnergy() << G4endl;
    ii++;
  }
}


//----------------------------------------------------------------------
G4double DetRecHitSet::GetTime() const 
{
  G4int hitsTimeType = GmHitsEventMgr::GetInstance()->GetHitsTimeType();
  if( hitsTimeType == 1 ) {
    return GetTimeMin();
  } else if( hitsTimeType == 2 ) {
    return GetTimeMax();
  } else {
    G4Exception("GmRecHit::GetTime",	
		"Wrong argument",
		FatalErrorInArgument,
		"With SD:Hits:TimeType <> 1 not implemented yet");
    return 0.; // to avoid compilation warning
  }

}
