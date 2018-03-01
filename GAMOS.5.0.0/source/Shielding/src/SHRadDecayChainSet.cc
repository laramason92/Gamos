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
#include "SHRadDecayChainSet.hh"
#include "SHRadDecayChain.hh"
#include "ShieldingVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4Track.hh"
#include "G4DecayTable.hh"
#include "G4ITDecayChannel.hh"
#include "G4NuclearLevelManager.hh"
#include "G4NuclearLevelStore.hh"
#include "G4RadioactiveDecay.hh"


//-----------------------------------------------------------------
SHRadDecayChainSet::SHRadDecayChainSet()
{
  theNEventsInJob = 0.;
  theJobTime = 0.;
  theNEventsPerNanoSecond = 0.;

}

//-----------------------------------------------------------------
SHRadDecayChainSet::SHRadDecayChainSet( G4String name )
  :theName( name )
{
  theNEventsInJob = 0.;
  theJobTime = 0.;
  theNEventsPerNanoSecond = 0.;

}

//-----------------------------------------------------------------
void SHRadDecayChainSet::AddData(std::vector<G4String>& wl )
{
  
  SHRadDecayChain* isot = 0;

  if( wl.size() > 9 && wl[2] == "NUMBER_OF_EVENTS_PER_SECOND=" ) {
    AddNEvents( wl );

  } else if( wl.size() > 4 && wl[2] == "PERIODS:" ) {
    AddPeriods( wl );

#ifndef GAMOS_NO_VERBOSE
    if( SHVerb(debugVerb) ) Dump(G4cout);
#endif

  } else if( wl.size() > 10 && wl[6] == "ACTIVITY=" ) {
    //%%% SHActivationUA_inCameraF TIME= 2.592e+06 sec Cr50[0.0]  ACTIVITY= 2.97071e-12 decays/sec  LIFETIME= 8.19451e+24 sec  NPART_IN_JOB= 6 NATOMS= 3.51203e+13

    G4String isotName = wl[5];
#ifndef GAMOS_NO_VERBOSE
    if( SHVerb(debugVerb) ) G4cout << theName << " SHRadDecayChainSet::AddData ADDING ISOT " << isotName << G4endl;
#endif
    double time = atof( wl[3].c_str());
    theTimes.insert( time );

    //    if( isotName == "neutron" ) continue;
    std::map<G4String,SHRadDecayChain*>::const_iterator ites = theChains.find( isotName );
    if( ites == theChains.end() ) {
      isot = new SHRadDecayChain(isotName, this);
      theChains[isotName] = isot;
    } else {
      isot = (*ites).second;
    }
    isot->AddData( wl );
  }
  
}

//-----------------------------------------------------------------
void SHRadDecayChainSet::AddNEvents(std::vector<G4String>& wl )
{
  //%%%%%% SHActivationUA_inCameraF:  NUMBER_OF_EVENTS_PER_SECOND= 2.28333e+10 = 10000 events in 437.956 nanoseconds 
  double nEventsPerNanoSecond = GmGenUtils::GetValue( wl[3] )/second;
  double nEventsInJob = GmGenUtils::GetValue( wl[5] );
  //  double jobTime = GmGenUtils::GetValue( wl[8]+"*"+wl[9] ); // in nanoseconds
  double jobTime = GmGenUtils::GetValue( wl[8] ); // in nanoseconds
  //first time 
  if( theNEventsPerNanoSecond != 0 && fabs(theNEventsPerNanoSecond - nEventsPerNanoSecond)/theNEventsPerNanoSecond > 1.E-9 ) {
    G4Exception("SHRadDecayChainSet::AddNEvents",
		"Warning",
		JustWarning,
		G4String("Two jobs with different NeventsPerNanoSecond: " + GmGenUtils::ftoa(theNEventsPerNanoSecond) + " <> " + wl[3]).c_str());
  }
 
  theNEventsInJob += nEventsInJob;
  theJobTime += jobTime;
  theNEventsPerNanoSecond = theNEventsInJob / theJobTime; // it is in nanoseconds
#ifndef GAMOS_NO_VERBOSE
  if( SHVerb(debugVerb) ) {
    G4cout << this << theName << " SHRadDecayChainSet::AddNEvents " << nEventsInJob << " jobTime " <<  jobTime << " nEventsPerNanoSecond " << nEventsPerNanoSecond << G4endl; 
    G4cout << theName << " SHRadDecayChainSet::AddNEvents " << theNEventsInJob << " theJobTime " <<  theJobTime << " theNEventsPerNanoSecond " << theNEventsPerNanoSecond << G4endl; 
  }
#endif

  Dump(G4cout);

}

//-----------------------------------------------------------------
void SHRadDecayChainSet::AddPeriods(std::vector<G4String>& wl )
{
  //%%%%%% SHActivationUA_inCameraF PERIODS: 3.1536e+07 1 3.1536e+08 0
  if( wl.size() % 2 != 1 ) {
    G4Exception("SHRadDecayChainSet::AddPeriods",
		"Error",
		FatalException,
		G4String("Number of words is is uneven = " + GmGenUtils::itoa(wl.size()-3) + " It should be PERIOD_1 ISACTIVE_1 PERIOD_2 ISACTIVE_2 ...").c_str());
  }
 
  for( unsigned ii = 3; ii < wl.size(); ii +=2 ) {
    thePeriods[GmGenUtils::GetValue(wl[ii])] = G4bool(GmGenUtils::GetValue(wl[ii+1]));
#ifndef GAMOS_NO_VERBOSE
    if( SHVerb(debugVerb) ) G4cout  << theName << " SHRadDecayChainSet::AddPeriods " << wl[ii] << " : " << wl[ii+1] << G4endl;
#endif
  }

}

//-----------------------------------------------------------------
void SHRadDecayChainSet::AddTime(std::vector<G4String>& wl )
{
  theTimes.insert( GmGenUtils::GetValue(wl[3]) );
}


/*
//-----------------------------------------------------------------
void SHRadDecayChainSet::AddIon( vi* chain )
{
  SHRadDecayChain* radChain = new SHRadDecayChain( chain, this );

  theChains[ GetChainName( chain ) ] = radChain;

}
*/

//-----------------------------------------------------------------
void SHRadDecayChainSet::AddChain( SHRadDecayChain* chain)
{
  theChains[chain->GetName()] = chain;
}

//-----------------------------------------------------------------
void SHRadDecayChainSet::StoreIon(const G4Track* aTrack, G4bool bAlsoLifeTime0 )
{
  G4ParticleDefinition* particle = aTrack->GetDefinition();

#ifndef GAMOS_NO_VERBOSE
  if( SHVerb(debugVerb) ) G4cout << " SHRadDecayChainSet::StoreIsotope TRKID " << aTrack->GetTrackID() << " PARTICLE " << particle->GetParticleName() << " TYPE= " << particle->GetParticleType() << " SUBTYPE= " << particle->GetParticleSubType() << " STABLE? " << particle->GetPDGStable() << " LIFETIME " << particle->GetPDGLifeTime() << G4endl;
#endif

//t bAlsoLIfeTime0 is true when called by SHActivationUA ?!
  const G4RadioactiveDecay* procDec = dynamic_cast<const G4RadioactiveDecay*>(aTrack->GetCreatorProcess());
  if( procDec != 0 ) return; // no ions from radioactive decay

   if( particle->GetParticleType() == "nucleus" && 
      ( bAlsoLifeTime0 || !particle->GetPDGStable() ) ) {
    theIons[(G4Ions*)(particle)] += aTrack->GetWeight();
  }

}

//-----------------------------------------------------------------
void SHRadDecayChainSet::StoreParentChildren(const G4Track* aTrack, G4bool bAlsoLifeTime0 )
{
  G4ParticleDefinition* particle = aTrack->GetDefinition();

#ifndef GAMOS_NO_VERBOSE
  if( SHVerb(debugVerb) ) G4cout << " SHRadDecayChainSet::StoreParentChildren TRKID " << aTrack->GetTrackID() << " PARTICLE " << particle->GetParticleName() << " TYPE= " << particle->GetParticleType() << " SUBTYPE= " << particle->GetParticleSubType() << " STABLE? " << particle->GetPDGStable() << " LIFETIME " << particle->GetPDGLifeTime() << G4endl;
#endif

  const G4RadioactiveDecay* procDec = dynamic_cast<const G4RadioactiveDecay*>(aTrack->GetCreatorProcess());
  if( procDec != 0 ) return; // not created from radioactive decay

  //t bAlsoLifeTime0 ??
  G4Ions* ion = dynamic_cast<G4Ions*>(particle);
  if( ion && 
      ( bAlsoLifeTime0 || !particle->GetPDGStable() ) ) {
     G4int trkID = aTrack->GetTrackID();
    theParentChildren[trkID] = aTrack->GetParentID();
    theIonsMap[trkID] = ion;
  }

}


//---------------------------------------------------------------------------
void SHRadDecayChainSet::StoreDecayBranchingRatios( const G4Track* aTrack )
{
  G4ParticleDefinition* part = aTrack->GetDefinition();
  G4DecayTable* theDecayTable = part->GetDecayTable();
#ifndef GAMOS_NO_VERBOSE
  if( SHVerb(debugVerb) ) G4cout << " SHRadDecayChainSet::StoreDecayBranchingRatios " << part->GetParticleName() << " STABLE " << part->GetPDGStable() << " DECAYTABLE " << theDecayTable  << G4endl;
#endif
  if( !theDecayTable ) return;

  mpmpd::const_iterator item = theBranchingRatios.find( part );
  if( item != theBranchingRatios.end() ) {
    return; // already done this particle
  }

  G4int nDecays = theDecayTable->entries();
#ifndef GAMOS_NO_VERBOSE
  if( SHVerb(debugVerb) ) G4cout << " SHRadDecayChainSet::StoreDecayBranchingRatios " << part->GetParticleName() << " N DECAYS " << nDecays << G4endl;
#endif
  for( G4int ii = 0; ii < nDecays; ii++ ){
    G4VDecayChannel* decChan = theDecayTable->GetDecayChannel(ii);
    
    theBranchingRatios[part] = new mpd;
    item = theBranchingRatios.find( part );
#ifndef GAMOS_NO_VERBOSE
    if( SHVerb(debugVerb) ) G4cout << " SHRadDecayChainSet::StoreDecayBranchingRatios add particle parent " << part->GetParticleName() << G4endl;
#endif
  
    mpd* BR = (*item).second;
    G4int nDaug = decChan->GetNumberOfDaughters();
 #ifndef GAMOS_NO_VERBOSE
    if( SHVerb(debugVerb) ) G4cout << " SHRadDecayChainSet::StoreDecayBranchingRatios decay channel " << ii << " = " << decChan->GetKinematicsName() << " parent " << decChan->GetParentName() << " nDaughters " << nDaug << G4endl;
#endif
    mpd::iterator iteBR;
    for( G4int jj = 0; jj < nDaug; jj++ ) {      
      G4ParticleDefinition * daug = decChan->GetDaughter(jj);
      if( daug->GetParticleType() == "nucleus" && daug->GetAtomicMass() > 4. ) {
	iteBR = BR->find(daug);
	if( iteBR == BR->end() ) {
	  (*BR)[daug] = decChan->GetBR();
#ifndef GAMOS_NO_VERBOSE
	  if( SHVerb(debugVerb) ) G4cout << " SHRadDecayChainSet::StoreDecayBranchingRatios add daughter " << daug->GetParticleName() << " BR : " << decChan->GetBR() << G4endl;
#endif
	} else {
	  (*iteBR).second += decChan->GetBR();
#ifndef GAMOS_NO_VERBOSE
	  if( SHVerb(debugVerb) ) G4cout << " SHRadDecayChainSet::StoreDecayBranchingRatios add BR to existing daughter " << daug->GetParticleName() << " BR : " << decChan->GetBR() << G4endl;
#endif
	}
      } else {
#ifndef GAMOS_NO_VERBOSE
	if( SHVerb(debugVerb) ) G4cout << " SHRadDecayChainSet::StoreDecayBranchingRatios NO add daughter BR " << daug->GetParticleName() << G4endl;
#endif
      }
    }
  }
  
}


//-----------------------------------------------------------------
std::map<G4String,SHRadDecayChain*> SHRadDecayChainSet::BuildPossibleDecayChains()
{
  theBranchingRatios.clear();
  
  // for each ion build any possible decay chain, looking at the decay branching ratios from the tables
  std::map<G4Ions*,G4double>::const_iterator itei;
  for( itei = theIons.begin(); itei != theIons.end(); itei++ ){
    G4Ions* ion = (*itei).first;
    SHRadDecayChain* chain = new SHRadDecayChain(ion);
#ifndef GAMOS_NO_VERBOSE
    if( SHVerb(testVerb) )
    G4cout << " NEW CHAIN " << chain->GetName() << G4endl; //GDEB
#endif
    theChainVector.push_back( chain );
    chain->SetNAtomsInJob( (*itei).second );
    // add daughters
    AddDaughters( chain );
  }
  
  DeleteShortLived();

  // merge chain with equal name (for example a KshellEC will produce the same chain as a MshellEC). And after deleting short lived they may become with the same name
  for( unsigned int ii = 0; ii < theChainVector.size(); ii++ ) {
    G4bool bFound = false;
    unsigned int jj = 0;
    for( ; jj < ii; jj++ ) {
      if( *(theChainVector[ii]) == *(theChainVector[jj]) ) {
	bFound = true;    
	break;
      }
    }
#ifndef GAMOS_NO_VERBOSE
    if( SHVerb(testVerb) )
    G4cout << theChainVector[ii] << " CHECK CHAIN " << theChainVector[ii]->GetName() << " NIONS " << theChainVector[ii]->GetIons().size() << " BR " << theChainVector[ii]->GetBR() <<G4endl;
#endif
    if( bFound ) {
      theChainVector[jj]->AddBR( theChainVector[ii]->GetBR() );
#ifndef GAMOS_NO_VERBOSE
    if( SHVerb(testVerb) )
      G4cout << " MERGE CHAINS " <<theChainVector[ii]->GetName() << " to " << theChainVector[jj]->GetName() << " : "<< theChains.size() << " BR " << theChainVector[jj]->GetBR() << G4endl;
#endif
    } else {
      theChains[theChainVector[ii]->GetName()] = theChainVector[ii];
      //-      theChainVector[ii]->ResetBR(); // reset it because ions have been deleted
#ifndef GAMOS_NO_VERBOSE
    if( SHVerb(testVerb) )
      G4cout << " ADD CHAIN " << theChainVector[ii]->GetName() << " : "<< theChains.size() << G4endl;
#endif

    }
  }

  //----- Add chains with shorter decay lists : A->B, add A, A->B->C, add A->B
  std::map<G4String, SHRadDecayChain* >::iterator ites, ites2;
  for( ites = theChains.begin(); ites != theChains.end(); ites++ ){
    SHRadDecayChain* chain = (*ites).second;
    G4int nIsot = chain->GetNIons();
    if( nIsot  > 1 ) {
      G4String chainSubName = chain->GetSubName( nIsot-1 );
      if( theChains.find("chainSubName") == theChains.end() ){
	theChains[chainSubName] = new SHRadDecayChain( *chain, nIsot-1 );
      }
    }
  }

  //----- Link the chains with the chainSet
  for( ites = theChains.begin(); ites != theChains.end(); ites++ ){
    (*ites).second->SetSet(this);
    //    (*ites).second->ResetBR();
  }

  return theChains;

}


//-----------------------------------------------------------------
void SHRadDecayChainSet::AddDaughters( SHRadDecayChain* chain )
{
  // get last ion added
  G4Ions* ion = chain->GetIon(chain->GetNIons()-1);
#ifndef GAMOS_NO_VERBOSE
  if( SHVerb(testVerb) )
    G4cout << "  SHRadDecayChainSet::AddDaughters START " << chain->GetName()<< G4endl;//GDEB
#endif

  /*  if( ion->GetPDGStable() ) { e.g. Th228[396.1] is stable, but has a decay table with an IT channel
    G4cout << "  SHRadDecayChainSet::AddDaughters END stable " << chain->GetName()<< G4endl;//GDEB
    return;
    } */

  G4DecayTable* decayTable = FindDecayTable( ion );
  if( !decayTable ) {
#ifndef GAMOS_NO_VERBOSE
    if( SHVerb(debugVerb) )    G4cout << "  SHRadDecayChainSet::AddDaughters END no decayTable " << chain->GetName()<< G4endl;//GDEB
#endif
    return;
  }
  
  G4bool bFirstDaughter = true;
  G4Ions* firstIon = 0;
  G4double firstChannelBR = 1.;
  for( G4int ic = 0; ic < decayTable->entries(); ic++ ) {
    G4VDecayChannel* channel = (*decayTable)[ic];
#ifndef GAMOS_NO_VERBOSE
    if( SHVerb(testVerb) )
    G4cout << ic << " SHRadDecayChainSet::AddDaughters start channel  BR " << channel->GetBR() << G4endl;//GDEB
#endif


    G4ITDecayChannel* channelIT = dynamic_cast<G4ITDecayChannel*>(channel);
    if( ! channelIT ) {
      for( G4int id = 0; id < channel->GetNumberOfDaughters(); id++ ) {
	// add to chain
	G4ParticleDefinition* daughter = channel->GetDaughter(id); 
	G4Ions* daughterIon = dynamic_cast<G4Ions*>(daughter);
#ifndef GAMOS_NO_VERBOSE
    if( SHVerb(testVerb) )
	G4cout << ic << " " << id << " SHRadDecayChainSet::AddDaughters daughter " << daughter->GetParticleName() 
	       << " STABLE? " << daughter->GetPDGStable() << " DECAY_TABLE " << (daughter->GetDecayTable()!=0) << " LIFETIME " << daughter->GetPDGLifeTime() << " FIRST " << bFirstDaughter << G4endl; // GDEB
#endif

	if( daughterIon && daughterIon->GetAtomicMass() > 4. ) {
	  //	  // only unstable ions with decay table
	  //	  if( daughterIon->GetPDGStable() || !daughterIon->GetDecayTable() ) continue;
	  // only unstable ions with decay table // put all to take into account BR, they will be deleted by DeleteShortLived()
	  //	  if( !FindDecayTable(daughterIon) ) continue;
	  
	  if( bFirstDaughter ) {
	    firstIon = daughterIon;
	    firstChannelBR = channel->GetBR();
	    bFirstDaughter = false;
	  } else {
#ifndef GAMOS_NO_VERBOSE
    if( SHVerb(testVerb) )
	    G4cout << " SHRadDecayChainSet::AddDaughters daughter with DECAY TABLE " << daughterIon->GetParticleName() << " " << bFirstDaughter << G4endl;//GDEB
#endif

	    // one chain for each daughter
	    G4double nIonsInJob = chain->GetNAtomsInJob();
	    //look for existing chain
	    //-	  SHRadDecayChain* newChain = FindChain( chain, daughterIon ); 
	    /*-	  if( !newChain ) {
	      newChain = new SHRadDecayChain(*chain); 
	      G4cout << " NEW CHAIN " << newChain->GetName() << G4endl; //GDEB
	      newChain->AddIon( daughterIon, channel->GetBR());
	      theChainVector.push_back( newChain );
	      newChain->SetNAtomsInJob(nIonsInJob );
	      if( !daughter->GetPDGStable() ) AddDaughters( newChain );
	      } else {
	      G4cout << " SHRadDecayChainSet::AddDaughters chain found, adding BR " << chain->GetBR() + channel->GetBR() << " old " << chain->GetBR() << " + " << channel->GetBR() << G4endl;//GDEB
	      chain->SumBR( channel->GetBR() );
	      }*/
	    SHRadDecayChain* newChain = new SHRadDecayChain(*chain); 
#ifndef GAMOS_NO_VERBOSE
    if( SHVerb(testVerb) )
	    G4cout << " NEW CHAIN " << newChain->GetName() << G4endl; //GDEB
#endif
	    newChain->AddIon( daughterIon, channel->GetBR());
	    theChainVector.push_back( newChain );
	    newChain->SetNAtomsInJob(nIonsInJob );
	    //	    if( !daughterIon->GetPDGStable() )
	    AddDaughters( newChain );
	  }
	}
      }
    } else { // channel IT

      G4double Z = ion->GetAtomicNumber();
      G4double A = ion->GetAtomicMass();
      G4double excitation = ion->GetExcitationEnergy();
#ifndef GAMOS_NO_VERBOSE
    if( SHVerb(testVerb) )
      G4cout << " SHRadDecayChainSet::AddDaughters IT Z " << Z << " A " << A << " excit " << excitation << " bFirst " << bFirstDaughter << G4endl;//GDEB
#endif
      G4NuclearLevelManager* levelManager = G4NuclearLevelStore::GetInstance()->GetManager(Z,A);
      const G4NuclearLevel* level =levelManager->NearestLevel(excitation) ;
      if( !level ) continue; // no data in G4LEVELGAMMADATA
#ifndef GAMOS_NO_VERBOSE
    if( SHVerb(testVerb) )
      G4cout << " SHRadDecayChainSet::AddDaughters " << levelManager << " " << level << " levelEnergy " << level->Energy() << " excitationE " << excitation << G4endl;//GDEB
#endif
      if( fabs (excitation - level->Energy() ) > 0.01 ) {
	G4Exception("SHRadDecayChainSet::AddDaughters",
		    "Warning",
		    JustWarning,
		    ("Too big different between ion excitation energy and energy level in G4LEVELGAMMADATA " + GmGenUtils::ftoa(excitation) + " - " + GmGenUtils::ftoa(level->Energy()) + " Z= " + GmGenUtils::itoa(Z) + " A= " + GmGenUtils::itoa(A) ).c_str());
      }
      excitation = level->Energy();
      G4int nGammas = level->NumberOfGammas();
      G4double BR = 0.;
      for( int ig = 0; ig < nGammas; ig++ ) {
	if( ig == 0 ) {
	  BR = (level->GammaCumulativeProbabilities())[ig];
	} else {
	  BR = (level->GammaCumulativeProbabilities())[ig] - (level->GammaCumulativeProbabilities())[ig-1];
	}
#ifndef GAMOS_NO_VERBOSE
    if( SHVerb(testVerb) )
	G4cout << ig << " IT BR " << BR << " accum " << (level->GammaCumulativeProbabilities())[ig] << G4endl;
#endif
	G4double gammaEnergy = (level->GammaEnergies())[ig];
	G4IonTable * iontab= (G4IonTable *)(G4ParticleTable::GetParticleTable()->GetIonTable());
	G4double newExcitation = excitation - gammaEnergy; 
	if( newExcitation < -0.02 ) {
	  G4Exception("SHRadDecayChainSet::AddDaughter",
		      "Error",
		      FatalException,
		      ("gamma energy is smaller than excitation : " + GmGenUtils::ftoa(newExcitation) + " = " + GmGenUtils::ftoa(excitation) + "-" + GmGenUtils::ftoa(gammaEnergy)).c_str());
	}
	if( newExcitation < 0.018 ) { // 18.5 keV is the minimum energy, for z47.a112. If it is smaller is because of precision problems 
	  newExcitation = 0.;
	}
	// correct level energy
	if( newExcitation != 0. ) {
	  const G4NuclearLevel* newLevel =levelManager->NearestLevel(newExcitation);
#ifndef GAMOS_NO_VERBOSE
    if( SHVerb(testVerb) )
	  G4cout << " newExcitation E " << newExcitation << " " <<newLevel->Energy() << G4endl; 
#endif

	  newExcitation = newLevel->Energy();
	}
	if( excitation == newExcitation ) {
	  G4Exception("SHRadDecayChainSet::AddDaughter",
		      "Warning",
		      JustWarning,
		      ("decaying gamma level is the same as parent level : " + GmGenUtils::ftoa(excitation)).c_str());
	}

	G4ParticleDefinition* partnew = iontab->GetIon(Z,A,newExcitation);
	G4Ions* daughterIon = dynamic_cast<G4Ions*>(partnew);
#ifndef GAMOS_NO_VERBOSE
    if( SHVerb(testVerb) )
	G4cout << " SHRadDecayChainSet::AddDaughters IT gamma " << ig << " E= " << gammaEnergy << " BR " << BR << " : " << daughterIon->GetParticleName() << G4endl;//GDEB
#endif

	//	if( !FindDecayTable(daughterIon) ) continue;
	
	if( bFirstDaughter ) {
	  firstIon = daughterIon;
	  firstChannelBR = BR;
	  bFirstDaughter = false;
	} else {
#ifndef GAMOS_NO_VERBOSE
    if( SHVerb(testVerb) )
	  G4cout << " SHRadDecayChainSet::AddDaughters IT daughter NON-STABLE " << daughterIon->GetParticleName() << " " << bFirstDaughter << G4endl;//GDEB
#endif

	  // one chain for each daughter
	  G4double nIonsInJob = chain->GetNAtomsInJob();
	  SHRadDecayChain* newChain = new SHRadDecayChain(*chain); 
#ifndef GAMOS_NO_VERBOSE
    if( SHVerb(testVerb) )
	  G4cout << " IT NEW CHAIN " << newChain->GetName() << G4endl; //GDEB
#endif

	  newChain->AddIon( daughterIon, BR );
	  theChainVector.push_back( newChain );
	  newChain->SetNAtomsInJob(nIonsInJob );
	  if( !daughterIon->GetPDGStable() ) AddDaughters( newChain );
	}
      }

    }
  }
  
#ifndef GAMOS_NO_VERBOSE
    if( SHVerb(testVerb) )
  G4cout << "  SHRadDecayChainSet::AddDaughters firstIon " << firstIon << G4endl;//GDEB
#endif

  if( firstIon ) {
#ifndef GAMOS_NO_VERBOSE
    if( SHVerb(testVerb) )
    G4cout << "  SHRadDecayChainSet::AddDaughters firstIon " << firstIon->GetParticleName()<< G4endl;//GDEB
#endif
    std::vector<SHRadDecayChain*>::const_iterator ite;
    chain->AddIon( firstIon, firstChannelBR );
    if( !firstIon->GetPDGStable() ) AddDaughters( chain );
  }

#ifndef GAMOS_NO_VERBOSE
    if( SHVerb(testVerb) )
  G4cout << "  SHRadDecayChainSet::AddDaughters END " << chain->GetName()<< G4endl;//GDEB
#endif


}

//-----------------------------------------------------------------
G4DecayTable* SHRadDecayChainSet::FindDecayTable( G4Ions* ion )
{
#ifndef GAMOS_NO_VERBOSE
    if( SHVerb(testVerb) )
  G4cout << " SHRadDecayChainSet::FindDecayTable " << ion->GetParticleName() << " " << ion->GetDecayTable() << " " << ion->GetExcitationEnergy() << G4endl;
#endif

  G4DecayTable* decayTable = ion->GetDecayTable();
  if( !decayTable && ion->GetExcitationEnergy() > 0. ) {
    G4ITDecayChannel *anITChannel = new G4ITDecayChannel(0, ion, 1);
    anITChannel->SetICM(true);
    anITChannel->SetARM(true);
    anITChannel->SetHLThreshold( -1.*second );
    decayTable = new G4DecayTable();
    decayTable->Insert(anITChannel);
  }

  return decayTable; 

}

//-----------------------------------------------------------------
void SHRadDecayChainSet::Clean()
{
  theIons.clear();
}


//-----------------------------------------------------------------
void SHRadDecayChainSet::Dump( std::ostream& out ) 
{
  out << this << " ISOTOPESET: " << theName
      << " NEventsInJob " << theNEventsInJob
      << " JobTime " << theJobTime
      << " NEventsPerSecond " << theNEventsPerNanoSecond*second << G4endl
      << " PERIODS:";
  std::map<G4double,G4bool>::const_iterator itep;
  for( itep = thePeriods.begin(); itep != thePeriods.end(); itep++ ){
    out << " " << (*itep).first/second << " " << (*itep).second;
  }
  out << G4endl 
	 << " TIMES:";
  std::set<G4double>::const_iterator itet;
  for( itet = theTimes.begin(); itet != theTimes.end(); itet++ ){
    out << " " << (*itet)/second;
  }
  out << G4endl 
	 << " ISOTOPES:";
  std::map<G4String,SHRadDecayChain*>::const_iterator ites;
  for( ites = theChains.begin(); ites != theChains.end(); ites++ ) {
    out << " " << (*ites).second->GetName();
  }
  out << G4endl;

}


//-----------------------------------------------------------------
void SHRadDecayChainSet::PrintData( std::ostream& out ) 
{
  out << "%%%%%% " << theName << "  NUMBER_OF_EVENTS_PER_SECOND= " << theNEventsPerNanoSecond*second << " = " << theNEventsInJob << " events in " << theJobTime/ns << " nanoseconds " << G4endl;;

  std::map<G4double,G4bool>::const_iterator itep;
  out << "%%%%%% " << theName << "  PERIODS:";
  for( itep = thePeriods.begin(); itep != thePeriods.end(); itep++ ){
    out << " " << (*itep).first/second << " " << (*itep).second;
  }
  out << G4endl;

  std::set<double>::const_iterator itet;
  for( itet = theTimes.begin(); itet != theTimes.end(); itet++ ) {
    double time = *itet;
    G4double totalActivity = 0.;
    std::map<G4String,SHRadDecayChain*>::const_iterator ites;
    for( ites = theChains.begin(); ites != theChains.end(); ites++ ){
      (*ites).second->PrintData(time,out);
      // for total activity count only longer chains, to avoid counting twice
      std::map<G4String,SHRadDecayChain*>::const_iterator ites2;
      G4bool bFound = false;
      for( ites2 = theChains.begin(); ites2 != theChains.end(); ites2++ ){// do not count twice a chain and the final isotope in it
	if( ites != ites2 && (*ites2).second->GetName().find( (*ites).first ) != G4String::npos ) bFound = true;
      }
      if( !bFound ) totalActivity += (*ites).second->GetActivity(time);
    }
    // print total activity recalculated 
    //t     out << "%%%%% " << theName << "  TIME= " << time << " sec  " << " TOTAL ACTIVITY= " << totalActivity << G4endl;

  }  

}

//-----------------------------------------------------------------
void SHRadDecayChainSet::SetTimes( std::vector<G4double> times )
{
  theTimes.clear();
  for( size_t ii = 0; ii < times.size(); ii++ ){
    theTimes.insert( times[ii] );
  }
}

//-----------------------------------------------------------------
void SHRadDecayChainSet::DeleteShortLived()
{
  std::vector<SHRadDecayChain*>::const_iterator ites;
  for( ites = theChainVector.begin(); ites != theChainVector.end(); ites++ ){
    (*ites)->DeleteShortLived(theMinLifeTime);
  }

}


//-----------------------------------------------------------------
void SHRadDecayChainSet::SetNAtomsInJob(  std::map<G4String,G4double> particles )
{ 
 // Find the isotope for each chain name
 std::map<G4String,G4double>::const_iterator itep;
 for( itep = particles.begin(); itep != particles.end(); itep++ ){
   std::map<G4String,SHRadDecayChain*>::const_iterator iter = theChains.find( (*itep).first );
   if( iter == theChains.end() ) {
     G4Exception(" SHRadDecayChain::SetNAtomsInJob",
		 "Error",
		 FatalException,
		 G4String("chain not found " + (*itep).first).c_str());
   }
#ifndef GAMOS_NO_VERBOSE
    if( SHVerb(debugVerb) )  G4cout << " SHRadDecayChainSet::SetNAtomsInJob " << (*itep).first  << " = " << (*itep).second << G4endl;
#endif
   (*iter).second->SetNAtomsInJob( (*itep).second );
   
 }
 
}
 
//--- Used by GmNoUseRadDecayTimeUA
//-----------------------------------------------------------------  
vi SHRadDecayChainSet::BuildChainFromTrack( G4int trkID )
{
  vi chain;
  std::map<G4int,G4int>::const_iterator itepc = theParentChildren.find(trkID);
  std::map<G4int,G4Ions*>::const_iterator itei = theIonsMap.find(trkID);
  if( itei != theIonsMap.end() ) {
    if( (*itei).second->GetParticleType() == "nucleus" ) {
      //    if( !(*itei).second->GetPDGStable() ) {              
      chain.push_back((*itei).second);
#ifndef GAMOS_NO_VERBOSE
      if( SHVerb(debugVerb) ) {
        G4cout << " SHRadDecayChainSet::BuildChainFromTrack  Add first ion " << (*itei).second->GetParticleName() << " to chain " << GetChainName( &chain ) << G4endl;
      }
#endif
    }
  }
 

  /*  for( itei = theIons.begin(); itei != theIons.end();itei++ ) {                                             
      G4cout << "IONS " << (*itei).first << " " << (*itei).second->GetParticleName() << " STABLE " << (*itei).sec\
      ond->GetPDGStable() << G4endl;                                                                                  
      }*/

  //--- add to the chain the list of ion ancestors
  do {
    trkID = (*itepc).second;
    itei = theIonsMap.find(trkID);
    if( itei != theIonsMap.end() ) {
      if( (*itei).second->GetParticleType() == "nucleus" ) {
        //      if( !(*itei).second->GetPDGStable() ) {      
        chain.insert(chain.begin(),(*itei).second);
#ifndef GAMOS_NO_VERBOSE
        if( SHVerb(debugVerb) ) {
          G4cout << " SHRadDecayChainSet::BuildChainFromTrack  Add ion " << (*itei).second->GetParticleName() <<\
	    " to chain " << GetChainName( &chain ) << G4endl;
        }
#endif
      }
    }
    itepc = theParentChildren.find(trkID);
  } while(itepc != theParentChildren.end() );

  return chain;

}
//-----------------------------------------------------------------       
G4String SHRadDecayChainSet::GetChainName( vi* chain )
{
  // build the chain name                   
  vi::const_iterator item;
  G4String chainName = "";
  for( item = chain->begin(); item != chain->end(); item++ ){
    G4Ions* ion = *item;
    if( item != chain->begin() ) chainName += "->";
    chainName += ion->GetParticleName();
  }

  return chainName;
}


//-----------------------------------------------------------------        
G4double SHRadDecayChainSet::GetDecayBR( G4ParticleDefinition* parent, G4ParticleDefinition* daughter, G4bool bMustExist )
{
  mpmpd::const_iterator item = theBranchingRatios.find( parent );
  if( item == theBranchingRatios.end() ) {
    if( bMustExist ) {
      G4Exception("SHRadDecayChainBuilder::GetDecayBR",
                  "Error, contact GAMOS developers",
                  FatalException,
                  G4String("No BranchingRatios stored for particle " + parent->GetParticleName()).c_str());
    } else {
      return 1.;
    }
  }

  mpd* BR = (*item).second;
  mpd::const_iterator itebr = BR->find( daughter );
  if( itebr == BR->end() ){
    if( bMustExist ) {
      G4Exception("SHRadDecayChainBuilder::GetDecayBR",
                  "Error, contact GAMOS developers",
                  FatalException,
                  G4String("No daugther " + daughter->GetParticleName() + " found in BranchingRatios of particle " + parent->GetParticleName())\
		  .c_str());
    } else {
      return 1.;
    }
  }

  return (*itebr).second;

}
