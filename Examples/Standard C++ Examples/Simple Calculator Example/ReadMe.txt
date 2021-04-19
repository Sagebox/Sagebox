*******************************************************
Calculator.Cpp -- SageBox Simple Calculator Application 
*******************************************************

Calculator is a simple SageBox program consisting of roughly 100 executable lines of C++ code.
It provides simple calculator functions such as +,-,/,*, as well as more advanced ones such as 
Sin, Cos, Log, etc.  It also features memory store, recall, and clear.

----------------------------
Outlined Features of SageBox
----------------------------

1. Button Groups and Windows-Style buttons

   Button groups can be used with regular buttons of any button style, as well as checkboxes and radio buttons.

   In this program button groups are demonstrated with Windows-Style buttons (i.e. rectangular) to fit a calculator look,
   although any button style can be used, including user-defined buttons.

   Button groups are a great way to specify an NxM row/column of buttons in programs that will use a number of 
   buttons that are generic.

   As with many controls in SageBox, Button groups manage themselves, and you do not need to remember them -- 
   Sagelight destroys them with the window.

   Only a user-supplied Group-ID is needed to access any button in the group.

   note: notice that one button group as a nullptr in the definition -- this allows a button space to be created without
         a button creation so that button groups can overlap, as they do in this example.

2. Using Controls without saving Object Pointers or other information

   As mentioned above, Button groups are defined as a group by a Group ID.  When used, only that number needs to be kept by the program to access button functionality.
   The actual button object can be retrieved for specific operations. 

   In this program, only the order-of-creation number is needed when asking SageBox is a member of the group has been pressed.

3. Edit Box functionality

   There is an edit box used in the calculator, and many of the functions show how it is used easily with floating point values.

4. Background Textures and ReadPGrBitmap()

   This reads a bitmap (either .BMP, .JPEG, or other format) embedded in a PGR (Portable Graphics Resource) file.  This is a quick
   way to get an ensure bitmap (i.e. there can be no error once it has been verified in testing once) is retreived. 

   There are many ways to retreive a bitmap or JPEG image in SageBox, and this is a nice way to store a collection and
   other information about a specific program.

   .PGR files may be used from the hard-drive or in-memory as in this program (i.e. for portability).
