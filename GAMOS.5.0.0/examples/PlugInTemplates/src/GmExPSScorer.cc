#include "GmExPSScorer.hh"
#include "G4Track.hh"
#include "G4VSolid.hh"
#include "G4UnitsTable.hh"

GmExPSScorer::GmExPSScorer(G4String name)
     :GmVPrimitiveScorer(name)
{
  /*  theUnit = 1.E4;
      theUnitName = "cm-2"; */
}

GmExPSScorer::~GmExPSScorer()
{;}

G4bool GmExPSScorer::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  /*
 G4double stepLength = aStep->GetStepLength();
  if ( stepLength == 0. ) return FALSE;
  G4double volume     = aStep->GetPreStepPoint()->GetPhysicalVolume()
    ->GetLogicalVolume()->GetSolid()->GetCubicVolume();
  G4double CellFlux = stepLength / volume ;
  FillScorer( aStep, CellFlux, aStep->GetPreStepPoint()->GetWeight() );
*/  
  return TRUE;

}

void GmExPSScorer::EndOfEvent(G4HCofThisEvent*)
{;}

void GmExPSScorer::DrawAll()
{;}

void GmExPSScorer::PrintAll()
{
}
