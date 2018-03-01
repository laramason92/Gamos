#include "../include/GmExFilter.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This is the class of a filter to be associated with a
// sensitive detector. 
///////////////////////////////////////////////////////////////////////////////

GmExFilter::GmExFilter(G4String name)
  :GmVFilter(name)
{;}

GmExFilter::~GmExFilter()
{;}

G4bool GmExFilter::Accept(const G4Step* aStep) const
{
  /*
  if (aStep->GetPreStepPoint()->GetCharge() != 0. ) return TRUE;
  return FALSE;
  */
}

