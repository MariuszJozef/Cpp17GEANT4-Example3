#include <memory>
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"

#include "MyDetectorConstruction.hh"
#include "MyPhysicsList.hh"
#include "MyPrimaryGeneratorAction.hh"

#include "Randomize.hh"
#include "time.h"

int main(int argc, char** argv)
{
	std::unique_ptr<G4RunManager> runManager {std::make_unique<G4RunManager>()};

	// Randomise the gun firing sequence, 
	// otherwise each simulation run would be the same.
	CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine());
	G4long seed {time(nullptr)};
	CLHEP::HepRandom::setTheSeed(seed);

	std::unique_ptr<MyDetectorConstruction> myDetectorConstruction
			{std::make_unique<MyDetectorConstruction>()};
	runManager->SetUserInitialization(myDetectorConstruction.get());

	std::unique_ptr<MyPhysicsList> myPhysicsList {std::make_unique<MyPhysicsList>()};
	runManager->SetUserInitialization(myPhysicsList.get());

	std::unique_ptr<G4VUserPrimaryGeneratorAction> myPrimaryGeneratorAction
	{std::make_unique<MyPrimaryGeneratorAction>()};
	runManager->SetUserAction(myPrimaryGeneratorAction.get());

	std::unique_ptr<G4VisManager> visManager {std::make_unique<G4VisExecutive>()};
	visManager->Initialize();

	G4UImanager *uiManager {G4UImanager::GetUIpointer()};
	std::unique_ptr<G4UIExecutive> uiExecutive
			{std::make_unique<G4UIExecutive>(argc, argv)};

	uiManager->ApplyCommand("/control/execute visualisation.macro");
	uiExecutive->SessionStart();

	return 0;
}
