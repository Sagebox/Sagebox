
#include "Pendulum.h"

// Sagebox Single Pendulum Example.   Programming with Sagebox Demonstration Program.
//
// Also See: Sagebox Double Pendulum in the Standard C++ Examples

// -----------------------
// Sagebox Single Pendulum
// -----------------------
// 
//      This version simply displays the pendulum moving until the window is closed, with the pendulum slowly 
//      coming to a stop. 
// 
//      This Single Pendulum is written just to show a pendulum swinging fro 45 degrees back and forth while it 
//      slowly comes to a stop. 
// 
//      For more of a full pendulum display (single and double) see the Double Pendulum example in the Standard C++ examples.
// 
//      You can change the values such as starting angle, rod length, bob size, window size, etc. 
// 
//      For larger angles, a smaller length with the rod top (cfOrigin) set more in the middle of the window will show the
//      pendulum when it is above the center peg.
// 
// 
// -------------
// Sagebox Usage
// -------------
//  
//      This program shows a single pendulum moving in realtime using Sagebox functions. 
//
//      The code itself is very short, using just a few GUI calls through Sagebox (at least if you don't count the DisplayValues() function
//      that prints real-time information to the window.
//
// ---------------
// Vertical Resync
// ---------------
// 
//      This program works by waiting for the vertical resync, then drawing the pendulum and updating the window. 
//      The kw::RealTime() setting enables the high resolution timer and sets other configgurations to allow better
//      real-time display
//
// --------------
// Timing Display 
// --------------
//
//      When showTiming is set to True, the time for each loop is displayed in the Sagebox Process Window, showing the milliseconds
//      taken to calculate and draw the pendulum.
//
// --------------
// Python Version
// --------------
//
// See the Python version of the single pendulum.
//

int main()
{
    auto& win = Sagebox::NewWindow("Sagebox C++ Pendulum Example",kw::SetSize(500,400) + kw::Realtime()); 

    double angle   = 45*Math::PI/180.0;    // Get a starting angle (try higher angles, such as 120 or 170)
    double damp    = .999;                 // Add a small amount of friction so pendulum slowly comes to a stop.
    double angleV  = 0;
    double accel   = 0;
    double len     = (double) (win.Height()-100);     // Sets the length based on the window height.

    CfPoint origin = { (double) win.Width()/2.0, 0 };        // Sets the top rod vertex.

    win.Cls("black,darkblue");      // Set a black-blue, vertical gradient for the background (ever time cls() is called)
    
    auto showTiming = false;       // When true, shows time it takes to calculate and draw the pendulum (in milliseconds)
    auto showValues = true;        // When true, shows real-time values via DisplayValues() function

    // DisplayValues() - Local/lambda function to display the changing values in real-time
    //
    auto DisplayValues = [&]()
    {
        win.SetWriteIndent(10);    // Set leftmost column at 10 pixels out we can just use \n without setting it for each line.
        win.SetWritePadding(5);    // Add some space between lines when writing to the window (for nicer display)

        // {g}  green, {c} = cyan.  {x=110) sets the X write position to that value so things line up

        win.SetWritePos(10,10); 

        win.printf("Rod Length:{x=110}{g}%.2f\n",  (float) (len)); 
        win.printf("Dampening:{x=110}{g}%g\n\n",   (float) (1.0-(damp))); 
        win.printf("Ang Accel:{x=110}{c}%f\n",     (float) accel); 
        win.printf("Ang Velocity:{x=110}{c}%f\n",  (float) angleV); 
        win.printf("Angle:{x=110}{c}%.2f\xb0\n",   (float) (angle*180.0/Sage::Math::PI)); 
    };

    // Wait for the vertical retrace.    Exits when the window is closed.

    while(win.VsyncWait())
    {
        SageTimer timer;        // Start a timer so we can time the process

        win.Cls();              // Clear the window (either the set bitmap or gradient if bitmap is not loaded)

        // Write a title in the bottom-center of the window with a size 20 font.
        // PadY(-10) brings the message up by 10 pixels so it isn't right at the bottom.
        // uses kw:: namespace for keywords (i.e. Sage::kw namespace)

        win.Write("Sagebox C++ Pendulum Example",kw::Font(20) | kw::JustBottomCenter() | kw::PadY(-10)); 

        // Calculate the new Angle Acceleration, add it to the angle velocity, and then
        // add all of that to the current angle. 

        accel =  (-sin(angle)) / len;
        angleV += accel;
        angle += angleV;

        angle *= damp;        // Apply dampening/friction
        
        // Get the point of the pendulum bob (and end of the rod) based on the angle & length of the rod. 

        auto fPoint = CfPoint(sin(angle), cos(angle)) * len + origin; 
        win.DrawLine(origin,fPoint,"white",kw::PenSize(5.0));                        // Draw the rod. Symbolic PanColor::White can also be used.


        win.FillCircle_f(origin,7.0,PanColor::White);       // Display a center peg (floating-point version of FillCircle)
                                                            // We can also use "white" here, (or "beige" below), but symbolic is faster, so for real-time loops,
                                                            // it's nicer to use the symbolic values (though for this example, it doesn't matter since it doesn't
                                                            // take much processing time for what it does)
                                                               
        // Draw the pendulum bob with MediumVioletRed at 40 pixels, and a border of
        // 5 pixels in Beige.
        // --> We can also use symbolic PanColor::MediumVioletRed and PanColor::Beige here, rather than strings . 
        //     --> Using symbolic colors is much faster for real-time displays.  Since this is a small example, it 
        //         doesn't really matter here, so the string color values are used instead for clarity. 

        win.FillCircle_f(fPoint,40.0,"MediumVioletRed",kw::BorderColor("beige") << kw::PenSize(5.0));  // (floating-point version of FillCircle)
        
        if (showValues) DisplayValues();
        auto iUS = (int) timer. ElapsedUs(); 
 
        win.Update();      // Send the image we created to the window

        // If we're showing timing, put out the milliseconds to the Sagebox Process Window

        if (showTiming) SageDebug::printf("us = {p}%d\n",iUS); 
    }

    return 0;
}
