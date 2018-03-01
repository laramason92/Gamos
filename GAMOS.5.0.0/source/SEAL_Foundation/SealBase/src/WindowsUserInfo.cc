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
#ifdef _WIN32

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

#define I_M_H_N_L	INTERNET_MAX_HOST_NAME_LENGTH

#define MyToChar(to,from,length)	\
  WideCharToMultiByte (CP_ACP,0,(LPWSTR) (from),-1,(to),(length),0,0)
#define MyToUnicode(to,from,length)	\
  MultiByteToWideChar (CP_ACP,0,(from),-1,(to),(length))

#define MyLsaQuery(to,item,info,part)				\
    if (LsaQueryInformationPolicy (lsa, item, (PVOID *) &info))	\
	throw SystemError (GetLastError ());			\
    MyCopyLsaName (to, info->part, I_M_H_N_L+1);		\
    LsaFreeMemory (info)

static void
MyCopyLsaName (WCHAR *to, LSA_UNICODE_STRING &from, size_t size)
{
    size = (size - 1) * sizeof (WCHAR);
    if (from.Length < size)
	size = from.Length;
    memcpy (to, from.Buffer, size);
    to [size / 2] = 0;
}

static void
MyGetServerInfo (LSA_HANDLE lsa, char *logsrv, char *domain)
{
    WCHAR			account [I_M_H_N_L + 1];
    WCHAR			primary [I_M_H_N_L + 1];
    PPOLICY_ACCOUNT_DOMAIN_INFO accinfo;
    PPOLICY_PRIMARY_DOMAIN_INFO priminfo;

    MyLsaQuery (account,PolicyAccountDomainInformation,
		accinfo,DomainName);
    MyLsaQuery (primary,PolicyPrimaryDomainInformation,
		priminfo,Name);

    LPSERVER_INFO_101		info;
    DWORD			count;
    DWORD			total;
    NET_API_STATUS		ret
	= NetServerEnum (0, 101, (LPBYTE *) &info,
			 MAX_PREFERRED_LENGTH,
			 &count, &total,
			 SV_TYPE_DOMAIN_CTRL, primary, 0);

    if (! ret && count > 0)
    {
	if (logsrv)
	    MyToChar (logsrv, info [0].sv101_name, I_M_H_N_L + 1);
	if (domain)
	    MyToChar (domain, primary, I_M_H_N_L + 1);
	NetApiBufferFree (info);
    }
    else
    {
	if (logsrv)
	    MyToChar (logsrv, account, I_M_H_N_L + 1);
	if (domain)
	    MyToChar (domain, account, I_M_H_N_L + 1);
    }
}

UserInfo::UserInfo (const std::string &id, UserInfo *next)
{
    // FIXME: dig out sid? see cygwin/uinfo.cc
    const char	*env;
    char	buf [512];

    if (id.empty ())
    {
	DWORD size = UNLEN + 1;
	m_uid = m_euid = 1; // FIXME: get real sids
	m_gid = m_egid = 1; // FIXME: get real sids

	if (GetUserName (buf, &size))
	    m_id = buf;

	// This is all NT and up only
	if ((env = getenv ("USERNAME")))
	    m_id = env;
	if ((env = getenv ("USERDOMAIN")))
	    m_domain = env;
	if ((env = getenv ("LOGONSERVER")) && env[0] == '\\' && env[1] == '\\')
	    m_logsrv = env + 2;

	// Get user id, logon server and domain
	LPWKSTA_USER_INFO_1 wuserinfo;
	if ((m_id.empty () || m_domain.empty ())
	    && ! NetWkstaUserGetInfo (NULL, 1, (LPBYTE *) &wuserinfo))
	{
	    MyToChar (buf, wuserinfo->wkui1_username, UNLEN + 1);
	    m_id = buf;

	    MyToChar (buf, wuserinfo->wkui1_logon_server, I_M_H_N_L + 1);
	    m_logsrv = buf;

	    MyToChar (buf, wuserinfo->wkui1_logon_domain, I_M_H_N_L + 1);
	    m_domain = buf;

	    NetApiBufferFree (wuserinfo);
	}

	// Get logon server
	if (m_logsrv.empty ())
	{
	    LSA_OBJECT_ATTRIBUTES	attr = { 0, 0, 0, 0, 0, 0 };
	    LSA_HANDLE			lsa = INVALID_HANDLE_VALUE;

	    if (LsaOpenPolicy (0, &attr, POLICY_ALL_ACCESS, &lsa))
		throw SystemError (GetLastError ());
	    MyGetServerInfo (lsa, buf, 0);
	    LsaClose (lsa);
	    m_logsrv = buf;
	}
    }
    else
	// FIXME: m_domain, m_logsrv?
	m_id = id;

    // Get user name and home dir and other such things
    LPUSER_INFO_3	userinfo = 0;
    WCHAR		wuser [UNLEN + 1];
    WCHAR		wlogsrv [I_M_H_N_L + 3];

    strcat (strcpy (buf, "\\\\"), m_logsrv.c_str ());
    MyToUnicode (wlogsrv, buf, I_M_H_N_L + 3);
    MyToUnicode (wuser, m_id.c_str (), UNLEN + 1);

    if (!NetUserGetInfo (0, wuser, 3, (LPBYTE *) &userinfo)
	|| !NetUserGetInfo (wlogsrv, wuser, 3, (LPBYTE *) &userinfo))
    {
	MyToChar (buf, userinfo->usri3_full_name, sizeof (buf));
	m_name = buf;

	MyToChar (buf, userinfo->usri3_home_dir, MAX_PATH);
	if (! *buf)
	{
	    MyToChar (buf, userinfo->usri3_home_dir_drive, MAX_PATH);
	    if (* buf)
		strcat (buf, "\\");
	    else
	    {
		env = getenv ("SYSTEMDRIVE");
		if (env && *env)
		    strcat (strcpy (buf, env), "\\");
		else
		    GetSystemDirectoryA (buf, MAX_PATH);
	    }
	}
	m_homepath = buf + 2;
	m_homedrive = std::string (buf, 2);

	NetApiBufferFree (userinfo);
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
	// FIXME: Check that the user exists!
	u = s_users = new UserInfo (id, s_users);

    return u;
}

} // namespace seal
#endif // _WIN32
