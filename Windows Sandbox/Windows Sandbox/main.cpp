
// SageBox -- Windows Sandbox

#include <iostream>
#include "CSageBox.h"

QuickSandbox(CSandbox);

void CSandbox::Main()
{
    out << "Hello World!\n";
    Write("Hello World",Font("Arial,100") | CenterXY());
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR lpCmdLine,_In_ int nCmdShow)
{
    CSageBox cSageBox;          // Do this only once.
    cSageBox.Main(new CSandbox);
}
