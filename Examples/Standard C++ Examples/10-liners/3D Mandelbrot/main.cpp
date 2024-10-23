
#include "SageBox.h"

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
// Note: This project is set for a console mode program with optimized code.  You can change it to 
//       a pure Windows program in the Build->Configuration settings
//
int main()
{
    // SetPos(50,20) is used to move the window up -- since we are expanding the vertical size to 1000
    // (from the default 800), the default position of (50,50) is too low, so (50,20) is better. 

    auto& win = Sagebox::NewWindow("Sagebox - 3-D Mandelbrot",kw::SetSize(1300,1000) + kw::SetPos(50,20)); 
    
   //win.SetAutoUpdate();   // Sets the window to update every 10-20ms or so.
   //                        // The default is to not update until called upon to do so, but it's nice to see the
   //                        // output as-it-happens, since this function might take a second or so to complete.
   //
    for (int i=-500;i<500;i++)
        for (int j=-900;j<400;j++)
        { 
            CComplex z{},dz{0,1},c{(double)j/400,(double) i/400};
            int iter = 0;
            while ((z = z*z + c).absSq() < 65536 && ++iter < 250) dz *= z*2;
            CComplex cVec = (z/dz).Normalize();     // Get the light angle 
            win.DrawPixel(j+900,i+500,iter == 250 ? SageColor::Black :  
                RgbColor::fromGray((int) (90*(cVec.real + cVec.imag) + 128)));   // Calculate the diffusion
        }

    return win.ExitButton(); // Wait for the user to hit the button on the botton of the window before closing.
}





