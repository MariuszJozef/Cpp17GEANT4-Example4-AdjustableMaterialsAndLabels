#ifndef HDR_MYPRIMARYGENERATORMESSENGER_HH_
#define HDR_MYPRIMARYGENERATORMESSENGER_HH_

#include "G4UImessenger.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcommand.hh"

class MyPrimaryGeneratorAction;

using std::unique_ptr;

class MyPrimaryGeneratorMessenger final: public G4UImessenger
{
public:
	MyPrimaryGeneratorMessenger(MyPrimaryGeneratorAction* myPrimaryGeneratorAction);
//	~MyPrimaryGeneratorMessenger();

	void SetNewValue(G4UIcommand* command, G4String newValue) override;
	G4String GetCurrentValue(G4UIcommand* command) override;

private:
	unique_ptr<MyPrimaryGeneratorAction> myPrimaryGeneratorAction {nullptr};
	unique_ptr<G4UIcmdWith3VectorAndUnit> gunPositionUICommand {nullptr};
	unique_ptr<G4UIcmdWith3Vector> gunDirectionUICommand {nullptr};
	unique_ptr<G4UIcmdWith3VectorAndUnit> gunDirectionAimAtPositionUICommand {nullptr};
	unique_ptr<G4UIcmdWithABool> beamDispersionOnUICommand {nullptr};
	unique_ptr<G4UIcommand> beamRandomDistributionUICommand {nullptr};
};

#endif /* HDR_MYPRIMARYGENERATORMESSENGER_HH_ */
