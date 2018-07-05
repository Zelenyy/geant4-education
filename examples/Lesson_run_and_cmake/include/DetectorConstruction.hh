//
// Created by mihail on 24.05.17.
//
#ifndef GEANT4_DETECTORCONSTRUCTION_HH
#define GEANT4_DETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"

class G4VPhysicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction {
public:
    G4VPhysicalVolume *Construct();
};

#endif //GEANT4_DETECTORCONSTRUCTION_HH