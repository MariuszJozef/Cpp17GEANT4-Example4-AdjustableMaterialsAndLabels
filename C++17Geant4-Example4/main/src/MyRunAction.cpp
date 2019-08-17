#include "MyRunAction.hh"
#include "MyDetectorConstruction.hh"
#include "MyPrimaryGeneratorAction.hh"

#include "G4Run.hh"
#include "G4UImanager.hh"
#include <fstream>
//#include "G4RunManager.hh"


MyRunAction::MyRunAction(MyDetectorConstruction *myDetectorConstruction,
						 MyPrimaryGeneratorAction *myPrimaryGeneratorAction)
: G4UserRunAction()
, myDetectorConstruction {unique_ptr<MyDetectorConstruction>(myDetectorConstruction)}
, myPrimaryGeneratorAction {unique_ptr<MyPrimaryGeneratorAction>(myPrimaryGeneratorAction)}
{}

//void MyRunAction::BeginOfRunAction(const G4Run* run)
//{
//	myDetectorConstruction->DisplayMaterialLabelsViaMacroFile();
//}

void MyRunAction::EndOfRunAction(const G4Run* run)
{
	G4UImanager *uiManager = G4UImanager::GetUIpointer();
	uiManager->ApplyCommand("/vis/drawVolume");
	uiManager->ApplyCommand("/vis/scene/add/trajectories smooth");
	uiManager->ApplyCommand("/vis/scene/endOfEventAction accumulate");
	uiManager->ApplyCommand("/vis/set/textColour white");
	uiManager->ApplyCommand("/vis/scene/add/text -18 -18 -18 cm 10 0 0 "
			+ myDetectorConstruction->GetLabMaterial());
	uiManager->ApplyCommand("/vis/scene/add/text 0.5 5 0 cm 10 0 0 "
			+ myDetectorConstruction->GetTrapezoidMaterial());
	uiManager->ApplyCommand("/vis/scene/add/text 0 -1.5 0 cm 10 0 0 "
			+ myDetectorConstruction->GetSphereMaterial());
	uiManager->ApplyCommand("/vis/scene/add/text 4 2 1 cm 10 0 0 "
			+ myDetectorConstruction->GetTetrahedronMaterial());
	uiManager->ApplyCommand("/vis/scene/add/text 14 9 6 cm 10 0 0 "
			+ myDetectorConstruction->GetTorusMaterial());
	uiManager->ApplyCommand("/vis/scene/add/text -13 0 0 cm 10 0 0 "
			+ myDetectorConstruction->GetConeMaterial());

//	myDetectorConstruction->DisplayMaterialLabelsViaG4Text(); // labels become fuzzy ??
//	DisplayGunInfo();
//	DisplayDynamicLabels();
}

void MyRunAction::DisplayGunInfo()
{
	G4String fileName = "./vis-particleGunLabels.macro";

	std::ofstream outFile;
	outFile.open(fileName);
	// Create and write to the file if it doesn't exit, otherwise overwrite it
	if (outFile.is_open())
	{
		outFile << "#ParticleGun:" << G4endl;
		outFile << "/vis/set/textColour cyan" << G4endl;
		outFile << "/vis/scene/add/text2D -0.9 0.7 12 ! ! gun particle: "
				+ myPrimaryGeneratorAction->GetGunParticleName() << G4endl;
		outFile << "/vis/scene/add/text2D -0.9 0.6 12 ! ! gun energy: "
				+ myPrimaryGeneratorAction->GetGunEnergyAndUnit() << G4endl;

		outFile << "#Scale interval set to yellow colour:" << G4endl;
		outFile << "/vis/set/textColour yellow" << G4endl;
		outFile << "/vis/scene/add/scale" << G4endl;
//
		outFile.close();
//		G4cout << "Text written to file " << fileName << G4endl;

		G4UImanager *uiManager = G4UImanager::GetUIpointer();
		uiManager->ApplyCommand("/control/execute vis-reInit.macro");
//		uiManager->ApplyCommand("/control/execute vis-particleGunLabels.macro");
	}
	else
	{
		G4cerr << "Could not create file " << fileName << G4endl;
	}
}

void MyRunAction::DisplayDynamicLabels()
{
	G4String fileName = "./vis-dynamicLabels.macro";

	std::ofstream outFile;
	outFile.open(fileName);
	// Create and write to the file if it doesn't exit, otherwise overwrite it
	if (outFile.is_open())
	{
		outFile << "#ParticleGun:\n";
		outFile << "/vis/set/textColour cyan\n";
		outFile << "/vis/scene/add/text2D -0.9 0.7 12 ! ! gun particle: "
				+ myPrimaryGeneratorAction->GetGunParticleName() << G4endl;
		outFile << "/vis/scene/add/text2D -0.9 0.6 12 ! ! gun energy: "
				+ myPrimaryGeneratorAction->GetGunEnergyAndUnit() << G4endl;

		outFile << "#LabMaterial\n";
		outFile << "/vis/set/textColour white\n";
		outFile << "/vis/scene/add/text -18 -18 -18 cm 10 0 0 "
				<< myDetectorConstruction->GetLabMaterial() << G4endl;

		outFile << "#TrapezoidMaterial\n";
		outFile << "/vis/set/textColour white\n";
		outFile << "/vis/scene/add/text 0 4 0 cm 10 0 0 "
				<< myDetectorConstruction->GetTrapezoidMaterial() << G4endl;

		outFile << "#Scale interval set to yellow colour:\n";
		outFile << "/vis/set/textColour yellow\n";
		outFile << "/vis/scene/add/scale" << G4endl;
//
		outFile.close();
//		G4cout << "Text written to file " << fileName << G4endl;

		G4UImanager *uiManager = G4UImanager::GetUIpointer();
		uiManager->ApplyCommand("/control/execute vis-reInit.macro");
//		uiManager->ApplyCommand("/control/execute vis-dynamicLabels.macro");
//		uiManager->ApplyCommand("/control/execute vis-particleGunLabels.macro");
	}
	else
	{
		G4cerr << "Could not create file " << fileName << G4endl;
	}
}
