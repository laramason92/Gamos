#include "../include/GmExClassifier.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"

GmExClassifier::GmExClassifier(G4String name) : GmVClassifier( name )
{
}

void GmExClassifier::SetParameters( const std::vector<G4String>&  )
{
}

G4int GmExClassifier::GetIndex(const G4Step* aStep)
{
  /*  G4StepPoint* preStep = aStep->GetPreStepPoint();
  G4TouchableHistory* th = (G4TouchableHistory*)(preStep->GetTouchable());
  return th->GetReplicaNumber(theIndexDepth);
  */
  return 0;
}
