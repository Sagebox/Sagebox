// ----------------------------------------------------
// Sagebox Quick C++ Console Mode (and Windows) project
// ----------------------------------------------------
//
// This project is a template for using Quick C++ as a Console Mode or Windows program, and as a 
// new project or as part of an existing one. 
//
// This is a pure console mode program, with the entry point at main(). 
// Use the "Windows" project setting to convert this program into a pure Windows program. the WinMain() is called in the WinMain.lib library,
// which transfers control to the main().
//
// Switching between Windows and Console mode is automatic when selecting the project type.
// The only practical result is the removal of the Console Window in Windows mode. 
//
// If you want a pure WinMain() entry, simply remove the WinMain.lib reference in this project, or use the Quick C++ Windows project.
//
// Read "ConsoleTemplate-readme.rtf" (or .txt) in the Project Templates directory for more information.
//
// *** NOTE NOTE NOTE ***
//
// Microsoft Visual Studio (as far as I know) has NO METHOD to set the default project setting in a template.
// 
// For this template, make sure the BUILD configuration settings are for the project you want:
//
//      1. Console Mode (i.e. has a Console Mode Window)            -- Use "Debug" and "Debug Optimized" (x32 or x64)
//      2. Windows (i.e. Pure Windows program, no Console Window)   -- Use "Debug Windows" or "Debug Windows Optimized" (x32 or x64)
//

#include "CQuickCpp.h"

int main()
{
    CreateQuickCpp();       // Start the Sagebox Quick C++ environment & Window (hidden at first)
    
    // Set Window title.  This can be removed or set to something else specific to your program
    // This version sets the template name and shows Sagebox Version information, using CString(),
    // similar to using string/streamstring, etc, but simpler overall. 

    win.SetWindowTitle(CString() << "Sagebox Quick C++ Console Mode/Windows Template -- " << win.GetVersionInfo()); 

    // Show some different ways to print out the console window and also the Sagebox Window
    // The Sagebox window does not appear until first used. 


    printf("Hello World (printf)\n");       // This will go out to the Console Window
    win << "Hello World (win)\n";           // Output to the window like cout
    cout << "Hello World (cout)\n";         // This will also go to the COnsole Window

    win.SetFont(100);               // A Shortcut for SetFont("Arial,100");
    win.printf("Hello World\n");    // This prints "Hello World" much larger in the Quick C++ Window

    // Draw a red circle to show how we can do it.  Instead of GetColor("Red)", rgb value
    // "{255,0,0}", a Windows COLORREF (RGB() macro), or RgbColor type value may also be used. 

    win.DrawCircle(300,200,50,win.GetColor("red"));  

    return win.ExitButton();    // We want the exit button (or win.WaitforClose()) to prevent the program from 
                                // terminating and destroying the window since we don't have any other user input.
}