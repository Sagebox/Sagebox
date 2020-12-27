// This file copyright(c) 2021 Rob Nelson, All Rights Reserved.    E-mail rob@projectsagebox.com for more information.
//

// --------------------------------------------------------------------------------------------------------------
// PRELIMINARY VERSION.  This file has been brought into the Sagebox project from the original sources and has 
// not yet been commented for Sagebox, or properly formatted (I used tabs and am converting to spaces).
// --------------------------------------------------------------------------------------------------------------
//
// ****** UNDER CONSTRUCTION ******
//
// This file is still under construction and may not yet include specifics, awaiting proper testing and integration into Sagebox.
//

#pragma once

/* 

---------------
Windows Sandbox
---------------

The Windows Sanbox is a Windows program that allows you to use SageBox as if you're more in a console mode with the power of Windows.
Since CSandbox::Main() is in a CWindow subclass, all functions are avaiable without specifying an object or namespace.

For example, you can simply use functions like:

    out << "Hello World\n";
    printf("This is item %d\n",iItemNo);
    MyValue = dialog.getInteger();
    console.getline(MyString);

as well as the more powerful windows functions such as creating dialogs, new windows, bitmap windows, buttons, sliders, listboxes and more.


This is a great ad-hoc way to create quick projects and then move them into more streamlined projects, or just to play around with SageBox or learn 
how to work with Windows, hence the name SandBox for this mode.

-----------------------------------
Windows Sandbox vs. Console Sandbox
-----------------------------------

With Sagebox, there is really no difference betwen the console versions and Windows versions of Sagebox, except that the Console version
offers a console window and can be mixed with regular console mode programming.

It is easier (and recommended) to first use Sagebox in Console mode because console mode is very easy to deal with in the case of a
runaway or hung program -- you can just press Control-C in the console window and get out of the window part of the program.

Since there is no other difference, the program can be instantly moved to a pure Windows program simply by moving the code
into a Windows SageBox project, without changing any SageBox-related code.

----------------
64-bit vs 32-bit
----------------

There is currently no difference betweem 64-bit and 32-bit versions of Sagebox, except that the 64-bit version has not been as thoroughly tested as 
the 32-bit version, and there are still a couple issues in-progress.

Otherwise, there should be no difference.  At the moment, however, the 64-bit version is still considered in a testing-mode.

--------------
WinMain() Code
--------------

This code is meant as a template that can be used to start any Sandbox-style program.

The code in WinMain(), which is the normal Windows entry point for all Windows programs, looks something like this:

    int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR lpCmdLine,_In_ int nCmdShow)
    {
        CSageBox cSageBox("SageBox Application");       // Create the main SageBox object.  Do this only once.
        cSageBox.Main(new CSandbox,"Sandbox Window");   // Transfer control to CSandBox::Main();
    }


--------------------------
An explanation of the code
--------------------------

--> 1. "int APIENTRY wWinMain" -- this is boilerplate for ALL windows programs

--> 2. CSageBox cSageBox;    

        This creates the CSagebox Object.  Do this only once.  Use references are pointers to use the object in other functions.
        However, once created in a Windows mode, CSageBox doesn't need to be used -- all useful functions are in the main window class object.
        Normally "cSageBox("app name") would be used.  With the Main() function, the window name is set as the application name.w

--> 3. cSageBox.Main(new CSandbox,"Sandbox Window");

        This calls the Main() function of CSandbox  (i.e. CSandbox::Main() or whatever the class name in QuickSandbox() is declared to be). 
        WHen CSandbox::Main() exits, a button is placed at the bottom of the window to continue. 

        This button is placed because when CSandbox::Main() has finished, CSageBox is destroyed and the Windows program is exited, in which case
        the main window is also destroyed.

        This is normal behavior for Windows, since this is the end of the Windows program.

        To run CSandbox::Main() without the window, use this line of code instead:

        cSagebox.NewWindow(new CSandbox,"Sandbox Window").Main();

        This will perform the same action without the automatic button at the end.
        (x,y) location and Width/Height can be declared with Main() and NewWindow() -- the ones presented are shortcuts.
        See the function prototypes for more information

--> 4. Ending the program. 

        When Main() returns, the Windows program is ended and deallocated from memory.
        This is initiated by CSageBox destructor, which is called when WinMain() is exited.


*/
