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
#include "GmVHitsIOMgr.hh"
#include "GmHitsEventMgr.hh"
#include "GmSDVerbosity.hh"
#include "GmEventTimeExtractorFromHit.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserAction.hh"

#include "G4RunManager.hh"
#include "G4EventManager.hh"

//-----------------------------------------------------------------------
GmVHitsIOMgr::GmVHitsIOMgr()
{ 
  theHitsEventMgr = GmHitsEventMgr::GetInstance(); // store a copy for optimisation
  theEventTimeExtractor = 0;

  theLastHit = 0;
  bLastHitRead = FALSE;
  bFirstHitRead = FALSE;

}

//-----------------------------------------------------------------------
void GmVHitsIOMgr::WriteAllHits()
{

  gamosSDHitMap hits = theHitsEventMgr->GetHitLists();
 
  G4int iev1 = GetFirstEventWithHit( hits );

  DeletePreviousEventHits( iev1 );

  gamosSDHitMap::const_iterator itehm;
  hitVector::const_iterator iteh;
  //store hits to be written, so that they are later written by event ID, not by SD type
  std::multimap<G4int,GmHit*> hits2write;
  for( itehm = hits.begin(); itehm != hits.end(); itehm++ ){
    GmHitList* hitlist = (*itehm).second;
    const hitVector* hitVector = hitlist->GetHitsCompatibleInTime();
    for( iteh = hitVector->begin(); iteh != hitVector->end(); iteh++ ){
      GmHit* hit = *iteh;
      if( !HitAlreadyWritten(hit) ) {
	hits2write.insert( std::multimap<G4int,GmHit*>::value_type( hit->GetEventID(), hit )); 
	theEvHitsIDs.insert( mmil::value_type( hit->GetEventID(), hit->GetDetUnitID()) );
#ifndef GAMOS_NO_VERBOSE
	if( SDVerb(debugVerb) ) G4cout << "GmVHitsIOMgr::WriteAllHits add EvHitsID " <<  hit->GetEventID() << " " << hit->GetDetUnitID() << " size " << theEvHitsIDs.size() << G4endl;
#endif
	
      }
      
    }
    
  }
  
  std::multimap<G4int,GmHit*>::const_iterator itehw;
  for( itehw =  hits2write.begin(); itehw !=  hits2write.end(); itehw++ ){
    WriteHit( (*itehw).second );
  }

}

//-----------------------------------------------------------------------
void GmVHitsIOMgr::WriteHitsInCurrentEvent()
{

}


//-----------------------------------------------------------------------
void GmVHitsIOMgr::WriteHitsInEvent( G4int )
{
}


//-----------------------------------------------------------------------
G4int GmVHitsIOMgr::GetFirstEventWithHit( gamosSDHitMap hits )
{
  G4int evtID1 = INT_MAX;
  gamosSDHitMap::const_iterator ite;
  for( ite = hits.begin(); ite != hits.end(); ite++ ){
    if( (*ite).second->size() != 0 ) {
      G4int evtID = (*((*ite).second->begin()))->GetEventID();
      if( evtID < evtID1 ) evtID1 = evtID;
    }
  }

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(debugVerb) ) G4cout << "GmVHitsIOMgr::GetFirstEventWithHit " << evtID1 << G4endl;
#endif

  return evtID1;

}


//-----------------------------------------------------------------------
void GmVHitsIOMgr::DeletePreviousEventHits( G4int iev1 )
{
  std::vector<mmil::iterator> delites;
  //  mmil::reverse_iterator ite;
  mmil::iterator ite = theEvHitsIDs.end();
  for( ite = theEvHitsIDs.begin();ite != theEvHitsIDs.end(); ite++ ){
    if( (*ite).first < iev1 ) {
      delites.push_back( ite );
    }
  }

  std::vector<mmil::iterator>::reverse_iterator itev;
  for( itev = delites.rbegin(); itev != delites.rend(); itev++ ){
#ifndef GAMOS_NO_VERBOSE
    if( SDVerb(debugVerb) ) G4cout << "GmVHitsIOMgr::DeletePreviousEventHits() hit deleted of event " << (*(*itev)).first << " < " << iev1 << " ID " << (*(*itev)).second << G4endl;
#endif
    theEvHitsIDs.erase(*itev);
  }

}


//-----------------------------------------------------------------------
G4bool GmVHitsIOMgr::HitAlreadyWritten(GmHit* hit)
{
  G4bool bIsFound = false;
  std::pair<mmil::iterator, mmil::iterator> pite = theEvHitsIDs.equal_range( hit->GetEventID() ); 
  mmil::iterator ite;
  for( ite = pite.first; ite != pite.second; ite++ ){
#ifndef GAMOS_NO_VERBOSE
    if( SDVerb(debugVerb) ) G4cout << "GmVHitsIOMgr::HitAlreadyWritten check hit in event " << hit->GetEventID() << " ?= " << (*ite).first << " detID " << hit->GetDetUnitID() << " ?= " << (*ite).second << G4endl;
#endif
    if( (*ite).second == hit->GetDetUnitID() ) {
      bIsFound = true;
#ifndef GAMOS_NO_VERBOSE
      if( SDVerb(debugVerb) ) G4cout << "GmVHitsIOMgr::HitAlreadyWritten hit in event " << (*ite).first << " detID " << (*ite).second << G4endl;
#endif
      break;
    }
  }

  return bIsFound;
}


//-----------------------------------------------------------------------
GmVHitsIOMgr::~GmVHitsIOMgr()
{
}

