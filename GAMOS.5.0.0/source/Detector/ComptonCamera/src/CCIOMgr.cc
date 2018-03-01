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
#include "CCIOMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include <iomanip>
#include <iostream>

//----------------------------------------------------------------------
CCIOMgr* CCIOMgr::theInstance = 0;

//----------------------------------------------------------------------
CCIOMgr* CCIOMgr::GetInstance()
{
  if( !theInstance ) {
    theInstance = new CCIOMgr;
  }

  return theInstance;

}

//-----------------------------------------------------------------------
CCIOMgr::CCIOMgr() 
{
  theOutputType = "CompCam";

  bBinFile = G4bool(GmParameterMgr::GetInstance()->GetNumericValue(theOutputType+":BinFile",1));

  if( bBinFile ) {
   theCCIOMgr = new GmIObinMgr;
  } else {
   theCCIOMgr = new GmIOtextMgr;
  }

  Init();

  G4String fnout = theOutputType+":DumpToCout";
  G4int dumpCoutOld = GmParameterMgr::GetInstance()->GetNumericValue(fnout,-99);
  if( dumpCoutOld != -99 ) {
   G4Exception("CCIOMgr::CCIOMgr",
	"Old parameter format, please use the new one: delete 'Output:'",
	JustWarning,
	G4String("Parameter used: " + fnout).c_str());
  } else {
    dumpCoutOld = 0;
  }
  
  fnout = theOutputType+":DumpToCout";
  bDumpCout = G4bool(GmParameterMgr::GetInstance()->GetNumericValue(fnout,G4bool(dumpCoutOld)));
  
  fnout = theOutputType+":DumpCartesian";
  G4int cartesianOld = GmParameterMgr::GetInstance()->GetNumericValue(fnout,0);
  if( cartesianOld != 0 ) {
   G4Exception("CCIOMgr::CCIOMgr",
	"Old parameter format, please use the new one: delete 'Output:'",
	JustWarning,
	G4String("Parameter used: " + fnout).c_str());
  } else {
    cartesianOld = 0;
  }
  fnout = theOutputType+":DumpCartesian";
  bCartesian = G4bool(GmParameterMgr::GetInstance()->GetNumericValue(fnout,G4bool(cartesianOld)));
}

//-----------------------------------------------------------------------
void CCIOMgr::WriteEvent( const G4ThreeVector& pos1, const G4double& energy1, const G4ThreeVector& pos2,const G4double& energy2, const G4String& name )
{
  if( theFileOut == 0 ) {
    G4Exception("CCIOMgr::WriteEvent",
		"Error",
		FatalException,
		"File not opened, call OpenFileOut() first ");
  }

  CCOutput po;
  strcpy(po.name,name);
  po.x1 = pos1.x();
  po.y1 = pos1.y();
  po.z1 = pos1.z();
  po.e1 = energy1;
  po.x2 = pos2.x();
  po.y2 = pos2.y();
  po.z2 = pos2.z();
  po.e2 = energy2;
 
  fwrite (&po, sizeof(struct CCOutput),1,theFileOut);

  if(bDumpCout) PrintEvent(" CCOutput: ", po, bCartesian);

}

//-----------------------------------------------------------------------
CCIOMgr::~CCIOMgr()
{
}

//-----------------------------------------------------------------------
void CCIOMgr::ReadFile()
{
  if( !theFileIn ) OpenFileIn();

  CCOutput po;
  G4bool bEof;
  for(;;) {
    po = ReadEvent( bEof );
    if(bDumpCout) PrintEvent(" CCOutput: ", po, bCartesian);
    if( bEof ) break;
  }

}

//-----------------------------------------------------------------------
CCOutput CCIOMgr::ReadEvent( G4bool& bEof )
{
  if( theFileIn == 0 ){
    G4Exception("CCIOMgr::ReadEvent",
		"Error",
		FatalException,
		"File not opened, call OpenFileIn() first ");
  }

  CCOutput po;
  fread (&po, sizeof(struct CCOutput),1,theFileIn);
  if ( feof( theFileIn ) ) {
    bEof = TRUE;
  } else {
    bEof = FALSE;
  }

  return po; 

}

//-----------------------------------------------------------------------
void CCIOMgr::PrintEvent(const G4String& text, const CCOutput& po, G4bool bCart )
{
  G4cout<< text << po.name;
  G4ThreeVector p1(po.x1, po.y1, po.z1 );
  G4ThreeVector p2(po.x2, po.y2, po.z2 );

  if( bCart ) {
    PrintCart(" p1",p1);
    G4cout<<" en1 "<<po.e1;
    PrintCart(" p2",p2);
    G4cout<<" en2 "<<po.e2;

  } else {
    PrintCyl(" p1",p1);
    G4cout<<" en1 "<<po.e1;
    PrintCyl(" p2",p2);
    G4cout<<" en2 "<<po.e2;
  }
  G4cout << G4endl;
}
