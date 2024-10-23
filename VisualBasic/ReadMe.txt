-----------------------
Sagebox in Visual Basic
-----------------------

The files included in this directory are a prototype version of Sagebox in Visual.  

------------------
Sierpinski Project
------------------

The Sierpinski Project is a sample Visual Basic Sagebox project that draws a Sierpinski Triangle, showing how to draw shapes, text, and create a simple input box.

	------------------
	Visual Studio 2022
	------------------

	The file Sierpinski.sln is a Visual 2022 solution and should compile and run as-is.

----------
Sagebox.vb
----------

This can be included in the source directory of the Visual Basic project. Simply declare 

	import <projectname>.Sagebox

to import Sagebox in the Visual Basic program.

Sagebox.vb is the main interface for Sagebox.  It is currently being more filled out and has many Sagebox functions.
The functions a currently mostly uncommented, except for a few used in examples.

-------------
SageboxVB.dll
-------------

SageboxVB.dll is required by Sagebox.vb.

To include SageboxVB.dll, it must be located by Sagebox.vb.  Search for the line: 

	Public Declare Function _SageVB_Init Lib "SageboxVB.dll" ...

and changed it to reflect the path to Sagebox.VB.dll, e.g. "mydir\myproject\SageboxVB.dll"


----------------------------
Future Versions and Feedback
----------------------------

This directoy is currently a prototype version of Sagebox in Visual Basic.  

Sagebox is actually complete for Visual Basic, with the only time-consuming work to fill out and
add functions to Sagebox.vb, as well as adding comments and instructions.

The project is looking for feedback on usage in Visual Basic.  

Please send e-mails to rob@ProjectSagebox.com
