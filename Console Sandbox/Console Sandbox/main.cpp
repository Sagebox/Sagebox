// -------------------------- 
// SageBox -- Console Sandbox
// --------------------------
//
// This is a template for a Sandbox version of a Win32/Win64 SageBox application.
//
// For documentation and help on this source file, right-click on "SandboxHelp.h" and select "Go do document WinSandbox.h"
//

#include <iostream>
#include "CSageBox.h"
#include "SandboxHelp.h"

QuickSandbox(CSandbox);

// CSandbox::Main() -- Main entry point for Sandbox
//
void CSandbox::Main()
{
    out << "Hello World!\n";
    std::cout << "Hello World!\n";

    Write("Hello World",Font("Arial,100") | CenterXY());
}


// WinMain() -- Main Entry Point for Windows Applications
//
int main()
{
    CSageBox cSageBox;
    cSageBox.Main(new CSandbox,"Sandbox Window");
}
