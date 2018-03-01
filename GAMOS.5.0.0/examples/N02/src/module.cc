#include "../include/ExN02DetectorConstruction.hh"
#include "../include/ExN02PrimaryGeneratorAction.hh"
#include "../include/ExN02PhysicsList.hh"
#include "../include/ExN02RunAction.hh"
#include "../include/ExN02EventAction.hh"
#include "../include/ExN02SteppingAction.hh"
#include "../include/ExN02TrackerSD.hh"

#include "GamosCore/GamosGeometry/include/GmGeometryFactory.hh"
#include "GamosCore/GamosGenerator/include/GmGeneratorFactory.hh"
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsFactory.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserActionFactory.hh"
#include "GamosCore/GamosSD/include/GmSensDetFactory.hh"

DEFINE_SEAL_MODULE ();
DEFINE_GAMOS_GEOMETRY(ExN02DetectorConstruction);
DEFINE_GAMOS_GENERATOR(ExN02PrimaryGeneratorAction);
DEFINE_GAMOS_PHYSICS(ExN02PhysicsList);
DEFINE_GAMOS_USER_ACTION(ExN02RunAction);
DEFINE_GAMOS_USER_ACTION(ExN02EventAction);
DEFINE_GAMOS_USER_ACTION(ExN02SteppingAction);
DEFINE_GAMOS_SENSDET(ExN02TrackerSD);

