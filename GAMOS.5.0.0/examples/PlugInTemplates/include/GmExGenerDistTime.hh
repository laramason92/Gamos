#ifndef GmExGenerDistTime_HH
#define GmExGenerDistTime_HH

#include "GamosCore/GamosFactories/include/GmVGenerDistTime.hh"

class GmParticleSource;

class GmExGenerDistTime : public GmVGenerDistTime
{
public:
  GmExGenerDistTime(){};
  virtual ~GmExGenerDistTime(){};

  virtual G4double GenerateTime( const GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );

};

#endif

