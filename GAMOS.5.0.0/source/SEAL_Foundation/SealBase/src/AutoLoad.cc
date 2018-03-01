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

#include "SealBase/AutoLoad.h"

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

AutoLoadLib::AutoLoadLib (const char *libname)
    : m_libname (libname),
      m_library (0)
{}

bool
AutoLoadLib::exists (void)
{
    try
    {
	return library ();
    }
    catch (AutoLoadError &)
    {
	return false;
    }
}

SharedLibrary *
AutoLoadLib::library (void)
{
    try
    {
	if (! m_library)
	    m_library = SharedLibrary::load (SharedLibrary::libname (m_libname));

	return m_library;
    }
    catch (Error &e)
    {
	throw AutoLoadError (m_libname, 0, e.clone ());
    }
}

SharedLibrary::Function
AutoLoadLib::function (const char *symname)
{
    SharedLibrary		*lib = library ();
    SharedLibrary::Function	entry = 0;

    try
    {
	entry = lib->function (symname);
    }
    catch (Error &e)
    {
	throw AutoLoadError (m_libname, symname, e.clone ());
    }

    if (! entry)
	throw AutoLoadError (m_libname, symname);

    return entry;
}

SharedLibrary::Data
AutoLoadLib::data (const char *symname)
{
    SharedLibrary		*lib = library ();
    SharedLibrary::Data		entry = 0;
    try
    {
	entry = lib->data (symname);
    }
    catch (Error &e)
    {
	throw AutoLoadError (m_libname, symname, e.clone ());
    }

    if (! entry)
	throw AutoLoadError (m_libname, symname);

    return entry;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

AutoLoadError::AutoLoadError (const char *lib, const char *sym, Error *next /* = 0 */)
    : Error (next),
      m_libname (lib),
      m_symname (sym)
{}

std::string
AutoLoadError::explainSelf (void) const
{
    return (std::string ("Failed to auto-load ")
	    + (m_symname ? std::string ("'") + m_symname + "' from library " : "")
	    + m_libname);
}

Error *
AutoLoadError::clone (void) const
{ return new AutoLoadError (*this); }

void
AutoLoadError::rethrow (void)
{ throw *this; }

} // namespace seal
