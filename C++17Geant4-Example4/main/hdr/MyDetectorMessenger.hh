#ifndef HDR_MYDETECTORMESSENGER_HH_
#define HDR_MYDETECTORMESSENGER_HH_

#include "G4UImessenger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcommand.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

class MyDetectorConstruction;

using std::unique_ptr;

class MyDetectorMessenger final: public G4UImessenger
{
public:
	MyDetectorMessenger(MyDetectorConstruction* myDetectorConstruction);
	~MyDetectorMessenger();

	void SetNewValue(G4UIcommand* command, G4String newValue) override;
	G4String GetCurrentValue(G4UIcommand* command) override;

private:
	unique_ptr<MyDetectorConstruction> myDetectorConstruction {nullptr};
	unique_ptr<G4UIcmdWithAString> labMaterialUICommand {nullptr};
	unique_ptr<G4UIcmdWithAString> trapezoidMaterialUICommand {nullptr};
	unique_ptr<G4UIcmdWithAString> sphereMaterialUICommand {nullptr};
	unique_ptr<G4UIcmdWithAString> tetrahedronMaterialUICommand {nullptr};
	unique_ptr<G4UIcmdWithAString> torusMaterialUICommand {nullptr};
	unique_ptr<G4UIcmdWithAString> coneMaterialUICommand {nullptr};
//	unique_ptr<G4UIcommand> activeRotationAxisAngleUICommand {nullptr};
	G4UIcommand *activeRotationAxisAngleUICommand {nullptr};

private:
	G4String& ConvertSynonymousMaterialNames(G4String& newValue);
};

#endif /* HDR_MYDETECTORMESSENGER_HH_ */
