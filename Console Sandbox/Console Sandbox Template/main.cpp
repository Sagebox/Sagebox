
// SageBox -- Console Sandbox Template

#include <iostream>
#include "CSageBox.h"

QuickSandbox(CSandbox);

void CSandbox::Main()
{
    out << "Hello World!\n";            // Output to Window
    std::cout << "Hello World!\n";      // Output to console
}

int main()
{
    CSageBox cSageBox;              // Only do this once.
    cSageBox.Main(new CSandbox); 
}
