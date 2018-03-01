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
#ifndef SHRadDecayChainSet_hh
#define SHRadDecayChainSet_hh 1
#include "globals.hh"
#include <map>
#include <vector>
#include <set>
#include <iostream>

#include "G4Ions.hh"
class SHRadDecayChain;
class SHRadDecayChainBuilder;
class G4Track;

typedef std::vector<G4Ions*> vi;
typedef std::map<G4ParticleDefinition*, G4double> mpd;
typedef std::map<G4ParticleDefinition*, mpd* > mpmpd;

class SHRadDecayChainSet
{

 public:
  SHRadDecayChainSet();
  SHRadDecayChainSet( G4String name );
  ~SHRadDecayChainSet(){};

public:
  void StoreIon(const G4Track*, G4bool bAlsoLifeTime0 = false);
  void StoreParentChildren(const G4Track* aTrack, G4bool bAlsoLifeTime0 = false);
  void StoreIon( G4Ions* ion, G4double count){
    theIons[ion] = count;
  }
  void Clean();

  void StoreDecayBranchingRatios( const G4Track* aTrack );
  std::map<G4String,SHRadDecayChain*> BuildPossibleDecayChains();

  void DeleteShortLived();
  void AddChain( SHRadDecayChain* chain);
  void AddData( std::vector<G4String>& wl );
  //  void AddIon( vi* chain );

  vi BuildChainFromTrack( G4int trkID );
  static G4String GetChainName( vi* chain );
  G4double GetDecayBR( G4ParticleDefinition* parent, G4ParticleDefinition* daughter, G4bool bMustExist = true );

  
  void Dump( std::ostream& out );
  void PrintData( std::ostream& out = G4cout );

  void SetNAtomsInJob(  std::map<G4String,G4double> particles );
  void SetTimes( std::vector<G4double> times );
  void SetTimes( std::set<G4double> times ){
    theTimes = times; 
  }
  void SetPeriods( std::map<G4double,G4bool>& periods ) {
    thePeriods = periods;
  }
  void SetNEventsInJob( G4double nev ) {
    theNEventsInJob = nev;
  }
  void SetJobTime( G4double jt ) {
    theJobTime = jt;
  }
  void SetNEventsPerNanoSecond( G4double nev ){
    theNEventsPerNanoSecond = nev;
  }
  void SetMinLifeTime( G4double minTime ) {
    theMinLifeTime = minTime;
  }

  std::map<G4double,G4bool> GetPeriods() const {
    return thePeriods;
  }
  G4String GetName() const{
    return theName;
  }
  G4double GetNEventsPerNanoSecond() const {
    return theNEventsPerNanoSecond; 
  }
  G4double GetNEventsInJob() const {
    return theNEventsInJob;
  }
  std::map<G4String,SHRadDecayChain*> GetChains() const {
   return theChains;
  }
  std::set<double> GetTimes() const {
    return theTimes; 
  }
  void AddTime( G4double time ) { // used only by ACAB
    theTimes.insert(time);
  }

private:
  void AddDaughters( SHRadDecayChain* chain );
  G4DecayTable* FindDecayTable( G4Ions* ion );
  void AddNEvents( std::vector<G4String>& wl );
  void AddPeriods( std::vector<G4String>& wl );
  void AddTime( std::vector<G4String>& wl );

public:
  G4String theName;
  G4double theNEventsInJob;
  G4double theJobTime;
  G4double theNEventsPerNanoSecond;
  std::map<G4double,G4bool> thePeriods; // periods where production is on or off (= only decay)
  std::vector<SHRadDecayChain*> theChainVector;
  std::map<G4String,SHRadDecayChain*> theChains;
  std::set<double> theTimes; // these are stored in the SHRadioactiveDecay also
  SHRadDecayChainBuilder* theDecayChainBuilder;
  G4double theMinLifeTime;
  
private:
  std::map<G4Ions*,G4double> theIons; // start of decay chain

  //DEPRECTAED
  mpmpd theBranchingRatios;
  std::map<G4int, G4int> theParentChildren;
  std::map<G4int,G4Ions*> theIonsMap; 

};

#endif
	  
