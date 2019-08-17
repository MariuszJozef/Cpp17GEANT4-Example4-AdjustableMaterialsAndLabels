#include "MyPhysicsList.hh"
#include "MyPhysicsListEM.hh"
#include "G4ParticleTypes.hh"

MyPhysicsList::MyPhysicsList()
: G4VModularPhysicsList()
, myPhysicsListEM(std::make_unique<MyPhysicsListEM>())
{
	SetCutsWithDefault();
	SetVerboseLevel(0);
}

//MyPhysicsList::~MyPhysicsList() {}

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
	AddTransportation();
	myPhysicsListEM->ConstructProcess(); // electromagnetic interactions
}

void MyPhysicsList::SetCuts()
{
//	G4int temp = GetVerboseLevel();
	SetVerboseLevel(0);
	SetCutsWithDefault();
//	SetVerboseLevel(temp);
}