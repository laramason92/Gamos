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
#include "RTVExtraInfoProviderLongByRegion.hh"
#include "iaea_record.hh"
#include "RTVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosGeometry/include/GmRegionCutsMgr.hh"
#include "G4Region.hh"
#include "G4Track.hh"


//---------------------------------------------------------------------
RTVExtraInfoProviderLongByRegion::RTVExtraInfoProviderLongByRegion()
{
  theRegionCutsMgr = GmRegionCutsMgr::GetInstance();
}


//---------------------------------------------------------------------
void RTVExtraInfoProviderLongByRegion::SetWordIndexFromRegion( const G4Track* aTrack )
{ 
  G4Region* reg = aTrack->GetVolume()->GetLogicalVolume()->GetRegion();
  SetWordIndex( theRegionCutsMgr->GetRegionIndex( reg ) );
}


//---------------------------------------------------------------------
G4String RTVExtraInfoProviderLongByRegion::GetIndexName( G4int index ) const
{
  index -= theShift;
#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(debugVerb) ) G4cout << "RTVExtraInfoProviderLongByRegion::GetIndexName " << index << " shift " << theShift << G4endl;
#endif
  G4Region* reg = theRegionCutsMgr->GetIndexRegion( index );
  if( reg == 0 ){
    G4Exception("RTVExtraInfoProviderLongByRegion::GetIndexName",
		"Wrong index",
		FatalException,
		G4String("index not found "+GmGenUtils::itoa(index)).c_str());
  }
  return reg->GetName();  
}


//---------------------------------------------------------------------
void RTVExtraInfoProviderLongByRegion::DumpIndices(std::ostream& out)
{
  theRegionCutsMgr->BuildRegionIndices();
  
  std::map<G4Region*,G4int> regInd = theRegionCutsMgr->GetRegionIndices();
  std::map<G4Region*,G4int>::const_iterator ite;
#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(debugVerb) ) G4cout << " RTVExtraInfoProviderLongByRegion::BuildIndices n regions " << regInd.size() << G4endl;
#endif
  for( ite = regInd.begin(); ite != regInd.end(); ite++ ) {
    out << GetName() << " LONG INDEX " << (*ite).second+theShift << " REGION= " << (*ite).first->GetName() << G4endl;
  }
}
