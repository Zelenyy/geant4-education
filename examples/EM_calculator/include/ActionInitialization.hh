//
// Created by mihail on 24.05.17.
//

#ifndef GEANT4_ACTIONINITIALIZATION_HH
#define GEANT4_ACTIONINITIALIZATION_HH

#include "G4VUserActionInitialization.hh"

class ActionInitialization : public G4VUserActionInitialization {
public:
    ActionInitialization() : G4VUserActionInitialization() {};

    ~ActionInitialization() {};

    void Build() const override;

};


#endif //GEANT4_ACTIONINITIALIZATION_HH
