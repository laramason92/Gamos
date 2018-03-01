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
//
// Class Description
//
// This class checks if the track direction is pointing inside a Z plane

#ifndef GmPSZPlaneDirChecker_h 
#define GmPSZPlaneDirChecker_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"
class G4Track;

class GmPSZPlaneDirChecker 
{
  public: // with description
  GmPSZPlaneDirChecker();

  public: 
  //  destructor 
  virtual ~GmPSZPlaneDirChecker(){};

  public:
  virtual G4bool IsInPlane( const G4ThreeVector dir, const G4ThreeVector pos );
  virtual G4bool IsInPlane( const G4Track* track );

private:
  G4double theWeight;
  G4double thePlaneX, thePlaneY, thePlaneZ;
  G4int iUseFOI;
};

#endif
