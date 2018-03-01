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
#ifndef IdealToroidWithRectangularCoil_HH
#define IdealToroidWithRectangularCoil_HH 1 


#include "G4Types.hh"
#include "G4ThreeVector.hh"
#include "G4MagneticField.hh"
#include "G4AffineTransform.hh"

#include <vector>
class IdealToroidWithRectangularCoil: public G4MagneticField
{
  public:  // with description

    IdealToroidWithRectangularCoil( G4double RToroid,G4double Lhoriz,G4double Lvert,G4double RoutCorner,G4double dCoil, G4double I);
    ~IdealToroidWithRectangularCoil() ;

    virtual void GetFieldValue(const G4double yTrack[4],
                                     G4double *MagField) const ;

   
    

  private:
    
    G4double rout_corner,dcoil,current;
    G4double z_max1,z_max2;
    G4double rho_min1,rho_min2;
    G4double rho_max1,rho_max2;
};

#endif
