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
#ifndef SHDecayChains_h
#define SHDecayChains_h 1

#include <map>
#include <vector>
#include "G4Ions.hh"

typedef std::vector<G4Ions*> vi;
class G4Track;

class SHDecayChains
{

 public:
  SHDecayChains();
  ~SHDecayChains();

 public:
  void StoreIon(const G4Track*);
  void StoreEventChains();
  void PrintChains( std::ostream& out );
  G4String GetChainName( vi* chain );
  std::map<G4String, vi* > GetChains() const{
    return theChains; 
  }
  void Clean();

 private:
  void AddChain( vi* );

private:
  std::map<G4int,G4Ions*> theIons;
  std::map<G4int, G4int> theParentChildren;
  std::map<G4String, vi* > theChains;
};

#endif
