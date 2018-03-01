#include "../include/GmExPSPrinter.hh"
#include "GamosCore/GamosScoring/include/GmVPrimitiveScorer.hh"

GmExPSPrinter::GmExPSPrinter(G4String name) : GmVPSPrinter( name )
{
  /*  theUnit = -1.;
      theUnitName = G4String(""); */
}

void GmExPSPrinter::SetParameters( const std::vector<G4String>& params )
{
  /*  if( params.size() != 2 && params.size() != 0 ){ //if 0 parameters, scorer unit/unitname is taken
    G4String parastr;
    for( size_t ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmExPSPrinter::SetParameters","There should be two parameters: UNIT UNIT_NAME",FatalErrorInArgument,G4String("They are: "+parastr).c_str());
  }

  if( params.size() == 2 ) {
    theUnit = GetVal( params[0] );
    theUnitName = params[1];
    G4cout << this << " GmExPSPrinter::SetParameters " << theUnit << " " << theUnitName << G4endl;
  }
  */
}


void GmExPSPrinter::DumpAll( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* theScorer )
{
  /*  if( theUnit == -1 ){
    theUnit = theScorer->GetUnit();
  }
  if( theUnitName == "" ){
    theUnitName = theScorer->GetUnitName();
  }

  //  G4cout <<" GmExPSPrinter::DumpAll() " << G4endl;
  G4cout << " MultiFunctionalDet: " << theScorer->GetMultiFunctionalDetector()->GetName() << G4endl;
  G4cout << " PrimitiveScorer: " << theScorer->GetName() << G4endl;
  G4cout << " Number of entries= " << RunMap->entries() << G4endl;
  std::map<G4int,G4double*>::iterator ite;
  G4int nev;
  if( bScoreByEvent ) {
    nev = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEvent();
  }

  for(ite = RunMap->GetMap()->begin(); ite != RunMap->GetMap()->end(); ite++){

    G4double sumX = (*(ite->second));
    G4cout << "  copy no.: " << ite->first;
    //	   << "  dose deposit= " << G4BestUnit(*(ite->second),"Dose")
    if( bScoreByEvent ){
      G4cout << "  = " << sumX/nev*theUnit;
    } else {
      G4cout << "  = " << sumX*theUnit;
    } 

    if( theScorer->ScoreErrors() ) {
      G4double error = theScorer->GetError( ite->first, sumX, nev );
      if( sumX != 0. ) error /= sumX;
      
      G4cout << " +-(REL) " << error << " " << theUnitName << G4endl;
    } else {
      G4cout << " " << theUnitName << G4endl;
    }
  }
  G4cout << this << " GmExPSPrinter::DumpAll " << theUnit << " " << theUnitName << G4endl;
  */

}

