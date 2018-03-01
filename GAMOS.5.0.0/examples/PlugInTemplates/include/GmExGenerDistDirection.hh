#ifndef GmExGenerDistDirection_HH
#define GmExGenerDistDirection_HH

#include "GamosCore/GamosFactories/include/GmVGenerDistDirection.hh"
class GmParticleSource;

class GmExGenerDistDirection : public GmVGenerDistDirection
{
public:
  GmExGenerDistDirection();
  virtual ~GmExGenerDistDirection(){};

  virtual G4ThreeVector GenerateDirection( const GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );

};

#endif

