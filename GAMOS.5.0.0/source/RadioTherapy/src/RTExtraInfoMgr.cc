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
#include "RTExtraInfoMgr.hh"
#include "RTVExtraInfoProviderFloat.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "RTVerbosity.hh"

//----------------------------------------------------------------------
RTExtraInfoMgr* RTExtraInfoMgr::theInstance = 0;

//----------------------------------------------------------------------
RTExtraInfoMgr* RTExtraInfoMgr::GetInstance()
{
  if( !theInstance ) {
    theInstance = new RTExtraInfoMgr;
  }

  return theInstance;

}


//-----------------------------------------------------------------------
RTExtraInfoMgr::RTExtraInfoMgr()
{
}


//-----------------------------------------------------------------------
RTExtraInfoMgr::~RTExtraInfoMgr()
{
}


//-----------------------------------------------------------------------
G4int RTExtraInfoMgr::AddExtraInfoProviderLong( RTVExtraInfoProviderLong* eip )
{
  eip->SetOrder( theProvidersLong.size() + theProvidersFloat.size() );
    //  G4cout << " setorder " << eip->GetName() << " = " << theProvidersLong.size() + theProvidersFloat.size() << G4endl;
  G4int shift = 0;
  for( unsigned int ii = 0; ii < theProvidersLong.size(); ii++ ){
    shift += theProvidersLong[ii]->GetNBits();
  }
  //  G4cout << " return shift " << shift << " " << theProvidersLong.size() << G4endl; 
  theProvidersLong.push_back(eip);
  
  return shift;
}

//-----------------------------------------------------------------------
G4int RTExtraInfoMgr::AddExtraInfoProviderFloat( RTVExtraInfoProviderFloat* eip )
{
  eip->SetOrder( theProvidersLong.size() + theProvidersFloat.size() );
    //  G4cout << " setorder " << eip->GetName() << " = " << theProvidersLong.size() + theProvidersFloat.size() << G4endl;

  theProvidersFloat.push_back(eip);
  return 0;
}


//-----------------------------------------------------------------------
void RTExtraInfoMgr::DumpProviders( std::ostream& out )
{
  DumpProvidersLong( out );
  DumpProvidersFloat( out );
}
//-----------------------------------------------------------------------
void RTExtraInfoMgr::DumpProvidersLong( std::ostream& out )
{
  for( unsigned int ii = 0;ii < theProvidersLong.size(); ii++ ) {
    theProvidersLong[ii]->DumpIndices(out);
  }
}
//-----------------------------------------------------------------------
void RTExtraInfoMgr::DumpProvidersFloat( std::ostream& out )
{
  for( unsigned int ii = 0;ii < theProvidersFloat.size(); ii++ ) {
    theProvidersFloat[ii]->DumpIndices(out);
  }
}

//-----------------------------------------------------------------------
void RTExtraInfoMgr::InitExtraInfo( iaea_record_type *p_iaea_read )
{
  for( int ii = 0; ii < p_iaea_read->iextralong; ii++ ){
    theExtraInfoLongs.push_back(0);
  }
  for( int ii = 0; ii < p_iaea_read->iextrafloat; ii++ ){
    theExtraInfoFloats.push_back(0.);
  }
#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(debugVerb) ) G4cout << " RTExtraInfoMgr::InitExtraInfo N long " << p_iaea_read->iextralong  << G4endl;
#endif
}

//-----------------------------------------------------------------------
void RTExtraInfoMgr::FillExtraInfo( iaea_record_type *p_iaea_read )
{
#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(debugVerb) ) G4cout << " RTExtraInfoMgr::FillExtraInfo N long " << p_iaea_read->iextralong  << G4endl;
#endif
  for( unsigned int ii = 0; ii < theExtraInfoLongs.size(); ii++ ){
    theExtraInfoLongs[ii] = p_iaea_read->extralong[ii];
#ifndef GAMOS_NO_VERBOSE
    if( RTVerb(debugVerb) ) G4cout << " RTExtraInfoMgr::FillExtraInfo  long " << ii << " = " << theExtraInfoLongs[ii] << G4endl;
#endif
  }

  for( unsigned int ii = 0; ii < theExtraInfoFloats.size(); ii++ ){
    theExtraInfoFloats[ii] = p_iaea_read->extrafloat[ii];
#ifndef GAMOS_NO_VERBOSE
    if( RTVerb(debugVerb) ) G4cout << " RTExtraInfoMgr::FillExtraInfo  float " << ii << " = " << theExtraInfoFloats[ii] << G4endl;
#endif
  }

}

//-----------------------------------------------------------------------
std::vector<bs32*> RTExtraInfoMgr::GetWordsLong()
{
  //long extra info providers share the bits of the words
  return RTVExtraInfoProviderLong::GetWords();
}

//-----------------------------------------------------------------------
std::vector<float*> RTExtraInfoMgr::GetWordsFloat()
{
  //each float extra info provider can have one and only one word
  std::vector<float*> words;
  for( unsigned int ii = 0;ii < theProvidersFloat.size(); ii++ ) {
    words.push_back( theProvidersFloat[ii]->GetWord() );
  }
  return words;
}
