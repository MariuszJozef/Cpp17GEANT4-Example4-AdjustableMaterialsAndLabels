#ifndef HDR_MYDETECTORMESSENGER_HH_
#define HDR_MYDETECTORMESSENGER_HH_

#include "G4UImessenger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

class MyDetectorConstruction;

using std::unique_ptr;
using std::make_unique;

class MyDetectorMessenger final: public G4UImessenger
{
public:
	MyDetectorMessenger(MyDetectorConstruction* myDetectorConstruction);

	void SetNewValue(G4UIcommand* command, G4String newValue) override;
	G4String GetCurrentValue(G4UIcommand* command) override;

private:
	unique_ptr<MyDetectorConstruction> myDetectorConstruction {nullptr};
	unique_ptr<G4UIcmdWithAString> labMaterialUICommand
		{make_unique<G4UIcmdWithAString>("/material/lab", this)};
	unique_ptr<G4UIcmdWithAString> trapezoidMaterialUICommand
		{make_unique<G4UIcmdWithAString>("/material/trapezoid", this)};
	unique_ptr<G4UIcmdWithAString> sphereMaterialUICommand
		{make_unique<G4UIcmdWithAString>("/material/sphere", this)};
	unique_ptr<G4UIcmdWithAString> tetrahedronMaterialUICommand
		{make_unique<G4UIcmdWithAString>("/material/tetrahedron", this)};
	unique_ptr<G4UIcmdWithAString> torusMaterialUICommand
		{make_unique<G4UIcmdWithAString>("/material/torus", this)};
	unique_ptr<G4UIcmdWithAString>
		coneMaterialUICommand {make_unique<G4UIcmdWithAString>("/material/cone", this)};

private:
	G4String& ConvertToSynonymousMaterialName(G4String& newValue);
};

#endif /* HDR_MYDETECTORMESSENGER_HH_ */
