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

#include "SealBase/FileAcl.h"

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

FileAcl::FileAcl (void)
    : m_owner (UID (-1)),
      m_group (GID (-1)),
      m_perms (0)
{}

FileAcl::FileAcl (int unixPerms,
		  UID uid /* = UID (-1) */,
		  GID gid /* = GID (-1) */)
    : m_owner (uid),
      m_group (gid),
      m_perms (unixPerms)
{}

//////////////////////////////////////////////////////////////////////
UID
FileAcl::owner (void) const
{ return m_owner; }

void
FileAcl::owner (UID uid)
{ m_owner = uid; }

//////////////////////////////////////////////////////////////////////
GID
FileAcl::group (void) const
{ return m_group; }

void
FileAcl::group (GID gid)
{ m_group = gid; }

//////////////////////////////////////////////////////////////////////
int
FileAcl::unix (void) const
{ return m_perms; }

int
FileAcl::addUnix (int mask)
{ return m_perms |= mask; }

//////////////////////////////////////////////////////////////////////
bool
FileAcl::readonly (void) const
{ return ! (m_perms & WriteBits); }

} // namespace seal
