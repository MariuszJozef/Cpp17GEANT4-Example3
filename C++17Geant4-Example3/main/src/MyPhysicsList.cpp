#include "MyPhysicsList.hh"
//#include "MyPhysicsListEM.hh"
#include "G4ParticleTypes.hh"

MyPhysicsList::MyPhysicsList()
: G4VModularPhysicsList()
//  myPhysicsListEM(0)
{
//	myPhysicsListEM = new MyPhysicsListEM();
	SetCutsWithDefault();
	SetVerboseLevel(0);
}

MyPhysicsList::~MyPhysicsList() {}

void MyPhysicsList::ConstructParticle()
{
	G4Gamma::GammaDefinition();
	G4OpticalPhoton::OpticalPhotonDefinition();

	G4Electron::ElectronDefinition();
	G4Positron::PositronDefinition();

	G4Proton::ProtonDefinition();
	G4AntiProton::AntiProtonDefinition();
}

void MyPhysicsList::ConstructProcess()
{
	AddTransportation();
	// Include electromagnetic interactions
//	myPhysicsListEM->ConstructProcess();
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
