#ifndef HDR_MYPRIMARYGENERATORMESSENGER_HH_
#define HDR_MYPRIMARYGENERATORMESSENGER_HH_

#include "G4UImessenger.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcommand.hh"

class MyPrimaryGeneratorAction;

using std::unique_ptr;
using std::make_unique;

class MyPrimaryGeneratorMessenger final: public G4UImessenger
{
public:
	MyPrimaryGeneratorMessenger(MyPrimaryGeneratorAction* myPrimaryGeneratorAction);

	void SetNewValue(G4UIcommand* command, G4String newValue) override;
	G4String GetCurrentValue(G4UIcommand* command) override;

private:
	unique_ptr<MyPrimaryGeneratorAction> myPrimaryGeneratorAction {nullptr};
	unique_ptr<G4UIcmdWith3VectorAndUnit> gunPositionUICommand
		{make_unique<G4UIcmdWith3VectorAndUnit>("/gun/position", this)};
	unique_ptr<G4UIcmdWith3Vector> gunDirectionUICommand
		{make_unique<G4UIcmdWith3Vector>("/gun/direction", this)};
	unique_ptr<G4UIcmdWith3VectorAndUnit> gunDirectionAimAtPositionUICommand
		{make_unique<G4UIcmdWith3VectorAndUnit>("/gun/direction/aimAtPosition", this)};
	unique_ptr<G4UIcmdWithABool> beamDispersionOnUICommand
		{make_unique<G4UIcmdWithABool>("/beam/dispersionOn", this)};
	unique_ptr<G4UIcommand> beamRandomDistributionUICommand
		{make_unique<G4UIcommand>("/beam/randomDistribution", this)};
};

#endif /* HDR_MYPRIMARYGENERATORMESSENGER_HH_ */
