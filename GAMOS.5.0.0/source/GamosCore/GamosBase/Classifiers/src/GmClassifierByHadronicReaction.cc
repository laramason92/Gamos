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
#include "GmClassifierByHadronicReaction.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Track.hh"
#include "G4ProcessType.hh"
#include "G4VProcess.hh"
#include "G4Neutron.hh"
#include "G4Proton.hh"
#include "G4Deuteron.hh"
#include "G4Triton.hh"
#include "G4He3.hh"
#include "G4Alpha.hh"
#include "G4Gamma.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4tgrFileIn.hh"
#include "G4HadronicProcess.hh"
#include "G4Nucleus.hh"

//------------------------------------------------------------------
GmClassifierByHadronicReaction::GmClassifierByHadronicReaction(G4String name) : GmVClassifier( name )
{
  theIonTable = G4ParticleTable::GetParticleTable()->GetIonTable();
}

//------------------------------------------------------------------
void GmClassifierByHadronicReaction::SetParameters( std::vector<G4String>& params)
{
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmClassifierByHadronicReaction::SetParameters " << params.size() << G4endl;
#endif
  /*
  for( unsigned int ii=0; ii < params.size(); ii++ ){
    theParticlesToExclude.insert(params[ii]);
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmClassifierByHadronicReaction::SetParameters particle to exclude  " << params[ii] << " " << theParticlesToExclude.size() << G4endl;
#endif 
  }
  */

}

//------------------------------------------------------------------
G4int GmClassifierByHadronicReaction::GetIndexFromStep(const G4Step* aStep)
{
  G4int index;
  G4String indexName;

  const G4VProcess* proc = aStep->GetPostStepPoint()->GetProcessDefinedStep();
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) 
    G4cout << " GmClassifierByHadronicReaction::GetIndexFromStep process " << proc << G4endl;
#endif
  if( !proc ) return 0;
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) 
    G4cout << " GmClassifierByHadronicReaction::GetIndexFromStep process type " << proc->GetProcessType() << " fHadronic " << fHadronic << G4endl;
#endif

  if( proc->GetProcessType() != fHadronic ) return 0;

  G4String procName = proc->GetProcessName();
  //other processes than inelastic
  if( procName.find("inelastic") != std::string::npos || procName.find("Inelastic") != std::string::npos 
      || procName.find("nCapture") != std::string::npos ) {
    
    indexName = GmG4Utils::GetInelasticName( aStep );
  }

  std::map<const G4String,G4int>::const_iterator ite = theIndexMap.find(indexName);
  if( ite == theIndexMap.end() ){
    index = theIndexMap.size()+1;
    theIndexMap[indexName] = index;
  } else {
    index = (*ite).second;
  }
  
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmClassifierByHadronicReaction::GetIndexFromStep " << index << " Secondary_names " << indexName << G4endl;
#endif
  
  return index;
}


//------------------------------------------------------------------
G4int GmClassifierByHadronicReaction::GetIndexFromTrack(const G4Track* )
{
  return 0;
  
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmClassifierByHadronicReaction::GetIndex  0 " << G4endl;
#endif

}


//--------------------------------------------------------------
G4String GmClassifierByHadronicReaction::GetIndexName(G4int index)
{
  G4String name = "NOT_FOUND";
  std::map<const G4String,G4int>::const_iterator ite;
  for( ite = theIndexMap.begin(); ite != theIndexMap.end(); ite++ ){
    if((*ite).second == index ){
      return (*ite).first;
      break;
    }
  }
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) 
    G4cout << " GmClassifierByHadronicReaction::GetIndexName " << name << " index " << index << G4endl;
#endif

  return name;
}

//---------------------------------------------------------------
GmClassifierByHadronicReaction::~GmClassifierByHadronicReaction()
{
  //print names of each index 
  G4cout << "%%%%% Table of indices for GmClassifierByHadronicReaction " << theName << G4endl;
  std::map<const G4String,G4int>::const_iterator ite;
  for( ite = theIndexMap.begin(); ite != theIndexMap.end(); ite++ ){
    G4cout << (*ite).first << " = " << (*ite).second << G4endl;
  }
}

