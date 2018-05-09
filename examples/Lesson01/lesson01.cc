//
// Created by zelenyy on 18.03.18.
//


#include <random>


#include "G4RunManager.hh"

#include "FTFP_BERT.hh"

#include "G4SystemOfUnits.hh"

#include "G4VUserDetectorConstruction.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"

using namespace std;
using namespace CLHEP;

class DetectorConstruction : public G4VUserDetectorConstruction {
public:
    G4VPhysicalVolume *Construct() {
        auto nistMngr = G4NistManager::Instance();
        auto vacuum = nistMngr->FindOrBuildMaterial("G4_Galactic");
        G4Box *worldSolid = new G4Box("world",1000*meter, 1000*meter, 1000*meter);
        G4LogicalVolume *worldLogic = new G4LogicalVolume(worldSolid, vacuum, "worldLogic");
        G4VPhysicalVolume *world = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), worldLogic, "WorldPhys", 0, false, 0);
        return world;
    }
};

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
    G4ParticleGun* fParticleGun;
    PrimaryGeneratorAction() : G4VUserPrimaryGeneratorAction() {
        G4int nofParticles = 1;
        fParticleGun = new G4ParticleGun(nofParticles);
        // default particle kinematic

        G4ParticleDefinition* particleDefinition
                = G4ParticleTable::GetParticleTable()->FindParticle("proton");

        fParticleGun->SetParticleDefinition(particleDefinition);
        fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
        fParticleGun->SetParticleEnergy(3.0*GeV);
        fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., 0.));
    }

    void GeneratePrimaries(G4Event *anEvent) override {
        fParticleGun->GeneratePrimaryVertex(anEvent);
    }

};


int main() {

    double kineticEnergy = 10 * MeV;
    random_device rd;
    uniform_int_distribution<long> uid(0, LONG_MAX);
    G4long seed = uid(rd);
    G4Random::setTheEngine(new CLHEP::RanecuEngine);
    G4Random::setTheSeed(seed);

    auto runManager = new G4RunManager;

    G4int verbose = 1;
    FTFP_BERT *physlist = new FTFP_BERT(verbose);
    runManager->SetUserInitialization(physlist);

    auto massWorld = new DetectorConstruction();
    runManager->SetUserInitialization(massWorld);

    auto gun = new PrimaryGeneratorAction();
    runManager->SetUserAction(gun);


    runManager->Initialize();



    return 0;
}