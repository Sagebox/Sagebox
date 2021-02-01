
The full GitHub Sagebox libraries for Windows C++ (using Microsoft Visual Stuio) can be downloaded:

https://www.github.com/Sagebox/Sagebox

Reach me at rob@projectsagebox.com


--------------------------------------------
Fractal Tree Project -- Sagebox Example Code
--------------------------------------------

This project is a good example of how to use Sagebox, both with simple applications and more advanced applications
using moderate-to-avanced C++ programming, as well as Sagebox functions.

All 3 programs are purely procedural, written in plain, procedural C++ without using events, callbacks, or the need
to understand Windows internals or event-driven programming.  Basically, they are written in the same way one might write
a scripted language, console mode application, or even a Basic program, but with the power, speed, and structur of compiled
C++ running directly on the CPU and not through a VM or interpreter. 


These programs are meant as learning programs to show how Sagebox is used within the procedural Windows C++ environment
facilitated by Sagebox.

There are three separate projects included in this folder:

    1. Basic Fractal Tree   - Shows a 20-line program that uses-more-or less simple Sagebox functions to create the 
                              main window and output to the screen.
    2. Fancy Fractal Tree   - This is basically the same 20-line program as the Basic Fractal Tree, but adds a color element 
                              to the output, using RGB tables.  This shows how Sagebox functions are simple, but also expand 
                              as you need more from them. 

These two programs are great examples of using Sagebox functions with regular C++ code.  They can be either Console Mode programs or
Windows programs depending on a project switch (they start as Windows programs).  Since they are 100% procedural, they are written as 
basic C++ programs that simple use some Sagebox functions for output. 

The programs show how Sagebox works to create simple and powerful programs for beginners and hobbyists. 

------------------------
3. Fractal Tree Animated
------------------------

This program -- also 100 procedural -- uses quite a few different Sagebox functions to create a UI, animate the Fractal Tree, write it out
to an AVI file, as well as display debug information and look for input from the user -- All in 40 lines of code!

The main() function is 40 lines (and it calls out to the above code using about 20 lines) and performs a lot in those few lines of code.

As with the first two programs, this program is heavily commented to point out how Sagebox is being used to create the program.

See the notes in the main.cpp file for this project.  It details how Sagebox functions, helper functions, and classes or used to create the 
program without much work. 

This program is also an example of writing professional-level applications using Sagebox to help, either in-house or release programs.
Some of the C++ constructs used, such as a recursive lamdba are fairly advanced, and shows how Sagebox keeps it C++ by letting you use your
own style and not forcing you into any specific constructs or boilerplate.

