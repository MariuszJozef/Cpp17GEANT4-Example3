# Cpp17GEANT4-Example3
The code of example GEANT4-MinimalExample3-TargetAndInteractions is refactored here into modern C++17 style; unique pointers are used instead of raw pointers with new and delete operators. However since GEANT4 methods accept old style raw pointers, it is often necessary to convert unique pointers into raw pointers via the .get() method. Moreover, some GEANT4 methods take ownership of the raw pointers that they accept, i.e. they delete the pointer; in such cases it is necessary to use .release() method insetad of the .get() method.

Visually the example here differs from GEANT4-MinimalExample3-TargetAndInteractions by only two extra solids which have been added to exemplify wireframe display option. But the greatest improvement is in the use of C++17 throughtout and overall clean up of the-code.

![](gifs/C++17Geant4-Example3-1.gif)

![](gifs/C++17Geant4-Example3-2.gif)

![](gifs/C++17Geant4-Example3-3.gif)

![](gifs/C++17Geant4-Example3-4.gif)
