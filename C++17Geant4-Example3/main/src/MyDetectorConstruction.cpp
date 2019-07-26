#include "MyDetectorConstruction.hh"

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"

#include "G4NistManager.hh"
#include "G4VisAttributes.hh"

MyDetectorConstruction::MyDetectorConstruction()
:
G4VUserDetectorConstruction(),
halfLabSize(G4ThreeVector(5*cm, 6*cm, 7*cm))
{
	// Be careful of this hard-coded value, position of particle gun in
	// MyPrimaryGeneratorAction.cpp
	// is also hard-coded and must be within lab dimensions.
}

MyDetectorConstruction::~MyDetectorConstruction() {}

G4VPhysicalVolume* MyDetectorConstruction::Construct()
{
	DefineMaterials();
	return ConstructDetector();
}

void MyDetectorConstruction::DefineMaterials()
{
	G4NistManager *nist {G4NistManager::Instance()};
	G4Material *air {nist->FindOrBuildMaterial("G4_AIR")};
	G4Material *Pb {nist->FindOrBuildMaterial("G4_Pb")};
	labMaterial = air;
//	labMaterial = std::unique_ptr<G4Material>(air); // smart pointer not required
}

G4VPhysicalVolume* MyDetectorConstruction::ConstructDetector()
{
	solidLab = std::make_unique<G4Box>("Lab",
					halfLabSize.x(), halfLabSize.y(), halfLabSize.z());

	logicalLab = std::make_unique<G4LogicalVolume>
					(solidLab.get(),   //raw pointer
					labMaterial, 	   //raw pointer
//					labMaterial.get(), //convert to raw pointer if smart pointer was used
					"Lab");

	physicalLab =
		std::make_unique<G4PVPlacement>
					(nullptr,	   	 //no rotation
					G4ThreeVector(), //at (0,0,0)
					logicalLab.get(),//raw pointer to logical volume
					"Lab",           //name
					nullptr,         //mother volume
					false,           //no boolean operation
					0); 	         //copy number};

	return physicalLab.get(); //must be raw pointer as smart pointer not accepted
}
