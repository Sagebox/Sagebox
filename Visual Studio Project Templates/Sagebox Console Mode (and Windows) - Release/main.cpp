
// --------------------------------
// Sagebox -- Console (and Windows)
// --------------------------------
//
// This is a template for a Console version of a Win32/Win64 SageBox application.
// This project can also be used to create a Windows-only app (i.e. no console window)
// by setting the appropriate project settings, listed below.
//
// This includes a Win32 version and Win64 version. 
// Project Types supported are:
//
//  Debug:              Full debugging with no optimization and full stack- and variable-checking
//  Debug Optimized:    Full debugging with no optimization and or stack/variable checking/
//                      This version is for speed, as some debugging can be slow in repetivie calls, etc.
//  Windows:            Windows App instead of console app.  The only difference is that there is no console window (so std::printfs() won't print anything)
//  Windows Optimized:  Same as Debug Optimized, but as a Windows App with no console window
//  
// In most Sagebox examples, Debug is the norm, but some set "Debug Optimized" as the default when speed is a factor.
//
// Note: When the Windows project settings are chosen, WinMain() is already defined and calls main() below. 
//       remove the inclusion of WinMain.lib in the project settings to use your own WinMain()
//
// Note: CSageBox::isConsoleApp() can be returned to determine if this is a Windows App or Console App, so the program
//       may adjust accordingly.
// 
// ************************* SET UP SAGEBOX ROOT DIRECTORY ******************************
// 
// Set the path of the macro ($SageboxRoot) to the Sagebox main directory (i.e. where the Include, Lib, Widgets, and other directories reside).
// 
// To do this, go to View->Other Windows->Property Manager and click on the first project in the window.  Then Click on "Property Sheet" and set 
// User Macros->SageboxRoot to the Sagebox path. By default, this project sets $(SageboxRoot) to the a directory above the current Solution Directory. 
// 
// The "kw::Font(30)" below expresses Sagebox options/keywords. 
// Many examples use "using namespace Sage::kw" so that items may be entered as just "Font(30)" rather than "kw::Font(30)"

#include "Sagebox.h"

// Use int main(int argc, char * argv[]) for canonical approach.  
// When used as a Windows app, WinMain() transfers in these arguments. 

int main()
{
    auto &cWin = Sagebox::NewWindow("My Program");  // Open a sample Window -- Title is not required
    
    cWin.printf("Hello World from the Sagebox Window (printf)\n");
    cWin << kw::Font(30) << "Hello World from the Sagebox Window (C++-style)\n";

    printf("Hello World from the Console Window\n");        // note: does not print when Project is set to Windows

    cWin.WaitforClose();                // Wait for the main window to be closed.  Sagebox::WaitPending() can also be used to look for all windows closed.
    printf("Program Finished.\n");      // Send exit message out to command window (also doesn't print when Project is Windows)
}