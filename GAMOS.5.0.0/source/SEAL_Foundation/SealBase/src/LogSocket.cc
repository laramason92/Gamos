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

#include "SealBase/Log.h"
#include "SealBase/LocalSocket.h"
#include "SealBase/InetSocket.h"
#include <cctype>
#include <cstdio>
#if HAVE_NEW
# include <new>
#elif HAVE_NEW_H
# include <new.h>
#endif
#include <cstring>

namespace seal {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>

/** Logging device that writes all output to a UNIX domain socket.  */
class SocketLog : public logstream
{
public:
    SocketLog (void);
    ~SocketLog (void);

    logstream *		initialise (bool local, bool tcp, const char *args);

protected:
    virtual void	emit (const char *psz, int n);

private:
    void		close (void);

    Socket		*m_socket;
#if HAVE_LOCAL_SOCKETS
    LocalSocket		m_local_socket;	//< The socket.
#endif
    InetSocket		m_inet_socket;

    static const int	s_default_port;
    static const char	s_default_host [];
    static const char	s_default_hostspec [];
    static const char	s_default_namespec [];
};

//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

const int SocketLog::s_default_port = 7852;
const char SocketLog::s_default_host [] = "localhost";
const char SocketLog::s_default_hostspec [] = "localhost.7852";
const char SocketLog::s_default_namespec [] = "debug.log";

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

logstream *
logstream::createTcpSocketLogDevice (const char *args)
{ return createSocketLogDevice (false, true, args); }

logstream *
logstream::createUdpSocketLogDevice (const char *args)
{ return createSocketLogDevice (false, false, args); }

logstream *
logstream::createLocalSocketLogDevice (const char *args)
{ return createSocketLogDevice (true, false, args); }

logstream *
logstream::createSocketLogDevice (bool local, bool tcp, const char *args)
{
    // make sure the log stays around beyond destructors
    static char		data[sizeof(SocketLog)];
    static SocketLog	*log = new (&data[0]) SocketLog;
    return log->initialise (local, tcp, args);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/** Initialise an empty socket log device.  */
SocketLog::SocketLog (void)
    : m_socket (0)
{}

/** Actually initialise the socket lock.  If @a local, create a local
    (UNIX) socket.  If not, create an internet socket; @a tcp
    indicates whether it should be stream or datagram socket.  */

logstream *
SocketLog::initialise (bool local, bool tcp, const char *args)
{
    ASSERT (! m_socket);

    char	host [64]; // FIXME: MAXHOSTNAMELEN?
    int		port = s_default_port;

    if (! *args)
    {
	if (local)
	    args = s_default_namespec;
	else
	    args = s_default_hostspec;
    }

    // Parse connection specification.  It can be either of these forms:
    //  !local: host.domain[.port]
    //  local: /path/to/socket
    if (! local)
    {
	STDC::size_t	length = strlen (args);
	const char	*end = args + length;
	const char	*portspec = end;
	while (portspec > args
	       && isdigit ((unsigned char) portspec[-1])
	       && portspec[-1] != '.')
	    --portspec;

	if (portspec < end)
	{
	    end = portspec-1;
	    sscanf (portspec, "%d", &port);
	}

	ASSERT (end >= args);
	if (STDC::size_t (end - args) >= sizeof (host))
	    return 0;
	else if (end - args == 0)
	{
	    ASSERT (sizeof (host) < sizeof (s_default_host));
	    strcpy (host, s_default_host);
	}
	else
	{
	    strncpy (host, args, end - args);
	    host [end - args] = 0;
	}

	try 
	{
	    m_inet_socket.create (tcp
				  ? SocketConst::TypeStream
				  : SocketConst::TypeDatagram);
	    m_inet_socket.connect (InetAddress (host, port));
	    m_socket = &m_inet_socket;
	}
	catch (...)
	{
	    return 0;
	}
    }
    else
    {
#if HAVE_LOCAL_SOCKETS
	try
	{
	    m_local_socket.create ();
	    m_local_socket.connect (args);
	    m_socket = &m_local_socket;
	}
	catch (...)
	{
	    return 0;
	}
#else // ! HAVE_LOCAL_SOCKETS
	return 0;
#endif // HAVE_LOCAL_SOCKETS
    }

    return this;
}

/** Flush and close the log. */
SocketLog::~SocketLog (void)
{
    flush ();
    if (m_socket)
	close ();
}

/** Dump output from @a string for @a length bytes to the socket.  */
void
SocketLog::emit (const char *string, int length)
{
    ASSERT (m_socket);
    try { m_socket->xwrite (string, length); } catch (...) {;}
}

/** Shut down the socket connection. */
void
SocketLog::close (void)
{
    ASSERT (m_socket);

    try
    {
	m_socket->xwrite ("\n\nLog closed, bailing out\n", 26);
	m_socket->close ();
	m_socket = 0;
    }
    catch (...)
    {}
}

} // namespace seal
