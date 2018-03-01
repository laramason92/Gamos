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
#include "GmBSLivermoreBremsstrahlungModel.hh"
#ifndef GAMOS_NO_VERBOSE
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsVerbosity.hh"
#endif
#include "G4ParticleDefinition.hh"
#include "G4MaterialCutsCouple.hh"

#include "G4DynamicParticle.hh"
#include "G4Element.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4SemiLogInterpolation.hh"
//
#include "G4VBremAngularDistribution.hh"
#include "G4ModifiedTsai.hh"
#include "G4Generator2BS.hh"
#include "G4Generator2BN.hh"
//
#include "G4BremsstrahlungCrossSectionHandler.hh"
//
#include "G4VEnergySpectrum.hh"
#include "G4eBremsstrahlungSpectrum.hh"

#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
GmBSLivermoreBremsstrahlungModel::GmBSLivermoreBremsstrahlungModel(const G4ParticleDefinition* part,
						     const G4String& nam)
  :G4LivermoreBremsstrahlungModel(part, nam)
{
  theAnaMgr = GmAnalysisMgr::GetInstance("testBrems");
  theAnaMgr->CreateHisto1D("ENERGY LOST BY e-",120,0.,6.,98229);
  theAnaMgr->CreateHisto1D("DIR THETA ",180,0.,180.,98230);
  theAnaMgr->CreateHisto1D("ENERGY BREMS ALL",120,0.,6.,98231);
  theAnaMgr->CreateHisto1D("ENERGY BREMS ACCEPTED",120,0.,6.,98232);
  theAnaMgr->CreateHisto1D("ENERGY BREMS NOT ACCEPTED",120,0.,6.,98233);
  theAnaMgr->CreateHisto1D("ENERGY BREMS NOT ACCEPTED PASSED RR",120,0.,6.,98234);
  theAnaMgr->CreateHisto1D("ENERGY BREMS NOT ACCEPTED NOT PASSED RR",120,0.,6.,98235);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
GmBSLivermoreBremsstrahlungModel::~GmBSLivermoreBremsstrahlungModel()
{
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void GmBSLivermoreBremsstrahlungModel::
SampleSecondariesPS(std::vector<G4DynamicParticle*>* secondaries, 
		    std::vector<G4double>& secondaryWeights,
		    const G4MaterialCutsCouple* couple,
		    const G4DynamicParticle* aDynamicParticle,
		    G4double tcut,
		    G4double ,
		    const G4Track& track)
{
 //PS<
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(+testVerb) ) G4cout << " GmBSLivermoreBremsstrahlungModel::SampleSecondariesPS, start wgt " << track.GetWeight() << G4endl;
#endif

  G4ThreeVector position = track.GetPosition();
  G4bool bIsApplicable = IsApplicable(position.z());  
  // Do splitting only above plane and for FAT electrons 
  if( !bIsApplicable ) {
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb) ) G4cout << "  GmBSLivermoreBremsstrahlungModel::SampleSecondariesPS DO NOT SPLIT : "
	   << " IsApplicable? " << bIsApplicable << G4endl;
#endif
    //g494    SampleSecondaries(secondaries, currentCouple, aDynamicParticle, tcut, 100.*TeV );
    SampleSecondaries(secondaries, couple, aDynamicParticle, tcut, 100.*TeV );
    for( unsigned int ii = 0; ii < secondaries->size(); ii++ ) {
      secondaryWeights.push_back(1.);
    }
    return;
  }
  //PS>
  
  //----- Store some variables
  G4double kineticEnergy = aDynamicParticle->GetKineticEnergy();
  
  // this is needed for pathological cases of no ionisation
  if (kineticEnergy <= fIntrinsicLowEnergyLimit)
    {
      fParticleChange->SetProposedKineticEnergy(0.);
      fParticleChange->ProposeLocalEnergyDeposit(kineticEnergy);
      return;
    }
    
  theWgt = track.GetWeight();   //PS<
  theNewNSplit = theNSplit;
  if( theWgt > 0.99 ) {
    theWgt *= theNSplitInv;   
  } else {
    theNewNSplit = 1;
  }
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(+debugVerb) ) G4cout << "  GmBSLivermoreBremsstrahlungModel::SampleSecondariesPS wgt " << theWgt << " NSplit " << theNewNSplit << G4endl;
#endif
  //PS>

  G4int nCOUNT = 0;
  G4int nCOUNTACC = 0;

  //----- Loop 
  G4double totalEnergy = kineticEnergy + electron_mass_c2;
  G4ThreeVector electronDirection = aDynamicParticle->GetMomentumDirection();
  G4double tGamma = 0.;
  G4ThreeVector gammaDirection;
  G4double tGamma1st = 0.;
  G4ThreeVector gammaDirection1st;
  G4double finalEnergy = 0.; // electron final energy 

  for (G4int ii=0; ii<theNewNSplit; ii++) {   //PS
    
    //Sample material
    G4int Z = crossSectionHandler->SelectRandomAtom(couple, kineticEnergy);
    
    //Sample gamma energy
    tGamma = energySpectrum->SampleEnergy(Z, tcut, kineticEnergy, kineticEnergy);

    //nothing happens                                                      
    if (tGamma == 0.) { return; }

    G4double totalEnergy = kineticEnergy + electron_mass_c2;
    finalEnergy = kineticEnergy - tGamma; // electron final energy  

    //Sample gamma direction                                               
    G4ThreeVector gammaDirection =
    GetAngularDistribution()->SampleDirection(aDynamicParticle,
                                              totalEnergy-tGamma,
                                              Z,
                                              couple->GetMaterial());

    G4ThreeVector electronDirection = aDynamicParticle->GetMomentumDirection();

#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(+debugVerb) ) G4cout << " GmBSLivermoreBremsstrahlungModel::SampleSecondariesPS  gammaDir " << gammaDirection << " elecDir " << aDynamicParticle->GetMomentumDirection() << G4endl;
#endif
    theAnaMgr->GetHisto1(98230)->Fill(gammaDirection.theta()/deg,1.); 

    theAnaMgr->GetHisto1(98231)->Fill(tGamma,1.); 
    nCOUNT++;
    if( AcceptGamma(gammaDirection,position) ) {   //PS<
    nCOUNTACC++;
      theNewWgt = theWgt;
      theAnaMgr->GetHisto1(98232)->Fill(tGamma,theNewWgt); 
    } else {
      theAnaMgr->GetHisto1(98233)->Fill(tGamma,theWgt); 
      //- Try Russian Roulette
      
      if ( RussRoul() ) {
	theNewWgt = 1.;
	theAnaMgr->GetHisto1(98234)->Fill(tGamma,theNewWgt); 
      } else {
	theAnaMgr->GetHisto1(98235)->Fill(tGamma,theWgt); 
	continue;
      }
    }    //PS>
    
    /*PS    if (finalEnergy < 0.) 
      {
      // Kinematic problem
      tGamma = kineticEnergy;
      fParticleChange->SetProposedKineticEnergy(0.);
      }
      else 
      {
      //do it only once  //PS
      	G4double momentum = std::sqrt((totalEnergy + electron_mass_c2)*kineticEnergy);
      G4double finalX = momentum*electronDirection.x() - tGamma*gammaDirection.x();
      G4double finalY = momentum*electronDirection.y() - tGamma*gammaDirection.y();
      G4double finalZ = momentum*electronDirection.z() - tGamma*gammaDirection.z();
      G4double norm = 1./std::sqrt(finalX*finalX + finalY*finalY + finalZ*finalZ);
      
      fParticleChange->ProposeMomentumDirection(finalX*norm, finalY*norm, finalZ*norm);
      fParticleChange->SetProposedKineticEnergy(finalEnergy);
      }   PS*/
    
    //Generate the bremsstrahlung gamma
    G4DynamicParticle* aGamma= new G4DynamicParticle (G4Gamma::Gamma(),
						      gammaDirection, tGamma);
    secondaries->push_back(aGamma);
    secondaryWeights.push_back( theNewWgt );
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(+debugVerb) ) G4cout << "GmBSLivermoreBremsstrahlungModel new particle of weight " << theNewWgt << " " << theNewNSplit << G4endl;
#endif
  }
  
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(+debugVerb) ) G4cout << "GmBSLivermoreBremsstrahlungModel N SECOS = " << secondaries->size() << G4endl;
#endif
  //  G4cout << " nCOUNT " << nCOUNT << G4endl;
  // if( nCOUNTACC != 0 )  G4cout << " nCOUNTACC " << nCOUNTACC << G4endl;

  //t  if( secondaries->size() > 0 ) {
    G4double momentum = std::sqrt((totalEnergy + electron_mass_c2)*kineticEnergy);
    G4double finalX = momentum*electronDirection.x() - tGamma1st*gammaDirection1st.x();
    G4double finalY = momentum*electronDirection.y() - tGamma1st*gammaDirection1st.y();
    G4double finalZ = momentum*electronDirection.z() - tGamma1st*gammaDirection1st.z();
    G4double norm = 1./std::sqrt(finalX*finalX + finalY*finalY + finalZ*finalZ);
    
    fParticleChange->ProposeMomentumDirection(finalX*norm, finalY*norm, finalZ*norm);
    fParticleChange->SetProposedKineticEnergy(finalEnergy); 
    theAnaMgr->GetHisto1(98229)->Fill(tGamma1st,theNewWgt); 
	// }  

#ifndef GAMOS_NO_VERBOSE
  if (verboseLevel > 1)
    {
      G4cout << "-----------------------------------------------------------" << G4endl;
      G4cout << "Energy balance from G4LivermoreBremsstrahlung" << G4endl;
      G4cout << "Incoming primary energy: " << kineticEnergy/keV << " keV" << G4endl;
      G4cout << "-----------------------------------------------------------" << G4endl;
      G4cout << "Outgoing primary energy: " << finalEnergy/keV << " keV" << G4endl;
      G4cout << "Gamma ray " << tGamma/keV << " keV" << G4endl;
      G4cout << "Total final state: " << (finalEnergy+tGamma)/keV << " keV" << G4endl;
      G4cout << "-----------------------------------------------------------" << G4endl;
    }

  if (verboseLevel > 0)
    {
      G4double energyDiff = std::fabs(finalEnergy+tGamma-kineticEnergy);
      if (energyDiff > 0.05*keV)
	G4cout << "G4LivermoreBremsstrahlung WARNING: problem with energy conservation: " 
	       << (finalEnergy+tGamma)/keV << " keV (final) vs. " 
	       << kineticEnergy/keV << " keV (initial)" << G4endl;
    }
#endif

  return;
}
