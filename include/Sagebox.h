// This file copyright(c) 2021 Rob Nelson, All Rights Reserved.    E-mail rob@projectsagebox.com for more information.
//

// --------------------------------------------------------------------------------------------------------------
// PRELIMINARY VERSION.  This file has been brought into the Sagebox project from the original sources and has 
// not yet been commented for Sagebox, or properly formatted (I used tabs and am converting to spaces).
// --------------------------------------------------------------------------------------------------------------
//
// ****** UNDER CONSTRUCTION ******
//
// This file is still under construction and may not yet include specifics, awaiting proper testing and integration into Sagebox.
//

//#pragma warning( disable : 26444) 
#pragma once

#if !defined (_CSageBox_H_)
#define _CSageBox_H_
//#pragma warning( disable : 4996) 

//#include "SageIncludes.h"
#include <cstdlib>
#include <cstdio>
#include <ctime>

#include "Sage.h"
#include "SageOpt.h"
#include "CSageTypes.h"
#include "CWindow.h"
#include "CTextWidget.h"
#include "CMemClass.h"
#include "CSageMacros.h"
#include "CDialog.h"
#include "CSageTools.h"
#include "StyleManifest.h"
#include "CQuickControls.h"
#include "CComplex.h"
#include "CQuickDialog.h"
#include "CSageTimer.h"
#include "InstructionSet.h"
#include "WinConio.h"
#include "CImageWindow.h"
#include "CBeforeAfterImage.h"
#include "CQuickForm.h"

//#include "CQuickForm.h"

// Thesse libraries are included here primarily to eliminate a "_NULL_IMPORT_DESCRIPTOR" warning. 
// On the other hand, I am also thinking of putting all external dependencies here so that they are not included in the 
// static library, but linked with the compiled application.  TBD 

 #pragma comment(lib,"gdiplus.lib")
 #pragma comment(lib,"ddraw.lib")
 #pragma comment(lib,"winmm.lib")

// -------------------
// Main CSageBox Class
// -------------------
//
// CSageBox needs to be instantiated as the first object in SageBox.  For quick console-mode functions, only CSageBox is needed. 
//
// Once CSageBox is created, many utility functions become available, and visible windows (CWindow class) can be created that
// can host output, input, controls widgets, etc.
//
// SageBox has a few modes, depending on the type of application.
//
// Console
//
// SageBox can be used in console mode to supplement console-mode programmging with quick dialog and button functions,
// and Windows that can host the entire set of SageBox functions while still in console-mode.
//
// SandBox
//
// The SandBox mode allows regular C/C++ programming in the context of a window class, mixing
// console-mode style programming with windows functionality. 
// 
// In this mode, the console mode window can also be available so that the window(s) created by SageBox and the Console mode
// window an be used simultaneously.
//
// The SandBox window acts just like a Windows programs, with controls like sliders, buttons, etc., and can also be
// used in a more console-mode style setting with various console functions such as printf, write(), and various i/o
// routines, including "in" and "out" that recplicate C++ cin and cout, but with more functionality.
//
// Regular Windows -- Event Drive and Procedural (i.e. regular C/C++)
//
// SageBox applications can be written in a regular C/C++ procedural style without the use of event callbacks or overriding classes, as well as regular windows.
// A mix of the two is also possible, the idea being to start with an easy procedural program and then later migrate some or all of it to a more event-drive
// structure as needed.
//
// With Windows, not all functionality needs to be event-driven to be the most efficient, but in some cases that may be useful dependonding on the application.
//
// For most applications, start-up projects, and prototypush programs, simple procedural code with the use of EventLoop() works just fine and creates
// a bonafide windows application.
//
// As the program grows, it is an easy transition to making some or all of the program work in an event-driven source-code structure.
//
// Event-Driven, Canonical Windows
//
// Sagebox also facilitates writing canonical windows programs, with overrides and classes (i.e. Message Handlers) to intercept all Windows messages
// with or without overriding the window or control class. 
//
// With a Message Handler, you can specify where to send Windows messages without overriding the class, or the entire window or control can be overriden for more
// flexibility.
//
// SageBox also provides a large array of functions to work with the Windows operating system, as well as keeping track of event states as they occur.
//
// SageBox Tools
//
// This is still being implemented.  The tools in Sagebox extend beyond windows functionality and into general tools, such as image-processing, neural networking, and other tools that
// have been developed over the last few years.
//
// This is still being developed.  See functions available in CSageTools as well as CRawBitmap, Sage::CString, and so-forth.
//

namespace Sage
{
using CImgView = CImageWin;

class CStoreValues;
// Empty values useful for returning null objects of various types when errors occur. 

static constexpr RGBColor_t EmptyRGB{};
static constexpr HSLColor_t EmptyHSL{};
static constexpr POINT		EmptyPoint{};
static constexpr SIZE		EmptySize{};
static constexpr RECT       EmptyRect {};
static constexpr SizeRect   EmptySizeRect{};

class CSageBox;
class CProcessWindow;
class CHandleWindowMsg;
using CSagebox = CSageBox;          // Allow CSagebox to be used, as well, since I keep putting CSagebox cSagebox in videos. 
using Sagebox = CSageBox;          // Allow CSagebox to be used, as well, since I keep putting CSagebox cSagebox in videos. 
using SageBox = CSageBox;          // Allow CSagebox to be used, as well, since I keep putting CSagebox cSagebox in videos. 
class CConsoleIO;
struct WinConio; 

#define SageboxStatic

#ifdef SageboxStatic
#define __sagestatic static        // used for possible .DLL changes in the future
#else
#define __sagestatic              // Otherwise define these functions as non-static
#endif

class CSageBox
{
    friend CHandleWindowMsg;
    friend WinConio;
	CWindow * m_cWin = nullptr;
public:


    
    // Process Window Start Mode
    //
    //   Default,        // Shows title bar in upper-right corner that expands when mouseover
    //   Debug,          // Shows full window including debug output window
    //   Hidden,         // Hidden until debug is output first time, mouse is held in upper-right corner, or user turns it on through sysmenu
    //
    enum class ProcessWinStartMode
    {
        Default,        // Shows title bar in upper-right corner that expands when mouseover
        Debug,          // Shows full window including debug output window
        Hidden,         // Hidden until debug is output first time, mouse is held in upper-right corner, or user turns it on through sysmenu

    };



private:

	void Postamble(CWindow & cWin);
	__sagestatic void Preamble(CWindow & cWin,bool bHidden = false);
	void PreambleStatic(CWindow & cWin,bool bHidden = false);
	void CreateDefaultWindow();
    bool InitDevControls();   // Initialize default Dev Controls Window -- added only if used. 
    static void CreateProcessAccelerator();       // Creates ^C accelerator

    HANDLE m_hWinThread = nullptr;
    CDevControls * m_cDevControls   = nullptr;          // Not created until first used. 
    bool m_bConsoleHidden           = true;             // true if the Console Window was hidden
    HWND m_hConsoleWindow           = (HWND) nullptr;   // HWND of the Console window if we're in console mode. 
    bool m_bConsoleDontRestore      = false;            // When true and the console is hidden, the console window is not 
                                                        // restored on program exit.  Otherwise, a hidden console window is restored
                                                        // When the program ends.
//    static inline void * m_pSageCallback    = nullptr;
//    static inline void * m_fSageCallback    = nullptr;
//    static inline bool m_bSageCallbackPlain = false;    // When true SageCallBack() form is called, otherwise SageCallBack(<T> * pData) is called

    // Process-Wide singleton

#define __sage__kLocalProcessWin

#ifdef __sage__kLocalProcessWin
    CProcessWindow * m_cProcessWindow    = nullptr;           // Process Watch Window & low-level debug.  One per process. 
    bool             m_bProcessWinActive = false;
    bool             m_bAcceptCtrlCBreak = false;        // When true, ^C offers to terminate the process. 
#else
    static CProcessWindow * m_cProcessWindow;           // Process Watch Window & low-level debug.  One per process. 
    static bool             m_bProcessWinActive;
    static bool             m_bAcceptCtrlCBreak;        // When true, ^C offers to terminate the process. 
#endif
	int m_iConsoleWinNum = 0;		// Used for positioning windows when ConsoleWin() is called
    CPasWindow * m_cWinCore = nullptr;
    void InitPriv(const char * sAppName);

    static CStoreValues DebugValues; 

    // Items destined to be removed from the release interface at some point (after the Alpha/Beta stage)
    // Public for now. 

kSagePrivPublic:
    void _privShutdown(); 
public:

    /// <summary>
    /// Console Output Functions
    /// <para></para>&#160;&#160;&#160;
    /// conio provides a set of useful functions for console output that allow for different text colors, font sizes, background colors, and so-forth.
    /// <para></para>
    /// You can use conio.printf() or conio.write() with "{}" markers to specify colors.
    /// <para></para>&#160;&#160;&#160;For example, printf("This is {red}red{/} and this is {b}blue") prints "red in red" and "blue" in blue.
    /// <para></para>Note the use of {b} as a shortcut for blue.  Colors can be "r" for "Red" and so-forth, and "db" for "darkblue", etc. Use  conio.GetColorNames() for a full list.
    /// <para></para>&#160;&#160;&#160;
    /// Background colors can also be set with "bg=r", or the entire line with "lbg=blue", for example.
    /// <para></para>&#160;&#160;&#160;
    /// --> "u" for underline, and "rev" as quick way to reverse text. Combine types, such as conio.printf("This {bg=blue}{y}word{/}{/} is written in a blue background with yellow text color")
    /// </summary>
    static WinConio conio; 

    // Public values to be removed from the release version at some later date (kSagePrivPublic)

kSagePrivPublic:

    // Some external variables used in initialization where the Sagebox object has not been created yet
    // or is not in the scope of a low-level processing setting up a configuration

    static WORD m_wC; 
    static const char * m_sC; 
    static const char * m_sT; 
    static bool _bIsSagebox; 

public:
	// The main SageBox Class
	//
	// note: Only instantiate one per-application.  Pass a reference or a pointer, or set
	// a pointer member variable to re-use in other functions and classes.
	//
	// Sagebox contains a number of console-based functions that can help with windowing functions
	// without creating a Window.  Use the CSageBox object to access these.
	//
	// All CSageBox functions are available through windows created by CSageBox
	//
	// For windows-based applications, the CSageBox object is not needed once it is instantiated, but
	// cannot be deleted until the end of the program, as it closes and destroys all windows, controls, widgets,
	// and memory created by any part of SageBox
	//
	CSageBox(const char * sAppName = nullptr);
	CSageBox(const char * sAppName,SageboxInit & stInit);
	CSageBox(SageboxInit & stInit);
    inline static DWORD        __dwMessageThreadID = 0;

	~CSageBox();
    void SetWinThread(HANDLE hThread);

    // Quick Console Window functions (same as NewWindow, just smaller)
	//
	// This creates quick small window (around 800x600), where NewWindow creates a much larger window. 
	// There isn't any other difference
	//
	CWindow & ConsoleWin(int iX,int iY,const char * sTitle = nullptr,const cwfOpt & cwOpt = cwfOpt());
    
    // Quick Console Window functions (same as NewWindow, just smaller)
	//
	// This creates quick small window (around 800x600), where NewWindow creates a much larger window. 
	// There isn't any other difference
	//
	CWindow & ConsoleWin(int iX,int iY,const cwfOpt & cwOpt);

	// Quick Console Window functions (same as NewWindow, just smaller)
	//
	// This creates quick small window (around 800x600), where NewWindow creates a much larger window. 
	// There isn't any other difference
	//
	CWindow & ConsoleWin(const char * sTitle = nullptr,const cwfOpt & cwOpt = cwfOpt());

	// Quick Console Window functions (same as NewWindow, just smaller)
	//
	// This creates quick small window (around 800x600), where NewWindow creates a much larger window. 
	// There isn't any other difference
	//
	CWindow & ConsoleWin(cwfOpt & cwOpt);


	// NewWindow -- Create a new popup window.
    // This creates a regular window with all of the functions and properties of the parent window.
    //
    // The Event properties (i.e. through EventLoop() and WaitforEvent() also run through the parent.
    // Therefore, the Parent's WaitforEvent() and EventLoop() can be used to check for the new window's events.
    //
    // A subclassed window object may be passed in to override event callbacks, Main(), and other CWindow components.
    // This object is deleted automatically (i.e. the object passed in)
    //
    // SetMessageHandler() can be used to override event messages without overriding the window class
    //
    // When the User presses the 'X' window close button, a WindowClosing() for the new window will come back as true, and a 
    // CloseButtonPressed() event will be triggerred.
    //
	__sagestatic CWindow & NewWindow(CWindow * cWin,int iX,int iY,int iWidth,int iHeight,const char * sWinTitle,const cwfOpt & cwOpt = cwfOpt());

	// NewWindow -- Create a new popup window.
    // This creates a regular window with all of the functions and properties of the parent window.
    //
    // The Event properties (i.e. through EventLoop() and WaitforEvent() also run through the parent.
    // Therefore, the Parent's WaitforEvent() and EventLoop() can be used to check for the new window's events.
    //
    // A subclassed window object may be passed in to override event callbacks, Main(), and other CWindow components.
    // This object is deleted automatically (i.e. the object passed in)
    //
    // SetMessageHandler() can be used to override event messages without overriding the window class
    //
    // When the User presses the 'X' window close button, a WindowClosing() for the new window will come back as true, and a 
    // CloseButtonPressed() event will be triggerred.
    //
    __sagestatic CWindow & NewWindow(CWindow * cWin,int iX,int iY,int iWidth,int iHeight,const cwfOpt & cwOpt = cwfOpt());

	// NewWindow -- Create a new popup window.
    // This creates a regular window with all of the functions and properties of the parent window.
    //
    // The Event properties (i.e. through EventLoop() and WaitforEvent() also run through the parent.
    // Therefore, the Parent's WaitforEvent() and EventLoop() can be used to check for the new window's events.
    //
    // A subclassed window object may be passed in to override event callbacks, Main(), and other CWindow components.
    // This object is deleted automatically (i.e. the object passed in)
    //
    // SetMessageHandler() can be used to override event messages without overriding the window class
    //
    // When the User presses the 'X' window close button, a WindowClosing() for the new window will come back as true, and a 
    // CloseButtonPressed() event will be triggerred.
    //
	__sagestatic CWindow & NewWindow(CWindow * cWin,const char * sWinTitle = nullptr,const cwfOpt & cwOpt = cwfOpt());

    // NewWindow -- Create a new popup window.
    // This creates a regular window with all of the functions and properties of the parent window.
    //
    // The Event properties (i.e. through EventLoop() and WaitforEvent() also run through the parent.
    // Therefore, the Parent's WaitforEvent() and EventLoop() can be used to check for the new window's events.
    //
    // A subclassed window object may be passed in to override event callbacks, Main(), and other CWindow components.
    // This object is deleted automatically (i.e. the object passed in)
    //
    // SetMessageHandler() can be used to override event messages without overriding the window class
    //
    // When the User presses the 'X' window close button, a WindowClosing() for the new window will come back as true, and a 
    // CloseButtonPressed() event will be triggerred.
    //
	__sagestatic CWindow & NewWindow(CWindow * cWin,const cwfOpt & cwOpt);

	// NewWindow -- Create a new popup window.
    // This creates a regular window with all of the functions and properties of the parent window.
    //
    // The Event properties (i.e. through EventLoop() and WaitforEvent() also run through the parent.
    // Therefore, the Parent's WaitforEvent() and EventLoop() can be used to check for the new window's events.
    //
    // A subclassed window object may be passed in to override event callbacks, Main(), and other CWindow components.
    // This object is deleted automatically (i.e. the object passed in)
    //
    // SetMessageHandler() can be used to override event messages without overriding the window class
    //
    // When the User presses the 'X' window close button, a WindowClosing() for the new window will come back as true, and a 
    // CloseButtonPressed() event will be triggerred.
    //
	__sagestatic CWindow & NewWindow(int iX,int iY,int iWidth,int iHeight,const char * sWinTitle, const cwfOpt & cwOpt = cwfOpt());

	// NewWindow -- Create a new popup window.
    // This creates a regular window with all of the functions and properties of the parent window.
    //
    // The Event properties (i.e. through EventLoop() and WaitforEvent() also run through the parent.
    // Therefore, the Parent's WaitforEvent() and EventLoop() can be used to check for the new window's events.
    //
    // A subclassed window object may be passed in to override event callbacks, Main(), and other CWindow components.
    // This object is deleted automatically (i.e. the object passed in)
    //
    // SetMessageHandler() can be used to override event messages without overriding the window class
    //
    // When the User presses the 'X' window close button, a WindowClosing() for the new window will come back as true, and a 
    // CloseButtonPressed() event will be triggerred.
    //
	__sagestatic CWindow & NewWindow(int iX,int iY,int iWidth,int iHeight,const cwfOpt & cwOpt = cwfOpt());

	// NewWindow -- Create a new popup window.
    // This creates a regular window with all of the functions and properties of the parent window.
    //
    // The Event properties (i.e. through EventLoop() and WaitforEvent() also run through the parent.
    // Therefore, the Parent's WaitforEvent() and EventLoop() can be used to check for the new window's events.
    //
    // A subclassed window object may be passed in to override event callbacks, Main(), and other CWindow components.
    // This object is deleted automatically (i.e. the object passed in)
    //
    // SetMessageHandler() can be used to override event messages without overriding the window class
    //
    // When the User presses the 'X' window close button, a WindowClosing() for the new window will come back as true, and a 
    // CloseButtonPressed() event will be triggerred.
    //
	__sagestatic CWindow & NewWindow(const char * sWinTitle = nullptr,const cwfOpt & cwOpt = cwfOpt());

	// NewWindow -- Create a new popup window.
    // This creates a regular window with all of the functions and properties of the parent window.
    //
    // The Event properties (i.e. through EventLoop() and WaitforEvent() also run through the parent.
    // Therefore, the Parent's WaitforEvent() and EventLoop() can be used to check for the new window's events.
    //
    // A subclassed window object may be passed in to override event callbacks, Main(), and other CWindow components.
    // This object is deleted automatically (i.e. the object passed in)
    //
    // SetMessageHandler() can be used to override event messages without overriding the window class
    //
    // When the User presses the 'X' window close button, a WindowClosing() for the new window will come back as true, and a 
    // CloseButtonPressed() event will be triggerred.
    //
    __sagestatic CWindow & NewWindow(cwfOpt & cwOpt);

	// NewWindow -- Create a new popup window.
    // This creates a regular window with all of the functions and properties of the parent window.
    //
    // Note when using NewWindow(SIZE(),..) varations, InnerSize() is assumed and the window size supplied will be the interior size.
    //
    // The Event properties (i.e. through EventLoop() and WaitforEvent() also run through the parent.
    // Therefore, the Parent's WaitforEvent() and EventLoop() can be used to check for the new window's events.
    //
    // A subclassed window object may be passed in to override event callbacks, Main(), and other CWindow components.
    // This object is deleted automatically (i.e. the object passed in)
    //
    // SetMessageHandler() can be used to override event messages without overriding the window class
    //
    // When the User presses the 'X' window close button, a WindowClosing() for the new window will come back as true, and a 
    // CloseButtonPressed() event will be triggerred.
    //
    __sagestatic CWindow & NewWindow(SIZE szSize,const char * sTitle = nullptr,const cwfOpt & cwOpt = cwfOpt());

    // NewWindow -- Create a new popup window.
    // This creates a regular window with all of the functions and properties of the parent window.
    //
    // Note when using NewWindow(SIZE(),..) varations, InnerSize() is assumed and the window size supplied will be the interior size.
    //
    // The Event properties (i.e. through EventLoop() and WaitforEvent() also run through the parent.
    // Therefore, the Parent's WaitforEvent() and EventLoop() can be used to check for the new window's events.
    //
    // A subclassed window object may be passed in to override event callbacks, Main(), and other CWindow components.
    // This object is deleted automatically (i.e. the object passed in)
    //
    // SetMessageHandler() can be used to override event messages without overriding the window class
    //
    // When the User presses the 'X' window close button, a WindowClosing() for the new window will come back as true, and a 
    // CloseButtonPressed() event will be triggerred.
    //
    __sagestatic CWindow & NewWindow(SIZE szSize,const cwfOpt & cwOpt); 
    __sagestatic CWindow & NewWindow(POINT pLoc,SIZE szSize,const cwfOpt & cwOpt = cwfOpt()); 
    __sagestatic CWindow & NewWindow(POINT pLoc,SIZE szSize,const char * sTitle,const cwfOpt & cwOpt = cwfOpt()); 

	// NewWindow -- Create a new popup window.
    // This creates a regular window with all of the functions and properties of the parent window.
    //
    // The Event properties (i.e. through EventLoop() and WaitforEvent() also run through the parent.
    // Therefore, the Parent's WaitforEvent() and EventLoop() can be used to check for the new window's events.
    //
    // A subclassed window object may be passed in to override event callbacks, Main(), and other CWindow components.
    // This object is deleted automatically (i.e. the object passed in)
    //
    // SetMessageHandler() can be used to override event messages without overriding the window class
    //
    // When the User presses the 'X' window close button, a WindowClosing() for the new window will come back as true, and a 
    // CloseButtonPressed() event will be triggerred.
    //
    __sagestatic CWindow & NewWindow(CPoint pLoc,const char * sTitle = nullptr,const cwfOpt & cwOpt = cwfOpt());

	// NewWindow -- Create a new popup window.
    // This creates a regular window with all of the functions and properties of the parent window.
    //
    // The Event properties (i.e. through EventLoop() and WaitforEvent() also run through the parent.
    // Therefore, the Parent's WaitforEvent() and EventLoop() can be used to check for the new window's events.
    //
    // A subclassed window object may be passed in to override event callbacks, Main(), and other CWindow components.
    // This object is deleted automatically (i.e. the object passed in)
    //
    // SetMessageHandler() can be used to override event messages without overriding the window class
    //
    // When the User presses the 'X' window close button, a WindowClosing() for the new window will come back as true, and a 
    // CloseButtonPressed() event will be triggerred.
    //
    __sagestatic CWindow & NewWindow(CPoint pLoc,const cwfOpt & cwOpt); 

    // ----------- WidgetWindow() variants of NewWindow() ---------------
    //
    // The only difference (so far) between WidgetWindow and NewWindow() is that NewWindow() sets a Primary Window Status



	// WidgetWindow -- Create a new popup window.  Same as NewWindow() but set Window as a Primary/Main Window
    // This creates a regular window with all of the functions and properties of the parent window.
    //
    // The Event properties (i.e. through EventLoop() and WaitforEvent() also run through the parent.
    // Therefore, the Parent's WaitforEvent() and EventLoop() can be used to check for the new window's events.
    //
    // A subclassed window object may be passed in to override event callbacks, Main(), and other CWindow components.
    // This object is deleted automatically (i.e. the object passed in)
    //
    // SetMessageHandler() can be used to override event messages without overriding the window class
    //
    // When the User presses the 'X' window close button, a WindowClosing() for the new window will come back as true, and a 
    // CloseButtonPressed() event will be triggerred.
    //
	__sagestatic CWindow & WidgetWindow(int iX,int iY,int iWidth,int iHeight,const char * sWinTitle, const cwfOpt & cwOpt = cwfOpt());

	// WidgetWindow -- Create a new popup window. Same as NewWindow() but set Window as a Primary/Main Window
    // This creates a regular window with all of the functions and properties of the parent window.
    //
    // The Event properties (i.e. through EventLoop() and WaitforEvent() also run through the parent.
    // Therefore, the Parent's WaitforEvent() and EventLoop() can be used to check for the new window's events.
    //
    // A subclassed window object may be passed in to override event callbacks, Main(), and other CWindow components.
    // This object is deleted automatically (i.e. the object passed in)
    //
    // SetMessageHandler() can be used to override event messages without overriding the window class
    //
    // When the User presses the 'X' window close button, a WindowClosing() for the new window will come back as true, and a 
    // CloseButtonPressed() event will be triggerred.
    //
	__sagestatic CWindow & WidgetWindow(int iX,int iY,int iWidth,int iHeight,const cwfOpt & cwOpt = cwfOpt());

	// WidgetWindow -- Create a new popup window. Same as NewWindow() but set Window as a Primary/Main Window
    // This creates a regular window with all of the functions and properties of the parent window.
    //
    // The Event properties (i.e. through EventLoop() and WaitforEvent() also run through the parent.
    // Therefore, the Parent's WaitforEvent() and EventLoop() can be used to check for the new window's events.
    //
    // A subclassed window object may be passed in to override event callbacks, Main(), and other CWindow components.
    // This object is deleted automatically (i.e. the object passed in)
    //
    // SetMessageHandler() can be used to override event messages without overriding the window class
    //
    // When the User presses the 'X' window close button, a WindowClosing() for the new window will come back as true, and a 
    // CloseButtonPressed() event will be triggerred.
    //
	__sagestatic CWindow & WidgetWindow(const char * sWinTitle = nullptr,const cwfOpt & cwOpt = cwfOpt());

	// WidgetWindow -- Create a new popup window. Same as NewWindow() but set Window as a Primary/Main Window
    // This creates a regular window with all of the functions and properties of the parent window.
    //
    // The Event properties (i.e. through EventLoop() and WaitforEvent() also run through the parent.
    // Therefore, the Parent's WaitforEvent() and EventLoop() can be used to check for the new window's events.
    //
    // A subclassed window object may be passed in to override event callbacks, Main(), and other CWindow components.
    // This object is deleted automatically (i.e. the object passed in)
    //
    // SetMessageHandler() can be used to override event messages without overriding the window class
    //
    // When the User presses the 'X' window close button, a WindowClosing() for the new window will come back as true, and a 
    // CloseButtonPressed() event will be triggerred.
    //
    __sagestatic CWindow & WidgetWindow(cwfOpt & cwOpt);

	// WidgetWindow -- Create a new popup window. Same as NewWindow() but set Window as a Primary/Main Window
    // This creates a regular window with all of the functions and properties of the parent window.
    //
    // Note when using WidgetWindow(SIZE(),..) varations, InnerSize() is assumed and the window size supplied will be the interior size.
    //
    // The Event properties (i.e. through EventLoop() and WaitforEvent() also run through the parent.
    // Therefore, the Parent's WaitforEvent() and EventLoop() can be used to check for the new window's events.
    //
    // A subclassed window object may be passed in to override event callbacks, Main(), and other CWindow components.
    // This object is deleted automatically (i.e. the object passed in)
    //
    // SetMessageHandler() can be used to override event messages without overriding the window class
    //
    // When the User presses the 'X' window close button, a WindowClosing() for the new window will come back as true, and a 
    // CloseButtonPressed() event will be triggerred.
    //
    __sagestatic CWindow & WidgetWindow(SIZE szSize,const char * sTitle = nullptr,const cwfOpt & cwOpt = cwfOpt());

    // WidgetWindow -- Create a new popup window. Same as NewWindow() but set Window as a Primary/Main Window
    // This creates a regular window with all of the functions and properties of the parent window.
    //
    // Note when using WidgetWindow(SIZE(),..) varations, InnerSize() is assumed and the window size supplied will be the interior size.
    //
    // The Event properties (i.e. through EventLoop() and WaitforEvent() also run through the parent.
    // Therefore, the Parent's WaitforEvent() and EventLoop() can be used to check for the new window's events.
    //
    // A subclassed window object may be passed in to override event callbacks, Main(), and other CWindow components.
    // This object is deleted automatically (i.e. the object passed in)
    //
    // SetMessageHandler() can be used to override event messages without overriding the window class
    //
    // When the User presses the 'X' window close button, a WindowClosing() for the new window will come back as true, and a 
    // CloseButtonPressed() event will be triggerred.
    //
    __sagestatic CWindow & WidgetWindow(SIZE szSize,const cwfOpt & cwOpt); 
    __sagestatic CWindow & WidgetWindow(POINT pLoc,SIZE szSize,const cwfOpt & cwOpt = cwfOpt()); 
    __sagestatic CWindow & WidgetWindow(POINT pLoc,SIZE szSize,const char * sTitle,const cwfOpt & cwOpt = cwfOpt()); 

	// WidgetWindow -- Create a new popup window. Same as NewWindow() but set Window as a Primary/Main Window
    // This creates a regular window with all of the functions and properties of the parent window.
    //
    // The Event properties (i.e. through EventLoop() and WaitforEvent() also run through the parent.
    // Therefore, the Parent's WaitforEvent() and EventLoop() can be used to check for the new window's events.
    //
    // A subclassed window object may be passed in to override event callbacks, Main(), and other CWindow components.
    // This object is deleted automatically (i.e. the object passed in)
    //
    // SetMessageHandler() can be used to override event messages without overriding the window class
    //
    // When the User presses the 'X' window close button, a WindowClosing() for the new window will come back as true, and a 
    // CloseButtonPressed() event will be triggerred.
    //
    __sagestatic CWindow & WidgetWindow(CPoint pLoc,const char * sTitle = nullptr,const cwfOpt & cwOpt = cwfOpt());

	// WidgetWindow -- Create a new popup window. Same as NewWindow() but set Window as a Primary/Main Window
    // This creates a regular window with all of the functions and properties of the parent window.
    //
    // The Event properties (i.e. through EventLoop() and WaitforEvent() also run through the parent.
    // Therefore, the Parent's WaitforEvent() and EventLoop() can be used to check for the new window's events.
    //
    // A subclassed window object may be passed in to override event callbacks, Main(), and other CWindow components.
    // This object is deleted automatically (i.e. the object passed in)
    //
    // SetMessageHandler() can be used to override event messages without overriding the window class
    //
    // When the User presses the 'X' window close button, a WindowClosing() for the new window will come back as true, and a 
    // CloseButtonPressed() event will be triggerred.
    //
    __sagestatic CWindow & WidgetWindow(CPoint pLoc,const cwfOpt & cwOpt); 

    // ----------- End WidgetWindow() variants of NewWindow() --------------


    // GetStaticSagebox() -- Returns the object for m_cStaticSagebox created when AutoWindow() is used. 
    //
    static CSagebox * GetStaticSagebox();
    static void * __sage_privdll_package(void * vpPackage = nullptr);     // various development package elements; TBD -- for internal use

    // This is used to register a widget with SageBox.
    // The primary use for this is for the widget to register with Sagebox when called by Sagebox.
    // 
    // When Sagebox calls the Widget::Register() function, the widget calls this function,
    // which fille the provided RegistryID with an ID for the widget that it can use to work with dialog boxes
    // created by Sagebox.
    //
    // Note:: this is not a user function and is used by SageBox and Widgets
    // 
	int RegisterWidget(int & iRegistryID);
#if 0
    // This is used to register a widget with SageBox.
    // The primary use for this is for the widget to register with Sagebox when called by Sagebox.
    // 
    // When Sagebox calls the Widget::Register() function, the widget calls this function,
    // which fille the provided RegistryID with an ID for the widget that it can use to work with dialog boxes
    // created by Sagebox.
    //
    // Note:: this is not a user function and is used by SageBox and Widgets
    // 
	CDialog & NewDialog(int iX,int iY,int iWidth = 0,int iHeight = 0,const char * sTitle = nullptr,const cwfOpt & cwOpt = cwfOpt());

    // This is used to register a widget with SageBox.
    // The primary use for this is for the widget to register with Sagebox when called by Sagebox.
    // 
    // When Sagebox calls the Widget::Register() function, the widget calls this function,
    // which fille the provided RegistryID with an ID for the widget that it can use to work with dialog boxes
    // created by Sagebox.
    //
    // Note:: this is not a user function and is used by SageBox and Widgets
    // 
	CDialog & NewDialog(int iX,int iY,int iWidth,int iHeight,const cwfOpt & cwOpt);

    // This is used to register a widget with SageBox.
    // The primary use for this is for the widget to register with Sagebox when called by Sagebox.
    // 
    // When Sagebox calls the Widget::Register() function, the widget calls this function,
    // which fille the provided RegistryID with an ID for the widget that it can use to work with dialog boxes
    // created by Sagebox.
    //
    // Note:: this is not a user function and is used by SageBox and Widgets
    // 
	CDialog & NewDialog(int iX,int iY,const char * sTitle,const cwfOpt & cwOpt = cwfOpt());

    // This is used to register a widget with SageBox.
    // The primary use for this is for the widget to register with Sagebox when called by Sagebox.
    // 
    // When Sagebox calls the Widget::Register() function, the widget calls this function,
    // which fille the provided RegistryID with an ID for the widget that it can use to work with dialog boxes
    // created by Sagebox.
    //
    // Note:: this is not a user function and is used by SageBox and Widgets
    // 
	CDialog & NewDialog(int iX,int iY,const cwfOpt & cwOpt = cwfOpt());
#endif
    /// <summary>
    /// Creates a Quick Dialog Window for creating quick dialog windows for user input, such as radio buttons, checkboxes, listboxes, etc.
    /// <para></para>
    /// This creates a dialog window with an Ok and Cancel button, to which you can add controls.  It returns a CQuickDialog object where you can add the controls and display the dialog box.
    /// <para></para>&#160;&#160;&#160;
    /// Use functions such as AddCheckboxes(), AddListbox() and other functions to add controls.  These functions will return the control (or Control Group, in the case of multiple radio buttons and checkboxes) object
    /// that can then be querired for input.
    /// <para></para>
    /// --> Use CQuickDialog::WaitforClose() to wait for the user to press OK or Cancel, after which values can be retrieved from all controls. Returns status of button pressed.
    /// <para></para>
    /// --> Use CQuickDialog::Show() to show the dialog box.  After this, you can use the GetEvent() loop or SageEvent message in your message handler to query CQuickDialog::StatusChanged() to determine of the user has pressed a button or closed
    /// the window.   This method allows you to dynamically look at control events while the user is using the dialog box (i.e. such as reacting to Checkbox presses, etc.)
    /// --> To re-use the dialog box, use CQuickDialog.ResetStatus() and then re-use WaitforClose() or Show()
    /// --> Use CDialog::Close() to close the dialog box.
    /// --> Use Modal() option to make dialog modal (disables parent window until user closes the window by pressing a button or closing it with the 'X' button)
    /// </summary>
    /// <param name="sHeader">(optional) top informational text header in the window.  This may be multi-line.  Set first character in first line as '+' to set title of window.</param>
    /// <param name="cwOpt">(optional).  Options passed to QuickDialog (i.e. Modal(), Title()</param>
    /// <returns></returns>
    CQuickDialog & QuickDialog(const char * sHeader = nullptr,const cwfOpt & cwOpt = CWindow::cwNoOpt);

    /// <summary>
    /// Creates a Quick Dialog Window for creating quick dialog windows for user input, such as radio buttons, checkboxes, listboxes, etc.
    /// <para></para>
    /// This creates a dialog window with an Ok and Cancel button, to which you can add controls.  It returns a CQuickDialog object where you can add the controls and display the dialog box.
    /// <para></para>&#160;&#160;&#160;
    /// Use functions such as AddCheckboxes(), AddListbox() and other functions to add controls.  These functions will return the control (or Control Group, in the case of multiple radio buttons and checkboxes) object
    /// that can then be querired for input.
    /// <para></para>
    /// --> Use CQuickDialog::WaitforClose() to wait for the user to press OK or Cancel, after which values can be retrieved from all controls. Returns status of button pressed.
    /// <para></para>
    /// --> Use CQuickDialog::Show() to show the dialog box.  After this, you can use the GetEvent() loop or SageEvent message in your message handler to query CQuickDialog::StatusChanged() to determine of the user has pressed a button or closed
    /// the window.   This method allows you to dynamically look at control events while the user is using the dialog box (i.e. such as reacting to Checkbox presses, etc.)
    /// --> To re-use the dialog box, use CQuickDialog.ResetStatus() and then re-use WaitforClose() or Show()
    /// --> Use CDialog::Close() to close the dialog box.
    /// --> Use Modal() option to make dialog modal (disables parent window until user closes the window by pressing a button or closing it with the 'X' button)
    /// </summary>
    /// <param name="sHeader">(optional) top informational text header in the window.  This may be multi-line.  Set first character in first line as '+' to set title of window.</param>
    /// <param name="cwOpt">(optional).  Options passed to QuickDialog (i.e. Modal(), Title()</param>
    /// <returns></returns>
    CQuickDialog & QuickDialog(const cwfOpt & cwOpt);

	// dialog functions. See documentation by pressing "dialog." for function list, or see CWindow for documentation
	//
	CWindow::WinDialog dialog;

    // EventLoop() -- Wait for a user event.  Even though the main SageBox window is not visible, all other controls
	// run through this EventLoop.
    //
    // This is the Main Event Loop for procedurally-driven programs to capture events without using event callbacks.
    //
    // EventLoop() returns for relavent events from the user.  It does not return for all Windows events, only those that affect the running of the
    // program.  All Windows message events can be intercepted by subclassing the window or using SetMessageHandler() to capture all messages.
    //
    //
    // EventLoop() returns if the window is closing with the WaitEvent::WindowClosing status. 
	// For the main (hidden) SageBox window, this must be exited to close the program since the window
	// does not display and there is now way to close it by choice.
    // 
    // Important Note:  Make sure EventLoop() does not return until it sees events.  With empty windows or corrupted windows, EventLoop()
    // can enter a processor-using wild loop.   When testing, it is a good idea to have printfs() to the window or console to make sure
    // only events are returned and it is not caught in a spining loop. 
    //
	bool EventLoop(Sage::WaitEvent * eStatus = nullptr); 
    
    // GetEvent() -- Wait for a user event.  Even though the main SageBox window is not visible, all other controls
	// run through this EventLoop.
    //
    // This is the Main Event Loop for procedurally-driven programs to capture events without using event callbacks.
    //
    // EventLoop() returns for relavent events from the user.  It does not return for all Windows events, only those that affect the running of the
    // program.  All Windows message events can be intercepted by subclassing the window or using SetMessageHandler() to capture all messages.
    //
    //
    // EventLoop() returns if the window is closing with the WaitEvent::WindowClosing status. 
	// For the main (hidden) SageBox window, this must be exited to close the program since the window
	// does not display and there is now way to close it by choice.
    // 
    // Important Note:  Make sure EventLoop() does not return until it sees events.  With empty windows or corrupted windows, EventLoop()
    // can enter a processor-using wild loop.   When testing, it is a good idea to have printfs() to the window or console to make sure
    // only events are returned and it is not caught in a spining loop. 
    //
    static bool GetEvent(bool bCheckOpenWindows = true);
   // static bool GetEvent(Sage::WaitEvent * eStatus = nullptr); // $$ Deprecated

    // QuickButton() -- Put up a quick button and information line. 
    //
    // This is used for quick Input-and-Go buttons, to stop program execution and get a quick button to continue.
    // QuickButton() simply puts up a "press to continue" message with an Ok button.
    //
    // The text can be is changed by setting sText.
    // The ProgramName (if it is set) is set in the title bar, unless sTitleBar is set other text
    //
    // QuickInfoButton() is similar to dialog.Info(), but uses a smaller font.
    // 
    // Multiple lines may be entered with '\n', and long lines are automatically broken into multiple lines
    //
	static dllvirt void QuickButton(const char * sText = nullptr,const char * sTitleBar = nullptr) dllvoid;

    // WinMessageBox() -- Bring up a standard Windows Message Box. 
    //
    // This follows the same format as ::MessageBox() in windows and simply calls it in the same way. 
    //
    // This allows more "Windows-like" messages, information, warnings, etc.
    //
    // Example: WinMessageBox("This is the message","This is the title",MB_OK | MB_ICONINFO)
    //
	static int WinMessageBox(const char * sMessage,const char * sTitle = nullptr,unsigned int dwFlags = MB_OK);

    // Sets the Program/Application name.  This can also be set when initiating Sagebox, i.e. CSageBox("My Application"); 
    //
    // The Application name is used in various window title bars in SageBox.  With no Program Name set, either the window title is blank or a default
    // text string is used.
    //
	static void SetProgramName(const char * sAppName); 

    // Gets the program name set in SageBox through either the SageBox constructor (i.e. CSageBox("My Application") or through GetProgramName();
    //
    static const char * GetProgramName();

    // GetInteger() -- Creates an Edit Box at the current console cursor (X,Y) point and waits for the user to enter an integer
    //
    // Pressing <CR> or the window closing will return from GetInteger().  Empty input (ESC or empty line) is treated as a 0 unless it is disabled with options listed below.
    //
        // GetInteger() will return immediately of WindowClosing() is true, allowing multiple console input functions in the code, as they will all fall through so you may
    // handle WindowClosing() without handling multiple Console Input functions
    //
    // --> iDefault -- the iDefault parameter sets the default value for input, and will set the input caret at the end of the line.
    //
    // Parameters in sControl
    //
    // --> Min()            -- Set minimum value.  A value entered below this value will generate an error and prompt the user to enter another value
    // --> Max()            -- Set maximum value.  A value entered below this value will generate an error and prompt the user to enter another value
    // --> Range()          -- Set minimum and maximum value.  A value entered outside this range will generate an error and prompt the user to enter another value
    // --> WinColors()      -- Sets a color scheme to a the current window colors, blending the box (i.e. there is no input box)
    // --> ColorsBW()       -- (default) Sets a color scheme to a white background with black text, allowing the box to stand out. The default color is the current windows background color
    //                         and text color, showing the input as the same color of the window text displayed in the window.
    //
    // --> fgColor()        -- Sets the color to any defined color in the system, i.e. "TextColor=Red", or TextColor=MyDefinedColor" this may be abbreviated as "fgColor=" or "bg="
    // --> bgColor()        -- Sets the background color for the text, i.e. "BgColor=lightgray" or "BgColor=MyDefinedColor"
    // --> NoEscDefault()   -- When set, pressing the ESC will clear the entire line and not reset to any default text
    // --> NoEsc()          -- When set, the ESC key does nothing.  Default behavior causes the ESC key to either clear the text line or reset it to the default text (see: "NoEscDefault")
    // --> AddBorder()      -- Put a border around the input box.  Default behavior is no border.  "Border" puts a small border around the input box for Clarity.
    // --> ThickBorder()    -- Puts a thick border around the input box, appearing more like a window rather than an input box.
    // --> NoBlankEntry()   -- Does not return unless some text is entered (i.e. will not return on a blank line).  If the Window is closing (WindowClosing() is true), GetString() will return regardless.
    //
	__sagestatic int GetInteger(const char * sTitle,bool & bCancelled,const cwfOpt & cwOptions = cwfOpt());

    // GetInteger() -- Creates an Edit Box at the current console cursor (X,Y) point and waits for the user to enter an integer
    //
    // Pressing <CR> or the window closing will return from GetInteger().  Empty input (ESC or empty line) is treated as a 0 unless it is disabled with options listed below.
    //
        // GetInteger() will return immediately of WindowClosing() is true, allowing multiple console input functions in the code, as they will all fall through so you may
    // handle WindowClosing() without handling multiple Console Input functions
    //
    // --> iDefault -- the iDefault parameter sets the default value for input, and will set the input caret at the end of the line.
    //
    // Parameters in sControl
    //
    // --> Min()            -- Set minimum value.  A value entered below this value will generate an error and prompt the user to enter another value
    // --> Max()            -- Set maximum value.  A value entered below this value will generate an error and prompt the user to enter another value
    // --> Range()          -- Set minimum and maximum value.  A value entered outside this range will generate an error and prompt the user to enter another value
    // --> WinColors()      -- Sets a color scheme to a the current window colors, blending the box (i.e. there is no input box)
    // --> ColorsBW()       -- (default) Sets a color scheme to a white background with black text, allowing the box to stand out. The default color is the current windows background color
    //                         and text color, showing the input as the same color of the window text displayed in the window.
    //
    // --> fgColor()        -- Sets the color to any defined color in the system, i.e. "TextColor=Red", or TextColor=MyDefinedColor" this may be abbreviated as "fgColor=" or "bg="
    // --> bgColor()        -- Sets the background color for the text, i.e. "BgColor=lightgray" or "BgColor=MyDefinedColor"
    // --> NoEscDefault()   -- When set, pressing the ESC will clear the entire line and not reset to any default text
    // --> NoEsc()          -- When set, the ESC key does nothing.  Default behavior causes the ESC key to either clear the text line or reset it to the default text (see: "NoEscDefault")
    // --> AddBorder()      -- Put a border around the input box.  Default behavior is no border.  "Border" puts a small border around the input box for Clarity.
    // --> ThickBorder()    -- Puts a thick border around the input box, appearing more like a window rather than an input box.
    // --> NoBlankEntry()   -- Does not return unless some text is entered (i.e. will not return on a blank line).  If the Window is closing (WindowClosing() is true), GetString() will return regardless.
    //
	__sagestatic int GetInteger(const char * sTitle,const cwfOpt & cwOptions = cwfOpt());

    // GetInteger() -- Creates an Edit Box at the current console cursor (X,Y) point and waits for the user to enter an integer
    //
    // Pressing <CR> or the window closing will return from GetInteger().  Empty input (ESC or empty line) is treated as a 0 unless it is disabled with options listed below.
    //
        // GetInteger() will return immediately of WindowClosing() is true, allowing multiple console input functions in the code, as they will all fall through so you may
    // handle WindowClosing() without handling multiple Console Input functions
    //
    // --> iDefault -- the iDefault parameter sets the default value for input, and will set the input caret at the end of the line.
    //
    // Parameters in sControl
    //
    // --> Min()            -- Set minimum value.  A value entered below this value will generate an error and prompt the user to enter another value
    // --> Max()            -- Set maximum value.  A value entered below this value will generate an error and prompt the user to enter another value
    // --> Range()          -- Set minimum and maximum value.  A value entered outside this range will generate an error and prompt the user to enter another value
    // --> WinColors()      -- Sets a color scheme to a the current window colors, blending the box (i.e. there is no input box)
    // --> ColorsBW()       -- (default) Sets a color scheme to a white background with black text, allowing the box to stand out. The default color is the current windows background color
    //                         and text color, showing the input as the same color of the window text displayed in the window.
    //
    // --> fgColor()        -- Sets the color to any defined color in the system, i.e. "TextColor=Red", or TextColor=MyDefinedColor" this may be abbreviated as "fgColor=" or "bg="
    // --> bgColor()        -- Sets the background color for the text, i.e. "BgColor=lightgray" or "BgColor=MyDefinedColor"
    // --> NoEscDefault()   -- When set, pressing the ESC will clear the entire line and not reset to any default text
    // --> NoEsc()          -- When set, the ESC key does nothing.  Default behavior causes the ESC key to either clear the text line or reset it to the default text (see: "NoEscDefault")
    // --> AddBorder()      -- Put a border around the input box.  Default behavior is no border.  "Border" puts a small border around the input box for Clarity.
    // --> ThickBorder()    -- Puts a thick border around the input box, appearing more like a window rather than an input box.
    // --> NoBlankEntry()   -- Does not return unless some text is entered (i.e. will not return on a blank line).  If the Window is closing (WindowClosing() is true), GetString() will return regardless.
    //
	__sagestatic int GetInteger(const cwfOpt & cwOptions = cwfOpt());

    // console.GetFloat() -- Get a floating-point value from the user, in an inline console-style input (i.e. as part of a text line rather than a windowed box)
    //
    // This function will get a small set of input from the user (typical a window of 15 characters or so) and return the floating point value once the user presses return.
    //
    // Basic Forms:
    //
    // --> double MyValue = GetFloat().                        -- Get a float value with default options
    // --> double MyValue = GetFloat(123.56).                -- Get a float value with default set to 123.56
    // --> double MyValue = GetFloat(<<options>>).            -- Get a float value with an option string and no default (see examples below)
    // --> double MyValue = GetFloat(123.56,<<options>>).    -- Get a float value with a default set to 123.56 and also specify options (see examples below)
    //
    // Inputs (all optional):
    //
    //        fDefault        - Default value that will be placed in the box when it first appears.  The user can then change it
    //      sOptions        - Text-based options to control the input and output
    //        cwOptions        - opt::Option()-style options -- you can use Text or opt::-style options, depending on your preference.
    //
    // --> Example: double MyValue = GetFloat(123.56,"range=-100,100,colorsBW")
    //              double Myalue = GetFloat(123.56,opt::Range(-100,100) | opt::ColorsBW())
    //
    // Both of these examples perform the same function: Set a default of "123.56", set a range of -100,100, and tell the box to have a white background with black text
    //
    //--> Available Options
    //
    // Range            -- Range of the value (i.e. Range(-100,100)).  If the input is outside the range, an error message appears telling the user to enter a number within the range specified.
    // MinValue            -- Sets the minimum value (i.e. MinValue(-100)).  If the input is less than the MinValue, an error message appears telling the user to enter a number above the MinValue
    // MaxValue            -- Sets the Maximum Value allowed.  If the input is greater than the MaxValue, an error message appears elling the user to enter a number less then the MinValue
    // ColorsBW            -- Sets the background of the box to white, with black text.  This can make the box more visible.  Otherwise, the box is the windows background color and current text color
    // NoBorder            -- Default is a thin border for visibility.  the "NoBorder" option will remove the border so the text looks more like normal text in the overall text output line.
    // ThickBorder        -- Provides a recessed window that looks more like an input window, but still in the text line.
    // NoBlankLine        -- Does not allow the user to enter a blank line.  When this is off (default), when the user presses enter with a blank line, the default value is returned (or 0 when here was no default set)
    // 
    // --> Examples
    //
    // --> double MyValue = console.GetFloat()                                                 -- Gets a float with default display set and no default alue
    // --> double MyValue = console.GetFloat(123.56)                                           -- Gets a float with default display set and with default value = 123.56
    // --> double MyValue = console.GetFloat(123.56,"NoBorder,MinValue=-100");                 -- string-style options
    // --> double MyValue = console.GetFloat(123.56,opt::NoBorder() | opt::MinValue(-100))     -- opt::-style options
    //
    // ** You can also use in >> MyValue to use C++ input stream style
    //
    // --> Same examples as above (assuming MyValue is defined as "double MyValue" ahead of the call):
    //
    // --> in >> MyValue                                                                       -- Gets a float with default display set and no default alue
    // --> in >> opt::Default(123.56) >> MyValue                                               -- Gets a float with default display set and with default value = 123.56
    // --> in >> opt::Default(123.56) >> opt::NoBorder() >> opt::MinValue(-100) >> MyValue;    -- opt::-style options
    // --> in >> Default(123.56) >> NoBorder() >> MinValue(-100) >> MyValue;                   -- opt::-style options (when "using namespace opt" is specified)
    //
    // After a certain point, it makes sense to use console.GetFloat() (over stream-style "in >> MyValue) when a large amount of options are used.  
    // For simple calls and prototyping, using "in >> MyValue" or things like "in >> opt::ColorwsBW >> MyValue" can be much quicker and easier to read, but
    // can be more clear using console.GetFLoat() when many options are being used (see above examples).
    //
    // There are 4 different function prototypes so that options can be omitted when not required.
    //
	dllvirt __sagestatic double GetFloat(const char * sTitle,bool & bCancelled,const cwfOpt & cwOptions = cwfOpt()) dllfloat;

    // console.GetFloat() -- Get a floating-point value from the user, in an inline console-style input (i.e. as part of a text line rather than a windowed box)
    //
    // This function will get a small set of input from the user (typical a window of 15 characters or so) and return the floating point value once the user presses return.
    //
    // Basic Forms:
    //
    // --> double MyValue = GetFloat().                        -- Get a float value with default options
    // --> double MyValue = GetFloat(123.56).                -- Get a float value with default set to 123.56
    // --> double MyValue = GetFloat(<<options>>).            -- Get a float value with an option string and no default (see examples below)
    // --> double MyValue = GetFloat(123.56,<<options>>).    -- Get a float value with a default set to 123.56 and also specify options (see examples below)
    //
    // Inputs (all optional):
    //
    //        fDefault        - Default value that will be placed in the box when it first appears.  The user can then change it
    //      sOptions        - Text-based options to control the input and output
    //        cwOptions        - opt::Option()-style options -- you can use Text or opt::-style options, depending on your preference.
    //
    // --> Example: double MyValue = GetFloat(123.56,"range=-100,100,colorsBW")
    //              double Myalue = GetFloat(123.56,opt::Range(-100,100) | opt::ColorsBW())
    //
    // Both of these examples perform the same function: Set a default of "123.56", set a range of -100,100, and tell the box to have a white background with black text
    //
    //--> Available Options
    //
    // Range            -- Range of the value (i.e. Range(-100,100)).  If the input is outside the range, an error message appears telling the user to enter a number within the range specified.
    // MinValue            -- Sets the minimum value (i.e. MinValue(-100)).  If the input is less than the MinValue, an error message appears telling the user to enter a number above the MinValue
    // MaxValue            -- Sets the Maximum Value allowed.  If the input is greater than the MaxValue, an error message appears elling the user to enter a number less then the MinValue
    // ColorsBW            -- Sets the background of the box to white, with black text.  This can make the box more visible.  Otherwise, the box is the windows background color and current text color
    // NoBorder            -- Default is a thin border for visibility.  the "NoBorder" option will remove the border so the text looks more like normal text in the overall text output line.
    // ThickBorder        -- Provides a recessed window that looks more like an input window, but still in the text line.
    // NoBlankLine        -- Does not allow the user to enter a blank line.  When this is off (default), when the user presses enter with a blank line, the default value is returned (or 0 when here was no default set)
    // 
    // --> Examples
    //
    // --> double MyValue = console.GetFloat()                                                 -- Gets a float with default display set and no default alue
    // --> double MyValue = console.GetFloat(123.56)                                           -- Gets a float with default display set and with default value = 123.56
    // --> double MyValue = console.GetFloat(123.56,"NoBorder,MinValue=-100");                 -- string-style options
    // --> double MyValue = console.GetFloat(123.56,opt::NoBorder() | opt::MinValue(-100))     -- opt::-style options
    //
    // ** You can also use in >> MyValue to use C++ input stream style
    //
    // --> Same examples as above (assuming MyValue is defined as "double MyValue" ahead of the call):
    //
    // --> in >> MyValue                                                                       -- Gets a float with default display set and no default alue
    // --> in >> opt::Default(123.56) >> MyValue                                               -- Gets a float with default display set and with default value = 123.56
    // --> in >> opt::Default(123.56) >> opt::NoBorder() >> opt::MinValue(-100) >> MyValue;    -- opt::-style options
    // --> in >> Default(123.56) >> NoBorder() >> MinValue(-100) >> MyValue;                   -- opt::-style options (when "using namespace opt" is specified)
    //
    // After a certain point, it makes sense to use console.GetFloat() (over stream-style "in >> MyValue) when a large amount of options are used.  
    // For simple calls and prototyping, using "in >> MyValue" or things like "in >> opt::ColorwsBW >> MyValue" can be much quicker and easier to read, but
    // can be more clear using console.GetFLoat() when many options are being used (see above examples).
    //
    // There are 4 different function prototypes so that options can be omitted when not required.
    //
    dllvirt __sagestatic double GetFloat(const char * sTitle,const cwfOpt & cwOptions = cwfOpt());

    // console.GetFloat() -- Get a floating-point value from the user, in an inline console-style input (i.e. as part of a text line rather than a windowed box)
    //
    // This function will get a small set of input from the user (typical a window of 15 characters or so) and return the floating point value once the user presses return.
    //
    // Basic Forms:
    //
    // --> double MyValue = GetFloat().                        -- Get a float value with default options
    // --> double MyValue = GetFloat(123.56).                -- Get a float value with default set to 123.56
    // --> double MyValue = GetFloat(<<options>>).            -- Get a float value with an option string and no default (see examples below)
    // --> double MyValue = GetFloat(123.56,<<options>>).    -- Get a float value with a default set to 123.56 and also specify options (see examples below)
    //
    // Inputs (all optional):
    //
    //        fDefault        - Default value that will be placed in the box when it first appears.  The user can then change it
    //      sOptions        - Text-based options to control the input and output
    //        cwOptions        - opt::Option()-style options -- you can use Text or opt::-style options, depending on your preference.
    //
    // --> Example: double MyValue = GetFloat(123.56,"range=-100,100,colorsBW")
    //              double Myalue = GetFloat(123.56,opt::Range(-100,100) | opt::ColorsBW())
    //
    // Both of these examples perform the same function: Set a default of "123.56", set a range of -100,100, and tell the box to have a white background with black text
    //
    //--> Available Options
    //
    // Range            -- Range of the value (i.e. Range(-100,100)).  If the input is outside the range, an error message appears telling the user to enter a number within the range specified.
    // MinValue            -- Sets the minimum value (i.e. MinValue(-100)).  If the input is less than the MinValue, an error message appears telling the user to enter a number above the MinValue
    // MaxValue            -- Sets the Maximum Value allowed.  If the input is greater than the MaxValue, an error message appears elling the user to enter a number less then the MinValue
    // ColorsBW            -- Sets the background of the box to white, with black text.  This can make the box more visible.  Otherwise, the box is the windows background color and current text color
    // NoBorder            -- Default is a thin border for visibility.  the "NoBorder" option will remove the border so the text looks more like normal text in the overall text output line.
    // ThickBorder        -- Provides a recessed window that looks more like an input window, but still in the text line.
    // NoBlankLine        -- Does not allow the user to enter a blank line.  When this is off (default), when the user presses enter with a blank line, the default value is returned (or 0 when here was no default set)
    // 
    // --> Examples
    //
    // --> double MyValue = console.GetFloat()                                                 -- Gets a float with default display set and no default alue
    // --> double MyValue = console.GetFloat(123.56)                                           -- Gets a float with default display set and with default value = 123.56
    // --> double MyValue = console.GetFloat(123.56,"NoBorder,MinValue=-100");                 -- string-style options
    // --> double MyValue = console.GetFloat(123.56,opt::NoBorder() | opt::MinValue(-100))     -- opt::-style options
    //
    // ** You can also use in >> MyValue to use C++ input stream style
    //
    // --> Same examples as above (assuming MyValue is defined as "double MyValue" ahead of the call):
    //
    // --> in >> MyValue                                                                       -- Gets a float with default display set and no default alue
    // --> in >> opt::Default(123.56) >> MyValue                                               -- Gets a float with default display set and with default value = 123.56
    // --> in >> opt::Default(123.56) >> opt::NoBorder() >> opt::MinValue(-100) >> MyValue;    -- opt::-style options
    // --> in >> Default(123.56) >> NoBorder() >> MinValue(-100) >> MyValue;                   -- opt::-style options (when "using namespace opt" is specified)
    //
    // After a certain point, it makes sense to use console.GetFloat() (over stream-style "in >> MyValue) when a large amount of options are used.  
    // For simple calls and prototyping, using "in >> MyValue" or things like "in >> opt::ColorwsBW >> MyValue" can be much quicker and easier to read, but
    // can be more clear using console.GetFLoat() when many options are being used (see above examples).
    //
    // There are 4 different function prototypes so that options can be omitted when not required.
    //
    dllvirt __sagestatic double GetFloat(const cwfOpt & cwOptions = cwfOpt());


    // GetString() -- Creates an Edit Box at the current console cursor (X,Y) point and waits for the user to enter text an press return.
    //
    // Pressing <CR> or the window closing will return from GetString().  Empty input is defined by a "" string.  Check return value [0] == 0 for no string entry. Unlike some functions, NULLPTR is not returned. 
    //
    // Pressing ESC will clear the text entry, remaining in the edit box for more input.  When there is a default set of text, the text will revert to the original default.
    //
    // GetString() will return immediately of WindowClosing() is true, allowing multiple console input functions in the code, as they will all fall through so you may
    // handle WindowClosing() without handling multiple Console Input functions
    //
    // --> sDefaut -- the sDefault parameter sets the default text for the text input, and will set the input caret at the end of the line.
    //
    // Parameters in sControl
    //
    // --> WinColors()      -- Sets a color scheme to a the current window colors, blending the box (i.e. there is no input box)
    // --> ColorsBW()       -- (default) Sets a color scheme to a white background with black text, allowing the box to stand out. The default color is the current windows background color
    //                         and text color, showing the input as the same color of the window text displayed in the window.
    //
    // --> fgColor()        -- Sets the color to any defined color in the system, i.e. "TextColor=Red", or TextColor=MyDefinedColor" this may be abbreviated as "fgColor=" or "bg="
    // --> bgColor()        -- Sets the background color for the text, i.e. "BgColor=lightgray" or "BgColor=MyDefinedColor"
    // --> Width()          -- Sets the width of the window in pixels.  The default sets the input window to the remainer of the window width from 
    //                         the start point (current console write position), which may seem too big.  "Width=100" sets the box with to 100 pixels, "With=50" sets it to 50, etc. 
    // --> CharWidth()      -- Sets the width of the window to number of chars in "CharWidth" based on average character width for the current font.
    //                         For example, "CharWidth=10" sizes the edit box to the width of 10 characters for the current font. This setting does not affect the number of characters
    //                         entered in the box, unless the "noscroll" option is set. 
    //                         If the "noscroll" option is set, input stops at the end of the box. Otherwise, the text will scroll horizontally as more text is entered
    // --> MaxChars()       -- Sets maximum number of characters that can be entered, regardless of window size (overrules "CharWidth" when "CharWidth" allows more characters
    //                         than "MaxChars".  For example, "MaxChars=10" will allow no more than 10 characters to be entered, regardless of input box size.
    // --> AllowScroll()    -- Allows horizontal scrolling in the input box
    //                         box.  Default behavior is to scroll text horizontally so that any number of characters can be entered in any size input box.
    // --> NoEscDefault()   -- When set, pressing the ESC will clear the entire line and not reset to any default text
    // --> NoEsc()          -- When set, the ESC key does nothing.  Default behavior causes the ESC key to either clear the text line or reset it to the default text (see: "NoEscDefault")
    // --> AddBorder()      -- Put a border around the input box.  Default behavior is no border.  "Border" puts a small border around the input box for Clarity.
    // --> ThickBorder()    -- Puts a thick border around the input box, appearing more like a window rather than an input box.
    // --> NoBlankEntry()   -- Does not return unless some text is entered (i.e. will not return on a blank line).  If the Window is closing (WindowClosing() is true), GetString() will return regardless.
    //
    // Examples:
    //
    // auto MyString = MyWindow.Console.GetString();    -- Gets a string using defaults
    // auto MyString = MyConsole.GetString(SageOpt_Colors("bw") SageOpt_CharWidth(10) SageOpt_BgColor("Yellow") SageOpt_BlankDefault);
    //
	__sagestatic CString GetString(const char * sTitle,bool & bCancelled,const cwfOpt & cwOptions = cwfOpt());

    // GetString() -- Creates an Edit Box at the current console cursor (X,Y) point and waits for the user to enter text an press return.
    //
    // Pressing <CR> or the window closing will return from GetString().  Empty input is defined by a "" string.  Check return value [0] == 0 for no string entry. Unlike some functions, NULLPTR is not returned. 
    //
    // Pressing ESC will clear the text entry, remaining in the edit box for more input.  When there is a default set of text, the text will revert to the original default.
    //
    // GetString() will return immediately of WindowClosing() is true, allowing multiple console input functions in the code, as they will all fall through so you may
    // handle WindowClosing() without handling multiple Console Input functions
    //
    // --> sDefaut -- the sDefault parameter sets the default text for the text input, and will set the input caret at the end of the line.
    //
    // Parameters in sControl
    //
    // --> WinColors()      -- Sets a color scheme to a the current window colors, blending the box (i.e. there is no input box)
    // --> ColorsBW()       -- (default) Sets a color scheme to a white background with black text, allowing the box to stand out. The default color is the current windows background color
    //                         and text color, showing the input as the same color of the window text displayed in the window.
    //
    // --> fgColor()        -- Sets the color to any defined color in the system, i.e. "TextColor=Red", or TextColor=MyDefinedColor" this may be abbreviated as "fgColor=" or "bg="
    // --> bgColor()        -- Sets the background color for the text, i.e. "BgColor=lightgray" or "BgColor=MyDefinedColor"
    // --> Width()          -- Sets the width of the window in pixels.  The default sets the input window to the remainer of the window width from 
    //                         the start point (current console write position), which may seem too big.  "Width=100" sets the box with to 100 pixels, "With=50" sets it to 50, etc. 
    // --> CharWidth()      -- Sets the width of the window to number of chars in "CharWidth" based on average character width for the current font.
    //                         For example, "CharWidth=10" sizes the edit box to the width of 10 characters for the current font. This setting does not affect the number of characters
    //                         entered in the box, unless the "noscroll" option is set. 
    //                         If the "noscroll" option is set, input stops at the end of the box. Otherwise, the text will scroll horizontally as more text is entered
    // --> MaxChars()       -- Sets maximum number of characters that can be entered, regardless of window size (overrules "CharWidth" when "CharWidth" allows more characters
    //                         than "MaxChars".  For example, "MaxChars=10" will allow no more than 10 characters to be entered, regardless of input box size.
    // --> AllowScroll()    -- Allows horizontal scrolling in the input box
    //                         box.  Default behavior is to scroll text horizontally so that any number of characters can be entered in any size input box.
    // --> NoEscDefault()   -- When set, pressing the ESC will clear the entire line and not reset to any default text
    // --> NoEsc()          -- When set, the ESC key does nothing.  Default behavior causes the ESC key to either clear the text line or reset it to the default text (see: "NoEscDefault")
    // --> AddBorder()      -- Put a border around the input box.  Default behavior is no border.  "Border" puts a small border around the input box for Clarity.
    // --> ThickBorder()    -- Puts a thick border around the input box, appearing more like a window rather than an input box.
    // --> NoBlankEntry()   -- Does not return unless some text is entered (i.e. will not return on a blank line).  If the Window is closing (WindowClosing() is true), GetString() will return regardless.
    //
    // Examples:
    //
    // auto MyString = MyWindow.Console.GetString();    -- Gets a string using defaults
    // auto MyString = MyConsole.GetString(SageOpt_Colors("bw") SageOpt_CharWidth(10) SageOpt_BgColor("Yellow") SageOpt_BlankDefault);
    //
	__sagestatic CString GetString(const char * sTitle,const cwfOpt & cwOptions = cwfOpt());

    // Duplication of C++ console-mode getline()
    //
    // This is the same as GetString() except that the sring input is specified in the function call.
    // This is to duplicate the C++ getline() function.
    //
    // CString and std::string are supported.
    //
    // See GetString() for option usage.
    //
    void getline(CString & cString,const cwfOpt & cwOpt = cwfOpt());

    // Duplication of C++ console-mode getline()
    //
    // This is the same as GetString() except that the sring input is specified in the function call.
    // This is to duplicate the C++ getline() function.
    //
    // CString and std::string are supported.
    //
    // See GetString() for option usage.
    //
	void getline(std::string & cString,const cwfOpt & cwOpt = cwfOpt());

	// Dialog Info/Question Windows

	__sagestatic void InfoWindow(const char * sTitle,const cwfOpt & cwOptions = cwfOpt());
	__sagestatic bool YesNoWindow(const char * sTitle,const cwfOpt & cwOptions = cwfOpt());
	__sagestatic Sage::DialogResult YesNoCancelWindow(const char * sTitle,const cwfOpt & cwOptions = cwfOpt());
	__sagestatic bool OkCancelWindow(const char * sTitle,const cwfOpt & cwOptions = cwfOpt());


    // PleaseWaitWindow() - Open Please Wait Window
    //
    // Input String:
    //
    // --> Input String can be text-based, i.e. PleaseWaitWindow("ProgressBar,HideCancel") or Opt Strings, i.e. PleaseWaitWindow(opts::ProgressBar() | opt::HideCancel());
    // ----> One string puts a subtitle under "Please Wait...", i.e. PleaseWaitWindow("System is doing something"); 
    // ----> Two Strings replaces "Please Wait.." with text, then puts sub-title for second line, i.e. PleaseWaitWindow("Wait a few seconds...\nSystem is doing something");
    // ----> Inserting a Sring with '+' changes title bar text, i.e. PleaseWaitWindow('+My Program\nWait a few seconds...\nSystem is doing something") sets "My Program" as small window title.
    //
    // Options:
    //
    // ----> CancelOk            - Adds a Cancel button.  This can be chacked with PleaseWaitCancelled()
    // ----> HideCancel            - Adds a hidden cancel button that can be shown or re-hidden.  Can be used to delay cancel ability. Also Useful with PleaseWaitGetOk().
    // ----> ProgressBar        - Adds a Progress Bar that can be updated with SetPleaseWaitProgress()
    //
    // Examples:
    // ----> PleaseWaitWindow();    -- Opens a simple please wait window, with no button.  Closed with ClosePleaseWaitWindow();
    // ----> PleaseWaitWindow(opt::ProgressBar()) or PleaseWaitWindow("progressbar") -- Opens a Please Wait Window with a progress bar
    // ----> PleaseWaitWindow("System is Calculating Information",opt::CancelOk()) -- Opens a Please Wait Window with a subtitle and cancel button (which may be checked with PleaseWaitCancelled())
    // ----> Typically accessed as a dialog function, i.e. MyWindow->dialog.PleaseWaitWindow();
    //
	__sagestatic void PleaseWaitWindow(const char * sText,const cwfOpt cwOptions);

    // PleaseWaitWindow() - Open Please Wait Window
    //
    // Input String:
    //
    // --> Input String can be text-based, i.e. PleaseWaitWindow("ProgressBar,HideCancel") or Opt Strings, i.e. PleaseWaitWindow(opts::ProgressBar() | opt::HideCancel());
    // ----> One string puts a subtitle under "Please Wait...", i.e. PleaseWaitWindow("System is doing something"); 
    // ----> Two Strings replaces "Please Wait.." with text, then puts sub-title for second line, i.e. PleaseWaitWindow("Wait a few seconds...\nSystem is doing something");
    // ----> Inserting a Sring with '+' changes title bar text, i.e. PleaseWaitWindow('+My Program\nWait a few seconds...\nSystem is doing something") sets "My Program" as small window title.
    //
    // Options:
    //
    // ----> CancelOk            - Adds a Cancel button.  This can be chacked with PleaseWaitCancelled()
    // ----> HideCancel            - Adds a hidden cancel button that can be shown or re-hidden.  Can be used to delay cancel ability. Also Useful with PleaseWaitGetOk().
    // ----> ProgressBar        - Adds a Progress Bar that can be updated with SetPleaseWaitProgress()
    //
    // Examples:
    // ----> PleaseWaitWindow();    -- Opens a simple please wait window, with no button.  Closed with ClosePleaseWaitWindow();
    // ----> PleaseWaitWindow(opt::ProgressBar()) or PleaseWaitWindow("progressbar") -- Opens a Please Wait Window with a progress bar
    // ----> PleaseWaitWindow("System is Calculating Information",opt::CancelOk()) -- Opens a Please Wait Window with a subtitle and cancel button (which may be checked with PleaseWaitCancelled())
    // ----> Typically accessed as a dialog function, i.e. MyWindow->dialog.PleaseWaitWindow();
    //
	__sagestatic void PleaseWaitWindow(const cwfOpt cwOptions);

    // ClosePlaseWait() -- Hides and Closes opened Please Wait Window
    //
	__sagestatic void ClosePleaseWait();

    // PleaseWaitCancelled() -- Returns true if the Cancel button was pressed on the Please Wait Window.  False, if not or Please Wait Window is not active.
    //
	__sagestatic bool PleaseWaitCancelled(Peek peek = Peek::No);

    // PleaseWaitGetOk() -- Changes "Cancel" button to "Ok" button and waits for the button to be pressed.  This is useful when a function has completed and you want to
    //                      make sure the user knows it is finish.
    //
    // This function returns TRUE when the butto is pressed, if the Cancel button exists (i.e. either "CancelOk" or "HideCancel" was specified).  Otherwise, TRUE is returned immediately
    //
	__sagestatic bool PleaseWaitGetOk();

    // PleaseWaitShowCancel() -- Shows (or Hides) the Cancel button.   If opt::CancelOk() or opt::HideCancel() is set in the PleaseWaitWindow() call, PleaseWaitShowCancel() will
    // hide or show the cancel button. 
    //
    // Input:
    //
    // PleaseWaitShowCancel(true) -- Shows the cancel button
    // PleaseWaitShowCancel(false) -- Hide the cancel button
    //
	__sagestatic bool PleaseWaitShowCancel(bool bShow = true);

    // SetPleaseWaitProgress() -- Sets the progress bar percent on the Please Wait Window
    //
    // This will set the bar to the percentage specified. For example, PleaseWaitProgress(57) will set the progress bar to 57% and print "57%" in the middle of the progress bar.
    //
    // Input:
    // --> iPercent: Percent completed, from 0-100
    // --> sMessage: When specified, replaces the percent display with the message.  This is useful for initialization or completion, for example:
    //      SetPleaseWaitProgress(100,"Finished.") will place "finished" on the progress bar instead of 100%
    //
    // Example:
    //    ----> SetPleaseWaitProgress(10,"Finished.");
    //    ----> PleaseWaitGetOk(); 
    //
    // --> These two lines show the progress bar full with the label "finished" within the progress bar.  The second line then changes the "Cancel" button to "Ok" and waits for the user to press it.
    //
    __sagestatic void SetPleaseWaitProgress(int iPercent,char * sMessage = nullptr);


    // Open a file through the windows dialog, allowing setting title, file types, and other criteria.
    // 
    // CString comes back empty if no file is chosen.
    // If CString is specified in the function, a bool is returned: TRUE if there is a filename,
    // FALSE if aborted by the user.
    // 
    // Use GetOpenFleStruct() or declare stOpenFileStruct to fill the structure.
    //
    // See notes on stOpenFilestruct to fill the requirements properly.
    //
    // This blocks program execution until the user selects a file or cancels.
    //
    // For a quick open, use GetOpenFile() with only file types, such as GetOpenFile("*.bmp;*.jpg"), etc.
    // Note: Use ';' to separate types.
    //
	__sagestatic CString GetOpenFile(Sage::stOpenFileStruct & stFile);

    // Open a file through the windows dialog, allowing setting title, file types, and other criteria.
    // 
    // CString comes back empty if no file is chosen.
    // If CString is specified in the function, a bool is returned: TRUE if there is a filename,
    // FALSE if aborted by the user.
    // 
    // Use GetOpenFleStruct() or declare stOpenFileStruct to fill the structure.
    //
    // See notes on stOpenFilestruct to fill the requirements properly.
    //
    // This blocks program execution until the user selects a file or cancels.
    //
    // For a quick open, use GetOpenFile() with only file types, such as GetOpenFile("*.bmp;*.jpg"), etc.
    // Note: Use ';' to separate types.
    //
    __sagestatic CString GetOpenFile(const char * sTypes = nullptr);
	
    // Open a file through the windows dialog, allowing setting title, file types, and other criteria.
    // 
    // CString comes back empty if no file is chosen.
    // If CString is specified in the function, a bool is returned: TRUE if there is a filename,
    // FALSE if aborted by the user.
    // 
    // Use GetOpenFleStruct() or declare stOpenFileStruct to fill the structure.
    //
    // See notes on stOpenFilestruct to fill the requirements properly.
    //
    // This blocks program execution until the user selects a file or cancels.
    //
    // For a quick open, use GetOpenFile() with only file types, such as GetOpenFile("*.bmp;*.jpg"), etc.
    // Note: Use ';' to separate types.
    //
	__sagestatic bool GetOpenFile(Sage::stOpenFileStruct & stFile,CString & csFilename);
    // Open a file through the windows dialog, allowing setting title, file types, and other criteria.
    // 
    // CString comes back empty if no file is chosen.
    // If CString is specified in the function, a bool is returned: TRUE if there is a filename,
    // FALSE if aborted by the user.
    // 
    // Use GetOpenFleStruct() or declare stOpenFileStruct to fill the structure.
    //
    // See notes on stOpenFilestruct to fill the requirements properly.
    //
    // This blocks program execution until the user selects a file or cancels.
    //
    // For a quick open, use GetOpenFile() with only file types, such as GetOpenFile("*.bmp;*.jpg"), etc.
    // Note: Use ';' to separate types.
    //
	__sagestatic bool GetOpenFile(const char * sTypes,CString & csFilename);

    // Open a file through the windows dialog, allowing setting title, file types, and other criteria.
    // 
    // CString comes back empty if no file is chosen.
    // If CString is specified in the function, a bool is returned: TRUE if there is a filename,
    // FALSE if aborted by the user.
    // 
    // Use GetOpenFleStruct() or declare stOpenFileStruct to fill the structure.
    //
    // See notes on stOpenFilestruct to fill the requirements properly.
    //
    // This blocks program execution until the user selects a file or cancels.
    //
    // For a quick open, use GetOpenFile() with only file types, such as GetOpenFile("*.bmp;*.jpg"), etc.
    // Note: Use ';' to separate types.
    //
	__sagestatic bool GetOpenFile(CString & csFilename);

    // GetOpenFileStruct() -- Return an stOpenFileStruct to set for using an GetOpenFile() or GetSaveFile() dialog.
    //
    // This is a shortcut for declaring a structure directly, so that auto& stStruct = GetOpenFileStruct() can be used
    // more easily.
    //
    __sagestatic Sage::stOpenFileStruct GetOpenFileStruct();

    // QuickThumbnail() -- Create and display a window with a thumbnail of bitmap data.
    //
    // This is used for development to show thumbnails of bitmap data in memory, in various forms and in various ways.
    //
    // The bitmap can be display by percentage, maximum width, height, etc.
    // See Sage::ThumbType for all variations
    //
    // If there is an error in the data (i.e. an empty bitmap), the window is not opened.
    // 
    // A RawBitmap_t or CBitmap must currently be provided.
    // Raw data and more data types (such as float, float mono, 16-bit bitmaps, etc.) will be supported in a future release.
    //
     __sagestatic bool QuickThumbnail(Sage::RawBitmap_t & stBitmap,int iWidth,int iHeight,Sage::ThumbType eType = ThumbType::BestFit,const char * sTitle = nullptr);

    // QuickThumbnail() -- Create and display a window with a thumbnail of bitmap data.
    //
    // This is used for development to show thumbnails of bitmap data in memory, in various forms and in various ways.
    //
    // The bitmap can be display by percentage, maximum width, height, etc.
    // See Sage::ThumbType for all variations
    //
    // If there is an error in the data (i.e. an empty bitmap), the window is not opened.
    // 
    // A RawBitmap_t or CBitmap must currently be provided.
    // Raw data and more data types (such as float, float mono, 16-bit bitmaps, etc.) will be supported in a future release.
    //
	 __sagestatic bool QuickThumbnail(Sage::RawBitmap_t & stBitmap,int iWidth,int iHeight,const char * sTitle);

        // QuickThumbnail() -- Create and display a window with a thumbnail of bitmap data.
    //
    // This is used for development to show thumbnails of bitmap data in memory, in various forms and in various ways.
    //
    // The bitmap can be display by percentage, maximum width, height, etc.
    // See Sage::ThumbType for all variations
    //
    // If there is an error in the data (i.e. an empty bitmap), the window is not opened.
    // 
    // A RawBitmap_t or CBitmap must currently be provided.
    // Raw data and more data types (such as float, float mono, 16-bit bitmaps, etc.) will be supported in a future release.
    //
     __sagestatic bool QuickThumbnail(CBitmap & cBitmap,int iWidth,int iHeight,ThumbType eType = ThumbType::BestFit,const char * sTitle = nullptr);

    // QuickThumbnail() -- Create and display a window with a thumbnail of bitmap data.
    //
    // This is used for development to show thumbnails of bitmap data in memory, in various forms and in various ways.
    //
    // The bitmap can be display by percentage, maximum width, height, etc.
    // See Sage::ThumbType for all variations
    //
    // If there is an error in the data (i.e. an empty bitmap), the window is not opened.
    // 
    // A RawBitmap_t or CBitmap must currently be provided.
    // Raw data and more data types (such as float, float mono, 16-bit bitmaps, etc.) will be supported in a future release.
    //
	 __sagestatic bool QuickThumbnail(CBitmap & cBitmap,int iWidth,int iHeight,const char * sTitle);

//--------------

    // QuickThumbnail() -- Create and display a window with a thumbnail of bitmap data.
    //
    // This is used for development to show thumbnails of bitmap data in memory, in various forms and in various ways.
    //
    // The bitmap can be display by percentage, maximum width, height, etc.
    // See Sage::ThumbType for all variations
    //
    // If there is an error in the data (i.e. an empty bitmap), the window is not opened.
    // 
    // A RawBitmap_t or CBitmap must currently be provided.
    // Raw data and more data types (such as float, float mono, 16-bit bitmaps, etc.) will be supported in a future release.
    //
     __sagestatic bool QuickThumbnail(POINT pLoc,Sage::RawBitmap_t & stBitmap,int iWidth,int iHeight,Sage::ThumbType eType = ThumbType::BestFit,const char * sTitle = nullptr);

    // QuickThumbnail() -- Create and display a window with a thumbnail of bitmap data.
    //
    // This is used for development to show thumbnails of bitmap data in memory, in various forms and in various ways.
    //
    // The bitmap can be display by percentage, maximum width, height, etc.
    // See Sage::ThumbType for all variations
    //
    // If there is an error in the data (i.e. an empty bitmap), the window is not opened.
    // 
    // A RawBitmap_t or CBitmap must currently be provided.
    // Raw data and more data types (such as float, float mono, 16-bit bitmaps, etc.) will be supported in a future release.
    //
	 __sagestatic bool QuickThumbnail(POINT pLoc,Sage::RawBitmap_t & stBitmap,int iWidth,int iHeight,const char * sTitle);

        // QuickThumbnail() -- Create and display a window with a thumbnail of bitmap data.
    //
    // This is used for development to show thumbnails of bitmap data in memory, in various forms and in various ways.
    //
    // The bitmap can be display by percentage, maximum width, height, etc.
    // See Sage::ThumbType for all variations
    //
    // If there is an error in the data (i.e. an empty bitmap), the window is not opened.
    // 
    // A RawBitmap_t or CBitmap must currently be provided.
    // Raw data and more data types (such as float, float mono, 16-bit bitmaps, etc.) will be supported in a future release.
    //
     __sagestatic bool QuickThumbnail(POINT pLoc,CBitmap & cBitmap,int iWidth,int iHeight,ThumbType eType = ThumbType::BestFit,const char * sTitle = nullptr);

    // QuickThumbnail() -- Create and display a window with a thumbnail of bitmap data.
    //
    // This is used for development to show thumbnails of bitmap data in memory, in various forms and in various ways.
    //
    // The bitmap can be display by percentage, maximum width, height, etc.
    // See Sage::ThumbType for all variations
    //
    // If there is an error in the data (i.e. an empty bitmap), the window is not opened.
    // 
    // A RawBitmap_t or CBitmap must currently be provided.
    // Raw data and more data types (such as float, float mono, 16-bit bitmaps, etc.) will be supported in a future release.
    //
	 __sagestatic bool QuickThumbnail(POINT pLoc,CBitmap & cBitmap,int iWidth,int iHeight,const char * sTitle);

//--------------

    // QuickThumbnail() -- Create and display a window with a thumbnail of bitmap data.
    //
    // This is used for development to show thumbnails of bitmap data in memory, in various forms and in various ways.
    //
    // The bitmap can be display by percentage, maximum width, height, etc.
    // See Sage::ThumbType for all variations
    //
    // If there is an error in the data (i.e. an empty bitmap), the window is not opened.
    // 
    // A RawBitmap_t or CBitmap must currently be provided.
    // Raw data and more data types (such as float, float mono, 16-bit bitmaps, etc.) will be supported in a future release.
    //
     __sagestatic bool QuickThumbnail(int iX,int iY,Sage::RawBitmap_t & stBitmap,int iWidth,int iHeight,Sage::ThumbType eType = ThumbType::BestFit,const char * sTitle = nullptr);

    // QuickThumbnail() -- Create and display a window with a thumbnail of bitmap data.
    //
    // This is used for development to show thumbnails of bitmap data in memory, in various forms and in various ways.
    //
    // The bitmap can be display by percentage, maximum width, height, etc.
    // See Sage::ThumbType for all variations
    //
    // If there is an error in the data (i.e. an empty bitmap), the window is not opened.
    // 
    // A RawBitmap_t or CBitmap must currently be provided.
    // Raw data and more data types (such as float, float mono, 16-bit bitmaps, etc.) will be supported in a future release.
    //
	 __sagestatic bool QuickThumbnail(int iX,int iY,Sage::RawBitmap_t & stBitmap,int iWidth,int iHeight,const char * sTitle);

        // QuickThumbnail() -- Create and display a window with a thumbnail of bitmap data.
    //
    // This is used for development to show thumbnails of bitmap data in memory, in various forms and in various ways.
    //
    // The bitmap can be display by percentage, maximum width, height, etc.
    // See Sage::ThumbType for all variations
    //
    // If there is an error in the data (i.e. an empty bitmap), the window is not opened.
    // 
    // A RawBitmap_t or CBitmap must currently be provided.
    // Raw data and more data types (such as float, float mono, 16-bit bitmaps, etc.) will be supported in a future release.
    //
     __sagestatic bool QuickThumbnail(int iX,int iY,CBitmap & cBitmap,int iWidth,int iHeight,ThumbType eType = ThumbType::BestFit,const char * sTitle = nullptr);

    // QuickThumbnail() -- Create and display a window with a thumbnail of bitmap data.
    //
    // This is used for development to show thumbnails of bitmap data in memory, in various forms and in various ways.
    //
    // The bitmap can be display by percentage, maximum width, height, etc.
    // See Sage::ThumbType for all variations
    //
    // If there is an error in the data (i.e. an empty bitmap), the window is not opened.
    // 
    // A RawBitmap_t or CBitmap must currently be provided.
    // Raw data and more data types (such as float, float mono, 16-bit bitmaps, etc.) will be supported in a future release.
    //
	 __sagestatic bool QuickThumbnail(int iX,int iY,CBitmap & cBitmap,int iWidth,int iHeight,const char * sTitle);



     __sagestatic CBitmap QuickResize(RawBitmap_t & stBitmap,int iWidth,int iHeight,ResizeType eType = ResizeType::BestFit);
     __sagestatic CBitmap QuickResize(RawBitmap_t & stBitmap,SIZE szSize,ResizeType eType = ResizeType::BestFit);

    // BitmapWindow() -- Create a window (popup or embedded) designed to show bitmaps.
    //
    // This can create a popup window or embedded window, with a bitmap included to display on creation.
    // When no location (iX,iY) is given the window pops up automatially (i.e. a separate window).
    // Otherwise, when iX and iY are given, the window is embedded into the parent window unless:
    //
    // When Popup() (i.e. opt::Popup()) is given as an option, the window is not embedded and a new popup window is 
    // created.
    //
    // The window is sized to the input bitmap.
    //
    // Once the window is created, it may be treated as any window.
    // Note: With Bitmap Popup Windows(), when the 'X' is pressed, the window is automatically closed
    // unlike regular CWindow windows where the 'X' button sets a notification and WindowClosing() status.
    //
	__sagestatic CWindow & _BitmapWindow(Sage::RawBitmap_t & stBitmap,const cwfOpt & cwOpt = cwfOpt());

    // BitmapWindow() -- Create a window (popup or embedded) designed to show bitmaps.
    //
    // This can create a popup window or embedded window, with a bitmap included to display on creation.
    // When no location (iX,iY) is given the window pops up automatially (i.e. a separate window).
    // Otherwise, when iX and iY are given, the window is embedded into the parent window unless:
    //
    // When Popup() (i.e. opt::Popup()) is given as an option, the window is not embedded and a new popup window is 
    // created.
    //
    // The window is sized to the input bitmap.
    //
    // Once the window is created, it may be treated as any window.
    // Note: With Bitmap Popup Windows(), when the 'X' is pressed, the window is automatically closed
    // unlike regular CWindow windows where the 'X' button sets a notification and WindowClosing() status.
    //
	__sagestatic CWindow & _BitmapWindow(CBitmap & cBitmap,const cwfOpt & cwOpt = cwfOpt());

    // BitmapWindow() -- Create a window (popup or embedded) designed to show bitmaps.
    //
    // This can create a popup window or embedded window, with a bitmap included to display on creation.
    // When no location (iX,iY) is given the window pops up automatially (i.e. a separate window).
    // Otherwise, when iX and iY are given, the window is embedded into the parent window unless:
    //
    // When Popup() (i.e. opt::Popup()) is given as an option, the window is not embedded and a new popup window is 
    // created.
    //
    // The window is sized to the input bitmap.
    //
    // Once the window is created, it may be treated as any window.
    // Note: With Bitmap Popup Windows(), when the 'X' is pressed, the window is automatically closed
    // unlike regular CWindow windows where the 'X' button sets a notification and WindowClosing() status.
    //
	__sagestatic CWindow & _BitmapWindow(int iX,int iY,CBitmap & cBitmap,const cwfOpt & cwOpt = cwfOpt());

    // BitmapWindow() -- Create a window (popup or embedded) designed to show bitmaps.
    //
    // This can create a popup window or embedded window, with a bitmap included to display on creation.
    // When no location (iX,iY) is given the window pops up automatially (i.e. a separate window).
    // Otherwise, when iX and iY are given, the window is embedded into the parent window unless:
    //
    // When Popup() (i.e. opt::Popup()) is given as an option, the window is not embedded and a new popup window is 
    // created.
    //
    // The window is sized to the input bitmap.
    //
    // Once the window is created, it may be treated as any window.
    // Note: With Bitmap Popup Windows(), when the 'X' is pressed, the window is automatically closed
    // unlike regular CWindow windows where the 'X' button sets a notification and WindowClosing() status.
    //
	__sagestatic CWindow & _BitmapWindow(POINT pLoc,CBitmap & cBitmap,const cwfOpt & cwOpt = cwfOpt());

    // BitmapWindow() -- Create a window (popup or embedded) designed to show bitmaps.
    //
    // This can create a popup window or embedded window, with a bitmap included to display on creation.
    // When no location (iX,iY) is given the window pops up automatially (i.e. a separate window).
    // Otherwise, when iX and iY are given, the window is embedded into the parent window unless:
    //
    // When Popup() (i.e. opt::Popup()) is given as an option, the window is not embedded and a new popup window is 
    // created.
    //
    // The window is sized to the input bitmap.
    //
    // Once the window is created, it may be treated as any window.
    // Note: With Bitmap Popup Windows(), when the 'X' is pressed, the window is automatically closed
    // unlike regular CWindow windows where the 'X' button sets a notification and WindowClosing() status.
    //
	__sagestatic CWindow & _BitmapWindow(int iX,int iY,Sage::RawBitmap_t & stBitmap,const cwfOpt & cwOpt = cwfOpt());

    // BitmapWindow() -- Create a window (popup or embedded) designed to show bitmaps.
    //
    // This can create a popup window or embedded window, with a bitmap included to display on creation.
    // When no location (iX,iY) is given the window pops up automatially (i.e. a separate window).
    // Otherwise, when iX and iY are given, the window is embedded into the parent window unless:
    //
    // When Popup() (i.e. opt::Popup()) is given as an option, the window is not embedded and a new popup window is 
    // created.
    //
    // The window is sized to the input bitmap.
    //
    // Once the window is created, it may be treated as any window.
    // Note: With Bitmap Popup Windows(), when the 'X' is pressed, the window is automatically closed
    // unlike regular CWindow windows where the 'X' button sets a notification and WindowClosing() status.
    //
	__sagestatic CWindow & _BitmapWindow(POINT pLoc,Sage::RawBitmap_t & stBitmap,const cwfOpt & cwOpt = cwfOpt());
     
    // BitmapWindowR() -- Create a window (popup or embedded) designed to show bitmaps.
    //                    (Bitmap displays upside-down compared to BitmapWindow() call)
    // 
    // This can create a popup window or embedded window, with a bitmap included to display on creation.
    // When no location (iX,iY) is given the window pops up automatially (i.e. a separate window).
    // Otherwise, when iX and iY are given, the window is embedded into the parent window unless:
    //
    // When Popup() (i.e. opt::Popup()) is given as an option, the window is not embedded and a new popup window is 
    // created.
    //
    // The window is sized to the input bitmap.
    //
    // Once the window is created, it may be treated as any window.
    // Note: With Bitmap Popup Windows(), when the 'X' is pressed, the window is automatically closed
    // unlike regular CWindow windows where the 'X' button sets a notification and WindowClosing() status.
    //
	__sagestatic CWindow & _BitmapWindowR(Sage::RawBitmap_t & stBitmap,const cwfOpt & cwOpt = cwfOpt());

    // BitmapWindowR() -- Create a window (popup or embedded) designed to show bitmaps.
    //                    (Bitmap displays upside-down compared to BitmapWindow() call)
    // 
    // This can create a popup window or embedded window, with a bitmap included to display on creation.
    // When no location (iX,iY) is given the window pops up automatially (i.e. a separate window).
    // Otherwise, when iX and iY are given, the window is embedded into the parent window unless:
    //
    // When Popup() (i.e. opt::Popup()) is given as an option, the window is not embedded and a new popup window is 
    // created.
    //
    // The window is sized to the input bitmap.
    //
    // Once the window is created, it may be treated as any window.
    // Note: With Bitmap Popup Windows(), when the 'X' is pressed, the window is automatically closed
    // unlike regular CWindow windows where the 'X' button sets a notification and WindowClosing() status.
    //
	__sagestatic CWindow & _BitmapWindowR(CBitmap & cBitmap,const cwfOpt & cwOpt = cwfOpt());

    // BitmapWindowR() -- Create a window (popup or embedded) designed to show bitmaps.
    //                    (Bitmap displays upside-down compared to BitmapWindow() call)
    // 
    // This can create a popup window or embedded window, with a bitmap included to display on creation.
    // When no location (iX,iY) is given the window pops up automatially (i.e. a separate window).
    // Otherwise, when iX and iY are given, the window is embedded into the parent window unless:
    //
    // When Popup() (i.e. opt::Popup()) is given as an option, the window is not embedded and a new popup window is 
    // created.
    //
    // The window is sized to the input bitmap.
    //
    // Once the window is created, it may be treated as any window.
    // Note: With Bitmap Popup Windows(), when the 'X' is pressed, the window is automatically closed
    // unlike regular CWindow windows where the 'X' button sets a notification and WindowClosing() status.
    //
	__sagestatic CWindow & _BitmapWindowR(int iX,int iY,CBitmap & cBitmap,const cwfOpt & cwOpt = cwfOpt());

    // BitmapWindowR() -- Create a window (popup or embedded) designed to show bitmaps.
    //                    (Bitmap displays upside-down compared to BitmapWindow() call)
    // 
    // This can create a popup window or embedded window, with a bitmap included to display on creation.
    // When no location (iX,iY) is given the window pops up automatially (i.e. a separate window).
    // Otherwise, when iX and iY are given, the window is embedded into the parent window unless:
    //
    // When Popup() (i.e. opt::Popup()) is given as an option, the window is not embedded and a new popup window is 
    // created.
    //
    // The window is sized to the input bitmap.
    //
    // Once the window is created, it may be treated as any window.
    // Note: With Bitmap Popup Windows(), when the 'X' is pressed, the window is automatically closed
    // unlike regular CWindow windows where the 'X' button sets a notification and WindowClosing() status.
    //
	__sagestatic CWindow & _BitmapWindowR(POINT pLoc,CBitmap & cBitmap,const cwfOpt & cwOpt = cwfOpt());

    // BitmapWindowR() -- Create a window (popup or embedded) designed to show bitmaps.
    //                    (Bitmap displays upside-down compared to BitmapWindow() call)
    // 
    // This can create a popup window or embedded window, with a bitmap included to display on creation.
    // When no location (iX,iY) is given the window pops up automatially (i.e. a separate window).
    // Otherwise, when iX and iY are given, the window is embedded into the parent window unless:
    //
    // When Popup() (i.e. opt::Popup()) is given as an option, the window is not embedded and a new popup window is 
    // created.
    //
    // The window is sized to the input bitmap.
    //
    // Once the window is created, it may be treated as any window.
    // Note: With Bitmap Popup Windows(), when the 'X' is pressed, the window is automatically closed
    // unlike regular CWindow windows where the 'X' button sets a notification and WindowClosing() status.
    //
	__sagestatic CWindow & _BitmapWindowR(int iX,int iY,Sage::RawBitmap_t & stBitmap,const cwfOpt & cwOpt = cwfOpt());

    // BitmapWindowR() -- Create a window (popup or embedded) designed to show bitmaps.
    //                    (Bitmap displays upside-down compared to BitmapWindow() call)
    // 
    // This can create a popup window or embedded window, with a bitmap included to display on creation.
    // When no location (iX,iY) is given the window pops up automatially (i.e. a separate window).
    // Otherwise, when iX and iY are given, the window is embedded into the parent window unless:
    //
    // When Popup() (i.e. opt::Popup()) is given as an option, the window is not embedded and a new popup window is 
    // created.
    //
    // The window is sized to the input bitmap.
    //
    // Once the window is created, it may be treated as any window.
    // Note: With Bitmap Popup Windows(), when the 'X' is pressed, the window is automatically closed
    // unlike regular CWindow windows where the 'X' button sets a notification and WindowClosing() status.
    //
	__sagestatic CWindow & _BitmapWindowR(POINT pLoc,Sage::RawBitmap_t & stBitmap,const cwfOpt & cwOpt = cwfOpt());
   
    /// <summary>
    /// Reads an image file and returns a CBitmap.  If the Bitmap is not valid, then there was an error. 
    /// <para></para>
    /// Use GetLastImageStatus() to determine error type.
    /// <para></para>
    /// --> Image Types Supported: BMP, Jpeg, and PNG.
    /// </summary>
    /// <param name="sPath">Path fo File </param>
    /// <param name="bSuccess">(optional) filled with true/false if image was read successfully.</param>
    /// <returns></returns>
    __sagestatic CBitmap ReadImageFile(const char * sPath,bool * bSuccess = nullptr);


    /// <summary>
    /// Writes 24-bit RGB bitmap out to a file.
    /// </summary>
    /// <param name="sFile"> - name of output bitmap</param>
    /// <param name="cBitmap">Bitmap tp write</param>
    /// <returns></returns>
    __sagestatic bool WriteBitmap(const char * sFile,CBitmap & cBitmap); 

    /// <summary>
    /// Reads an image file and returns a CBitmap.  If the Bitmap is not valid, then there was an error. 
    /// <para></para>
    /// Use GetLastImageStatus() to determine error type.
    /// <para></para>
    /// --> Image Types Supported: BMP, Jpeg, and PNG.
    /// </summary>
    /// <param name="sPath">Path fo File </param>
    /// <param name="bSuccess">(optional) filled with true/false if image was read successfully.</param>
    /// <returns></returns>
   __sagestatic CBitmap ReadImageFile(std::string & sPath,bool * bSuccess = nullptr);
    __sagestatic CBitmap32 ReadImageFile32(const char * sPath,bool * bSuccess = nullptr);
    __sagestatic CBitmap32 ReadImageFile32(std::string & sPath,bool * bSuccess = nullptr);
    __sagestatic ImageStatus GetLastImageStatus();


    // ReadJpegFile -- Read a jpeg file and store it into a CBitmap. 
    // This reads standard 8-bit jpeg (3-channel or monochrome).
    // If the file does not exist or is in an unsupported format, CBitmap will come back empty.
    // Use GetJpegError() to get the status of the last ReadJpegFile() call, which will
    // give information on why the Bitmap came back empty.
    //
    // if (bSuccess) is supplied, it is filled with true for a successful read, otherwise false.
    //
	__sagestatic CBitmap ReadJpegFile(const char * sPath,bool * bSuccess = nullptr);

    // ReadJpegFile -- Read a jpeg file and store it into a CBitmap. 
    // This reads standard 8-bit jpeg (3-channel or monochrome).
    // If the file does not exist or is in an unsupported format, CBitmap will come back empty.
    // Use GetJpegError() to get the status of the last ReadJpegFile() call, which will
    // give information on why the Bitmap came back empty.
    //
    // if (bSuccess) is supplied, it is filled with true for a successful read, otherwise false.
    //
	__sagestatic CBitmap ReadJpegMem(const unsigned char * sData,int iDataLength,bool * bSuccess);

    // Returns the last Jpeg status.  This will return CJpeg::Status::Ok if there was no error, 
    // or an indication of what went wrong with the last call, such as CJpeg::Status::EmptyFilePath or CJpeg::Status::FileNotFound
    //
    __sagestatic CJpeg::Status GetJpegError();

   /// <summary>
    /// Updates (repaints the window with any changes made to the window since the last update) all windows.
    /// <para></para>
    /// This function will not update windows with an AutoUpdate status of DontUpdate.
    /// <para></para>
    /// This function will update windows with a temporary DontUpdate() status. 
    /// </summary>
    /// <param name="iUpdateMS"></param>
    /// <returns></returns>
    __sagestatic bool UpdateAll(int iUpdateMS = 0);                                                                                                                      // $QC
    __sagestatic bool UpdateAll(bool bForced);                                                                                                                      // $QC

    /// #DevSetBgColor
    /// <summary>
    /// Sets the background color (or colors) of the Dev Window.<para></para>
    /// --> Note: This function should be called before any Dev Controls are created (i.e. buttons, sliders, etc.) so that<para></para>
    /// they will blend properly into the background.<para></para>
    /// Two colors may be used to form a gradient, which will span the maximum vertical length of the Dev Window (i.e. it will show more as controls are added)
    /// .<para></para>
    /// Colors can be Sagebox Rgb Color (i.e. RgbColor(0,255,0), SageColor::Red, PanColor::Green,etc.) or string such as <para></para>
    /// "Green","red","PanColor:ForestGreen", etc.<para></para>
    /// .<para></para>
    /// Examples: DevSetBgColor(SageColor::Black)<para></para>
    ///          DevSetBgColor("black","blue")<para></para>
    /// </summary>
    /// <param name="rgbColor1">- Color to clear the dev window</param>
    /// <param name="rgbColor2">- [optional] Second color to clear the background with a gradient</param>
    /// <param name="bDrawBar">- [optional] When true (default), the top menu bar is drawn in the original color.  When false, the top menu bar is covered by the new clear screan colors.</param>
    /// <returns></returns>
    bool __sagestatic DevSetBgColor(RgbColor rgbColor1,RgbColor rgbColor2 = Sage::Rgb::Undefined,bool bDrawBar = true);  // # DevSetBgColor
    bool __sagestatic DevSetBgColor(RgbColor rgbColor1,bool bDrawBar);                                                   // # DevSetBgColor
    bool __sagestatic DevSetBgColor(const char * sColor1,const char * sColor2 = nullptr,bool bDrawBar = true);           // # DevSetBgColor
    bool __sagestatic DevSetBgColor(const char * sColor1,bool bDrawBar);                                                 // # DevSetBgColor

    /// #DevSetBgBitmap
    /// <summary>
    /// Sets the background bitmap of the Dev Window.  The bitmap provided should span the <para></para>
    /// width and height of the full Dev Window, as Dev Windows grow vertically as controls are added.<para></para>
    /// --> Note: This function should be called before any Dev Controls are created (i.e. buttons, sliders, etc.) so that<para></para>
    /// .<para></para>
    /// Options can be used to set a new Y position.  Use opt::PadY() option to set the current Y position<para></para>
    /// This can be useful if the bitmap has a header, allowing the first control to start beneath it.<para></para>
    /// .<para></para>
    /// Examples:  DevSetBgBitmap(MyBitmap)<para></para>
    ///            DevSetBgBitmap("c:\\bitmaps\\MyBitmap.jpg")<para></para>
    ///            DevSetBgBitmap(MyBitmap,false,opt::PadY(50))<para></para>
    /// 
    /// </summary>
    /// <param name="cBitmap">- The bitmap to set as background bitmap.  This bitmap may be a Sage::CBitmap type or string containing the bitmap path.</param>
    /// <param name="bDrawBar">- [optional] When true (default), the top menu bar is drawn in the original color.  When false, the top menu bar is covered by the new clear screan colors.</param>
    /// <param name="cwOpt"> - [optional] Options.  The only useful option (right now) is the PadY() option to set the first control Y position in the DevWindow</param>
    /// <returns></returns>
    bool __sagestatic DevSetBgBitmap(CBitmap & cBitmap,bool bDrawBar = true,const cwfOpt & cwOpt = cwfOpt());       // # DevSetBgBitmap
    bool __sagestatic DevSetBgBitmap(const char * sBitmap,bool bDrawBar = true,const cwfOpt & cwOpt = cwfOpt());    // # DevSetBgBitmap
    bool __sagestatic DevSetBgBitmap(CBitmap & cBitmap,const cwfOpt & cwOpt);                                       // # DevSetBgBitmap
    bool __sagestatic DevSetBgBitmap(const char * sBitmap,const cwfOpt & cwOpt);                                    // # DevSetBgBitmap


    /// <summary>
    /// When bAllowClose = false, Disables the Dev Window from closing on its own -- the program must close it purposely. <para></para>
    /// This will disable the 'x' from appearing when the Dev Window is the only window open, so that the  <para></para>
    /// user cannot close it with the 'x' button. <para></para>
    /// . <para></para>
    /// This function must be called before AllowClose() is called, otherwise results may be unpredictable. <para></para>
    /// Note: to manually add the 'x' (and Close Button), use AllowClose().
    /// </summary>
    /// <returns></returns>
    __sagestatic bool DevAllowAutoClose(bool bAllowClose = true); 

    /// <summary>
    /// Sets the Y position of the next control added.
    /// </summary>
    __sagestatic bool DevSetNextY(int iY);


    // DevControls -- passed through to default CDevControls so that adding buttons, sliders, etc. is easy. 
    // For more controls (i.e. other Dev Control windows, use DevControlsWindow() to create new ones. 

 	// DevButton() -- Add a button to the Default Dev Control Window.  This accepts all options as normal buttons, but 
	// the default will add a regular button. 
	//
	// The Name used as a title for the button, but is optional. 
	//
    __sagestatic CButton & DevButton(const char * sButtonName = nullptr,const cwfOpt & cwOpt = cwfOpt());

    // DevCheckbox() -- Add a checkbox to the DevWindow. 
    //
    // All usual options apply to the checkbox, such as providing fgColor, Font, etc. 
    // If no text is supplied, a name for the checkbox is chosen automatically, such as "Checkbox 1", Checkbox 2", etc.
    //
    // --> If the checkbox name is preceded by '~' (i.e. "~My Checkbox"), this tells the DevWindow
    //     to place multiple checkboxes side-by-side (2 per-line) in the DevWindow to save space.
    //
    //     If the '~' is omitted (i.e. "My Checkbox"), all checkboxes are placed in the next vertical space
    //     in the DevWindow
    //
    //     note: to use '~' as the first character of the checkbox name, i.e. ("~Display Image"), use a double "~~". 
    //           For example, "~~Display Image" will display as "~Display Image"
    //
    __sagestatic CButton & DevCheckbox(const char * sCheckboxName = nullptr,const cwfOpt & cwOpt = cwfOpt());


    /// <summary>
    /// Adds a RadioGroup to the DevWindow.  See different prototypes for different methods (const char *, const char **, vector&lt;char *&gt;
    /// <para></para>
    /// In the case of using a simple string, separate button names with '\n', such as: 
    /// <para></para>
    /// --> AddRadioButtonGroup("Button1\nButton2\nButton3"); 
    /// <para></para>
    /// Use Title() for the section title, and Default to set a default (if Default() is not supplied, the first button is the default)
    /// <para></para>
    /// --> Example: AddRadioButtonGroup("Button1\nButton2\nButton3",Title("Select Button") | Default(1))
    /// <para></para>
    /// This sets the title to "Select Button" and sets the default button to the second button.
    /// <para></para>
    /// You can also use char * *, or vector&lt;char *&gt; to give a list of pointers to each button. 
    /// <para></para>
    /// --> Important Note: In the case of using a "char * *" array, the last entry MUST be a nullptr.  You can precede the char ** array with the number of items to avoid using a nullptr.
    /// </summary>
    /// <param name="iNumButtons">- (optional) When using "const char * *" for a list of names, you can give it the number of items (the end does not need to be a nullptr in this case)</param>
    /// <param name="sButtonNames">String of button names, or char * * list, or std::vector&lt;const char *&gt; of names.</param>
    /// <param name="cwOpt">Options such as Default() and Title()</param>
    /// <returns></returns>
    __sagestatic ButtonGroup DevRadioButtons(const char * sButtonNames,const cwfOpt & cwOpt = cwfOpt());

    /// <summary>
    /// Adds a RadioGroup to the DevWindow.  See different prototypes for different methods (const char *, const char **, vector&lt;char *&gt;
    /// <para></para>
    /// In the case of using a simple string, separate button names with '\n', such as: 
    /// <para></para>
    /// --> AddRadioButtonGroup("Button1\nButton2\nButton3"); 
    /// <para></para>
    /// Use Title() for the section title, and Default to set a default (if Default() is not supplied, the first button is the default)
    /// <para></para>
    /// --> Example: AddRadioButtonGroup("Button1\nButton2\nButton3",Title("Select Button") | Default(1))
    /// <para></para>
    /// This sets the title to "Select Button" and sets the default button to the second button.
    /// <para></para>
    /// You can also use char * *, or vector&lt;char *&gt; to give a list of pointers to each button. 
    /// <para></para>
    /// --> Important Note: In the case of using a "char * *" array, the last entry MUST be a nullptr.  You can precede the char ** array with the number of items to avoid using a nullptr.
    /// </summary>
    /// <param name="iNumButtons">- (optional) When using "const char * *" for a list of names, you can give it the number of items (the end does not need to be a nullptr in this case)</param>
    /// <param name="sButtonNames">String of button names, or char * * list, or std::vector&lt;const char *&gt; of names.</param>
    /// <param name="cwOpt">Options such as Default() and Title()</param>
    /// <returns></returns>
    __sagestatic ButtonGroup DevRadioButtons(const char * * sButtonNames,const cwfOpt & cwOpt = cwfOpt());

    /// <summary>
    /// Adds a RadioGroup to the DevWindow.  See different prototypes for different methods (const char *, const char **, vector&lt;char *&gt;
    /// <para></para>
    /// In the case of using a simple string, separate button names with '\n', such as: 
    /// <para></para>
    /// --> AddRadioButtonGroup("Button1\nButton2\nButton3"); 
    /// <para></para>
    /// Use Title() for the section title, and Default to set a default (if Default() is not supplied, the first button is the default)
    /// <para></para>
    /// --> Example: AddRadioButtonGroup("Button1\nButton2\nButton3",Title("Select Button") | Default(1))
    /// <para></para>
    /// This sets the title to "Select Button" and sets the default button to the second button.
    /// <para></para>
    /// You can also use char * *, or vector&lt;char *&gt; to give a list of pointers to each button. 
    /// <para></para>
    /// --> Important Note: In the case of using a "char * *" array, the last entry MUST be a nullptr.  You can precede the char ** array with the number of items to avoid using a nullptr.
    /// </summary>
    /// <param name="iNumButtons">- (optional) When using "const char * *" for a list of names, you can give it the number of items (the end does not need to be a nullptr in this case)</param>
    /// <param name="sButtonNames">String of button names, or char * * list, or std::vector&lt;const char *&gt; of names.</param>
    /// <param name="cwOpt">Options such as Default() and Title()</param>
    /// <returns></returns>
    __sagestatic ButtonGroup DevRadioButtons(int iNumButtons,const char * * sButtonNames,const cwfOpt & cwOpt = cwfOpt());

     /// <summary>
    /// Adds a RadioGroup to the DevWindow.  See different prototypes for different methods (const char *, const char **, vector&lt;char *&gt;
    /// <para></para>
    /// In the case of using a simple string, separate button names with '\n', such as: 
    /// <para></para>
    /// --> AddRadioButtonGroup("Button1\nButton2\nButton3"); 
    /// <para></para>
    /// Use Title() for the section title, and Default to set a default (if Default() is not supplied, the first button is the default)
    /// <para></para>
    /// --> Example: AddRadioButtonGroup("Button1\nButton2\nButton3",Title("Select Button") | Default(1))
    /// <para></para>
    /// This sets the title to "Select Button" and sets the default button to the second button.
    /// <para></para>
    /// You can also use char * *, or vector&lt;char *&gt; to give a list of pointers to each button. 
    /// <para></para>
    /// --> Important Note: In the case of using a "char * *" array, the last entry MUST be a nullptr.  You can precede the char ** array with the number of items to avoid using a nullptr.
    /// </summary>
    /// <param name="iNumButtons">- (optional) When using "const char * *" for a list of names, you can give it the number of items (the end does not need to be a nullptr in this case)</param>
    /// <param name="sButtonNames">String of button names, or char * * list, or std::vector&lt;const char *&gt; of names.</param>
    /// <param name="cwOpt">Options such as Default() and Title()</param>
    /// <returns></returns>
    __sagestatic ButtonGroup DevRadioButtons(std::vector<char *> & vButtonNames,const cwfOpt & cwOpt = cwfOpt());

    __sagestatic ButtonGroup DevCheckboxGroup(const char * sButtonNames,const cwfOpt & cwOpt = cwfOpt());
    __sagestatic ButtonGroup DevCheckboxGroup(const char * * sButtonNames,const cwfOpt & cwOpt = cwfOpt());
    __sagestatic ButtonGroup DevCheckboxGroup(int iNumButtons,const char * * sButtonNames,const cwfOpt & cwOpt = cwfOpt());
    __sagestatic ButtonGroup DevCheckboxGroup(std::vector<char *> & vButtonNames,const cwfOpt & cwOpt = cwfOpt());


    // QuickSlider() -- Add a slider to the Default Dev Controls Window.  The default width is 200 with a 0-100 range.  
    // The Range can be changed with default Slider options, i.e. opt::Range(0,200), for example, to set a range of 0-200.
	// -->
	// The title is displayed beneath the slider, as well as the value. 
	//
    __sagestatic CSlider & DevSlider(const char * sSliderName = nullptr,const cwfOpt & cwOpt = cwfOpt());

    // QuickSlider() -- Add a slider to the Default Dev Controls Window.  The default width is 200 with a 0-100 range.  
    // The Range can be changed with default Slider options, i.e. opt::Range(0,200), for example, to set a range of 0-200.
	// -->
	// The title is displayed beneath the slider, as well as the value. 
	//
    __sagestatic CSlider & DevSliderf(const char * sSliderName = nullptr,const cwfOpt & cwOpt = cwfOpt());

    // DevEditBox() -- Add an EditBox to the default Dev control Window.  The sEditBoxTitle, while optional, will provide a
	// label to the left of the edit box.  The default width is 150 pixels or so, but can be changed with normal EditBox options
	//
    // Note: DevEditBox() and DevInputBox() are the same.  The term Editbox is deprecated and is used for consitency with Windows control names.
    //
    __sagestatic CEditBox & DevEditBox(const char * sEditBoxName = nullptr,const cwfOpt & cwOpt = cwfOpt());
  
    // DevInputBox() -- Add an InputBox to the default Dev control Window.  The sInputBoxName, while optional, will provide a
	// label to the left of the Input Box.  The default width is 150 pixels or so, but can be changed with normal InputBox options
	//
    // Note: DevEditBox() and DevInputBox() are the same.  The term Editbox is deprecated and is used for consitency with Windows control names.
    //
    __sagestatic CEditBox & DevInputBox(const char * sInputBoxName = nullptr,const cwfOpt & cwOpt = cwfOpt()) { return DevEditBox(sInputBoxName,cwOpt); } // $$moveme
  
	__sagestatic CComboBox & DevComboBox(const char * sItemList,const cwfOpt & cwOpt = cwfOpt());
	__sagestatic CComboBox & DevComboBox(const cwfOpt & cwOpt = cwfOpt());
	__sagestatic CWindow & DevWindow(const char * sTitle,int iNumlines,const cwfOpt & cwOpt = cwfOpt());
	__sagestatic CWindow & DevWindow(int iNumLines,const cwfOpt & cwOpt = cwfOpt());
	__sagestatic CWindow & DevWindow(const cwfOpt & cwOpt = cwfOpt());
	__sagestatic CWindow & DevWindow(const char * sTitle,const cwfOpt & cwOpt = cwfOpt());

    __sagestatic CTextWidget & DevText(const char * sText,const cwfOpt & cwOpt  = cwfOpt());
    __sagestatic CTextWidget & DevText(const char * sText,int iHeight,const cwfOpt & cwOpt  = cwfOpt());
    __sagestatic CTextWidget & DevText(int iHeight,const cwfOpt & cwOpt  = cwfOpt());
    __sagestatic CTextWidget & DevText(const cwfOpt & cwOpt  = cwfOpt());


    /// # DevBitmap
    /// <summary>
    /// Adds a bitmap to the window. This bitmap is copied and stored by the Dev Window.  The original bitmap does not need to be kept allocated.<para></para>
    /// The bitmap can be a 32-bit BMP or .PNG file and will blend in to the background.<para></para>
    /// --> Bitmaps in the Dev Window are meant to be either icons or small bitmaps for headers.  <para></para>
    /// A String may be added to the the right of the bitmap.  If the bitmap is not found, the string will still be printed.<para></para>
    /// Sagebox options can be used to set the Font and color of the text.  The text will be centered vertically to the bitmap's center.<para></para>
    /// ---> Examples: AddBitmap(MyBitmap)<para></para>
    /// DevBitmap("c:\\bitmaps\\mybitmap.bmp",false) <para></para>
    /// DevBitmap("c:\\bitmaps\\mybitmap.png"," Project Controls",opt::PadY(10))        -- add 10 pixels to the Y position in the window after the bitmap.<para></para>
    /// DevBitmap("c:\\bitmaps\\mybitmap.png"," Project Controls",opt::Font(20 | opt::TextColor("green"))<para></para>
    /// </summary>   
    /// <param name="cBitmap">- The bitmap to display.  This can either be a Sage::CBitmap type (with transparencies) or a path to the bitmap.</param>
    /// <param name="sText">- [optional] Text to the right of the bitmap. Note the space in the example above to add some blank space.</param>
    /// <param name="bDrawTopBar">- [optional] When true (default), the top menu bar is drawn in the original color.  When false, the top menu bar is covered by the new clear screan colors.</param>
    /// <param name="cwOpt">- [optional] Options such as opt::Font(), opt::TextColor(), opt::PadY() to control the next and next control position.</param>
    /// <returns></returns>
    __sagestatic bool DevBitmap(CBitmap & cBitmap,bool bDrawTopBar = true);                                                               /// # DevBitmap
    __sagestatic bool DevBitmap(CBitmap & cBitmap,const char * sText,bool bDrawTopBar = true,const cwfOpt & cwOpt = CWindow::cwNoOpt);    /// # DevBitmap
    __sagestatic bool DevBitmap(CBitmap & cBitmap,const char * sText,const cwfOpt & cwOpt);                                               /// # DevBitmap

    __sagestatic bool DevBitmap(const char * sPath,bool bDrawTopBar = true);                                                               /// # DevBitmap
    __sagestatic bool DevBitmap(const char * sPath,const char * sText,bool bDrawTopBar = true,const cwfOpt & cwOpt = CWindow::cwNoOpt);    /// # DevBitmap
    __sagestatic bool DevBitmap(const char * sPath,const char * sText,const cwfOpt & cwOpt);                                               /// # DevBitmap

    /// <summary>
    /// Auto-hides the DevWindow when the user presses the 'x' button or close button.  This does not destroy the devwindow, and only hides it.
    /// <para></para>
    /// Once Hidden, the DevWindow can be shown again with a call to Show()
    /// </summary>
    /// <param name="bAutoHide"> - True to auto-hide window; false to turn auti-hide off</param>
    /// <param name="bAddCloseButton"> -- True to create a close button.  Otherwise only the 'x' appears in the upper-right corner.</param>
    /// <returns></returns>
    __sagestatic bool DevWindowAutoHide(bool bAutoHide = true,bool bAddCloseButton = false);

    /// <summary>
    /// Shows the DevWindow if it is hidden.  
    /// </summary>
    /// <param name="bShow">True to show the Dev Window. False to hide the DevWindow</param>
    /// <returns></returns>
    __sagestatic bool DevWindowShow(bool bShow = true);

    /// <summary>
    /// Hides the DevWindow if it is visible.  
    /// </summary>
    /// <param name="bShow">True to hide the Dev Window. False to show the DevWindow</param>
    /// <returns></returns>
    __sagestatic bool DevWindowHide(bool bHide = true); 

    /// <summary>
    /// Adds 'X' on top-right and optional "Close" button.
    /// <para></para>&#160;&#160;&#160;
    /// Dev Windows are not typically closed and do not have close controls by default.  DevAllowClose() adds the ability to close the window,
    /// which can be used as a way to terminate a console-mode or other program that does not have a visible window. 
    /// <para></para>
    /// Use DevWindowClosed() to determine if the window has been closed (or the added Close Button has been pressed).
    /// <para></para>
    /// When the 'X' button is pressed, a Sage Event is sent to the window where it can be checked. The Close button also sends a Sage Event.
    /// <para></para>&#160;&#160;&#160;
    /// Note: When the user presses the 'X' button or "Close" button, the window is not closed or hidden.  These controls are only used
    /// to provide an indication (through DevWindowClosed()) that the user has pressed a control as a message to terminate the application.
    /// <para></para>&#160;&#160;&#160;
    /// ---> This function is not used to close the Dev Window but as an easy way for the user to signal to close the application.
    /// </summary>
    /// <param name="bAddCloseButton">when TRUE adds a "Close" button.  Otherwise only the 'X' is placed on the right-top of the window for closure.</param>
    /// <returns></returns>
    __sagestatic bool DevAllowClose(bool bAllowClose = true,bool bAddCloseButton = false); 
  
    /// <summary>
    /// Sets the window to close automatically when there are no other windows open. <para></para>
    /// By default, the Dev Window is a 'primary' window and won't close when <para></para>
    /// functions such as WaitPending() or GetEvent() are used.<para></para>
    /// .<para></para>
    /// When set to false (default), the window won't close until it is closed by the user. 
    /// or the program exits.
    /// </summary>
    /// <param name="bAutoClose">when true, the dev window will close automatically.  When false, the user must close it.</param>
    __sagestatic bool DevAutoClose(bool bAutoClose = true);
  
    /// <summary>
    /// Sets the location of the Dev Window
    /// </summary>
    __sagestatic bool DevSetLocation(int iX,int iY);

    /// <summary>
    /// Sets the location of the Dev Window
    /// </summary>
    __sagestatic bool DevSetLocation(POINT pLoc);

    /// <summary>
    /// Returns TRUE if the "Close" Button or 'X' has been pressed (both are added by DevAllowClose()).
    /// <para></para>
    /// This can be used as a quick way to close an application that only has a Dev Window and no other window except the Console Window.
    /// </summary>
    /// <param name="bAddCloseButton">when TRUE adds a "Close" button on first usage.  Otherwise only the 'X' is placed on the right-top of the window for closure.</param>
    /// <returns>true if the user has attempted to close the window.</returns>
    __sagestatic bool DevWindowClosed(bool bAddCloseButton = false); 

    /// <summary>
    /// Waits for the DevWindow to close before returning.  If DevAllowClose() has not been called, an 'x' will appear in the upper-right 
    /// part of the dev window.
    /// <para></para>
    /// This can be useful when there is no main window (or it is hidden) to use for a method to wait for the user to indicate closing the program down, without
    /// using a console mode input or other button window. 
    /// </summary>
    /// <param name="bAddCloseButton">- when TRUE, adds a close button to the window for easier close options</param>
    /// <returns>true if everything is fine; false if there is an issue with the DevWindow</returns>
    __sagestatic bool DevWaitforClose(bool bAddCloseButton = false);

    /// <summary>
    /// Returns true if the DevWindow Close Button was pressed, but only once -- it returns false after this point until the DevWindow
    /// is again made visible by a call to DevWindowShow()
    /// <para></para>
    /// --> This difference from DevWindowsClosed() which will continuously give a Windows-Close status of TRUE if the window is closed
    /// <para></para>
    /// DevWindowCloseEvent(), however, is an event status and only returns TRUE once after the close (or X) button is pressed.
    /// </summary>
    /// <param name="bAddCloseButton"> -- True to create a close button.  Otherwise only the 'x' appears in the upper-right corner.</param>
    /// <returns></returns>
    __sagestatic bool DevWindowCloseEvent(bool bAddCloseButton = false);

    /// <summary>
    /// Waits for an event in the Dev Window Only.  Only use this when there is no other window -- otherwise use GetEvent() for the window (DevWindow events come through this window)
    /// <para></para>
    /// --> This function is intended to be used as a method to retrieve Dev Window events and react to its closing like a regular event loop.  This function will return false
    /// if the Dev Window is closed, allowing passthrough.  NOTE: It is up to the program to handle the window closure.
    /// <para></para>
    /// This function is useful when the DevWindow is the only window displaying. 
    /// <para></para>
    /// --> This function will return TRUE when an event occurs; FALSE if the window is closed by the user. 
    /// --> This function will add a Close Button if it does not exist (when bReturnFalseOnClose is set to TRUE)
    /// </summary>
    /// <param name="bReturnFalseOnClose">- (optional) When TRUE (default), DevWaitEvent() returns false if the user closes the window, also adding a Close Button.  
    /// When false, it returns TRUE as a regular event if the window is closed (and does not add a close option automatically)</param>
    /// <returns></returns>
    __sagestatic bool DevWaitEvent(bool bReturnFalseOnClose = true);

 	// DevAddSection() -- Adds a text section to the default Dev Controls window, to separate types of controls.
	// You can use opt::fgColor() to set the text color of the section name.
	//
    __sagestatic bool DevAddSection(const char * sSectionName = nullptr,const cwfOpt & cwOpt = cwfOpt());

    // GetDevControlsPtr() -- returns the pointer to the default CDevControls object. 
    // *** Important note *** -- this will return NULLPTR until a control is created with
    // QuickButton(), QuickSlider(), etc. The window is not created until a control is created in order
    // to save memory.
    //
    // With the pointer, any CDevContgrols() function can be used, though most are replicated directly
    // through CSageBox functions for ease of use.
    //
    __sagestatic CDevControls * GetDevControlsPtr();

    // DevGetWindow() -- Returns the Window (i.e. CWindow) of the default DevWindow.  This can be used to access window functions for the 
    // the default DevWindow. 
    //
    // For user-created Dev Control Windows, use the CDevControls::GetWindow() function, i.e. cMyDevWindow->GetWindow(); 
    //
    __sagestatic CWindow * DevGetWindow();

    // DevGetGroup() -- returns the group (i.e. WinGroup) substructure for the default Dev Window.  This can be used to perform group functions.
    // For example, when using multiple DevSlider() calls with a declared common group (i.e. DevSlider("MySlider",opt::Group(100,1)), group.SliderMoved(100)
    // can be used to determine if any slider in the group was moved, the slider ID and the slider position.
    //
    // Groups created in DevControls must be used through that Dev Controls Window's group function, which is why DevGroup is provided.
    //
    // For example, using DevGetGroup()->SliderMoved(MyGroup,..) will check any sliders within the given group within the DevWindow.
    //
    // This function only works for the default DevWindow.  For user-created DevWindows, use the CDevControls::group() function, i.e.
    // cMyDevWindow->group().SliderMoved(MyGroup,...)
    //
    __sagestatic CWindow::WinGroup * DevGetGroup();
    
    // NewDevWindow() -- Create a CDevControls Window, allowing for quick creation and automatic placement of
    // controls (buttons, slider, editboxes, etc.) in the window.   This allows quick prototyping and development of non
    // GUI functions with GUI controls. 
    // 
    // See documentationn in CQuickControls.h for more information
    //
    // ** Important note ** the object pointer returned MUST BE DELETED, as it is not a managed object. 
    // Example code tends to use Obj<CDevControls> cDevControls = DevControlsWindow() to treat it as a stack object
    // that is automatically deleted when the current function (or class) goes out of scope.
    // (However, since this is usually only used for develpment, leaving it allocated prior to program end causes no problems)
    //
    __sagestatic CDevControls & NewDevWindow(int iX,int iY,const cwfOpt & cwOpt = CWindow::cwNoOpt);

    // NewDevWindow() -- Create a CDevControls Window, allowing for quick creation and automatic placement of
    // controls (buttons, slider, editboxes, etc.) in the window.   This allows quick prototyping and development of non
    // GUI functions with GUI controls. 
    // 
    // See documentationn in CQuickControls.h for more information
    //
    // ** Important note ** the object pointer returned MUST BE DELETED, as it is not a managed object. 
    // Example code tends to use Obj<CDevControls> cDevControls = DevControlsWindow() to treat it as a stack object
    // that is automatically deleted when the current function (or class) goes out of scope.
    // (However, since this is usually only used for develpment, leaving it allocated prior to program end causes no problems)
    //
    __sagestatic CDevControls & NewDevWindow(POINT pLoc,const cwfOpt & cwOpt = CWindow::cwNoOpt);

    // NewDevWindow() -- Create a CDevControls Window, allowing for quick creation and automatic placement of
    // controls (buttons, slider, editboxes, etc.) in the window.   This allows quick prototyping and development of non
    // GUI functions with GUI controls. 
    // 
    // See documentationn in CQuickControls.h for more information
    //
    // ** Important note ** the object pointer returned MUST BE DELETED, as it is not a managed object. 
    // Example code tends to use Obj<CDevControls> cDevControls = DevControlsWindow() to treat it as a stack object
    // that is automatically deleted when the current function (or class) goes out of scope.
    // (However, since this is usually only used for develpment, leaving it allocated prior to program end causes no problems)
    //
    __sagestatic CDevControls & NewDevWindow(const cwfOpt & cwOpt = CWindow::cwNoOpt);

    // DevWindowTopmost() -- Sets the Default Dev controls windows as a 'topmost' window, 
    // preventing other windows from overlapping and obscuring the Dev Controls Window.
    // This is the equivalent of GetDevControlsPtr()->SetTopmost(), or using SetTopMost() in a 
    // user-created window (see DevControlsWindow() to create more Dev Control Windows).
    // -->
    // Setting bTopMost = false sets the default Dev Controls Window to normal behavior, allowing
    // other windows to overlap when they gain focus and are overlapping the default Dev Controls Window
    //
    __sagestatic bool DevWindowTopmost(bool bTopmost = true);

    // ExitButton() -- Create a modal button (i.e. a button that needs input before continuing) with a message.  This can be used
    // prior to exiting a console-mode program so that the windows and controls remain intact until the user pressed
    // the "OK" button.  This is the same as QuickInfoButton() but uses a default "program ending" message when there is
    // no text included.
    // --> sText sets the message to display to the user.  When sText is omitted a message declaring the program has ended
    //     is displayed as the default message.
    // 
    // Exit Button returns 0. 
    //
    __sagestatic int ExitButton(const char * sText = nullptr); 

   // ReadPgrBitmap() -- Reads a Bitmap or JPEG file from a .PGR file (or PGR Memory) and returns a 
    // CBitmap.
    //
    // This function is used to quick access to BMP or JPEG files embedded in PGR file without 
    // opening the PGR file and searching for the image.
    //
    // ReadPgrBitmap() opens the PGR, searches for the image (BMP, compress BMP (TPC), or JPEG) and loads it
    // if found.  Otherwise, an empty CBitmap is returned.
    //
    // if bSucess is passed, this is filled with TRUE if an image was found, and FALSE if there was no image or an 
    // error occurred loading the image.
    //
    // --> Examples:
    // --> auto cBitmap = ReadPgrBitmap("ImageName","myPgrFile.pgr");
    // --> auto cBitmap = ReadPgrBitmap("Bitmaps:Image1",PgrMem); 
    //
	__sagestatic CBitmap ReadPgrBitmap(const char * sImageTitle,const char * sPgrFile,bool * bSuccess = nullptr);

    // ReadPgrBitmap() -- Reads a Bitmap or JPEG file from a .PGR file (or PGR Memory) and returns a 
    // CBitmap.
    //
    // This function is used to quick access to BMP or JPEG files embedded in PGR file without 
    // opening the PGR file and searching for the image.
    //
    // ReadPgrBitmap() opens the PGR, searches for the image (BMP, compress BMP (TPC), or JPEG) and loads it
    // if found.  Otherwise, an empty CBitmap is returned.
    //
    // if bSucess is passed, this is filled with TRUE if an image was found, and FALSE if there was no image or an 
    // error occurred loading the image.
    //
    // --> Examples:
    // --> auto cBitmap = ReadPgrBitmap("ImageName","myPgrFile.pgr");
    // --> auto cBitmap = ReadPgrBitmap("Bitmaps:Image1",PgrMem); 
    //
	__sagestatic CBitmap ReadPgrBitmap(const char * sImageTitle,const unsigned char * sPGRMemory,bool * bSuccess = nullptr);

    // GetBitmapStruct() -- Returns a RawBitmap_t struct with memory for the Width and height.
    // If Height is omitted, a Bitmap structure of height 1 is returned.
    //
    // This is returned as an aligned 8-bit-per-channel bitmap.
    //
    // [[nodiscard]] -- Important note: This returns a RawBitmap_t structure which has allocate memory.
    // RawBitmap_t::Delete() must be called to delete this memory.
    //
    // Assign this to CBitmap, such as CBitmap cBitmap = GetWindowBitmap(); 
    // CBitmap will delete this memory automatically.  Otherwise, call RawBitmap_t::Delete() to make sure the memory is deleted
    //
   __sagestatic  CBitmap CreateBitmap(int iWidth,int iHeight = 1);

    // GetBitmapStruct() -- Returns a RawBitmap_t struct with memory for the Width and height.
    // If Height is omitted, a Bitmap structure of height 1 is returned.
    //
    // This is returned as an aligned 8-bit-per-channel bitmap.
    //
    // [[nodiscard]] -- Important note: This returns a RawBitmap_t structure which has allocate memory.
    // RawBitmap_t::Delete() must be called to delete this memory.
    //
    // Assign this to CBitmap, such as CBitmap cBitmap = GetWindowBitmap(); 
    // CBitmap will delete this memory automatically.  Otherwise, call RawBitmap_t::Delete() to make sure the memory is deleted
    //
    __sagestatic CBitmap CreateBitmap(SIZE szBitmapSize);

    /// <summary>
    /// Shows the Console Window after it has been hidden.
    /// </summary>
    /// <param name="bShow"></param>
    /// <returns>Returns 0 for easy return from main(), i.e. return ShowConsole()</returns>
    __sagestatic int ShowConsole(bool bShow = true);

    /// <summary>
    /// Hides the Console Window.
    /// <para></para>
    /// --> Important note: 
    /// </summary>
    /// <param name="bPermanent"></param>
    /// <returns></returns>
    __sagestatic bool HideConsole(bool bPermanent = false);
    __sagestatic bool ValidateControl(HWND hWnd);
    __sagestatic bool ValidateWindow(CWindow * cWin);

    // Determine if we're console app or a windows app.  
    //
    // This returns true of the app started as a Windows app (i.e. _WINDOWS is defined), even if it has created a console window.
    // This returns false if both _CONSOLE mode was set and there is no console window.  Therefore, if a console mode program has deleted
    // its console window, this will return true to indicate to the program that this isn't a console-capable application, since it has no console window
    //
    // This is meant to be called at any time, but particularly before or right after SageBox is created so that a prorgram may determine 
    // a course of action.
    // 
    // This is a static function and can be called without SageBox having been created, i.e. CSageBox::isConsoleApp()
    //
    static bool isConsoleApp() { 
            #ifdef _WINDOWS 
                return false;        // if _WINDOWS is defined, it's a good bet, so we'll trust it, even if it has defined a console app on its own.
            #else           
                // When _CONSOLE is defined, lets also make sure we have a console window
                return GetConsoleWindow() != nullptr;
            #endif          
    }

    /// <summary>
    /// Sets the console window as the foreground window, on top of all other windows (except those with TOPMOST status)
    /// <para></para>
    /// This is useful when printing to the Console Window to make sure it is visible to the user
    /// </summary>
    static bool ConsoleBringFront();

    /// <summary>
    /// Sets the console window behind the given window, making sure it is underneath the given window, but 
    /// not at the bottom of the Z-order (i.e. all windows)
    /// </summary>
    /// <param name="cWin">Window to place Console Window underneath</param>
    /// <returns></returns>
    static bool ConsoleSetBehind(CWindow & cWin); 
     /// <summary>
    /// Sets the console window as the bottom window, making sure it is underneath all other windows.
    /// <para></para>
    /// You can also use the windows ConsoleSetBehind() function, i.e. MyWindow.ConsoleSetBehind()
    /// </summary>
    /// 
   static bool ConsoleSetBottom(); 


    // Returns true of the main thread is stopped.  
    // Use StartThread() to resume the main thread. 
    //
    // This only applies to the original program thread and will not work for other threads created after
    // the program started.
    //
    __sagestatic bool ThreadStopped();
    
    // Returns the status of the thread.  ThreadStatus::Running or ThreadStatus::Suspended (if the thread is stopped).
    // This only applies to the original program thread and will not work for other threads created after
    // the program started.
    //
    __sagestatic ThreadStatus GetThreadStatus();

    // Stop the main thread.  This is usually used when ending the main thread and transferring to full event-driven control. 
    //
    // This only applies to the original program thread and will not work for other threads created after
    // the program started.
    //
    // the value iReturnValue is returned from StopThread() when it wakes up.
    //
    //
    __sagestatic int StopThread();

    // Resume the main thread if it is suspended.
    //
    // iValue will be returned by StopThread() when it is awakened. 0 is returned if it was awakened due to the window closing.
    //
    __sagestatic bool ResumeThread(int iValue = 0);

    // End the program when in the main message thread.  This is used when StopThread() has been used to stop the main thread but events are
    // still being handled through the Main Windows Message Thread.
    //
    // EndProgram() sets the window closing status and resumes the main thread.  Typically, the main thread will exit and SageBox will end.
    // However, StopThread() can be used anywhere and does not need to exit immediately.  It can take care of cleanup, memory deallocations, etc. 
    //
    // iValue will be returned by StopThread() when it is awakened. 0 is returned if it was awakened due to the window closing.
    //
    __sagestatic bool EndProgram(int iReturnValue = 0);

   // Get a named color.  This returns an RGBColor_t (or DWORD -- see prototypes) of a named color.
    // 
    // Example:
    //
    // auto rgbBlue = GetColor("Blue");                 -- Get RGBColor_t value for Blue
    // RGBColor_t rgbMyColor = GetColor("MyColor");     -- Get RGBColor_t value for "MyColor" created previously.
    //
    __sagestatic RGBColor_t  GetColor(const char * sColor,bool * pColorFound = nullptr);

    // Get a named color.  This returns an RGBColor_t (or DWORD -- see prototypes) of a named color.
    // 
    // Example:
    //
    // auto rgbBlue = GetColor("Blue");                 -- Get RGBColor_t value for Blue
    // RGBColor_t rgbMyColor = GetColor("MyColor");     -- Get RGBColor_t value for "MyColor" created previously.
    //
    __sagestatic bool GetColor(const char * sColor,DWORD & rgbColor);
   // MakeColor() -- Make a named system color useable throughout SageBox functions.
    //
    // Make Color can create a color for use with other function, by namin the color and returning an RGBColor_t value that can be used with all Sagebox
    // functions.
    //
    // Example:
    //
    // MakeColor("MyColor",{ 255,128,128} );    -- Make a light red and call it MyColor
    // LightRed = MakeColor("MyColor",{ 255,128,128} );    -- Make "MyColor" and also store it in the RGBColor_t value LightRed
    //
    // cWin.SetFgColor("MyColor");      -- Set the foreground color to the color "MyColor";
    // cWin.SetFgColor(LightRed);       -- Set the foreground color to the RGBColor_t color LightRed
    // cWin.Write("{MyColor}This is light red{/}")   -- Set the color "MyColor" in an output string.
    //
    __sagestatic RGBColor_t MakeColor(const char * sColor,DWORD rgbColor);

    // MakeColor() -- Make a named system color useable throughout SageBox functions.
    //
    // Make Color can create a color for use with other function, by namin the color and returning an RGBColor_t value that can be used with all Sagebox
    // functions.
    //
    // Example:
    //
    // MakeColor("MyColor",{ 255,128,128} );    -- Make a light red and call it MyColor
    // LightRed = MakeColor("MyColor",{ 255,128,128} );    -- Make "MyColor" and also store it in the RGBColor_t value LightRed
    //
    // cWin.SetFgColor("MyColor");      -- Set the foreground color to the color "MyColor";
    // cWin.SetFgColor(LightRed);       -- Set the foreground color to the RGBColor_t color LightRed
    // cWin.Write("{MyColor}This is light red{/}")   -- Set the color "MyColor" in an output string.
    //
    __sagestatic RGBColor_t MakeColor(const char * sColor,RGBColor_t rgbColor);

   // ImportClipboardText() -- Returns Text String in the Windows clipboard, if it exists.
    //
    // If there is text within the Windows Clipboard, a CString object will be returned with its contents.
    // Otherwise, an empty CString will be returned.
    //
    // a bSuccess pointer may be included which will be filled with the results (true if text was found, false if the CString returned is empty);
    //
    __sagestatic CString ImportClipboardText(bool * bSuccess = nullptr);

    // ImportClipboardTextW() -- Returns a Unicode Text String in the Windows clipboard, if it exists.
    //
    // If there is text within the Windows Clipboard, a CStringW object will be returned with its contents.
    // Otherwise, an empty CStringW will be returned.
    //
    // a bSuccess pointer may be included which will be filled with the results (true if text was found, false if the CStringW returned is empty);
    //
    __sagestatic CStringW ImportClipboardTextW(bool * bSuccess = nullptr);

    // ImportClipboardBitmap() -- Returns a CBitmap with a copy of the Bitmap in the Clipboard buffer. 
    //
    // If there is no bitmap in the clipboard buffer, an empty CBitmap will be returned. 
    //
    // if a bSuccess pointer is provided, this is fille with the bitmap status (TRUE if the bitmap was filled, FALSE if there is no bitmap
    // in the Windows Clipboard, and the returned CBitmap is empty)
    //
    // This will copy an 8-bit, 24-bit, or 32-bit bitmap.  In the case of an 8-bit bitmap, the grayscale bitmap is returned in the 
    // CBitmap with the Red, Green, and Blue values filled with the gray value
    //
    // With 32-bit bitmaps, a mask element is created in the CBitmap, to which the Alpha channel for the bitmap is copied.
    //
    __sagestatic CBitmap ImportClipboardBitmap(bool * bSuccess = nullptr);

    /// <summary>
    /// Disables ^C from exiting the program when pressed in input boxes. 
     /// <para></para>&#160;&#160;&#160;
   /// By default, ^C will exit the program when the user presses it when in an input loop.
    /// For SageBox Quick C++ applications, pressing ^C any time will exit the program by default.  In Sagebox,
    /// calling EnableControlCExit() must be called. 
    /// <para></para>&#160;&#160;&#160;
    /// When this is disabled, the ^C will no longer have any effect, but the program can still be terminated via the Process Window (when active)
    /// or the System Menu. 
    /// </summary>
    /// <param name="bDisable">Disables Control^C exit when true.  Enables Control ^C exit when false</param>
    /// <returns></returns>
    __sagestatic bool DisableControlCExit(bool bDisable = true);

     /// <summary>
    /// Enables pressing ^C to exit from the program  when pressed in input boxes or when the program is running.
    /// <para></para>&#160;&#160;&#160;
    /// By default, ^C will exit the program when the user presses it when in an input loop.
    /// For SageBox Quick C++ applications, pressing ^C any time will exit the program by default.  In Sagebox,
    /// calling EnableControlCExit() must be called. 
    /// <para></para>&#160;&#160;&#160;
    /// When this is disabled, the ^C will no longer have any effect, but the program can still be terminated via the Process Window (when active)
    /// or the System Menu. 
    /// </summary>
    /// <param name="bEnable">Enables Exit by ^C when true; Disables when false</param>
    /// <returns></returns>
    __sagestatic bool EnableControlCExit(bool bEnable = true);

   /// <summary>
    /// Returns true when ^C will exit the program when pressed
    /// </summary>
    /// <returns></returns>
    __sagestatic bool isControlCExitDisabled(); 


    /// <summary>
    /// Returns a string with version information for the Sagebox library, as well as current
    /// platform (i.e. 32 bits or 64 bits). 
    /// </summary>
    /// <returns>String with current library version and run-time platform</returns>
    static CString GetVersionInfo();

    /// <summary>
    /// returns TRUE of running 64-bit windows, false if 32-it windows. 
    /// </summary>
    /// <returns></returns>
    static bool is64BitWindows();

    static bool CreateProcessWindow(ProcessWinStartMode eStartMode = ProcessWinStartMode::Default);
    static bool WriteProcessWindow(const char * sText); 
    static bool ShowProcessWindow(bool bShow,bool bShowDebug);
    static bool ProcessWindowVisible();
    static bool ProcessWindowKillTimer(bool bKillTimer);
    static bool ProcessWindowShowLineNumbers(bool bShowLineNumbers);

    // Debug class -- directly replicated from CWindow::WinDebug with a couple exceptions, such as starting the 
    // ^C accelerator.   Otherwise, it passes through the main hidden window in Sagebox



    // ---------------
    // Debug Functions
    // ---------------
    //
    // Provides debug output functions. 
    //
    // Note: This section is currently still under construction and is expected to grow over time.,
    //       Currently, it offers debug functions mostly through the Process Window, a widow that is disconected from
    //       Sagebox and any windows in Sagebox for the purpose of more low-level debugging.
    //
    // In the future, a more robust debug window more integral to Sagebox will also be provided.
    //
    // Debugging is meant to be as low-level as possible. 
    //
    // Important note:  Do not change this independently.  Except for unique items, CWindow::WinDebug must remain in parallel here. 
    //                  (they will probably be merged at some point)
    //
    // See items unique to CSagebox version (this one) at the bottom. 
    //
    struct Debug
    {
        friend CSagebox;
    private:
        CWindow         * m_cWin = nullptr;     // Main CSagebox Window (which is always hidden)
    public:
        Debug & operator << (const char * x)       ;
        Debug & operator << (char x)               ;
        Debug & operator << (std::string & cs)     ;
        Debug & operator << (CString & cs)         ;
        Debug & operator << (int x)                ;
        Debug & operator << (unsigned int x)       ;
        Debug & operator << (float x)              ;
        Debug & operator << (double x)             ;

        // Process Control Window initialization types. 
        // Determines how to open Process Control Window when it is initialized with Init()
        //
        enum class InitType
        {
            Hidden,                 // Initially Hidden.  Use the System Menu on any main window or move move to upper-right desktop
            Visible,                // Shows as Title Bar only in upper-right of desktop.  Mouse mouse to title bar to enlarge
            Debug,                  // Brings up the Process Control Window as visible with the Debug Window.
            DebugLineNumbers,       // Brings up the Process Control Window as visible with the Debug Window with Line Numbers showing
        };

        enum class UpdateType
        {
            Off,
            OffOnce,
            On,

        };

        __sagestatic bool SetUpdate(UpdateType eType);
        __sagestatic bool Update();

        __sagestatic bool SetGlobalString(int iIndex,const char * sString);
        __sagestatic bool SetGlobalValue(int iIndex,const char * sString);
        __sagestatic bool SetGlobalValue(int iIndex,long long iValue);
        __sagestatic bool SetGlobalValue(int iIndex,double fValue);
        __sagestatic bool SetGlobalValue(int iIndex,int iValue);
        __sagestatic bool SetGlobalValue(int iIndex,unsigned int iValue);
        __sagestatic bool SetGlobalValue(int iIndex,unsigned long long uiValue);
        __sagestatic bool SetGlobalValue(int iIndex, void* vPointer);
        __sagestatic bool SetGlobalValue(int iIndex, bool bValue);

        __sagestatic bool SetGlobalString(const char * sName,const char * sString);
        __sagestatic bool SetGlobalValue(const char * sName,const char * sString);
        __sagestatic bool SetGlobalValue(const char * sName,long long iValue);
        __sagestatic bool SetGlobalValue(const char * sName,double fValue);
        __sagestatic bool SetGlobalValue(const char * sName,int iValue);
        __sagestatic bool SetGlobalValue(const char * sName,unsigned int iValue);
        __sagestatic bool SetGlobalValue(const char * sName,unsigned long long uiValue);
        __sagestatic bool SetGlobalValue(const char* sName, void* vPointer);
        __sagestatic bool SetGlobalValue(const char* sName, bool bValue);

        __sagestatic const char * GetGlobalString(int iIndex);
        __sagestatic long long GetGlobalValue(int iIndex);
        __sagestatic double GetGlobalDouble(int iIndex);
        __sagestatic int GetGlobalInt(int iIndex);
        __sagestatic unsigned int GetGlobalUInt(int iIndex);
        __sagestatic unsigned long long GetGlobalULongLong(int iIndex);
        __sagestatic void * GetGlobalPointer(int iIndex);
        __sagestatic bool GetGlobalBool(int iIndex);

        __sagestatic const char * GetGlobalString(const char * sName);
        __sagestatic long long GetGlobalValue(const char * sName);
        __sagestatic double GetGlobalDouble(const char * sName);
        __sagestatic int GetGlobalInt(const char * sName);
        __sagestatic unsigned int GetGlobalUInt(const char * sName);
        __sagestatic unsigned long long GetGlobalULongLong(const char * sName);
        __sagestatic void* GetGlobalPointer(const char* sName);
        __sagestatic bool GetGlobalBool(const char* sName);

        // Init() -- Initialize the process control window.
        //
        // Init() initializes the process control window and initially brings it up as hidden (by default), unless overridden. 
        //
        // Note: when the Process Control Window is hidden, you can use the System Menu in any main window to bring it up, or 
        // move the mouse to the uper-right-hand corner of the window for more than .25 seconds (which toggles the Process Control Window display)
        //
        // Init() may be called with one of the following options
        //
        //      Init() or Init(false)                   -- Bring up Process Control Window as hidden.
        //      Init(true)                              -- Brings up the Process Control Window as a title bar in the 
        //                                                 upper-right of the window.  Move the mouse over the window to enlarge.
        //      Init(Debug::InitType::Debug)            -- Brings up the Process Control Window with the Debug Window showing. This will
        //                                                 not auto-minimize as it does when the "Show Debug" switch is off. 
        //      Init(Debug::InitType::DebugLineNumbers)            -- Same as WinDebug::InitType::Debug, but with line numbers turn on
        //                                                            (see Debug:ShowLineNumbers())
        //
        // Init() is not necessary for debug output -- the first call to any debug output routine will automatically initalize
        // and display the Process Control Window and its debug window. 
        //
        // returns TRUE if Process Window is active. 
        //
        __sagestatic bool Init(bool bVisible = false);
        
        // Init() -- Initialize the process control window. 
        //
        // Init() initializes the process control window and initially brings it up as hidden (by default), unless overridden. 
        //
        // Note: when the Process Control Window is hidden, you can use the System Menu in any main window to bring it up, or 
        // move the mouse to the uper-right-hand corner of the window for more than .25 seconds (which toggles the Process Control Window display)
        //
        // Init() may be called with one of the following options
        //
        //      Init() or Init(false)                   -- Bring up Process Control Window as hidden.
        //      Init(true)                              -- Brings up the Process Control Window as a title bar in the 
        //                                                 upper-right of the window.  Move the mouse over the window to enlarge.
        //      Init(Debug::InitType::Debug)            -- Brings up the Process Control Window with the Debug Window showing. This will
        //                                                 not auto-minimize as it does when the "Show Debug" switch is off. 
        //      Init(Debug::InitType::DebugLineNumbers)            -- Same as WinDebug::InitType::Debug, but with line numbers turn on
        //                                                            (see Debug:ShowLineNumbers())
        //
        // Init() is not necessary for debug output -- the first call to any debug output routine will automatically initalize
        // and display the Process Control Window and its debug window. 
        //
        // returns TRUE if Process Window is active. 
        //
         __sagestatic bool Init(InitType eInitType);
        
        
        // Show() -- Show or Hide the Process Control Window
        //
         __sagestatic bool Show(bool bShow = true); 

        // Hide() -- Hide or Show the Process Control Window
        //
         __sagestatic bool Hide(bool bHide = true);

        // printf() -- print out to the debug window, just as in std::printf()
        //
         __sagestatic bool printf(const char * Format,...);
// __Sandbox__Except__ deprecated (for now)
//#ifdef __Sandbox__Except__
        // printf() -- print out to the debug window, just as in std::printf()
        //
         __sagestatic bool __printf(const char * Format,...);
//#endif
        // Write() -- Write out to the debug window.  Can be quicker than using printf()
        //
         __sagestatic bool Write(const char * sText);

        // KillTimer() -- Kills the 250ms timer that looks at mouse movement.  
        //
        // KillTimer() removes the timer in the Process Window.  It is activated every 250ms to look for mousemovements to 
        // show or hide the Process Control Window when the mouse is moved to the upper-right-hand corner of the desktop. 
        //
        // The timer doesn't impact program execution time, but for those programs that do not want a timer for one simple purpose
        // can turn it off. 
        //
        // Use KillTimer(false) to turn the timer back on.
        //
         __sagestatic bool KillTimer(bool bKillTimer = true);

        // ShowLineNumbers() -- Shows line numbers in the debug display.
        //
        // The Debug window defaults to ShowLineNumbers off.  You can use 
        // Init(CWindow::Debug::InitType::DebugLineNumbers) to initialize the window with line numbers on.
        //
        // Use ShowLineNumbers(true) turn turn line numbers on
        // Use ShowLineNumbers(false) to turn line numbers off.
        //
         __sagestatic bool ShowLineNumbers(bool bShowLineNumbers = true);

        // -------------------------------------------------------
        // CSagebox::Debug unique items (may be called by CWindow)
        // -------------------------------------------------------

        // SetControlCBreak() -- Allow ^C to terminate the process. 
        //
        // When set to TRUE, using Control-C will bring up a message box and allow the user to terminate the process. 
        // This is a useful debugging tool to allow terminating the process if the process caught in a loop or may be otherwise
        // unresponsive.  Also use debug.Init() to activate the Process Window for more control, even if the window is not visible. 
        //
        // For Console Applications, you can just press control-C in the console-window. 
        //
        // Note: debug.Init() activates ^C, but debug.Write(), when it instantiates the Proess Control Window will not turn it on. 
        // use debug.Init() or SetControlCBreak() to ensure use of ^C to break the program execution.
        //
        __sagestatic bool SetControlCBreak(bool bActive); 

     };

    __sagestatic Debug debug; 

    // This is a pointer to the global CSagebox.  There is only one per application.  This can 
    // be automatically allocated by using static Sagebox functions.
    //
    // Use GetStaticSagebox() to retrieve the value.  it will create the sagebox obejct if it doesn't exist. 
    //
    static CSagebox * m_cStaticSagebox;

    __sagestatic CWindow * GetSageWindow(); 
    __sagestatic bool SetGlobalSageEventHook(SageEventHookFunc fHookFunc,void * vpData = nullptr,int64_t ullExtraData = 0); 
    __sagestatic bool RemoveGlobalSageEventHooks(); 
    __sagestatic bool RemoveGlobalSageEventHook(SageEventHookFunc fHookFunc); 

    /// <summary>
    /// Initialize static Sagebox.  Most functions will initialize Sagebox automatically. 
    /// <para></para>
    /// However, there are a few Sagebox functions that will return unprocress if Sagebox has not been created when called.
    /// <para></para>
    /// --> CSagebox::Init() will initialize Sagebox statitcally to ensure all static CSagebox functions run correctly.
    /// <para></para>&#160;&#160;&#160;
    /// --> Note: When specifyin a Sagebox object, i.e. "CSagebox cSagebox", this initializes Sagebox and CSagebox::Init() is unnecessary.
    /// <para></para>&#160;&#160;&#160;
    /// --> Important Note:  Do not create a Sagebox object after using CSagebox::Init() -- Use GetStaticSagebox() instead.
    /// </summary>
    /// <returns></returns>
    __sagestatic void Init(); 

    /// <summary>
    /// Initializes Windows GDI+. This only needs to be called once and is initialized for the entire application.  InitGdiPlus() does not need to be called
    /// for every Window, and is provided in the CWindow class for ease-of-use.
    /// <para></para>&#160;&#160;&#160;
    /// --> Note:  For individual windows, you can call CWindow::CreateGdiObject(), which will return a GDI Graphics object for the current window.  This will also 
    /// <para></para>&#160;&#160;&#160;
    /// --> call InitGdiPlus() automatically, eliminating the need to call InitGdiPlus()
    /// See CreateGDIObject() for more information.             
    /// </summary>
    /// <returns></returns>
    __sagestatic bool InitGdiPlus();

    /// <summary>
    /// Waits for the monitor VSync blank time to start before returning.  This will sync your graphics with the graphic output to the monitor. 
    /// <para></para>
    /// --> Note: DirectDraw() must be available on your system and initialize correctly.  Otherwise, VsyncWait() will return immediately.
    /// <para></para>
    /// --> Use VsyncIsValid() to check for initialization status (a return of TRUE indicates the VsyncWait() is working correctly)
    /// <para></para>
    /// --> Important Note: VsyncReady() and VsyncWait() are mutually exclusive. Using them together will cause performance issues. 
    /// </summary>
    /// <returns></returns>
    __sagestatic bool VsyncWait(bool bWaitEnd = false); 
    __sagestatic bool VsyncWaitBegin();
    __sagestatic bool VsyncWaitEnd();

    /// <summary>
    /// Returns TRUE if the VsyncWait and VsyncThread capabilities are accurate (i.e. if DirectDraw() was initialized and can look for the Vertical ReSync)
    /// <para></para>&#160;&#160;&#160;
    /// --> Note this will initialize DirectDraw() and put the application in a performance mode, using more processing time (i.e. it assumes you are using graphics by checking its status)
    /// <para></para>
    /// Use this as a DEBUG function to check the validity of Vsync operations (once true, then there is no need to check)
    /// </summary>
    /// <returns></returns>
    __sagestatic bool VsyncIsValid();

    /// <summary>
    /// Returns true if a Vsync blank period has started, setting the status on the Vertical Retrace of your monitor
    /// <para></para>&#160;&#160;&#160;
    /// --> Note: This will start a passive thread to monitor the Vsync state (see: VsyncStartThread()), which will then send an event to your window, causing
    /// VsyncReady() to return true.
    /// <para></para>
    /// VSyncReady() is an event, and therefore will return TRUE only once until the Vsync is signaled again, unless Peek::Yes is used to intentially not reset it.
    /// <para></para>
    /// --> Important Note: VsyncReady() and VsyncWait() are mutually exclusive. Using them together will cause performance issues. 
    /// <para></para>
    /// --> Note: Only one window can use the Vsync Thread at a time.  Using the thread in another window will cause the thread to be reset to that window.
    /// </summary>
    /// <param name="cWin">Window to send Sage Event to when Vsync is ready</param>
    /// <param name="peek">Does not change the event status if Peek == Peek::Yes</param>
    /// <returns></returns>
    __sagestatic bool VsyncReady(CWindow & cWin,Peek peek = Peek::No);

    /// <summary>
    /// Starts the main Vsync thread to send a VsyncReady() event to your window with the Vertical Retrace occurs on your monitor. 
    /// <para></para>
    /// --> Use VsyncStartThread() with no window to use the global Sagebox Window
    /// <para></para>
    /// Using VsyncReady() starts the thread automatically.  VsyncStartThread() can be used as a pre-step to make sure the thread is operating
    /// and valid (see VsyncIsValid()) before entering the main loop, saving an initial burst of processing time on the first VsyncReady() call. 
    /// <para></para>
    /// --> The Vsync Thread is a passive thread that sleeps until the Vertical Retrace occurs, and takes little processing time. 
    /// <para></para>
    /// --> Note: Only one window can use the Vsync Thread at a time.  Using the thread in another window will cause the thread to be reset to that window.
    /// <para></para>
    /// --> Note: All Vsync functions assume graphics processing and put your application in a performance mode, using more CPU time, but will
    /// not cause your system to become sluggish unless you manually set your application to a Realtime or High-priority mode.
    /// </summary>
    /// <param name="cWin">Window to send VsyncReady() events</param>
    /// <returns></returns>
    __sagestatic bool VsyncStartThread();

    /// <summary>
    /// Starts the main Vsync thread to send a VsyncReady() event to your window with the Vertical Retrace occurs on your monitor. 
    /// <para></para>
    /// --> Use VsyncStartThread() with no window to use the global Sagebox Window
    /// <para></para>
    /// Using VsyncReady() starts the thread automatically.  VsyncStartThread() can be used as a pre-step to make sure the thread is operating
    /// and valid (see VsyncIsValid()) before entering the main loop, saving an initial burst of processing time on the first VsyncReady() call. 
    /// <para></para>
    /// --> The Vsync Thread is a passive thread that sleeps until the Vertical Retrace occurs, and takes little processing time. 
    /// <para></para>
    /// --> Note: Only one window can use the Vsync Thread at a time.  Using the thread in another window will cause the thread to be reset to that window.
    /// <para></para>
    /// --> Note: All Vsync functions assume graphics processing and put your application in a performance mode, using more CPU time, but will
    /// not cause your system to become sluggish unless you manually set your application to a Realtime or High-priority mode.
    /// </summary>
    /// <param name="cWin">Window to send VsyncReady() events</param>
    /// <returns></returns>
    __sagestatic bool VsyncStartThread(CWindow & cWin);


    /// <summary>
    /// Ends the Vsync Retrace Thread.  Use this when finished with your graphics loop using the vertical retrace. 
    /// <para></para>
    /// --> The VSync Thread is passive and is ok to leave running, as it will not take up too much processing time. 
    /// </summary>
    /// <returns></returns>
    __sagestatic bool VsyncEndThread();

    // Get the size of the desktop (i.e. monitor). 
    // This returns the current monitor size (i.e. 1920x1080).  This can help in centering and otherwize
    // placing windows and controls.
    //
    // Currently, this returns the active desktop and will be updated to work with systems with multiple monitors in a future update.
    //
    __sagestatic SIZE GetDesktopSize(); 

 
    /// <summary>
    /// Sets "Quick C++" mode that activates the a number of debug and development Sagebox functions for easier development:
    /// <para></para>
    /// - Sets timer-based updates.  All windows that need updates are updated automatically every 20ms.
    /// <para></para>
    /// - Turns on the debug window as initially hidden so that moving the mouse to the upper-right corner will display it and allow easy program termination by pressing the "Terminate Program" button
    /// <para></para>
    /// - Sets word wrap on for windowed text
    /// <para></para>
    /// - Sets ^C on to allow Control-C to break out of the program and dialog boxes. 
    /// <para></para>
    /// - Adds menu option in system menu for all windows to allow terminating program from the system menu.
    /// <para></para>
    /// ---> You can also use opt::QuickCpp() in Sagebox::NewWindow() as an option when creating an initialwindow rather than a separate call.
    /// </summary>
    /// <returns></returns>
    __sagestatic void SetQuickCpp();

    /// <summary>
    /// Sets a number of debug and development Sagebox functions for easier debugging and development
    /// <para></para>
    /// - Turns on the debug window as initially hidden so that moving the mouse to the upper-right corner will display it and allow easy program termination by pressing the "Terminate Program" button
    /// <para></para>
    /// - Sets word wrap on for windowed text
    /// <para></para>
    /// - Sets ^C on to allow Control-C to break out of the program and dialog boxes. 
    /// <para></para>
    /// - Adds menu option in system menu for all windows to allow terminating program from the system menu.
    /// <para></para>
    /// ---> You can also use opt::DebugMode() in Sagebox::NewWindow() as an option when creating an initialwindow rather than a separate call.
    /// </summary>
    /// <returns></returns>
    __sagestatic void SetDebugMode();

    __sagestatic void __AttachInput(bool bAttach);
    __sagestatic CDataStore * __GetDataStoreClassPointer();   

    __sagestatic CDataStore * __RegisterandIncrementUserStore(int & iUserdID,CDataStore::DataStore_t * & stDataStore);
    __sagestatic CDataStore::DataStore_t * __QuickRegisterUserDataStore();

    /// #ImgView
    /// <summary>
    /// Shows a bitmap in a separate window, with an optional Image Zoom Window (Zoombox) that can be used to
    /// <para></para>
    /// navigate in the image and with multiple windows using the ImgShow() functions.
    /// <para></para>&#160;&#160;&#160;
    /// - You can move the image around with the mouse and zoom in and out with the mousewheel. 
    /// <para></para>&#160;&#160;&#160;
    /// - Right-click the mouse to reset the view to the original image.
    /// <para></para>&#160;&#160;&#160;
    /// - Use the system menu (click on the upper-left box in the window) to get instructions and for more tools.
    /// <para></para>
    /// --> options include:<para></para>
    /// opt::Title() - Sets the title of the Window (in the upper title bar)<para></para>
    /// opt::Label() -- Sets the title of the window and adds a label<para></para>
    /// --> Also see ImgOpt options: ImgOpt::FillZoom(), ImgOpt::Maximize(), ImgOpt::ZoomBox(), ImgOpt::Percent()<para></para>
    /// --> Note:  You do not need to assign an object.  Use the Delete() function to close the window programatically.
    /// </summary>
    /// <param name="cBitmap"> - Input Bitmap</param>
    /// <param name="pWinLoc"> - [optional] Location for image window</param>
    /// <param name="szWinSize"> - [optional] Size of Image Window</param>
    /// <param name="cwOpt"> - [optional] Additional options</param>
    /// <returns>Returns a CImgView object where you can look for changes and update the image.  This is optional (you don't have to assign an object, as Sagebox manages the window and input)</returns>
    __sagestatic CImgView ImgView(CBitmap & cBitmap,const cwfOpt & cwOpt = cwfOpt());                                    // #ImgView
    __sagestatic CImgView ImgView(CBitmap & cBitmap,SIZE szWinSize,const cwfOpt & cwOpt = cwfOpt());                     // #ImgView
    __sagestatic CImgView ImgView(CBitmap & cBitmap,POINT pWinLoc,const cwfOpt & cwOpt = cwfOpt());                      // #ImgView
    __sagestatic CImgView ImgView(CBitmap & cBitmap,POINT pWinLoc,SIZE szWinSize,const cwfOpt & cwOpt = cwfOpt());       // #ImgView

    __sagestatic CImgView ImgView(const char * sPath,const cwfOpt & cwOpt = cwfOpt());                                    // #ImgView
    __sagestatic CImgView ImgView(const char * sPath,SIZE szWinSize,const cwfOpt & cwOpt = cwfOpt());                     // #ImgView
    __sagestatic CImgView ImgView(const char * sPath,POINT pWinLoc,const cwfOpt & cwOpt = cwfOpt());                      // #ImgView
    __sagestatic CImgView ImgView(const char * sPath,POINT pWinLoc,SIZE szWinSize,const cwfOpt & cwOpt = cwfOpt());       // #ImgView


    /// #ImgViewR
    /// <summary>
    /// This function is the same as ImgShow(), but puts the image upsidedown.
    /// <para></para>
    /// opt::Reversed() may also be used in ImgShow() for the same effect.
    /// <para></para>
    /// --> See ImgShow() for more information (parameter information, available options, etc.)
    /// </summary>
    __sagestatic CImgView ImgViewR(CBitmap & cBitmap,const cwfOpt & cwOpt = cwfOpt())                               ;       // #ImgViewR
    __sagestatic CImgView ImgViewR(CBitmap & cBitmap,SIZE szWinSize,const cwfOpt & cwOpt = cwfOpt())                ;       // #ImgViewR
    __sagestatic CImgView ImgViewR(CBitmap & cBitmap,POINT pWinLoc,const cwfOpt & cwOpt = cwfOpt())                 ;       // #ImgViewR
    __sagestatic CImgView ImgViewR(CBitmap & cBitmap,POINT pWinLoc,SIZE szWinSize,const cwfOpt & cwOpt = cwfOpt())  ;       // #ImgViewR

    __sagestatic CImgView ImgViewR(const char * ,const cwfOpt & cwOpt = cwfOpt())                               ;       // #ImgViewR
    __sagestatic CImgView ImgViewR(const char * ,SIZE szWinSize,const cwfOpt & cwOpt = cwfOpt())                ;       // #ImgViewR
    __sagestatic CImgView ImgViewR(const char * ,POINT pWinLoc,const cwfOpt & cwOpt = cwfOpt())                 ;       // #ImgViewR
    __sagestatic CImgView ImgViewR(const char * ,POINT pWinLoc,SIZE szWinSize,const cwfOpt & cwOpt = cwfOpt())  ;       // #ImgViewR

    /// <summary>
    /// This function is the same as ImgShow(), but adds a ZoomBox.
    /// <para></para>
    /// ImgOpt::ZoomBox() may also be used in ImgShow() for the same effect.
    /// <para></para>
    /// --> See ImgShow() for more information (parameter information, available options, etc.)
    /// </summary>
    __sagestatic CImgView ImgZoom(CBitmap & cBitmap,const cwfOpt & cwOpt = cwfOpt())                                ;
    __sagestatic CImgView ImgZoom(CBitmap & cBitmap,SIZE szWinSize,const cwfOpt & cwOpt = cwfOpt())                 ;
    __sagestatic CImgView ImgZoom(CBitmap & cBitmap,POINT pWinLoc,const cwfOpt & cwOpt = cwfOpt())                  ;
    __sagestatic CImgView ImgZoom(CBitmap & cBitmap,POINT pWinLoc,SIZE szWinSize,const cwfOpt & cwOpt = cwfOpt())   ;

    __sagestatic CImgView ImgZoom(const char * sBitmap,const cwfOpt & cwOpt = cwfOpt())                                ;
    __sagestatic CImgView ImgZoom(const char * sBitmap,SIZE szWinSize,const cwfOpt & cwOpt = cwfOpt())                 ;
    __sagestatic CImgView ImgZoom(const char * sBitmap,POINT pWinLoc,const cwfOpt & cwOpt = cwfOpt())                  ;
    __sagestatic CImgView ImgZoom(const char * sBitmap,POINT pWinLoc,SIZE szWinSize,const cwfOpt & cwOpt = cwfOpt())   ;

    /// <summary>
    /// This function is the same as ImgShow(), but adds a ZoomBox and places the image upside-down
    /// <para></para>
    /// ImgOpt::ZoomBox() and opt::Reversed() may also be used in ImgShow() for the same effect.
    /// <para></para>
    /// --> See ImgShow() for more information (parameter information, available options, etc.)
    /// </summary>
    __sagestatic CImgView ImgZoomR(CBitmap & cBitmap,const cwfOpt & cwOpt = cwfOpt())                               ;
    __sagestatic CImgView ImgZoomR(CBitmap & cBitmap,SIZE szWinSize,const cwfOpt & cwOpt = cwfOpt())                ;
    __sagestatic CImgView ImgZoomR(CBitmap & cBitmap,POINT pWinLoc,const cwfOpt & cwOpt = cwfOpt())                 ;
    __sagestatic CImgView ImgZoomR(CBitmap & cBitmap,POINT pWinLoc,SIZE szWinSize,const cwfOpt & cwOpt = cwfOpt())  ;

    __sagestatic CImgView ImgZoomR(const char * sBitmap,const cwfOpt & cwOpt = cwfOpt())                               ;
    __sagestatic CImgView ImgZoomR(const char * sBitmap,SIZE szWinSize,const cwfOpt & cwOpt = cwfOpt())                ;
    __sagestatic CImgView ImgZoomR(const char * sBitmap,POINT pWinLoc,const cwfOpt & cwOpt = cwfOpt())                 ;
    __sagestatic CImgView ImgZoomR(const char * sBitmap,POINT pWinLoc,SIZE szWinSize,const cwfOpt & cwOpt = cwfOpt())  ;


    /// <summary>
    /// Display a Window with a Before and After Image, including a ZoomBox window to help navigate in the image. 
    /// <para></para>&#160;&#160;&#160;
    /// - You can move the image around with the mouse and zoom in and out with the mousewheel. 
    /// <para></para>&#160;&#160;&#160;
    /// - Right-click the mouse to reset the view to the original image.
    /// <para></para>&#160;&#160;&#160;
    /// - Use the system menu (click on the upper-left box in the window) to get instructions and for more tools.
    /// <para></para>
    /// --> options include:<para></para>
    /// opt::Title() - Sets the title of the Window (in the upper title bar)<para></para>
    /// opt::Label() -- Sets the label above the two images<para></para>
    /// --> Also see ImgOpt options: , ImgOpt::Maximize(), ImgOpt::ZoomBox(), ImgOpt::Percent()<para></para>
    /// --> Note:  You do not need to assign an object.  Use the Delete() function to close the window programatically.
    /// </summary>
    /// <param name="cBefore">Before (i.e. Original) image</param>
    /// <param name="cAfter">After (i.e. Resultant) Image</param>
    /// <param name="cwOpt">[optional] Additional Options</param>
    /// <returns>Returns a CBeforeAfter object where you can look for changes and update the image.  This is optional (you don't have to assign an object, as Sagebox manages the window and input)</returns>
    __sagestatic CBeforeAfterImage ImgBeforeAfter(const char * sTitle,CBitmap & cBefore,CBitmap & cAfter,const cwfOpt & cwOpt = cwfOpt());
    __sagestatic CBeforeAfterImage ImgBeforeAfter(CBitmap & cBefore,CBitmap & cAfter,const cwfOpt & cwOpt = cwfOpt());

    /// <summary>
    /// This function is the same as ImgBeforeAfter(), places the image upside-down
    /// <para></para>
    /// opt::Reversed() may also be used in ImgBeforeAfter() for the same effect.
    /// <para></para>
    /// --> See ImgBeforeAfter() for more information (parameter information, available options, etc.)
    /// </summary>
    __sagestatic CBeforeAfterImage ImgBeforeAfterR(const char * sTitle,CBitmap & cBefore,CBitmap & cAfter,const cwfOpt & cwOpt = cwfOpt());
    __sagestatic CBeforeAfterImage ImgBeforeAfterR(CBitmap & cBefore,CBitmap & cAfter,const cwfOpt & cwOpt = cwfOpt());


    /// <summary>
    /// Wait for primary windows to close before continuing.<para></para>
    /// This includes Image windows (i.e. ImgView() type windows), as well as any window<para></para>
    /// that uses __WindowSetAsPrimary() to set this status.<para></para>
    /// <para></para>
    /// --> This function returns false if there were no windows pending, so that function like ExitButton() can be used if there is no wait.
    /// <para></para>
    /// --> Note: WaitPending() and WaitforPending() are the same function
    /// </summary>
    /// <param name="iMaxTime">Maximum number of milliseconds to wait before giving up and continuing (returning false)</param>
    /// <returns>milliseconds waited for window to close (maxes out at INT_MAX).  Returns 0 if there were no windows visible (i.e. returned immediately)</returns>
    __sagestatic int WaitforPending(int iMaxTime = 0); 

    /// <summary>
    /// Returns true if any window open was closed.  This includes windows that are allowed to close.  If a window that has its close disabled 
    /// this window will not close and will not be reported.
    /// <para></para>
    /// See WindowClosePending() to react to any close window press (or attempt by the system to close the window), regardless of its 'close disabled' status. 
    /// <para></para>
    /// --> Note that this function is an event and will return true only one time per-event (and will return false until a subsequent event of the same type is issued)
    /// </summary>
    /// <param name="peek">When set to Peek::Yes, the event is not reset and will return true again for the same event</param>
    /// <returns></returns>
    __sagestatic bool WindowClosed(Peek peek = Peek::No);

    /// <summary>
    ///  Returns True if a Close Window Button was pressed for any open window (or the system is trying to close the window). This will return true
    /// even if the close is disabled.  Use WindowClosed() to only retrieve close events for windows allowed to close.
    /// </summary>
    /// <param name="peek">When set to Peek::Yes, the event is not reset and will return true again for the same event</param>
    /// <returns></returns>
    __sagestatic bool WindowClosePending(Peek peek = Peek::No);

    /// <summary>
    /// Wait for primary windows to close before continuing.<para></para>
    /// This includes Image windows (i.e. ImgView() type windows), as well as any window<para></para>
    /// that uses __WindowSetAsPrimary() to set this status.<para></para>
    /// <para></para>
    /// --> This function returns false if there were no windows pending, so that function like ExitButton() can be used if there is no wait.
    /// <para></para>
    /// --> Note: WaitPending() and WaitforPending() are the same function
    /// </summary>
    /// <param name="iMaxTime">Maximum number of milliseconds to wait before giving up and continuing (returning false)</param>
    /// <returns>milliseconds waited for window to close (maxes out at INT_MAX).  Returns 0 if there were no windows visible (i.e. returned immediately)</returns>
    __sagestatic int WaitPending(int iMaxTime = 0); 

    /// <summary>
    /// Wait for any open window to close before continuing.<para></para>
    /// This does not include Window Close Presses if the window close is disabled.  Use WaitClosePressedAny() to also look for close button presses even if the close is disabled for that window.
    /// <para></para>
    /// This includes Image windows (i.e. ImgView() type windows), as well as any window<para></para>
    /// that uses __WindowSetAsPrimary() to set this status.<para></para>
    /// <para></para>
    /// --> This function immediately if there are no windows currently open.
    /// </summary>
    /// <param name="iMaxTime">Maximum number of milliseconds to wait before giving up and continuing (returning false)</param>
    /// <returns>milliseconds waited for window to close (maxes out at INT_MAX).  Returns 0 if there were no windows visible (i.e. returned immediately)</returns>
    __sagestatic int WaitCloseAny(int iMaxTime = 0);

    /// <summary>
    /// Wait for any open window to attempt to close before continuing.<para></para>
    /// This also includes Window Close Presses even if the window close is disabled.  Use WaitCloseAny() to only look for windows allowed to close.
    /// <para></para>
    /// This includes Image windows (i.e. ImgView() type windows), as well as any window<para></para>
    /// that uses __WindowSetAsPrimary() to set this status.<para></para>
    /// <para></para>
    /// --> This function immediately if there are no windows currently open.
    /// </summary>
    /// <param name="iMaxTime">Maximum number of milliseconds to wait before giving up and continuing (returning false)</param>
    /// <returns>milliseconds waited for window to close (maxes out at INT_MAX).  Returns 0 if there were no windows visible (i.e. returned immediately)</returns>
    __sagestatic int WaitClosePressedAny(int iMaxTime = 0);

    /// <summary>
    ///  Returns the number of primary windows (i.e. main-level windows (set with __WindowSetasPrimary())
    /// <para></para>
    /// that are visible on the desktop, such as ImgView() windows, etc. 
    /// </summary>
    /// <returns></returns>
    __sagestatic int WindowsPending(); 

    /// <summary>
    /// Returns true of WaitforPending() has been called and it is still waiting for closure of all primary windows.
    /// </summary>
    /// <returns></returns>
    __sagestatic bool __inWaitPending();

    /// <summary>
    /// Sends an event through the Sagebox procedural-based event system, forcing a wakeup for a routine in GetEvent()
    /// <para></para>
    /// Sent messages are processed immediately, which can hang if this is called from a routine called from the Windows Event Loop.
    /// <para></para>
    /// --> See PostSageboxEvent();
    /// </summary>
    /// <returns></returns>
    __sagestatic bool SendSageboxEvent();

    __sagestatic bool EventPending(Peek peek = Peek::No);


    /// <summary>
    /// Posts an event through the Sagebox procedural-based event system, forcing a wakeup for a routine in GetEvent();
    /// <para></para>
    /// This will post the message in the message queue, to be prcessed after any current or pending messages.
    /// <para></para>
    /// This is safer to use in a function that is (or may be) in the WIndows Event Thread, as it will not be sent to the
    /// <para></para>
    /// message queue until any function called from the message queue has returned.
    /// </summary>
    /// <returns></returns>
    __sagestatic bool PostSageboxEvent();



    // GetTextSize() -- Get the text size of the text using the current font.
    //
    // This returns the size the text will use in the window.  This can help with the placement of the text or controls around the text.
    // For example, using auto szSize = GetTextSize("This is some Text"), and then Write((szDesktopSize.cx-szSize.cx)/2,100,MyText)) 
    // will center the text in the X plane.
    //
    __sagestatic SIZE GetTextSize(const wchar_t * sText);                                                                                                             
  
    // GetTextSize() -- Get the text size of the text using the current font.
    //
    // This returns the size the text will use in the window.  This can help with the placement of the text or controls around the text.
    // For example, using auto szSize = GetTextSize("This is some Text"), and then Write((szDesktopSize.cx-szSize.cx)/2,100,MyText)) 
    // will center the text in the X plane.
    //
    __sagestatic SIZE GetTextSize(HFONT hFont,const wchar_t * sText);                                                                                                 

    // GetTextSize() -- Get the text size of the text using the current font.
    //
    // This returns the size the text will use in the window.  This can help with the placement of the text or controls around the text.
    // For example, using auto szSize = GetTextSize("This is some Text"), and then Write((szDesktopSize.cx-szSize.cx)/2,100,MyText)) 
    // will center the text in the X plane.
    //
    __sagestatic bool GetTextSize(const char * sText,SIZE & Size);                                                                                                    

    // GetTextSize() -- Get the text size of the text using the current font.
    //
    // This returns the size the text will use in the window.  This can help with the placement of the text or controls around the text.
    // For example, using auto szSize = GetTextSize("This is some Text"), and then Write((szDesktopSize.cx-szSize.cx)/2,100,MyText)) 
    // will center the text in the X plane.
    //
    __sagestatic SIZE GetTextSize(const char * sText);                                      
    __sagestatic SIZE GetTextSize(HFONT hFont,const char * sText);                          
    __sagestatic bool GetTextSize(HFONT hFont,const char * sText,SIZE & Size);              
    __sagestatic SIZE GetTextSize(const char * sFont,const char * sText);                   
    __sagestatic bool GetTextSize(const char * sFont,const char * sText,SIZE & Size);       
    __sagestatic SIZE GetTextSize(const char * sFont,const wchar_t * sText);                
    __sagestatic SIZE GetTextSize(int iFontSize,const wchar_t * sText);                     
    __sagestatic SIZE GetTextSize(int iFontSize,const char * sText);                        
    __sagestatic bool GetTextSize(int iFontSize,const char * sText,SIZE & Size);            

    __sagestatic RgbColor GetDefaultBgColor();
    __sagestatic RgbColor GetDefaultFgColor();

    __sagestatic CQuickForm QuickForm(const char * sFormType,int iWidth,int iHeight,const cwfOpt & cwOpt = CWindow::cwNoOpt);
    __sagestatic CQuickForm QuickForm(int iWidth,int iHeight,const cwfOpt & cwOpt = CWindow::cwNoOpt);
    __sagestatic CQuickForm QuickForm(const char * sFormType,SIZE szWinSize,const cwfOpt & cwOpt = CWindow::cwNoOpt);
    __sagestatic CQuickForm QuickForm(SIZE szWinSize,const cwfOpt & cwOpt = CWindow::cwNoOpt);
    __sagestatic CQuickForm QuickForm(const char * sFormType,const cwfOpt & cwOpt = CWindow::cwNoOpt);
    __sagestatic CQuickForm QuickForm(const cwfOpt & cwOpt = CWindow::cwNoOpt);

    __sagestatic CDataStore * DataStore();

    __sagestatic void __SetInternalCallback(void * fCallback, void * pData);

    template<typename T>
    __sagestatic void SetEventCallback(void (*fCallback)(T *),T * p) { __SetInternalCallback((void *) fCallback,(void *) p);  }
    template<typename T>
    __sagestatic void SetEventCallback(void (*fCallback)(T *),T & p) { __SetInternalCallback((void *) fCallback,(void *) &p);  }
    __sagestatic void SetEventCallback(void (*fCallback)());

    template<typename T>
    __sagestatic void SetEventHandler(void (*fCallback)(T *),T * p) { __SetInternalCallback((void *) fCallback,(void *) p);  }
    template<typename T>
    __sagestatic void SetEventHandler(void (*fCallback)(T *),T & p) { __SetInternalCallback((void *) fCallback,(void *) &p);  }
    __sagestatic void SetEventHandler(void (*fCallback)());
    __sagestatic void SetEventHandler(std::function<void()> const & fFunction);

    __sagestatic void ResetEventCallback();

     __sagestatic long long WaitforProcess(long long (*fCallback)(void *), void * pData);
    template<typename T>
    __sagestatic long long WaitforProcess(long long (*fCallback)(T *),T & p) { return WaitforProcess((long long (*)(void *)) fCallback,(void *) &p);  }
    template<typename T>
    __sagestatic long long WaitforProcess(long long (*fCallback)(T *),T * p) { return WaitforProcess((long long (*)(void *)) fCallback,(void *) p);  }
    __sagestatic long long WaitforProcess(std::function<long long()> const & Function);
    __sagestatic void WaitforProcess(std::function<void()> const & Function);
    __sagestatic void ShowImgViewInstructions();

};
#include "CSageTypes2.h" // items applicable after Classes and Types are defined. 

}; // namespace Sage;




#endif // _CSageBox_H_