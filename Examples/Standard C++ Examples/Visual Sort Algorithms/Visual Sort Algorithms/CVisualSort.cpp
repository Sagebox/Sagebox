// CVisualSort.Cpp -- Copyright(c) 2020, 2021 Rob Nelson.  robnelsonxx2@gmail.com -- all rights reserved.
// This file, information, and process within are for personal use only and may not be distributed without permission.
// Please modify, copy, do whatever you want for personal uses.  For professional, distribution or commercial uses, 
// contact the e-mail address above

// -----------------------------------------------------------------
// CVisualSort.Cpp -- SageBox Visual Sorting Algorithm Demonstration
// -----------------------------------------------------------------
//
// This program uses SageBox to display various sorting algorithms.  These algorithms were taken directly from sources on the Internet (CodeProject, I think) 
// copied without changes into the SageBox program.
//
// The code was then put into a class where an insert was made to draw a graph of the current output.  This insert is passive and, when not defined, causes the
// sorting routines to draw as normal.
//
// The classes were built to support the drawing routine, but otherwise the sorting algorithms are completely untouched.
//
// -----------------
// Using the Program
// -----------------
//
// The program shows graphically how various sorting routines work. Try lowering the threshold value (which determines how how often the graph draw,
// with slower  values drawing more).  This slows down the display and makes it even more interesting to watch sorting algorithms do their work.
//
// On the bottom of the window are two triangles, showing where the sorting algorithm is in its progress in sorting.  Al of the sorting algorithms
// use a left and right setting and sort within that field.  The triangles shows the location of the left and right indexes, and often appears to be
// moving entire sections left or right. 
//
// -----------------------------------------
// SageBox Features Outlined in this program
// -----------------------------------------
//
// 1. Adapting existing code with little or no changes & footprint
//
//      The source code for this program was taken from a console-mode program and has been changed
//      very little from the original.
//
//      One of SageBox's main paradigms is to allow procedural, non-event programs to use Windows, GUI-Based functionality without 
//      changing the structure of the code.  This allows additions to code with little or not footprint.
//
//      SageBox code can be compiled out with a switch and/or removed easily.  Also, Sagebox's design allows
//      for methods to access SageBox functionality without core routines knowing of its existence, keeping 
//      pure functions pure. 
//
// 2. Using SageBox in a Console-Mode Program
//
//      While SageBox is a provider of GUI functions, it started as a way to develop and emulate rapidly low-level code,  which meant
//      using existing code without changing it.
//
//      This program is a console-mode program that just happens to use a GUI to show what is happening in realtime.  The GUI elements are easily
//      compiled-in for testing and compiled-out for a release that can be used as a regular .OBJ or .LIB file.
//
//      This file can be converted to a Windows program by selecting one of the Windows project settings, though all output currently going to 
//      the console window will be muted since the console window will not exist. 
//
// 3. Checking whether or not we're a Console Mode program or Windows Program
//
//      In this program, there is a call to CSageBox::isConsoleApp(), which tells the program if its in a console mode or if it is a Windows app.
//      In the function DrawGraph(), and the stop button is pressed, the output to tell the user it has received the message is diferent depending 
//      on the isConsoleApp() result.
//
//      If the program is a Console App, it prints the message via std::printf().  If it is a windows app, it displays an Info dialog message since there
//      is no console mode window.
//
//      note: When this is a Windows app, printf()s are ignored and the information printed is not seen.
//
// 4. Using the console windows as a permanent debug window that can be turned off.
//
//      One of the ways to develop a Windows program is to start it as a console mode and then move it to windows.
//      This provides a great way to keep testing output in your program without altering your window program:
//
//      Just use the console window.  When the program is set as a console mode program, all testing and diagnostic output
//      will be printed to the console mode windows.
//
//      When the program is a Windows program, the output goes nowhere and does not show anywhere in the Windows program.
//      Plus, the time to print out to the window is now neglible since printf() should just return without processing
//      ($$note$$ I don't know this for sure, as I haven't trace it down).
//
// 5. Using Child Windows
//
//      This program shows how to use Child Windows, which are embedded in the parent window.  
//      Child windows are otherwise exactly the same as Popup windows createed with NewWindow(),
//      except that thay may also have a border.
//
//      See the code for more details.
//
// 6. Using backdrop textures and CLS Bitmaps
//
//      With many applications, clearing the screen with a solid color or gradient works just fine, and SageBox remembers the colors when you
//      clear the screen with Cls().
//
//      In this program a texure is used (see ReadPGRBitmap()) and then set as the CLS background -- when the program then performs a Cls() elsewhere, it doesn't
//      need to know or care about the texture -- SageBox automatically reprints the texture rather than clearing the window with a color.
//
//      See InitUI() for more information
//
// 7. ReadPgBitmap()
//
//      ReadPgrBitmap() is an easy way to get a verified .BMP or .JPG image from a memory structure.  a PGR is a Portable Graphics Resource that
//      are compiled from a text file with the name of the file, path, and can contain multiple files, directives, settings and so-forth to 
//      completely control an object or environment.
//
//      In this case, there is a jpeg called "texture" which ReadPgrBitmaps() reads automatically from the PGR that has been compiled as a memory structure.
//
// 8. Using the PleaseWaitWindow() functionality
//
//      When the close window button is pressed, the PleaseWaitWindow() is used when this is a Windows program to put up a message that the window will close when
//      the sort is done.  
//
//      note that the PleaseWaitWindow() can be used in the same wait by using cSageBox.PleaseWaitWindow() even in console mode.  It is used here in windows-mode only
//      for two reasons:  1. As a demonstration, and 2. the PleaseWaitWindow() overlaps the main window.  In a Console mode, it's put out to the console mode window in order
//      to put out the message about the pending close but without the PleaseWaitWindow() obscuring the view.
//
//      In the Windows version, there is no console mode window so the PleaseWaitWindow() is used.  Although it could be used in Console Mode as well, it was
//      a better fit to just print out to the console mode window when possible.
//
// 9. Setting up and Using Fire-and-Forget Menus
//
//      Menus are used in the this program, but the menu object itself is not kept.  The menu object only needs to be kept if future
//      operations will be done on the menu (adding or removing items, check marks, etc.)
//
//      In this case, the menus are used to look for an Exit() and About() selection in the menus
//
// 10. Setting a CloseButton Menu Status
//
//      Also shown is a method to make the "Exit" menu button press the Close Button so it does not need to be checked.
//
// 11. About Window
//
//      This program uses a function that uses a class called CSimpleDoc to draw an image to the screen that is compatible with 
//      HTML editors (i.e. it is a very lmiited version of HTML to allow for structured placement and editing in an HTML editor)
//
//      This function takes one line, as the html file, backdrop image, and other information is located in a .PGR file, which is 
//      compiled from a simple text file that gives it the information it needs. 
//
//      CSimpleDoc() is still under development.
//

#include "CVisualSort.h"
#include "sort-texture.pgr2.h"
#include "AboutSorting.pgr2.h"

// InitDataArray() -- Fill the random array with unique values, 0-(kSortSize-1)
//
// This fills the array with values from 0-(kSortSize-1) in an array of kSortSize.
// Each value (0-(kSortSize-1)) is only used one time, so that we can sort 
// through the kSortSize array and end up with 0-(kSortSize-1) sequentially
//
void CVisualSort::InitDataArray()
{
    m_ipFilled.ClearMem();            
    for (int i=0;i<kSortSize;i++) 
    {
        int iValue = rand() % kSortSize;
        while(m_ipFilled[iValue]) ++iValue %= kSortSize;
        m_ipFilled[iValue] = true;
        m_ipArray[i] = iValue;
    }
};

// DrawGraph() -- Draw the main graph of the current state of the sort. 
//
// This function is called indirectly through the Sort functions Update() call.
// Update() can also be null in the sort function
//
// DrawGraph() draws two triangles underneath the graph, representing LEFT and RIGHT index
// of the sort algorithm, which can show how the algorithm is working -- especiall if the slider is used to make
// the threshold a small number.

// This function also looks at events, since it is the main running component.  This allows us to avoid
// using overloads and an event-driven structure to handle messages. 
// 
// If we want to isolate DrawGraph away from the UI, we could use an event-driven structure or Signal used in 
// SageBox to prompt DrawGraph() into action without knowing about the UI. 
//
// In this case, we just let DrawGraph look at the state of things and decide what to do. 
//
void CVisualSort::DrawGraph(int a,int b)
{
    m_iThreshold = m_cSliderThreshold->GetPos();        // Set the threshold to the value of the slider, so as it moves we just get it here.
    
    // Check the menu item for the About Window and launch the window if it has been selected.
    // The other menu item (Exit) was set in the InitWindow() function to emulate the Close Button press, so it is automatic
    // (i.e. when it is selected, CloseButtonPressed() will come back true as an event.

    // If we get a menu item and it's the About Menu Item, show the About Window 
    //    (even the we don't need to check the exit, we can still receive it, so we need to differentiate it from the About item)

    int iMenuItem; 
    if (m_cWin->MenuItemSelected(iMenuItem))
        if (iMenuItem == (int) MenuItems::About) m_cWin->DrawSimpleDoc(sAbout,NoClose());       // Display the HTML file and Backdrop. 
                                                                                                // Option: NoClose() tells CSimpleDoc() to 
                                                                                                // Show the window and return immediately without waiting for 
                                                                                                // a Close Button pressed. 
                                                                                                //
                                                                                                // This allows the About Window to open while the sorting display 
                                                                                                // can continue unimpeded.

    // If the close button was pressed, acknowledge that we received it. by printing to the console window.
    //
    // The sort finishes before the program exits becuase we don't return an abort signal to the 
    // sort functions -- this could be added, but its not a big deal for our purposes here.
    //
    // In this case, we could just bump the threshold to some very large number (i.e. 500,000) to make the sort
    // finished almost instantaneously. 
    
    // Windows vs. Console Mode.
    //
    // If we're in Windows mode, the console is not available.  Put out a wait window if we're not on console mode/
    // Otherwise, just put out the information to the console window.

    if (m_cWin->CloseButtonPressed())
    {
        CString cs = "Close Window Detected.  Program will exit when current sort is finished\n";
        if (m_bConsoleApp)
        printf(*cs);        // i.e. std::printf()
        else
            m_cWin->dialog.PleaseWaitWindow(cs);
    }

                                                
    if ((++m_iCounter % m_iThreshold)) return;        // return if we haven't reached the number of iterations before drawing a graph, 

    m_cGraphWin->Cls();        // Clear the screen or reprint the bitmap texture, whichever we're using.

    RGBColor_t rgbColor;
    for (int i=0;i<kSortSize;i++) 
    {
        POINT pPoint = { i,kSortSize/2 };

        // If we hit or left or right value, then draw a left or right triangle.

        if (i == a || i == b) 
        {
            rgbColor = (i == a) ?  Sage::RGBColor_t() = { 255,255, 0 }: Sage::RGBColor_t() = { 0,255,255 };
            m_cGraphWin->DrawTriangle(m_pTri[0] + pPoint, m_pTri[1] + pPoint, m_pTri[2] + pPoint,*rgbColor); 
        }

        // Draw the line color of HUE of the index we're at.  
        // We could refer to a pre-built table to save the HSLtoRGB calculation, but we don't need that
        // kind of speed here. 

        CSageTools::HSLtoRGB((double) m_ipArray[i]/kSortSize,1.0,.5,rgbColor);
        m_cGraphWin->DrawLine(i,kSortSize/2-m_ipArray[i]/2,i,kSortSize/2,rgbColor);
    }
    m_cGraphWin->Update();
}

// RunSort() -- Run an invidiual sort, after initilizing the data array and printing the sort name.
//
bool CVisualSort::RunSort(const SortType & stSort)
{
    m_cText->Write(stSort.sTitle);
    printf("Current Sort: %s\n",stSort.sTitle);
    InitDataArray();
    clock_t ctStart = clock();
    (this->*stSort.fSortFunc)();        // Call the actual sort function.

    printf("time = %d ms\n",(int) (clock()-ctStart));
    DrawGraph();                        // One more update to make sure we got them all, since we skipped some.
    return !m_cWin->WindowClosing();    // If the window is closing, then return false.
}

// Go() -- Run all sort functions, displaying the data to the screen.
// This continues until the window is closed (or ^C is pressed in the console window)
//
void CVisualSort::Go()
{
    // Create the sort array, with the name and function.  This was original defined in the 
    // class description, but it works better here.
    //
    // If Sorts are added, then they can just be added here and the will be automatically added to the list of 
    // sorts that run in the main loop

     SortType stSort[] = {  "Merge Sort",       &CVisualSort::MergeSort,
                            "Quick Sort",       &CVisualSort::QuickSort,
                            "Selection Sort",   &CVisualSort::SelectionSort,
                            "Heap Sort",        &CVisualSort::HeapSort,
                            "Shell Sort",       &CVisualSort::ShellSort };

    static constexpr int kNumSorts = sizeof(stSort)/sizeof(stSort[0]);

    // Run until RunSort() signals us that the window close button was pressed.

    for (int i=0;;++i %= kNumSorts) if (!RunSort(stSort[i])) break;
}

// InitWindow() -- Create the Main and GraphWindows, load textures (or clear the window with a gradient), create controls, etc -- 
//                 All the things needed to set up the GUI part of the program.
//
bool CVisualSort::InitWindow(CSageBox & cSageBox)
{
    // Create our main window.  This (and the Graph Window) will scale to the size of kSortSize. 
    // However, the textures are set for a kSortSize of 1000.  
    // Comment-out or remove the SetClsBitmap() lines to remove the textures -- this is why there is also a Cls() with a color
    // for each window, so that we can remove the SetClsBitmap() lines and still have a background color that looks nice. 
    //
    // Note: InnerSize() tells SageBox to create the window at the size specified in the client area (i.e. the display canvas)
    // otherwise, the default is to make the entire window (including the non-client/top bar, etc.) the size specified.

    m_cWin = &cSageBox.NewWindow(600,100,kSortSize+100,kSortSize/2+100,"SageBox -- Sorting Algorithms Visualization",InnerSize());    

    auto cMenu = m_cWin->CreateMenu();                                                  // Create a menu object
    cMenu.AddMenuItem("&Exit",(int) MenuItems::Exit);
    cMenu.AddMenuItem("&About",(int) MenuItems::About);
    cMenu.SetCloseButtonItem((int) MenuItems::Exit);                                    // Using the Exit Menu item will now press the "X" close button on the window
    cMenu.ActivateMenu(true);                                                           // Activation is only needed for the top (main) menu to tell
                                                                                        // Windows to add the menu bar to the window.  There are no sub-menus
                                                                                        // in this menu. note: true option tells ActivateMenu to re-adjust the 
                                                                                        // window client area to the same width & height it had before the menu was added.

    m_cWin->SetClsBitmap(m_cWin->ReadPgrBitmap("Texture",sSortTexture));                // Read the texture in the PGR file convert to an .H file in sort-texture.pgr2.h
                                                                                        // (this just reads a JPEG file and converts it to a bitmap)
    m_cWin->Cls({48,48,48},{0,0,0});                                                    // Set a gradient for the main window so we have it if we don't use the texture

    // Create the Graph Window -- a child window embedded into the parent window (m_cWin)
    //
    // The options specified are as follows:
    //
    //        AddBorder()       -- Adds a border around the window (otherwise the window just looks like a filled or blank rectangle)
    //        CenterX()         -- Center the window in the parent Window in the X plane -- this is why X is set to 0, but not Y in the first two values
    //        NoAutoUpdate()    -- Don't auto update the window, which is the default.  Since clearing the screen and updating, we would otherwise see shearing when
    //                             SageBox auto-updates the screen right after a Cls() by before any data is put out.

    m_cGraphWin = &m_cWin->ChildWindow(0,40,kSortSize,kSortSize/2+kTriangleSize,AddBorder() | CenterX() | NoAutoUpdate());    // Inset Window

    // If we don't have a texture (that also has a label) in the main window, put one as a Text Widget.
    // This looks at the location of the m_cGraph win to set the location right above the left corner of the top. 

    if (m_cWin->GetClsBitmap().isEmpty())
        m_cWin->TextWidget(m_cGraphWin->GetWinLocation().x,10,"Sorting Algorithms Visualization",Transparent() | Font("Arial,22"));

    // Create a Text Widget at the bottom of the graph window.   This allows us to just set the text and not worry about 
    // clearing the previous text, updating the window, etc.
    //
    // The options are:
    //
    //        Font()            -- Make it a larger font ("Arial,18").  The default font is smaller
    //        Transparent()     -- Make the background transparent (i.e. copy the parent window's background).  Since we have a texture or gradient, 
    //                             We want the text to blend in.
    //        CenterX()         -- This centers the Text Widget Window.  We could make the window the size of the parent window, but setting what we need is nicer and gives
    //                             us more control.  This also means we want to center the Text Widget window so the text will be centered.
    //        TextCenterX()     -- This Centers the Text in the window.  The Window is centered, but the window itself is 300 pixels long.  The Text will
    //                             vary, and it won't be exactly 300 pixels wide.  The default would put this text to the left part of the Text Widget Window.
    //                             This tells the TextWidget to center the text within the 300-pixel-wide window, which itself is centered in the main window.
    //                             This will center the text in the main window even though we're writing out to a text widget window.

    m_cText = &m_cWin->TextWidget(0,45+kSortSize/2+15,300,0,Font("Arial,18") | Transparent() | CenterX() | TextCenterX());

    m_cGraphWin->SetClsBitmap(m_cWin->ReadPgrBitmap("Texture2",sSortTexture));    // Set the texture for the Graph Window.  Remove line to remove the texture.

    m_cGraphWin->Cls("black");    // Clear the screen to black so we can use this if we don't use the texture. 

    // Write some text explaining the threshold slider.  "{cyan}" is used twice because the color, when set, is active until the end of the line, 
    // so the next line needs to specify the color again.
    //
    // note: DevText() is just a TextWidget() placed on the QuickDev controls window.  We don't save the returned object because we don't change the text.
    // However, if we saved the returned object, such as auto &cText = &DevText("Some text (or this can be blank)"), we can use cText->Write() later to change the text.
    // Since we don't need it again here, we don't save it and it gets deleted with its parent window.
    //
    // note: As the first use of the Quick Dev Controls Window, this causes the window to open automatically. 

    m_cWin->DevText( "{cyan}Use Threshold Slider to control speed of display. Higher Values\n"
                     "{cyan}speed up the display.");
    
    // Write another informational message in a smaller font and differnet color -- "ly" is short for "lightyellow"
    // note: we can also specify these colors as opt::bgColor("ly") or opt::bgColor{100,200,100} (i.e. an RGB color),
    // but using the {} is nice shorthand -- and we can also use the colors for specific items, such as:
    //
    //        "This {red}text{/} is red and this {yellow}text{/}" to cause only certain text to appear a different color.
    //
    // The closing "{/}" is not needed when it continues to the end of the text line.

    m_cWin->DevText( "{ly}Use the Mouse Wheel on the Slider for finer control",Font("Arial,11,italic"));

    // Set the threshold range between 1-500 --> Lower values slow it down, faster values speed it up.

    m_cSliderThreshold = &m_cWin->DevSlider("Threshold",Range(1,500) | Default(m_iThreshold)); 
    m_cSliderThreshold->SetFineControl();   // Set MouseWheel to move in increments of 1

    // Set the location of the Dev Window outside of the main window. 

    auto cDev = m_cWin->GetDevControlsPtr();        // Get the Dev Window object pointer so we can use it directly.
                                                    // (in this case to set the window location).

    // All little POINT (i.e. CPoint) math.  This gets the main window size, adding its location to get the 
    // location of the lower-right-hand corner of the main window.  
    //
    // The WIDTH of the Quick Dev Window is subtraced to right-align the Dev Window.  Then 40 is added to get 
    // the Dev Window below the main window.
    //
    // The result is a Dev Window that is just below the main window, with both edges aligned in the same column.
    // This is done with CPoint() so we can do it as a mathematical expression.
    //
    // Otherwise, the Dev Window pops up in a place that overlaps the display window, and its nice to get it out of the display.

    CPoint cpSize = CPoint(m_cWin->GetWindowSize(true)) + SIZE{ -cDev->GetWindowSize().cx, 40 } + m_cWin->GetWinLocation();
    cDev->SetLocation(cpSize);

    // We could check for an error here, but when it works once it will always work unless something catastrophic is going on
    // (i.e. some other application took up too much memory), so once it passes testing we don't need to worry about it. 

    return true;
}

bool CVisualSort::InitMem()
{
     m_ipArray  = kSortSize;        // Get the memory for the array
     m_ipFilled = kSortSize;        // Get the memory for our table to insure unique values

    // Return false if we have an error.
    //
    // We don't really need to check here, and this is done mostly for completeness-of-code.
    // As with the InitWidow() function, the idea that we wouldn't get the memory means there is something 
    // going on system-wide and this program crashing due to a bad memory address isn't going to hurt anything.
    //
    // On the other hand, it does help if kSortSize somehow ends up as a out-of-range number (such as negative or huge, which
    // can be checked initially before we allocate), it's always nice to check.
    //
    // Plus, we can set an exception to handle it anyway.  So, we don't need the next line, but it can be nice
    // to get a reality check in larger development than this small program. 

    return !m_ipArray.isEmpty() && !m_ipFilled.isEmpty(); 
}
bool CVisualSort::main(bool bConsoleApp)
{
    // Get SageBox here -- we only allocated it once, so it's either do it here or put it on as a class member, which we really don't
    // need to do -- we can just pass it to InitWindow, and we don't need it once we create the main window.  We could even do a static
    // variable in InitWindow(), but doing it here is more in plain sight.
    //
    // SageBox will go out of scope and delete all Windows, controls, etc. after this function exits, which is why we do it in the main() function.

    CSageBox cSageBox;

    // Use asserts for unreasonable errors, such as InitWindow() or InitMem() failing.
    // Neither of these happen here, but this can be good to know on a larger system doing more
    //
    // But, whenever we're dealing with allocating unknown sizes of memory (which we really do know here, but the
    // program itself doesn't), it's always good to check it for errors so that the structure is there if the program
    // grows larger and this routine is buried somewhere in a program that allocates a lot more memory
    // (i.e. suddenly we want to sort a gigabyte of data instead of just 1000 elements). 

    // The stdAssert mechanism is used all over SageBox and appears in larger examples where there is more going on.  It's mostly here 
    // to show how it works and to deal with any potential errors if the code is altered. 

    stdTry;

    stdAssert(InitWindow(cSageBox),"InitWindow() failed");      // We don't really use these messages, but we can if we want to.
    stdAssert(InitMem(),"InitMem() failed");                    // "stdNoMsg" can also be used in the place of a message.


    m_bConsoleApp = bConsoleApp;                                // See DrawGraph() where this is used to print a message differently
    printf("Quick Sort Visualization\n");
    Go();
    printf("Quick Sort Finished\n");


    stdCatch;       // Catch if we had an error -- the messages entered can be printed out here if we cared, but since these errors will never happen,
                    // we can address it if there ever is a failure (which there wont be) 

    return !bError;     // Return true if there is no error. 
}

