
// ***********************************
// Multiple Color Wheel Widget Example
// ***********************************
//
// This program is an example of using multiple widgets, in this case 2 or 3 instantiations of the Color Wheel Widget.
//
// It demonstrates how to use multiple widgets, even when they are the same Widget. 
//
// ----------------
// 3-Widget Version
// ----------------
//
//  This version shows using the three widgets.  As in the 2-Widget version, 2 of the Color Wheels control the two rectangles, filling
//  them with a gradient from BLACK to the selected color.
//
//  In this version, we have three widgets and three rectangles
//
// --------------------------------------
// SageBox Features Shown in This Example
// --------------------------------------
//
//  1. Text Widgets
//
//      This version shows using fire-and-forget Text Widgets, which don't need to be tracked since the are only used once for labels -- 
//      SageBox takes care of deleting them when the window is deleted.
//
//  2. NoAutoUpdate() and Update()
//
//      Many programs never need an update because windows start with the Auto Update Status set to TRUE.
//      Since the image is changing so much, we want to update it ourselves to avoid flashing:
//
//          Since we're putting out three rectangles at a time, if we auto-update, the auto-update
//          could decide to update the window between rectangles, which could cause the display to 
//          look irregular.
//
//          We want to make sure the screen updates after the rectgangle are drawn to the window (which is drawn to a bitmap, and
//          sent to the window when it updates)
//
//          AutoUpdate (which can be specified in the Window Creation or as a SetAutoUpdate() function) is set to false, meaning the window
//          will only update when we want.
//
//          Update(). This tells the window to update.  Update(<milliseconds>) can be used to tell the window to only update
//          every <milliseconds> (i.e. Update(10)). 
//
//      Most of the time, we don't need to Update ourselves, but when displaying multiple items in a loop, setting NoAutoUpdate()
//      and updating ourselves makes the update much more uniform and avoids flashing and shearing when updates hit while we're drawing the
//      window.

#include "SageBox.h"
#include "CColorWheelWidget.h"

using namespace kw;         // Sagebox Keywords

int main()
{
    // Create our Window.  Use NoAutoUpdate() to turn off auto updates of the window, and InnerSize()
    // to get create the window 1080x800 on the canvas (otherwise the window would be 1080x800, including the frame)

    auto& win = Sagebox::NewWindow(100,100,1080,800,"Multiple Color Widget Example with the Color Wheel Widget #2",NoAutoUpdate());
    
    win.Cls(PanColor::RoyalBlue,PanColor::DarkBlue);        // Set an initial background gradient from black to blue
                                                            // We can also use string here, such as "RoyalBlue,DarkBlue"

    constexpr int x = 0;            // Set initial points (we don't really need these, but they are leftover
    constexpr int y = 0;            // from the previous version, and this program is a modification of that version.

    // Create our Color Wheel Widgets, located vertically down the window on the right side.
    // Transaprent() copies the parent background rather than filling the Widget Background with a solid color

    ColorWheelWidget color1(win,x + 850,y + 100 + 220*0,Transparent()); // Note:  Transparent() is deprecated and is being change to the default.
    ColorWheelWidget color2(win,x + 850,y + 100 + 220*1,Transparent());
    ColorWheelWidget color3(win,x + 850,y + 100 + 220*2,Transparent());

    // Get the size of the Color Wheel Widgets so we can set the Text Widgets the same width
    // We locate the Text Widget on the top of each widget, and center it:
    //
    //    TextCenterX() Centers the text in the Widget
    //    Transparent() copies the parent background rather than filling the Text Widget with a solid color

    auto szWidgetSize = color1.GetWindowSize();

    auto& label1 = win.TextWidget(x+850,y + 100 + 220*0-20,szWidgetSize.cx,0,"Outer Square",TextCenterX());
    auto& label2 = win.TextWidget(x+850,y + 100 + 220*1-20,szWidgetSize.cx,0,"Inner Square",TextCenterX());
    auto& label3 = win.TextWidget(x+850,y + 100 + 220*2-20,szWidgetSize.cx,0,"Backdrop",TextCenterX());

    // Set some initial values for each rectangle (and backdrop).  
    // These are basically chosen at random

    color1.SetRgbColor(PanColor::Blue);    
    color2.SetRgbColor({15,200,60});               // Kind of a green -- shows we can use RGB values as well as known text-based values
    color3.SetRgbColor(PanColor::Purple);


    constexpr RGBColor_t rgbBlack = { 0,0,0 };      // Get an easy way to refer to an RGB black color

    CPoint rect = { 100,100 };                    // Rectangle Indent

    CPoint loc = { x, y };

    // Update Lamdba() -- use this as a Lamdba so we can call it twice -- this helps avoid making it a class or separate function
    // (in which case we'd have to pass all used variable or make them global).  Both options aren't great at this point, so the lamdba
    // really helps.

    auto Update = [&]()
    {
        RGBColor_t rgbColor1 = color1.GetRgbColor();        // Get RGB values for each Widget
        RGBColor_t rgbColor2 = color2.GetRgbColor();    
        RGBColor_t rgbColor3 = color3.GetRgbColor();

        // Draw the gradients for each color and rectangle areas.
        // This alternates the Gradient in the middle from Color->Black just to make it more interesting.
 
        win.FillRectangle(loc + rect*0,{ 800,800 },{rgbBlack,rgbColor3});  // Can also use DrawGradientFunction()
        win.FillRectangle(loc + rect*1,{ 600,600 },{rgbColor1,rgbBlack});
        win.FillRectangle(loc + rect*2,{ 400,400 },{rgbBlack,rgbColor2});

        // Update the upper-left corner of each rectangle with the RGB values
    
        win.printf(x + 100*0 +5,y + 100*0+5,"Backdrop Color = (%d,%d,%d)",        rgbColor3.iRed,rgbColor3.iGreen,rgbColor3.iBlue); 
        win.printf(x + 100*1 +5,y + 100*1+5,"Outer Square Color = (%d,%d,%d)",    rgbColor1.iRed,rgbColor1.iGreen,rgbColor1.iBlue); 
        win.printf(x + 100*2 +5,y + 100*2+5,"Inner Square Color = (%d,%d,%d)",    rgbColor2.iRed,rgbColor2.iGreen,rgbColor2.iBlue); 

        win.Update();        // Update the window since we turned off auto updates

    };

    Update();        // Call our lamdba Update function so we have a display before we get a change-color event


    // Wait until we get an event (or the window closes, in which case it returns FALSE and exits

    while(win.GetEvent())
    {
        // We only want to know if a value in any widget changed.
        // We could look at them separately, but in this case if any have changed we 
        // update the window.
        //
        // In other applications, it can make more sense to check them individually. 

        if (color1.ColorChanged() || color2.ColorChanged() || color3.ColorChanged()) Update();
    }

    // End program (or return the Console Window when its a console app)

    return 0;
}
