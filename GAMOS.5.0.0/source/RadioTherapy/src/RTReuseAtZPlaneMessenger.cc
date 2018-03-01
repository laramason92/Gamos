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
#include "RTReuseAtZPlaneMessenger.hh"
#include "RTReuseAtZPlaneProcess.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
RTReuseAtZPlaneMessenger::RTReuseAtZPlaneMessenger()
{ 
  theRTDir = new G4UIdirectory("/gamos/RT/");
  theRTDir->SetGuidance("Command to define uniform magnetic fields");
  
  theReuseCmd = new G4UIcmdWithoutParameter("/gamos/RT/ReuseAtZPlane",this);  
  theReuseCmd->SetGuidance("Define magnetic field vector: FIELD_X FIELD_Y FIELD_Z");
  theReuseCmd->AvailableForStates(G4State_Idle); 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
RTReuseAtZPlaneMessenger::~RTReuseAtZPlaneMessenger()
{
  delete theReuseCmd;
  delete theRTDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
void RTReuseAtZPlaneMessenger::SetNewValue(G4UIcommand* command,G4String)
{ 
  if( command == theReuseCmd ) {
    G4ParticleTable::G4PTblDicIterator* theParticleIterator = G4ParticleTable::GetParticleTable()->GetIterator();
    theParticleIterator->reset();

    RTReuseAtZPlaneProcess* reuseProc = new RTReuseAtZPlaneProcess;
    while( (*theParticleIterator)() ){
      G4ParticleDefinition* particle = theParticleIterator->value();
      G4ProcessManager* pmanager = particle->GetProcessManager();
      pmanager->AddDiscreteProcess( reuseProc );
    }
  }

}
