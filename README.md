# Cpp17GEANT4-Example4
This is a substantial enhancement of [Cpp17GEANT4-Example3](https://github.com/MariuszJozef/Cpp17GEANT4-Example3); basically it is a modern C++11/14/17 refactoring of [GEANT4-Example4-MaterialsAndLabels](https://github.com/MariuszJozef/GEANT4-Example4-MaterialsAndLabels). Unique pointers are used whenever possible but in some situations smart pointers cannot replace raw pointers since the GENT4 kernel takes ownership of the raw pointers and deletes them. In those cases even releasing the unique pointer via .release() does not help.

![](gifs/C++17Geant4-Example4-1.gif)

![](gifs/C++17Geant4-Example4-2.gif)

![](gifs/C++17Geant4-Example4-3.gif)

![](gifs/C++17Geant4-Example4-4.gif)
