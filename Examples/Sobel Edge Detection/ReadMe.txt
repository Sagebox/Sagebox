************************************
SageBox Sobel Edge Detection Example
************************************

 This program shows a quick example of  loading an image with the GetFile functionality of SageBox, then 
 converting it to a bitmap. 
 
 After conversion, a sobel convolution is then applied to get the edges. 
 
 This program shows how easy it is to get a file and to perform image-processing a calculations on an image.
 
 There are two versions (projects) of the Sobel Edge Detection -- one for black-and-white edges, and the other for color
 that show the edge in the color of the area behind the edges.

------------------------------
This is a Console Mode Program
------------------------------

 This is a great exampe of a console mode program using SageBox to give it graphic components.
 Note the "conAssert" macro and ErrorExit() function below -- these are console-mode functions to 
 exit the program with a message.  These are printed to std::printf.  As a console-mode program,
 there is no implementation for passing errors to another window.

 If a Windows mode is set in the project settings, this program will still work fine, but there will
 be no error messages if the program fails to load the JPEG or no filename is entered (since there is no console window).

--------------------------------------
SageBox Features Shown in this Program
--------------------------------------

This program is very short, so just look around and see what SageBox functions are used.  They are usually
self-explanatory and self-documenting.  In most cases, you can hold the mouse over the function and the 
documentation for that function will appear on the screen.
