
// ****************
// Basic Mandelbrot
// ****************
//
// This set of Mandelbrot programs shows creating and displaying a Mandelbrot and/or Julia Set 
// in various stages, from very basic, to faster, all the way to an interactive program.
//
// This is #1 Basic Mandelbrot
//
// ---------------------------
// Mandelbrot Example Projects
// ---------------------------
//
//  1. Basic Mandelbrot
//
//      This is a simple mandelbrot with no color gradation, so the colors change abruptly vs. the Smooth Color version that
//      has a nice color gradient throughout. 
// 
//      Either type can create some interesting mandelbrot displays and colors.
// 
//  2. Smooth Color, Julia Set, and Dev Window
//
//      This is a small change from the previous version, adding smooth coloring and the Julia
//      set to the output.   
//
//      The Dev Window is also used to show the ease-of-use of the Dev Controls without affecting your main code or window.
//
//      This version also adds some more power by assing z.sq() instead of z*z for a little faster
//      Mandelbrot calculation.  FastSetPixel() is also used in CSageBitmap to set the pixel much faster.
//
//  3. MandelBrot Interactive (zoom in/out, resize window, set Julia Set and more) 
//
//      This shows how to add Development-based controls to change values of the Mandelbrot
//      with just a few lines of code.
//
//      It shows using GetEvent() in the main eventloop to get and react to events. 
//  
//
// -------------------------------------
// About This Version (Basic Mandelbrot)
// -------------------------------------
//
//   This program is a very basic version of the Mandelbrot program. 
//   
//   It uses std::complex and Windows DrawPixel (aka SetPixel) for output.  They are easy to use, but both are much slower than using
//   other methods which are just as easy (See next version "Basic Mandelbrot Faster"). 
//  
//   See comments on two things: 
//  
//      1. A couple different settings for different Mandelbrots
//      2. Changing a couple values to convert the program to produce a Julia Set (which are also very nice)
//
// -------------------------------------------------------
// Using elements of this program for your own programming
// -------------------------------------------------------
//
// As with all SageBox demo code, it is written to show how SageBox works, but also general C/C++ programming practices.
// 
// ** Please feel free to use any of this code for your own personal purposes **
//
// SageBox is specifically written to be portable and used by example and the "intellisense" comments that come up for each function.
// Most SageBox code snippets are also portable and can be taken from one program and used in another.
//
// SageBox is free for personal use. 
//

#include "SageBox.h"
#include <complex>

int main()

{
    CfPoint winSize      = { 1400, 900 };     // Initial Window Size
 
    // Initial Mandelbrot Values (Center, Range and Iterations)

    CfPoint pCenter    = { -.6, 0 };       // Initial Mandelbrot Center
    double range       = 3.5;              // Initial Range (i.e. "zoom" factor)
    int maxIter        = 50;               // Max Mandelbrot Iterations

    CfPoint pRange     = { range, range*winSize.y/winSize.x };   // Range based on X-axis

    CfPoint fD         = pRange/winSize;                            // Unit Increment for each pixel
    CfPoint pStart     = pCenter - fD*winSize/2;                   // Upper-left X,Y position to start

    // Color Table based on Wikipedia Mandelbrot Colors

    static RgbColor rgbColors[16] = 
    {
        { 0, 0, 0       }, { 25, 7, 26     }, { 9, 1, 47      }, { 4, 4, 73      }, 
        { 0, 7, 100     }, { 12, 44, 138   }, { 24, 82, 177   }, { 57, 125, 209  },
        { 134, 181, 229 }, { 211, 236, 248 }, { 241, 233, 191 }, { 248, 201, 95  },
        { 255, 170, 0   }, { 204, 128, 0   }, { 153, 87, 0    }, { 106, 52, 3    },
    }; 

    
    auto& win = Sagebox::NewWindow("Basic Mandelbrot (basic, no frills, standard C++ version)",kw::SetSize(winSize));

    for (int i=0;i<winSize.y;i++)
    {
        double y = (double) i*fD.y + pStart.y;
        for (int j=0;j<winSize.x;j++)
        {
            double x = (double) j*fD.x + pStart.x;

            int iter = 0;

            // For Julia Set, set z = { x, y } and c to a static value, such as (.285, 0) or (-4.,.6) (and set pCenter to (0,0);

            CComplex c = { x, y };
            auto z = c;
            while (abs(z) < 2 && iter++ < maxIter-1) z = z*z + c;

            int color = (iter == maxIter) ? 0 : iter % 16; //iter*15/(maxIter-1);

            // Note: Windows DrawPixel() is slow! See next version ("Faster Mandelbrot") for a better method using
            // a bitmap output that prints a line at a time (using CSageBitmap)

            win.DrawPixel(j,i,rgbColors[color]); 
        }
    }

    return win.ExitButton(); // Get user input so the window doesn't close when CSageBox is deleted.
}