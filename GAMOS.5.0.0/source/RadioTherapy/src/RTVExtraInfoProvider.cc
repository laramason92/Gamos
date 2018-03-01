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
#include "RTExtraInfoProviderZLast.hh"
#include "RTExtraInfoMgr.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserRunActionList.hh"
#include "G4RunManager.hh"

//----------------------------------------------------------------
RTVExtraInfoProvider::RTVExtraInfoProvider()
{ 
  //-----  Extra info providers should be executed before RTPhaseSpaceUA
  GmUserRunActionList* runActionList = (GmUserRunActionList*)(G4RunManager::GetRunManager()->GetUserRunAction());
  G4int nPHSP = runActionList->CountActions("RTPhaseSpaceUA");

  if( nPHSP != 0 ) {
    G4Exception("RTVExtraInfoProvider::RTVExtraInfoProvider ",
		"Creating an extra info after when RTPhaseSpaceUA, it should be done before",
		FatalException,
		"change order of '/gamos/userAction' commands");
  }

}


#include <fstream>
//---------------------------------------------------------------------
void RTVExtraInfoProvider::BeginOfRunAction( const G4Run* )
{
  InitVariables();
  std::ofstream fout;
  if( theOrder == 0 ) {
    fout.open(theFileName);
  } else {
    fout.open(theFileName,std::ios_base::app);
  }
  DumpIndices(fout);
  DumpIndices(G4cout);
  fout.close();

}
