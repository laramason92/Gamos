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

#include "CPETLine.h"
#include "CUserParameters.h"

// #include "CUniformRandomGenerator.h"
// #include "CLHEP/Random/RandGauss.h"

#include "CLHEP/Random/RandFlat.h"

#include <iostream>
#include <cmath>


CPETLine::CPETLine()
{
}

CPETLine::~CPETLine()
{
}

// copy constructor
CPETLine::CPETLine(const CPETLine& in_obj)
{
	*this = in_obj;
}

// assignment operator
CPETLine&
CPETLine::operator= (const CPETLine& in_obj)
{
	if (this != &in_obj)
	{
		// copy member data
	}
	return *this;
}

void
CPETLine::GetRandomPositionOnLine(  const C3Vector& in_hitPosition1, const C3Vector& in_hitPosition2
				, C3Vector& io_positionOnLine) //  const
{
	double x1 = in_hitPosition1.GetX();
	double y1 = in_hitPosition1.GetY();
	double z1 = in_hitPosition1.GetZ();

	double x2 = in_hitPosition2.GetX();
	double y2 = in_hitPosition2.GetY();
	double z2 = in_hitPosition2.GetZ();

	// TODO: only select inside FOV!!!!!!!!!!!!

	if ( CUserParameters::Instance()->GetDoLimitRange() )
	{
		x1 = x1 > x2 ? fmin(x1, CUserParameters::Instance()->GetUpperX()) : fmax(x1, CUserParameters::Instance()->GetLowerX());
		x2 = x2 > x1 ? fmin(x2, CUserParameters::Instance()->GetUpperX()) : fmax(x2, CUserParameters::Instance()->GetLowerX());

		y1 = y1 > y2 ? fmin(y1, CUserParameters::Instance()->GetUpperY()) : fmax(y1, CUserParameters::Instance()->GetLowerY());
		y2 = y2 > y1 ? fmin(y2, CUserParameters::Instance()->GetUpperY()) : fmax(y2, CUserParameters::Instance()->GetLowerY());

		z1 = z1 > z2 ? fmin(z1, CUserParameters::Instance()->GetUpperZ()) : fmax(z1, CUserParameters::Instance()->GetLowerZ());
		z2 = z2 > z1 ? fmin(z2, CUserParameters::Instance()->GetUpperZ()) : fmax(z2, CUserParameters::Instance()->GetLowerZ());
	}

	C3Vector LORVec( x2 - x1, y2 - y1, z2 - z1 );

	double factor = CLHEP::RandFlat::shoot(1.0);
	LORVec = LORVec * factor;
	
	io_positionOnLine.Set(  x1 + LORVec.GetX()
						  , y1 + LORVec.GetY()
						  , z1 + LORVec.GetZ() );
}

bool
CPETLine::operator==(const CPETLine& in_obj) const
{
	return 1;
}

bool
CPETLine::operator!=(const CPETLine& in_obj) const
{
	return !(*this == in_obj);
}

// (de)serialization
void
CPETLine::Serialize( std::ostream& io_outstream ) const
{
}

void
CPETLine::Deserialize( std::istream& in_instream )
{
}



