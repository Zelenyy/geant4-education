//
// Created by mihail on 24.05.17.
//

#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"

void ActionInitialization::Build() const {
    PrimaryGeneratorAction *gun = new PrimaryGeneratorAction();
    SetUserAction(gun);
}
