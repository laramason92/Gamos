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
#include <stdio.h>
#if HAVE_NEW
# include <new>
#elif HAVE_NEW_H
# include <new.h>
#endif
//#if HAVE_NEW_IOSTREAMS
//# include <iostream>
//#else
//# include <iostream.h>
//#endif

namespace seal {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>

//<doc>		Console logger.
//
//		This is a loggind device that dumps all output to the standard
//		error.
class StderrLog : public logstream
{
public:
    StderrLog (void);
    ~StderrLog (void);

protected:
    virtual void	emit (const char *psz, int n);
};

//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

logstream *
logstream::createStderrLogDevice (const char *)
{
    // make sure the log stays around beyond destructors
    static char		data[sizeof(StderrLog)];
    static StderrLog	*log = new (&data[0]) StderrLog;
    return log;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

//<doc>		Initialise a console logging device.
StderrLog::StderrLog (void)
{ }

//<doc>		Flush the log.
StderrLog::~StderrLog (void)
{
    flush ();
}

//<doc>		Dump output to the console.
//<args>	string			The data to write.
//		length			The length of data to write.
void StderrLog::emit (const char *string, int length)
{
    // std::cerr.write (string, length);
    // std::cerr.flush ();
    fwrite (string, 1, length, stderr);
    fflush (stderr);
}

} // namespace seal
