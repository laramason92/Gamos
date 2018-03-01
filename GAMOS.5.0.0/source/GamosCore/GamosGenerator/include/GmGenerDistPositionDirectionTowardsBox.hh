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
#ifndef GmGenerDistPositionDirectionTowardsBox_HH
#define GmGenerDistPositionDirectionTowardsBox_HH

#include "GamosCore/GamosGenerator/include/GmVGenerDistPosition.hh"
#include "GamosCore/GamosGenerator/include/GmVGenerDistDirection.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include <vector>
class GmTouchable;

struct TouchInfo{
  G4String name;
  G4double minX;
  G4double maxX;
  G4double minY;
  G4double maxY;
  G4ThreeVector dir;
  G4double stereoAngle;
  G4RotationMatrix rotmPlane;
};

class GmParticleSource;

class GmGenerDistPositionDirectionTowardsBox : public GmVGenerDistPosition, public GmVGenerDistDirection
{
public:
  GmGenerDistPositionDirectionTowardsBox();
  virtual ~GmGenerDistPositionDirectionTowardsBox(){};

  virtual G4ThreeVector GeneratePosition( const GmParticleSource* source );
  virtual G4ThreeVector GenerateDirection( const GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );

private:
  void SetDirCosineLimits( const GmTouchable* touch );

private:
  G4ThreeVector thePoint;
  std::vector< TouchInfo* > theTouchInfos;
  G4double theTotalStereoAngle;
};

#endif
