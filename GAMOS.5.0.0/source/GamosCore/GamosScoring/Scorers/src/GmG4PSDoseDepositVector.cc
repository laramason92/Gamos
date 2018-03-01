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
//#define VERBOSE_DOSEDEP
#define protected public
#include "G4VPrimitiveScorer.hh"
//define protected protected
#define private public
#include "G4EnergyLossForExtrapolator.hh"
//#define private private
#include "GmG4PSDoseDepositVector.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"

#include "GamosCore/GamosReadDICOM/include/GmRegularParamUtils.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

#include "G4VSolid.hh"
#include "G4UnitsTable.hh"
#include "G4PhantomParameterisation.hh"
#include "G4RegularNavigationHelper.hh"
#include "G4EmCalculator.hh"

//G4bool GmG4PSDoseDepositVector::bNewEvent = TRUE;

//--------------------------------------------------------------------
GmG4PSDoseDepositVector::GmG4PSDoseDepositVector(G4String name) 
   :GmVPrimitiveScorerVector(name)
{ 
  theUnit = gray;
  theUnitName = G4String("Gy");

  theElossExt = new G4EnergyLossForExtrapolator(0);
  thePhantomParam = 0;

  theNIterations = G4int(GmParameterMgr::GetInstance()->GetNumericValue("GmG4PSDoseDepositVector:NIterations",2));

}


//--------------------------------------------------------------------
G4bool GmG4PSDoseDepositVector::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( !AcceptByFilter( aStep ) ) return false;

  G4double edep = aStep->GetTotalEnergyDeposit();
  if ( edep == 0. ) return FALSE;
  G4double volume = theGeomUtils->GetCubicVolume( aStep->GetPreStepPoint()->GetPhysicalVolume() );

  G4double density = aStep->GetTrack()->GetMaterial()->GetDensity();
  G4double dose    = edep / ( density * volume );
  G4double weight = 1.;
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << "  GmG4PSDoseDepositVector::ProcessHits dose " << dose << " edep " << edep << " density " << density << " volume " << volume << " g= " << g << " joule " << joule << G4endl;
#endif
  if( fWeighted ) {
    weight = aStep->GetPreStepPoint()->GetWeight(); 
    //    dose *= weight; 
  }

  FillScorer( aStep, dose, weight );
  //  FillScorer( aStep, dose, aStep->GetPreStepPoint()->GetWeight());
  return TRUE;
}


//--------------------------------------------------------------------
G4bool GmG4PSDoseDepositVector::FillScorer(G4Step* aStep, G4double val, G4double wei)
{

#ifndef GAMOS_NO_VERBOSE
  //  if( ScoringVerb(debugVerb) ) G4cout << "GmG4PSDoseDepositVector::FillScorer EvtMap Number of entries " << EvtMap->size() << " tmp " << theSumV_tmp.size() << G4endl;
#endif

#ifdef VERBOSE_DOSEDEP
  G4bool verbose = 1;
#endif

  if( aStep == 0 ) return FALSE;  // it is 0 when called by GmScoringMgr after last event

#ifdef VERBOSE_DOSEDEP
  if( verbose ) G4cout << "GmG4PSDoseDepositVector::FillScorer totalEdepo " << aStep->GetTotalEnergyDeposit() 
		       << " Nsteps " << G4RegularNavigationHelper::Instance()->theStepLengths.size() << G4endl;
#endif
  //----- Do not distribute dose in voxels 
  if( G4RegularNavigationHelper::Instance()->theStepLengths.size() <= 1 || aStep->GetTrack()->GetDefinition()->GetPDGCharge() == 0 )  { // we are only counting dose deposit
    G4int index = GetIndex(aStep);
    GmVPrimitiveScorerVector::FillScorer( index, val, wei );
    
#ifdef VERBOSE_DOSEDEP
    if( verbose) G4cout  << "GmG4PSDoseDepositVector::FillScorer  RN: energy lost " << val*wei << " index " << index << G4endl;
#endif
    
    //----- Distribute dose in voxels 
  } else {
    std::vector< std::pair<G4int,G4double> > rnsl = G4RegularNavigationHelper::Instance()->theStepLengths; 
    //      G4double geomSL = rnsl[0].second;
    //      G4double SL = aStep->GetStepLength(); 
    if( !thePhantomParam ) thePhantomParam = GmRegularParamUtils::GetInstance()->GetPhantomParam();
    //      const G4Material* mate = thePhantomParam->GetMaterial( rnsl[0].first );
    const G4ParticleDefinition* part = aStep->GetTrack()->GetDefinition();
    G4double kinEnergyPreOrig = aStep->GetPreStepPoint()->GetKineticEnergy();
    G4double kinEnergyPre = kinEnergyPreOrig;
    
    G4double stepLength = aStep->GetStepLength();
    G4double slSum = 0.;
    unsigned int ii;
    for( ii = 0; ii < rnsl.size(); ii++ ){
      G4double sl = rnsl[ii].second;
      slSum += sl;
#ifdef VERBOSE_DOSEDEP
      if(verbose) G4cout  << "GmG4PSDoseDepositVector::FillScorer"<< ii << " RN: it\
er1 step length geom " << sl << G4endl;
#endif
    }
    
#ifdef VERBOSE_DOSEDEP
    if( verbose )
      G4cout << "GmG4PSDoseDepositVector RN:  step length geom TOTAL " << slSum 
	     << " true TOTAL " << stepLength 
	     << " ratio " << stepLength/slSum 
	     << " Energy " << aStep->GetPreStepPoint()->GetKineticEnergy() 
	     << " Material " << aStep->GetPreStepPoint()->GetMaterial()->GetName() 
	     << " Number of geom steps " << rnsl.size() << G4endl;
#endif
    //----- No iterations to correct elost and msc, distribute dose according to geometrical step length in each voxel
    if( theNIterations == 0 ) { 
      for( unsigned int ii = 0; ii < rnsl.size(); ii++ ){
	G4int index = G4RegularNavigationHelper::Instance()->theStepLengths[ii].first;
	G4double sl = G4RegularNavigationHelper::Instance()->theStepLengths[ii].second;
	G4double valStep = val * sl/slSum; //divide dose along steps, proportional to step lengthr
#ifdef VERBOSE_DOSEDEP
	G4double valwei = valStep*wei;
	if(verbose) G4cout  << "GmG4PSDoseDepositVector::FillScorer"<< ii 
			    << " dose " << valwei 
			    << " in " << index << G4endl;
#endif
	
	GmVPrimitiveScorerVector::FillScorer( index, valStep, wei );
	
      }
    } else { //  1 or more iterations demanded
      
#ifdef VERBOSE_DOSEDEP
      // print corrected energy at iteration 0 
      if(verbose)  {
	G4double slSum = 0.;
	for( ii = 0; ii < rnsl.size(); ii++ ){
	  G4double sl = rnsl[ii].second;
	  slSum += sl;
	}
	
	for( ii = 0; ii < rnsl.size(); ii++ ){
	  G4cout  << "GmG4PSDoseDepositVector::FillScorer "<< ii
		  << " RN: iter0 corrected energy lost " << aStep->GetTotalEnergyDeposit()*rnsl[ii].second/slSum  
		  << G4endl;
	}
      }
#endif
      G4double slRatio = stepLength/slSum;
#ifdef VERBOSE_DOSEDEP
      if(verbose) G4cout << "GmG4PSDoseDepositVector::FillScorer  RN: iter" << iiter << " step ratio " << slRatio << G4endl;
#endif
      
      //--- energy at each interaction
      G4EmCalculator emcalc;
      G4double totalELost = 0.;
      std::vector<G4double> kinELost;
      std::vector<G4double> stepLengths;
      for( int iiter = 1; iiter <= theNIterations; iiter++ ) {
	//--- iter1: distribute true step length in each voxel: geom SL in each voxel is multiplied by a constant so that the sum gives the total true step length
	if( iiter == 1 ) {
	  for( ii = 0; ii < rnsl.size(); ii++ ){
	    G4double sl = rnsl[ii].second;
	    stepLengths.push_back( sl * slRatio );
#ifdef VERBOSE_DOSEDEP
	    if(verbose) G4cout  << "GmG4PSDoseDepositVector::FillScorer"<< ii << " RN: iter" << iiter << " corrected step length " << sl*slRatio << G4endl;
#endif
	  }
	  
	  for( ii = 0; ii < rnsl.size(); ii++ ){
	    const G4Material* mate = thePhantomParam->GetMaterial( rnsl[ii].first );
	    G4double dEdx = 0.;
	    if( kinEnergyPre > 0. ) {  //t check this 
	      dEdx = emcalc.GetDEDX(kinEnergyPre, part, mate);
	    }
	    G4double elost = stepLengths[ii] * dEdx;
	    
#ifdef VERBOSE_DOSEDEP
	    if(verbose) G4cout  << "GmG4PSDoseDepositVector::FillScorer"<< ii << " RN: iter1 energy lost "  << elost 
				<< " energy at interaction " << kinEnergyPre 
				<< " = stepLength " << stepLengths[ii] 
				<< " * dEdx " << dEdx << G4endl;
#endif
	    kinEnergyPre -= elost;
	    kinELost.push_back( elost );
	    totalELost += elost;
	  }
	  
	} else{
	  //------ 2nd and other iterations
	  //----- Get step lengths corrected by changing geom2true correction
	  //-- Get ratios for each energy 
	  slSum = 0.;
	  kinEnergyPre = kinEnergyPreOrig;
	  for( ii = 0; ii < rnsl.size(); ii++ ){
	    const G4Material* mate = thePhantomParam->GetMaterial( rnsl[ii].first );
	    stepLengths[ii] = theElossExt->TrueStepLength( kinEnergyPre, rnsl[ii].second , mate, part );
	    kinEnergyPre -= kinELost[ii];
	    
#ifdef VERBOSE_DOSEDEP
	    if(verbose) G4cout << "GmG4PSDoseDepositVector::FillScorer" << ii 
			       << " RN: iter" << iiter << " step length geom " << stepLengths[ii] 
			       << " geom2true " << rnsl[ii].second / stepLengths[ii]  << G4endl;
#endif
	    
	    slSum += stepLengths[ii];
	  }
	  
	  //Correct step lengths so that they sum the total step length
	  G4double slratio = aStep->GetStepLength()/slSum;
#ifdef VERBOSE_DOSEDEP
	  if(verbose) G4cout << "GmG4PSDoseDepositVector::FillScorer" << ii << " RN: iter" << iiter << " step ratio " << slRatio << G4endl;
#endif
	  for( ii = 0; ii < rnsl.size(); ii++ ){
	    stepLengths[ii] *= slratio;
#ifdef VERBOSE_DOSEDEP
	    if(verbose) G4cout  << "GmG4PSDoseDepositVector::FillScorer"<< ii << " RN: iter" << iiter << " corrected step length " << stepLengths[ii] << G4endl;
#endif
	    }
	  
	  //---- Recalculate energy lost with this new step lengths
	  G4double kinEnergyPre = aStep->GetPreStepPoint()->GetKineticEnergy();
	  totalELost = 0.;
	  for( ii = 0; ii < rnsl.size(); ii++ ){
	    const G4Material* mate = thePhantomParam->GetMaterial( rnsl[ii].first );
	    G4double dEdx = 0.;
	    if( kinEnergyPre > 0. ) {
	      dEdx = emcalc.GetDEDX(kinEnergyPre, part, mate);
	    }
	    G4double elost = stepLengths[ii] * dEdx;
#ifdef VERBOSE_DOSEDEP
	    if(verbose) G4cout  << "GmG4PSDoseDepositVector::FillScorer"<< ii << " RN: iter" << iiter << " energy lost " << elost 
				<< " energy at interaction " << kinEnergyPre 
				<< " = stepLength " << stepLengths[ii] 
				<< " * dEdx " << dEdx << G4endl;
#endif
	    kinEnergyPre -= elost;
	    kinELost[ii] = elost;
	    totalELost += elost;
	  }
	  
	}
	
	//correct energies so that they reproduce the real step energy lost
	G4double enerRatio = (aStep->GetTotalEnergyDeposit()/totalELost);
	
#ifdef VERBOSE_DOSEDEP
	if(verbose) G4cout  << "GmG4PSDoseDepositVector::FillScorer"<< ii << " RN: iter" << iiter << " energy ratio " << enerRatio << G4endl;
#endif
	
#ifdef VERBOSE_DOSEDEP
	G4double elostTot = 0.; 
#endif
	for( ii = 0; ii < kinELost.size(); ii++ ){
	  kinELost[ii] *= enerRatio;
#ifdef VERBOSE_DOSEDEP
	  elostTot += kinELost[ii];
	  if(verbose) G4cout  << "GmG4PSDoseDepositVector::FillScorer "<< ii << " RN: iter" << iiter << " corrected energy lost " << kinELost[ii] 
			      << " orig elost " << kinELost[ii]/enerRatio 
			      << " energy before interaction " << kinEnergyPreOrig-elostTot+kinELost[ii]
			      << " energy after interaction " << kinEnergyPreOrig-elostTot
			      << G4endl;
#endif
	}
      }
      
      //---- Compute the dose (for N iterations)
      G4double valsum = 0.;
      G4double volume = theGeomUtils->GetCubicVolume( aStep->GetPreStepPoint()->GetPhysicalVolume() );
      G4double density = aStep->GetTrack()->GetMaterial()->GetDensity();
      
      for( ii = 0; ii < kinELost.size(); ii++ ){
	G4double dose  = kinELost[ii] / ( density * volume );
	G4double valwei = dose*wei;
	G4int index = rnsl[ii].first;
#ifdef VERBOSE_DOSEDEP
	if(verbose) G4cout  << "GmG4PSDoseDepositVector::FillScorer"<< ii 
			    << " dose " << valwei
			    << " in " << index
			    << " RN: deposited energy " << kinELost[ii]*wei 
			    << G4endl;
#endif
	
	GmVPrimitiveScorerVector::FillScorer( index, dose, wei );
	valsum += valwei;
	
      }
      
      //      G4cout << "VAL-VALSUM " << (val-valsum)/val << " VAL " << val << " VALSUM " << valsum << G4endl;
#ifndef GAMOS_NO_VERBOSE
      if( (val-valsum)/val > 1.E-9 ) if( ScoringVerb(debugVerb) ) G4cout << "GmG4PSDoseDepositVector::FillScorer"<< "ERRORVAL-VALSUM " << (val-valsum)/val << " VAL " << val << " VALSUM " << valsum << G4endl;
#endif
    }
  }

  G4RegularNavigationHelper::Instance()->ClearStepLengths();

  return TRUE;
} 


//--------------------------------------------------------------------
G4double GmG4PSDoseDepositVector::GetGeom2TrueStepLength( G4double kinEnergy ) 
{

  G4double g2tratio;
  if( kinEnergy < 0.04 ) {
    g2tratio = 1.776 - 22.4*kinEnergy + 988.6*pow(kinEnergy,2.) -2.231e4*pow(kinEnergy,3.) + 1.718e5*pow(kinEnergy,4.);
  } else if( kinEnergy < 1. ) {
    g2tratio = 1.405 - 0.9334*kinEnergy + 1.072*pow(kinEnergy,2.) -0.4395*pow(kinEnergy,3.);
  } else {// if( kinEnergy < 6. ) {
    g2tratio = 1.1 + 0.008554*kinEnergy - 0.009195*pow(kinEnergy,2.) + 0.001009*pow(kinEnergy,3.);
  }

  return g2tratio;
}

//--------------------------------------------------------------------
void GmG4PSDoseDepositVector::EndOfEvent(G4HCofThisEvent*)
{
}

//--------------------------------------------------------------------
void GmG4PSDoseDepositVector::DrawAll()
{;}

//--------------------------------------------------------------------
void GmG4PSDoseDepositVector::PrintAll()
{
  G4cout <<" GmG4PSDoseDepositVector::PrintAllDefault() " << G4endl;
  G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
  G4cout << " PrimitiveScorer " << GetName() << G4endl;
  G4cout << " Number of entries " << EvtMap.size() << G4endl;
  for( unsigned idx = 0; idx < EvtMap.size(); idx++){
    G4cout << "  copy no.: " << idx
	   << "  dose deposit: " << G4BestUnit(EvtMap[idx],"Dose")
	   << G4endl;
  }
}
 #include "GamosCore/GamosBase/Base/include/GmVClassifier.hh" 
G4int GmG4PSDoseDepositVector::GetIndex(G4Step* aStep ) 
 { 
 return theClassifier->GetIndexFromStep( aStep ); 
} 
