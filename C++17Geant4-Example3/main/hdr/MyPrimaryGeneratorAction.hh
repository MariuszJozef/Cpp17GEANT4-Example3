#ifndef MYPRIMARYGENERATORACTION_HH_
#define MYPRIMARYGENERATORACTION_HH_

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"
#include <memory>

class G4ParticleGun;
class G4ParticleDefinition;
class G4Event;

class MyPrimaryGeneratorAction: public G4VUserPrimaryGeneratorAction
{
public:
	MyPrimaryGeneratorAction(G4ThreeVector halfLabSize);
	~MyPrimaryGeneratorAction();

public:
	void GeneratePrimaries(G4Event* anEvent);

private:
	std::unique_ptr<G4ParticleGun> particleGun {nullptr};
	G4ParticleDefinition *geantino {nullptr}; // smart pointers not required
	G4ParticleDefinition *electron {nullptr};
	G4ParticleDefinition *positron {nullptr};
	G4ParticleDefinition *opticalphoton {nullptr};
	G4ParticleDefinition *gamma {nullptr};
	G4ParticleDefinition *proton {nullptr};
	G4ParticleDefinition *antiproton {nullptr};

	G4double gunEnergy;
	G4ThreeVector gunPosition, halfLabSize;
};

#endif /* MYPRIMARYGENERATORACTION_HH_ */
