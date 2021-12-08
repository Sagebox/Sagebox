// ---------------------------------------------------------
// Sagebox Hello World Example (Windows via Console Project)
// ---------------------------------------------------------
//
// This is a Windows Hello World example, which prints Hello world to the Sagebox Window.  
// It then displays a circle in the Sagebox Window.
//
// This example is exactly the same as the Console version (including the use of main() as the entry point), except
// that WinMain() is called first as a Windows program -- it then transfers control to main() for the ease of switching
// between Console and Windows programs.
//
// See the Windows Project Template and "Windows (WinMain)" template for an example that uses WinMain()
//
//      This is a pure Windows program without a Console Mode.  The "Console Mode and Windows" templates provide both
//      versions so it is easy to switch back-and-forth to Windows or Console Mode, which allows three things:
//
//           1. Using Console Mode as a debug output for a Windows program.  When in Windows mode, Console Mode output
//              is ignored and not seen
//           2. Writing a Windows program while initially working in Console Mode for debug, ease of debugging, etc.
//           3. Writing pure Windows programs, in general.  The pure Windows template (which calls WinMain() directly) 
//              does this, too.
//
//              Using the Console Mode and then switching to Windows shows how easy writing Windows programs can be. 
//              The only difference between a pure Windows program and Console Mode program is the Console Window, 
//              which is just a conveniently-provided Windows window operated by another process. 
//
// When CreateQuickCpp() is called, a Sagebox window is created as hidden and then can be used as a global "win"
// variable.  The Sagebox Window is not visible until it is used for something that causes a display, and may
// be permanently hidden with win.Hide(), and then shown when you're ready with win.Show(). 
//
// ** This is a Pure Windows program, using the WinMain.lib to transfer control to main(). 
//    With the "Console Mode (and Windows)" project template, you can switch between a Console Mode Program and
//    a Pure Windows program with a Build->Configuration switch without changing code.
//
//    See the project "Hello World (WinMain)" for an example starting at WinMain()
//

#include "Sagebox.h"
#include <iostream>     // needed for std::cout

int main()
{
   auto & cWin = Sagebox::NewWindow();   // Since its a small app, create static Sagebox and Window at the same time.

    // Set Window title. This is optional. Here, it is helpful to have a display of
    // the differences with version.  CString() is used in Sagebox as stack-and-heap-based
    // string() replacement, for quick string operations with easier usage.
    //
    // It is passed to SetWindowTitle() as a "const char *"
    //c
    // The Window Title is set here, though it could have een set in AutoWindow() with an opt::Title() option.
    // It is done here for clarity of code. 

    cWin.SetWindowTitle(CString() << "Sagebox Windows Hello World -- " << cWin.GetVersionInfo()); 

    // Show some different ways to print out the console window and the Sagebox Window
    // Note these comments between Console, Windows, and Sandbox versions. 


    printf("Hello World (printf)\n");           // This will be ignored, since we have no Console Window
    cWin << "Hello World (win)\n";              // Output to the Sagebox window like cout
    std::cout << "Hello World (cout)\n";        // This will be ignored, since we have no Console Window

    cWin.SetFont(100);               // A Shortcut for SetFont("Arial,100");
    cWin.printf("Hello World\n");    // This prints "Hello World" much larger in the Sagebox Window

    // Draw a red circle to show how we can do it.  Instead of GetColor("Red)", rgb value
    // {255,0,0}, a Windows COLORREF, or RgbColor type value may also be used. 

    cWin.DrawCircle(300,200,100,PanColor::Red);  

    return cWin.ExitButton();    // We use the exit button (or win.WaitforClose()) to prevent the program from 
                                // terminating and destroying the window since we don't have any other user input.
}