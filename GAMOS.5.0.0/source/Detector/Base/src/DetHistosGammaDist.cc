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
#include "DetHistosGammaDist.hh"
#include "DetVerbosity.hh"

#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosUtils/include/GmLine.hh"
#include "GamosCore/GamosAnalysis/include/GmCheckOriginalGamma.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"

#include "G4Tubs.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"
#include "G4RunManager.hh"

#include <math.h>

//---------------------------------------------------------------------------
DetHistosGammaDist::DetHistosGammaDist()
{

  theAnaMgr = GmAnalysisMgr::GetInstance("NHGammaDist") ;

  std::string hnam;
  std::string hgnam = "NHGammaDist: ";
  hnam = hgnam + "angle between gammas at vertex (deg)";
  theAnaMgr->CreateHisto1D(hnam,100,0.,180.1,104000+1);
  hnam = hgnam + "angle between gammas at entering SD (deg)";
  theAnaMgr->CreateHisto1D(hnam,100,0.,180.1,104000+2);
  hnam = hgnam + "DCA orig from Gamma vertex (mm)";
  theAnaMgr->CreateHisto1D(hnam,100,0,50.,104000+3);
  hnam = hgnam + "DCA orig from Gamma entering SD (mm)";
  theAnaMgr->CreateHisto1D(hnam,100,0,50.,104000+4);

  hnam = hgnam + "Orig Pos Z if Gamma reaches SD (mm)";
  theAnaMgr->CreateHisto1D(hnam,100,0,200.,104000+5);
  hnam = hgnam + "Orig Pos R if Gamma reaches SD (mm)";
  theAnaMgr->CreateHisto1D(hnam,100,0,200.,104000+6);

  //----- Get  "ring" volume radius
  std::vector<G4LogicalVolume*> rings = GmGeometryUtils::GetInstance()->GetLogicalVolumes("ring",1);
  /*  G4VSolid* ring = rings[0]->GetSolid();
  if( ring->GetEntityType() != "G4Tubs" ) {
    G4Exception("DetHistosGammaDist::DetHistosGammaDist: ring is not a Tubs, but a " + ring->GetEntityType() );
  }
  G4Tubs* ringtubs = static_cast<G4Tubs*>(ring);
  theRingRadius = ringtubs->GetInnerRadius();
  */
  theNgamma = 0;

}

//---------------------------------------------------------------------------
void DetHistosGammaDist::PreUserTrackingAction(const G4Track* aTrack)
{
  isOriginalGamma = GmCheckOriginalGamma::CheckIfOriginalGamma( aTrack );

  if ( isOriginalGamma != 0) theNgamma++;
}


//---------------------------------------------------------------------------
void DetHistosGammaDist::UserSteppingAction(const G4Step* aStep)
{ 
  //-   G4cout << this << "step isoriginalgamma " << isOriginalGamma << G4endl;
  //----- Only original gammas 
  if ( isOriginalGamma == 0) return;

  if( !IsInterestingStep( aStep ) ) return ;

  if( aStep->GetTrack()->GetCurrentStepNumber() == 1 ) { // store the first step in the interaction list
#ifndef GAMOS_NO_VERBOSE
    if( DetVerb(infoVerb) ) G4cout << " DetHistosGammaDist::UserSteppingAction " << theNgamma*10 << G4endl;
#endif
    theInteractions[theNgamma*10] = GmTrajPointPosMom( aStep->GetPreStepPoint() ); 
  } else  {
#ifndef GAMOS_NO_VERBOSE
    if( DetVerb(infoVerb) ) G4cout << " DetHistosGammaDist::UserSteppingAction " << theNgamma*10+1 << G4endl;
#endif
    theInteractions[theNgamma*10+1] = GmTrajPointPosMom( aStep->GetPostStepPoint() );
  }

}
 

//---------------------------------------------------------------------------
G4bool DetHistosGammaDist::IsInterestingStep(const G4Step* aStep)
{
  //  if( aStep->GetTrack()->GetCurrentStepNumber() == 0 ) return 0;

  if(  aStep->GetTrack()->GetCurrentStepNumber() == 1 ) return 1;

  //----- Check if step is in SD //of type GmCaloSD
  G4Track* aTrack = aStep->GetTrack();
  G4VSensitiveDetector* sdetStart = aTrack->GetVolume()->GetLogicalVolume()->GetSensitiveDetector();
  G4VSensitiveDetector* sdetEnd = 0;
  if( aTrack->GetNextVolume() ) sdetEnd = aTrack->GetNextVolume()->GetLogicalVolume()->GetSensitiveDetector();

#ifndef GAMOS_NO_VERBOSE
  if( DetVerb(debugVerb) ) G4cout << "DetHistosGammaDist::IsInterestingStep( " << sdetStart << " " << sdetEnd << G4endl;
#endif
  if( sdetStart == 0 && sdetEnd != 0 ) {
    return 1;
  }else {
    return 0;
  }

}


//---------------------------------------------------------------------------
void DetHistosGammaDist::EndOfEventAction( const G4Event* evt )
{
  theEventVertex = evt->GetPrimaryVertex(0)->GetPosition();

  FillHistos();

  theInteractions.clear();

  theNgamma = 0;
}


//---------------------------------------------------------------------------
void DetHistosGammaDist::FillHistos()
{
  if( theInteractions.find(11) != theInteractions.end() ) {
    theAnaMgr->GetHisto1(104000+5)->Fill(theInteractions[10].GetPosition().z() );
    theAnaMgr->GetHisto1(104000+6)->Fill(theInteractions[10].GetPosition().perp() );
  }
  if( theInteractions.find(21) != theInteractions.end() ) {
    theAnaMgr->GetHisto1(104000+5)->Fill(theInteractions[20].GetPosition().z() );
    theAnaMgr->GetHisto1(104000+6)->Fill(theInteractions[20].GetPosition().perp() );
  }

  if( theInteractions.find(10) == theInteractions.end() || theInteractions.find(20) == theInteractions.end() ) return;

  G4ThreeVector posExtr1 = ExtrapolateToRing( theInteractions[10] );
  G4ThreeVector posExtr2 = ExtrapolateToRing( theInteractions[20] );
#ifndef GAMOS_NO_VERBOSE
  if( DetVerb(debugVerb) ) G4cout << " posExtr1 " << posExtr1 << " posExtr2 " << posExtr2 << G4endl;
#endif

  G4ThreeVector momVtx1 = theInteractions[10].GetMomentum();
  G4ThreeVector momVtx2 = theInteractions[20].GetMomentum();

  theAnaMgr->GetHisto1(104000+1)->Fill( momVtx1.angle( momVtx2 )/deg );
#ifndef GAMOS_NO_VERBOSE
  if( DetVerb(debugVerb) ) G4cout << " angle vtx " <<  momVtx1.angle( momVtx2 ) << " " << deg << G4endl;
#endif

  GmLine linVtx = GmLine(posExtr1,posExtr2);
  G4double distOrigVtx = linVtx.GetDist( theEventVertex );
  theAnaMgr->GetHisto1(104000+3)->Fill(distOrigVtx/mm );

  if( theInteractions.find(11) == theInteractions.end() || theInteractions.find(21) == theInteractions.end() ) return;

  G4ThreeVector momSd1 = theInteractions[11].GetMomentum();
  G4ThreeVector momSd2 = theInteractions[21].GetMomentum();

  theAnaMgr->GetHisto1(104000+2)->Fill( momSd1.angle( momSd2 )/deg );
#ifndef GAMOS_NO_VERBOSE
  if( DetVerb(debugVerb) ) G4cout << " angle sd " << momSd1.angle( momSd2 ) << " " << deg << G4endl;
#endif

  GmLine linSd = GmLine( theInteractions[11].GetPosition(), theInteractions[21].GetPosition() );
  G4double distOrigSd = linSd.GetDist( theEventVertex );
  theAnaMgr->GetHisto1(104000+4)->Fill(distOrigSd/mm );

}


//---------------------------------------------------------------------------
G4ThreeVector DetHistosGammaDist::ExtrapolateToRing( GmTrajPointPosMom& inter )
{
  return inter.GetPosition()+inter.GetMomentum();
}
