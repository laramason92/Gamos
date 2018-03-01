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
#ifndef MAGCOSEQUATIONOFMOTION
#define MAGCOSEQUATIONOFMOTION 
#include "G4Mag_EqRhs.hh"
#include "G4MagneticField.hh"
#include "G4String.hh"

// DESCRIPTION
// -----------
//
// This class defines the equation of motion used in the simulation. 
// 
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// PUBLIC MEMBER FUNCTIONS
// -----------------------
//
// MagneticEquationOfMotion(G4MagneticField* MagField )
//    Constructor.
//
// ~MagneticEquationOfMotion()
//    Destructor.
//

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// CHANGE HISTORY
// --------------
//
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
////////////////////////////////////////////////////////////////////////////////
//

class MagneticEquationOfMotion : public G4Mag_EqRhs
{
   public:  // with description

     // Constructor and destructor.
     MagneticEquationOfMotion( G4MagneticField* MagField );
    ~MagneticEquationOfMotion() {;}
    
     // Given the value of the magnetic field B, this function 
     // calculates the value of the derivative dydx.

     void EvaluateRhsGivenB( const G4double y[],
			     const G4double B[3],
				   G4double dydx[] ) const;
     
     // this is a pointer on one of the equation of motion methods
     
     void (MagneticEquationOfMotion::*Equation)( const G4double y[],
			                       const G4double B[3],
				               G4double dydx[] ) const;	
				   
     //Lorentz equation of motion				   
     void LorentzMotion( const G4double y[],
			  const G4double B[3],
		          G4double dydx[] ) const;

     //Equation for tracing magnetic field line			  
     void MotionAlongBfieldLine( const G4double y[],
			         const G4double B[3],
				 G4double dydx[]) const;			   			   		   
     
     //Set methods
     
     void SetReverseTimeMode(G4bool abool);  
     void SetEquationType(G4String aString);
     
    
     
    private:
     G4bool reverse_time;
     G4double direction;
     
     
};

#endif 
