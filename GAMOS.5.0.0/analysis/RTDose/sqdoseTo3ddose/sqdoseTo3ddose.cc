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
#include "GamosCore/GamosReadDICOM/include/Gm3ddose.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv) 
{

  if(argc!=3) { 
    G4Exception("sqdoseTo3ddose",
		"wrong argument",
		FatalErrorInArgument,
		"YOU MUST SUPPLY TWO ARGUMENTS: SQDDOSE_FILE_INPUT 3DDOSE_FILE_OUTPUT");
  }

  G4String fileListName = argv[1];

  G4String fNameOut = argv[2];
  std::ofstream* fout =  new std::ofstream(argv[2]);

  GmSqdose* sqdose;
  G4cout << "### sqdoseTo3ddose: reading sqdose file " << argv[1]  << G4endl;
  sqdose = new GmSqdose();
  sqdose->Read(argv[1]);

  Gm3ddose* dose3d = new Gm3ddose(*sqdose);

  G4cout << "### sqdoseTo3ddose: writing 3ddose file " << argv[2]  << G4endl;
  *fout << *dose3d;

  delete sqdose;
  delete dose3d;
  fout->close();

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

