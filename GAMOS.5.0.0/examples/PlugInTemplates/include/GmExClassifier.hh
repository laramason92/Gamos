#ifndef GmExClassifier_hh
#define GmExClassifier_hh 1

#include <vector>
#include "globals.hh"
//
class G4Step;
#include "GamosCore/GamosBase/include/GmVClassifier.hh"

class GmExClassifier  : public GmVClassifier
{
public: // with description
  GmExClassifier(G4String);
  virtual ~GmExClassifier(){ };
  
  virtual G4int GetIndex(const G4Step*);

  virtual void SetParameters( const std::vector<G4String>& param );
};

#endif

