#include "MyDetectorMessenger.hh"
#include "MyDetectorConstruction.hh"

using std::make_unique;

MyDetectorMessenger::MyDetectorMessenger(MyDetectorConstruction* myDetectorConstruction)
: G4UImessenger()
, myDetectorConstruction {unique_ptr<MyDetectorConstruction>(myDetectorConstruction)}
, labMaterialUICommand {make_unique<G4UIcmdWithAString>("/material/lab", this)}
, trapezoidMaterialUICommand {make_unique<G4UIcmdWithAString>("/material/trapezoid", this)}
, sphereMaterialUICommand {make_unique<G4UIcmdWithAString>("/material/sphere", this)}
, tetrahedronMaterialUICommand {make_unique<G4UIcmdWithAString>("/material/tetrahedron", this)}
, torusMaterialUICommand {make_unique<G4UIcmdWithAString>("/material/torus", this)}
, coneMaterialUICommand {make_unique<G4UIcmdWithAString>("/material/cone", this)}
{
	labMaterialUICommand->SetGuidance("Set lab material.");
	labMaterialUICommand->SetParameterName("newLabMaterial",false);
	labMaterialUICommand->AvailableForStates(G4State_PreInit,G4State_Idle);

	trapezoidMaterialUICommand->SetGuidance("Set trapezoid material.");
	trapezoidMaterialUICommand->SetParameterName("newTrapezoidMaterial",false);
	trapezoidMaterialUICommand->AvailableForStates(G4State_PreInit,G4State_Idle);

	sphereMaterialUICommand->SetGuidance("Set sphere material.");
	sphereMaterialUICommand->SetParameterName("newSphereMaterial",false);
	sphereMaterialUICommand->AvailableForStates(G4State_PreInit,G4State_Idle);

	tetrahedronMaterialUICommand->SetGuidance("Set tetrahedron material.");
	tetrahedronMaterialUICommand->SetParameterName("newTetrahedronMaterial",false);
	tetrahedronMaterialUICommand->AvailableForStates(G4State_PreInit,G4State_Idle);

	torusMaterialUICommand->SetGuidance("Set torus material.");
	torusMaterialUICommand->SetParameterName("newTorusMaterial",false);
	torusMaterialUICommand->AvailableForStates(G4State_PreInit,G4State_Idle);

	coneMaterialUICommand->SetGuidance("Set cone material.");
	coneMaterialUICommand->SetParameterName("newConeMaterial",false);
	coneMaterialUICommand->AvailableForStates(G4State_PreInit,G4State_Idle);
}

//MyDetectorMessenger::~MyDetectorMessenger() {}

void MyDetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
	newValue = ConvertSynonymousMaterialNames(newValue);
	if (command == labMaterialUICommand.get())
	{
		myDetectorConstruction->SetLabMaterial(newValue);
	}
	else if (command == trapezoidMaterialUICommand.get())
	{
		myDetectorConstruction->SetTrapezoidMaterial(newValue);
	}
	else if (command == sphereMaterialUICommand.get())
	{
		myDetectorConstruction->SetSphereMaterial(newValue);
	}
	else if (command == tetrahedronMaterialUICommand.get())
	{
		myDetectorConstruction->SetTetrahedronMaterial(newValue);
	}
	else if (command == torusMaterialUICommand.get())
	{
		myDetectorConstruction->SetTorusMaterial(newValue);
	}
	else if (command == coneMaterialUICommand.get())
	{
		myDetectorConstruction->SetConeMaterial(newValue);
	}
}

G4String MyDetectorMessenger::GetCurrentValue(G4UIcommand* command)
{
	G4String currentValue;

	if (command == labMaterialUICommand.get())
	{
		currentValue = myDetectorConstruction->GetLabMaterial();
	}
	else if (command == trapezoidMaterialUICommand.get())
	{
		currentValue = myDetectorConstruction->GetTrapezoidMaterial();
	}
	else if (command == sphereMaterialUICommand.get())
	{
		currentValue = myDetectorConstruction->GetSphereMaterial();
	}
	else if (command == tetrahedronMaterialUICommand.get())
	{
		currentValue = myDetectorConstruction->GetTetrahedronMaterial();
	}
	else if (command == torusMaterialUICommand.get())
	{
		currentValue = myDetectorConstruction->GetTorusMaterial();
	}
	else if (command == coneMaterialUICommand.get())
	{
		currentValue = myDetectorConstruction->GetConeMaterial();
	}

	return currentValue;
}

G4String& MyDetectorMessenger::ConvertSynonymousMaterialNames(G4String& newValue)
{
	if (newValue == "vacuum")
	{
		newValue = "G4_Galactic";
	}
	else if (newValue == "air")
	{
		newValue = "G4_AIR";
	}
	else if (newValue == "water")
	{
		newValue = "G4_WATER";
	}
	else if (newValue == "steam")
	{
		newValue = "G4_WATER_VAPOR";
	}
	else if (newValue == "concrete")
	{
		newValue = "G4_CONCRETE";
	}
	else if (newValue == "skin")
	{
		newValue = "G4_SKIN_ICRP";
	}
	else if (newValue == "soft tissue")
	{
		newValue = "G4_TISSUE_SOFT_ICRP";
	}
	else if (newValue == "muscle with sucrose")
	{
		newValue = "G4_MUSCLE_WITH_SUCROSE";
	}
	else if (newValue == "bone")
	{
		newValue = "G4_BONE_COMPACT_ICRU";
	}
	else if (newValue == "Pb")
	{
		newValue = "G4_Pb";
	}
	return newValue;
}
