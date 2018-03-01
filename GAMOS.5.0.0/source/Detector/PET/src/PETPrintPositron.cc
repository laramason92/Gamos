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
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4VVisManager.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"
#include "G4RunManager.hh"

#include <math.h>

#include "PETPrintPositron.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"

//----------------------------------------------------------------
PETPrintPositron::PETPrintPositron()
{
}

//----------------------------------------------------------------
void PETPrintPositron::UserSteppingAction(const G4Step* aStep)
{
  const G4Track* aTrack = aStep->GetTrack();
  ///* for JM
  if( aTrack->GetDefinition()->GetParticleName() == "e+" ) {
    if( aTrack->GetCurrentStepNumber() == 1 ) {
      G4double mass = electron_mass_c2;
      G4double T =  aTrack->GetKineticEnergy();
      G4double velocity = c_light*std::sqrt(T*(T+2.*mass))/(T+mass);
      G4cout << "STEP: 0 " << std::setprecision(6) << 
	aTrack->GetVertexPosition().x() << " " << 
	aTrack->GetVertexPosition().y() << " " << 
	aTrack->GetVertexPosition().z() << " " << 
	aTrack->GetVertexKineticEnergy() << " " << 
	"0" << " " << 
	velocity << " " << G4endl;
    }
    G4cout << "STEP: " << std::setprecision(6) << aTrack->GetCurrentStepNumber() << " " << 
      aTrack->GetPosition().x() << " " << 
      aTrack->GetPosition().y() << " " << 
      aTrack->GetPosition().z() << " " << 
      aTrack->GetKineticEnergy() << " " << 
      aTrack->GetGlobalTime() << " " << 
      aTrack->GetVelocity() << " " << G4endl;
 
  }
 // */


  if( aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() != "annihil" ) return;

  G4bool bPrintPos = TRUE;
  if( bPrintPos ) G4cout << " e+POS " << aTrack->GetPosition().x() << " " <<aTrack->GetPosition().y() << " " <<aTrack->GetPosition().z() << " VTX " <<
aTrack->GetVertexPosition().x() << " " << aTrack->GetVertexPosition().y() << " " <<aTrack->GetVertexPosition().z() << " " << G4endl;
 
}
