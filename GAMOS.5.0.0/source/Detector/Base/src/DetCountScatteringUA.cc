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
#include "DetCountScatteringUA.hh"
#include "DetVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosAnalysis/include/GmCheckOriginalGamma.hh"

#include "G4Run.hh"
#include "G4Event.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ios.hh"
#include <vector>
//----------------------------------------------------------------
DetCountScatteringUA::DetCountScatteringUA()
{
}
//----------------------------------------------------------------
void DetCountScatteringUA::BeginOfRunAction( const G4Run* )
{
  std::vector<G4String> vs;
  theVolumes = GmParameterMgr::GetInstance()->GetVStringValue("DetCountScatteringUA:VolumeNames",vs);
  std::vector<G4String> procNamesV; 
  G4String pn = "compt";
  if( pn != "" ) procNamesV.push_back(pn);
  pn = "Rayl";
  if( pn != "" ) procNamesV.push_back(pn);
  procNamesV = GmParameterMgr::GetInstance()->GetVStringValue("DetCountScatteringUA:ProcessNames",procNamesV);

  for( unsigned int ii = 0; ii < procNamesV.size(); ii++ ){
    pn = procNamesV[ii];
    if( pn != "" ) theProcNames.insert(pn);
    //   G4cout << ii << " theProcNames " << GetProcessName("gamma",pn) << " : " << procNamesV[ii] << G4endl;
  }

  theEnergyMin = GmParameterMgr::GetInstance()->GetNumericValue("DetCountScatteringUA:EnergyMin",-1.);

  theAnaMgr = GmAnalysisMgr::GetInstance("pet");
  theAnaMgr->CreateHisto1D("DetCountScatteringUA: E lost in interaction",100,0.,600.,theHistoNumber+1);

}


//----------------------------------------------------------------
void DetCountScatteringUA::BeginOfEventAction( const G4Event* )
{
  theOriginalTracks.clear();
}


//----------------------------------------------------------------
void DetCountScatteringUA::UserSteppingAction(const G4Step* aStep)
{
#ifndef GAMOS_NO_VERBOSE
  //  G4cout << DetVerb(testVerb) << G4EventManager::GetEventManager()->GetNonconstCurrentEvent()->GetEventID() << G4endl;
#endif
  const G4Track* aTrack = aStep->GetTrack();

  if( GmCheckOriginalGamma::CheckIfOriginalGamma( aTrack ) != 0 ){
    G4String procName = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
    if( theProcNames.find(procName) != theProcNames.end() ){
#ifndef GAMOS_NO_VERBOSE
      if( DetVerb(debugVerb) ) G4cout << aTrack->GetTrackID() <<  " DetCountScatteringUA process found " << procName << " " << G4endl;
#endif
      G4String volName = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();
      G4bool bInVolume = FALSE;
      for( unsigned int ii = 0; ii < theVolumes.size(); ii++ ){
	if( GmGenUtils::AreWordsEquivalent(theVolumes[ii],volName) ){
#ifndef GAMOS_NO_VERBOSE
	  if( DetVerb(debugVerb) ) G4cout << aTrack->GetTrackID() << " DetCountScatteringUA process and volume found " << volName << G4endl;
#endif
	  bInVolume = TRUE;
	  break;
	}
      }
      if( bInVolume ) {
	//--- Check energy
	if( aStep->GetPreStepPoint()->GetKineticEnergy() - aStep->GetPostStepPoint()->GetKineticEnergy() >= theEnergyMin ) {
	  std::map<G4int,G4int>::iterator ite = theOriginalTracks.find(aTrack->GetTrackID());
	  if( ite == theOriginalTracks.end() ){
	    theOriginalTracks[aTrack->GetTrackID()] = 1;
	  } else {
	    (*ite).second++;
	  }
#ifndef GAMOS_NO_VERBOSE
	  if( DetVerb(debugVerb) ) G4cout << aTrack->GetTrackID() << " DetCountScatteringUA process, volume and energy found " << aStep->GetPreStepPoint()->GetKineticEnergy() - aStep->GetPostStepPoint()->GetKineticEnergy() << G4endl;
#endif
	}
	theAnaMgr->GetHisto1(theHistoNumber+1)->Fill((aStep->GetPreStepPoint()->GetKineticEnergy() - aStep->GetPostStepPoint()->GetKineticEnergy())/keV);
      }
      
    } else {
#ifndef GAMOS_NO_VERBOSE
      if( DetVerb(testVerb) ) G4cout << " DetCountScatteringUA NO scattered found " << procName << " volname " << aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() << " trk ID " << aTrack->GetTrackID() << G4endl;
#endif

    }
  }

}


//----------------------------------------------------------------
G4int DetCountScatteringUA::GetNScatterOriginalTrack( G4int id )
{
 std::map<G4int,G4int>::iterator ite = theOriginalTracks.find(id);
 if( ite == theOriginalTracks.end() ){
#ifndef GAMOS_NO_VERBOSE
   if( DetVerb(testVerb) )  G4cout << "  DetCountScatteringUA::GetNScatterOriginalTrac NO found " << id << G4endl;
#endif
   return 0;
 } else {
#ifndef GAMOS_NO_VERBOSE
    if( DetVerb(testVerb) )  G4cout << "  DetCountScatteringUA::GetNScatterOriginalTrac found " << id << G4endl;
#endif
   return (*ite).second;
 }

}
