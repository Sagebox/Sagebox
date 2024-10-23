// C++ Sierpinski Triangle Example .   Programming with Sagebox Demonstration Program.
// 
// ---------------------------
// Sierpinski Triangle Example
// ---------------------------
//  
// From Wikipedia: 
// 
//     "The Sierpiński triangle, also called the Sierpiński gasket or Sierpiński sieve, is a fractal with the overall shape of
//     an equilateral triangle, subdivided recursively into smaller equilateral triangles."
// 
// In programming practice, this is a pretty simple implementation and small program to write. 
// 
// However, it also turns out to be a good (and also simple) example of recursive programming. 
// 
// ---------------------
// Notes on this Example
// ---------------------
// 
// This is a good example of using graphics and doing something mathematically interesting at the same time, showing many Sagebox functions
// mixed in with the main algorithm. 
// 
// Input box       -- This example shows using an input box in the Dev Window to get the level of the Sierpinski Triangle
// 
// Range Validation    -- Using the range keyword (from 0 to 11) will validate these values in the input box, with a 
//                        'ok'-based dialog box that lets the user know the value is out of range, then returning to the input
//                        
//                        This allows setting range values without having to validate them, since Sagebox does it already.
//                        
//                        using the keyword kw::CancelOk() will allow the input box to be canceled with any value showing
//                        (e.g. by pressing the escape key or cancel button (when present), in which case the function
//                        InputBox::WasCanceled() can be called to determine if the input box was canceled or has a valid number
//                        within the range specified)
//                        
// ClsRadial()    -- this shows using ClsRadial() instead of the typical Cls() to form a radial clear window effect. It's 
//                   basically for aesthetic reasons, and, in this case, directs the eye to the Sierpinski triangle much more effectively
//                   than a single-color Cls() or vertical-gradient Cls(). 
//                    
// Various {}-based write formatting -- Sagebox uses '{}' directives for text, such as "{75}", which will set the text size to 75, 
//                                      or "{yellow}", will set the text color to yellow.

#include "SageBox.h"

// ---------------------------------------------------------------------
// About the following line (const char * colors[3]) -- We can also use:
// ---------------------------------------------------------------------
//
// RgbColor colors[3] = { SageColor::Green, SageColor::Yellow, SageColor::Purple };
//
// (PanColors, e.g. PanColor::Green, etc. can also be used, but the PanColor values don't tend to stick to primary colors, as with the SageColor colors)
// 
// The above text-based array colors (e.g. "green","yellow", and "purple") are used for ease-of-reading the code
// and to show how it can be easy to use text-based strings when ad-hoc programming, then filling in symbolic values in a refactoring process
// when it matters. 
//
// note:  You can comment out the first 'colors[3]' line and uncomment the above "RgbColor colors[3]" line, and the program will work just the
//        same without any other changes (due to the 'auto' assignment used in CalcTriangles, and that all Sagebox functions take string-based or symbolic colors). 
//

const char * colors[3] = { "green","yellow","purple" };     // define come colors for the three main sections of the triangle display


void CalcTriangles(CWindow & win,int level,CfPointf top_point, CfPointf left_point,CfPointf right_point,int depth)
{
    static auto color = colors[0];  // Set initial color (for when 0 is entered as the level maximum)

    if (!level) win.FillTriangle_f(top_point, right_point, left_point,color);
    else
    {
        
        auto left_mid   = (top_point + left_point) /2.0f; 
        auto right_mid  = (top_point + right_point) /2.0f; 
        auto bottom_mid = (left_point + right_point) /2.0f;

        // Recursively draw smaller triangles.

        if (!depth) color = colors[0];      // set colors for largest sections (when we are at depth 0)
        CalcTriangles(win, level - 1, top_point , left_mid  , right_mid,depth+1  );
        if (!depth) color = colors[1];
        CalcTriangles(win, level - 1, left_mid  , left_point, bottom_mid,depth+1 );
        if (!depth) color = colors[2];
        CalcTriangles(win, level - 1, right_mid , bottom_mid, right_point,depth+1);
    }
}

// Main function
//
int main()
{
    auto& win = Sagebox::NewWindow();      // Create our main window.  Defaults to 1200x800, but we can set it if we want to (we don't want to here)

    auto center = win.GetWindowCenter();  

    win.ClsRadial("darkblue,black");       // Clear the window with a radial gradient
    win.Write("{60}Enter level to draw Sierpinski Triangle.",kw::Just("center"));

    auto & cInputBox = Sagebox::DevInputBox("Enter Level (0-11)",kw::NumbersOnly() + kw::Range(0,11));

    while(Sagebox::GetEvent())
    {
        if (cInputBox.ReturnPressed())              // we could call a function here, but the code is inlined below for easy reading
        {
            int level = cInputBox.GetInteger();    // get the value of the input box

            win.Cls();                              // clear the window display (it will keep with the last radial cls values) --
                                                    //  win.Cls() (when empty) remembers the last Cls() and just repeats it.
            win.Write(CString() << "{40}C++ Sierpinski Triangle\n{15}{cyan}Level " << level,kw::Just("topcenter") + kw::PadY(20));    // Write.. just and just text should be the same. 
             
            CfPointf points[3] = { {0,-300}, {500,350}, {-500,350} };

            CalcTriangles(win,level,points[0] + center,points[1] + center,points[2] + center,0);

            cInputBox.ClearText();      // clear the input box text window  
        }
    }

    return Sagebox::ExitButton();       // Puts up a "program finished" window.  We don't need this here since the program waits until the 
} 