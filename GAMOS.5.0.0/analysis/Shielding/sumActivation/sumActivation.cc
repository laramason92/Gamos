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
#include "Shielding/include/SHRadioactiveChainSet.hh"
#include "Shielding/include/SHRadioactiveChain.hh"
#include "Shielding/include/ShieldingVerbosityMgr.hh"
#include <fstream>
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosApplication/include/GmRunManager.hh"
std::map<G4String,SHRadioactiveChainSet*> theIsotopeSets;
void ReadActivityResults(G4String fileName);
void PrintData(std::ofstream& out);
void PrintHelp();
void ReadPeriods( const G4String& fileName );
void ReadTimes(G4String fileName);
std::map<G4double,G4bool> thePeriods;
std::vector<G4double> theTimes;

//-------------------------------------------------------------------------------
int main(int argc,char** argv) 
{
  G4String theFileListName = "";
  G4String theFileNameOut = "";
  G4String theFileNamePeriods = "";
  G4String theFileNameTimes = "";

  if( argc == 2 ) {
     if( G4String(argv[1]) == "-help" ) {
       PrintHelp();
       return 0;
     }
  } else {
    if(argc%2 != 1) { 
      G4Exception("sumActivation",
		  "Error",
		  FatalErrorInArgument,
		  "WRONG NUMBER OF ARGUMENTS: THEY MUST BE -XX1 VAL_XX1 -XX2 VAL_XX2 ... ");
     }
    for( G4int ii = 1; ii < argc; ii+=2 ){
      G4String argvstr = argv[ii];
      if( argvstr == "-f" ) {
	theFileListName = argv[ii+1];
      } else if( argvstr == "-fOut" ) {
	theFileNameOut = argv[ii+1];
      } else if( argvstr == "-fPeriods" ) {
	theFileNamePeriods = argv[ii+1];
      } else if( argvstr == "-fTimes" ) {
	theFileNameTimes = argv[ii+1];
      } else if( argvstr == "-verb" ) {
	G4String verbstr = "ShieldingVerbosity " + G4String(argv[ii+1]);
	GmRunManager* runManager = new GmRunManager();
	runManager->SelectVerbosity(verbstr);
      } else {
	PrintHelp();
	G4Exception("sumActivation",
		    "Wrong argument",
		    FatalErrorInArgument,
		    "WRONG ARGUMENTS: THEY MUST BE -XX1 VAL_XX1 -XX2 VAL_XX2 ... ");
      }
    }
  }

  if( theFileNamePeriods != "" ) {
    ReadPeriods( theFileNamePeriods );
  }

  if( theFileNameTimes != "" ) {
    ReadTimes( theFileNameTimes );
  }

  std::vector<G4String> wl;
  GmFileIn finlis = GmFileIn::GetInstance(theFileListName);
  
  // Read each file
  int ii;
  for( ii = 0;; ii++) {
    if( ! finlis.GetWordsInLine(wl) ) break;      
    
    if( wl.size() != 1 ) {
      G4Exception("sumActivation",
		  "Wrong atgument",
		  FatalErrorInArgument,
		  G4String("YOUR FILE_LIST HAVE TOO MANY WORDS IN LINE NUMBER: " + GmGenUtils::itoa(ii+1)).c_str());
    }

    G4cout << "@@@@@@@ Reading FILE: " << wl[0]  << G4endl;
    ReadActivityResults( wl[0] );

  }

  std::ofstream fout(theFileNameOut.c_str());
  PrintData(fout);
  
  return 0;
}


void ReadActivityResults(G4String fileName)
{
  std::vector <G4String> wl;

  GmFileIn fin = GmFileIn::GetInstance(fileName); 
  fin.SetCheckQuotes( false );
  //%%%%%% SHActivationUA_inCameraF:  NUMBER OF EVENTS PER SECOND = 2.28311e+10 = 1e+07 events in 438000 nanoseconds 
  //%%% SHActivationUA_inCameraF: END_PRODUCTION: 3.1536e+08 sec  TIME: 2.592e+06 sec : Nb93[30.8] : ACTIVITY= 490.959 decays/sec   LIFETIME=7.34332e+08 sec   NPART_IN_JOB= 88   NIsotopes= 5.20131e+11 
  SHRadioactiveChainSet* isotSet = 0;
  std::map<G4String,SHRadioactiveChainSet*>::const_iterator ites;
  for(;;){
    if( !fin.GetWordsInLine(wl) ) break;
    //    std::cout << " LINE " << wl.size() << " " << wl[0] << std::endl;
    if( wl.size() > 1 && wl[0].find("%%") != G4String::npos && wl[1].find("SHActivationUA") != G4String::npos ) {
      ites =  theIsotopeSets.find(wl[1]);
      if( ites !=  theIsotopeSets.end() ) {
	isotSet = (*ites).second;
       	G4cout << " OLD isotSet " << isotSet << " " << wl[1] << G4endl;
      } else {
	isotSet = new SHRadioactiveChainSet( wl[1] );
	theIsotopeSets[wl[1]] = isotSet;
	// one new set if there are SHActivationUA with different filters
       	G4cout << " NEW isotSet " << isotSet << " " << wl[1] << G4endl;
      } 
      G4cout << "SETS " << theIsotopeSets.size() << G4endl;
      isotSet->AddData(wl);

    }
  }
  
}
 

//------------------------------------------------------------------
void ReadPeriods( const G4String& fileName )
{

  GmFileIn fin = GmFileIn::GetInstance(fileName);
  std::vector<G4String> wl;
  G4double periodSum = 0.;
  for(;;){
    if( ! fin.GetWordsInLine(wl)  ) break;
    if( wl.size() != 2 ) {
      G4Exception("SHActivation::ReadPeriods",
		  "Error",
		  FatalException,
		  (G4String("Problem reading file, there should be two words per line, there are ") + GmGenUtils::itoa(wl.size())).c_str());
    }
    periodSum += GmGenUtils::GetValue(wl[0]);
    thePeriods[ periodSum ] = GmGenUtils::GetBool(wl[1]); 
  }
}

//-------------------------------------------------------------------------------
void ReadTimes(G4String fileName)
{
  std::vector <G4String> wl;

  GmFileIn fin = GmFileIn::GetInstance(fileName); 
  for(;;){
    if( !fin.GetWordsInLine(wl) ) break;
    for( unsigned int ii = 0; ii < wl.size(); ii++) {
      theTimes.push_back( GmGenUtils::GetValue(wl[ii]) );
    }
  }
}

//-------------------------------------------------------------------------------
void PrintData(std::ofstream& out)
{
  std::map<G4String,SHRadioactiveChainSet*>::const_iterator ite;
  G4String printedIsotNames = "";

  for( ite = theIsotopeSets.begin(); ite != theIsotopeSets.end(); ite++) {
    SHRadioactiveChainSet* isotSet = (*ite).second;
    if( thePeriods.size() != 0 ) isotSet->SetPeriods(thePeriods);
    if( theTimes.size() != 0 ) isotSet->SetTimes(theTimes);
    isotSet->CheckLifeTimes();
    isotSet->PrintData(out);
  }
}

void PrintHelp()
{
  G4cout << " -f       activation file list name " << G4endl
	 << " -fOut    output file name " << G4endl
	 << " -fPeriods file name of periods file " << G4endl
	 << " -fTimes   file name of times file " << G4endl
	 << " -verb    verbosity: it sets the ShieldingVerbosity" << G4endl
	 << " -help    prints the set of arguments " << G4endl;
}
