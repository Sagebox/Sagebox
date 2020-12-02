***************************
SageBox Dial Widget Example
***************************

       This program is an example of using a dial, which can be used as a way to emulate a device such as a wall heater, pool heater, or any other
       similar device.

       The program is a demonstration to show how the Dial widget works, simply putting out the numbers in the dial to the right side of the window.

       About 15 lines of code, this is a good way to look at some simple SageBox functions, such as creating the window, and a basic use of the 
       Event Loop

       It's also a good example of the Text Widget, which allows us to draw a large number on the screen, centered in its
       own declared window -- the window itself doesn't show, because it copies the background of the main window through use
       of the Transparent() option.

       This program also shows the use of a fire-and-forget Text Widget that displays a message, but then has no other use -- therefore the 
       returned Widget object pointer does not need to be saved, since the Widget is managed and deleted by the Window.

-----------------------------
The Diagnostic Window in Dial
-----------------------------

 A special thing to notice in this program is the diagnostics window contained in the Dial Window.
 
 Clicking the Right-Mouse Button. The Dial Widget uses another widget that pops up a small menu window when the right-mouse button is clicked.
 In the Dial, three options come up, allowing the developer various options.  Try it to see what it does.
 
 The right-click menu that comes up is one of many SageBox development functions that make developing widgets and programs easier. 
 The main purpose in the dial is to make sure the mask for the dial color is correct, where one mode is to show the mask as the dial 
 is used.

 The source code for the Dial Widget will be released at a later date.
