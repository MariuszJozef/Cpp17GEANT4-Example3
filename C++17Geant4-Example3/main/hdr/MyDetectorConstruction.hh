#ifndef HDR_MYDETECTORCONSTRUCTION_HH_
#define HDR_MYDETECTORCONSTRUCTION_HH_

#include "G4VUserDetectorConstruction.hh"
#include <memory>

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4Box;

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

private:
	G4ThreeVector halfLabSize;

	std::unique_ptr<G4Box> solidLab {nullptr};
	std::unique_ptr<G4LogicalVolume> logicalLab {nullptr};
	std::unique_ptr<G4VPhysicalVolume> physicalLab {nullptr};

//	std::unique_ptr<G4Material> labMaterial {nullptr};
	G4Material *labMaterial {nullptr}; // smart pointer not required since no new invoked

	void DefineMaterials();
	G4VPhysicalVolume* ConstructDetector();
};

#endif /* HDR_MYDETECTORCONSTRUCTION_HH_ */
