#ifndef HDR_MYDETECTORCONSTRUCTION_HH_
#define HDR_MYDETECTORCONSTRUCTION_HH_

#include "G4VUserDetectorConstruction.hh"
#include "MyDetectorMessenger.hh"

#include "G4Material.hh"
#include "G4Trd.hh"
#include "G4Sphere.hh"
#include "G4Tet.hh"
#include "G4Torus.hh"
#include "G4Cons.hh"

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

class G4Box;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4VisAttributes;

using std::unique_ptr;
using std::make_unique;

class MyDetectorConstruction final: public G4VUserDetectorConstruction
{
public:
	MyDetectorConstruction();

	G4VPhysicalVolume* Construct() override;

	G4ThreeVector GetHalfLabSize() const { return halfLabSize; }

	G4String GetLabMaterial() const { return labMaterial->GetName(); }
	void SetLabMaterial(const G4String& newMaterialName);

	G4String GetTrapezoidMaterial() const { return trapezoidMaterial->GetName(); }
	void SetTrapezoidMaterial(const G4String& newMaterialName);

	G4String GetSphereMaterial() const { return sphereMaterial->GetName(); }
	void SetSphereMaterial(const G4String& newMaterialName);

	G4String GetTetrahedronMaterial() const { return tetrahedronMaterial->GetName(); }
	void SetTetrahedronMaterial(const G4String& newMaterialName);

	G4String GetTorusMaterial() const { return torusMaterial->GetName(); }
	void SetTorusMaterial(const G4String& newMaterialName);

	G4String GetConeMaterial() const { return coneMaterial->GetName(); }
	void SetConeMaterial(const G4String& newMaterialName);

private:
	unique_ptr<MyDetectorMessenger> myDetectorMessenger
		{make_unique<MyDetectorMessenger>(this)};
	G4ThreeVector halfLabSize {G4ThreeVector(20*cm, 21*cm, 22*cm)};

//	Raw pointers of G4Material type MUST NOT be deleted nor converted to smart pointers!
//	(else will incur double deletion at end of scope)!
//	Moreover, copy constructor and assignment operator of G4Material are private.
	G4Material *labMaterial {nullptr};
	G4Material *trapezoidMaterial {nullptr};
	G4Material *sphereMaterial {nullptr};
	G4Material *tetrahedronMaterial {nullptr};
	G4Material *torusMaterial {nullptr};
	G4Material *coneMaterial {nullptr};

//  solidLab MUST be a raw pointer otherwise changing materials fails,
//	i.e. a short dump upon e.g. /material/trapezoid ... followed by /run/beamOn
	G4Box *solidLab {nullptr};
	unique_ptr<G4Trd> solidTrapezoid {nullptr};
	unique_ptr<G4Sphere> solidSphere {nullptr};
	unique_ptr<G4Tet> solidTetrahedron {nullptr};
	unique_ptr<G4Torus> solidTorus {nullptr};
	unique_ptr<G4Cons> solidCone {nullptr};

	G4LogicalVolume *logicalLab {nullptr};
	unique_ptr<G4LogicalVolume> logicalTrapezoid {nullptr};
	unique_ptr<G4LogicalVolume> logicalSphere {nullptr};
	unique_ptr<G4LogicalVolume> logicalTetrahedron {nullptr};
	unique_ptr<G4LogicalVolume> logicalTorus {nullptr};
	unique_ptr<G4LogicalVolume> logicalCone {nullptr};

	G4VPhysicalVolume *physicalLab {nullptr};
	unique_ptr<G4VPhysicalVolume> physicalTrapezoid {nullptr};
	unique_ptr<G4VPhysicalVolume> physicalSphere {nullptr};
	unique_ptr<G4VPhysicalVolume> physicalTetrahedron {nullptr};
	unique_ptr<G4VPhysicalVolume> physicalTorus {nullptr};
	unique_ptr<G4VPhysicalVolume> physicalCone {nullptr};

	G4bool checkOverlaps {true};

	enum class Colour { red, green, blue,
						yellow, orange, brown, cyan, magenta, white, invisible };
	Colour colour { Colour::yellow };

	enum class Texture { wireframe, solid };
	Texture texture { Texture::solid };

	enum class MaterialCustomBuilt { Al, Pb, Ti, CsI,
									 graphite, pressurisedWater, heavyWater };
	MaterialCustomBuilt materialCustom { MaterialCustomBuilt::graphite };

	enum class MaterialNISTdatabase { G4waterLiquid, G4waterSteam, G4Pb, G4concrete,
										G4skinICRP, G4softTissueICRP,
										G4muscleWithSucrose, G4boneICRU,
										G4vacuum, G4air };
	MaterialNISTdatabase materialNIST { MaterialNISTdatabase::G4air };

private:
	G4VPhysicalVolume* ConstructDetector();
	G4VPhysicalVolume* BuildLab();
	unique_ptr<G4VPhysicalVolume> BuildTrapezoid();
	unique_ptr<G4VPhysicalVolume> BuildSphere();
	unique_ptr<G4VPhysicalVolume> BuildTetrahedron();
	unique_ptr<G4VPhysicalVolume> BuildTorus();
	unique_ptr<G4VPhysicalVolume> BuildCone();
	unique_ptr<G4VisAttributes> ChooseColour(Colour colour,
											Texture texture = Texture::solid);
	G4Material* ChooseMaterial(MaterialCustomBuilt materialCustom);
	G4Material* ChooseMaterial(MaterialNISTdatabase materialNIST);
	G4Material* ChooseMaterial(const G4String& materialNIST);
};

#endif /* HDR_MYDETECTORCONSTRUCTION_HH_ */
