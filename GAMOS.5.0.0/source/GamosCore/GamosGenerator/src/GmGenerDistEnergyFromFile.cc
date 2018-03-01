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
#include "GmGenerDistEnergyFromFile.hh"
#include "GmGenerVerbosity.hh"

#include "CLHEP/Random/RandFlat.h"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//-----------------------------------------------------------------------
GmGenerDistEnergyFromFile::GmGenerDistEnergyFromFile()
{

}

//-----------------------------------------------------------------------
void GmGenerDistEnergyFromFile::ReadEnergyDist()
{
  // Read energy - probability  pairs
  G4String path( getenv( "GAMOS_SEARCH_PATH" ) );
  theFileName = GmGenUtils::FileInPath( path, theFileName );

  GmFileIn fin = GmFileIn::GetInstance(theFileName);

  std::vector<G4String> wl;
  std::map<G4double,G4double> enerprob;
  G4int ii = 1;
  for( ;; ){
    if(! fin.GetWordsInLine( wl ) ) break;
    if( wl.size() != 2) {
      G4Exception("GmGenerDistEnergyFromFile::ReadEnergyDist",
		  "Error in number of words in line",
		  FatalErrorInArgument,
		  G4String("Reading file " + theFileName 
			   + ", line number " + GmGenUtils::itoa(ii) 
			   + " All lines must have two words: ENERGY PROBABILITY").c_str());
    }
    enerprob[ GmGenUtils::GetValue( wl[0] ) * theUnit ] = GmGenUtils::GetValue( wl[1] );
    ii++;
  }

  /*  //--- For interpolate: add one bin at the end 
  if( theCalculationType == EFFCT_Interpolate || theCalculationType == EFFCT_InterpolateLog ){
    std::map<G4double,G4double>::const_iterator ite1 = enerprob.end();
    ite1--;
    std::map<G4double,G4double>::const_iterator ite2 = enerprob.end();
    ite2--; ite2--;
    enerprob[(*ite1).first+ (*ite1).first-(*ite2).first] = 0.;//1.e-20;  // if =0., it find the last instead of the second to last
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistEnergyFromFile::ReadEnergyDist add one bin for interpolate " << (*ite1).first+ (*ite1).first-(*ite2).first << " = 0. " << G4endl;
#endif
    }*/

  //---- For histogram: Check that the binning is fixed
  std::map<G4double,G4double>::iterator ite, ite2;
  if( theCalculationType == EFFCT_Histogram ){
    for(ite = enerprob.begin(); ite != enerprob.end(); ite++){
      ite2 = ite; ite2++;
      if( ite == enerprob.begin() ) {
	theHBin = (*ite2).first - (*ite).first;
      }
      if( ite2 != enerprob.end() ){
	G4double hbinn = (*ite2).first - (*ite).first;
	if( fabs(hbinn - theHBin) > 1.E-6*theHBin ) {
	  G4Exception("GmGenerDistEnergyFromFile::ReadEnergyDist",
		      "Asking for histogram type, and energy bin is not fixed",
		      FatalErrorInArgument,
		      G4String("Previous bin = " + GmGenUtils::ftoa(theHBin) + " new bin = " + GmGenUtils::ftoa(hbinn) + " difference= " + GmGenUtils::ftoa(hbinn-theHBin) ).c_str());
	}
      }
    }
  }


  //---- Calculate sum of probabilities to normalize 
  G4double tp = 0.;
  for(ite = enerprob.begin(); ite != enerprob.end(); ite++){
    tp += (*ite).second;
  }
#ifndef GAMOS_NO_VERBOSE
  if( tp != 1. && GenerVerb(warningVerb) ) G4cerr << "GmGenerDistEnergyFromFile::ReadEnergyDist probabilities do not sum up to 1., but to " << tp << " They will be normalized to 1. " << G4endl;
#endif
  G4double maxProbInv = 1./tp;

  //--- Get the inverse of probabilities - energies distribution
  tp = 0.;
  for(ite = enerprob.begin(); ite != enerprob.end(); ite++){
    G4double prob = (*ite).second; 
      if( prob == 0. && std::distance( enerprob.begin(), ite ) == G4int(enerprob.size())-1 ) {
      prob = 1.E-10; // for last bin limit the probability is set to 0. by convention
    }
    tp += prob * maxProbInv;
    theProbEner.insert( std::multimap<G4double,G4double>::value_type(tp, (*ite).first));
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(infoVerb) ) G4cout << "GmGenerDistEnergyFromFile  Reading data: Energy =  " << (*ite).first 
	   << " Integrated probability= " << tp << G4endl;
#endif
  }

}

//-----------------------------------------------------------------------
G4double GmGenerDistEnergyFromFile::GenerateEnergy( const GmParticleSource* )
{
  G4double energy;
  G4double pv = CLHEP::RandFlat::shoot();
  std::multimap<G4double,G4double>::iterator ite = theProbEner.upper_bound( pv );
  energy = (*ite).second;

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << "GmGenerDistEnergyFromFile energy bin found =  " << energy << " prob = " << pv << G4endl;
#endif
  /* Keep it in case other calculation types are requested
  if( theCalculationType != EFFCT_Fixed ){
    if( ite != theProbEner.end() ){
      if( ite != theProbEner.begin() ){
	ite--;
	ener1 = (*ite).second;
      } else {
	ener1 = 0.;
      }
      
    } else {
      //precision problem, it should not happen
      G4Exception("GmGenerDistEnergyFromFile::GenerateEnergy",
		  "Probability too small, this should not happen",
		  FatalErrorInArgument,
		  G4String("probability is = " + GmGenUtils::ftoa(pv)).c_str());
    }
    }*/
  
  if( theCalculationType == EFFCT_Histogram ){
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << "GmGenerDistEnergyFromFile histogram prev energy =  " << energy << G4endl;
#endif
    energy += (-0.5 + CLHEP::RandFlat::shoot() ) * theHBin;
  } else if( theCalculationType == EFFCT_Interpolate ){
    if( std::distance(ite, theProbEner.end() ) != 1) ite++;  //because of precision it may happen that it returns the last one when pv=0.99999...
    G4double energyBin = (*ite).second - energy;
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << "GmGenerDistEnergyFromFile old energy =  " 
				      << energy << " second energy " << (*ite).second << G4endl;
#endif
    energy +=  energyBin * CLHEP::RandFlat::shoot();
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << "GmGenerDistEnergyFromFile new energy =  " 
				      << energy << " energyBin " <<  energyBin << G4endl;
#endif
  } else if( theCalculationType == EFFCT_InterpolateLog ){
    if( std::distance(ite, theProbEner.end() ) != 1) ite++;  //because of precision it may happen that it returns the last one when pv=0.99999...
    G4double energyBin = log((*ite).second) - log(energy);
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << "GmGenerDistEnergyFromFile old energy =  " 
				      << energy << " second energy " << (*ite).second << " energyBin " <<  energyBin << G4endl;
#endif
    energy *= exp(energyBin * CLHEP::RandFlat::shoot());
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << "GmGenerDistEnergyFromFile new energy =  " 
				      << energy << G4endl;
#endif
  }
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << "GmGenerDistEnergyFromFile  energy =  " 
				    << energy << G4endl;
#endif

  return energy;

}

//---------------------------------------------------------------------
void GmGenerDistEnergyFromFile::SetParams( const std::vector<G4String>& params )
{

  G4int nParams = params.size();
  theUnit = 1.;
  G4String calcType = "histogram";

  switch (nParams) {
  case 3:
    theUnit = GmGenUtils::GetValue( params[2] );
  case 2:
    calcType = params[1];
  case 1:
    theFileName = params[0];
    break;
  default:
    G4Exception("GmGenerDistEnergyFromFile::SetParams",
		"Wrong number of parameters", 
		FatalErrorInArgument,
		G4String("There should be 1, 2 or 3 parameters: FILE_NAME CALCULATION_TYPE UNIT, there are " + GmGenUtils::itoa(nParams)).c_str());
  }

  if( calcType == "fixed" ) {
    theCalculationType = EFFCT_Fixed;
  } else if( calcType == "histogram" ) {
    theCalculationType = EFFCT_Histogram;
  } else if( calcType == "interpolate" ) {
    theCalculationType = EFFCT_Interpolate;
  } else if( calcType == "interpolate_log" ) {
    theCalculationType = EFFCT_InterpolateLog;
  } else {
    G4Exception("GmGenerDistEnergyFromFile::SetParams",
		"Error in type of calculation", 
		  FatalErrorInArgument,
		  G4String("only options supported: fixed , histogram, you have selected " + calcType).c_str());
  }

  ReadEnergyDist();

}
