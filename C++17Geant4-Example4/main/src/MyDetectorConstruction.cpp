#include "MyDetectorConstruction.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4NistManager.hh"
#include "G4VisAttributes.hh"
#include "G4RunManager.hh"

G4VPhysicalVolume* MyDetectorConstruction::Construct()
{
	DefineMaterials();
	return ConstructDetector();
}

void MyDetectorConstruction::DefineMaterials()
{
	G4String name, symbol;
	G4double density, temperature, pressure;
	G4double molarMass; 			// A, mass of a mole
	G4double atomicNumber; 			// Z, number of protons
	G4int nucleonsPerNucleus, numberOfIsotopes, atomsPerMolecule, numberOfComponents;
	G4double relativeAbundance, fractionOfMass;

//	Instantiate custom built materials to make them available whenever changing
//	materials via GUI commands such as:
//		/materials/lab Lead
//	or
//		/materials/trapezoid Titanium
//	etc, (cf. MyDetectorMessenger.cpp) - otherwise get "Material not found" error.

	// DO NOT convert G4Material to raw pointers!
	G4Material *Al = new G4Material(name="aluminium",
									atomicNumber=13,
									molarMass=26.98*g/mole,
									density=2.7*g/cm3);
	G4Material *Pb = new G4Material(name="lead",
									atomicNumber=82,
									molarMass= 207.19*g/mole,
									density= 11.35*g/cm3);
	G4Material *Ti = new G4Material(name="titanium",
									atomicNumber=22,
									molarMass=47.867*g/mole,
									density=4.54*g/cm3);

	// cesium iodide
	auto Cs {make_unique<G4Element>("cesium", symbol="Cs",
									atomicNumber=55,
									molarMass=132.9*g/mole)};
	auto I {make_unique<G4Element>("iodine", symbol="I",
									atomicNumber=53,
									molarMass=126.9*g/mole)};
	G4Material *CsI = new G4Material(name="CsI",
									density=4.51*g/cm3,
									numberOfComponents=2);
	CsI->AddElement(Cs.release(), fractionOfMass=0.5);
	CsI->AddElement(I.release(), fractionOfMass=0.5);

	// graphite
	auto C12isotope {make_unique<G4Isotope>("C12",
											atomicNumber=6,
											nucleonsPerNucleus=12)};
	auto C12element {make_unique<G4Element>("Graphite",
											symbol="C12",
											numberOfComponents=1)};
	C12element->AddIsotope(C12isotope.release(), relativeAbundance=100*perCent);
	G4Material *graphite = new G4Material(name="graphite",
										density=2.27*g/cm3,
										numberOfComponents=1,
										kStateSolid,
										temperature=293*kelvin,
										pressure=1*atmosphere);
	graphite->AddElement(C12element.release(), atomsPerMolecule=1);

	// pressurized water
	auto H {make_unique<G4Element>("Hydrogen", symbol="H",
									atomicNumber=1,
									molarMass=1.0079*g/mole)};
	auto O {make_unique<G4Element>("Oxygen", symbol="O",
									atomicNumber=8,
									molarMass=16.00*g/mole)};
	G4Material *H2Opressurised = new G4Material(name="pressurised water",
												density=1.000*g/cm3,
												numberOfComponents=2,
												kStateLiquid,
												temperature=593*kelvin,
												pressure=150*bar);
	H2Opressurised->AddElement(H.release(), atomsPerMolecule=2);
	H2Opressurised->AddElement(O.release(), atomsPerMolecule=1);
	H2Opressurised->GetIonisation()->SetMeanExcitationEnergy(78.0*eV);

	// heavy water
	auto Deuteron {make_unique<G4Isotope>("Deuteron",
										atomicNumber=1,
										nucleonsPerNucleus=2)};
	auto Deuterium {make_unique<G4Element>("Deuterium", symbol="D",
										numberOfIsotopes=1)};
//	redefine oxygen since it has been released above
	O = make_unique<G4Element>("Oxygen", symbol="O",
								atomicNumber=8,
								molarMass=16.00*g/mole);
	Deuterium->AddIsotope(Deuteron.release(), relativeAbundance=100*perCent);
	G4Material *D2O = new G4Material(name="heavy water",
									density=1.11*g/cm3,
									numberOfComponents=2,
									kStateLiquid,
									temperature=293.15*kelvin,
									pressure=1*atmosphere);
	D2O->AddElement(Deuterium.release(), atomsPerMolecule=2);
	D2O->AddElement(O.release(), atomsPerMolecule=1);

//	Prevent changing materials to default values if they were adjusted at run time:
//	i.e. in case G4RunManager::GetRunManager()->ReinitializeGeometry() is invoked by
//	some user command, such as /rotate/,,,
	if (labMaterial == nullptr)
		labMaterial = ChooseMaterialFromNISTdatabase(Material::G4air);
	if (trapezoidMaterial == nullptr)
		trapezoidMaterial = Ti;
	if (sphereMaterial == nullptr)
		sphereMaterial = ChooseMaterialFromNISTdatabase("G4_Au");
	if (tetrahedronMaterial == nullptr)
		tetrahedronMaterial = ChooseMaterialFromNISTdatabase("G4_Ag");
	if (torusMaterial == nullptr)
		torusMaterial = Pb;
	if (coneMaterial == nullptr)
		coneMaterial = ChooseMaterialFromNISTdatabase(Material::G4vacuum);

//	DisplayMaterialLabelsViaMacroFile();
}

G4Material* MyDetectorConstruction::ChooseMaterialFromNISTdatabase(Material material)
{
// A selection of popular materials
	switch (material)
	{
		case Material::G4waterLiquid:
			return G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER");
		case Material::G4waterSteam:
			return G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER_VAPOR");
		case Material::G4Pb:
			return G4NistManager::Instance()->FindOrBuildMaterial("G4_Pb");
		case Material::G4concrete:
			return G4NistManager::Instance()->FindOrBuildMaterial("G4_CONCRETE");
		case Material::G4skinICRP:
			return G4NistManager::Instance()->FindOrBuildMaterial("G4_SKIN_ICRP");
		case Material::G4softTissueICRP:
			return G4NistManager::Instance()->FindOrBuildMaterial("G4_TISSUE_SOFT_ICRP");
		case Material::G4muscleWithSucrose:
			return G4NistManager::Instance()->FindOrBuildMaterial("G4_MUSCLE_WITH_SUCROSE");
		case Material::G4boneICRU:
			return G4NistManager::Instance()->FindOrBuildMaterial("G4_BONE_COMPACT_ICRU");
		case Material::G4vacuum:
			return G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
		case Material::G4air:
			return G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
		default:
			return G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
	}
}

G4Material* MyDetectorConstruction::ChooseMaterialFromNISTdatabase(const G4String& material)
{
//	Additional materials can be obtained by supplying the correct name, e.g. "G4_Au".
	return G4NistManager::Instance()->FindOrBuildMaterial(material);
}

G4VPhysicalVolume* MyDetectorConstruction::ConstructDetector()
{
	physicalLab = BuildLab();
	physicalTrapezoid = BuildTrapezoid();
	physicalSphere = BuildSphere();
	physicalTetrahedron = BuildTetrahedron();
	physicalTorus = BuildTorus();
	physicalCone = BuildCone();

	return physicalLab;
}

G4VPhysicalVolume* MyDetectorConstruction::BuildLab()
{
	solidLab = new G4Box("Lab", halfLabSize.x(), halfLabSize.y(), halfLabSize.z());

	logicalLab = new G4LogicalVolume(solidLab,
									labMaterial, //material
									"Lab");      //name

//	Uncomment below if you don't want to visualise the lab box
//	logicalLab->SetVisAttributes(ChooseColour(Colour::invisible).release());

	return new G4PVPlacement(nullptr,	   	 // no rotation
							G4ThreeVector(), // at (0,0,0)
							logicalLab,		 // raw pointer to logical volume
							"Lab",           // name
							nullptr,         // mother volume
							false,           // no boolean operation
							0); 	         // copy number};
}

unique_ptr<G4VPhysicalVolume> MyDetectorConstruction::BuildTrapezoid()
{
	G4double halfLengthX1 {8*cm}, halfLengthX2 {5*cm};
	G4double halfLengthY1 {6*cm}, halfLengthY2 {4*cm};
	G4double halfLengthZ {3*cm};
	solidTrapezoid = make_unique<G4Trd>
								("Trapezoid",
								halfLengthX1, halfLengthX2,
								halfLengthY1, halfLengthY2,
								halfLengthZ);

	logicalTrapezoid = make_unique<G4LogicalVolume>
									(solidTrapezoid.get(), // convert to raw pointer
									trapezoidMaterial, 	   // it's a raw pointer
									"Trapezoid");

//	The SetVisAttributes method accepts a raw pointer and takes ownership of it,
//	hence must .release() the unique_ptr returned by ChooseColour
	logicalTrapezoid->SetVisAttributes(ChooseColour(Colour::yellow).release());

//	Various G4PVPlacement constructors are available, e.g. uncomment #1 or #2 (not both)
//	1. This constructor uses physical mother volume:
//	return make_unique<G4PVPlacement>
//						(nullptr,				// no rotation
//						G4ThreeVector(), 		// at (x,y,z)
//						"Trapezoid",     		// name
//						logicalTrapezoid.get(),	// logical volume
//						physicalLab.get(), 		// PHYSICAL mother volume
//						false,           		// no boolean operations
//						0,               		// copy number
//						checkOverlaps); 		// checking overlaps

//	2. This constructor uses logical mother volume:
	return make_unique<G4PVPlacement>
						(nullptr,				// no rotation
						G4ThreeVector(), 		// at (0, 0, 0)
						logicalTrapezoid.get(),	// convert to raw pointer
						"Trapezoid",     		// name
						logicalLab,		 		// LOGICAL mother volume
						false,           		// no boolean operations
						0,               		// copy number
						checkOverlaps); 		// checking overlaps
}

unique_ptr<G4VPhysicalVolume> MyDetectorConstruction::BuildSphere()
{
	G4double innerRadius {0}, outerRadius {2.5*cm};
	G4double startPhi {0}, endPhi {360*deg};
	G4double startTheta {0}, endTheta {180*deg};
	solidSphere = make_unique<G4Sphere>("Sphere",	   			// name
										innerRadius, outerRadius,
										startPhi, endPhi,
										startTheta, endTheta);

	logicalSphere = make_unique<G4LogicalVolume>
								(solidSphere.get(),	// convert to raw pointer
								sphereMaterial, 	// it is a raw pointer
								"Sphere");

	logicalSphere->SetVisAttributes(
			ChooseColour(Colour::orange, Texture::wireframe).release());

//	Various G4PVPlacement constructors are available, e.g. uncomment #1 or #2 (not both)
//	1. This constructor uses physical mother volume:
//	return make_unique<G4PVPlacement>
//						(nullptr,               // no rotation
//						G4ThreeVector(), 		// at (0, 0, 0)
//						"Sphere",      			// its name
//						logicalSphere.get(), 	// convert to raw pointer
//						physicalTrapezoid.get(),// NB: NOT physicalLab for mother volume
//						false,           		// no boolean operations
//						0,               		// copy number
//						checkOverlaps); 		// checking overlaps

//	2. This constructor uses logical mother volume:
	return make_unique<G4PVPlacement>
						(nullptr,               // no rotation
						G4ThreeVector(),		// at (0, 0, 0)
						logicalSphere.get(), 	// convert to raw pointer
						"Sphere",      			// its name
						logicalTrapezoid.get(), // NB: NOT logicalLab for mother volume
						false,           		// no boolean operations
						0,               		// copy number 0
						checkOverlaps); 		// checking overlaps
}

unique_ptr<G4VPhysicalVolume> MyDetectorConstruction::BuildTetrahedron()
{
	G4ThreeVector vertex1 = G4ThreeVector(-12*mm, 0, 0);
	G4ThreeVector vertex2 = G4ThreeVector(12*mm, 0, 0);
	G4ThreeVector vertex3 = G4ThreeVector(0, 20*mm, 0);
	G4ThreeVector vertex4 = G4ThreeVector(0, 0, 30*mm);
	solidTetrahedron = make_unique<G4Tet>("Tetrahedron",
										vertex1, vertex2, vertex3, vertex4);

	logicalTetrahedron = make_unique<G4LogicalVolume>
									(solidTetrahedron.get(),// convert to raw pointer
									tetrahedronMaterial,	// it's a raw pointer
									"Tetrahedron");

	logicalTetrahedron->SetVisAttributes(
			ChooseColour(Colour::brown, Texture::solid).release());

	unique_ptr<G4RotationMatrix> rotation {make_unique<G4RotationMatrix>()};
	rotation->rotateX(150*deg);
	rotation->rotateY(0*deg);
	rotation->rotateZ(5*deg);

	return make_unique<G4PVPlacement>
						(rotation.release(),    // give up ownership of rotation pointer
						G4ThreeVector(4*cm, 2*cm, 1*cm), // at (x, y, z)
						logicalTetrahedron.get(), 		 // convert to raw pointer
						"Tetrahedron",     				 // name
						logicalTrapezoid.get(), // NB: NOT logicalLab for mother volume since inside trapezoid
						false,           		// no boolean operations
						0,               		// copy number 0
						checkOverlaps); 		// checking overlaps
}


unique_ptr<G4VPhysicalVolume> MyDetectorConstruction::BuildTorus()
{
	G4double radius1min {1.25*cm}, radius1max {2*cm}, radius2 {4*cm};
	G4double angle1 {60*deg}, angle2 {2.5*rad};
	solidTorus = make_unique<G4Torus>
							("Torus",
							radius1min, radius1max,
							radius2,
							angle1, angle2);

	logicalTorus = make_unique<G4LogicalVolume>
							(solidTorus.get(), // convert to raw pointer
							torusMaterial,	   // it's a raw pointer
							"Torus");

	logicalTorus->SetVisAttributes(ChooseColour(Colour::magenta,
												Texture::wireframe).release());

//	unique_ptr<G4RotationMatrix> rotation {make_unique<G4RotationMatrix>()};
	G4RotationMatrix *rotation = new G4RotationMatrix();
	rotation->rotateX(rotationAngleX);
	rotation->rotateY(rotationAngleY);
	rotation->rotateZ(rotationAngleZ);

	return make_unique<G4PVPlacement>
//					(rotation.release(), // give up ownership of rotation pointer
					(rotation,
					G4ThreeVector(14*cm, 9*cm, 6*cm), // at (x,y,z)
					logicalTorus.get(),				  // its logical volume
					"Torus",		   				  // its name
					logicalLab,		    // logical mother volume - it's a raw pointer
					false,           				  // no boolean operations
					0,               				  // copy number
					checkOverlaps); 				  // checking overlaps
}

unique_ptr<G4VPhysicalVolume> MyDetectorConstruction::BuildCone()
{
	G4double radiusBaseInner {0.7*cm}, radiusBaseOuter {3.4*cm};
	G4double radiusTopInner {1.7*cm}, radiusTopOuter {2*cm};
	G4double halfHeightZ {2.5*cm};
	G4double anglePhiStart {0}, anglePhiStop {235*deg};
	solidCone = make_unique<G4Cons>
							("Cone",
							radiusBaseInner, radiusBaseOuter,
							radiusTopInner, radiusTopOuter,
							halfHeightZ,
							anglePhiStart, anglePhiStop);

	logicalCone = make_unique<G4LogicalVolume>
								(solidCone.get(),	// convert to raw pointer
								coneMaterial,		// it is a raw pointer
								"Cone");

	logicalCone->SetVisAttributes(ChooseColour(Colour::cyan, Texture::solid).release());

	return make_unique<G4PVPlacement>
						(nullptr,			     	 // no rotation
						G4ThreeVector(-13*cm, 0, 0), // at (x,y,z)
						logicalCone.get(),			 // logical volume
						"Cone",		      			 // name
						logicalLab,		// logical mother volume - it's a raw pointer
						false,           			 // no boolean operations
						0,               			 // copy number
						checkOverlaps); 			 // checking overlaps
}


unique_ptr<G4VisAttributes> MyDetectorConstruction::
									ChooseColour(Colour colour, Texture texture)
{
	G4double opacity {0.4};
	G4bool isVisible {true};
	unique_ptr<G4VisAttributes> chosenColour;

	switch (colour)
	{
		case Colour::red:
		{
			chosenColour = make_unique<G4VisAttributes>(G4Colour(1, 0, 0, opacity));
			break;
		}
		case Colour::green:
		{
			chosenColour = make_unique<G4VisAttributes>(G4Colour(0, 1, 0, opacity));
			break;
		}
		case Colour::blue:
		{
			chosenColour = make_unique<G4VisAttributes>(G4Colour(0, 0, 1, opacity));
			break;
		}
		case Colour::yellow:
		{
			chosenColour = make_unique<G4VisAttributes>(G4Colour(1, 1, 0, opacity));
			break;
		}
		case Colour::orange:
		{
			chosenColour = make_unique<G4VisAttributes>(G4Colour(1, 0.65, 0, opacity));
			break;
		}
		case Colour::brown:
		{
			chosenColour = make_unique<G4VisAttributes>
								(G4Colour(0.545, 0.271, 0.075, opacity));
			break;
		}
		case Colour::cyan:
		{
			chosenColour = make_unique<G4VisAttributes>(G4Colour(0, 1, 1, opacity));
			break;
		}
		case Colour::magenta:
		{
			chosenColour = make_unique<G4VisAttributes>(G4Colour(1, 0, 1, opacity));
			break;
		}
		case Colour::white:
		{
			opacity = 1;
			chosenColour = make_unique<G4VisAttributes>(G4Colour(1, 1, 1, opacity));
			break;
		}
		case Colour::invisible:
		{
			chosenColour = make_unique<G4VisAttributes>(G4Colour(0, 0, 0, opacity));
			isVisible = false;
			break;
		}
		default:
		{
			chosenColour = make_unique<G4VisAttributes>(G4Colour(1, 1, 1, opacity));
			break;
		}
	}

	switch (texture)
	{
	case Texture::wireframe:
		chosenColour->SetForceWireframe(true);
		break;
	case Texture::solid:
		chosenColour->SetForceSolid(true);
		break;
	}

	chosenColour->SetVisibility(isVisible);
	return chosenColour;
}

void MyDetectorConstruction::SetActiveRotationAngles(G4double rotationAngleX,
													G4double rotationAxisY,
													G4double rotationAxisZ)
{
	this->rotationAngleX = rotationAngleX;
	this->rotationAngleY = rotationAngleY;
	this->rotationAngleZ = rotationAngleZ;
//  Rotation effective immediately; removes beam:
//	G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
//  Rotation takes effect upon: /run/beamOn
	G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void MyDetectorConstruction::SetLabMaterial(const G4String& newMaterialName)
{
	G4Material *ptrToMaterial = G4NistManager::Instance()->FindOrBuildMaterial(newMaterialName);
	if (ptrToMaterial)
	{
		if (labMaterial != ptrToMaterial)
		{
			labMaterial = ptrToMaterial;
			logicalLab->SetMaterial(labMaterial);
			G4RunManager::GetRunManager()->PhysicsHasBeenModified();
			G4cout << "Lab material changed to: " << GetLabMaterial() << G4endl;
		}
	}
	else
	{
		G4cerr << "Material not found" << G4endl;
	}
}

void MyDetectorConstruction::SetTrapezoidMaterial(const G4String& newMaterialName)
{
	G4Material *ptrToMaterial = G4NistManager::Instance()->FindOrBuildMaterial(newMaterialName);
	if (ptrToMaterial)
	{
		if (trapezoidMaterial != ptrToMaterial)
		{
			trapezoidMaterial = ptrToMaterial;
			logicalTrapezoid->SetMaterial(trapezoidMaterial);
			G4RunManager::GetRunManager()->PhysicsHasBeenModified();
			G4cout << "Trapezoid material changed to: " << GetTrapezoidMaterial() << G4endl;
		}
	}
	else
	{
		G4cerr << "Material not found" << G4endl;
	}
}

void MyDetectorConstruction::SetSphereMaterial(const G4String& newMaterialName)
{
	G4Material *ptrToMaterial = G4NistManager::Instance()->FindOrBuildMaterial(newMaterialName);
	if (ptrToMaterial)
	{
		if (sphereMaterial != ptrToMaterial)
		{
			sphereMaterial = ptrToMaterial;
			logicalSphere->SetMaterial(sphereMaterial);
			G4RunManager::GetRunManager()->PhysicsHasBeenModified();
			G4cout << "Sphere material changed to: " << GetSphereMaterial() << G4endl;
		}
	}
	else
	{
		G4cerr << "Material not found" << G4endl;
	}
}

void MyDetectorConstruction::SetTetrahedronMaterial(const G4String& newMaterialName)
{
	G4Material *ptrToMaterial = G4NistManager::Instance()->FindOrBuildMaterial(newMaterialName);
	if (ptrToMaterial)
	{
		if (tetrahedronMaterial != ptrToMaterial)
		{
			tetrahedronMaterial = ptrToMaterial;
			logicalTetrahedron->SetMaterial(tetrahedronMaterial);
			G4RunManager::GetRunManager()->PhysicsHasBeenModified();
			G4cout << "Tetrahedron material changed to: " << GetTetrahedronMaterial() << G4endl;
		}
	}
	else
	{
		G4cerr << "Material not found" << G4endl;
	}
}

void MyDetectorConstruction::SetTorusMaterial(const G4String& newMaterialName)
{
	G4Material *ptrToMaterial = G4NistManager::Instance()->FindOrBuildMaterial(newMaterialName);
	if (ptrToMaterial)
	{
		if (torusMaterial != ptrToMaterial)
		{
			torusMaterial = ptrToMaterial;
			logicalTorus->SetMaterial(torusMaterial);
			G4RunManager::GetRunManager()->PhysicsHasBeenModified();
			G4cout << "Torus material changed to: " << GetTorusMaterial() << G4endl;
		}
	}
	else
	{
		G4cerr << "Material not found" << G4endl;
	}
}

void MyDetectorConstruction::SetConeMaterial(const G4String& newMaterialName)
{
	G4Material *ptrToMaterial = G4NistManager::Instance()->FindOrBuildMaterial(newMaterialName);
	if (ptrToMaterial)
	{
		if (coneMaterial != ptrToMaterial)
		{
			coneMaterial = ptrToMaterial;
			logicalCone->SetMaterial(coneMaterial);
			G4RunManager::GetRunManager()->PhysicsHasBeenModified();
			G4cout << "Cone material changed to: " << GetConeMaterial() << G4endl;
		}
	}
	else
	{
		G4cerr << "Material not found" << G4endl;
	}
}

void MyDetectorConstruction::DisplayMaterialLabelsViaMacroFile()
{
	G4String fileName = "./vis-materialLabels.macro";

	std::ofstream outFile;
	outFile.open(fileName);
	// Create and write to the file if it doesn't exit, otherwise overwrite it
	if (outFile.is_open())
	{
		outFile << "#LabMaterial" << G4endl;
		outFile << "/vis/set/textColour white" << G4endl;
		outFile << "/vis/scene/add/text -18 -18 -18 cm 10 0 0 "
				<< labMaterial->GetName() << G4endl;

		outFile << "#TrapezoidMaterial" << G4endl;
		outFile << "/vis/set/textColour white" << G4endl;
		outFile << "/vis/scene/add/text 0 4 0 cm 10 0 0 "
				<< trapezoidMaterial->GetName() << G4endl;

		outFile << "#SphereMaterial" << G4endl;
		outFile << "/vis/set/textColour white" << G4endl;
		outFile << "/vis/scene/add/text 0 -1.5 0 cm 10 0 0 "
				<< sphereMaterial->GetName() << G4endl;

		outFile << "#TetrahedronMaterial" << G4endl;
		outFile << "/vis/set/textColour white" << G4endl;
		outFile << "/vis/scene/add/text 4 2 1 cm 10 0 0 "
				<< tetrahedronMaterial->GetName() << G4endl;

		outFile << "#TorusMaterial" << G4endl;
		outFile << "/vis/set/textColour white" << G4endl;
		outFile << "/vis/scene/add/text 14 9 6 cm 10 0 0 "
				<< torusMaterial->GetName() << G4endl;

		outFile << "#ConeMaterial" << G4endl;
		outFile << "/vis/set/textColour white" << G4endl;
		outFile << "/vis/scene/add/text -13 0 0 cm 10 0 0 "
				<< coneMaterial->GetName() << G4endl;

//		outFile << "#Scale interval set to yellow colour:" << G4endl;
//		outFile << "/vis/set/textColour yellow" << G4endl;

		outFile.close();
	}
	else
	{
		G4cerr << "Could not create file " << fileName << G4endl;
	}
}
