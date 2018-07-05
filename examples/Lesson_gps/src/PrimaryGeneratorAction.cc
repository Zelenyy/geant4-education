#include "G4ParticleTable.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"

using namespace CLHEP;

PrimaryGeneratorAction::PrimaryGeneratorAction() :
        G4VUserPrimaryGeneratorAction() ,
        generalParticleSource(new G4GeneralParticleSource){
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent) {
    generalParticleSource->GeneratePrimaryVertex(anEvent);
}