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
#ifndef RTPhaseSpaceHistos_hh
#define RTPhaseSpaceHistos_hh

#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "iaea_record.hh"
#include <map>
#include <set>
#include <vector>
class RTPhaseSpaceUA;
class EGSPhspParticle;
class GmVClassifier;
class G4Step;

class RTPhaseSpaceHistos 
{
public:
  RTPhaseSpaceHistos(RTPhaseSpaceUA* ua);
  RTPhaseSpaceHistos(G4double zstop);
  ~RTPhaseSpaceHistos(){}; 

  void DefineHistoLimits();
  void BookHistos(G4String partName, G4int index);
  void FillHistos( iaea_record_type *p_iaea_record , G4double zstop, const G4Step* step);  
  void FillHistos( iaea_record_type *p_iaea_record, G4int hid, G4int zstopID, G4int index );
  void FillHistos( EGSPhspParticle *particle, G4double zstop );
  void FillHistos( EGSPhspParticle *particle, G4int hid, G4int zstopID, G4int index = 0);
  G4int GetHistoID(G4String partName);
  G4int GetHistoID(float charge);

  G4String GetHistoName(G4int index);
  void SetClassifier( GmVClassifier* cl ){
    theClassifier = cl; }

private:
  void Initialise();

private:
  G4int theNbins;
  G4double theHisR;
  G4double theHisAng;
  G4double theHisE;
  std::map<G4String,G4int> theParticleHistoIDs;
  GmAnalysisMgr* theAnaMgr;
  std::vector<G4double> theZstops;

  std::set<G4int> theHistos;
  GmVClassifier* theClassifier;

  G4bool bHadrons;
};

#endif
