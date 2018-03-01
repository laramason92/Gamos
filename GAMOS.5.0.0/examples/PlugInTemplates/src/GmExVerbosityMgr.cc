#include "../include/GmExVerbosityMgr.hh"

GmVerbosity GmExVerb;
#include "globals.hh"

GmExVerbosityMgr::GmExVerbosityMgr()
{
  //  G4cout << GmExVerb(infoVerb) << " GmExVerbosityMgr " << GmExVerb.GetFilterLevel() << G4endl;
}


void GmExVerbosityMgr::SetFilterLevel( int fl )
{
  //  GmExVerb.SetFilterLevel( fl );
}

int GmExVerbosityMgr::GetFilterLevel() const
{
  // return GmExVerb.GetFilterLevel();
}
