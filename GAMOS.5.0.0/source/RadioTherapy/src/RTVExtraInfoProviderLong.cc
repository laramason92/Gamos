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
#include "RTVExtraInfoProviderLong.hh"
#include "RTExtraInfoMgr.hh"
#include "RTVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

std::vector<bs32*> RTVExtraInfoProviderLong::theWords;

//----------------------------------------------------------------
RTVExtraInfoProviderLong::RTVExtraInfoProviderLong()
{ 
  
}

//----------------------------------------------------------------
void RTVExtraInfoProviderLong::InitVariables()
{
  theShift = RTExtraInfoMgr::GetInstance()->AddExtraInfoProviderLong(this);
#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(infoVerb) ) G4cout << " RTVExtraInfoProviderLong Shift " << theShift << G4endl;
#endif
  theFileName = GmParameterMgr::GetInstance()->GetStringValue("RTExtraInfoProviderLong:FileName","RTExtraInfoProvider.summ");
#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(infoVerb) ) G4cout << " RTVExtraInfoProviderLong Filename " << theFileName << G4endl;
#endif
  theNBits = G4int(GmParameterMgr::GetInstance()->GetNumericValue(theName+":NBits",32));
#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(infoVerb) ) G4cout << " RTVExtraInfoProviderLong NBits " << theNBits << G4endl;
#endif
  if( theShift == 0 ){
    SetWordsReset(true);
  } else {
    SetWordsReset(false);
    //check that number of bits is OK
  }
  G4int ii1 = (theShift-1)/32;
  if( theShift == 0 ) ii1 = -1;
  for( G4int ii = ii1; ii < (theShift+theNBits-1)/32; ii++ ){
    if( theWords.size() < NUM_EXTRA_LONG ) theWords.push_back( new bs32 );
#ifndef GAMOS_NO_VERBOSE
    if( RTVerb(infoVerb) ) G4cout << "RTVExtraInfoProviderLong WORDS create " << (theNBits-1)/32 << " " << theWords.size() << G4endl;
#endif
  }

  if( theShift+theNBits> NUM_EXTRA_LONG*32 ) {
    G4Exception("RTVExtraInfoProviderLong::RTVExtraInfoProviderLong",
		"Too many bits asked for",
		JustWarning,
		G4String("Asking for "+GmGenUtils::itoa(theShift+theNBits)
			 +" while only it is possible "+GmGenUtils::itoa(NUM_EXTRA_LONG*32)).c_str());
  }
}


//---------------------------------------------------------------------
void RTVExtraInfoProviderLong::SetWordIndex( G4int index )
{
  index += theShift;
  if( index < NUM_EXTRA_LONG*32 ) {
    G4int iw = index/32;
    if( iw != 0 ) index = index%32; 
    theWords[iw]->set(index,1);
#ifndef GAMOS_NO_VERBOSE
    if( RTVerb(debugVerb) ) G4cout << GetName() << " SetWordIndex " 
	   << index << " shift " << theShift << " iw " << iw << " " << *(theWords[iw]) << " " 
	   << theWords[iw]->to_ulong() << G4endl;
#endif
  } //else it is not taken into account!!
}


//----------------------------------------------------------------
void RTVExtraInfoProviderLong::DumpIndices(std::ostream& out)
{
  for( G4int jj = 0; jj < GetNBits(); jj++ ) { 
    out << GetName() << " LONG INDEX " 
	<< "  " << jj 
	<< "  = " << GetIndexName(jj) << G4endl;
  }
}
