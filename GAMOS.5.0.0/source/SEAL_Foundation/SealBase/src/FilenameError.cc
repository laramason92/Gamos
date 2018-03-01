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

#include "SealBase/FilenameError.h"
#include "SealBase/DebugAids.h"

namespace seal {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Construct a filename expansion error.  */
FilenameError::FilenameError (Reason cause, const std::string &spec /* = "" */)
    : m_reason (cause),
      m_spec (spec)
{}

/** Return the cause for this error.  */
FilenameError::Reason
FilenameError::reason (void) const
{ return m_reason; }

/** Return the variable reference that triggered this error.  For
    #BAD_SYNTAX the value is empty.  */
const std::string &
FilenameError::variable (void) const
{ return m_spec; }

/** Return a printable explanation describing this error.  */
std::string
FilenameError::explainSelf (void) const
{
    switch (m_reason)
    {
    case UNTERMINATED_VAR:
	return std::string ("no closing `}' in variable reference `")
	    + m_spec + "'";

    case BAD_SYNTAX:
	return std::string ("`$' not followed by a variable name or `$'");

    case NO_SUCH_VAR:
	return std::string ("variable `")
	    + m_spec + "' referenced but has no value";

    default:
	ASSERT (false);
	return "oops, internal error -- unknown reason";
    }
}

/** @reimp  */
Error *
FilenameError::clone (void) const
{ return new FilenameError (*this); }

/** @reimp */
void
FilenameError::rethrow (void)
{ throw *this; }

} // namespace seal
