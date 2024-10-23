
// ***********************************
// Multiple Color Wheel Widget Example
// ***********************************
//
// This program is an example of using multiple widgets, in this case 2 or 3 instantiations of the Color Wheel Widget.
//
// It demonstrates how to use multiple widgets, even when they are the same Widget. 
//
// ----------------
// 2-Widget Version
// ----------------
//
//  This version shows using the two widgets.  As the colors in the Widgets are changed, this is drawn as
//  Gradients from BLACK->(color) in each respective rectangular area.
//
// --------------------------------------
// SageBox Features Shown in This Example
// --------------------------------------
//
//  1. NoAutoUpdate() and Update()
//
//    Many programs never need an update because windows start with the Auto Update Status set to TRUE.
//    Since the image is changing so much, we want to update it ourselves to avoid flashing:
//
//      Since we're putting out three rectangles at a time, if we auto-update, the auto-update
//      could decide to update the window between rectangles, which could cause the display to 
//      look irregular.
//
//      We want to make sure the screen updates after the rectgangle are drawn to the window (which is drawn to a bitmap, and
//      sent to the window when it updates)
//
//      AutoUpdate (which can be specified in the Window Creation or as a SetAutoUpdate() function) is set to false, meaning the window
//      will only update when we want.
//
//      Update(). This tells the window to update.  Update(<milliseconds>) can be used to tell the window to only update
//      every <milliseconds> (i.e. Update(10)). 
//
//    Most of the time, we don't need to Update ourselves, but when displaying multiple items in a loop, setting NoAutoUpdate()
//    and updating ourselves makes the update much more uniform and avoids flashing and shearing when updates hit while we're drawing the
//    window.

#include "Sagebox.h"
#include "CColorWheelWidget.h"

using namespace kw;         // Sagebox Keywords

int main()
{
    // Create our Window.  Use NoAutoUpdate() to turn off auto updates of the window, and InnerSize()
    // to get create the window 1080x800 on the canvas (otherwise the window would be 1080x800, including the frame)

    auto& win = Sagebox::NewWindow(100,100,880,700,"Multiple Color Widget Example with the Color Wheel Widget",NoAutoUpdate());

    win.Cls("DodgerBlue");       // Set an initial background color to blue  - Symbolic PanColor::DodgerBlue (without quotes) can be used also.
    
    auto x = 50;      // Starting point of outer-most rectangle
    auto y = 50;

    auto rgbBlack = Rgb("Black");            // Get an easy way to refer to an RGB black color. Rgb(0,0,0) can also be used.

    // Create our Color Wheel Widgets, located vertically down the window on the right side.
    // Transaprent() copies the parent background rather than filling the Widget Background with a solid color

    CColorWheelWidget color1(win,x + 620,y + 220*0,Transparent()); // note: Transparent() here is deprecated and will be default in upcoming release.
    CColorWheelWidget color2(win,x + 620,y + 220*1,Transparent());

    // Set a coule random colors for each rectangle

    color1.SetRgbColor(Rgb("blue"));        // SetRGBValue() doesn't currently take strings, so we can use Rgb(<string>) instead. 
    color2.SetRgbColor(Rgb("red"));
    
    // Wait until we get an event (or the window closes, in which case it returns FALSE and exits
    // Usually, GetEvent() won't return until there is an event (or a Window Close).  In this case,
    // we wouldn't see an initial display until we received an event such as a mouse move, mouse click, etc.
    //
    // We get away with an instant first display because the first call into GetEvent() returns true automatically
    // just for purpose like this -- in this case, to draw an initial display. 
    //
    // After the first call, GetEvent() acts as normal and will not return until there is an event in the queue.

    while(win.GetEvent())
    {
        // Get the colors from each Color Widget

        RGBColor_t rgbColor1 = color1.GetRgbColor();
        RGBColor_t rgbColor2 = color2.GetRgbColor();

        // Draw the Gradients for each Rectangle

        win.FillRectangle(x,y,600,600,{rgbColor1,rgbBlack});             // Can also use DrawGradient() function
        win.FillRectangle(x+100,y+100,400,400,{rgbBlack,rgbColor2});
    
        // Update the upper-left corner of each rectangle with the RGB values

        win.printf(x + 5        ,y + 5,     "Outer Square Color = (%d,%d,%d)", rgbColor1.iRed,rgbColor2.iGreen,rgbColor1.iBlue); 
        win.printf(x + 100 + 5,y + 100 +5,  "Inner Square Color = (%d,%d,%d)", rgbColor2.iRed,rgbColor2.iGreen,rgbColor2.iBlue); 

        win.Update();    // Update the window since we turned off auto updates

    };

    // End program (or return the Console Window when its a console app)

    return 0;
}