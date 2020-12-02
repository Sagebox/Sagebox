
// This is a sample project for the "My Projects" directory.
// This was created with the "Console Mode and Windows Only" template. 
//
// All provided project templates will work for this structure, where:
//
//  The top-level project or solution directory (i.e. "Sample Project") is in the same directory as
//
//      lib
//      include
//      Widgets
//
// When you load a project in this manner (no matter the root directory structure underneath), the Sagebox program will
// compile and run right away with the sample code provided in the project template.
//
// In this case (spawning from the "Console Mode and Windows Only" project), you can run a Console Mode or Window mode program
// in Win32 or Win64. Look at Build->Build Configurations to set the project configuration

#include "CSageBox.h"

// Use int main(int argc, char * argv[]) for canonical approach.  
// When used as a Windows app, WinMain() transfers in these arguments.
//
int main()
{
    CSageBox cSageBox("My Program");                // Open SageBox, titling program cSageBox() may also be used for no title 
    auto &cWin = cSageBox.NewWindow(NoClose());     // Open a sample Window
                                                    // NoClose() -- disallows window closing if the user presses the "X"/Windows close
                                                    // button in the upper-right.  This allows us to put the Exit Button out (otherwise it would fall
                                                    // through).  If we didn't do this we could also call cWin.ResetWindowClosing()
    
    cWin.Cls("SkyBlueDark","SkyBlueLight");         // We can also do Cls({100,200,100}) (or similar) for RGB values rather than stock colors

    SIZE szWinSize = cWin.GetWindowSize();

    // Draw a red rectangle (with white border) on the screen
    // It's a lambda so we can use it twice without having to move it out to an external function
    //
    // note: AutoUpdates are on by default, so this will update on its own -- you may see an occasional flicker because of it.
    //       The way to be more specific (and avoid flickering) is to set AutoUpdates to OFF and then use Update() to update every 
    //       rectangle draw (i.e. put it right after DrawRectangle(), or whenever you're done with drawing)
    //
    // Another way is to call DontUpdate() before the Cls() and then Update() after that last draw function (i.e. DrawRectangle), 
    // which allows a period of non-updating even though the // AutoUpdate is on -- the AutoUpdate resumes when it sees the first 
    // Update() call after DontUpdate(). 
    //
    // In this case, we don't really care because it works fine, as it does for most things.

    auto DrawRect = [&](POINT pLoc)
    {
        cWin.Cls();     // Not the most efficient compared to a region, but good enough
        cWin.DrawRectangle(pLoc.x,pLoc.y,200,200,cWin.GetColor("Red"),cWin.GetColor("White")); 
    };

    DrawRect({szWinSize.cx/2-100,szWinSize.cy/2-100});      // Draw initial rectangle in the center

    // Event Loop -- Wait for a mouse movement with the mouse button down, then draw a rectangle
    // wherever the mouse is. 
    //
    // CloseButtonPressed() is called because we told the window to not allow automatic closing (in which 
    // case GetEvent() would return falls and break the loop) -- without that, we need to know when to exit on
    // our own (via CloseButtonPressed()) or some other event (such as a button, etc.)

    while(cWin.GetEvent() && !cWin.CloseButtonPressed())
    {
        POINT pMouse;
        if (cWin.MouseMoved(pMouse) && cWin.MouseButtonDown()) DrawRect(CPoint(-150,-150) + pMouse);
    }

    cWin.ExitButton();      // Put a button at the bottom of the screen to inform the user we're done.
                            // We could just exit, but its nice to know the program got our message
}