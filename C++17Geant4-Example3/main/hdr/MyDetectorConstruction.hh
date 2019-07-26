#ifndef HDR_MYDETECTORCONSTRUCTION_HH_
#define HDR_MYDETECTORCONSTRUCTION_HH_

#include "G4VUserDetectorConstruction.hh"
#include <memory>

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

#include "G4Box.hh"
#include "G4Trd.hh"
#include "G4Sphere.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

class MyDetectorConstruction: public G4VUserDetectorConstruction
{
public:
	MyDetectorConstruction();
	virtual ~MyDetectorConstruction();
	virtual G4VPhysicalVolume* Construct() override;
	G4ThreeVector GetHalfLabSize() const { return halfLabSize; }

private:
	G4ThreeVector halfLabSize;
	std::unique_ptr<G4Box> solidLab {nullptr};
	std::unique_ptr<G4Box> solidBox {nullptr};
	std::unique_ptr<G4Trd> solidTrapezoid {nullptr};
	std::unique_ptr<G4Sphere> solidSphere {nullptr};
//	G4Box *solidLab = nullptr;
//	G4Trd *solidTrapezoid = nullptr;
//	G4Sphere *solidSphere = nullptr;

	std::unique_ptr<G4LogicalVolume> logicalLab {nullptr};
	std::unique_ptr<G4LogicalVolume> logicalBox {nullptr};
	std::unique_ptr<G4LogicalVolume> logicalTrapezoid {nullptr};
	std::unique_ptr<G4LogicalVolume> logicalSphere {nullptr};
//	G4LogicalVolume *logicalLab = nullptr;
//	G4LogicalVolume *logicalTrapezoid = nullptr;
//	G4LogicalVolume *logicalSphere = nullptr;

	std::unique_ptr<G4VPhysicalVolume> physicalLab {nullptr};
	std::unique_ptr<G4VPhysicalVolume> physicalBox {nullptr};
	std::unique_ptr<G4VPhysicalVolume> physicalTrapezoid {nullptr};
	std::unique_ptr<G4VPhysicalVolume> physicalSphere {nullptr};
//	G4VPhysicalVolume *physicalLab = nullptr;
//	G4VPhysicalVolume *physicalTrapezoid = nullptr;
//	G4VPhysicalVolume *physicalSphere = nullptr;

	G4Material *labMaterial = nullptr;
	G4Material *boxMaterial = nullptr;
	G4Material *trapezoidMaterial = nullptr;
	G4Material *sphereMaterial = nullptr;

	void DefineMaterials();
	G4VPhysicalVolume* ConstructDetector();
};

//	std::unique_ptr<G4Box> solidLab {nullptr};
////	std::unique_ptr<G4Trd> solidTrapezoid {nullptr};
//	G4Trd *solidTrapezoid = nullptr;
//	std::unique_ptr<G4Sphere> solidSphere {nullptr};
//
//	std::unique_ptr<G4LogicalVolume> logicalLab {nullptr};
//	std::unique_ptr<G4LogicalVolume> logicalTrapezoid {nullptr};
//	G4LogicalVolume *logicalTrapezoid = nullptr;
//	std::unique_ptr<G4LogicalVolume> logicalSphere {nullptr};
//
//	std::unique_ptr<G4VPhysicalVolume> physicalLab {nullptr};
//	std::unique_ptr<G4VPhysicalVolume> physicalTrapezoid {nullptr};
//	G4VPhysicalVolume *physicalTrapezoid = nullptr;
//
////	std::unique_ptr<G4Material> labMaterial {nullptr};
//	G4Material *labMaterial {nullptr}; // smart pointer not required since no new invoked
//	G4Material *trapezoidMaterial {nullptr};
//	G4Material *sphereMaterial {nullptr};
//
//	void DefineMaterials();
//	G4VPhysicalVolume* ConstructDetector();
//};

#endif /* HDR_MYDETECTORCONSTRUCTION_HH_ */
