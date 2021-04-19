
---------------------------------------------
Sagebox Quick C++ Example Project Directories
---------------------------------------------

There are two types of project example directories:

    1. Basic Examples.  These are "Hello-World" level examples, or a little above, to show 
       basic Sagebox Quick C++ code without creating an entire program. 


    2. Examples.  Meant for standard C++ programming, where main() and WinMain() are called directly by
       the OS.  The Sagebox enviroment is called manually from main() or WinMain()

See the documentation below for more information.

---------------------
Standard C++ Examples
---------------------

The Standard C++ examples are meant for more general programming, and are no different than any other C++ Console 
mode or Windows program.  

Your main() or WinMain() is called directly by the OS, and the Sagebox environment must be started manully with
instantating CSagebox, and then creating a Window with NewWindow() (or using CSagebox::AutoWindow()). 

Standard C++ projects are regular C++ programs, and with these projects, you can add Sagebox to your program by
simply referencing the Sagebox.lib library and CSagebox.h.  See the "TestConsolePlain" project in the "My Projects" directory.

