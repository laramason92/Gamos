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
#include"CurvedSegment.hh"
#include"G4UIcmdWith3VectorAndUnit.hh"
#include"special_functions.hh"
#include"globals.hh"
#include"BiotSavartIntegrator.hh"
////////////////////////////////////////////////////////////////////////////////
//
CurvedSegment::CurvedSegment(G4double D1, G4double RIN,G4double ROUT, G4double PHI0, G4double DPHI, G4double current)//current turn from y to z
{ 

  d1=D1;
  phi0=PHI0;
  dphi=DPHI;
  rin=RIN;
  rout=ROUT;
  I=current;
  
  theBiotSavartIntegrator = BiotSavartIntegrator::GetInstance();
} 
////////////////////////////////////////////////////////////////////////////////
//
CurvedSegment::~CurvedSegment()
{;
}
////////////////////////////////////////////////////////////////////////////////
//
void CurvedSegment::GetFieldValue(const G4double yTrack[4],G4double *MagField) const
{ 
  
 
  //first section;
  G4double pos[3];
  pos[0]=yTrack[0];
  pos[1]=yTrack[1];
  pos[2]=yTrack[2]; 

  G4ThreeVector B = theBiotSavartIntegrator->BfieldFromCurvedSegmentInYZ(pos, phi0, dphi , rout, rin, d1);
  
  
 
   
   //still need to multiply by the factor mu_0/4./pi
   G4double fac=mu0*I/4./pi;
   B=B*fac;
   
   
   
   MagField[0]=B.x();
   MagField[1]=B.y();
   MagField[2]=B.z(); 
   
 
}
