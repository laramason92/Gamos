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
#ifndef RTReuseAtZPlaneProcess_h 
#define RTReuseAtZPlaneProcess_h 1

#include "globals.hh"
#include "G4ios.hh"
#include "G4VDiscreteProcess.hh"
#include "G4ParticleChange.hh"
enum MWR {NOMir, MirX, MirY, MirXY};

class RTReuseAtZPlaneProcess : public G4VDiscreteProcess
{
  public: // with description
  //  constructor requires the process name and type
  RTReuseAtZPlaneProcess( const G4String& aName =  "RTReuseAtZPlane",
                 G4ProcessType   aType = fUserDefined );

  public: 
  //  destructor 
  virtual ~RTReuseAtZPlaneProcess(){};

  public:

  virtual G4double PostStepGetPhysicalInteractionLength(
                             const G4Track& track,
                             G4double   previousStepSize,
                             G4ForceCondition* condition
                            );
  
  virtual G4VParticleChange* PostStepDoIt(
                             const G4Track& track,
                             const G4Step&  stepData
                            );

  virtual G4double GetMeanFreePath(const G4Track& aTrack,
                             G4double   previousStepSize,
                             G4ForceCondition* condition
                                                               );

private:
  G4double theZPlane;
  G4int theNReuse;
  MWR MirrorWhenReuse;
};

#endif
