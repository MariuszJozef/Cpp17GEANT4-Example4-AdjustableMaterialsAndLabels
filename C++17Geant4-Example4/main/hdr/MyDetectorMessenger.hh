#ifndef HDR_MYDETECTORMESSENGER_HH_
#define HDR_MYDETECTORMESSENGER_HH_

#include "G4UImessenger.hh"
using std::unique_ptr;

class MyDetectorConstruction;
#include "G4UIcmdWithAString.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

class MyDetectorMessenger final: public G4UImessenger
{
public:
	MyDetectorMessenger(MyDetectorConstruction* myDetectorConstruction);
//	~MyDetectorMessenger();

	void SetNewValue(G4UIcommand* command, G4String newValue) override;
	G4String GetCurrentValue(G4UIcommand* command) override;

private:
	unique_ptr<MyDetectorConstruction> myDetectorConstruction {nullptr};
	unique_ptr<G4UIcmdWithAString> labMaterialUICommand {nullptr};
	unique_ptr<G4UIcmdWithAString> trapezoidMaterialUICommand {nullptr};

private:
	G4String& ConvertSynonymousMaterialNames(G4String& newValue);
};

#endif /* HDR_MYDETECTORMESSENGER_HH_ */
