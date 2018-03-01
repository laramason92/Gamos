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
#include "GmPSeBremsstrahlung.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsVerbosity.hh"
#include "G4Gamma.hh"
#include "GmEWPSBremsstrahlungModel.hh"
#include "G4LossTableManager.hh"

//-------------------------------------------------------------------------
GmPSeBremsstrahlung::GmPSeBremsstrahlung(const G4String& aName)
  : GmVPSEnergyLossProcess(aName),
  isInitialised(false)
{
  SetProcessSubType(fBremsstrahlung);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4bool GmPSeBremsstrahlung::IsApplicable(const G4ParticleDefinition& p)
{
  return (&p == G4Electron::Electron() || &p == G4Positron::Positron());
}

//-------------------------------------------------------------------------
void GmPSeBremsstrahlung::InitialiseEnergyLossProcess(
                                                const G4ParticleDefinition* p,
                                                const G4ParticleDefinition* )
{
  if(!isInitialised) {
    particle = p;
    SetSecondaryParticle(G4Gamma::Gamma());
    SetIonisation(false);

    if (!EmModel(1)) SetEmModel(new GmEWPSBremsstrahlungModel(), 1);
    //GAMOS    if (!EmModel(2)) SetEmModel(new G4eBremsstrahlungRelModel(), 2);

    EmModel(1)->SetLowEnergyLimit(MinKinEnergy());
    EmModel(1)->SetHighEnergyLimit(MaxKinEnergy()); //GAMOS
    //GAMOS    EmModel(1)->SetHighEnergyLimit(EmModel(2)->LowEnergyLimit());
    //GAMOS    EmModel(2)->SetHighEnergyLimit(MaxKinEnergy());
                
    //    G4VEmFluctuationModel* fm = 0;
    //GAMOS    AddEmModel(1, EmModel(1), fm);
    //GAMOS    AddEmModel(2, EmModel(2), fm);
    isInitialised = true;
  }
  G4LossTableManager* man = G4LossTableManager::Instance(); 
  G4double eth = man->BremsstrahlungTh(); 
  EmModel(1)->SetSecondaryThreshold(eth);
  //GAMOS  EmModel(2)->SetSecondaryThreshold(eth);

  // Only high energy model LMP flag is ON/OFF
  EmModel(1)->SetLPMFlag(false);

#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(infoVerb) ) G4cout << "GmPSeBremsstrahlung::GmPSeBremsstrahlung  registering model " << MinKinEnergy() << " " << MaxKinEnergy() << " " << EmModel(1) << " " << EmModel(2) << "NM  " << NumberOfModels() << " eth " << eth << G4endl;
#endif
  //GAMOS  EmModel(2)->SetLPMFlag(man->LPMFlag());

}


void GmPSeBremsstrahlung::PrintInfo()
{
  if(EmModel(1)) {
    G4LossTableManager* man = G4LossTableManager::Instance(); 
    G4double eth = man->BremsstrahlungTh(); 
    G4cout << "      LPM flag: " << man->LPMFlag() << " for E > " 
	   << EmModel(1)->HighEnergyLimit()/GeV << " GeV";
    if(eth < DBL_MAX) G4cout << ",  HighEnergyThreshold(GeV)= " << eth/GeV; 
    G4cout << G4endl;
  }
}
