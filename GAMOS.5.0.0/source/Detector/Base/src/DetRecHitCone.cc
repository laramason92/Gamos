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
#include "DetRecHitCone.hh"
#include "DetVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//----------------------------------------------------------------------
DetRecHitCone::DetRecHitCone(const GmRecHit* rhit1, GmRecHit* rhit2 )
{
  thePoint = rhit1->GetPosition();
  theDir = rhit2->GetPosition() - rhit1->GetPosition();
  theDir = theDir.unit();

  // theAngle = acos( (rhit2->GetEnergy() - rhit1->GetEnergy() ) / rhit2->GetEnergy() );

  // cos (theAngle) = 1 - mc2 * (Etracker) / ((Ecalor)*(Etracker+Ecalor))
  // mc2 = electron rest mass equivalent energy (=511 keV)
  // PE absorption in 2 is assumed, so that:
  // total Egamma (initial photon) = Etracker(Compton)+Ecalor(PE)

  //  G4double ener1 = rhit1->GetEnergy();
  G4double ener2 = rhit2->GetEnergy();
  // assume first energy is 511 keV
  G4double firstEner = GmParameterMgr::GetInstance()->GetNumericValue("DetRecHitCone:InitialHitEnergy",electron_mass_c2);
  //  theAngle = acos (1.- electron_mass_c2 * (firstEner-ener2) / ( ener2 * firstEner ) ); 
  theAngle = acos (1.- (firstEner-ener2) / ener2 ); 
  
#ifndef GAMOS_NO_VERBOSE
  if( DetVerb(infoVerb) ) G4cout << " Created DetRecHitCone pnt " << thePoint << " dir " << theDir << " angle " << theAngle/deg << G4endl;
#endif
#ifndef GAMOS_NO_VERBOSE
  if( DetVerb(infoVerb) ) G4cout << " 1 cone " << *rhit1 << " 2 cone " << *rhit2 << G4endl;
#endif
  //-  G4cout << " angle " << ener1 / ( ener2 * (ener1 + ener2) ) << "  " << ener1 << " " << ( ener2 * (ener1 + ener2) ) << G4endl;

}
