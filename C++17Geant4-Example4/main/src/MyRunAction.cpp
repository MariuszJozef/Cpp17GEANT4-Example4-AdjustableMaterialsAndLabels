#include "MyRunAction.hh"
#include "MyDetectorConstruction.hh"
#include "MyPrimaryGeneratorAction.hh"

#include "G4Run.hh"
#include "G4UImanager.hh"

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

	uiManager->ApplyCommand("/vis/set/textColour cyan");
	uiManager->ApplyCommand("/vis/scene/add/text2D -0.9 0.7 12 ! ! gun particle: "
			+ myPrimaryGeneratorAction->GetGunParticleName());
	uiManager->ApplyCommand("/vis/scene/add/text2D -0.9 0.6 12 ! ! gun particle: "
			+ myPrimaryGeneratorAction->GetGunEnergyAndUnit());

}
