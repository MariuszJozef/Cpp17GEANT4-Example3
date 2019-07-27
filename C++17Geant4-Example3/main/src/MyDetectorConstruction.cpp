#include "MyDetectorConstruction.hh"

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4VisAttributes.hh"
#include "G4RotationMatrix.hh"

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
	labMaterial = ChooseMaterial(Material::G4H2Osteam);
	trapezoidMaterial = ChooseMaterial(Material::CsI);
	sphereMaterial = ChooseMaterial(Material::CsI);
	torusMaterial = ChooseMaterial(Material::G4H2Oliquid);
}

std::unique_ptr<G4Material> MyDetectorConstruction::ChooseMaterial(Material material)
{
	G4NistManager *nist = G4NistManager::Instance();
	G4String symbol;
	G4double a, z, density;
	G4int ncomponents;
	std::unique_ptr<G4Material> chosenMaterial;

	switch (material)
	{
		case Material::Al:
		{
			chosenMaterial = std::make_unique<G4Material>
								("Aluminum", z=13., a=26.98*g/mole, density=2.7*g/cm3);
			break;
		}
		case Material::Ti:
		{
			chosenMaterial = std::make_unique<G4Material>
								("Titanium", z=22, a=47.867*g/mole, density=4.54*g/cm3);
			break;
		}
		case Material::CsI:
		{
			std::unique_ptr<G4Element> Cs
			{std::make_unique<G4Element>("Cesium", symbol="Cs", z=55, a=132.9*g/mole)};

			std::unique_ptr<G4Element> I
			{std::make_unique<G4Element>("Iodine", symbol="I", z=53, a=126.9*g/mole)};

			chosenMaterial =
				std::make_unique<G4Material>("CsI", density=4.51*g/cm3, ncomponents=2);
//			N.B. AddElement accepts a raw pointer and takes ownership of it,
//			therefore use .release() NOT .get()
			chosenMaterial->AddElement(I.release(), 0.5);
			chosenMaterial->AddElement(Cs.release(), 0.5);
			break;
		}
		case Material::vacuum:
		{
			G4Material *vacuum = nist->FindOrBuildMaterial("G4_Galactic");
			chosenMaterial = std::unique_ptr<G4Material>(vacuum);
			break;
		}
		case Material::air:
		{
			G4Material *air = nist->FindOrBuildMaterial("G4_AIR");
			chosenMaterial = std::unique_ptr<G4Material>(air);
			break;
		}
		case Material::G4H2Oliquid:
		{
			G4Material *G4H2Oliquid = nist->FindOrBuildMaterial("G4_WATER");
			chosenMaterial = std::unique_ptr<G4Material>(G4H2Oliquid);
			break;
		}
		case Material::G4H2Osteam:
		{
			G4Material *G4H2Osteam = nist->FindOrBuildMaterial("G4_WATER_VAPOR");
			chosenMaterial = std::unique_ptr<G4Material>(G4H2Osteam);
			break;
		}
		case Material::Pb:
		{
			G4Material *Pb = nist->FindOrBuildMaterial("G4_Pb");
			chosenMaterial = std::unique_ptr<G4Material>(Pb);
			break;
		}
	}
	return chosenMaterial;
}

G4VPhysicalVolume* MyDetectorConstruction::ConstructDetector()
{
	G4bool checkOverlaps {true};

	solidLab = std::make_unique<G4Box>("Lab",
					halfLabSize.x(), halfLabSize.y(), halfLabSize.z());

	logicalLab = std::make_unique<G4LogicalVolume>
					(solidLab.get(),   // convert to raw pointer
					labMaterial.get(), // convert to raw pointer
					"Lab");

	physicalLab = std::make_unique<G4PVPlacement>
					(nullptr,	   	 // no rotation
					G4ThreeVector(), // at (0,0,0)
					logicalLab.get(),// raw pointer to logical volume
					"Lab",           // name
					nullptr,         // mother volume
					false,           // no boolean operation
					0); 	         // copy number};

//	Uncomment below if you don't want to visualise the lab box
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
						(solidTrapezoid.get(), 		// convert to raw pointer
						trapezoidMaterial.get(),    // convert to raw pointer
						"Trapezoid");

//	Various G4PVPlacement constructors are available, e.g. uncomment #1 or #2 (not both)
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
						logicalTrapezoid.get(),	// convert to raw pointer
						"Trapezoid",     		// name
						logicalLab.get(), 		// logical mother volume
						false,           		// no boolean operations
						0,               		// copy number
						checkOverlaps); 		// checking overlaps

//	The SetVisAttributes method accepts a raw pointer and takes ownership of it,
//	hence must .release() the uniquie_ptr returned by ChooseColour
	logicalTrapezoid->SetVisAttributes(ChooseColour(Colour::yellow).release());


	solidSphere = std::make_unique<G4Sphere>
						("Sphere",	   // name
						0, 			   // innerRadius,
						2.5*cm,		   // outerRadius,
						0*deg,         // Starting phi
						360*deg,       // Delta phi
						0*deg,         // Starting theta
						180*deg);      // Delta theta

	logicalSphere = std::make_unique<G4LogicalVolume>
						(solidSphere.get(),		// convert to raw pointer
						sphereMaterial.get(),	// convert to raw pointer
						"Sphere");

//	Various G4PVPlacement constructors are available, e.g. uncomment #1 or #2 (not both)
//	1. This constructor uses physical mother volume:
//	physicalSphere = std::make_unique<G4PVPlacement>
//						(nullptr,               // no rotation
//						G4ThreeVector(), 		// at (0, 0, 0)
//						"Sphere",      			// its name
//						logicalSphere.get(), 	// convert to raw pointer
//						physicalTrapezoid.get(),// NB: NOT physicalLab for mother volume
//						false,           		// no boolean operations
//						0,               		// copy number
//						checkOverlaps); 		// checking overlaps

//	2. This constructor uses logical mother volume:
	physicalSphere = std::make_unique<G4PVPlacement>
						(nullptr,               // no rotation
						G4ThreeVector(),		// at (0, 0, 0)
						logicalSphere.get(), 	// convert to raw pointer
						"Sphere",      			// its name
						logicalTrapezoid.get(), // NB: NOT logicalLab for mother volume
						false,           		// no boolean operations
						0,               		// copy number 0
						checkOverlaps); 		// checking overlaps

//	The SetVisAttributes method accepts a raw pointer and takes ownership of it,
//	hence must .release() the uniquie_ptr returned by ChooseColour
	logicalSphere->SetVisAttributes(ChooseColour(Colour::orange, Texture::wireframe).release());


	G4double radius1min {1.25*cm}, radius1max {2*cm}, radius2 {4*cm};
	G4double angle1 {60*deg}, angle2 {2.5*rad};
	solidTorus = std::make_unique<G4Torus>
							("Torus",
							radius1min, radius1max,
							radius2,
							angle1, angle2);

	logicalTorus = std::make_unique<G4LogicalVolume>
							(solidTorus.get(),		// convert to raw pointer
							torusMaterial.get(),	// convert to raw pointer
							"Torus");

	std::unique_ptr<G4RotationMatrix> rotation {std::make_unique<G4RotationMatrix>()};
//	G4RotationMatrix *rotation = new G4RotationMatrix();
	rotation->rotateX(150*deg);
	rotation->rotateY(0*deg);
	rotation->rotateZ(5*deg);
	physicalTorus = std::make_unique<G4PVPlacement>
							(rotation.release(),        // give up ownership of rotation pointer
//							(rotation,			        // raw pointer
							G4ThreeVector(14*cm, 9*cm, 6*cm), // at (x,y,z)
							logicalTorus.get(),			// its logical volume
							"Torus",		   			// its name
							logicalLab.get(),    		// logical mother volume
							false,           			// no boolean operations
							0,               			// copy number
							checkOverlaps); 			// checking overlaps

	logicalTorus->SetVisAttributes(ChooseColour(Colour::magenta, Texture::solid).release());

	return physicalLab.get();
}

std::unique_ptr<G4VisAttributes> MyDetectorConstruction::
						ChooseColour(Colour colour, Texture texture)
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
