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
#ifndef MagneticDipole_HH
#define MagneticDipole_HH 1 
#include "G4Types.hh"
#include "G4ThreeVector.hh"
#include "G4MagneticField.hh"

class MagneticDipole: public G4MagneticField
{
  public:  // with description

    MagneticDipole(const G4ThreeVector& aDipoleMoment, const G4ThreeVector& aPos);
    ~MagneticDipole() ;

    virtual void GetFieldValue(const G4double yTrack[4],
                                     G4double *MagField) const ;

    inline void SetDipoleMoment(const G4ThreeVector& aDipoleMoment) {theDipoleMoment= aDipoleMoment;}
    G4ThreeVector GetDipoleMoment(){return theDipoleMoment;}
    inline void SetDipoleCenter(const G4ThreeVector& aPos) {theDipoleCenter= aPos;}
    G4ThreeVector GetDipoleCenter(){return theDipoleCenter;}
    
    
    

  private:

    G4ThreeVector theDipoleMoment;
    G4ThreeVector theDipoleCenter;
};

#endif
