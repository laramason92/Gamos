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
#include "FileData.hh"
#include "ScorerData.hh"
#include "ScoreData.hh"
#include <set>

long int FileData::theNEventsSum = 0;

FileData::FileData()
{
}


void FileData::AddScorerData(ScorerData* sd)
{
  theScorerData.push_back(sd);

}


FileData FileData::operator+=( const FileData& fd2 )
{
  
  /*std::vector<ScorerData*>::iterator ite1, ite2;
    for( ite1 = theScorerData.begin(), ite2.GetScorerData().begin(); ite1 != theScorerData.end(); ite++ ){
    ScorerData* sd = *ite;
    
    }*/
  if( ScoreData::verbosity >= 2 ) 
    G4cout << "Score Data size before " << this << " " << theScorerData.size() << G4endl;
  
  std::set<ScorerData*> scorerData2Used;

  unsigned int siz1 = GetNumberOfScorers();
  unsigned int siz2 = fd2.GetNumberOfScorers();
  for( unsigned int ii = 0; ii < siz1; ii++ ){
    ScorerData* sd1 = theScorerData[ii];
    for( unsigned int jj = 0; jj < siz2; jj++ ){
      ScorerData* sd2 = fd2.GetScorerData(jj);
      
      if( ! (*sd1 != *sd2) ) {
	*sd1 += *sd2;
	scorerData2Used.insert(sd2);
      }    
    }
    if( ScoreData::verbosity >= 2 ) 
      G4cout << "Score Data size after " << this << " " << theScorerData.size() << G4endl;
  }    

  for( unsigned int ii = 0; ii < siz2; ii++ ){
    ScorerData* sd1 = fd2.GetScorerData(ii);
    if( scorerData2Used.find(sd1) != scorerData2Used.end() ) continue;
    
    for( unsigned int jj = 0; jj < siz1; jj++ ){
      ScorerData* sd2 = theScorerData[jj];
      if( ! (*sd1 != *sd2) ) {
	*sd1 += *sd2;
      }    
    }
    if( ScoreData::verbosity >= 2 ) 
      G4cout << "Score Data size after " << this << " " << theScorerData.size() << G4endl;
  }    

  return *this;

}

ScorerData* FileData::GetScorerData(unsigned int ii) const
{
  return theScorerData[ii];
}

void FileData::Print( std::ofstream& fout )
{
  
  fout << "  Number of events processed : " << FileData::theNEventsSum << G4endl;

  for( unsigned int ii = 0; ii < theScorerData.size(); ii++ )
    {
      theScorerData[ii]->Print(fout);
    }

}
