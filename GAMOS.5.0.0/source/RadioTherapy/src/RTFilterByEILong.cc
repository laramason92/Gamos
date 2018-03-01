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
#include "RTFilterByEILong.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "RTVerbosity.hh"
#include "RTExtraInfoMgr.hh"

//---------------------------------------------------------------
RTFilterByEILong::RTFilterByEILong(G4String name)
  :GmVFilter(name)
{
  theExtraInfoMgr = RTExtraInfoMgr::GetInstance();
}


//---------------------------------------------------------------
RTFilterByEILong::~RTFilterByEILong()
{ 
}


//---------------------------------------------------------------
G4bool RTFilterByEILong::AcceptStep(const G4Step* aStep)
{
  return AcceptTrack( aStep->GetTrack() );
}

//---------------------------------------------------------------
G4bool RTFilterByEILong::AcceptTrack(const G4Track*)
{
  G4int nWords = theExtraInfoMgr->GetNumberOfLongs();
  if( nWords == 0 ) return 0;

  G4bool bFound;
  std::set<G4int>::const_iterator ite;
  for( ite = theIndices.begin(); ite != theIndices.end(); ite++ ){
    bFound = false;
    G4int iw = (*ite)/32;
    if( theExtraInfoMgr->GetExtraInfoLong(iw) & 1<< *ite ) {
      bFound = true;
    }
#ifndef GAMOS_NO_VERBOSE
    if( RTVerb(debugVerb) ) G4cout << " RTFilterByEILong::AcceptStep " << *ite << " = " << bFound << " in " << theExtraInfoMgr->GetExtraInfoLong(iw) << G4endl;
#endif
    if( !bFound ) return FALSE;
  }

  return TRUE;
}


//---------------------------------------------------------------
void RTFilterByEILong::show()
{
    G4cout << "----RTFilterByEILong ------"<<G4endl;
}


//---------------------------------------------------------------
void RTFilterByEILong::SetParameters( std::vector<G4String>& params)
{
  if( params.size() < 1  ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("RTFilterByEILong::SetParameters",
		"There should be at least one parameter: INDEX_1",
		FatalErrorInArgument,
		"There is no parameter");
  }
  
  for( unsigned int ii = 0; ii < params.size(); ii++ ){
    theIndices.insert( atoi(params[ii]) );
  }
}
