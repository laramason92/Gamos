#include "../include/GmExGenerDistPosition.hh"
#include "../include/GmExVerbosityMgr.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"


//---------------------------------------------------------------------
G4ThreeVector GmExGenerDistPosition::GeneratePosition( const GmParticleSource* )
{
  G4ThreeVector thePoint;
  return thePoint;

}


//---------------------------------------------------------------------
void GmExGenerDistPosition::SetParams( const std::vector<G4String>& params )
{

  /*  if( params.size() == 0 ) {
    thePoint = G4ThreeVector();
  } else if( params.size() == 3 ) {
    thePoint = G4ThreeVector(GetVal( params[0] ), GetVal( params[1] ), GetVal( params[2] ) );
  } else {
    G4Exception(" GmExGenerDistPosition to set point you have to add 3 parameters: POS_X POS_Y POS_Z");
  }
  G4cout << GenerVerb(infoVerb) << "GmExGenerDistPosition::SetParams point " << thePoint << G4endl;
  */

}
