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
#ifndef _WIN32

namespace seal {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

UserInfo::UserInfo (struct passwd *entry, UserInfo *next)
{
    // `entry' can be null only for myself.  If we can't look
    // ourselves up in the password file, fall back on some basic
    // environment variables.
    if (! entry)
    {
	m_uid = getuid ();
	m_euid = geteuid ();
	m_gid = getgid ();
	m_egid = getegid ();
	entry = ::getpwuid (m_uid);
    }

    if (! entry)
    {
	if (const char *id = ::getenv ("USER"))
	    m_id = id;

	if (const char *dir = ::getenv ("HOME"))
	    m_homepath = dir;
    }
    else
    {
	// m_domain, m_logsrc, m_homedrive will be left empty and do
	// not matter on unixen.
	m_id		= entry->pw_name;
	m_password	= entry->pw_passwd;
	m_uid		= entry->pw_uid;
	m_euid		= entry->pw_uid;
	m_gid		= entry->pw_gid;
	m_egid		= entry->pw_gid;
	m_shell		= entry->pw_shell;

	// Prefer $HOME over the directory specified in /etc/passwd
	if (const char *dir = ::getenv ("HOME"))
	    m_homepath = dir;
	else
	    m_homepath = entry->pw_dir;

	// Extract name from gecos -- it is the first subfield,
	// separated from the other subfields by commas.
	size_t end = 0;
	while (entry->pw_gecos [end] && entry->pw_gecos [end] != ',')
	    ++end;

	m_name		= std::string (entry->pw_gecos, end);
    }

    m_next = next;
}

UserInfo *
UserInfo::user (const std::string &id)
{
    // FIXME: make thread safe
    if (id.empty ())
	return self ();

    UserInfo *u = s_users;
    while (u && u->m_id != id)
	u = u->m_next;

    if (! u)
    {
	struct passwd *entry = getpwnam (id.c_str ());
	if (! entry)
	    return 0;

	u = s_users = new UserInfo (entry, s_users);
    }

    return u;
}

} // namespace seal
#endif // !_WIN32
