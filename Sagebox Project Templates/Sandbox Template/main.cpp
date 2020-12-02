
// ----------------------------------------
// Sagebox -- Sandbox (Console and Windows)
// ----------------------------------------
//
// This is a template for a Sandbox-style Sagebox-based program
//
// A Sandbox program is an easy and fast way to start using Sagebox and protyping simple or more advanced
// programs. 
//
// The program starts immediately in a main() function with a window already created.  The window is local, 
// so std:: functions like printf() work. "in" and "out" may be used as replacements for "cin" and "cout"
//
// std::printf() may be used to print to the console window if one exists.
//
// See program below for more details.
//
// As with the "Console Mode and Windows Only", this program may be set to be a Console Mode or Windows program. 
// As a Sandbox program, it starts with a SageBox window pre-created, however.
//
// note: See project settings to choose Console Mode vs. a Windows program.  Also see the 
//       project "Console Mode and Windows Only" for regular main()-type and WinMain()-type programs.

#include "CSageBox.h"
#include <iostream>

QuickSandbox(CSandbox)          // Create the sandbox class. 
                                // This can be expanded on by copying the QuickSandbox macro
                                // and filling in your class members.
                                // You can change CSandbox to your class name.


// ------------------
// Main Program Start 
// ------------------
//
void CSandbox::Main()
{
    // A few ways to print hello world. 

    printf("Hello World!\n");                                           // Regular printf (to the window)
    Write("Hello World\n");                                             // Same as printf, but can be faster
    Write("Hello World\n",Font("Arial,26") | bgColor("red"));           // Add some parameters to control the output

    out << "Hello World\n";                                             // just like cout
    out << Font("Arial,35") << fgColor("Yellow") << "Hello World\n";    // like cout, but adding some attributes.

    std::printf("Hello World\n");      // Print out to the console window (if it exists)
    cout << "Hello World\n";           // Also print out to the console window (if it exists)

    // Note the program just ends -- there is no need to check the GetEvent() or WaitforWindowClose()
    // This program is meant to act like a regular console-based main() function, but with the features of Sagebox.
    //
    // The cSageBox.Main() call prints an automatic ExitButton to keep from automatically exiting and destroying the window.
}

// std main() -- this simply transfers to the CSandbox::Main() function. 
//               (if the project setting is for Windows, WinMain() transfers to here automatically)
int main()
{
    CSageBox cSageBox("Sagebox -- Sandbox Program");    // Open Sagebox, titling program cSageBox() may also be used for no title 
    cSageBox.Main(new CSandbox);                        // Specify cSageBox.NewWindow(new CSandbox).Main() to avoid exit button on exit. 

    return 0;

}