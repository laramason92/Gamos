#include "G4Run.hh"
#include "G4Event.hh"

#include "../include/GmExUserAction.hh"
#include "GamosCore/GamosBase/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/include/GmAnalysisMgr.hh"

#include "GamosCore/GamosSD/include/GmHitsEventMgr.hh"


//-----------------------------------------------------------------------
GmExUserAction::GmExUserAction() : GmUserRunAction(), GmUserEventAction(), GmUserTrackingAction(), GmUserSteppingAction()
{
}


//-----------------------------------------------------------------------
void GmExUserAction::BeginOfRunAction( const G4Run* )
{
  /*
  G4int hisMax = GmParameterMgr::GetInstance()->GetNumericValue("Example:HisMax",100.);
  theAnaMgr = GmAnalysisMgr::GetInstance("example");
  theAnaMgr->CreateHisto1D("example",100,0,hisMax,99999);
  */
} 

//-----------------------------------------------------------------------
void GmExUserAction::BeginOfEventAction( const G4Event* )
{
}

//-----------------------------------------------------------------------
void GmExUserAction::PreUserTrackingAction(const G4Track* )
{
}

//-----------------------------------------------------------------------
void GmExUserAction::UserSteppingAction(const G4Step* aStep)
{
}


//-----------------------------------------------------------------------
void GmExUserAction::PostUserTrackingAction(const G4Track* )
{
}

//-----------------------------------------------------------------------
void GmExUserAction::EndOfEventAction( const G4Event* evt )
{
  /*
  gamosSDHitMap hits = GmHitsEventMgr::GetInstance()->GetHits();

  gamosSDHitMap::const_iterator itehmp;
  for( itehmp = hits.begin(); itehmp != hits.end(); itehmp++ ){
    GmHitList* hitlist = (*itehmp).second;
    GmHitList::iterator iteh;
    //--- Loop to hits
    for( iteh = hitlist->begin(); iteh != hitlist->end(); iteh++ ){
      GmHit* hit = (*iteh);
      theAnaMgr->GetHisto1(99999)->Fill(hit->GetEnergy());
    }
  }
  */
}

//-----------------------------------------------------------------------
void GmExUserAction::EndOfRunAction( const G4Run* )
{
}

