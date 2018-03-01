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

#include "PluginManager/ModuleDescriptor.h"
#include "SealBase/DebugAids.h"
#include <algorithm>

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

ModuleDescriptor::ModuleDescriptor (ModuleDescriptor *parent /* = 0 */,
				    const std::string &t1 /* = "" */,
				    const std::string &t2 /* = "" */,
				    const std::string &t3 /* = "" */,
				    const std::string &t4 /* = "" */,
				    const std::string &t5 /* = "" */,
				    const std::string &t6 /* = "" */,
				    const std::string &t7 /* = "" */,
				    const std::string &t8 /* = "" */)
    : m_parent (parent)
{
    if (m_parent)
	m_parent->addChild (this);

    if (! t1.empty ())
	m_tokens.push_back (t1);

    if (! t2.empty ())
    {
	ASSERT (! t1.empty ());
	m_tokens.push_back (t2);
    }

    if (! t3.empty ())
    {
	ASSERT (! t2.empty ());
	m_tokens.push_back (t3);
    }

    if (! t4.empty ())
    {
	ASSERT (! t3.empty ());
	m_tokens.push_back (t4);
    }

    if (! t5.empty ())
    {
	ASSERT (! t4.empty ());
	m_tokens.push_back (t5);
    }

    if (! t6.empty ())
    {
	ASSERT (! t5.empty ());
	m_tokens.push_back (t6);
    }

    if (! t7.empty ())
    {
	ASSERT (! t7.empty ());
	m_tokens.push_back (t7);
    }

    if (! t8.empty ())
    {
	ASSERT (! t8.empty ());
	m_tokens.push_back (t8);
    }
}

ModuleDescriptor::ModuleDescriptor (ModuleDescriptor *parent,
				    const std::vector<std::string> &tokens)
    : m_parent (parent),
      m_tokens (tokens)
{
    if (m_parent)
	m_parent->addChild (this);
}


ModuleDescriptor::~ModuleDescriptor (void)
{
    for (unsigned i = children (); i > 0; --i)
	delete m_children [i-1];
}

//////////////////////////////////////////////////////////////////////
ModuleDescriptor *
ModuleDescriptor::parent (void) const
{ return m_parent; }

unsigned
ModuleDescriptor::children (void) const
{ return m_children.size (); }

ModuleDescriptor *
ModuleDescriptor::child (unsigned index) const
{ ASSERT (index < m_children.size ()); return m_children [index]; }

void
ModuleDescriptor::addChild (ModuleDescriptor *child)
{
    ASSERT (child);
    ASSERT (std::find (m_children.begin (), m_children.end (), child)
	    == m_children.end ());

    m_children.push_back (child);
}

//////////////////////////////////////////////////////////////////////
unsigned
ModuleDescriptor::tokens (void) const
{ return m_tokens.size (); }

const std::string &
ModuleDescriptor::token (unsigned index) const
{ ASSERT (index < m_tokens.size ()); return m_tokens [index]; }

void
ModuleDescriptor::addToken (const std::string &token)
{ m_tokens.push_back (token); }

//////////////////////////////////////////////////////////////////////
void
ModuleDescriptor::dup (ModuleDescriptor *to) const
{
    ModuleDescriptor *self = new ModuleDescriptor (to, m_tokens);
    for (unsigned i = 0; i < children (); ++i)
	child (i)->dup (self);
}

} // namespace seal
