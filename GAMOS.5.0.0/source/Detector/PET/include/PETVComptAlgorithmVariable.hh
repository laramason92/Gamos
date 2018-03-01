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
#ifndef PETVComptAlgorithmVariable_H
#define PETVComptAlgorithmVariable_H

#include "globals.hh"
#include "DetRecHitSet.hh"
#include <set>
class GmAnalysisMgr;
class GmTrajStep;
typedef std::multimap<G4int,GmTrajStep*> mmits;

class PETVComptAlgorithmVariable 
{

public:
  PETVComptAlgorithmVariable();
  virtual ~PETVComptAlgorithmVariable(){};
  void InitVariables();

  virtual void BookHistos(G4int histoNumber);

  virtual void FillHistosFromValues(std::vector<G4double>& vars, G4int nh);
  virtual void FillHistosInteractions( std::pair<mmits::iterator,mmits::iterator> itemmp, G4int nh ) = 0;
  virtual void FillHistosRHits( const std::vector<std::pair<GmTrajStep*,GmRecHit*> >& recHitOrdered, G4int nh ) = 0;

protected:
  G4String theName;
  G4double theValueMin;
  G4double theValueMax;
  GmAnalysisMgr* theAnaMgr;

  G4int theGroupingNumber;

};

#endif
