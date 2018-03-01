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
#include "GmPhysicsGammaEWPSLowEner.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleDefinition.hh"

#include "GmPSPhotoElectricEffect.hh"
#include "GmEWPSLivermorePhotoElectricModel.hh"

#include "GmPSComptonScattering.hh"
#include "GmEWPSLivermoreComptonModel.hh"

#include "GmPSGammaConversion.hh"
#include "G4LivermoreGammaConversionModel.hh"

#include "G4RayleighScattering.hh" 
#include "G4LivermoreRayleighModel.hh"

GmPhysicsGammaEWPSLowEner::GmPhysicsGammaEWPSLowEner(const G4String& name): GmVPhysicsGamma(name)
{ }

GmPhysicsGammaEWPSLowEner::~GmPhysicsGammaEWPSLowEner()
{ }

void GmPhysicsGammaEWPSLowEner::ConstructProcess()
{
  // Add EPDL processes for gammas
  
  theParticleIterator -> reset();

  G4double LivermoreHighEnergyLimit = GeV;
  while( (*theParticleIterator)() )
  {
      G4ParticleDefinition* particle = theParticleIterator -> value();
      G4ProcessManager* pmanager = particle -> GetProcessManager();
      G4String particleName = particle -> GetParticleName();
      
    if (particleName == "gamma") {

      GmPSPhotoElectricEffect* photoElectric = new GmPSPhotoElectricEffect();
      GmEWPSLivermorePhotoElectricModel* photoElectricModel = 
	new GmEWPSLivermorePhotoElectricModel();
      //      AddDeexcitation( photoElectric, photoElectricModel);
      photoElectricModel->SetHighEnergyLimit(LivermoreHighEnergyLimit);
      photoElectric->AddEmModel(0, photoElectricModel);
      photoElectric->AddPSEmModel(photoElectricModel);
      pmanager->AddDiscreteProcess(photoElectric);

      GmPSComptonScattering* compt = new GmPSComptonScattering();
      GmEWPSLivermoreComptonModel* comptModel = 
	new GmEWPSLivermoreComptonModel();
      comptModel->SetHighEnergyLimit(LivermoreHighEnergyLimit);
      compt->AddEmModel(0, comptModel);
      pmanager->AddDiscreteProcess(compt);
      compt->AddPSEmModel(comptModel);

      GmPSGammaConversion* conv = new GmPSGammaConversion();
      G4LivermoreGammaConversionModel* convModel = 
	new G4LivermoreGammaConversionModel();
      convModel->SetHighEnergyLimit(LivermoreHighEnergyLimit);
      conv->AddEmModel(0, convModel);
      pmanager->AddDiscreteProcess(conv);

      G4RayleighScattering* rayl = new G4RayleighScattering();
      G4LivermoreRayleighModel* raylModel = new G4LivermoreRayleighModel();
      raylModel->SetHighEnergyLimit(LivermoreHighEnergyLimit);
      rayl->AddEmModel(0, raylModel);
      pmanager->AddDiscreteProcess(rayl);
      
    }
  }
}
