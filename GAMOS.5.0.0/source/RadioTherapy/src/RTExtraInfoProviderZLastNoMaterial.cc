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
#include "RTExtraInfoProviderZLastNoMaterial.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "RTVerbosity.hh"
#include "G4Region.hh"
#include "G4Track.hh"


//---------------------------------------------------------------------
RTExtraInfoProviderZLastNoMaterial::RTExtraInfoProviderZLastNoMaterial()
{
  std::vector<G4String> materials;
  materials.push_back("G4_AIR");

  materials = GmParameterMgr::GetInstance()->GetVStringValue("RTExtraInfoProviderZLastNoMaterial:Materials",materials);

  for( size_t ii = 0; ii < materials.size(); ii++ ){
    theExcludedMaterials.insert(materials[ii]);
  }

  theWord = 0;
}


//---------------------------------------------------------------------
void RTExtraInfoProviderZLastNoMaterial::UserSteppingAction( const G4Step* aStep )
{
  const G4Track* aTrack = aStep->GetTrack();

  if( theWord ) theWordPrev = *theWord;
  G4ProcessType procType = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessType();
  if( procType != fNotDefined
      && procType !=  fTransportation ) { 
    G4Material* mate = aStep->GetTrack()->GetMaterial();
    if( theExcludedMaterials.find(mate->GetName()) == theExcludedMaterials.end() ){
      *theWord = aTrack->GetPosition().z();
    }
  }

#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(debugVerb) ) G4cout << " RTExtraInfoProviderZLastNoMaterial ( aTrack ) " << *theWord << G4endl;
#endif
  
}


//---------------------------------------------------------------------
void RTExtraInfoProviderZLastNoMaterial::CheckZStop( G4double zstop )
{
  if( *theWord > zstop ) *theWord = theWordPrev;
}
