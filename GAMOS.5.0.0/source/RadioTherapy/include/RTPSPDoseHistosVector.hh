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
#ifndef RTPSPDoseHistosVector_hh
#define RTPSPDoseHistosVector_hh 1

#include <vector>
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "GamosCore/GamosScoring/Management/include/GmVPSPrinter.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
class GmVPrimitiveScorerVector;
class GmSqdoseHeader;
class G4PhantomParameterisation;
// class description:
//
// This class fill histograms to get the PDD in a phantom

class RTPSPDoseHistosVector : public GmVPSPrinter
{
public: // with description
  RTPSPDoseHistosVector(G4String name, GmSqdoseHeader* doseh= 0);
  virtual ~RTPSPDoseHistosVector(){ };
  
  virtual void DumpAll( G4THitsMap<G4double>* , GmVPrimitiveScorer* ){};
  virtual void DumpAllVector( std::vector<G4double>*, GmVPrimitiveScorerVector* );


private:
  void BookHistos();
  void FillHisto1( GmHisto1* his, G4int ibin, G4double val, G4double error );
 
  void CheckHistoLimits( G4int nXmin, G4int nXmax, G4int nYmin, G4int nYmax, G4int nZmin, G4int nZmax );
  void FillHisto( std::vector<G4String>& wl );
  void FillHisto1N_X( std::vector<G4String>& wl  );
  void FillHisto1N_Y( std::vector<G4String>& wl  );
  void FillHisto1N_Z( std::vector<G4String>& wl  );
  void FillHisto1N_X( const G4String& hisName, G4int nXmin, G4int nXmax, G4int nYmin, G4int nYmax, G4int nZmin, G4int nZmax );
  void FillHisto1N_Y( const G4String& hisName, G4int nYmin, G4int nYmax, G4int nXmin, G4int nXmax, G4int nZmin, G4int nZmax );
  void FillHisto1N_Z( const G4String& hisName, G4int nZmin, G4int nZmax, G4int nXmin, G4int nXmax, G4int nYmin, G4int nYmax );
  void FillHisto2N_XY( std::vector<G4String>& wl  );
  void FillHisto2N_XZ( std::vector<G4String>& wl  );
  void FillHisto2N_YZ( std::vector<G4String>& wl  );
  void FillHisto2N_XY( const G4String& hisName, G4int nXmin, G4int nXmax, G4int nYmin, G4int nYmax, G4int nZmin, G4int nZmax );
  void FillHisto2N_XZ( const G4String& hisName, G4int nXmin, G4int nXmax, G4int nZmin, G4int nZmax, G4int nYmin, G4int nYmax );
  void FillHisto2N_YZ( const G4String& hisName, G4int nYmin, G4int nYmax, G4int nZmin, G4int nZmax, G4int nXmin, G4int nXmax );
  G4double ConvertN2DimX(G4int nX);
  G4double ConvertN2DimY(G4int nY);
  G4double ConvertN2DimZ(G4int nZ);

private:
  GmAnalysisMgr* theAnaMgr;

  G4PhantomParameterisation* theRegParam;
  GmSqdoseHeader* theDoseHeader;
  G4ThreeVector thePhantomTranslation;

  G4int theNHistos;
  G4double theDimX;
  G4double theDimY;
  G4double theDimZ;
  G4int theNVoxelX;
  G4int theNVoxelY;
  G4int theNVoxelZ;
  G4double theNevInv;
  G4double theUnitInv;

  std::vector<G4double> theMap;

  GmVPrimitiveScorerVector* theScorer;
};

#endif
