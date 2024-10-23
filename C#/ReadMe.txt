-------------
Sagebox in C#
-------------

The files included in this directory are a prototype version of Sagebox in C#.  


------------------
Sierpinski Project
------------------

The Sierpinski Project is a sample C# Sagebox that draws a Sierpinski Triangle, showing how to draw shapes, text, and create a simple input box.

	------------------
	Visual Studio 2022
	------------------

	The file Sierpinski.sln is a Visual 2022 solution and should compile and run as-is.


	-----------
	Visual Code
	-----------

	To use the Sierpsinki Project in Visual Code, simply open this directory as a folder, 
	and Visual Studio should determine it is a C# project. 

	Just run the program, selecting the default C# debugger and compiler.

----------
Sagebox.cs
----------

This can be included in the source directory of the C# project. Simply declare 

	using Sagebox.Main;

to import Sagebox in the C# program.

Sagebox.cs is the main interface for Sagebox.  It is currently being more filled out and has 
many Sagebox functions.

The functions a currently mostly uncommented, except for a few used in examples.

-------------
SageboxCS.dll
-------------

SageboxCS.dll is required by Sagebox.cs.
To include SageboxCS.dll, it must be located by Sagebox.cs.  Search for the line: 

	private const string kDLL = "SageboxCS.dll";

and changed it to reflect the path to Sagebox.CS.dll, e.g. "mydir\\myproject\\SageboxCS.dll"


----------------------------
Future Versions and Feedback
----------------------------

This directoy is currently a prototype version of Sagebox in C#.  Sagebox is actually complete
for C#, with the only time-consuiming work being to fill out and add functions to Sagebox.cs, 
as well as adding comments and instructions.

The project is looking for feedback on usage in C#.  Please send e-mails to rob@ProjectSagebox.com
