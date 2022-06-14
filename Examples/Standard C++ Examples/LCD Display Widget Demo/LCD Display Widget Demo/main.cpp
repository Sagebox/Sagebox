// Lcd Display Widget Example -- Copyright(c) 2020, 2021 Rob Nelson.  robnelsonxx2@gmail.com -- all rights reserved.
// This file, information, and process within are for personal use only and may not be distributed without permission.
// Please modify, copy, do whatever you want for personal uses.  For professional, distribution or commercial uses, 
// contact the e-mail address above

// **************************
// SageBox LCD Widget Example
// **************************
//
//  This program is an example that shows a very simple top-level emulation of an LCD dipslay module. 
//  The program itself is very short, showing how to use SageBox Widgets. 
//  
//  The code for the widget itself will be released at a later date.
//  
//  Widgets are easy to write, and can be used by any SageBox application.
//  In this case, the .H file is included for the widget, and the widget is put into the window
//  with one line of code. 
//  
//  See the code for a general idea of how to use SageBox functions, as well as how the LCD widget works. 
//  
//  The general idea is that later, as an emulation tool, this can be expanded to 
//  work with lower and lower levels, such as the LCD segments and even the output ports. 
//  
//  Once working at a lower level, we can then use it to talk to hardware directly and rather than emulating, we can then use it
//  to report what is happening on an embedded system itself.

// ----------------------------------------------
// Using the Program - Fast Mode vs. default Mode
// ----------------------------------------------
//
//  There are two buttons in this program, and both to the same thing: count from 0 - 1000000. 
//
//  However, they do this in two ways, showing how SageBox can be used generally, but also for much more efficient programs.
//
//  Default Mode Button
//
//    This button simply counts to 1,000,000, updating the LCD at each count.  Being slower than the fast mode button, it also shows how to change
//    the button title and use signals to get input rather than looking at the button through Sagebox calls.
//
// Fast Mode Button
//
//    In many places in SageBox, it is suggested that -- when auto updates are turned off -- the Update be given as Update(10), which will
//    cause the SageBox to only update the window every 10-20 ms (or whatever number you give it). This allows the system to only update when it is 
//    time to update -- rather than updating thousands of times a second, it moves to 50-100 frames per-second.
//
//    This is fast enough for humans to read it and to act in real-time, and also saves a lot of processing power where it is needed.
//
//    Add to that the function UpdateReady(), which tells you if it is time to do an update, a function can skip building the update altogether
//    until it matters.
//
//    With the LCD function, setting it to FastMode() does exactly that -- the functions are identical in that they call the LCD update for each iteration from 0-1,000,000
//    In fast mode, however, the LCD function only builds and updates the display every 10-20ms, allowing the loop to run thousands of times faster since it doesn't have to 
//    update every time.
//
//    The loop itself doesn't know or care, so it just blindly calls the LCD update function, while the LCD Update function takes care of the timing.
//
//    A subsequent LastUpdate() tells the LCD function to update anything that might be left over. 
//
// -----------------------------------------
// SageBox Features Outlined in this program
// -----------------------------------------
//
// The best way to see how SageBox works is to look at the code.  This is a small program and shows many SageBox functions.
// However, here are some specific things to look for in this program:
//
// 1. Efficient, Fast Updating vs. Updating Every Iteration
//
//      See notes above about Fast mode button vs. regular button.  SageBox has the functionality to enable very fast processing with changing
//      how the Update() function is viewed as well as determining whether there is a need for an update or building an output.  In this program, these
//      are best seen in the Fast Mode vs. regular mode usage of the LCD widget, outlined above.
//
// 2. Changing a button's title and color
//
//      In this program, the top button's color and title are changed to re-purpose the button as a stop button.
//      An alternative method is to create two buttons, and Show() and Hide() whichever one is active.
//
//      In this case, changing the title and color of the button was used.
//
// 3. Button Signals and Button ClearEvents()
//
//      Button Signals (and Signals, in general) are really nothing but a boolean set when a button is pressed or a control is used.  
//      In some cases (such as a Checkbox, or Edit Box), a Signal can 
//      also fill another pointer with information about the control (i.e. whether checked or not, or the text in an edit box).
//
//      Signals allow for two things:
//
//        1. Abstraction -- control of processes without the process itself knowing or caring if a GUI is present.  Since a boolean is filled in the button press,
//           a process can just look at the boolean, which can be passed in by whoever calls it. 
//
//           This allows fast prototyping and development, without the need to either insert GUI elements into a lower-level process, or to 
//           overload element to abstract incoming data away from the GUI.
//           
//           Signals are more advanced, and should be used with care.  In development, for example, Signals can be used as a temporary and quick way to 
//           deal with functions, without creating a structure around it to deal with controls being changed.  In many cases, these would be temporary and
//           later replaced with a different structure.
//           
//           However, used with care, Signals can be very useful over the lifetime of a process or program, especially when the same Signal is used
//           for multiple controls -- more about that in another document.
//
//        2. Speed.  Signals allow for greater speed by allowing a process to check a single boolean rather than making a call to determine a 
//           control's status.  For example, in this program, rather than calling Button::Pressed(), which calls into the SageBox core to get the 
//           button status, the status check is handled with basically one instruction that retrieves the boolean directly.
//
// 4. Using CPoint functionality
//
//    CPoint is an OO-based structure similar to POINT, also similar to MSVC CPoint.  CPoint allows math operations on the POINT pair, giving
//    flexibility and brevity to code using the CPoint structure.  CPoint translates automatically to a POINT (and vice-versa) as necessary.
//

#include "Sagebox.h"
#include "Widgets\include\CLCDDisplayWidget.h"

int main()
{

    // Create the window sized to our needs.  InnerSize() is Opt::InnerSize() telling Sagebox we want the interior size to be 320x320
    // (otherwise, the default is to create the window the Width x Height specified, including frame/border). 

    auto& cWin = Sagebox::NewWindow(100,100,320,320,"Sagebox - LCD Widget example");
    cWin.Cls(SageColor::SkyBlue,SageColor::SkyBlueDark);                    // Clear screen with a gradient of two stock colors
    CPoint pLcdLoc = { 10,10 };                                             // Get Start X,Y of Lcd Widget
    CLcdDisplayWidget clcd(&cWin,pLcdLoc.x,pLcdLoc.y,0,Transparent());      // Create the LCD Widget

    CPoint szSize = clcd.GetWindowSize();                                   // Get Size of LCD Windows

    // Use some CPoint arithmetic to place a Text Widget (label) centered under the LCD Widget
    //
    //  We want to put a label, but we don't know how long the text is going to be, so we use 200 pixels know it won't
    //  be any longer than that. 
    //  
    //  In the X direction, we center the 200-pixel-width text widget X with the calculation X = (szSize.x + 200)/2.  To make it 
    //  easy to get the Y position, the calculation szSize.x - (szSize.x+200)/2 is used, which is the same value.
    //  
    //  In the Y direction, since we're putting the text widget just underneath the LCD widget, we get the bottom value
    //  (i.e. pLcdLoc.y + szSize.y, but we do the entire CPoint);

    pLcdLoc += szSize - CPoint((szSize.x + 200)/2, 0);        

    // Create the text widget -- we just harcode 200 since it's more than we'll ever need. 

    cWin.TextWidget(pLcdLoc.x,pLcdLoc.y,200,0,"LCD Emulation Widget",TextCenter() | Transparent());

    // Create two buttons -- one for a slow count and one for a fast count. 
    // with cButtonCount, since it will be slow, we also change the color and label when we run the count
    // (see the Event Loop below)
    //
    // note: CenterX() centers the button in the window, and Transparent() copies the gradient beneath the button so it blends
    // with the window background.

    constexpr const char * sButtonCountTitle = "Count to 1,000,000";        // This is set as a value because we use it more than once (see the event loop)

    auto& cButtonCount = cWin.NewButton(20,200,250,sButtonCountTitle,CenterX() | Transparent()); 
    auto& cButtonFast  = cWin.NewButton(20,240,250,"Count to 1,000,000 (Fast Mode)",CenterX() | Transparent());

    // Create a checkbox so we can switch styles, from regular LCD to blue LED

    auto& cCheckboxBlue    = cWin.NewCheckbox(cButtonFast.GetLocation().x,280,"Blue Led Style",Transparent()); 

    // Create a signal for the cButtonCount button.  They might seem esoteric at first, but they're not -- they're just booleans.
    //
    // This used to check in the middle of the loop.
    //
    // See notes placed before the Signal is used, as well as the following three notes:
    //
    //  1. Speed.  The only reason we use the Signal here is for speed.  We want the "cButtonFast" routine to be as fast as possible, since 
    //      the point of it is that we can call the LCD without a real penalty, as it will only update when necessary (every 50-100 ms or so).
    //      We can use cButtonFast->Pressed() to check for the press, which will gives us the same value.  However, this means we have to make 
    //      a call into the system that is 1+ levels deep, just to get a boolean.  Since we want speed here, we can just check a boolean 
    //      (i.e. GetSignal) and move on. 
    //
    //      If we really wanted speed, we would just check bStop.bPressed and reset the boolean later.  But, we don't need that sort of speed here, just
    //      enough to avoid some calls on every iteration of our counter.
    //    
    //  2. This can just be "bool bStop".  If you look at the ButtonSignal interface, it is just a boolean and a bChecked (for checkboxes),
    //      with some inline functions to get the flag and reset it.  It is used here instead of a plain "bool bStop" so we can use bStop.GetSignal()
    //      which returns the boolean value (i.e. if the button was pressed). It also resets the value so we can use it for the next press, but 
    //      we don't need it here becasue we reset it before every loop with a call to SetSignal() -- it's in the code this way for clarity.
    //
    //      It also serves as more self-documenting code.   
    //
    //  3. Signals can be used effectively in routines that do not want to care or know about the interface -- they just want to check 
    //      a status -- in this case, the pointer to "bPressed" bool can be sent in as a parameter, or a boolean can be used instead of the Button Signal.
    
    ButtonSignal bStop{};            // This gets initialized when we setup the signal with SetSignal()

    // The Main Event Loop -- Get events until the window is closed. 
    // 
    // This function is dormant until an event is received, such as a mouse movement, mouse press, button press, etc. 
    //
    // In this case, we're looking for any two of the buttons or the checkbox presses, and nothing else. 
    // Since this routine is small -- and typical of in-process development -- the routines are run within the loop (this is also
    // to show the code), where if this were to grow, the routines would be refactored out to separate functions to streamline the loop
    // and decouple it from the actions of the controls its checking.
    //
    while(cWin.GetEvent())
    {
        bool bBlueChecked; 
        if (cCheckboxBlue.Pressed(bBlueChecked)) clcd.SetMode(bBlueChecked ? CLcdDisplayWidget::Mode::BlueLed
                                                                           : CLcdDisplayWidget::Mode::PlainLed); 

        if (cButtonCount.Pressed())
        {
            // Change the button text and color -- this is needed because this routine will take a very long time to reach 1,000,000.
            // The Fast version takes no time at all.  
            //
            // Still, this updates anywhere from 1,000-2,000 times a second, which is pretty fast. 
        
            cButtonCount.SetText("Press to Stop Counting");     // Change the button title 
            cButtonCount.SetStyle("Red");                       // Set the button color so it is clear it has changed and means something else

            // A signal is set here so that this function does not need to check cButtonCount->Pressed() for a stop. 
            // This is because we want this routine to be as fast as possible, and cButtonCount->Pressed() makes a couple calls to get 
            // the value. 
            //
            // A Signal is just a boolean, and the bStop.GetSignal() simply checks the boolean and resets it inline, making no real impact
            // on the loop -- since there is no call and we're just checking a boolean, the processor keeps its cache and doesn't spend time
            // calling down into the system to get a value.

            bStop = {}; 

            cButtonCount.SetSignal(bStop);

            for (int i=0;i<1000000;i++)
            {
                clcd.SetValue(i);
                if (bStop.GetSignal()) break;       // We could instead specify "if (bStop.bPressed)" if we wanted to be even more efficient.
                                                    // It's used here for clarity, since we don't need to reset the signal, only to just check it.
            }

            // We got the signal, but there is now a pending event on the button press because we basically used this button as two buttons in 
            // two different ways -- the next time we call "cButtonCount.Pressed()" it will return as TRUE, telling us we have a button press
            // (which was the abort press we reacted to, if it was pressed during the counting). 
            // 
            // This is because the Signal is a low-level operation to directly set the boolean (i.e. it doesn't go through the normal process of
            // clearing the button flags of the actual button object), so the button now has a button press that it knows about and will report
            // the next time GetEvent() returns with another event (such as a mouse move).
            //
            // This happens in this particular program because we re-purposed the "start counting" button for a stop button, and the original button
            // will inherit the button press of the previous usage as a "stop" button.

            cButtonCount.ClearFlags();                          // Clear any events, so we don't get the button press we already responded
                                                                // to when the button was used for a different purpose.
            cButtonCount.SetStyle("default");                   // Get back to our original color
            cButtonCount.SetText(sButtonCountTitle);            // get back to our original button text
        }

        if (cButtonFast.Pressed())
        {
            // Set the fast mode, which only updates every 10-20ms -- still 50-100 frames per-second, and this
            // allows the function to count and set the LCD value on every iteration without the LCD updating every single 
            // call.

            clcd.SetFastMode(true);            // Set the fast mode
            for (int i=0;i<1000000;i++) clcd.SetValue(i);
            clcd.SetFastMode(false);        // Reset the fast mode so we don't have to worry about setting it for the other mode.

            clcd.UpdateLast();              // Update one more time to make sure we got the last count on the lcd window.
                                            // This is because we don't know the last time it updated due to the fast mode, so 
                                            // We have to update it one last time to know we have the current value.
        }
    }
    return 0;
}