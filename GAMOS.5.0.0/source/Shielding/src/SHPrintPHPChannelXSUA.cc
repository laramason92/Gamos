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
#include "G4NeutronHPFinalState.hh"

#include "SHPrintPHPChannelXSUA.hh"
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
#include "G4NeutronHPFission.hh"
#include "G4NeutronHPChannelList.hh"
#include "G4NeutronHPChannel.hh"
#include "G4ParticleHPInelastic.hh"
#include "G4ParticleHPChannelList.hh"
#include "G4ParticleHPChannel.hh"
#include "G4ParticleHPCapture.hh"
#include "G4ParticleHPFission.hh"

#include "G4tgrFileIn.hh"
#include "G4HadronicProcessStore.hh"

#include <iomanip>

//------------------------------------------------------------------
SHPrintPHPChannelXSUA::SHPrintPHPChannelXSUA() 
{
  //  theParticleName  = GmParameterMgr::GetInstance()->GetStringValue("SHPrintPHPChannelXSUA:ParticleName","neutron");
  GetEnergyParameters("SHPrintPHPChannelXSUA");
  

  //--- for use in histogram 
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
void SHPrintPHPChannelXSUA::BeginOfRunAction(const G4Run*)
{
  bNormaliseToTotalXS = G4bool(GmParameterMgr::GetInstance()->GetNumericValue(theName+":NormaliseToTotalXS",0));

  bOnlyTotalXS = G4bool(GmParameterMgr::GetInstance()->GetNumericValue(theName+":OnlyTotalXS",0));

  bYields = G4bool(GmParameterMgr::GetInstance()->GetNumericValue(theName+":ParticleYields",0));

  bPrintXS0 = G4bool(GmParameterMgr::GetInstance()->GetNumericValue(theName+":PrintXS0",0));

  theProcIdxMax = G4int(GmParameterMgr::GetInstance()->GetNumericValue(theName+":ProcessIndexMax",10));
  theMateIdxMax = G4int(GmParameterMgr::GetInstance()->GetNumericValue(theName+":MaterialIndexMax",10));
  theIsotIdxMax = G4int(GmParameterMgr::GetInstance()->GetNumericValue(theName+":IsotopeIndexMax",50));
  theProdIdxMax = G4int(GmParameterMgr::GetInstance()->GetNumericValue(theName+":ProductIndexMax",200));

  theProductMaxAZ = G4int(GmParameterMgr::GetInstance()->GetNumericValue(theName+":ProductMaxAZ",9999999));
 
  SetHistoNameAndNumber(theName,theFilters,theClassifier);

  SHUtils::BuildElementNames();

}

//------------------------------------------------------------------
void SHPrintPHPChannelXSUA::BeginOfEventAction(const G4Event* evt)
{

  G4PrimaryParticle* primParticle  = evt->GetPrimaryVertex(0)->GetPrimary(0);
  G4ParticleDefinition* particleDef = primParticle->GetG4code();
  G4ThreeVector direction(1., 0., 0. ); // dummy direction
  
  thePrimaryParticleName = primParticle->GetParticleDefinition()->GetParticleName();
  SHUtils::StoreHadronicProcesses(thePrimaryParticleName);

  //--- Loop to materials 
  const G4MaterialTable* matTable = G4Material::GetMaterialTable();
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
      G4String hname = material->GetName() + "_" + hadProc->GetProcessName();
      theAnaMgr->CreateHisto1D(hname.c_str(), theNStepsE, theMinE, theMaxE, theHistoNumber+imat*100000+iproc*10000);
      GmHisto1* his = theAnaMgr->GetHisto1( theHistoNumber+imat*100000+iproc*10000);

      //--- Loop to energies
      for( size_t iener = 0; iener != theNStepsE; iener++ ) {
	G4double ener = theEnergies[iener];
	G4double cross = 0.;
	//--- Sum up cross sections of elements 
	G4DynamicParticle* aParticle = new G4DynamicParticle( particleDef, direction, ener);
	for (size_t iele=0; iele<nelm; iele++) {
	  const G4Element* elm = (*elementVector)[iele];
	  double crosst = hadProc->GetMicroscopicCrossSection(aParticle,elm,material);
	  cross += atomNumDensityVector[iele] * crosst;
	}

	if( !bLogE ) {
	  his->Fill( ener, cross/nAtomsPermm3/barn );
	} else {
	  his->Fill( log10(ener), cross/nAtomsPermm3/barn );
	}
	G4double XSTOT = cross/nAtomsPermm3/barn;
	G4cout << "@@SHPrintPHPChannelXSUA " << material->GetName() 
	       << " " << ener << " MeV" 
	       << " " << hadProc->GetProcessName() 
	  //	     << " 1./MFP= " << 1./mfp << " 1/mm" 
	       << " TOTAL_XS(macro)= " << cross/10. << " cm-1" 
	       << " TOTAL_XS(micro)= " << XSTOT << " barn" 
	       << G4endl;

	if( bOnlyTotalXS ) continue;
	//-- Inelastic
	G4HadronicInteraction* hadModel = hadProc->ChooseHadronicInteraction(ener, material, (*elementVector)[0]); // same model is assumed for all elements !!
	if( SHUtils::GetNChannelParticles() == 0 ) SHUtils::BuildChannelList(primParticle->GetParticleDefinition(), theName);

	if( hadModel->GetModelName() == "ParticleHPInelastic" ){
	//	if( static_cast<G4ParticleHPInelastic*>(hadModel) != 0 ){
	  G4ParticleHPInelastic* HPinelas = (G4ParticleHPInelastic*)(hadModel);
	  if( SHUtils::GetNChannelParticles() == 0 ) SHUtils::BuildChannelList(primParticle->GetParticleDefinition(), theName);

	  G4HadProjectile hadProj(*aParticle);

	  //--- loop to elements 
	  G4double XSSUM = 0;
	  for (size_t iele=0; iele<nelm; iele++) {
	    G4Element* elem = (*elementVector)[iele];
	    //    G4cout << " STOREXS " << store->GetInelasticCrossSectionPerAtom(G4Proton::Proton(),ener,elem)/barn << G4endl;
	    //	    G4cout << " LOOPING ELEMENT " << elem->GetName() << G4endl;
	    G4int index = elem->GetIndex();

	    //	    G4ParticleHPChannelList HPChannelList = HPinelas->theInelastic[index]; // one List per element
	    
	    G4ParticleHPChannel ** HPChannels = HPinelas->theInelastic[index]->theChannels;
	    G4int nChannels = HPinelas->theInelastic[index]->nChannels;
	    G4int numberOfIsos = HPChannels[0]->GetNiso(); // all channels have the same number of isotopes??
	    //--- loop to isotopes in element
	    for( G4int iisot = 0; iisot < numberOfIsos; iisot++ ) {
	      G4String isotName = elem->GetIsotope(iisot)->GetName();
	      
	      G4double XSSUM_ISOT = 0;
	      std::map<G4int,G4double> FSXSs;
	      //--- loop to channel for this isotope: sum up total XS 
	      for(G4int ic=0; ic<nChannels; ic++) {
		G4ParticleHPChannel* HPChannel = HPChannels[ic];
		if( HPChannel->HasFSData(iisot) ) { //some channels have only F02 data
		  G4double FSXS = HPChannel->GetFSCrossSection(ener, iisot);
		  FSXSs[ic] = FSXS;
		  XSSUM_ISOT += FSXS;
		}
	      } // numberOfChannels
	      
	      std::map<G4double,G4double> totalProductYields; // sum for all channels
	      //--- loop to channel for this isotope
	      for(G4int ic=0; ic<nChannels; ic++) {
		G4ParticleHPChannel* HPChannel = HPChannels[ic];
		//		G4cout << iisot << " LOOPING TO CHANNEL " << HPChannel->theFSType << " HAS DATA " << HPChannel->HasFSData(iisot) << G4endl;
		if( HPChannel->HasFSData(iisot) ) {// one channel per isotope, one final state per isotope
		  G4ParticleHPFinalState * FS = HPChannel->theFinalStates[iisot];
		
		  //--- Print XS
		  G4String FSType = HPChannel->theFSType;
		  G4String channelName = SHUtils::GetChannelName(int(FS->theBaseZ), int(FS->theBaseA), FSType.substr(0,3), GmG4Utils::GetParticleShortName(thePrimaryParticleName) );
		  G4double FSXS = FSXSs[ic];
		  if( bNormaliseToTotalXS ) FSXS *= XSTOT/XSSUM_ISOT;
		  if( FSXS != 0 || bPrintXS0 ) {
		    G4cout << "@@SHPrintPHPChannelXSUA " << material->GetName() 
			   << " " << ener << " MeV" 
			   << " " << hadProc->GetProcessName() 
			   << " CHANNEL= " << FSType << " " << channelName
			   << " XS(micro)= " << FSXS << " barn" 
			   << G4endl;
		    G4int histoN = theHistoNumber 
		      + (ic+1)*theProcIdxMax*theMateIdxMax*theIsotIdxMax*10  
		      + iisot*theProcIdxMax*theMateIdxMax*10  
		      + imat*theProcIdxMax*10  
		      + iproc*10;
		    GmHisto1* his = theAnaMgr->GetHisto1(histoN,0);
		    if( his == 0) {
		      G4String hname = hadProc->GetProcessName() + "_" + material->GetName() + "_" + channelName + "_XS";
		      theAnaMgr->CreateHisto1D(hname.c_str(), theNStepsE, theMinE, theMaxE, histoN);
		      his = theAnaMgr->GetHisto1(histoN);
		    }
		    if( !bLogE ) {
		      his->Fill( ener,FSXS );		 
		    } else {
		      his->Fill( log10(ener),FSXS );		 
		    }
		  }
		  XSSUM += FSXS;

		  //--- Print yields of each product
		  if( bYields ) { 
		    G4ParticleHPEnAngCorrelation* energyAngData;
		    G4ReactionProduct * aTarget = new G4ReactionProduct();
		    
		    aTarget->SetMass(FS->theBaseA); //t
		    
		    if( FSType == "F01/" || FSType == "F23/" || FSType == "F24/" || FSType == "F25/" || FSType == "F26/" || FSType == "F27/" ){
		      G4ParticleHPInelasticCompFS* FSC = (G4ParticleHPInelasticCompFS*)(FS);
		      G4int it = FSC->SelectExitChannel(ener);
		      energyAngData = FSC->theEnergyAngData[it];
		      if( !energyAngData ) continue;
		      energyAngData->SetTarget(*aTarget);
		      //		      G4cout << FSC << " Use CompFS theEnergyAngData " << it << " " << FSC->theEnergyAngData[it] << G4endl;
		      
		    } else {
		      G4ParticleHPInelasticBaseFS* FSB = (G4ParticleHPInelasticBaseFS*)(FS);
		      energyAngData = FSB->theEnergyAngData;
		      if( !energyAngData ) continue;
		      energyAngData->SetTarget(*aTarget);
		      //		    G4cout << FSB << " Use BaseFS theEnergyAngData " << FSB->theEnergyAngData << G4endl;
		    }
		    
		    G4int nProducts = energyAngData->nProducts;
		    G4ParticleHPProduct * products = energyAngData->theProducts;
		    
		    for(G4int ip=0; ip<nProducts; ip++) {
		      G4ParticleHPProduct* product = &(products[ip]);
		      if( product->theMassCode > theProductMaxAZ ) continue;
		      G4double yield = product->GetMeanYield(ener);
		      if( yield != 0 && (FSXS != 0 || bPrintXS0) ) {
			totalProductYields[product->theMassCode] += yield*FSXS;
			G4String productName = SHUtils::GetProductName( product->theMassCode );
			G4cout << "@@SHPrintPHPChannelXSUA " << material->GetName() 
			       << " " << ener << " MeV" 
			       << " " << hadProc->GetProcessName() 
			       << " CHANNEL= " << FSType << " " << channelName
			       << " PRODUCT= " << productName 
			       << " YIELD= " << yield
			       << " PRODUCT_XS= " << yield*FSXS << " barn" 
			       << G4endl;
			//--- Fill histos 
			G4int iprodu = SHUtils::GetProductID( product->theMassCode );			
			G4int histoN = theHistoNumber 
			  + iprodu*theProcIdxMax*theMateIdxMax*theIsotIdxMax*30*10 
			  + (ic+1)*theProcIdxMax*theMateIdxMax*theIsotIdxMax*10  
			  + iisot*theProcIdxMax*theMateIdxMax*10  
			  + imat*theProcIdxMax*10  
			  + iproc*10
			  + 3;
			
			G4cout << theHistoNumber << " HISTON " << histoN << " " << hname << G4endl; //GDEB
			GmHisto1* hisY = theAnaMgr->GetHisto1(histoN,0);
			GmHisto1* hisYXS;
			if( hisY == 0) {
			  G4String hname = hadProc->GetProcessName() + "_" + material->GetName() + "_" + channelName + "_" + productName;
			  theAnaMgr->CreateHisto1D((hname+":YIELD").c_str(), theNStepsE, theMinE, theMaxE, histoN);
			  theAnaMgr->CreateHisto1D((hname+":XS_YIELD").c_str(), theNStepsE, theMinE, theMaxE, histoN+1);
			  hisY = theAnaMgr->GetHisto1(histoN,0);
			}
			hisYXS = theAnaMgr->GetHisto1(histoN+1);
			if( !bLogE ) {
			  hisY->Fill( ener, yield );		 
			  hisYXS->Fill( ener,yield*FSXS );		 
			} else {
			  hisY->Fill( log10(ener), yield );		 
			  hisYXS->Fill( log10(ener),yield*FSXS );	
			}
			
		      } // if yield <> 0
		    }
		  } // if bYields
		} // if HasFSData
	      } // numberOfChannels

	      //--- Fill histos sum of all channels 
	      std::map<G4double,G4double>::const_iterator itepy;
	      for( itepy = totalProductYields.begin(); itepy != totalProductYields.end(); itepy++ ){
		G4int iprodu = SHUtils::GetProductID( (*itepy).first );
		
		G4String productName = SHUtils::GetProductName( (*itepy).first );

		G4cout << "@@SHPrintPHPChannelXSUA " << material->GetName() 
		       << " " << ener << " MeV" 
		       << " " << hadProc->GetProcessName() 
		       << " PRODUCT= " << productName 
		       << " TOTAL YIELD*XS= " << (*itepy).second << " barn" 
		       << G4endl;

		G4int histoNAllChann = theHistoNumber 
		  + iprodu*theProcIdxMax*theMateIdxMax*theIsotIdxMax*30*10 
		  + iisot*theProcIdxMax*theMateIdxMax*10  
		  + imat*theProcIdxMax*10 
		  + iproc*10
		  + 1;
		hname = hadProc->GetProcessName() + "_" + material->GetName() + "_" + isotName + "_ALL_" + productName;
		
		TH1F* hisYXSALL = theAnaMgr->GetHisto1(histoNAllChann,0);
		if( !hisYXSALL ) {
		  theAnaMgr->CreateHisto1D((hname+":YIELD").c_str(), 100, 0, theMaxE, histoNAllChann );
		  theAnaMgr->CreateHisto1D((hname+":YIELD_XS").c_str(), 100, 0, 180., histoNAllChann+1 );
		  hisYXSALL = theAnaMgr->GetHisto1(histoNAllChann);
		}
		
		if( !bLogE ) {
		  hisYXSALL->Fill( ener,(*itepy).second );
		} else {
		  hisYXSALL->Fill( log10(ener),(*itepy).second );
		}		
	      }

	    } // numberOfIsos 
	  }

	}else if( hadModel->GetModelName() == "NeutronHPInelastic" ){
	  //	}else if( static_cast<G4NeutronHPInelastic*>(hadModel) ) {
	  //	} else if( hadProc->GetProcessName() == "NeutronInelastic" ) {
	    G4NeutronHPInelastic* HPinelas = (G4NeutronHPInelastic*)(hadModel);

	  //	  G4double XSTOT = cross/nAtomsPermm3/barn;

	  //--- loop to elements 
	  G4double XSSUM = 0;
	  for (size_t iele=0; iele<nelm; iele++) {
	    G4Element* elem = (*elementVector)[iele];
	    //	    G4cout << " STOREXS " << store->GetInelasticCrossSectionPerAtom(G4Neutron::Neutron(),ener,elem)/barn << G4endl;
	    //	    G4cout << " LOOPING ELEMENT " << elem->GetName() << G4endl;
	    G4int index = elem->GetIndex();


	    if(hadModel  == 0 ) {
	      G4Exception("SHPrintPHPChannelXSUA::BeginOfEventAction",
			  "Inelastic cross section not found",
			  JustWarning,
			  G4String("Probably energy not in DB: " + GmGenUtils::ftoa(ener) + " element " + elem->GetName()).c_str());
	      continue;
	    }

	    //	    G4NeutronHPChannelList HPChannelList = HPinelas->theInelastic[index]; // one List per element
	    
	    G4NeutronHPChannel ** HPChannels =  HPinelas->theInelastic[index]->theChannels;

	    //	    G4cout << " GET hadInteraction " << hadInteraction << " HPinelas " << HPinelas << " HPChannels " <<  HPChannels << G4endl;
	    //	    HPChannelList.DumpInfo();

	    
	    G4int nChannels = HPinelas->theInelastic[index]->nChannels;
	    G4int numberOfIsos = HPChannels[0]->GetNiso(); // all channels have the same number of isotopes??
	    for( G4int iisot = 0; iisot < numberOfIsos; iisot++ ) {
	      
	      G4double XSSUM_ISOT = 0;
	      std::vector<G4double> FSXSs;
	      for(G4int ic=0; ic<nChannels; ic++) {
		G4NeutronHPChannel* HPChannel = HPChannels[ic];
		if( HPChannel->HasFSData(iisot) ) { //some channels have only F02 data
		  G4double FSXS = HPChannel->GetFSCrossSection(ener, iisot);
		  FSXSs.push_back( FSXS );
		  XSSUM_ISOT += FSXS;
		}
	      } // numberOfChannels
		/*		G4double enerTherm = aThermalE.GetThermalEnergy(hadProj,
								HPChannel->GetN(iisot),
								HPChannel->GetZ(iisot),
								aTemp); 
		G4cout << " SHPrintPHPChannelXSUA enerTherm " << enerTherm << " " << ener << " = " << hadProj.GetKineticEnergy() << G4endl;
		*/

	      std::map<G4double,G4double> totalProductYields; // sum for all channels
	      for(G4int ic=0; ic<nChannels; ic++) {
		G4NeutronHPChannel* HPChannel = HPChannels[ic];
		G4NeutronHPFinalState * FS = HPChannel->theFinalStates[iisot];
		//		  G4cout << " LOOPING TO ISOTOPE " << FS->theBaseZ << "/" << FS->theBaseA << G4endl;
		
		if( HPChannel->HasFSData(iisot) ) {// one channel per isotope , one final state per isotope
		  //		  G4NeutronHPFinalState * FS = HPChannel->theFinalStates[iisot];
		  /*		  G4cout << " FS Base A/Z" << FS->theBaseA << " " << FS->theBaseZ
				  << " NDL A/Z " << FS->theNDLDataA << " " << FS->theNDLDataZ
				  << " DataDir " << FS->theDataDir << G4endl;*/
		  G4String FSType = HPChannel->theFSType;
		  G4String channelName = SHUtils::GetChannelName(int(FS->theBaseZ), int(FS->theBaseA), FSType.substr(0,3), GmG4Utils::GetParticleShortName(thePrimaryParticleName) );
		  G4double FSXS = HPChannel->GetFSCrossSection(ener, iisot);
		  if( bNormaliseToTotalXS ) FSXS *= XSTOT/XSSUM_ISOT;		  
		  if(FSXS != 0 || bPrintXS0) {
		    G4cout << "@@SHPrintPHPChannelXSUA " << material->GetName() 
			   << " " << ener << " MeV" 
			   << " " << hadProc->GetProcessName() 
		      //			 << " CHANNEL= " << HPChannel->theFSType
		      //			 << " ISOTOPE= " << iisot << " Z/A= " << FS->theBaseZ << "/" << FS->theBaseA 
		      //			 << GetChannelName(FS->theBaseZ, FS->theBaseA, HPChannel->theFSType.substr(0,3) )
			   << " CHANNEL= " << HPChannel->theFSType << " " << channelName
		      // GetChannelName(int(FS->theBaseZ), int(FS->theBaseA), HPChannel->theFSType.substr(0,3), "n" )
			   << " XS(micro)= " << FSXS << " barn" 
		      //-			 << " numberOfIsos " << numberOfIsos
			   << G4endl;
		    G4int hisNumber = theHistoNumber+imat*100000+iproc*10000+iisot*100+ic*1;
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
		    }
		  } 
		  XSSUM += FSXS;

		  //--- Print yields of each product
		  if( bYields && (FSXS != 0 || bPrintXS0) ) { 
		    G4NeutronHPEnAngCorrelation* energyAngData;
		    G4ReactionProduct * aTarget = new G4ReactionProduct();
		    
		    aTarget->SetMass(FS->theBaseA); //t
		    
		    if( FSType == "F01/" || FSType == "F23/" || FSType == "F24/" || FSType == "F25/" || FSType == "F26/" || FSType == "F27/" ){
		      G4NeutronHPInelasticCompFS* FSC = (G4NeutronHPInelasticCompFS*)(FS);
		      G4int it = FSC->SelectExitChannel(ener);
		      energyAngData = FSC->theEnergyAngData[it];
		      if( !energyAngData ) continue;
		      energyAngData->SetTarget(*aTarget);
		    } else {
		      G4NeutronHPInelasticBaseFS* FSB = (G4NeutronHPInelasticBaseFS*)(FS);
		      energyAngData = FSB->theEnergyAngData;
		      if( !energyAngData ) continue;
		      energyAngData->SetTarget(*aTarget);
		    }
		    
		    G4int nProducts = energyAngData->nProducts;
		    G4NeutronHPProduct * products = energyAngData->theProducts;
		    
		    for(G4int ip=0; ip<nProducts; ip++) {
		      G4NeutronHPProduct* product = &(products[ip]);
		      if( product->theMassCode > theProductMaxAZ ) continue;
		      G4double yield = product->GetMeanYield(ener);
		      if( yield  != 0 ) {
			totalProductYields[product->theMassCode] += yield*FSXS;
			G4String productName = SHUtils::GetProductName( product->theMassCode );
			G4cout << "@@SHPrintPHPChannelXSUA " << material->GetName() 
			       << " " << ener << " MeV" 
			       << " " << hadProc->GetProcessName() 
			       << " CHANNEL= " << FSType << " " << channelName
			       << " PRODUCT= " << productName 
			       << " YIELD= " << yield
			       << " PRODUCT_XS= " << yield*FSXS << " barn" 
			       << G4endl;
			//--- Fill histos 
			G4int iprodu = SHUtils::GetProductID( product->theMassCode );			
			G4int histoN = theHistoNumber 
			  + iprodu*theProcIdxMax*theMateIdxMax*theIsotIdxMax*30*10 
			  + (ic+1)*theProcIdxMax*theMateIdxMax*theIsotIdxMax*10  
			  + iisot*theProcIdxMax*theMateIdxMax*10  
			  + imat*theProcIdxMax*10  
			  + iproc*10
			  + 3;
			
			GmHisto1* hisY = theAnaMgr->GetHisto1(histoN,0);
			GmHisto1* hisYXS;
			if( hisY == 0) {
			  G4String hname = hadProc->GetProcessName() + "_" + material->GetName() + "_" + channelName + "_" + productName;
			  theAnaMgr->CreateHisto1D((hname+":YIELD").c_str(), theNStepsE, theMinE, theMaxE, histoN);
			  theAnaMgr->CreateHisto1D((hname+":XS_YIELD").c_str(), theNStepsE, theMinE, theMaxE, histoN+1);
			  hisY = theAnaMgr->GetHisto1(histoN,0);
			}
			hisYXS = theAnaMgr->GetHisto1(histoN+1);
			if( !bLogE ) {
			  hisY->Fill( ener, yield );		 
			  hisYXS->Fill( ener,yield*FSXS );		 
			} else {
			  hisY->Fill( log10(ener), yield );		 
			  hisYXS->Fill( log10(ener),yield*FSXS );	
			}
			
		      } // if yield <> 0
		    }
		  }  // if bYields
		}
	      } // numberOfChannels// if

	    } // numberOfIsos 
	  } // elem 
	}else if( hadModel->GetModelName() == "NeutronHPCapture" ){
	  //	} else if( static_cast<G4NeutronHPCapture*>(hadModel) ){
	  //--- loop to elements 
	  G4double XSSUM = 0;
	  for (size_t iele=0; iele<nelm; iele++) {
	    G4double XSSUM_ELE = 0;
	    G4Element* elem = (*elementVector)[iele];

	    G4NeutronHPCapture* HPcapt = (G4NeutronHPCapture*)(hadModel);

	    if(hadModel == 0 ) {
	      G4Exception("SHPrintPHPChannelXSUA::BeginOfEventAction",
			  "Inelastic cross section not found",
			  JustWarning,
			  G4String("Probably energy not in DB: " + GmGenUtils::ftoa(ener) + " element " + elem->GetName()).c_str());
	      continue;
	    }
	    std::vector<G4NeutronHPChannel*> HPChannels =  HPcapt->theCapture;

	    G4int numberOfIsos = HPChannels.size(); // all channels have the same number of isotopes??
	    return; // NOT WELL UNDERSTOOD why for Ca and iisot = 3  HPChannels[iisot]->theFinalStates[iisot] = 0x21
	    for( G4int iisot = 0; iisot < numberOfIsos; iisot++ ) {
	      G4NeutronHPFinalState * FS = HPChannels[iisot]->theFinalStates[iisot];
	      //		  G4cout << " LOOPING TO ISOTOPE " << FS->theBaseZ << "/" << FS->theBaseA << G4endl;
	      
	      if( HPChannels[iisot]->HasFSData(iisot) ) {// one channel per isotope , one final state per isotope
		G4double FSXS = HPChannels[iisot]->GetFSCrossSection(ener, iisot);
		
		if( HPChannels[iisot]->theFSType != "" && // for H1 it is ""
		    ( FSXS != 0 || bPrintXS0 ) ) {
		  G4cout << "@@SHPrintPHPChannelXSUA " << material->GetName() 
			 << " " << ener << " MeV" 
			 << " " << hadProc->GetProcessName() 
			 << " CHANNEL= " << HPChannels[iisot]->theFSType << " " 
			 << SHUtils::GetChannelName(int(FS->theBaseZ), int(FS->theBaseA), HPChannels[iisot]->theFSType.substr(0,3), GmG4Utils::GetParticleShortName(thePrimaryParticleName) )
			 << " XS(micro)= " << FSXS << " barn" 
			 << G4endl;
		  
		  XSSUM_ELE += FSXS;
		  XSSUM += FSXS;
		}
	      }
	    } // numberOfIsos
	  } // nelm
	
	}else if( hadModel->GetModelName() == "ParticleHPCapture" ){
	  //	} else if( static_cast<G4ParticleHPCapture*>(hadModel) ) {
	  //--- loop to elements 
	  G4double XSSUM = 0;
	  for (size_t iele=0; iele<nelm; iele++) {
	    G4double XSSUM_ELE = 0;
	    G4Element* elem = (*elementVector)[iele];

	    G4ParticleHPCapture* HPcapt = (G4ParticleHPCapture*)(hadModel);

	    if(hadModel == 0 ) {
	      G4Exception("SHPrintPHPChannelXSUA::BeginOfEventAction",
			  "Inelastic cross section not found",
			  JustWarning,
			  G4String("Probably energy not in DB: " + GmGenUtils::ftoa(ener) + " element " + elem->GetName()).c_str());
	      continue;
	    }

	    return; // NOT WELL UNDERSTOOD why for O and iisot = 2  HPChannels[iisot]->theFinalStates[iisot] = 0x3f73767be5dd1caf

	    G4ParticleHPChannel * HPChannel = (HPcapt->theCapture)[0];

	    G4int numberOfIsos = HPChannel->GetNiso(); // all channels have the same number of isotopes??                                                                           
            for( G4int iisot = 0; iisot < numberOfIsos; iisot++ ) {
              G4ParticleHPFinalState * FS = HPChannel->theFinalStates[iisot];
              //                  G4cout << " LOOPING TO ISOTOPE " << FS->theBaseZ << "/" << FS->theBaseA << G4endl;                                                                 
              if( HPChannel->HasFSData(iisot) ) {// one channel per isotope , one final state per isotope  
                G4double FSXS = HPChannel->GetFSCrossSection(ener, iisot);
		
		if( HPChannel->theFSType != "" && // for H1 it is ""
		    ( FSXS != 0 || bPrintXS0 ) ) {
		  G4cout << "@@SHPrintPHPChannelXSUA " << material->GetName() 
			 << " " << ener << " MeV" 
			 << " " << hadProc->GetProcessName() 
			 << " CHANNEL= " << HPChannel->theFSType << " "
			 << SHUtils::GetChannelName(int(FS->theBaseZ), int(FS->theBaseA), HPChannel->theFSType.substr(0,3), GmG4Utils::GetParticleShortName(thePrimaryParticleName) )
			 << " XS(micro)= " << FSXS << " barn" 
			 << G4endl;
		  
		  XSSUM_ELE += FSXS;
		  XSSUM += FSXS;
		}
	      }
		
	    } // numberOfChannels
	  }

	}

	delete aParticle;

      }

      //      rootFile->cd();
      //      graph->Write();
    }
  }

}


//------------------------------------------------------------------
void SHPrintPHPChannelXSUA::EndOfRunAction(const G4Run*)
{

}