// Dial Widget Example -- Copyright(c) 2020, 2021 Rob Nelson.  robnelsonxx2@gmail.com -- all rights reserved.
// This file, information, and process within are for personal use only and may not be distributed without permission.
// Please modify, copy, do whatever you want for personal uses.  For professional, distribution or commercial uses, 
// contact the e-mail address above

// ***************************
// SageBox Dial Widget Example
// ***************************
//
//        This program is an example of using a dial, which can be used as a way to emulate a device such as a wall heater, pool heater, or any other
//        similar device.
//
//        The program is a demonstration to show how the Dial widget works, simply putting out the numbers in the dial to the right side of the window.
//
//        About 15 lines of code, this is a good way to look at some simple SageBox functions, such as creating the window, and a basic use of the 
//        Event Loop
//
//        It's also a good example of the Text Widget, which allows us to draw a large number on the screen, centered in its
//        own declared window -- the window itself doesn't show, because it copies the background of the main window through use
//        of the Transparent() option.
//
//        This program also shows the use of a fire-and-forget Text Widget that displays a message, but then has no other use -- therefore the 
//        returned Widget object pointer does not need to be saved, since the Widget is managed and deleted by the Window.
//
// -----------------------------
// The Diagnostic Window in Dial
// -----------------------------
//
//  A special thing to notice in this program is the diagnostics window contained in the Dial Window.
//  
//  Clicking the Right-Mouse Button. The Dial Widget uses another widget that pops up a small menu window when the right-mouse button is clicked.
//  In the Dial, three options come up, allowing the developer various options.  Try it to see what it does.
//  
//  The right-click menu that comes up is one of many SageBox development functions that make developing widgets and programs easier. 
//  The main purpose in the dial is to make sure the mask for the dial color is correct, where one mode is to show the mask as the dial 
//  is used.
//
//  The source code for the Dial Widget will be released at a later date.
//

#include "SageBox.h"
#include "Widgets\include\CDialWidget.h"

int main()
{
    
    auto& cWin = Sagebox::NewWindow(100,100,550,270,"Sagebox - Dial Widget Example");
    cWin.Cls(SageColor::SkyBlue,SageColor::SkyBlueDark);                              // Clear screen with a gradient of two stock colors

    CDialWidget cDial(&cWin,10,30);                                // Give the dial our parent window and location to put it
    
    cDial.SetRange(0,1000);
    cDial.SetValue(450);        // Set initial value

    // Get a Text widget to display the number we get from the dial, using a large font.
    // In this case, we use TextCenter(), which centers the value in the text window we created.
    //
    // the text widget takes up the rest of the window, but since its transparent and centers the text,
    // it blends well as it displays on the screen.

    auto& cText = cWin.TextWidget(210,60,550-210,0,Transparent() | Font("Arial,130") | TextCenter());

    // Create a text widget to remind the user to right-click on the dial for some options. 
    //
    // The options in the Text Widget do the following (the ones not listed are self-explanatory)
    //
    //        JustTopCenter()       -- this places the widget centered at the top of the screen. 
    //        OffsetY(5)            -- This adds 5 pixels from the Y location of the window, giving it more space
    //                                 (so that it is off the top edge by 5 pixels)
    //        Transparent()         -- This allows the background of the Text widget to blend with the window's background, since it is a 
    //                                 gradient and not a solid color (i.e. it copies the parent window's background)

    cWin.TextWidget(0,0,"Right-Click Mouse on Dial Face for Development Window and Options",Font("arial,12,bold") | TextColor(PanColor::Cyan) | JustTopCenter() | OffsetY(5)| Transparent());

    // A Lambda to write the value, since we use it more than once (and it's too small to create a function),
    // once before we enter the main event loop, and then again when we get new dial values. 

    auto WriteValue = [&]() { cText.Write(CString() >> (int) cDial.GetValue()); };    // Put out the number to our text widget

    WriteValue();

    // Main EventLoop -- Returns events as they occur, and exits when the window is closing.
    //
    // In this case, we just want one event -- check the dial if its changed and then print the value. 

    while(cWin.GetEvent())
    {
        if (cDial.ValueChanged()) WriteValue();
    }
    return 0;
}