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
/*#define private public
#include "G4EventManager.hh"
#define private private
*/

#include "GmTrajPointProcess.hh"
#include "G4Step.hh"
#include "G4VProcess.hh"
#include "G4TrackingManager.hh"
#include "G4SteppingManager.hh"
#include "G4TrackVector.hh"


//----------------------------------------------------------------------
GmTrajPointProcess::GmTrajPointProcess( const G4StepPoint* aStepPoint )
{
  if( !aStepPoint->GetProcessDefinedStep() ) {
    theProcessName = "NULL";
  } else {
    theProcessName = aStepPoint->GetProcessDefinedStep()->GetProcessName();
    //    G4cout << " GmTrajPointProcess theProcessName " <<theProcessName << G4endl; 
  }
}


//----------------------------------------------------------------------
GmTrajPointProcess::~GmTrajPointProcess()
{

}


//----------------------------------------------------------------------
std::ostream & operator<<(std::ostream & os, const GmTrajPointProcess & tp)
{
  os << "GmTrajPointProcess: ProcessName= " << tp.GetProcessName()
     << G4endl;
  return os;

}