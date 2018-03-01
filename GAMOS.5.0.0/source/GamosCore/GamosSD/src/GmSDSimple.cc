//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  GAMOS software  is  copyright of the Copyright  Holders  of *
// * the GAMOS Collaboration.  It is provided  under  the  terms  and *
// * conditions of the GAMOS Software License,  included in the  file *
// * LICENSE and available at  http://fismed.ciemat.es/GAMOS/license .*
// * These include a list of copyright holders.                       *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GAMOS collaboration.                       *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the GAMOS Software license.           *
// ********************************************************************
//
#include "GmSDSimple.hh"
#include "GmHit.hh"
#include "GmDetUnitIDBuilderFromAncestors.hh"
#include "GmSDVerbosity.hh"
#include "GmDetUnitIDBuilderFromAncestors.hh"

#include "G4Step.hh"
#include "G4StepPoint.hh"

//-------------------------------------------------------------------
GmSDSimple::GmSDSimple(G4String sdtype): GmVSD( sdtype )
{
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << sdtype << "GmSDSimple: SD type ->" << theSDType << " SD name " << SensitiveDetectorName << G4endl;
#endif

  theDetUnitIDBuilder = new GmDetUnitIDBuilderFromAncestors(theSDType);

}

//-------------------------------------------------------------------
GmSDSimple::~GmSDSimple()
{
}


//-------------------------------------------------------------------
unsigned long long GmSDSimple::GetDetUnitID( G4Step* aStep )
{
  return theDetUnitIDBuilder->BuildID( aStep );

}

//-------------------------------------------------------------------
void GmSDSimple::CalculateAndSetPosition( GmHit* hit, G4Step* aStep )
{
  //-  return;

  const G4VTouchable*  touch= aStep->GetPreStepPoint()->GetTouchable();
  G4AffineTransform trans = touch->GetHistory()->GetTopTransform().Inverse();

  G4ThreeVector centreGlobal = trans.TransformPoint( G4ThreeVector(0.,0.,0.) );

  hit->SetPosition( centreGlobal );
  //  G4cout << " GmSDSimple::CalculateAndSetPosition pos " << 
} 


//-------------------------------------------------------------------
void GmSDSimple::EndOfEvent(G4HCofThisEvent*HCE)
{
#ifndef GAMOS_NO_VERBOSE
  if(theHitsInEvent.size() != 0) if( SDVerb(debugVerb) ) G4cout << "GmSDSimple::EndOfEvent SD: " << SensitiveDetectorName << " Nhits= " << theHitsInEvent.size() << G4endl;
#endif
  GmVSD::EndOfEvent( HCE );
}
