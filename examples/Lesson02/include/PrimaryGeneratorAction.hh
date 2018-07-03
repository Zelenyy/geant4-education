#ifndef PRIMARYGENERATORACTION
#define PRIMARYGENERATORACTION

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"


class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
    PrimaryGeneratorAction();

    G4ParticleGun *fParticleGun;

    void GeneratePrimaries(G4Event *anEvent) override;


};

#endif //PRIMARYGENERATORACTION

