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
// this code implementation is the intellectual property of
// neutron_hp -- source file
// J.P. Wellisch, Nov-1996
// A prototype of the low energy neutron transport model.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4ParticleHPInelastic.cc,v 1.1 2012/10/02 17:21:10 arce Exp $
// GEANT4 tag $Name:  $
//
// 070523 bug fix for G4FPE_DEBUG on by A. Howard (and T. Koi)
// 081203 limit maximum trial for creating final states add protection for 1H isotope case by T. Koi
//

#include "G4ParticleHPInelastic.hh"

G4ParticleHPInelastic::G4ParticleHPInelastic(G4ParticleDefinition* projectile, const G4String& dataDirVariable, const G4String& name )
    :G4HadronicInteraction(name)
{

  if(!getenv(dataDirVariable)){
    G4String message("Please setenv " + dataDirVariable + " to point to the " + projectile->GetParticleName() + " cross-section files.");
    throw G4HadronicException(__FILE__, __LINE__,message.c_str());
  }

  dirName = getenv(dataDirVariable);
  G4cout << "@@@ G4ParticleHPInelastic instantiated for particle " << projectile->GetParticleName() << " data directory variable is " << dataDirVariable << " pointing to " << dirName << G4endl;

    SetDataDirVariable( dataDirVariable);
    SetProjectile( projectile );

    SetMinEnergy( 0.0 );
    SetMaxEnergy( 20.*MeV );
//    G4cout << " entering G4ParticleHPInelastic constructor"<<G4endl;
    G4String tString = "/Inelastic";
    dirName = dirName + tString;
    numEle = G4Element::GetNumberOfElements();
    theInelastic = new G4ParticleHPChannelList[numEle];
    for( G4int ii = 0; ii < numEle; ii++ ) {
      theInelastic[ii].SetProjectile( theProjectile );
    }

    for (G4int i=0; i<numEle; i++)
    { 
      theInelastic[i].Init((*(G4Element::GetElementTable()))[i], dirName);
      G4int itry = 0;
      do
      {
	theInelastic[i].Register(&theNFS, "F01"); // has
	theInelastic[i].Register(&theNXFS, "F02");
	theInelastic[i].Register(&the2NDFS, "F03");
 	theInelastic[i].Register(&the2NFS, "F04"); // has, E Done
 	theInelastic[i].Register(&the3NFS, "F05"); // has, E Done
  	theInelastic[i].Register(&theNAFS, "F06");
	theInelastic[i].Register(&theN3AFS, "F07");
	theInelastic[i].Register(&the2NAFS, "F08");
	theInelastic[i].Register(&the3NAFS, "F09");
	theInelastic[i].Register(&theNPFS, "F10");
	theInelastic[i].Register(&theN2AFS, "F11");
	theInelastic[i].Register(&the2N2AFS, "F12");
	theInelastic[i].Register(&theNDFS, "F13");
	theInelastic[i].Register(&theNTFS, "F14");
	theInelastic[i].Register(&theNHe3FS, "F15");
	theInelastic[i].Register(&theND2AFS, "F16");
	theInelastic[i].Register(&theNT2AFS, "F17");
	theInelastic[i].Register(&the4NFS, "F18"); // has, E Done
	theInelastic[i].Register(&the2NPFS, "F19");
	theInelastic[i].Register(&the3NPFS, "F20");
	theInelastic[i].Register(&theN2PFS, "F21");
	theInelastic[i].Register(&theNPAFS, "F22");
  	theInelastic[i].Register(&thePFS, "F23");
	theInelastic[i].Register(&theDFS, "F24");
	theInelastic[i].Register(&theTFS, "F25");
	theInelastic[i].Register(&theHe3FS, "F26");
	theInelastic[i].Register(&theAFS, "F27");
	theInelastic[i].Register(&the2AFS, "F28");
	theInelastic[i].Register(&the3AFS, "F29");
	theInelastic[i].Register(&the2PFS, "F30");
	theInelastic[i].Register(&thePAFS, "F31");
	theInelastic[i].Register(&theD2AFS, "F32");
	theInelastic[i].Register(&theT2AFS, "F33");
	theInelastic[i].Register(&thePDFS, "F34");
	theInelastic[i].Register(&thePTFS, "F35");
	theInelastic[i].Register(&theDAFS, "F36");
	theInelastic[i].RestartRegistration();
        itry++;
      }
      //while(!theInelastic[i].HasDataInAnyFinalState());
      while( !theInelastic[i].HasDataInAnyFinalState() && itry < 6 );
                                                              // 6 is corresponding to the value(5) of G4ParticleHPChannel. TK  
      if ( itry == 6 ) 
      {
         // No Final State at all.
         G4bool exceptional = false;
         if ( (*(G4Element::GetElementTable()))[i]->GetNumberOfIsotopes() == 1 )
         {
            if ( (*(G4Element::GetElementTable()))[i]->GetIsotope( 0 )->GetZ() == 1 && (*(G4Element::GetElementTable()))[i]->GetIsotope( 0 )->GetN() == 1 ) exceptional = true;  //1H
         } 
         if ( !exceptional ) throw G4HadronicException(__FILE__, __LINE__, "Channel: Do not know what to do with this element");
      }
    }
  }

  G4ParticleHPInelastic::~G4ParticleHPInelastic()
  {
    delete [] theInelastic;
  }
  
  #include "G4ParticleHPThermalBoost.hh"
  
  G4HadFinalState * G4ParticleHPInelastic::ApplyYourself(const G4HadProjectile& aTrack, G4Nucleus& )
  {
    const G4Material * theMaterial = aTrack.GetMaterial();
    G4int n = theMaterial->GetNumberOfElements();
    G4int index = theMaterial->GetElement(0)->GetIndex();
    G4int it=0;
    if(n!=1)
    {
      xSec = new G4double[n];
      G4double sum=0;
      G4int i;
      const G4double * NumAtomsPerVolume = theMaterial->GetVecNbOfAtomsPerVolume();
      G4double rWeight;    
      G4ParticleHPThermalBoost aThermalE;
      for (i=0; i<n; i++)
      {
        index = theMaterial->GetElement(i)->GetIndex();
        rWeight = NumAtomsPerVolume[i];
	if(aTrack.GetDefinition() == G4Neutron::Neutron() ) {
	  xSec[i] = theInelastic[index].GetXsec(aThermalE.GetThermalEnergy(aTrack,
									   theMaterial->GetElement(i),
									   theMaterial->GetTemperature()));
	} else {
	  xSec[i] = theInelastic[index].GetXsec(aTrack.GetKineticEnergy());
	}
        xSec[i] *= rWeight;
        sum+=xSec[i];
#ifdef G4PARTICLEHPDEBUG
	if( getenv("G4ParticleHPDebug") ) G4cout << " G4ParticleHPInelastic XSEC ELEM " << i << " = " << xSec[i] << G4endl;
#endif
      }
      G4double random = G4UniformRand();
      G4double running = 0;
      for (i=0; i<n; i++)
      {
        running += xSec[i];
        index = theMaterial->GetElement(i)->GetIndex();
        it = i;
        //if(random<=running/sum) break;
        if( sum == 0 || random<=running/sum) break;
      }
      delete [] xSec;
    }
    return theInelastic[index].ApplyYourself(theMaterial->GetElement(it), aTrack);
  }

void G4ParticleHPInelastic::SetProjectile( G4ParticleDefinition* part )
{
  theProjectile = part;  
  the2AFS.SetProjectile( part );
  the2N2AFS.SetProjectile( part );
  the2NAFS.SetProjectile( part );
  the2NDFS.SetProjectile( part );
  the2NFS.SetProjectile( part );
  the2NPFS.SetProjectile( part );
  the2PFS.SetProjectile( part );
  the3AFS.SetProjectile( part );
  the3NAFS.SetProjectile( part );
  the3NFS.SetProjectile( part );
  the3NPFS.SetProjectile( part );
  the4NFS.SetProjectile( part );
  theAFS.SetProjectile( part );
  theD2AFS.SetProjectile( part );
  theDAFS.SetProjectile( part );
  theDFS.SetProjectile( part );
  theHe3FS.SetProjectile( part );
  theN2AFS.SetProjectile( part );
  theN2PFS.SetProjectile( part );
  theN3AFS.SetProjectile( part );
  theNAFS.SetProjectile( part );
  theND2AFS.SetProjectile( part );
  theNDFS.SetProjectile( part );
  theNHe3FS.SetProjectile( part );
  theNFS.SetProjectile( part );
  theNPAFS.SetProjectile( part );
  theNPFS.SetProjectile( part );
  theNT2AFS.SetProjectile( part );
  theNTFS.SetProjectile( part );
  theNXFS.SetProjectile( part );
  thePAFS.SetProjectile( part );
  thePDFS.SetProjectile( part );
  thePFS.SetProjectile( part );
  thePTFS.SetProjectile( part );
  theT2AFS.SetProjectile( part );
  theTFS.SetProjectile( part );

}

void G4ParticleHPInelastic::SetDataDirVariable( const G4String& ddir )
{
  theDataDirVariable = ddir;  
  the2AFS.SetDataDirVariable( ddir );
  the2N2AFS.SetDataDirVariable( ddir );
  the2NAFS.SetDataDirVariable( ddir );
  the2NDFS.SetDataDirVariable( ddir );
  the2NFS.SetDataDirVariable( ddir );
  the2NPFS.SetDataDirVariable( ddir );
  the2PFS.SetDataDirVariable( ddir );
  the3AFS.SetDataDirVariable( ddir );
  the3NAFS.SetDataDirVariable( ddir );
  the3NFS.SetDataDirVariable( ddir );
  the3NPFS.SetDataDirVariable( ddir );
  the4NFS.SetDataDirVariable( ddir );
  theAFS.SetDataDirVariable( ddir );
  theD2AFS.SetDataDirVariable( ddir );
  theDAFS.SetDataDirVariable( ddir );
  theDFS.SetDataDirVariable( ddir );
  theHe3FS.SetDataDirVariable( ddir );
  theN2AFS.SetDataDirVariable( ddir );
  theN2PFS.SetDataDirVariable( ddir );
  theN3AFS.SetDataDirVariable( ddir );
  theNAFS.SetDataDirVariable( ddir );
  theND2AFS.SetDataDirVariable( ddir );
  theNDFS.SetDataDirVariable( ddir );
  theNHe3FS.SetDataDirVariable( ddir );
  theNFS.SetDataDirVariable( ddir );
  theNPAFS.SetDataDirVariable( ddir );
  theNPFS.SetDataDirVariable( ddir );
  theNT2AFS.SetDataDirVariable( ddir );
  theNTFS.SetDataDirVariable( ddir );
  theNXFS.SetDataDirVariable( ddir );
  thePAFS.SetDataDirVariable( ddir );
  thePDFS.SetDataDirVariable( ddir );
  thePFS.SetDataDirVariable( ddir );
  thePTFS.SetDataDirVariable( ddir );
  theT2AFS.SetDataDirVariable( ddir );
  theTFS.SetDataDirVariable( ddir );

}
