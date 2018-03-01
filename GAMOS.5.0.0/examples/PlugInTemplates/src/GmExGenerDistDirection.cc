#include "../include/GmExGenerDistDirection.hh"
#include "../include/GmExVerbosityMgr.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "CLHEP/Random/RandFlat.h"

//---------------------------------------------------------------------
GmExGenerDistDirection::GmExGenerDistDirection()
{
}

//---------------------------------------------------------------------
G4ThreeVector GmExGenerDistDirection::GenerateDirection( const GmParticleSource* )
{
  G4ThreeVector newDir;

  return newDir;

}

//---------------------------------------------------------------------
void GmExGenerDistDirection::SetParams( const std::vector<G4String>& params )
{
  /*  if( params.size() != 4 ) {
    G4Exception(" GmExGenerDistDirection to set direction you have to add 3 parameters: DIR_X DIR_Y DIR_Z OPENING_ANGLE");
   }

  theInitialDir = G4ThreeVector(GetValue( params[0] ), GetValue( params[1] ), GetValue( params[2] ) );

  theOpeningAngle = GetVal(params[3]);

  G4cout << GenerVerb(infoVerb) << "GmExGenerDistDirection::SetParams iitialDir " << theInitialDir << " openingAngle " << theOpeningAngle << G4endl;
  */
}
