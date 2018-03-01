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

#include "SealBase/FileMapping.h"
#include "SealBase/FileMappingError.h"
#include "SealBase/File.h"
#include "SealBase/DebugAids.h"
#include "SealBase/sysapi/FileMapping.h"
#ifndef _WIN32

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

void
FileMapping::initialise (void)
{
# if !defined MAP_ANONYMOUS && !defined MAP_ANON
    if (! m_file)
	m_file = new File ("/dev/zero", IOFlags::OpenRead | IOFlags::OpenWrite);
# endif
}

void *
FileMapping::create (IOOffset pos, IOSize length, int prot, int flags)
{
    ASSERT (pos >= 0);
    ASSERT (! (flags & MapFixed));
    ASSERT (! (flags & MapDenyWrite));
    ASSERT (! (flags & MapExecutable));
    ASSERT ((flags & (MapShared | MapPrivate)) == MapPrivate
	    || (flags & (MapShared | MapPrivate)) == MapShared);

    IOFD fd = m_file ? m_file->fd () : IOFD_INVALID;
    int sysprot = 0;
    int sysflags = 0;

    if (prot & ProtExec)
	sysprot |= PROT_EXEC;
    if (prot & ProtRead)
	sysprot |= PROT_READ;
    if (prot & ProtWrite)
	sysprot |= PROT_WRITE;
    if (prot & ProtNone)
	sysprot |= PROT_NONE;

    if (flags & MapShared)
	sysflags |= MAP_SHARED;
    if (flags & MapPrivate)
	sysflags |= MAP_PRIVATE;

# ifdef MAP_ANONYMOUS
    if (fd == IOFD_INVALID)
	sysflags |= MAP_ANONYMOUS;
# else
    // Must have a real file or /dev/zero opened in initialise().
    ASSERT (fd != IOFD_INVALID);
# endif

    void *address = mmap (0, length, sysprot, sysflags, fd, pos);
    if (address == MAP_FAILED)
	throw FileMappingError ("mmap()", errno);

    View view = { address, 0, length };
    m_views.push_back (view);
    return view.m_address;
}

void
FileMapping::sysunmap (View &view)
{
    ASSERT (! view.m_bias);
    if (munmap ((caddr_t) view.m_address, view.m_size) == -1)
	throw FileMappingError ("munmap()", errno);
}

void
FileMapping::sysclose (void)
{}

void
FileMapping::flush (void *address, IOSize size)
{
    ASSERT (address);
    if (msync ((caddr_t) address, size, MS_SYNC | MS_INVALIDATE) == -1)
	throw FileMappingError ("msync()", errno);
}

} // namespace seal
#endif // !_WIN32