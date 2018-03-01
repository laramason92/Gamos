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
// 080319 Compilation warnings - gcc-4.3.0 fix by T. Koi
//

// neutron_hp -- source file
// J.P. Wellisch, Nov-1996
// A prototype of the low energy neutron transport model.
//
#include "G4ParticleHPorLElastic.hh"
#include "G4ParticleHPElasticFS.hh"

G4ParticleHPorLElastic::G4ParticleHPorLElastic()
  :G4HadronicInteraction("NeutronHPorLElastic")
{
   overrideSuspension = false;
   G4ParticleHPElasticFS * theFS = new G4ParticleHPElasticFS;
   if(!getenv("G4NEUTRONHPDATA")) 
       throw G4HadronicException(__FILE__, __LINE__, "Please setenv G4NEUTRONHPDATA to point to the neutron cross-section files.");
   dirName = getenv("G4NEUTRONHPDATA");
   G4String tString = "/Elastic/";
   dirName = dirName + tString;
//    G4cout <<"G4ParticleHPorLElastic::G4ParticleHPorLElastic testit "<<dirName<<G4endl;
   numEle = G4Element::GetNumberOfElements();
   theElastic = new G4ParticleHPChannel[numEle];
   unavailable_elements.clear();
   for (G4int i=0; i<numEle; i++)
   {
      theElastic[i].Init((*(G4Element::GetElementTable()))[i], dirName);
      //while(!theElastic[i].Register(theFS));
      try { while(!theElastic[i].Register(theFS)) ; }
      catch ( G4HadronicException )
      {
          unavailable_elements.insert ( (*(G4Element::GetElementTable()))[i]->GetName() ); 
      }
   }
   delete theFS;
   SetMinEnergy(0.*eV);
   SetMaxEnergy(20.*MeV);
   if ( unavailable_elements.size() > 0 ) 
   {
      std::set< G4String>::iterator it;
      G4cout << "HP Elastic data are not available for thess  elements "<< G4endl;
      for ( it = unavailable_elements.begin() ; it != unavailable_elements.end() ; it++ )
         G4cout << *it << G4endl;
      G4cout << "Low Energy Parameterization Models will be used."<< G4endl;
   }

   createXSectionDataSet();
}
  
G4ParticleHPorLElastic::~G4ParticleHPorLElastic()
{
   delete [] theElastic;
   delete theDataSet; 
}
  
#include "G4ParticleHPThermalBoost.hh"
  
G4HadFinalState * G4ParticleHPorLElastic::ApplyYourself(const G4HadProjectile& aTrack, G4Nucleus& )
{
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

        //xSec[i] = theElastic[index].GetXsec(aThermalE.GetThermalEnergy(aTrack,
        //		                                                     theMaterial->GetElement(i),
        //								     theMaterial->GetTemperature()));
        xSec[i] = theElastic[index].GetXsec(x);

        xSec[i] *= rWeight;
        sum+=xSec[i];
      }
      G4double random = G4UniformRand();
      G4double running = 0;
      for (i=0; i<n; i++)
      {
        running += xSec[i];
        index = theMaterial->GetElement(i)->GetIndex();
        if(random<=running/sum) break;
      }
      delete [] xSec;
      // it is element-wise initialised.
    }
    G4HadFinalState* finalState = theElastic[index].ApplyYourself(aTrack);
    if (overrideSuspension) finalState->SetStatusChange(isAlive);
    return finalState;
}



G4bool G4ParticleHPorLElastic::IsThisElementOK( G4String name )
{
   if ( unavailable_elements.find( name ) == unavailable_elements.end() ) 
      return TRUE;
   else 
      return FALSE; 
}



void G4ParticleHPorLElastic::createXSectionDataSet()
{
   theDataSet = new G4ParticleHPorLElasticData ( theElastic , &unavailable_elements );
}
