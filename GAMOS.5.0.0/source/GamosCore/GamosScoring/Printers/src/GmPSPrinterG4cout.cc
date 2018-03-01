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
#include "GmPSPrinterG4cout.hh"
#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"

#include "G4PhantomParameterisation.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmNumberOfEvent.hh"
#include "G4UnitsTable.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"


GmPSPrinterG4cout::GmPSPrinterG4cout(G4String name) : GmVPSPrinter( name )
{
  theUnit = -1.;
  theUnitName = G4String("");

  bPrintSumV2 = G4bool( GmParameterMgr::GetInstance()->GetNumericValue(theName+":PrintSumV2",1));

}

void GmPSPrinterG4cout::SetParameters( const std::vector<G4String>& params )
{
  if( params.size() != 2 && params.size() != 0 ){ //if 0 parameters, scorer unit/unitname is taken
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmPSPrinterG4cout::SetParameters",
		"There should be two parameters: UNIT UNIT_NAME",
		FatalErrorInArgument,
		G4String("They are: "+parastr).c_str());
  }

  if( params.size() == 2 ) {
    theUnit = GmGenUtils::GetValue( params[0] );
    theUnitName = params[1];
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) )
      G4cout << this << " GmPSPrinterG4cout::SetParameters " << theUnit << " " << theUnitName << G4endl;
#endif
  }
}


void GmPSPrinterG4cout::DumpAll( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* theScorer )
{
  if( theUnit == -1 ) {
    theUnit = theScorer->GetUnit();
    theUnitName = theScorer->GetUnitName();
  }

  //  G4cout <<" GmPSPrinterG4cout::DumpAll() " << G4endl;
  G4cout << G4endl << " MultiFunctionalDet: " << theScorer->GetMultiFunctionalDetector()->GetName() << G4endl;
  G4cout << " PrimitiveScorer: " << theScorer->GetName() << G4endl;
  G4cout << " Number of entries= " << RunMap->entries() << G4endl;
  std::map<G4int,G4double*>::iterator ite;
  G4double nev;
  //??  G4bool bScoreByEvent = theScorer->ScoreByEvent();

  if( bScoreByEvent ) {
    nev = GmNumberOfEvent::GetNumberOfEvent();
  } else {
    nev = 1;
  }

  G4double aveALL = 0.;
  G4double errorALL = 0.;
  GmVClassifier* classifier = theScorer->GetClassifier();
  for(ite = RunMap->GetMap()->begin(); ite != RunMap->GetMap()->end(); ite++){

    G4double sumX = (*(ite->second));
    G4double aveX;
    //	   << "  dose deposit= " << G4BestUnit(*(ite->second),"Dose")
    //    G4cout << " SUMX " << sumX << " AVEX " << aveX << " NEV " << nev << G4endl;
    if( bScoreByEvent ){
      aveX = sumX/nev/theUnit;
    } else {
      aveX = sumX/theUnit;
    }
    
    G4cout << "  index: " << classifier->GetIndexName(ite->first) << "  = " << aveX;
    aveALL += aveX;
    if( theScorer->ScoreErrors() ) {
      G4double error = theScorer->GetErrorRelative( ite->first, sumX, nev );
      //   if( aveX != 0. ) error /= (aveX/theUnit); // relative error
      G4cout << " +-(REL) " << error << " " << theUnitName;
      errorALL += (error*aveX)*(error*aveX);
      if( bPrintSumV2 ) {
	G4cout << " sumV2= " << theScorer->GetSumV2(ite->first)/(theUnit*theUnit);
      }
      G4cout << G4endl;
    } else {
      G4cout << " " << theUnitName << G4endl;
    }

    //    G4cout << "  copy no.:EV " << bScoreByEvent << " = " << sumX << " nev " << nev << " unit " << theUnit << G4endl;
  }
  //  G4cout << this << " GmPSPrinterG4cout::DumpAll " << theUnit << " " << theUnitName << G4endl;

  errorALL = std::sqrt(errorALL);
  if( aveALL != 0. ) errorALL /= aveALL;
  G4cout << theScorer->GetName() << " SUM ALL: " << aveALL << " +-(REL) " << errorALL << " " << theUnitName;

  G4cout << G4endl;
}
