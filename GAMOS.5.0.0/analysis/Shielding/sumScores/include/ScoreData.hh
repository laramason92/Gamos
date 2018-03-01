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
#ifndef ScoreData__HH
#define ScoreData__HH
#include "globals.hh"
#include <vector>
class ScorerData;

class ScoreData 
{
public:
  ScoreData(std::vector<G4String> wl, G4bool bTotal);
  ~ScoreData(){};

  ScoreData operator+=( const ScoreData& id2 );
  bool operator!=( const ScoreData& id2 );
  bool operator==( const ScoreData& id2 );

  int GetNEvents() const {
    return theNEvents;	
  }
  long double GetScoreTot() const;
  long double GetScore() const;
  long double GetScoreSq() const;
  long double GetScoreError() const;
  
  void Print( std::ofstream& fout );
  void SetScorerData( ScorerData* sd );
  void MultiplyByNEvents();
  G4String GetName() const {
    return theName;
  }

  static int verbosity;

private:
  
  long double theScoreDivideError;
  long double theInvErrorExp2;
  G4int theNumberOfFiles;
  long double theScoreTot; // score* nevents processed
  long double theScore;
  long double theScoreSq;
  long double theScoreError;
  int theNEvents;
  G4String theName;

  G4bool bTotal;
  ScorerData* theScorerData;
};

#endif
