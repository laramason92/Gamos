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
#include "DetCAlgoYZPos.hh"
#include "DetVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosAnalysis/include/GmTrajStep.hh"
#include "GamosCore/GamosSD/include/GmRecHit.hh"

//----------------------------------------------------------------------
DetCAlgoYZPos::DetCAlgoYZPos()
{
  theName = "YZPos";
  InitVariables();
}


//----------------------------------------------------------------------
void DetCAlgoYZPos::FillHistosInteractions( std::pair<mmits::iterator,mmits::iterator> itemmp, G4int nh )
{
  std::vector<G4double> values;
  mmits::iterator itemm;
  for( itemm = itemmp.first; itemm != itemmp.second; itemm++ ){
    G4ThreeVector pos = (*itemm).second->GetPositionPost();
    values.push_back( sqrt(sqr(pos.y())+sqr(pos.z())) );
  }

  FillHistosFromValues( values, nh+0 );

}


//----------------------------------------------------------------------
void DetCAlgoYZPos::FillHistosRHits( const std::vector<std::pair<GmTrajStep*,GmRecHit*> >& recHitOrdered, G4int nh )
{
  std::vector<G4double> values;
  
  std::vector<std::pair<GmTrajStep*,GmRecHit*> >::const_iterator iterho;
  for( iterho = recHitOrdered.begin(); iterho != recHitOrdered.end(); iterho++ ){
    //      GmTrajStep* ts = (*iterho).first;
    GmRecHit* rh = (*iterho).second;
    G4ThreeVector pos = rh->GetPosition();
    values.push_back( sqrt(sqr(pos.y())+sqr(pos.z())) );
  }
  
  FillHistosFromValues( values, nh+100 );
}

