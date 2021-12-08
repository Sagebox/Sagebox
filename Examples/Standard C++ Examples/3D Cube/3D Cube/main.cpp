// *****************************************
// SageBox 3D Interactive Cube Demonstration
// *****************************************
//
// See notes in CDrawCube.cpp for more information
//

#include "SageBox.h"
#include "CDrawCube.h"

int main()
{
    auto& cWin = Sagebox::NewWindow("Sagebox -- 3-D Cube",NoAutoUpdate());
    DrawCube cDrawCube(&cWin);
    cDrawCube.Main();
}
