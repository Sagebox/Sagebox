*****************************************************
CCounter.Cpp -- SageBox Neural Network 7-Bit Counter
*****************************************************

note:    see "Original notes for the Console Version (before SageBox additions)" below, for information the workings of the Neural
         Network code itself.


This program shows two things: 

     1. CDevNN, a neural-network class training a Seven-Bit Counter (read the notes below)
     2. Sagebox showing resal-time results of training the Neural Network
        This is discussed here, where the Neural-Network specifics are discussed below. 

SageBox allows the results of the neural network output to be seen realtime.  With the console version, it can print out results in real-time also, but it cannot
show the graphing and output matrix that SageBox provides.

The 128 bar graphs represent the 128 outputs from training and show them simultaneously.
This allows us to see how the neural network is being trained, which can show what is happening in the neural network.

In this case, there are a small set of nodes being used, and it is very interesting to see how the neural network trains many of the 
values in sync with each other. 

The output matrix, shows the output value for the 128 results (the bar graph shows the accuracy of each training set of node inputs in realtime),
giving a sense of where the training is at any given time.

The Epoch, training rate, and other values are also shown in realtime, as well as whether the neural network is converging or diverging.


-----------------------------------------
SageBox Features Outlined in this program
-----------------------------------------

1. Adapting existing code with little or no changes & footprint

     The source code for this program was taken from a console-mode program and has been changed
     very little from the original (see notes in the TrainUpdate() function where additions are made at the bottom of the function).

     One of SageBox's main paradigms is to allow procedural, non-event programs to use Windows, GUI-Based functionality without 
     changing the structure of the code.  This allows additions to code with little or not footprint.

     SageBox code can be compiled out with a switch and/or removed easily.  Also, Sagebox's design allows
     for methods to access SageBox functionality without core routines knowing of its existence, keeping 
     pure functions pure. 

5. Using Text Widgets

     This program uses text widgets to show the real-time information to the screen, such as Epoch #, learning-rate, etc. 
     How to create text widgets and update them are shown, as well as using the Transparent() flag to allow the 
     Text to blend into the background since we're using a texture rather than a plain background (where it could just use a solid color)
     Transparent() tells the Text Widget to copy the background.

     With Text Widgets, you do not have to manage the output -- the background and output is updated on every write, so there is no reason
     to worry about overwriting.  Also, if a justification attribute is used (i.e. Center()), then the new write will recalculate the correct position.

     Though now shown in this program, TextWidgets can also be fire-and-forget (i.e. you don't need to remember the object pointer) in cases where they are just labels.

6. Using Child Windows

     This program shows how to use Child Windows, which are embedded in the parent window.  
     Child windows are otherwise exactly the same as Popup windows createed with NewWindow(),
     except that thay may also have a border.

     See the code for more details.

7. Using backdrop textures and CLS Bitmaps

     With many applications, clearing the screen with a solid color or gradient works just fine, and SageBox remembers the colors when you
     clear the screen with Cls().

     In this program a texure is used (see ReadPGRBitmap()) and then set as the CLS background -- when the program then performs a Cls() elsewhere, it doesn't
     need to know or care about the texture -- SageBox automatically reprints the texture rather than clearing the window with a color.

     See InitUI() for more information

8. ReadPgBitmap()

     ReadPgrBitmap() is an easy way to get a verified .BMP or .JPG image from a memory structure.  a PGR is a Portable Graphics Resource that
     are compiled from a text file with the name of the file, path, and can contain multiple files, directives, settings and so-forth to 
     completely control an object or environment.

     In this case, there is a jpeg called "texture" which ReadPgrBitmaps() reads automatically from the PGR that has been compiled as a memory structure.

9. Using {}-style commands in text strings to set colors, fonts, etc.

     To make development easier, SageBox supports embeded colors, fonts, and other directives in text sent to Widgets and functions like write() and printf().

     SageBox has many stock colors, such as "Red", "blue", etc.  In this program "cyan" is used.  For example, expressing Write("This is {cyan}different{/} color") puts the 
     word "different" in cyan.  You can also use MakeColor("MyColor") and then used {MyColor} in its place to put the color in text.  Fonts may also be used, as well as 
     text placement for aligning multiple lines of text. 

10. Using quick dialog functions (dialog.Info())

     This program shows the "dialog" set of functions in SageBox.  In this case, info.dialog() is used to put up an info dialog to notify the user that the 
     program is complete.

11. Setting up and Using Fire-and-Forget Menus

     Menus are used in the this program, but the menu object itself is not kept.  The menu object only needs to be kept if future
     operations will be done on the menu (adding or removing items, check marks, etc.)

     In this case, the menus are used to look for an Exit() and About() selection in the menus

12. Setting a CloseButton Menu Status

     Also shown is a method to make the "Exit" menu button press the Close Button so it does not need to be checked.

13. About Window

     This program uses a function that uses a class called CSimpleDoc to draw an image to the screen that is compatible with 
     HTML editors (i.e. it is a very lmiited version of HTML to allow for structured placement and editing in an HTML editor)

     This function takes one line, as the html file, backdrop image, and other information is located in a .PGR file, which is 
     compiled from a simple text file that gives it the information it needs. 

     CSimpleDoc() is still under development.


 ------------------------------------------------------ Other Notes -----------------------------------------------------

-----------------------------------------------------------------
Original notes for the Console Version (before SageBox additions)
-----------------------------------------------------------------

--> Note: The original version was a 6-Bit counter, but has since been moved to a 7-bit counter.


SixBitCounter -- CDevNN.CPP Example
Rob Nelson, robnelsonxx2@gmail.com
08/11/19

This sets up a neural network to form a 6-bit "counter", and is a great example if a neural network training that works in a text-based
environment. 

a 3-bit counter is a neural network example seen a lot, so I thought a 6-bit counter might be interesting, showing the neural network working with two
layers and various activations to complete the training.

This 6-but counter version accepts 6 inputs set to 0 or 1, corresponding to the bits in a 6-bit value.  The outputs are also 6 bits, with the intention to
put out the input + 1, rolling over to 0 at 63. 

The training data is all values between 0-63, all six inputs.  So, 0 would be 0-0-0-0-0-0 (i0-i5, respectively), and 1 would be 0-0-0-0-0-1, all the way up to 63 at
1-1-1-1-1-1, with respetive outputs (six) at 0-0-0-0-1, 0-0-0-0-1-0, and 0-0-0-0-0-0, reflecting the binary value of the input + 1 and rollover at 63. 

It would be interesting to see an incomplete set of training data (say, every other value) given to the neural network, but I haven't tried this out yet.

I put "counter" in qoutes because it's just fitting curves around the points given that turn out to move from 0-63 and back to 0, and I think the idea of it
counting is a little misleading -- but, maybe the logic of it is extractable in some way (something I am working on: the idea of coloring local functional
areas in the neural network into workable sets of functions).

---------------------------------------------------------------------
Two parts of training: predicting and completely training on the data
---------------------------------------------------------------------

I think this program is a good example of training a network to "predicting" values vs. completely mapping out the outputs and overtraining.  As with most activation-
based neural networks, the output can be considered a "0" if the actual output is < .5, and considered a "1" if the output is >= .5.  In this case, I put the threshold at
.4, where the output is considered accurate if it is within .4 of its expected output (i.e. <.4 is accurate if we want a "0", and >.6 is accurate if we want a "1"). 
At this point, the idea is that the neural network has a basic form around the parameters and training data we've given it and is given a good guess (an accurate guess),
i.e. prediction, of the value.

The network then continues to train and will train (if not stopped) until there is essentially 0 error, in which case it is giving the actual value rather than its
prediction, which can turn into overtraining problems with neural networks where it won't react well to data it hasn't trained on.  Thinking about it, this would make a 
great augmentation to this program, looking at non-binary values (i.e. .3, .6, -.4), or training on an incomplete set of data to see what level of training works better than
others.

---------------------
Structure of the Code
---------------------

This program shows how to use the TrainUpdate() and TrainUpdateForward() to have the neural network perform a forward propagation during training.
The TrainUpdate() function looks for an accuracy of 100% (based on the .4 threshold), and continues on until the error is much less.  At the end, the
main part of the program (after Train() has returned) prints out the results.

During the training, TrainUpdate() will print out the percentage complete (of the .4 threshold), the current error, the learning rate (which changes a lot),
and the trend (converging or diverging)

note: Neural networking backpropagation (training) is very CPU-intensive.  Setting up speed optimization and removing some debug elements, such as frame checking,
can speed up the program 2-3 fold.

--------------------------------------------------
Neural Network Layer Structure for Six Bit Counter
--------------------------------------------------

In many ways, the activations for the outputs are arbitrary, though they do make a difference.
In this case, these are set to values and layers that get results in a decent amount of time, but also show the 
neural network training.

It is possible to get a result with just one layer and 6,8,10 (somewhere around there) nodes, it just takes longer.
If you reduce the amount of layers and nodes, you can see the neural network working harder, sometimes never coming to 
a conclusion.

In this setup, the neural network not only finishes but can basically reduce the output to 0 error.  This is a big difference, and in
many neural networks, you really don't want zero error, otherwise the idea of it "predicting" or approximating gets thrown out the window
(i.e. it gets overtrained).

This example shows that it can basically achieve almost zero error, far after it achives 100% accuracy based on a .4 threshold (i.e. any output
below .4 is accurate if the output is supposed to be 0, and any output > .6 is accurate if the output should be 1), which is the idea of the neural network
predicting the output, vs. just straight mapping it out functionally, which it does as the error further reduces towards 0. It's fun to watch, though.
