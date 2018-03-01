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
#include"ConfinedField.hh"
#include"G4UIcmdWith3VectorAndUnit.hh"

////////////////////////////////////////////////////////////////////////////////
//
ConfinedField::ConfinedField(G4MagneticField* aField, G4double var1_min,G4double var1_max, 
    			G4double var2_min,G4double var2_max, 
			G4double var3_min,G4double var3_max,
			G4bool aBool)
{ theField = aField;
  IsCartesian = aBool;
  if (IsCartesian){
  	x_min=var1_min;
	x_max=var1_max;
	y_min=var2_min;
	y_max=var2_max;
	z_min=var3_min;
	z_max=var3_max;
  }
  else {
  	rho_min=var1_min;
	rho_max=var1_max;
	phi_min=var2_min;
	phi_max=var2_max;
	z_min=var3_min;
	z_max=var3_max;
  }
  

} 
////////////////////////////////////////////////////////////////////////////////
//
ConfinedField::~ConfinedField()
{;
}
////////////////////////////////////////////////////////////////////////////////
//
void ConfinedField::GetFieldValue(const G4double yTrack[4],G4double *MagField) const
{ G4ThreeVector B = G4ThreeVector(0.,0.,0.);
  G4bool ComputeTheField =false;
  
  if (IsCartesian){
  	ComputeTheField = (yTrack[0]<=x_max && yTrack[0]>=x_min &&  
			   yTrack[1]<=y_max && yTrack[1]>=y_min &&
			   yTrack[2]<=z_max && yTrack[2]>=z_min );
  }
  else {
  	G4double rho =std::sqrt(yTrack[0]*yTrack[0] + yTrack[1]*yTrack[1]);
	G4double cphi=1.;
	G4double sphi=0.;
	if (rho >0.0001*m){
		cphi=yTrack[0]/rho;
		sphi=yTrack[1]/rho;
	}
	G4double phi=std::acos(cphi);
	if (sphi <0) phi=twopi-phi;
	ComputeTheField = (rho<=rho_max && rho>=rho_min &&
			   phi<=phi_max && phi>= phi_min &&
			   yTrack[2]<=z_max && yTrack[2]>=z_min );
				
  
  }
  if (ComputeTheField) {
  	theField->GetFieldValue(yTrack,MagField);
  } 
  else {
  	MagField[0]=0.;
  	MagField[1]=0.;
  	MagField[2]=0.; 
  	
  }	
  
  
    
}
