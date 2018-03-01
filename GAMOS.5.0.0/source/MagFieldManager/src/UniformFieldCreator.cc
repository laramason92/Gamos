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
#include"UniformFieldCreator.hh"
#include"G4UniformMagField.hh"
#include"G4UIcmdWith3VectorAndUnit.hh"
#include"G4UIdirectory.hh"

////////////////////////////////////////////////////////////////////////////////
//
UniformFieldCreator::UniformFieldCreator():
		ParametrizedFieldCreator(G4String("UniformField")) 
{theCmd =0;
 UniformFieldDir =0;
} 
////////////////////////////////////////////////////////////////////////////////
//
UniformFieldCreator::~UniformFieldCreator()
{ if (theCmd) delete theCmd;
  if (UniformFieldDir) delete UniformFieldDir;
}
////////////////////////////////////////////////////////////////////////////////
//
void UniformFieldCreator::CreateMacroCommands(G4String cmd_dir_name, G4UImessenger* aMessenger)
{ 
 
  G4String dir_name= cmd_dir_name+"/UniformField/";
  
  UniformFieldDir=new G4UIdirectory(dir_name.data());
  UniformFieldDir->SetGuidance("Interactive commands to define a uniform field");
  
  
  G4String title_cmd =cmd_dir_name+"/UniformField/SetBvec";
  theCmd = new G4UIcmdWith3VectorAndUnit(title_cmd,aMessenger);
  theCmd->SetGuidance("Define a uniform field");
  theCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  theCmd->SetUnitCategory("Magnetic flux density");
  
    
  
}
////////////////////////////////////////////////////////////////////////////////
//
G4bool UniformFieldCreator::DefineParameters(G4UIcommand* aCmd , G4String parameter_string)
{ if (aCmd != theCmd) return false;
  B_vec = theCmd->GetNew3VectorValue(parameter_string);
  return true;

}
////////////////////////////////////////////////////////////////////////////////
//
G4MagneticField* UniformFieldCreator::CreateNewField()
{ nb_fields_of_same_model+=1;
  return new G4UniformMagField(B_vec);

}
