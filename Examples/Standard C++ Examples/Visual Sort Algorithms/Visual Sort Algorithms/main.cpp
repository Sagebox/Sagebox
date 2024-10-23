
// ----------------------------------------------
// SageBox Visual Sorting Algorithm Demonstration
// ----------------------------------------------
//
// This program uses SageBox to display various sorting algorithms.  These algorithms were taken directly from sources on the Internet (CodeProject, I think) 
// copied without changes into the Sagebox program.
//
// See notes in CVisualSort.cpp for more information.
//
// Note: This program works as a Console Mode or Pure Windows program, depending on the Build Configuration.
//

#include "Sagebox.h"
#include "CVisualSort.h"

int main(int argc,char * argv[])
{

    // Check if we're a console app.  We a couple things differently in the 
    // main program if we're Console or Windows
    //
    // There is no real reason to do it her vs. inside of CVisualSort, since
    // CVisualSort::main() acts as a regular main.  
    //
    // isConsoleApp() is done here main to show it exists as a Sagebox function, and
    // that we can determine this before we create the CSagebox object, whichis createed
    // in CVisualSort::main().

    bool consoleApp = SageBox::isConsoleApp();

    CVisualSort sort;
    sort.main(consoleApp);    
}

