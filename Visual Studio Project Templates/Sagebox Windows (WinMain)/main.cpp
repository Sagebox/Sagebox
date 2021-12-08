
// -----------------------
// SageBox -- Pure Windows
// -----------------------
//
// This is a template for a Windows version of a Win32/Win64 SageBox application.
//
// This includes a Win32 version and Win64 version. 
//
// Project Types supported are:
//
//
//  Windows:            Windows App instead of console app.  The only difference is that there is no console window (so std::printfs() won't print anything)
//  Windows Optimized:  Same as Debug Optimized, but as a Windows App with no console window
//  
// In most SageBox examples, Debug is the norm, but some set "Debug Optimized" as the default when speed is a factor.
//
// Note: Windows programs can also be built in the "Console mode and Window" project -- programs built with this
//       project can be built in both Console Mode or as a Windows program, with WinMain() included (calls main()).  
//
//
// ************************* SET UP SAGEBOX ROOT DIRECTORY ******************************
// 
// Set the path of the macro ($SageboxRoot) to the Sagebox main directory (i.e. where the Include, Lib, Widgets, and other directories reside).
// 
// To do this, go to View->Other Windows->Property Manager and click on the first project in the window.  Then Click on "Property Sheet" and set 
// User Macros->SageboxRoot to the Sagebox path. By default, this project sets $(SageboxRoot) to the a directory above the current Solution Directory. 
// 

#include "Sagebox.h"

// WinMain() -- Main Entry Point for Windows Applications
//
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR lpCmdLine,_In_ int nCmdShow)
{
    auto& cWin = Sagebox::NewWindow();              // Create as many of these as you like.
 
    cWin.printf("Hello World! -- Press Window Close Button to End Program. (printf)\n");
    cWin << Font(30) << "Hello World! -- Press Window Close Button to End Program. (C++-Style)\n";

    return cWin.ExitButton();   // Put up an exit button and wait for the button press.
                                // cWin.WaitforClose() can also be called without the while loop. 
                                // ExitButton() returns 0
}
