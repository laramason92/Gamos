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
#include "SHPrintEMDataUA.hh"
#include "G4EmCalculator.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosGenerator/include/GmGenerator.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "CLHEP/Units/SystemOfUnits.h"
#include "G4Material.hh"
#include "G4Event.hh"
#include <iomanip>

//------------------------------------------------------------------
SHPrintEMDataUA::SHPrintEMDataUA() 
{
  //  theParticleName  = GmParameterMgr::GetInstance()->GetStringValue("SHPrintEMDataUA:ParticleName","neutron");
  GetEnergyParameters("SHPrintEMDataUA");
  
  //--- for use in histogram 
  if( !bLogE ) {
    theMinE -= 0.5 * theStepE;
    theMaxE += 0.5 * theStepE;
  } else {
    theMinE = log10(theMinE) - 0.5 * theStepE;
    theMaxE = log10(theMaxE) + 0.5 * theStepE;
  } 

  theEmCalculator = new G4EmCalculator;
}


//------------------------------------------------------------------
void SHPrintEMDataUA::BeginOfRunAction(const G4Run*)
{
  SetHistoNameAndNumber(theName,theFilters,theClassifier);

}

//------------------------------------------------------------------
void SHPrintEMDataUA::BeginOfEventAction(const G4Event* evt)
{

  G4PrimaryParticle* primParticle  = evt->GetPrimaryVertex(0)->GetPrimary(0);
  G4ParticleDefinition* particleDef = primParticle->GetG4code();
  G4ThreeVector direction(1., 0., 0. ); // dummy direction  
  G4String primaryParticleName = primParticle->GetParticleDefinition()->GetParticleName();
  G4ProcessManager* pManager = particleDef->GetProcessManager();

  //--- Loop to materials 
  const G4MaterialTable* matTable = G4Material::GetMaterialTable();
  for( unsigned int imat = 0; imat < matTable->size(); imat++ ){
    G4Material* aMaterial = (*matTable)[imat];

    //--- Loop to energies
    for( size_t iener = 0; iener != theNStepsE; iener++ ) {
      G4double energy = theEnergies[iener];

      if( particleDef->GetPDGCharge() != 0 ) {
	G4double data = theEmCalculator->GetDEDX(energy, particleDef, aMaterial);
	G4cout << "@@SHPrintEMDataUA " << aMaterial->GetName() 
	       << " " << energy << " MeV" 
	       << " " << primaryParticleName
	       << " DEDX= " << data/(MeV/mm) << " MeV/mm"
	       << G4endl;
      }

      // cross section
      G4ProcessVector* pvect = pManager->GetProcessList();
      int jj, sizproc = pvect->size();
      G4double microXS = 0.;
      for( jj = 0; jj < sizproc; jj++ ) {
	G4VProcess* proc = (*pvect)[jj];

	G4double mfp = theEmCalculator->GetMeanFreePath(energy, particleDef, proc->GetProcessName(), aMaterial);
	G4double micXS = 1./mfp/aMaterial->GetTotNbOfAtomsPerVolume();
	microXS += micXS;
	G4cout << "@@SHPrintEMDataUA " << aMaterial->GetName() 
	       << " " << energy << " MeV" 
	       << " " << primaryParticleName
	       << " " << proc->GetProcessName() 
	       << " MFP= " << mfp << " mm"
	       << " MICRO_XS " << micXS/barn << " barn"
	       << G4endl;
      }

      G4cout << "@@SHPrintEMDataUA " << aMaterial->GetName() 
	       << " " << energy << " MeV" 
	       << " " << primaryParticleName
	       << " TOTAL_MICRO_XS= " << microXS/barn << " barn"
	       << G4endl;

    }
  }

}


//------------------------------------------------------------------
void SHPrintEMDataUA::EndOfRunAction(const G4Run*)
{

}
