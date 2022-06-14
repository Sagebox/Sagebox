// CCounter.Cpp -- Copyright(c) 2020, 2021 Rob Nelson.  robnelsonxx2@gmail.com -- all rights reserved.
// This file, information, and process within are for personal use only and may not be distributed without permission.
// Please modify, copy, do whatever you want for personal uses.  For professional, distribution or commercial uses, 
// contact the e-mail address above

// *****************************************************
// CCounter.Cpp -- SageBox Neural Network 7-Bit Counter
// *****************************************************
//
// note:    see "Original notes for the Console Version (before SageBox additions)" below, for information the workings of the Neural
//          Network code itself.
//
//
// This program shows two things: 
//
//      1. CDevNN, a neural-network class training a Seven-Bit Counter (read the notes below)
//      2. Sagebox showing resal-time results of training the Neural Network
//         This is discussed here, where the Neural-Network specifics are discussed below. 
//
// SageBox allows the results of the neural network output to be seen realtime.  With the console version, it can print out results in real-time also, but it cannot
// show the graphing and output matrix that SageBox provides.
//
// The 128 bar graphs represent the 128 outputs from training and show them simultaneously.
// This allows us to see how the neural network is being trained, which can show what is happening in the neural network.
//
// In this case, there are a small set of nodes being used, and it is very interesting to see how the neural network trains many of the 
// values in sync with each other. 
//
// The output matrix, shows the output value for the 128 results (the bar graph shows the accuracy of each training set of node inputs in realtime),
// giving a sense of where the training is at any given time.
//
// The Epoch, training rate, and other values are also shown in realtime, as well as whether the neural network is converging or diverging.
//

// -----------------------------------------
// SageBox Features Outlined in this program
// -----------------------------------------
//
// 1. Adapting existing code with little or no changes & footprint
//
//      The source code for this program was taken from a console-mode program and has been changed
//      very little from the original (see notes in the TrainUpdate() function where additions are made at the bottom of the function).
//
//      One of SageBox's main paradigms is to allow procedural, non-event programs to use Windows, GUI-Based functionality without 
//      changing the structure of the code.  This allows additions to code with little or not footprint.
//
//      SageBox code can be compiled out with a switch and/or removed easily.  Also, Sagebox's design allows
//      for methods to access SageBox functionality without core routines knowing of its existence, keeping 
//      pure functions pure. 
//
// 5. Using Text Widgets
//
//      This program uses text widgets to show the real-time information to the screen, such as Epoch #, learning-rate, etc. 
//      How to create text widgets and update them are shown, as well as using the Transparent() flag to allow the 
//      Text to blend into the background since we're using a texture rather than a plain background (where it could just use a solid color)
//      Transparent() tells the Text Widget to copy the background.
//
//      With Text Widgets, you do not have to manage the output -- the background and output is updated on every write, so there is no reason
//      to worry about overwriting.  Also, if a justification attribute is used (i.e. Center()), then the new write will recalculate the correct position.
//
//      Though now shown in this program, TextWidgets can also be fire-and-forget (i.e. you don't need to remember the object pointer) in cases where they are just labels.
//
// 6. Using Child Windows
//
//      This program shows how to use Child Windows, which are embedded in the parent window.  
//      Child windows are otherwise exactly the same as Popup windows createed with NewWindow(),
//      except that thay may also have a border.
//
//      See the code for more details.
//
// 7. Using backdrop textures and CLS Bitmaps
//
//      With many applications, clearing the screen with a solid color or gradient works just fine, and SageBox remembers the colors when you
//      clear the screen with Cls().
//
//      In this program a texure is used (see ReadPGRBitmap()) and then set as the CLS background -- when the program then performs a Cls() elsewhere, it doesn't
//      need to know or care about the texture -- SageBox automatically reprints the texture rather than clearing the window with a color.
//
//      See InitUI() for more information
//
// 8. ReadPgBitmap()
//
//      ReadPgrBitmap() is an easy way to get a verified .BMP or .JPG image from a memory structure.  a PGR is a Portable Graphics Resource that
//      are compiled from a text file with the name of the file, path, and can contain multiple files, directives, settings and so-forth to 
//      completely control an object or environment.
//
//      In this case, there is a jpeg called "texture" which ReadPgrBitmaps() reads automatically from the PGR that has been compiled as a memory structure.
//
// 9. Using {}-style commands in text strings to set colors, fonts, etc.
//
//      To make development easier, SageBox supports embeded colors, fonts, and other directives in text sent to Widgets and functions like write() and printf().
//
//      SageBox has many stock colors, such as "Red", "blue", etc.  In this program "cyan" is used.  For example, expressing Write("This is {cyan}different{/} color") puts the 
//      word "different" in cyan.  You can also use MakeColor("MyColor") and then used {MyColor} in its place to put the color in text.  Fonts may also be used, as well as 
//      text placement for aligning multiple lines of text. 
//
// 10. Using quick dialog functions (dialog.Info())
//
//      This program shows the "dialog" set of functions in SageBox.  In this case, info.dialog() is used to put up an info dialog to notify the user that the 
//      program is complete.
//
// 11. Setting up and Using Fire-and-Forget Menus
//
//      Menus are used in the this program, but the menu object itself is not kept.  The menu object only needs to be kept if future
//      operations will be done on the menu (adding or removing items, check marks, etc.)
//
//      In this case, the menus are used to look for an Exit() and About() selection in the menus
//
// 12. Setting a CloseButton Menu Status
//
//      Also shown is a method to make the "Exit" menu button press the Close Button so it does not need to be checked.
//
// 13. About Window
//
//      This program uses a function that uses a class called CSimpleDoc to draw an image to the screen that is compatible with 
//      HTML editors (i.e. it is a very lmiited version of HTML to allow for structured placement and editing in an HTML editor)
//
//      This function takes one line, as the html file, backdrop image, and other information is located in a .PGR file, which is 
//      compiled from a simple text file that gives it the information it needs. 
//
//      CSimpleDoc() is still under development.
//

#include "SageBox.h"
#include "CCounter.h"
#include "nn-texture.pgr2.h"
#include "AboutSevenBitCounter.pgr2.h"

#include <vector>

// -----------------------------------------------------------------
// Original notes for the Console Version (before SageBox additions)
// -----------------------------------------------------------------
//
// --> Note: The original version was a 6-Bit counter, but has since been moved to a 7-bit counter.
//
//
// SixBitCounter -- CDevNN.CPP Example
// Rob Nelson, robnelsonxx2@gmail.com
// 08/11/19
//
// This sets up a neural network to form a 6-bit "counter", and is a great example if a neural network training that works in a text-based
// environment. 
// 
// a 3-bit counter is a neural network example seen a lot, so I thought a 6-bit counter might be interesting, showing the neural network working with two
// layers and various activations to complete the training.
// 
// This 6-but counter version accepts 6 inputs set to 0 or 1, corresponding to the bits in a 6-bit value.  The outputs are also 6 bits, with the intention to
// put out the input + 1, rolling over to 0 at 63. 
// 
// The training data is all values between 0-63, all six inputs.  So, 0 would be 0-0-0-0-0-0 (i0-i5, respectively), and 1 would be 0-0-0-0-0-1, all the way up to 63 at
// 1-1-1-1-1-1, with respetive outputs (six) at 0-0-0-0-1, 0-0-0-0-1-0, and 0-0-0-0-0-0, reflecting the binary value of the input + 1 and rollover at 63. 
// 
// It would be interesting to see an incomplete set of training data (say, every other value) given to the neural network, but I haven't tried this out yet.
// 
// I put "counter" in qoutes because it's just fitting curves around the points given that turn out to move from 0-63 and back to 0, and I think the idea of it
// counting is a little misleading -- but, maybe the logic of it is extractable in some way (something I am working on: the idea of coloring local functional
// areas in the neural network into workable sets of functions).
// 
// ---------------------------------------------------------------------
// Two parts of training: predicting and completely training on the data
// ---------------------------------------------------------------------
//
// I think this program is a good example of training a network to "predicting" values vs. completely mapping out the outputs and overtraining.  As with most activation-
// based neural networks, the output can be considered a "0" if the actual output is < .5, and considered a "1" if the output is >= .5.  In this case, I put the threshold at
// .4, where the output is considered accurate if it is within .4 of its expected output (i.e. <.4 is accurate if we want a "0", and >.6 is accurate if we want a "1"). 
// At this point, the idea is that the neural network has a basic form around the parameters and training data we've given it and is given a good guess (an accurate guess),
// i.e. prediction, of the value.
// 
// The network then continues to train and will train (if not stopped) until there is essentially 0 error, in which case it is giving the actual value rather than its
// prediction, which can turn into overtraining problems with neural networks where it won't react well to data it hasn't trained on.  Thinking about it, this would make a 
// great augmentation to this program, looking at non-binary values (i.e. .3, .6, -.4), or training on an incomplete set of data to see what level of training works better than
// others.
// 
// ---------------------
// Structure of the Code
// ---------------------
// 
// This program shows how to use the TrainUpdate() and TrainUpdateForward() to have the neural network perform a forward propagation during training.
// The TrainUpdate() function looks for an accuracy of 100% (based on the .4 threshold), and continues on until the error is much less.  At the end, the
// main part of the program (after Train() has returned) prints out the results.
// 
// During the training, TrainUpdate() will print out the percentage complete (of the .4 threshold), the current error, the learning rate (which changes a lot),
// and the trend (converging or diverging)
// 
// note: Neural networking backpropagation (training) is very CPU-intensive.  Setting up speed optimization and removing some debug elements, such as frame checking,
// can speed up the program 2-3 fold.
//

#define kShift(_i,_n) (double) ((_i >> _n) & 1)     // Simple macro to keep code clean, creating 1 or 0 decimal conversion for bit on/off

// Neural Network Layer Structure for Six Bit Counter
//
// In many ways, the activations for the outputs are arbitrary, though they do make a difference.
// In this case, these are set to values and layers that get results in a decent amount of time, but also show the 
// neural network training.
//
// It is possible to get a result with just one layer and 6,8,10 (somewhere around there) nodes, it just takes longer.
// If you reduce the amount of layers and nodes, you can see the neural network working harder, sometimes never coming to 
// a conclusion.
//
// In this setup, the neural network not only finishes but can basically reduce the output to 0 error.  This is a big difference, and in
// many neural networks, you really don't want zero error, otherwise the idea of it "predicting" or approximating gets thrown out the window
// (i.e. it gets overtrained).
//
// This example shows that it can basically achieve almost zero error, far after it achives 100% accuracy based on a .4 threshold (i.e. any output
// below .4 is accurate if the output is supposed to be 0, and any output > .6 is accurate if the output should be 1), which is the idea of the neural network
// predicting the output, vs. just straight mapping it out functionally, which it does as the error further reduces towards 0. It's fun to watch, though.
//
static CDevNN::stLayerInput_t stLayers[]  = 
{
    CCounterNN::kBits    ,CDevNN::ActType::Input    ,   // one input for each "bit". "Input" is just reminder there is no activation on the input layer.
    12                   ,CDevNN::ActType::Sigmoid  ,   // 12 nodes for first layer, Sigmoid activation
    12                   ,CDevNN::ActType::Sigmoid  ,   // 12 nodes for second laer, Sigmoid activation
    CCounterNN::kBits    ,CDevNN::ActType::Sigmoid  ,   // one output for each "bit". Sigmoid activation, which worked well (surprisingly)
    0                    ,CDevNN::ActType::End      ,   // End of structure
};


// CCounterNN() Constructor -- Really nothing to do here but initialize the CDevNN() object
//
// The constructor tells CDevNN that we won't be using lockable weights (which can be used to shut off weights and nodes).
//
CCounterNN::CCounterNN(unsigned int uiRandSeed,CDevNN::stLayerInput_t * stLayers)  : CDevNN(uiRandSeed,CDevNN::LockEnable::Off,stLayers) {}



// TrainUpdate() -- Show what is going on with the training and run some forward propagation passes to show current training status.
//
// This shows using the TrainUpdateForward() function to perform a foward propagation pass during training.  TrainUpdate() gets called
// on complettion of every "epoch" (foraward() and backward() pass for all data sets) during training.
//
// This also shows an early soft (non-error) exit when the error reaches a certain minimum.
//
cnnErr CCounterNN::TrainUpdate(const char * & sMsg)
{
    static int iUpdateCount     = 0;
    static int iIter            = -1;
    float fThreshold            = .4f;              // Thresold for success -- < .4 = 0, >.4 = 1
    
    double * fInputs,* fOutputs;

    auto cnnReturnStatus = cnnErr::Ok;         // Ok for now, but might return a stop status

    if (++iIter < m_iTrainResolution) return cnnErr::Ok; // Omly process every m_iTrainResolution iterations to keep it faster

    double fError = getError();                     // Get current NN error

    GetUpdateMem(fInputs,fOutputs);                 // Get memory for update inputs and outputs.  
   
    int iCorrect = 0;
    m_cGraphWin->Cls();                             // Clear the Graph Window so we can make a new set of bar charts.

    for (int i=0;i<(1 << kBits);i++)
    {
        // Set the inputs, one binary digit for loop value i
        
        for (int k=0;k<kBits;k++) fInputs[k] = kShift(i,k);
    
        TrainUpdateForward();           // Perform a Forward Propogation for our X,Y point.  

        double fTemp[kBits];
        int j = (i+1) & ((1 << kBits) -1); 

        // set the outputs to compare against, one binary digit for value (i+1, 0 when i+1 == 63)
        
        for (int k=0;k<kBits;k++) fTemp[k] = kShift(j,k);

        int iOutValue = 0;
        double fIndError = 0;

        for (int k=0;k<kBits;k++)
        {
            iOutValue |= (fOutputs[k] >= .5 ? 1 : 0) << k;      // Compile output decimal value
            if ((!fTemp[k] && fOutputs[k] <= fThreshold) || (fTemp[k] && fOutputs[k] >= (1.0-fThreshold))) iCorrect++;
            double fTemp2 = fTemp[k]-fOutputs[k];
            fIndError += (fTemp2*fTemp2)/(double) kBits;
        }

        fIndError = 1.0-sqrt(fIndError);

        // Print a bar for each value if we have less than 50% error

        int iBarHeight = (int) (310.0*(fIndError-.5)*2.0);
        m_cBarGraph.DrawBar(i*7,309,5,iBarHeight,rgbBarGraph[(i & 1)]); // Alternate bar colors

        m_iOutValues[i] = iOutValue;
    }
    m_cGraphWin->Update();

    double fPercent = (double) iCorrect/(double) m_iMaxValues;      // Get percent correct.

    // -------------------------------------------------------------------------------------------------------------------------------------
    // Print out values on to the screen.  This could be a function (such as UpdateValues() or something), but since it's small, I just left
    // it here -- if it grows then it will probably be refactored out to a function call (it's on the edge of needing it)
    // -------------------------------------------------------------------------------------------------------------------------------------

    m_cTextIteration    ->Write(CString() << "Epoch {cyan}" << getEpoch());
    m_cTextPercent      ->Write(CString() << "Percent = {cyan}" << (int) (fPercent*100.0) << "%{/} (" << iCorrect << " out of " << m_iMaxValues << ")");
    m_cTextError        ->Write(CString() << "Error = {cyan}" << fError);
    m_cTextTrend        ->Write(CString() << "Trend: {cyan}" << getTrendString());
    m_cTextLR           ->Write(CString() << "Learning Rate: {cyan}" << getLearningRate());

    // ** Original Console Output before SageBox was used to make give it a Graphic Output **

    printf("[%d] P = %lf (%d/%d) -- LR = %lf, Error = %lf (%s)\n",getEpoch(),fPercent,iCorrect,m_iMaxValues,getLearningRate(),fError,getTrendString());
    
    if (!(iUpdateCount++ % 10)) UpdateValueBox();   // Only update the Values every 10 times, to keep the output faster, since it does a lot of work. 

    auto SetStopMsg = [&](const char * sMessage) { sMsg = sMessage; cnnReturnStatus = cnnErr::Stop; }; // Just a shortcut lamdba to streamline code

    // Stop for three reasons:
    //
    //      1. Abort Button was pressed.
    //      2. Window was closed
    //      3. About menu item was selected.
    //
    // We could also have set up a Signal, in which case we'd only have to check a value, such as m_bAbort or
    // ButtonSignal.GetSignal() to avoid mixing GUI with the function -- but, since we're already putting GUI info out,
    // we don't need to worry about it -- later refactoring might make a Signal more useful to allow the button press
    // and other selections without the function knowing the source.
    //
    // Another way to handle it is to check the event in the SageEvent() overload in the a Message Handler to 
    // use the exact same line as below, but then set an m_bAbort boolean that would be then checked here, eliminating
    // the need to check the actual Window and Button statuses here.
    //
    // But, since this is a demo of development and protyping, it is done here -- For more mature product, this might be 
    // refactored out to as described above.
    //
    // Note the usage of "Peek::Yes" in MenuItemSelected() -- this allows us to check the menu status without clearing the event,
    // so when the program ends up back in the event loop, the event will still be active -- otherwise it would be cleared here and 
    // the GetEvent() loop wouldn't know that the About Menu was selected.

    if (m_cButton->Pressed() || m_cWin->MenuItemSelected(Peek::Yes) || m_cWin->WindowClosing()) SetStopMsg("User Canceled Counter.");

    // If we have a small-enough error, then quit.  This continues long past the 100% mark to zero-on on more of an
    // accurate set of Neural Network values. 

    if (fError < .001) SetStopMsg("Error-threshold went below minimum.");   // add "|| iCorrect >= m_iMaxValues" to exit earlier

    iIter = 0;

    return cnnReturnStatus;
}

// CreateDataMap() -- Create the training inputs and outputs
//
// This creates the training inputs and outputs, based on 6 inputs and 6 outputs, representing their binary values.
// The input is 0-63, and the output is 1-63 and then 0 when it rolls around, i.e. input = X and output = X+1 (rolling over to 0
// at 63).
//
// The values are hardcoded for clarity, but can be change to the class-defined values to change the bit width of the counter.
//
void CCounterNN::CreateDataMap()
{
    // There are 64 data sets, representing 0-63.  An interesting change might be to give the neural network and incomplete set
    // (i.e. random omissions, or every other value, for example) to see what the neural network does with it. 
  
   for (int i=0;i<(1 << kBits);i++)
    {
        int j = (i+1) & ((1 << kBits)-1);       // Output value, i+1 (0 when i = 63)
        for (int k=0;k<kBits;k++)
        {
            m_fTrainInputs[i*kBits+k]   =  kShift(i,k);         // Get decimal inputs, 0 or 1 depending on bit position   
            m_fTrainOutputs[i*kBits+k]  =  kShift(j,k);         // Get corresponding outputs
        }
    } 
}

void CCounterNN::UpdateValueBox()
{
    m_cResultsWin->Cls();   // Clear the screen (in this case, reprints the background bitmap)

    // Print out input and output -- print output in RED of it is not correct, or GREEN if correct.
    // Print multiple columns (using SetIndent() to make it easy)

    for (int i=0;i<m_iDataSets;i++)
    {
        if (!(i % 16))  // Print 16 rows, by however many columns needed (8, in the case of the 7-bit counter)
        {
            m_cResultsWin->SetWritePos(0 ,10);                  // Put the write position at the top of the window (10 pixels down)
            m_cResultsWin->console.SetIndent(15+110 *(i/16));   // Set new <CR> position so we an print multiple lines using \n
        }
        bool bCorrect = m_iOutValues[i] == ((i + 1) & ((1 << kBits)-1));    // Correct = value+1 (or 0 if overflowed)
         
        m_cResultsWin->printf("In = {cyan}%.2d{/} -- out = %s%.2d{/}\n",i,bCorrect ? sColorCorrect : sColorWrong,m_iOutValues[i]);
    }
    m_cResultsWin->Update();    // Update the window since we turned AutoUpdate() off
}

void CCounterNN::InitUI()
{
    cwfOpt cwOpt;
    cwOpt << Font("Arial,14") | Transparent();  // Set options for the Text Widgets
                                                // i.e. Font Size and Transparent (i.e. copy background of parent window)

    // Put out the Text Widgets that display real-time information. 
    // The values (930,50) we're just eyeballed after the graph window was created.

    m_cTextIteration    = &m_cWin->TextWidget(930,50+22*0,260,0,cwOpt);
    m_cTextError        = &m_cWin->TextWidget(930,50+22*1,260,0,cwOpt);
    m_cTextPercent      = &m_cWin->TextWidget(930,50+22*2,260,0,cwOpt);
    m_cTextTrend        = &m_cWin->TextWidget(930,50+22*3,260,0,cwOpt);
    m_cTextLR           = &m_cWin->TextWidget(930,50+22*4,260,0,cwOpt);

    // Create the Bar Graph Window -- Add a border and shut off Automatic Updating (default)
    //                                to avoid flickering since it is erased and re-printed in realtime (same for the Results Window)

    m_cGraphWin = &m_cWin->ChildWindow(kWinX,kWinY,kWinWidth,kWinHeight,AddBorder() | NoAutoUpdate()); 
    m_cGraphWin->Cls(krgbGraphWin1,krgbGraphWin2);
    
    // Create the Text Results Window -- In this case, make it transparent (i.e. copy the parent window bitmap; subsequent Cls() calls 
    //                                   will re-display the bitmap to clear the window)
    
    m_cResultsWin = &m_cWin->ChildWindow(kWinX,kWinY+kWinHeight+50,900,240,Font("Arial,11") | NoAutoUpdate() | Transparent() | AddBorder());
    m_cResultsWin->SetAutoScroll(WindowScroll::Disabled);   // Disable in case we print too close to the bottom and it wants to scroll the window

    // Create the labels for the two windows we just created (Bar Graph and text Results Window

    m_cWin->TextWidget(kWinX,kWinY+kWinHeight+10,"Bar Graph of Each Input (composite of percent correct for N-Bits for each byte-wise output, 0-127)",Font("Arial,13") | Transparent());
    m_cWin->TextWidget(kWinX,kWinY+kWinHeight+50+240+10,"Current Numeric/Value Results of Input vs. Output ",Font("Arial,13") | Transparent());

    m_cButton = &m_cWin->NewButton(930,200,150,"Stop Counter",Style("black"));      // Set specific length just to make it wide.  We can also use spaces in the title.
                                                                                    // Set it as "black" -- otherwise, it defaults to a dark gray but it clashed 
                                                                                    // with the blue background and black looked better.
    
    m_cBarGraph.Init(*m_cGraphWin);                                                 // Lastly, Initialize the bar graph with the Bar Graph Window
}

CString CCounterNN::RunNeuralNetwork()
{
    CreateDataMap();                        // Create the traning inputs and outputs.

    // Set up the learning rate values and automatic status.  These vary for every neural network.

    SetLearnRate(.0001);                    // Set initial learning rate
    SetRateChangeBounds(.0035/4,0,0);       // Set minimum learning rate (ignore other parameters)
    SetRateChange(true,.005/4,.1,3.5);      // Allow automatic learning rate, and set divergence change magnitude and max learning rate.
    
    auto eStatus = Train(m_iMaxEpochs,m_iDataSets,m_fTrainInputs,m_fTrainOutputs);  // All the action takes place in TrainUpdate()

    // Check status so we can print an appropriate message.
    // Returns are cnnErr, cnnStop (TrainUdate() or user aborted), or cnnOk

    CString cMsg;   // Used for the return output.  We can sprintf() into it up to 300 chars (more
                    // if we allocate a known size on instantiation (i.e. CString cMsg(BufferSize))

    const char * sMsg;

    if (eStatus == cnnErr::Err) sprintf(cMsg,"Returned with Error: %s\n",GetErrorMsg());
    else if (eStatus == cnnErr::Stop) 
        (sMsg = GetErrorMsg()) ? sprintf(cMsg,"Done.\n%s\n",sMsg) : printf("Done.  Stopped by user or automatic process.\n");
    else sprintf(cMsg,"Train() Finished ok.\n");
    
    UpdateValueBox();   // Update values one more time just to be complete.

    return cMsg;        // Return the message to display to the user.
}

// Go() -- Initialize the data, set learning rate values/boundaries, and run the neural network.
//
void CCounterNN::Go(CWindow & cWin)
{ 
    m_cWin = &cWin;
    InitUI();
   
    // Enter the event loop, waiting for the start button press or a window close ("X" button on the window)
    // If the window is closing, simply return.  GetEvent() returns false when the window is closign
    // (cWin.DisableClose() -- or opt::NoClose() on creation) will disable the window from closing when the
    // user presses the 'X' button on the window.

    auto cMsg = RunNeuralNetwork();         // Run the Neural Network
                                            // This is basically the same body used for the console mode version 
                                            // without the graphics.

    // Put up a Info Dialog.  CenterWin() centers the dialog in the current window (otherwise it is 
    // centered in the screen.  Hover the mouse over "CenterWin" for details.

    cWin.dialog.InfoWindow(CString () << "7-Bit Counter Finished\n" << *cMsg,opt::CenterWin());
}

// main() -- Create neural network object and run it.
//
int CCounterNN::main()
{
 
    // Get quick bitmap from a PGR container file (in this case stored as a .JPEG)
    // This can be used instead of opening the PGR container and extracting 
    // the file manually.

    auto cBitmap = Sagebox::ReadPgrBitmap("Texture",sCounterTexture); 

    // Create the Main Window for the 7-bit counter within the window that was passed to this function.

    auto& cWin = SageBox::NewWindow(10,10,1200,690,"SageBox - Neural Network Example: 7-Bit Counter");

    // ------------
    // Add the menu
    // ------------
    //
    // Note that we put none of this in the main class, as we only need it here, including 
    // looking for the About Event -- the internals of the Counter (while counting) look at 
    // whether or not a menu item was selected, but it doesn't know or care which one, so it doesn't
    // need any of the information below. 

    // Since we only have two menu items, regular values are used here.  if this were much larger, these
    // would be in an enum class.

    static constexpr int kMenuItemExit     = 100;           // Exit program (set to automatically press Close Window Button)
    static constexpr int kMenuItemAbout    = 101;           // Launch About Window

    cWin.CreateMenu("&Exit=100,&About");
    cWin.DisplayBitmap(0,0,cBitmap);        // Display the bitmap we loaded from the PGR container
                                            // The window gets updated when the sub-windows are created.

    // Get our 7-bit counter in a container so we don't need to delete it. 
    // CounterNN is too big to put on the stack.

    Obj<CCounterNN> cCounterNN = new CCounterNN(1234,stLayers); // Give it a random number for the neural network, and the NN layer 
                                                                // description.
    cCounterNN->Go(cWin);

    while (cWin.GetEvent())
    {
        // If we get a menu item and it's the About Menu Item, show the About Window 
        //    (even the we don't need to check the exit, we can still receive it, so we need to differentiate it from the About item)

        int iMenuItem;
        if (cWin.MenuItemSelected(iMenuItem))
            if (iMenuItem == kMenuItemAbout) cWin.DrawSimpleDoc(sAbout,Modal() | CenterXY());   // Options: Modal()    -- this keeps the other windows from being used until the
                                                                                                //                        About Window is dismissed by the user.
                                                                                                //          CenterXY() -- This centers the About Window on the screen.  Otherwise it
                                                                                                //                        is more in the upper-left part of the screen.
    }

    return 0;
}

