#include "../include/GmExGenerDistTime.hh"
#include "../include/GmExVerbosityMgr.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------------
G4double GmExGenerDistTime::GenerateTime( const GmParticleSource* source )
{
  G4double theTime;

  return theTime;

}

//---------------------------------------------------------------------
void GmExGenerDistTime::SetParams( const std::vector<G4String>& params )
{
  /*  if( params.size() != 1 ) {
    G4Exception("GmExGenerDistTime  only one paremeter: ACTIVITY");
  }

  theActivity = GetValue( params[0] );

 G4cout << GenerVerb(infoVerb) << " GmExGenerDistTime::SetParams  activity " << theActivity << G4endl;
  */
}

