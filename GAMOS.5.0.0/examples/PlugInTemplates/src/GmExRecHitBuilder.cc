#include "../include/GmExRecHitBuilder.hh"
#include "GamosCore/GamosSD/include/GmSDVerbosityMgr.hh"
#include "GamosCore/GamosSD/include/GmRecHit.hh"
#include "GamosCore/GamosSD/include/GmHit.hh"
#include "GamosCore/GamosSD/include/GmHitList.hh"

//------------------------------------------------------------------------
GmExRecHitBuilder::GmExRecHitBuilder() 
{
}


//------------------------------------------------------------------------
std::vector<GmRecHit*> GmExRecHitBuilder::ReconstructHits(const std::vector<GmHit*>& hlist )
{
  /*  std::vector<GmRecHit*>::const_iterator itec;
  hitlist::const_iterator iteh;
  //assume hits are ordered by energy
  for( iteh = hlist.begin(); iteh != hlist.end(); iteh++ ){
    theRecHits.push_back( new GmRecHit( *iteh ) );
  }

  G4cout << SDVerb(infoVerb) << " GmExRecHitBuilder::ReconstructHits:  n RecHi
ts = " << theRecHits.size() << G4endl;

  return theRecHits;
  */
}

