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
#include "DetVComptAlgorithmVariable.hh"
#include "DetVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------------
DetVComptAlgorithmVariable::DetVComptAlgorithmVariable()
{
  theValueMin = 0.;
  theValueMax = 1.;
  theGroupingNumber = -1;

}

//----------------------------------------------------------------------
void DetVComptAlgorithmVariable::InitVariables()
{
  theValueMin = 0.;
  theValueMax = 1.;

  theGroupingNumber = G4int(GmParameterMgr::GetInstance()->GetNumericValue("DetCAlgo"+theName+":HistoGroupingNumber",3));
  
}


//----------------------------------------------------------------------
void DetVComptAlgorithmVariable::BookHistos(G4int histoNumber)
{
  theAnaMgr = GmAnalysisMgr::GetInstance("compton") ;

  std::vector<std::string> orig(2);
  orig[0] = "Interaction: ";
  orig[1] = "RecHit: ";
 
  G4String hgnam = "DetCompton:Algo: "; 
  G4String hgnam2, hnam;
  
  for( G4int ii = 0; ii <= 1; ii++ ){
    hgnam2 = hgnam + orig[ii] + theName + " ";
    hnam = hgnam2;
    theAnaMgr->CreateHisto1D(hnam,100,theValueMin,theValueMax,histoNumber+ii*100+60);
    for( G4int jj = 1; jj <= theGroupingNumber; jj++ ){
      if( jj == 1 ) {
	hnam = hgnam2 + ":1st";
      } else if( jj == 2 ) {
	hnam = hgnam2 + ":2nd";
      } else if( jj == 3 ) {
	hnam = hgnam2 + ":3rd";
      } else {
	hnam = hgnam2 + ":" + GmGenUtils::itoa(jj) + "th";
      }
      if( jj == theGroupingNumber ) {
	hnam += "+";
      }

      theAnaMgr->CreateHisto1D(hnam,100,theValueMin,theValueMax,histoNumber+ii*100+60+jj);
      theAnaMgr->CreateHisto1D(hnam + " - others",100,-theValueMax,theValueMax,histoNumber+ii*100+70+jj);
      theAnaMgr->CreateHisto2D(hnam + " vs others",100,theValueMin,theValueMax,100,theValueMin,theValueMax,histoNumber+ii*100+80+jj);
    }

  }
  
}


//----------------------------------------------------------------------
void DetVComptAlgorithmVariable::FillHistosFromValues(std::vector<G4double>& vars, G4int nh)
{
  std::vector<G4double>::iterator ite;
  for( unsigned int ii = 0; ii < vars.size(); ii++ ){
    G4double var = vars[ii];
    theAnaMgr->GetHisto1(nh+60)->Fill(var);
    G4int iih = ii+1;
    if( G4int(ii) >= theGroupingNumber ) iih = theGroupingNumber;
    theAnaMgr->GetHisto1(nh+60+iih)->Fill(var);
    for( unsigned int jj = 0; jj < vars.size(); jj++ ){
      if( ii == jj ) continue;
      theAnaMgr->GetHisto1(nh+70+iih)->Fill(var-vars[jj]);
      theAnaMgr->GetHisto2(nh+80+iih)->Fill(var,vars[jj]);
    }

  }

}



