// Interactive Mandelbrot Example -- Copyright(c) 2020, 2021 Rob Nelson.  robnelsonxx2@gmail.com -- all rights reserved.
// This file, information, and process within are for personal use only and may not be distributed without permission.
// Please modify, copy, do whatever you want for personal uses.  For professional, distribution or commercial uses, 
// contact the e-mail address above

// ******************************
// Interactive Mandelbrot Example
// ******************************
//
// This set of Mandelbrot programs shows creating and displaying a Mandelbrot and/or Julia Set 
// in various stages, from very basic, to faster, all the way to an interactive program.
//
// This is #4 Interactive Mandelbrot Example
//
// ---------------------------
// Mandelbrot Example Projects
// ---------------------------
//
//  1. Basic Mandelbrot
//
//      This is a simple Mandelbrot with color, using std::complex. 
//      It's slow, but works and is only about 25 lines of code.
//  
//  2. Basic Mandelbrot Faster
//
//      This adds speed to the mandelbrot by replacing std::complex with CComplex, 
//      Windows DrawPixel() with a bitmap funciton via CSageBitmap. 
//
//  3. Smooth Color, Julia Set, and Dev Window
//
//      This is a small change from the previous version, adding smooth coloring and the Julia
//      set to the output.   
//
//      The Dev Window is also used to show the ease-of-use of the Dev Controls without affecting your main code or window.
//
//      This version also adds some more power by assing z.sq() instead of z*z for a little faster
//      Mandelbrot calculation.  FastSetPixel() is also used in CSageBitmap to set the pixel much faster.
//
//  4. MandelBrot Interactive (zoom in/out, resize window, set Julia Set and more) 
//
//      This shows how to add Development-based controls to change values of the Mandelbrot
//      with just a few lines of code.
//
//      It shows using GetEvent() in the main eventloop to get and react to events. 
//  
//
// -------------------------------------------
// About This Version (MandelBrot Interactive)
// -------------------------------------------
//
//  This version adds interativity where you can zoom in and out of the image with the MouseWheel and
//  can click anywhere on the image to set the center.
//
//  The window can also be resized for smaller or larger windows -- smaller windows print faster.
//  The iterations can also be set in an edit box or with the mousewheel, as well as number of other options.
//
//  Run the program and look at the source code fore more information and comments.
//
//  I expect to do a video series on this set of programs and will explain more in-depth about the use of SageBox in
//  the videos.
//
// -------------------------------------------------------
// Using elements of this program for your own programming
// -------------------------------------------------------
//
// As with all SageBox demo code, it is written to show how SageBox works, but also general C/C++ programming practices.
// 
// ** Please feel free to use any of this code for your own personal purposes **
//
// SageBox is specifically written to be portable and used by example and the "intellisense" comments that come up for each function.
// Most SageBox code snippets are also portable and can be taken from one program and used in another.
//
// SageBox is free for personal use. 
//

#include "SageBox.h"

// Class Declaration
//
// (this would normally be in its own file, but is included in the main
//  source file for context)
//
class CMandelbrot
{
    SIZE        szWinSize   = { 800, 600 };     // Initial Window Size
    CfPoint     cfWinSize   = szWinSize;        // Get a floating-point version of window size
    CfPoint     cfCenter    = { -.6, 0 };       // Initial Mandelbrot Center
    CComplex    cfJulia     = { -.4, .6 };
    double      fRange      = 3.7;              // Initial Range (i.e. "zoom" factor)
    int         iMaxIter    = 50;               // Max Mandelbrot Iterations
    bool        bJuliaSet   = false;
 
    static constexpr int kColorTableSize = 16384;   // Color Table Size (must be divisible by 16)
    RGBColor_t rgbColorTable[kColorTableSize];      // Color Table derieved from rgbColors

    static double rgbTable[17][3];

    CWindow * cWin      = nullptr;      // Mandelbrot Window
    CWindow * cDevWin   = nullptr;      // DevWindow (here so it can be used in multiple functions)
    void CreateColorTable();
public:
    void DrawMandelbrot(bool & bAbortSignal);
    int main();
};

// Color Table based on Wikipedia Mandelbrot Colors
//
double CMandelbrot::rgbTable[17][3] = 
{
    { 0, 0, 0       }, { 25, 7, 26     }, { 9, 1, 47      }, { 4, 4, 73      }, 
    { 0, 7, 100     }, { 12, 44, 138   }, { 24, 82, 177   }, { 57, 125, 209  },
    { 134, 181, 229 }, { 211, 236, 248 }, { 241, 233, 191 }, { 248, 201, 95  },
    { 255, 170, 0   }, { 204, 128, 0   }, { 153, 87, 0    }, { 106, 52, 3    },
    { 106, 52, 3    }, // Added one more to make CreatColorTable()'s job easy
}; 

// CreateColorTable() -- Creates a smooth n-sized color table based on the 16-color rgbTable. 
// 
// The output table *kTableSize) must be divisible by 16. 
//
// Note the addition of a 17th element in the rgbTable that is a repeat of the
// previous color -- this is so we can index the next color. This allows the writing of a 'dumb'
// routine.  It also means the last segment is a solid color, but with the Mandelbrot, the edge color
// usually is only seen as an outline, so it shouldn't make too much of a difference in the output.
//
// It might be nice to put another color in its place, but I wouldn't know what color to add, so
// I just repeated the previous color.
//
void CMandelbrot::CreateColorTable()
{
    for (int i=0;i<16;i++)
    { 
        for (int j=0;j<kColorTableSize/16;j++)
        {
            double fPercent = 16*(double) j/kColorTableSize;

            // Use a lambda function so we can just repeat it for each color (Red, Green, Blue) 

            auto newColor = [&](int c) { return (int) ((rgbTable[i][c])*(1.0-fPercent) + (rgbTable[i][c+3])*fPercent); };
            rgbColorTable[kColorTableSize*i/16 + j] = { newColor(0), newColor(1), newColor(2) }; 
        }
    }
}
void CMandelbrot::DrawMandelbrot(bool & bAbortSignal)
{   
    static int iCount;  // Redraw Counter, just for reference, assuming one thread (for now)

    cDevWin->printf("Drawing Mandelbrot ({lightyellow}Julia Set = %s) {lb}(#%d) {/} {/}\n",bJuliaSet ? "true" : "false",++iCount);
    cDevWin->printf("Center = {cyan}(%f,%f){/} -- Range = {cyan}%.14lf{/}\n",cfCenter.x,cfCenter.y,fRange);

    clock_t ctStart = clock();  // Get start time in ms -- not the most accurate, but good enough for our purposes.

    CfPoint cfRange     = { fRange, fRange*cfWinSize.y/cfWinSize.x };   // Range based on X-axis

    CfPoint cfD         = cfRange/cfWinSize;                            // Unit Increment for each pixel
    CfPoint cfStart     = cfCenter - cfD*cfWinSize/2;                   // Upper-left X,Y position to start
 
    // Create SageBox and SageBox Window

    auto cBitmap = cWin->CreateBitmap(szWinSize.cx);                    // Create bitmap of (Width,1) --> Returns a CSageBitmap object
                                                                        // Ideally, this would be created in the constructor and not here
                                                                        // so we don't do it every time. It's left here to show the process.
    for (int i=0;i<szWinSize.cy;i++)
    {
        double fy = (double) i*cfD.y + cfStart.y;
        for (int j=0;j<szWinSize.cx;j++)
        {
            double fx = (double) j*cfD.x + cfStart.x;

            int iIter = 0;

            // For Julia Set, set z = { fx, fy } and c to a static value, such as (.285, 0) or (-4.,.6) (and set cfCenter to (0,0);

            CComplex c  = { fx, fy };    
            CComplex z  = c;

            if (bJuliaSet) c = cfJulia; // The only change needed to move it to a Julia Set vs. Mandelbrot. 

            while (z.absSq() < 65536 && iIter++ < iMaxIter-1) z = z.sq() + c; 

            RGBColor_t rgbOut{};    // Get RGB(0,0,0) for color when we gone past maximum iterations
                                    // This can be any color, such as { 0,0, 255} for blue  

            // Get color for value is we're < iMaxIter -- otherwise keep black/preset color in rgbOut

            if (iIter < iMaxIter)
            {
                // Generate Smooth color by giving a gradient from one iteration to the next

                double fLog     = log2(log2(z.absSq())/2); 
                double fIter    = (((double) iIter)+1.0 - fLog);
                
                fIter /= ((double) iMaxIter-1); // Normalize to 1 so we can do a pow() to brighten it.
                fIter = max(0,min(1,fIter));    // Make sure it stays within 0-1

                int    iIndex = (int) ((double)(kColorTableSize-1)*pow(fIter,.37)); //pow(fIter,.7));

                rgbOut = rgbColorTable[iIndex];
            }
            cBitmap.SetPixel(j,rgbOut);              // Set the pixel in the one-line bitmap.
        }
        cWin->DisplayBitmap(0,i,cBitmap);           // Print the one-line bitmap to the Y row in the window.
        if (bAbortSignal) break;                    // Madnelbrots can take a long time -- so, abort if we received a signal to do so
    }
    clock_t ctEnd = clock();
    cDevWin->printf("Finished. Time = {g}%d{/} ms\n",ctEnd-ctStart);
}


int CMandelbrot::main()
{
    // Create Sagebox and Main Mandelbrot Window

    cWin = &Sagebox::NewWindow(100,100,szWinSize.cx,szWinSize.cy,"SageBox: Mandelbrot Smooth Color and 3-D Depth",InnerSize() | Resizeable());   // Create Mandelbrot window
                                                                                                // Resizeable() allows the user to resize it with the mouse/maximize
    // Set the canvas size so we can resize the window with the mouse.
    // Otherwise, the window cannot be resized -- resizing can be turned off by adding "false" to
    // SetCanvasSize() call. 

    cWin->SetCanvasSize(cWin->GetDesktopSize());       // Get the entir desktop size so the maximize button can be used. 

    // Set a message in the DevWindow.  The first usage of the Dev Window opens it.  In this case, just a text message to 
    // point out that the mousewheel and clicking on the screen can be used.
    //
    // note: {ly} = "lightyellow", to set the foreground/text color
    //       TextColor("lightgray") (also bgColor("lightgray") is an alternative method, especially
    //       when using rgbColors (i.e. {128,200,100} or a variable, i.e. rgbMyColor) that are not
    //       translatable from text in the "{<colorname>}" format.

    cWin->DevText("{ly}Use the MouseWheel to zoom in/out. Click on screen to center.");
    cWin->DevText("Resize the window with the mouse to change display size.",TextColor(SageColor::LightGray));   // Use light gray to subdue it a little.

    // -------------------------------
    // Create and Set Control Defaults
    // -------------------------------
    //
    // In a larger program, Creating, Initializing, and Using the controls would be split into three separate parts,
    // i.e. CreateControls(), SetControls(), and EventLoop()
    //
    // However, with this small program in its development stage, we can just set them here and the move them out 
    // when we're ready and as the program grows and reaches a refactoring point (This program is pretty much at this point, but
    // not yet).
    //
    // Whem the time comes to split this function into separate functions, we can select the ones we want to keep and get rid of the
    // ones that aren't needed, putting the rest in the class structure.
    //
    // Remember that all of these controls are "development controls" in the Quick Dev Window.
    // The next version of this program will remove the Dev Window and put the controls into a proper
    // window, with a sub-window for the Mandelbrot. This would be the time to split the functionality and
    // define the controls in the class instead of here as development controls.

    auto& cJulia        = cWin->DevCheckbox("Draw Julia Set"); 

    // With EditBoxes, Default() is used, which sets the text to the value (string, float, or integer).
    // Also, SetText() can be used instead of Default() after the edit box is created. 
    // See how cJuliaImag.SetText() is used instead of Default in the code below.
    
    auto& cEditBox      = cWin->DevEditBox("Iterations",opt::MinValue(50) | Default(iMaxIter));
    
    // The "ArrowBox" style adds up/down arrows to the right of the Edit Box. These send
    // MouseWheel messages to the Edit Box.  Paired with the SetMouseWheel() calls below, allows the arrows and mousewheel
    // to be used to increment and decrement the values.   
    //
    // The Up/Down arrows may be held down constantly increase/decrease the value. 

     auto& cJuliaReal    = cWin->DevEditBox("Julia Real",Style("Arrowbox") | Default(cfJulia.fR));
     auto& cJuliaImag    = cWin->DevEditBox("Julia Imag",Style("Arrowbox")); // Don't set default here to show how it is done with SetText()
      
    
     // Set an abort signal for the Mandlebrot, since they can take a long time. 
     // In this case, it sets the WindowClose signal, so if the Window Close button is pressed,
     // or something else causes the Window Close status to become active, bAbortSignal will be set to true. 
     //
     // This can be used internally in the DrawMandelbrot() function to abort without knowing about
     // the GUI or control internals about how to abort
     //
     // This can also be extended to abort on any control movement that would require a redraw, speeding up the program.

     bool bAbortSignal{};

     cWin->SetSignal(SignalEvents::WindowClose,bAbortSignal); 

    cDevWin = &cWin->DevWindow("Status Window",10);     // Open a basic Window int the Quick Dev Window, for output. 

    cJuliaImag.SetText(cfJulia.fI);                     // This is another way to set displayed value or string in the Edit Box at any time.

    // SetMouseWheel() tells the edit box how to react to the mousewheel, setting the increase value and decrease value. 
    // Minimum and maximums may also be set.  Hover the mouse over the function for more information.

    cJuliaReal.SetMouseWheel(.001,-.001);       // Set MouseWheel (and ArrowBox) Julia Real inc/dec to .001
    cJuliaImag.SetMouseWheel(.001,-.001);       // Set MouseWheel (and ArrowBox) Julia Imaginary inc/dec to .001
    cEditBox.SetMouseWheel(100,-100);           // Set MouseWheel iteration inc/dec to 100 - it already has a minimum of 50.

    CreateColorTable();
    DrawMandelbrot(bAbortSignal);

    // Main Event Loop

    while (cWin->GetEvent())
    {
        bool bRedraw = false;
        int iMouseWheel;
        POINT pMouse;

        // Get the event status of edutboxes and checkboxes, setting bRedraw if they have been set. 
        // The values are filled since they are included as parameters, but can be filled by retriving the 
        // values with a function call, such as cJulia.Checked() or cJuliaReal.GetText(), etc.
        //
        // This method provides a nice shortcut to get values without having to conditionally check and set values.

        bRedraw |= cJulia.Pressed(bJuliaSet);
        bRedraw |= cJuliaReal.ReturnPressed(cfJulia.fR);
        bRedraw |= cJuliaImag.ReturnPressed(cfJulia.fI);
        bRedraw |= cEditBox.ReturnPressed(iMaxIter);

        // Check if the mousewheel in the Mandelbrot window was moved.  If so, increase/decrease the zoom factor
        // based on the direction.  iMouseWheel comes back as a positive or integer value, but may be 1,2, 3 or so.

        if (cWin->MouseWheelMoved(iMouseWheel))
        {
            fRange *= iMouseWheel > 0 ? .65 : 1/.65;
            bRedraw = true;
        }

        if (cWin->MouseClicked(pMouse))
        { 
            CfPoint cfPoint        = pMouse;
            CfPoint cfRange     = { fRange, fRange*cfWinSize.y/cfWinSize.x } ;

            cfCenter = cfCenter + cfRange*(cfPoint-cfWinSize/2)/cfWinSize;
            bRedraw = true;
        }

        // If the window was resized (the user changed the size and then let go of the mouse), redraw it with the new size.

        if (cWin->WindowResized(szWinSize))
        {
            cfWinSize = szWinSize;
            bRedraw = true;
        }
        if (bRedraw) DrawMandelbrot(bAbortSignal);      // Redraw if we had an event that needs to update.

    }

    return 0; 
}
int main()
{
    CMandelbrot cMandelbrot;
    return cMandelbrot.main();
}