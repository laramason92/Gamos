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
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

#ifdef G4VIS_USE
//#include "GamosCore/GamosBase/include/GmVisManager.hh"
#include "G4VisExecutive.hh"
#endif

#include "G4GeometryManager.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosReadDICOM/include/GmSqdose.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv) 
{

  if(argc != 3 && argc != 4 ) { 
    G4Exception("sumSqdose",
		"wrong argument",
		FatalErrorInArgument,
		"YOU MUST SUPPLY TWO OR THREE ARGUMENTS: FILE_WITH_LIST_OF_SQDDOSE_FILES FILE_OUTPUT FILE_OUTPUT_TYPE");
  }

  G4String fileListName = argv[1];

  G4String fNameOut = argv[2];
  FILE* fout  = fopen(fNameOut,"wb");

  std::vector<G4String> wl;
  GmFileIn finlis = GmFileIn::GetInstance(fileListName);
  G4double nevents = 0.;

  GmSqdose* dose;
  GmSqdose* dose2;
  G4int ii;
  for( ii = 0;; ii++){
    if( ! finlis.GetWordsInLine(wl) ) break;      
    G4cout << "### MergeSqdoseUA: reading dose file " << wl[0]  << G4endl;
    if( ii == 0 ) {
      dose = new GmSqdose();
      dose->Read(wl[0]);
      nevents+= dose->GetHeader()->GetNumberOfEvents();
      if( argc == 4 ) {
	if( argv[3] == "ALL" ) {
	  dose->SetSqdoseType(SqTALL);
	} else if( argv[3] == "FILLED" ) {
	  dose->SetSqdoseType(SqTFILLED);
	} else {
	  G4Exception("sumSqdose",
		      "Wrong type of output file (third argument)",
		      FatalErrorInArgument,
		      G4String("It can only be 'ALL' or 'FILLED', it is " + G4String(argv[3])).c_str());
	}
      }
    } else {
      dose2 = new GmSqdose();
      dose2->Read(wl[0]);
      *dose += *dose2;
      nevents+= dose->GetHeader()->GetNumberOfEvents();
      G4cout << " adding doses " << nevents << G4endl;
      delete dose2;
    }
    G4cout << "$$$$$ MERGED " << ii << " files into " << fNameOut <<  " NEVENTS= "<< nevents << G4endl;
  }
  
  G4cout << "$$$$$ MERGED " << ii << " files into " << fNameOut <<  " NEVENTS= "<< nevents << G4endl;
  if( ii != 0 ) dose->Print(fout);

  delete dose;

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

