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
// GmG4PSDoseDeposit_NoSkipEqualMate
#include "GmG4PSDoseDeposit_NoSkipEqualMate.hh"
#include "G4VSolid.hh"
#include "G4UnitsTable.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"

#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

////////////////////////////////////////////////////////////////////////////////
// (Description)
//   This is a primitive scorer class for scoring only energy deposit.
// 
///////////////////////////////////////////////////////////////////////////////

GmG4PSDoseDeposit_NoSkipEqualMate::GmG4PSDoseDeposit_NoSkipEqualMate(G4String name)
  :GmVPrimitiveScorer(name)
{
  theUnit = gray;
  theUnitName = G4String("Gy");

  bUseVolume = true;
}

GmG4PSDoseDeposit_NoSkipEqualMate::~GmG4PSDoseDeposit_NoSkipEqualMate()
{;}

G4bool GmG4PSDoseDeposit_NoSkipEqualMate::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( !AcceptByFilter( aStep ) ) return false;

  G4double edep = aStep->GetTotalEnergyDeposit();
  if ( edep == 0. ) return FALSE;
  G4double volume = GetVolume(aStep);
  G4double density = aStep->GetTrack()->GetMaterial()->GetDensity();
  G4double dose    = edep / ( density * volume );
  G4double weight = 1.;
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << "  GmG4PSDoseDeposit_NoSkipEqualMate::ProcessHits dose " << dose << " edep " << edep << " density " << density << " volume " << volume << " g= " << g << " joule " << joule << G4endl;
#endif
  if( fWeighted ) {
    weight = aStep->GetPreStepPoint()->GetWeight(); 
    //    dose *= weight; 
  }

  FillScorer( aStep, dose, weight );
  //  FillScorer( aStep, dose, aStep->GetPreStepPoint()->GetWeight());
  return TRUE;
}


void GmG4PSDoseDeposit_NoSkipEqualMate::EndOfEvent(G4HCofThisEvent*)
{
}

void GmG4PSDoseDeposit_NoSkipEqualMate::DrawAll()
{;}

void GmG4PSDoseDeposit_NoSkipEqualMate::PrintAll()
{
  G4cout <<" GmG4PSDoseDeposit_NoSkipEqualMate::PrintAllDefault() " << G4endl;
  G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
  G4cout << " PrimitiveScorer " << GetName() << G4endl;
  G4cout << " Number of entries " << EvtMap->entries() << G4endl;
  std::map<G4int,G4double*>::iterator itr = EvtMap->GetMap()->begin();
  for(; itr != EvtMap->GetMap()->end(); itr++) {
    G4cout << "  copy no.: " << itr->first
	   << "  dose deposit: " << G4BestUnit(*(itr->second),"Dose")
	   << G4endl;
  }
}
 #include "GamosCore/GamosBase/Base/include/GmVClassifier.hh" 
G4int GmG4PSDoseDeposit_NoSkipEqualMate::GetIndex(G4Step* aStep ) 
 { 
 return theClassifier->GetIndexFromStep( aStep ); 
} 
