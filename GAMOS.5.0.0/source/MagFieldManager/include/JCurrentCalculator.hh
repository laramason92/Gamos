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
///////////////////////////////////////////////////////////
/////////Code developed by SpaceIt GmbH, Bern Switzerland
/////////For the Space european Agency
/////////First Author :L. Desorgher
///////////////////////////////////////////////////////////
#ifndef JCurrentCalculator_h
#define JCurrentCalculator_h 1
#include"globals.hh"
#include"G4ios.hh"
#include"G4MagneticField.hh"
#include<vector>
#include "G4ThreeVector.hh" 


class MagFieldModManMessenger;



class JCurrentCalculator 
{ public:
  
   JCurrentCalculator();
   virtual ~JCurrentCalculator();
   
   static JCurrentCalculator* GetInstance();
   
   
  public: 
  
  
  //field
   
  void SetField(G4MagneticField* aField);
  G4ThreeVector CalculateJCurrent(G4ThreeVector pos,G4double t,G4double ds);
  void CalculateJCurrentBetweenTwoPositions(G4ThreeVector pos1,G4ThreeVector pos2,G4double t, G4int npos,G4double ds, G4String output_file_name);
  void CalculateJCurrentOnACartesianGrid(G4double x0,G4double  dx, G4int nX,
					 G4double y0,G4double  dy, G4int nY,
					 G4double z0,G4double  dz, G4int nZ,G4double ds,G4double t,
					 G4String file_output);

 
 
 private:
 
   static JCurrentCalculator* instance;
 
 //attributes
   
   G4MagneticField* theField;
   
  

   
   
};
#endif
