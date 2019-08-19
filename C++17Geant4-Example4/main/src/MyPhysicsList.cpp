#include "MyPhysicsList.hh"
#include "G4ParticleTypes.hh"

void MyPhysicsList::ConstructParticle()
{
	G4Geantino::GeantinoDefinition();
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
	myPhysicsListEM->ConstructProcess(); // electromagnetic interactions
}

void MyPhysicsList::SetCuts()
{
	SetVerboseLevel(0);
	SetCutsWithDefault();
}
