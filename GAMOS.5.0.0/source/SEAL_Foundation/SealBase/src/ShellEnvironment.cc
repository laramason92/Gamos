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

#include "SealBase/ShellEnvironment.h"
#include <cstdlib>

namespace seal {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>

#if !HAVE_UNSETENV
extern "C" { extern char **environ; }
#endif

//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

bool
ShellEnvironment::has (const std::string &name) const
{ return getenv (name.c_str ()); }

std::string
ShellEnvironment::get (const std::string &name, bool *have /* = 0 */) const
{
    std::string value;
    if (const char *val = getenv (name.c_str ()))
    {
	value = val;
	if (have) *have = true;
    }
    else
    {
	value = "";
	if (have) *have = false;
    }

    return value;
}


void
ShellEnvironment::unset (const std::string &name)
{
#if HAVE_UNSETENV
    unsetenv (name.c_str ());
#else
    char **val;
    char **out;
    for (val = out = environ; *val; ++val)
	if (strncmp (*val, name.c_str (), name.size ())
	    || (*val) [name.size ()] != '=')
	    *out++ = *val;

    *out++ = 0;
#endif
}

void
ShellEnvironment::set (const std::string &name, const std::string &value)
{
#if HAVE_SETENV
    setenv (name.c_str (), value.c_str (), true);
#else
    char *val = (char *) malloc (name.size () + 1 + value.size () + 1);
    strcpy (val, name.c_str ());
    strcat (val, "=");
    strcat (val, value.c_str ());
    putenv (val);
#endif
}

} // namespace seal
