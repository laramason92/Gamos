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
#ifndef PETVComptClassificationVariable_H
#define PETVComptClassificationVariable_H

#include "globals.hh"
#include "DetRecHitSet.hh"
#include <set>
class GmAnalysisMgr;

class PETVComptClassificationVariable 
{

public:
  PETVComptClassificationVariable();
  virtual ~PETVComptClassificationVariable(){};
  void InitVariables();

  virtual void BookHistos(G4int histoNumber);
  virtual void BookGeneralHistos(G4int histoNumber);
  virtual void BookSpecificHistos(G4int histoNumber) = 0;
  virtual G4int GetClassification( DetRecHitSet& prhs, G4int nh = -1 ) = 0;
  virtual void SetCurrentClassificationAndFillHistos( DetRecHitSet& prhs, G4int nh );
  void FillHisto( G4int hn, G4double val);
  G4int GetIndexFromValue( G4double val );

protected:
  G4String theName;
  std::vector<G4double> thes;
  G4double theMin;
  G4double theMax;
  G4double theStep;
  std::set<G4double> theIntervalLimits;
  G4int theCurrentIndex;
  GmAnalysisMgr* theAnaMgr;
};

#endif
