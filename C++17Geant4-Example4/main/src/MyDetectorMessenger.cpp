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
//, activeRotationAxisAngleUICommand {make_unique<G4UIcommand>("/rotate/cone", this)}
, activeRotationAxisAngleUICommand {new G4UIcommand("/target/rotationAxisAngle", this)}
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

	// START OF MY CUSTOM COMMAND
	activeRotationAxisAngleUICommand->SetGuidance("Set rotation axis and angle: x, y, z, angle, angularUnit");

//	unique_ptr<G4UIparameter> activeRotationAxisComponentX
//			{ make_unique<G4UIparameter>("rotationAxisComponentX", 'd', false) };
//	activeRotationAxisComponentX->SetGuidance("X component of rotation axis");
//	//	Is this next line redundant?
//	activeRotationAxisComponentX->SetParameterName("rotationAxisComponentX");
//	activeRotationAxisAngleUICommand->SetParameter(activeRotationAxisComponentX.release());
//
//	unique_ptr<G4UIparameter> activeRotationAxisComponentY
//			{ make_unique<G4UIparameter>("rotationAxisComponentY", 'd', false) };
//	activeRotationAxisComponentY->SetGuidance("Y component of rotation axis");
//	activeRotationAxisComponentY->SetParameterName("rotationAxisComponentY");
//	activeRotationAxisAngleUICommand->SetParameter(activeRotationAxisComponentY.release());
//
//	unique_ptr<G4UIparameter> activeRotationAxisComponentZ
//			{ make_unique<G4UIparameter>("rotationAxisComponentZ", 'd', false) };
//	activeRotationAxisComponentZ->SetGuidance("Z component of rotation axis");
//	activeRotationAxisComponentZ->SetParameterName("rotationAxisComponentZ");
//	activeRotationAxisAngleUICommand->SetParameter(activeRotationAxisComponentZ.release());
//
//	unique_ptr<G4UIparameter> activeRotationAngle
//		{make_unique<G4UIparameter>("rotationAngle", 'd', false)};
//	activeRotationAngle->SetGuidance("Angle of rotation about the rotation axis");
//	activeRotationAngle->SetParameterName("rotationAngle");
//	activeRotationAxisAngleUICommand->SetParameter(activeRotationAngle.release());
//
//	unique_ptr<G4UIparameter> activeRotationAngleUnit
//		{make_unique<G4UIparameter>("rotationAngleUnit", 's', false)};
//	activeRotationAngleUnit->SetGuidance("Unit of rotation angle: deg or rad");
//	// activeRotationAngleUnit->SetParameterName("rotationAngleUnit");
//	G4String angleUnitList = G4UIcommand::UnitsList(G4UIcommand::CategoryOf("deg"));
//	angleUnitList += " ";
//	angleUnitList += G4UIcommand::UnitsList(G4UIcommand::CategoryOf("rad"));
//	activeRotationAngleUnit->SetParameterCandidates(angleUnitList);
//	//	activeRotationAngleUnit->SetDefaultUnit("deg"); // doesn't exist
//	activeRotationAxisAngleUICommand->SetParameter(activeRotationAngleUnit.release());
//
//	activeRotationAxisAngleUICommand->AvailableForStates(G4State_PreInit, G4State_Idle);
//	activeRotationAxisAngleUICommand->SetToBeBroadcasted(false);
	// END OF MY CUSTOM COMMAND

	G4UIparameter *setRotationXaxisPrm = new G4UIparameter("rotationAxisComponentX", 'd',
				false);
	setRotationXaxisPrm->SetGuidance("X component of rotation axis");
	//	Is this next line redundant?
	setRotationXaxisPrm->SetParameterName("rotationAxisComponentX");
	activeRotationAxisAngleUICommand->SetParameter(setRotationXaxisPrm);

	G4UIparameter *setRotationYaxisPrm = new G4UIparameter("rotationAxisComponentY", 'd',
			false);
	setRotationYaxisPrm->SetGuidance("Y component of rotation axis");
	setRotationYaxisPrm->SetParameterName("rotationAxisComponentY");
	activeRotationAxisAngleUICommand->SetParameter(setRotationYaxisPrm);

	G4UIparameter *setRotationZaxisPrm = new G4UIparameter("rotationAxisComponentZ", 'd',
			false);
	setRotationZaxisPrm->SetGuidance("Z component of rotation axis");
	setRotationZaxisPrm->SetParameterName("rotationAxisComponentZ");
	activeRotationAxisAngleUICommand->SetParameter(setRotationZaxisPrm);

	G4UIparameter *setRotationAnglePrm = new G4UIparameter("rotationAngle", 'd', false);
	setRotationAnglePrm->SetGuidance("Angle of rotation about the rotation axis");
	setRotationAnglePrm->SetParameterName("rotationAngle");
	activeRotationAxisAngleUICommand->SetParameter(setRotationAnglePrm);

	G4UIparameter *setUnitOfRotationAnglePrm = new G4UIparameter("rotationAngleUnit", 's',
			false);
	setUnitOfRotationAnglePrm->SetGuidance("Unit of rotation angle: deg or rad");
	// setUnitOfRotationAnglePrm->SetParameterName("rotationAngleUnit");
	G4String angleUnitList = G4UIcommand::UnitsList(G4UIcommand::CategoryOf("deg"));
	angleUnitList += " ";
	angleUnitList += G4UIcommand::UnitsList(G4UIcommand::CategoryOf("rad"));
	setUnitOfRotationAnglePrm->SetParameterCandidates(angleUnitList);
	//	setUnitOfRotationAnglePrm->SetDefaultUnit("deg"); // doesn't exist
	activeRotationAxisAngleUICommand->SetParameter(setUnitOfRotationAnglePrm);

	activeRotationAxisAngleUICommand->AvailableForStates(G4State_PreInit,
			G4State_Idle);
	activeRotationAxisAngleUICommand->SetToBeBroadcasted(false);
}

MyDetectorMessenger::~MyDetectorMessenger()
{
	delete activeRotationAxisAngleUICommand;
}

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
	else if (command == activeRotationAxisAngleUICommand)
	{
		G4double rotationAxisX, rotationAxisY, rotationAxisZ, rotationAngle;
		G4String rotationAngleUnit;

		std::istringstream is(newValue);
		is >> rotationAxisX >> rotationAxisY >> rotationAxisZ >> rotationAngle >> rotationAngleUnit;

		G4ThreeVector rotationAxis = G4ThreeVector(rotationAxisX, rotationAxisY, rotationAxisZ);
		rotationAngle *= G4UIcommand::ValueOf(rotationAngleUnit);

		myDetectorConstruction->SetActiveRotationAxisAngle(rotationAxis, rotationAngle);
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
	else if (command == activeRotationAxisAngleUICommand)
	{
//		Display rotationAxis with opposite sign to what is used internally for calculating (passive)
//		G4RotationMatrix because user inputs active (not passive) rotations; output should coincide
//		with input.
		G4ThreeVector rotationAxis = -myDetectorConstruction->GetRotationAxis();
		G4double rotationAngle = myDetectorConstruction->GetRotationAngle();
		currentValue = activeRotationAxisAngleUICommand->ConvertToString(rotationAxis);
		currentValue += " ";
		currentValue += activeRotationAxisAngleUICommand->ConvertToString(rotationAngle, "deg");
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
