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
#include "SHActivationFromFluxUA.hh"
#include "ShieldingVerbosity.hh"
#include "SHMetastableIsotMgr.hh"
#include "SHRadDecayChainSet.hh"
#include "SHRadDecayScorerFromFlux.hh"

#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmNumberOfEvent.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "G4Track.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"

//-----------------------------------------------------------------
SHActivationFromFluxUA::SHActivationFromFluxUA()
{
  theName = "SHActivationFromFluxUA";

  theFluxScorer = new SHRadDecayScorerFromFlux();

  theMaximumTime = -1;
  theMinimumTime = DBL_MAX;

}


//-----------------------------------------------------------------
SHActivationFromFluxUA::~SHActivationFromFluxUA() 
{
}

//------------------------------------------------------------------
void SHActivationFromFluxUA::BeginOfRunAction( const G4Run* )
{
  InitialiseRun();

}

//-----------------------------------------------------------------
void SHActivationFromFluxUA::UserSteppingAction(const G4Step* aStep)
{
  theFluxScorer->ProcessHits(aStep);
}

//-----------------------------------------------------------------
void SHActivationFromFluxUA::PreUserTrackingAction(const G4Track*)
{
}

//-----------------------------------------------------------------
void SHActivationFromFluxUA::BeginOfEventAction( const G4Event* )
{
}
 
//-----------------------------------------------------------------
void SHActivationFromFluxUA::EndOfRunAction(const G4Run* )
{
  // set number of ions from flux
  G4THitsMap<G4double>* RunMap = theFluxScorer->GetRunMap();

  std::map<G4int,G4double*>::iterator ite;
  G4double nev = GmNumberOfEvent::GetNumberOfEvent();

  for(ite = RunMap->GetMap()->begin(); ite != RunMap->GetMap()->end(); ite++){
    G4double sumX = (*(ite->second));
    G4double aveX;
    //    G4cout << " SUMX " << sumX << " AVEX " << aveX << " NEV " << nev << G4endl;
    aveX = sumX/nev;

    G4cout << "  index: " << (ite->first) << "  = " << aveX;
    theRadDecayChainSet->StoreIon( theFluxScorer->GetIon(ite->first), sumX );   
//t    G4double error = theFluxScorer->GetErrorRelative( ite->first, sumX, nev );

  }

  PrintActivities();

}



