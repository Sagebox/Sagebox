// ----------------------------------
// Sagebox Color Wheel Widget Example
// ----------------------------------
//
// This is an example of using Widgets, but also using widgets built into Sagebox.
//
// This Widget it the Color Selector, an expansion on the Color Wheel Widget (see the Color Wheel example). 
//
// About the Color Wheel Widget
// ----------------------------
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
//      In this example, the Color Wheel is created on the stack, so it does not need to be deleted.
//
//      In other cases, as part of a class, this may be allocated with C++ "new". When Widgets are allocated,
//      the do not need to be deleted. Widgets are managed by the parent (i.e. passed-in) window, and do not need to 
//      be deleted, and are deleted when the parent window is closed.
//      
//      When deleted, the parent window is notified and does not try to delete it later. 
//
//      With the Color Wheel object, Hide() can be called to remove the Color Wheel from the window.
//      See a more advanced used of the Color Wheel widget in the Examples directory. 
//
// *** Note: This can be a Console Program with a Console Window or a Pure Windows program.  See the Build->Configuration settings.

#include "CSagebox.h"
#include "CColorWheelWidget.h"

int main()
{
    auto & cWin = CSagebox::AutoWindow();                           // Since its a small app, create static Sagebox and Window at the same time.

    SIZE RectSize = { 200,200 };                                    // Rectangle Size
    auto RectPos  = ((CPoint) cWin.GetWindowSize() - RectSize)/2;    // Put Rectangle in center of window 
                                                                    // Uses CPoint (a class-based version of POINT) to calculate placement.
  
    // Create a text Widget to give the user some information
    //
    //      (0,0)               -- This is usually the X, Y (or both) coordinates of where we want the Text Widget, but since 
    //                             we used JustBottomCenter(), we don't use them
    //      JustBottomCenter()  -- Sets the text Widget at the bottom/center of the window.
    //      Font()              -- Sets the Font we want for the text widget (otherwise the default is used)
    //      OffsetY()           -- Tells the TextWidget to bring up the text -10 pixels, so it doesn't rest on the bottom, 
    //                             useful especially with larger fonts.
    //
    // Hover the mouse over each item for a description.
    //
    // We could just use win.Write(), as well,  TextWidgets are nice because they are persistent. We can update or Hide the Text Widget, 
    // but in this case we used it as a fire-and-forget, so we didn't save the return object, which is deleted automatically when the Window closes. 
   
    cWin.TextWidget(0,0,"Move Color Wheel to change color of Rectangle. Close Window to Exit",Font("Arial,18") | JustBottomCenter() | OffsetY(-10));

    // Create the color wheel and place it at 20,20, passing in our Window (cWin) object as the parent

    CColorWheelWidget cColorWheel(&cWin,20,20); 

    // Put up an initial rectangle until the color wheel is moved.
    // As a refactoring step, we could move this into a lambda since we duplicate the code below. 

    cWin.DrawRectangle(RectPos,RectSize,cColorWheel.GetRGBValues());

    // Wait for an event.  If the Window is closed, GetEvent() returns false so the loop can exit (this can be disabled).
    // While waiting for events (mouse movements, clicks, buttons, etc.), the code below is asleep and uses no processor time.

    while(cWin.GetEvent())
    {
        // The Color Wheel will cause an event if the color has changed, and 
        // we can check that with "ValueChanged()" (or event.ValueChanged()) in the Color Wheel object.

        RgbColor rgbColor;  // We can also call ValueChanged() and then GetRGBValues() if we don't want to use a variable.

        // If the user changed the color, Color Wheel caused an event and set ValueChanged() to true. 

        if (cColorWheel.ValueChanged(rgbColor)) cWin.DrawRectangle(RectPos,RectSize,rgbColor);
    }
}