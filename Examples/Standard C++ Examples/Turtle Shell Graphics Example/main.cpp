
// ----------------------------
// TurtleShell Graphics Example
// ----------------------------
//
// This example uses the TurtleShell prototype library.
//
// The TurtleShell library is based on Turtle Graphics, using the same commands with some added commands.
// This is just in a prototype stage, with planned additions for: 
//
//     - More graphics functions, such as curves, other interesting shapes.
//     - GPU usage
//     - 3D Turtle Graphics
//     - Planar and 3D shapes
//
// The TurtleShell class features real-time-based turtle graphics for smooth animations.
// 
// TurtleShell is not part of the Sagebox package, per se, but is an example of an external library written using Sagebox functions,
// which is why "TurtleShell.h" is included -- the library is currently compiled into Sagebox mostly for convenience. 
//
// ------------------------
// This TurtleShell Example
// ------------------------
//
// This example started off simply, as a set of 36 triangles rotated in a circle about a larger circle.  The triangles were filled with a transparent color
// that range through all hues.  The main image was than redrawn continuously in real-time at an incresing angle to rotate the image.
//
// ** Playing With Turtle Graphics
//
// Playing with some code and expanding the angle of the triangle sides changed the image into something interesting, especially with it's real-time application.
// This example is a good example of how just playing around and experimenting with slight changes can be fun and create interesting images.
// 
// ** Real-Time Turtle Graphics Example
//
// This example also shows how nice real-time display is with turtle graphics.  By not showing the items being drawn, and only showing the result at the end,
// the resultant image is slowly changed into a nice real-time, moving and growing display.
//
// This example shows using the sleep(0) function that shuts off any intermediate display (allowing the program to use update() to show the result only),
// as well as kw::Realtime() to set a real-time status for the window that is initially created.
//
// ** Showing timing
// 
// Uncomment the lines regarding timing to show how many milliseconds it takes to draw each frame.
// ------
// Future
// ------
//
// The TurtleShell class is experimental, but shows some great results.  This example is a small example, and there will be more examples and additons over time.
// I'm basically waiting for feedback, suggestions, and requests to let help direct its future path.
//
// 3-D, Planar, and other drawing aspects, as well as GPU functionality are planned for future updates.
//
#include "SageBox.h"
#include "TurtleShell.h"

int main()
{
     auto & win = Sagebox::NewWindow(kw::Realtime());  // Create a window.  Set real-time status since we want to use the vsync to set the fps.

    TurtleShell t(win);    // Create Turtle Shell object.
    
    int rotAngle = 0;
    
    t.set_speed(0);         // Sets speed to 0, to not display items as they are drawn (> 0 numbers are milliseconds to wait between draw items)

    // In the main loop, wait for the vertical resync so we can have smooth motion at the sync-rate of the monitor (usually 60fps)

    while(win.VsyncWait())
    {
        // Put some text on top of the window

        win.Cls("black"); 
        win.Write("{40}Sagebox Turtle Graphics\n{17}{lb}Some Random Turtle Graphics Example",kw::CenterX() + kw::fgColor("White") + kw::PadY(20));

        // The code below uses multiples of the angle, so we rotate by basically 1440 degrees (or 360*4)

        if (rotAngle > 360*4) rotAngle -= 360*4;
        t.color("white(150)");  // Set main color (for the lines drawn) at white with 150 (out of 255) opacity (i.e. a little transparent)
        t.pen_size(2);   

 //       SageTimer csTimer;                        // Start a timer -- ** uncomment to show timing information
        
        // Draw 36 triangles (or semi-triangles, since we manipulate the angles of the triangles based on current iteration value)
        //
        for (int i=0;i<36;i++)
        {
            auto color = SageTools::HSLtoRGB((double) (i*10)/360.0);     // Get a color that represents a changing hue 

            // Turtle Graphics Code

            t.pen_up();             
            t.set_pos(0,0);                             // Go back to center
            t.set_heading(-i*10+rotAngle/2);            // Set angle of triangle placment and triangle itself (/2 is just to slow down the rotation m the vsync)
            t.forward(50);                              // Move out to the first vertex
            t.pen_down();

            t.begin_fill(RgbA(color,92));               // Start a fill for the triangle with our chosen hue color and transparency of 92

            t.forward(200);                             // Draw first semi-triangle side
            t.left((float) -rotAngle/4.0f-120.0f);      // Rotate 60 degrees to the last line for our triangle          
            t.forward(200);                             // Draw second triangle side
            t.left((float) -rotAngle/4.0f-120.0f);      // Rotate another 60 degrees to the triangle interior
            t.forward(200);                             // Draw third triangle side
            t.end_fill();                               // Fill the triangle
        }
        win.Update();      // For real-time windows, using VsynvWait(), we must update the window ourselves
        rotAngle += 2;     // Set a new angle and prepare to draw the entire triangle-wheel again at a rotated angle.

//        int iElapsedUs = csTimer.ElapsedUs();                     // ** uncomment to show timing information
//        SageDebug::printf("Elapsed = {g}%d us\n",iElapsedUs);     // ** uncomment to show timing information

    }

    return Sagebox::ExitButton();   // We don't really need this since the user closes the window, but it's nice to have the program put up
                                    // a "program is finished" message with a button to press to close it all down.
}