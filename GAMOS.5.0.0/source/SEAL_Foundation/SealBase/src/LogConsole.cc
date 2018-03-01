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
#ifdef _WIN32
# include <windows.h>
#else
# include <unistd.h>
# include <fcntl.h>
#endif
#if HAVE_NEW
# include <new>
#elif HAVE_NEW_H
# include <new.h>
#endif

namespace seal {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>

/** Console logger: a logging device that dumps all output to a
    Windows console window or UNIX @c /dev/console. */
class ConsoleLog : public logstream
{
public:
    ConsoleLog (void);
    ~ConsoleLog (void);

protected:
    virtual void	emit (const char *psz, int n);

private:
#ifdef _WIN32
    bool		m_own;
#else
    int			m_fd;
#endif
};

//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

logstream *
logstream::createConsoleLogDevice (const char *)
{
    // make sure the log stays around beyond destructors
    static char		data[sizeof(ConsoleLog)];
    static ConsoleLog	*log = new (&data[0]) ConsoleLog;
    return log;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/** Initialise a console logging device.  */
ConsoleLog::ConsoleLog (void)
{
#ifdef _WIN32
    m_own = AllocConsole ();
#else
    m_fd = ::open ("/dev/console", O_WRONLY, 0);
#endif
}

/** Flush the log.  */
ConsoleLog::~ConsoleLog (void)
{
    flush ();
#ifdef _WIN32
    if (m_own) FreeConsole ();
#else
    ::close (m_fd);
#endif
}

/** Dump output from @a string for @a length bytes to the console.  */
void
ConsoleLog::emit (const char *string, int length)
{
#ifdef _WIN32
    unsigned long written;
    ::WriteConsole (GetStdHandle (STD_OUTPUT_HANDLE),
		    string, length, &written, 0);
#else
    ::write (m_fd, string, length);
#endif
}

} // namespace seal
