
#include "Sagebox.h"
#include <functional>

using namespace Sage::kw;      // Sagebox Keyword Options

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


void FractalTree(CWindow & win,CfPoint szWinSize,double _ang,double line_len)
{
    // The original code from Rosetta Project created a 2-D vector type.
    // Sagebox already has the Point3D_t type that can be used with just the X & Y
    // components with the same functionality. 

    fDrawTree DrawTree = [&](Point3D_t & sp, double line_len, double a,  double rg,double depth)
    {
        auto r = Point3D_t{0,-line_len}.RotateZ(a += rg *_ang) + sp;

        // DrawLineFast() is used here to keep from anti-aliasing the pixels -- keeps it brighter

        win.DrawLineFast((CfPointf) sp,(CfPointf) r,SageTools::HSVtoRGB({.15 + depth/12,1,.9}));
	    for (int i=0;i<2;i++) if (depth < 12) DrawTree(r, line_len*.75, a, i*2-1,depth+1 );
    };

    Point3D_t sp{szWinSize.x/2,szWinSize.y-1 - line_len};
    win.DrawLine2(sp,{0,(int) line_len},{255,0,0});
	for (int i=0;i<2;i++) DrawTree(sp, line_len, 0, i*2-1 ,0);
}

// Main -- Create the Sagebox window and then call the Fractal Tree function

int main( int argc, char* argv[] )
{ 
    // Create the main window.  AutoWindow() also creates a static CSagebox object. Since
    // we don't use it, we can just get a window.  The CSagebox object can be retreived with 
    // CSagbox::GetStaticSagebox()
    //
    auto& win = Sagebox::NewWindow("Sagebox - Fractal Tree",kw::SetSize(1300,900)); 
    win.ClsRadial("darkblue,black");        // Clear the background with a radial gradient to look nice. 

    FractalTree(win,win.GetWindowSize(),Math::ToRad(24),130.0f*1.45);
    return win.ExitButton();

}

