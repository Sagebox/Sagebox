***********************************
Multiple Color Wheel Widget Example
***********************************

This program is an example of using multiple widgets, in this case 2 or 3 instantiations of the Color Wheel Widget.

It demonstrates how to use multiple widgets, even when they are the same Widget. 

----------------
2-Widget Version
----------------

 This version shows using the two widgets.  As the colors in the Widgets are changed, this is drawn as
 Gradients from BLACK->(color) in each respective rectangular area.

----------------
3-Widget Version
----------------

 This version shows using the three widgets.  As in the 2-Widget version, 2 of the Color Wheels control the two rectangles, filling
 them with a gradient from BLACK to the selected color.

 In this version, we have three widgets and three rectangles

--------------------------------------
SageBox Features Shown in This Example
--------------------------------------

 1. NoAutoUpdate() and Update()

   Many programs never need an update because windows start with the Auto Update Status set to TRUE.
   Since the image is changing so much, we want to update it ourselves to avoid flashing:

     Since we're putting out three rectangles at a time, if we auto-update, the auto-update
     could decide to update the window between rectangles, which could cause the display to 
     look irregular.

     We want to make sure the screen updates after the rectgangle are drawn to the window (which is drawn to a bitmap, and
     sent to the window when it updates)

     AutoUpdate (which can be specified in the Window Creation or as a SetAutoUpdate() function) is set to false, meaning the window
     will only update when we want.

     Update(). This tells the window to update.  Update(<milliseconds>) can be used to tell the window to only update
     every <milliseconds> (i.e. Update(10)). 

   Most of the time, we don't need to Update ourselves, but when displaying multiple items in a loop, setting NoAutoUpdate()
   and updating ourselves makes the update much more uniform and avoids flashing and shearing when updates hit while we're drawing the
   window.
