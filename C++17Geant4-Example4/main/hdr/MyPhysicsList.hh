#ifndef MYPHYSICSLIST_HH_
#define MYPHYSICSLIST_HH_

#include "G4VModularPhysicsList.hh"
#include "MyPhysicsListEM.hh"

class G4VPhysicsConstructor;

using std::unique_ptr;
using std::make_unique;

class MyPhysicsList final: public G4VModularPhysicsList
{
protected:
	void ConstructParticle() override;
	void ConstructProcess() override;
	void SetCuts() override;

private:
	unique_ptr<G4VPhysicsConstructor> myPhysicsListEM {make_unique<MyPhysicsListEM>()};
};

#endif /* MYPHYSICSLIST_HH_ */
