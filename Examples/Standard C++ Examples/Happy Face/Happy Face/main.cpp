
// ----------
// Happy Face 
// ----------
//
// This is just a simple program to draw a happy face.
//
// It's main point is to show that using graphics primitives (rectangles, circles, etc.) with Sagebox
// is pretty much just as easy in C++ as it used to be basic when it used to be more fun, without needing to understand
// event-driven programming, the GUI os, etc.
//
// It shows the overall environment in Sagebox that allows functions to be put together easily and an in ad-hoc manner.
//
// The keywords make it even easier.  Type kw:: on the source code to show all available keywords. 
//
// Some notes:
//
// - The Write() function can use embedded nomenclature, such as setting the font or color.
//   In this example, "{65}" is used to set the font in the second Write() example, setting the font to
//   an "arial,65" font.  Font(65) could also be used as a keyword function.  With a keyword function, you can set a more 
//   specific font, such as Font("Times New Roman,65,bold,italic)"
//
//   Although not shown here, colors can be embedded, too, such as "{green}" (or "{g}" for short)
//   using "{}" to end the color, such as "this is {g}text is in green{}, and this is now back to the original color"
//
// - PenColor("blue,green") causes the border (with PenSize(10)) to draw as a gradient from blue to green.
//   the pen gradient angle can be changed, as with PenGradientAngle(45), etc.
// 
//   To draw a gradient in the circle, just specify two colors, such as "yellow,green" instead of the "yellow"
//   used in the program below.
//
// - Text colors, e.g. "red".  In this example, text strings are used for the colors.  This can make programming in an ad-hoc
//   or casual manner more interesting and easier.  Since C++ is largely a symbolic language, symblic colors can be used, such as
//   PanColor::Red, PanColor::Black, etc.  
// 
//   Symbolic colors use no time in parsing the text (that is, it's faster processing) and detects mistakes at run-time/
//   For example, mispelling "green" as "gren" or "greeen" would not be dectected until it was seen by a user at run-time;
//   where mispelling PanColor::Green and PanColor::Gren would be detected the second you typed it, causing a red underling to appear
//   even before you compiled.  Also, if you compiled it, the compiler would let you know it was mispelled.
// 
// - DrawArc() is just a simple arc function.  The "-150,120" represents the "start angle" and "sweep angle" of the arc
//   LineCaps(LineCapType::RoundAnchor) puts 'caps' on the beginning and end of the arc.   
// 
//   Line Caps can also be expressed as string, such as "rounc anchor" vs. LineCapType::RoundAnchor
// 
//   Different caps can be used such as "round" (default), "square", "arrow", "arrow anchor", 
//   etc. "flat" will remove the caps.
//
// - ExitButton().  Since the program does not loop, the program would end and the window would close as soon 
//   as the program ended.  ExitButton() puts up a message in a box for the user explaining that the program is finished,
//   and also prevents the program from ending, keeping the window open util the user pressed "ok"
//
//   There are a few other ways to keep the window from closing, such as window.WaitforClose(), etc.

#include "SageBox.h"
using namespace kw;     // Use keyword space, so we can type things like PenColor() vs kw::PenColor(), etc. for our keyword options.

int main()
{
    auto& win = Sagebox::NewWindow(bgColor("black,midblue"));

    // JustCenter() causes the text to be centered horizontally. 

    win.Write(0,50,"Remember when programming with Graphics in C/C++\nwas easy and fun?",JustCenterX() + Font(40)); // Can also use {40}. Write() below. 
    
    // PenColor("blue,green") draws a blue-green gradient in the border of the circle (i.e. the "pen"))

    win.FillCircle(590, 405,180,"yellow",(PenSize(10), PenColor("blue,green")));
    win.FillCircle(510, 360,30,"black",PenSize(5));  
    win.FillCircle(670, 360,30,"black",PenSize(5)); 
    win.DrawArc(590,410      ,100,100,150,-120,"black",(PenSize(10), LineCaps(LineCapType::RoundAnchor)));
 
    // {65} sets an "arial,65" font.  The keyword function Font(65) can be added after JustCenter() instead.
    
    win.Write(0,650,"{65}Sagebox does.",JustCenterX()); 

    return Sagebox::ExitButton();   // Since we're not in a loop waiting for something, this shows the user the program is over.
                                    // Otherwise, the program would and and the window close immediately.
}