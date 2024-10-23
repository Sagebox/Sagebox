
#include "Sagebox.h"

// -----------------------
// 10-Line Julia Output #1
// -----------------------
// 
// Part of some 10-line examples posted on Quora, just for the fun of it.
//
//
// Notes:  The source code is a little 'compressed' fit within 10-lines, but no so much.
//         The main difference between a regular program and a 10-line program is that the
//         range (i & j) other elements are canned where they would normally be variables.
//
// The 10-line example draw a nice red & cyan spiral-like Julia set.
//
// It shows using Sagebox in a nice graphic setting and also how it works in various ways
//
// This Julia set is nearly the same as the 10-line Mandelbrot except that z changes rather than c, 
// and the Range and Color are different -- it also doesn't calculate a diffusion value, using the 
// result directly.  Otherwise, it is the same code.
//
// Note: This project is set for a console mode program with optimized code.  You can change it to 
//       a pure Windows program in the Build->Configuration settings
//
int main()
{
    // Create a window of a specifix size.  AutoWindow() also creates a static CSagebox class that
    // we don't use, so we don't need to remember it. 
    
    auto& win = Sagebox::NewWindow("Sagebox - Julia Set 1",kw::SetSize(800,1000) + kw::SetPos(50,20));

    auto maxIter = 50; 
     
    for (int i=-500;i<500;i++)
        for (int j=-400;j<400;j++) 
        { 
            CComplex z((double)j/400,(double) i/400),c(.285,0.0);
            int iter = 0; 
            while (z.abs() < 65536 && iter++ < maxIter) z = z*z + c;
            
            // Get smooth color
            
            auto value = (int) (255.0*(((double) iter)+1 - log2(log2(z.absSq())/2))/maxIter);

            win.DrawPixel(j+400,i+500,{min(255,value*2),max(0,(255-value)*2),max(0,(255-value)*2)});
        }

    return Sagebox::ExitButton();     // Wait for the user to close the window.
}

