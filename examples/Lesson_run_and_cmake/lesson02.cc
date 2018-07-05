//
// Created by zelenyy on 18.03.18.
//


#include <random>
#include <G4UIExecutive.hh>
#include <G4VisExecutive.hh>
#include <G4UImanager.hh>
#include "G4RunManager.hh"
#include "FTFP_BERT.hh"
#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

using namespace std;
using namespace CLHEP;

int main(int argc, char **argv) {

    random_device rd;
    uniform_int_distribution<long> uid(0, LONG_MAX);
    G4long seed = uid(rd);
    G4Random::setTheEngine(new CLHEP::RanecuEngine);
    G4Random::setTheSeed(seed);

    auto runManager = new G4RunManager;

    G4int verbose = 0;
    FTFP_BERT *physlist = new FTFP_BERT(verbose);
    runManager->SetUserInitialization(physlist);

    auto massWorld = new DetectorConstruction;
    runManager->SetUserInitialization(massWorld);

    auto action = new ActionInitialization;
    runManager->SetUserInitialization(action);

    runManager->Initialize();

#ifdef G4UI_USE
    G4UIExecutive *ui = 0;
    G4UImanager *UImanager = G4UImanager::GetUIpointer();
    if (argc == 1) {
        ui = new G4UIExecutive(argc, argv);
    }
    if (!ui) {
        // batch mode
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    }
#ifdef G4VIS_USE
    if (ui) {
        G4VisManager *visManager = new G4VisExecutive;
        visManager->Initialize();
        UImanager->ApplyCommand("/control/execute init_vis.mac");
    }
#endif
    ui->SessionStart();
    delete ui;
#endif
    return 0;
}
