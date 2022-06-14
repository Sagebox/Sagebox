// ------------------------------------------------------------
// Sagebox -- Mouse Movement and Basic Events (Console Version)
// ------------------------------------------------------------
//
// This example shows how to get events in a procedural loop, allowing us to 
// get windows and other events in a procedural program like a console mode program,
// or pure Windows program. 
//
// While Sagebox supports and contains many functions for fully event-driven programs, one of its
// main modes is completely procedural, or a mix of the two.  Other examples will show these modes. 
//
// This example shows the following:
//
//      1. Getting events in the GetEvent() loop.
//         GetEvent() only returns if there is a useful event such as a MouseMove, keypress, mouse click, etc.
//         GetEvent() also returns if the window is closed.  GetEvent() always returns true unless the Window is closing
//         (if the user closed it or the system is closing it down).
//
//         In between events, the program is not active and is asleep.
//
//      2. Processing events.  In the procedural method of GetEvent(), we know we hav 
//         an event, so we just look around to see what has changed for events in which we're interested.
//
//         In a callback mechanism via Sagebox, (not shown in this example) we can do the same, or 
//         create a handler for specific events.  We can respond to the low-level window message.
//
//      3. Using the debug window.  The debug window s a quick way to throw out debug information with it
//         cluttering your program or making a specific window.  You can also create a DevWindow to print out to
//         for quick debugging.
//
//         Note that the debug window does not appear until it is first used.  It may also be hidden (and re-shown) by
//         moving the mouse to the upper-right-hand corner of the screen.
// 
// *** Note: This can be a Console Program with a Console Window or a Pure Windows program.  See the Build->Configuration settings.

#include "Sagebox.h"

int main()
{
   auto& cWin = Sagebox::NewWindow();   // Since its a small app, create static Sagebox and Window at the same time.

    // Print a message out to the window.  

    cWin << "Click Mouse on the Window to Draw Rectangle.  Close Window to Exit.";

    // Get an Event.  The program is dormant (using no processor time)  until an event is received
    // or the window is closed (which can be disabled).
    //
    // GetEvent() returns TRUE on an event, FAKLSE on Window closing (so it exits)
    //
  
    while(cWin.GetEvent())
    {
        POINT pMouse;       // Store mouse location here.  It is only filled 
                            // When an event occurs.  We could also use win.GetMousePos()
                            // to get the mouse position, but this is easier.

        if (cWin.MouseMoved(pMouse)) 
        {
            // Print out to the console window

            printf("Mouse Moved at (%d,%d)\n",pMouse.x,pMouse.y); 

            // Print to the debug window to show an easy debug method. The window does not 
            // become visible until it is printed to (or explicitly shown with debug.Show().
            //
            // Note that "{g}" is used as a method to change the text color to green.
          
            cWin.debug.printf("Mouse Moved at {g}(%d,%d)\n",pMouse.x,pMouse.y); 
        }

        // If we clicked on the window, draw a rectangle of radius 100 with the mouse point as the center
        // win.GetColor() is used, but an RGB color can also be put in place, either directly, with 
        // Windows COLORREF type of RgbColor type. -- for example, using "{255,0,0}" is the same as win.GetColor("Red");

        if (cWin.MouseClicked(pMouse)) cWin.FillRectangle(pMouse.x-50,pMouse.y-50,100,100,PanColor::Red);
    }

    // We don't need to hold up the exit, since the user closed the window manually.
}