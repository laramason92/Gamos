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
#ifndef SHPlotPHPDBSpectraUA_hh
#define SHPlotPHPDBSpectraUA_hh

#include "SHPrintPHPChannelXSUA.hh"

class GmAnalysisMgr;
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include "GamosCore/GamosAnalysis/include/GmVHistoBuilder.hh"
#include "GamosCore/GamosBase/Base/include/GmChangeEnergies.hh"
#include <map>
#include <vector>

class G4PVPlacement;
class G4HadronicProcess;
class G4Material;
class GmAnalysisMgr;
class G4ParticleDefinition;

class SHPlotPHPDBSpectraUA : public GmUserRunAction,
			     public GmUserEventAction,
			     public GmVHistoBuilder,
			     public GmChangeEnergies
{
  public:
  SHPlotPHPDBSpectraUA();
  ~SHPlotPHPDBSpectraUA(){};
  virtual void BeginOfRunAction(const G4Run*);
  virtual void BeginOfEventAction(const G4Event*);
  virtual void EndOfRunAction(const G4Run*);

private:
  //  void BookHistos();
private:
  G4String thePrimaryParticleName;

  G4int theNEvents;
  G4int theMateIdxMax;
  G4int theIsotIdxMax;
  G4int theProdIdxMax;
  G4int theProcIdxMax;
  G4int theProductMaxAZ;

};
#endif