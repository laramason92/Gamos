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
#include "ASHistoMgr.hh"
#include "Detector/SPECT/include/SPECTIOMgr.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"

//-----------------------------------------------------------------------
int main(int argc,char** argv) 
{
  G4String theFileName = "";
  G4String theHistoFileName = "spectout";
  G4String hisPosMin = "-100.";
  G4String hisPosMax = "-100";
  G4String hisEnerMin = "0.";
  G4String hisEnerMax = "1.";
  G4String theFileNameOut = "analyseSPECT";
  G4int nRead = -1; 

  if( argc == 2 ) {
    if( G4String(argv[1]) == "-help" ) {
      G4cout << "  -f    output file name (in this case do not use the file name alone as first argument as before)" << G4endl
	     << "  -NRead   number of particles to be read from the output file" << G4endl
	     << "  -fHistos    name of file with list of histograms " << G4endl
	     << "  -PosMin    minimum limit of the position histograms" << G4endl
	     << "  -PosMax    maximum limit of the position histograms" << G4endl
	     << "  -EnerMin    minimum limit of the energy histogram" << G4endl
	     << "  -EnerMax    maximum limit of the energy histogram" << G4endl
	     << "  -verb    verbosity: it sets the RTVerbosity. Default is warning, that will print the above lines; debug, that will print each particle read form the phase space file" << G4endl
	     << "  -help    prints the set of arguments" << G4endl;
      return 0;
    } else {
      theFileName = argv[1];
    }

  } else {
    if(argc%2 != 1) { 
      G4Exception("analyseSPECT",
		  "Wrong argument",
		  FatalErrorInArgument,
		  "WRONG NUMBER OF ARGUMENTS: THEY MUST BE -XX1 VAL_XX1 -XX2 VAL_XX2 ... ");
    }
    for( G4int ii = 1; ii < argc; ii+=2 ){
      G4String argvstr = argv[ii];
      if( argvstr == "-f" ) {
	theFileName = argv[ii+1];
      } else if( argvstr == "-fHistos" ) {
	theHistoFileName = argv[ii+1];

      } else if( argvstr == "-PosMin" ) {
	hisPosMin = argv[ii+1];
  
      } else if( argvstr == "-PosMax" ) {
	hisPosMax = argv[ii+1];

      } else if( argvstr == "-EnerMin" ) {
	hisEnerMin = argv[ii+1];

      } else if( argvstr == "-EnerMax" ) {
	hisEnerMax = argv[ii+1];

      } else if( argvstr == "-NRead" ) {
	nRead = GmGenUtils::GetInt(argv[ii+1]);

      } else if( argvstr == "-verb" ) {
	//t	G4String verbstr = "AnaSPECTVerbosity " + G4String(argv[ii+1]);
	//t	runManager->SelectVerbosity(verbstr);

      } else {
	G4Exception("analyseSPECT",
		    "Wrong argument",
		    FatalErrorInArgument,
		    G4String("Use -help for a list of available arguments, the used one is not permitted: "+argvstr).c_str());
      }
    }
  }

  SPECTIOMgr::GetInstance()->OpenFileIn(theFileName);
  G4bool bEof;
  G4int ii = 0;
  ASHistoMgr* histoMgr = new ASHistoMgr(theHistoFileName);
  for( ;; ) {
    ii++;
    SPECTOutput sout = SPECTIOMgr::GetInstance()->ReadEvent( bEof );
    histoMgr->FillHistos( sout );
    if( nRead != -1 && ii >= nRead ) {
      break;
    }
    if(bEof) break;
  }

  G4cout << "NUMBER OF EVENTS READ = " << ii << G4endl;
 
  GmAnalysisMgr::DeleteInstances();

}
