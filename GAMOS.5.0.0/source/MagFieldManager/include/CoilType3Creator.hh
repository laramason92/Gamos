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
#ifndef CoilType3Creator_HH
#define CoilType3Creator_HH 1
#include"globals.hh"
#include"G4ios.hh"
#include"ParametrizedFieldCreator.hh"

class G4UIcmdWithADoubleAndUnit;
class G4UIdirectory;


class CoilType3Creator : public ParametrizedFieldCreator 
{ 
  public:
  
   CoilType3Creator();
   ~CoilType3Creator();
  
  public: //methods
  
   virtual void CreateMacroCommands(G4String cmd_dir_name, G4UImessenger* aMessenger);
   virtual G4bool DefineParameters(G4UIcommand* aCmd , G4String parameter_string);
   virtual G4MagneticField* CreateNewField();
   
  private :
   
   G4UIdirectory* CoilType3CmdDir;
   G4UIcmdWithADoubleAndUnit* SetRoutCmd;
   G4UIcmdWithADoubleAndUnit* SetRoutCornerCmd;
   G4UIcmdWithADoubleAndUnit* SetRinCmd;
   G4UIcmdWithADoubleAndUnit* SetD1Cmd;
   G4UIcmdWithADoubleAndUnit* SetD2Cmd;
   G4UIcmdWithADoubleAndUnit* SetICmd;
   G4UIcmdWithADoubleAndUnit* SetAlpha0Cmd;
   G4UIcmdWithADoubleAndUnit* SetdAlphaCmd;
   
   
   G4double D1;
   G4double D2;
   G4double Rout, Rin;
   G4double RoutCorner;
   G4double Alpha0,dAlpha;
   G4double I;
   
   

} ;

#endif