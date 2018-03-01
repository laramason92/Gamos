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

#include "SealBase/UserInfo.h"
#include "SealBase/SystemError.h"
#include "SealBase/sysapi/UserInfo.h"

namespace seal {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

UserInfo	*UserInfo::s_self = 0;
UserInfo	*UserInfo::s_users = 0;

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

UserInfo *
UserInfo::self (void)
{
    // FIXME: make thread safe
    if (! s_self)
    {
#ifdef _WIN32 // FIXME: Get rid of this #if
	s_self = new UserInfo ("", 0);
#else
	s_self = new UserInfo (0, 0);
#endif
    }

    return s_self;
}

/** Get the numerical user id.  */
UID
UserInfo::uid (bool effective /* = true */)
{ return effective ? m_euid : m_uid; }

/** Get the numerical group id. */
GID
UserInfo::gid (bool effective /* = true */)
{ return effective ? m_egid : m_gid; }

/** Get user id (the account name).  */
std::string
UserInfo::id (void)
{ return m_id; }

/** Get user account's domain (only meaningful on Windows). */
std::string
UserInfo::domain (void)
{ return m_domain; }

/** Get user account's logon server (only meaningful on Windows).  */
std::string
UserInfo::logonserver (void)
{ return m_logsrv; }

/** Get the user's (real) name. */
std::string
UserInfo::name (void)
{ return m_name; }

/** Get the user's home directory. */
std::string
UserInfo::homedir (void)
{ return m_homepath; }

/** Get user home drive (only meaningful on Windows). */
std::string
UserInfo::homedrive (void)
{ return m_homedrive; }

/** Get the user's (encrypted) password.  May be empty if
    inaccessible. */
std::string
UserInfo::password (void)
{ return m_password; }

/** Get the user's shell. */
std::string
UserInfo::shell (void)
{ return m_shell; }

} // namespace seal
