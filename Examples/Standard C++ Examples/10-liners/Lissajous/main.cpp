
// File copyright(c) 2021, Rob Nelson, All rights reserved.  rob@projectsagebox.com
// Sagebox is free for personal use.  website: www.projectsagebox.com -- github repository: https://wwww.github.com/Sagebox/Sagebox

#include "CSageBox.h"

// -------------------------
// 10-Line Lissajous Program
// -------------------------
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
// Note: This project is set for a Windows program.  You can change it to 
//       a console-mode  program in the Build->Configuration settings
//

// Lissajous -- 10 Line version
//
int main()
{
    auto& cWin = CSagebox::AutoWindow(CSize(850,850), Title("10-Line Liassjous")); 

    double fFreq = 3.01;               // Get initial frequency multiplier

    // Main loop.  Draw the initial Lissajous and then repeat while the user is entering numbers
    // note: The windows auto-updates about every 10-20ms, and updates any leftover items when GetFloat()
    // is called (All sagebox UI functions update the UI before continuing to flush the display buffer).

    do
    {      
        cWin.Cls(SageColor::Black,SageColor::SkyBlueDark);        // Clear screen with a gradient
         
        for (double i=0;i<6240;i++)
        { 
            // Use the CfPoint structure for some easy point-based math.

            CfPoint pLoc = { cos(-1.57+i/100*fFreq), sin(-1.57+i/100) };
            auto rgbColor = CTools::HSLtoRGB({fmod(i/315,1),1,.5});
            cWin.DrawCircle(pLoc*400+425,2,rgbColor);
        }
    }
    while(cWin.dialog.GetFloat("+Lissajous\nEnter Frequency Multiplier\nPress Cancel to quit",
                                                                            fFreq,Default(fFreq)));

    // In the GetFloat() call above:
    //
    // "+Lissajous"         -- When the first item starts with a '+', this specifies the title of the window
    // The first "fFreq"    --  specified where to put the value
    // Default(fFreq)       -- Specifies the default for the input box.

    return 0;       // Just return.  We don't need to wait for the window to close since the user canceled.
}