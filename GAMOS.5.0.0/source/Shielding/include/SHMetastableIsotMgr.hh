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
#ifndef SHMetastableIsotMgr_h
#define SHMetastableIsotMgr_h

#include "globals.hh"
#include <vector>
#include <map>
class G4ParticleDefinition;
class G4Ions;

typedef std::pair<G4double,G4double> pdd;
typedef std::pair<G4ParticleDefinition*,pdd> ppdd;

struct MetastableData {
  G4String name;
  G4double energy;
  G4double halflife;
  G4double branchingRatio;
};    //Sc44m 270.95*keV 58.61*hour 0.988

//------------------------------------------------------------------------
class SHMetastableIsotMgr 
{ 
 private:
  SHMetastableIsotMgr();

 public:
  ~SHMetastableIsotMgr();

  //! Get the only instance 
  static SHMetastableIsotMgr* GetInstance();
  
  void ReadData();
  std::pair<G4Ions*,G4double> FindIsomere( G4ParticleDefinition* nucleus, std::vector<ppdd> tracks ); // return isomere and its weight

private:

  static SHMetastableIsotMgr* theInstance;
  std::map<G4String, G4Ions* > theMetastableIsotopes;
  std::map<G4String, MetastableData > theMetastableIsotopesData;

};
#endif
