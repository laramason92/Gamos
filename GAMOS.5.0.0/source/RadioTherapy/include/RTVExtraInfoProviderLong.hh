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
#ifndef RTVExtraInfoProviderLong_HH
#define RTVExtraInfoProviderLong_HH

#include "globals.hh"
#include <vector>
#include <bitset>
#include "RTVExtraInfoProvider.hh"

typedef std::bitset<32> bs32;
 
class RTVExtraInfoProviderLong : public RTVExtraInfoProvider
{
public: // With description

  // Construct with filter name
  RTVExtraInfoProviderLong();

  virtual ~RTVExtraInfoProviderLong(){};

  virtual void InitVariables();

  virtual void DumpIndices(std::ostream& out);

  void SetWordIndex( G4int index );

  static std::vector<bs32*> GetWords(){
    return theWords;
  }

  std::vector<G4String> GetIndexNames() const {
    return theIndexNames;
  }
 
  virtual G4String GetIndexName( G4int index ) const = 0;
 
  void SetWordsReset( G4bool wr ){
    bWordsReset = wr;
  }

  G4int GetShift() const {
    return theShift;
  }

  G4int GetNBits() const {
    return theNBits;
  }

  G4bool GetWordsReset() const {
    return  bWordsReset;
  }

protected:
  std::vector<G4String> theIndexNames;

  static std::vector<bs32*> theWords;

  G4int theShift;
  G4int theNBits;
  G4bool bWordsReset;

};

#endif
