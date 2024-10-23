
#include "Pendulum.h"

// Sagebox Double Pendulum (Simple Version).   Programming with Sagebox Demonstration Program.
//
// Also See: Interactive Pendulum                       -- Allows stopping and moving bobs/chaging length of bobs.
//           Interactive Pendulum with Dev Controls     -- Adds sliders, buttons and checkboxes to change various aspects
//                                                          of the double pendulum in real-time while running.
// ----------------------------------------
// Sagebox Double Pendulum (Simple Version)
// ----------------------------------------
// 
//      This version simply displays the pendulum moving until the window is closed, with the pendulum slowly 
//      coming to a stop. 
// 
//      Change the values in the pend.Init() call to set Mass and Length for each bob, dampening, starting angles (i.e. how high), etc.
// 
//      The bobs are initially set at 150 degrees so when the drop they will swing for quite a while.
//      The Interactive and Interactive with Dev Controls version sets them much lower so that they can be moved with the mouse and then dropped
//      by the user. 
// 
// ----------------------------------
// Sagebox Double Pendulum In General
// ----------------------------------
// 
//      This double pendulum program shows a double pendulum moving in realtime using Sagebox functions. 
//
//      The window is created in the main() function and passed to the DPenulum class where it calculates and draws the pendulum.
//
//      In this version, the pendulum runs until it stops. 
//
//      The Init() function can be changed to change the bob masses, lengths, dampening and other aspects. 
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
// Pendulum Trail
// --------------
//
//      The program shows a fading trail of where the second bob swings around.  This can be disabled by setting m_bShowTrail in 
//      DPendulum to false.
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
// See the Python versions of the double pendulum.
//
int main()
{
    bool displayValues         = true;     // Display current values to the window on/off
    bool showTiming            = false;    // Display real-time timing information in ms 

    // Create main pybox window

    auto& win = Sagebox::NewWindow("Sagebox C++ Double Pendulum",kw::Realtime() << kw::bgColor("black") << kw::SetSize(1200,700));

    // Initialize the pendulum with the Sagebox Window, as well as the Rod Lengths, Mass1 & Mass2, dampening
    // and starting angles (in degrees)

    DoublePendulum pend(win,240, 225, 10.0, 10.0, -150.0, -150.0,.9985, .25);

    // Display relevant pendulum values out to the window. 
    //
    auto DisplayValues = [&]()
    {
        win.SetWriteIndent(10);    // Set leftmost column at 10 pixels out we can just use \n without setting it for each line.
        win.SetWritePadding(5);    // Add some space between lines when writing to the window (for nicer display)

        win.SetFgColor(SageColor::Gray172);         // Set the text color.  Also can be simpl "Gray172" in quotes.

        // {g}  green, {c} = cyan.  {x=130) sets the X write position to that value so things line up

        win.SetWritePos(10,10); 

        win.printf("Mass 1:{x=130}{g}%.2f\n",          (float) pend.m_fMass[0]); 
        win.printf("Mass 2:{x=130}{g}%.2f\n",          (float) pend.m_fMass[1]); 
        win.printf("Length 1:{x=130}{g}%.2f\n",        (float) (pend.m_fLength[0])); 
        win.printf("Length 2:{x=130}{g}%.2f\n",        (float) (pend.m_fLength[1])); 
        win.printf("Dampening:{x=130}{g}%g\n\n",       (float) (1.0-(pend.m_fDamp1*pend.m_fOverflowMul))); 
        win.printf("Ang Accel 1:{x=130}{c}%f\n",       (float) pend.m_fAngAccel1); 
        win.printf("Ang Accel 2:{x=130}{c}%f\n",       (float) pend.m_fAngAccel2); 
        win.printf("Ang Velocity 1:{x=130}{c}%f\n",    (float) pend.m_fAngVel1); 
        win.printf("Ang Velocity 2:{x=130}{c}%f\n",    (float) pend.m_fAngVel2); 
        win.printf("Angle 1:{x=130}{c}%.2f\xb0\n",     (float) (pend.m_fAngle[0]*180.0/Sage::Math::PI)); 
        win.printf("Angle 2:{x=130}{c}%.2f\xb0\n\n",   (float) (pend.m_fAngle[1]*180.0/Sage::Math::PI)); 

        // If there has been an overflow, then display it in red.  This means the math/float-point had
        // some sort of resolution problem, usually when the bob starts wobbling severely.

        // Only display it if we've have more than two, since they can happen at end points here and there. 

        if (pend.m_iOverflowCount > 2)
        {
            win.printf("\n\n\n\n{r}Math Overflow (%d)\n",pend.m_iOverflowCount-2); 
            win.printf("{12}Try increasing dampening (also lowering weight values and weight ratios).");
        }
        win.SetWritePadding(0);    // Reset Padding (since we may be displaying instructions)

    };

    // -- Main Render Loop --
    //
    // Waits for vertical resync and then draws updates the pendulum and renders it. 
 
    while (win.VsyncWait())
    {
        // Use EventPending() so we only look for events when we know there is one to look for. 
        // EventPending() is not required, but allows us to not spend time looking for events when nothing has happened.

        win.Cls();                         // Clear the window
        SageTimer timer;                 // Time the loop so we can see how long it takes 

        pend.Update();                      // Update the pendulum position
        pend.Render();                      // Draw the pendulum

        // Print title in upper center of window. 
        //
        // {w} sets color to white (overriding current gray color for output)

       // win.Write(0,10,"{w}Sagebox C++ Double Pendulum",kw::CenterX() << kw::Font(30));


        if (displayValues) DisplayValues();

        auto usTime = timer.ElapsedUs();                                              // Get time we took to update and draw the pendulum in microseconds
        if (showTiming) SageDebug::printf("Time = {p}%f ms\n",(float) usTime/1000);    // Print the time out to the Sagebox process/debug window

        win.Update();                  // Update the bitmap in the window. 

    }
  
    return 0; 
}
