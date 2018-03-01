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

#include "CHit.h"
#include "constants.h"

CHit::CHit()
	: m_e(0.0)
	, m_position(0.0, 0.0, 0.0)
{
}

CHit::CHit(const C3Vector& in_position, const double& in_e)
	: m_e(in_e)
	, m_position(in_position)
{
}

CHit::~CHit()
{
}

// copy constructor
CHit::CHit(const CHit& in_obj)
{
	*this = in_obj;
}

// assignment operator
CHit&
CHit::operator= (const CHit& in_obj)
{
	if (this != &in_obj)
	{
        m_e = in_obj.GetE();
        m_position = in_obj.GetPosition();
	}
	return *this;
}

void
CHit::Set(const C3Vector& in_position, const double& in_e)
{
	m_e = in_e;
	m_position = in_position;
}

bool
CHit::operator==(const CHit &in_obj) const
{
	bool result = (
			    doubleEquals(m_e, in_obj.GetE(), 0.0001)
		    && 	m_position == in_obj.GetPosition()
				  );

	return result;
}

bool
CHit::operator!=(const CHit &in_obj) const
{
    return !(*this == in_obj);
}

// (De)Serialization
void
CHit::Serialize( std::ostream& io_outstream ) const
{
	// ascii
	// in_outstream << m_e << " " << m_position.GetX() << " " << m_position.GetY() << " " << m_position.GetZ() << " " << m_detectorName << " ";

	// binary
	io_outstream.write((char*) &m_e, sizeof(m_e));
	m_position.Serialize(io_outstream);
	//	WriteString(io_outstream, m_detectorName);
}

void
// binary_iarchive does not work with const std::ifstream
// C3Vector::Deserialize( const std::ifstream& in_instream )
CHit::Deserialize( std::istream& io_instream )
{
	// ascii
	// double x, y, z;
	// in_instream >> m_e >> x >> y >> z >> m_detectorName;
	// m_position.Set(x, y, z);

	// binary
	io_instream.read((char*) &m_e, sizeof(m_e));
	m_position.Deserialize( io_instream );
	//	ReadString( io_instream, m_detectorName);
}




