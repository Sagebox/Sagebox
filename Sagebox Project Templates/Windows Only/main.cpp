
// --------------------------------
// SageBox -- Console (and Windows)
// --------------------------------
//
// This is a template for a Console version of a Win32/Win64 SageBox application.
// This project can also be used to create a Windows-only app (i.e. no console window)
// by setting the appropriate project settings, listed below.
//
// This includes a Win32 version and Win64 version. 
// Project Types supported are:
//

//  Windows:            Windows App instead of console app.  The only difference is that there is no console window (so std::printfs() won't print anything)
//  Windows Optimized:  Same as Debug Optimized, but as a Windows App with no console window
//  
// In most SageBox examples, Debug is the norm, but some set "Debug Optimized" as the default when speed is a factor.
//
// Note: Windows programs can also be built in the "Console mode and Windows Only" project -- programs built with this
//       project can be built in both Console Mode or as a Windows program, with WinMain() included (calls main()).  
//

#include "CSageBox.h"

// WinMain() -- Main Entry Point for Windows Applications
//
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR lpCmdLine,_In_ int nCmdShow)
{
    CSageBox cSageBox;                              // Create the main SageBox object.  Do this only once.
    auto& cWin = cSageBox.NewWindow();              // Create as many of these as you like.
 
    cWin.printf("Hello World! -- Press Window Close Button to End Program.\n");
    cWin.Write("Hello World!",Font("Arial,100") << CenterXY());

    while(cWin.GetEvent());     // Wait for window to close.  cWin.WaitforClose() can also be called without the while loop. 
}
