#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"

#include "../include/GmExPrimaryGeneratorAction.hh"
#include "../include/GmExVerbosityMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//------------------------------------------------------------------------
GmExPrimaryGeneratorAction::GmExPrimaryGeneratorAction()
{

  /* G4int n_particle = 1;
  particleGun = new G4ParticleGun(n_particle);

// default particle

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle("proton");
  
  particleGun->SetParticleDefinition(particle);
  particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  particleGun->SetParticleEnergy(3.0*GeV);
  */
}

//------------------------------------------------------------------------
GmExPrimaryGeneratorAction::~GmExPrimaryGeneratorAction()
{
}

//------------------------------------------------------------------------
void GmExPrimaryGeneratorAction::GeneratePrimaries(G4Event* evt)
{
  /*  G4double position = -100.;
  particleGun->SetParticlePosition(G4ThreeVector(0.*cm,0.*cm,position));
  
  particleGun->GeneratePrimaryVertex(anEvent);
  */
}
