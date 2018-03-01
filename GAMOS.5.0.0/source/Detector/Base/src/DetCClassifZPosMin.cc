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
#include "DetCClassifZPosMin.hh"
#include "DetVerbosity.hh"
#include "GamosCore/GamosSD/include/GmRecHit.hh"
#include "GamosCore/GamosSD/include/GmHit.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"

//----------------------------------------------------------------------
DetCClassifZPosMin::DetCClassifZPosMin()
{
  theName = "ZPosMin";
  InitVariables();

}

//----------------------------------------------------------------------
DetCClassifZPosMin::~DetCClassifZPosMin()
{
}
//----------------------------------------------------------------------
void DetCClassifZPosMin::BookSpecificHistos(G4int histoNumber)
{

  G4String hnam = "DetCompton:Classif: RecHit " + theName;
  theAnaMgr->CreateHisto1D(hnam,300,theMin,theMax,histoNumber+80);
  hnam =  "DetCompton:Classif: RecHit ZPos";
  theAnaMgr->CreateHisto1D(hnam,300,theMin,theMax,histoNumber+81);
  
}


//----------------------------------------------------------------------
G4int DetCClassifZPosMin::GetClassification( DetRecHitSet& prhs, G4int nh )
{
  DetRecHitSet::iterator iterhs;
  std::set<GmRecHit*>::iterator iterh;

  // Get minimum energy
  G4double minPos = DBL_MAX;
  for( iterhs = prhs.begin(); iterhs != prhs.end(); iterhs++ ){
    G4double val = fabs((*iterhs)->GetPosition().z());
    if( val< minPos ) minPos = val;
    if( nh != -1 ) {
      theAnaMgr->GetHisto1(nh + 81 )->Fill(val);
    }
  }
  if( nh != -1 ) {
    theAnaMgr->GetHisto1(nh + 80 )->Fill(minPos);
  }

  return GetIndexFromValue( minPos );
}
