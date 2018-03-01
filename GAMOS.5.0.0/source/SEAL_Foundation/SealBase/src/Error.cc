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
//<<<<<< INCLUDES                                                       >>>>>>

#include "SealBase/Error.h"

namespace seal {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

Error::Error (Error *chain /* = 0 */)
    : m_chain (chain)
{}

/** Copy the exception object @a x.  Makes a deep copy of the chain.
    In general this method needs to be overridden in derived classes
    only if they contain data members that cannot be copied correctly
    trivially.  The derived method does not need to copy the chain,
    just itself.  */
Error::Error (const Error &x)
    : m_chain (x.m_chain ? x.m_chain->clone () : 0)
{}

/** Copy the exception object @a x.  Makes a deep copy of the chain.
    In general this method needs to be overridden in derived classes
    only if they contain data members that cannot be copied correctly
    trivially.  The derived method does not need to copy the chain,
    just itself.  */
Error &
Error::operator= (const Error &x)
{
    if (this != &x)
    {
	delete m_chain;
	m_chain = 0;

	if (x.m_chain)
	    m_chain = x.m_chain->clone ();
    }

    return *this;
}

Error::~Error (void)
{ delete m_chain; }

/** Describe the reason for the error as text.  Unravels the chain for
    up to @a depth levels in addition to this error object, or the
    full chain if @a depth is negative.  */
std::string
Error::explain (int depth /* = -1 */) const
{
    std::string result (explainSelf ());
    Error	*chain = next ();

    if (chain && depth < 0 || depth > 0)
    {
	result += " (because of ";
   	result += chain->explain (depth < 0 ? depth : depth-1);
       	result += ")";
    }

    return result;
}

} // namespace seal
