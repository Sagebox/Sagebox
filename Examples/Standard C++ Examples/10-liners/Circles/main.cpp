
// File copyright(c) 2021, Rob Nelson, All rights reserved.  rob@projectsagebox.com
// Sagebox is free for personal use.  website: www.projectsagebox.com -- github repository: https://wwww.github.com/Sagebox/Sagebox

#include "SageBox.h"

// -----------------------
// 10-Line Circles Program
// -----------------------
// 
// Part of some 10-line examples posted on Quora, just for the fun of it.
//
//
// Notes:  The source code is a little 'compressed' fit within 10-lines, but no so much.
//         The main difference between a regular program and a 10-line program is that the
//         range (i & j) other elements are canned where they would normally be variables.
//
// The 10-line example draws 50 or so circles to form a larger circle, kind of like a spirograph.
//
// It shows using Sagebox in a nice graphic setting and also how it works in various ways
//
// Note: This project is set for a Windows program.  You can change it to 
//       a console-mode  program in the Build->Configuration settings
//
int main()
{
    // Create a window of a specifix size.  AutoWindow() also creates a static CSagebox class that
    // we don't use, so we don't need to remember it. 
    //
    // bgGradient() -- Clears the window using a gradient from the first color to the second.

    auto& cWin = Sagebox::NewWindow(bgGradient(PanColor::Black,PanColor::DarkBlue));

    double fR = 150;        // Set a radius. 

    CfPoint pWinSize = cWin.GetWindowSize();        // Get the internal window size
                                                    // GetWindowSize(true) returns entire window size (+frame)

    for (int i=0;i<50;i++)
    {
        double fAngle = (double) i*3.14159/25;

        cWin.SetPenSize(2); 

        // Use a floating-point structure so we can do some math with it. 
        // Similar to MSVC Point but a floating-pointer version

        CfPoint pLoc = pWinSize/2 + CfPoint{ sin(fAngle), cos(fAngle) }*fR;
        cWin.DrawCircle(pLoc,fR,RgbColor::fromHSL(rand() % 360)); 
    }
    return cWin.ExitButton();       // Change to cwin.WaitforClose() to remove button but wait for window close
}
