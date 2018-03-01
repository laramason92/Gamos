#ifndef GmExPSPrinter_hh
#define GmExPSPrinter_hh 1

#include <vector>
#include "globals.hh"
#include "GamosCore/GamosScoring/include/GmVPSPrinter.hh"

class GmExPSPrinter : public GmVPSPrinter
{
public: // with description
  GmExPSPrinter(G4String);
  virtual ~GmExPSPrinter(){ };
  
  virtual void DumpAll( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* theScorer );
  virtual void SetParameters( const std::vector<G4String>& param );

private:

  G4double theUnit;
  G4String theUnitName;
};

#endif

