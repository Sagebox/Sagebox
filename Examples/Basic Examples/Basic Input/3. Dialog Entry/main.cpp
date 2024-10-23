
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
//      2.     Windows-Based Console Input w/ Validation    -- Adds Sagebox Options to add quick-and-easy validation to the previous version
//      3. --> One-Item Dialog Entry                        -- Uses a Sagebox GetInteger() dialog to get a single Radius value.
//
//             Uses one of many Sagebox one-line dialog functions.  GetInteger() brings up a dialog box to get an integer from the 
//             user.  This can have many forms, such as  Modal Window, a window without a cancel button, and -- as used here -- validaiton
//             of the input range. 
//
//             Note the the last example, we used win.console.GetInteger(), which is a console/terminal form of entering an Integer.
//             This is win.GetInteger(), but also "win.dialog.GetInteger()", which is the GUI form. 
//
//      4.     Two-Item Dialog Entry                        -- Uses Sagebox Quick Dialog to get both radius values (with validation)
//      5.     Sagebox-Based Two Items and a Checkbox       -- Adds a checkbox to the previous example to select filled/non-filled ellipse. 
//

#include "Sagebox.h"

using namespace Sage::kw;       // Sagebox Keyword Options 

int main()
{
    auto &win = SageBox::NewWindow();       // It's a simple app so we can just create Sagebox and the Window together.

    constexpr int RadiusMin = 1;
    constexpr int RadiusMax = 400;

    RgbColor rgbColor = Rgb("Red");         // We can also use PanColor::Red, SageColor;:Red, {255,0,0} or Windows COLORREF RGB(255,0,0)                                            // We can also use a standard Windows COLORREF RGB(255,0,0); 

    // In this case, we're showing how to get one number, so we draw a circle instead. 
    // We added "NoCancel()" so the user must enter a value (unless the window is close)

    int Radius = win.GetInteger("Enter a Radius",Range(RadiusMin,RadiusMax));

    // We can stil get a 0 in Radius if the window was closed or canceled.  
    // Since we don't have the main window showing, we can just set "NoCancel()" as an option to 
    // make sure user enter anumber.  NoClose() can also be used to disallow closing the window.
    //
    // The next example will show the use of "NoCancel()" to remove the cancel button.
  
    win.FillCircle(400,400,Radius,rgbColor);       // If the user canceled, this will just fall through with Radius = 0

    // Only show the exit button if we haven't canceled.

    if (Radius) win.ExitButton();       // Since we're not in a sandbox project, we want to wait for the user to press
                                        // a button or close the window.
}
