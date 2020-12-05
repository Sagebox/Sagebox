// Sobel Edge Detection Source -- Copyright(c) 2020, 2021 Rob Nelson.  robnelsonxx2@gmail.com -- all rights reserved.
// This file, information, and process within are for personal use only and may not be distributed without permission.
// Please modify, copy, do whatever you want for personal uses.  For professional, distribution or commercial uses, 
// contact the e-mail address above

// ************************************
// Sagebox Sobel Edge Detection Example
// ************************************
// 
//  This program shows a quick example of  loading an image with the GetFile functionality of SageBox, then 
//  converting it to a bitmap. 
//  
//  After conversion, a sobel convolution is then applied to calculate the edges. 
//  
//  This program shows how easy it is to get a file and to perform image-processing a calculations on an image.
//  
//  There are two versions (projects) of the Sobel Edge Detection -- one for black-and-white edges, and the other for color
//  that show the edge in the color of the area behind the edges.
//
// ------------------------------
// This is a Console Mode Program
// ------------------------------
//
//  This is a great example of a console mode program using SageBox to give it graphic components.
//  Note the "conAssert" macro and ErrorExit() function below -- these are console-mode functions to 
//  exit the program with a message.  These are printed to std::printf.  As a console-mode program,
//  there is no implementation for passing errors to another window.
//
//  If a Windows mode is set in the project settings, this program will still work fine, but there will
//  be no error messages if the program fails to load the JPEG or no filename is entered (since there is no console window).
//
// --------------------------------------
// SageBox Features Shown in this Program
// --------------------------------------
//
// This program is very short, so just look around and see what SageBox functions are used.  They are usually
// self-explanatory and self-documenting.  In most cases, you can hold the mouse over the function and the 
// documentation for that function will appear on the screen.
//

#include "CSageBox.h"

// A simple assert to exit back to the Command Window on error 
// That's the nice thing about using a console mode program, it's easy to just abort.
//
#define conAssert(_x,_Msg) { if (!(_x)) ErrorExit(_Msg); }

// ErrorExit() -- Print an error message then exit back to the command window.
//
void ErrorExit(const char * sMsg)
{
    printf("Error: %s\n",sMsg);
    exit(0);
}

int Sobel()
{
    CSageBox cSageBox("Sagebox Sobel Example");        // Initial Sagebox with a window title

    // Get a file from the user. If the file comes back blank, it was aborted.
    // an stFileOpenStruct_t can be used for more control, such as labels.
    //
    // Alternatively, since this is a console-mode application, we could also just send in a file name from the command line, 
    // but this is nice because the user can browse thumbnails.

    auto csFile = cSageBox.GetOpenFile("*.jpg");                // Get the file from the user, listing only jpegs
    conAssert(!csFile.isEmpty(),"No File Specified.");          // Error-out if the user entered no file.

    // Read the JPEG.  if it comes back invalid or empty, then it either was not a JPEG, not found, or there
    // was an error reading it.  You can get the specific Jpeg error by calling  GetJpegError() -- in this case we don't 
    // care about the actual error, just that we didn't get a bitmap in memory from loading the file.

    auto cBitmap = cSageBox.ReadJpegFile(csFile);                                   // Get the JPEG
    conAssert(cBitmap.isValid(),"JPEG not found or not a valid JPEG file.");        // Error-out if something didn't happen correctly

    printf("FileName: %s\nOriginal Size = %dx%d\n",*csFile,cBitmap.GetWidth(),cBitmap.GetHeight());

    // Use two thumbnail/resize tools. The default for both are "BestFit" (it is shown in the QuickThumbnail call for reference, but not required),
    // Which will size the image to the smallest dimension in the Width/Height given while keeping the proportions of the image. 
    // If the image is smaller than this, then it will return the image without resizing it. 
    //
    // "ExactBestFit" will resize the image to the lowest Width/Height value (keeping the image the same proportion), even of the image is smaller.
    // There are also a number of other options.

    cSageBox.QuickThumbnail(cBitmap,350,350,ThumbType::BestFit);            // Get a Best Fit thumbnail and display it on the screen
    auto cNewBitmap = cSageBox.QuickResize(cBitmap,1200,800);               // Get a best fit, no more than 1200X, 800Y, if the image is larger than this.

    int iWidth  = cNewBitmap.GetWidth();
    int iHeight = cNewBitmap.GetHeight();
     
    auto cBitmapOut = cSageBox.CreateBitmap(cNewBitmap.GetSize());          // Get an output bitmap of the same size of the one we resized. 

    // ** Note: There is no error-checking on the bitmap.  If it is assumed there are no memory allocation errors in their creation.  This is safe, since 
    //           we've already verified we have a valid bitmap (i.e. we errored-out back to the command window if we didn't get a valid bitmap in memory
    //           from loading a JPEG)
    //
    // This generally falls under the assumption that if you have a memory problem here, something is systemically wrong in the system.
    //
    // In release-level software and huge memory allocations (such as a 200 Megapixel image), etc., The return bitmaps can be checked
    // for a valid or empty state to determine if a memory error has occured, or exception handling can also be installed.
    
    cBitmapOut.FillColor({0,0,0});    // Clear the bitmap with black since we don't fill in the edges.

    // Arrays for the convolution on the Sobel Matrix:  X,Y, and Multiplier (Where X and Y or offsetts from the current pixel)
    
    int iArrayV[6][3] = {   { -1,-1, 1 },
                            { -1, 0, 2 },
                            { -1, 1, 1 },
                            {  1,-1,-1 },
                            {  1, 0,-2 },
                            {  1, 1,-1 } };

    int iArrayH[6][3] = {   { -1,-1, 1 },
                            {  0,-1, 2 },
                            {  1,-1, 1 },
                            { -1, 1,-1 },
                            {  0, 1,-2 },
                            {  1, 1,-1 } };


    double fDiv = sqrt(2);    // Divide by sqrt(2) since we're doing y = sqrt(a^2 + b^2)

    // Get the pixel from the array (X,Y, multiplier), multiply it and return it as a Gray value
    // Normally I would use a lambda here, but MSVC lambdas do not yet support __inline, which 
    // makes it twice as slow as need be -- so an old-fashion define lets us do it inline.

    #define GetGray(ipInd) (ipInd[2]*(int) cNewBitmap.GetPixel(j+ipInd[0],i+ipInd[1]).Gray())

    // The main loop -- go through each pixel and get the values of the two convolutions for each pixel. 
    // Then get the magnitude and put it out as a pixel in our new bitmap.

    for (int i=1;i<iHeight-1;i++)
    {
        for (int j=1;j<iWidth-1;j++)
        {
            double fGrayV = 0,fGrayH = 0;
            for (int k=0;k<6;k++) fGrayV += GetGray(iArrayV[k]);
            for (int k=0;k<6;k++) fGrayH += GetGray(iArrayH[k]);

            int iMag = (int) min(255,sqrt(fGrayV*fGrayV + fGrayH*fGrayH)/fDiv);

            cBitmapOut.SetPixel(j,i,{iMag,iMag,iMag });
        }
    }

    cSageBox.BitmapWindow(cBitmapOut,Title("Sobel Result"));    // Show the new bitmap
    cSageBox.ExitButton("Sobel Edge Detection Finished.");      // Get an input from the user so we don't exit the program
                                                                // (which deallocates all memory and closes all windows)
    return 0; 
}

int main()
{
    return Sobel();
}