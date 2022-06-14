
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
//      The opt::RealTime() setting enables the high resolution timer and sets other configgurations to allow better
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
//      When bShowTiming is set to True, the time for each loop is displayed in the Sagebox Process Window, showing the milliseconds
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
    bool bDisplayValues         = true;     // Display current values to the window on/off
    bool bDisplayInstructions   = true;     // Display instructions.  Set to off as soon as one of the bobs is moved by the user.
    bool bShowTiming            = false;    // Display real-time timing information in ms 

    // Create main pybox window

    auto& cWin = Sagebox::NewWindow(SIZE{1200,700},"Sagebox C++ Interactive Double Pendulum",opt::RealTime() << opt::bgColor("black"));

    // Initialize the pendulum with the Sagebox Window, as well as the Rod Lengths, Mass1 & Mass2, dampening
    // and starting angles (in degrees)

    DoublePendulum pend(cWin,220, 185, 10.0, 10.0, -15.0, -15.0, .9985, .33);

    // Build Dev Controls

    int iDragging = 0;                          // > 0 when we're dragging a pendulum bob
    double fThickness[3] = { 1.0, 2.0, 3.0 };   // Thickness for the Rod Thickness radio box

    auto szWinSize = cWin.GetWindowSize();      // Used when we move the display offset up/down with right mouse button
    
     // If the mouse was clicked, Pause or Unpause the display.  Also set a dragging indicator if the user pressed on 
    // one of the pendulum bobs. 
    //
    auto HandleMouseClick = [&]()
    {
        bDisplayInstructions = false;
        bool bForceHold = !pend.m_bPause;
  
        CfPoint pPos = cWin.GetMousePos(); 
        double fRadius1 = pend.fTopRadius*pend.m_fZoom*pend.m_fCircleMult;
        double fRadius2 = pend.fBotRadius*pend.m_fZoom*pend.m_fCircleMult;

        iDragging = 0;

        if (pPos.WithinRect(pend.m_RodVertex[1]-fRadius1,pend.m_RodVertex[1] + fRadius1)) iDragging = 1;
        if (pPos.WithinRect(pend.m_RodVertex[2]-fRadius2,pend.m_RodVertex[2] + fRadius2)) iDragging = 2;

        if (!iDragging) pend.m_bPause = bForceHold;
        else pend.m_bPause = true;
    };

    // Handle when the mouse is down and moving and we're dragging a pendulum.
    //
    auto HandleMouseDrag = [&]()
    {
        auto pDelta =  (CfPoint) cWin.GetMousePos()-pend.m_RodVertex[iDragging-1];
        pend.m_fAngle[iDragging-1] = std::atan2(pDelta.x,pDelta.y);

        pend.m_fLength[iDragging - 1] = std::sqrt(pDelta.x*pDelta.x + pDelta.y*pDelta.y)/pend.m_fZoom;
        pend.Reset(); 
    };

    // Handle events occuring in the Controls, such as checkboxes, setting the mass, line thickness, etc.

    auto HandleEvents = [&]()
    {
        if (cWin.MouseClicked()) HandleMouseClick();
        if (cWin.MouseDragEvent() && iDragging) HandleMouseDrag();

        if (auto iDir = cWin.MouseWheelMoved()) iDir < 0 ? pend.m_fZoom *= .95 : pend.m_fZoom *= 1/.95; 

        if (cWin.MouseRButtonDown()) pend.m_RodVertex[0].y = cWin.GetMousePos().y;
    };

    // Display instruction in the window
    //
    auto DisplayInstructions = [&]()
    {
        cWin.SetFgColor("Gray192");         // Set the text color

        // {y} = yellow, {g} = green. {30} = set font to size 30pt

        cWin.SetWriteIndent(300);   // Set the newline position for each line 
        cWin.SetWritePos(300,100); 
        cWin.Write("{30}{y}Double Pendulum\n"); 
        cWin.Write("\nClick on either or both pendulums to set position.\n"); 
        cWin.Write("Click on the screen (or press {g}\"Drop\"{/}) to drop the pendulum.\n\n");
        cWin.Write("Click on {g}\"Maintain Rod Length\"{/} to change pendulum angles without changing the rod length.\n\n");
        cWin.Write("Use the controls to the left to change states while pendulum is in motion or before dropping the pendulum.\n\n");
        cWin.Write("{p}While the pendulum is moving\n\n"); 
        cWin.Write("Right-click on the screen to move the display area up and down\n"); 
        cWin.Write("Use the Mouse Wheel to zoom in and out\n"); 
        cWin.Write("Click on the display area to stop the pendulums so you can move them.");            
        cWin.SetWriteIndent(0);     // Set newline indent back to 0

    };

    // Display relevant pendulum values out to the window. 
    //
    auto DisplayValues = [&]()
    {
        cWin.SetWriteIndent(10);    // Set leftmost column at 10 pixels out we can just use \n without setting it for each line.
        cWin.SetWritePadding(5);    // Add some space between lines when writing to the window (for nicer display)

        cWin.SetFgColor(SageColor::Gray172);         // Set the text color.  Also can be simpl "Gray172" in quotes.

        // {g}  green, {c} = cyan.  {x=130) sets the X write position to that value so things line up

        cWin.SetWritePos(10,10); 

        cWin.printf("Mass 1:{x=130}{g}%.2f\n",          (float) pend.m_fMass[0]); 
        cWin.printf("Mass 2:{x=130}{g}%.2f\n",          (float) pend.m_fMass[1]); 
        cWin.printf("Length 1:{x=130}{g}%.2f\n",        (float) (pend.m_fLength[0])); 
        cWin.printf("Length 2:{x=130}{g}%.2f\n",        (float) (pend.m_fLength[1])); 
        cWin.printf("Dampening:{x=130}{g}%g\n\n",       (float) (1.0-(pend.m_fDamp1*pend.m_fOverflowMul))); 
        cWin.printf("Ang Accel 1:{x=130}{c}%f\n",       (float) pend.m_fAngAccel1); 
        cWin.printf("Ang Accel 2:{x=130}{c}%f\n",       (float) pend.m_fAngAccel2); 
        cWin.printf("Ang Velocity 1:{x=130}{c}%f\n",    (float) pend.m_fAngVel1); 
        cWin.printf("Ang Velocity 2:{x=130}{c}%f\n",    (float) pend.m_fAngVel2); 
        cWin.printf("Angle 1:{x=130}{c}%.2f\xb0\n",     (float) (pend.m_fAngle[0]*180.0/Sage::Math::PI)); 
        cWin.printf("Angle 2:{x=130}{c}%.2f\xb0\n\n",   (float) (pend.m_fAngle[1]*180.0/Sage::Math::PI)); 
        cWin.printf("Zoom:{x=130}{g}%.2f%%\n",          (float) (pend.m_fZoom*100.0)); 

        // If there has been an overflow, then display it in red.  This means the math/float-point had
        // some sort of resolution problem, usually when the bob starts wobbling severely.

        // Only display it if we've have more than two, since they can happen at end points here and there. 

        if (pend.m_iOverflowCount > 2)
        {
            cWin.printf("\n\n\n\n{r}Math Overflow (%d)\n",pend.m_iOverflowCount-2); 
            cWin.printf("{12}Try increasing dampening (also lowering weight values and weight ratios).");
        }
        cWin.SetWritePadding(0);    // Reset Padding (since we may be displaying instructions)

    };

    // -- Main Render Loop --
    //
    // Waits for vertical resync and then draws updates the pendulum and renders it. 
   
    while (cWin.VsyncWait())
    {
        cWin.Cls();                         // Clear the window
        CSageTimer csTimer;                 // Time the loop so we can see how long it takes 

        // Use EventPending() so we only look for events when we know there is one to look for. 
        // EventPending() is not required, but allows us to not spend time looking for events when nothing has happened.

        if (cWin.EventPending()) HandleEvents(); 

        pend.Update();                      // Update the pendulum position
        pend.Render();                      // Draw the pendulum

        // Print title in upper center of window. 
        //
        // {w} sets color to white (overriding current gray color for output)

        cWin.Write(0,10,"{w}Sagebox C++ Interactive Double Pendulum",opt::CenterX() << opt::Font(30));


        if (bDisplayInstructions) DisplayInstructions();
        if (bDisplayValues) DisplayValues();

        auto usTime = csTimer.ElapsedUs();                                              // Get time we took to update and draw the pendulum in microseconds
        if (bShowTiming) SageDebug::printf("Time = {p}%f ms\n",(float) usTime/1000);    // Print the time out to the Sagebox process/debug window

        cWin.Update();                  // Update the bitmap in the window. 

    }
    return 0; 
}
