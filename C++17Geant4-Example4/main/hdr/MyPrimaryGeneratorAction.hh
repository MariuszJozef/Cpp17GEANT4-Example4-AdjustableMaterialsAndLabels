#ifndef MYPRIMARYGENERATORACTION_HH_
#define MYPRIMARYGENERATORACTION_HH_

#include "G4VUserPrimaryGeneratorAction.hh"
#include "MyPrimaryGeneratorMessenger.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"

class G4ParticleDefinition;
class G4Event;

using std::unique_ptr;
using std::make_unique;

class MyPrimaryGeneratorAction final: public G4VUserPrimaryGeneratorAction
{
public:
	MyPrimaryGeneratorAction(const G4ThreeVector& halfLabSize);

	void GeneratePrimaries(G4Event* anEvent) override;

	G4ThreeVector GetGunPosition() const { return gunPosition; }
	void SetGunPosition(const G4ThreeVector& gunPosition);

	const G4ThreeVector& GetGunDirection() const { return gunDirection; }
	void SetGunDirection(const G4ThreeVector& gunDirection);
	void SetGunDirectionAimAtPosition(const G4ThreeVector& aimedPosition);

	const G4String& GetGunParticleName() const { return gunParticleName; }
	const G4String& GetGunEnergyAndUnit() const { return gunEnergyAndUnit; }

	G4bool IsBeamDispersionOn() const { return beamDispersionOn; }
	void SetBeamDispersionOn(G4bool beamDispersionOn) {
		this->beamDispersionOn = beamDispersionOn;
	}

	G4String GetRandomDistribution() const { return randomDistribution; }
	G4double GetAmplitudeOrSigma() const { return amplitudeOrSigma; }
	void SetBeamDistribution(const G4String& randomDistribution, G4double amplitudeOrSigma);

private:
	G4bool IsGunInsideLab(const G4ThreeVector& gunPosition) const;
	const G4ThreeVector& ObtainUnitThreeVector(const G4ThreeVector& aThreeVector);
	const G4ThreeVector& RandomiseDirection(const G4ThreeVector& gunDirection);

private:
	G4ThreeVector halfLabSize;
	G4ThreeVector gunPosition;
	unique_ptr<MyPrimaryGeneratorMessenger> myPrimaryGeneratorMessenger
		{make_unique<MyPrimaryGeneratorMessenger>(this)};
	unique_ptr<G4ParticleGun> particleGun {make_unique<G4ParticleGun>(1)};
	G4double gunEnergy {25*MeV};
	G4ThreeVector gunDirection {G4ThreeVector(0, 0, 1)};
	G4bool beamDispersionOn {true};
	G4String randomDistribution {"uniform"};
	G4double amplitudeOrSigma {0.2};

	G4String gunParticleName {};
	G4String gunEnergyAndUnit {};

	G4ParticleDefinition *electron {nullptr};
	G4ParticleDefinition *positron {nullptr};
	G4ParticleDefinition *gamma {nullptr};
	G4ParticleDefinition *proton {nullptr};
	G4ParticleDefinition *antiProton {nullptr};
};

#endif /* MYPRIMARYGENERATORACTION_HH_ */
