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
#if HAVE_NEW
# include <new>
#elif HAVE_NEW_H
# include <new.h>
#endif

namespace seal {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>

/** Logging device that discard all output.  #logstream uses this as a
    fallback when the user-specified device fails to initialise.  It
    may also be used explicitly at run time via the @c $LOG.  */
class SinkLog : public logstream
{
public:
    SinkLog (void);
    ~SinkLog (void);

protected:
    virtual void	emit (const char *string, int length);
};

//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

logstream *
logstream::createSinkLogDevice (const char *)
{
    // make sure the log stays around beyond destructors
    static char		data[sizeof(SinkLog)];
    static SinkLog	*log = new (&data[0]) SinkLog;
    return log;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/** Initialise the sink.  */
SinkLog::SinkLog (void)
{ }

/** No-op.  */
SinkLog::~SinkLog (void)
{ }

/** Discard output. */
void
SinkLog::emit (const char *, int)
{ }

} // namespace seal
