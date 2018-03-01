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
#include "RTReuseAtZPlaneProcess.hh"
#include "RTVerbosity.hh"
#include "G4VUserTrackInformation.hh"
#include "RTReuseAtZPlaneTrackInfo.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//------------------------------------------------------------------------
RTReuseAtZPlaneProcess::RTReuseAtZPlaneProcess(const G4String& aName, G4ProcessType aType)
  : G4VDiscreteProcess(aName,aType)
{
  GmParameterMgr* parMgr = GmParameterMgr::GetInstance(); 
  theZPlane = parMgr->GetNumericValue("RTReuseAtZPlane:ZReusePlane", -9E9 );

  if( theZPlane == -9E9 ) {
    //--- Use last Z phase space plane
    std::vector<G4double> theZStops = parMgr->GetVNumericValue("RTPhaseSpaceUA:ZStops", std::vector<G4double>());
    if( theZStops.size() == 0 ){
      G4Exception(" RTReuseAtZPlaneProcess::RTReuseAtZPlaneProcess ",
		  " No Z plane defined ",
		  FatalErrorInArgument,
		  "Use /gamos/setParam RTReuseAtZPlane:ZReusePlane VALUE");
    } else {
      theZPlane = theZStops[theZStops.size()-1];  
    }
  }

  theNReuse = G4int(parMgr->GetNumericValue("RTReuseAtZPlane:NReuse", -1 ));

#ifndef GAMOS_NO_VERBOSE
   if( RTVerb(infoVerb) )  G4cout << "RTReuseAtZPlaneProcess ZPlane : " << theZPlane << " NReuse " << theNReuse << G4endl;
#endif

  //  fNReuseInv = 1./fNReuse;
  //  fReuseLevel = G4int(parMgr->GetNumericValue("GmBremsReusetingProcess:ReuseLevel",1));

  G4String strMirrorWhenReuse = parMgr->GetStringValue("RTReuseAtZPlane:MirrorWhenReuse", "");
  if( strMirrorWhenReuse == "" ) {
    MirrorWhenReuse = NOMir;
  } else if( strMirrorWhenReuse == "X" ){
    MirrorWhenReuse = MirX;
  } else if( strMirrorWhenReuse == "Y" ){
    MirrorWhenReuse = MirY;
  } else if( strMirrorWhenReuse == "XY" ){
    MirrorWhenReuse = MirXY;
  } else {
    G4Exception("RTReuseAtZPlaneProcess::RTReuseAtZPlaneProcess()",
		"Wrong value of MirrorWhenReuse",
		FatalErrorInArgument,
		G4String("It can be X, Y, XY, it is = " + strMirrorWhenReuse).c_str());
  }


}

//------------------------------------------------------------------------
G4double RTReuseAtZPlaneProcess::PostStepGetPhysicalInteractionLength(
                             const G4Track& track,
                             G4double   previousStepSize,
                             G4ForceCondition* condition
                            )
{
  //--- Condition is set to "Not Forced"
  *condition = NotForced;

  //--- Avoid making two consecutive steps (due to precision problem distance to plane can be very small when in the previous step the track has been moved to plane)
 const G4Step* aStep = track.GetStep();
  if( aStep->GetPreStepPoint()->GetProcessDefinedStep() ) {
    //    G4cout << " ZPlaneProcess::PostStepGetPhysicalInteractionLength PROCESS PRE " << aStep->GetPreStepPoint()->GetProcessDefinedStep()->GetProcessName() << G4endl;
    if( aStep->GetPreStepPoint()->GetProcessDefinedStep() == this ) return DBL_MAX;
  }
  
  //--- Get mean free path
  return GetMeanFreePath(track, previousStepSize, condition);

}


//------------------------------------------------------------------------
G4VParticleChange* RTReuseAtZPlaneProcess::PostStepDoIt(const G4Track& track, const G4Step&  step)
{
#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(debugVerb) ) G4cout << "RTReuseAtZPlaneProcess::PostStepDoIt = reuse" << theNReuse << " at pos= " << track.GetPosition() << " dir= " << track.GetMomentumDirection() << " Ener= " << track.GetKineticEnergy() << G4endl;
#endif
  aParticleChange.Initialize(track);
  aParticleChange.SetNumberOfSecondaries(theNReuse);
  for( G4int ii = 0; ii < theNReuse; ii++ ) {
    //  for( G4int ii = 0; ii < theNReuse-1; ii++ ) {
    G4int nMirror = 0;
    switch ( MirrorWhenReuse ){
    case NOMir:
      nMirror = 0;
      break;
    case MirX:
      nMirror = ii%2;
      break;
    case MirY:
      nMirror = (ii%2) * 2;
      break;
    case MirXY:
      nMirror = ii%4;
      break;
    }
    if( nMirror < 0 ) nMirror = 0; // when theMaxNReuse = theNReuse + 1
    
    G4ThreeVector pos = track.GetPosition();
    G4ThreeVector momV = track.GetMomentumDirection();
    switch (nMirror) {
    case 0: // no mirror
      break;
    case 1: // mirror X
      pos.setX(-pos.x());
      momV.setX(-momV.x());
      break;
    case 2: // mirror Y
      pos.setY(-pos.y());
      momV.setY(-momV.y());
      break;
    case 3: // mirror X & Y
      pos.setX(-pos.x());
      momV.setX(-momV.x());
      pos.setY(-pos.y());
      momV.setY(-momV.y());
      break;
    }
    
    G4DynamicParticle* secoDP = new G4DynamicParticle (track.GetParticleDefinition(), momV, track.GetKineticEnergy());
    G4Track* secoTRK = new G4Track( secoDP, track.GetGlobalTime(), pos );
    secoTRK->SetUserInformation( new RTReuseAtZPlaneTrackInfo );
    aParticleChange.AddSecondary(secoTRK);
    secoTRK->SetWeight( track.GetWeight() / theNReuse );
  }
  aParticleChange.ProposeWeight( track.GetWeight() / theNReuse );
  
  return G4VDiscreteProcess::PostStepDoIt( track, step);
  
}


//------------------------------------------------------------------------
G4double RTReuseAtZPlaneProcess::GetMeanFreePath(const G4Track& aTrack,
						 G4double ,
						 G4ForceCondition* )
{
  G4VUserTrackInformation* trkInfo = aTrack.GetUserInformation();
  if( trkInfo ) {
    if( RTVerb(debugVerb) ){
      G4double distZ = theZPlane - aTrack.GetPosition().z();
      G4double cosTrk = aTrack.GetMomentumDirection().z();
#ifndef GAMOS_NO_VERBOSE
      if( RTVerb(debugVerb) ) G4cout << "RTReuseAtZPlaneProcess::GetMeanFreePath =  track already reused, do not reuse " << " distZ " << distZ << " cosTrk " << cosTrk << " dir " << aTrack.GetMomentumDirection() << G4endl;
#endif
    }
    RTReuseAtZPlaneTrackInfo* trkInfoS = (RTReuseAtZPlaneTrackInfo*)trkInfo;
    if( trkInfoS->IsReuse() ) return DBL_MAX;
  }

  G4double distZ = theZPlane - aTrack.GetPosition().z();
  if( distZ <= 0 ) {
    return DBL_MAX;
  }
  G4double cosTrk = aTrack.GetMomentumDirection().z();
#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(debugVerb) ) G4cout << "ZPlaneProcess::GetMeanFreePath =  " << distZ / cosTrk << " distZ " << distZ << " cosTrk " << cosTrk << " dir " << aTrack.GetMomentumDirection() << G4endl;
#endif
  if( cosTrk <= 0 ) {
    return DBL_MAX;
  } else {
    return distZ / cosTrk;
  }

}
