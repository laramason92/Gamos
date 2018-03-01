#ifndef GmExPSScorer_h
#define GmExPSScorer_h 1

#include "GamosCore/GamosScoring/include/GmVPrimitiveScorer.hh"
#include "G4THitsMap.hh"

class GmExPSScorer : public GmVPrimitiveScorer
{
   public: // with description
      GmExPSScorer(G4String name);
      virtual ~GmExPSScorer();

  protected: // with description
      virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);

  public: 
      virtual void EndOfEvent(G4HCofThisEvent*);
      virtual void DrawAll();
      virtual void PrintAll();

  private:
};
#endif

