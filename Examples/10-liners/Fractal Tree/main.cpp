// File Copyright (c) 2021 Rob Nelson, All Rights Reserved.    Sagebox is free for personal use. 
// Please feel free to use and copy-paste elements of this program for your own programs that use Sagebox.

// Include the main CSagebox.h file.  This includes most typical C++ .h files, as well. 
//
// <functional> is included for the std::function reference, since I couldn't use auto because DrawTree is recursive
//

#include "CSageBox.h"
#include <functional>

// ------------------------------
// Fractal Tree - 10 Line Version
// ------------------------------
// 
// Part of some 10-line examples posted on Quora, just for the fun of it.
//
// Notes:  The source code is a little 'compressed' fit within 10-lines, but no so much.
//         The main difference between a regular program and a 10-line program is there would be
//         quite a few variable assignments (rather than passed).
//
// This came from a Fractal Tree project that will be posted in the Examples as its own project
// at some point, including a version write out the frames as a video to convert the output into a
// 60fps WEBM or GIF file.
//
// This is a sample of using Sagebox to create Procedural C++ Windows programs.
//
// This program can be a Windows program or Console Mode program with a switch on a project setting.
// Since it doesn't use the Console Mode Window, it is better off as a Windows program.  
// Console-Mode can be turned back on for debugging output and other I/O.
//
// The original source code for this program was found at Rosetta Code at http://rosettacode.org/wiki/Category:C%2B%2B
//
// The comments in this program are oriented towards showing how Sagebox works and less about the
// Fractal Tree.  I intend to do a non-shortened version (that is larger but more self-documenting) that will
// describe more about how it is working. 
//
// Note: This project is set for a Windows program.  You can change it to 
//       a console-mode  program in the Build->Configuration settings
//

// use std::function for the lamdba.  This is used because DrawTree() is a lambda function.
// Since it's recursive, it has to be declared beforehand, so 'auto' can't be used. 
//
using fDrawTree = std::function<void(Point3D_t &, double, double, double,double)>;


void FractalTree(CWindow & cWin,CfPoint szWinSize,double _ang,double line_len)
{
    // The original code from Rosetta Project created a 2-D vector type.
    // Sagebox already has the Point3D_t type that can be used with just the X & Y
    // components with the same functionality. 

    fDrawTree DrawTree = [&](Point3D_t & sp, double line_len, double a,  double rg,double fDepth)
    {
        auto r = Point3D_t{0,-line_len}.RotateZ(a += rg *_ang) + sp;
        cWin.DrawLine(sp,r,CTools::HSVtoRGB({.15 + fDepth/12,1,.9}));
	    for (int i=0;i<2;i++) if (fDepth < 12) DrawTree(r, line_len*.75, a, i*2-1,fDepth+1 );
    };

    Point3D_t sp{szWinSize.x/2,szWinSize.y-1 - line_len};
    cWin.DrawLine2(sp,{0,(int) line_len},{255,0,0});
	for (int i=0;i<2;i++) DrawTree(sp, line_len, 0, i*2-1 ,0);
}

// Main -- Create the Sagebox window and then call the Fractal Tree function

int main( int argc, char* argv[] )
{ 
    // Create the main window.  AutoWindow() also creates a static CSagebox object. Since
    // we don't use it, we can just get a window.  The CSagebox object can be retreived with 
    // CSagbox::GetStaticSagebox()
    //
    auto& cWin = CSagebox::AutoWindow(CSize(1300,900),Title("Fractal Tree") | bgGradient("black","darkblue")); 
    FractalTree(cWin,cWin.GetWindowSize(),24*3.14159/180,130.0f*1.45);
    return cWin.WaitforClose(); // Wait for user to close the window

}

