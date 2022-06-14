
// -----------------------------------------------
// Sagebox Hello World Example (WinMain() version)
// -----------------------------------------------
//
// This is a Windows Hello World example, which prints Hello world to the console and Sagebox
// Window.  It then displays a circle in the Sagebox Window.
//
// This example is a Pure Windows version, starting at WinMain().
//
// See the Windows Project Template and "Windows" template for an example that uses main() but is also a
// pure Windows Program -- execution from WinMain() is simply transferred to main() for convenience.
//
// About using Sagebox with WinMain()
// ----------------------------------
//
//      Once CSagebox is instantiated, a WinMain() program can run procedurally just like a console-mode (main()) program,
//      but with no console window. 
//
//      A WinMain() program must have a window, which is provided by Sagebox when NewWindow() or AutoWindow() is called.
//
//      A traditional WinMain() program is usually responsible for setting up a thread and a window structure and callbacks. 
//      This is done automatically so you can just start programming procedurally and don't have to do any of this yourself.
//
//      However, Sagebox works well with traditional WinMain() programs and can initiated with a CSagebox instantiation any time
//      during a traditional WinMain program.
//
//      Sagebox features heavy use and support of traditional events, with the Windows MessageHandler class easily implemented
//      to support event-driven uses (or a hybrid of events and procedural programming) with Sagebox.
//
// This is a pure Windows program without a Console Mode. 
//

#include "Sagebox.h"
#include <iostream>     // needed for std::cout

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,_In_ LPSTR lpCmdLineA,_In_ int nCmdShow)
{
    auto& cWin = Sagebox::NewWindow();   // Since its a small app, create static Sagebox and Window at the same time.

    // Set Window title. This is optional. Here, it is helpful to have a display of
    // the differences with version.  CString() is used in Sagebox as stack-and-heap-based
    // string() replacement, for quick string operations with easier usage.
    //
    // It is passed to SetWindowTitle() as a "const char *"
    //
    // This can be done inside of AutoWindow() or NewWindow() with an opt:Title() option.  It is
    // done here for clarity of code. 
   
    cWin.SetWindowTitle(CString() << "Sagebox Windows (WinMain) Hello World Program -- " << cWin.GetVersionInfo()); 

    // Show some different ways to print out the console window and the Sagebox Window
    // Note these comments between Console, Windows, and Sandbox versions. 

    printf("Hello World (printf)\n");           // This will be ignored, since we have no Console Window
    cWin << "Hello World (win)\n";              // Output to the Sagebox window like cout
    std::cout << "Hello World (cout)\n";        // This will be ignored, since we have no Console Window

    cWin.SetFont(100);               // A Shortcut for SetFont("Arial,100");
    cWin.printf("Hello World\n");    // This prints "Hello World" much larger in the Sagebox Window

    // Draw a red circle to show how we can do it.  Instead of GetColor("Red)", rgb value
    // {255,0,0}, a Windows COLORREF, or RgbColor type value may also be used. 

    cWin.FillCircle(300,200,100,PanColor::Red);  

    return cWin.ExitButton();   // We use the exit button (or win.WaitforClose()) to prevent the program from 
                                // terminating and destroying the window since we don't have any other user input.
}