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
#ifndef ScorerData__HH
#define ScorerData__HH
#include "globals.hh"
#include <vector>
#include <map>
#include <fstream>
class ScoreData;

class ScorerData 
{
public:
  ScorerData(std::vector<G4String> wl);
  ~ScorerData(){};

public:

  void AddScoreData(ScoreData*, G4bool bSetScorer = true);
  ScorerData operator+=( const ScorerData& sd2 );
  bool operator!=( const ScorerData& sd2 );
  ScoreData* GetScoreData(unsigned int) const;
  void Print( std::ofstream& fout );
  G4double GetNEvents() const{
    return theNEvents; 
  }

  void SetNEvents(G4double evts);

  static G4double GetNEventsSum(G4String scorerName){
    return theNEventsSum[scorerName]; 
  }

  G4double GetNScores() const {
    return theNScores;
  }
  G4String GetDetectorName() const {
    return theDetectorName;
  }
  G4String GetScorerName() const {
    return theScorerName;
  }
  void SetScorerName(G4String name) {
    theScorerName = name;
  }

  void SetNScores(G4double evts) {
    theNScores = evts;
  }

  G4int GetNumberOfFiles() { 
    return theNumberOfFiles;
  }
  
private:
  std::vector<ScoreData*> theScoreData;
  G4String theDetectorName;
  G4String theScorerName;
  G4double theNEvents;
  static std::map<G4String,G4double> theNEventsSum;
  G4double theNScores;
  G4int theNumberOfFiles;
};

#endif
