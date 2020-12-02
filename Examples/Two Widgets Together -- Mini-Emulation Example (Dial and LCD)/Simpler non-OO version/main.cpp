// Two Widgets/MiniEmulator Example -- Copyright(c) 2020, 2021 Rob Nelson.  robnelsonxx2@gmail.com -- all rights reserved.
// This file, information, and process within are for personal use only and may not be distributed without permission.
// Please modify, copy, do whatever you want for personal uses.  For professional, distribution or commercial uses, 
// contact the e-mail address above

// ********************************
// Two Widgets/MiniEmulator Example
// ********************************
//
//  This program shows the use of the Dial and LCD widgets working together, forming the basis for emulating hardware.
//  Believe it or not, this program (the OO version, not the monlithic version contained in main()) is the basis for 
//  real emulation.  Only certain parts need to be overladed (the virtual functions) to talk to real hardware.
//
//  The Emulation Premise
//
//  The premise of the emulator is a simple local temperature dial and a remote, embedded device that receives the data (and has an LCD display).
//
//  The temperature dial could be for a wall heater, or a pool heater (a real situation where SageBox has been used), or another device.
//  The LCD display can be a real LCD display across a yard or courtyard to the actual heater, displaying real-time data.
//
//  Most emulators actually start off this small, expanding over time into something much larger.
//
// -----------------------------------------
// The PGR File - Portable Graphics Resource
// -----------------------------------------
//
//  The PGR file is used in many SageBox programs, mostly to provide a background texture.  In this program, it is used for more.
//  It contains the background, but also information where to put things. 
//
//  This allows the emulator to have many different configurations with different hardware without changing code: all controlled and defined
//  by the PGR file.  Any number of PGR files can be used to direct where to put objects, and which ones to use and not use.
//
//  The PGR file itself is compiled from a very simple text file with the values read in (location of buttons, etc.) and the background image. 
//  It is then converted to memory so that the PGR file does not need to exist on disk locally when the program is run.
//
// -----------------------------------
// This version: main() implementation 
// -----------------------------------
//
//  This file is written on one main() function
//
//  It is written this way for clarity and to show more easily some SageBox functions.
//
//  Also, it is a good example of using lamdbas prior to moving to a class/OO-based program.
//  The lambdas work as local class functions since they have acess to all local variables, and are 
//  a great way to functionalize areas of code locally before deciding where they belong -- in the class, 
//  as lambdas, static functions, etc. 
//
//  The OO version is the more "proper" version for emulating hardware because it's easier to overload
//  functions to actual hardware, emulation, debugging, etc. without modifying core emulator code. 
//  (though in a pure 'C' environment function pointers can work just as well, too, though not as readable)

#include "CSageBox.h"
#include "Widgets\include\CDialWidget.h"
#include "Widgets\include\CLCDDisplayWidget.h"

// Data files compiled as memory 

#include "..\lcddialtexture.pgr2.h"         // backdrop data file
#include "..\AboutTwoWidgets.pgr2.h"        // About Box HTML and background image data file

int main()
{
    CSageBox cSageBox("SageBox -- Using two Widgets / Simple Emulation Example"); 

    // Open up the PGR file -- it has the main bitmap but also specified where
    // control objects and the text window need to be placed. 

    // .PGR files contain files and declarations to control the data. 

    CSagePGR cPgr(sTexture_1);

    auto pLcdLoc        = cPgr.ReadPoint("LcdXY");              // Get the location of various things
    auto pDialLoc       = cPgr.ReadPoint("DialXY");             // which are pretty obvious by their titles.
    auto pDebugLoc      = cPgr.ReadPoint("DebugWinXY");
    auto pButtonLoc     = cPgr.ReadPoint("ButtonXY");
    auto szDebugSize    = cPgr.ReadSize("DebugWinSize");
    auto csDebugColor   = cPgr.ReadText("DebugWinColor"); 
    auto cBitmap        = cPgr.ReadBitmap("texture");

    // Open a window sized to the background image & texture.
    // 
    // NoBorder() -- Specifies no title bar (i.e. client area only) for popup windows. 

    auto& cWin = cSageBox.NewWindow(100,100,cBitmap.GetWidth(),cBitmap.GetHeight(),NoBorder());

    cWin.DisplayBitmap(cBitmap);        // Display the background image.

    // Allow the window be dragged around by any open area in the window (since we don't have the title bar)
    // Otherwise the user wouldn't be able to move the window at all since we specified "NoBorder()" on
    // window creation.

    cWin.SetWindowDrag();

    // Create the LCD Widget, which will display the raw data from the Dial Widget. 
    // This represents emulating a remote embedded device, disconnected from the user and
    // main dial control.

    CLcdDisplayWidget clcd(&cWin,pLcdLoc,0,Transparent()); 

    // Create the Dial Widget.  When this is rotated, the value is displayed, and the raw value
    // from the dial is sent to the remote device (represented by the LCD). 
    //
    // The dial can be a temperature or other guage, such as a pool temperature or local heater control.

    CDialWidget cDial(&cWin,pDialLoc);

    // Create the debug output window.  Here, it only displays the dial value, representing
    // the output we might get from any device.  In this case, it represents the low-level
    // value we might get from a hardware device such as the dial control.

    auto& cDebugWin = cWin.ChildWindow(pDebugLoc,szDebugSize,bgColor(csDebugColor) | Font("Courier New,12")); 

    // Set up an "Immediate" update type.  This will update the debug window after each and every write, so we don't
    // have to worry about its state.  This is a slower way to do it and not recommended when you want ultra-speed, but is
    // fine for this purpose.  Otherwise, we'd have to Update() ourselves, as letting the auto-udpate handle it does not
    // guarentee the latest output until an Update() is called.

    cDebugWin.SetAutoUpdate(AutoUpdateType::Immediate);        // Slow-but-sure updates (i.e. don't have to worry about updating)
    cDebugWin.printf("{g}Emulator Debug Window Ready (non-OO version)\n");            // Print a message so the user knows its operating. "{g}" sets the text color to green.

    // -----------
    // The Lambdas
    // -----------
    //
    // Lambdas are used here in place of what would normally be virtual class functions -- functions that can be 
    // overridden in a derived class to talk directly to harwdware instead of using the mouse/keyboard for emulated input.
    //
    // The derived class could then make use of the display elements to show actual data rather than emulated data; or to 
    // get user input to directly drive hardware and display the results. 
    //
    // This is where we get more abstract than we would normally because we want to emulate hardware.
    // Normally, we could just initialize things here or in one routine. 
    //
    // note: lambdas are often a great pre-step to refactoring into multiple functions and before moving to a proper class.  That's
    //       more-or-less what is being done in this program, since its too small at the moment to effectively refactor into a class structure.
    
    auto InitDial = [&]()
    {
        cDial.SetRange(0,150);
        cDial.SetValue(75);
        clcd.SetValue((int) cDial.GetValue());
    };

    auto InitLCD = [&]()
    {
        // Initialze LCD here.  Since this is just the display version, there is nothing to do.
        // a derived class could set it up to communicate to a real LCD.
    };
    
    // CheckDialEvent() -- this effectively combines three functions 
    //    1. Check the hardware event (the dial). 
    //  2. Print out changed data to the debug window 
    //  3. Set the LCD Value (which could be hardware or emulated)
    //
    // They are just put into one function here for clarity and keep the program short, since it's just a small demo
    //
    auto CheckDialEvent = [&]()
    {
        double fValue;
        if (cDial.event.ValueChanged(fValue))
        {
            cDebugWin.printf("{g}Event.{/}  Value = {y}%d{/} -- Raw Value = {c}%d{/} -- Sent Value to Device (LCD)\n",(int) fValue,cDial.GetRawValue());
            clcd.SetValue((int) fValue);
        }
    };

    InitDial();        // Init hardware and/or display elements.
    InitLCD();

    // Create a couple buttons.  Since I didn't put a menu, I put an about button here. 

    auto &cButtonClose = cWin.NewButton(pButtonLoc.x,pButtonLoc.y,120,0,"Exit");
    auto &cButtonAbout = cWin.NewButton(pButtonLoc.x+125,pButtonLoc.y,120,0,"About");

    // Add some hover messages (i.e. tooltips) so when the buttons or hovered-over with the mouse, a message displays 

    cButtonClose.SetHoverMsg("Press to Close Window and Exit program."); 
    cButtonAbout.SetHoverMsg("Show About Box Window explaining this program.");

    // The main event loop. 
    //
    // In this case, we're just looking for a value from the dial (which may be an emulation, as here, or real hardware.  We then display the
    // results to the debug window. 
    //
    // As with some other routines, the calls below to lambda functions would normally be virtual class functions, but are implemented as lambdas 
    // here for more clarity. 
    // 
    // CheckDialEvent() would eventually be refactored into CheckHardware() event as elements are added.

    while(cWin.GetEvent() && !cButtonClose.Pressed())
    {
        CheckDialEvent();        // Check events we might want to emulate (in this case, just the dial)

        // Check other events -- in this case, we just have the about button, but could grow it over time.

        if (cButtonAbout.Pressed()) cWin.DrawSimpleDoc(sAbout, Modal() | CenterXY());   // Options: Modal()     -- this keeps the other windows from being used until the
                                                                                        //                         About Window is dismissed by the user.
                                                                                        //          CenterXY()  -- This centers the About Window on the screen.  Otherwise it
                                                                                        //                         is more in the upper-left part of the screen.
    }
    return 0;
}
