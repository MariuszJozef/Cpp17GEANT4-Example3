#include "MyPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

MyPrimaryGeneratorAction::MyPrimaryGeneratorAction()
:
particleGun{std::make_unique<G4ParticleGun>(1)}
{
	G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
	G4String particleName;

	geantino = particleTable->FindParticle(particleName="geantino");
	electron = particleTable->FindParticle(particleName="e-");
	positron = particleTable->FindParticle(particleName="e+");
	gamma = particleTable->FindParticle(particleName="gamma");
	proton = particleTable->FindParticle(particleName="proton");
}

MyPrimaryGeneratorAction::~MyPrimaryGeneratorAction()
{
//	Must not explicitly delete particleGun as it is a unique_ptr
}

void MyPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
	G4ParticleDefinition* particle {electron};
	particleGun->SetParticleDefinition(particle);

	particleGun->SetParticleMomentumDirection(
					G4ThreeVector(1, G4UniformRand() - 0.5, G4UniformRand() - 0.5) );
	particleGun->SetParticleEnergy(15*MeV);
	// Be careful not to place gun outside of lab valume, cf. MyDetectorConstruction
	particleGun->SetParticlePosition(G4ThreeVector(-5*cm, 0, 0));

	particleGun->GeneratePrimaryVertex(anEvent);
}






