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
#include "DetV1stHitAlgorithm.hh"
#include "DetRecHitCone.hh"
#include "DetVerbosity.hh"
#include "GamosCore/GamosSD/include/GmRecHit.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "CLHEP/Random/RandFlat.h"


//----------------------------------------------------------------------
DetV1stHitAlgorithm::DetV1stHitAlgorithm()
{
}

//----------------------------------------------------------------------
void DetV1stHitAlgorithm::InitVariables()
{
  theSelectionOrder = G4int(GmParameterMgr::GetInstance()->GetNumericValue("Det1stHit"+theName+":SelectionOrder",1));
}


//----------------------------------------------------------------------
GmRecHit* DetV1stHitAlgorithm::GetRecHitByX( mmdrh& recHitMap, G4bool startByLast )
{
  G4int ii;
  std::pair< mmdrh::iterator, mmdrh::iterator > pite;

  if( startByLast ) {
    mmdrh::reverse_iterator mdrite = recHitMap.rbegin();
    
    for( ii = 0; ii < theSelectionOrder; ii++ ){
      if( ii >= G4int(recHitMap.size())-1 ) break;
      mdrite++;
    }
    pite = recHitMap.equal_range((*mdrite).first);
    if( pite.first != pite.second ) {
      return(*mdrite).second;
    }
    
  } else {
    mmdrh::iterator mdrite = recHitMap.begin();
    
    for( ii = 0; ii < theSelectionOrder; ii++ ){
      if( ii >= G4int(recHitMap.size())-1 ) break;
      mdrite++;
    }
    pite = recHitMap.equal_range((*mdrite).first);
    if( pite.first != pite.second ) {
      return (*mdrite).second;
    }
  }
    
  //it may happen that two have the same X, and in simulation the first rec hit create is the first, while in real data it is random: randomize to properly simulate real data
  mmdrh::iterator ite;
  G4int nequal = 0;
  for( ite = pite.first; ite != pite.second; ite++,nequal++ );
  
  G4double rand = CLHEP::RandFlat::shoot()*nequal;
  ite = pite.first;
  for( G4int ii = 0; ii < rand; ii++ ){
    ite++;
    return (*ite).second;
  }

  return 0; // to avoid compilation warning
}

