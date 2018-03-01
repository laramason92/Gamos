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

#include "RTCutsStudyFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "G4ThreeVector.hh"
#include "G4Step.hh"

//----------------------------------------------------------------
RTCutsStudyFilter::RTCutsStudyFilter(const G4String& name) : GmVFilter(name)
{ 
  thePlaneZ = GmParameterMgr::GetInstance()->GetNumericValue("RTCutsStudyFilter:PlaneZ",100.*cm);
  thePlaneXDim = GmParameterMgr::GetInstance()->GetNumericValue("RTCutsStudyFilter:PlaneXDim",100.*cm);
  thePlaneYDim = GmParameterMgr::GetInstance()->GetNumericValue("RTCutsStudyFilter:PlaneYDim",100.*cm);

}


//----------------------------------------------------------------
G4bool RTCutsStudyFilter::AcceptStep(const G4Step* aStep)
{
  G4ThreeVector prePos = aStep->GetPreStepPoint()->GetPosition();
  G4ThreeVector postPos = aStep->GetPostStepPoint()->GetPosition();
  if( ( (postPos.z() - thePlaneZ) == 0. || 
      (prePos.z() - thePlaneZ) * (postPos.z() - thePlaneZ) < 0. ) && 
      fabs(postPos.x()) < thePlaneXDim && 
      fabs(postPos.y()) < thePlaneYDim ) {
    //  G4cout << " RTCutsStudyFilter: track accepted " << prePos << " " << postPos << G4endl;
    return true; 
  } else {
    //  G4cout << " track not accepted " << prePos << " " << postPos << " thePlaneZ " << thePlaneZ << " thePlaneYDim " << thePlaneYDim << " thePlaneXDim " << thePlaneXDim << G4endl;
    return false;
  }
}
