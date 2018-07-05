#ifndef PRIMARYGENERATORACTION
#define PRIMARYGENERATORACTION

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"


class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
    PrimaryGeneratorAction();

    G4GeneralParticleSource *generalParticleSource;

    void GeneratePrimaries(G4Event *anEvent) override;


};

#endif //PRIMARYGENERATORACTION

