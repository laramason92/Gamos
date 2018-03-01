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
#include "ScoreData.hh"
#include "ScorerData.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

int ScoreData::verbosity = 0;

ScoreData::ScoreData(std::vector<G4String> wl, G4bool bTot) :
  bTotal(bTot)
{
  theScore = atof(wl[3]);
  theScoreTot = atof(wl[3]); // score * nEvents
  if( !bTotal ) {
    theName = wl[1];
    theScoreError = atof(wl[5])*theScore;
  }else {
    theName = "SUM ALL";
    theScoreError = atof(wl[5]);
  }
  if( ScoreData::verbosity >= 2 )
    G4cout << " ScoreData " << this << " " << theName << G4endl;
 
  // get dispersion of flux values in different files
  theScoreSq = theScore*theScore;

  if( theScoreError != 0 ) {  
    theScoreDivideError = theScore/sqr(theScoreError);
    theInvErrorExp2 = 1/sqr(theScoreError);
  } else {
    theScoreDivideError = 0;
    theInvErrorExp2 = 0;
  }

  //  if( theName.find( "neutron_7" )!= std::string::npos)
  if( ScoreData::verbosity >= 2 )
    G4cout << " ScoreData " << theName << " " << theScore <<  " " << theScoreTot << " +- " << theScoreError << " theScoreDivideError " << theScoreDivideError << " theInvErrorExp2 " << theInvErrorExp2 << G4endl;

  theNumberOfFiles = 1;

} 

void ScoreData::SetScorerData( ScorerData* sd )
{
  theScorerData = sd;
}

void ScoreData::MultiplyByNEvents()
{
  // for summing each score is multiplied by the number of events
  double nev = theScorerData->GetNEvents();
  theNEvents = G4int(nev);
  //  G4cout << " SetScorerDataPRE " << theName << " TOT " << theScoreTot << " nev " << nev << G4endl;
  theScoreTot *= nev; 
  if( ScoreData::verbosity >= 2 )
    G4cout << " SetScorerData " << theName << " TOT " << theScoreTot << G4endl;
  // only used for dispersion  theScoreSq *= nev*nev; 
}

bool ScoreData::operator!=( const ScoreData& sd2 )
{
  return !(*this == sd2);

}
bool ScoreData::operator==( const ScoreData& sd2 )
{
  //     G4cout << " CHECKSEPAR " << theName << " " << sd2.GetName() << G4endl;
 // compare name also checking that 1.E-05-0.0001 is equal than 1.E-05-1.E-04
  if (theName == sd2.GetName() ) {
    /*    G4cout << " FATAL error, the Score in Scorer don't have the same format  " 
	   << " First Score " << theName
	   << " Second Score " << sd2.GetName() 
	   << G4endl; */
    return true;
  } else {
    // try to decompose: look for a '-' separating min and max
    int iSepar1 = -1;
    for( size_t ii = 1; ii < theName.length(); ii++ ) {
      if( theName[ii] == '-' ) {
	if( theName[ii-1] != 'E' && theName[ii-1] != 'e' ) {
	  iSepar1 = ii;
	  break;
	}
      }
    }
    int iSepar2 = -1;
    G4String theName2 = sd2.GetName();
    for( size_t ii = 1; ii < theName2.length(); ii++ ) {
      if( theName2[ii] == '-' ) {
	if( theName2[ii-1] != 'E' && theName2[ii-1] != 'e' ) {
	  iSepar2 = ii;
	  break;
	}
      }
    }
    //    G4cout << " SEPAR " << theName << " " << theName2 << " " << iSepar1 << " " << iSepar2 <<  G4endl;
    if( iSepar1 != -1 && iSepar2 != -1 ) {
      G4String name1Sub1 = theName.substr(0,iSepar1);
      if( !GmGenUtils::IsNumber(name1Sub1) ) return false;
      G4String name2Sub1 = theName2.substr(0,iSepar2);
      if( !GmGenUtils::IsNumber(name2Sub1) ) return false;
      G4String name1Sub2 = theName.substr(iSepar1+1,theName.length());
      if( !GmGenUtils::IsNumber(name2Sub1) ) return false;
      G4String name2Sub2 = theName2.substr(iSepar2+1,theName.length());
      if( !GmGenUtils::IsNumber(name2Sub2) ) return false;
      if( GmGenUtils::GetValue(name1Sub1) == GmGenUtils::GetValue(name2Sub1) && 
	  GmGenUtils::GetValue(name1Sub2) == GmGenUtils::GetValue(name2Sub2) ) {
	return true;
      }

    }

  }
  return false;

}


ScoreData ScoreData::operator+=( const ScoreData& ed2 )
{
  //  if( theEnergy == 0.01)  G4cout << " 1theInvErrorExp2 " << this << " " << theInvErrorExp2 << G4endl;
  
  theScoreTot += ed2.GetScoreTot();
  theScore += ed2.GetScore();
  theNumberOfFiles++;

  theScoreSq += ed2.GetScoreSq();
  
  if ( ed2.GetScoreError() != 0. ) {    
    theScoreDivideError += (ed2.GetScore())/sqr(ed2.GetScoreError());
    theInvErrorExp2 += 1/sqr(ed2.GetScoreError());
    if( ScoreData::verbosity >= 2 )
      G4cout << bTotal << " Add ScoreData " << theName << " WITH " << ed2.GetName() << " " << theScore << " scoreSq " << theScoreSq << " theScoreDivideError " << theScoreDivideError << " theInvErrorExp2 " << theInvErrorExp2 << G4endl;
  }  
  
  if( ScoreData::verbosity >= 2 )
    G4cout << " ScoreDataAdd " << theName << " " << theScore << " +- " << theScoreError << " theScoreSq " << theScoreSq << " theScoreDivideError " << theScoreDivideError << " theInvErrorExp2 " << theInvErrorExp2 << G4endl;

  return *this;
}


long double ScoreData::GetScoreTot() const
{
  return theScoreTot;
}

long double ScoreData::GetScore() const
{
  return theScore;
}

long double ScoreData::GetScoreSq() const
{
  return theScoreSq;
}

long double ScoreData::GetScoreError() const
{
  return theScoreError;
}

void ScoreData::Print( std::ofstream& fout )
{
  long double scoreAver = 0.;
  if( theInvErrorExp2 != 0 ) scoreAver = theScoreDivideError/theInvErrorExp2;
  long double scoreError;
  if( scoreAver != 0. ) {
    scoreError = 1./sqrt(theInvErrorExp2);
  } else {
    scoreError = 0.;
  }
  //  G4int nFiles =theNumberOfFiles; 
  G4int nFiles = theScorerData->GetNumberOfFiles();
  long double scoreDisp = sqrt((theScoreSq/nFiles - sqr(theScore/nFiles)) / ((nFiles-1)));

  if( ScoreData::verbosity >= 2 ) 
    G4cout <<  theName << " scoreDisp " << scoreDisp << " N " << nFiles << " score " << theScore << " scoreSq " << theScoreSq << " theScoreSq/nFiles - sqr(theScore/nFiles) " << theScoreSq/nFiles - sqr(theScore/nFiles) << " " << theScoreTot << G4endl;

  if( !bTotal ) {
    fout << " index: ";
  } else {
    fout << theScorerData->GetScorerName() << " ";
  }
  double averScorePerEvent = theScoreTot/ScorerData::GetNEventsSum(theScorerData->GetScorerName()); 
  double errorRel = 0;
  if( averScorePerEvent != 0. ) errorRel = scoreDisp/averScorePerEvent;
  fout << theName << "  = " 
       << averScorePerEvent
    //       << " " << theScoreTot
    //  << " / " << ScorerData::GetNEventsSum(theScorerData->GetScorerName()) 
       << " +-(REL) " 
       << errorRel
       << " Average " << theScore/nFiles;
    //       << "  " << scoreDisp << " N " << nFiles << " score " << theScore << " scoreSq " << theScoreSq << "  theScore*theScore/nFiles " << theScore*theScore/nFiles ;
 
  //  fout << " non_weighted_average " << theScore/ScorerData::GetNEventsSum(theScorerData->GetScorerName())  << " score " << theScore << " NEV " << ScorerData::GetNEventsSum(theScorerData->GetScorerName()) 
  double errorRel2 = 0.;
  if( scoreAver != 0 ) errorRel2 = scoreError/scoreAver;
  fout << " weighted_average " 
	 << scoreAver
	 << " +-(REL) "
	 << errorRel2;
  
  //  fout << " SCORE_dispersion(REL) " << scoreDisp/scoreAver;
  fout << G4endl;
}
