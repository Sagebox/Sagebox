// -----------------------------------------------
// Sagebox Color Selector Widget Example
// -----------------------------------------------
//
// This is an example of using Widgets, but also using widgets built into Sagebox.
//
// This Widget it the Color Selector, an expansion on the Color Wheel Widget (see the Color Wheel example). 
//
// About the Color Selector Widget
// -------------------------------
//
//      The Color Selector widget is a quick way to get a color from the user.  The user can enter RGB values or use the mouse to select
//      the color.  This is the first version of the Color Selector, and newer version will support other color models, such as LAB and HSL,
//      and also have an option to use a rectangle rather than a triangle for more accurate color selection.
//
//      The Color Selector uses the Color Wheel Widget, which is a widget that can be used to obtain more informal colors quickly, 
//      and is easier to embed in the window, rather than using it as a separate window.
//
//      The Color Selector can be used as a separate window or embedded in the parent window, though most usage is better off as a 
//      separate window.
//
//      The Color Selector has many options.  Press "cColorSelector." to see all of the functions.
//
//      CColorSelector is still in progress, and some thing may not be documented.
//
// Widget Object Management
// ------------------------
//
//      In this example, the Color Selector is created on the stack, so it does not need to be deleted.
//
//      In other cases, as part of a class, this may be allocated with C++ "new". When Widgets are allocated,
//      the do not need to be deleted. Widgets are managed by the parent (i.e. passed-in) window, and do not need to 
//      be deleted, and are deleted when the parent window is closed.
//      
//      Most widgets will cause a compiler error if they are explicitly deleted (ColorSelector may in the future).
//
//      When deleted, the parent window is notified and does not try to delete it later. 
//
//      In the case of the Color Selector, you can dismiss the object with cColorSelect.CloseWindow() when allocated on the stack, or
//      instead of deleting it (you can also use cColorSelector.Hide())

// *** Note: This can be a Console Program with a Console Window or a Pure Windows program.  See the Build->Configuration settings.

#include "Sagebox.h"

using namespace Sage::kw;       // Sagebox Keywords

int main()
{
    auto & win = Sagebox::NewWindow(50,50,800,500);   // Since its a small app, create static Sagebox and Window at the same time.

    // Start the color selector. Our Window object is passed in as the parent.
    //
    // Popup() -- tells the color selector to act as its own window.  Otherwise, it puts itself in as a child window
    //            of the parent window.  See the Color Wheel example for an embedded example.
    //
 
    ColorSelector cColorSelect(win,870,50,kw::Popup() | kw::NoCancel());

    // Print a mesage out to the screen (which will automatically show the window)
    //
    // The first message centers itself in the screen (XY dimension, i.e. CenterXY())
    // The second one centers itself only in the X plane on the next line from the last mesage.

    win << CenterXY() << Font("Arial,20") << "Use Color Wheel to Change Color of Screen\n";    // Demonstration of streaming output method.

    // With the first one, we used streaming akin to "cout", but with options.
    // With this one, do the same thing, but using the function Write() instead, with similar options/
    // (note the "|" is the same as "<<", and just tends to look better when used as options to functions)

    win.Write("{16}Close Window to end program",CenterX());    // {16} sets font to Arial, 16

    // Wait for an event to occur.  We only care about the Color Selector and if its value has changed.
    // GetEvent() will return false when the window is closed (this can be disabled). 
    //
    // While events (such as mousemoves, presses, etc.) are not occuring, the loop below is dormant and using no CPU time.
    //
    //      cColorSelect->WindowClosed() is called and will return true (and cause an event) when the user closes
    //      the Color Selector Window.  Closing the Color Selector Window can be disabled by passing kw::NoClose() when
    //      initialized (and then we don't need to check if it is closed).
    //
    //      If we don't look for cColorSelect->WindowClosed(), the only thing that happens is that the Color Selector
    //      Window closes, and we remain in the loop -- no error will occur, the Color Selector Window will just disappear,
    //      and the user can still exit the loop by closing the window.

    while(win.GetEvent() && !cColorSelect.WindowClosed())
    {
        RgbColor rgbColor;

        // The Color Selector will cause an event if the color has changed, and 
        // we can check that with "ValueChanged()" (or event.ValueChanged()) in the color selector object.

        if (cColorSelect.ColorChanged(rgbColor)) win.Cls(rgbColor);

        // If the user pressed OK or cancel, just exit like a window close.
        // --> For normal uses, these would be handled as separate issues.

        if (cColorSelect.OkButtonPressed() || cColorSelect.CancelButtonPressed()) break;
    }
}