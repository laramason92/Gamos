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
#include "GamosCore/GamosUtils/include/GmFileIn.hh"

#include <stdlib.h>
#include <iostream>
#include <fstream>

std::vector<GmFileIn*> GmFileIn::theInstances;

GmFileIn::GmFileIn()
{
  theSeparator = ' ';
}

//-----------------------------------------------------------------------
GmFileIn& GmFileIn::GetInstance( const G4String& filename )
{
  std::vector<GmFileIn*>::const_iterator vfcite;
  for( vfcite = theInstances.begin(); vfcite != theInstances.end(); vfcite++) {
    if( (*vfcite)->GetName() == filename) {
      return *(*vfcite);
    }
  }

  GmFileIn* instance = 0;
  if( vfcite == theInstances.end() ) {
    instance = new GmFileIn( filename );
    
    instance->theCurrentFile = -1;
    instance->OpenNewFile( filename.c_str() );
    instance->SetSeparator(' ');
    instance->SetSuppressQuotes(1);

    theInstances.push_back( instance );

  }

  return *instance;

}

//-----------------------------------------------------------------------
void GmFileIn::OpenNewFile( const char* filename )
{ 
  theCurrentFile++;
  std::ifstream* fin = new std::ifstream(filename);
  theifstream = fin;
  theFiles.push_back(fin);

  //-  G4int lineno = new G4int;
  //-  G4int lineno = 0;
  theLineNo.push_back( 0 );

  theNames.push_back( filename );

#ifndef OS_SUN_4_2
  if( !fin->is_open()) {
    G4cerr << "!!!! Input file does not exist: " << filename << G4endl;
    exit(1);
  }
#endif
}



//-----------------------------------------------------------------------
GmFileIn& GmFileIn::GetInstanceOpened( const G4String& filename )
{
  GmFileIn& filein = GmFileIn::GetInstance(filename);
  if (filein.GetName() != filename ) {
    G4cerr << "Error: file not opened yet " << filename << G4endl; 
    exit(0); 
  } else {
    return filein;
  }
}


//----------------------------------------------------------------------- 
G4int GmFileIn::GetWordsInLine(std::vector<G4String>& wordlist)
{
  G4int isok = 1;

  //---------- Read a line of file:
  //@@@@--- Cannot be read with a istream_iterator, because it uses G4cout, and then doesn't read '\n'
  //----- Clear wordlist
  G4int wsiz = wordlist.size();
  G4int ii;
  for (ii = 0; ii < wsiz; ii++) {
    wordlist.pop_back();
  } 

  //---------- Loop lines while there is an ending '\' or line is blank   
  const G4int NMAXLIN = 10000;
  char ltemp[NMAXLIN]; //there won't be lines longer than NMAXLIN characters
  for (;;) {
    (theLineNo[theCurrentFile])++;
    for( ii = 0; ii < NMAXLIN; ii++) ltemp[ii] = ' ';
    theFiles[theCurrentFile]->getline( ltemp, NMAXLIN ); 
    //    G4cout << " FILEIN ltemp " << ltemp << G4endl;
    //---------- Check for lines longer than NMAXLIN character
    G4int ii;
    for ( ii=0; ii < NMAXLIN; ii++) {
      if ( ltemp[ii] == '\0' ) break;
    }
    if ( ii == NMAXLIN-1 ) {
      ErrorInLine();
      G4cerr << "!!!!FATAL ERROR in GmFileIn:  line longer than " << NMAXLIN << " characters" << 
	G4endl << " please split it putting a '\\' at the end of line" << G4endl;
      exit(0);
    }
    
    //---------- End of file
    //-    if ( theFiles[theCurrentFile]->eof() ) {
    if ( eof() ) {
      if( wordlist.size() != 0 ) {
	DumpException("File does not end with an empty line");
      }
      //t          exit(0);
      return 0;
    }
    
    //---------- Convert line read to istrstream to split it in words 
    std::istringstream istr_line(ltemp);
     
    //--------- count how many words are there in ltemp (this sohuld not be needed, but sun compiler has problems) !! this has to be nvestigated...
    G4int NoWords = 0;
    char* tt = ltemp;
    G4String stemp(ltemp);
    std::vector<G4int> separators;
    G4int it = 0;
    do{ 
      if( *tt != theSeparator && *(tt) != '\0' ) {
	if( tt == ltemp) {
	  NoWords++;
	  //	  G4cout << it << " NoWords" << NoWords << ltemp << G4endl;
	  separators.push_back(it);
	} else if( *(tt-1) == theSeparator || *(tt-1) == '\015' ||  *(tt-1) == '\t') {
	  NoWords++; 
	  separators.push_back(it);
	  //	  G4cout << it << " NoWords" << NoWords << ltemp << G4endl;
	}
      } else if( theSeparator != ' ' && *(tt-1) == theSeparator ) { // two separators together
	NoWords++; 
	separators.push_back(it);
	//	G4cout << it << " NoWords" << NoWords << ltemp << G4endl;
      }
      tt++;
      it++;
    }while((*tt != '\0') && (stemp.length()!=0));
    G4String stempt (ltemp);
    if(stempt.length() == 0) NoWords = 0;
    
    //--------- Read words from istr_line and write them into wordlist
    //    G4int stre = 1;
    //-    G4int sep1 = 0;
    for( ii=0; ii < NoWords; ii++) {
      G4String stemp = "";
      if( theSeparator == ' ' ) {
	    istr_line >> stemp;   //?? gives warning in Insure++
	//	G4cout << " STEMP BUILT " << stemp << G4endl;
      } else {
        G4int sep1 = separators[ii];
		if( ii != NoWords-1 ) {
          stemp = stempt.substr(sep1,separators[ii+1]-sep1-1);
		} else {
		  stemp = stempt.substr(sep1,stempt.length()-sep1);
		}
	/*	size_t sep2 = stempt.find(theSeparator,sep1);
	if( sep2 == std::string::npos ) sep2 = stempt.length();
	stemp = stempt.substr(sep1,sep2-sep1);
	G4cout << " STEMP BUILT " << stemp << G4endl;
	sep1 = sep2+1;*/

      }
      if ( theSeparator == ' ' && stemp.length() == 0 ) break;
      G4int comment = stemp.find(G4String("//") );
      //    G4cout << "!!!COMMENT" << comment << stemp.c_str() << G4endl;
      if ( comment == 0 ) {
	break; 
      } else if ( comment > 0 ) {
	stemp = stemp.substr( 0, comment );
	wordlist.push_back(stemp);
	break;
	//-   for( int jj=0; jj < stemp.length()-comment; jj++) stemp.pop_back();
      } 
      wordlist.push_back(stemp);
    }
    
    //These two algorithms should be the more STL-like way, but they don't work for files whose lines end without '\015'=TAB (STL problem: doesn't find end of string??)
    // istream_iterator<G4String, ptrdiff_t> G4String_iter(istr_line);
    // istream_iterator<G4String, ptrdiff_t> eosl;
    // copy(G4String_iter, eosl, back_inserter(wordlist));
    // typedef istream_iterator<G4String, ptrdiff_t> G4String_iter;
    // copy(G4String_iter(istr_line), G4String_iter(), back_inserter(wordlist));
    
    if ( wordlist.size() != 0 ) {
      if( (*(wordlist.end()-1)).compare("\\") == 0 ) {   //use '\' to mark continuing line  
	wordlist.pop_back();
      } else {
	break;
      }
    }
  }
  
  if( bSuppressQuotes ) {
    //--------- A pair of double quotes delimits a word, therefore, look for the
    //          case where there is more than one word between two double quotes
    std::vector<G4String> wordlist2;
    G4String wordq = "";
    unsigned int imerge = 0;
    for( size_t ii = 0; ii < wordlist.size(); ii++)
      {
	if( wordlist[ii].substr(0,1) == "\"" )
	  {
	    imerge = 1;
	  } 
    if( wordlist[ii][ wordlist[ii].size()-1 ] == '\"' )
      {
	if( imerge != 1 )
	  {
	    if( bCheckQuotes ) {
	      G4String err1 = " word with trailing '\"' while there is no";
	      G4String err2 = " previous word with leading '\"' in line ";
	      G4String err = err1 + err2;
	      DumpException(err);
	    }
	  }
	imerge = 2;
      }
    if( imerge == 0 )
      {
	wordlist2.push_back( wordlist[ii] );
      }
    else if( imerge == 1 )
      {
	if( wordq == "" )
	  {
	    wordq.append( wordlist[ii].substr(1,wordlist[ii].size()) );
	  }
	else
	  {
	    wordq.append( wordlist[ii].substr(0,wordlist[ii].size()) );
	  }
	wordq.append(" ");
      }
    else if( imerge == 2 )
      {
	if( wordq == "" )
	  {
	    wordq.append( wordlist[ii].substr(1,wordlist[ii].size()-2));
	  }
	else
	  {
	    wordq.append( wordlist[ii].substr(0,wordlist[ii].size()-1) );
	  } 
	wordlist2.push_back( wordq );
	wordq = "";
	imerge = 0;
      }
      }
    if( imerge == 1 )
      {
	if( bCheckQuotes ) {
	  G4String err1 = " word with leading '\"' in line while there is no";
	  G4String err2 = " later word with trailing '\"' in line ";
	  G4String err = err1 + err2;
	  DumpException(err);
	}
  }
    
    wordlist = wordlist2;

  }

  //or why not like this?:
  //typedef istream_iterator<G4String, ptrdiff_t> string_iter;
  //copy(string_iter(istr_line), string_iter(), back_inserter(wordlist));
  
  //-  G4cout << " checking for include " << wordlist[0] << G4endl;
  // check if including a new file
  if( wordlist[0] == "#include" ) {
    if( wordlist.size() != 2 ) {
      ErrorInLine();
      G4cerr << "'#include' should have as second argument the filename " << G4endl;
      exit(0);
    }
    //-    G4cout << "include found " << G4endl;
    OpenNewFile( wordlist[1].c_str() );
    isok = GetWordsInLine( wordlist);

  }

  return isok;  
}

//----------------------------------------------------------------------- 
G4int GmFileIn::GetWordsInLineDouble(std::vector<G4double>& wordlist)
{
  G4int isok = 1;

  //---------- Read a line of file:
  //@@@@--- Cannot be read with a istream_iterator, because it uses G4cout, and then doesn't read '\n'
  //----- Clear wordlist
  G4int wsiz = wordlist.size();
  G4int ii;
  for (ii = 0; ii < wsiz; ii++) {
    wordlist.pop_back();
  } 

  //---------- Loop lines while there is an ending '\' or line is blank   
  const G4int NMAXLIN = 10000;
  char ltemp[NMAXLIN]; //there won't be lines longer than NMAXLIN characters

  (theLineNo[theCurrentFile])++;
  for( ii = 0; ii < NMAXLIN; ii++) ltemp[ii] = ' ';
  theFiles[theCurrentFile]->getline( ltemp, NMAXLIN ); 
#ifdef FILEIN_DEBUG
  G4cout << " GmFileIn::GetWordsInLineDouble " << ltemp << G4endl;
#endif
  //---------- Check for lines longer than NMAXLIN character
  for ( ii=0; ii < NMAXLIN; ii++) {
      if ( ltemp[ii] == '\0' ) break;
  }
  if ( ii == NMAXLIN-1 ) {
    ErrorInLine();
    G4cerr << "!!!!FATAL ERROR in GmFileIn:  line longer than " << NMAXLIN << " characters" << 
      G4endl << " please split it putting a '\\' at the end of line" << G4endl;
    exit(0);
  }
  
  //---------- End of file
  //-    if ( theFiles[theCurrentFile]->eof() ) {
  if ( eof() ) {
#ifdef FILEIN_DEBUG
  G4cout << " GmFileIn::GetWordsInLineDouble END OF FILE FOUND " << G4endl;
#endif
    return 0;
  }
  
  //---------- Convert line read to istrstream to split it in words 
  std::istringstream istr_line(ltemp);
  
  //--------- count how many words are there in ltemp (this sohuld not be needed, but sun compiler has problems) !! this has to be nvestigated...
  G4int NoWords = 0;
  char* tt = ltemp;
  G4String stemp(ltemp);
  do{ 
    if( *tt != theSeparator && *(tt) != '\0' ) {
      if( tt == ltemp) {
	NoWords++;
#ifdef FILEIN_DEBUG
	G4cout << " GmFileIn::GetWordsInLineDouble first word " << NoWords << G4endl;
#endif
      } else if( *(tt-1) == theSeparator ||  *(tt-1) == '\015' ||  *(tt-1) == '\t') {
	NoWords++; 
#ifdef FILEIN_DEBUG
	G4cout << " GmFileIn::GetWordsInLineDouble new word " << NoWords << G4endl;
#endif
      }
    }
    tt++;
  }while((*tt != '\0') && (stemp.length()!=0));
  G4String stempt (ltemp);

  if(stempt.length() == 0) NoWords = 0;
  
  //--------- Read words from istr_line and write them into wordlist
  //    G4int stre = 1;
  //-  G4int sep1 = 0;
  for( ii=0; ii < NoWords; ii++) {
    G4double dtemp;
    istr_line >> dtemp; 
    wordlist.push_back(dtemp);
#ifdef FILEIN_DEBUG
    G4cout << " GmFileIn::GetWordsInLineDouble word " << dtemp << " in vector " << wordlist.size() << G4endl;
#endif
  }

#ifdef FILEIN_DEBUG
  G4cout << " GmFileIn::GetWordsInLineDouble returning " << isok << G4endl;
#endif
  
  return isok;  
}


//-----------------------------------------------------------------------
void GmFileIn::ErrorInLine()
{
  G4cerr << "!! EXITING: ERROR IN LINE No " << theLineNo[theCurrentFile] << " file: " << theNames[theCurrentFile] << " : ";

}


//-----------------------------------------------------------------------
G4bool GmFileIn::eof()
{
  G4bool isok = theFiles[theCurrentFile]->eof();
  if( isok ) {
    //G4cout << " eof theCurrentFile " << theCurrentFile << G4endl;
    theCurrentFile--;
    if( theCurrentFile != -1 ) Close();  // last file will be closed by the user
  }
  //only real closing if all files are closed
  //-  G4cout << " eof " << isok << " " << theCurrentFile << G4endl;
  if( theCurrentFile != -1 ) { 
    return 0;
  } else {
    return isok;
  }
}


//-----------------------------------------------------------------------
void GmFileIn::Close()
{
  //  G4cout << " close " << theCurrentFile << " size " << theFiles.size() << G4endl;
/*  if( theCurrentFile+1 != 0 ) {
    ErrorInLine();
    G4cerr << "trying to close file while reading other files included in it " << theCurrentFile+1 << G4endl;
    //    exit(0);
    } else { 
 theFiles[theCurrentFile+1]->close();
 theFiles.pop_back();
 //  }*/
  theifstream->close();
  std::vector<GmFileIn*>::iterator vfcite;
  for( vfcite = theInstances.begin(); vfcite != theInstances.end(); vfcite++) {
    if( (*vfcite)->theifstream == theifstream) {
      break;
    } 
  } 
  if( vfcite != theInstances.end() ) {
    theInstances.erase( vfcite );
  }
}

//-----------------------------------------------------------------------
void GmFileIn::Rewind()
{
  theifstream->seekg(0, std::ios::beg);
}

//-----------------------------------------------------------------------
void GmFileIn::DumpException( const G4String& sent )
{
  G4cerr << "!!! EXITING: " << sent << " in file " << theName << " line No " << theLineNo[theCurrentFile] << G4endl;
  exit(1); 

}
