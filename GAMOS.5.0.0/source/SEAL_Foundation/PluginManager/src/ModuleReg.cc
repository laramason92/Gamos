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

#include "PluginManager/ModuleReg.h"
#include "PluginManager/ModuleDescriptor.h"
#include "PluginManager/Module.h"
#include "SealBase/DebugAids.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>

namespace seal {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

const char ModuleReg::s_tag [] = "file";

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

ModuleReg::ModuleReg (const Filename &file,
		      const std::string &time,
		      const std::string &bad)
    : m_file (file),
      m_time (strtoul (time.c_str (), 0, 10)),
      m_bad (bad == "0")
{ ASSERT (! file.empty ()); ASSERT (! time.empty ()); ASSERT (! bad.empty ()); }

ModuleReg::ModuleReg (const Filename &file, unsigned time, bool bad)
    : m_file (file),
      m_time (time),
      m_bad (bad)
{ ASSERT (! file.empty ()); }

ModuleReg::~ModuleReg (void)
{
    for (Iterator p = begin (); p != end (); ++p)
	delete *p;
}

//////////////////////////////////////////////////////////////////////
void
ModuleReg::add (Module *module)
{ ASSERT (! m_bad); ASSERT (module); m_modules.push_back (module); }

ModuleReg::Iterator
ModuleReg::erase (Iterator pos)
{ return m_modules.erase (pos); }

//////////////////////////////////////////////////////////////////////
/** Dump the contents of the module registration into the cache @a to.
    Writes a #ModuleDescriptor for the file itself and then caches
    info from all the #Module objects contained in this object.  */
void
ModuleReg::cache (ModuleDescriptor *to) const
{
    char time [40]; // 128+ bits
    char bad [2] = { m_bad ? '0' : '1', '\0' };
    sprintf (time, "%lu", (unsigned long) m_time);

    // Ask each module to dump itself under myself.
    ModuleDescriptor *info
	= new ModuleDescriptor (to, s_tag, std::string (m_file),time,bad);
    for (ConstIterator i = m_modules.begin (); i != m_modules.end (); ++i)
	(*i)->cache (info);
}

//////////////////////////////////////////////////////////////////////
/** Check if the module registration has been marked bad.  */
bool
ModuleReg::bad (void) const
{ return m_bad; }

/** Mark the module registration good or bad according to @a value.
    If @a value is @c true, all existing modules are destroyed and
    only a memory of a bad file and it's timestamp is retained.  */
void
ModuleReg::bad (bool value)
{
    if (value)
    {
	for (Iterator p = begin (); p != end (); ++p)
	    delete *p;

	m_modules.clear ();
    }
    m_bad = value;
}

} // namespace seal
