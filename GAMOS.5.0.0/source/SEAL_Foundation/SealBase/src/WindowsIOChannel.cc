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

#include "SealBase/IOChannel.h"
#include "SealBase/IOError.h"
#include "SealBase/DebugAids.h"
#include "SealBase/sysapi/IOChannel.h"
#include "IOSelectWindows.h"
#ifdef _WIN32

namespace seal {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IOSize
IOChannel::read (void *into, IOSize n)
{
    DWORD read;
    if (! ReadFile (fd (), into, n, &read, 0))
	throw IOError ("ReadFile()", GetLastError ());

    return read;
}

IOSize
IOChannel::readv (IOBuffer *into, IOSize buffers)
{ return IOInput::readv (into, buffers); } // FIXME: WIN32 implementation?

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
IOSize
IOChannel::write (const void *from, IOSize n)
{
    DWORD written;
    if (! WriteFile (fd (), from, n, &written, 0))
	throw IOError ("WriteFile()", GetLastError ());

    return written;
}

IOSize
IOChannel::writev (const IOBuffer *from, IOSize buffers)
{ return IOOutput::writev (from, buffers); } // FIXME: WIN32 implementation?

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
IOChannel::setBlocking (bool value)
{
    // FIXME: How about pipes etc?
    unsigned long mode = value;
    if (ioctlsocket ((SOCKET) fd (), FIONBIO, &mode) == -1)
	throw IOError ("ioctlsocket()", errno);
}

bool
IOChannel::isBlocking (void) const
{
    // FIXME
    return true;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
IOChannel::select (IOSelectMethod *to, const IOSelectRequest &req)
{
    IOSelectWindows *wto = dynamic_cast<IOSelectWindows *> (to);
    ASSERT (wto);

    // FIXME: check file handle type and provide appropriate selector
    // handler
    wto->attach (req);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
bool
IOChannel::sysclose (IOFD fd, int *error /* = 0 */)
{
    bool ret = CloseHandle (fd);
    if (error) *error = GetLastError ();
    return ret;
}

} // namespace seal
#endif // _WIN32
