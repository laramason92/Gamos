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
#include"ConfinedFieldCreator.hh"
#include"ConfinedField.hh"
#include"G4UIcommand.hh"
#include"G4UIcmdWithoutParameter.hh"
#include"G4UIcmdWithADoubleAndUnit.hh"
#include"G4UIcmdWithAString.hh"


#include"G4UIdirectory.hh"
#include"G4UIparameter.hh"
#include"MagneticFieldModelManager.hh"
#include"TransformationManager.hh"
#include"G4AffineTransform.hh"

////////////////////////////////////////////////////////////////////////////////
//
ConfinedFieldCreator::ConfinedFieldCreator():
		ParametrizedFieldCreator(G4String("ConfinedField")) 
{ 
  
} 
////////////////////////////////////////////////////////////////////////////////
//
ConfinedFieldCreator::~ConfinedFieldCreator()
{
 
}
////////////////////////////////////////////////////////////////////////////////
//
void ConfinedFieldCreator::CreateMacroCommands(G4String cmd_dir_name, G4UImessenger* aMessenger)
{ 
  G4String dir_name= cmd_dir_name+"/ConfinedField/";
  ConfinedFieldCmdDir=new G4UIdirectory(dir_name.data());
  ConfinedFieldCmdDir->SetGuidance("Interactive commands to define a compound field");
  
  
  
  //Definition of Xmin
  G4String title_cmd =dir_name+"SetXmin";
  SetXminCmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetXminCmd->SetUnitCategory("Length");
  SetXminCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  //Definition of Ymin
  title_cmd =dir_name+"SetYmin";
  SetYminCmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetYminCmd->SetUnitCategory("Length");
  SetYminCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  //Definition of Zmin
  title_cmd =dir_name+"SetZmin";
  SetZminCmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetZminCmd->SetUnitCategory("Length");
  SetZminCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  title_cmd =dir_name+"SetXmax";
  SetXmaxCmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetXmaxCmd->SetUnitCategory("Length");
  SetXmaxCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  //Definition of Ymax
  title_cmd =dir_name+"SetYmax";
  SetYmaxCmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetYmaxCmd->SetUnitCategory("Length");
  SetYmaxCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  //Definition of Zmax
  title_cmd =dir_name+"SetZmax";
  SetZmaxCmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetZmaxCmd->SetUnitCategory("Length");
  SetZmaxCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  title_cmd =dir_name+"SetField";
  SetFieldCmd = new G4UIcmdWithAString(title_cmd,aMessenger);
  SetFieldCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  aField=0;



  
}
////////////////////////////////////////////////////////////////////////////////
//
G4bool ConfinedFieldCreator::DefineParameters(G4UIcommand* aCmd , G4String parameter_string){
	if (aCmd == SetXminCmd){
		Xmin=SetXminCmd->GetNewDoubleValue(parameter_string);
		G4cout<<Xmin<<std::endl;
	}
	else if (aCmd == SetYminCmd){
		Ymin=SetYminCmd->GetNewDoubleValue(parameter_string);
		G4cout<<Ymin<<std::endl;
	}
	else if (aCmd == SetZminCmd){
			Zmin=SetZminCmd->GetNewDoubleValue(parameter_string);

	}
	else if (aCmd == SetXmaxCmd){
		Xmax=SetXmaxCmd->GetNewDoubleValue(parameter_string);
		G4cout<<Xmax<<std::endl;
	}
	else if (aCmd == SetYmaxCmd){
		Ymax=SetYmaxCmd->GetNewDoubleValue(parameter_string);
		G4cout<<Ymax<<std::endl;
	}
	else if (aCmd == SetZmaxCmd){
			Zmax=SetZmaxCmd->GetNewDoubleValue(parameter_string);

	}
	else if (aCmd == SetFieldCmd){
		aField = MagneticFieldModelManager::GetInstance()->GetCreatedField(parameter_string);;

	}


	else return false;
	
	return true;

}
////////////////////////////////////////////////////////////////////////////////
//
G4MagneticField* ConfinedFieldCreator::CreateNewField()
{ nb_fields_of_same_model+=1;
  if (aField){
	  return new  ConfinedField(aField,Xmin,Xmax,Ymin,Ymax,Zmin,Zmax,true);
  }
  return 0;
}
