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

#include "IOSelectSig.h"
#include "SealBase/DebugAids.h"
# if HAVE_POSIX_RT_SIGNALS && HAVE_F_SETSIG && HAVE_F_SETAUXFL

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

/** Attach the file descriptor from request @a req.  The caller should
    have already established handler (Signal::revert() is sufficient)
    for SIGIO and SIGURG.  */
void
IOSelectSigFd::attach (const IOSelectRequest &req)
{
    ASSERT (req.fd >= 0);
    int flags;
    if ((flags = fcntl (req.fd, F_GETAUXFL, 0)) == -1
	|| fcntl (req.fd, F_SETAUXFL, flags | O_ONESIGFD) == -1)
	throw IOError ("fcntl()", errno);

    IOSelectSig::attach (req);
}

/** Remove the client registration at @a index.  */
void
IOSelectSigFd::detach (const IOSelectRequest &req)
{
    ASSERT (req.fd >= 0);
    int flags;
    if ((flags = fcntl (req.fd, F_GETAUXFL, 0)) == -1
	|| fcntl (req.fd, F_SETAUXFL, flags & ~O_ONESIGFD) == -1)
	throw IOError ("fcntl()", errno);

    IOSelectSig::detach (req);
}

} // namespace seal
#endif // HAVE_POSIX_RT_SIGNALS && HAVE_F_SETSIG && HAVE_F_SETAUXFL
