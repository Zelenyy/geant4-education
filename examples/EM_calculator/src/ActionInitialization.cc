#include "ActionInitialization.hh"
#include "RunAction.hh"

void ActionInitialization::Build() const {
    RunAction* runAction = new RunAction();
    SetUserAction(runAction);
}
