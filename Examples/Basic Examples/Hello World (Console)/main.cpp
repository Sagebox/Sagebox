// -------------------------------------
// Sagebox Hello World Example (Console)
// -------------------------------------
//
// This is a Console-Mode Hello World example, which prints Hello world to the console and Sagebox
// Window.  It then displays a circle in the Sagebox Window.
//
// ** This program is Console-Mode only and does not have a Windows option.  See the "Hello World (Windows)" 
//    and "Hello World (WinMain)" examples.
//
// ** Note: When the window comes up, it may cover the Console Window.  SImply move the window to uncover it.

#include "Sagebox.h"
#include <iostream>     // Needed for cout

int main()
{
    auto & cWin = Sagebox::NewWindow();   // Since its a small app, create static Sagebox and Window at the same time.

    // Set Window title. This is optional. Here, it is helpful to have a display of
    // the differences with version.  CString() is used in Sagebox as stack-and-heap-based
    // string() replacement, for quick string operations with easier usage.
    //
    // It is passed to SetWindowTitle() as a "const char *"
    //
    // We could have done this with an opt:Title() option in AutoWindow(), but do it here for clarity.

    cWin.SetWindowTitle(CString() << "Sagebox Console-Mode Hello World Program -- " << cWin.GetVersionInfo()); 

    // Show some different ways to print out the console window and the Sagebox Window
    // Note these comments between Console, Windows, and Sandbox versions. 

    printf("Hello World (printf)\n");       // This will go out to Console Window (stdio)
    cWin << "Hello World (win)\n";          // Output to the Sagebox window like cout
    std::cout << "Hello World (cout)\n";    // cout also goes to the Console Window (stdio)

    cWin.SetFont(100);               // A Shortcut for SetFont("Arial,100");
    cWin.printf("Hello World\n");    // This prints "Hello World" much larger in the Sagebox Window

    // Draw a red circle to show how we can do it.  Instead of GetColor("Red)", rgb value
    // {255,0,0}, a Windows COLORREF, or RgbColor type value may also be used. 

    cWin.FillCircle(300,200,100,PanColor::Red);  

    return cWin.ExitButton();    // We use the exit button (or win.WaitforClose()) to prevent the program from 
                                // terminating and destroying the window since we don't have any other user input.
}