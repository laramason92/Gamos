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
#include"ReplicateFieldCreator.hh"
#include"ReplicateField.hh"
#include"G4UIcommand.hh"
#include"G4UIcmdWithoutParameter.hh"
#include"G4UIcmdWithAString.hh"
#include"G4UIcmdWithAnInteger.hh"
#include"G4UIcmdWithADoubleAndUnit.hh"
#include"G4UIdirectory.hh"
#include"G4UIparameter.hh"
#include"MagneticFieldModelManager.hh"
#include"TransformationManager.hh"
#include"G4AffineTransform.hh"

////////////////////////////////////////////////////////////////////////////////
//
ReplicateFieldCreator::ReplicateFieldCreator():
		ParametrizedFieldCreator(G4String("ReplicateField")) 
{ ReplicateFieldCmdDir =0;
  SelectFieldCmd =0 ;
  SetRCmd =0 ;
  SetPhi0Cmd =0 ;
  SetdPhiCmd =0 ;
  SetNReplicateCmd =0 ;
} 
////////////////////////////////////////////////////////////////////////////////
//
ReplicateFieldCreator::~ReplicateFieldCreator()
{ if (ReplicateFieldCmdDir) delete ReplicateFieldCmdDir; 
  if  (SelectFieldCmd ) delete  SelectFieldCmd;
  if  (SetRCmd ) delete  SetRCmd;
  if  (SetPhi0Cmd ) delete  SetPhi0Cmd;
  if  (SetdPhiCmd ) delete  SetdPhiCmd;
  if  (SetNReplicateCmd ) delete  SetNReplicateCmd;
 
}
////////////////////////////////////////////////////////////////////////////////
//
void ReplicateFieldCreator::CreateMacroCommands(G4String cmd_dir_name, G4UImessenger* aMessenger)
{ 
  G4String dir_name= cmd_dir_name+"/ReplicateField/";
  ReplicateFieldCmdDir=new G4UIdirectory(dir_name.data());
  ReplicateFieldCmdDir->SetGuidance("Interactive commands to define a replicated field");
  
  G4String title_cmd =dir_name+"SelectField";
  SelectFieldCmd = new G4UIcmdWithAString(title_cmd,aMessenger);
  SelectFieldCmd ->SetGuidance("Select the field that shoud be replicated");
  SelectFieldCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  title_cmd =dir_name+"SetR";
  SetRCmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetRCmd->SetUnitCategory("Length");
  SetRCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  title_cmd =dir_name+"SetPhi0";
  SetPhi0Cmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetPhi0Cmd->SetUnitCategory("Angle");
  SetPhi0Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  title_cmd =dir_name+"SetdPhi";
  SetdPhiCmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetdPhiCmd->SetUnitCategory("Angle");
  SetdPhiCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  title_cmd =dir_name+"SetNCopies";
  SetNReplicateCmd= new G4UIcmdWithAnInteger(title_cmd,aMessenger) ;
  SetNReplicateCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
 

}
////////////////////////////////////////////////////////////////////////////////
//
G4bool ReplicateFieldCreator::DefineParameters(G4UIcommand* aCmd , G4String parameter_string){
	if (aCmd == SelectFieldCmd){
		FieldToReplicate = MagneticFieldModelManager::GetInstance()->GetCreatedField(parameter_string);
		if (!FieldToReplicate){
			G4cout<<"The field that you did select does not exist!"<<std::endl;
		}	
	}
	else if (aCmd == SetRCmd){
		 r = SetRCmd->GetNewDoubleValue(parameter_string);
	}
	else if (aCmd == SetPhi0Cmd){
		 phi0 = SetPhi0Cmd->GetNewDoubleValue(parameter_string);
	}
	else if (aCmd == SetdPhiCmd){
		 dphi = SetdPhiCmd->GetNewDoubleValue(parameter_string);
	}
	else if (aCmd == SetNReplicateCmd){
		  n_replicate = size_t(SetNReplicateCmd->GetNewIntValue(parameter_string));
	}
	
	
	else return false;
	
	return true;

}
////////////////////////////////////////////////////////////////////////////////
//
G4MagneticField* ReplicateFieldCreator::CreateNewField()
{ nb_fields_of_same_model+=1;
  if ( FieldToReplicate) {
  	return new ReplicateField(phi0, dphi, n_replicate, r, FieldToReplicate);
  }
  else {
  	G4cout<<"You did not select a field to replicate!"<<std::endl;
	return 0; 
  }	
}
