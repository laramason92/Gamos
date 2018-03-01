#ifndef GmExFilter_HH
#define GmExFilter_HH 1

class G4Step;
#include "globals.hh"
#include "GamosCore/GamosBase/include/GmVFilter.hh"

class GmExFilter : public GmVFilter 
{

  public: 
      GmExFilter(G4String name);
      virtual ~GmExFilter();

  public: 
      virtual G4bool Accept(const G4Step*) const;

};
#endif

