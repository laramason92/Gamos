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
#include "MagneticEquationOfMotion.hh"


////////////////////////////////////////////////////////////////////////////////
//
MagneticEquationOfMotion::MagneticEquationOfMotion( G4MagneticField* MagField )
                              : G4Mag_EqRhs( MagField ) 
{ reverse_time=false;
  direction=1.;
  Equation=&MagneticEquationOfMotion::LorentzMotion;
}
////////////////////////////////////////////////////////////////////////////////
//
void MagneticEquationOfMotion::EvaluateRhsGivenB( const G4double y[],
			                        const G4double B[3],
				                      G4double dydx[] ) const
{ (this->*Equation)( y,  B, dydx);
  return ;
}
/////////////////////////////////////////////////////////////////////////////////
//
void MagneticEquationOfMotion::MotionAlongBfieldLine( const G4double*,
			                            const G4double B[3],
				                          G4double dydx[] ) const
{
  G4double Bmag=direction*std::sqrt(B[0]*B[0] + B[1]*B[1] + B[2]*B[2]);
  dydx[0] = B[0]/Bmag;       
  dydx[1] = B[1]/Bmag;       
  dydx[2] = B[2]/Bmag;
   
  dydx[3]=0.;
  dydx[4]=0.;
  dydx[5]=0.; 
  return ;
}
/////////////////////////////////////////////////////////////////////////////////
//
void MagneticEquationOfMotion::LorentzMotion( const G4double y[],
			                    const G4double B[3],
				            G4double dydx[] ) const
{
  G4double momentum_mag_square = y[3]*y[3] +  y[4]*y[4] +  y[5]*y[5];
  G4double inv_momentum_magnitude = 
            direction * 1.0 / sqrt( momentum_mag_square );

  G4double cof =   FCof()*inv_momentum_magnitude;

  dydx[0] = y[3]*inv_momentum_magnitude;       //  (d/ds)x = Vx/V
  dydx[1] = y[4]*inv_momentum_magnitude;       //  (d/ds)y = Vy/V
  dydx[2] = y[5]*inv_momentum_magnitude;       //  (d/ds)z = Vz/V

  dydx[3] = cof*(y[4]*B[2] - y[5]*B[1]) ;   // Ax = a*(Vy*Bz - Vz*By)
  dydx[4] = cof*(y[5]*B[0] - y[3]*B[2]) ;   // Ay = a*(Vz*Bx - Vx*Bz)
  dydx[5] = cof*(y[3]*B[1] - y[4]*B[0]) ;   // Az = a*(Vx*By - Vy*Bx)
   

  return ;
}

////////////////////////////////////////////////////////////////////////////////
//
void MagneticEquationOfMotion::SetReverseTimeMode(G4bool abool)
{ direction=1.;
  reverse_time=abool;
  if (reverse_time) direction= -1.;
}
////////////////////////////////////////////////////////////////////////////////
//
void MagneticEquationOfMotion::SetEquationType(G4String aString)
{ if (aString=="LORENTZ_MOTION")  
          Equation=&MagneticEquationOfMotion::LorentzMotion;
  if (aString=="BFIELD_LINE")
           Equation=&MagneticEquationOfMotion::MotionAlongBfieldLine;
}




