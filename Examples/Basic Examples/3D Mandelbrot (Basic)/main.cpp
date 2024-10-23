
/// Sagebox is free for personal use.  website: www.projectsagebox.com -- github repository: https://wwww.github.com/Sagebox/Sagebox

#include "SageBox.h"
#include <complex>

// ---------------------------
// 3D-Mandelbrot Basic Example
// ---------------------------
// 
// This is a simple implementation of a 3-D mandelbrot. 
//
// This just a basic example, showing how to use standard C++ and Sagebox graphics at the same time.
//
// For information on the 3-D Mandelbrot, see documentation in the Sagebox 10-Liner "3D Mandelbrot" project.
//
// *** Note: This can be a Console Program with a Console Window or a Pure Windows program.  See the Build->Configuration settings.
// *** Note: CComplex is used instead of std::complex -- std::complex is 3x slower on 64bit applications, and just insanely slow for 32-bit applications.

int main()
{

    auto winSize   = CfPoint(1400, 900);     // Initial Window Size

    // Create a window with a client size cWinSize

    auto& win      = Sagebox::NewWindow("3-D Mandelbrot (Basic) Example",kw::SetSize(winSize));
  

    auto range      = 4.0f;                         // Initial Range (i.e. "zoom" factor)
    auto maxIter    = 50;                           // Max Mandelbrot Iterations

    auto center     = CfPoint(-.6f, 0.0f);          // Initial Mandelbrot Center
    auto cfRange    = CfPoint(range,range * (float) winSize.y / (float) winSize.x);

    auto fd         = cfRange / winSize;            // Unit Increment for each pixel
    auto start      = center - fd * winSize/2.0;    // Upper-left X,Y position to start

    for (int i = 0; i < winSize.y; i++)
    {
        double fy = (double)i * fd.y + start.y;
        for (int j = 0; j < winSize.x; j++)
        {
            double fx = (double)j * fd.x + start.x;

            int iter = 0;

            // For Julia Set, set z = { fx, fy } and c to a sta tic value, such as (.285, 0 ) or (-4.,.6) (and set cfCenter to (0,0);

            auto c = CComplex(fx, fy);      // Using CComplex instead of std::complex due to std::complex slowness, even in release versions.

            auto z = c;  
            auto dz = CComplex(0.0, 1.0);
            dz *= z+z; 
             
            while (abs(z) < 256 && iter++ < maxIter-1)
            {
                dz *= z + z;
                z = z * z + c; 
            }

            auto rgbOut = Rgb(0,0,0); 
                                      
            if (iter != maxIter)
            {
                auto vec = z / dz;
                vec /= abs(vec);
                auto gray = (int) (90.0 * (vec.real + vec.imag) + 128.0);
                rgbOut = Rgb(gray, gray, gray);
            }

            win.DrawPixel(j, i, rgbOut);
        }
    }

    return win.ExitButton(); // Wait for the user to press a button or close the window
}
