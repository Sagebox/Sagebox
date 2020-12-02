---------------------
My Projects Directory
---------------------

This directory contains a basic outline of how to use the Sagebox library with the project templates. 
It can be copied into any project directory to be used with the Sagebox project templates.

The required items for any project that uses Sagebox are:

	1. All files in the include directory (and subdirectories) -- these do not need to be part of your project. 
           only CSagebox.h needs to be included (and also some other files for special items, such as Widgets)
	2. The SageBox.lib file for x32 or x64
	3. WinMain.lib -- only if you're using the "Console Mode and Windows Only" template in Windows mode 
   	   (or which to have WinMain call your main() automatically)

The Widgets are optional, and will be filled out over time. 

As long as the files above are somewhere in the project for the MSVC compiler to find, that is all you need.


This directory structure can be copied directly into any project directory and the Sagebox templates will work with them.  Otherwise, it is
easy to make your own projects that refer to the directories and files wherever the are (i.e. include path and .lib path)


The sample project, "Sample Project" shows the basic form -- the Sample Project is a simple project, but can also be a solution,
as long as its top directory is in the same directory where the lib, include, and Widgets directory are located. 

The lib, include, and Widgets directory are included here to make sure that new updates do not change any compatibility with projects added to this directory.
