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
#include "RTExtraInfoProviderInteractions.hh"
#include "RTVerbosity.hh"
#include "G4Region.hh"
#include "G4Track.hh"
#include "G4ProcessType.hh"

//---------------------------------------------------------------------
RTExtraInfoProviderInteractions::RTExtraInfoProviderInteractions()
{
}


//---------------------------------------------------------------------
void RTExtraInfoProviderInteractions::PreUserTrackingAction( const G4Track*  )
{
  if( bWordsReset ) {
    for( unsigned int ii = 0; ii < theWords.size(); ii++ ) {
      theWords[ii]->reset();
    }
  }
}
 

//---------------------------------------------------------------------
void RTExtraInfoProviderInteractions::UserSteppingAction( const G4Step* aStep )
{
  G4ProcessType procType = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessType();
  if( procType != fElectromagnetic 
      && procType != fOptical
      && procType != fHadronic
      && procType != fPhotolepton_hadron
      && procType != fDecay ) {
    return;
  }

  G4Track* aTrack = aStep->GetTrack();
  SetWordIndexFromRegion( aTrack );
}
