#include "../include/GmExGenerDistEnergy.hh"
#include "../include/GmExVerbosityMgr.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "G4UnitsTable.hh"

//---------------------------------------------------------------------
G4double GmExGenerDistEnergy::GenerateEnergy( const GmParticleSource* )
{
  G4double theEnergy;
  return theEnergy;

}


//---------------------------------------------------------------------
void GmExGenerDistEnergy::SetParams( const std::vector<G4String>& params )
{
  /*  if( params.size() != 1 ) {
      G4Exception("GmExGenerDistEnergy  one parameter should be given: ENERGY");
    }

  theEnergy = GetVal( params[0] );
  G4cout << GenerVerb(infoVerb) << "GmExGenerDistEnergy::SetParams  energy " << theEnergy << G4endl;
  */

}
