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
#ifndef TransformationManager_h
#define TransformationManager_h 1

#include"globals.hh"
#include"G4ios.hh"
#include"G4AffineTransform.hh"
#include<vector> 


class TransManMessenger;

class TransformationManager 
{ public:
  
   TransformationManager();
   virtual ~TransformationManager();
   
   static TransformationManager* GetInstance();
   
 
  public: 
   
   G4AffineTransform* GetTransformation(G4String TransformationName);
   void SelectTransformation(G4String TransformationName);
   void ResetToUnitary();
   void AddRotationAroundX(G4double angle);
   void AddRotationAroundY(G4double angle);
   void AddRotationAroundZ(G4double angle);
   void AddRotationAroundAnAxis(G4double angle, G4ThreeVector anAxis);
   void AddTranslation(G4ThreeVector aVec);
   void MultiplyTransformations(G4String name1, G4String name2);
   void MultiplyBy(G4String name1);
   
   void SaveTransformation(G4String name);
   void ClearListOfTransformations();
  // void RemoveATransformation(G4String name);
  
  private:
 
   static TransformationManager* instance;
 
 //attributes
   
   //List of Transformation
   std::vector<G4String> listOfTransformationNames;
   std::vector<G4AffineTransform*> listOfTransformations;
   
   G4AffineTransform theTransformation;
 
   
   TransManMessenger* theMessenger;
  

   
   
};
#endif
