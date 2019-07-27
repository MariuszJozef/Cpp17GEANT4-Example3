#include "MyPhysicsListEM.hh"
#include "globals.hh"
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
//			ph->RegisterProcess(new G4PhotoElectricEffect(), particle);
//			ph->RegisterProcess(new G4ComptonScattering(),   particle);
//			ph->RegisterProcess(new G4GammaConversion(),     particle);
			ph->RegisterProcess(make_unique<G4PhotoElectricEffect>().get(),   particle);
			ph->RegisterProcess(make_unique<G4ComptonScattering>().release(), particle);
			ph->RegisterProcess(make_unique<G4GammaConversion>().release(),   particle);
		}
		else if (particleName == "opticalphoton")
		{
			// Rayleigh scattering, Thomson scattering ???
		}
		else if (particleName == "e-")
		{
//			ph->RegisterProcess(new G4eMultipleScattering(), particle);
//			ph->RegisterProcess(new G4eIonisation(),           particle);
//			ph->RegisterProcess(new G4eBremsstrahlung(),     particle);
			ph->RegisterProcess(make_unique<G4eMultipleScattering>().release(), particle);
			ph->RegisterProcess(make_unique<G4eIonisation>().release(), 		particle);
			ph->RegisterProcess(make_unique<G4eBremsstrahlung>().release(), 	particle);
		}
		else if (particleName == "e+")
		{
//			ph->RegisterProcess(new G4eMultipleScattering(), particle);
//			ph->RegisterProcess(new G4eIonisation(),           particle);
//			ph->RegisterProcess(new G4eBremsstrahlung(),     particle);
//			ph->RegisterProcess(new G4eplusAnnihilation(),   particle);
			ph->RegisterProcess(make_unique<G4eMultipleScattering>().release(), particle);
			ph->RegisterProcess(make_unique<G4eIonisation>().release(), 		particle);
			ph->RegisterProcess(make_unique<G4eBremsstrahlung>().release(), 	particle);
			ph->RegisterProcess(make_unique<G4eplusAnnihilation>().release(), 	particle);
		}
		else if ( particleName == "proton" )
		{
			ph->RegisterProcess(new G4hMultipleScattering(), particle);
			ph->RegisterProcess(new G4hIonisation(),         particle);
			ph->RegisterProcess(new G4hBremsstrahlung(),     particle);
//			ph->RegisterProcess(make_unique<G4hMultipleScattering>().get(), particle);
//			ph->RegisterProcess(make_unique<G4hIonisation>().get(), 		particle);
//			ph->RegisterProcess(make_unique<G4hBremsstrahlung>().get(), 	particle);
		}
		else if ( particleName == "anti_proton" )
		{
			ph->RegisterProcess(new G4hMultipleScattering(), particle);
			ph->RegisterProcess(new G4hIonisation(),         particle);
			ph->RegisterProcess(new G4hBremsstrahlung(),     particle);
			ph->RegisterProcess(new G4hPairProduction(),     particle);
//			ph->RegisterProcess(make_unique<G4hMultipleScattering>().get(), particle);
//			ph->RegisterProcess(make_unique<G4hIonisation>().get(), 		particle);
//			ph->RegisterProcess(make_unique<G4hBremsstrahlung>().get(), 	particle);
//			ph->RegisterProcess(make_unique<G4hPairProduction>().get(), 	particle);
		}
	}

	G4VAtomDeexcitation* de = new G4UAtomicDeexcitation();
	de->SetFluo(true);
	de->SetAuger(false);
	de->SetPIXE(false);
	G4LossTableManager::Instance()->SetAtomDeexcitation(de);
}
