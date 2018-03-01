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
#ifndef SHActivationUA_h
#define SHActivationUA_h 1

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"

#include <map>
class G4ParticleDefinition;
#include "SHMetastableIsotMgr.hh"
class SHRadDecayChainSet;

class SHActivationUA : public GmUserRunAction, 
		       public GmUserEventAction,
		       public GmUserTrackingAction
{
  
public:
  SHActivationUA();
  ~SHActivationUA();

public:
  virtual void BeginOfRunAction( const G4Run* );
  virtual void BeginOfEventAction( const G4Event* );
  virtual void EndOfRunAction( const G4Run* );
  virtual void PreUserTrackingAction(const G4Track* );

protected:
  void InitialiseRun();  
  void SetPeriods( const std::vector<G4String>& periods );
  void ReadPeriods(const G4String& fileName);
  void PrintActivities();

protected:
  G4double theMaximumTime;
  G4double theMinimumTime;
  std::map<G4String,G4double> theParticles;
  std::vector<G4double> theTimes;
  //  std::vector<ppdd> theParticlesInEvent;
  std::map<G4ParticleDefinition*,G4double> theParticlesInEvent;
  SHMetastableIsotMgr* theMetastableIsotMgr;
  G4double theTotalActivity;
  std::map<G4double,G4bool> thePeriods;

  SHRadDecayChainSet* theRadDecayChainSet;
};

#endif
