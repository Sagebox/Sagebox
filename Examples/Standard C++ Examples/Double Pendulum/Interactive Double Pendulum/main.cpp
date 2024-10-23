
#include "Pendulum.h"

// Sagebox Interactive Pendulum.  Programming with Sagebox Demonstration Program. 
//
// Also See: Double Pendulum (Simple Version)           -- Only calculates and renders the double pendulum, with no event handling.
//           Interactive Pendulum with Dev Controls     -- Adds sliders, buttons and checkboxes to change various aspects
//                                                          of the double pendulum in real-time while running.
//
// ----------------------------
// Sagebox Interactive Pendulum
// ----------------------------
// 
//      This version adds to the Sagebox Double Pendulum (Simple Version) by adding the following interactive control:
// 
//          1. The pendulum can be stoped and then moved by clicking on the window.
//          2. The bobs can be moved anywhere and the lengths of the rods will changed as you move the bobs.
//          3. After moving the bobs click anywhere on the screen (not on a bob) to drop the pendulum.
//          4. Zoom Control.  You can use the MouseWheel to zoom in and out, enlarging or reducing the zoom of the pendulum 
//                            The size and mass of the pendulum stay the same, as only the zoom changes.
//          5. Moving the Pedulum Up and Down.  Clicking on the Window with the Right Mouse Button will set Y position of the Peg holding up the
//                                          Pendulum.  You can then move the mouse up and down to set the high or low point of the pendulum peg.
// 
//      Functions Added
// 
//          HandleEvents()          - This handles the events occuring such as mouse clicks, mouse moving, Mouse Wheel, etc.
//          HandleMouseClick()      - This handles the mouse click to stop the pendulum moving and determine if a pendulum bob is being moved.
//          HandleMouseDrag()       - Handles the case where the mouse button is down and moving and a bob is being dragged.
//                                    This sets the new position of the pendulum bobs and their rod lengths.
//          DisplayInstructions()   - Displays instructions on how to use the mouse to move the bobs, etc. Disappears when the Mouse clicks on the window
// 
//      The initial angular velocity is set purposely low so that the pendulums can be moved with the mouse and then dropped (by clicking the window once moved)
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
    bool displayInstructions   = true;     // Display instructions.  Set to off as soon as one of the bobs is moved by the user.
    bool showTiming            = false;    // Display real-time timing information in ms 

    // Create main pybox window

    auto& win = Sagebox::NewWindow("Sagebox C++ Interactive Double Pendulum",kw::Realtime() << kw::bgColor("black") << kw::SetSize(1200,700));

    // Initialize the pendulum with the Sagebox Window, as well as the Rod Lengths, Mass1 & Mass2, dampening
    // and starting angles (in degrees)

    DoublePendulum pend(win,220, 185, 10.0, 10.0, -15.0, -15.0, .9985, .33);

    // Build Dev Controls

    int dragging = 0;                           // > 0 when we're dragging a pendulum bob
    double thickness[3] = { 1.0, 2.0, 3.0 };    // Thickness for the Rod Thickness radio box

    auto winSize = win.GetWindowSize();         // Used when we move the display offset up/down with right mouse button
    
    // If the mouse was clicked, Pause or Unpause the display.  Also set a dragging indicator if the user pressed on 
    // one of the pendulum bobs. 
    //
    auto HandleMouseClick = [&]()
    {
        displayInstructions = false;
        bool forceHold = !pend.m_bPause;
  
        CfPoint pPos = win.GetMousePos(); 
        double radius1 = pend.fTopRadius*pend.m_fZoom*pend.m_fCircleMult;
        double radius2 = pend.fBotRadius*pend.m_fZoom*pend.m_fCircleMult;

        dragging = 0;

        if (pPos.WithinRect(pend.m_RodVertex[1]-radius1,pend.m_RodVertex[1] + radius1)) dragging = 1;
        if (pPos.WithinRect(pend.m_RodVertex[2]-radius2,pend.m_RodVertex[2] + radius2)) dragging = 2;

        if (!dragging) pend.m_bPause = forceHold;
        else pend.m_bPause = true;
    };

    // Handle when the mouse is down and moving and we're dragging a pendulum.
    //
    auto HandleMouseDrag = [&]()
    {
        auto pDelta =  (CfPoint) win.GetMousePos()-pend.m_RodVertex[dragging-1];
        pend.m_fAngle[dragging-1] = std::atan2(pDelta.x,pDelta.y);

        pend.m_fLength[dragging - 1] = std::sqrt(pDelta.x*pDelta.x + pDelta.y*pDelta.y)/pend.m_fZoom;
        pend.Reset(); 
    };

    // Handle events occuring in the Controls, such as checkboxes, setting the mass, line thickness, etc.

    auto HandleEvents = [&]()
    {
        if (win.MouseClicked()) HandleMouseClick();
        if (win.MouseDragEvent() && dragging) HandleMouseDrag();

        if (auto iDir = win.MouseWheelMoved()) iDir < 0 ? pend.m_fZoom *= .95 : pend.m_fZoom *= 1/.95; 

        if (win.MouseRButtonDown()) pend.m_RodVertex[0].y = win.GetMousePos().y;
    };

    // Display instruction in the window
    //
    auto DisplayInstructions = [&]()
    {
        win.SetFgColor("Gray192");         // Set the text color

        // {y} = yellow, {g} = green. {30} = set font to size 30pt

        win.SetWriteIndent(300);   // Set the newline position for each line 
        win.SetWritePos(300,100); 
        win.Write("{30}{y}Double Pendulum\n"); 
        win.Write("\nClick on either or both pendulums to set position.\n"); 
        win.Write("Click on the screen (or press {g}\"Drop\"{/}) to drop the pendulum.\n\n");
        win.Write("Click on {g}\"Maintain Rod Length\"{/} to change pendulum angles without changing the rod length.\n\n");
        win.Write("Use the controls to the left to change states while pendulum is in motion or before dropping the pendulum.\n\n");
        win.Write("{p}While the pendulum is moving\n\n"); 
        win.Write("Right-click on the screen to move the display area up and down\n"); 
        win.Write("Use the Mouse Wheel to zoom in and out\n"); 
        win.Write("Click on the display area to stop the pendulums so you can move them.");            
        win.SetWriteIndent(0);     // Set newline indent back to 0

    };

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
        win.printf("Zoom:{x=130}{g}%.2f%%\n",          (float) (pend.m_fZoom*100.0)); 

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
        win.Cls();                      // Clear the window
        SageTimer timer;                // Time the loop so we can see how long it takes 

        // Use EventPending() so we only look for events when we know there is one to look for. 
        // EventPending() is not required, but allows us to not spend time looking for events when nothing has happened.

        if (win.EventPending()) HandleEvents(); 

        pend.Update();                      // Update the pendulum position
        pend.Render();                      // Draw the pendulum

        // Print title in upper center of window. 
        //
        // {w} sets color to white (overriding current gray color for output)

        win.Write(0,10,"{w}Sagebox C++ Interactive Double Pendulum",kw::CenterX() << kw::Font(30));


        if (displayInstructions) DisplayInstructions();
        if (displayValues) DisplayValues();

        auto usTime = timer.ElapsedUs();                                              // Get time we took to update and draw the pendulum in microseconds
        if (showTiming) SageDebug::printf("Time = {p}%f ms\n",(float) usTime/1000);    // Print the time out to the Sagebox process/debug window

        win.Update();                  // Update the bitmap in the window. 

    }
    return 0; 
}
