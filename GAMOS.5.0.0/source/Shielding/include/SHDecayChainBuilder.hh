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
#ifndef SHDecayChainBuilder_h
#define SHDecayChainBuilder_h 1

#include <map>
#include <vector>
#include "G4Ions.hh"

typedef std::vector<G4Ions*> vi;
typedef std::map<G4ParticleDefinition*, G4double> mpd;
typedef std::map<G4ParticleDefinition*, mpd* > mpmpd;

class G4Track;

class SHDecayChainBuilder
{

 public:
  SHDecayChainBuilder();
  ~SHDecayChainBuilder();

 public:
  void StoreIon(const G4Track*, G4bool bAlsoLifeTime0 = false);
  std::vector< vi* > StoreEventChains();
  void PrintChains( std::ostream& out = G4cout );
  static G4String GetChainName( vi* chain );
  std::map<G4String, vi* > GetChains() const{
    return theChains; 
  }
  vi GetChainFromTrack( G4int trkID );
  void Clean();

  G4double GetDecayBR( G4ParticleDefinition* parent, G4ParticleDefinition* daughter, G4bool bMustExist = true );

  void StoreDecayBranchingRatios( const G4Track* aTrack );
  void BuildPossibleDecayChains();

private:
  void AddChain( vi* );
  void AddDaughters( vi* chain );
  
private:
  std::map<G4Ions*,G4int> theIons; // start of decay chain
  std::map<G4int, G4int> theParentChildren;
  //  std::map<G4String, vi* > theChains;
  std::vector< vi* > theChains;

  mpmpd theBranchingRatios;

};

#endif
