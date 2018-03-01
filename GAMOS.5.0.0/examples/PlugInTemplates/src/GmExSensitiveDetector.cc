#include "G4Step.hh"
#include "G4StepPoint.hh"

#include "../include/GmExSensitiveDetector.hh"
#include "GamosCore/GamosSD/include/GmHit.hh"
#include "CLHEP/Random/RandGauss.h"

//-------------------------------------------------------------------
GmExSensitiveDetector::GmExSensitiveDetector(G4String sdtype): GmVSD( sdtype )
{
}

//-------------------------------------------------------------------
GmExSensitiveDetector::~GmExSensitiveDetector()
{
}

//-------------------------------------------------------------------
unsigned long long GmExSensitiveDetector::GetDetUnitID( G4Step* aStep )
{

}

//-------------------------------------------------------------------
void GmExSensitiveDetector::CalculateAndSetPosition( GmHit* hit, G4Step* aStep )
{
  /*  const G4VTouchable*  touch= aStep->GetPreStepPoint()->GetTouchable();
  G4AffineTransform trans = touch->GetHistory()->GetTopTransform().Inverse();

  G4ThreeVector centreGlobal = trans.TransformPoint( G4ThreeVector(0.,0.,0.) );

  hit->SetPosition( centreGlobal );
  */
} 


//-------------------------------------------------------------------
void GmExSensitiveDetector::EndOfEvent(G4HCofThisEvent*HCE)
{
  GmVSD::EndOfEvent( HCE );
}
