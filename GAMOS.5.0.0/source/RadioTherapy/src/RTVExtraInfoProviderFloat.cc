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
#include "RTVExtraInfoProviderFloat.hh"
#include "RTExtraInfoMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------
RTVExtraInfoProviderFloat::RTVExtraInfoProviderFloat()
{ 
 
}

//----------------------------------------------------------------
void RTVExtraInfoProviderFloat::InitVariables()
{
  theWord = new float;  

  RTExtraInfoMgr::GetInstance()->AddExtraInfoProviderFloat(this);
  theFileName = GmParameterMgr::GetInstance()->GetStringValue("RTExtraInfoProviderFloat:FileName","RTExtraInfoProvider.summ");
}


//----------------------------------------------------------------
void RTVExtraInfoProviderFloat::DumpIndices(std::ostream& out)
{
  out << GetName() 
      << " FLOAT INDEX " << theOrder << G4endl;
}