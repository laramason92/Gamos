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

#include "PluginManager/PluginFactoryBase.h"
#include "PluginManager/PluginManager.h"
#include "PluginManager/ModuleDescriptor.h"
#include "PluginManager/ModuleCache.h"
#include "PluginManager/Module.h"
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

PluginFactoryBase::PluginFactoryBase (const std::string &tag)
    : m_tag (tag)
{
    ASSERT (PluginManager::get ());
    ASSERT (! m_tag.empty ());
    PluginManager::get ()->addFactory (this);
}

PluginFactoryBase::~PluginFactoryBase (void)
{
    ASSERT (PluginManager::get ());
    ASSERT (! m_tag.empty ());
    PluginManager::get ()->removeFactory (this);
}

const std::string &
PluginFactoryBase::category (void) const
{ return m_tag; }

void
PluginFactoryBase::rebuild (void)
{
    PluginManager			*db = PluginManager::get ();
    PluginManager::DirectoryIterator	dir;
    ModuleCache::Iterator		module;
    ModuleDescriptor			*cache;
    unsigned				i;

    // The modules cannot have infos already cached that we need to
    // avoid recreating.  This is because the infos can be created
    // only in one of two ways, and in either case the factory knows
    // *and* has already cleared out those infos in the derived
    // rebuild() before invoking us.  Infos restored from the cache
    // are managed by the factory, and thus the known and already
    // deleted.  When infos are created via the factory's describe()
    // method, the factory must already exist and thus the infos are
    // automatically registered (and thus already deleted).  The
    // latter is because the factory must be in the same library as the
    // one invoking describe(), or one of its dependents; either way
    // since the factory is a global object, it has been constructed (the
    // query happens after global constructors).
    //
    // Which brings us back to the point: there cannot be infos we
    // care about at this point.  As we start, the derived factory has no
    // infos (guaranteed by derived rebuild()), so the only infos that
    // exist are those we create here below.
    for (dir = db->beginDirectories (); dir != db->endDirectories (); ++dir)
	for (module = (*dir)->begin (); module != (*dir)->end (); ++module)
	    for (cache=(*module)->cacheRoot(), i=0; i < cache->children(); ++i)
		if (cache->child (i)->token (0) == category ())
		    restore (*module, cache->child (i));
}

} // namespace seal
