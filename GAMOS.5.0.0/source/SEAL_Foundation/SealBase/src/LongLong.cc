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

#include "SealBase/LongLong.h"
#include "SealBase/BitTraits.h"
#include <cstdio>

namespace seal {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>

#if !HAVE_OSTREAM_LONG_LONG_OVERLOAD && ! defined HAVE_INT64
SEAL_BASE_API std::ostream &operator<< (std::ostream &s, long long n)
{
    // FIXME: Format flags?
    char buf [BitTraits<long long>::Digits + 2];
    sprintf (buf, "%Ld", n);  // FIXME: ll, L, or q?
    return out << buf;
}

SEAL_BASE_API std::ostream &operator<< (std::ostream &s, unsigned long long n)
{
    // FIXME: Format flags?
    char buf [BitTraits<long long>::Digits + 2];
    sprintf (buf, "%Lu", n);  // FIXME: ll, L, or q?
    return out << buf;
}
#endif

//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

} // namespace seal
