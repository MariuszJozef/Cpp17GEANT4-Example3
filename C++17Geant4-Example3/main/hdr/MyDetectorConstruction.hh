#ifndef HDR_MYDETECTORCONSTRUCTION_HH_
#define HDR_MYDETECTORCONSTRUCTION_HH_

#include "G4VUserDetectorConstruction.hh"
#include <memory>

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4VisAttributes;

#include "G4Box.hh"
#include "G4Trd.hh"
#include "G4Sphere.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
//#include "G4VisAttributes.hh"

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
	std::unique_ptr<G4Trd> solidTrapezoid {nullptr};
	std::unique_ptr<G4Sphere> solidSphere {nullptr};

	std::unique_ptr<G4LogicalVolume> logicalLab {nullptr};
	std::unique_ptr<G4LogicalVolume> logicalTrapezoid {nullptr};
	std::unique_ptr<G4LogicalVolume> logicalSphere {nullptr};

	std::unique_ptr<G4VPhysicalVolume> physicalLab {nullptr};
	std::unique_ptr<G4VPhysicalVolume> physicalTrapezoid {nullptr};
	std::unique_ptr<G4VPhysicalVolume> physicalSphere {nullptr};

	std::unique_ptr<G4Material> labMaterial {nullptr};
	std::unique_ptr<G4Material> trapezoidMaterial {nullptr};
	std::unique_ptr<G4Material> sphereMaterial {nullptr};
//	G4Material *labMaterial {nullptr};
//	G4Material *trapezoidMaterial {nullptr};
//	G4Material *sphereMaterial {nullptr};

	enum class Colour { yellow, orange, brown, cyan, magenta, invisible };
	Colour colour { Colour::yellow };
	enum class Texture { wireframe, solid };
	Texture texture { Texture::solid };
	enum class Material { Al, Ti, CsI, vacuum, air, G4H2Oliquid, G4H2Osteam, Pb };
	Material material { Material::vacuum };

private:
	void DefineMaterials();
	G4VPhysicalVolume* ConstructDetector();
	std::unique_ptr<G4VisAttributes> ChooseColour(Colour colour, Texture texture = Texture::solid);
	std::unique_ptr<G4Material> ChooseMaterial(Material material);
};

#endif /* HDR_MYDETECTORCONSTRUCTION_HH_ */
