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
#ifndef PETEventClassifierUA_H
#define PETEventClassifierUA_H

#include "Detector/Base/include/DetRecHitSet.hh"
#include "G4ThreeVector.hh"
#include "GamosCore/GamosSD/include/GmRecHit.hh"
#include "GamosCore/GamosUtils/include/GmLine.hh"
#include "GamosCore/GamosUtils/include/GmCone.hh"

class GmAnalysisMgr;
class G4Event;
class GmHitsEventMgr;
class GmCone;
class DetCountScatteringUA;
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include <map>
class DetV1stHitAlgorithm;
class DetComptonStudyHistosUA;

class PETEventClassifierUA : public GmUserEventAction,
			     public GmUserRunAction
{

public:
  PETEventClassifierUA();
  virtual ~PETEventClassifierUA();
  void BeginOfRunAction(const G4Run* aRun );
  virtual void EndOfEventAction(const G4Event* );
  virtual void EndOfRunAction(const G4Run*);
 
  G4int Classify( gamosRecHitList& rhitList, const G4Event* evt  );
  G4int ClassifyPET( gamosRecHitList& rhitList, const G4Event* evt );

  void Get511ComptonRecHits( const gamosRecHitList& rhitList );
  G4double GetPETLineDistanceToVtx( const GmLine& line, const G4ThreeVector& eventVtx );
  G4ThreeVector GetEventVertex( const G4Event* evt );

  G4bool CheckDifferentOriginalGammaAndEvent( DetRecHitSetList& rhitsets );
  G4bool CheckRecHitDifferentOriginalGammaAndEvent( GmRecHit* rhit, G4int& origGamma );
  G4bool CheckOriginalGammaScattered( DetRecHitSetList& rhitsets );
  G4bool CheckRecHitOriginalGammaScattered( GmRecHit* rhit );

private:
  void PrintPETClassificationTable( G4int nevt );

  void BookHistos();
  void DeleteHitsInRecHits();

  void FillDCAHistos( G4double linedist );

  void CheckTimeCoincidence();

  private:
  G4int theEventClass;

  DetRecHitSetList the511RecHits;
  G4double the511EPrecMin;
  G4double the511EPrecMax;
  G4double thePETLineDistToVtx;
  G4double theCoincidenceTime;

  GmAnalysisMgr* theAnaMgr;

  GmHitsEventMgr* theHitsEventMgr;

  DetCountScatteringUA* theCountScattering;

  GmLine thePetLine;

  G4bool bDumpLM;
  G4bool bDumpPD;

  G4double theComptonRecHitsDist;

  G4int isOriginalGamma;

  DetV1stHitAlgorithm* the1stHitAlgo1;
  DetV1stHitAlgorithm* the1stHitAlgo2;

  G4bool bComptonStudyHistos;
  DetComptonStudyHistosUA* theComptonStudyHistos;

};

#endif
