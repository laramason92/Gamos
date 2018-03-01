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
#include <math.h>
//#define protected public
//#define private public
#include "G4NeutronHPFinalState.hh"

#include "SHPlotPHPDBSpectraUA.hh"
#include "SHUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosGenerator/include/GmGenerator.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "G4Material.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4RunManager.hh"
#include "G4ElementVector.hh"
#include "G4HadronicProcess.hh"
#include "G4UserRunAction.hh"

#define private public
#include "G4NeutronHPThermalBoost.hh"
#include "G4NeutronHPInelastic.hh"
#include "G4NeutronHPCapture.hh"
#include "G4NeutronHPChannelList.hh"
#include "G4NeutronHPChannel.hh"
#include "G4ParticleHPInelastic.hh"
#include "G4ParticleHPChannelList.hh"
#include "G4ParticleHPChannel.hh"
#include "G4NeutronHPInelasticCompFS.hh"
#include "G4NeutronHPInelasticBaseFS.hh"
#include "G4tgrFileIn.hh"
#include "G4HadronicProcessStore.hh"
#include "G4ReactionProduct.hh"

#include <iomanip>

//------------------------------------------------------------------
SHPlotPHPDBSpectraUA::SHPlotPHPDBSpectraUA() 
{
  //  theParticleName  = GmParameterMgr::GetInstance()->GetStringValue("SHPlotPHPDBSpectraUA:ParticleName","neutron");
  GetEnergyParameters("SHPlotPHPDBSpectraUA");
  
  if( !bLogE ) {
    theMinE -= 0.5 * theStepE;
    theMaxE += 0.5 * theStepE;
  } else {
    theMinE = log10(theMinE) - 0.5 * theStepE;
    theMaxE = log10(theMaxE) + 0.5 * theStepE;
  } 
  //  G4cout << " MINE " << theMinE << " " << theMaxE << " " << theStepE << G4endl;

}


//------------------------------------------------------------------
void SHPlotPHPDBSpectraUA::BeginOfRunAction(const G4Run*)
{
  theNEvents = G4int(GmParameterMgr::GetInstance()->GetNumericValue(theName+":NEvents",1000));

  theMateIdxMax = G4int(GmParameterMgr::GetInstance()->GetNumericValue(theName+":MaterialIndexMax",5));
  theIsotIdxMax = G4int(GmParameterMgr::GetInstance()->GetNumericValue(theName+":IsotopeIndexMax",20));
  theProdIdxMax = G4int(GmParameterMgr::GetInstance()->GetNumericValue(theName+":ProductIndexMax",50));
  theProcIdxMax = G4int(GmParameterMgr::GetInstance()->GetNumericValue(theName+":ProcessIndexMax",4));

  theProductMaxAZ = G4int(GmParameterMgr::GetInstance()->GetNumericValue(theName+":ProductMaxAZ",9999999));
  SetHistoNameAndNumber(theName,theFilters,theClassifier);

  SHUtils::BuildElementNames();

}

//------------------------------------------------------------------
void SHPlotPHPDBSpectraUA::BeginOfEventAction(const G4Event* evt)
{

  //  TFile* rootFile = new TFile((theName+".root").c_str(),"recreate");

  G4PrimaryParticle* primParticle  = evt->GetPrimaryVertex(0)->GetPrimary(0);
  G4ParticleDefinition* particleDef = primParticle->GetG4code();
  G4ThreeVector direction(1., 0., 0. ); // dummy direction
  
  thePrimaryParticleName = primParticle->GetParticleDefinition()->GetParticleName();
  SHUtils::StoreHadronicProcesses(thePrimaryParticleName);
  SHUtils::BuildElementNames();

  //--- Loop to materials 
  const G4MaterialTable* matTable = G4Material::GetMaterialTable();
  if( G4int(matTable->size()) > theMateIdxMax ) {
    G4Exception("SHPlotPHPDBSpectraUA",
		"Fatal error",
		FatalException,
		G4String("Number of materials too big, change it with parameter /gamos/setParam " + theName + ":MaterialIndexMax  VALUE ").c_str());
  }
  if( SHUtils::GetNHadronicProcesses() > theProcIdxMax ){
    G4Exception("SHPlotPHPDBSpectraUA",
		"Fatal error",
		FatalException,
		G4String("Number of hadronic processes too big, change it with parameter /gamos/setParam " + theName + ":ProcessIndexMax  VALUE ").c_str());
  }
  
  for( unsigned int imat = 0; imat < matTable->size(); imat++ ){
    G4Material* material = (*matTable)[imat];

    size_t nelm = material->GetNumberOfElements();
    //    G4double aTemp = material->GetTemperature(); 
    G4ElementVector* elementVector = const_cast<G4ElementVector*>(material->GetElementVector());
    G4double* atomNumDensityVector = const_cast<G4double*>(material->GetVecNbOfAtomsPerVolume());
    G4double nAtomsPermm3 = 0;
    for (size_t iele=0; iele<nelm; iele++) {
      nAtomsPermm3 += atomNumDensityVector[iele];
    }
    //--- Loop to hadronic processes 
    for( G4int iproc = 0; iproc < SHUtils::GetNHadronicProcesses(); iproc++ ){
      G4HadronicProcess* hadProc = SHUtils::GetHadronicProcess(iproc);

      //--- Loop to energies
      for( size_t iener = 0; iener != theNStepsE; iener++ ) {
	G4double ener = theEnergies[iener];

	//--- Loop to elements (isotopes) in material
	G4DynamicParticle* aParticle = new G4DynamicParticle( particleDef, direction, ener);
	//-- Inelastic
	G4HadronicInteraction* hadModel = hadProc->ChooseHadronicInteraction(ener, material, (*elementVector)[0]);
	if( hadModel->GetModelName() == "ParticleHPInelastic" ){
	  if( SHUtils::GetNChannelParticles() == 0 ) SHUtils::BuildChannelList(primParticle->GetParticleDefinition(), theName);

	  G4HadProjectile hadProj(*aParticle);
	  TH1* hisE;
	  TH1* hisTh;
	  TH1* hisEALL;
	  TH1* hisThALL;

	  //--- loop to elements 
	  for (size_t iele=0; iele<nelm; iele++) {
	    G4Element* elem = (*elementVector)[iele];
	    //	    G4cout << " LOOPING ELEMENT " << elem->GetName() << G4endl;
	    G4int index = elem->GetIndex();

	    G4HadronicInteraction* hadInteraction = hadProc->ChooseHadronicInteraction(ener, material, elem );
	    G4ParticleHPInelastic* HPinelas = (G4ParticleHPInelastic*)(hadInteraction);
	    G4ParticleHPChannel ** HPChannels = HPinelas->theInelastic[index]->theChannels;

	    //	    G4cout << " GET hadInteraction " << hadInteraction << " HPinelas " << HPinelas << " HPChannels " <<  HPChannels << G4endl;

	    G4int nChannels = HPinelas->theInelastic[index]->nChannels;
	    G4int numberOfIsos = HPChannels[0]->GetNiso(); // all channels have the same number of isotopes??
	    if( numberOfIsos > theIsotIdxMax ) {
	      G4Exception("SHPlotPHPDBSpectraUA",
			  "Fatal error",
			  FatalException,
			  G4String("Number of isotopes too big, change it with parameter /gamos/setParam " + theName + ":IsotopeIndexMax  VALUE ").c_str());
	    }
	    
	    for( G4int ii = 0; ii < numberOfIsos; ii++ ) {
	      G4String isotName = elem->GetIsotope(ii)->GetName();
	      size_t ifi = isotName.find("_I");
	      if( ifi != G4String::npos ) isotName = isotName.substr(0,ifi);
	      
	      for(G4int ic = 0; ic < nChannels; ic++) {
		G4ParticleHPChannel* HPChannel = HPChannels[ic];
		if( HPChannel->HasFSData(ii) ) {// one channel per isotope , one final state per isotope

		  G4double FSXS = HPChannel->GetFSCrossSection(ener, ii);

		  G4String FSType = HPChannel->theFSType;
		  std::map<G4String, ChannelData >::const_iterator ite = SHUtils::GetChannelParticle(FSType.substr(0,FSType.length()-1));
		  G4String FSName = (*ite).second.name;
		  //		  G4cout << " FSNAME " << (*ite).first << " " << (*ite).second.name << G4endl;
		  //		  G4cout << GmGenUtils::ftoa(ener) + "_" + hadProc->GetProcessName() + "_" + material->GetName() + "_" + isotName + "_" + FSName << " XS= " << FSXS << G4endl;;
		  
		  if( FSXS == 0. )  continue;
		  
		  G4ParticleHPFinalState * FS = HPChannel->theFinalStates[ii];
		  
		  G4String channelName = SHUtils::GetChannelName(int(FS->theBaseZ), int(FS->theBaseA), HPChannel->theFSType.substr(0,3), GmG4Utils::GetParticleShortName(thePrimaryParticleName) );
		  
		  G4ParticleHPEnAngCorrelation* energyAngData;
		  G4ReactionProduct * aTarget = new G4ReactionProduct();

		  aTarget->SetMass(FS->theBaseA); //t

		  if( FSType == "F01/" || FSType == "F23/" || FSType == "F24/" || FSType == "F25/" || FSType == "F26/" || FSType == "F27/" ){
		    G4ParticleHPInelasticCompFS* FSC = (G4ParticleHPInelasticCompFS*)(FS);
		    G4int it = FSC->SelectExitChannel(ener);
		    energyAngData = FSC->theEnergyAngData[it];
		    //		    G4cout << " Com SET TARGET " << energyAngData << G4endl;
		    if( !energyAngData ) continue;
		    energyAngData->SetTarget(*aTarget);
		    G4ReactionProduct incidentProduct(particleDef);
		    energyAngData->SetProjectileRP(incidentProduct);
		    //		    G4cout << FSC << " Use CompFS theEnergyAngData " << it << " " << FSC->theEnergyAngData[it] << G4endl;

		  } else {
		    G4ParticleHPInelasticBaseFS* FSB = (G4ParticleHPInelasticBaseFS*)(FS);
		    energyAngData = FSB->theEnergyAngData;
		    energyAngData->SetTarget(*aTarget);
		    G4ReactionProduct incidentProduct(particleDef);
		    energyAngData->SetProjectileRP(incidentProduct);
		    //		    G4cout << " Base SET TARGET " << energyAngData << G4endl;
		    if( !energyAngData ) continue;
		    //		    G4cout << FSB << " Use BaseFS theEnergyAngData " << FSB->theEnergyAngData << G4endl;
		  }

		  G4int nProducts = energyAngData->nProducts;
		  G4ParticleHPProduct * products = energyAngData->theProducts;
		  
		  for(G4int ip=0; ip<nProducts; ip++) {
		    G4ParticleHPProduct* product = &(products[ip]);
		    if( product->theMassCode > theProductMaxAZ ) continue;
		    G4String productName = SHUtils::GetProductName( product->theMassCode );
		    G4VParticleHPEnergyAngular * dist = product->theDist; 
		    G4String reactionName = "(" + thePrimaryParticleName + "," + productName + ")"; 
		    G4String hname = GmGenUtils::ftoa(ener) + "_" + hadProc->GetProcessName() + "_" + material->GetName() + "_" + channelName + "_" + productName;
		    G4int iprodu = SHUtils::GetProductID( product->theMassCode );
		    if( iprodu > theProdIdxMax ) {
		      G4Exception("SHPlotPHPDBSpectraUA",
				  "Fatal error",
				  FatalException,
				  G4String("Number of products too big, change it with parameter /gamos/setParam " + theName + ":ProductIndexMax  VALUE ").c_str());
		    }


		    G4int histoN = theHistoNumber 
		      + iener*theProcIdxMax*theMateIdxMax*theIsotIdxMax*30*theProdIdxMax*10 
		      + iprodu*theProcIdxMax*theMateIdxMax*theIsotIdxMax*30*10 
		      + (ic+1)*theProcIdxMax*theMateIdxMax*theIsotIdxMax*10  
		      + ii*theProcIdxMax*theMateIdxMax*10  
		      + imat*theProcIdxMax*10  
		      + iproc*10;
		    theAnaMgr->CreateHisto1D((hname+":Energy").c_str(), 100, 0, theMaxE, histoN );
		    hisE = theAnaMgr->GetHisto1(histoN);
		    theAnaMgr->CreateHisto1D((hname+":Theta").c_str(), 100, 0, 180., histoN+1 );
		    hisTh = theAnaMgr->GetHisto1(histoN+1);
		    G4int histoNAllChann = theHistoNumber 
		      + iener*theProcIdxMax*theMateIdxMax*theIsotIdxMax*30*theProdIdxMax*10 
		      + iprodu*theProcIdxMax*theMateIdxMax*theIsotIdxMax*30*10 
		      + 0*theProcIdxMax*theMateIdxMax*theIsotIdxMax*10 
		      + ii*theProcIdxMax*theMateIdxMax*10  
		      + imat*theProcIdxMax*10 
		      + iproc*10;
		    hname = GmGenUtils::ftoa(ener) + "_" + hadProc->GetProcessName() + "_" + material->GetName() + "_" + isotName + "_ALL_" + productName;

		    hisEALL = theAnaMgr->GetHisto1(histoNAllChann,0);
		    if( !hisEALL ) {
		      theAnaMgr->CreateHisto1D((hname+":Energy").c_str(), 100, 0, theMaxE, histoNAllChann );
		      theAnaMgr->CreateHisto1D((hname+":Theta").c_str(), 100, 0, 180., histoNAllChann+1 );
		      hisEALL = theAnaMgr->GetHisto1(histoNAllChann);
		    }
		    hisThALL = theAnaMgr->GetHisto1(histoNAllChann+1);
		    dist->SetTarget(aTarget);
		    G4ReactionProduct incidentProduct(particleDef);
		    dist->SetProjectileRP(&incidentProduct);
		    for( G4int jj= 0; jj < theNEvents; jj++ ){
		      G4ReactionProduct * reacProd = dist->Sample(ener, product->theMassCode, product->theMass);
		      dist->ClearHistories();
		      G4double energy2ary = reacProd->GetKineticEnergy();
		      //		      G4cout << FSType << " " << ip << " Ei= " << ener << " ENERGY PRODUCT " << product->theMassCode << " " << product->theMass << " Ep= " << energy2ary << G4endl;
		      hisE->Fill(energy2ary);
		      hisTh->Fill(reacProd->GetMomentum().theta()/deg);
		      hisEALL->Fill(energy2ary);
		      hisThALL->Fill(reacProd->GetMomentum().theta()/deg);
		    }
		  }
  
		  G4cout << "@@SHPlotPHPDBSpectraUA " << material->GetName() 
			 << " " << ener << " MeV" 
			 << " " << hadProc->GetProcessName() 
		    //			 << " CHANNEL= " << HPChannel->theFSType
		    //			 << " ISOTOPE= " << ii << " Z/A= " << FS->theBaseZ << "/" << FS->theBaseA 
		    //			 << GetChannelName(FS->theBaseZ, FS->theBaseA, HPChannel->theFSType.substr(0,3) )
			 << " CHANNEL= " << HPChannel->theFSType << " " << channelName
			 << G4endl;
		  /*		  G4int hisNumber = theHistoNumber+imat*100000+iproc*10000+ii*100+ic*1;
		  GmHisto1* his = theAnaMgr->GetHisto1(hisNumber,0);
		  if( his == 0) {
		    G4String hname = material->GetName() + "_" + channelName + "_" + hadProc->GetProcessName();
		    theAnaMgr->CreateHisto1D(hname.c_str(), theNStepsE, theMinE, theMaxE, hisNumber);
		      his = theAnaMgr->GetHisto1(hisNumber);
		    }
		    if( !bLogE ) {
		      his->Fill( ener,FSXS );		 
		    } else {
		      his->Fill( log10(ener),FSXS );		 
		      }*/
		}
	      } // numberOfChannels
	    } // numberOfIsos 
	  }

	} else if( hadModel->GetModelName() == "NeutronHPInelastic" ){
	  if( SHUtils::GetNChannelParticles() == 0 ) SHUtils::BuildChannelList(primParticle->GetParticleDefinition(), theName);

	  G4HadProjectile hadProj(*aParticle);
	  TH1* hisE;
	  TH1* hisTh;
	  TH1* hisEALL;
	  TH1* hisThALL;

	  //--- loop to elements 
	  for (size_t iele=0; iele<nelm; iele++) {
	    G4Element* elem = (*elementVector)[iele];
	    //	    G4cout << " LOOPING ELEMENT " << elem->GetName() << G4endl;
	    G4int index = elem->GetIndex();

	    G4HadronicInteraction* hadInteraction = hadProc->ChooseHadronicInteraction(ener, material, elem );
	    G4NeutronHPInelastic* HPinelas = (G4NeutronHPInelastic*)(hadInteraction);
	    G4NeutronHPChannel ** HPChannels = HPinelas->theInelastic[index]->theChannels;

	    //	    G4cout << " GET hadInteraction " << hadInteraction << " HPinelas " << HPinelas << " HPChannels " <<  HPChannels << G4endl;

	    G4int nChannels = HPinelas->theInelastic[index]->nChannels;
	    G4int numberOfIsos = HPChannels[0]->GetNiso(); // all channels have the same number of isotopes??
	    if( numberOfIsos > theIsotIdxMax ) {
	      G4Exception("SHPlotPHPDBSpectraUA",
			  "Fatal error",
			  FatalException,
			  G4String("Number of isotopes too big, change it with parameter /gamos/setParam " + theName + ":IsotopeIndexMax  VALUE ").c_str());
	    }
	    
	    for( G4int ii = 0; ii < numberOfIsos; ii++ ) {
	      G4String isotName = elem->GetIsotope(ii)->GetName();
	      size_t ifi = isotName.find("_I");
	      if( ifi != G4String::npos ) isotName = isotName.substr(0,ifi);
	      
	      for(G4int ic = 0; ic < nChannels; ic++) {
		G4NeutronHPChannel* HPChannel = HPChannels[ic];
		if( HPChannel->HasFSData(ii) ) {// one channel per isotope , one final state per isotope
		  G4double FSXS = HPChannel->GetFSCrossSection(ener, ii);

		  G4String FSType = HPChannel->theFSType;
		  std::map<G4String, ChannelData >::const_iterator ite = SHUtils::GetChannelParticle(FSType.substr(0,FSType.length()-1));
		  G4String FSName = (*ite).second.name;
		  //		  G4cout << " FSNAME " << (*ite).first << " " << (*ite).second.name << G4endl;
		  //		  G4cout << GmGenUtils::ftoa(ener) + "_" + hadProc->GetProcessName() + "_" + material->GetName() + "_" + isotName + "_" + FSName << " XS= " << FSXS << G4endl;;

		  if( FSXS == 0. )  continue;
		  
		  G4NeutronHPFinalState * FS = HPChannel->theFinalStates[ii];
		  
		  G4String channelName = SHUtils::GetChannelName(int(FS->theBaseZ), int(FS->theBaseA), HPChannel->theFSType.substr(0,3), GmG4Utils::GetParticleShortName(thePrimaryParticleName ) );
		  
		  G4NeutronHPEnAngCorrelation* energyAngData;
		  G4ReactionProduct * aTarget = new G4ReactionProduct();

		  aTarget->SetMass(FS->theBaseA); //t

		  if( FSType == "F01/" || FSType == "F23/" || FSType == "F24/" || FSType == "F25/" || FSType == "F26/" || FSType == "F27/" ){
		    G4NeutronHPInelasticCompFS* FSC = (G4NeutronHPInelasticCompFS*)(FS);
		    G4int it = FSC->SelectExitChannel(ener);
		    energyAngData = FSC->theEnergyAngData[it];
		    //		    G4cout << " Com SET TARGET " << energyAngData << G4endl;
		    if( !energyAngData ) continue;
		    energyAngData->SetTarget(*aTarget);
		    G4ReactionProduct incidentProduct(particleDef);
		    energyAngData->SetNeutron(incidentProduct);
		    //		    G4cout << FSC << " Use CompFS theEnergyAngData " << it << " " << FSC->theEnergyAngData[it] << G4endl;

		  } else {
		    G4NeutronHPInelasticBaseFS* FSB = (G4NeutronHPInelasticBaseFS*)(FS);
		    energyAngData = FSB->theEnergyAngData;
		    //		    G4cout << " Base SET TARGET " << energyAngData << G4endl;
		    if( !energyAngData ) continue;
		    energyAngData->SetTarget(*aTarget);
		    G4ReactionProduct incidentProduct(particleDef);
		    energyAngData->SetNeutron(incidentProduct);
		    //		    G4cout << FSB << " Use BaseFS theEnergyAngData " << FSB->theEnergyAngData << G4endl;
		  }

		  G4int nProducts = energyAngData->nProducts;
		  G4NeutronHPProduct * products = energyAngData->theProducts;
		  
		  for(G4int ip=0; ip<nProducts; ip++) {
		    G4NeutronHPProduct* product = &(products[ip]);
		    if( product->theMassCode > theProductMaxAZ ) continue;
		    G4String productName = SHUtils::GetProductName( product->theMassCode );
		    G4VNeutronHPEnergyAngular * dist = product->theDist; 
		    G4String reactionName = "(" + thePrimaryParticleName + "," + productName + ")"; 
		    G4String hname = GmGenUtils::ftoa(ener) + "_" + hadProc->GetProcessName() + "_" + material->GetName() + "_" + channelName + "_" + productName;
		    G4int iprodu = SHUtils::GetProductID( product->theMassCode );
		    G4int histoN = theHistoNumber 
		      + iener*theProcIdxMax*theMateIdxMax*theIsotIdxMax*30*theProdIdxMax*10 
		      + iprodu*theProcIdxMax*theMateIdxMax*theIsotIdxMax*30*10 
		      + (ic+1)*theProcIdxMax*theMateIdxMax*theIsotIdxMax*10  
		      + ii*theProcIdxMax*theMateIdxMax*10  
		      + imat*theProcIdxMax*10  
		      + iproc*10;
		    //		    G4cout << hname << " HISTO N E= " << iener << " PRODU= " << iprodu << " CHAN= " << ic << " ISOT= " << ii << " IMAT= " << imat << " PROC= " << iproc << G4endl;
		    theAnaMgr->CreateHisto1D((hname+":Energy").c_str(), 100, 0, theMaxE, histoN );
		    hisE = theAnaMgr->GetHisto1(histoN);
		    theAnaMgr->CreateHisto1D((hname+":Theta").c_str(), 100, 0, 180., histoN+1 );
		    hisTh = theAnaMgr->GetHisto1(histoN+1);
		    G4int histoNAllChann = theHistoNumber 
		      + iener*theProcIdxMax*theMateIdxMax*theIsotIdxMax*30*theProdIdxMax*10 
		      + iprodu*theProcIdxMax*theMateIdxMax*theIsotIdxMax*30*10 
		      + 0*theProcIdxMax*theMateIdxMax*theIsotIdxMax*10 
		      + ii*theProcIdxMax*theMateIdxMax*10  
		      + imat*theProcIdxMax*10 
		      + iproc*10;
		    hname = GmGenUtils::ftoa(ener) + "_" + hadProc->GetProcessName() + "_" + material->GetName() + "_" + isotName + "_ALL_" + productName;

		    hisEALL = theAnaMgr->GetHisto1(histoNAllChann,0);
		    if( !hisEALL ) {
		      theAnaMgr->CreateHisto1D((hname+":Energy").c_str(), 100, 0, theMaxE, histoNAllChann );
		      theAnaMgr->CreateHisto1D((hname+":Theta").c_str(), 100, 0, 180., histoNAllChann+1 );
		      hisEALL = theAnaMgr->GetHisto1(histoNAllChann);
		    }
		    hisThALL = theAnaMgr->GetHisto1(histoNAllChann+1);
		    dist->SetTarget(aTarget);
		    G4ReactionProduct incidentProduct(particleDef);
		    dist->SetNeutron(&incidentProduct);
		    for( G4int jj= 0; jj < theNEvents; jj++ ){
		      G4ReactionProduct * reacProd = dist->Sample(ener, product->theMassCode, product->theMass);
		      dist->ClearHistories();
		      G4double energy2ary = reacProd->GetKineticEnergy();
		      //		      G4cout << FSType << " " << ip << " Ei= " << ener << " ENERGY PRODUCT " << product->theMassCode << " " << product->theMass << " Ep= " << energy2ary << G4endl;
		      hisE->Fill(energy2ary);
		      hisTh->Fill(reacProd->GetMomentum().theta()/deg);
		      hisEALL->Fill(energy2ary);
		      hisThALL->Fill(reacProd->GetMomentum().theta()/deg);
		    }
		  }
  
		  G4cout << "@@SHPlotPHPDBSpectraUA " << material->GetName() 
			 << " " << ener << " MeV" 
			 << " " << hadProc->GetProcessName() 
		    //			 << " CHANNEL= " << HPChannel->theFSType
		    //			 << " ISOTOPE= " << ii << " Z/A= " << FS->theBaseZ << "/" << FS->theBaseA 
		    //			 << GetChannelName(FS->theBaseZ, FS->theBaseA, HPChannel->theFSType.substr(0,3) )
			 << " CHANNEL= " << HPChannel->theFSType << " " << channelName
			 << G4endl;
		  /*		  G4int hisNumber = theHistoNumber+imat*100000+iproc*10000+ii*100+ic*1;
		  GmHisto1* his = theAnaMgr->GetHisto1(hisNumber,0);
		  if( his == 0) {
		    G4String hname = material->GetName() + "_" + channelName + "_" + hadProc->GetProcessName();
		    theAnaMgr->CreateHisto1D(hname.c_str(), theNStepsE, theMinE, theMaxE, hisNumber);
		      his = theAnaMgr->GetHisto1(hisNumber);
		    }
		    if( !bLogE ) {
		      his->Fill( ener,FSXS );		 
		    } else {
		      his->Fill( log10(ener),FSXS );		 
		      }*/
		}
	      } // numberOfChannels
	    } // numberOfIsos 
	  }

	} else if( hadProc->GetProcessName() == "nCapture" ) {

	}

	delete aParticle;

      }

      //      rootFile->cd();
      //      graph->Write();
    }
  }

}


//------------------------------------------------------------------
void SHPlotPHPDBSpectraUA::EndOfRunAction(const G4Run*)
{

}

