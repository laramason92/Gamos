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
// SHRadDecayScorerFromFlux
//#define protected public
//#define private public
#include "SHRadDecayScorerFromFlux.hh"
#include "SHUtils.hh"
#include "ShieldingVerbosity.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmNumberOfEvent.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4HadronicProcess.hh"
#include "G4ParticleHPInelastic.hh"
#include "G4ReactionProduct.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4Track.hh"
#include "G4VSolid.hh"
#include "G4IonTable.hh"
#include "G4ParticleTable.hh"

//-------------------------------------------------------------------
SHRadDecayScorerFromFlux::SHRadDecayScorerFromFlux()
{
  bAdjustPHP = !getenv("G4PARTICLEHP_DO_NOT_ADJUST_FINAL_STATE"); 
  bAdjustHP = !getenv("G4NEUTRONHP_DO_NOT_ADJUST_FINAL_STATE");
  theEnergyInterpStep = GmParameterMgr::GetInstance()->GetNumericValue("SHRadDecayScorerFromFlux:EnergyInterpolationStep",DBL_MAX);
  theIonTable = (G4IonTable *)(G4ParticleTable::GetParticleTable()->GetIonTable());

  sumALL = 0.;

  bNewEvent = TRUE;

  theEvtMap = new G4THitsMap<G4double>("SHRadDecayScorerFromFlux","SHRadDecayScorerFromFlux");
  theRunMap = new G4THitsMap<G4double>("SHRadDecayScorerFromFlux","SHRadDecayScorerFromFlux");
  bInitialised = FALSE;

}

//-------------------------------------------------------------------
SHRadDecayScorerFromFlux::~SHRadDecayScorerFromFlux()
{;}

//-------------------------------------------------------------------
void SHRadDecayScorerFromFlux::ProcessHits(const G4Step* aStep)
{
  if( !bInitialised ) {
    SHUtils::StoreHadronicProcesses(aStep->GetTrack()->GetDefinition()->GetParticleName());
    bInitialised = TRUE;
  }
  G4cout << "  SHRadDecayScorerFromFlux::ProcessHits " << G4endl;//GDEB
  G4double energyPre = aStep->GetPreStepPoint()->GetKineticEnergy();
  G4double energyPost = aStep->GetPostStepPoint()->GetKineticEnergy();
  G4LogicalVolume* lv = aStep->GetTrack()->GetVolume()->GetLogicalVolume();
  G4Material* material = lv->GetMaterial();
  G4double stepLength = aStep->GetStepLength();
  G4double cubicVolume = lv->GetSolid()->GetCubicVolume();

  size_t numberOfElem = material->GetNumberOfElements();
  //    G4double aTemp = material->GetTemperature(); 
  G4ElementVector* elementVector = const_cast<G4ElementVector*>(material->GetElementVector());
  
  const G4double* atomNumDensityVector = material->GetVecNbOfAtomsPerVolume();
  
  //--- Loop to hadronic processes 
  for( G4int iproc = 0; iproc < SHUtils::GetNHadronicProcesses(); iproc++ ){
    G4HadronicProcess* hadProc = SHUtils::GetHadronicProcess(iproc);
    G4String hname = material->GetName() + "_" + hadProc->GetProcessName();

    if( SHUtils::GetNChannelParticles() == 0 ) SHUtils::BuildChannelList(aStep->GetTrack()->GetDefinition(), "SHRadDecayScorerFromFlux");

    //-- Inelastic
    G4HadronicInteraction* hadModel = hadProc->ChooseHadronicInteraction(energyPre, material, (*elementVector)[0]);
    G4cout << " hadModel " << hadModel->GetModelName() << G4endl;//GDEB
    //    G4String modelName = hadModel->GetModelName();
    if( dynamic_cast<G4ParticleHPInelastic*>(hadModel) != 0 ) {
      G4ParticleHPInelastic* HPinelas = (G4ParticleHPInelastic*)(hadModel);
      
G4cout << "  SHRadDecayScorerFromFlux::ProcessHits G4ParticleHPInelastic nelem " << numberOfElem << G4endl;//GDEB
      //--- loop to elements 
      for (size_t iele=0; iele<numberOfElem; iele++) {
	G4Element* elem = (*elementVector)[iele];
	G4double nAtomsPermm3 = atomNumDensityVector[iele];
	G4int index = elem->GetIndex();
	
	G4ParticleHPChannel ** HPChannels = HPinelas->theInelastic[index]->theChannels;
	
	G4int nChannels = HPinelas->theInelastic[index]->nChannels;
	//--- loop to isotopes
	G4int numberOfIsos = HPChannels[0]->GetNiso(); // all channels have the same number of isotopes??
G4cout << "  SHRadDecayScorerFromFlux::ProcessHits G4ParticleHPInelastic nchan " << nChannels << " nisot " << numberOfIsos << G4endl;//GDEB
	for( G4int ip = 0; ip < numberOfIsos; ip++ ) {
	  //	  G4String isotName = elem->GetIsotope(ip)->GetName();
	  
	  for(G4int ic=0; ic<nChannels; ic++) {
	    G4ParticleHPChannel* HPChannel = HPChannels[ic];
	    //		G4cout << ip << " LOOPING TO CHANNEL " << HPChannel->theFSType << G4endl;
	    if( HPChannel->HasFSData(ip) ) {// one channel per isotope , one final state per isotope
	      G4ParticleHPFinalState * FS = HPChannel->theFinalStates[ip];
	      
	      //--- Get production for each ion
	      G4ParticleHPEnAngCorrelation* energyAngData;
	      G4ReactionProduct * aTarget = new G4ReactionProduct();
	      
	      aTarget->SetMass(FS->theBaseA); //t
	      
	      if( dynamic_cast<G4ParticleHPInelasticCompFS*>(FS) ) {
		//	      if( FSType == "F01/" || FSType == "F23/" || FSType == "F24/" || FSType == "F25/" || FSType == "F26/" || FSType == "F27/" ){
		G4ParticleHPInelasticCompFS* FSC = (G4ParticleHPInelasticCompFS*)(FS);
		G4int it = FSC->SelectExitChannel(energyPre);
		energyAngData = FSC->theEnergyAngData[it];
		if( !energyAngData ) continue;
		energyAngData->SetTarget(*aTarget);
               G4cout << FSC << " Use CompFS theEnergyAngData " << it << " " << FSC->theEnergyAngData[it] << G4endl;
		
	      } else {
		G4ParticleHPInelasticBaseFS* FSB = (G4ParticleHPInelasticBaseFS*)(FS);
		energyAngData = FSB->theEnergyAngData;
		energyAngData->SetTarget(*aTarget);
		if( !energyAngData ) continue;
	     G4cout << FSB << " Use BaseFS theEnergyAngData " << FSB->theEnergyAngData << G4endl;
	      }
	
	      if( !bAdjustPHP ) {
		G4int nProducts = energyAngData->nProducts;
		G4ParticleHPProduct * products = energyAngData->theProducts;
		
		for(G4int ip=0; ip<nProducts; ip++) {
		  G4ParticleHPProduct* product = &(products[ip]);
		  if( product->GetMassCode() < 2004 ) continue; // only ions, bigger than alpha
		  G4double ionProd = 0.;
		  for( G4double ener = energyPre; ener < energyPost; ener += theEnergyInterpStep ) {
		    
		    //	    G4double dEdX = theEmCalculator->GetDEDX(ener, particleDef, material);
		    G4double yield = product->GetMeanYield(ener);
		    // Micro XS
		    G4double FSXS = HPChannel->GetFSCrossSection(ener, ip);
		    // Macro XS
		    //		    G4double cross = FSXS*(nAtomsPermm3*isotFraction)*barn;
		    G4double cross = FSXS*(nAtomsPermm3); // FSXS has into account the isotope fraction
		    //		    G4double cross = FSXS*(nAtomsPermm3)*barn; // FSXS has into account the isotope fraction
		    
		    ionProd += yield * stepLength * cross / cubicVolume;
#ifndef GAMOS_NO_VERBOSE
		    if( SHVerb(debugVerb) && yield != 0 ) {
		      G4String FSType = HPChannel->theFSType;
		      G4String channelName = SHUtils::GetChannelName(int(FS->theBaseZ), int(FS->theBaseA), FSType.substr(0,3), GmG4Utils::GetParticleShortName(aStep->GetTrack()->GetDefinition()->GetParticleName()) );
		      G4String productName = SHUtils::GetProductName( product->theMassCode );
		      G4cout << material->GetName() 
			     << " " << hadProc->GetProcessName() 
			     << " CHANNEL= " << FSType << " " << channelName
			     << " PRODUCT= " << productName 
			     << " YIELD= " << yield
			     << " ION_PRODUCTION= " << ionProd
			     << " XS micro " << FSXS 
			     << " XS macro " << cross 
			     << " Ener " << ener 
			     << " cross " << cross
			     << " nAtomsPermm3 " << nAtomsPermm3
			     << G4endl;
		    }
#endif
		  } // energy loop
		  // Get index: 		  
		  G4int index = GetIndexFromProduct( product );
		  FillScorer( aStep, index, ionProd, aStep->GetPreStepPoint()->GetWeight()); // call directly the filling with index
		  
		} // products loop
	      } else {

		std::map<G4int,G4double> theIonProds;
		std::map<G4int,G4double> theAZs;
		for( G4double ener = energyPre; ener < energyPost; ener += theEnergyInterpStep ) {
		  theAZs.clear();
		  // sample it N times and get all posible A-Z combinations
		  G4int nSamples = 1000;
		  for( G4int is = 0; is < nSamples; is++ ) {
		    G4ReactionProductVector* prodVector = energyAngData->Sample(ener);
		    G4ReactionProductVector::const_iterator itepv;
		    G4int sumA = 0;
		    G4int sumZ = 0;
		    for( itepv = prodVector->begin(); itepv != prodVector->end(); itepv++ ){
		      G4ReactionProduct* product = (*itepv);
		      G4cout << " PRODUCT GETMASS " << product->GetMass() << " > 4.? " << G4endl;
		      if( product->GetMass() > 4. ) continue; // not ions bigger than alpha
		      sumZ += product->GetDefinition()->GetAtomicNumber();
		      sumA += product->GetDefinition()->GetAtomicMass();
		    }
		    G4int sumAZ = 10000 * sumZ + sumA;
		    theAZs[sumAZ] ++;
		  }
		  std::map<G4int,G4double>::const_iterator iteaz;
		  for( iteaz = theAZs.begin(); iteaz != theAZs.end(); iteaz++ ) {
		    G4double FSXS = HPChannel->GetFSCrossSection(ener, ip);
		    // Macro XS
		    //		    G4double cross = FSXS*(nAtomsPermm3*isotFraction)*barn;
		    G4double cross = FSXS*(nAtomsPermm3); // FSXS has into account the isotope fraction
		    //		    G4double cross = FSXS*(nAtomsPermm3)*barn; // FSXS has into account the isotope fraction
		    G4double yield = (*iteaz).second / nSamples; 
		    theIonProds[(*iteaz).first] += yield * stepLength * cross / cubicVolume;// average ion production
		      G4String FSType = HPChannel->theFSType;
		      G4String channelName = SHUtils::GetChannelName(int(FS->theBaseZ), int(FS->theBaseA), FSType.substr(0,3), GmG4Utils::GetParticleShortName(aStep->GetTrack()->GetDefinition()->GetParticleName()) );
		      G4cout << material->GetName() 
			     << " " << hadProc->GetProcessName() 
			     << " CHANNEL= " << FSType << " " << channelName
			     << " AZ= " << (*iteaz).first
			     << " YIELD= " << yield
			     << " ION_PRODUCTION= " << yield * stepLength * cross / cubicVolume
			     << " XS micro " << FSXS 
			     << " XS macro " << cross 
			     << " Ener " << ener 
			     << " cross " << cross
			     << " nAtomsPermm3 " << nAtomsPermm3
			     << G4endl;
		  }
		} // energy loop
		// Get index: 		  
		std::map<G4int,G4double>::const_iterator iteip;
		const G4Isotope* isotope = elem->GetIsotope(ip);
		for( iteip = theIonProds.begin(); iteip != theIonProds.end(); iteip++ ){
		  G4int index = GetIndexAdjustingIon( (*iteip).first, isotope );
		  FillScorer( aStep, index, (*iteip).second, aStep->GetPreStepPoint()->GetWeight()); // call directly the filling with index
		}				  
	      }// if bAdjust PHP
	    } // if HasFSData
	  } // numberOfChannels
	} // numberOfIsos
      } // numberOfElem
      
      /*    }else if( modelName == "NeutronHPInelastic" ){
	
    } else if( modelName == "NeutronHPCapture" ){
      
    } else if( modelName == "ParticleHPnCapture" ){
      */
    }
  }

}

//-------------------------------------------------------------------
G4int SHRadDecayScorerFromFlux::GetIndexFromProduct(G4ParticleHPProduct* product) 
{
  std::map<G4ParticleHPProduct*,G4int>::const_iterator ite = theHPProductIndices.find(product);
  if( ite == theHPProductIndices.end() ) {
    G4int massCode = G4int(product->GetMassCode());
    G4Ions* ion = (G4Ions*)(theIonTable->GetIon(massCode/1000,massCode%1000,0.));
    G4int index = theIonIndices.size();
    theIonIndices[index] = ion;
    theHPProductIndices[product] = index;
    return index;
  } else {
    return (*ite).second;
  }

}
//-------------------------------------------------------------------
G4int SHRadDecayScorerFromFlux::GetIndexAdjustingIon( G4int AZ, const G4Isotope* isotope )
{
  G4int newAZ = isotope->GetZ()*10000 + isotope->GetA() - AZ;
  if( newAZ / 10000 <= 0 || newAZ % 10000 <= 0 ) {
    G4Exception("SHRadDecayScorerFromFlux::GetIndexAdjustingIon",
		"Error",
		FatalException,
		("WRONG AZ " +GmGenUtils::itoa(newAZ) + " THIS SHOULD NOT HAPPEN, CONTACT GAMOS AUTHORS").c_str());
  }

  std::map<G4int,G4int>::const_iterator ite = theAZindices.find(newAZ);
  if( ite == theAZindices.end() ) {
    G4Ions* ion = (G4Ions*)(theIonTable->GetIon(newAZ/10000, newAZ%10000,0.));
    G4int index = theIonIndices.size();
    theIonIndices[index] = ion;
    theAZindices[newAZ] = index;
    return index;
  } else {
    return (*ite).second;
  }

}

//--------------------------------------------------------------------
G4bool SHRadDecayScorerFromFlux::FillScorer(const G4Step*, G4int index, G4double val, G4double wei)
{
#ifndef GAMOS_NO_VERBOSE
  //  if( SHVerb(debugVerb) ) G4cout << EvtMap << " EvtMap Number of entries " << EvtMap->entries() << " tmp " << theSumV_tmp.size() << " val " << val << " weight " << wei << G4endl;
#endif
  if( bNewEvent ) {
    ScoreNewEvent();
  }
  bNewEvent = false;

  G4double valwei = val*wei;
  theEvtMap->add(index,valwei);
#ifndef GAMOS_NO_VERBOSE
  if( SHVerb(debugVerb) ) G4cout << "SHRadDecayScorerFromFlux"
	 << " index " << index 
	 << " val " << val 
	 << " wei " << wei << G4endl;
#endif
  
  theSumV_tmp[index] += val*wei; 
#ifndef GAMOS_NO_VERBOSE
  if( SHVerb(debugVerb) ) G4cout << "SHRadDecayScorerFromFlux chk theSumV_tmp " << index << " = " << theSumV_tmp[index] << G4endl;
#endif
  
  sumALL += val*wei;
#ifndef GAMOS_NO_VERBOSE
  if( SHVerb(debugVerb) ) G4cout << " EvtMap Number of entries " << theEvtMap->entries() << " tmp " << theSumV2.size() << " SUMALL " << sumALL << G4endl;
#endif

  return true;
} 

//--------------------------------------------------------------------
void SHRadDecayScorerFromFlux::ScoreNewEvent() 
{
  //--- Store X and X2 of last event
  //  G4double eventSum = 0.;

  std::map<G4int,G4double>::iterator ite;
  for( ite = theSumV_tmp.begin(); ite !=theSumV_tmp.end(); ite++ ){
    G4int idx = (*ite).first;
    theSumV2[idx] += (*ite).second*(*ite).second;
    
#ifndef GAMOS_NO_VERBOSE
    if( SHVerb(debugVerb) ) G4cout<< "SHRadDecayScorerFromFlux " << idx << " EVENT SumV= " << (*ite).second << " SumV2= " <<  theSumV2[idx] << G4endl;
#endif
    /*    if( theConvergenceTester ) {
      eventSum += (*ite).second;
      } */
  }
 
  /*  if( theConvergenceTester ) {
    theConvergenceTester->AddScore( eventSum );
    } */

  theSumV_tmp.clear();

  *theRunMap += *theEvtMap;
  theEvtMap->clear();
}


//--------------------------------------------------------------------
void SHRadDecayScorerFromFlux::CalculateErrors(G4THitsMap<G4double>* RunMap)
{
  G4double nev;
  nev = GmNumberOfEvent::GetNumberOfEvent();
  
  std::map<G4int,G4double*>::iterator ite;
  for(ite = RunMap->GetMap()->begin(); ite != RunMap->GetMap()->end(); ite++){
    G4double sumWX = (*(ite->second));
    theError[ite->first] = GetError( ite->first, sumWX, nev );
#ifndef GAMOS_NO_VERBOSE
    if( SHVerb(debugVerb) ) G4cout << ite->first << " CERROR " << theError[ ite->first ] << G4endl;
#endif
  }
  
}


//--------------------------------------------------------------------
G4double SHRadDecayScorerFromFlux::GetError( G4int index )
{
  return theError[index];
}

//--------------------------------------------------------------------
G4double SHRadDecayScorerFromFlux::GetError( G4int index, G4double sumWX, G4double nEvents )
{
  if( theSumV2.find( index ) == theSumV2.end() ) return 0.;
  //  G4double error = (theSumV2[index]*nEvents - sumWX*sumWX) / (nEvents*nEvents*(nEvents-1));
  G4double error = (theSumV2[index]*nEvents - sumWX*sumWX) / (nEvents-1);
#ifndef GAMOS_NO_VERBOSE
  if( SHVerb(debugVerb) ) G4cout << " GetError " << index << " e= " << error/(nEvents*nEvents) << " S1 " << theSumV2[index]*nEvents << " S2 " << sumWX*sumWX << " S3 " << (nEvents*nEvents*(nEvents-1)) << G4endl;
#endif
  
  if( error <= 0. ) {
    if( error < -1.E-30 ) G4cerr << " !!WARNING  Error squared in scorer SHRadDecayScorerFromFlux  is negative: " << error << G4endl;
    error = 0.;
  } else {
    error = std::sqrt(error)/nEvents;
  }

#ifndef GAMOS_NO_VERBOSE
  G4cout<< SHVerb(debugVerb) << " GetError " << index << " e= " << error << " e2= " << error*error << " nev " << nEvents << " sumWX " << sumWX << " sumW2 " << theSumV2[index] << G4endl;
#endif
 
  //  G4cout << " SHRadDecayScorerFromFlux::GetError " << index << " = " << error << " sumW " << sumWX << " sumW2 " << theSumV2[index] << " nEvents " << nEvents << G4endl;
  return error;
}

//--------------------------------------------------------------------
G4double SHRadDecayScorerFromFlux::GetErrorRelative( G4int index, G4double sumWX, G4double nEvents )
{
  G4double errorrel = GetError( index );
#ifndef GAMOS_NO_VERBOSE
  if( SHVerb(debugVerb) )  G4cout << " GetErrorRelative temp er " << errorrel << G4endl;
#endif
  // divide by averageX
  if( sumWX != 0. ) errorrel /= (sumWX/nEvents); 
#ifndef GAMOS_NO_VERBOSE
  if( SHVerb(debugVerb) ) G4cout << " GetErrorRelative " << index << " er= " << errorrel << " nev " << nEvents << " sumWX " << sumWX << G4endl;
#endif
  
  return errorrel;
}

G4Ions* SHRadDecayScorerFromFlux::GetIon(G4int index) const 
{
  std::map<G4int,G4Ions*>::const_iterator ite = theIonIndices.find(index);
  if( ite == theIonIndices.end() ) {
    G4Exception("SHRadDecayScorerFromFlux::GetIon",
		"Wron argument",
		FatalErrorInArgument,
		("index not found " + GmGenUtils::itoa(index)).c_str());
  }

  return (*ite).second;
}
