// Color Selector Example -- Copyright(c) 2020, 2021 Rob Nelson.  robnelsonxx2@gmail.com -- all rights reserved.
// This file, information, and process within are for personal use only and may not be distributed without permission.
// Please modify, copy, do whatever you want for personal uses.  For professional, distribution or commercial uses, 
// contact the e-mail address above

// **********************
// Color Selector Example
// **********************
//
//    This example shows using the Color Selector (vs. the Color Wheel) on a simple image multiply function. 
//    This also shows an example returning an image-processing result in real-time.
//
//    The Color Selector is a Widget that uses a Widget.  The Color Selector uses the Color Wheel Widget, and then adds on to it 
//    to provide an enhanced Color Selection with Edit Boxes to set the color as well as the original Color Widget.  As the colors 
//    change, the Edit Boxes and Circular Controls on the Color Wheel update automatically.
//
//    The Color Selector is a Widget itself, and shows how Widgets can be used directly or used in other widgets. 
//
// -------------------------------
// SageBox Fetures in thie Program
// -------------------------------
//
//  Review the source code for generally using SageBox function.  But, there are some specific features to note:
//  
//  1. Getting File Name Input
//  
//      This program uses GetOpenFile() to get a filename from the user.  This is a simple way to do it.  You can also use GetOpenFileStruct() to get a file 
//      structure to fill in more information, such as window titles (for the file window) and more. 
//
//      In this case GetOpenFile() returns a CString() object, which either has the name of the file or is blank if no file name was entered. 
//  
//  2. Error control by checking for Empty bitmap/object
//  
//      In many SageBox examples, error-checking is not used, as it is not necessary for most elements.  In this case, however, because we're dealing with filling memory,
//      we need to make sure the filename is valid and the JPEG is loaded correctly.
//  
//      The filename and bitmap are checked for either isValid() or isEmpty() to determine the validity of the elements returned. 
//      See the stdAssert() statements in the source code.
//  
//      Most function also have the option to fill a boolean for success or failure, but are not used in this program.
//  
//  3. HideConsole()
//  
//      This program is a console program and has a console window.  However, the console window is not used.  This shows an example of hiding the 
//      console window, effectively turning the program window a Windows program for the duration of the program's run.  The console-mode window 
//      will appear again automatically when the program ends -- unless HideConsole(true) is issued, in which case this tells SageBox to not bring
//      back the console window at the end of the program.
//  
//  4. Using Bitmaps (CSageBitmap)
//  
//      Bitmaps are a large part of using graphics.  SageBox provides CSageBitmap as a powerful set of functions to use bitmaps.  See the functions below for exmaples
//      of using CSageBitmap. 
//  
//  5. Asserts (stdAssert)
//  
//      This program uses asserts to control the flow when unexpected errors occur -- in this case, if no file name is entered or the JPEG won't load because it's not a 
//      JPEG or is corrupted.
//  
//      Asserts allow for error-checking and action based on the error without using if blocks, which can keep code clean while handling errors and 
//      avoiding early exits.
//  
//      with stdTry, it defines "bError", and stdAssert() supports sending a message with the error.
//      In this program, when an error is thrown, the message is printed.  bError is not used in this program, but can be checked to return a status code and to 
//      perform actions if the function did not operate correctly (i.e. such as clearing memory items that would otherwise be returned, etc.)
//  
//  6. WinMessageBox (Console Mode or Windows)
//  
//      Even though this is a console mode program, Windows "MessageBox()" function is used -- wrapped as WinMessageBox in SageBox.  This is a great way to present
//      errors to the user, as the Message Boxes are utilitarian and a bit noisy, which definitely can grab the user's attention that something is not right.
//  
//  7. Hiding and Showing the Window
//  
//      The Window is initially hidden on creation with the opt::Hidden() option. It is hidden so the window may be resized and the image can be put in the window, without
//      the window showing first (which can make it look like it flashes).
//  
//      Show() is used after the window is resized and image put out to the window to show the window all at once.
//      If there is an error, the window is not shown at all, giving more prominence to the error message.
//
// ------------------------------
// This is a Console Mode Program
// ------------------------------
//
//    This program is a console mode program, showing how SageBox functions can be used in a procedural, console-mode program without going into Windows
//    or GUI specifics.  In SageBox, there is a thin line between console and Windows -- you can set the project settings for "Windows Debug" or "Windows Debug (Optimized)"
//    to convert it to a Windows program without changing any source code -- in this case, WinMain() is called by Windows, which is declared in SageBox() and transfers control
//    to main()
//

#include "CSageBox.h"
#include "CColorSelector.h"

int main()
{
    CSageBox cSageBox("SageBox Color Selector Example - Simple Image Processing (Multiply)"); 

    cSageBox.HideConsole(); // We don't need the console window, so let's hide it -- it comes back when the program is done and is a nice
                            // assurance that the program exited properly.  So we get a consoel mode program that acts exactly like a Windows
                            // program. 
                            // We can change this to a full Windows program (with no console window) with a project setting. 

    auto& cWin = cSageBox.NewWindow(Hidden());          // Create a new window, but keep it hidden so we can resize it before showing it.

    stdTry;

    CString cs        = cWin.GetOpenFile("*.jpg");      // Get a .JPG file
    auto cBitmap    = cWin.ReadJpegFile(cs);            // Read the file in -- if it doesn't exist or it is an empty string, it 
                                                        // just returns

    // Look at some errors using an Assert -- a nice way to avoid early exits and cleanup without a lot of if() statements.
    // If the file selection was cancelled, cs will be empty.  If not, then if cBitmap is empty, then there was an error
    // trying to load the jpeg (i.e. not found, corrupted, not a jpeg, etc. -- GetLastJpegError() can be called to find out.

    stdAssert(!cs.isEmpty(),"No image was specified."); 
    stdAssert(!cBitmap.isEmpty(),"The JPEG file could not be read. It is either not a JPEG or corrupt.");

    auto cResized = cWin.QuickResize(cBitmap,1200,800);     // Resize the bitmap to a Max Width of 1200 OR a max Height of 800, 
                                                            // Whichever the image fits into, keeping it the same proportions. 

    CSageBitmap csNew(cResized.GetSize());                  // Get a bitmap sized to our new resized bitmap.

    int iIndent = 50;                                       // Indent for the bitmap display

    int iWidth    = csNew.GetWidth();
    int iHeight = csNew.GetHeight();

    // Resize the window the size of the resized bitmap + the indent.  The "true" parameter tells SageBox
    // to make the client area (i.e. non-border areas) the size we want.  Otherwise it will resize the total size
    // of the window (borders and all) the size requested.

    cWin.SetWindowSize(CPoint(cResized.GetSize()) + SIZE{iIndent*2,iIndent*2},true); 

    cWin.Cls("black","darkblue");               // Now the we've resized it, lets give it a gradient of Black-to-Dark Blue

    CColorSelector cColor(&cWin,cResized.GetWidth() + iIndent*2 + 30,20,Popup() | NoClose());

    // Do simple multiply of the selected color to tone the image.
    // This is a lambda so we can call it initially and in the GetEvent() loop.
    // This allows us to not worry about a) making things global or b) refactoring
    // until it grows a little more. 

    auto Process =[&](RGBColor_t rgbColor)
    {
        unsigned char * sNew = csNew;       // Get starting address of the output bitmap memory
        unsigned char * sCur = cResized;    // Get starting address of the Resized bitmap memory.

        for (int i=0;i<iHeight;i++)
        {
            for (int j=0;j<iWidth;j++)
            {
                // There are various ways to do this quickly -- this one is fast enough while
                // keeping the code self-documenting.

                RGBColor_t rgbCur{sCur[2],sCur[1],sCur[0]}; // Get the three colors in RGB form
                rgbCur *= rgbColor;                         // This does a normalize multiply (normalized to 255), in this case (a*b)/255
        
                *sNew++ = rgbCur.iBlue;                     // Put out the new value.
                *sNew++ = rgbCur.iGreen;
                *sNew++ = rgbCur.iRed;
                sCur += 3;                                  // Get past current pixel 

            }
            sNew += csNew.GetOverhang();                    // Line-lengths of Windows bitmaps are divisible
            sCur += cResized.GetOverhang();                 // by 4, and the OverHang tells us how much we have left at the
                                                            // end of the line.
        }
        cWin.DisplayBitmap(iIndent,iIndent,csNew);          // Display the bitmap centered in the window
        cWin.Update();                                      // Update the window, 
    };

    Process({255,255,255});     // Process the first one with white so it won't change.
    cWin.Show();                // Show it since it was hidden so we could resize it.
    
    // The main event loop.
    // 
    // When the color is changed, re-process the image with the new color.
    // We don't really need anything else from the Color Selector, but since we're here, 
    // if the Ok, Cancel button is pressed (or the Color Selector Window is closed), we also exit
    // the loop.
    
    while(cWin.GetEvent())
    {
        if (cColor.ValueChanged()) Process(cColor.GetRGBValues());
        if (cColor.OkButtonPressed() || cColor.CancelButtonPressed()) break;
    }

    // Catch() -- We catch(const char * sErrMsg) here and display the error in a Windows Message Box -- 
    // Windows Message Boxes have a certain gravity that is great for scary error messages.

    stdCatcher
    {
        cWin.WinMessageBox(CString() << "Error displaying image --" << sErrMsg,"Load Image error",MB_OK | MB_ICONEXCLAMATION); 
    }

    return 0;
}