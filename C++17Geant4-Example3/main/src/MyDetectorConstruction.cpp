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

G4VPhysicalVolume* MyDetectorConstruction::ConstructDetector()
{
	G4bool checkOverlaps {true};
	G4double opacity {0.4};

	G4VisAttributes* invisible = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
	invisible->SetVisibility(false);

	G4VisAttributes *orange = new G4VisAttributes(G4Colour(1, 0.65, 0, opacity));
	orange->SetVisibility(true);
	orange->SetForceWireframe(true); // Whichever is last overwrites the previous one!
	orange->SetForceSolid(true);     // Whichever is last overwrites the previous one!

	G4VisAttributes *yellow = new G4VisAttributes(G4Colour(1, 1, 0, opacity));
	yellow->SetVisibility(true);
	yellow->SetForceWireframe(true);
	yellow->SetForceSolid(true);

	G4VisAttributes *cyan = new G4VisAttributes(G4Colour(0, 1, 1, opacity));
	cyan->SetVisibility(true);
	cyan->SetForceSolid(true);

	G4VisAttributes *magenta = new G4VisAttributes(G4Colour(1, 0, 1, opacity));
	magenta->SetVisibility(true);
	magenta->SetForceSolid(true);

	G4VisAttributes *brown = new G4VisAttributes(
			G4Colour(0.545, 0.271, 0.075, opacity));
//			G4Colour(139.0/255.0, 69.0/255.0, 19.0/255.0, opacity));
	magenta->SetVisibility(true);
	magenta->SetForceSolid(true);

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


	G4double halfLengthX1 {8*cm}, halfLengthX2 {5*cm};
	G4double halfLengthY1 {6*cm}, halfLengthY2 {4*cm};
	G4double halfLengthZ {3*cm};
	solidTrapezoid = std::make_unique<G4Trd>
						("Trapezoid",
						halfLengthX1, halfLengthX2,
						halfLengthY1, halfLengthY2,
						halfLengthZ);

	logicalTrapezoid = std::make_unique<G4LogicalVolume>
						(solidTrapezoid.get(), //raw pointer
						trapezoidMaterial.get(),	   //raw pointer
						"Trapezoid");

//	This constructor uses physical mother volume
//	physicalTrapezoid = std::make_unique<G4PVPlacement>
//						(nullptr,				// no rotation
//						G4ThreeVector(), 		// at (x,y,z)
//						"Trapezoid",     		// name
//						logicalTrapezoid.get(),	// logical volume
//						physicalLab.get(), 		// physical mother volume
//						false,           		// no boolean operations
//						0,               		// copy number
//						checkOverlaps); 		// checking overlaps

//	This constructor uses logical mother volume
	physicalTrapezoid = std::make_unique<G4PVPlacement>
						(nullptr,				// no rotation
						G4ThreeVector(), 		// at (0, 0, 0)
						logicalTrapezoid.get(),	// logical volume
						"Trapezoid",     		// name
						logicalLab.get(), 		// logical mother volume
						false,           		// no boolean operations
						0,               		// copy number
						checkOverlaps); 		// checking overlaps

	logicalTrapezoid->SetVisAttributes(yellow);


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

//	This constructor uses physical mother volume
//	physicalSphere = std::make_unique<G4PVPlacement>
//						(nullptr,               // no rotation
//						G4ThreeVector(), 		// at (0, 0, 0)
//						"Sphere",      			// its name
//						logicalSphere.get(), 	// its logical volume
//						physicalTrapezoid.get(),// physical mother volume
//						false,           		// no boolean operations
//						0,               		// copy number
//						checkOverlaps); 		// checking overlaps

//	This constructor uses logical mother volume
	physicalSphere = std::make_unique<G4PVPlacement>
						(nullptr,               // no rotation
						G4ThreeVector(), 		// at (0, 0, 0)
						logicalSphere.get(), 	// its logical volume
						"Sphere",      			// its name
						logicalTrapezoid.get(), // logical mother volume
						false,           		// no boolean operations
						0,               		// copy number
						checkOverlaps); 		// checking overlaps

	logicalSphere->SetVisAttributes(orange);

	return physicalLab.get();
}

