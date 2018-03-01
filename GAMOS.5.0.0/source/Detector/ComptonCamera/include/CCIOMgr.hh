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
#ifndef CCIOMgr_h
#define CCIOMgr_h

/*---------------------------------------------------------------------------   
ClassName:   CCIOMgr    
Author:      P. Arce
Changes:     08/07: creation 
	     02/11: Edited by L. Harkness for Compton output
---------------------------------------------------------------------------*/ 
// Description  
//----------------------------------------------- 

#include <iostream>
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "GamosCore/GamosBase/Base/include/GmIObinMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmIOtextMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmIOMgr.hh"

struct CCOutput
{
char name[9];
float x1,y1,z1,e1,x2,y2,z2,e2;
};

//------------------------------------------------------------------------
class CCIOMgr: public GmIObinMgr
{ 
 private:
  CCIOMgr();

public:
  ~CCIOMgr();
 static CCIOMgr* GetInstance();
void WriteEvent( const G4ThreeVector& pos1, const G4double& energy1, const G4ThreeVector& pos2, const G4double& energy2, const G4String& name = "COMPCAM " );  
  void ReadFile();
  CCOutput ReadEvent( G4bool& bEof );
  void PrintEvent(const G4String& text, const CCOutput& po, G4bool bCart );

 
private:

GmIOMgr* theCCIOMgr;
static CCIOMgr* theInstance;
G4bool bBinFile; 
G4bool bDumpCout;
G4bool bCartesian;

};

#endif
