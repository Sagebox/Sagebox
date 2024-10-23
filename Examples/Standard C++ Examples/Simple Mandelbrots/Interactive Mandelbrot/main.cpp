
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
//      This is a simple mandelbrot with no color gradation, so the colors change abruptly vs. the Smooth Color version that
//      has a nice color gradient throughout. 
// 
//      Either type can create some interesting mandelbrot displays and colors.
// 
//  2. Smooth Color, Julia Set, and Dev Window
//
//      This is a small change from the previous version, adding smooth coloring and the Julia
//      set to the output.   
//
//      The Dev Window is also used to show the ease-of-use of the Dev Controls without affecting your main code or window.
//
//      This version also adds some more power by assing z.sq() instead of z*z for a little faster
//      Mandelbrot calculation.  FastSetPixel() is also used in CSageBitmap to set the pixel much faster.
//
//  3. MandelBrot Interactive (zoom in/out, resize window, set Julia Set and more) 
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

using namespace Sage::kw;      // Sagebox keyword options, so things like Range() can be used rather than kw::Range(), etc.

// Class Declaration
//
// (this would normally be in its own file, but is included in the main
//  source file for context)
//
class CMandelbrot
{
    static constexpr int kColorTableSize = 16384;   // Color Table Size (must be divisible by 16)

    CPoint      winSize     = { 1200, 800 };  // Initial Window Size
    CfPoint     winSizef    = winSize;        // Get a floating-point version of window size
    CfPoint     center      = { -.6, 0 };     // Initial Mandelbrot Center
    CComplex    pJulia      = { -.4, .6 };
    double      range       = 3.7;            // Initial Range (i.e. "zoom" factor)
    int         maxIter     = 100;            // Max Mandelbrot Iterations
    bool        juliaSet    = false;
    
    int         colorOffset     = 8300;             // Color table index offset for canned color table
    int         colorStart      = 0;                // Start of color table (i.e. offset into color table for display)
    int         colorWidth      = kColorTableSize;  // Width of color table indexing, which will repeat over and over, depending on width

    RgbColor rgbColorTableRaw[kColorTableSize];     // Color Table derieved from rgbColors
    RgbColor rgbColorTable[kColorTableSize+1];      // Color Table derieved from rgbColors, implementing colorWidth


    CWindow * win       = nullptr;      // Mandelbrot Window
    CWindow * devWin    = nullptr;      // DevWindow (here so it can be used in multiple functions)
    std::vector<int> mandelData;        // Mandelbrot Data
    CBitmap bitmap;                     // Mandelbrot output bitmap
    void CreateColorTable();
    void DisplayMandelbrot();
    void CreateMandelbrot(bool & abortSignal);
public:
    int main();
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
    // Structure for a color palette entry
    struct ColorData
    {
        RgbColor rgbColor;  // color value
        int start;          // Where this color starts in the color table (based on max color table entries)
    };

    // A color table with 5 entries, equally spaced apart. 

    std::array<ColorData,5> colors = 
    {{
        { RgbColor{ 255,255,255 },0 *kColorTableSize/5 },
        { RgbColor{ 255,204,0   },1 *kColorTableSize/5 },
        { RgbColor{ 135,31,19   },2 *kColorTableSize/5 },
        { RgbColor{ 0,0,153     },3 *kColorTableSize/5 },
        { RgbColor{ 0,102,255   },4 *kColorTableSize/5 },
    }};

    // Create base gradient, then gradient based on starting position and color width

    int iIndex = 0;
    int iSize = (int) colors.size();

    for (int i=0;i<iSize;i++)
    {
        auto rgbColor1 = colors[i].rgbColor;
        auto rgbColor2 = i == iSize-1 ? colors[0].rgbColor : colors[i+1].rgbColor;

        int iWidth = i < iSize - 1 ? colors[i+1].start-colors[i].start : kColorTableSize-colors[i].start;

        auto fRed   = (double) rgbColor1.iRed;
        auto fGreen = (double) rgbColor1.iGreen;
        auto fBlue  = (double) rgbColor1.iBlue; 

        double fAddR = ((double) rgbColor2.iRed-fRed)/(double) iWidth;
        double fAddG = ((double) rgbColor2.iGreen-fGreen)/(double) iWidth;
        double fAddB = ((double) rgbColor2.iBlue-fBlue)/(double) iWidth;

        int x = colors[i].start;

        for (int j=x;j<x+iWidth+1;j++)
        {
            if (j < kColorTableSize)
                rgbColorTableRaw[j] = Rgb((int) fRed,(int) fGreen,(int) fBlue);

            fRed    += fAddR;
            fGreen  += fAddG;
            fBlue   += fAddB;
        }
    }

    // Create actual gradient based on start and size. 

    iIndex = colorStart + colorOffset;
        
    double fAdd = (double) kColorTableSize/(double) colorWidth;
    double fPlace = (double) iIndex;
    for (int i=0;i<kColorTableSize;i++)
    {
        if (fPlace >= kColorTableSize) fPlace -= (double) kColorTableSize;
        int iNewIndex = (int) fPlace;

        fPlace += fAdd;
        rgbColorTable[i] = rgbColorTableRaw[iNewIndex];
    }
}

void CMandelbrot::DisplayMandelbrot()
{
    auto input  = &mandelData[0];   // A little faster than indexing through a vector
    int iWidth  = bitmap.Width();
    int iHeight = bitmap.Height();

    for (int i=0;i<iHeight;i++)
    {
        for (int j=0;j<iWidth;j++)
            bitmap.SetPixel(j,i,rgbColorTable[*input++]);
    }
    win->DisplayBitmapR(bitmap);    // Put out the bitmap to the window
}

void CMandelbrot::CreateMandelbrot(bool & abortSignal)
{   
    static int iCount;  // Redraw Counter, just for reference, assuming one thread (for now)

    devWin->printf("Drawing Mandelbrot ({lightyellow}Julia Set = %s) {lb}(#%d) {/} {/}\n",juliaSet ? "true" : "false",++iCount);
    devWin->printf("Center = {cyan}(%f,%f){/} -- Range = {cyan}%.14lf{/}\n",center.x,center.y,range);
    

    CfPoint pRange     = { range, range*winSizef.y/winSizef.x };   // Range based on X-axis

    CfPoint fD         = pRange/winSizef;                            // Unit Increment for each pixel
    CfPoint pStart     = center - fD*winSizef/2;                   // Upper-left X,Y position to start
 
    // Adjust center for Julia Set 
    
    if (juliaSet) pStart += decltype(pStart) { .6, 0 };        // Julia starts at (0,0), but we come in with Mandelbrot Center at (.6,0)

    // If the window size has changed (or memory has not yet been initialized), allocat
    // memory for the Mandelbrot Data as well as the bitmap. 

    if (mandelData.size() != (size_t) (winSize.x*winSize.y))
    {
        mandelData.resize(winSize.x*winSize.y);
        bitmap = win->CreateBitmap(winSize.x,winSize.y); 
    }

    SageTimer timer;     // Start a measurement timer

    // Use parallel processing via omp

#pragma omp parallel for
    for (int i=0;i<winSize.y;i++)
    {
        double y = (double) i*fD.y + pStart.y;
        auto output = &mandelData[i*winSize.x]; // using a point is a little faster than indexing through a vector each time.
        for (int j=0;j<winSize.x;j++)
        {

            double x = (double) j*fD.x + pStart.x;

            int iter = 0;

            // For Julia Set, set z = { x, y } and c to a static value, such as (.285, 0) or (-4.,.6) (and set center to (0,0);

            CComplex c  = { x, y };    
            CComplex z  = c;

            if (juliaSet) c = pJulia; // The only change needed to move it to a Julia Set vs. Mandelbrot. 

            while (z.absSq() < 65536 && iter++ < maxIter-1) z = z.sq() + c; 

            // Get color for value is we're < maxIter -- otherwise keep black/preset color in rgbOut

            int colorIndex = kColorTableSize;
            if (iter < maxIter)
            {
                // Generate Smooth color by giving a gradient from one iteration to the next

                double log     = log2(log2(z.absSq())/2); 
                double fIter    = (((double) iter)+1.0 - log);
                
                fIter /= ((double) maxIter-1); // Normalize to 1 so we can do a pow() to brighten it.
                fIter = max(0,min(1,fIter));    // Make sure it stays within 0-1

                colorIndex = (int) ((double)(kColorTableSize-1)*pow(fIter,.37)); //fGamma /* .37+fGamma/10.0 */)); //pow(fIter,.7));

            }

            *output++ = colorIndex;   // Set mandelbrot data; to be converted to color later

        }
    }
    auto elapsedMs = (int) timer.ElapsedMs();
    clock_t ctEnd = clock();
    devWin->printf("Finished. Time = {g}%d{/} ms\n",elapsedMs);
}


int CMandelbrot::main()
{

    rgbColorTable[kColorTableSize] = PanColor::Black;

    // Create Sagebox and Main Mandelbrot Window
    // kw::Resizeable() allows the user to resize it with the mouse/maximize
    // kw::SetSize() sets a specific size, rather than the default (usually 1200x800) 

    win = &Sagebox::NewWindow("SageBox: Mandelbrot Smooth Color and 3-D Depth",Resizeable() + SetSize(winSize));

    // Set the canvas size so we can resize the window with the mouse.
    // Otherwise, the window cannot be resized -- resizing can be turned off by adding "false" to
    // SetCanvasSize() call. 

    win->SetCanvasSize(win->GetDesktopSize());       // Get the entir desktop size so the maximize button can be used. 

    // Set a message in the DevWindow.  The first usage of the Dev Window opens it.  In this case, just a text message to 
    // point out that the mousewheel and clicking on the screen can be used.
    //
    // note: {ly} = "lightyellow", to set the foreground/text color
    //       TextColor("lightgray") (also bgColor("lightgray") is an alternative method, especially
    //       when using rgbColors (i.e. {128,200,100} or a variable, i.e. rgbMyColor) that are not
    //       translatable from text in the "{<colorname>}" format.

    Sagebox::DevText("{y}Use the MouseWheel to zoom in/out. Click on screen to center.");
    Sagebox::DevText("Resize the window with the mouse to change display size.",TextColor(SageColor::LightGray));   // Use light gray to subdue it a little.

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

    auto& checkboxJulia        = Sagebox::DevCheckbox("Draw Julia Set"); 

    // With EditBoxes, Default() is used, which sets the text to the value (string, float, or integer).
    // Also, SetText() can be used instead of Default() after the edit box is created. 
    // See how checkboxJuliaImag.SetText() is used instead of Default in the code below.
    
    auto& inputBox      = Sagebox::DevInputBox("Iterations",kw::MinValue(25) | Default(maxIter));
    
    auto& sliderColorWidth = Sagebox::DevSlider("Color Width",Range(0,16384) + Default(16384)); 
    auto& sliderColorStart = Sagebox::DevSlider("Color Start",Range(0,16384)); 


    // The "ArrowBox" style adds up/down arrows to the right of the Edit Box. These send
    // MouseWheel messages to the Edit Box.  Paired with the SetMouseWheel() calls below, allows the arrows and mousewheel
    // to be used to increment and decrement the values.   
    //
    // The Up/Down arrows may be held down constantly increase/decrease the value. 

     auto& checkboxJuliaReal    = Sagebox::DevInputBox("Julia Real",kw::AddArrowBox() | Default(pJulia.real));
     auto& checkboxJuliaImag    = Sagebox::DevInputBox("Julia Imag",kw::AddArrowBox()); // Don't set default here to show how it is done with SetText()
      
    auto radioOOBColor = Sagebox::DevRadioButtons("Black\nWhite",Title("Out of Bounds Mandelbrot Color") + Horz()); 

    // Set an abort signal for the Mandlebrot, since they can take a long time. 
    // In this case, it sets the WindowClose signal, so if the Window Close button is pressed,
    // or something else causes the Window Close status to become active, abortSignal will be set to true. 
    //
    // This can be used internally in the DrawMandelbrot() function to abort without knowing about
    // the GUI or control internals about how to abort
    //
    // This can also be extended to abort on any control movement that would require a redraw, speeding up the program.

    bool abortSignal{};

    win->SetSignal(SignalEvents::WindowClose,abortSignal); 

    devWin = &Sagebox::DevWindow("Status Window",10);     // Open a basic Window int the Quick Dev Window, for output. 

    checkboxJuliaImag.SetText(pJulia.imag);                     // This is another way to set displayed value or string in the Edit Box at any time.

    // SetMouseWheel() tells the edit box how to react to the mousewheel, setting the increase value and decrease value. 
    // Minimum and maximums may also be set.  Hover the mouse over the function for more information.

    checkboxJuliaReal.SetMouseWheel(.001,-.001);       // Set MouseWheel (and ArrowBox) Julia Real inc/dec to .001
    checkboxJuliaImag.SetMouseWheel(.001,-.001);       // Set MouseWheel (and ArrowBox) Julia Imaginary inc/dec to .001
    inputBox.SetMouseWheel(25,-25);           // Set MouseWheel iteration inc/dec to 100 - it already has a minimum of 50.

    // Set the Dev Window just adjacent to the top of the main window -- i.e. x = 25 + left + width, y = 0 + top + 0

    Sagebox::DevSetLocation(CPoint(25,0) + win->GetWinLocation() + CPoint(win->GetWindowWidth(),0));        
    Sagebox::DevWindowTopmost();    // Make the Dev Window 'topmost', meaning it will never get obscured by the main window (it will always be on top)
    
 
    bool redraw = true;
    bool calcColorTable = true;

    // Main Event Loop
    //
    // Note: We don't need to create or display the initial mandelbrot before entering the event loop.  
    // The first GetEvent() always falls through so that the inner-function can draw or setup initial values.

    while (Sagebox::GetEvent())
    {
        // Get the event status of edutboxes and checkboxes, setting redraw if they have been set. 
        // The values are filled since they are included as parameters, but can be filled by retriving the 
        // values with a function call, such as checkboxJulia.Checked() or checkboxJuliaReal.GetText(), etc.
        //
        // This method provides a nice shortcut to get values without having to conditionally check and set values.

        redraw |= checkboxJulia.Pressed(juliaSet);                  // Redraw of Julia/Mandelbrot type changed
        redraw |= checkboxJuliaReal.ReturnPressed(pJulia.real);     // Redraw of Jula Real or Imaginary values changes
        redraw |= checkboxJuliaImag.ReturnPressed(pJulia.imag);
        redraw |= inputBox.ReturnPressed(maxIter);                  // Redraw of the maximum iteration value changed

        // Switch Mandelbrot out-of-bounds color (interior, etc.) if one of the radio buttons was pressed
        
        if (radioOOBColor.Pressed())
        {
            rgbColorTable[kColorTableSize] = radioOOBColor.GetCheckedButton() ? PanColor::White : PanColor::Black;
            calcColorTable = true;
        }
        
        // Color Width Slider handling -- set the color width based on the value, with 1 as a minimum. 
        
        if (auto pos = sliderColorWidth.opMoved())
        {
            colorWidth = max(1,*pos); 
            calcColorTable = true; 
        }

        // Set the color offset, if the slider was moved

        if (auto pos = sliderColorStart.opMoved())
        {
            colorStart = *pos; 
            calcColorTable = true;
        }

        // Check if the mousewheel in the Mandelbrot window was moved.  If so, increase/decrease the zoom factor
        // based on the direction.  mouseWheel comes back as a positive or integer value, but may be 1,2, 3 or so.
        
        if (auto mouseWheel = win->opMouseWheelMoved())
        {
            range *= *mouseWheel > 0 ? .65 : 1/.65;
            redraw = true;
        }

        // Center the mandelbrot based on where the mouse was clicked. 

        if (auto mousePos = win->opMouseClicked())
        { 
            CfPoint pRange     = { range, range*winSizef.y/winSizef.x } ;

            center += pRange*((CfPoint) *mousePos-winSizef/2)/winSizef;
            redraw = true;
        }

        // If the window was resized (the user changed the size and then let go of the mouse), redraw it with the new size.

        if (auto size = win->opWindowResized())
        {
            winSizef = winSize = *size;     // Set integer and float window sizes
            redraw = true;
        }

        // Calculate and/or set create the color table (and then redraw the mandelbrot) if anything significant changed.

        if (redraw) CreateMandelbrot(abortSignal);      
        if (calcColorTable) CreateColorTable();
        if (redraw || calcColorTable) DisplayMandelbrot();

        redraw = false;            // Reset redraw and recalc color table
        calcColorTable = false;


    }

    return 0; 
}


int main()
{
    CMandelbrot cMandelbrot;
    return cMandelbrot.main();
}