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
#include"CompoundField.hh"
#include"G4UIcmdWith3VectorAndUnit.hh"

////////////////////////////////////////////////////////////////////////////////
//
CompoundField::CompoundField(const std::vector<G4MagneticField*> aList_of_fields,const std::vector<G4AffineTransform*> aList_of_transformations)
{list_of_fields.clear();
 list_of_transformations.clear();
 list_of_inverse_transformations.clear();
 for (size_t i=0;i<aList_of_fields.size();i++){
 	list_of_fields.push_back(aList_of_fields[i]);
	list_of_transformations.push_back(aList_of_transformations[i]);
	if (aList_of_transformations[i]) {
		G4AffineTransform* aTransformation = new G4AffineTransform();
		*aTransformation = aList_of_transformations[i]->Inverse();
		list_of_inverse_transformations.push_back(aTransformation);
	}	
	else list_of_inverse_transformations.push_back(0);
 }
} 
////////////////////////////////////////////////////////////////////////////////
//
CompoundField::~CompoundField()
{;
}
////////////////////////////////////////////////////////////////////////////////
//
void CompoundField::GetFieldValue(const G4double yTrack[4],G4double *MagField) const
{ G4ThreeVector B = G4ThreeVector(0.,0.,0.);
  G4ThreeVector pos = G4ThreeVector(yTrack[0],yTrack[1],yTrack[2]);
  for (size_t i=0;i<list_of_fields.size();i++){
  	G4ThreeVector new_pos = pos;
	if (list_of_transformations[i] ) new_pos = list_of_transformations[i]->TransformPoint(pos);
	G4double y[4];
	y[0]=new_pos.x();
	y[1]=new_pos.y();
	y[2]=new_pos.z();
	y[3]=yTrack[3];
	list_of_fields[i]->GetFieldValue(y,MagField);
	G4ThreeVector B1 = G4ThreeVector(MagField[0],MagField[1],MagField[2]);
	if (list_of_transformations[i] ) list_of_inverse_transformations[i]->ApplyAxisTransform(B1);
	B+=B1;
  
  }
  MagField[0]=B.x();
  MagField[1]=B.y();
  MagField[2]=B.z(); 
    
}
