// *****************************************
// SageBox 3D Interactive Cube Demonstration
// *****************************************
//
// See notes in CDrawCube.cpp for more information
//

#include "CSageBox.h"
#include "CDrawCube.h"

int main()
{
    CSageBox cSageBox("SageBox -- 3-D Cube");
    auto& cWin = cSageBox.NewWindow(NoAutoUpdate());
    
    DrawCube cDrawCube(&cWin);
    cDrawCube.Main();
}
