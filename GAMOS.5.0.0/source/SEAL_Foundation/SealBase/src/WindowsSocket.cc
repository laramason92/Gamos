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

#include "SealBase/Socket.h"
#include "SealBase/NetworkError.h"
#include "SealBase/sysapi/Socket.h"
#include "IOSelectWindows.h"
#ifdef _WIN32

namespace seal {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
Socket::select (IOSelectMethod *to, const IOSelectRequest &req)
{
    IOSelectWindows *wto = dynamic_cast<IOSelectWindows *> (to);
    ASSERT (wto);

    // FIXME: provide appropriate selector handler
    wto->attach (req);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
Socket::sysinit (void)
{
    // FIXME: WSACleanup?
    static bool init = false;
    if (! init)
    {
	int	err;
	WSADATA wsaData;

	if ((err = ::WSAStartup (WS_VERSION_REQD, &wsaData)))
	    throw NetworkError ("WSAStartup()", err);

	if ((LOBYTE (wsaData.wVersion) < WS_VERSION_MAJOR)
	    || (LOBYTE (wsaData.wVersion) == WS_VERSION_MAJOR
		&& HIBYTE (wsaData.wVersion) < WS_VERSION_MINOR)
	    || wsaData.iMaxSockets < MIN_SOCKETS_REQD)
	{
	    WSACleanup();
	    throw NetworkError ("WSA version");
	}
	init = true;
    }
}

} // namespace seal
#endif // _WIN32