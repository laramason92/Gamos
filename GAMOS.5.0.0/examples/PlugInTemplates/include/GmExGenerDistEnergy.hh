#ifndef GmExGenerDistEnergy_HH
#define GmExGenerDistEnergy_HH

#include "GamosCore/GamosFactories/include/GmVGenerDistEnergy.hh"
class GmParticleSource;

class GmExGenerDistEnergy : public GmVGenerDistEnergy
{
public:
  GmExGenerDistEnergy(){};
  virtual ~GmExGenerDistEnergy(){};

  virtual G4double GenerateEnergy( const GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );

};

#endif

