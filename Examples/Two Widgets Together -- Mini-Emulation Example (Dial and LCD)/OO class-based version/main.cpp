// Two Widgets/MiniEmulator Example -- Copyright(c) 2020, 2021 Rob Nelson.  robnelsonxx2@gmail.com -- all rights reserved.
// This file, information, and process within are for personal use only and may not be distributed without permission.
// Please modify, copy, do whatever you want for personal uses.  For professional, distribution or commercial uses, 
// contact the e-mail address above

// ********************************
// Two Widgets/MiniEmulator Example
// ********************************
//
//  This program shows the use of the Dial and LCD widgets working together, forming the basis for emulating hardware.
//  Believe it or not, this program (the OO version, not the monlithic version contained in main()) is the basis for 
//  real emulation.  Only certain parts need to be overladed (the virtual functions) to talk to real hardware.
//
//  The Emulation Premise
//
//  The premise of the emulator is a simple local temperature dial and a remote, embedded device that receives the data (and has an LCD display).
//
//  The temperature dial could be for a wall heater, or a pool heater (a real situation where SageBox has been used), or another device.
//  The LCD display can be a real LCD display across a yard or courtyard to the actual heater, displaying real-time data.
//
// Most emulators actually start off this small, expanding over time into something much larger.
//
// -----------------------------------------
// The PGR File - Portable Graphics Resource
// -----------------------------------------
//
//  The PGR file is used in many SageBox programs, mostly to provide a background texture.  In this program, it is used for more.
//  It contains the background, but also information where to put things. 
//
//  This allows the emulator to have many different configurations with different hardware without changing code: all controlled and defined
//  by the PGR file.  Any number of PGR files can be used to direct where to put objects, and which ones to use and not use.
//
//  The PGR file itself is compiled from a very simple text file with the values read in (location of buttons, etc.) and the background image. 
//  It is then converted to memory so that the PGR file does not need to exist on disk locally when the program is run.
//
// ------------------------------------
// This version: Class-Based OO version
// ------------------------------------
//
//  This file is the same program as the non-oo version, just with the lamdbas and variables needed put into 
//  the class rather than keeping in the main() function/
//
//  This allows for expandability and overloading of functions for true emulation where desired. 
//  The virtual functions can be overloaded and work with real hardware.

#include "CSageBox.h"
#include "Widgets\include\CDialWidget.h"
#include "Widgets\include\CLCDDisplayWidget.h"

// Data files compiled as memory 

#include "..\lcddialtexture.pgr2.h"     // backdrop data file
#include "..\AboutTwoWidgets.pgr2.h"    // About Box HTML and background image data file

class CTwoWidgets
{
    // Texture image and control/window locations read from the PGR file (see LoadTextData() comments)

    POINT       m_pLcdLoc;          // Location of LCD Widget in window
    POINT       m_pDialLoc;         // Location of Dial Widget in window
    POINT       m_pDebugLoc;        // Location of debug window
    POINT       m_pButtonLoc;       // Start location of lower buttons
    SIZE        m_szDebugSize;      // Size of debug window
    CString     m_csDebugColor;     // background color of the debug window
    CBitmap     m_cBitmap;          // The main texture/bitmap image filling the entire window

    // Basic Controls 

    CWindow        * m_cWin         = nullptr;  // Main window sized to the bitmap size in the PGR File
    CWindow        * m_cDebugWin    = nullptr;  // Debug Window 

    CLcdDisplayWidget   * m_cLcd;               // LCD Widhet 
    CDialWidget         * m_cDial;              // Dial Widget

    void LoadTextureData();                     // Loads texture image and control/window locations for display
    void InitWindow(CSageBox & cSageBox);    
    void InitDebugWindow();
    void CheckEvents();                         // Check events in which we're interested 
    void DrawAboutBox();

    virtual void InitDial();
    virtual void InitLCD();
    virtual void InitHardware();                // Initialize all hardware / emulated hardware
    virtual void CheckDialEvent();
    virtual void UpdateLCD();
public:
    int main();                                 // local main() routine, used as std C/C++ main()

};

// LoadTextureData() -- Open up the PGR file, a profile and data storage format.
//
// It has the main bitmap but also specified where control objects and the text window need to be placed. 
//
// .PGR files contain files and declarations to control the data. 
//
// This is put into the PGR so that it can later be generic and change -- as the 
// location of various element change, thos ecan be changed in the PGR file so
//
//        a) Code here doesn't need to change because a graphic has changed
//        b) Different images (i.e. PGR files) can be used for the same process
//           (for example, a different scenario with some or all of the same tools -- the PGR File
//            can contain any value, including dictating which elements to use and which may not be available.
//
//
void CTwoWidgets::LoadTextureData()
{
    CSagePGR cPgr(sTexture_1);

    // Load values by name in the PGR file

    m_pLcdLoc       = cPgr.ReadPoint("LcdXY");            
    m_pDialLoc      = cPgr.ReadPoint("DialXY");
    m_pDebugLoc     = cPgr.ReadPoint("DebugWinXY");
    m_pButtonLoc    = cPgr.ReadPoint("ButtonXY");
    m_szDebugSize   = cPgr.ReadSize("DebugWinSize");
    m_csDebugColor  = cPgr.ReadText("DebugWinColor"); 
    m_cBitmap       = cPgr.ReadBitmap("texture");        // The main image
}

// Create the Dial Widget.  When this is rotated, the value is displayed, and the raw value
// from the dial is sent to the remote device (represented by the LCD). 
//
// The dial can be a temperature or other guage, such as a pool temperature or local heater control.
//
void CTwoWidgets::InitDial()
{
    m_cDial = new CDialWidget(m_cWin,m_pDialLoc);
    m_cDial->SetRange(0,150);
    m_cDial->SetValue(75);
}

// Create the LCD Widget, which will display the raw data from the Dial Widget. 
// This represents emulating a remote embedded device, disconnected from the user and
// main dial control.
//
void CTwoWidgets::InitLCD()
{
    m_cLcd = new CLcdDisplayWidget(m_cWin,m_pLcdLoc,0,Transparent()); 
}

void CTwoWidgets::UpdateLCD()
{
    m_cLcd->SetValue((int) (int) m_cDial->GetValue());
}
void CTwoWidgets::InitHardware()
{
    InitDial();
    InitLCD();
}

// CheckDialEvent() -- this effectively combines three functions 
//
//  1. Check the hardware event (the dial). 
//  2. Print out changed data to the debug window 
//  3. Set the LCD Value (which could be hardware or emulated)
//
// They are just put into one function here for clarity and keep the program short, since it's just a small demo
//
void CTwoWidgets::CheckDialEvent()
{
    double fValue;
    if (m_cDial->ValueChanged(fValue))
    {
        m_cDebugWin->printf("{g}Event.{/}  Value = {y}%d{/} -- Raw Value = {c}%d{/} -- Sent Value to Device (LCD)\n",(int) fValue,m_cDial->GetRawValue());
        UpdateLCD();
    }
}
void CTwoWidgets::CheckEvents()
{
    CheckDialEvent();
}

void CTwoWidgets::InitDebugWindow()
{
    // Create the debug output window.  Here, it only displays the dial value, representing
    // the output we might get from any device.  In this case, it represents the low-level
    // value we might get from a hardware device such as the dial control.

    m_cDebugWin = &m_cWin->ChildWindow(m_pDebugLoc,m_szDebugSize,bgColor(m_csDebugColor) | Font("Courier New,12")); 

    // Set up an "Immediate" update type.  This will update the debug window after each and every write, so we don't
    // have to worry about its state.  This is a slower way to do it and not recommended when you want ultra-speed, but is
    // fine for this purpose.  Otherwise, we'd have to Update() ourselves, as letting the auto-udpate handle it does not
    // guarentee the latest output until an Update() is called.

    m_cDebugWin->SetAutoUpdate(AutoUpdateType::Immediate);                  // Slow-but-sure updates (i.e. don't have to worry about updating)
    m_cDebugWin->printf("{g}Emulator Debug Window Ready (OO Version)\n");   // Print a message so the user knows its operating. "{g}" sets the text color to green.
}

void CTwoWidgets::InitWindow(CSageBox & cSageBox)
{
    LoadTextureData();

    // Open a window sized to the background image & texture.
    // 
    // NoBorder() -- Specifies no title bar (i.e. client area only) for popup windows. 

    m_cWin = &cSageBox.NewWindow(100,100,m_cBitmap.GetWidth(),m_cBitmap.GetHeight(),NoBorder());

    m_cWin->DisplayBitmap(m_cBitmap);        // Display the background image.

    // Allow the window be dragged around by any open area in the window (since we don't have the title bar)
    // Otherwise the user wouldn't be able to move the window at all since we specified "NoBorder()" on
    // window creation.

    m_cWin->SetWindowDrag();

    InitHardware();
    InitDebugWindow();
    UpdateLCD();
}

void CTwoWidgets::DrawAboutBox()
{
    // Options: Modal()     -- this keeps the other windows from being used until the
    //                         About Window is dismissed by the user.
    //          CenterXY()  -- This centers the About Window on the screen.  Otherwise it
    //                         is more in the upper-left part of the screen.

    m_cWin->DrawSimpleDoc(sAbout,Modal() | CenterXY());
}
int CTwoWidgets::main()
{
    CSageBox cSageBox("SageBox -- Using two Widgets / Simple Emulation Example"); 
    InitWindow(cSageBox);

    // Create a couple buttons.  Since I didn't put a menu, I put an about button here. 

    auto &cButtonClose = m_cWin->NewButton(m_pButtonLoc.x,        m_pButtonLoc.y,120,0,"Exit");
    auto &cButtonAbout = m_cWin->NewButton(m_pButtonLoc.x+125,    m_pButtonLoc.y,120,0,"About");

    // Add some hover messages (i.e. tooltips) so when the buttons or hovered-over with the mouse, a message displays 

    cButtonClose.SetHoverMsg("Press to Close Window and Exit program."); 
    cButtonAbout.SetHoverMsg("Show About Box Window explaining this program.");

    // The main event loop. 
    //
    // In this case, we're just looking for a value from the dial (which may be an emulation, as here, or real hardware.  We then display the
    // results to the debug window. 
    //

    while(m_cWin->GetEvent() && !cButtonClose.Pressed())
    {
        CheckEvents();        // Check events we might want to emulate (in this case, just the dial)

        // Check local events -- in this case, we just have the about button, but could grow it over time.

        if (cButtonAbout.Pressed()) DrawAboutBox();
    }
    return 0;
}

int main()
{
    CTwoWidgets cExample;
    return cExample.main();
}
