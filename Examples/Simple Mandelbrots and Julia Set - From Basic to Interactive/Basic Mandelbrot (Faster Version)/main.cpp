// Basic Mandelbrot Faster -- Copyright(c) 2020, 2021 Rob Nelson.  robnelsonxx2@gmail.com -- all rights reserved.
// This file, information, and process within are for personal use only and may not be distributed without permission.
// Please modify, copy, do whatever you want for personal uses.  For professional, distribution or commercial uses, 
// contact the e-mail address above

// ***********************
// Basic Mandelbrot Faster
// ***********************
//
// This set of Mandelbrot programs shows creating and displaying a Mandelbrot and/or Julia Set 
// in various stages, from very basic, to faster, all the way to an interactive program.
//
// This is #2 Smooth Color, Basic Mandelbrot Faster
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
// --------------------------------------------
// About This Version (Basic Mandelbrot Faster)
// --------------------------------------------
//
//  This version is an extension of the Basic Mandelbrot, change a small amount of code to make the Mandelbrot generation
//  and display 5x faster or more (actually, I think much more)
//
//  As with many std namespace functions, std::complex can be  slow.  This is replace with CComplex, which is a small (and growing)
//  set of complex functions that inlines code directly, making it just about as fast as expanding the complex functions into vector-based math
//  but without the headache. 
//
//  DrawPixel (which calls Windows::SetPixel) is replaced with a CSageBitmap::SetPixel call.  Windows::SetPixel is very, very slow!
//  It is usually never a good idea to use Windows::Setpixel (accessed through Sagebox CWindow::DrawPixel()) for general output.
//
//  Instead, putting values out to a bitmap is a much faster way to go.  
//
//  Using CComplex
//
//     1. All CComplex functions are inline, meaning there are not calls for general routines to flush the processor cache, on of the things that
//        makes it faster than CComplex.  Some calls are made, only as required, such as sqrt() for the abs() function, and so-forth.
//
//     2. Note use of z.absSq().  We can't use abs(z) or because it's not a template, so we must us z.abs() instead as a member function.
//        In this case, we use absSq() that allows us to get the magnitude^2 without calling a sqrt() which makes it much faster. 
//
//
//  Run the program and look at the source code fore more information and comments.
//
//  I expect to do a video series on this set of programs and will explain more in-depth about the use of SageBox in
//  the videos.
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

    CSageBox cSageBox("Basic Mandelbrot Plus (Faster Mandelbrot using CCompled and CSageBitmap)");
    
    auto cWin = &cSageBox.NewWindow(100,100,cWinSize.x,cWinSize.y,InnerSize());

    CSageBitmap cBitmap = cWin->CreateBitmap(cWinSize.x);       // Create bitmap of (Width,1) --> Returns a CSageBitmap object

    for (int i=0;i<cWinSize.y;i++)
    {
        double fy = (double) i*cfD.y + cfStart.y;
        for (int j=0;j<cWinSize.x;j++)
        {
            double fx = (double) j*cfD.x + cfStart.x;

            int iIter = 0;

            // For Julia Set, set z = { fx, fy } and c to a static value, such as (.285, 0) or (-4.,.6) (and set cfCenter to (0,0);

            CComplex c = { fx, fy };    
            auto z = c;
            while (z.absSq() < 4 && iIter++ < iMaxIter-1) z = z*z + c; 

            int iColor = (iIter == iMaxIter) ? 0 : iIter % 16;  //iIter*15/(iMaxIter-1);

            cBitmap.SetPixel(j,rgbColors[iColor]);              // Set the pixel in the one-line bitmap.
        }
        cWin->DisplayBitmap(0,i,cBitmap);                       // Print the one-line bitmap to the Y row in the window.
    }

    cWin->ExitButton(); // Get user input so the window doesn't close when CSageBox is deleted.

    return 0;
}