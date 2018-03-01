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
#ifndef RTClassifierByEILong_hh
#define RTClassifierByEILong_hh 1

#include <map>
#include "globals.hh"
#include "RTVClassifierByExtraInfo.hh"

class RTClassifierByEILong  : public RTVClassifierByExtraInfo
{
public: // with description
  RTClassifierByEILong(G4String);
  virtual ~RTClassifierByEILong();
  
  virtual G4int GetIndexFromStep(const G4Step*);

  virtual void SetParameters( std::vector<G4String>& param );

  virtual G4String GetIndexName(G4int);

  virtual std::set<G4int> GetIndexSet() const;

  virtual void SetIndices( std::vector<G4String> wl );

private:
  std::map<G4int,G4String> theIndexNames;
  std::map<G4int,G4int> theIndexMap;
  G4int theFirstBit;
  G4int theNBits;
  G4int theLastBit;
  G4int theMaxNFound;
};

#endif
