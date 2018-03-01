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

#include "IOSelectFdMap.h"
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
IOSelectFdMap::fdmap (IOFD fd, size_t index)
{
    ASSERT (fd >= 0);
    if ((size_t) fd >= m_fd2index.size ())
	m_fd2index.resize (fd+1, ~0lu);

    // We require the descriptor to be unused, i.e. not to refer to
    // request index.  We cannot support client which re-subscribe
    // themselves before unsubscribing.  The derived class is allowed
    // to look at only the registered descriptors without counting
    // duplicates.  If a descriptor is added twice, the second writes
    // over the first one, and unsubscribing the first then loses the
    // second.  The only solution that can be guaranteed to be correct
    // is to either not re-subscribe or always unsubscribe first, and
    // never to subscribe any descriptor twice.
    ASSERT (m_fd2index [fd] == ~0lu);
    m_fd2index [fd] = index;
}

size_t
IOSelectFdMap::purge (IOFD fd)
{
    // Mark the descriptor unused
    ASSERT (fd >= 0);
    ASSERT ((size_t) fd < m_fd2index.size ());

    size_t index = m_fd2index [fd];

    m_fd2index [fd] = ~0lu;

    // This is always used in combination to removeReq, which causes
    // indices greater than those of this file descriptor to shrink.
    // So update all indices larger than the one we are removing.
    // Remember which fd was the last actually used one.
    size_t last = m_fd2index.size ();
    for (size_t i = 0; i < m_fd2index.size (); ++i)
	if (m_fd2index [i] != ~0ul)
	{
	    last = i+1;
	    if (m_fd2index [i] > index)
		--m_fd2index [i];
	}

    // Now shrink the table to the limit we actually need.
    m_fd2index.resize (last);

    // Return original index for removing client request.
    return index;
}

void
IOSelectFdMap::compact (void)
{
    std::vector<size_t> junk;
    std::swap (junk, m_fd2index);
}

} // namespace seal
#endif // !_WIN32
