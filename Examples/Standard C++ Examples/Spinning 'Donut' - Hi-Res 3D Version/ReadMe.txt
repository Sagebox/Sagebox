**************************
3D Ascii Donut Source Code
**************************

This example is an extension of the Ascii Donut Code.

Where the previous version was to show how original source code from a program can be taken and put into 
SageBox with little or no change, this version shows how we can then adapt code for a much broader usage
by writing code around the original source.

This is a high-res 3D version that still largely leaves the original source untouched -- the only changes are to 
use tables for some equations for speed purposes.

This program is also a good example of using a derived CWindow type of project, making the main window part of the same namespace as the program.  See documentation below.

-----------------------
Windows Debug Optimized
-----------------------

The default project setting for this program is "Windows Debug Optimized". 
This is because it is much, much slower without optimization, and there is no reason for it to be console mode program.

-----------------------------------------
SageBox Features Outlined in this program
-----------------------------------------

For the most part, the code can be reviewed to see SageBox elements -- in most cases, the 
code is self-explanatory or you can hold the mouse over the function for documentation.

Features Outlined

1. Dervived CWindow for main() (aka SandBox-type project)

       This program uses a class derived from CWindow (CAsciiDonut), which bring the CWindow functions into the same namespace
       as the window attached to CWindow.

       This makes using various Windows functions more inline with the code, vs. using MyWindow->Function().
       This can be very useful when overriding various message, where otherwise a message handler would be needed, or the
       window would be derived just for the purpose of handling messages.

       In this case, the main program is in the same class as the window, so the window functions are called directly.        

       While the SandBox-style project is not recommended for large projects, it is great for quick prototypying
       and trials because there is no work to do to get up and running right away.

2. Adapting existing code with little or no changes & footprint

     The source code for this program was taken from a console-mode program and has been changed
     very little from the original.

     One of SageBox's main paradigms is to allow procedural, non-event programs to use Windows, GUI-Based functionality without 
     changing the structure of the code.  This allows additions to code with little or not footprint.

     SageBox code can be compiled out with a switch and/or removed easily.  Also, Sagebox's design allows
     for methods to access SageBox functionality without core routines knowing of its existence, keeping 
     pure functions pure. 

3. Using Child Windows

     This program shows how to use Child Windows, which are embedded in the parent window.  
     Child windows are otherwise exactly the same as Popup windows createed with NewWindow(),
     except that thay may also have a border.

     See the code for more details.
