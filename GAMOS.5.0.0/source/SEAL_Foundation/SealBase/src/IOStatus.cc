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

#include "SealBase/IOStatus.h"
#include "SealBase/Log.h"
#include "SealBase/sysapi/IOStatus.h"

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

// FIXME: SEAL_BASE_API?
logstream &
operator<< (logstream &log, const IOStatus &s)
{
    log << "I/O channel status information:" << indent
	<< "\ncreate time = " << s.m_ctime
	<< "\nmodify time = " << s.m_mtime
	<< "\naccess time = " << s.m_atime
	<< "\ntype        = ";
    switch (s.m_type)
    {
    case IOStatus::TypeFile:		log << "File";			break;
    case IOStatus::TypeDirectory:	log << "Directory";		break;
    case IOStatus::TypeSocket:		log << "Socket";		break;
    case IOStatus::TypeSymlink:		log << "Symlink";		break;
    case IOStatus::TypeNetworkSpecial:	log << "Network special";	break;
    case IOStatus::TypeBlockDev:	log << "Block device";		break;
    case IOStatus::TypeCharDev:		log << "Character device";	break;
    case IOStatus::TypeFIFO:		log << "FIFO";			break;
    case IOStatus::TypeVolumeId:	log << "Volume label";		break;
    default:				log << "Unknown";		break;
    }
    log << "\nacls        = " << s.m_acl.unix () << endl // FIXME: just send acl itself
	<< "\n# links     = " << s.m_links << endl
	<< "\nsize        = " << s.m_size << undent;

    return log;
}

void
IOStatus::clear (void)
{
    m_ctime = m_mtime = m_atime = 0;
    m_type = TypeUnknown;
    m_acl = 0;
    m_flags = 0;
    m_unique [0] = m_unique [1] = m_unique [2] = 0;
    m_links = 0;
    m_size = 0;
}

} // namespace seal
