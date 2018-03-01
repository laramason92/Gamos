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

#include "PluginManager/ModuleDef.h"
#include "PluginManager/Module.h"
#include "debug.h"
#include "SealBase/DebugAids.h"
#include <algorithm>
#include <functional>

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

ModuleDef::ModuleDef (void)
    : m_module (0)
{}

ModuleDef::~ModuleDef (void)
{
    if (m_module)
	LOG (0, warning, LFplugin_manager, "Attached ModuleDef destroyed\n");
}

void
ModuleDef::attach (void)
{}

void
ModuleDef::detach (void)
{}

void
ModuleDef::query (void)
{}

void
ModuleDef::bind (Module *module)
{
    ASSERT (! m_module);
    ASSERT (module);
    m_module = module;
}

void
ModuleDef::release (void)
{
    ASSERT (m_module);
    m_module = 0;
}

Module *
ModuleDef::module (void) const
{
    ASSERT (m_module);
    return m_module;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
SimpleModuleDef::query (void)
{
    std::for_each (onQueryMD.begin (), onQueryMD.end (),
		   std::bind2nd (std::mem_fun_ref (&MDAction::operator()), this));
    std::for_each (onQuery.begin (), onQuery.end (),
		   std::mem_fun_ref (&Action::operator()));
}

void
SimpleModuleDef::attach (void)
{
    std::for_each (onAttachMD.begin (), onAttachMD.end (),
		   std::bind2nd (std::mem_fun_ref (&MDAction::operator()), this));
    std::for_each (onAttach.begin (), onAttach.end (),
		   std::mem_fun_ref (&Action::operator()));
}

void
SimpleModuleDef::detach (void)
{
    std::for_each (onDetachMD.begin (), onDetachMD.end (),
		   std::bind2nd (std::mem_fun_ref (&MDAction::operator()), this));
    std::for_each (onDetach.begin (), onDetach.end (),
		   std::mem_fun_ref (&Action::operator()));
}

} // namespace seal
