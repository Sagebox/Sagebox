
// ------------------ 
// SageBox -- Console
// ------------------
//
// This is a template for a Console version of a Win32/Win64 SageBox application.
//

#include <iostream>
#include "CSageBox.h"

int main()
{
    CSageBox cSageBox("SageBox Application");

    auto & cWin = cSageBox.ConsoleWin();                // Make a quick Console Window (use NewWinow for a larger window)
    
    std::cout << "Hello World from the Console!\n\n";   // Otutput to console window
    cWin.out << "Hello World from SageBox!\n\n";        // Output to SageBox Window

    cSageBox.QuickButton("Hello World! Press Button to continue.");
 
    cWin.out << "Click mouse to display location, or close window to exit. {g}Control-C from console window also works!\n\n";

    // Enter the main EventLoop.  See notes on EventLoop() (hold mouse over EventLoop() text)
    // If there is now window, use cSageBox.EventLoop() for various events, exiting when appropriate.
    // also see notes on cSageBox.EventLoop()

    while (cWin.EventLoop())
    {
        POINT pMouse;  
        
        // Check for a mouseclick event.  cWin.MouseClicked() is a shortcut for cWin.event.MouseClicked()

        if (cWin.MouseClicked(pMouse))
        {
            cWin.printf("Mouse Clicked at (%d,%d)\n",pMouse.x,pMouse.y);
            printf("Mouse Clicked at (%d,%d)\n",pMouse.x,pMouse.y);
        }
    }
}
