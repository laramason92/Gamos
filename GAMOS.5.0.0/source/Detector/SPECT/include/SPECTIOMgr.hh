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
#ifndef SPECTIOMgr_h
#define SPECTIOMgr_h

/*---------------------------------------------------------------------------   
ClassName:   SPECTIOMgr    
Author:      P. Arce
Changes:     08/07: creation  
---------------------------------------------------------------------------*/ 
// Description  
//----------------------------------------------- 

#include <iostream>
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "GamosCore/GamosBase/Base/include/GmIObinMgr.hh"

struct SPECTOutput
{
  char name[8];
  float xVtx,yVtx,zVtx,x1,y1,z1,x2,y2,z2;
  float energy;
  int eventClass;
};


//------------------------------------------------------------------------
class SPECTIOMgr : public GmIObinMgr
{ 
 private:
  SPECTIOMgr();

 public:
  ~SPECTIOMgr();

  //! Get the only instance 
  static SPECTIOMgr* GetInstance();
  void WriteEvent( const G4ThreeVector& pos1, const G4ThreeVector& pos2, const G4ThreeVector& posVtx, const G4double energy, G4int evtClass, const G4String& name = "SPECT " );  
  void ReadFile();
  SPECTOutput ReadEvent( G4bool& bEof );
  void PrintEvent(const G4String& text, const SPECTOutput& po, G4bool bCart );

private:
  static SPECTIOMgr* theInstance;
  G4bool bDumpCout;
  G4bool bCartesian;
};

#endif
