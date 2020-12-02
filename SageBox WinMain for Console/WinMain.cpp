// WinMain.Cpp -- Copyright(c) 2020, 2021 Rob Nelson.  robnelsonxx2@gmail.com -- all rights reserved.
// This file, information, and process within are for personal use only and may not be distributed without permission.
// Please modify, copy, do whatever you want for personal uses.  For professional uses, contact the e-mail address above

// **************************************************************************************************
// WinMain.Cpp -- WinMain Entry point for console-mode SageBox programs using Windows project Setting
// **************************************************************************************************
//
// This file provides an automatic WinMain() that will call the main() in the console-mode-based program.
// It converts the arguments from Windows-style arguments to normal main(int argc,char * argv[]) arguments.
//
// int main(int argc,char * argv[]) must exist in the program's main() declaration. note: it can
// be declared as "int main()"
//
// The Windows project setting for a project that started as a Console Mode program converts the program into
// a valid Windows program, removing the console window. 
//
// As for other values, i.e. instance, nCmdShow, etc.  These can either be obtained easily (such as GetInstance() in Windows), or
// this file can be alterted to store the information somewhere accessible.
//
// To supply your own WinMain(), remove the WinMain.obj reference in the LINKER settings in the project. 
// SageBox also has a Windows set of projects where the program can started in windows and then convert back to 
// a console mode program (i.e. the reverse of this).
//

#include "CSageBox.h"
#include <vector>

extern int main(int argc, char * argv[]);      // external main reference()

// Windows Entry
//
// note: wWinMain does not work when WinMain is in a library.  Therefore, WinMain() is used
//       This routine was originally written for wWinMain() which worked until it was converted to 
//       a .lib format.  The first few lines of code are adjusting for the need to switch it to WinMain()
//       without altering the rest of the code. 

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,_In_ LPSTR lpCmdLineA,_In_ int nCmdShow)
{

    // Fixup from previously using wWinMain() -- convert command line into a wide string. 
    // This is used because (afik) CommandLineToArgvA() does not exist in the MSVC library, and it 
    // is too useful to ignore.

    CStringW wCmdLine   = lpCmdLineA;       // Convert to wchar_t as if we use wWinMain as intended
    wchar_t * lpCmdLine = wCmdLine;         // Assign it to the original lpCmdLine we had before we converted from wWinMain
    
    std::vector<CString> vArgs;

    // --> Continue on as it was originall writtem (i.e. for wWinMain)
    //     This also allows the conversion later to wWinMain by just removing the above.

    char * sAltArgs[1];       // Set a version we know will work (i.e. the command line only) in case of any errors in the process
                              // -- which never happens, but we do it anyway

    int iArgC       = 1;        // Initially one argument for the command line we know we have
    int iNumArgs    = 0;

    char sModulePath[MAX_PATH+1024];

    GetModuleFileNameA(nullptr,sModulePath,MAX_PATH+1023);

    sAltArgs[0] = sModulePath;          // Set up argc = 1 and module path in case we get any errors. 
      
    char * * sOutArgs = sAltArgs;       // Default for error -- send in the command line we already know about 
    
    Mem<char *> sArgs;                  // Get memory for our argv[] list (this is just a placeholder until
                                        // the actual allocation happens below).
                                        // (** this must be before the try or it will go out of scope and we won't know it **)

    // Use an assert() here so we don't need an if() block for the 'unreasonable error'.
    // this method prevents a lot of if() blocks just to check something that shouldn't be happening

    stdTry;
    stdAssert(!wCmdLine.isEmpty(),stdNoMsg)     // It will never fail, but, again, more canonical error-checking
                                                // note: $$ Remove this when/if converted to wWinMain version

    // Initially, we can send the command path, so we initialize with an argc of 1 and the commandline (i.e. no parameters)

    // Get the command line -- in wchar_t * * form -- interestingly, call this function with an empty lpCmdLine, and it incorrectly
    // parses the command path. 

    wchar_t * * wsArgs = EmptyString(lpCmdLine) ? nullptr : CommandLineToArgvW(lpCmdLine,&iNumArgs);

    // Convert arguments to char * to pass, rather than wchar_t *, to fit normal C/C++ main(int argc, char * argv[])
    //
    // This is an ultra-conservative routine, checking for any errors in memory. 
    //
    // Realistically, no error will ever happen, but its good to check anyway for release-software (which this is) to 
    // guard against anomalies, which can happen in production environments, such as when 
    // some of this gets automated through a testing process, etc. 
    //
    // --> In a non-release development situation, we would just assume it all passes, as it would
    // --> be an extremely rare case -- if we have memory issues here, the system is already corrupt
    // --> for reasons outside of our control and preceding our startup, and is going to crash anyway. 

    iNumArgs = min(iNumArgs,10000);     // Protect it against some corrupted number that would be ridiculous, but 
                                        // keep it to something we could probably handle anyway.
                                        // As mentioned, if this were actually needed, the system is 
                                        // about to crash anyway.  This is my embedded systems history at play here.

    // If we have no arguments, just send in the .exe path, which is normal main() input

    if (iNumArgs > 0)
    {
        sArgs = iNumArgs+1+50;      // Allocate what we need.  Normally this could go on the stack, but 
                                    // we want to be prepared for a rogue element (i.e. something perhaps automated)
                                    // that has thousands of elements... Who knows..
                                    // note: 50 is added because when programs check argv[], it is very common to 
                                    // overshoot and check argv[] indexes that aren't valid -- this provides a little margin
                                    // for programs to, say, look for a nullptr or index N in (though not good programming, it does happen)

        sArgs.ClearMem();           // Clear the memory so any indexes that exceed the number returned at least returns a nullptr

        stdAssert(sArgs.isValid(),stdNoMsg);    // Error-out if things don't match up

        sArgs[0] = sModulePath;         // Set the first value, which is the executable path
        vArgs.resize(iNumArgs);         // Get number of CString elements we need -- which is the number of arguments we found. 

        // Again, more memory-checking than we need.  Just being careful.  

        stdAssert(vArgs.size() == iNumArgs,stdNoMsg);   // Fail if for some reason we didn't allocated what we needed with the vector

        iArgC      = iNumArgs+1;        // Set to the number of arguments + 1 for the exe name
        sOutArgs   = sArgs;             // Set to our output now that we know we have the memory we need.

        // CString() memory is already guaranteed, except in the rare case where the 
        // argument exceeds 300 characters (when CString allocates), but we'll just let that one go
        // (we could check CString.isValid() on each call, but all of this memory-checking
        // is already overkill. 

        for (int i=0;i<iNumArgs;i++) vArgs[i] = wsArgs[i];  // Convert the wchar_t * to char *

        // We could have done this with one loop -- without the resize (and using vector.push_back instead), 
        // we couldn't trust the addresses we're storing.  Even with vector.resize() I don't fully trust
        // all std::vector implementations not to change memory addresses as we added CString() elements. 
        // 
        // To be safe, I get the addresses here now that we've allocated all vector strings.

        for (int i=0;i<iNumArgs;i++) sArgs[i+1] = vArgs[i];

    }

    // Call the C/C++ main function()
    //
    // note: the main() being called is assumed to now be more than a console mode program with some form of input
    // and output in a GUI (SageBox or anything that gets out of the console mode, if only Windows MessageBox(), for example),
    // as the console mode window does not exist in this implementation. 
    //
    // Or, if it was a console mode program that just provided a service and no I/O, this is a good way to 
    // leave printf() outputs and such for testing, but get rid of the console mode creation so that it's just a 
    // a pure console mode program.  
    //
    // For example, a console-mode program, once done doing whatever it wants to do, can call MessageBox() to
    // communicate a finished status. Converting it here would remove the Console Mode window while preserving
    // the MessageBox() call.

    stdCatch;       // Nothing to do... but....

    // ...if we have an error thrown by stdAssert(), set the output to the command .exe filepath as in a normal main() 
    // without parameters.  This could be expanded to set a global error so that if a process doesn't get parameters it can't
    // explain, the global error could be checked to make sure parameters were passed correctly.

    if (bError)
    {
        iArgC = 1;              // Reset to 1 parameter and the .exe path, since we know we have these.
        sOutArgs = sAltArgs;
    }
    
    return main(iArgC,sOutArgs);

    // sArgs & vArgs get deleted here automatically

}