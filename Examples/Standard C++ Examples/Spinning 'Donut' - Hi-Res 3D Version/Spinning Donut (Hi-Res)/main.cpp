
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

// **************************
// 3D Ascii Donut Source Code
// **************************
//
// This example is an extension of the Ascii Donut Code.
//
// Where the previous version was to show how original source code from a program can be taken and put into 
// SageBox with little or no change, this version shows how we can then adapt code for a much broader usage
// by writing code around the original source.
//
// This is a high-res 3D version that still largely leaves the original source untouched -- the only changes are to 
// use tables for some equations for speed purposes.
//
// This program is also a good example of using a derived CWindow type of project, making the main window part of the same namespace as the program.  See documentation below.
//
// -----------------------
// Windows Debug Optimized
// -----------------------
//
// The default project setting for this program is "Windows Debug Optimized". 
// This is because it is much, much slower without optimization, and there is no reason for it to be console mode program.
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
// 1. Dervived CWindow for main() (aka SandBox-type project)
//
//        This program uses a class derived from CWindow (AsciiDonut), which bring the CWindow functions into the same namespace
//        as the window attached to CWindow.
//
//        This makes using various Windows functions more inline with the code, vs. using MyWindow->Function().
//        This can be very useful when overriding various message, where otherwise a message handler would be needed, or the
//        window would be derived just for the purpose of handling messages.
//
//        In this case, the main program is in the same class as the window, so the window functions are called directly.        
//
//        While the SandBox-style project is not recommended for large projects, it is great for quick prototypying
//        and trials because there is no work to do to get up and running right away.
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

#include "CAsciiDonut.h"

using namespace Sage::kw;       // Sagebox Keywords

// FillColorTable() -- Fill the color table for the color we want the donut. 
//
void AsciiDonut::FillColorTable(Sage::HSLColor_t stHSL)
{
    pRGB32 sTempOut = (pRGB32) m_ucColorTable + 1;    // Get past value used for BG Color

    for (int i=0;i<256;i++)
            CSageTools::HSLtoRGB(stHSL.fH,1.0,(double) i/255,* (pRGB24) sTempOut++);
}

// FillBgTable() -- Get the bitmap of the window backdrop.
// Then reverse it (from top to bottom, i.e. make it upside-down) because bitmaps
// start as upside-down, so this makes it right-side up and easier to work with.
//
// This gets a 1-pixel width bitmap the height of the window.
// We only need to know what the color of the line is -- we're using
// a vertical gradient that has the same color for every pixel in each
// row, so we only need the first one.
//
void AsciiDonut::FillBgTable()
{
    cBitmapBg = m_cWin->GetWindowBitmap({0,0},{1,m_szWindowSize.y});
    (*cBitmapBg).ReverseBitmap();
}
//CalcBitmap() -- Calculate the bitmap based on the light intensity in the incoming map.
//                A value of 0 is the background, which we has in cBitmapBg. 
//
void AsciiDonut::CalcBitmap(int * pMap)
{
    unsigned char * sOut            = m_cOut;
    unsigned char * ucColorTable    = m_ucColorTable;
    pRGB32 ucBgTable                = (pRGB32) (**cBitmapBg + m_pOffset.y * (*cBitmapBg).iWidthBytes);

    // Only go through the area was need (Inset Size), and either add the value or the backdrop
    // pixel value, so it will blend in with the background

    for (int j=0;j<m_pInsetSize.y;j++)
    {
        * (pRGB24) ucColorTable = * (pRGB24) ucBgTable++;            // Get the line color of the background

        // Put out the bitmap values in the table for each intensity. 
        // note: pRGB24 is an easy way to put out all three RGB values to memory at one time.

        for (int i=0;i<m_pInsetSize.x;i++,sOut += 3)
            * (pRGB24) sOut = * (pRGB24) (ucColorTable + ((*pMap++ + 1) << 2));

        sOut += m_cOut.GetOverhang();    // Go to the next line in the output.
                                        // Bitmaps have unknown line-length, and overhang
                                        // is the value that needs to be added at the end of each line.
    }
}

// CalcSinCosTable() -- Calculate the Sines and Cosines used in the main function.
// This makes things much faster since we just re-use these values over and over.
//
// Drawing them from memory makes things much faster than calling 
// processor-expensive items like sin() and cos()
//
void AsciiDonut::CalcSinCosTable()
{
    int iIndex = 0,iIndex2 = 0;
    for (double i = 0; 6.28 > i; i += 0.01/4.0/1.2)
    {
        m_fSinIMem[iIndex]        = sin(i);
        m_fCosIMem[iIndex++]    = cos(i);
    }
    for (double j = 0; 6.28 > j; j += 0.035/4.0/1.2)
    {
        m_fSinJMem[iIndex2]        = sin(j);
        m_fCosJMem[iIndex2++]    = cos(j);
    }
}

// Main() -- the actual main() since we've derived from a CWindow (called from main())
//
int AsciiDonut::Main() 
{
    m_cWin = &Sagebox::NewWindow("SageBox -- Ascii Donut by Andy Sloan in High Res",kw::NoAutoUpdate()); 

    auto& cWin = *m_cWin;

    cWin.Cls("black,blue");

    m_szWindowSize  = { 800, 550 };                             // Set the Window size to something reasonable
    m_pInsetSize    = { 640,352 };                              // Get a 80x24 area, akin to a terminal (just to center the output)

    m_pOffset       = (m_szWindowSize-m_pInsetSize)/2;          // Get the start X,Y for centering the Ascii output.
    m_pOffset.y     -= 30;                                      // Subract a little so isn't so cenered in the Y plan (looks nicer)

    cWin.SetWindowSize(m_szWindowSize,true);                         // Set the Window size we decided on (i.e. the Windows window)

    // Put a text Widget with some text telling the user to click the window to exit
    // The user can also close the window
    
    auto &StopButton = cWin.NewButton(0,m_szWindowSize.y-30,"   Press Button or Close Window to Stop   ",Center());
    CColorWheelWidget cWheel(cWin,0,15,Transparent() | kw::JustTopRight() | PadX(-20));        // note: "kw::" not needed, but using it will display a list of options
                                                                                                // note: Transparent() option is deprecated and will be default in upcoming release.
    SIZE szSize     = cWheel.GetWindowSize();
    POINT szPoint   = cWheel.GetWindowPos();

    cWin.TextWidget(szPoint.x,szPoint.y+szSize.cy,szSize.cx,0,"Set Torus Color",Font("Arial,14,bold") | fgColor(SageColor::NearWhite) | TextCenterX());

    FillColorTable({ 0,1.0,1.0 });        // HSL Color: H = 0, S = 1, L = 1
    FillBgTable();

    m_cOut = CreateBitmap(m_pInsetSize);

    // Get memory as in the original version, but for roughly 144 times the resolution
    // i.e. a 12x12 set of pixels for each original character (or 18x8, as tthe case may be)

    Mem<float>  zMem(1760*144);
    Mem<int>    bMem(1760*144);

    CalcSinCosTable();        // Calculate sin & cos tables for speed.

    // ----------------------------------------------------------------------
    // Preserve the original code, showing we made small changes for speed
    // and bitmap output.  Otherwise, it remains the same.
    //
    // There are various ways to make this faster, but part of the
    // demonstration is to show how we can adapt existing code farily easily
    // -- and then refactor it later (which isn't done here except to speed
    //    it up.
    // ----------------------------------------------------------------------

    // As before use the "for(;;)" to preserve the original code. 

    double A = 0, B = 0;
    for (;;) 
    {
        double m = cos(B), e = sin(A), n = sin(B), g = cos(A);
        double * fSinJ = m_fSinJMem, * fCosJ = m_fCosJMem;

        auto z = &*zMem;    // Get address of z & b buffers.  We don't want to use the bMem and zMem originals
        auto b = &*bMem;    // because these are classes, and it would de-reference on each call slowing the program down.
                            //
                            // note: "auto z = &*zMem" is the equivalent of stating "float * z = zMem", and 
                            //         "auto b = &*bMem" as "int * b = bMem", but its nice to write it this way so that if we 
                            //         ever change types this code will automatically inherit those changes (for example, b should
                            //         really be "unsigned char *" and not "int *", so if it changes this won't have to, nor the memsets below).

        // Clear the memory we used -- we cold probably size down the bitmap to 8 bits for some speed here. 
    
        memset(b, 0xFF, bMem.GetMemSize());            // We can also use bMem.ClearMem(0xFF) here, but I kept it as close to the original
        memset(z, 0, zMem.GetMemSize());            // as possible

        for (double j = 0; 6.28 > j; j += 0.035/4.0/1.2)
        {
            double * fSinI =  m_fSinIMem;
            double * fCosI =  m_fCosIMem;
            double d = *fCosJ++;
            double f = *fSinJ++;
            double h = d + 2;

            for (double i = 0; 6.28 > i; i += 0.01/4.0/1.2) 
            {
                double    c = *fSinI++,
                        l = *fCosI++,
                        D = 1 / (c * h * e + f * g + 5), 
                        t = c * h * g - f * e;
                int x = 8*40 + (int) (1.2*.8*8.0*30.0 * D * (l * h * m - t * n)),
                    y = 15*12 + (int) (1.2*1.5*8.0*15.0 * D * (l * h * n + t * m)), 
                    o = x + 80*8 * y;
                double  N = .75*((((f * e - c * d * g) * m - c * d * e - f * g - l * d * n)));
                int iN = (int) (N*N*N*255.0);
                if (22*20 > y && y > 0 && x > 0 && 80*8 > x && D > z[o] ) 
                {
                    z[o] = (float) D;
                    b[o] = max(0,min(255,iN));
                }
            }
        }
        
        CalcBitmap(b);                                    // Create the output bitmap based on intensity value. 
        cWin.DisplayBitmapR(m_pOffset.x,m_pOffset.y,m_cOut); // Display bitmap upside-down, becuase it is upside-down in memory, so it is now right-side-up

        A += 0.04;
        B += 0.02;

        cWin.Update();      // Update the screen with the new bitmap
        Sleep(0);           // Give the system some time so we don't make it sluggish

        if (cWheel.ColorChanged()) FillColorTable(cWheel.GetHslValue());        // If the color wheel was moved, then change the colors for the next bitmap
        if (cWin.WindowClosing() || StopButton.Pressed()) break;                        // Break out if someone closed the window or pressed the stop button
    }
    return 0;
}

// main() entry point.  Note: In a Windows Project (default), this is called from WinMain() automatically, and 
// WinMain() does not need to be implemented.
//
int main()
{
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

    return AsciiDonut::Main();
}


