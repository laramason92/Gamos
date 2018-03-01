#------------------------------------------------------------------------------
# 
# Module : MagFieldManager
# Package: 
#
#------------------------------------------------------------------------------
# List external includes needed.

# List external includes needed.
include(GamosSetup)
include(UseRoot)
include(UseGeant4)

#
# Define the GAMOS Module.
#
include(Geant4MacroDefineModule)
GEANT4_DEFINE_MODULE(NAME MagFieldManager

   HEADERS
       BiotSavartIntegrator.hh
       CoilType2.hh
       LinearSegmentCreator.hh
       LinearSegment.hh
       CircularCoilCreator.hh
       IdealToroidWithRectangularCoilCreator.hh
       RacetrackCoilCreator.hh
       MagFieldModManMessenger.hh
       ReplicateFieldCreator.hh
       JCurrentCalculator.hh
       CoilType3Creator.hh
       IdealToroidWithRectangularCoil.hh
       CircularSolenoid.hh
       ReplicateField.hh
       UniformFieldCreator.hh
       CoilType3.hh
       InterpolatedField.hh
       CircularSolenoidCreator.hh
       CompoundField.hh
       CurvedSegment.hh
       RacetrackCoil.hh
       CircularCoil.hh
       IdealToroidWithCircularCoilCreator.hh
       MultiplyField.hh
       MagneticEquationOfMotion.hh
       MagneticFieldModelManager.hh
       ConfinedFieldCreator.hh
       CurvedSegmentCreator.hh
       InterpolatedFieldCreator.hh
       IdealToroidWithCircularCoil.hh
       TransformationManager.hh
       CompoundFieldCreator.hh
       TransManMessenger.hh
       CoilType2Creator.hh
       MultiplyFieldCreator.hh
       MagneticDipoleCreator.hh
       MagneticDipole.hh
       ConfinedField.hh
       ParametrizedFieldCreator.hh
       special_functions.hh

   SOURCES
       CircularCoil.cc
       plugin.cc
       InterpolatedField.cc
       UniformFieldCreator.cc
       JCurrentCalculator.cc
       CoilType3Creator.cc
       ReplicateField.cc
       special_functions.cc
       CurvedSegment.cc
       ConfinedField.cc
       IdealToroidWithCircularCoilCreator.cc
       ParametrizedFieldCreator.cc
       CompoundFieldCreator.cc
       ConfinedFieldCreator.cc
       MagneticFieldModelManager.cc
       CompoundField.cc
       MultiplyFieldCreator.cc
       LinearSegment.cc
       CoilType3.cc
       IdealToroidWithRectangularCoil.cc
       InterpolatedFieldCreator.cc
       ReplicateFieldCreator.cc
       TransManMessenger.cc
       TransformationManager.cc
       CoilType2.cc
       RacetrackCoilCreator.cc
       CoilType2Creator.cc
       CircularSolenoidCreator.cc
       BiotSavartIntegrator.cc
       CircularSolenoid.cc
       MultiplyField.cc
       MagFieldModManMessenger.cc
       MagneticDipole.cc
       CurvedSegmentCreator.cc
       IdealToroidWithRectangularCoilCreator.cc
       LinearSegmentCreator.cc
       IdealToroidWithCircularCoil.cc
       MagneticDipoleCreator.cc
       CircularCoilCreator.cc
       RacetrackCoil.cc
       MagneticEquationOfMotion.cc
 
   GRANULAR_DEPENDENCIES

   GLOBAL_DEPENDENCIES
    ${Geant4_LIBRARIES}

   LINK_LIBRARIES
)

# List any source specific properties here
