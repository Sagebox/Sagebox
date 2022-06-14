
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
//      The opt::RealTime() setting enables the high resolution timer and sets other configgurations to allow better
//      real-time display
//
// --------------
// Timing Display 
// --------------
//
//      When bShowTiming is set to True, the time for each loop is displayed in the Sagebox Process Window, showing the milliseconds
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
    auto& cWin = Sagebox::NewWindow(SIZE{500,400},"Sagebox C++ Pendulum Example",opt::RealTime()); 

    double fAngle   = 45*Math::PI/180.0;    // Get a starting angle (try higher angles, such as 120 or 170)
    double fDamp    = .999;                 // Add a small amount of friction so pendulum slowly comes to a stop.
    double fAngleV  = 0;
    double fAccel   = 0;
    double fLen     = (double) (cWin.Height()-100);     // Sets the length based on the window height.

    CfPoint cfOrigin = { (double) cWin.Width()/2.0, 0 };        // Sets the top rod vertex.

    cWin.Cls(PanColor::Black,PanColor::DarkBlue);   // In case the bitmap fails to load, a black-blue gradient will occur
                                                    // in future Cls() calls. 
    
    // Set a bitmap to display when the window is cleared.  Remove this if you increase the size of the window, as it doesn't
    // stretch to fit.

    cWin.SetClsBitmap(Sagebox::ReadImageFile("texture-pendulum.jpg"),true); 

    bool bShowTiming = false;       // When true, shows time it takes to calculate and draw the pendulum (in milliseconds)
    bool bShowValues = true;        // When true, shows real-time values via DisplayValues() function

    // DisplayValues() - Local/lambda function to display the changing values in real-time
    //
    auto DisplayValues = [&]()
    {
        cWin.SetWriteIndent(10);    // Set leftmost column at 10 pixels out we can just use \n without setting it for each line.
        cWin.SetWritePadding(5);    // Add some space between lines when writing to the window (for nicer display)

        // {g}  green, {c} = cyan.  {x=110) sets the X write position to that value so things line up

        cWin.SetWritePos(10,10); 

        cWin.printf("Rod Length:{x=110}{g}%.2f\n",  (float) (fLen)); 
        cWin.printf("Dampening:{x=110}{g}%g\n\n",   (float) (1.0-(fDamp))); 
        cWin.printf("Ang Accel:{x=110}{c}%f\n",     (float) fAccel); 
        cWin.printf("Ang Velocity:{x=110}{c}%f\n",  (float) fAngleV); 
        cWin.printf("Angle:{x=110}{c}%.2f\xb0\n",   (float) (fAngle*180.0/Sage::Math::PI)); 
    };

    // Wait for the vertical retrace.    Exits when the window is closed.

    while(cWin.VsyncWait())
    {
        CSageTimer csTimer;     // Start a timer so we can time the process

        cWin.Cls();             // Clear the window (either the set bitmap or gradient if bitmap is not loaded)

        // Write a title in the bottom-center of the window with a size 20 font.
        // PadY(-10) brings the message up by 10 pixels so it isn't right at the bottom.

        cWin.Write("Sagebox C++ Pendulum Example",Font(20) | opt::JustBottomCenter() | PadY(-10)); 

        // Calculate the new Angle Acceleration, add it to the angle velocity, and then
        // add all of that to the current angle. 

        fAccel =  (-sin(fAngle)) / fLen;
        fAngleV += fAccel;
        fAngle += fAngleV;

        fAngle *= fDamp;        // Apply dampening/friction
        
        // Get the point of the pendulum bob (and end of the rod) based on the angle & length of the rod. 

        auto fPoint = CfPoint(sin(fAngle), cos(fAngle)) * fLen + cfOrigin; 
        cWin.DrawLine(cfOrigin,fPoint,PanColor::White,5.0);                        // Draw the rod

        cWin.FillCircle(cfOrigin,7.0,PanColor::White);       // Display a center peg

        // Draw the pendulum bob with MediumVioletRed at 40 pixels, and a border of
        // 5 pixels in Beige.

        cWin.FillCircle(fPoint,40.0,PanColor::MediumVioletRed,PanColor::Beige,5.0);
        
        if (bShowValues) DisplayValues();
        int iUS = (int) csTimer.ElapsedUs(); 
 
        cWin.Update();      // Send the image we created to the window

        // If we're showing timing, put out the milliseconds to the Sagebox Process Window

        if (bShowTiming) SageDebug::printf("Us = {p}%d\n",iUS); 
    }

    return 0;
}
