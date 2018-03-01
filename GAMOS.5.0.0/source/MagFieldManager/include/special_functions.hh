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
#ifndef SPECFUNC_h
#define SPECFUNC_h 1
#include <fstream>
#include <complex>
#include <vector>



namespace special_functions{
	double rf(double x, double y, double z);
	double rd(double x, double y, double z);
	double CompleteEllipticIntegral1stKindK(double m);
	double CompleteEllipticIntegral2ndKindE(double m);
	void CompleteEllipticIntegrals(double m, double& K, double& E);
	void bfield_from_IZ_line(double r[3],double o[3],double l,  double b[3]);
	void bfield_from_IX_line(double r[3],double o[3],double l,  double b[3]);
	void bfield_from_IY_line(double r[3],double o[3],double l,  double b[3]);
	void bx_from_IYZ_surface(double r[3],double o[3],double ly,double lz,  double& bx);
	void by_from_IXZ_surface(double r[3],double o[3],double lx,double lz,  double& by);
	

}
#endif
