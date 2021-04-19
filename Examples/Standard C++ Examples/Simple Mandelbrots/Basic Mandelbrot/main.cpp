// Basic Mandelbrot -- Copyright(c) 2020, 2021 Rob Nelson.  robnelsonxx2@gmail.com -- all rights reserved.
// This file, information, and process within are for personal use only and may not be distributed without permission.
// Please modify, copy, do whatever you want for personal uses.  For professional, distribution or commercial uses, 
// contact the e-mail address above

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
//      This is a simple Mandelbrot with color, using std::complex. 
//      It's slow, but works and is only about 25 lines of code.
//  
//  2. Basic Mandelbrot Faster
//
//      This adds speed to the mandelbrot by replacing std::complex with CComplex, 
//      Windows DrawPixel() with a bitmap funciton via CSageBitmap. 
//
//  3. Smooth Color, Julia Set, and Dev Window
//
//      This is a small change from the previous version, adding smooth coloring and the Julia
//      set to the output.   
//
//      The Dev Window is also used to show the ease-of-use of the Dev Controls without affecting your main code or window.
//
//      This version also adds some more power by assing z.sq() instead of z*z for a little faster
//      Mandelbrot calculation.  FastSetPixel() is also used in CSageBitmap to set the pixel much faster.
//
//  4. MandelBrot Interactive (zoom in/out, resize window, set Julia Set and more) 
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

#include "CSageBox.h"
#include <ccomplex>

int main()

{
    POINT cWinSize      = { 800, 600 };     // Initial Window Size
    CfPoint cfWinSize   = cWinSize;         // Get a floating-point version of window size
 
    // Initial Mandelbrot Values (Center, Range and Iterations)

    CfPoint cfCenter    = { -.6, 0 };       // Initial Mandelbrot Center
    double fRange       = 3.5;              // Initial Range (i.e. "zoom" factor)
    int iMaxIter        = 50;               // Max Mandelbrot Iterations

    CfPoint cfRange     = { fRange, fRange*cfWinSize.y/cfWinSize.x };   // Range based on X-axis

    CfPoint cfD         = cfRange/cfWinSize;                            // Unit Increment for each pixel
    CfPoint cfStart     = cfCenter - cfD*cfWinSize/2;                   // Upper-left X,Y position to start

    // Color Table based on Wikipedia Mandelbrot Colors

    static RGBColor_t rgbColors[16] = 
    {
        { 0, 0, 0       }, { 25, 7, 26     }, { 9, 1, 47      }, { 4, 4, 73      }, 
        { 0, 7, 100     }, { 12, 44, 138   }, { 24, 82, 177   }, { 57, 125, 209  },
        { 134, 181, 229 }, { 211, 236, 248 }, { 241, 233, 191 }, { 248, 201, 95  },
        { 255, 170, 0   }, { 204, 128, 0   }, { 153, 87, 0    }, { 106, 52, 3    },
    }; 

    // Create SageBox and SageBox Window

    CSageBox cSageBox("Basic Mandelbrot (basic, no frills, standard C++ version)");
    
    auto cWin = &cSageBox.NewWindow(100,100,cWinSize.x,cWinSize.y,InnerSize());

    // note: AutoUpdate is on, so the window updates automatically every 10-20ms. When ExitButton() is called, it 
    // performs any pending updates finishing any part of the Mandelbrot output that may not have updated to the window
    // in the last 10-20ms of output.

    for (int i=0;i<cWinSize.y;i++)
    {
        double fy = (double) i*cfD.y + cfStart.y;
        for (int j=0;j<cWinSize.x;j++)
        {
            double fx = (double) j*cfD.x + cfStart.x;

            int iIter = 0;

            // For Julia Set, set z = { fx, fy } and c to a static value, such as (.285, 0) or (-4.,.6) (and set cfCenter to (0,0);

            std::complex<double> c = { fx, fy };
            auto z = c;
            while (abs(z) < 2 && iIter++ < iMaxIter-1) z = z*z + c;

            int iColor = (iIter == iMaxIter) ? 0 : iIter % 16; //iIter*15/(iMaxIter-1);

            // Note: Windows DrawPixel() is slow! See next version ("Faster Mandelbrot") for a better method using
            // a bitmap output that prints a line at a time (using CSageBitmap)

            cWin->DrawPixel(j,i,rgbColors[iColor]); 
        }
    }

    cWin->ExitButton(); // Get user input so the window doesn't close when CSageBox is deleted.

    return 0;
}