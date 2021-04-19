
// File copyright(c) 2021, Rob Nelson, All rights reserved.  rob@projectsagebox.com
// Sagebox is free for personal use.  website: www.projectsagebox.com -- github repository: https://wwww.github.com/Sagebox/Sagebox

#include "CSageBox.h"

// ---------------------
// 10-Line 3D-Mandelbrot
// ---------------------
// 
// Part of some 10-line examples posted on Quora, just for the fun of it.
//
//
// Notes:  The source code is a little 'compressed' fit within 10-lines, but no so much.
//         The main difference between a regular program and a 10-line program is that the
//         range (i & j) other elements are canned where they would normally be variables.
//
// The 10-line example show using Sagebox in a nice graphic setting and also how it works in 
// various ways
//
// Note: This uses DrawPixel() which is a Windows function and is slow! 
//       The examples in the Simple Mandelbrot Examples (except for the first one) use a bitmap
//       which is about 10 times faster. When the display is slow, it ist mostly due to 
//       DrawPixel(), but in the black areas it may be a little slow due the main loop.
//
// Note: This project is set for a console mode program with optimized code.  You can change it to 
//       a pure Windows program in the Build->Configuration settings
//
int main()
{
    // Create a window of a specific size.  AutoWindow() also creates a static CSagebox class that
    // we don't use, so we don't need to remember it. 

    auto& cWin = CSagebox::AutoWindow(CSize(1300,1000)); 
    
    for (int i=-500;i<500;i++)
        for (int j=-900;j<400;j++)
        { 
            CComplex z{},dz{0,1},c{(double)j/400,(double) i/400};
            int iIter = 0;
            while ((z = z*z + c).absSq() < 65536 && ++iIter < 250) dz *= z*2;
            CComplex cVec = (z/dz).Normalize();     // Get the light angle 
            cWin.DrawPixel(j+900,i+500,iIter == 250 ? 0 :  
                RGBColor_t().fromGray((int) (90*(cVec.fR + cVec.fI) + 128)));   // Calculate the diffusion
        }

    return cWin.WaitforClose(); // Wait for the user to close the window
}





