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
// --------------------------------------------------------------
#ifndef RTExtraInfoTrackInfo_hh
#define RTExtraInfoTrackInfo_hh
#include "iaea_record.hh"

enum CutsTI{ CTI_ProdCuts = -1, CTI_MinRange = -2, CTI_RangeRejection = -3 };

#include "G4VUserTrackInformation.hh"

class RTExtraInfoTrackInfo : public G4VUserTrackInformation
{
public:

  RTExtraInfoTrackInfo( iaea_record_type *p_iaea_read ); 

  virtual ~RTExtraInfoTrackInfo();

  void Print() const {};

  long GetExtraInfoLong1() const {
    return theExtraInfoLong1;
  }
  long GetExtraInfoLong2() const {
    return theExtraInfoLong2;
  }
  float GetExtraInfoFloat1() const {
    return theExtraInfoFloat1;
  }
  float GetExtraInfoFloat2() const {
    return theExtraInfoFloat2;
  }

private:
  long theExtraInfoLong1;
  long theExtraInfoLong2;
  float theExtraInfoFloat1;
  float theExtraInfoFloat2;

};

#endif
