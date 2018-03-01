//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// neutron_hp -- source file
// J.P. Wellisch, Nov-1996
// A prototype of the low energy neutron transport model.
//
// 070523 bug fix for G4FPE_DEBUG on by A. Howard ( and T. Koi)
//
#define private public 
#include "G4ParticleHPChannel.hh"

#include "G4ParticleHPChannelList.hh"
#include "G4Element.hh"
#include "G4HadFinalState.hh"
#include "G4HadProjectile.hh"
#include "G4ParticleHPFinalState.hh"

  G4int G4ParticleHPChannelList::trycounter = 0;

G4ParticleHPChannelList::G4ParticleHPChannelList(G4int n, G4ParticleDefinition* projectile)
  :theProjectile(projectile)
  {
    nChannels = n;
    theChannels  = new G4ParticleHPChannel * [n];
    allChannelsCreated = false;
    theInitCount = 0;
  }
  
G4ParticleHPChannelList::G4ParticleHPChannelList()
  {
    nChannels = 0;
    theChannels = 0;
    allChannelsCreated = false;
    theInitCount = 0;
    theProjectile = 0;
  }
  
  G4ParticleHPChannelList::~G4ParticleHPChannelList()
  {
    if(theChannels!=0)
    {
      for(G4int i=0;i<nChannels; i++)
      {
        delete theChannels[i];
      }
      delete [] theChannels;
    }
  }
    
  #include "G4ParticleHPThermalBoost.hh"
  G4HadFinalState * G4ParticleHPChannelList::ApplyYourself(const G4Element * , const G4HadProjectile & aTrack)
  {
    G4ParticleHPThermalBoost aThermalE;
    G4int i, ii;
    // decide on the isotope
    G4int numberOfIsos(0);
    for(ii=0; ii<nChannels; ii++)
    {
      numberOfIsos = theChannels[ii]->GetNiso();
      if(numberOfIsos!=0) break;
    }
    G4double * running= new G4double [numberOfIsos];
    running[0] = 0;
    for(i=0;i<numberOfIsos; i++)
    {
      if(i!=0) running[i] = running[i-1];
      for(ii=0; ii<nChannels; ii++)
      {
	if(theChannels[ii]->HasAnyData(i))
	{
	  G4double XSEC = theChannels[ii]->GetWeightedXsec(aThermalE.GetThermalEnergy(aTrack,
		                                                  theChannels[ii]->GetN(i),
								  theChannels[ii]->GetZ(i),
						  		  aTrack.GetMaterial()->GetTemperature()),
					                i);
          running[i] += XSEC;
#ifdef G4PARTICLEHPDEBUG
	  if( getenv("G4ParticleHPDebug") ) G4cout << " G4ParticleHPChannelList::ApplyYourself channel " << theChannels[ii]->GetFSType() << " E " << aTrack.GetKineticEnergy() << " XSEC " << XSEC << G4endl; 
#endif
	}
      }
    }
    G4int isotope=nChannels-1;
    G4double random=G4UniformRand();
    for(i=0;i<numberOfIsos; i++)
    {
      isotope = i;
      //if(random<running[i]/running[numberOfIsos-1]) break;
      if(running[numberOfIsos-1] != 0) if(random<running[i]/running[numberOfIsos-1]) break;
    }
    delete [] running;
    
    // decide on the channel
    running = new G4double[nChannels];
    running[0]=0;
    for(i=0; i<nChannels; i++)
    {
      if(i!=0) running[i] = running[i-1];
      if(theChannels[i]->HasAnyData(isotope))
      {
        running[i] += theChannels[i]->GetFSCrossSection(aThermalE.GetThermalEnergy(aTrack,
		                                                  theChannels[i]->GetN(isotope),
								  theChannels[i]->GetZ(isotope),
						  		  aTrack.GetMaterial()->GetTemperature()),
					                isotope);
      }
    }
    G4int lChan=0;
    random=G4UniformRand();
    for(i=0; i<nChannels; i++)
    {
      lChan = i;
      if(running[nChannels-1] != 0) if(random<running[i]/running[nChannels-1]) break;
    }
    delete [] running;
#ifdef G4PARTICLEHPDEBUG
    if( getenv("G4ParticleHPDebug"))    G4cout << " G4ParticleHPChannelList APPLY CHANNEL " << lChan << " " << theChannels[lChan]->theFSType << G4endl;
#endif
    return theChannels[lChan]->ApplyYourself(aTrack, isotope);
  }
      
  void G4ParticleHPChannelList::Init(G4Element * anElement, const G4String & dirName)
  {
    theDir = dirName;
    theElement = anElement;
  }
  
  void G4ParticleHPChannelList::Register(G4ParticleHPFinalState * theFS, 
                                        const G4String & aName)
  {
    if(!allChannelsCreated)
    {
      if(nChannels!=0)
      {
	G4ParticleHPChannel ** theBuffer = new G4ParticleHPChannel * [nChannels+1];
	G4int i;
	for(i=0; i<nChannels; i++)
	{
	  theBuffer[i] = theChannels[i];
	}
	delete [] theChannels;
	theChannels = theBuffer;
      }
      else
      {
	theChannels = new G4ParticleHPChannel * [nChannels+1];
      }
      G4String name;
      name = aName+"/";
      theChannels[nChannels] = new G4ParticleHPChannel(theProjectile);
      theChannels[nChannels]->Init(theElement, theDir, name);
      //	  (*theChannels)[ii].SetProjectile( theProjectile );
      theChannels[nChannels]->SetProjectile( theProjectile );

      nChannels++;
    }
    
    //110527TKDB  Unnessary codes, Detected by gcc4.6 compiler 
    //G4bool result;
    //result = theChannels[theInitCount]->Register(theFS);
    theChannels[theInitCount]->Register(theFS);
    theInitCount++; 
  } 


void G4ParticleHPChannelList::DumpInfo(){

  G4cout<<"================================================================"<<G4endl;
  G4cout<<" Element: "<<theElement->GetName()<<G4endl;
  G4cout<<" Number of channels: "<<nChannels<<G4endl;
  G4cout<<" Directory name: "<<theDir<<G4endl;
  for(int i=0;i<nChannels;i++){
    if(theChannels[i]->HasDataInAnyFinalState()){
      G4cout<<"----------------------------------------------------------------"<<G4endl;
      theChannels[i]->DumpInfo();
      G4cout<<"----------------------------------------------------------------"<<G4endl;
    }
  }
  G4cout<<"================================================================"<<G4endl;

}
