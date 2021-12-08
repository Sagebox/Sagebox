// File Copyright (c) 2021 Rob Nelson, All Rights Reserved.    Sagebox is free for personal use. 
// Please feel free to use and copy-paste elements of this program for your own programs that use Sagebox.

// ----------------------------
// Fractal Tree - Basic Version
// ----------------------------
//
// This program is about 20 lines of code, and is a good example of using Sagebox at a basic level -- 
// most of the code is about the Fractal Tree, and Sagebox is mostly to create the main window and put out the graphics. 
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

#include "SageBox.h"
#include "CAviFile.h"
#include <functional>

// DrawTre() -- Draw the tree recursively
//
// This function is passed a lot of data.  Alternatives are to make some of the data global (like cWin,_ang, fMul),
// or to make this part of a class so that it can use class members that are not passed.
//
// In the "Fancy Fractal Tree", this function is converted into a lambda that resides in the FractalTree()
// function -- this solves the problem of having to pass so much without having to make a class structure
// or make values global (and assign them).
//
void DrawTree(CWindow & cWin,Point3D_t & sp, double _ang,double fMul,double line_len, double a, double rg)
{
    static int iDepth = 0;  // We could pass this, too, but it is fine here (as a single-threaded application)

    // Make this lower or higher for a more simple or more complicated tree (i.e. 8,10,-15+, etc.)

	if (iDepth >= 11) return;

    iDepth++;

    // The Rosetta code declares a vector class in the code.  Sagebox has Point3D_t 
    // as an accessible type that performs the same actions. 

    auto r = Point3D_t{0,-line_len}.RotateZ(a += rg *_ang) + sp;

    // Draw a line in the window. Get an RGB value from a HUE, Saturation, and luminance value
    // CSageTools has various tools.  This also sets a pen size that is a smaller as the depth is greater

    cWin.DrawLine(sp,r);

	DrawTree(cWin,r, _ang,fMul, line_len*fMul, a, -1 );
	DrawTree(cWin,r, _ang,fMul, line_len*fMul, a, 1 );
    iDepth--;
};

// FracalTree()) -- Main Tree-drawing function. 
//
void FractalTree(CWindow & cWin,CfPoint szWinSize,double _ang,double line_len,double fMul,bool bShadow = false)
{
    Point3D_t sp{szWinSize.x/2,szWinSize.y-1 - line_len};

    // DrawLine2() is the same as DrawLine() except that you only need to specify length rather
    // than the actual endpoints, which is often convenience. 

    cWin.DrawLine2(sp,{0,(int) line_len});
 
	DrawTree(cWin,sp, _ang,fMul,line_len, 0, -1 );
	DrawTree(cWin,sp, _ang,fMul,line_len, 0, 1 );
}


int main( int argc, char* argv[] )
{ 

    // Create a window and CSagebox object together.  This is useful when you only want to create a Window
    // and don't need a cSagebo object.  You can obtain the CSagebox object created if necessary.
    //
    // Title()          - sets the name of the window
    // bgGradient()     - clears the background of the window in a gradient with the colors used.
    //
    // The '|' function as << (which can also be used), as in Title("My Title)" << bgGradient("color1","color2"); 
    // the '|' just make the code more readable (for me, anyway)
    //

    auto& cWin = Sagebox::NewWindow(CSize(1000,700),"Basic Fractal Tree",InnerSize() | bgGradient(SageColor::Black,SageColor::SkyBlueDark)); 

    // These values can be changed for a different effect. 
  
    double fAngle       = 24;   // Higher Values will generate more block-like trees( i.e. try 45)
    double fLineLen     = 140;  // Makes the tree smaller or larger. 
    double fLineMult    = .758; // Controls how much shorter each segment gets

    // Draw the Fractal Tree

    FractalTree(cWin,cWin.GetWindowSize(),fAngle*3.14159/180,fLineLen,fLineMult); 
    cWin.ExitButton();                      // Put a button at the bottom of the window and wait for it to be pressed, 
                                            // then exit.  (To get rid of the button but still wait, use WaitforClose(), which 
                                            // will wait for the window to be closed without adding the button)
}

