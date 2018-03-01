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

#include "IOSelectMethod.h"

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

IOSelectMethod::~IOSelectMethod (void) {}
void IOSelectMethod::setSignal (int) {}

#if 0
size_t
IOSelectMethod::findReq (IOFD fd)
{
    for (size_t i = 0; i < m_requests.size (); ++i)
	if (m_requests [i].fd == fd)
	    return i;

    ASSERT (false);
    return m_requests.size ();
}
#endif

size_t
IOSelectMethod::addReq (const IOSelectRequest &req)
{
    m_requests.push_back (req);
    return m_requests.size () - 1;
}

void
IOSelectMethod::removeReq (const IOSelectRequest &req, size_t index)
{
    ASSERT (index < m_requests.size ());
    ASSERT (m_requests [index].fd == req.fd);
    ASSERT (m_requests [index].channel == req.channel);
    m_requests.erase (m_requests.begin () + index);
}

#if 0
void
IOSelectMethod::removeReq (const IOSelectRequest &req)
{ removeReq (req, findReq (req.fd)); }
#endif

void
IOSelectMethod::clearReqs (void)
{ std::vector<IOSelectRequest> junk; std::swap (m_requests, junk); }

} // namespace seal
