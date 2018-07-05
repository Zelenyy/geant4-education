#include "G4ParticleTable.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"

using namespace CLHEP;

PrimaryGeneratorAction::PrimaryGeneratorAction() : G4VUserPrimaryGeneratorAction() {
    G4int nofParticles = 1;
    fParticleGun = new G4ParticleGun(nofParticles);
    // default particle kinematic
    G4ParticleDefinition *particleDefinition
            = G4ParticleTable::GetParticleTable()->FindParticle("proton");
    fParticleGun->SetParticleDefinition(particleDefinition);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., -1.));
    fParticleGun->SetParticleEnergy(3.0 * GeV);
    fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., 0.));
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent) {
    fParticleGun->GeneratePrimaryVertex(anEvent);
}