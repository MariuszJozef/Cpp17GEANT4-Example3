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

	G4Material* Al = new G4Material("Aluminum", z=13., a=26.98*g/mole, density=2.7*g/cm3);

	G4Element* Cs = new G4Element("Cesium", symbol="Cs", z=55, a=132.9*g/mole);
	G4Element* I = new G4Element("Iodine", symbol="I", z=53, a=126.9*g/mole);
	G4Material* CsI = new G4Material("CsI", density=4.51*g/cm3, ncomponents=2);
	CsI->AddElement(I, .5);
	CsI->AddElement(Cs, .5);

//	labMaterial = std::unique_ptr<G4Material>(air); // smart pointer not required
	labMaterial = air;
	boxMaterial = Pb;
	trapezoidMaterial = Al;
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

//	solidLab = new G4Box("Lab",          //its name
//						halfLabSize.x(), halfLabSize.y(), halfLabSize.z());	//its size
//
//	logicalLab = new G4LogicalVolume(solidLab,
//									labMaterial, //its material
//									"Lab");      //its name
//
//	physicalLab = new G4PVPlacement(0,   			 //no rotation
//									G4ThreeVector(), //at (0,0,0)
//									logicalLab,      //its logical volume
//									"Lab",           //its name
//									0, //NULL,       //its mother volume
//									false,           //no boolean operation
//									0);              //copy number

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

	solidBox = std::make_unique<G4Box>("Box", 3*cm, 4*cm, 5*cm);

	logicalBox = std::make_unique<G4LogicalVolume>
					(solidBox.get(),   //raw pointer
					boxMaterial, 	   //raw pointer
//					boxMaterial.get(), //convert to raw pointer if smart pointer was used
					"Box");

	 physicalBox = std::make_unique<G4PVPlacement>
	 				(nullptr,	   	 //no rotation
	 				G4ThreeVector(0, 0, 9*cm), //at (0,0,0)
	 				logicalBox.get(),//raw pointer to logical volume
	 				"Box",           //name
	 				logicalLab.get(),//mother volume
	 				false,           //no boolean operation
	 				0); 	         //copy number};

	 logicalBox->SetVisAttributes(brown);

	G4double halfLengthX1 {8*cm}, halfLengthX2 {5*cm};
	G4double halfLengthY1 {6*cm}, halfLengthY2 {4*cm};
	G4double halfLengthZ {3*cm};
//	solidTrapezoid = new G4Trd("Trapezoid",
//								halfLengthX1, halfLengthX2,
//								halfLengthY1, halfLengthY2,
//								halfLengthZ);
//
//	logicalTrapezoid = new G4LogicalVolume(solidTrapezoid,
//	                                       trapezoidMaterial,
//	                                       "Trapezoid");
//
//	new G4PVPlacement(0, //rotation
//					G4ThreeVector(0, 0, 0), // translation
//					"Trapezoid",            // name
//					logicalTrapezoid,       // logical volume
//					physicalLab.get(),            // mother volume
//					checkOverlaps,
//					0);						// copy number

	solidTrapezoid = std::make_unique<G4Trd>
						("Trapezoid",
						halfLengthX1, halfLengthX2,
						halfLengthY1, halfLengthY2,
						halfLengthZ);

	logicalTrapezoid = std::make_unique<G4LogicalVolume>
						(solidTrapezoid.get(), //raw pointer
						trapezoidMaterial, 	   //raw pointer
						"Trapezoid");

	physicalTrapezoid = std::make_unique<G4PVPlacement>
						(nullptr,				// no rotation
						G4ThreeVector(0, 0, 0), // at (x,y,z)
						"Trapezoid",     		// name
						logicalTrapezoid.get(),	// logical volume
						physicalLab.get(), 		// logical mother volume
						false,           		// no boolean operations
						0,               		// copy number
						checkOverlaps); 		// checking overlaps
//	std::make_unique<G4PVPlacement>
//						(nullptr,				// no rotation
//						G4ThreeVector(0, 0, 0), // at (x,y,z)
//						"Trapezoid",     		// name
//						logicalTrapezoid.get(),	// logical volume
//						physicalLab.get(), 		// logical mother volume
//						false,           		// no boolean operations
//						checkOverlaps,
//						0);               		// copy number
//	std::make_unique<G4PVPlacement>
//						(nullptr,				// no rotation
//						G4ThreeVector(0, 0, 0), // at (x,y,z)
//						logicalTrapezoid.get(),	// logical volume
//						"Trapezoid",     		// name
//						logicalLab.get(), 		// logical mother volume
//						false,           		// no boolean operations
//						0,               		// copy number
//						checkOverlaps); 		// checking overlaps

	logicalTrapezoid->SetVisAttributes(yellow);


//	solidSphere = new G4Sphere("Sphere",
//							0, 			   // innerRadius,
//							2.5*cm,		   // outerRadius,
//							0*deg,         // Starting phi
//							360*deg,       // Delta phi
//							0*deg,         // Starting theta
//							180*deg);      // Delta theta
//
//	logicalSphere = new G4LogicalVolume(solidSphere,
//										sphereMaterial,
//										"Sphere",
//										0, 0, 0);
//
//	new G4PVPlacement(0,               			// no rotation
//	                  G4ThreeVector(0, 0, 0), 	// at (x,y,z)
//					  logicalSphere, 			// its logical volume
//	                  "Sphere",      			// its name
////	                  logicalTrapezoid,     	// mother volume
////	                  logicalTrapezoid.get(),     	// mother volume
//					  logicalLab.get(),
//	                  false,           			// no boolean operations
//	                  0,               			// copy number
//	                  checkOverlaps); 			// checking overlaps

	solidSphere = std::make_unique<G4Sphere>("Sphere",
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

//	std::unique_ptr<G4VPhysicalVolume> physicalSphere
	physicalSphere = std::make_unique<G4PVPlacement>
						(nullptr,               // no rotation
						G4ThreeVector(-5*cm, -3*cm, -8*cm), // at (x,y,z)
						logicalSphere.get(), 	// its logical volume
						"Sphere",      			// its name
//						logicalTrapezoid,     	// mother volume
//						logicalTrapezoid.get(), // mother volume
						logicalLab.get(),
						false,           		// no boolean operations
						0,               		// copy number
						checkOverlaps); 		// checking overlaps
//	new G4PVPlacement	(nullptr,               			// no rotation
//						G4ThreeVector(0, 0, 0), 	// at (x,y,z)
//						logicalSphere.get(), 			// its logical volume
//						"Sphere",      			// its name
//						//	                  logicalTrapezoid,     	// mother volume
//						//	                  logicalTrapezoid.get(),     	// mother volume
//						logicalLab.get(),
//						false,           			// no boolean operations
//						0,               			// copy number
//						checkOverlaps); 			// checking overlaps

	logicalSphere->SetVisAttributes(orange);

	return physicalLab.get();
}



//G4VPhysicalVolume* MyDetectorConstruction::ConstructDetector()
//{
//	G4bool checkOverlaps {true};
//	G4double opacity {0.4};
//
//	std::unique_ptr<G4VisAttributes> invisible {std::make_unique<G4VisAttributes>(G4Colour(1, 1, 1))};
//	invisible->SetVisibility(false);
//
//	std::unique_ptr<G4VisAttributes> orange {std::make_unique<G4VisAttributes>(G4Colour(1, 0.65, 0, opacity))};
//	orange->SetVisibility(true);
//	orange->SetForceWireframe(true); // Whichever is last overwrites the previous one!
//	orange->SetForceSolid(true);     // Whichever is last overwrites the previous one!
//
//	std::unique_ptr<G4VisAttributes> yellow {std::make_unique<G4VisAttributes>(G4Colour(1, 1, 0, opacity))};
//	yellow->SetVisibility(true);
//	yellow->SetForceSolid(true);
//
//	std::unique_ptr<G4VisAttributes> cyan {std::make_unique<G4VisAttributes>(G4Colour(0, 1, 1, opacity))};
//	cyan->SetVisibility(true);
//	cyan->SetForceSolid(true);
//
//	std::unique_ptr<G4VisAttributes> magenta {std::make_unique<G4VisAttributes>(G4Colour(1, 0, 1, opacity))};
//	magenta->SetVisibility(true);
//	magenta->SetForceSolid(true);
//
//	std::unique_ptr<G4VisAttributes> brown {std::make_unique<G4VisAttributes>(G4Colour(139.0/255.0, 69.0/255.0, 19.0/255.0, opacity))};
//	magenta->SetVisibility(true);
//	magenta->SetForceSolid(true);
//
//
//
//
//
//	solidTrapezoid = new G4Trd("Trapezoid",
//								halfLengthX1, halfLengthX2,
//								halfLengthY1, halfLengthY2,
//								halfLengthZ);
//
//	logicalTrapezoid = new G4LogicalVolume(solidTrapezoid,
//	                                       trapezoidMaterial,
//	                                       "Trapezoid");
//
//	new G4PVPlacement(nullptr,					// no rotation
//	                  G4ThreeVector(0, 0, 0), 	// at (x,y,z)
//					  logicalTrapezoid,			// logical volume
//	                  "Trapezoid",     			// name
//	                  logicalLab.get(),	     	// logical mother volume
//	                  false,           			// no boolean operations
//	                  0,               			// copy number
//	                  checkOverlaps); 			// checking overlaps
//
//
//	logicalTrapezoid->SetVisAttributes(yellow.get());
//
//	return physicalLab.get(); //must be raw pointer as smart pointer not accepted
//}
