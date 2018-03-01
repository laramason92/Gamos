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
#ifndef __DetHistosGammaDist__
#define __DetHistosGammaDist__

#include "GamosCore/GamosAnalysis/include/GmTrajPointPosMom.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"

class G4Event;
class GmAnalysisMgr;

class DetHistosGammaDist : public GmUserEventAction,  public GmUserTrackingAction, public GmUserSteppingAction
{
public:
  DetHistosGammaDist();
  ~DetHistosGammaDist(){};
  virtual void EndOfEventAction( const G4Event* evt );
  virtual void PreUserTrackingAction(const G4Track* aTrack);
  virtual void UserSteppingAction(const G4Step* aStep);
  
private:
  G4bool IsInterestingStep(const G4Step* aStep);
  void FillHistos();  
  G4ThreeVector ExtrapolateToRing( GmTrajPointPosMom& inter );

private:
  GmAnalysisMgr* theAnaMgr;

  std::map<int,GmTrajPointPosMom> theInteractions;
  G4ThreeVector theEventVertex;
  G4double theRingRadius;
  G4int theNgamma;

  G4int isOriginalGamma;

};

#endif
