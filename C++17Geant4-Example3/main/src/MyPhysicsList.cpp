#include "MyPhysicsList.hh"
#include "G4ParticleTypes.hh"

MyPhysicsList::MyPhysicsList() {}

MyPhysicsList::~MyPhysicsList() {}

void MyPhysicsList::ConstructParticle()
{
	G4Geantino::GeantinoDefinition();
	G4Gamma::GammaDefinition();
	G4OpticalPhoton::OpticalPhotonDefinition();
	G4Electron::ElectronDefinition();
	G4Positron::PositronDefinition();
	G4Proton::ProtonDefinition();
}

void MyPhysicsList::ConstructProcess()
{
	// Define only the transportation process; no interactions yet!
	AddTransportation();
}

void MyPhysicsList::SetCuts()
{
	// suppress error messages even in case e/gamma/proton do not exist
	G4int temp = GetVerboseLevel();
	SetVerboseLevel(0);
	//  "G4VUserPhysicsList::SetCutsWithDefault" method sets
	//   the default cut value for all particle types
	SetCutsWithDefault();

	// Retrieve verbose level
	SetVerboseLevel(temp);
}
