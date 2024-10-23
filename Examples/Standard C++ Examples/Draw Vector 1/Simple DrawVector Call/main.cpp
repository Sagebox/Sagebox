// ----------------------------------------------------------------------------------
// Sagebox Draw Vector (Example #1).   Programming with Sagebox Demonstration Program
// ----------------------------------------------------------------------------------

// ------------------------
// Examples in this project
// ------------------------
// 
// 1. Simple Draw Vector Call (this example) --  Calls CWindow::DrawVector().
// 2. Using CDrawVector Class                --  Uses the CDrawVector class directoy for a more refined and flexible way to draw and change vectors.

// ------------------------
// Draw Vector (Example #1)
// ------------------------
//  
// The DrawVector() function in Sagebox draws a vector from point P1 to point P2, with a rounded beginning and an arrow on the end.
// 
// DrawVector() is much like a line, but can have 'caps' (i.e. beginning and end 'caps' such as rounded, arrow, etc.), a title, 
// varying angles, and can otherwise be controlled in its shape, orientation, and display with various keywords.
// 
// Drawing a vector is simple, with only points P1 and P2 needed, as well as a line thickness.  Everything else is automatic
// and can be controlled through keywords. 
// 
// See DrawVector() documentation (by hovering the mouse over the DrawVector() function in the editor) for information on the various
// keywords that can be used to change how the vector displays. 
// 
// --> Also type "vkw::" in the editor for a full list of available keywords with documentation.
// 
// ---------------------------------------------------------------------------------------------
// Example#1 (this example) - A Vector rounding a circle, using Dev Sliders to control the angle 
// ---------------------------------------------------------------------------------------------
// 
// This example sets up two Dev Sliders to control the angle of a vector rotating around a circle.
// 
// --> Calling CWindow::DrawVector().  This example calls CWindow::DrawVector() as an all-in-one solution to draw the vector.
//                                     This is a way of drawing vectors quickly with just one call.
//                                     See the CDrawVector example that shows how to create a vector object and use it 
//                                     in just a couple lines of code for more flexibility, re-usage, etc.
//
// The first slider controls where the vector is on the circle.  The vector automatically orients to the circle 'normal'. 
// 
// The second slider controls the angle of the slider from its relative placement, allowing the vector to be rotated in place on its 
// point in the circle.
// 
// -------------
// Various Notes
// -------------
// 
// 1. Slider Rollover -- The 'rollover' attribute is set on both sliders.  This means the slider will wrap around when using the mouse wheel.
// This allows a smooth motion with the mouse wheel.  Otherwise, with the mouse wheel, the slider would stop at either end without rolling over.
// 
// Using the mouse won't rollover/wrap-around.  This only applies to the Mouse Wheel.
// 
// 2. Dev Slider -- This Draw Vector example also makes a good example of quick and simple usage of Dev Slider, using only 
// two lines each: 1) to set up the slider, and 2) to use the slider (getting its value)
// 
// 3. write_xy() centering and fonts -- note how the write_xy() function is used to center text on multiple lines, as well as 
// setting a quick color change and font-size change using {} brackets.
// 
// ---------------------------------
// 4. Keywords used for DrawVector() 
// ---------------------------------
// 
// The following keywords are used in the DrawVector() call:
// 
//         1. LabelJust("leftcenter") -- This sets the label in the 'left center top' area.  Other options can be left, center, bottom left, etc.
// 
//                                       --> note: vkwType::LabelJustType can be used here, which can provide a cleaner method:
// 
//                                                e.g. LabelJust(vkwType::LabelJustType::LeftCenter);
//         
//         2. Title(title) --  This sets the label on the vector (Label() can also be used).  title is a string already set up for the title. 
//                               Otherwise, no title displays on the vector.
//                             
//         3. Color(PanColor::Orange) --    This causes the color of the vector to be orange.  The label color defaults to white, but can be changed
//                                          with the keyword "LabelColor("yellow"), e.g. LabelColor("yellow")
// 
//                                          --> note: "orange" may also be used here, e.g. Color("orange")
//                             
//         4. SetCenter((CfPoint) pOut) -- "SetCenter" causes DrawVector() to disregard points P1 and P2, using only the length and angle 
//                                          calculated from P1 and P2.  It then sets the center of the line to the point set in set_center, 
//                                          then draw the vector at the angle previously calculated for line P1-P2, with any angle setting 
//                                          added to this angle (see Angle() or AngleRad() keyword)
// 
//         5. AngleRad(dispAngle+Math::Rad90)   -- Since SetCenter() is used, and the angle is 0 between P1 and P2, this rotates the vector from it's 
//                                                  center to this this new angle.  This allows the program to set the circle point as the center and then
//                                                  set the angle to the angle to the point + 90 degrees to make it normal to the circle. 
//      
//                                                  -- >note:   AngleRad() is used to specify the angle in radians.  Angle() or AngleDeg() can be used
//                                                              to specify the angle in degrees.
// 
// -----------------------
// 5. NewWindow() keywords
// ----------------------- 
//      
//      1. NoAutoUpdate --  Tells the window to not update automatically.  Since we're in a graphics, real-time-ish
//                          loop, the window, by default, updates when enough time has passed (about 10ms).  But, we don't 
//                          want it to update while we're drawing, to avoid a flicker.  NoAutoUpdateTrue() keeps it from udpating
//                          until we want it to.
// 
//      2. Fullscreen   --  When true, it creates a fullscreen window with no border, otherwise it defaults to the normal size with 
//                          a window title and menu bar.
// 
//                 This is true because we set no size for the window. If we set a size with kw::SetSize(), the window is created with no 
//                 borders, title, menu bars, etc. and placed in the center of the screen at the specified size.
//
//                                
// 6. ExitButton() -- This is not necessary, since the program does not end until the user closes the window.  However, it is 
//                     always nice to get an indication from the program itself that it has ended.  ExitButton() is a quick and 
//                     useful function for this purpose.                                                                                                               
// 
#include "SageBox.h"
#include "CDrawVector.h"

using namespace Sage::kw;       // Sagebox options/keywords

int main()
{
    bool fullScreen = false;   // Set to true for a window that fills the entire screen with no borders. 

    // Create the main window. 
    // 
    // Keywords used: 
    //
    // bgColor      -- Sets the background of the window to black instead of it's default sky blue
    // 
    // See notes above about usage of NoAutoUpdate() and Fullscreen()
    // 
    auto & win = Sagebox::NewWindow(bgColor("black") + NoAutoUpdate() + Fullscreen(fullScreen));
    
    // If we are fullscreen, move the dev box to right righ and a little lower.

    if (fullScreen) Sagebox::DevAdjustWinPos(425,50);  

    auto winCenter = win.GetWindowCenter() + CPoint{0,60};    // Get the window center + a little vertical offset
     
    int radius = 200;      // Our basic radius for the circle and where to put the vector.

    Sagebox::DevSetBgColor("Blue48,SkyBlue");   // Set some colors for the dev window, rather than the regular dark-gray gradient

    // Sets a title in the dev window..

    Sagebox::DevText("Sagebox C++ DrawVector() Function Test",kw::Font(18) + kw::TextColor("cyan")); 

    // Create our two sliders. 
    // 
    // See notes above about AllowRollover()
    //

    auto& sliderVecPos     = Sagebox::DevSlider("Vector Position",kw::Range(-180,180) + kw::AllowRollover() + kw::Default(-90)); 
    auto& sliderVecAngle   = Sagebox::DevSlider("Vector Angle",kw::Range(-180,180) + kw::AllowRollover()); 

    // Loop forever until the window is closed, or the dev window is closed.
    //
    // When DevWindowClosed() is called, it adds an 'x' button in the upper-right of the Dev Window. 

    while(win.GetEvent() && !Sagebox::DevWindowClosed())
    {
        // If the mouse wheel is moved, adjust the position of the vector on the circle.
        // --> This is not necessary because we can use the mousewheel on the slider.
        //
        //     This allows us to use the mousewheel anywhere on the screen. 

        if (auto opMouseWheel = win.MouseWheelMoved()) sliderVecPos.AdjustPos(*opMouseWheel); 

        // Get the two angles from the sliders. 

        auto vecPos    = Math::ToRad(sliderVecPos.GetPos()); 
        auto vecAngle  = Math::ToRad(sliderVecAngle.GetPos()); 

        win.Cls();         // Clear the window so we can draw out to it.
        win.DrawGrid();    // Draw a graph-like grid.

        // Write out some titling.  See the notes above about usage of the write() function here. 

        win.Write(0,70,"{50}Sagebox C++ DrawVector() Function Test\n"
                        "{18}{LightGreen}(CWindow::DrawVector() call example)",kw::JustCenterX());

        // We can use symbolic SageColor::LightBlue instead of quotes (it's faster, but not an issue for this program)

        win.DrawCircle(winCenter, radius, "LightBlue", Sage::kw::PenSize(15));  // Draw the large circle in light blue with a 15-pixel radius
        win.FillCircle(winCenter,15,"LightBlue");                               // Draw the small circle as a filled circle.

        // Move around the circle, drawing a vector at each point

        auto dispAngle = vecPos+vecAngle; 
        if (dispAngle >= Math::Rad360) dispAngle -= Math::Rad360f; 

        auto x = cos(vecPos)*((double) radius+7.5+5.0);  // We add 7.5 for the circle radius and 5.0 for the vector radius -- to place
        auto y = sin(vecPos)*((double) radius+7.5+5.0);  // the vector just on top of the circle borders.

        CPoint pOut = CPoint{(int) x,(int) y} + winCenter;      // Where to put the vector center.

        auto title = CString() << "Rotation Angle = " << (int) Math::ToDeg(vecPos) << "°";   // Title for the vector.

        // Draw the vector with various keywords.  See notes above for how these keywords are used.
        //
        // note: we don't need vkw:: here if we want to express 'using namespace vkw' above.  vkw:: is left in for clarity for the example.
        //
        // Since we're using SetCenter(), with the {0,0} and {350,0} for P1 and P2, we just want to specify a length of the vector at 0 degrees.
        //
        win.DrawVector({0,0},{350,0},10,"orange",vkw::LabelJust("leftcenter")   << 
                                         vkw::Title(title)                      << 
                                         vkw::SetCenter((CfPoint) pOut)         << 
                                         vkw::AngleRad(dispAngle+Math::Rad90));

        // Note there is no update() function here, as often seen in various Sagebox programs.
        // This is because the GetEvent() call will update any windows that need it (i.e. have been drawn on since the last update).
    }
    return Sagebox::ExitButton();   // Not necessary since the window doesn't close until the user closes it, but it's always nice to get 
                                    // The message that the program has understood it is closing down.
}