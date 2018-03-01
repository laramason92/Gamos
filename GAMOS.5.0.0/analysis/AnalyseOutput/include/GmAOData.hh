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
#ifndef GmAOData_hh
#define GmAOData_hh

#include <vector>
#include <map>
#include "globals.hh"
class GmAODataSearch;

//------------------------------------------------------------------------
class GmAOData 
{ 
public:
  GmAOData( const std::vector<G4String>& wl, GmAODataSearch* ds, G4String fname );
  G4String GetMessage() const;
  void PrintData() const;

  std::vector<G4String> GetLine() const 
  {
    return theLine;
  }

  G4String GetWord(G4int ) const;

  GmAODataSearch* GetDataSearch() const
  {
    return theDataSearch;
  }

  G4int GetNEvents();

  static std::map<G4String,G4int> theNEvents; // filename - number of events processed

 private:
  std::vector<G4String> theLine;
  GmAODataSearch* theDataSearch;
  G4String theFileName;

};

#endif
