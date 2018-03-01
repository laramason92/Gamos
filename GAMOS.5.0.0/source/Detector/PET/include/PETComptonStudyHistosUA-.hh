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
#ifndef DetComptonStudyHistosUA-_H
#define DetComptonStudyHistosUA-_H

#include "globals.hh"
#include "G4ThreeVector.hh"
class GmAnalysisMgr;
class G4Event;
class G4Track;
class G4Step;
#include "GamosCore/GamosAnalysis/include/GmVHistoBuilder.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"

#include "DetRecHitSet.hh"
#include <map>
class GmTrajStep;
typedef std::multimap<GmTrajStep*,GmRecHit*> mmiterh;                     

class DetComptonStudyHistosUA- : public GmVHistoBuilder,
				public GmUserEventAction,
				public GmUserTrackingAction,
				public GmUserSteppingAction
{

public:
  DetComptonStudyHistosUA-();
  virtual ~DetComptonStudyHistosUA-();
  virtual void BeginOfEventAction(const G4Event* );
  virtual void PreUserTrackingAction(const G4Track* aTrack);
  virtual void PostUserTrackingAction(const G4Track* aTrack);
  virtual void UserSteppingAction(const G4Step* aStep);
  void BookHistos();
  void FillHistos(G4int nh, std::vector<G4double>& vars);
  //  void PlotDist511RecHits();
  void FillDist511RecHits( DetRecHitSetList& rh511 );
 
private:
  void BookHistosForVariable(G4String var, G4double hlimMin, G4double hlimMax, G4int nh);
  void BookOtherHistos();
  G4int GetTrackID( GmRecHit* rhit );
  void AnalyseRecHitInteractionAssociation( mmiterh recHitOrderedTemp, DetRecHitSet rhitset );

private:
  G4int isOriginalGamma;
  typedef std::multimap<G4int,GmTrajStep*> mmits;
  mmits theInteractions;
};

#endif

