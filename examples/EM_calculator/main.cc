//
// Created by zelenyy on 18.03.18.
//


#include <random>
#include <ActionInitialization.hh>
#include <DetectorConstruction.hh>
#include <PrimaryGeneratorAction.hh>
#include "G4RunManager.hh"
#include "FTFP_BERT.hh"

using namespace std;
using namespace CLHEP;

int main() {

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

    auto action = new ActionInitialization();
    runManager->SetUserInitialization(action);
    
    runManager->Initialize();

    runManager->BeamOn(1);

    delete runManager;

    return 0;
}