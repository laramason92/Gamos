#ifndef GmExSensitiveDetector_HH
#define GmExSensitiveDetector_HH

#include "GamosCore/GamosSD/include/GmVSD.hh"

class G4LogicalVolume;
class G4Step;
class GmHit;

class GmExSensitiveDetector: public GmVSD
{

public:
  GmExSensitiveDetector(G4String type);
  ~GmExSensitiveDetector();

  virtual void EndOfEvent(G4HCofThisEvent*HCE);

  virtual unsigned long long GetDetUnitID( G4Step* aStep );
  virtual void CalculateAndSetPosition( GmHit* hit, G4Step* aStep );

};

#endif

