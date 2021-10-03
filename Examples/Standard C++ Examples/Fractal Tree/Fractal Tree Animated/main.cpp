
// -------------------------------
// Fractal Tree - Animated Version
// -------------------------------
//
// *** This program is written to give an example and lessons in how to use Sagebox.  Therefore, it is heavily commented. ***
// *** This program must be set to write out to an AVI -- Search for bWriteFrames to turn this on.
// 
// Note: Where the two previous project versions (Basic Tree and Fancy Fractal Tree) are good examples of using simple Sagebox 
// controls, this program is an example of creating a small-but-capable program using many Sagebox features.
//
// Individually, all of the Sagebox features used in this example are pretty simple. For the first-time viewer, it may
// seem complicated because there are 10+ different Sagebox elements being used at the same time.  Broken down individually,
// these Sagebox components tend to be relatively simple.   
//
// However, there are some powerful functions used, such as the TextWidget() -- which is easy enough once the 
// basic manner of specifying options with Sagebox is understood.
//
// note: Also note that you can hold the mouse over any Sagebox function or option (in Microsoft Visual Studio) and this
// will popup documentation on the function or option.
//
// ------------------------------------------------------------------------------------------------------------
// The main() code which performs all program duties (except displaying the Fractal Tree) is only about 40 lines, 
// but may look larger with the numerous comments explaining the code (try removing the comments) to see how small
// the code is)
//
// This main() function is only 40 lines because of the powerful set of Sagebox tools.  See below for a description
// of some of the Sagebox elements in this program.
// ------------------------------------------------------------------------------------------------------------
//
// Some Sagebox components used in this program are as follows:
//
//    1. Point3D_t        -- A class used for Point & Vector math.  It is used as a 2-D vector in this program
//    2. CString()        -- A String class similar std::string() but with much more power, felxibilty, and speed.
//    3. RGbColor         -- A simple RGB class comprised of Red,Green, and Blue, but with various functions and expression overloads.
//    4. Window Options   -- In the main AutoWindow call, many options are used.  These are easy to use once the nomenclature is understood. 
//    5. Text Widget      -- Text Widgets are easy to use, but also very expansive.  This program uses some of the more advanced options.
//    6. Dev Window       -- The Dev Window is a very quick and easy way to get controls going for programs where you don't need a formal 
//                           User Interface.  This program uses the Dev Window in some powerful ways
//    7. Inline Text Controls     -- The "{g}" and "{EndFont}" sort of text you see are controls you can give inside of the text
//                                   This gives a way to change fonts, set tab stops, and do other things in inline text.
//    8. Fonts            -- This program creates some fonts with AddFont(), names them, and uses them in inline text.
//                           for example, the font "SmallFont" is used to switch to smaller font just after print the Frame Size,
//                           which is in a Text Widget pre-created with an "Arial 25" font
//    9. Update Background    -- The UpdateBg() call below updates the background of the Text Widgets, copying the window
//                               beneath.  Since we're animating, this is always changing, and the background are updated just prior
//                               to writing out the frame.
//   10. Manual Update        -- Most of the time, programs can be written in Sagebox with an automatic window update as you go.
//                               When performing animations or changing graphics, it can be better to turn off the auto-update and
//                               perform it manually.  The NoAutoUpdate() option in AutoWindow() tells Sagebox not to update the window
//                               automatically.  Later, the Update() call updates the window once the Fractal tree has been drawn. 
//   11. Reading & Display Images, etc. -- There are a few more things used here in the Sagebox library that make it easy to use.  For example,
//                                         the main background image is read in and displayed with just 2 lines of code.  If the image is not found
//                                         the display just falls through.  Though this program doesn't check for it (because it doesn't care), it is 
//                                         easy to determine of the read failed by calling a Sagebox function or just checking of the bitmap is empty. 
//
// note: This project is set for Windows Debug, a console-mode program.  This can be a Windows program with 
// a project switch, but was left as a console-mode program because console-mode programs are relatively easy
// to end prematurely (with a Control-C), and is a good development method to use before converting it to Windows.

// Include the main CSagebox.h file.  This includes most typical C++ .h files, as well. 
//
// <functional> is included for the std::function reference, since I couldn't use auto because DrawTree is recursive

#include "CSageBox.h"
#include "CAviFile.h"
#include <functional>

// Note: iMaxDepth is initially set to 11 (for faster speed) and WriteFrames = false (to avoid writing to an AVI file)
//  
// Set iMaxDepth = 14 and bWriteFrames to true to write an AVI, and also specify the AVI path

static constexpr int    iMaxDepth       = 11;       // Change to less than 14 for faster output (10-12 works fine for more real-time)
                                                    // Dont go over 14 without adding to or removing references to rgbColors
                                                     
static constexpr int    iTotalFrames    = 1200;     // We're looking for 1200 frames for about 20 seconds (60fps*20)
static constexpr double fTotalFrames    = (double) iTotalFrames;
static constexpr int    iFps            = 60;       // Frames per-second for the AVI file
static constexpr bool   bWriteFrames    = false;    // Set this to true to write out to an AVI file
    
// Set these paths as appropriate

static constexpr const char * sAviOutputFile    = "c:\\sagebox\\avi\\avitest.avi";
static constexpr const char * sBackgroundImage  = "C:\\SageBox\\git\\Examples\\Standard C++ Examples\\Fractal Tree\\Fractal Tree Animated\\texture-fractal-tree.jpg";

// Set a color table for each depth.  If the max depth limitation moves past 15, extra RGB values need to be added.
//
RgbColor rgbColors[15] = {  { 213,88 ,31  }, { 194,50 ,0   }, { 193,86 ,0   }, 
                            { 164,116,22  }, { 45 ,122,163 }, { 35 ,89 ,242 },
                            { 201,0  ,218 }, { 255,0  ,173 }, { 255,0  ,107 },
                            { 255,21 ,41  }, { 244,52 ,0   }, { 172,76 ,0   },
                            { 81 ,96 ,0   }, { 151,168,0   }, { 248,250,0   },  };

// use std::function for the lamdba.  This is used because DrawTree() is a lambda function.
// Since it's recursive, it has to be declared beforehand, so 'auto' can't be used. 
//
using fDrawTree = std::function<void(Point3D_t &, double, double, double)>;

// FracalTree()) -- Main Tree-drawing function. 
//
// The recursive DrawTree() is used as a lambda so we can refer to the angle, depth, and Window object as if we 
// were in a class.  DrawTree() is easily moved out to its own function if iDepth,_ang, and cWin are either 
// passed or set as globals/class members
//
// iDepth should probably be passed to Drawtree() anyway, because it reduces two code lines in doing so,
// but it also makes a good example of using a lambda vs. extracting DrawTree() out to its own function.
//
void FractalTree(CWindow & cWin,CfPoint szWinSize,double _ang,double line_len,double fMul,bool bShadow = false)
{
    static int iDepth = 0;

    // I used a lambda here so I could avoid passing a lot of things.  Lamdbas 
    // are nice pre-steps to refactoring and branching out into other functions.
    //
    fDrawTree DrawTree = [&](Point3D_t & sp, double line_len, double a, double rg)
    {
	    if (iDepth >= iMaxDepth) return;

        // The Rosetta code declares a vector class in the code.  Sagebox has Point3D_t 
        // as an accessible type that performs the same actions. 

        auto r = Point3D_t{0,-line_len}.RotateZ(a += rg *_ang) + sp;

        // Draw a line in the window. Get an RGB value from a HUE, Saturation, and luminance value
        // CSageTools has various tools.  This also sets a pen size that is a smaller as the depth is greater

        cWin.SetPenSize((15-iDepth)/7);  
        cWin.DrawLine(sp,r,rgbColors[(int)++iDepth]);

	    DrawTree(r, line_len*fMul, a, -1 );
	    DrawTree(r, line_len*fMul, a, 1 );
        iDepth--;
    };

    Point3D_t sp{szWinSize.x/2,szWinSize.y-1 - line_len};

    // DrawLine2() is the same as DrawLine() except that you only need to specify length rather
    // than the actual endpoints, which is often convenience. 

    cWin.SetPenSize(2);     // Draw the larger branches a little bigger, so start with a pen size of 2

    cWin.DrawLine2(sp,{0,(int) line_len},rgbColors[0]);
 
	DrawTree(sp, line_len, 0, -1 );
	DrawTree(sp, line_len, 0, 1 );
}

// Main() -- Animate the Fractal Tree and put it out to an AVI file
//
// This function is fairly simple and is broken down into a few components:
//
// 1. Setting up the DevWindow
//
//    There are a few calls to DevText(), DevWindow(), DevButton(), to set up a dev window.
//    The dev window is used to show the staus and to look for a cancel button press.
//
//    Some fonts are created that are used as controls in text strings to the Dev Window and
//    Dev TextWidgets, to have different font sizes for output. 
//
//     While not necessary, these different font sizes make the output look nicer. 
//
// 2. Drawing The Fractal Tree, Updating Status, and Writing the Frame
//
//    The actual drawing of the Fractal tree is accomplished in one line.  There are a few lines of setup that aren't necessary,
//    but were done to allow experimenting with numbers and clarity about what those numbers mean. 
//
//    The AVI frame is written, and then a line is put out to the Dev Window with information about the current frame.
//
// 3. Closing the AVI and waiting for exit
//
//    When finished, the AVI file is closed (if we we're writing it), and the program waits for the user to close the window.
//    Note that the Cancel Button is never release used.  CButton::SendCloseMessage() is called to tell the button to 
//    send a window-close message when the button is pressed.  Now when CWindow::WaitforClose() is called, it will return
//    when the window is closed by the user or the Cancel Button is pressed.
//
//    This was done just for simplicity -- otherwise, without having called CButton::SendCloseMessage(), we could go into a 
//    GetEvent() loop and look for the button to be pressed. Instead of cWin.WaitforClose(), the line 
//
//              while(GetEvent() && !cCancelButton.Pressed());
//
//    does exactly the same thing.
//
int main( int argc, char* argv[] )
{ 

    // Create a window and CSagebox object together.  This is useful when you only want to create a Window
    // and don't need a cSagebox object.  You can obtain the CSagebox object created if necessary.
    //
    // Title()          - sets the name of the window
    // bgGradient()     - clears the background of the window in a gradient with the colors used.
    // NoAutoUpdate()   - Specifies that the window should not update on its own (wait for an Update() call)
    // NoClose()        - Don't send a window-close message when the user presses the window close. 
    //                    (lets us check it ourselves)
    //
    // The '|' function as << (which can also be used), as in Title("My Title)" << bgGradient("color1","color2"); 
    // the '|' just make the code more readable (for me, anyway)
    //
    // Note the opt::NoClose() -- when opt:: is specified, Microsoft Visual Studio will give you a list of all available options, so
    // you don't have to remember them all.  
    //
    // opt:: is a namespace that is being used, so NoAutoUpdate() is actually opt::NoAutoUpdate(), and I probably used opt::NoClose() because I 
    // wasn't sure of the name I wanted.  Try putting opt:: in the line below for Visual Studio to present you with a list of options you 
    // can send to controls and other functions.
    // 

    auto& cWin = CSagebox::AutoWindow(CSize(650,450+50),Title("Fractal Tree") | bgGradient(SageColor::Black,SageColor::DarkBlue) | NoAutoUpdate() | opt::NoClose()); 

    // Put a Text Widget with the title.  We can just print it out if we want, but the TextWidget is easy
    // to use.  In this case, the returned object isn't saved because we don't need it once we displayed it.
    // With the return object, the TextWidget can be changed.
    //
    // Font()               - sets the font
    // JustBottomCenter()   - sets the text at the bottom in the window, centered. 
    // JustCenterX()        - Centers the text widget in the middle of the X dimension, without changint the specified Y position. 
    // Transparent()        - tells the TextWidget to blend in the background rather than using a solid color.
    // Trnsparent(value)    - Sets the background as transparent, and also tells the Widget to blend the result at
    //                        the value given (for example, Transparent(75) makes the result 75% opaque, and 
    //                        Transparent(25) makes it nearly invisible as 25% opacity (or 75% transparent).
    // AddShadow()          - Adds a shadow to the text in the TextWidget.
    //
    // The first two parameters (X,Y) are zero because JustBottomCenter() sets X and Y itself.
    //
    auto & cText = cWin.TextWidget(0,0,0,30," C++ Fractal Tree ",Font("Arial,33") | JustBottomCenter() | OffsetY(-25) | Transparent(90) | AddShadow());

    // This originally used a larger font.  Now that its smaller, we don't really need the blending value in the Transparent() option; we
    // could now just use a mid-gray.  But I left it in anyway. 

    auto & cText2 = cWin.TextWidget(0,cText.EndY(),"github.com/Sagebox/FractalTree",Font("Arial,15") | JustCenterX() | Transparent(45));


    CAviFile cLocalAvi;         // Instantiate the AVI reader/writer. 
 
    // Create the AVI file.  If it is a bad path or otherwise can't be created all of the AVI functions just call through.
    // The return code can be checked and cLocalAvi.GetStatusMsg() can be called to get a text string explaining the error.


    if (bWriteFrames) cLocalAvi.CreateAviFile(sAviOutputFile,cWin.GetWindowSize().cx,cWin.GetWindowSize().cy,iFps); 

    // Get a generic Text Widget in the Dev Window -- Set it to a large font to display the frame number 

    auto& cDevText = cWin.DevText(Font("Arial,25")); 

    // Put out some static data.  It never changes, so we don't save the returned text widget object reference (i.e. fire-and-forget)

    cWin.DevText(CString() << "Frames Per Second: {y} " << iFps << "{/}\n" 
                           << "Writing output .AVI: {y} " << BoolStringYU(bWriteFrames) << "{/}\n"
                           << "Depth Limit: {g}" << iMaxDepth << "{/} (out of 14 max)");

    auto& cTextWin = cWin.DevWindow("Output");              // Get a basic output window inside the Dev Window
    auto & cButtonCancel = cWin.DevButton("   Cancel   ");  // Get a cancel button -- we never use it directly, but set it to send a Widow-Close
                                                            // message when pressed.

    // Create two fonts to use in the Dev Window 

    cWin.AddFont("Arial,15","SmallFont");       // Font used after Frame ID
    cWin.AddFont("Arial,18","EndFont");         // Font used for "aborted" to show more noticeably.

    // Get the background image.  It's basically the size of the window.  We can include this as data or as a .PGR file, but 
    // for code-demonstration purposes, this was left as a direct load of a jpeg.  If it fails, the call to DisplayBitmap() calls through
    // (cBg.isValid()) can be checked to make sure the bitmap was loaded)

    auto cBg = cWin.ReadImageFile(sBackgroundImage); 

    cWin.SetClsBitmap(cBg);     // When we do it this way, the screen clears even when we fail to load the bitmap.

    CPoint szWinSize = cWin.GetWindowSize();

    int iFramesWritten = 0;

    // Call the Fractal Tree.  GetWindowSize() returns a generic SIZE structure even though FractalTree()
    // receives it as a CPoint object -- CPoint is a ubqiqitous object-oriented POINT replacement (much like MSVC's Cpoint),
    // and translates back-and-forth between SIZE and POINT automatically. 
    //
    for (int j=iTotalFrames-1;j>=0;j-=1)
    {
        double i = (fTotalFrames*pow((double) j/fTotalFrames,2.5));

        cWin.Cls(cBg);        // Display the background bitmap to clear the last output

        // Just some setup for easy changes if we want to experiment

        double fAngle       = (24+(double) (i*450/fTotalFrames)/3);
        double fLineLen     = 130.0f*1.7/2.46;
        double fLineMult    = .758;

        // Draw the Fractal Tree

        FractalTree(cWin,CPoint(0,-50) + cWin.GetWindowSize(),fAngle*3.14159/180,fLineLen,fLineMult); //.4 + (double) i/1000);

        // Put some data out to the DevWindow we created.

        cTextWin.printf("Angle = {g}%g{/}, Line Length = {g}%g{/}, Line Mult = {g}%g{/}\n",fAngle,fLineLen,fLineMult);

        // Update the background of the two text widgets, since they need to blend with the background we just created.

        cText.UpdateBg();
        cText2.UpdateBg();
        cWin.Update();      // Update the main window. 

        // Write the AVI frame (if we're writing)

        if (bWriteFrames) cLocalAvi.WriteFrame(**cWin.GetWindowBitmap(true));

        // Write out to the text widget, changing the font for the latter half. 

        cDevText.Write(CString() << "Frame #{c}" << 1199-j+1 << "{/}{SmallFont} (out of 1200)");

        iFramesWritten++;

        // Check if the cancel button was pressed but use Peek::Yes to keep the event status active
        // we we can check it outside of the loop. 

        if (cButtonCancel.Pressed(Peek::Yes)) break;

    }

    // Display out status (anorted or completed) out to the Output Window in the Dev Window
    // This changes the font to a larger font, and when aborted, sets the line background to red

    cTextWin << (cButtonCancel.Pressed() ? "{EndFont}{lbg=r}Aborted.\n" : "{EndFont}{g}Finished.\n");

    if (bWriteFrames)
    {
        cLocalAvi.CloseFile();

        if (iFramesWritten != iTotalFrames) cTextWin << "{cyan}Process Aborted -- only " << iFramesWritten << " written.\n"; 
        else 
        cTextWin << iFramesWritten << " frames written to Avi file.\n";
    }

    // WaitforClose() simply waits for the user to close the window.  cWin.ExitButton() can be used to automatically
    // create a button with a message (default or set by the function call) to tell the user to press the button to close the window.
    //
    // WaitforClose() returns 0 (or you can set the value as a parameter).  Many programs end right after WaitforClose(), so it returns
    // a value for the convineince of using it on the same line as the return. 
    //
    // Now wait for window to close or the cancel button to be pressed again to exit. 

    cWin.DisableClose(false);           // Let the user close the window now (we disabled the close with NoClose(), so now release it)
    
    cButtonCancel.SetText("  Quit  ");  // Change text of button

    cButtonCancel.SendCloseMessage();   // One way to wait for the window close only is to set a button 
                                        // status to send a close message.  Now we can just wait for a window close, whether
                                        // the user pressed the close button (now titled "quit") or closed the window by pressing the 'X'
                                        // or Alt-F4

    cWin.WaitforClose();                // Wait for close (or Cancel Button to be pressed since it will now send a close message)
}

