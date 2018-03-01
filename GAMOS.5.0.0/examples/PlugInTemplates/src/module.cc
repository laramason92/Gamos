#include "GamosCore/GamosFactories/include/GmGeometryFactory.hh"
#include "../include/GmExDetectorConstruction.hh"
DEFINE_SEAL_MODULE ();

DEFINE_GAMOS_GEOMETRY(GmExDetectorConstruction);

#include "GamosCore/GamosFactories/include/GmPhysicsFactory.hh"
#include "../include/GmExPhysicsList.hh"
DEFINE_GAMOS_PHYSICS(GmExPhysicsList);

#include "GamosCore/GamosFactories/include/GmGeneratorFactory.hh"
#include "../include/GmExPrimaryGeneratorAction.hh"
DEFINE_GAMOS_GENERATOR (GmExPrimaryGeneratorAction);
#include "GamosCore/GamosFactories/include/GmGeneratorDistributionFactories.hh"
#include "../include/GmExGenerDistPosition.hh"
DEFINE_GAMOS_GENER_DIST_POSITION(GmExGenerDistPosition);
#include "../include/GmExGenerDistDirection.hh"
DEFINE_GAMOS_GENER_DIST_DIRECTION(GmExGenerDistDirection);
#include "../include/GmExGenerDistEnergy.hh"
DEFINE_GAMOS_GENER_DIST_ENERGY(GmExGenerDistEnergy);
#include "../include/GmExGenerDistTime.hh"
DEFINE_GAMOS_GENER_DIST_TIME(GmExGenerDistTime);


#include "GamosCore/GamosFactories/include/GmSensDetFactory.hh"
#include "../include/GmExSensitiveDetector.hh"
DEFINE_GAMOS_SENSDET(GmExSensitiveDetector);
#include "GamosCore/GamosFactories/include/GmRecHitBuilderFactory.hh"
#include "../include/GmExRecHitBuilder.hh"
DEFINE_GAMOS_RECHIT_BUILDER(GmExRecHitBuilder);

#include "GamosCore/GamosFactories/include/GmUserActionFactory.hh"
#include "../include/GmExUserAction.hh"
DEFINE_GAMOS_USER_ACTION(GmExUserAction);

#include "GamosCore/GamosFactories/include/GmVerbosityFactory.hh"
#include "../include/GmExVerbosityMgr.hh"
DEFINE_GAMOS_VERBOSITY(GmExVerbosityMgr);

#include "GamosCore/GamosScoring/include/GmPrimitiveScorerFactory.hh"
#include "../include/GmExPSScorer.hh"
DEFINE_GAMOS_SCORER(GmExPSScorer);
#include "GamosCore/GamosBase/include/GmFilterFactory.hh"
#include "../include/GmExFilter.hh"
DEFINE_GAMOS_FILTER(GmExFilter);
#include "GamosCore/GamosScoring/include/GmPSPrinterFactory.hh"
#include "../include/GmExPSPrinter.hh"
DEFINE_GAMOS_SCORER_PRINTER(GmExPSPrinter);
#include "GamosCore/GamosBase/include/GmClassifierFactory.hh"
#include "GmExClassifier.hh"
DEFINE_GAMOS_CLASSIFIER(GmExClassifier);

