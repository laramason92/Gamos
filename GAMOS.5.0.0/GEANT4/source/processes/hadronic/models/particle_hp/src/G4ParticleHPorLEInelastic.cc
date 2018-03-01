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
//
// 05-11-21 NeutronHP or Low Energy Parameterization Models 
//          Implemented by T. Koi (SLAC/SCCS)
//          If NeutronHP data do not available for an element, then Low Energy 
//          Parameterization models handle the interactions of the element.
//

// neutron_hp -- source file
// J.P. Wellisch, Nov-1996
// A prototype of the low energy neutron transport model.
//
#include "G4ParticleHPorLEInelastic.hh"
//#include "G4ParticleHPInelasticFS.hh"

G4ParticleHPorLEInelastic::G4ParticleHPorLEInelastic(G4ParticleDefinition* projectile, const G4String& dataDir )
  :G4HadronicInteraction("NeutronHPorLEInelastic")
{
  SetDataDirVariable( dataDir);
  SetProjectile( projectile );

   SetMinEnergy(0.*eV);
   SetMaxEnergy(20.*MeV);

//   G4ParticleHPInelasticFS * theFS = new G4ParticleHPInelasticFS;
   if(!getenv(theDataDirVariable)) 
       throw G4HadronicException(__FILE__, __LINE__, "Please setenv G4NEUTRONHPDATA to point to the neutron cross-section files.");
   dirName = getenv(theDataDirVariable);
   G4String tString = "/Inelastic/";
   dirName = dirName + tString;
//    G4cout <<"G4ParticleHPorLEInelastic::G4ParticleHPorLEInelastic testit "<<dirName<<G4endl;
   numEle = G4Element::GetNumberOfElements();
   theInelastic = new G4ParticleHPChannelList[numEle];
   for( G4int ii = 0; ii < numEle; ii++ ) {
     theInelastic[ii].SetProjectile( theProjectile );
   }
   unavailable_elements.clear();
   for (G4int i=0; i<numEle; i++)
   {
      theInelastic[i].Init( (*(G4Element::GetElementTable()))[i] , dirName );
      do
      {
       
         try 
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
         } 

         catch ( G4HadronicException )
         {
            unavailable_elements.insert ( (*(G4Element::GetElementTable()))[i]->GetName() ); 
         }
         theInelastic[i].RestartRegistration();
      } 
      while( !theInelastic[i].HasDataInAnyFinalState()); 

   }

//   delete theFS;
   if ( unavailable_elements.size() > 0 ) 
   {
      std::set< G4String>::iterator it;
      G4cout << "HP Inelastic data are not available for thess  elements "<< G4endl;
      for ( it = unavailable_elements.begin() ; it != unavailable_elements.end() ; it++ )
         G4cout << *it << G4endl;
      G4cout << "Low Energy Parameterization Models will be used."<< G4endl;
   }

   createXSectionDataSet();
}
  
G4ParticleHPorLEInelastic::~G4ParticleHPorLEInelastic()
{
   delete [] theInelastic;
   delete theDataSet; 
}
  
#include "G4ParticleHPThermalBoost.hh"
  
G4HadFinalState * G4ParticleHPorLEInelastic::ApplyYourself(const G4HadProjectile& aTrack, G4Nucleus& )
{
   G4int it=0;
   const G4Material * theMaterial = aTrack.GetMaterial();
   G4int n = theMaterial->GetNumberOfElements();
   G4int index = theMaterial->GetElement(0)->GetIndex();
   if(n!=1)
   {
      G4int i;
      xSec = new G4double[n];
      G4double sum=0;
      const G4double * NumAtomsPerVolume = theMaterial->GetVecNbOfAtomsPerVolume();
      G4double rWeight;    
      G4ParticleHPThermalBoost aThermalE;
      for (i=0; i<n; i++)
      {
        index = theMaterial->GetElement(i)->GetIndex();
        rWeight = NumAtomsPerVolume[i];
        G4double x = aThermalE.GetThermalEnergy(aTrack, theMaterial->GetElement(i), theMaterial->GetTemperature());

        //xSec[i] = theInelastic[index].GetXsec(aThermalE.GetThermalEnergy(aTrack,
        //		                                                     theMaterial->GetElement(i),
        //								     theMaterial->GetTemperature()));
        xSec[i] = theInelastic[index].GetXsec(x);

        xSec[i] *= rWeight;
        sum+=xSec[i];
      }
      G4double random = G4UniformRand();
      G4double running = 0;
      for (i=0; i<n; i++)
      {
        running += xSec[i];
        index = theMaterial->GetElement(i)->GetIndex();
        it = i;
        if(random<=running/sum) break;
      }
      delete [] xSec;
      // it is element-wise initialised.
    }
    //return theInelastic[index].ApplyYourself(aTrack); 
   return theInelastic[index].ApplyYourself( theMaterial->GetElement(it) , aTrack ); 
}



G4bool G4ParticleHPorLEInelastic::IsThisElementOK( G4String name )
{
   if ( unavailable_elements.find( name ) == unavailable_elements.end() ) 
      return TRUE;
   else 
      return FALSE; 
}



void G4ParticleHPorLEInelastic::createXSectionDataSet()
{
    theDataSet = new G4ParticleHPorLEInelasticData ( theInelastic , &unavailable_elements );
}

void G4ParticleHPorLEInelastic::SetProjectile( G4ParticleDefinition* part )
{
  
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

  //g  G4cout << "   G4ParticleHPInelastic::SetProjectile( " << part->GetParticleName() << " AFS " << &theAFS << G4endl;
}
