// SageBox Mouse Drawing Example -- Copyright(c) 2020, 2021 Rob Nelson.  robnelsonxx2@gmail.com -- all rights reserved.
// This file, information, and process within are for personal use only and may not be distributed without permission.
// Please modify, copy, do whatever you want for personal uses.  For professional, distribution or commercial uses, 
// contact the e-mail address above

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

int main()
{
    CSageBox cSageBox; 
    auto& cWin = cSageBox.NewWindow();

    // Cycle through colors for each time the mouse is lifted and then pressed again.
    
    int iColorIndex     = 0;    // Initial Color Index
    int iPenThickness   = 4;    // Initial Pen Thickness

    RGBColor_t rgbCurColor;        

    cWin.SetPenThickness(iPenThickness);    // Set the initial pen thickness. 

    cWin.Cls(PanColor::Black,PanColor::DarkBlue);            // Clear the screen with a gradient 

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

    auto& cText = cWin.TextWidget(0,0,cWin.GetWindowSize().cx,0,TextCenterX() | Font("Arial,16") | TextColor("Cyan") | Transparent());

    // A lambda function to show the banner, since we display it in two places (initially and when we change the pen thickness).

    auto ShowBanner=[&] { cText.Write(CString() >> "Pen Thickness = " << iPenThickness << " -- Use Mousewheel to change thickness, right-click to clear screen"); };

    ShowBanner();    // Show initial banner. 

    // The main Event Loop -- Get events (such as mouse movements, mouse scroll wheel, etc.) 
    // GetEvent() returns false and exits the loop when the Window is closed by pressing the "X" button.

    while(cWin.GetEvent())
    {
        // If the mouse is clicked, consider this the starting point for the mouse, since it was previously not pressed.

        if (cWin.MouseClicked()) rgbCurColor = RgbColor::fromHSL(rand() % 360);   // Get a random color at full saturation
        if (cWin.MouseRButtonClicked()) cWin.Cls();         // If the Right Mouse Buttton was clicked, clear the screen

        // If the MouseWheel was moved, then increase or decrease the pen thickness depending on the direction.
        // iMouseWheel will be a positive value, usually -1 or 1, but sometimes -2 or 2 if the mousewheel was moved quickly.

        int iMouseWheel;
        if (cWin.MouseWheelMoved(iMouseWheel)) 
        {    
            // Set the pen thickness, but read it back to correct going below the minimum.

            iPenThickness = cWin.SetPenThickness(iPenThickness + iMouseWheel); 
            ShowBanner();        // Update the banner to show the new pen thickness
        }

        // If the mouse was moved and the mouse button is down, draw a line from the last mouse
        // position to the current one.

        if (cWin.MouseDragEvent(true)) cWin.DrawLine(cWin.MouseDragPrev(),cWin.MouseDragLast(),rgbCurColor);
    }
    return 0;
}


