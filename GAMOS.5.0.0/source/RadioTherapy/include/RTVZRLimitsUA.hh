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
#ifndef __RTVZRLimitsUA__
#define __RTVZRLimitsUA__

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"

#include <map>
class G4Trajectory;
typedef std::map<G4int,G4Trajectory*> maptraj;
class G4LogicalVolume;

class RTVZRLimitsUA : public GmUserRunAction, public GmUserEventAction, public GmUserTrackingAction, public GmUserSteppingAction
{
public:
  struct XYdata{
    G4double xmin;
    G4double xmax;
    G4double ymin;
    G4double ymax;
  };
  
public:
  RTVZRLimitsUA();
  ~RTVZRLimitsUA(){}; 
  virtual void BeginOfRunAction( const G4Run* aRun );
  virtual void UserSteppingAction(const G4Step* aStep);
  virtual void EndOfRunAction( const G4Run* aRun );
 
protected:
  void GetWorld();
  virtual void BuildXYLimits() = 0;

protected:
  G4double theWorldX, theWorldY, theWorldZ;
  G4LogicalVolume* theWorldLogical;
  std::map<G4double,G4String> theXYvolName;
  std::map<G4double,XYdata> theXYlimits;
  std::map<G4double,G4int> theXYcounts;

  G4bool bRussRoul;
  G4double theRussRoulValue;

};

std::ostream& operator<<(std::ostream&, const RTVZRLimitsUA::XYdata&);

#endif
