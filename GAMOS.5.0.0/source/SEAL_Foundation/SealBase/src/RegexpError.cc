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

#include "SealBase/RegexpError.h"
#include "SealBase/Regexp.h"
#include "SealBase/BitTraits.h"
#include <cstdio>

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

RegexpError::RegexpError (const char *context, int syntax, int error)
    : m_context (context),
      m_syntax (syntax),
      m_error (error)
{}

RegexpError::RegexpError (const char *context, int syntax, const char *msg)
    : m_context (context),
      m_syntax (syntax),
      m_error (0),
      m_message (msg ? msg : "")
{}

std::string
RegexpError::explainSelf (void) const
{
    std::string message;

    switch (m_syntax)
    {
    case Regexp::Perl:
	message += "PERL";
	break;

    case Regexp::POSIXBasic:
	message += "POSIX basic";
	break;

    case Regexp::POSIXExtended:
	message += "POSIX extended";
	break;

    default:
	message += "Unknown";
	break;
    }

    message += " regular expression failed";
    if (m_context)
    {
	message += " in ";
	message += m_context;
    }

    if (m_message.length ())
    {
	message += " with error message: ";
	message += m_message;
    }
    else
    {
	message += " with error code ";
	char buf [BitTraits<int>::Digits + 1];
	sprintf (buf, "%d", m_error);
	message += buf;
    }

    return message;
}

Error *
RegexpError::clone (void) const
{ return new RegexpError (*this); }

void
RegexpError::rethrow (void)
{ throw *this; }

} // namespace seal
