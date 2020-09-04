
// SageBox -- Console Template

#include <iostream>
#include "CSageBox.h"

int main()
{
    CSageBox cSageBox;      // Do this only once
// auto & cWin = cSageBox.ConsoleWin();               
   
    printf("Hello World!\n");    // Output to console.
                                // use cWin.out << Text, cWin.printf(), or cWin.Write() to output to Sagebox Window
    cSageBox.QuickButton("Hello World! Press Button to continue.");
}
