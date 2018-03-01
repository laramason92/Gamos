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
#include"IdealToroidWithCircularCoilCreator.hh"
#include"IdealToroidWithCircularCoil.hh"
#include"G4UIcommand.hh"
#include"G4UIcmdWithoutParameter.hh"
#include"G4UIdirectory.hh"
#include"G4UIparameter.hh"
#include"MagneticFieldModelManager.hh"

#include"G4UIcmdWithADoubleAndUnit.hh"

////////////////////////////////////////////////////////////////////////////////
//
IdealToroidWithCircularCoilCreator::IdealToroidWithCircularCoilCreator():
		ParametrizedFieldCreator(G4String("IdealToroidWithCircularCoil")) 
{ 
  
} 
////////////////////////////////////////////////////////////////////////////////
//
IdealToroidWithCircularCoilCreator::~IdealToroidWithCircularCoilCreator()
{
 
}
////////////////////////////////////////////////////////////////////////////////
//
void IdealToroidWithCircularCoilCreator::CreateMacroCommands(G4String cmd_dir_name, G4UImessenger* aMessenger)
{ 
  G4String dir_name= cmd_dir_name+"/IdealToroidWithCircularCoil/";
  IdealToroidWithCircularCoilCmdDir=new G4UIdirectory(dir_name.data());
  IdealToroidWithCircularCoilCmdDir->SetGuidance("Interactive commands to define a compound field");  
 
 
 
 //Definition of RToroid
  G4String title_cmd =dir_name+"SetRToroid";
  SetRToroidCmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetRToroidCmd->SetGuidance("Define the Radius of the Toroid");
  SetRToroidCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetRToroidCmd->SetUnitCategory("Length");
  
   //Definition of RoutCoil
  title_cmd =dir_name+"SetRoutCoil";
  SetRoutCoilCmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetRoutCoilCmd->SetGuidance("Define the outer Radius of the circular sections  of the  Coils in the Toroid");
  SetRoutCoilCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetRoutCoilCmd->SetUnitCategory("Length");
 
  //Definition of Dcoil
  title_cmd =dir_name+"SetDcoil";
  SetDcoilCmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetDcoilCmd->SetGuidance("Define the width of the the Coil in the Rho direction");
  SetDcoilCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetDcoilCmd->SetUnitCategory("Length");
  
  
  

  
  //Definition of the the Current
 
  title_cmd =dir_name+"SetI";
  SetICmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetICmd->SetGuidance("Define the current turns nI");
  SetICmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetICmd->SetUnitCategory("Electric current");
 
  
  
  
  

}
////////////////////////////////////////////////////////////////////////////////
//
G4bool IdealToroidWithCircularCoilCreator::DefineParameters(G4UIcommand* aCmd , G4String parameter_string){
	if (aCmd == SetRoutCoilCmd){
		RoutCoil = SetRoutCoilCmd->GetNewDoubleValue(parameter_string);
	}
	if (aCmd == SetRToroidCmd){
		RToroid = SetRToroidCmd->GetNewDoubleValue(parameter_string);
	}
	else if (aCmd == SetDcoilCmd){
		Dcoil = SetDcoilCmd->GetNewDoubleValue(parameter_string);
	}
	
	
	else if (aCmd == SetICmd){
		I = SetICmd->GetNewDoubleValue(parameter_string);
	}
	else return false;
	
	return true;
	
	

}
////////////////////////////////////////////////////////////////////////////////
//
G4MagneticField* IdealToroidWithCircularCoilCreator::CreateNewField()
{ nb_fields_of_same_model+=1;
  return new IdealToroidWithCircularCoil( RToroid, RoutCoil, Dcoil,  I);
 
  
}
