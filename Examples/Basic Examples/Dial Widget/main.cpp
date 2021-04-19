
// ----------------------------
// Sagebox  Dial Widget Example
// ----------------------------
//
// This example shows using a Widget that is not included in the main library.
// Widgets can be written by anyone and can be distributed and linked with the Sagebox library.
//
// Widgets can be anything that a window can use, such as as a Text Widget, Color Selector, Curves Window, Graphing, etc. 
// In this case, a simple dial similar to a dial on a wall.
//
// In this case, the "Dial Widget" is used.  The Dial Widget is an example widget to show widgets in their
// various forms.
//
// Widgets can be called (as in this case), or set in the event-driven Windows environment (this will be shown in 
// another example). 
//
// Including The Widget in the Source Code
// ---------------------------------------
//
// To include the Widget, a #include file is needed, as well as (typically) a library (.DLLs will be supported in the next version)
// To include the library, it can be added to the project, or -- as shown below -- simply included in the soure code with a #pragma statement.
// (all of Sagebox may be included this way, such as using "Quickcpp.lib" with a #pragma in an existing project)
//
// About the Dial Widget
// ---------------------
//
// This dial widget was written specifically as an example of how Widgets work in Sagebox.  ALso see the LCD Widget example
//
// The Dial Widget is a simple dial with a range, which can be set.  It has a dial to change the value, as well as a
// couple arrow buttons to change the value one step at a time.
//
// The source code for the Dial Widget will be released as an upcoming example of creating widgets. 
//
// notes:
//
//      1. right-click on the dial widget.  This will bring up some selections: 
//         The "Debug Output" shows some debug information in the window
//         The "Debug Window" option shows a primitive graph determining how to higlight the ring.
//      2. Note that the dial widget is attached to the window created below.  It is its own window set inside
//         of your window as a "child window". Some widgets will pop up as an individual window, while others
//         (such as the Color Selector) give a choice as an option.
//      3. This example shows how to use the Event Loop with GetEvent().  You can also 
//         use the Windows event structure/methodology via events from the Dial Widget.
//
// *** Note: This can be a Console Program with a Console Window or a Pure Windows program.  See the Build->Configuration settings.

#include "CSagebox.h"

// We want to include the Dial Widget.  Rather than including the library in the project
// configuration, we can just include it here.  The Project configuration already has the correct
// libary path for x32 and x64 Widgets. 

#pragma comment(lib,"CDialWidget.lib")          // Specify the library we want (otherwise, include it in the project specification)

#include "..\Widgets\include\CDialWidget.h"     // Include the Dial Widget file, relative to the Sagebox main include path


// Main program: Demonstration of the Dial Widget and quick usage of debug output.
// 
// This program doesn't use the Console mode, but it kept anyway.  
//
// Use the buld configuration to switch between Console Mode and Pure Windows program.  The
// basic effect is the Windows version has no Console Mode Window, though the program itself
// is programmed procedurally in the same manner as a console-mode program.

int main()
{    
   auto & cWin = CSagebox::AutoWindow();   // Since its a small app, create static Sagebox and Window at the same time.
                                           
    // Create a Dial Widget and put it at (25,25) in the window.
    // Normally, I wouldn't use magic numbers and assign these values,
    // but for demo purposes, it's nicer to focus on the specific Sagebox code.

    CDialWidget cDial(&cWin,25,25 + 25);        // Pass in our Window (cWin) as the parent window

    // We want to center the widget, so add (50,50) to center it since it's at (25,25)
    // The "-5" is to compensate for the shadow on the right edge (this is kind of an oversight in the widget)
    // With graphics (as opposed to text input/output), much if it becomes about eye-balling it. 

    auto szSize = CPoint{ 50-5,50 + 15 } + cDial.GetWindowSize();       // GetWindowSize() returns a SIZE, but we use CPoint so we can 
                                                                        // do some easy math.
    
    cWin.SetWindowSize(szSize,true);                             // Set the window size to about 50 pixels larger than the widget itself,
                                                                // so it looks centered.  The "true" option tells SetWindowSize() 
                                                                // to make the client size the size given (otherwise it's the entire window size,
                                                                // including borders and top bar).

    // Add a label.  It's done after the window size, because CenterX() centers it in the visible window
    // (which was much wider before).  We can also use Write() or printf(), but the TextWidget is overall more useful for labels
    // and persistent text. 

    cWin.TextWidget(0,15,"Dial Widget",opt::CenterX() | Font("arial,23")); 

    // Look for any events.  We only want the dial event, so that's all we look for.
    // GetEvent() returns false (and the while loop exits) when the window is closed (this action can be disabled)

    while(cWin.GetEvent())
    {
        // If the value is changed, put the value out to the debug window.
        // This is an example of using the debug window, which is in the Process Control Window,
        // --> The debug window provides a very quick way to get out debugging info that doesn't
        //     get in the way of the output, and doesn't need to be set up.
        //
        // note: when debug is first used, it creates the Process Control Window, so no initialization is required.

        if (cDial.ValueChanged()) cWin.debug.printf("Value = {g}%d\n",(int) cDial.GetValue());
    }

    // We don't need an exit button or WaitforClose() since we waited for the window close with the GetEvent() loop

    return 0;   // not strictly necessary in main()
}