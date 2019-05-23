//
// Created by zelenyy on 26.03.18.
//

#ifndef GEANT4_THUNDERSTORM_DETECTORCONSTRUCTION_HH
#define GEANT4_THUNDERSTORM_DETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

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

#endif //GEANT4_THUNDERSTORM_DETECTORCONSTRUCTION_HH
