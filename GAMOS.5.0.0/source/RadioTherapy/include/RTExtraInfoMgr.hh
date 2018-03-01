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
#ifndef RTExtraInfoMgr_h
#define RTExtraInfoMgr_h

/*---------------------------------------------------------------------------   
ClassName:   RTExtraInfoMgr    
Author:      P. Arce
Changes:     Created. 02/2009 
---------------------------------------------------------------------------*/ 
// Description  
//----------------------------------------------- 

#include "globals.hh"
#include <vector>
#include <iostream>
#include "iaea_record.hh"
#include "RTVExtraInfoProviderLong.hh"
class RTVExtraInfoProviderFloat;

//------------------------------------------------------------------------
class RTExtraInfoMgr 
{ 
 private:
  RTExtraInfoMgr();

 public:
  ~RTExtraInfoMgr();
  //! Get the only instance 
  static RTExtraInfoMgr* GetInstance();

  G4int AddExtraInfoProviderLong( RTVExtraInfoProviderLong* eip );
  G4int AddExtraInfoProviderFloat( RTVExtraInfoProviderFloat* eip );

  std::vector<RTVExtraInfoProviderLong*> GetProvidersLong() {
    return theProvidersLong;
  }
  std::vector<RTVExtraInfoProviderFloat*> GetProvidersFloat() {
    return theProvidersFloat;
  }

  std::vector<bs32*> GetWordsLong();
  std::vector<float*> GetWordsFloat();

  void DumpProviders( std::ostream& out = G4cout );
  void DumpProvidersLong( std::ostream& out = G4cout );
  void DumpProvidersFloat( std::ostream& out = G4cout );

  void InitExtraInfo( iaea_record_type *p_iaea_read );
  void FillExtraInfo( iaea_record_type *p_iaea_read );

  long GetExtraInfoLong(int nei) const {
    return theExtraInfoLongs[nei];
  }
  float GetExtraInfoFloat(int nei) const {
    return theExtraInfoFloats[nei];
  }

  G4int GetNumberOfLongs() {
    return theExtraInfoLongs.size();
  }
  G4int GetNumberOfFloats() {
    return theExtraInfoFloats.size();
  }

private:
  static RTExtraInfoMgr* theInstance;

  std::vector<RTVExtraInfoProviderLong*> theProvidersLong;
  std::vector<RTVExtraInfoProviderFloat*> theProvidersFloat;

  std::vector<long> theExtraInfoLongs;
  std::vector<float> theExtraInfoFloats;

};

#endif
