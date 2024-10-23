********************************
Two Widgets/MiniEmulator Example
********************************

 This program shows the use of the Dial and LCD widgets working together, forming the basis for emulating hardware.
 Believe it or not, this program (the OO version, not the monlithic version contained in main()) is the basis for 
 real emulation.  Only certain parts need to be overladed (the virtual functions) to talk to real hardware.

 The Emulation Premise

 The premise of the emulator is a simple local temperature dial and a remote, embedded device that receives the data (and has an LCD display).

 The temperature dial could be for a wall heater, or a pool heater (a real situation where SageBox has been used), or another device.
 The LCD display can be a real LCD display across a yard or courtyard to the actual heater, displaying real-time data.

Most emulators actually start off this small, expanding over time into something much larger.

-----------------------------------------
The PGR File - Portable Graphics Resource
-----------------------------------------

 The PGR file is used in many SageBox programs, mostly to provide a background texture.  In this program, it is used for more.
 It contains the background, but also information where to put things. 

 This allows the emulator to have many different configurations with different hardware without changing code: all controlled and defined
 by the PGR file.  Any number of PGR files can be used to direct where to put objects, and which ones to use and not use.

 The PGR file itself is compiled from a very simple text file with the values read in (location of buttons, etc.) and the background image. 
 It is then converted to memory so that the PGR file does not need to exist on disk locally when the program is run.

-----------------------------------
This version: main() implementation 
-----------------------------------

 This file is written on one main() function

 It is written this way for clarity and to show more easily some SageBox functions.

 Also, it is a good example of using lamdbas prior to moving to a class/OO-based program.
 The lambdas work as local class functions since they have acess to all local variables, and are 
 a great way to functionalize areas of code locally before deciding where they belong -- in the class, 
 as lambdas, static functions, etc. 

 The OO version is the more "proper" version for emulating hardware because it's easier to overload
 functions to actual hardware, emulation, debugging, etc. without modifying core emulator code. 
 (though in a pure 'C' environment function pointers can work just as well, too, though not as readable)

------------------------------------
This version: Class-Based OO version
------------------------------------

 This file is the same program as the non-oo version, just with the lamdbas and variables needed put into 
 the class rather than keeping in the main() function/

 This allows for expandability and overloading of functions for true emulation where desired. 
 The virtual functions can be overloaded and work with real hardware.
