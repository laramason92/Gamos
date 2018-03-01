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
#include "RTVClassifierByExtraInfo.hh"
#include "G4UIcommand.hh"
#include "RTExtraInfoMgr.hh"

//---------------------------------------------------------------
RTVClassifierByExtraInfo::RTVClassifierByExtraInfo(G4String name) : GmVClassifier( name )
{
  theExtraInfoMgr = RTExtraInfoMgr::GetInstance();
}

//---------------------------------------------------------------
G4String RTVClassifierByExtraInfo::GetIndexName(G4int index)
{
   return G4UIcommand::ConvertToString( G4int(index) );
}
/*	if( theNExtraInfoLong == 2 ) {
	  G4Exception(" RTPhaseSpaceUA::BeginOfRunAction"
		      ,"Two many extra info providers for variables of type long",
		      JustWarning,
		      G4String("This provider info will not be used: "
			       + eiProviders[ii]->GetName()
			       + " , position "+ GmGenUtils::itoa(jj)).c_str());
	} else {
*/
