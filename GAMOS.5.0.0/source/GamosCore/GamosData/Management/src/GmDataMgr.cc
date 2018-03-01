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
#include "GmDataMgr.hh"
#include "GmVData.hh"
#include "GmDataVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "G4tgrUtils.hh"

#include "Reflex/PluginService.h"

GmDataMgr* GmDataMgr::theInstance = 0;

//----------------------------------------------------------------------
GmDataMgr::GmDataMgr()
{
}

//----------------------------------------------------------------------
GmDataMgr* GmDataMgr::GetInstance()
{
  if( !theInstance ) {
    theInstance = new GmDataMgr;
  }

  return theInstance;
}

//----------------------------------------------------------------------
GmDataMgr::~GmDataMgr()
{
}

//----------------------------------------------------------------------
GmVData* GmDataMgr::BuildData( const G4String& dataName )
{
  GmVData* data;

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(testVerb) ) G4cout << " User::Build1Data " << dataName << G4endl;
#endif

  G4int iSeparator = GetNextSeparator( 0, dataName );
  if( iSeparator == G4int(std::string::npos) ) {
    data = Reflex::PluginService::Create<GmVData*>("GmData" + dataName);
    if( data == 0 ) 
      {
	G4Exception("GmVDataUser::GmVDataUser",
		    "Data type not found",
		    FatalErrorInArgument,
		    G4String("Asking for data: " + dataName).c_str());
      } 
  } else {
    data = new GmVData();
    //-----  decompose the regular expression
    unsigned int dLen = dataName.length();
    std::string expressions;
    std::vector<G4String> words;
    std::vector<G4bool> isSeparator;

    G4String word = dataName.substr(0,iSeparator);
    if( iSeparator != 0 ) {
    // first word has to be a separator
      words.push_back("");
      isSeparator.push_back(1);
      words.push_back( word );
      isSeparator.push_back( IsSeparator(word) );

      word = dataName.substr(iSeparator,1);
      words.push_back( word );
      isSeparator.push_back(1);
    } else {
      word = dataName.substr(0,1);
      words.push_back( word );
      isSeparator.push_back(1);
    }
    G4int iSeparatorNew;
    
    for( ;; ){
      iSeparatorNew = GetNextSeparator( iSeparator+1, dataName );
      
      if( iSeparatorNew != G4int(std::string::npos) ) {
	if( iSeparatorNew-iSeparator-1 != 0 )  {
	  word = dataName.substr(iSeparator+1, iSeparatorNew-iSeparator-1);
	  words.push_back( word );
	  isSeparator.push_back( IsSeparator(word) );
	}
	word = dataName.substr(iSeparatorNew, 1);
	words.push_back( word );
	isSeparator.push_back(1);

	iSeparator = iSeparatorNew;
      } else {
	break;
      } 
    }
    if( iSeparator != G4int(dLen) - 1 ) {
      word = dataName.substr(iSeparator+1, dLen);
      words.push_back( word );
    }
    isSeparator.push_back( IsSeparator(word) );

    
    std::vector<G4String> subExpressions;
    for( unsigned int ii = 0; ii < words.size(); ii++ ){
#ifndef GAMOS_NO_VERBOSE
      if( DataVerb(infoVerb) ) G4cout << " WORD " << words[ii] << " " << isSeparator[ii] << G4endl;
#endif
      if( isSeparator[ii] ) {
	if( ii && isSeparator[ii-1] ) {
	  // modify last subExpressions
	  subExpressions[subExpressions.size()-1] += words[ii];
	} else {
	  subExpressions.push_back(words[ii]);
	}
      } else {
#ifndef GAMOS_NO_VERBOSE
	if( DataVerb(infoVerb) ) G4cout << " GmVDataUser::Build1Data  AddSubData " << words[ii] << " ii " << ii << G4endl;
#endif
	data->AddSubData( CreateData( words[ii] ) );
	// 
      }

    }
      
    for( unsigned int ii = 0; ii < subExpressions.size(); ii++ ){
#ifndef GAMOS_NO_VERBOSE
      if( DataVerb(infoVerb) ) G4cout << " GmVDataUser::Build1Data  AddSubExpression " << subExpressions[ii] << " ii " << ii << G4endl;
#endif
      data->AddSubExpression( subExpressions[ii] );
    }
  }

  return data;
}


//----------------------------------------------------------------
GmVData* GmDataMgr::CreateData( G4String dataName )
{
  GmVData * data = Reflex::PluginService::Create<GmVData*>("GmData" + dataName);
  if( data == 0 ) 
    {
      G4Exception("GmVDataUser::GmVDataUser",
		  "Wrong data type",
		  FatalErrorInArgument,
		  G4String("Asking for data: " + dataName).c_str());
    } 
  
  return data;
} 


//----------------------------------------------------------------
std::string::size_type GmDataMgr::GetNextSeparator( G4int iSeparator, G4String dataName )
{
  if( iSeparator >= G4int(dataName.length()) ) return std::string::npos;

  const char* separators("+-*/()");
  std::string::size_type isepF = std::string::npos;
  for( G4int ii = 0; ii < 6; ii++ ){
    std::string::size_type isepFt = dataName.find(separators[ii],iSeparator);
    //-    G4cout  << " separator " << separators[ii] << " " << isepFt << G4endl;
    // check for case of exponential
    if( ii < 2 && G4int(isepFt) >= 2 ) {
      if( ( dataName[isepFt-1] == 'e' || dataName[isepFt-1] == 'E' )
	  && ( GmGenUtils::IsNumber( dataName[isepFt-2] ) || dataName[isepFt-2] == '.' )
	  && ( GmGenUtils::IsNumber( dataName[isepFt+1] ) ) ){
	 isepFt = dataName.find(isepFt+1,iSeparator);
      }
    }

    if( isepFt != std::string::npos ) {
      isepF = std::min( isepF, isepFt );
    }
  }

  //  G4cout << " GmVDataUser::GetNextSeparator( " <<  G4int(isepF) << " " << dataName << " " << iSeparator << " LEN " << dataName.length() << G4endl;
  return isepF;
}


//----------------------------------------------------------------
G4bool GmDataMgr::IsSeparator( const G4String word )
{
  
  if( G4tgrUtils::WordIsFunction( word ) || GmGenUtils::IsNumber( word ) || GmGenUtils::IsUnit(word) ){
    return true;
  } else {	
    return false;
  }
}
