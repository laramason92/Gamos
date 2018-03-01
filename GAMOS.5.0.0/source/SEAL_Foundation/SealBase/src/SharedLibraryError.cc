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

#include "SealBase/SharedLibraryError.h"
#include "SealBase/SystemError.h"

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

SharedLibraryError::SharedLibraryError (const char *context,
					const std::string &cause)
    : m_context (context ? context : ""),
      m_cause (cause)
{}

SharedLibraryError::SharedLibraryError (const char *context, int code)
    : Error (new SystemError (code)),
      m_context (context ? context : "")
{}

SharedLibraryError::SharedLibraryError (const char *context, Error *next)
    : Error (next),
      m_context (context ? context : "")
{}

SharedLibraryError::SharedLibraryError (Error *next)
    : Error (next)
{}

std::string
SharedLibraryError::explainSelf (void) const
{
    std::string message = "Shared library operation";
    if (! m_context.empty ())
    {
	message += " ";
	message += m_context;
    }

    message += " failed";

    if (! m_cause.empty ())
    {
	message += " because: ";
	message += m_cause;
    }

    return message;
}

Error *
SharedLibraryError::clone (void) const
{ return new SharedLibraryError (*this); }

void
SharedLibraryError::rethrow (void)
{ throw *this; }

} // namespace seal
