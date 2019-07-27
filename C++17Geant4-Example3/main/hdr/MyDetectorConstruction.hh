#ifndef HDR_MYDETECTORCONSTRUCTION_HH_
#define HDR_MYDETECTORCONSTRUCTION_HH_

#include "G4VUserDetectorConstruction.hh"
#include <memory>
using std::unique_ptr;
using std::make_unique;

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4VisAttributes;

#include "G4Box.hh"
#include "G4Trd.hh"
#include "G4Sphere.hh"
#include "G4Torus.hh"
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
	unique_ptr<G4Material> labMaterial {nullptr};
	unique_ptr<G4Material> trapezoidMaterial {nullptr};
	unique_ptr<G4Material> sphereMaterial {nullptr};
	unique_ptr<G4Material> torusMaterial {nullptr};
//	G4Material *labMaterial {nullptr};
//	G4Material *trapezoidMaterial {nullptr};
//	G4Material *sphereMaterial {nullptr};
//	G4Material *torusMaterial {nullptr};

	unique_ptr<G4Box> solidLab {nullptr};
	unique_ptr<G4Trd> solidTrapezoid {nullptr};
	unique_ptr<G4Sphere> solidSphere {nullptr};
	unique_ptr<G4Torus> solidTorus {nullptr};

	unique_ptr<G4LogicalVolume> logicalLab {nullptr};
	unique_ptr<G4LogicalVolume> logicalTrapezoid {nullptr};
	unique_ptr<G4LogicalVolume> logicalSphere {nullptr};
	unique_ptr<G4LogicalVolume> logicalTorus {nullptr};

	unique_ptr<G4VPhysicalVolume> physicalLab {nullptr};
	unique_ptr<G4VPhysicalVolume> physicalTrapezoid {nullptr};
	unique_ptr<G4VPhysicalVolume> physicalSphere {nullptr};
	unique_ptr<G4VPhysicalVolume> physicalTorus {nullptr};

	G4bool checkOverlaps {true};

	enum class Colour { yellow, orange, brown, cyan, magenta, invisible };
	Colour colour { Colour::yellow };

	enum class Texture { wireframe, solid };
	Texture texture { Texture::solid };

	enum class Material { Al, Ti, CsI, vacuum, air, G4H2Oliquid, G4H2Osteam, Pb };
	Material material { Material::vacuum };

private:
	G4VPhysicalVolume* ConstructDetector();
	unique_ptr<G4VPhysicalVolume> BuildLab();
	unique_ptr<G4VPhysicalVolume> BuildTrapezoid();
	unique_ptr<G4VPhysicalVolume> BuildSphere();
	unique_ptr<G4VPhysicalVolume> BuildTorus();
	unique_ptr<G4VisAttributes> ChooseColour(Colour colour, Texture texture = Texture::solid);
	unique_ptr<G4Material> ChooseMaterial(Material material);
};

#endif /* HDR_MYDETECTORCONSTRUCTION_HH_ */
