#ifndef HDR_MYPHYSICSLISTEM_HH_
#define HDR_MYPHYSICSLISTEM_HH_

#include "G4VPhysicsConstructor.hh"

class MyPhysicsListEM: public G4VPhysicsConstructor
{
public:
	MyPhysicsListEM();
	virtual ~MyPhysicsListEM();

public: // NOT protected as needs to be accessed by MyPhysicsList.cc
//	Dummy method; it's already implemented in MyPhysicsList.cc
	virtual void ConstructParticle() override {};
	virtual void ConstructProcess() override;

protected:
	virtual void ConstructEM();
};

#endif /* HDR_MYPHYSICSLISTEM_HH_ */
