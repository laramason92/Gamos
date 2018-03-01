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
#ifndef CircularCoil_HH
#define CircularCoil_HH 1 
#include "G4Types.hh"
#include "G4ThreeVector.hh"
#include "G4MagneticField.hh"

class CircularCoil: public G4MagneticField
{
  public:  // with description

    CircularCoil(G4double R,G4double dr, G4double dz,G4double current);
    ~CircularCoil();

    virtual void GetFieldValue(const G4double yTrack[4],
                                     G4double *MagField) const ;

    inline void SetRadius(G4double aRad) {Radius= aRad;}
    inline void SetDR(G4double aVal) {DR= aVal;}
    inline void SetDZ(G4double aVal) {DZ= aVal;}
    inline void SetI(G4double aVal) {I= aVal;}
    
    
    

  private:
    G4double Radius;
    G4double DR,DZ;
    G4double I;
    
    void BCurrentloop(G4double a, G4double i, G4double rho, G4double z, G4double& Brho, G4double& Bz) const;
    void SumOfLoops(G4int nr, G4int nz,G4double rho, G4double z, G4double& Brho, G4double& Bz) const;
  
  
};

#endif
