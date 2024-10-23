
// ************************************
// Sagebox Sobel Edge Detection Example
// ************************************
// 
//  This program shows a quick example of  loading an image with the GetFile functionality of SageBox, then 
//  converting it to a bi tmap. 
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

#include "Sagebox.h"

// A simple assert to exit back to the Command Window on error 
// That's the nice thing about using a console mode program, it's easy to just abort.
//
#define conAssert(_x,_Msg) { if (!(_x)) ErrorExit(_Msg); }


int Sobel()
{

    // Get a file from the user. If the file comes back blank, it was aborted.
    // an stFileOpenStruct_t can be used for more control, such as labels.
    //
    // Alternatively, since this is a console-mode application, we could also just send in a file name from the command line, 
    // but this is nice because the user can browse thumbnails.

    auto csFile = Sagebox::GetOpenFile("*.jpg");                // Get the file from the user, listing only jpegs
    conAssert(!csFile.isEmpty(),"No File Specified.");          // Error-out if the user entered no file.

    // Read the JPEG.  if it comes back invalid or empty, then it either was not a JPEG, not found, or there
    // was an error reading it.  You can get the specific Jpeg error by calling  GetJpegError() -- in this case we don't 
    // care about the actual error, just that we didn't get a bitmap in memory from loading the file.

    auto bitmap = Sagebox::ReadJpegFile(csFile);                                   // Get the JPEG
    conAssert(bitmap.isValid(),"JPEG not found or not a valid JPEG file.");        // Error-out if something didn't happen correctly

    printf("FileName: %s\nOriginal Size = %dx%d\nResizing...",*csFile,bitmap.GetWidth(),bitmap.GetHeight());

    // Use two thumbnail/resize tools. The default for both are "BestFit" (it is shown in the QuickThumbnail call for reference, but not required),
    // Which will size the image to the smallest dimension in the Width/Height given while keeping the proportions of the image. 
    // If the image is smaller than this, then it will return the image without resizing it. 
    // "ExactBestFit" will resize the image to the lowest Width/Height value (keeping the image the same proportion), even of the image is smaller.
    // There are also a number of other options.

    Sagebox::QuickThumbnail(bitmap,350,350);                       // Get a Best Fit thumbnail and display it on the screen
    auto newBitmap = Sagebox::QuickResize(bitmap,1200,800);       // Get a best fit, no more than 1200X, 800Y, if the image is larger than this.

    printf("\nDone Resizing.\n");

    int width  = newBitmap.GetWidth();
    int height = newBitmap.GetHeight();
     
    auto bitmapOut = Sagebox::CreateBitmap(newBitmap.GetSize());          // Get an output bitmap of the same size of the one we resized. 

    // ** Note: There is no error-checking on the bitmap.  If it is assumed there are no memory allocation errors in their creation.  This is safe, since 
    //           we've already verified we have a valid bitmap (i.e. we errored-out back to the command window if we didn't get a valid bitmap in memory
    //           from loading a JPEG)
    //
    // This generally falls under the assumption that if you have a memory problem here, something is systemically wrong in the system.
    //
    // In release-level software and huge memory allocations (such as a 200 Megapixel image), etc., The return bitmaps can be checked
    // for a valid or empty state to determine if a memory error has occured, or exception handling can also be installed.
    
    bitmapOut.FillColor({0,0,0});    // Clear the bitmap with black since we don't fill in the edges.

    // Arrays for the convolution on the Sobel Matrix:  X,Y, and Multiplier (Where X and Y or offsetts from the current pixel)
    
    int arrayV[6][3] = {    { -1,-1, 1 },
                            { -1, 0, 2 },
                            { -1, 1, 1 },
                            {  1,-1,-1 },
                            {  1, 0,-2 },
                            {  1, 1,-1 } };

    int arrayH[6][3] = {    { -1,-1, 1 },
                            {  0,-1, 2 },
                            {  1,-1, 1 },
                            { -1, 1,-1 },
                            {  0, 1,-2 },
                            {  1, 1,-1 } };


    double fDiv = sqrt(2);    // Divide by sqrt(2) since we're doing y = sqrt(a^2 + b^2)

    // Get the pixel from the array (X,Y, multiplier), multiply it and return it as a Gray value
    // Normally I would use a lambda here, but MSVC lambdas do not yet support __inline, which 
    // makes it twice as slow as need be -- so an old-fashion define lets us do it inline.

    #define GetGray(ipInd) (ipInd[2]*(int) newBitmap.GetPixel(j+ipInd[0],i+ipInd[1]).Gray())
    
    // The main loop -- go through each pixel and get the values of the two convolutions for each pixel. 
    // Then get the magnitude and put it out as a pixel in our new bitmap.
      
    for (int i=1;i<height-1;i++)
    {
        for (int j=1;j<width-1;j++)
        {
            double grayV = 0,grayH = 0;
            for (int k=0;k<6;k++) grayV += GetGray(arrayV[k]);
            for (int k=0;k<6;k++) grayH += GetGray(arrayH[k]);

            int iMag = (int) min(255,sqrt(grayV*grayV + grayH*grayH)/fDiv);

            // Put out the edge using the color of the pixel 
            // 
            // In the previous version, the next line was:
            //
            //     bitmapOut.SetPixel(j,i,{iMag,iMag,iMag });
            //
            // Now two lines are added, where we get the pixel of the current location and multiply it against the 
            // magnitude.  This could be done in one line as before, but it was separated into two lines to amke
            // code more legible.

            auto rgbColor = newBitmap.GetPixel(j,i)*RGBColor_t{iMag,iMag,iMag };    // Get Pixel*Mag

            bitmapOut.SetPixel(j,i,rgbColor);    // Put the new color out to the bitmap
        }
    }

   Sagebox::ImgView(bitmapOut,kw::Title("Sobel Result"));    // Show the new bitmap
    
    return Sagebox::ExitButton("Sobel Color Edge Detection Completed");     // Tell the user we've completed the sobel process. 
                                                                            // The menu was added first so it just didn't "pop up" after the user clicked the 
                                                                            // OK button in the info dialog. (ExitButton() always returns 0)
}

int main()
{
    return Sobel();
}