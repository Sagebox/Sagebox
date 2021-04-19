***********************
Ascii Donut Source Code
***********************

This example is to show how original source code from a program can be taken and put into 
SageBox with little or no change. 

This is an ascii-output 'donut' by Andy Sloan. 

The original code was taken as-is, untouched, except for one small change to an output because
Windows does not support ANSI codes very well in Console Mode. 

The original code is outlined in the source code. 
There is also a 3D high-res version (see other projects), which shows how current code can be 
taken and enhanced greatly by using Sagebox graphic functions.

This program is also a good example of using the SandBox project type in Sagebox.  See documentation below.

-----------------------------------------
SageBox Features Outlined in this program
-----------------------------------------

For the most part, the code can be reviewed to see SageBox elements -- in most cases, the 
code is self-explanatory or you can hold the mouse over the function for documentation.

Features Outlined

1. Sandbox Project 

   The Sandbox-Style project in Sagebox is a great way to quickly start up a session with SageBox. 
   the main function is derived from CWindow, and the basic interface works the same as C/C++, with 
   printf(), and SageBox functions can now be directly called. 

   When the main() funciton is entered, the window is already created, and it is simple to perform
   various functions with SageBox, rather than a) the need to create a window and b) de-referencing the window, i.e. cWin.printf or 
   cwin.DrawRectangle() vs. simple "printf()" and "DrawRectangle()"

   When the project is opened, CSageBox is already created along with the window, and programs can be instantly started in the 
   Sandbox's main() function.

   All that is needed is the following code, which is already written when the project is loaded:

       #include "CSageBox.h"
       QuickSandbox(CSandBox)

       void CSandBox::Main()
       {
           // Start Code here
       }

       int main()
       {
           CSageBox cSageBox("Enter program name here or leave blank");
           cSageBox.Main(new CSandBox);
           return 0;
       }

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
