**************************
SageBox LCD Widget Example
**************************

 This program is an example that shows a very simple top-level emulation of an LCD dipslay module. 
 The program itself is very short, showing how to use SageBox Widgets. 
 
 The code for the widget itself will be released at a later date.
 
 Widgets are easy to write, and can be used by any SageBox application.
 In this case, the .H file is included for the widget, and the widget is put into the window
 with one line of code. 
 
 See the code for a general idea of how to use SageBox functions, as well as how the LCD widget works. 
 
 The general idea is that later, as an emulation tool, this can be expanded to 
 work with lower and lower levels, such as the LCD segments and even the output ports. 
 
 Once working at a lower level, we can then use it to talk to hardware directly and rather than emulating, we can then use it
 to report what is happening on an embedded system itself.

----------------------------------------------
Using the Program - Fast Mode vs. default Mode
----------------------------------------------

 There are two buttons in this program, and both to the same thing: count from 0 - 1000000. 

 However, they do this in two ways, showing how SageBox can be used generally, but also for much more efficient programs.

 Default Mode Button

   This button simply counts to 1,000,000, updating the LCD at each count.  Being slower than the fast mode button, it also shows how to change
   the button title and use signals to get input rather than looking at the button through Sagebox calls.

Fast Mode Button

   In many places in SageBox, it is suggested that -- when auto updates are turned off -- the Update be given as Update(10), which will
   cause the SageBox to only update the window every 10-20 ms (or whatever number you give it). This allows the system to only update when it is 
   time to update -- rather than updating thousands of times a second, it moves to 50-100 frames per-second.

   This is fast enough for humans to read it and to act in real-time, and also saves a lot of processing power where it is needed.

   Add to that the function UpdateReady(), which tells you if it is time to do an update, a function can skip building the update altogether
   until it matters.

   With the LCD function, setting it to FastMode() does exactly that -- the functions are identical in that they call the LCD update for each iteration from 0-1,000,000
   In fast mode, however, the LCD function only builds and updates the display every 10-20ms, allowing the loop to run thousands of times faster since it doesn't have to 
   update every time.

   The loop itself doesn't know or care, so it just blindly calls the LCD update function, while the LCD Update function takes care of the timing.

   A subsequent LastUpdate() tells the LCD function to update anything that might be left over. 

-----------------------------------------
SageBox Features Outlined in this program
-----------------------------------------

The best way to see how SageBox works is to look at the code.  This is a small program and shows many SageBox functions.
However, here are some specific things to look for in this program:

1. Efficient, Fast Updating vs. Updating Every Iteration

     See notes above about Fast mode button vs. regular button.  SageBox has the functionality to enable very fast processing with changing
     how the Update() function is viewed as well as determining whether there is a need for an update or building an output.  In this program, these
     are best seen in the Fast Mode vs. regular mode usage of the LCD widget, outlined above.

2. Changing a button's title and color

     In this program, the top button's color and title are changed to re-purpose the button as a stop button.
     An alternative method is to create two buttons, and Show() and Hide() whichever one is active.

     In this case, changing the title and color of the button was used.

3. Button Signals and Button ClearEvents()

     Button Signals (and Signals, in general) are really nothing but a boolean set when a button is pressed or a control is used.  
     In some cases (such as a Checkbox, or Edit Box), a Signal can 
     also fill another pointer with information about the control (i.e. whether checked or not, or the text in an edit box).

     Signals allow for two things:

       1. Abstraction -- control of processes without the process itself knowing or caring if a GUI is present.  Since a boolean is filled in the button press,
          a process can just look at the boolean, which can be passed in by whoever calls it. 

          This allows fast prototyping and development, without the need to either insert GUI elements into a lower-level process, or to 
          overload element to abstract incoming data away from the GUI.
          
          Signals are more advanced, and should be used with care.  In development, for example, Signals can be used as a temporary and quick way to 
          deal with functions, without creating a structure around it to deal with controls being changed.  In many cases, these would be temporary and
          later replaced with a different structure.
          
          However, used with care, Signals can be very useful over the lifetime of a process or program, especially when the same Signal is used
          for multiple controls -- more about that in another document.

       2. Speed.  Signals allow for greater speed by allowing a process to check a single boolean rather than making a call to determine a 
          control's status.  For example, in this program, rather than calling Button::Pressed(), which calls into the SageBox core to get the 
          button status, the status check is handled with basically one instruction that retrieves the boolean directly.

4. Using CPoint functionality

   CPoint is an OO-based structure similar to POINT, also similar to MSVC CPoint.  CPoint allows math operations on the POINT pair, giving
   flexibility and brevity to code using the CPoint structure.  CPoint translates automatically to a POINT (and vice-versa) as necessary.
