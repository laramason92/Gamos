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

#include "SealBase/FileLock.h"
#include "SealBase/File.h"
#include "SealBase/FileError.h"
#include "SealBase/DebugAids.h"
#include "SealBase/sysapi/FileLock.h"
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

bool
FileLock::locked (File *file,
		  IOOffset pos,
		  IOOffset length,
		  bool write /* = true */)
{
    IOFD fd = file->fd ();
    ASSERT (fd != IOFD_INVALID);
    ASSERT (pos + length >= 0);

    struct flock desc;
    desc.l_type		= write ? F_WRLCK : F_RDLCK;
    desc.l_whence	= SEEK_SET;
    desc.l_start	= pos;
    desc.l_len		= length;
    if (::fcntl (fd, F_GETLK, &desc) == -1)
	throw FileError ("fcntl()", errno);

    // FIXME: return l_pid, l_start, l_len of the whole lock?
    return desc.l_type == F_UNLCK;
}

void
FileLock::lock (File *file,
		IOOffset pos,
		IOOffset length,
		bool write /* = true */,
		bool wait /* = false */)
{
    ASSERT (file);
    IOFD fd = file->fd ();
    ASSERT (fd != IOFD_INVALID);
    ASSERT (pos + length >= 0);

    ASSERT (! m_file);
    ASSERT (! m_offset);
    ASSERT (! m_length);

    m_file = file;
    m_offset = pos;
    m_length = length;

    struct flock desc;
    desc.l_type		= write ? F_WRLCK : F_RDLCK;
    desc.l_whence	= SEEK_SET;
    desc.l_start	= pos;
    desc.l_len		= length;
    if (::fcntl (fd, wait ? F_SETLK : F_SETLKW, &desc) == -1)
	throw FileError ("fcntl()", errno);
}

void
FileLock::unlock (File *file, IOOffset pos, IOOffset length)
{
    ASSERT (file);
    IOFD fd = file->fd ();
    ASSERT (fd != IOFD_INVALID);
    ASSERT (pos + length >= 0);

    struct flock desc;
    desc.l_type		= F_UNLCK;
    desc.l_whence	= SEEK_SET;
    desc.l_start	= pos;
    desc.l_len		= length;
    if (::fcntl (fd, F_SETLK, &desc) == -1)
	throw FileError ("fcntl()", errno);
}

} // namespace seal
#endif // _WIN32
