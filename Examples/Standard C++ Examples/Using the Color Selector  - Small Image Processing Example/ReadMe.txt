**********************
Color Selector Example
**********************

   This example shows using the Color Selector (vs. the Color Wheel) on a simple image multiply function. 
   This also shows an example returning an image-processing result in real-time.

   The Color Selector is a Widget that uses a Widget.  The Color Selector uses the Color Wheel Widget, and then adds on to it 
   to provide an enhanced Color Selection with Edit Boxes to set the color as well as the original Color Widget.  As the colors 
   change, the Edit Boxes and Circular Controls on the Color Wheel update automatically.

   The Color Selector is a Widget itself, and shows how Widgets can be used directly or used in other widgets. 

-------------------------------
SageBox Fetures in thie Program
-------------------------------

 Review the source code for generally using SageBox function.  But, there are some specific features to note:
 
 1. Getting File Name Input
 
     This program uses GetOpenFile() to get a filename from the user.  This is a simple way to do it.  You can also use GetOpenFileStruct() to get a file 
     structure to fill in more information, such as window titles (for the file window) and more. 

     In this case GetOpenFile() returns a CString() object, which either has the name of the file or is blank if no file name was entered. 
 
 2. Error control by checking for Empty bitmap/object
 
     In many SageBox examples, error-checking is not used, as it is not necessary for most elements.  In this case, however, because we're dealing with filling memory,
     we need to make sure the filename is valid and the JPEG is loaded correctly.
 
     The filename and bitmap are checked for either isValid() or isEmpty() to determine the validity of the elements returned. 
     See the stdAssert() statements in the source code.
 
     Most function also have the option to fill a boolean for success or failure, but are not used in this program.
 
 3. HideConsole()
 
     This program is a console program and has a console window.  However, the console window is not used.  This shows an example of hiding the 
     console window, effectively turning the program window a Windows program for the duration of the program's run.  The console-mode window 
     will appear again automatically when the program ends -- unless HideConsole(true) is issued, in which case this tells SageBox to not bring
     back the console window at the end of the program.
 
 4. Using Bitmaps (CSageBitmap)
 
     Bitmaps are a large part of using graphics.  SageBox provides CSageBitmap as a powerful set of functions to use bitmaps.  See the functions below for exmaples
     of using CSageBitmap. 
 
 5. Asserts (stdAssert)
 
     This program uses asserts to control the flow when unexpected errors occur -- in this case, if no file name is entered or the JPEG won't load because it's not a 
     JPEG or is corrupted.
 
     Asserts allow for error-checking and action based on the error without using if blocks, which can keep code clean while handling errors and 
     avoiding early exits.
 
     with stdTry, it defines "bError", and stdAssert() supports sending a message with the error.
     In this program, when an error is thrown, the message is printed.  bError is not used in this program, but can be checked to return a status code and to 
     perform actions if the function did not operate correctly (i.e. such as clearing memory items that would otherwise be returned, etc.)
 
 6. WinMessageBox (Console Mode or Windows)
 
     Even though this is a console mode program, Windows "MessageBox()" function is used -- wrapped as WinMessageBox in SageBox.  This is a great way to present
     errors to the user, as the Message Boxes are utilitarian and a bit noisy, which definitely can grab the user's attention that something is not right.
 
 7. Hiding and Showing the Window
 
     The Window is initially hidden on creation with the opt::Hidden() option. It is hidden so the window may be resized and the image can be put in the window, without
     the window showing first (which can make it look like it flashes).
 
     Show() is used after the window is resized and image put out to the window to show the window all at once.
     If there is an error, the window is not shown at all, giving more prominence to the error message.

------------------------------
This is a Console Mode Program
------------------------------

   This program is a console mode program, showing how SageBox functions can be used in a procedural, console-mode program without going into Windows
   or GUI specifics.  In SageBox, there is a thin line between console and Windows -- you can set the project settings for "Windows Debug" or "Windows Debug (Optimized)"
   to convert it to a Windows program without changing any source code -- in this case, WinMain() is called by Windows, which is declared in SageBox() and transfers control
   to main()
