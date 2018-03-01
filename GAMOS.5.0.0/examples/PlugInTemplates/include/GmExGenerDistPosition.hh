#ifndef GmExGenerDistPosition_HH
#define GmExGenerDistPosition_HH

#include "GamosCore/GamosFactories/include/GmVGenerDistPosition.hh"
class GmParticleSource;

class GmExGenerDistPosition : public GmVGenerDistPosition
{
public:
  GmExGenerDistPosition(){};
  virtual ~GmExGenerDistPosition(){};

  virtual G4ThreeVector GeneratePosition( const GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );

};

#endif

