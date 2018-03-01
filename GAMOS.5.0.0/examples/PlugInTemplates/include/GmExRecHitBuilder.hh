#ifndef GmExRecHitBuilder_HH
#define GmExRecHitBuilder_HH

#include "GamosCore/GamosSD/include/GmVRecHitBuilderFromHits.hh"
class GmRecHit;
class GmHit;

class GmExRecHitBuilder : public GmVRecHitBuilderFromHits
{

public:
  
  GmExRecHitBuilder();
  virtual ~GmExRecHitBuilder(){};

  virtual std::vector<GmRecHit*> ReconstructHits(const std::vector<GmHit*>&);

private:
  G4bool CheckHitInRecHit( const GmRecHit* rhit, const GmHit* hit );

};

#endif
