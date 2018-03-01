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
#include "SHStudyDecayChannelsUA.hh"
#include "ShieldingVerbosity.hh"
#include "SHMetastableIsotMgr.hh"
#include "SHRadDecayChainSet.hh"

#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "G4Track.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"

#include "G4DecayTable.hh"
#include "G4VDecayChannel.hh"

//-----------------------------------------------------------------
SHStudyDecayChannelsUA::SHStudyDecayChannelsUA()
{
  theName = "SHStudyDecayChannelsUA";

}


//-----------------------------------------------------------------
SHStudyDecayChannelsUA::~SHStudyDecayChannelsUA() 
{
}

//------------------------------------------------------------------
void SHStudyDecayChannelsUA::BeginOfRunAction( const G4Run* )
{

}

//------------------------------------------------------------------
void SHStudyDecayChannelsUA::UserSteppingAction(const G4Step* aStep)
{
  const G4VProcess* proc = aStep->GetPostStepPoint()->GetProcessDefinedStep();
  if( proc && proc->GetProcessName() == "RadioactiveDecay") {
    std::vector<G4Track*> secos = GmG4Utils::GetSecondariesOfCurrentStep();
    G4cout << aStep->GetTrack()->GetDefinition()->GetParticleName() << " DECAY: ";
    for( size_t ii = 0; ii < secos.size(); ii++ ){
      G4cout << secos[ii]->GetDefinition()->GetParticleName() << " " << secos[ii]->GetKineticEnergy() << " ";
    }
    G4cout << G4endl;
  }
}



//-----------------------------------------------------------------
void SHStudyDecayChannelsUA::PostUserTrackingAction(const G4Track* aTrack)
{
  G4ParticleDefinition* particle = aTrack->GetDefinition();

  G4DecayTable* decayTab = particle->GetDecayTable();
  G4cout << particle->GetParticleName() << " stable? " << particle->GetPDGStable() << " lifetime " << particle->GetPDGLifeTime() << G4endl;
  if( decayTab ) {
    theParticles.insert(particle);
    G4cout << " decayTab " << particle->GetParticleName() << G4endl;
    G4int nDec = decayTab->entries();
    for( G4int ii = 0; ii < nDec; ii++ ) {
      G4VDecayChannel* decayChan = decayTab->GetDecayChannel(ii);
      decayChan->DumpInfo();
    }
  } else {
    if( !particle->GetPDGStable() )  G4cout << " Unstable and NO decayTab " << particle->GetParticleName() << G4endl;
  }

}


//-----------------------------------------------------------------
void SHStudyDecayChannelsUA::EndOfRunAction(const G4Run* )
{
  std::set<G4ParticleDefinition*>::const_iterator ite;
  for( ite = theParticles.begin(); ite != theParticles.end(); ite++ ){
    G4ParticleDefinition* particle = *ite;
    G4DecayTable* decayTab = particle->GetDecayTable();
    G4cout << "%%" << theName << " DECAT_TABLE " << particle->GetParticleName() << G4endl;
    G4int nDec = decayTab->entries();
    for( G4int ii = 0; ii < nDec; ii++ ) {
      G4VDecayChannel* decayChan = decayTab->GetDecayChannel(ii);
      decayChan->DumpInfo();
    }
  }
}


