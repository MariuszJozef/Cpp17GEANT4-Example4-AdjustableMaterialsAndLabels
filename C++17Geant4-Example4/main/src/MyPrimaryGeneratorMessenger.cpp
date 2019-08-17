#include "MyPrimaryGeneratorMessenger.hh"
#include "MyPrimaryGeneratorAction.hh"

#include "G4PhysicalConstants.hh"

using std::make_unique;

MyPrimaryGeneratorMessenger::MyPrimaryGeneratorMessenger(
		MyPrimaryGeneratorAction *myPrimaryGeneratorAction)
: G4UImessenger()
, myPrimaryGeneratorAction
	{unique_ptr<MyPrimaryGeneratorAction>(myPrimaryGeneratorAction)}
, gunPositionUICommand {make_unique<G4UIcmdWith3VectorAndUnit>("/gun/position", this)}
, gunDirectionUICommand {make_unique<G4UIcmdWith3Vector>("/gun/direction", this)}
, gunDirectionAimAtPositionUICommand
	{make_unique<G4UIcmdWith3VectorAndUnit>("/gun/direction/aimAtPosition", this)}
, beamDispersionOnUICommand {make_unique<G4UIcmdWithABool>("/beam/dispersionOn", this)}
, beamRandomDistributionUICommand {make_unique<G4UIcommand>("/beam/randomDistribution", this)}
{
	gunPositionUICommand->SetGuidance("Set gun position");
	gunPositionUICommand->SetParameterName("x", "y", "z", false);
	gunPositionUICommand->SetDefaultUnit("cm");
	gunPositionUICommand->AvailableForStates(G4State_PreInit, G4State_Idle);

	gunDirectionUICommand->SetGuidance("Set gun direction, doesn't need to be a unit vector.");
	gunDirectionUICommand->SetGuidance("Input 0 0 0 implies aim gun at origin from current position.");
	gunDirectionUICommand->SetParameterName("x", "y", "z", false);
	gunDirectionUICommand->AvailableForStates(G4State_PreInit, G4State_Idle);

	gunDirectionAimAtPositionUICommand->SetGuidance("Set gun direction by specifying target position (with units!)");
	gunDirectionAimAtPositionUICommand->SetParameterName("x", "y", "z", false);
	gunDirectionAimAtPositionUICommand->SetDefaultUnit("cm");
	gunDirectionAimAtPositionUICommand->AvailableForStates(G4State_PreInit, G4State_Idle);

	beamDispersionOnUICommand->SetGuidance("True or false");
	beamDispersionOnUICommand->SetParameterName("trueOrFalse", false);
	beamDispersionOnUICommand->SetDefaultValue(true);
	beamDispersionOnUICommand->AvailableForStates(G4State_PreInit, G4State_Idle);

	beamRandomDistributionUICommand->SetGuidance("on or off u or g (uniform or Gaussian) amplitude or sigma");

//	G4UIparameter *beamRandomDistributionType = new G4UIparameter("RandomDistribution", 's', false);
	unique_ptr<G4UIparameter> beamRandomDistributionType
		{ make_unique<G4UIparameter>("RandomDistribution", 's', false) };
	beamRandomDistributionType->SetGuidance("u for uniform or g for Gaussian distribution");
	beamRandomDistributionType->SetParameterName("distribution");
	beamRandomDistributionType->SetDefaultValue("u");
	beamRandomDistributionUICommand->SetParameter(beamRandomDistributionType.release());

//	G4UIparameter *beamRandomDistributionAmplitudeOrSigma = new G4UIparameter("RandomDistribution", 'd', false);
	unique_ptr<G4UIparameter> beamRandomDistributionAmplitudeOrSigma
		{ make_unique<G4UIparameter>("RandomDistribution", 'd', false) };
	beamRandomDistributionAmplitudeOrSigma->SetGuidance("amplitude for uniform distribution or sigma for Gaussian");
	beamRandomDistributionAmplitudeOrSigma->SetParameterName("amplitudeOrSigma");
	beamRandomDistributionAmplitudeOrSigma->SetDefaultValue(0.2);
	beamRandomDistributionUICommand->SetParameter(beamRandomDistributionAmplitudeOrSigma.release());

	beamRandomDistributionUICommand->AvailableForStates(G4State_PreInit,G4State_Idle);
	beamRandomDistributionUICommand->SetToBeBroadcasted(false);
}

//MyPrimaryGeneratorMessenger::~MyPrimaryGeneratorMessenger() {}

void MyPrimaryGeneratorMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
	if( command == gunPositionUICommand.get() )
	{
		myPrimaryGeneratorAction->SetGunPosition(
				gunPositionUICommand->GetNew3VectorValue(newValue));
//		For convenience point gun at origin after changing the position.
//		Can override this direction by issuing UI command
//		/gun/direction ... OR /gun/direction/atAtPosition ...
//		myPrimaryGeneratorAction->SetGunDirection(G4ThreeVector());
	}
	else if( command == gunDirectionUICommand.get() )
	{
		myPrimaryGeneratorAction->SetGunDirection(
				gunDirectionUICommand->GetNew3VectorValue(newValue));
	}
	else if( command == gunDirectionAimAtPositionUICommand.get() )
	{
		myPrimaryGeneratorAction->SetGunDirectionAimAtPosition(
				gunDirectionAimAtPositionUICommand->GetNew3VectorValue(newValue));
	}
	else if( command == beamDispersionOnUICommand.get() )
	{
		myPrimaryGeneratorAction->SetBeamDispersionOn(
				beamDispersionOnUICommand->GetNewBoolValue(newValue));
	}
	else if( command == beamRandomDistributionUICommand.get() )
	{
		G4String randomDistribution;
		G4double amplitudeOrSigma;

		std::istringstream is(newValue);
		is >> randomDistribution >> amplitudeOrSigma;

		if (randomDistribution == "u" || randomDistribution == "uniform" ||
			randomDistribution == "g" || randomDistribution == "gaussian" ||
			randomDistribution == "Gaussian")
		{
			myPrimaryGeneratorAction->SetBeamDistribution(randomDistribution, amplitudeOrSigma);
		}
		else
		{
			G4cerr << "Undefined random distribution, possible choices are:"
				   << "'u' or 'uniform' and 'g' or 'gaussian' or 'Gaussian'" << G4endl;
		}
	}
}

G4String MyPrimaryGeneratorMessenger::GetCurrentValue(G4UIcommand *command)
{
	G4String currentValue;

	if ( command == gunPositionUICommand.get() )
	{
		G4ThreeVector gunPosition = myPrimaryGeneratorAction->GetGunPosition();
		currentValue = gunPositionUICommand->ConvertToString(gunPosition, "cm");
	}
	else if ( command == gunDirectionUICommand.get() )
	{
		currentValue = gunDirectionUICommand->ConvertToString(
							myPrimaryGeneratorAction->GetGunDirection());
	}
	else if ( command == gunDirectionAimAtPositionUICommand.get() )
	{
		currentValue = gunDirectionAimAtPositionUICommand->ConvertToString(
							myPrimaryGeneratorAction->GetGunDirection());
	}
	else if( command == beamDispersionOnUICommand.get() )
	{
		currentValue = myPrimaryGeneratorAction->IsBeamDispersionOn()
					? "true" : "false";
	}
	else if ( command == beamRandomDistributionUICommand.get() )
	{
		G4String randomDistribution = myPrimaryGeneratorAction->GetRandomDistribution();
		G4double amplitudeOrSigma = myPrimaryGeneratorAction->GetAmplitudeOrSigma();

		if (randomDistribution == "u")
			randomDistribution = "uniform";
		else if (randomDistribution == "g")
			randomDistribution = "gaussian";

		currentValue = randomDistribution;
		currentValue += ", amplitudeOrSigma = ";
		currentValue += beamRandomDistributionUICommand->ConvertToString(amplitudeOrSigma);
	}

	return currentValue;
}
