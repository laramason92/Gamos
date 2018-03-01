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

#ifndef PETProjDataMgr_h
#define PETProjDataMgr_h

/*---------------------------------------------------------------------------  
ClassName:   PETProjDataMgr
Author:      M. Canadas, P. Arce
Changes:     01/11: creation 

-------------------------------------------------------------------------
// Description
	 
    PET output (List-mode X Y Z) into projection data (sinograms). 

    Output data: Sinograms for PET image reconstruction. Interfile format, STIR compatible (.hs/.s), 
                 STIR Software for Tomographic Image Reconstruction: http://stir.sourceforge.net/main.htm     

-----------------------------------------------------------------------*/

#include <iostream>
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "GamosCore/GamosBase/Base/include/GmIObinMgr.hh"
//#include <math.h>
#include <map>
#include <vector>
#include <set>
#include <string>

using namespace std;

 typedef unsigned short SINO_TYPE;  //!!NOTE:  Check "Write_sino3D" (.hv file) if SINO_TYPE changes !!

//------------------------------------------------------------------------
class PETProjDataMgr : public GmIObinMgr
{ 
 private:
  PETProjDataMgr();

 public:
  ~PETProjDataMgr();

  //! Get the only instance 
  static PETProjDataMgr* GetInstance();
  void AddEvent(const G4ThreeVector& pos1, const G4ThreeVector& pos2); 
  //void ReadFile();
  //PETOutput ReadEvent( G4bool& bEof );
  void WriteInterfile();

private:
  static PETProjDataMgr* theInstance;
  
  G4double theDistAxial,theD_FOV, theOffsetX;
  G4int theN_planes,theN_bin,theN_ang,theMRD, theOutFormat, theTOTaxial_pl;
  G4String theFileOUTname;

  //G4bool bDumpCout;
  
  SINO_TYPE ***theSino3D;
  SINO_TYPE *theSino_line;
  
  unsigned long int theN_CoinciTot;
  unsigned long int theN_CoinciVal;
  unsigned long int theMaxCoinci;  
  
  FILE *theOUTfile;
};

#endif
