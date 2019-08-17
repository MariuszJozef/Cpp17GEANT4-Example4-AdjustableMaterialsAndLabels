#ifndef HDR_MYPHYSICSLISTEM_HH_
#define HDR_MYPHYSICSLISTEM_HH_

#include "G4VPhysicsConstructor.hh"

class MyPhysicsListEM final: public G4VPhysicsConstructor
{
public:
	void ConstructParticle() override {}; // implemented in MyPhysicsList.cpp
	void ConstructProcess() override;

protected:
	void ConstructEM();
};

#endif /* HDR_MYPHYSICSLISTEM_HH_ */
