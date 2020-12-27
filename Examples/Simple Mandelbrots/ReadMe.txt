******************************
Interactive Mandelbrot Example
******************************

This set of Mandelbrot programs shows creating and displaying a Mandelbrot and/or Julia Set 
in various stages, from very basic, to faster, all the way to an interactive program.

This is #4 Interactive Mandelbrot Example

---------------------------
Mandelbrot Example Projects
---------------------------

 1. Basic Mandelbrot

     This is a simple Mandelbrot with color, using std::complex. 
     It's slow, but works and is only about 25 lines of code.
 
 2. Basic Mandelbrot Faster

     This adds speed to the mandelbrot by replacing std::complex with CComplex, 
     Windows DrawPixel() with a bitmap funciton via CSageBitmap. 

 3. Smooth Color, Julia Set, and Dev Window

     This is a small change from the previous version, adding smooth coloring and the Julia
     set to the output.   

     The Dev Window is also used to show the ease-of-use of the Dev Controls without affecting your main code or window.

     This version also adds some more power by assing z.sq() instead of z*z for a little faster
     Mandelbrot calculation.  FastSetPixel() is also used in CSageBitmap to set the pixel much faster.

 4. MandelBrot Interactive (zoom in/out, resize window, set Julia Set and more) 

     This shows how to add Development-based controls to change values of the Mandelbrot
     with just a few lines of code.

     It shows using GetEvent() in the main eventloop to get and react to events. 
 

-------------------------------------------
About Project #1:    MandelBrot Interactive
-------------------------------------------

 This version adds interativity where you can zoom in and out of the image with the MouseWheel and
 can click anywhere on the image to set the center.

 The window can also be resized for smaller or larger windows -- smaller windows print faster.
 The iterations can also be set in an edit box or with the mousewheel, as well as number of other options.

 Run the program and look at the source code fore more information and comments.

 I expect to do a video series on this set of programs and will explain more in-depth about the use of SageBox in
 the videos.

--------------------------------------------
About Project #2:    Basic Mandelbrot Faster
--------------------------------------------

 This version is an extension of the Basic Mandelbrot, change a small amount of code to make the Mandelbrot generation
 and display 5x faster or more (actually, I think much more)

 As with many std namespace functions, std::complex can be  slow.  This is replace with CComplex, which is a small (and growing)
 set of complex functions that inlines code directly, making it just about as fast as expanding the complex functions into vector-based math
 but without the headache. 

 DrawPixel (which calls Windows::SetPixel) is replaced with a CSageBitmap::SetPixel call.  Windows::SetPixel is very, very slow!
 It is usually never a good idea to use Windows::Setpixel (accessed through Sagebox CWindow::DrawPixel()) for general output.

 Instead, putting values out to a bitmap is a much faster way to go.  

 Using CComplex

    1. All CComplex functions are inline, meaning there are not calls for general routines to flush the processor cache, on of the things that
       makes it faster than CComplex.  Some calls are made, only as required, such as sqrt() for the abs() function, and so-forth.

    2. Note use of z.absSq().  We can't use abs(z) or because it's not a template, so we must us z.abs() instead as a member function.
       In this case, we use absSq() that allows us to get the magnitude^2 without calling a sqrt() which makes it much faster. 


 Run the program and look at the source code fore more information and comments.

 I expect to do a video series on this set of programs and will explain more in-depth about the use of SageBox in
 the videos.

-------------------------------------------------------------
AAbout Project #3:    Smooth Color, Julia Set, and Dev Window
-------------------------------------------------------------

 This verison adds smooth color to the mandelbrot and also adds the ability to display the Julia Set
 (The Mandelbrot is special case of the Julia Set).

 This version also adds a couple items to the DevWindow, showing how easy it is to use the Dev Controls
 to add controls to your program without creating them or adding them to an existing window.

 You can set the Julia set and also the Julia Set paramaters from the Dev Window, and 
 you can also use the Mouse Wheel on the edit boxes.

 Run the program and look at the source code fore more information and comments.

 I expect to do a video series on this set of programs and will explain more in-depth about the use of SageBox in
 the videos.

-------------------------------------------
About Project #4:    MandelBrot Interactive
-------------------------------------------

 This version adds interativity where you can zoom in and out of the image with the MouseWheel and
 can click anywhere on the image to set the center.

 The window can also be resized for smaller or larger windows -- smaller windows print faster.
 The iterations can also be set in an edit box or with the mousewheel, as well as number of other options.

 Run the program and look at the source code fore more information and comments.

 I expect to do a video series on this set of programs and will explain more in-depth about the use of SageBox in
 the videos.

-------------------------------------------------------
Using elements of this program for your own programming
-------------------------------------------------------

As with all SageBox demo code, it is written to show how SageBox works, but also general C/C++ programming practices.

** Please feel free to use any of this code for your own personal purposes **

SageBox is specifically written to be portable and used by example and the "intellisense" comments that come up for each function.
Most SageBox code snippets are also portable and can be taken from one program and used in another.

SageBox is free for personal use. 

