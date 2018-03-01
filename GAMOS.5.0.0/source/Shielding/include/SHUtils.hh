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
#ifndef SHUtils_hh
#define SHUtils_hh

#include <map>
#include <vector>
#include "globals.hh"

class G4PVPlacement;
class G4HadronicProcess;
class G4Material;
class GmAnalysisMgr;
class G4ParticleDefinition;

struct ChannelData {
  G4String name;
  G4int nProtons;
  G4int nNeutrons;
};

class SHUtils 
{
  public:
  SHUtils();
  ~SHUtils(){};

public:
  static void StoreHadronicProcesses(G4String primaryParticleName);
  static void BuildChannelList(const G4ParticleDefinition* part, G4String UAName);
  static void BuildElementNames();
  static G4String GetChannelName(G4int Z, G4int A, G4String channelName, std::string projectile);
  static G4String GetElementName( G4int Z );
  static G4String GetProductName( G4int massCode );
  static G4int GetProductID( G4double massCode );
  static G4int GetProcessID( const G4HadronicProcess* );
  static G4int GetNHadronicProcesses()  
  {
    return theHadronicProcesses.size();
  }
  static G4HadronicProcess* GetHadronicProcess( G4int iproc )
  {
    return theHadronicProcesses[iproc];
  }

  static G4int GetNChannelParticles()  
  { 
    return theChannelParticles.size();
  }
  static std::map<G4String, ChannelData >::const_iterator GetChannelParticle(G4String chName )
  {
    return theChannelParticles.find(chName);
  }

private:
  static std::vector<G4HadronicProcess*> theHadronicProcesses;
  
  static std::map<G4String, ChannelData > theChannelParticles;
  static std::map<G4int,G4String> theElements;
  static std::map<G4double,G4int> theProductIDs;
};
#endif
