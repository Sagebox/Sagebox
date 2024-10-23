
// --------------------------------------
// Sagebox  Basic Input Examples
// --------------------------------------
//
// This is a set of 5 examples showing basic input, from basic stdio input/output to using console-based
// Sagebox functions, to using one-line Sagebox dialogs and quick dialogs with multiple input controls. 
//
// ** Note: The examples in this solution can be either Console Mode or Pure Windows programs. 
// **       Select Build->Configuration to Set Console Mode vs. Windows, and other options.
//
// The program simply asks for two numbers, Radius1 and Radius 2, and puts an ellipse with using the two 
// radius values in a Sagebox window.
//
// The examples slowly start adding validation and filled vs. non-filled ellipse selections.
//
//      Examples Description
//      --------------------
//
//      1.     Windows-Based Console Input                  -- uses the Sagebox Window's console-like input/output 
//      2. --> Windows-Based Console Input w/ Validation    -- Adds Sagebox Options to add quick-and-easy validation to the previous version
//
//             This version adds validation to the last example, but does so in some interesting ways:
//
//                  1. Uses the Sagebox Options format to automatically validate input
//                  2. Use win.console.getInteger(), which is what "win >> MyInteger" ultimately calls, we just do it directly here.
//                     This adds the kw::Range() option, will tells Sagebox to validate the input
//                  3. in the "cin" replacement "win" (i.e. the class object for any Sagebox Window), we insert
//                     the kw::Range() in the stream to tell Sagebox to validate the input.  This can be useful for ad-hoc
//                     adding things quickly without having to convert to console.GetInteger() or a dialog box. 
//
//             See more notes above the main() function
//
//             With Validation                              -- Adds Sagebox Options to add quick-and-easy validation to the previous version
//      3.     One-Item Dialog Entry                        -- Uses a Sagebox GetInteger() dialog to get a single Radius value.
//      4.     Two-Item Dialog Entry                        -- Uses Sagebox Quick Dialog to get both radius values (with validation)
//      5.     Sagebox-Based Two Items and a Checkbox       -- Adds a checkbox to the previous example to select filled/non-filled ellipse. 
//

#include "Sagebox.h"

using namespace Sage::kw;       // Sagebox Keyword Options 

// ** Try entering values out of range (i.e. -10, 500, etc.)

int main()
{   
    auto &win = SageBox::NewWindow();    // It's a simple app so we can just create Sagebox and the Window together.
                           
    constexpr int RadiusMin = 1;
    constexpr int RadiusMax = 400;

    RgbColor rgbColor = Rgb("Red");     // We can also use PanColor::Red, SageColor;:Red, {255,0,0} or Windows COLORREF RGB(255,0,0)                                            // We can also use a standard Windows COLORREF RGB(255,0,0); 

    int Radius1, Radius2;

    // The "{cyan}" and "{18}" shows using a font and a color.  {cyan} can also be abbreviated as "{c}"
    
    win << "{16}Let's Draw an Ellipse. {cyan}Enter two radius values between 1 and 400 (try going outside of the range).\n\n";

    // Use the window's console i/o versions for input/output.  If the user closes the window the entry falls through with 
    // a return of 0 -- if the user presses control-C, the program exits as in a console program (this can be turned off).

    // Here, we are using win.console.GetInteger(), which is what "win >> Radius2" below eventually calls. 
    // In this case, we specify "kw::Range" (we do not need to the kw:: part, but typing kw:: will cause the editor to show all options)
    // to tell the GetInteger() routine the acceptable range, and to bring up a message box.
   
    win << "Enter Radius 1: ";
    Radius1 = win.console.GetInteger(kw::Range(RadiusMin,RadiusMax));

    // Use the C++ stream format like "cin" -- here, we can add the same kw::Range() function (this time without specifying "kw::")
    // to tell Sagebox we want to validate the value entered. 
    
    win << "Enter Radius 2: ";
    win >> Range(RadiusMin,RadiusMax) >> Radius2;

    // If we get here, then we know we have some good values. 

    win.FillEllipse(400,400,Radius1,Radius2,rgbColor); 

    win.ExitButton();   // Now we really need an ExitButton() or WaitforClose() (or some method to pause exit),
                        // because the program is done, and everything will close on exit. 
}

