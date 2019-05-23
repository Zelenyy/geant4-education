#ifndef GEANT4_THUNDERSTORM_RUNACTIONDWYER2003_HH
#define GEANT4_THUNDERSTORM_RUNACTIONDWYER2003_HH

#include "G4UserRunAction.hh"

class RunAction : public  G4UserRunAction{
public:
    RunAction();
    void BeginOfRunAction(const G4Run* aRun);
};

#endif //GEANT4_THUNDERSTORM_RUNACTIONDWYER2003_HH