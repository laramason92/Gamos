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

#include "SealBase/Storage.h"
#include "SealBase/DebugAids.h"

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

Storage::Storage (void)
{}

Storage::~Storage (void)
{}

//////////////////////////////////////////////////////////////////////
IOSize
Storage::read (IOBuffer into, IOOffset pos)
{ return read (into.data (), into.size (), pos); }

IOSize
Storage::read (void *into, IOSize n, IOOffset pos)
{
    // FIXME: this is not thread safe!  split into separate interface
    // that a particular storage can choose to support or not?  make
    // sure that throw semantics are correct here!
    // FIXME: use saveposition object in case exceptions are thrown?
    IOOffset here = position ();
    position (pos);
    n = read (into, n);
    position (here);
    return n;
}

//////////////////////////////////////////////////////////////////////
IOSize
Storage::write (IOBuffer from, IOOffset pos)
{ return write (from.data (), from.size (), pos); }

IOSize
Storage::write (const void *from, IOSize n, IOOffset pos)
{
    // FIXME: this is not thread safe!  split into separate interface
    // that a particular storage can choose to support or not?  make
    // sure that throw semantics are correct here!

    // FIXME: use saveposition object in case exceptions are thrown?
    IOOffset here = position ();
    position (pos);
    n = write (from, n);
    position (here);
    return n;
}

//////////////////////////////////////////////////////////////////////
IOOffset
Storage::position (void) const
{
    Storage *self = const_cast<Storage *> (this);
    return self->position (0, CURRENT);
}

IOOffset
Storage::size (void) const
{
    // FIXME: use saveposition object in case exceptions are thrown?
    Storage *self = const_cast<Storage *> (this);
    IOOffset here = position ();
    self->position (0, END);
    IOOffset size = position ();
    self->position (here); // FIXME: VERIFY()?
    return size;
}

void
Storage::rewind (void)
{ position (0); }

//////////////////////////////////////////////////////////////////////
void
Storage::flush (void)
{}

void
Storage::close (void)
{}

//////////////////////////////////////////////////////////////////////
bool
Storage::eof (void) const
{ return position () == size (); }

} // namespace seal
