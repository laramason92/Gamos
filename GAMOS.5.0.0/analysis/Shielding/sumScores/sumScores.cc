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
#include "FileData.hh"
#include "ScorerData.hh"
#include "ScoreData.hh"
#include <fstream>
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

int main(int argc,char** argv) 
{

  if(argc != 3 ) { 
    G4Exception("sumScores",
		"Wrong argument",
		FatalErrorInArgument,
		"YOU MUST SUPPLY TWO ARGUMENT: FILE_LIST   OUTPUT_FLIE");
  }

//Confirmamos la lista de archivos a sumar y el nombre del output//

  G4String fileListName = argv[1];
  
  if( ScoreData::verbosity >= 0 )
  G4cout << "FILE lIST SIZE  " << fileListName.size() << G4endl;
  
  G4String fNameOut = argv[2];
  std::ofstream fout(fNameOut);
  
  std::vector<G4String> wl;
  std::vector<G4String> w2;
  GmFileIn finlis = GmFileIn::GetInstance(fileListName);
  G4double nevents = 0.;
  //G4double theNEvents ;
  
  std::vector<FileData*> theFileData;
  
// Vamos a leer  cada archivo// 
  
  G4int ii;
  for( ii = 0;; ii++) {
    if( ! finlis.GetWordsInLine(wl) ) break;      
    
    if( wl.size() != 1 ) {
      
    G4Exception("sumScores",
		"Wrong argument",
		FatalErrorInArgument,
		G4String("YOUR FILE_LIST HAVE TOO MANY WORDS IN LINE NUMBER: " + GmGenUtils::itoa(ii+1)).c_str());
    }
    GmFileIn Infile = GmFileIn::GetInstance(wl[0]);
    Infile.SetSuppressQuotes(0);
    
    if( ScoreData::verbosity >= 0 )
    G4cout << "@@@@@@@ Reading FILE: " << wl[0]  << G4endl;
    
    FileData* fileData = new FileData();
    theFileData.push_back( fileData ); 
    
    ScorerData* scorerData = 0;   	
    
    G4int jj;   
    nevents = -1;
    G4bool bScorerFound = false;
    for( jj = 0;; jj++) {
      if( ! Infile.GetWordsInLine(wl) ) break;
      
      //----- Read and save the name of each  in the file
      
      if ( wl.size() > 2 && wl[0]=="Number" &&  wl[1]=="of"  &&  wl[2]=="events")   {
	nevents = GmGenUtils::GetValue (wl[5]);
	FileData::theNEventsSum += (long int)(nevents);
	
	if( ScoreData::verbosity >= 0 )
	G4cout << " NEvents " << nevents << G4endl; 
      }
      
      if(wl.size() > 4 && wl[0]=="Run" &&  wl[1]=="Aborted"  &&  wl[4]=="events") {
	nevents = GmGenUtils::GetValue (wl[3]);
	FileData::theNEventsSum += (long int)(nevents);

	if( ScoreData::verbosity >= 0 )
	  G4cout << " NEvents " << nevents << G4endl; 
      }
     
      if ( wl[0]=="MultiFunctionalDet:" )  {
	//	      G4cout << " Size wl " << wl.size() << G4endl;  
	bScorerFound = true;
	scorerData = new ScorerData(wl);
	fileData->AddScorerData( scorerData );

      } else if ( wl[0]=="PrimitiveScorer:" ) {
	scorerData->SetScorerName( wl[1] );
	scorerData->SetNEvents( G4int(nevents) ); // total sum is calculated for each type of scorer name
      } else if (wl.size() == 4 && 
		 wl[0]=="Number" && 
		 wl[2] == "entries=") {
	scorerData->SetNScores( GmGenUtils::GetValue(wl[3]) );
	
	//---- Read and save score
      }  else if (wl.size() >= 6 && wl[0]=="index:") {
	ScoreData* scoreData = new ScoreData(wl, false);
	
	scorerData->AddScoreData( scoreData);
	
      } else if (wl.size() >= 6 && 
		 wl[1]=="SUM" && wl[2] == "ALL:") {
	
	ScoreData* scoreData = new ScoreData(wl, true);
	
	scorerData->AddScoreData( scoreData);
		
      }
    }
    
    Infile.Close();
    if( nevents == -1 && bScorerFound ) {
    G4Exception("sumScores",
		"Wrong argument",
		FatalErrorInArgument,
		"File does not contain number of events processed, it cannot be used");
    }
  }
  
  
  FileData* fdAVE = new FileData();
  
  std::vector<FileData*>::const_iterator ite;
  for( ite = theFileData.begin(); ite != theFileData.end(); ite++ ) {
    FileData* fd = *ite;
    if( ite == theFileData.begin() ) {
      *fdAVE = *fd;
    } else {
      *fdAVE += *fd;
    }
  }
  
  fdAVE->Print(fout);
  
  return 0;
}


