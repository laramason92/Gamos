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

#include "PluginManager/PluginManager.h"
#include "SealBase/StringOps.h"
#include "SealBase/Signal.h"
#include "SealBase/Error.h"
#include <iostream>
#include <cstdlib>
#include <cstring>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

using namespace seal;

void feedback (PluginManager::FeedbackData data)
{
    std::string explanation;
    if (data.error)
	explanation = StringOps::replace (data.error->explain (), '\n', "\n\t");

    if (data.code == PluginManager::StatusLoading)
	std::cerr << "Note: Loading " << data.scope << "\n";

    else if (data.code == PluginManager::ErrorLoadFailure)
	std::cerr << "  *** WARNING: module `" << data.scope
		  << "' failed to load for the following reason\n\t"
		  << explanation << "\n";

    else if (data.code == PluginManager::ErrorBadModule)
	std::cerr << "  *** WARNING: module `" << data.scope
		  << "' ignored until problems with it are fixed.\n\n";

    else if (data.code == PluginManager::ErrorBadCacheFile)
	std::cerr << "  *** WARNING: cache file " << data.scope
		  << " is corrupted.\n";

    else if (data.code == PluginManager::ErrorEntryFailure)
	std::cerr << "  *** WARNING: module `" << data.scope
		  << "' does not have the required entry point, reason was\n\t"
		  << explanation << "\n";

    // This cannot actually trigger in SealPlugin* utility, for example only
    else if (data.code == PluginManager::ErrorNoFactory)
	std::cerr << "  *** WARNING: module `" << data.scope
		  << "' missing one or more factories for plug-ins\n";
}

int main (int, char **argv)
{
    Signal::handleFatal (argv [0]);

    PluginManager::destroyOnExit (argv[1] && !strcmp (argv[1], "-clean"));
    PluginManager *db = PluginManager::get ();
    db->addFeedback (CreateCallback (&feedback));
    db->initialise ();
    return EXIT_SUCCESS;
}
