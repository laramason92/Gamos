#ifndef GmExUserAction__HH
#define GmExUserAction__HH

#include "GamosCore/GamosBase/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"

class GmExUserAction : public GmUserRunAction, public GmUserEventAction, public GmUserTrackingAction, public GmUserSteppingAction
{
public:
  GmExUserAction();
  ~GmExUserAction(){};

  virtual void BeginOfRunAction( const G4Run* );
  virtual void EndOfRunAction( const G4Run* );
  virtual void BeginOfEventAction( const G4Event* );
  virtual void EndOfEventAction( const G4Event* );
  virtual void PreUserTrackingAction(const G4Track* aTrack);
  virtual void PostUserTrackingAction(const G4Track* aTrack);
  virtual void UserSteppingAction(const G4Step* aStep);

private:
  GmAnalysisMgr* theAnaMgr;
  
};

#endif
