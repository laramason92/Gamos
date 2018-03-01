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

#include "SealBase/DirIterator.h"
#include "SealBase/Filename.h"
#include "SealBase/FileSystemError.h"
#include "SealBase/DebugAids.h"
#include "SealBase/sysapi/DirIterator.h"
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

/** System-specific version of #next().  */
bool
DirIterator::sysnext (void)
{
    errno = 0;
    if (! (m_data->m_dirent = readdir (m_data->m_dir)) && errno)
	throw FileSystemError ("readdir()", errno);
    else if (! m_data->m_dirent)
    {
	// no more files
	m_data->m_last = true;
	return false;
    }
    else
	// skip `.' and `..' directory entries
	return (! m_data->m_dots
		&& m_data->m_dirent->d_name[0] == '.'
		&& (m_data->m_dirent->d_name[1] == 0
		    || (m_data->m_dirent->d_name[1] == '.'
			&& m_data->m_dirent->d_name[2] == 0)));
}

/** System-specific version of #initialise().  */
void
DirIterator::sysinitialise (const char *dirname)
{
    m_data->m_dir = 0;
    m_data->m_dirent = 0;

    if (dirname && dirname[0])
    {
	if (! (m_data->m_dir = opendir (dirname)))
	    throw FileSystemError ("opendir()", errno);
	next ();
    }
}

/** System-specific version of #close().  */
void
DirIterator::sysclose (bool throwable)
{
    DIR *dir = m_data->m_dir;
    m_data->m_dir = 0;

    if (dir && closedir (dir) == -1 && throwable)
	throw FileSystemError ("closedir()", errno);
}

/** System-specific version of #operator*().  */
std::string
DirIterator::sysderef (void) const
{ ASSERT (m_data->m_dirent); return m_data->m_dirent->d_name; }

} // namespace seal
#endif // !_WIN32
