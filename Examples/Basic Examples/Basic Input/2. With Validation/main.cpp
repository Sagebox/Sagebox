
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
//                  2. Use cWin.console.getInteger(), which is what "win >> MyInteger" ultimately calls, we just do it directly here.
//                     This adds the opt::Range() option, will tells Sagebox to validate the input
//                  3. in the "cin" replacement "cWin" (i.e. the class object for any Sagebox Window), we insert
//                     the opt::Range() in the stream to tell Sagebox to validate the input.  This can be useful for ad-hoc
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

// ** Try entering values out of range (i.e. -10, 500, etc.)

int main()
{   
    auto &cWin = SageBox::NewWindow();    // It's a simple app so we can just create Sagebox and the Window together.
                           
    constexpr int iRadiusMin = 1;
    constexpr int iRadiusMax = 400;

    RgbColor rgbColor = PanColor::Red; // We could use { 255,0,0 }
                                        // We can also use a standard Windows COLORREF RGB(255,0,0); 

    int iRadius1, iRadius2;

    // The "{cyan}" and "{18}" shows using a font and a color.  {cyan} can also be abbreviated as "{c}"
    
    cWin << "{16}Let's Draw an Ellipse. {cyan}Enter two radius values between 1 and 400 (try going outside of the range).\n\n";

    // Use the window's console i/o versions for input/output.  If the user closes the window the entry falls through with 
    // a return of 0 -- if the user presses control-C, the program exits as in a console program (this can be turned off).

    // Here, we are using cWin.console.GetInteger(), which is what "cWin >> iRadius2" below eventually calls. 
    // In this case, we specify "opt::Range" (we do not need to the opt:: part, but typing opt:: will cause the editor to show all options)
    // to tell the GetInteger() routine the acceptable range, and to bring up a message box.
   
    cWin << "Enter Radius 1: ";
    iRadius1 = cWin.console.GetInteger(opt::Range(iRadiusMin,iRadiusMax));

    // Use the C++ stream format like "cin" -- here, we can add the same opt::Range() function (this time without specifying "opt::")
    // to tell Sagebox we want to validate the value entered. 
    
    cWin << "Enter Radius 2: ";
    cWin >> Range(iRadiusMin,iRadiusMax) >> iRadius2;

    // If we get here, then we know we have some good values. 

    cWin.FillEllipse(400,400,iRadius1,iRadius2,rgbColor); 

    cWin.ExitButton();   // Now we really need an ExitButton() or WaitforClose() (or some method to pause exit),
                        // because the program is done, and everything will close on exit. 
}

