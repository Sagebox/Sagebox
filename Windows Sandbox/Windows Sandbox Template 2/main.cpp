
// SageBox -- Windows Sandbox Template

#include <iostream>
#include "CSageBox.h"

QuickSandbox(CSandbox);

void CSandbox::Main()
{
    out << "Hello World!\n";
    Write("Hello World",Font("Arial,100") | CenterXY());

    while (EventLoop());                // Wait for events/window close since there is no automatic button on return.
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR lpCmdLine,_In_ int nCmdShow)
{
    CSageBox cSageBox;                          // Do this only once.
    cSageBox.NewWindow(new CSandbox).Main();    // Avoids "program finished" button on return/ 
}
