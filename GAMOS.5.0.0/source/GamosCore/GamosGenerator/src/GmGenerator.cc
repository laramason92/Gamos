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
#include "GmGenerator.hh"
#include "GmGeneratorMessenger.hh"
#include "GmIsotopeMgr.hh"
#include "GmSingleParticleSource.hh"
#include "GmIsotopeSource.hh"
#include "GmDoubleBackToBackParticleSource.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosData/Distributions/include/GmVNumericDistribution.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmMovementUtils.hh"

#include "CLHEP/Random/RandFlat.h"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4PrimaryVertex.hh"
#include "globals.hh"

#include <set>

//------------------------------------------------------------------------
GmGenerator::GmGenerator()
{
  theIsotopeMgr = GmIsotopeMgr::GetInstance();
  new GmGeneratorMessenger( this );

  GmParameterMgr* parmgr = GmParameterMgr::GetInstance(); 
  G4String fname = parmgr->GetStringValue("Generator:Isotope:FileName", "isotopes.dat");
  theIsotopeMgr->ReadIsotopes( fname );

  theLastEventTime = 0.;

 // GmMovementUtils::SetbUsingGmGenerator( true );

  bBiasDistributions = false;
}

//------------------------------------------------------------------------
GmGenerator::~GmGenerator()
{
}

//----------------------------------------------------------------------
void GmGenerator::AddSingleParticleSource( const G4String& newValues )
{
  G4String sourceName;
  G4String particleName;
  G4String energyStr;
  G4String unitStr;
  G4double energy;
  
  std::vector<G4String> wl = GmGenUtils::GetWordsInString(newValues);
  if( wl.size() != 3 ) {
    G4Exception("GmGenerator::AddSingleParticleSource",
		"Wrong argument",
		FatalErrorInArgument,
		G4String("There must be three parameters: SOURCE_NAME PARTICLE_NAME ENERGY " + newValues).c_str());
  }
  energy = GmGenUtils::GetValue( wl[2] ); 
  
  GmSingleParticleSource* parts = new GmSingleParticleSource( wl[0], wl[1], energy );
  theSources.push_back( parts );
}

//----------------------------------------------------------------------
void GmGenerator::AddDoubleBackToBackParticleSource( const G4String& newValues )
{
  G4String sourceName;
  G4String particleName;
  G4String energyStr;
  G4String unitStr;
  G4double energy;
  
  std::vector<G4String> wl = GmGenUtils::GetWordsInString(newValues);
  if( wl.size() != 3 ) {
    G4Exception("GmGenerator::AddDoubleBackToBackParticleSource",
		"Wrong argument",
		FatalErrorInArgument,
		G4String("There must be three parameters: SOURCE_NAME PARTICLE_NAME ENERGY " + newValues).c_str());
  }
  energy = GmGenUtils::GetValue( wl[2] ); 
  
  GmDoubleBackToBackParticleSource* parts = new GmDoubleBackToBackParticleSource( wl[0], wl[1], energy );
  theSources.push_back( parts );
}


//----------------------------------------------------------------------
void GmGenerator::AddIsotopeSource( const G4String& newValues )
{
  G4String sourceName;
  G4String isoName;
  G4double activity;
  
  std::vector<G4String> wl = GmGenUtils::GetWordsInString(newValues);
  if( wl.size() != 3 ) {
    G4Exception("GmGenerator::AddIsotopeSource",
		"Wrong argument",
		FatalErrorInArgument,
		G4String("There must be three parameters: SOURCE_NAME PARTICLE_NAME ACTIVITY " + newValues).c_str());
  }
  activity = GmGenUtils::GetValue( wl[2] );
  
  theSources.push_back( theIsotopeMgr->AddActiveIsotopeSource( wl[0], wl[1], activity ) );
}


//----------------------------------------------------------------------
void GmGenerator::SetDistribution( const G4String& distType, std::vector<G4String>& wl )
{
  G4String sourceName = wl[0];
  G4String distName = wl[1];
  std::vector<G4String> extraParams;
  for( unsigned int ii = 2; ii < wl.size(); ii++ ){
    extraParams.push_back(wl[ii]);
  }

  GmParticleSource* source = FindSource( wl[0] );

  if( distType == "time" ) {
    source->SetDistributionTime( wl[1], extraParams );
  } else if( distType == "energy" ) {
    source->SetDistributionEnergy( wl[1], extraParams );
  } else if( distType == "position" ) {
    source->SetDistributionPosition( wl[1], extraParams );
  } else if( distType == "direction" ) {
    source->SetDistributionDirection( wl[1], extraParams );
  }
}

//------------------------------------------------------------------------
GmParticleSource* GmGenerator::FindSource( const G4String& name, G4bool mustExist )
{
  GmParticleSource* source = 0;
  std::vector<GmParticleSource*>::const_iterator ite;
  for( ite = theSources.begin(); ite != theSources.end(); ite++ ){
    if( (*ite)->GetName() == name ) source = *ite;
  }

  if( !source ) {
    if( mustExist ) {
      G4Exception(" GmGenerator::FindSource",
		  "Wrong Argument",
		  FatalErrorInArgument,
		  G4String("Source not found: " + name ).c_str()); 
    } else {
      G4Exception(" GmGenerator::FindSource",
		  "Warning",
		  JustWarning,
		  G4String("Source not found: " + name ).c_str()); 
    }
  }

  return source;
}


//------------------------------------------------------------------------
void GmGenerator::GeneratePrimaries(G4Event* evt)
{
  if( theSources.size() == 0 ) {
    G4Exception("GmGenerator::GeneratePrimaries",
		"Wrong Argument",
		FatalErrorInArgument,
		"No active source : select it with command '/gamos/generator/addSingleParticleSource  or  /gamos/generator/addIsotopeSource'");
  }

  std::vector<GmParticleSource*>::const_iterator ite;
  std::set<GmParticleSource*> currentSources;
  std::set<GmParticleSource*>::const_iterator ite2;
  //----- Select smaller time
  G4double timeMin = DBL_MAX;
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << "GmGenerator::GeneratePrimaries, Nsources " << theSources.size() << G4endl;
#endif
  std::multimap<G4double, GmParticleSource*> times;

  for( ite = theSources.begin(); ite != theSources.end(); ite++ ){
    (*ite)->SetWeight( 1. );
    G4double tim = (*ite)->GenerateTime();
    if( bBiasDistributions ) {
      BiasTime(*ite, tim);
    }

    if( tim < timeMin ) timeMin = tim;
    times.insert(std::multimap<G4double, GmParticleSource*>::value_type( tim, *ite));
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << "GmGenerator::GeneratePrimaries, source " << (*ite)->GetName() << " time proposed " << tim << " <? " << timeMin << G4endl;
#endif
  }

  //---- For the current source set last time to 0. and for the other sources add the time to the last time active 
  std::multimap<G4double, GmParticleSource*>::const_iterator itets;
  for( itets = times.begin(); itets != times.end() ; itets++ ){
    G4double tim = (*itets).first;
    GmParticleSource* source = (*itets).second;
    if( tim <= timeMin ) { // select both sources
#ifndef GAMOS_NO_VERBOSE
      if( GenerVerb(debugVerb) ) G4cout << "GmGenerator::GeneratePrimaries, in event " << evt->GetEventID() << " add source " << source->GetName() << " time proposed " << tim << " <? " << timeMin << G4endl;
#endif
      currentSources.insert( source );
      source->LastTimeActive(0.);
    } else {
      source->LastTimeNotActive( timeMin );
    }
  }

  theLastEventTime += timeMin;
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << "GmGenerator::GeneratePrimaries  event time= " << theLastEventTime << " increase= " << timeMin << G4endl;
#endif

  for( ite2 = currentSources.begin(); ite2 != currentSources.end(); ite2++ ){
    evt->AddPrimaryVertex( (*ite2)->GenerateVertex( theLastEventTime ) );
  }
  
}


//------------------------------------------------------------------------
void GmGenerator::DeleteSources()
{
  std::vector<GmParticleSource*>::iterator ite;
  for( ite = theSources.begin(); ite != theSources.end(); ite++ ){
    delete *ite;
  }

  theSources.clear();
}

//------------------------------------------------------------------------
void GmGenerator::ReInitialiseDistributions()
{
  std::vector<GmParticleSource*>::const_iterator ite;
  for( ite = theSources.begin(); ite != theSources.end(); ite++ ){
    (*ite)->ReInitialiseDistributions();
  }

}

//---------------------------------------------------------------------
void GmGenerator::AddBiasDistribution( const G4String& sourceName, const G4String& varName, const G4String& biasDistName )
{
  if( theSources.size() == 0 ) {
    G4Exception("GmGenerator::AddBiasDistribution",
		"Error ",
		FatalException,
		"No particle source is defined yet. Define a particle source first");
  }
  
  std::vector<GmParticleSource*>::const_iterator ite;

  for( ite = theSources.begin(); ite != theSources.end(); ite++ ){
    if( (*ite)->GetName() == sourceName ) {
      if( varName == "PosX"
	  || varName == "PosY" 
	  || varName == "PosZ" 
	  || varName == "PosR2" 
	  || varName == "PosR" 
	  || varName == "PosPhi" 
	  || varName == "PosTheta" 
	  || varName == "DirTheta" 
	  || varName == "DirPhi" 
	  || varName == "Energy" 
	  || varName == "Time" ) {
	(*ite)->AddBiasDistribution( varName, biasDistName );
      }else {
	G4Exception("GmGenerator::AddBiasDistribution",
		    "Wrong variable name",
		    FatalErrorInArgument,
		    G4String("Available names are: PosX PosY PosZ PosR2 PosR PosTheta PosPhi Energy Time, you are using " + varName).c_str());
      }
    }
  }

  bBiasDistributions = true;

}


//-----------------------------------------------------------------------
void GmGenerator::BiasTime(GmParticleSource* source, G4double& time)
{
  std::map<G4String,GmVNumericDistribution*> distributions = source->GetBiasDistributions();
  std::map<G4String,GmVNumericDistribution*>::const_iterator ite = distributions.find("Time");
  if( ite != distributions.end() ) {
    G4int ii = 0;
    G4int maxBiasIterations = source->GetMaxBiasIterations();
    for( ;; ) {
      G4double val = (*ite).second->GetNumericValueFromIndex( time );
      G4double rnd = CLHEP::RandFlat::shoot();
      if( rnd > val ) {
	source->SetWeight( source->GetWeight() / val );
	break;
      }
      time = source->GenerateTime();
      ii++;
      if( ii == maxBiasIterations ) {
	G4Exception("GmSingleParticleSource::BiasTime",
		    "Warning",
		    JustWarning,
		    G4String("Too many iterations to calculate bias. N iterations = " + GmGenUtils::itoa(ii)).c_str());
      }
    }
    
  }

}
