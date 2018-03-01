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
#ifndef TransManMessenger_h
#define TransManMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class TransformationManager;
class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;
class G4UIcmdWith3Vector;
class G4UIcmdWith3VectorAndUnit;

class TransManMessenger : public G4UImessenger
{public:
   TransManMessenger(TransformationManager* aTransManager);
  ~TransManMessenger();
  
   void SetNewValue(G4UIcommand * command,G4String newValues);
 
 private:
   TransformationManager* theTransManager;
   G4UIdirectory*  TransManagerDir;
   
   G4UIcmdWithAString* SelectTransformationCmd;
   G4UIcmdWithoutParameter* ResetToUnitaryCmd;
   G4UIcmdWithADoubleAndUnit* AddRotationAroundXCmd;
   G4UIcmdWithADoubleAndUnit* AddRotationAroundYCmd;
   G4UIcmdWithADoubleAndUnit* AddRotationAroundZCmd;
   G4UIcommand* AddRotationAroundAnAxisCmd;
   G4UIcmdWith3VectorAndUnit* AddTranslationCmd;
   
   G4UIcommand* MultiplyTransformationsCmd;
   G4UIcmdWithAString* MultiplyByCmd;
   G4UIcmdWithAString* SaveTransformationCmd;
   G4UIcmdWithoutParameter* ClearListOfTransformationsCmd; 
  
  
 
   //  commands
   
};
#endif
