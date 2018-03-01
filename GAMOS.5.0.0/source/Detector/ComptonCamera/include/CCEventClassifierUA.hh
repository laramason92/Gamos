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
#ifndef CCEventClassifierUA_H
#define CCEventClassifierUA_H

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "GamosCore/GamosSD/include/GmRecHit.hh"
#include "Detector/Base/include/DetRecHitSet.hh"

class GmAnalysisMgr;
class G4Event;
class GmHitsEventMgr;
class DetCountScatteringUA;
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include <map>
class GmTrajStep;
class DetV1stHitAlgorithm;
class DetComptonStudyHistosUA;

class CCEventClassifierUA : public GmUserEventAction, public GmUserRunAction
{

public:
  CCEventClassifierUA();
  virtual ~CCEventClassifierUA();
  void BeginOfRunAction(const G4Run* aRun );
  virtual void EndOfEventAction(const G4Event* );
  virtual void EndOfRunAction(const G4Run*);
 
  G4int Classify( gamosRecHitList& rhitList, const G4Event* evt  );
  G4int ClassifyCC( gamosRecHitList& rhitList, const G4Event* evt );

  void GetFullComptonRecHits( const gamosRecHitList& rhitList);

  G4bool CheckDifferentOriginalGammaAndEvent( DetRecHitSetList& rhitsets );
  G4bool CheckRecHitDifferentOriginalGammaAndEvent( GmRecHit* rhit, G4int& origGamma );
  G4bool CheckOriginalGammaScattered( DetRecHitSetList& rhitsets );
  G4bool CheckRecHitOriginalGammaScattered( GmRecHit* rhit );

private:
  void PrintCCClassificationTable( G4int nevt );

  void BookHistos();
  void DeleteHitsInRecHits();

  void CheckTimeCoincidence();

private:
  G4int theEventClass;

  DetRecHitSetList theFullRecHits;
  G4double theGammaEnergy;
  G4double thePhotopeakEPrecMin;
  G4double thePhotopeakEPrecMax;
  G4double theCoincidenceTime;
   
  GmAnalysisMgr* theAnaMgr;

  GmHitsEventMgr* theHitsEventMgr;

  DetCountScatteringUA* theCountScattering;

  G4bool bDump;
  G4bool bDumpSingles;
  G4bool bDumpMultiples;
  G4double theComptonRecHitsDist;
G4double theComptonRecHitsDistScat;
G4double theComptonRecHitsDistAbs;
  G4int isOriginalGamma;

  DetV1stHitAlgorithm* the1stHitAlgo1;
  DetV1stHitAlgorithm* the1stHitAlgo2;

  G4bool bComptonStudyHistos;
  DetComptonStudyHistosUA* theComptonStudyHistos;

  G4int theSelectPosOrder;
  G4double sumEnergy;
  G4double totalEnergy;
  G4double scatEnergy;
  G4double absEnergy;
  G4double scatHits;
  G4double absHits;
  G4ThreeVector scatPosition;
  G4ThreeVector absPosition;
  G4String type; 
  G4double singles;
  G4double absorbed;
  G4double coincident;
};

#endif
