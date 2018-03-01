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
NamedPipeServer::sysinit (const char *name, FileAcl access)
{
    // FIXME: #if HAVE_MKFIFO
    if (mkfifo (name, access.native ()) == -1)
	throw FileSystemError ("mkfifo()", errno);

    // FIXME: #elif HAVE_MKNOD
    // if (mknod (name, access | S_IFIFO, 0) == -1)
    //   throw FileSystemError ("mknod()", errno);

    // FIXME: #else what?
}

void
NamedPipeServer::sysclose (void)
{
    if (unlink (m_name.c_str ()) < 0)
	throw FileSystemError ("unlink()", errno);
}

void
NamedPipeServer::sysdetach (void)
{}

} // namespace seal
#endif // !_WIN32
