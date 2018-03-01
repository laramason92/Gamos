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
#include"MultiplyFieldCreator.hh"
#include"MultiplyField.hh"
#include"G4UIcommand.hh"
#include"G4UIcmdWithADoubleAndUnit.hh"
#include"G4UIdirectory.hh"
#include"MagneticFieldModelManager.hh"

////////////////////////////////////////////////////////////////////////////////
//
MultiplyFieldCreator::MultiplyFieldCreator():
		ParametrizedFieldCreator(G4String("MultiplyField")) 
{ MultiplyFieldCmdDir=0;
  SetFieldAndFactorCmd =0;
} 
////////////////////////////////////////////////////////////////////////////////
//
MultiplyFieldCreator::~MultiplyFieldCreator()
{
 if (MultiplyFieldCmdDir) delete MultiplyFieldCmdDir;
 if (SetFieldAndFactorCmd) delete SetFieldAndFactorCmd;
 
}
////////////////////////////////////////////////////////////////////////////////
//
void MultiplyFieldCreator::CreateMacroCommands(G4String cmd_dir_name, G4UImessenger* aMessenger)
{ 
 G4String dir_name= cmd_dir_name+"/MultiplyField/";
 
  MultiplyFieldCmdDir=new G4UIdirectory(dir_name.data());
  MultiplyFieldCmdDir->SetGuidance("Interactive commands to define a field multiplied by a constant factor");
  
  //Definition of the compound field
  G4String title_cmd =dir_name+"DefineFieldAndFactor";
  SetFieldAndFactorCmd = new G4UIcommand(title_cmd,aMessenger);
  SetFieldAndFactorCmd->SetGuidance("Define the field and the factor");
  G4UIparameter* field_name = new G4UIparameter("Field name",'s',false);
  G4UIparameter* factor_par = new G4UIparameter("Factor",'d',true);
  SetFieldAndFactorCmd->SetParameter(field_name);
  SetFieldAndFactorCmd->SetParameter(factor_par);
  SetFieldAndFactorCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  
  
  
 
  
  
}
////////////////////////////////////////////////////////////////////////////////
//
G4bool MultiplyFieldCreator::DefineParameters(G4UIcommand* aCmd , G4String parameter_string){
	if (aCmd ==SetFieldAndFactorCmd){
		const char* paramString= parameter_string;
     		G4String field_name;
     		std::istringstream is((char*)paramString);
     		is>>field_name>>factor;
		theField = MagneticFieldModelManager::GetInstance()->GetCreatedField(field_name);
			
	}
	
	else return false;
	
	return true;

}
////////////////////////////////////////////////////////////////////////////////
//
G4MagneticField* MultiplyFieldCreator::CreateNewField()
{ nb_fields_of_same_model+=1;
  return new MultiplyField(theField, factor);
}
