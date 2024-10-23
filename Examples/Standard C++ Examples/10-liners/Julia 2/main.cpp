
#include "SageBox.h"

using namespace Sage::kw;  // Sagebox Keyword Options

// -----------------------
// 10-Line Julia Output #2
// -----------------------
// 
// Part of some 10-line examples posted on Quora, just for the fun of it.
//
//
// Notes:  The source code is a little 'compressed' fit within 10-lines, but no so much.
//         The main difference between a regular program and a 10-line program is that the
//         range (i & j) other elements are canned where they would normally be variables.
//
// The 10-line example draw a nice blue & orange-yellow spiral-like Julia set.
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
    using namespace kw;     // So we don't have to use kw:: for options, e.g. kw::bgColor() as used below. 
    
    // Create a window of a specific size.  
    // Use bgColor() to set the base color to RGB(0,0,128), since 0 values in the main loop will be that value. 
    // --> This lets the textwidget blend.  Otherwise, we can just add the text widget later.
    //
    // kw::SetPos(50,20) is used because the default position of (50,50) causes the window to run off the bottom of a 1920x1080 screen a little.

    auto& win = SageBox::NewWindow("Sagebox - Julia Set 2",SetSize(1200,1000) + bgColor({0,0,128}) + SetPos(50,20));

    //win.SetAutoUpdate();    // Sets the window to update every 10-20ms or so.
                            // The default is to not update until called upon to do so, but it's nice to see the
                            // output as-it-happens, since this function might take a second or so to complete.

    win.TextWidget(0,10,"Julia Set (-.4, .6)",Font("Arial,50") | JustCenterX());

    for (int i=-500;i<500;i++)
        for (int j=-600;j<600;j++)
        { 
            CComplex z{(double)j/400,(double) i/400},c{-.4,.6};
            double iter = 0;
            while (z.abs() < 65536 && iter++ < 450) z = z*z + c;

            // Get a smooth color value

            auto value = (int) (255.0*(iter+1 - log2(log2(z.absSq())/2))/450);

           win.DrawPixel(j+600,i+500,{min(255,value*4),min(255,value*2),(255-value)/2});
        }

        // Since we have an extra line to keep within 10 lines of code, add a Label the image. 
        // Transparent() blends Background, JustCenterX() centers the message

    return Sagebox::ExitButton();
}
 