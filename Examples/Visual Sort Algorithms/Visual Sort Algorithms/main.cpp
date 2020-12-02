
// ------------------ 
// SageBox -- Console
// ------------------
//
// This is a template for a Console version of a Win32/Win64 SageBox application.
//
// This includes a Win32 version and Win64 version. 
// Project Types supported are:
//
//  Debug:  Full debugging with no optimization and full stack- and variable-checking
//  Debug Optimized: Full debugging with no optimization and or stack/variable checking/
//                   This version is for speed, as some debugging can be slow in repetivie calls, etc.
//
//  In most SageBox examples, Debug is the norm, but some set "Debug Optimized" as the default when speed is a factor.

#include "CSageBox.h"
#include "CVisualSort.h"

int main(int argc,char * argv[])
{
    bool bConsoleApp = CSageBox::isConsoleApp();

    CVisualSort cSort;
    cSort.main(bConsoleApp);
}

