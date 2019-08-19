#ifndef HDR_MYRUNACTION_HH_
#define HDR_MYRUNACTION_HH_

#include "G4UserRunAction.hh"

class G4Run;
class MyDetectorConstruction;
class MyPrimaryGeneratorAction;

using std::unique_ptr;

class MyRunAction final: public G4UserRunAction
{
public:
	MyRunAction(MyDetectorConstruction *myDetectorConstruction,
				MyPrimaryGeneratorAction *myPrimaryGeneratorAction);

	void EndOfRunAction(const G4Run* run) override;

private:
	void DisplayMaterialLabelsViaApplyCommand();

private:
	unique_ptr<MyDetectorConstruction> myDetectorConstruction;
	unique_ptr<MyPrimaryGeneratorAction> myPrimaryGeneratorAction;
};

#endif /* HDR_MYRUNACTION_HH_ */
