#include "MyPhysicsListEM.hh"
#include "globals.hh"
using std::unique_ptr;
using std::make_unique;

MyPhysicsListEM::MyPhysicsListEM()
: G4VPhysicsConstructor()
{}

MyPhysicsListEM::~MyPhysicsListEM() {}

void MyPhysicsListEM::ConstructProcess()
{
	ConstructEM();
}

#include "G4PhotoElectricEffect.hh"
#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"

#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4hMultipleScattering.hh"
#include "G4hIonisation.hh"
#include "G4hBremsstrahlung.hh"
#include "G4hPairProduction.hh"

#include "G4UAtomicDeexcitation.hh"
#include "G4LossTableManager.hh"

#include "G4PhysicsListHelper.hh"

void MyPhysicsListEM::ConstructEM()
{
	G4PhysicsListHelper *ph = G4PhysicsListHelper::GetPhysicsListHelper();

	auto particleIterator = GetParticleIterator();
	particleIterator->reset();

	while( (*particleIterator)() )
	{
		G4ParticleDefinition *particle = particleIterator->value();
		G4String particleName = particle->GetParticleName();

		if (particleName == "gamma")
		{
			ph->RegisterProcess(make_unique<G4PhotoElectricEffect>().release(),   particle);
			ph->RegisterProcess(make_unique<G4ComptonScattering>().release(), particle);
			ph->RegisterProcess(make_unique<G4GammaConversion>().release(),   particle);
		}
		else if (particleName == "opticalphoton")
		{
			// Rayleigh scattering, Thomson scattering ???
		}
		else if (particleName == "e-")
		{
			ph->RegisterProcess(make_unique<G4eMultipleScattering>().release(), particle);
			ph->RegisterProcess(make_unique<G4eIonisation>().release(), 		particle);
			ph->RegisterProcess(make_unique<G4eBremsstrahlung>().release(), 	particle);
		}
		else if (particleName == "e+")
		{
			ph->RegisterProcess(make_unique<G4eMultipleScattering>().release(), particle);
			ph->RegisterProcess(make_unique<G4eIonisation>().release(), 		particle);
			ph->RegisterProcess(make_unique<G4eBremsstrahlung>().release(), 	particle);
			ph->RegisterProcess(make_unique<G4eplusAnnihilation>().release(), 	particle);
		}
		else if ( particleName == "proton" )
		{
			ph->RegisterProcess(make_unique<G4hMultipleScattering>().release(), particle);
			ph->RegisterProcess(make_unique<G4hIonisation>().release(), 		particle);
			ph->RegisterProcess(make_unique<G4hBremsstrahlung>().release(), 	particle);
		}
		else if ( particleName == "anti_proton" )
		{
			ph->RegisterProcess(make_unique<G4hMultipleScattering>().release(), particle);
			ph->RegisterProcess(make_unique<G4hIonisation>().release(), 		particle);
			ph->RegisterProcess(make_unique<G4hBremsstrahlung>().release(), 	particle);
			ph->RegisterProcess(make_unique<G4hPairProduction>().release(), 	particle);
		}
	}

	unique_ptr<G4VAtomDeexcitation> deexcitation {make_unique<G4UAtomicDeexcitation>()};
	deexcitation->SetFluo(true);
	deexcitation->SetAuger(false);
	deexcitation->SetPIXE(false);
	G4LossTableManager::Instance()->SetAtomDeexcitation(deexcitation.release());
}
