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
#ifndef RTGeneratorPhaseSpace_HH
#define RTGeneratorPhaseSpace_HH

class G4Event;
class GmParticleSource;
class GmIsotopeMgr;
class iaea_record_type;
#include "globals.hh"
#include <iostream>
#include <fstream>
#include "GamosCore/GamosGenerator/include/GmGeneratorFromFile.hh"
class G4ParticleDefinition;
class RTPhaseSpaceHistos;
class RTExtraInfoMgr;
enum MWR {NOMir, MirX, MirY, MirXY};

class RTGeneratorPhaseSpace: public GmGeneratorFromFile
{
public:
  RTGeneratorPhaseSpace();
  ~RTGeneratorPhaseSpace();
  void GeneratePrimaries(G4Event* evt);

private:
  FILE* p_rfile;
  iaea_record_type *p_iaea_read;

  G4int theMaxNReuse, theNReused;
  G4int theMaxNRecycle, theNRecycled;
  G4String theFileName;
  G4int theNEventsInFile;
  G4bool bMaxNReuseAutomatic;
  G4ThreeVector theInitialDisp;
  MWR MirrorWhenReuse;

  G4double theInitialRotAngleX;
  G4double theInitialRotAngleY;
  G4double theInitialRotAngleZ;

  G4int theNEventsSkip;
  G4ParticleDefinition* theParticleDef;

  G4bool bHistos;
  RTPhaseSpaceHistos* theHistos;

  RTExtraInfoMgr* theExtraInfoMgr;

  enum transType { TTDisplacement, TTRotateXYZ, TTRotateThetaPhiSelf } ;/// " D RXYZ, RTPS 
  
  struct transformation_struct{
    transType operation;
    G4double  opx;
    G4double  opy;
    G4double  opz;
  };
  
  std::vector<transformation_struct> theTransformations;

  G4int theNNewEvents;
  G4double theNParticlesRead;
  G4double theNOrigEvents;  
};

#endif
