#include "MyDetectorConstruction.hh"

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4VisAttributes.hh"

MyDetectorConstruction::MyDetectorConstruction()
:
G4VUserDetectorConstruction(),
halfLabSize{G4ThreeVector(20*cm, 21*cm, 22*cm)}
{
}

MyDetectorConstruction::~MyDetectorConstruction() {}

G4VPhysicalVolume* MyDetectorConstruction::Construct()
{
	DefineMaterials();
	return ConstructDetector();
}

void MyDetectorConstruction::DefineMaterials()
{
	G4NistManager *nist = G4NistManager::Instance();
	G4Material *vacuum = nist->FindOrBuildMaterial("G4_Galactic");
	G4Material *air = nist->FindOrBuildMaterial("G4_AIR");
	G4Material *G4H2Oliquid = nist->FindOrBuildMaterial("G4_WATER");
	G4Material *G4H2Osteam = nist->FindOrBuildMaterial("G4_WATER_VAPOR");
	G4Material *Pb = nist->FindOrBuildMaterial("G4_Pb");

	G4String symbol;
	G4double a, z, density;
	G4int ncomponents;

//	G4Material* Al = new G4Material("Aluminum", z=13., a=26.98*g/mole, density=2.7*g/cm3);
//	std::unique_ptr<G4Material> Al {std::make_unique<G4Material>("Aluminum", z=13., a=26.98*g/mole, density=2.7*g/cm3)};
	auto Al {std::make_unique<G4Material>("Aluminum", z=13., a=26.98*g/mole, density=2.7*g/cm3)};

	G4Element* Cs = new G4Element("Cesium", symbol="Cs", z=55, a=132.9*g/mole);
	G4Element* I = new G4Element("Iodine", symbol="I", z=53, a=126.9*g/mole);
	G4Material* CsI = new G4Material("CsI", density=4.51*g/cm3, ncomponents=2);
	CsI->AddElement(I, .5);
	CsI->AddElement(Cs, .5);

//	labMaterial = std::unique_ptr<G4Material>(air); // smart pointer not required
	labMaterial = air;
//	trapezoidMaterial = Al.get();
	trapezoidMaterial = std::move(Al);
	sphereMaterial = G4H2Oliquid;
}

std::unique_ptr<G4VisAttributes> MyDetectorConstruction::
						ChooseColour(Colour colour, Texture texture = Texture::solid)
{
	G4double opacity {0.4};
	G4bool isVisible {true};
	std::unique_ptr<G4VisAttributes> chosenColour;

	switch (colour)
	{
		case Colour::yellow:
		{
			chosenColour = std::make_unique<G4VisAttributes>(G4Colour(1, 1, 0, opacity));
			break;
		}
		case Colour::orange:
		{
			chosenColour = std::make_unique<G4VisAttributes>(G4Colour(1, 0.65, 0, opacity));
			break;
		}
		case Colour::brown:
		{
			chosenColour = std::make_unique<G4VisAttributes>
								(G4Colour(0.545, 0.271, 0.075, opacity));
			break;
		}
		case Colour::cyan:
		{
			chosenColour = std::make_unique<G4VisAttributes>(G4Colour(0, 1, 1, opacity));
			break;
		}
		case Colour::magenta:
		{
			chosenColour = std::make_unique<G4VisAttributes>(G4Colour(1, 0, 1, opacity));
			break;
		}
		case Colour::invisible:
		{
			chosenColour = std::make_unique<G4VisAttributes>(G4Colour(1, 1, 1, opacity));
			isVisible = false;
			break;
		}
		default:
		{
			chosenColour = std::make_unique<G4VisAttributes>(G4Colour(1, 1, 1, opacity));
			break;
		}
	}

	switch (texture)
	{
	case Texture::wireframe:
		chosenColour->SetForceWireframe(true);
		break;
	case Texture::solid:
		chosenColour->SetForceSolid(true);
		break;
	}

	chosenColour->SetVisibility(isVisible);
	return chosenColour;
}

G4VPhysicalVolume* MyDetectorConstruction::ConstructDetector()
{
	G4bool checkOverlaps {true};

	solidLab = std::make_unique<G4Box>("Lab",
					halfLabSize.x(), halfLabSize.y(), halfLabSize.z());

	logicalLab = std::make_unique<G4LogicalVolume>
					(solidLab.get(),   //raw pointer
					labMaterial, 	   //raw pointer
					"Lab");

	physicalLab = std::make_unique<G4PVPlacement>
					(nullptr,	   	 //no rotation
					G4ThreeVector(), //at (0,0,0)
					logicalLab.get(),//raw pointer to logical volume
					"Lab",           //name
					nullptr,         //mother volume
					false,           //no boolean operation
					0); 	         //copy number};

//	logicalLab->SetVisAttributes(ChooseColour(Colour::invisible).release());


	G4double halfLengthX1 {8*cm}, halfLengthX2 {5*cm};
	G4double halfLengthY1 {6*cm}, halfLengthY2 {4*cm};
	G4double halfLengthZ {3*cm};
	solidTrapezoid = std::make_unique<G4Trd>
						("Trapezoid",
						halfLengthX1, halfLengthX2,
						halfLengthY1, halfLengthY2,
						halfLengthZ);

	logicalTrapezoid = std::make_unique<G4LogicalVolume>
						(solidTrapezoid.get(), 		//raw pointer
						trapezoidMaterial.get(),    //raw pointer
						"Trapezoid");

//	Various G4PVPlacement constructors are available, e.g.
//	1. This constructor uses physical mother volume:
//	physicalTrapezoid = std::make_unique<G4PVPlacement>
//						(nullptr,				// no rotation
//						G4ThreeVector(), 		// at (x,y,z)
//						"Trapezoid",     		// name
//						logicalTrapezoid.get(),	// logical volume
//						physicalLab.get(), 		// physical mother volume
//						false,           		// no boolean operations
//						0,               		// copy number
//						checkOverlaps); 		// checking overlaps

//	2. This constructor uses logical mother volume:
	physicalTrapezoid = std::make_unique<G4PVPlacement>
						(nullptr,				// no rotation
						G4ThreeVector(), 		// at (0, 0, 0)
						logicalTrapezoid.get(),	// logical volume
						"Trapezoid",     		// name
						logicalLab.get(), 		// logical mother volume
						false,           		// no boolean operations
						0,               		// copy number
						checkOverlaps); 		// checking overlaps

//	The SetVisAttributes method accepts a raw pointer and takes ownership of it,
//	hence must .release() the uniquie_ptr returned by ChooseColour
	logicalTrapezoid->SetVisAttributes(ChooseColour(Colour::yellow).release());


	solidSphere = std::make_unique<G4Sphere>
						("Sphere",
						0, 			   // innerRadius,
						2.5*cm,		   // outerRadius,
						0*deg,         // Starting phi
						360*deg,       // Delta phi
						0*deg,         // Starting theta
						180*deg);      // Delta theta

	logicalSphere = std::make_unique<G4LogicalVolume>
						(solidSphere.get(),
						sphereMaterial,
						"Sphere");

//	Various G4PVPlacement constructors are available, e.g.
//	1. This constructor uses physical mother volume:
//	physicalSphere = std::make_unique<G4PVPlacement>
//						(nullptr,               // no rotation
//						G4ThreeVector(), 		// at (0, 0, 0)
//						"Sphere",      			// its name
//						logicalSphere.get(), 	// its logical volume
//						physicalTrapezoid.get(),// physical mother volume
//						false,           		// no boolean operations
//						0,               		// copy number
//						checkOverlaps); 		// checking overlaps

//	2. This constructor uses logical mother volume:
	physicalSphere = std::make_unique<G4PVPlacement>
						(nullptr,               // no rotation
						G4ThreeVector(),		// at (0, 0, 0)
						logicalSphere.get(), 	// its logical volume
						"Sphere",      			// its name
						logicalTrapezoid.get(), // logical mother volume
						false,           		// no boolean operations
						0,               		// copy number
						checkOverlaps); 		// checking overlaps

//	The SetVisAttributes method accepts a raw pointer and takes ownership of it,
//	hence must .release() the uniquie_ptr returned by ChooseColour
	logicalSphere->SetVisAttributes(ChooseColour(Colour::orange, Texture::solid).release());

	return physicalLab.get();
}

