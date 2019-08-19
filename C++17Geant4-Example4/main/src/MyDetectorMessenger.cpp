#include "MyDetectorMessenger.hh"
#include "MyDetectorConstruction.hh"

MyDetectorMessenger::MyDetectorMessenger(MyDetectorConstruction* myDetectorConstruction)
: G4UImessenger()
, myDetectorConstruction {unique_ptr<MyDetectorConstruction>(myDetectorConstruction)}
{
	labMaterialUICommand->SetGuidance("Set lab material");
	labMaterialUICommand->SetParameterName("newLabMaterial",false);
	labMaterialUICommand->AvailableForStates(G4State_PreInit,G4State_Idle);

	trapezoidMaterialUICommand->SetGuidance("Set trapezoid material");
	trapezoidMaterialUICommand->SetParameterName("newTrapezoidMaterial",false);
	trapezoidMaterialUICommand->AvailableForStates(G4State_PreInit,G4State_Idle);

	sphereMaterialUICommand->SetGuidance("Set sphere material");
	sphereMaterialUICommand->SetParameterName("newSphereMaterial",false);
	sphereMaterialUICommand->AvailableForStates(G4State_PreInit,G4State_Idle);

	tetrahedronMaterialUICommand->SetGuidance("Set tetrahedron material");
	tetrahedronMaterialUICommand->SetParameterName("newTetrahedronMaterial",false);
	tetrahedronMaterialUICommand->AvailableForStates(G4State_PreInit,G4State_Idle);

	torusMaterialUICommand->SetGuidance("Set torus material");
	torusMaterialUICommand->SetParameterName("newTorusMaterial",false);
	torusMaterialUICommand->AvailableForStates(G4State_PreInit,G4State_Idle);

	coneMaterialUICommand->SetGuidance("Set cone material");
	coneMaterialUICommand->SetParameterName("newConeMaterial",false);
	coneMaterialUICommand->AvailableForStates(G4State_PreInit,G4State_Idle);

	activeRotationAxisAngleUICommand->SetGuidance(
				"Set rotation axis and angle: x, y, z, angle, angularUnit");

//	G4UIparameter *activeRotationAngleAboutXaxis
	G4UIparameter *activeRotationAngleAboutXaxis
		= new G4UIparameter("rotationAngleAboutXaxis", 'd', false);
	activeRotationAngleAboutXaxis->SetGuidance("rotation angle about x axis");
	activeRotationAngleAboutXaxis->SetParameterName("rotationAngleAboutXaxis"); // redundant?
	activeRotationAxisAngleUICommand->SetParameter(activeRotationAngleAboutXaxis);

	G4UIparameter *activeRotationAngleAboutYaxis
		= new G4UIparameter("rotationAngleAboutYaxis", 'd', false);
	activeRotationAngleAboutYaxis->SetGuidance("rotation angle about y axis");
	activeRotationAngleAboutYaxis->SetParameterName("rotationAngleAboutYaxis"); // redundant?
	activeRotationAxisAngleUICommand->SetParameter(activeRotationAngleAboutYaxis);

	G4UIparameter *activeRotationAngleAboutZaxis
		= new G4UIparameter("rotationAngleAboutZaxis", 'd', false);
	activeRotationAngleAboutZaxis->SetGuidance("rotation angle about z axis");
	activeRotationAngleAboutZaxis->SetParameterName("rotationAngleAboutZaxis"); // redundant?
	activeRotationAxisAngleUICommand->SetParameter(activeRotationAngleAboutZaxis);

	G4UIparameter *activeRotationAngleUnit = new G4UIparameter("rotationAngleUnit", 's',
				false);
	activeRotationAngleUnit->SetGuidance("Unit of rotation angle: deg or rad");
	// activeRotationAngleUnit->SetParameterName("rotationAngleUnit");
	G4String angleUnitList = G4UIcommand::UnitsList(G4UIcommand::CategoryOf("deg"));
	angleUnitList += " ";
	angleUnitList += G4UIcommand::UnitsList(G4UIcommand::CategoryOf("rad"));
	activeRotationAngleUnit->SetParameterCandidates(angleUnitList);
	//	activeRotationAngleUnit->SetDefaultUnit("deg"); // doesn't exist
	activeRotationAxisAngleUICommand->SetParameter(activeRotationAngleUnit);

	activeRotationAxisAngleUICommand->AvailableForStates(G4State_PreInit, G4State_Idle);
	activeRotationAxisAngleUICommand->SetToBeBroadcasted(false);
}

MyDetectorMessenger::~MyDetectorMessenger()
{
	delete activeRotationAxisAngleUICommand;
}

void MyDetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
	newValue = ConvertToSynonymousMaterialName(newValue);
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
	else if (command == activeRotationAxisAngleUICommand)
	{
		G4double rotationAngleX, rotationAngleY, rotationAngleZ;
		G4String rotationAngleUnit;

		std::istringstream is(newValue);
		is >> rotationAngleX >> rotationAngleY >> rotationAngleZ >> rotationAngleUnit;

		rotationAngleX *= G4UIcommand::ValueOf(rotationAngleUnit);
		rotationAngleY *= G4UIcommand::ValueOf(rotationAngleUnit);
		rotationAngleZ *= G4UIcommand::ValueOf(rotationAngleUnit);

		myDetectorConstruction->SetActiveRotationAngles(rotationAngleX,
														rotationAngleY, rotationAngleZ);
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

G4String& MyDetectorMessenger::ConvertToSynonymousMaterialName(G4String& newValue)
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
