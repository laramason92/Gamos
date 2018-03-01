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
// 08.06.2006 V.Ivanchenko: remove stopping
// 25.04.2007 G.Folger: Add quasielastic as option, use quasielastic by default
//
//----------------------------------------------------------------------------
//
#ifndef GmHadronPhysicsQGSP_BIC_PHP_h
#define GmHadronPhysicsQGSP_BIC_PHP_h 1

#include "globals.hh"
#include "G4ios.hh"

#include "G4VPhysicsConstructor.hh"
#include "G4MiscLHEPBuilder.hh"

#include "G4PiKBuilder.hh"
#include "G4LEPPiKBuilder.hh"
#include "G4QGSPPiKBuilder.hh"

#include "GmProtonHPBaseBuilder.hh"
#include "GmProtonHPBuilder.hh"
#include "G4QGSPProtonBuilder.hh"
#include "G4LEPProtonBuilder.hh"
#include "G4BinaryProtonBuilder.hh"

#include "G4NeutronBuilder.hh"
#include "G4LEPNeutronBuilder.hh"
#include "G4QGSPNeutronBuilder.hh"
#include "G4BinaryNeutronBuilder.hh"
#include "G4NeutronHPBuilder.hh"

class GmHadronPhysicsQGSP_BIC_PHP : public G4VPhysicsConstructor
{
  public: 
    GmHadronPhysicsQGSP_BIC_PHP(G4int verbose =1);
    GmHadronPhysicsQGSP_BIC_PHP(const G4String& name, G4bool quasiElastic=true);
    virtual ~GmHadronPhysicsQGSP_BIC_PHP();

  public: 
    virtual void ConstructParticle();
    virtual void ConstructProcess();

  private:
    void CreateModels();
    G4NeutronBuilder * theNeutrons;
    G4LEPNeutronBuilder * theLEPNeutron;
    G4QGSPNeutronBuilder * theQGSPNeutron;
    G4BinaryNeutronBuilder * theBinaryNeutron;
    G4NeutronHPBuilder * theHPNeutron;
    
    G4PiKBuilder * thePiK;
    G4LEPPiKBuilder * theLEPPiK;
    G4QGSPPiKBuilder * theQGSPPiK;
    
    GmProtonHPBaseBuilder * thePro;
    G4QGSPProtonBuilder * theQGSPPro; 
    G4LEPProtonBuilder * theLEPPro;
    G4BinaryProtonBuilder * theBinaryPro;
    GmProtonHPBuilder * thePHPPro;
    
    G4MiscLHEPBuilder * theMiscLHEP;
    
    G4bool QuasiElastic;
};

#endif

