
// *****************************
// SageBox Mouse Drawing Example
// *****************************
//
//    This program is a very short program to show how powerful Sagebox can be with just a few general Sagebox functions, such as
//    looking for the mouse click, mouse movement, and drawing lines. 
//
//    The program itself is about 25 lines, and uses common Sagebox functions.  That is, its not esoteric and is a good way to
//    learn how Sagebox works.
//
//    Colors used in the program are text-based, i.e. "White","Red","LightYellow", etc.  These are stock colors in Sagebox.  
//    Regular RGB values or variable can also be used, such as {0,0,255}, for blue, for example.     In this program, since its
//    a small demo, stock colors were used for ease-of-use and show how they can be used rather than specific RGB values.
//
// -----------------
// Using the program
// -----------------
//
//    Just draw on the screen!  The colors change automatically.  Right-Click the mouse to clear the screen, and use the Mouse Wheel to increase the
//    thickness of the lines drawn.

#include "SageBox.h"

using namespace Sage::kw;      // Sagebox keyword options

int main()
{
    auto& win = Sagebox::NewWindow();

    // Cycle through colors for each time the mouse is lifted and then pressed again.
    
    int iColorIndex     = 0;    // Initial Color Index
    int penThickness   = 4;    // Initial Pen Thickness

    RgbColor curColor{};        

    win.SetPenThickness(penThickness);    // Set the initial pen thickness. 

    win.Cls("black,darkblue");            // Clear the screen with a gradient 

    // Create a text Widget to display information and the pen thickness.
    //
    // The options do the following:
    //
    //        TextCenterX() -- Centers the text within the widget. The widget is the width of the screen, so we don't worry about 
    //                         centering the widget itself.
    //        Font          -- Set a larger font -- the defualt is around Arial, 13
    //        TextColor()   -- Set the Text Color to a Cyan -- RGB values can be used to.  "Cyan" is one of a number of stock colors
    //        Transparent() -- Copy the Window background for the TextWidget background so the text blends in, even though it is a 
    //                         part of another window.  Otherwise, the Text Widget picks a solid color for the window.

    // The Text Widget allows us to clear the screen without losing the text or worrying about how to update it. 

    auto& text = win.TextWidget(0,0,win.GetWindowSize().cx,0,TextCenterX() | Font("Arial,16") | TextColor("Cyan") | Transparent());

    // A lambda function to show the banner, since we display it in two places (initially and when we change the pen thickness).

    auto ShowBanner=[&] { text.Write(CString() >> "Pen Thickness = " << penThickness << " -- Use Mousewheel to change thickness, right-click to clear screen"); };

    ShowBanner();    // Show initial banner. 

    // The main Event Loop -- Get events (such as mouse movements, mouse scroll wheel, etc.) 
    // GetEvent() returns false and exits the loop when the Window is closed by pressing the "X" button.

    while(win.GetEvent())
    {
        // If the mouse is clicked, consider this the starting point for the mouse, since it was previously not pressed.

        if (win.MouseClicked()) curColor = RgbColor::fromHSL(rand() % 360);   // Get a random color at full saturation
        if (win.MouseRButtonClicked()) win.Cls();         // If the Right Mouse Buttton was clicked, clear the screen

        // If the MouseWheel was moved, then increase or decrease the pen thickness depending on the direction.
        // iMouseWheel will be a positive value, usually -1 or 1, but sometimes -2 or 2 if the mousewheel was moved quickly.

        if (auto mouseWheel = win.opMouseWheelMoved()) 
        {    
            // Set the pen thickness, but read it back to correct going below the minimum.

            penThickness = win.SetPenThickness(penThickness + *mouseWheel); 
            ShowBanner();        // Update the banner to show the new pen thickness
        }

        // If the mouse was moved and the mouse button is down, draw a line from the last mouse
        // position to the current one.

        if (win.MouseDragEvent(true)) win.DrawLine(win.MouseDragPrev(),win.MouseDragPos(),curColor);
    }
    return 0;
}


