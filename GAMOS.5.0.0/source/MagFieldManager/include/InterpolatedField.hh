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
#ifndef InterpolatedField_HH
#define InterpolatedField_HH 1 


#include "G4Types.hh"
#include "G4ThreeVector.hh"
#include "G4MagneticField.hh"
#include "globals.hh"
#include "vector"
class BiotSavartIntegrator;

class InterpolatedField: public G4MagneticField
{
  public:  // with description

    InterpolatedField(G4String file_name);
    ~InterpolatedField();

    virtual void GetFieldValue(const G4double yTrack[4],
                                     G4double *MagField) const ;      
  private:
    
     void Read3DGrid(G4String file_name);
    
  
  private:
     G4bool IsCartesian;
  
    
    
    //Matrices for  interpolation
    //////////////////////////////
	
     std::vector< std::vector< std::vector<float> > > B1_grid3D; //X or rho component
     std::vector< std::vector< std::vector<float> > > B2_grid3D; //Y or phi component
     std::vector< std::vector< std::vector<float> > > B3_grid3D; //Z component
     G4double xmin,xmax,ymin,ymax,zmin,zmax,dx,dy,dz;
     G4double rho_min,rho_max,drho;
     G4double phi1,phi2,dphi;
     
    

};

#endif

