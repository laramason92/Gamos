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
#ifndef RacetrackCoil_HH
#define RacetrackCoil_HH 1 


#include "G4Types.hh"
#include "G4ThreeVector.hh"
#include "G4MagneticField.hh"
class BiotSavartIntegrator;

class RacetrackCoil: public G4MagneticField
{
  public:  // with description

    RacetrackCoil(G4double Rout,G4double D1, G4double D2,G4double L1,G4double L2,G4double current);
    ~RacetrackCoil();

    virtual void GetFieldValue(const G4double yTrack[4],
                                     G4double *MagField) const ;
    
    inline void SetRout(G4double aVal) {rout= aVal;}
    inline void SetD1(G4double aVal) {d1= aVal;}
    inline void SetD2(G4double aVal) {d2= aVal;}
    inline void SetL1(G4double aVal) {l1= aVal;}
    inline void SetL2(G4double aVal) {l2= aVal;}
    
    

  private:
    G4double d1,d2, I;
    G4double l1,l2,rout,rin;
    BiotSavartIntegrator* theBiotSavartIntegrator;
    G4double y1,y2,z2,z3,y4,z4,y5,y6,z6,z7,y8,z8; //y and z positions of the centers of the different sections
    
    

    
   
  
  
};

#endif

