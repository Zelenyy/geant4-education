//
// Created by mihail on 24.05.17.
//
#include "DetectorConstruction.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"

using namespace CLHEP;

G4VPhysicalVolume *DetectorConstruction::Construct() {
    auto nistMngr = G4NistManager::Instance();
    auto vacuum = nistMngr->FindOrBuildMaterial("G4_Galactic");
    G4Box *worldSolid = new G4Box("world",1000*meter, 1000*meter, 1000*meter);
    G4LogicalVolume *worldLogic = new G4LogicalVolume(worldSolid, vacuum, "worldLogic");
    G4VPhysicalVolume *world = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), worldLogic, "WorldPhys", 0, false, 0);
    return world;
}
