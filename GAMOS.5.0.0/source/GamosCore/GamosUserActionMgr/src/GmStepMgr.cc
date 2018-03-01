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
#include "G4TouchableHandle.hh"
#include "G4TouchableHistory.hh"
#define private public
#include "G4Track.hh"

#include "GmStepMgr.hh"
#include "GmFutureFilterUA.hh"

#include "GamosCore/GamosUserActionMgr/include/GmUAVerbosity.hh"

#include "GmUserActionMgr.hh"
#include "GmFutureFilter.hh"

#include "G4Step.hh"

#include <set>

GmStepMgr* GmStepMgr::theInstance = 0;

//-----------------------------------------------------------------------
GmStepMgr::GmStepMgr()
{
  GmFutureFilterUA* ffUA = new GmFutureFilterUA;
  ffUA->SetName( "GmFutureFilterUA" );
  ffUA->SetApplyFiltersToStepping(FALSE);
  ffUA->SetStepMgr( this );
  //  GmUserActionMgr::GetInstance()->RegisterEventAction( ffUA ); 
  //  GmUserActionMgr::GetInstance()->RegisterSteppingAction( ffUA );
  GmUserActionMgr::GetInstance()->PutSteppingActionFirst( ffUA ); 
}

//------------------------------------------------------------------------
GmStepMgr* GmStepMgr::GetInstance()
{
  if( !theInstance ) {
    theInstance = new GmStepMgr();
  }

  return theInstance;
}

//-----------------------------------------------------------------------
void GmStepMgr::SaveStep( const G4Step* aStep )
{
#ifndef GAMOS_NO_VERBOSE
  if( UAVerb(debugVerb) ) {
    G4cout << "GmStepMgr::SaveStep " << G4endl;
  }
#endif

  G4Track* newTrack = SaveTrack( aStep->GetTrack() );

  G4Step* newStep = new G4Step();

  G4StepPoint* preSP = new G4StepPoint( *(aStep->GetPreStepPoint()) );
  theNewStepPoints.push_back( preSP );
  G4StepPoint* postSP = new G4StepPoint( *(aStep->GetPostStepPoint()) );

  theNewStepPoints.push_back( postSP );
  newStep->SetPreStepPoint( preSP );
  newStep->SetPostStepPoint( postSP );
  newStep->SetTrack(newTrack);
  newTrack->SetStep(newStep);

  // step length
  newStep->SetStepLength(aStep->GetStepLength());

   // total energy deposit 
  newStep->SetTotalEnergyDeposit(aStep->GetTotalEnergyDeposit());

  // total non-ionizing energy deposit 
  newStep->SetNonIonizingEnergyDeposit(aStep->GetNonIonizingEnergyDeposit());

  // control flag for stepping
  newStep->SetControlFlag(aStep->GetControlFlag());

  if(aStep->IsFirstStepInVolume() ) newStep->SetFirstStepFlag();
  if(aStep->IsLastStepInVolume() ) newStep->SetLastStepFlag();

  G4TrackVector* secondaries = new G4TrackVector( *(aStep->GetSecondary()) );
  newStep->SetSecondary( secondaries );

  theNewSteps.push_back(newStep);
}

//-----------------------------------------------------------------------
G4Track* GmStepMgr::SaveTrack( const G4Track* aTrack )
{
 G4Track* newTrack = new G4Track( *aTrack );
  
  newTrack->SetTrackID(aTrack->GetTrackID());
  newTrack->SetParentID(aTrack->GetParentID());
  newTrack->fCurrentStepNumber = aTrack->GetCurrentStepNumber();
  newTrack->SetCreatorProcess( aTrack->GetCreatorProcess());
  newTrack->SetUserInformation( aTrack->GetUserInformation());
  newTrack->SetWeight( aTrack->GetWeight() );
  newTrack->SetStepLength( aTrack->GetStepLength() );

  G4TouchableHistory* newTouchable = new G4TouchableHistory();
  newTouchable->UpdateYourself( aTrack->GetTouchable()->GetVolume(), aTrack->GetTouchable()->GetHistory() );
  newTrack->SetTouchableHandle( newTouchable );
  G4TouchableHistory* newNextTouchable = new G4TouchableHistory();
  newNextTouchable->UpdateYourself( aTrack->GetNextTouchable()->GetVolume(), aTrack->GetNextTouchable()->GetHistory() );
  newTrack->SetNextTouchableHandle( newNextTouchable );
  theNewTouchables.push_back( newNextTouchable );
  if( aTrack->GetOriginTouchable() ) {
    G4TouchableHistory* newOriginTouchable = new G4TouchableHistory();
    newOriginTouchable->UpdateYourself( aTrack->GetOriginTouchable()->GetVolume(), aTrack->GetOriginTouchable()->GetHistory() );
    newTrack->SetOriginTouchableHandle( newOriginTouchable );     
    theNewTouchables.push_back( newOriginTouchable );
  }

  theNewTracks[aTrack->GetTrackID()] = newTrack;

  return newTrack;
 }

//-----------------------------------------------------------------------
std::vector<G4Step*> GmStepMgr::GetSteps( GmFutureFilter* filter, const G4Step* currentStep )
{
  std::vector<G4Step*> acceptedSteps;
  
#ifndef GAMOS_NO_VERBOSE
  if(  UAVerb(debugVerb)) G4cout << " GmStepMgr::GetSteps " << filter->GetName() << G4endl;
#endif
  if( !filter->IsFilterWithChildren() ) { // look only to current track
    std::vector<G4Step*>::const_iterator ites;
    for( ites = theNewSteps.begin(); ites != theNewSteps.end(); ites++ ){
      if( currentStep->GetTrack()->GetTrackID() == (*ites)->GetTrack()->GetTrackID() 
	  && filter->AcceptPastStep( *ites ) ) acceptedSteps.push_back( *ites );
#ifndef GAMOS_NO_VERBOSE
      if(  UAVerb(debugVerb)) G4cout << " GmStepMgr::GetSteps FilterWithoutChildren currentStep TrackID " << currentStep->GetTrack()->GetTrackID() << " =? step TrackID " << (*ites)->GetTrack()->GetTrackID()  << " NacceptedSetps " <<acceptedSteps.size() << G4endl;
#endif

    }
  } else { // futureFilter is on children, so look to all ancestors too
    // build list of ancestors of current step
    std::map<G4int,G4Track*>::const_iterator itet;
    std::set<G4int> ancestorTracks; 
    G4int trkID = currentStep->GetTrack()->GetTrackID();
    // get track currespoding to step
    G4Track* trk = 0;
    for( itet = theNewTracks.begin(); itet != theNewTracks.end(); itet++ ){
      if( (*itet).second->GetTrackID() == trkID ) { 
	trk = (*itet).second;
	break;
      }
    }
    if( !trk ) {
      G4Exception("GmStepMgr::GetSteps",
		  "Track does not exist",
		  FatalException,
		  "Please contact GAMOS authors");
    }
    // Now look for parents 
    for(;;) {
      ancestorTracks.insert( trk->GetTrackID() );
#ifndef GAMOS_NO_VERBOSE
      if( UAVerb(debugVerb) ) {
	G4cout << "GmStepMgr::GetSteps insert ancestor track " << trk->GetTrackID() << " parent " << trk->GetParentID() << G4endl;
      }
#endif  
      std::map<G4int,G4Track*>::const_iterator itetn = theNewTracks.find( trk->GetParentID() );
      if( itetn == theNewTracks.end() ) {
	G4cerr << " GmStepMgr::GetSteps : no parent track found for track " << trk->GetTrackID() << G4endl;
	for( itetn = theNewTracks.begin(); itetn != theNewTracks.end(); itetn++ ){
	  //	  G4cerr << " GmStepMgr::GetSteps : SAVE TRACK " << (*itetn).second->GetTrackID() << " parent= " << (*itetn).first << G4endl;
	}
	break;
      }
      if( trk->GetParentID() == 0 ) break;
      trk = (*itetn).second;
    }
  
    // accept step if track is in the list of ancestors
    std::vector<G4Step*>::const_iterator ites;
    for( ites = theNewSteps.begin(); ites != theNewSteps.end(); ites++ ){
      if( ancestorTracks.find( (*ites)->GetTrack()->GetTrackID() ) != ancestorTracks.end() &&
	  filter->AcceptPastStep( *ites ) ) acceptedSteps.push_back( *ites );
    }
  }

#ifndef GAMOS_NO_VERBOSE
  if( UAVerb(debugVerb) ) {
    G4cout << "GmStepMgr::GetSteps " << acceptedSteps.size() << G4endl;
  }
#endif  
  
  return acceptedSteps;

}

//-----------------------------------------------------------------------
void GmStepMgr::ClearObjects()
{
#ifndef GAMOS_NO_VERBOSE
  if( UAVerb(debugVerb) ) {
    G4cout << "GmStepMgr::ClearObjects " << G4endl;
  }
#endif

  // --- Delete all new objects   
  std::vector<G4Step*>::const_iterator ites;
  for( ites = theNewSteps.begin(); ites != theNewSteps.end(); ites++ ){
    delete (*ites);
  }
  theNewSteps.clear();

  std::map<G4int,G4Track*>::const_iterator itet;
  for( itet = theNewTracks.begin(); itet != theNewTracks.end(); itet++ ){
    delete (*itet).second;
  }
  theNewTracks.clear();

  std::vector<G4StepPoint*>::const_iterator itesp;
  for( itesp = theNewStepPoints.begin(); itesp != theNewStepPoints.end(); itesp++ ){
    //    G4cout << " DELETE Step point " << *itesp << G4endl;
    //    delete (*itesp);
  }
  theNewStepPoints.clear();

  std::vector<G4TouchableHistory*>::const_iterator iteto;
  for( iteto = theNewTouchables.begin(); iteto != theNewTouchables.end(); iteto++ ){
    //    delete (*iteto);
  }
  theNewTouchables.clear();

}

