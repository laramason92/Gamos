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

#include "SealBase/NamedPipeServer.h"
#include "SealBase/FileSystemError.h"
#include "SealBase/DebugAids.h"
#include "SealBase/sysapi/NamedPipeServer.h"
#ifdef _WIN32

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
NamedPipeServer::sysinit (const char *name, FileAcl access)
{
    // FIXME: CreateNamedPipeServer -- gets handle
    // FIXME: non-blocking open needs CreateNamedPipeServer, not File!
    //   --> this needs to be a IOChannel of its own; required for
    //       IOSelector support anyway, but need to separate
    //       NamedPipeServerObject and NamedPipeServerChannel from each other
    static const int PIPE_BUF_SIZE = 4096; // Size reserved for buffers -- unix like
    if ((m_handle = CreateNamedPipe (name,
				     PIPE_ACCESS_DUPLEX
				     // | FILE_FLAG_OVERLAPPED (FIXME?)
				     // | FILE_FLAG_WRITE_THROUGH (FIXME?)
				     | FILE_FLAG_FIRST_PIPE_INSTANCE,
				     PIPE_TYPE_BYTE
				     // | PIPE_NOWAIT (for immediate *open*)
				     | PIPE_READMODE_BYTE,
				     PIPE_UNLIMITED_INSTANCES,
				     PIPE_BUF_SIZE, PIPE_BUF_SIZE,
				     0 /* default time out */,
				     0 /* FIXME security attrs */))
	== IOFD_INVALID)
	throw FileSystemError ("CreateNamedPipeServer()", GetLastError ());
}

void
NamedPipeServer::sysclose (void)
{
    // FIXME: Destroy pipe object -- just CloseHandle()?
    CloseHandle (m_handle);
    m_handle = IOFD_INVALID;
}

void
NamedPipeServer::sysdetach (void)
{ m_handle = IOFD_INVALID; }

} // namespace seal
#endif // _WIN32
