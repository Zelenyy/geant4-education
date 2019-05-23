#include "RunAction.hh"
#include "G4EmCalculator.hh"


void RunAction::BeginOfRunAction(const G4Run *aRun) {
    G4EmCalculator *emCalculator = new G4EmCalculator();
}
