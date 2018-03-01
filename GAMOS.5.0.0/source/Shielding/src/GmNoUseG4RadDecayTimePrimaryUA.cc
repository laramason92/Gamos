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
#define private public
#include "GmNoUseG4RadDecayTimePrimaryUA.hh"
#include "ShieldingVerbosity.hh"

#include "GamosCore/GamosGenerator/include/GmGenerDistTimeDecay.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "G4Track.hh"
#include "G4EventManager.hh"
#include "G4ParticleDefinition.hh"

//---------------------------------------------------------------------------
GmNoUseG4RadDecayTimePrimaryUA::GmNoUseG4RadDecayTimePrimaryUA()
{
}

//---------------------------------------------------------------------------
void GmNoUseG4RadDecayTimePrimaryUA::BeginOfRunAction( const G4Run* )
{
}

//---------------------------------------------------------------------------
void GmNoUseG4RadDecayTimePrimaryUA::UserSteppingAction(const G4Step* aStep )
{
  //---- Particles created by radioactive decay of a secondary
  G4Track* aTrack = const_cast<G4Track*>(aStep->GetTrack());
  const G4VProcess* proc = aStep->GetPostStepPoint()->GetProcessDefinedStep();
  //  G4double lifeTimeT = GetLifeTime(aTrack->GetDefinition());

  //----- Get activity of this isotope
  if( proc && proc->GetProcessName() == "RadioactiveDecay"
      && !aTrack->GetDefinition()->GetPDGStable() ) { // lifeTimeT = -1 ) {
    if( aTrack->GetParentID() != 0 ) return;
    G4StepPoint* preSP = aStep->GetPreStepPoint();
    G4StepPoint* postSP = aStep->GetPostStepPoint();

    // G4cout << " DEC TIME pre " << preSP->GetGlobalTime() << " post " << postSP->GetGlobalTime() << G4endl;

    G4double velocity = preSP->GetVelocity();
    G4double timeChange = 0.;
    if( velocity != 0 ) timeChange = aStep->GetStepLength()/preSP->GetVelocity();
    G4double gtime = preSP->GetGlobalTime() + timeChange;
    postSP->SetGlobalTime( gtime );
    aTrack->SetGlobalTime( gtime );
    G4double ltime = preSP->GetLocalTime() + timeChange;
    postSP->SetLocalTime( ltime );
    aTrack->SetLocalTime( gtime );

    G4TrackVector secos = GmG4Utils::GetSecondariesOfCurrentStep();
    G4TrackVector::iterator ite;
    for( ite = secos.begin(); ite != secos.end(); ite++ ) {
      (*ite)->SetGlobalTime( gtime );
      (*ite)->SetLocalTime( ltime );
    }
    //    G4cout << " DEC TIME AFTER pre " << preSP->GetGlobalTime() << " post " << postSP->GetGlobalTime() << G4endl;
  }
}
