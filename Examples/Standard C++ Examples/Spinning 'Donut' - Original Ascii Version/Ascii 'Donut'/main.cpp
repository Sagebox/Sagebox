
    // **************************************** DEPRECATED *****************************************
    // 
    // ** Note: This program is a prototype of using a subclassed window, as well as creating a Sagebox object 
    //          vs. calling using static functions
    //
    // ** Mostly, this has been deprecated and is not a good example of how to use Sagebox, but it does 
    //    show an approach where you can:
    //
    //       1. Control when Sagebox gets insantiated and when it gets deleted (it gets deleted when you delete the object created, vs. 
    //          an unknown order when using funcition statically).  As long as the Sagebox object is created before any Sagebox global functions are used,
    //          Sagebox will be deleted when the object is deleted vs. at program termination.
    //
    //       2. Using a sub-classed window.  In this case, the window functions are used as regular functions (which is really deprecated at this point).
    //          This example was left in as most windowing packages tend to work within the subclassed window.  For various reasons, I don't think
    //          this is a good approach in today's programming world.
    //
    // ** This example will probably be changed to the current format of working with static Sagebox functions and the Window as a separate object.


    // >>>>>>>>>>>>>>>>>>>>>>>>>> Original Notes Follow <<<<<<<<<<<<<<<<<<<<<<<<<<


// ***********************
// Ascii Donut Source Code
// ***********************
//
// This example is to show how original source code from a program can be taken and put into 
// SageBox with little or no change. 
//
// This is an ascii-output 'donut' by Andy Sloan. 
//
// The original code was taken as-is, untouched, except for one small change to an output because
// Windows does not support ANSI codes very well in Console Mode. 
//
// The original code is outlined in the source code. 
// There is also a 3D high-res version (see other projects), which shows how current code can be 
// taken and enhanced greatly by using Sagebox graphic functions.
//
// This program is also a good example of using the SandBox project type in Sagebox.  See documentation below.
//
// -----------------------------------------
// SageBox Features Outlined in this program
// -----------------------------------------
//
// For the most part, the code can be reviewed to see SageBox elements -- in most cases, the 
// code is self-explanatory or you can hold the mouse over the function for documentation.
//
// Features Outlined
//
// 1. Sandbox Project 
//
//    The Sandbox-Style project in Sagebox is a great way to quickly start up a session with SageBox. 
//    the main function is derived from CWindow, and the basic interface works the same as C/C++, with 
//    printf(), and SageBox functions can now be directly called. 
//
//    When the main() funciton is entered, the window is already created, and it is simple to perform
//    various functions with SageBox, rather than a) the need to create a window and b) de-referencing the window, i.e. cWin.printf or 
//    cwin.DrawRectangle() vs. simple "printf()" and "DrawRectangle()"
//
//    When the project is opened, CSageBox is already created along with the window, and programs can be instantly started in the 
//    Sandbox's main() function.
//
//    All that is needed is the following code, which is already written when the project is loaded:
//
//        #include "CSageBox.h"
//        QuickSandbox(CSandBox)
//
//        void CSandBox::Main()
//        {
//            // Start Code here
//        }
//
//        int main()
//        {
//            CSageBox cSageBox("Enter program name here or leave blank");
//            cSageBox.Main(new CSandBox);
//            return 0;
//        }
//
//      While the SandBox-style project is not recommended for large projects, it is great for quick prototypying
//      and trials because there is no work to do to get up and running right away.
//
// 2. Adapting existing code with little or no changes & footprint
//
//      The source code for this program was taken from a console-mode program and has been changed
//      very little from the original.
//
//      One of SageBox's main paradigms is to allow procedural, non-event programs to use Windows, GUI-Based functionality without 
//      changing the structure of the code.  This allows additions to code with little or not footprint.
//
//      SageBox code can be compiled out with a switch and/or removed easily.  Also, Sagebox's design allows
//      for methods to access SageBox functionality without core routines knowing of its existence, keeping 
//      pure functions pure. 
//
// 3. Using Child Windows
//
//      This program shows how to use Child Windows, which are embedded in the parent window.  
//      Child windows are otherwise exactly the same as Popup windows createed with NewWindow(),
//      except that thay may also have a border.
//
//      See the code for more details.
//

#include "SageBox.h"

using namespace Sage::opt;      // Sagebox options

int main()
{
    auto& cWin = Sagebox::NewWindow("SageBox - Ascii Donut by Andy Sloan"); 

    cWin.Cls(SageColor::SkyBlueDark,SageColor::SkyBlue);             // Clear screen with a gradient, using stock colors. 

    const char * sFont = "Courier New,14";                      // Set the Ascii Font we want to use (i.e. non-proportional font)
    CPoint pTermSize = CPoint( 80, 25 ) * cWin.getCharSize(sFont);   // Get an (80x25) window for our terminal, sized to our font
    CPoint pWinSize = CPoint( 800,550 );                    
    cWin.SetWindowSize(pWinSize,true);                               // Set our main window size (true == size the interior this size)

    // Get the window where we will print out the ascii characters. 
    //
    // CenterX() centers the window in the X-dimension in the parent window
    // AddBorder() adds a small border around the window
    // Font() Sets the font we've chosen

    auto& AsciiWindow = cWin.ChildWindow(0,40,pTermSize.x,pTermSize.y,CenterX() | AddBorder() | Font(sFont));
    AsciiWindow.Cls(SageColor::Black);     

    // Set the background to Opaque since we're not clearing the screen and overwriting previous characters.
    // The default is that characters are transparent and won't fill in the background.

    AsciiWindow.SetBkMode(BkMode::Opaque);

    // Set a fire-and-forget message centered above the Ascii Window, giving credit to the original author

    cWin.TextWidget(0,10,"Ascii Donut by Andy Sloan", Font("arial,18") | CenterX() | Transparent());
    
    // Create a close button so the user can stop the program -- the close window button also works
    // Center() Centers it in the X-dimension.

    auto& CloseButton = cWin.NewButton(0,pWinSize.y-50,"    Press Button or Close Window to Exit    ",Center());

    // Create another text widget.  This updates the count, and also redraws the parent's background (which is a gradient),
    // so we don't have to worry about any of it. (the 00000000 is to reserve needed space)

    auto& cText = cWin.TextWidget(80,pTermSize.y + 40+7,"UpdateCount = 000000000000", fgColor(SageColor::Yellow) | Font("Arial,14") | Transparent());
    
    int iCount = 0;

    // ----- Original Code ------
    //
    // The following is the original code, as-is, showing how we can take existing code and put it into 
    // SageBox with little or no modification.
    //
    // In this case, the code is exactly the same -- except for an ANSI print routine that had to 
    // change because of the difference with Microsoft Console Mode (this is noted below)

    double A = 0, B = 0, z[1760];
    char b[1760];
    
    // This loop runs very fast. It could be set to wait for the vertical resync (with Sagebox::WaitVsync(), or just a Sleep(50) or so to sleep 50ms between loops

    for (;;)        // original code, as opposed to a more canonical (these days) while(true)
    {
        memset(b, 32, 1760); 
        memset(z, 0, sizeof(z));
        for (double j = 0; 6.28 > j; j += 0.07)
            for (double i = 0; 6.28 > i; i += 0.02) {
                double    c = sin(i),
                        d = cos(j),
                        e = sin(A), 
                        f = sin(j), 
                        g = cos(A),
                        h = d + 2, 
                        D = 1 / (c * h * e + f * g + 5), 
                        l = cos(i),
                        m = cos(B),
                        n = sin(B),
                        t = c * h * g - f * e;
                int x = 40 + (int) (30 * D * (l * h * m - t * n)),
                    y = 12 + (int) (15 * D * (l * h * n + t * m)), 
                    o = x + 80 * y,
                    N = (int) (8 * (((f * e - c * d * g) * m - c * d * e - f * g - l * d * n)));
                if (22 > y && y > 0 && x > 0 && 80 > x && D > z[o]) {
                    z[o] = D;
                    b[o] = ".,-~:;=!*#$@"[N > 0 ? N : 0];
                }
            }
    
        AsciiWindow.SetWritePos(0,0);       // Note: this is the only difference from the original.
                                            // The original line used an ANSI code to set the cursor position to the 
                                            // top-left corner of the screen.  This does the same thing, as the method isn't
                                            // really available in Win32 Console Mode

        for (int k = 0; 1760 > k; k++) AsciiWindow.putchar(k % 80 ? b[k] : 10);

        A += 0.04/2;        // I may have put the "/2" in there to slow it down.
        B += 0.02/2;

        // ------- End of Original Code --------

        cText.Write(CString() << "Update Count " << ++iCount);    // Put out the count to our text widget

        // See if the close button was pressed or the window is closing.

        if (CloseButton.Pressed() || cWin.WindowClosing()) break;
        // CSagebox::VsyncWait(); // Add this line to slow down the display to 60fps (or whatever the monitor refresh rate)
    }
    return 0;
}
