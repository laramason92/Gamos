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
#include "ScorerData.hh"
#include "ScoreData.hh"
#include <set>

std::map<G4String,G4double> ScorerData::theNEventsSum;

ScorerData::ScorerData(std::vector<G4String> wl)
{
  theDetectorName = wl[1];
  theNumberOfFiles = 1;
}


void ScorerData::AddScoreData(ScoreData* ed, G4bool bSetScorer )
{
	
  theScoreData.push_back(ed);
  ed->SetScorerData( this );
  if( bSetScorer) ed->MultiplyByNEvents();
  if( ScoreData::verbosity >= 2 ) G4cout << " ScorerData::AddScoreData " << theScorerName << " " << theScoreData.size() << G4endl;

}

void ScorerData::SetNEvents(G4double evts) 
{
  theNEventsSum[theScorerName] += evts;
  theNEvents = evts;
}


ScorerData ScorerData::operator+=( const ScorerData& sd2 )
{
  
  /*std::vector<ScorerData*>::iterator ite1, ite2;
    for( ite1 = theScorerData.begin(), ite2.GetScorerData().begin(); ite1 != theScorerData.end(); ite++ ){
    ScorerData* sd = *ite;
    }*/
  
  if( ScoreData::verbosity >= 1 )
    G4cout << this << " SCORER TO ADD " << sd2.GetScorerName() << " N= " << sd2.GetNScores() << " TO " << theScorerName << " N= " << theNScores << G4endl;

  /*  std::set<ScoreData*> scoreData2Used;
  for( unsigned int ii = 0; ii < theNScores+1; ii++ ){
    ScoreData* id1 = theScoreData[ii];

    for( unsigned int jj = 0; jj < sd2.GetNScores()+1; jj++ ){
      ScoreData* id2 = sd2.GetScoreData(jj);
      //-      G4cout << theScorerName << "CHECK DATA " << id1->GetName() << " " << id2->GetScorerName() << G4endl; 
      if( (*id1 == *id2) ) {
	*id1 += *id2;
	if( ScoreData::verbosity >= 2 )
	  G4cout << theScorerName << "ADDING DATA " << id1->GetName() << " " << id2->GetName() << G4endl; 
	scoreData2Used.insert(id2);
      }
    }
  }

  for( unsigned int ii = 0; ii < sd2.GetNScores()+1; ii++ ){
    ScoreData* id2 = sd2.GetScoreData(ii);
    if( scoreData2Used.find(id2) != scoreData2Used.end() ) continue;
    if( ScoreData::verbosity >= 0 )
      G4cout << theScorerName << " DATA NOT USED ADDED " << id2->GetName() << G4endl; 
    AddScoreData( id2, false );
    theNScores += 1;
  }
  */

  // loop to score data of 2 and check if same score data is found in 1: if yes, add data, if not create new data
  for( unsigned int ii2 = 0; ii2 < sd2.GetNScores()+1; ii2++ ){
    ScoreData* id2 = sd2.GetScoreData(ii2);
    bool bFound1 = false;
    ScoreData* id1 = 0;
    for( unsigned int ii1 = 0; ii1 < theNScores+1; ii1++ ){
      id1 = theScoreData[ii1];
      if( *id1 == *id2 ) {
	bFound1 = true;
	break;
      }
    }
    if( bFound1 ) {
      *id1 += *id2;
      if( ScoreData::verbosity >= 2 ) G4cout << theScorerName << " ADDING DATA " << id2->GetName() << " to " << id1->GetName() << G4endl; 
    } else {
      AddScoreData( id2, false );
      theNScores += 1;
      if( ScoreData::verbosity >= 2 ) G4cout << theScorerName << " CREATING NEW DATA " << id2->GetName() << G4endl; 
    }

  }
      
  theNumberOfFiles++;

  /*  for( unsigned int ii = 0; ii < theScoreData.size(); ii++ ) {
    G4cout << "ADDED NOW " << theScoreData[ii]->GetName()  << " " << theScoreData[ii]->GetScoreTot() << " EV SUM " << theNEventsSum[theScorerName] << G4endl;
    } */
 
  return *this;
  
}

bool ScorerData::operator!=( const ScorerData& sd2 )
{

  if (theDetectorName != sd2.GetDetectorName() 
      || theScorerName != sd2.GetScorerName() ) {
    return true;
  }

  return false;

}

ScoreData* ScorerData::GetScoreData(unsigned int ii) const
{
  if( ii >= theScoreData.size() ) {
    G4Exception("ScorerData::GetScoreData",
		"Asking for an score that does not exists",
		FatalException,
		"You are trying to merge files with different scores");
  }
  return theScoreData[ii];
}


void ScorerData::Print( std::ofstream& fout )
{
  fout << " MultiFunctionalDet: " << theDetectorName << G4endl;
  fout << " PrimitiveScorer: " << theScorerName << G4endl;
  fout << " Number of entries= " << theNScores << G4endl;
  
  for( unsigned int ii = 0; ii < theScoreData.size(); ii++ ) {
    theScoreData[ii]->Print(fout);
  }

}





