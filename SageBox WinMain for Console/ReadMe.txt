**************************************************************************************************
WinMain.Cpp -- WinMain Entry point for console-mode Sagebox programs using Windows project Setting
**************************************************************************************************

This file provides an automatic WinMain() that will call the main() in the console-mode-based program.
It converts the arguments from Windows-style arguments to normal main(int argc,char * argv[]) arguments.

int main(int argc,char * argv[]) must exist in the program's main() declaration. note: it can
be declared as "int main()"

The Windows project setting for a project that started as a Console Mode program converts the program into
a valid Windows program, removing the console window. 

As for other values, i.e. Instance, nCmdShow, etc.  These can either be obtained easily (such as GetInstance() in Windows), or
this file can be alterted to store the information somewhere accessible.

To supply your own WinMain(), remove the WinMain.obj reference in the LINKER settings in the project. 
Sagebox also has a Windows set of projects where the program can started in windows and then convert back to 
a console mode program (i.e. the reverse of this).


--------------
Important Note
--------------

This is a live Sagebox project.  The compilation for x32 and x64 targets compiles directly to the the WinMain.lib
file in the directories "pub\lib\x32" and "pub\lib\x64".

WinMain.lib (x32 and x64) are used for Windows projects to automatically include a WinMain() function that calls the
regular main() function.

Make sure you have a backup, as these files will be over-written when the project is compiled.

