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
#include "GmSDMessenger.hh"
#include "GmSDVirtSegmentedSphereThetaPhi.hh"
#include "GmSDVirtSegmentedSphereRThetaPhi.hh"
#include "GmSDSimple.hh"
#include "GmSDOpticalPhoton.hh"

#include "G4SDManager.hh"
#include "G4UImanager.hh"
#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"
#include "G4LogicalVolumeStore.hh"

#include "GamosCore/GamosSD/include/GmSDVerbosity.hh"
#include "GamosCore/GamosSD/include/GmDetUnitIDBuilderFromAncestors.hh"

#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "Reflex/PluginService.h"

#include <sstream>

//-------------------------------------------------------------------
GmSDMessenger::GmSDMessenger() 
{
  assocCmd = new GmUIcmdWithAString("/gamos/SD/assocSD2LogVol",this);
  assocCmd->SetGuidance("Associate a Sensitive Detector to a Logical Volume");
  assocCmd->SetParameterName("choice",true);
  assocCmd->AvailableForStates(G4State_Idle);

  /*  nSimpleNAncesCmd = new GmUIcmdWithAString("/gamos/SD/simpleSDnAncestors",this);
  nSimpleNAncesCmd->SetGuidance("For an Simple SD, set the number of ancestors that are used for building the DetUnitID");
  nSimpleNAncesCmd->SetParameterName("choice",true);
  nSimpleNAncesCmd->AvailableForStates(G4State_Idle);


  nSimpleNShiftCmd = new GmUIcmdWithAString("/gamos/SD/simpleSDnShift",this);
  nSimpleNShiftCmd->SetGuidance("For an Simple SD, set the shift that will be used for each ancestor level when building the DettUnitID");
  nSimpleNShiftCmd->SetParameterName("choice",true);
  nSimpleNShiftCmd->AvailableForStates(G4State_Idle);
  */
}

//-------------------------------------------------------------------
GmSDMessenger::~GmSDMessenger() 
{
  if (assocCmd) delete assocCmd;
}

//-------------------------------------------------------------------
void GmSDMessenger::SetNewValue(G4UIcommand * command,
						   G4String newValues)    
{ 
  if (command == assocCmd) {
    GmGenUtils::CheckNWords(newValues,3,"Command: "+ command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + "  needs 3 arguments: SDclass SDtype logvol"); 

    G4String SDclass;
    G4String SDtype;
    G4String logvol;
    
    std::istringstream is((char*)newValues.c_str());
    is >> SDclass >> SDtype >> logvol;

    AssociateSD2LogVol( SDclass, SDtype, logvol );

    /*
  } else if( command == nSimpleNAncesCmd ) {
    G4String logvol;
    G4String snances;
    std::istringstream is((char*)newValues.c_str());
    is >> logvol >> snances;
    std::vector<G4LogicalVolume*> lvp = GmGeometryUtils::GetInstance()->GetLogicalVolumes( logvol, 1 );
    for( unsigned int ii = 0; ii > lvp.size(); ii++ ){
      G4VSensitiveDetector* sd = lvp[ii]->GetSensitiveDetector();
      if( ! static_cast<GmSDSimple*>(sd) ) {
        //t     GmGeometryUtils::GetInstance()->DumpLVTree();
        G4Exception(" GmSDMessenger::SetNewValue setting simpleSDnAncestors for a volume that does not exist "  + logvol );
      } else {
        GmSDSimple* sdsim = static_cast<GmSDSimple*>(sd);
        sdsim->GetDetUnitIDBuilder()->SetNAncestors( atoi(snances) );
      }
    }
  } else if( command == nSimpleNShiftCmd ) {
    G4String logvol;
    G4String snshift;
    std::istringstream is((char*)newValues.c_str());
    is >> logvol >> snshift;
    std::vector<G4LogicalVolume*> lvp = GmGeometryUtils::GetInstance()->GetLogicalVolumes( logvol, 1 );
    for( unsigned int ii = 0; ii > lvp.size(); ii++ ){  
      G4VSensitiveDetector* sd = lvp[ii]->GetSensitiveDetector();
      if( ! static_cast<GmSDSimple*>(sd) ) {
	//t     GmGeometryUtils::GetInstance()->DumpLVTree();
	G4Exception(" GmSDMessenger::SetNewValue setting simpleSDnShift for a volume that does not exist "  + logvol );
      } else {
	GmSDSimple* sdsim = static_cast<GmSDSimple*>(sd);
	sdsim->GetDetUnitIDBuilder()->SetNShift( atoi(snshift) );
      }
    }
    */
  }
  
}


//-------------------------------------------------------------------
void GmSDMessenger::AssociateSD2LogVol( G4String& sdclass, G4String& sdtype, G4String& lvname )
{

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(debugVerb) ) G4cout << " GmSDMessenger::AssociateSD2LogVol: associat sdclass " << sdclass << G4endl; 
#endif
  std::vector<G4LogicalVolume*> logvols = GmGeometryUtils::GetInstance()->GetLogicalVolumes( lvname, FALSE ); //error is detected below

  if( logvols.size() == 0 ) {
    GmGeometryUtils::GetInstance()->DumpG4LVTree();
    G4Exception("GmSDMessenger::AssociateSD2LogVol",
		" Cannot associate sensitive detector to a logical volume that does not exist ",
		FatalErrorInArgument,
		("Command issued:  /gamos/SD/assocSD2LogVol " + sdclass + " " + sdtype + " " +  lvname).c_str() );
  }

  for( unsigned int ii = 0; ii < logvols.size(); ii++ ){
    G4LogicalVolume* logvol = logvols[ii];
    if( GmVSD::GetSDSegmType() == SDvirtSegm ) {
      G4SDManager* sdmgr = G4SDManager::GetSDMpointer();
      //	G4VSensitiveDetector* sd = 0;
      G4String sdPathAndName = sdtype+G4String("/")+logvol->GetName();

      G4VSensitiveDetector* theSD = Reflex::PluginService::Create<G4VSensitiveDetector*>(sdclass, sdPathAndName);
      //      G4cout << " sdclass " << sdclass << G4endl;
      
      if( sdclass == "GmSDVirtSegmentedSphereRThetaPhi" || sdclass == "GmSDVirtSegmentedSphereThetaPhi" ) {
	GmSDVirtSegmentedSphere* sdv = (GmSDVirtSegmentedSphere*)(theSD);
	sdv->SetLogVol( logvol );
	sdv->DefineSegmentation(sdtype);
	//	G4cout << " setting logvol " << G4endl;
      }
      
      // G4cout << " sd " << sd << G4endl;
      if( !theSD ) {
	G4Exception("GmSDMessenger::AssociateSD2Vol",
		    "Wrong argument",
		    FatalErrorInArgument,
		    ("SensDet class does not exist: "+sdclass+" , for type: "+sdtype+" and volume: "+lvname).c_str());
      }
#ifndef GAMOS_NO_VERBOSE
      if( SDVerb(infoVerb) ) G4cout << " adding new SD " << sdtype+G4String("/")+logvol->GetName() << G4endl;
#endif
      sdmgr->AddNewDetector(theSD);
      logvol->SetSensitiveDetector( theSD );
    }
  }
 
}
