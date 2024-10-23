
// This is a sample project for the "My Projects" directory.
//
// This program draws a red square with a white outline on the screen, moving the rectangle 
// to the mouse coordinates when the mouse is moved and the mouse button is pressed.
//
// This was created with the "Console Mode and Windows Only" template. 
//
// The project SageBox templates will work for this structure, where:
// You can copy this directory structure (and remove this sample project directory) to have a clean Sagebox project structure.
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

#include "Sagebox.h"

// Use int main(int argc, char * argv[]) for canonical approach.  
// When used as a Windows app, WinMain() transfers in these arguments.
//
int main()
{
    auto &cWin = Sagebox::NewWindow("Sagebox - Square Program",kw::NoClose());      // Open a sample Window
                                                                                // NoClose() -- disallows window closing if the user presses the "X"/Windows close
                                                                                // button in the upper-right.  This allows us to put the Exit Button out (otherwise it would fall
                                                                                // through).  If we didn't do this we could also call cWin.ResetWindowClosing()

    cWin.Cls(SageColor::SkyBlueDark,SageColor::SkyBlueLight);         // We can also do Cls({100,200,100}) (or similar) for RGB values rather than stock colors

    CPoint szWinSize = cWin.GetWindowSize();        // Get in a CPoint (GetWindowSize() returns a SIZE) so we can use it later when we call DrawRect()
                                                    // We could just use this inline below, I left it here for clarity of code.

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
        cWin.FillRectangle(pLoc.x-100,pLoc.y-100,200,200,"red",kw::PenColor("white"));  // Can also use PanColor::Red and PanColor::White, respectively.
    };

    DrawRect(szWinSize/2);      // Draw initial rectangle in the center (CPoint will divide 'x and 'y' element by 2 here)

    // Event Loop -- Wait for a mouse movement with the mouse button down, then draw a rectangle
    // wherever the mouse is. 
    //
    // CloseButtonPressed() is called because we told the window to not allow automatic closing (in which 
    // case GetEvent() would return falls and break the loop) -- without that, we need to know when to exit on
    // our own (via CloseButtonPressed()) or some other event (such as a button, etc.)

    while(cWin.GetEvent() && !cWin.CloseButtonPressed())
    {
        // Put event-related things here, or use an event-driven system by subclassing the window to get events directly.

        if (cWin.MouseDragEvent(true)) DrawRect(cWin.GetMousePos());
    }

    return cWin.ExitButton();       // Put a button at the bottom of the screen to inform the user we're done.
                                    // We could just exit, but its nice to know the program got our close-button click.
                                    // Exit Button() returns 0
}