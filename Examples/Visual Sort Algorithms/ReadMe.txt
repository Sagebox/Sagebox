*******************************************************
CVisualSort.Cpp -- SageBox Neural Network 7-Bit Counter
*******************************************************

This program uses SageBox to display various sorting algorithms.  These algorithms were taken directly from sources on the Internet (CodeProject, I think) 
copied without changes into the SageBox program.

The code was then put into a class where an insert was made to draw a graph of the current output.  This insert is passive and, when not defined, causes the
sorting routines to draw as normal.

The classes were built to support the drawing routine, but otherwise the sorting algorithms are completely untouched.

-----------------
Using the Program
-----------------

The program shows graphically how various sorting routines work. Try lowering the threshold value (which determines how how often the graph draw,
with slower  values drawing more).  This slows down the display and makes it even more interesting to watch sorting algorithms do their work.

On the bottom of the window are two triangles, showing where the sorting algorithm is in its progress in sorting.  Al of the sorting algorithms
use a left and right setting and sort within that field.  The triangles shows the location of the left and right indexes, and often appears to be
moving entire sections left or right. 

-----------------------------------------
SageBox Features Outlined in this program
-----------------------------------------

1. Adapting existing code with little or no changes & footprint

     The source code for this program was taken from a console-mode program and has been changed
     very little from the original.

     One of SageBox's main paradigms is to allow procedural, non-event programs to use Windows, GUI-Based functionality without 
     changing the structure of the code.  This allows additions to code with little or not footprint.

     SageBox code can be compiled out with a switch and/or removed easily.  Also, Sagebox's design allows
     for methods to access SageBox functionality without core routines knowing of its existence, keeping 
     pure functions pure. 

2. Using SageBox in a Console-Mode Program

     While SageBox is a provider of GUI functions, it started as a way to develop and emulate rapidly low-level code,  which meant
     using existing code without changing it.

     This program is a console-mode program that just happens to use a GUI to show what is happening in realtime.  The GUI elements are easily
     compiled-in for testing and compiled-out for a release that can be used as a regular .OBJ or .LIB file.

     This file can be converted to a Windows program by selecting one of the Windows project settings, though all output currently going to 
     the console window will be muted since the console window will not exist. 

3. Checking whether or not we're a Console Mode program or Windows Program

     In this program, there is a call to CSageBox::isConsoleApp(), which tells the program if its in a console mode or if it is a Windows app.
     In the function DrawGraph(), and the stop button is pressed, the output to tell the user it has received the message is diferent depending 
     on the isConsoleApp() result.

     If the program is a Console App, it prints the message via std::printf().  If it is a windows app, it displays an Info dialog message since there
     is no console mode window.

     note: When this is a Windows app, printf()s are ignored and the information printed is not seen.

4. Using the console windows as a permanent debug window that can be turned off.

     One of the ways to develop a Windows program is to start it as a console mode and then move it to windows.
     This provides a great way to keep testing output in your program without altering your window program:

     Just use the console window.  When the program is set as a console mode program, all testing and diagnostic output
     will be printed to the console mode windows.

     When the program is a Windows program, the output goes nowhere and does not show anywhere in the Windows program.
     Plus, the time to print out to the window is now neglible since printf() should just return without processing
     ($$note$$ I don't know this for sure, as I haven't trace it down).

5. Using Child Windows

     This program shows how to use Child Windows, which are embedded in the parent window.  
     Child windows are otherwise exactly the same as Popup windows createed with NewWindow(),
     except that thay may also have a border.

     See the code for more details.

6. Using backdrop textures and CLS Bitmaps

     With many applications, clearing the screen with a solid color or gradient works just fine, and SageBox remembers the colors when you
     clear the screen with Cls().

     In this program a texure is used (see ReadPGRBitmap()) and then set as the CLS background -- when the program then performs a Cls() elsewhere, it doesn't
     need to know or care about the texture -- SageBox automatically reprints the texture rather than clearing the window with a color.

     See InitUI() for more information

7. ReadPgBitmap()

     ReadPgrBitmap() is an easy way to get a verified .BMP or .JPG image from a memory structure.  a PGR is a Portable Graphics Resource that
     are compiled from a text file with the name of the file, path, and can contain multiple files, directives, settings and so-forth to 
     completely control an object or environment.

     In this case, there is a jpeg called "texture" which ReadPgrBitmaps() reads automatically from the PGR that has been compiled as a memory structure.

8. Using the PleaseWaitWindow() functionality

     When the close window button is pressed, the PleaseWaitWindow() is used when this is a Windows program to put up a message that the window will close when
     the sort is done.  

     note that the PleaseWaitWindow() can be used in the same wait by using cSageBox.PleaseWaitWindow() even in console mode.  It is used here in windows-mode only
     for two reasons:  1. As a demonstration, and 2. the PleaseWaitWindow() overlaps the main window.  In a Console mode, it's put out to the console mode window in order
     to put out the message about the pending close but without the PleaseWaitWindow() obscuring the view.

     In the Windows version, there is no console mode window so the PleaseWaitWindow() is used.  Although it could be used in Console Mode as well, it was
     a better fit to just print out to the console mode window when possible.

9. Setting up and Using Fire-and-Forget Menus

     Menus are used in the this program, but the menu object itself is not kept.  The menu object only needs to be kept if future
     operations will be done on the menu (adding or removing items, check marks, etc.)

     In this case, the menus are used to look for an Exit() and About() selection in the menus

10. Setting a CloseButton Menu Status

     Also shown is a method to make the "Exit" menu button press the Close Button so it does not need to be checked.

11. About Window

     This program uses a function that uses a class called CSimpleDoc to draw an image to the screen that is compatible with 
     HTML editors (i.e. it is a very lmiited version of HTML to allow for structured placement and editing in an HTML editor)

     This function takes one line, as the html file, backdrop image, and other information is located in a .PGR file, which is 
     compiled from a simple text file that gives it the information it needs. 

     CSimpleDoc() is still under development.

