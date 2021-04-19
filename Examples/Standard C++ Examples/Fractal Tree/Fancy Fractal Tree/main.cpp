// File Copyright (c) 2021 Rob Nelson, All Rights Reserved.    Sagebox is free for personal use. 
// Please feel free to use and copy-paste elements of this program for your own programs that use Sagebox.

// ------------------------------
// Fractal Tree - Fancier Version
// ------------------------------
//
// This program is about 20 lines of code, just as the Basic Fractal Tree, showing using Sagebox in more-or-less
// minimal way -- most of the code is about the Fractal Tree, and Sagebox usage is mostly to create the main window 
// and put out the graphics. 
// 
// The main difference in this version is that it uses a color table to put out some nice color for the tree, and 
// converts DrawTree() into a recursive lambda to make it easier to use (this part has nothing to do with Sagebox, 
// but C++ proramming)
//
// This program shows how expansive Sagebox functions can be.  For example, DrawLine() in the basic version
// did not provide a color, relying on the current Pen Color.  In this example, the same line is used, provided
// from the color table -- this tells DrawLine() to use this color instead, and does not change the current Pen Color. 
//
// This is an example of using Sagebox to create Procedural C++ Windows programs.
//
// The original source code for this program was found at Rosetta Stone at http://rosettacode.org/wiki/Category:C%2B%2B
//
// note: This project is set for Windows Debug Optimized, and can be changed to a Console Mode program with 
// a project switch.

// Include the main CSagebox.h file.  This includes most typical C++ .h files, as well. 
//
// <functional> is included for the std::function reference, since I couldn't use auto because DrawTree is recursive

#include "CSageBox.h"
#include "CAviFile.h"
#include <functional>

// Set a color table for each depth.  If the depth limitation moves past 14, extra RGB values need to be added.
//
RgbColor rgbColors[15] = {  { 213,88 ,31  }, { 194,50 ,0   }, { 193,86 ,0   }, 
                            { 164,116,22  }, { 45 ,122,163 }, { 35 ,89 ,242 },
                            { 201,0  ,218 }, { 255,0  ,173 }, { 255,0  ,107 },
                            { 255,21 ,41  }, { 244,52 ,0   }, { 172,76 ,0   },
                            { 81 ,96 ,0   }, { 151,168,0   }, { 248,250,0   },  };

// use std::function for the lamdba.  This is used because DrawTree() is a lambda function.
// Since it's recursive, it has to be declared beforehand, so 'auto' can't be used. 
//
using fDrawTree = std::function<void(Point3D_t &, double, double, double)>;

// FracalTree()) -- Main Tree-drawing function. 
//
// The recursive DrawTree() is used as a lambda so we can refer to the angle, depth, and Window object as if we 
// were in a class.  DrawTree() is easily moved out to its own function if iDepth,_ang, and cWin are either 
// passed or set as globals/class members
//
// iDepth should probably be passed to Drawtree() anyway, because it reduces two code lines in doing so,
// but it also makes a good example of using a lambda vs. extracting DrawTree() out to its own function.
//
void FractalTree(CWindow & cWin,CfPoint szWinSize,double _ang,double line_len,double fMul,bool bShadow = false)
{
    static int iDepth = 0;

    // I used a lambda here so I could avoid passing a lot of things.  Lamdbas 
    // are nice pre-steps to refactoring and branching out into other functions.
    //
    fDrawTree DrawTree = [&](Point3D_t & sp, double line_len, double a, double rg)
    {
        // Make this lower (i.e. from 14 to 12,11,10), etc. for a more plain tree. 

	    if (iDepth >= 14) return;

        // The Rosetta code declares a vector class in the code.  Sagebox has Point3D_t 
        // as an accessible type that performs the same actions. 

        auto r = Point3D_t{0,-line_len}.RotateZ(a += rg *_ang) + sp;

        // Draw a line in the window. Get an RGB value from a HUE, Saturation, and luminance value
        // CSageTools has various tools.  This also sets a pen size that is a smaller as the depth is greater

        cWin.DrawLine(sp,r,rgbColors[(int)++iDepth]);

	    DrawTree(r, line_len*fMul, a, -1 );
	    DrawTree(r, line_len*fMul, a, 1 );
        iDepth--;
    };

    Point3D_t sp{szWinSize.x/2,szWinSize.y-1 - line_len};

    // DrawLine2() is the same as DrawLine() except that you only need to specify length rather
    // than the actual endpoints, which is often convenient. 

    cWin.DrawLine2(sp,{0,(int) line_len},rgbColors[0]);
 
	DrawTree(sp, line_len, 0, -1 );
	DrawTree(sp, line_len, 0, 1 );
}

// Main Entry Point -- Create the window and call FractalTree() to draw the tree
//
int main( int argc, char* argv[] )
{ 
    // Create a window and CSagebox object together.  This is useful when you only want to create a Window
    // and don't need a cSagebox object.  You can obtain the CSagebox object created if necessary.
    //
    // Title()          - sets the name of the window
    // bgGradient()     - clears the background of the window in a gradient with the colors used.
    //
    // The '|' function as << (which can also be used), as in Title("My Title)" << bgGradient("color1","color2"); 
    // the '|' just make the code more readable (for me, anyway)
    //

    auto& cWin = CSagebox::AutoWindow(CSize(1000,700),Title("Fractal Tree") | bgGradient("black","skybluedark")); 

    // These values can be changed for a different effect. 
  
    double fAngle       = 24;
    double fLineLen     = 140;
    double fLineMult    = .758;

    // Draw the Fractal Tree

    FractalTree(cWin,cWin.GetWindowSize(),fAngle*3.14159/180,fLineLen,fLineMult); //.4 + (double) i/1000);
    cWin.WaitforClose();               
}

