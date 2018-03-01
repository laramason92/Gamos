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

#include "PluginManager/PluginInfo.h"
#include "PluginManager/ModuleDescriptor.h"
#include "PluginManager/PluginManager.h"
#include "PluginManager/Module.h"
#include "debug.h"
#include "SealBase/DebugAids.h"
#include "SealBase/Log.h"

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

PluginInfo::PluginInfo (Module *module,
			const std::string &name,
			const std::string &category)
    : m_module (module),
      m_name (name),
      m_category (category)
{
    ASSERT (m_module);
    ASSERT (! m_name.empty ());
    ASSERT (! m_category.empty ());
    LOG (0, trace, LFplugin_manager, "defining " << m_category
	 << ' ' << m_name << '\n');
}

PluginInfo::~PluginInfo (void)
{
    ASSERT (m_module);
    ASSERT (! m_name.empty ());
    ASSERT (! m_category.empty ());
    m_module->detachInfo (this);
}

void
PluginInfo::finish (bool needCache)
{
    // Derived constructor is complete, tell the module I exist.  This
    // adds myself to the module and propagates the notification to
    // the relevant factory.  Note that the factory must already exist, so
    // it will get notified immediately.  However it might still be in
    // the middle of construction, which is why we propagate the
    // notification through the base classes and not directly to the
    // factory from the derived class.
    m_module->addInfo (this, needCache);
}

Module *
PluginInfo::module (void) const
{
    ASSERT (m_module);
    ASSERT (! m_name.empty ());
    ASSERT (! m_category.empty ());
    return m_module;
}

const std::string &
PluginInfo::name (void) const
{
    ASSERT (m_module);
    ASSERT (! m_name.empty ());
    ASSERT (! m_category.empty ());
    return m_name;
}

const std::string &
PluginInfo::category (void) const
{
    ASSERT (m_module);
    ASSERT (! m_name.empty ());
    ASSERT (! m_category.empty ());
    return m_category;
}

void
PluginInfo::cache (ModuleDescriptor *to) const
{
    ASSERT (m_module);
    ASSERT (! m_name.empty ());
    ASSERT (! m_category.empty ());
    ASSERT (to);
    new ModuleDescriptor (to, m_category, m_name);
}

void
PluginInfo::detach (void)
{
    ASSERT (m_module);
    ASSERT (! m_name.empty ());
    ASSERT (! m_category.empty ());
}

void
PluginInfo::noFactory (void) const
{
    m_module->manager ()->feedback
	(PluginManager::ErrorNoFactory,
	 m_name + " (" + m_module->libraryName ().name () + ")");
    ASSERT (false);
}

} // namespace seal
