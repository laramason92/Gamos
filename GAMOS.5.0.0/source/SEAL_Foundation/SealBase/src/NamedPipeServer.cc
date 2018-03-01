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
#include "SealBase/DebugAids.h"
#include "SealBase/sysapi/NamedPipeServer.h"

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

/** Create a new named pipe object without a pipe attached to it.  */
NamedPipeServer::NamedPipeServer (void)
    : m_handle (IOFD_INVALID)
{}

/** Create a new named pipe.  The pipe will be give access permissions
    @a access.  */
NamedPipeServer::NamedPipeServer (const std::string &name, FileAcl access /* = 0666 */)
    : m_handle (IOFD_INVALID)
{ create (name.c_str (), access); }

/** Create a new named pipe.  The pipe will be give access permissions
    @a access.  */
NamedPipeServer::NamedPipeServer (const char *name, FileAcl access /* = 0666 */)
    : m_handle (IOFD_INVALID)
{ create (name, access); }

/** Destroy the named pipe object.  The client must have already
    called either #close() or #detach().  */
NamedPipeServer::~NamedPipeServer (void)
{ ASSERT (m_name.empty ()); ASSERT (m_handle == IOFD_INVALID); }

/** Create a new named pipe in the operating system.  Does not yet
    connect to it.  Redirected to the overloaded method taking a
    "const char *" name.  (FIXME: Can windows have named pipes that
    nobody is connected to?) */
void
NamedPipeServer::create (const std::string &name, FileAcl access /* = 0666 */)
{ create (name.c_str (), access); }

/** Create a new named pipe in the operating system.  Does not yet
    connect to it.  (FIXME: Can windows have named pipes that nobody
    is connected to?) */
void
NamedPipeServer::create (const char *name, FileAcl access /* = 0666 */)
{
    ASSERT (name && *name);
    close ();
    sysinit (name, access);
    m_name = name;
}

void
NamedPipeServer::close (void)
{
    if (! m_name.empty ())
    {
	sysclose ();
	m_name = "";
    }
}

void
NamedPipeServer::detach (void)
{
    ASSERT (! m_name.empty ());
    sysdetach ();
    m_name = "";
}

} // namespace seal
