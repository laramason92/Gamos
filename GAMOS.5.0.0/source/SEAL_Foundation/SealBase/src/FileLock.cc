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

// FIXME: doc linux open/O_EXCL docs about broken locking over NFS
//   -> use Filename locking
// FIXME: doc lost locks signals (NFS) cause fatal death: SIGLOST

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

/** @fn bool FileLock::locked (File *file, IOOffset pos, IOOffset length,
			       bool write = true)

    Check if any process currently holds a lock for the specified
    region (@a length may be negative).  If @a write, checks for
    exclusive (write) locks, otherwise checks only for read locks.  */

/** @fn void FileLock::lock (File *file, IOOffset pos, IOOffset length,
    			     bool write = true, bool wait = false)

    Lock the specified region of the @a file (@a length may be negative).
    If @a write, requests an exclusive (write) lock, otherwise a read
    lock.  If @a wait, waits until the region can be locked, otherwise
    fails if someone else already holds a conflicting lock.  If @a
    wait, the wait can be interrupted by a signal (e.g. an alarm)
    (unix only), and may fail if the system detects a dead-lock
    situation (which it may not, even if there is one). */

/** @fn void FileLock:unlock (File *file, IOOffset pos, IOOffset length)

    Unlock the previously locked region of the file.  */

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
FileLock::FileLock (void)
    : m_file (0),
      m_offset (0),
      m_length (0)
{}

FileLock::FileLock (File *file, IOOffset pos, IOOffset len,
		    bool write /* = true */, bool wait /* = false */)
    : m_file (0),
      m_offset (0),
      m_length (0)
{ lock (file, pos, len, write, wait); }

FileLock::~FileLock (void)
{ if (m_file) unlock (m_file, m_offset, m_length); }

/** Unlock the previously locked region of the file.  */
void
FileLock::unlock (void)
{
    ASSERT (m_file);
    unlock (m_file, m_offset, m_length);
    m_offset = m_length = 0;
    m_file = 0;
}

} // namespace seal
