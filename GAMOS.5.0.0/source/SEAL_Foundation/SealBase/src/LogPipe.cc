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
#include "SealBase/Error.h"
#include "SealBase/PipeCmd.h"
#if HAVE_NEW
# include <new>
#elif HAVE_NEW_H
# include <new.h>
#endif

namespace seal {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>

/** Logging device sending all output to another process via a pipe. */
class PipeLog : public logstream
{
public:
    PipeLog (void);
    ~PipeLog (void);

    logstream *		initialise (const char *command);

protected:
    virtual void	emit (const char *string, int length);

private:
    void		close (void);

    PipeCmd		m_pipe; //< The pipe
};

//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>

//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

logstream *
logstream::createPipeLogDevice (const char *args)
{
    // make sure the log stays around beyond destructors
    static char		data[sizeof(PipeLog)];
    static PipeLog	*log = new (&data[0]) PipeLog;
    return log->initialise (args);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/** Default-initialise the log device. */
PipeLog::PipeLog (void)
{}

/** Start up the command @a name as the pipe end point. */
logstream *
PipeLog::initialise (const char *command)
{
    try
    {
	m_pipe.open (command, IOFlags::OpenWrite);
	m_pipe.xwrite ("\n--- Log opened\n", 16);
    }
//      catch (Error &e)
//      {
//  	return 0;
//      }
    catch (...)
    {
	return 0;
    }

    return this;
}

/** Flush and close the log.  */
PipeLog::~PipeLog (void)
{ flush (); close (); }

/** Dump output from @a string for @a length bytes to the file.  */
void PipeLog::emit (const char *string, int length)
{ try { m_pipe.xwrite (string, length); } catch (...) {;} }

/* Shut down the coprocess connection. */
void PipeLog::close (void)
{
    emit ("\n\nLog closed, bailing out\n", 26);
    try { m_pipe.close (); } catch (...) {;}
}

} // namespace seal
