
// SageBox -- Console Sandbox Template

#include <iostream>
#include "CSageBox.h"

QuickSandbox(CSandbox);

void CSandbox::Main()
{
    out << "Hello World!\n";
    Write("Hello World",Font("Arial,100") | CenterXY());

    while (EventLoop());                // Wait for events/window close since there is no automatic button on return.
}

int main()
{
    CSageBox cSageBox;                          // Do this only once.
    cSageBox.NewWindow(new CSandbox).Main();    // Avoids "program finished" button on return/ 
}
