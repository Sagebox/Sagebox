************************************
10-Line Sagebox Windows C++ Examples
************************************

The full GitHub Sagebox libraries for Windows C++ (using Microsoft Visual Stuio) can be downloaded:

https://www.github.com/Sagebox/Sagebox


This is a set of examples that originated with a Quora Question:

"What is the coolest C++ program you can write in 10 lines of code."

I thought it was a nice challenge and wrote a number of programs in 10-line to show Sagebox
working in Windows and Procedural C++ together. 

These are all graphics examples, and other examples show more of what Sagebox can do with dialog and other UI features,
as well as helper functions and classes. 

Note: To keep within the 10-line limit and to keep the code readable, DrawPixel() is used to draw pixels for the Mandelbrot and
Julia Sets.  This passes through to Windows SetPixel() which is known to be slow (10x slower than using a bitmap) 

The best way to output is with a simple Bitmap -- see the bottom of this readme file for information.
Also see the Simple Mandelbrot project -- All but the first example shows how to output using a bitmap
instead of DrawPixel() to much faster execution speed.

Note: See screenshots.jpg for a collage of screenshots of all programs.

Note: Windows vs. Console Mode.  Some of the examples are pure Windows programs and some are Console Mode.  With Sagebox, you can do either
      without changing any code -- Sagebox handles the WinMain and calls your main().
      
      With a different project template you can make a pure Windows program where you specify your own WinMain() in a more canonical
      Windows manner.

    
-------------------------------------
The programs contained in the project
-------------------------------------

    1. 3D-Mandelbrot

    This shows a nice 3D-mandelbrot displayed in a monochrome image.  It is very detailed and is printed out as a large
    image.  See the Simple Mandelbrot example for more expansive and interact Mandelbrots.

    2. Julia Set #1

    This is basically the same code as the above, except the the 'z' value changes rather than the 'c' value.  It prints out a nice
    solid red and cyan graphic

    3. Julia Set #2 

    This is also basically the same as the Mandelbrot.  It's almost exactly the same as the #2 example, just with a different static 'c' value
    and a different method to display the colors.

    4. Fractal Tree

    This prints out a colorful fractal tree.  This shows using a recursive lambda, which can be interesting, especially since you can't use 'auto'
    (see the source code)

    This program kind of 'squished' into 10 lines.  Still readable, but you can do much more with it by expanding it.
    The Fractal Tree will be its own Example Sagebox Project soon, including a version that writes out an AVI to create a 60fps
    animation that can be converted into a WEBM, GIF, or MP4.

    6. Lissajous (with user input)

    Lissjous is somewhat of a basic program, but shows some nice use of the simple graphics routines, as well as showing a little more
    advanced/intermediate programming with the use of the CfPoint structure for point-based math operations, as well as the powerful
    dialog.GetFloat() function to get a number from the user.

    5. Circles

    An even more basic program, but still pretty.  This program prints 50 circles around a larger circle. 
    This program displays an Exit Button on the bottom of the screen, which is a function used to let the
    user know the program has ended -- other examples simply wait for the window to close (without a button)

-----------------------------
DrawPixel() vs using a Bitmap
-----------------------------

As mentioned above, the programs in this project use DrawPixel, which can be slow.  There is an easy way 
to avoid useing DrawPixel and make the output up to 10x faster. 

When using DrawPixel, typically this is specified:

    DrawPixel(x,y,MyColor);


Instead, you can do this:

    1.  Specify a bitmap at the top of the loop with the area dimensions

        auto cBitmap = cWin.CreateBitmap(GetWindowSize());  // In this case we want the window size, but it can be anything

    2.  Use Setpixel() in the bitmap instead of DrawPixel()

        cBitmap.SetPixel(x,y,MyColor);          // or you can use SetFastPixel() which is faster but does no range-checking

    3. Draw the bitmap at the end of the main loop:

        cWin.DisplayBitmap(cBitmap);

and that's it!

If you want to see the bitmap each line, you can just make a one line bitmap and do the following:

    1. On the top loop:                 auto cBitmap = cWin.NewBitmap(Width)
    2. When putting out the pixel:      cBitmap.SetPixel(x);                    // use no y here
    3. When the inner X loop is done:   cWin.DisplayBitmap(0,y,cBitmap);        // Display the bitmap on the Y line we're on

This will be at least 10x faster per-pixel than using DrawPixel() (aka SetPixel() in Windows).

See the "Simple Mandelbrot" project for an example that uses this method. 