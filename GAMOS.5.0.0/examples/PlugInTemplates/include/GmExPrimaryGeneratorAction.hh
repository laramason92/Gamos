#ifndef GmExPrimaryGeneratorAction_HH
#define GmExPrimaryGeneratorAction_HH

class G4Event;
class G4ParticleGun;

#include "globals.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

class GmExPrimaryGeneratorAction:public G4VUserPrimaryGeneratorAction
{
public:
  GmExPrimaryGeneratorAction();
  ~GmExPrimaryGeneratorAction();
  void GeneratePrimaries(G4Event* evt);

private:
 G4ParticleGun* particleGun;

};

#endif

