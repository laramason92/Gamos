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
#include "GmPhysicsGammaEWPSStandard.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleDefinition.hh"
#include "GmPSComptonScattering.hh"
#include "GmEWPSKleinNishinaComptonModel.hh"
#include "GmPSPhotoElectricEffect.hh"
#include "GmEWPSPEEffectModel.hh"
#include "GmPSGammaConversion.hh"
#include "GmEWPSBetheHeitlerModel.hh"

GmPhysicsGammaEWPSStandard::GmPhysicsGammaEWPSStandard(const G4String& name): GmVPhysicsGamma(name)
{ }

GmPhysicsGammaEWPSStandard::~GmPhysicsGammaEWPSStandard()
{ }

void GmPhysicsGammaEWPSStandard::ConstructProcess()
{
  // Add standard processes for gammas
  
  theParticleIterator -> reset();

  while( (*theParticleIterator)() )
    {
      G4ParticleDefinition* particle = theParticleIterator -> value();
      G4ProcessManager* manager = particle -> GetProcessManager();
      G4String particleName = particle -> GetParticleName();

      if (particleName == "gamma") 
	{
	  GmPSPhotoElectricEffect* photoElectric = new GmPSPhotoElectricEffect("GmEWPSphot"); 
	  GmEWPSPEEffectModel* photoElectricModel = new GmEWPSPEEffectModel(G4Gamma::Gamma(),"GmEWPSphotModel");
	  photoElectric->AddEmModel(0,photoElectricModel);
	  photoElectric->AddPSEmModel(photoElectricModel);
	  manager -> AddDiscreteProcess( photoElectric );

	  GmPSComptonScattering* compt = new GmPSComptonScattering("GmEWPScompt");
	  GmEWPSKleinNishinaComptonModel* theComptonModel = 
	    new GmEWPSKleinNishinaComptonModel(G4Gamma::Gamma(),"GmEWPScomptModel");
	  theComptonModel->SetHighEnergyLimit(100.*TeV);
	  compt->AddEmModel(0, theComptonModel);
	  compt->AddPSEmModel(theComptonModel);
	  manager->AddDiscreteProcess(compt);

	  GmPSGammaConversion* conv = new GmPSGammaConversion("GmEWPSconv");
	  GmEWPSBetheHeitlerModel* theGammaConvModel = new GmEWPSBetheHeitlerModel(G4Gamma::Gamma(),"GmEWPScomptModel");
	  conv->AddEmModel(0, theGammaConvModel);
	  conv->AddPSEmModel(theGammaConvModel);
	  manager -> AddDiscreteProcess(conv);
	}   
    }
}
