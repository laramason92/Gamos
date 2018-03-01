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
#ifndef DetCutsStudyFilter_HH
#define DetCutsStudyFilter_HH
// Filter tracks looking if it is making a hit (= step in sensitive detector with deposited energy != 0 )

#include "GamosCore/GamosPhysics/Cuts/include/GmCutsStudyMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

#include "globals.hh"
#include "G4String.hh"
#include <iostream>

class DetCutsStudyFilter : public GmVFilter {

public: // With description

  // Construct with filter name
  DetCutsStudyFilter(const G4String& name);

  virtual ~DetCutsStudyFilter(){};
  
  // Filter method
  virtual G4bool AcceptStep(const G4Step*);

  // Print configuration
  virtual void PrintAll(std::ostream& ) const{};
  
  // Reset 
  virtual void Reset(){};

private:

};

#endif
