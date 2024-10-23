// --------------------------
// Sagebox LCD Widget Example
// --------------------------
//
// This is an example of using an external Widget that may be written and provided by anyone to link
// with Sagebox, and included easily into your program. 
//
// This example shows a simple use of an "LCD Widget", which is really just a simple counter that looks like an LCD.
//
// The library for the LCD widget is included as part of the project setup.  See the "Dial Widget" for an example where the 
// library is explicitly included in the source code so that the project does not need to be changed. 
//
// About the LCD Widget
// --------------------
//
// The LCD widget has a few different modes, but, right now, is basically just a counter. 
// The intention is to expand this widget into an LCD emulator, what can have various forms, depending on how much
// you want to emulate, from simple counting, to piecing together numbers and letters, all the way down to 
// communicating with the hardware device and the I/O level.
//
// This example just shows counting to focus on how to install and use a widget easily.
//
// Notes:
//
//   1. Try removing the Sleep() and running the demo with and without calling lcd.SetFastMode()
//      This is a mode where the LCD will only display when necessary.
//
//      This is an example of creating fast and efficient program with a little extra effort. 
//
//      Note: SetFastMode() requires an additional lcd.UpdateLast() afer the main loop exits. 
//
// *** Note: This can be a Console Program with a Console Window or a Pure Windows program.  See the Build->Configuration settings.

#include "Sagebox.h"
#include "..\Widgets\include\CLCDDisplayWidget.h"     // Include the LCD Widget file

int main()
{
    auto & win = Sagebox::NewWindow();   // Since its a small app, create static Sagebox and Window at the same time. 

    // Bring up the LCD Widget.  Send in our Window (&win as the parent)
    //
    // Put it at (50,50) with a value of 1234
   
    CLcdDisplayWidget lcd(&win,50,50); 

    // Count until the user presses a dev button to stop the program.

    auto& stopButton   = win.DevButton("Stop Counting");      // A quick way of creating development controls
                                                                // with just one line of code.
    int iCount = 0;

    while (!stopButton.Pressed() && !win.WindowClosing())
    {
        // Once the LCD reaches its maximum, it will stop counting, but we'll still be in the loop.
 
        lcd.SetValue(iCount++); 
        Sleep(1);       
    }

    // Put lcd.UpdateLast() when lcd.FastMode() is used.

    return win.ExitButton();   // Put up a button on the bottom of the window so we know we got the stop button message.
}