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
#include"MagneticDipole.hh"
#include"G4UIcmdWith3VectorAndUnit.hh"

////////////////////////////////////////////////////////////////////////////////
//
MagneticDipole::MagneticDipole(const G4ThreeVector& aDipoleMoment, const G4ThreeVector& aPos)
{theDipoleMoment = aDipoleMoment;
 theDipoleCenter = aPos;
} 
////////////////////////////////////////////////////////////////////////////////
//
MagneticDipole::~MagneticDipole()
{;
}
////////////////////////////////////////////////////////////////////////////////
//
void MagneticDipole::GetFieldValue(const G4double yTrack[4],G4double *MagField) const
{ 
  //Formulae taken from Classical Electrodynamics, Jackson J. D., 3rd Edition, p. 186, eq. 5.56 
  G4ThreeVector pos_rel= G4ThreeVector(yTrack[0], yTrack[1], yTrack[2])-theDipoleCenter;
  G4double r=pos_rel.mag();
  G4double r3=r*r*r;
  pos_rel = pos_rel/r;
  G4double fac_mag=1.; //change for mu/4*pi
  G4ThreeVector Bvec=3.*pos_rel*(pos_rel.dot(theDipoleMoment)) - theDipoleMoment;
  Bvec=fac_mag*Bvec/r3;
  MagField[0]=Bvec.x();
  MagField[1]=Bvec.y();
  MagField[2]=Bvec.z();
  
  
  
    
}
