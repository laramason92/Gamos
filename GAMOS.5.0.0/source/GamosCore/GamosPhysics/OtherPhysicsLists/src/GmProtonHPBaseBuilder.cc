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
#include "GmProtonHPBaseBuilder.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleHPInelasticData.hh"

void GmProtonHPBaseBuilder::Build()
{
  wasActivated = true;
  theProtonInelastic=new G4ProtonInelasticProcess;
  std::vector<G4VProtonBuilder *>::iterator i;
  for(i=theModelCollections.begin(); i!=theModelCollections.end(); i++)
    {
      (*i)->Build(theProtonInelastic);
    }

  G4ParticleHPInelasticData* theProtonHPInelasticData=new G4ParticleHPInelasticData(G4Proton::Proton(),"G4PROTONHPDATA");
  theProtonHPInelasticData->SetMaxKinEnergy(200.);
  theProtonInelastic->AddDataSet(theProtonHPInelasticData);

  G4ProcessManager * theProcMan = G4Proton::Proton()->GetProcessManager();
  theProcMan->AddDiscreteProcess(theProtonInelastic);
}

GmProtonHPBaseBuilder::
GmProtonHPBaseBuilder(): wasActivated(false)  
{

}

GmProtonHPBaseBuilder::
~GmProtonHPBaseBuilder() 
{
  delete theProtonInelastic;
}

