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
#include"IdealToroidWithCircularCoil.hh"
#include"G4UIcmdWith3VectorAndUnit.hh"

////////////////////////////////////////////////////////////////////////////////
//
IdealToroidWithCircularCoil::IdealToroidWithCircularCoil( G4double RToroid, G4double RoutCoil,G4double dCoil, G4double I)
{ 
  rout_coil =RoutCoil ;
  dcoil = dCoil;
  current = I;
  r_toroid = RToroid;
  
  
  
} 
////////////////////////////////////////////////////////////////////////////////
//
IdealToroidWithCircularCoil::~IdealToroidWithCircularCoil()
{;
}
////////////////////////////////////////////////////////////////////////////////
//
void IdealToroidWithCircularCoil::GetFieldValue(const G4double yTrack[4],G4double *MagField) const
{ 
  G4ThreeVector B = G4ThreeVector(0.,0.,0.);
  
  MagField[0] = 0.;
  MagField[1] = 0.;
  MagField[2] = 0.;
  
  G4double rho =std::sqrt(yTrack[0]*yTrack[0] + yTrack[1]*yTrack[1]);
  G4double drho =rho - r_toroid;
  
  G4double enclosed_current=0.;
  G4double dr=std::sqrt(yTrack[2]*yTrack[2] + drho*drho);
  if (dr < rout_coil-dcoil) enclosed_current=current;
  else if (dr<rout_coil)  enclosed_current=current* (rout_coil-dr)/dcoil;
  else  return ;
		
	

  
  G4double b_phi =mu0*enclosed_current/2./pi/rho;
  //the direction of I is kept as for the Coil type1
  MagField[0] = yTrack[1]*b_phi/rho;
  MagField[1] = -yTrack[0]*b_phi/rho;
  
  return;	
 
  
  
 
  
  
    
}
