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
#include "GmPhysicsElectronEWPSLowEner.hh"
#include "G4eBremsstrahlung.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleDefinition.hh"
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsMultipleScattering.hh"
#include "G4UniversalFluctuation.hh"

//?? #include "GmPSeIonisation.hh"
#include "G4eIonisation.hh"
#include "G4LivermoreIonisationModel.hh"

#include "GmPSeBremsstrahlung.hh"
//#include "G4LivermoreBremsstrahlungModel.hh"
#include "GmEWPSLivermoreBremsstrahlungModel.hh"

#include "GamosCore/GamosUtils/include/GmG4Utils.hh"


GmPhysicsElectronEWPSLowEner::GmPhysicsElectronEWPSLowEner(const G4String& name): GmVPhysicsElectron(name)
{ }

GmPhysicsElectronEWPSLowEner::~GmPhysicsElectronEWPSLowEner()
{ }

void GmPhysicsElectronEWPSLowEner::ConstructProcess()
{
  // Add EEDL processes for electrons
  
  theParticleIterator->reset();

  while( (*theParticleIterator)() )
  {
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
    
    if (particleName == "e-") 
    {
      GmPhysicsMultipleScattering* msc = new GmPhysicsMultipleScattering("msc","Electron");
      msc->SetStepLimitType(fUseDistanceToBoundary);
      pmanager->AddProcess(msc,                   -1, 1, 1);
      
      // Ionisation
      //??      GmPSeIonisation* eIoni = new GmPSeIonisation(");
      G4eIonisation* eIoni = new G4eIonisation();
      G4LivermoreIonisationModel* ioniModel = new G4LivermoreIonisationModel();
      eIoni->AddEmModel(0, ioniModel, new G4UniversalFluctuation() );

      //      AddDeexcitation( eIoni, ioniModel );

      eIoni->SetStepFunction(0.2, 100*um); //     
      pmanager->AddProcess(eIoni,                 -1, 2, 2);

      // Bremsstrahlung
      GmPSeBremsstrahlung* eBremProcess = new GmPSeBremsstrahlung("GmEWPSBrems");
      GmEWPSLivermoreBremsstrahlungModel* bremsModel = new GmEWPSLivermoreBremsstrahlungModel();
      SelectBremssAngularDist( bremsModel );
      eBremProcess->AddEmModel(0, bremsModel);
      eBremProcess->AddPSEmModel(bremsModel);
      pmanager->AddProcess(eBremProcess, -1,-3, 3);

    }
  }
}
