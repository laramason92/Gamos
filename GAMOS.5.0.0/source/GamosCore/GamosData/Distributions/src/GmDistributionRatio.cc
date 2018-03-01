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
#include "GmDistributionRatio.hh"
#include "GamosCore/GamosData/Distributions/include/GmDistributionVerbosity.hh"

#include "GamosCore/GamosBase/Base/include/GmDistributionMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "GamosCore/GamosData/Management/include/GmVData.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"

//---------------------------------------------------------------------
GmDistributionRatio::GmDistributionRatio(G4String name)
  : GmVDistribution(name)
{
  BuildData();
}

//---------------------------------------------------------------------
void GmDistributionRatio::BuildData()
{
  GmParameterMgr* parMgr = GmParameterMgr::GetInstance();
  G4String distName = parMgr->GetStringValue(theName+":DistributionPre","");
  std::vector<G4String> params; 
  params.push_back(distName);
  params.push_back(distName);
  theDistPre = GmDistributionMgr::GetInstance()->FindOrBuildDistribution(params);

  distName = parMgr->GetStringValue(theName+":DistributionPost","");
  params.clear();
  params.push_back(distName);
  params.push_back(distName);
  theDistPost = GmDistributionMgr::GetInstance()->FindOrBuildDistribution(params);

  theMaxValue = parMgr->GetNumericValue(theName+":MaxValue",DBL_MAX);

  bNoRepeatWeight = G4bool( parMgr->GetNumericValue(theName+":NoRepeatWeight",0));

}


//---------------------------------------------------------------------
G4double GmDistributionRatio::GetValueFromStep(const G4Step* aStep)
{

  G4double valuePre = theDistPre->GetValueFromStep( aStep );
  G4double valuePost = theDistPost->GetValueFromStep( aStep );
  if( bNoRepeatWeight && 1.001/aStep->GetTrack()->GetWeight() >= valuePost )  {
#ifndef GAMOS_NO_VERBOSE
    if( DistVerb(debugVerb) && valuePost > valuePre ) {
      G4cout << " GmDistributionRatio NoRepeatWeight " << 1.001/aStep->GetTrack()->GetWeight() << " " << valuePost << G4endl;
    }
#endif
    return 1;
  }

  if( valuePre == 0. ) {
    return 0;
  } else {
    G4double indexVal = valuePost/valuePre;
    if( indexVal > theMaxValue ) {
      G4Exception("GmDistributionRatio::GetValueFromStep",
		  "Too big return value",
		  JustWarning,
		  G4String("Value = "+GmGenUtils::ftoa(indexVal)+ " >= " + GmGenUtils::ftoa(theMaxValue)).c_str());
      indexVal = theMaxValue;
    }
#ifndef GAMOS_NO_VERBOSE
    if( DistVerb(debugVerb)) {
      G4cout << "GmDistributionRatio::GetValueFromStep " << indexVal
	     << " valuePost " << valuePost
	     << " valuePre " << valuePre 
	     << G4endl;
    }
#endif
    return indexVal;
  }

}


//---------------------------------------------------------------------
G4double GmDistributionRatio::GetValueFromTrack(const G4Track* aTrack)
{

  G4double valuePre = theDistPre->GetValueFromTrack( aTrack );
  G4double valuePost = theDistPost->GetValueFromTrack( aTrack );

  if( valuePre == 0. ) {
    return 0;
  } else {
    G4double indexVal = valuePost/valuePre;
    if( indexVal > theMaxValue ) {
      G4Exception("GmDistributionRatio::GetValueFromTrack",
		  "Too big return value",
		  JustWarning,
		  G4String("Value = "+GmGenUtils::ftoa(indexVal)+ " >= " + GmGenUtils::ftoa(theMaxValue)).c_str());
      indexVal = theMaxValue;
    }
    return indexVal;
  }

}
