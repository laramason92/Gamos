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
#include"LinearSegmentCreator.hh"
#include"LinearSegment.hh"
#include"G4UIcommand.hh"
#include"G4UIcmdWithADoubleAndUnit.hh"
#include"G4UIdirectory.hh"

////////////////////////////////////////////////////////////////////////////////
//
LinearSegmentCreator::LinearSegmentCreator():
		ParametrizedFieldCreator(G4String("LinearSegment")) 
{ LinearSegmentCmdDir=0;
  SetD1Cmd=0;
  SetD2Cmd=0;
  SetLCmd=0;
  SetICmd=0;
} 
////////////////////////////////////////////////////////////////////////////////
//
LinearSegmentCreator::~LinearSegmentCreator()
{
 if (LinearSegmentCmdDir) delete LinearSegmentCmdDir;
 if (SetD1Cmd) delete SetD1Cmd;
 if (SetD2Cmd) delete SetD2Cmd;
 if (SetLCmd) delete SetLCmd;
 if (SetICmd) delete SetICmd;
}
////////////////////////////////////////////////////////////////////////////////
//
void LinearSegmentCreator::CreateMacroCommands(G4String cmd_dir_name, G4UImessenger* aMessenger)
{ 
 G4String dir_name= cmd_dir_name+"/LinearSegment/";
  LinearSegmentCmdDir=new G4UIdirectory(dir_name.data());
  LinearSegmentCmdDir->SetGuidance("Interactive commands to define a LinearSegment");
  
  
  
 
 
  //Definition of D1
  G4String title_cmd =dir_name+"SetLx";
  SetD1Cmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetD1Cmd->SetGuidance("Define the x size of the linear segement");
  SetD1Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetD1Cmd->SetUnitCategory("Length");
  
  //Definition of D2
  title_cmd =dir_name+"SetLy";
  SetD2Cmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetD2Cmd->SetGuidance("Define the y size of the linear segment");
  SetD2Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetD2Cmd->SetUnitCategory("Length");
  //Definition of the the Current
  
  
  //Definition of L
  title_cmd =dir_name+"SetLz";
  SetLCmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetLCmd->SetGuidance("Define the z size of the linear segement");
  SetLCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetLCmd->SetUnitCategory("Length");
  
 //Definition of the the Current
 
  title_cmd =dir_name+"SetI";
  SetICmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetICmd->SetGuidance("Define the current flowing trough the segment");
  SetICmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetICmd->SetUnitCategory("Electric current");
 
  
  
  
  
  
 
  
  
}
////////////////////////////////////////////////////////////////////////////////
//
G4bool LinearSegmentCreator::DefineParameters(G4UIcommand* aCmd , G4String parameter_string){
	if (aCmd == SetD1Cmd){
		D1 = SetD1Cmd->GetNewDoubleValue(parameter_string);
	}
	else if (aCmd == SetD2Cmd){
		D2 = SetD1Cmd->GetNewDoubleValue(parameter_string);
	}
	else if (aCmd == SetLCmd){
		L = SetLCmd->GetNewDoubleValue(parameter_string);
	}
	else if (aCmd == SetICmd){
		I = SetICmd->GetNewDoubleValue(parameter_string);
	}
	else return false;
	
	return true;

}
////////////////////////////////////////////////////////////////////////////////
//
G4MagneticField* LinearSegmentCreator::CreateNewField()
{ nb_fields_of_same_model+=1;
  return new LinearSegment(D1,D2, L,I);
}