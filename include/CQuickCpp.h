//#pragma once -- temporarily removed.  Tying to find the source of a Microsoft "intellisense" issue.
#include "CSagebox.h"
#if !defined (__QuickCpp_h__)
#define __QuickCpp_h__

// __SAGE_QUICKCPP_PURE_WINDOWSMAIN__ and __SAGE_SANDBOX_MERGED__ are mutually exclusive

#ifdef __SAGE_QUICKCPP_PURE_WINDOWSMAIN__
    #ifdef __SAGE_SANDBOX_MERGED__
        #undef __SAGE_SANDBOX_MERGED__
    #endif
#endif

// Reroute cin to __sage_quickcpp_mainwin (later Win)
//
// This is temporary and will probably be changed to a redirection of the CIN buffer
// (i.e. this is for the prototype and first release only)
#include <iostream>
#ifdef __SAGE_SANDBOX_MERGED__
#include "mergestdio.h"   
#endif



/// <summary>
/// Creates Quick C++ windows.  After CreateQuickCpp() is called, the main window (Win) is created as hidden. 
/// This allows operations (sizing, drawing, etc.) to occur before the window is shown. 
/// <para></para>
/// &#160;&#160;&#160;---> Note: Though initially hidden, any output to the window with any "Win." (or other) function that causes a display will cause the window to become visible.
/// <para></para>
/// &#160;&#160;&#160; ---> Use Win.Show() or Win.Hide() to Show/Hide the wndow.  Use Win.NewWindow() or Win.ChildWindow() to create new windows.
/// <para></para>
/// &#160;&#160;&#160;A Process Control Window is created for debug output and process termination for unresponsive programs.  This initially comes up
/// as a title-bar in the upper-right corner of the screen. Specify CreateQuickCPP(false) to keep it hidden.
/// <para></para>
/// &#160;&#160;&#160;Use the Process Window for debugging.  try debug.printf("This is debug text") to automatically open the Debug Window in the Process Control Window.
/// </summary>
/// <param name="bShowProcessWindow">: &#160;When TRUE (default), the Process Window is visible.  When FALSE, the Proess window is hidden</param>
void CreateQuickCpp(bool bShowProcessWindow = false);

namespace Sandbox
{

    extern CWindow * __sage_quickcpp_mainwin;

    extern CWindow::WinConsole  console;
    extern CWindow::WinDialog  dialog;
    extern CWindow::WinDebug  debug;
    extern CWindow::WinEvent  event;
    extern CWindow::WinGroup  group;
    extern WinConio conio;
    extern bool m_bInitialized;
    extern bool m_bNoExitMsg;

    extern int NoExitMsg(bool bNoExit = true);
    extern int NoExitMsg(int iReturnCode,bool bNoExit = true);
class CSetSandbox;
class CQuickCpp
{
    friend CSetSandbox;
private:
    static bool        bErrorShown;
    static bool        bIsSandbox;
public:
    static bool CreateQuickCpp(bool bShowProcessWindow = false)
    {
       CreateQuickCpp(bShowProcessWindow);
       return true;
    }

    // $$ to be removed at some point. 
    //
    #ifdef __sage_quickcpp_internal
    #include "../../__internal/__internal.h"
    #endif

    /// <summary>
    /// returns TRUE if this is a Quick C++ Sandbox application (i.e. your main was called either as a Console (+Window) app or a Windows-only app
    /// that calls your main() instead of a canonical WinMain()).
    /// <para></para>
    /// When false, using CreateQuickCpp() is required, otherwise it is optional (i.e. is ignored).
    /// <para></para>
    /// ---> This function is useful for exit procedures to put up an exit button, message, or otherwise hold up program end when not a sandbox app, and
    /// may be used to optionally execute such functions
    /// <para></para>
    /// ---> Sandbox applications automatically display an exit message, either on the console window command line or in the main window.
    /// </summary>
    /// <returns></returns>
    static bool isSandboxApp();

    CQuickCpp();

  
    static int printf(int iX,int iY,const char * Format,...);
    static int printf(int iX,int iY,const cwfOpt & cwOpt,const char * Format,...);
    static int printf(const char * Format,...);
    static int printf(const cwfOpt & cwOpt,const char * Format,...);

    static int __printf(int iX,int iY,const char * Format,...);
    static int __printf(int iX,int iY,const cwfOpt & cwOpt,const char * Format,...);
    
    static int __printf(const char * Format,...);
    static int __printf(const cwfOpt & cwOpt,const char * Format,...);
 
    // IO (i.e. in, out, and Win << and Win >> std::cout and std::cin uses

 
    // *** The following is still under-construction and somewhat experimental ***
    //
    // This allows a reference to the window, such as cWin << "Hello World" to print as in cOut. 
    // It is TBD, to make sure it doesn't interfere with other overloading. 

    CWindow::CCIO & operator << (cwfOpt & opt)          ;
    CWindow::CCIO & operator << (char * x)              ;
    CWindow::CCIO & operator << (const char * x)        ;
    CWindow::CCIO & operator << (char x)                ;
    CWindow::CCIO & operator << (CDevString & cs)       ;
    CWindow::CCIO & operator << (std::string & cs)      ;
    CWindow::CCIO & operator << (CString & cs)          ;
    CWindow::CCIO & operator << (int x)                 ;
    CWindow::CCIO & operator << (unsigned int x)        ;
    CWindow::CCIO & operator << (float x)               ;
    CWindow::CCIO & operator << (double x)              ;
    CWindow::CCIO & operator << (wchar_t * x)           ;
                                                        ;
    // Input operations, allowing CString, std::string, int, options, and double for inputs. 
    
    CWindow::CCIO & operator >> (CString & cs)          ;
    CWindow::CCIO & operator >> (std::string & cs)      ;
    CWindow::CCIO & operator >> (int & fValue)          ;
    CWindow::CCIO & operator >> (cwfOpt & opt)          ;
    CWindow::CCIO & operator >> (double & fValue)       ;
  
    // Duplicated CWindow Types

    // Class for snapping the window to the main desktop (i.e. correcting overlap)

    enum class Snap
    {
        WarnRed,            // turn window to RED so user knows it went out-of-bounds
        Snap,               // Just place it.
        SnapWarnRed,        // Place it and turn it red.
        NoAction,           // Do nothing. 
    };


    /// <summary>
    /// Gets the Sagebox CWindow object of the default, global Quick C++ Window (i.e.'win'). 
    /// <para></para>
    /// This can be used to access the cWIndow object directly and to also pass 
    /// the CWindow object to general functions that need a Sagebox Window pointer.
    /// <para></para>
    /// --> GetWindow() is the same as GetWindowObject()
    /// <para></para>
    /// --> Important Note: Use GetWindow() after the Quick C++ environment is created. With a Sandbox project, this is
    /// any time.  With standard C++ projects, this is after CreateQuickCpp() is called.
    /// </summary>
    /// <returns></returns>
    CWindow * GetWindow(); 

    /// <summary>
    /// Gets the Sagebox CWindow object of the default, global Quick C++ Window (i.e.'win'). 
    /// <para></para>
    /// This can be used to access the cWIndow object directly and to also pass 
    /// the CWindow object to general functions that need a Sagebox Window pointer.
    /// <para></para>
    /// --> GetWindow() is the same as GetWindowObject()
    /// <para></para>
    /// --> Important Note: Use GetWindow() after the Quick C++ environment is created. With a Sandbox project, this is
    /// any time.  With standard C++ projects, this is after CreateQuickCpp() is called.
    /// </summary>
    /// <returns></returns>
    CWindow * GetWindowObject(); 



    // Function that use duplicated types from CWindow (and couldn't be automated directly)

    // SnaptoWin() -- Used by Widgets and controls to make sure their window/control is completely within the parent window
    //
    // iPadX and iPadY -- How far the window needs to be out of range to SnaptoWin() to activate.
    //
    // Snap action:
    //
    //   WarnRed,            -- turn window to RED so user knows it went out-of-bounds
    //   Snap,               -- Just place it.
    //   SnapWarnRed,        -- Place it and turn it red.
    //   NoAction,           -- Do nothing. 
    //
    // CWidget -- the widget can send in its cWidget when a Snap action needs to change the color of the window (otherwise nothing is done)
    //
    bool SnaptoWin(CWindow * cWin,int iPadX = 0,int iPadY = 0,Snap eAction = Snap::Snap,CWidget * cWidget = nullptr); // $QC
    bool SnaptoDesktop(int iPadX = 0,int iPadY = 0,Snap eAction = Snap::Snap,CWidget * cWidget = nullptr); // $QC


    // ---------------------------------------------------------------------------
    // $Automated_Begin(QuickCPPH) -- Automated Interface for Quick CPP functions
    // ---------------------------------------------------------------------------

    //-->  Original Prototype: __forceinline bool EventReady() { return (!this || !m_cUserWin) ? false : m_cUserWin->EventPending();

    // Returns true if an event has occurred.  This can be used when GetEvent() isn't appropriate (such as in a loop that takes a lot of time)
    // When GetEvent() is called, the Event Pending Status is cleared back to false
    //
    static bool EventReady();

    //-->  Original Prototype: __forceinline bool ClearEvent(SageEvent eEvent) { return (!this || !m_cUserWin) ? false : m_cUserWin->ClearEvent(eEvent);

    // ClearEvent() -- Clear a specific pending event (whether it is pending or not).  This can be called before GetEvent() to clear the 
    // Event Queue of any specific event. For example, ClearEvent(SageEvent::MouseMove) or ClearEvent(SageEvent::WindowResize). 
    //
    // The latter example can be used to remove a pending window size when a Windows was initially sized with SetWindowSize(), which will
    // set a pending WindowResize() event.
    //
    // Use ClearEvents() to clear all pending events. 
    //
    static bool ClearEvent(SageEvent eEvent);

    //-->  Original Prototype: __forceinline bool ClearEvents() { return ClearEvent(SageEvent::All);

    // CLearEvents() -- Clear all pending events.  This can be used to clear any incoming events prior to calling GetEvent(). 
    // This can be useful when a setup, initialization, or other code has caused an unwanted event prior to the GetEvent() loop.
    //
    static bool ClearEvents();

    //-->  Original Prototype: bool SetMessageHandler(CWindowHandler * cHandler,void * pClassInfo = nullptr);

    // SetMessageHandler() -- Set the message handler for the window.  This overrides the default message handler.
    // This allows events such as OnMouseMove(), OnButton() press, as well as all other Windows Messages to be 
    // acted upon as received by windows. 
    //
    static bool SetMessageHandler(CWindowHandler * cHandler,void * pClassInfo = nullptr);

    //-->  Original Prototype: bool SetMessageHandler(CWindowHandler & cHandler,void * pClassInfo = nullptr);

    // SetMessageHandler() -- Set the message handler for the window.  This overrides the default message handler.
    // This allows events such as OnMouseMove(), OnButton() press, as well as all other Windows Messages to be 
    // acted upon as received by windows. 
    //
    static bool SetMessageHandler(CWindowHandler & cHandler,void * pClassInfo = nullptr);

    //-->  Original Prototype: int WinMessageBox(const char * sMessage,const char * sTitle = nullptr,unsigned int dwFlags = MB_OK);

    // WinMessageBox() -- Bring up a standard Windows Message Box. 
    //
    // This follows the same format as ::MessageBox() in windows and simply calls it in the same way. 
    //
    // This allows more "Windows-like" messages, information, warnings, etc.
    //
    // Example: WinMessageBox("This is the message","This is the title",MB_OK | MB_ICONINFO)
    //
    static int WinMessageBox(const char * sMessage,const char * sTitle = nullptr,unsigned int dwFlags = MB_OK);

    //-->  Original Prototype: int GetInteger(const char * sTitle,bool & bCancelled,const cwfOpt & cwOptions = cwfOpt());

    /// <summary>
    /// Gets an integer from a dialog box.
    /// <para></para>
    /// Options Supported:
    /// <para></para>
    /// --> Range() - Set range for integer
    /// <para></para>
    /// --> Modal() - Make dialog box modal (disables parent window)
    /// <para></para>
    /// --> NoClose() - Does not provide a cancel button or allow closing the dialog box or empty lines (default is to allow return on blank lines and cancel)
    /// <para></para>
    /// Note: All parameters are optional.  You can use GetInteger() by itself.
    /// <para></para>
    /// Note: 0 is returned of the main window is closed (use Modal() to prevent parent window close)
    /// </summary>
    /// <param name="sTitle"> = (optional) Title of the Window</param>
    /// <param name="bCancelled"> = (optional) TRUE if user canceled operation or entered blank line</param>
    /// <param name="iInteger"> = (optional) value to be filled when GetInteger() returns a bool (value is only filled if dialog box not canceled)</param>
    /// <param name="cwOptions">options such as opt::Range(), opt::Modal(), etc.</param>
    /// <returns>Integer entered or TURE/FALSE (for canceled, or number entered), depending on format.  See function prototypes.</returns>
    static int GetInteger(const char * sTitle,bool & bCancelled,const cwfOpt & cwOptions = cwfOpt());

    //-->  Original Prototype: int GetInteger(const char * sTitle = nullptr,const cwfOpt & cwOptions = cwfOpt());

    /// <summary>
    /// Gets an integer from a dialog box.
    /// <para></para>
    /// Options Supported:
    /// <para></para>
    /// --> Range() - Set range for integer
    /// <para></para>
    /// --> Modal() - Make dialog box modal (disables parent window)
    /// <para></para>
    /// --> NoClose() - Does not provide a cancel button or allow closing the dialog box or empty lines (default is to allow return on blank lines and cancel)
    /// <para></para>
    /// Note: All parameters are optional.  You can use GetInteger() by itself.
    /// <para></para>
    /// Note: 0 is returned of the main window is closed (use Modal() to prevent parent window close)
    /// </summary>
    /// <param name="sTitle"> = (optional) Title of the Window</param>
    /// <param name="bCancelled"> = (optional) TRUE if user canceled operation or entered blank line</param>
    /// <param name="iInteger"> = (optional) value to be filled when GetInteger() returns a bool (value is only filled if dialog box not canceled)</param>
    /// <param name="cwOptions">options such as opt::Range(), opt::Modal(), etc.</param>
    /// <returns>Integer entered or TURE/FALSE (for canceled, or number entered), depending on format.  See function prototypes.</returns>
    static int GetInteger(const char * sTitle = nullptr,const cwfOpt & cwOptions = cwfOpt());

    //-->  Original Prototype: bool GetInteger(const char * sTitle,int & iInteger,const cwfOpt & cwOptions = cwfOpt());

    /// <summary>
    /// Gets an integer from a dialog box.
    /// <para></para>
    /// Options Supported:
    /// <para></para>
    /// --> Range() - Set range for integer
    /// <para></para>
    /// --> Modal() - Make dialog box modal (disables parent window)
    /// <para></para>
    /// --> NoClose() - Does not provide a cancel button or allow closing the dialog box or empty lines (default is to allow return on blank lines and cancel)
    /// <para></para>
    /// Note: All parameters are optional.  You can use GetInteger() by itself.
    /// <para></para>
    /// Note: 0 is returned of the main window is closed (use Modal() to prevent parent window close)
    /// </summary>
    /// <param name="sTitle"> = (optional) Title of the Window</param>
    /// <param name="bCancelled"> = (optional) TRUE if user canceled operation or entered blank line</param>
    /// <param name="iInteger"> = (optional) value to be filled when GetInteger() returns a bool (value is only filled if dialog box not canceled)</param>
    /// <param name="cwOptions">options such as opt::Range(), opt::Modal(), etc.</param>
    /// <returns>Integer entered or TURE/FALSE (for canceled, or number entered), depending on format.  See function prototypes.</returns>
    static bool GetInteger(const char * sTitle,int & iInteger,const cwfOpt & cwOptions = cwfOpt());

    //-->  Original Prototype: bool GetInteger(int & iInteger,const cwfOpt & cwOptions = cwfOpt());

    /// <summary>
    /// Gets an integer from a dialog box.
    /// <para></para>
    /// Options Supported:
    /// <para></para>
    /// --> Range() - Set range for integer
    /// <para></para>
    /// --> Modal() - Make dialog box modal (disables parent window)
    /// <para></para>
    /// --> NoClose() - Does not provide a cancel button or allow closing the dialog box or empty lines (default is to allow return on blank lines and cancel)
    /// <para></para>
    /// Note: All parameters are optional.  You can use GetInteger() by itself.
    /// <para></para>
    /// Note: 0 is returned of the main window is closed (use Modal() to prevent parent window close)
    /// </summary>
    /// <param name="sTitle"> = (optional) Title of the Window</param>
    /// <param name="bCancelled"> = (optional) TRUE if user canceled operation or entered blank line</param>
    /// <param name="iInteger"> = (optional) value to be filled when GetInteger() returns a bool (value is only filled if dialog box not canceled)</param>
    /// <param name="cwOptions">options such as opt::Range(), opt::Modal(), etc.</param>
    /// <returns>Integer entered or TURE/FALSE (for canceled, or number entered), depending on format.  See function prototypes.</returns>
    static bool GetInteger(int & iInteger,const cwfOpt & cwOptions = cwfOpt());

    //-->  Original Prototype: int GetInteger(const cwfOpt & cwOptions);

    /// <summary>
    /// Gets an integer from a dialog box.
    /// <para></para>
    /// Options Supported:
    /// <para></para>
    /// --> Range() - Set range for integer
    /// <para></para>
    /// --> Modal() - Make dialog box modal (disables parent window)
    /// <para></para>
    /// --> NoClose() - Does not provide a cancel button or allow closing the dialog box or empty lines (default is to allow return on blank lines and cancel)
    /// <para></para>
    /// Note: All parameters are optional.  You can use GetInteger() by itself.
    /// <para></para>
    /// Note: 0 is returned of the main window is closed (use Modal() to prevent parent window close)
    /// </summary>
    /// <param name="sTitle"> = (optional) Title of the Window</param>
    /// <param name="bCancelled"> = (optional) TRUE if user canceled operation or entered blank line</param>
    /// <param name="iInteger"> = (optional) value to be filled when GetInteger() returns a bool (value is only filled if dialog box not canceled)</param>
    /// <param name="cwOptions">options such as opt::Range(), opt::Modal(), etc.</param>
    /// <returns>Integer entered or TURE/FALSE (for canceled, or number entered), depending on format.  See function prototypes.</returns>
    static int GetInteger(const cwfOpt & cwOptions);

    //-->  Original Prototype: double GetFloat(const char * sTitle,bool & bCancelled,const cwfOpt & cwOptions = cwfOpt());

    /// <summary>
    /// Gets a floating point value from a dialog box.
    /// <para></para>
    /// Options Supported:
    /// <para></para>
    /// --> Range() - Set range for floating point value
    /// <para></para>
    /// --> Modal() - Make dialog box modal (disables parent window)
    /// <para></para>
    /// --> NoClose() - Does not provide a cancel button or allow closing the dialog box or empty lines (default is to allow return on blank lines and cancel)
    /// <para></para>
    /// Note: All parameters are optional.  You can use GetFloat() by itself.
    /// <para></para>
    /// Note: 0 is returned of the main window is closed (use Modal() to prevent parent window close)
    /// </summary>
    /// <param name="sTitle"> = (optional) Title of the Window</param>
    /// <param name="bCancelled"> = (optional) TRUE if user canceled operation or entered blank line</param>
    /// <param name="fFloat"> = (optional) value to be filled when GetFloat() returns a bool (value is only filled if dialog box not canceled)</param>
    /// <param name="cwOptions">options such as opt::Range(), opt::Modal(), etc.</param>
    /// <returns>Integer entered or TURE/FALSE (for canceled, or number entered), depending on format.  See function prototypes.</returns>
    static double GetFloat(const char * sTitle,bool & bCancelled,const cwfOpt & cwOptions = cwfOpt());

    //-->  Original Prototype: double GetFloat(const char * sTitle = nullptr,const cwfOpt & cwOptions = cwfOpt());

    /// <summary>
    /// Gets a floating point value from a dialog box.
    /// <para></para>
    /// Options Supported:
    /// <para></para>
    /// --> Range() - Set range for floating point value
    /// <para></para>
    /// --> Modal() - Make dialog box modal (disables parent window)
    /// <para></para>
    /// --> NoClose() - Does not provide a cancel button or allow closing the dialog box or empty lines (default is to allow return on blank lines and cancel)
    /// <para></para>
    /// Note: All parameters are optional.  You can use GetFloat() by itself.
    /// <para></para>
    /// Note: 0 is returned of the main window is closed (use Modal() to prevent parent window close)
    /// </summary>
    /// <param name="sTitle"> = (optional) Title of the Window</param>
    /// <param name="bCancelled"> = (optional) TRUE if user canceled operation or entered blank line</param>
    /// <param name="fFloat"> = (optional) value to be filled when GetFloat() returns a bool (value is only filled if dialog box not canceled)</param>
    /// <param name="cwOptions">options such as opt::Range(), opt::Modal(), etc.</param>
    /// <returns>Integer entered or TURE/FALSE (for canceled, or number entered), depending on format.  See function prototypes.</returns>
    static double GetFloat(const char * sTitle = nullptr,const cwfOpt & cwOptions = cwfOpt());

    //-->  Original Prototype: bool GetFloat(const char * sTitle,double & fFloat,const cwfOpt & cwOptions = cwfOpt());

    /// <summary>
    /// Gets a floating point value from a dialog box.
    /// <para></para>
    /// Options Supported:
    /// <para></para>
    /// --> Range() - Set range for floating point value
    /// <para></para>
    /// --> Modal() - Make dialog box modal (disables parent window)
    /// <para></para>
    /// --> NoClose() - Does not provide a cancel button or allow closing the dialog box or empty lines (default is to allow return on blank lines and cancel)
    /// <para></para>
    /// Note: All parameters are optional.  You can use GetFloat() by itself.
    /// <para></para>
    /// Note: 0 is returned of the main window is closed (use Modal() to prevent parent window close)
    /// </summary>
    /// <param name="sTitle"> = (optional) Title of the Window</param>
    /// <param name="bCancelled"> = (optional) TRUE if user canceled operation or entered blank line</param>
    /// <param name="fFloat"> = (optional) value to be filled when GetFloat() returns a bool (value is only filled if dialog box not canceled)</param>
    /// <param name="cwOptions">options such as opt::Range(), opt::Modal(), etc.</param>
    /// <returns>Integer entered or TURE/FALSE (for canceled, or number entered), depending on format.  See function prototypes.</returns>
    static bool GetFloat(const char * sTitle,double & fFloat,const cwfOpt & cwOptions = cwfOpt());

    //-->  Original Prototype: bool GetFloat(double & fFloat,const cwfOpt & cwOptions = cwfOpt());

    /// <summary>
    /// Gets a floating point value from a dialog box.
    /// <para></para>
    /// Options Supported:
    /// <para></para>
    /// --> Range() - Set range for floating point value
    /// <para></para>
    /// --> Modal() - Make dialog box modal (disables parent window)
    /// <para></para>
    /// --> NoClose() - Does not provide a cancel button or allow closing the dialog box or empty lines (default is to allow return on blank lines and cancel)
    /// <para></para>
    /// Note: All parameters are optional.  You can use GetFloat() by itself.
    /// <para></para>
    /// Note: 0 is returned of the main window is closed (use Modal() to prevent parent window close)
    /// </summary>
    /// <param name="sTitle"> = (optional) Title of the Window</param>
    /// <param name="bCancelled"> = (optional) TRUE if user canceled operation or entered blank line</param>
    /// <param name="fFloat"> = (optional) value to be filled when GetFloat() returns a bool (value is only filled if dialog box not canceled)</param>
    /// <param name="cwOptions">options such as opt::Range(), opt::Modal(), etc.</param>
    /// <returns>Integer entered or TURE/FALSE (for canceled, or number entered), depending on format.  See function prototypes.</returns>
    static bool GetFloat(double & fFloat,const cwfOpt & cwOptions = cwfOpt());

    //-->  Original Prototype: double GetFloat(const cwfOpt & cwOptions);

     /// <summary>
    /// Gets a floating point value from a dialog box.
    /// <para></para>
    /// Options Supported:
    /// <para></para>
    /// --> Range() - Set range for floating point value
    /// <para></para>
    /// --> Modal() - Make dialog box modal (disables parent window)
    /// <para></para>
    /// --> NoClose() - Does not provide a cancel button or allow closing the dialog box or empty lines (default is to allow return on blank lines and cancel)
    /// <para></para>
    /// Note: All parameters are optional.  You can use GetFloat() by itself.
    /// <para></para>
    /// Note: 0 is returned of the main window is closed (use Modal() to prevent parent window close)
    /// </summary>
    /// <param name="sTitle"> = (optional) Title of the Window</param>
    /// <param name="bCancelled"> = (optional) TRUE if user canceled operation or entered blank line</param>
    /// <param name="fFloat"> = (optional) value to be filled when GetFloat() returns a bool (value is only filled if dialog box not canceled)</param>
    /// <param name="cwOptions">options such as opt::Range(), opt::Modal(), etc.</param>
    /// <returns>Integer entered or TURE/FALSE (for canceled, or number entered), depending on format.  See function prototypes.</returns>
    static double GetFloat(const cwfOpt & cwOptions);

    //-->  Original Prototype: void InfoWindow(const char * sTitle,const cwfOpt & cwOptions = cwfOpt());

    // Documentation TBD
    static void InfoWindow(const char * sTitle,const cwfOpt & cwOptions = cwfOpt());

    //-->  Original Prototype: bool YesNoWindow(const char * sTitle,const cwfOpt & cwOptions = cwfOpt());

    // Documentation TBD
    static bool YesNoWindow(const char * sTitle,const cwfOpt & cwOptions = cwfOpt());

    //-->  Original Prototype: Sage::DialogResult YesNoCancelWindow(const char * sTitle,const cwfOpt & cwOptions = cwfOpt());

    // Documentation TBD
    static Sage::DialogResult YesNoCancelWindow(const char * sTitle,const cwfOpt & cwOptions = cwfOpt());

    //-->  Original Prototype: bool OkCancelWindow(const char * sTitle,const cwfOpt & cwOptions = cwfOpt());

    // Documentation TBD
    static bool OkCancelWindow(const char * sTitle,const cwfOpt & cwOptions = cwfOpt());

    //-->  Original Prototype: void Write(const char * sText,const char * sOptions = nullptr);

    // Write() -- Write text to the window in a simplified manner. 
    // 
    // Write() writes simple text to the window.  This is faster than printf() and other methods and can be used
    // to create faster output to the window.  
    //
    // Text can have the same SageBox-style color encoding and other {} attributes, such as 
    //            This is cWindow->Write("This is {green}color{/} text and this is another {red}color{/}"); 
    // 
    // Text can also have newline and other characters, such as cWindow->Write("This is line 1\nand this is line 2");
    //
    // The window will automatically scroll if scrolling is turned on.  The window will also scroll if it is designated as a text window (TBD)
    //
    static void Write(const char * sText,const char * sOptions = nullptr);

    //-->  Original Prototype: void Write(const char * sText,const cwfOpt & cwOptions);

    // Write() -- Write text to the window in a simplified manner. 
    // 
    // Write() writes simple text to the window.  This is faster than printf() and other methods and can be used
    // to create faster output to the window.  
    //
    // Text can have the same SageBox-style color encoding and other {} attributes, such as 
    //            This is cWindow->Write("This is {green}color{/} text and this is another {red}color{/}"); 
    // 
    // Text can also have newline and other characters, such as cWindow->Write("This is line 1\nand this is line 2");
    //
    // The window will automatically scroll if scrolling is turned on.  The window will also scroll if it is designated as a text window (TBD)
    //
    static void Write(const char * sText,const cwfOpt & cwOptions);

    //-->  Original Prototype: void Write(int iX,int iY,const char * sText,const cwfOpt & cwOptions);

    // Write() -- Write text to the window in a simplified manner. 
    // 
    // Write() writes simple text to the window.  This is faster than printf() and other methods and can be used
    // to create faster output to the window.  
    //
    // Text can have the same SageBox-style color encoding and other {} attributes, such as 
    //            This is cWindow->Write("This is {green}color{/} text and this is another {red}color{/}"); 
    // 
    // Text can also have newline and other characters, such as cWindow->Write("This is line 1\nand this is line 2");
    //
    // The window will automatically scroll if scrolling is turned on.  The window will also scroll if it is designated as a text window (TBD)
    //
    static void Write(int iX,int iY,const char * sText,const cwfOpt & cwOptions);

    //-->  Original Prototype: void Write(POINT pLoc,const char * sText,const cwfOpt & cwOptions = cwfOpt());

    // Write() -- Write text to the window in a simplified manner. 
    // 
    // Write() writes simple text to the window.  This is faster than printf() and other methods and can be used
    // to create faster output to the window.  
    //
    // Text can have the same SageBox-style color encoding and other {} attributes, such as 
    //            This is cWindow->Write("This is {green}color{/} text and this is another {red}color{/}"); 
    // 
    // Text can also have newline and other characters, such as cWindow->Write("This is line 1\nand this is line 2");
    //
    // The window will automatically scroll if scrolling is turned on.  The window will also scroll if it is designated as a text window (TBD)
    //
    static void Write(POINT pLoc,const char * sText,const cwfOpt & cwOptions = cwfOpt());

    //-->  Original Prototype: void Writeln(const char * sText = nullptr,const char * sOptions = nullptr);

    // Writeln() -- Same as Write() but adds a '\n' at the end of the line for convenience
    static void Writeln(const char * sText = nullptr,const char * sOptions = nullptr);

    //-->  Original Prototype: void Writeln(const char * sText,const cwfOpt & cwOptions);

    // Writeln() -- Same as Write() but adds a '\n' at the end of the line for convenience
    static void Writeln(const char * sText,const cwfOpt & cwOptions);

    //-->  Original Prototype: void Writeln(const cwfOpt & cwOptions);

    // Writeln() -- Same as Write() but adds a '\n' at the end of the line for convenience
    static void Writeln(const cwfOpt & cwOptions);

    //-->  Original Prototype: void Writeln(int iX,int iY,const char * sText,const cwfOpt & cwOptions);

    // Writeln() -- Same as Write() but adds a '\n' at the end of the line for convenience
    static void Writeln(int iX,int iY,const char * sText,const cwfOpt & cwOptions);

    //-->  Original Prototype: void Writeln(POINT pLoc,const char * sText,const cwfOpt & cwOptions = cwfOpt());

    static void Writeln(POINT pLoc,const char * sText,const cwfOpt & cwOptions = cwfOpt());

    //-->  Original Prototype: void Writeln(int iX,int iY,const cwfOpt & cwOptions);

    // Writeln() -- Same as Write() but adds a '\n' at the end of the line for convenience
    static void Writeln(int iX,int iY,const cwfOpt & cwOptions);

    //-->  Original Prototype: int putchar(char cChar);

    // Put a character on the window just as putchar() in regular 'C'.
    // This is useful for Ansii-text and emulation
    // note: This can be useful for terminal emulation when a non-proportional font (i.e. a terminal font)
    //       is used, i.e. "Courier New,14", etc.
    //
    static int putchar(char cChar);

    //-->  Original Prototype: int __putchar(char cChar);

    // Put a character on the window just as putchar() in regular 'C'.
    // This is useful for Ansii-text and emulation
    // note: This can be useful for terminal emulation when a non-proportional font (i.e. a terminal font)
    //       is used, i.e. "Courier New,14", etc.
    //
    static int __putchar(char cChar);

    //-->  Original Prototype: bool SetTabPos(int iPos);

    // SetTabPos() -- Set the X position for Write/out/putchar/etc output to the character nth character position.
    // For example. SetTabPos(40) puts the output position (i.e. cursor) at charatcer position 40, based on the average character width.
    // This can be used to align console input boxes, text, etc. 
    // Since most fonts are proportionally spaced, the average character width is used.  In the case of terminal fonts, the character width is
    // the same for all characters and is more predictable
    //
    // This is included as a function from Basic() that is used in a number of programs
    //
    static bool SetTabPos(int iPos);

    //-->  Original Prototype: bool ClipWindow(int iX,int iY,int iWidth,int iHeight,bool bAllowScroll = false);

    // ClipWindow() -- Clips the window in the given rectangle, restricting output and drawing to that region.
    // This can be used to restrict write areas, Cls(), Drawing, etc. 
    // For example, a graph can be drawn with the clip and will not exceed the clip area.  This can be very useful and 
    // allow such drawing and output in place of creating a child window to perform the same task.
    // Use  ResetClipWindow() or ClipWindow() (with no paramaters) to remove the clipping region
    //
    // bAllowScroll allows the clip region to scroll based on writing text to the screen with '\n' newlines.
    // Default is to not allow the clipping region to scroll.
    //
    static bool ClipWindow(int iX,int iY,int iWidth,int iHeight,bool bAllowScroll = false);

    //-->  Original Prototype: bool ClipWindow(POINT pPoint,SIZE szSize,bool bAllowsScroll = false);

    // ClipWindow() -- Clips the window in the given rectangle, restricting output and drawing to that region.
    // This can be used to restrict write areas, Cls(), Drawing, etc. 
    // For example, a graph can be drawn with the clip and will not exceed the clip area.  This can be very useful and 
    // allow such drawing and output in place of creating a child window to perform the same task.
    // Use  ResetClipWindow() or ClipWindow() (with no paramaters) to remove the clipping region
    //
    // bAllowScroll allows the clip region to scroll based on writing text to the screen with '\n' newlines.
    // Default is to not allow the clipping region to scroll.
    //
    static bool ClipWindow(POINT pPoint,SIZE szSize,bool bAllowsScroll = false);

    //-->  Original Prototype: bool ClipWindow();

    // ClipWindow() -- Clips the window in the given rectangle, restricting output and drawing to that region.
    // This can be used to restrict write areas, Cls(), Drawing, etc. 
    // For example, a graph can be drawn with the clip and will not exceed the clip area.  This can be very useful and 
    // allow such drawing and output in place of creating a child window to perform the same task.
    // Use  ResetClipWindow() or ClipWindow() (with no paramaters) to remove the clipping region
    //
    static bool ClipWindow();

    //-->  Original Prototype: bool ResetClipWindow();

    // ResetClipWindow() -- Resets any clipping region active for the current window.
    //
    static bool ResetClipWindow();

    //-->  Original Prototype: bool SetAutoScroll(WindowScroll scroll);

    // SetAutoScroll() -- By default, the window will autoscroll when a '\n' is received and the current font will exceed the current output position.
    // Sometimes this is not desired behavior and can be shut off with SetAutoScroll()
    //
    // Conversely, sometimes the window should scroll and does not when Auto Scroll is turned off.
    // SetAutoScroll will turn the scrolling on. 
    //
    // Modes:
    // Disabled       -- Do not scroll the window
    // Enabled        -- Scroll the window when needed
    // Auto           -- Expirmental and probably doesn't work at the moment.
    //
    static bool SetAutoScroll(WindowScroll scroll);

    //-->  Original Prototype: WindowScroll GetAutoScroll();

    // GetAutoScroll() -- Get the current scroll status
    // This can be used when the window doesn't seem to be scrolling to determine if the AutoScroll setting may be the issue.
    //
    static WindowScroll GetAutoScroll();

    //-->  Original Prototype: void Cls(DWORD iColor1=-1,DWORD iColor2 = -1);

    // Cls() -- Clear the window with a specified color or with a color gradient
    //
    // Once the window is cleared, the window's background color is changed to the new color.
    // In the case of a gradient, the transparency should be ON, causing the background color to not be used for printing text.
    //
    // note: When the background is tranparent, this takes longer to write text to the screen, which can cause slowness for programs that write out to
    // to the screen alot.  Also see Text Windows (TBD)
    //
    // Clear the screen with one color: cWindow->Cls(RGB(255,0,0));                    // Clears the screen to red.
    // Clear the screen with a Gradient cWindow->Cls(RGB(255,0,0),RGB(0,0,255));    // Clear the screen in a gradient from red to blue.
    //
    // Realistically, the gradient is used to set more of a tone, such as:
    //
    //        DWORD dwDarkGray = RGB(32,32,32);
    //        DWORD dwGray = RGB(92,92,92);
    //
    //        cWindow->Cls(dwDarkGray,dwGray); 
    //
    // This clears the window from Dark Gray to a lighter Gray, which can look nicer than a flat color.
    //
    // note: When Davinci is used as a DLL, MSIMG32.DLL must be located in the system, otherwise the first color us used as a simple flat-color Cls();
    //
    static void Cls(DWORD iColor1 = -1,DWORD iColor2 = -1);

    //-->  Original Prototype: void Cls(RGBColor_t rgbColor);

    // Cls() -- Clear the window with a specified color or with a color gradient
    //
    // Once the window is cleared, the window's background color is changed to the new color.
    // In the case of a gradient, the transparency should be ON, causing the background color to not be used for printing text.
    //
    // note: When the background is tranparent, this takes longer to write text to the screen, which can cause slowness for programs that write out to
    // to the screen alot.  Also see Text Windows (TBD)
    //
    // Clear the screen with one color: cWindow->Cls(RGB(255,0,0));                    // Clears the screen to red.
    // Clear the screen with a Gradient cWindow->Cls(RGB(255,0,0),RGB(0,0,255));    // Clear the screen in a gradient from red to blue.
    //
    // Realistically, the gradient is used to set more of a tone, such as:
    //
    //        DWORD dwDarkGray = RGB(32,32,32);
    //        DWORD dwGray = RGB(92,92,92);
    //
    //        cWindow->Cls(dwDarkGray,dwGray); 
    //
    // This clears the window from Dark Gray to a lighter Gray, which can look nicer than a flat color.
    //
    // note: When Davinci is used as a DLL, MSIMG32.DLL must be located in the system, otherwise the first color us used as a simple flat-color Cls();
    //
    static void Cls(RGBColor_t rgbColor);

    //-->  Original Prototype: void Cls(const char * sColor1,const char * sColor2=nullptr);

    // Cls() -- Clear the window with a specified color or with a color gradient
    //
    // Once the window is cleared, the window's background color is changed to the new color.
    // In the case of a gradient, the transparency should be ON, causing the background color to not be used for printing text.
    //
    // note: When the background is tranparent, this takes longer to write text to the screen, which can cause slowness for programs that write out to
    // to the screen alot.  Also see Text Windows (TBD)
    //
    // Clear the screen with one color: cWindow->Cls(RGB(255,0,0));                    // Clears the screen to red.
    // Clear the screen with a Gradient cWindow->Cls(RGB(255,0,0),RGB(0,0,255));    // Clear the screen in a gradient from red to blue.
    //
    // Realistically, the gradient is used to set more of a tone, such as:
    //
    //        DWORD dwDarkGray = RGB(32,32,32);
    //        DWORD dwGray = RGB(92,92,92);
    //
    //        cWindow->Cls(dwDarkGray,dwGray); 
    //
    // This clears the window from Dark Gray to a lighter Gray, which can look nicer than a flat color.
    //
    // note: When Davinci is used as a DLL, MSIMG32.DLL must be located in the system, otherwise the first color us used as a simple flat-color Cls();
    //
    static void Cls(const char * sColor1,const char * sColor2 = nullptr);

    //-->  Original Prototype: void Cls(const char * sColor1,RGBColor_t rgbColor);

    // Cls() -- Clear the window with a specified color or with a color gradient
    //
    // Once the window is cleared, the window's background color is changed to the new color.
    // In the case of a gradient, the transparency should be ON, causing the background color to not be used for printing text.
    //
    // note: When the background is tranparent, this takes longer to write text to the screen, which can cause slowness for programs that write out to
    // to the screen alot.  Also see Text Windows (TBD)
    //
    // Clear the screen with one color: cWindow->Cls(RGB(255,0,0));                    // Clears the screen to red.
    // Clear the screen with a Gradient cWindow->Cls(RGB(255,0,0),RGB(0,0,255));    // Clear the screen in a gradient from red to blue.
    //
    // Realistically, the gradient is used to set more of a tone, such as:
    //
    //        DWORD dwDarkGray = RGB(32,32,32);
    //        DWORD dwGray = RGB(92,92,92);
    //
    //        cWindow->Cls(dwDarkGray,dwGray); 
    //
    // This clears the window from Dark Gray to a lighter Gray, which can look nicer than a flat color.
    //
    // note: When Davinci is used as a DLL, MSIMG32.DLL must be located in the system, otherwise the first color us used as a simple flat-color Cls();
    //
    static void Cls(const char * sColor1,RGBColor_t rgbColor);

    //-->  Original Prototype: void Cls(RGBColor_t rgbColor,const char * sColor2);

    // Cls() -- Clear the window with a specified color or with a color gradient
    //
    // Once the window is cleared, the window's background color is changed to the new color.
    // In the case of a gradient, the transparency should be ON, causing the background color to not be used for printing text.
    //
    // note: When the background is tranparent, this takes longer to write text to the screen, which can cause slowness for programs that write out to
    // to the screen alot.  Also see Text Windows (TBD)
    //
    // Clear the screen with one color: cWindow->Cls(RGB(255,0,0));                    // Clears the screen to red.
    // Clear the screen with a Gradient cWindow->Cls(RGB(255,0,0),RGB(0,0,255));    // Clear the screen in a gradient from red to blue.
    //
    // Realistically, the gradient is used to set more of a tone, such as:
    //
    //        DWORD dwDarkGray = RGB(32,32,32);
    //        DWORD dwGray = RGB(92,92,92);
    //
    //        cWindow->Cls(dwDarkGray,dwGray); 
    //
    // This clears the window from Dark Gray to a lighter Gray, which can look nicer than a flat color.
    //
    // note: When Davinci is used as a DLL, MSIMG32.DLL must be located in the system, otherwise the first color us used as a simple flat-color Cls();
    //
    static void Cls(RGBColor_t rgbColor,const char * sColor2);

    //-->  Original Prototype: bool Cls(CBitmap & cBitmap);

    // Cls() -- Clear the window with a specified color or with a color gradient
    //
    // Once the window is cleared, the window's background color is changed to the new color.
    // In the case of a gradient, the transparency should be ON, causing the background color to not be used for printing text.
    //
    // note: When the background is tranparent, this takes longer to write text to the screen, which can cause slowness for programs that write out to
    // to the screen alot.  Also see Text Windows (TBD)
    //
    // Clear the screen with one color: cWindow->Cls(RGB(255,0,0));                    // Clears the screen to red.
    // Clear the screen with a Gradient cWindow->Cls(RGB(255,0,0),RGB(0,0,255));    // Clear the screen in a gradient from red to blue.
    //
    // Realistically, the gradient is used to set more of a tone, such as:
    //
    //        DWORD dwDarkGray = RGB(32,32,32);
    //        DWORD dwGray = RGB(92,92,92);
    //
    //        cWindow->Cls(dwDarkGray,dwGray); 
    //
    // This clears the window from Dark Gray to a lighter Gray, which can look nicer than a flat color.
    //
    // note: When Davinci is used as a DLL, MSIMG32.DLL must be located in the system, otherwise the first color us used as a simple flat-color Cls();
    //
    static bool Cls(CBitmap & cBitmap);

    //-->  Original Prototype: void Cls(RGBColor_t rgbColor,RGBColor_t rgbColor2);

    // Cls() -- Clear the window with a specified color or with a color gradient
    //
    // Once the window is cleared, the window's background color is changed to the new color.
    // In the case of a gradient, the transparency should be ON, causing the background color to not be used for printing text.
    //
    // note: When the background is tranparent, this takes longer to write text to the screen, which can cause slowness for programs that write out to
    // to the screen alot.  Also see Text Windows (TBD)
    //
    // Clear the screen with one color: cWindow->Cls(RGB(255,0,0));                    // Clears the screen to red.
    // Clear the screen with a Gradient cWindow->Cls(RGB(255,0,0),RGB(0,0,255));    // Clear the screen in a gradient from red to blue.
    //
    // Realistically, the gradient is used to set more of a tone, such as:
    //
    //        DWORD dwDarkGray = RGB(32,32,32);
    //        DWORD dwGray = RGB(92,92,92);
    //
    //        cWindow->Cls(dwDarkGray,dwGray); 
    //
    // This clears the window from Dark Gray to a lighter Gray, which can look nicer than a flat color.
    //
    // note: When Davinci is used as a DLL, MSIMG32.DLL must be located in the system, otherwise the first color us used as a simple flat-color Cls();
    //
    static void Cls(RGBColor_t rgbColor,RGBColor_t rgbColor2);

    //-->  Original Prototype: void ClsCanvas(int iColor1=-1,int iColor2 = -1);

    // ClsCanvas() -- Same as Cls() but clears the entire Windows Canvas rather then the display area.
    // Cls() clears the entire displayable part of the window. However, the Window may be much larger and the visible portion only
    // part of it.
    //
    // For a size-enabled window, ClsCanvas() will clear the entire canvas of the window.
    // Using Cls() and then enlarging the window with the mouse will show the rectangle where the Cls()
    // cleared only the visible portion.
    //
    // ClsCanvas() has the same parameters as Cls()
    // See Cls() for usage information
    //
    static void ClsCanvas(int iColor1 = -1,int iColor2 = -1);

    //-->  Original Prototype: void ClsCanvas(RGBColor_t rgbColor);

    // ClsCanvas() -- Same as Cls() but clears the entire Windows Canvas rather then the display area.
    // Cls() clears the entire displayable part of the window. However, the Window may be much larger and the visible portion only
    // part of it.
    //
    // For a size-enabled window, ClsCanvas() will clear the entire canvas of the window.
    // Using Cls() and then enlarging the window with the mouse will show the rectangle where the Cls()
    // cleared only the visible portion.
    //
    // ClsCanvas() has the same parameters as Cls()
    // See Cls() for usage information
    //
    static void ClsCanvas(RGBColor_t rgbColor);

    //-->  Original Prototype: void ClsCanvas(const char * sColor1,const char * sColor2=nullptr);

    // ClsCanvas() -- Same as Cls() but clears the entire Windows Canvas rather then the display area.
    // Cls() clears the entire displayable part of the window. However, the Window may be much larger and the visible portion only
    // part of it.
    //
    // For a size-enabled window, ClsCanvas() will clear the entire canvas of the window.
    // Using Cls() and then enlarging the window with the mouse will show the rectangle where the Cls()
    // cleared only the visible portion.
    //
    // ClsCanvas() has the same parameters as Cls()
    // See Cls() for usage information
    //
    static void ClsCanvas(const char * sColor1,const char * sColor2 = nullptr);

    //-->  Original Prototype: void ClsCanvas(const char * sColor1,RGBColor_t rgbColor);

    // ClsCanvas() -- Same as Cls() but clears the entire Windows Canvas rather then the display area.
    // Cls() clears the entire displayable part of the window. However, the Window may be much larger and the visible portion only
    // part of it.
    //
    // For a size-enabled window, ClsCanvas() will clear the entire canvas of the window.
    // Using Cls() and then enlarging the window with the mouse will show the rectangle where the Cls()
    // cleared only the visible portion.
    //
    // ClsCanvas() has the same parameters as Cls()
    // See Cls() for usage information
    //
    static void ClsCanvas(const char * sColor1,RGBColor_t rgbColor);

    //-->  Original Prototype: void ClsCanvas(RGBColor_t rgbColor,const char * sColor2);

    // ClsCanvas() -- Same as Cls() but clears the entire Windows Canvas rather then the display area.
    // Cls() clears the entire displayable part of the window. However, the Window may be much larger and the visible portion only
    // part of it.
    //
    // For a size-enabled window, ClsCanvas() will clear the entire canvas of the window.
    // Using Cls() and then enlarging the window with the mouse will show the rectangle where the Cls()
    // cleared only the visible portion.
    //
    // ClsCanvas() has the same parameters as Cls()
    // See Cls() for usage information
    //
    static void ClsCanvas(RGBColor_t rgbColor,const char * sColor2);

    //-->  Original Prototype: void ClsCanvas(RGBColor_t rgbColor,RGBColor_t rgbColor2);

    // ClsCanvas() -- Same as Cls() but clears the entire Windows Canvas rather then the display area.
    // Cls() clears the entire displayable part of the window. However, the Window may be much larger and the visible portion only
    // part of it.
    //
    // For a size-enabled window, ClsCanvas() will clear the entire canvas of the window.
    // Using Cls() and then enlarging the window with the mouse will show the rectangle where the Cls()
    // cleared only the visible portion.
    //
    // ClsCanvas() has the same parameters as Cls()
    // See Cls() for usage information
    //
    static void ClsCanvas(RGBColor_t rgbColor,RGBColor_t rgbColor2);

    //-->  Original Prototype: void Update(int iUpdateMS=0);

    // Update() -- Update the window, showing any new changes
    //
    // Update() repaints the window with any changes made to the window since the last update or Windows PAINT (activated automatically for various reasons, such as moving the window, or 
    // changing to another application and then back to the window). 
    //
    // Update() is used when the window is buffered (which is the default behavior), which outputs to a bitmap rather than the window iself.
    // This is faster, since many output functions can be performed before the window is updated.  Otherwise, with a buffered window, updating the bitmap to the window
    // for every write() or other function will be slow.
    //
    // When the window is not buffered, Update() is not required, and the window is painted directly.  However, this means the application must have a WM_PAINT callback or
    // override of the Paint() function in CWindow(), as it is now the application's responsibility to know what to paint on every call. 
    //
    static void Update(int iUpdateMS = 0);

    //-->  Original Prototype: bool UpdateReady(int iUpdateMS = 10);

    // UpdateReady() -- Returns true of <iUpdateMS> has occurred since the last update.
    //
    // UpdateReady() is a powerful function leading to very fast output.  When Auto Updates are off, Update(<iUpdateMS>) can be used to update the screen if it is needed
    // every <iUpdateMS>
    //
    // However, with Update(), this means the output must still be drawn. 
    //
    // With UpdateReady() screen drawing can be skipped until there is a reason to do it.  With Update(iUpdateMS), it only skips the update but the screen must still be drawn.
    // Use if (UpdateReady(iUpdateMS)) DrawItems() to only draw when an update is required.
    // Use UpdateReady() to use the default 10ms -- which has been determined to be the best value overall, as most updates 
    // will occur between 10-20ms, allowing 50-100 frames per second, but probably on the lower side.
    //
    static bool UpdateReady(int iUpdateMS = 10);

    //-->  Original Prototype: bool SetWindowDrag(bool bGrab = true);

    // SetWindowDraw() -- When TRUE, this allows the window or control (either in a popup or embedded in a window) to be moved around by dragging the control
    //
    // For popup windows (such as dialogs), this allows the window to be moved by grabbing it anywhere with the mouse.
    // For example, all dialog boxes (Info(), GetFloat(), etc) turn this on so that the window may be used easily.
    // For BitmapWindows (popup or embedded), TextWidgets, and other controls, this can allow the controls to be moved around the screen or
    // parent window by grabbing the control or image.
    //
    // In some cases, this may require a right-click or control-key simultaneously -- this is up to the control, and usually occurs in controls where normal mouse-clicking is
    // part of the control, so an alternate method is required.
    //
    static bool SetWindowDrag(bool bGrab = true);

    //-->  Original Prototype: bool SetWindowSize(SIZE szSize,bool bInnerSize = false);

    // SetWindowSize() -- Set the size of the window.
    // 
    // This sets the entire size of the window, both the visible portion and the canvas size.
    // See SetCanvasSize() to set a larger canvas than the displayed window size.
    //
    // bInnerSize -- when true, this calculates the window size based on the inner canvas size.
    // When false, this calculates the window (when it is a popup or has a frame) to be the size of the entire window, 
    // making the canvas size smaller.
    //
    static bool SetWindowSize(SIZE szSize,bool bInnerSize = false);

    //-->  Original Prototype: bool SetWindowSize(int iWidth,int iHeight,bool bInnerSize = false);

    // SetWindowSize() -- Set the size of the window.
    // 
    // This sets the entire size of the window, both the visible portion and the canvas size.
    // See SetCanvasSize() to set a larger canvas than the displayed window size.
    //
    // bInnerSize -- when true, this calculates the window size based on the inner canvas size.
    // When false, this calculates the window (when it is a popup or has a frame) to be the size of the entire window, 
    // making the canvas size smaller.
    //
    static bool SetWindowSize(int iWidth,int iHeight,bool bInnerSize = false);

    //-->  Original Prototype: SIZE GetCanvasSize();

    // GetCanvasSize() -- Get the size of the Window Canvas. 
    // 
    // The Window Canvas is the bitmap that is shown in the window and may be larger than the window (but may never be smaller). 
    // This allows the Window to be resized dynamically, or to use a larger bitmap and move it around in the window, such as for scrolling.
    //
    static SIZE GetCanvasSize();

    //-->  Original Prototype: bool SetCanvasSize(SIZE szMaxSize,bool bAllowResizing = true);

    // Set CanvasSize() -- Set the Canvas size of the window
    // This value must be greater than the displayed window canvas or it is ignored.
    //
    // This sets the Canvas size of the window (i.e. the drawable space), which can exceed the size of the window.
    //
    static bool SetCanvasSize(SIZE szMaxSize,bool bAllowResizing = true);

    //-->  Original Prototype: bool SetCanvasSize(SIZE szMinSize,SIZE szSize,bool bAllowResizing = true);

    static bool SetCanvasSize(SIZE szMinSize,SIZE szSize,bool bAllowResizing = true);

    //-->  Original Prototype: bool SetCanvasSize(int iWidth,int iHeight,bool bAllowResizing = true);

    // Set CanvasSize() -- Set the Canvas size of the window
    // This value must be greater than the displayed window canvas or it is ignored.
    //
    // This sets the Canvas size of the window (i.e. the drawable space), which can exceed the size of the window.
    //
    static bool SetCanvasSize(int iWidth,int iHeight,bool bAllowResizing = true);

    //-->  Original Prototype: bool AllowResizing(bool bAllowResizing = true);

    static bool AllowResizing(bool bAllowResizing = true);

    //-->  Original Prototype: bool GetClientSize(SIZE & Size);

    // GetClientSize() -- Get the current client area size (i.e. writeable/drawable part of the window)
    // This value may be larger than the visible window.
    //
    // GetWindowSize(), for example, returns the visible canvas area.
    //
    static bool GetClientSize(SIZE & Size);

    //-->  Original Prototype: bool UpdateRegion(RECT & rRegion,int iUpdateMS = 0);

    // UpdateRegion() - Update a region of the window
    //
    // UpdateRegion() works the same way Update() works, except it only updates the given region.
    // This can be much faster than Update(), since update updates the entire Window.
    //
    // Use iUpdateMS to tell SageBox to only update the region every <iUpdateMS> millisecond, which can make it much faster
    //
    // A Bitmap may also be given to autmatically determine the region based on the bitmap size
    //
    // Note: When using iUpdateMS() a last Update() or UpdateRegion() without iUpdateMS will be required to ensure the last known 
    // draw of that region is updated to the screen
    //
    static bool UpdateRegion(RECT & rRegion,int iUpdateMS = 0);

    //-->  Original Prototype: bool UpdateRegion(int iX,int iY,int iWidth,int iHeight,int iUpdateMS = 0);

    // UpdateRegion() - Update a region of the window
    //
    // UpdateRegion() works the same way Update() works, except it only updates the given region.
    // This can be much faster than Update(), since update updates the entire Window.
    //
    // Use iUpdateMS to tell SageBox to only update the region every <iUpdateMS> millisecond, which can make it much faster
    //
    // A Bitmap may also be given to autmatically determine the region based on the bitmap size
    //
    // Note: When using iUpdateMS() a last Update() or UpdateRegion() without iUpdateMS will be required to ensure the last known 
    // draw of that region is updated to the screen
    //
    static bool UpdateRegion(int iX,int iY,int iWidth,int iHeight,int iUpdateMS = 0);

    //-->  Original Prototype: bool UpdateRegion(int iX,int iY,RawBitmap_t stBitmap,int iUpdateMS = 0);

    // UpdateRegion() - Update a region of the window
    //
    // UpdateRegion() works the same way Update() works, except it only updates the given region.
    // This can be much faster than Update(), since update updates the entire Window.
    //
    // Use iUpdateMS to tell SageBox to only update the region every <iUpdateMS> millisecond, which can make it much faster
    //
    // A Bitmap may also be given to autmatically determine the region based on the bitmap size
    //
    // Note: When using iUpdateMS() a last Update() or UpdateRegion() without iUpdateMS will be required to ensure the last known 
    // draw of that region is updated to the screen
    //
    static bool UpdateRegion(int iX,int iY,RawBitmap_t stBitmap,int iUpdateMS = 0);

    //-->  Original Prototype: SIZE GetTextSize(const wchar_t * sText);

    // GetTextSize() -- Get the text size of the text using the current font.
    //
    // This returns the size the text will use in the window.  This can help with the placement of the text or controls around the text.
    // For example, using auto szSize = GetTextSize("This is some Text"), and then Write((szDesktopSize.cx-szSize.cx)/2,100,MyText)) 
    // will center the text in the X plane.
    //
    static SIZE GetTextSize(const wchar_t * sText);

    //-->  Original Prototype: SIZE GetTextSize(HFONT hFont,const wchar_t * sText);

    // GetTextSize() -- Get the text size of the text using the current font.
    //
    // This returns the size the text will use in the window.  This can help with the placement of the text or controls around the text.
    // For example, using auto szSize = GetTextSize("This is some Text"), and then Write((szDesktopSize.cx-szSize.cx)/2,100,MyText)) 
    // will center the text in the X plane.
    //
    static SIZE GetTextSize(HFONT hFont,const wchar_t * sText);

    //-->  Original Prototype: bool GetTextSize(const char * sText,SIZE & Size);

    // GetTextSize() -- Get the text size of the text using the current font.
    //
    // This returns the size the text will use in the window.  This can help with the placement of the text or controls around the text.
    // For example, using auto szSize = GetTextSize("This is some Text"), and then Write((szDesktopSize.cx-szSize.cx)/2,100,MyText)) 
    // will center the text in the X plane.
    //
    static bool GetTextSize(const char * sText,SIZE & Size);

    //-->  Original Prototype: SIZE GetTextSize(const char * sText);

    // GetTextSize() -- Get the text size of the text using the current font.
    //
    // This returns the size the text will use in the window.  This can help with the placement of the text or controls around the text.
    // For example, using auto szSize = GetTextSize("This is some Text"), and then Write((szDesktopSize.cx-szSize.cx)/2,100,MyText)) 
    // will center the text in the X plane.
    //
    static SIZE GetTextSize(const char * sText);

    //-->  Original Prototype: SIZE GetTextSize(HFONT hFont,const char * sText);

    static SIZE GetTextSize(HFONT hFont,const char * sText);

    //-->  Original Prototype: bool GetTextSize(HFONT hFont,const char * sText,SIZE & Size);

    static bool GetTextSize(HFONT hFont,const char * sText,SIZE & Size);

    //-->  Original Prototype: SIZE GetTextSize(const char * sFont,const char * sText);

    static SIZE GetTextSize(const char * sFont,const char * sText);

    //-->  Original Prototype: bool GetTextSize(const char * sFont,const char * sText,SIZE & Size);

    static bool GetTextSize(const char * sFont,const char * sText,SIZE & Size);

    //-->  Original Prototype: bool AddWindowShadow();

    // AddWindowShadow() -- Adds a shadow to the window.  This can be useful for popup-windows or
    // child windows embedded in the current window.
    //
    static bool AddWindowShadow();

    //-->  Original Prototype: bool DrawRectangle(int iX,int iY,int iWidth,int iHeight,int iColor,int iColor2 = -1);

    // Recangle() -- Put out a filled or outlined rectangle to the window.
    //
    // Rectangle() will display a rectangle to the screen, at point (x,y) with a width and height specified.
    // The Recangle function will generally draw a filled recangle, but can draw a rectangle with a specific PEN and BRUSH (Windows terms).
    //
    // Specifically, the first color is the background color (i.e. Windows BRUSH).  When only the first color is provided, the entire rectangle is filled
    // with this color.
    //
    // If a second color is specified, this is used as the Windows PEN, which is an outline.  The Windows Pen With is typicall 1, but SetPenWidth() can be used to set the width of the pen prior to calling
    // Rectangle() or any other function that uses a Windows PEN.
    //
    //        DWORD dwRed = RGB(255,0,0);        // Or cWindow->GetColor(CDavinci::Colors::Red);  or CDavinci::GetColor("Red");
    //        DWORD dwGreen = RGB(0,255,0);    // Or cWindow->GetColor(CDavinci::Colors::Green);     or GetColor("Green")   (when 'using namespace Davinci')
    //        Rectangle(50,50,400,200,dwRed); 
    //
    // Draw a filled rectangle of color RED.
    //
    //        Rectangle(50,50,400,200,dwRed,dwGreen);
    //
    // Draw a filled rectangle of color RED, with an outlined of color GREEN with the current Pen Width (defaults to 1)
    //
    static bool DrawRectangle(int iX,int iY,int iWidth,int iHeight,int iColor,int iColor2 = -1);

    //-->  Original Prototype: bool DrawRectangle(POINT pLoc,SIZE szSize,int iWidth,int iHeight,int iColor,int iColor2 = -1);

    // Recangle() -- Put out a filled or outlined rectangle to the window.
    //
    // Rectangle() will display a rectangle to the screen, at point (x,y) with a width and height specified.
    // The Recangle function will generally draw a filled recangle, but can draw a rectangle with a specific PEN and BRUSH (Windows terms).
    //
    // Specifically, the first color is the background color (i.e. Windows BRUSH).  When only the first color is provided, the entire rectangle is filled
    // with this color.
    //
    // If a second color is specified, this is used as the Windows PEN, which is an outline.  The Windows Pen With is typicall 1, but SetPenWidth() can be used to set the width of the pen prior to calling
    // Rectangle() or any other function that uses a Windows PEN.
    //
    //        DWORD dwRed = RGB(255,0,0);        // Or cWindow->GetColor(CDavinci::Colors::Red);  or CDavinci::GetColor("Red");
    //        DWORD dwGreen = RGB(0,255,0);    // Or cWindow->GetColor(CDavinci::Colors::Green);     or GetColor("Green")   (when 'using namespace Davinci')
    //        Rectangle(50,50,400,200,dwRed); 
    //
    // Draw a filled rectangle of color RED.
    //
    //        Rectangle(50,50,400,200,dwRed,dwGreen);
    //
    // Draw a filled rectangle of color RED, with an outlined of color GREEN with the current Pen Width (defaults to 1)
    //
    static bool DrawRectangle(POINT pLoc,SIZE szSize,int iWidth,int iHeight,int iColor,int iColor2 = -1);

    //-->  Original Prototype: bool DrawOpenRectangle(int iX,int iY,int iWidth,int iHeight,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);

    // DrawOpenRectangle() -- Draws an 'open' rectangle with only the outline color and no inside color (i.e. the inside
    // is not drawn, only the border). 
    //
    // This draws the border, the size of the current pen.  You can also add the pen size that will be used as an extra parameter, which will
    // override the current pen setting without changing the value of the current pen size
    //
    // See DrawRectangle() and SetPenSize() for more information
    //
    static bool DrawOpenRectangle(int iX,int iY,int iWidth,int iHeight,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);

    //-->  Original Prototype: bool DrawOpenRectangle(int iX,int iY,int iWidth,int iHeight,int iColor,int iPenSize = 0);

    // DrawOpenRectangle() -- Draws an 'open' rectangle with only the outline color and no inside color (i.e. the inside
    // is not drawn, only the border). 
    //
    // This draws the border, the size of the current pen.  You can also add the pen size that will be used as an extra parameter, which will
    // override the current pen setting without changing the value of the current pen size
    //
    // See DrawRectangle() and SetPenSize() for more information
    //
    static bool DrawOpenRectangle(int iX,int iY,int iWidth,int iHeight,int iColor,int iPenSize = 0);

    //-->  Original Prototype: bool DrawOpenRectangle(POINT pLoc,SIZE szSize,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);

    // DrawOpenRectangle() -- Draws an 'open' rectangle with only the outline color and no inside color (i.e. the inside
    // is not drawn, only the border). 
    //
    // This draws the border, the size of the current pen.  You can also add the pen size that will be used as an extra parameter, which will
    // override the current pen setting without changing the value of the current pen size
    //
    // See DrawRectangle() and SetPenSize() for more information
    //
    static bool DrawOpenRectangle(POINT pLoc,SIZE szSize,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);

    //-->  Original Prototype: bool DrawOpenRectangle(POINT pLoc,SIZE szSize,int iColor,int iPenSize = 0);

    // DrawOpenRectangle() -- Draws an 'open' rectangle with only the outline color and no inside color (i.e. the inside
    // is not drawn, only the border). 
    //
    // This draws the border, the size of the current pen.  You can also add the pen size that will be used as an extra parameter, which will
    // override the current pen setting without changing the value of the current pen size
    //
    // See DrawRectangle() and SetPenSize() for more information
    //
    static bool DrawOpenRectangle(POINT pLoc,SIZE szSize,int iColor,int iPenSize = 0);

    //-->  Original Prototype: bool DrawGradient(int ix,int iy,int iWidth,int iHeight,RGBColor_t rgbColor1,RGBColor_t rgbColor2,bool bHorizontal = false);

    static bool DrawGradient(int ix,int iy,int iWidth,int iHeight,RGBColor_t rgbColor1,RGBColor_t rgbColor2,bool bHorizontal = false);

    //-->  Original Prototype: bool DrawGradient(POINT pLoc,SIZE szSize,RGBColor_t rgbColor1,RGBColor_t rgbColor2,bool bHorizontal = false);

    static bool DrawGradient(POINT pLoc,SIZE szSize,RGBColor_t rgbColor1,RGBColor_t rgbColor2,bool bHorizontal = false);

    //-->  Original Prototype: bool DrawGradient(RECT rGradientRect,RGBColor_t rgbColor1,RGBColor_t rgbColor2,bool bHorizontal = false);

    static bool DrawGradient(RECT rGradientRect,RGBColor_t rgbColor1,RGBColor_t rgbColor2,bool bHorizontal = false);

    //-->  Original Prototype: bool DrawRectangle(int ix,int iy,int iWidth,int iHeight,RGBColor_t rgbColor  = Rgb::Default,RGBColor_t rgbColor2 = Rgb::Undefined);

    // Recangle() -- Put out a filled or outlined rectangle to the window.
    //
    // Rectangle() will display a rectangle to the screen, at point (x,y) with a width and height specified.
    // The Recangle function will generally draw a filled recangle, but can draw a rectangle with a specific PEN and BRUSH (Windows terms).
    //
    // Specifically, the first color is the background color (i.e. Windows BRUSH).  When only the first color is provided, the entire rectangle is filled
    // with this color.
    //
    // If a second color is specified, this is used as the Windows PEN, which is an outline.  The Windows Pen With is typicall 1, but SetPenWidth() can be used to set the width of the pen prior to calling
    // Rectangle() or any other function that uses a Windows PEN.
    //
    //        DWORD dwRed = RGB(255,0,0);        // Or cWindow->GetColor(CDavinci::Colors::Red);  or CDavinci::GetColor("Red");
    //        DWORD dwGreen = RGB(0,255,0);    // Or cWindow->GetColor(CDavinci::Colors::Green);     or GetColor("Green")   (when 'using namespace Davinci')
    //        Rectangle(50,50,400,200,dwRed); 
    //
    // Draw a filled rectangle of color RED.
    //
    //        Rectangle(50,50,400,200,dwRed,dwGreen);
    //
    // Draw a filled rectangle of color RED, with an outlined of color GREEN with the current Pen Width (defaults to 1)
    //
    static bool DrawRectangle(int ix,int iy,int iWidth,int iHeight,RGBColor_t rgbColor = Rgb::Default,RGBColor_t rgbColor2 = Rgb::Undefined);

    //-->  Original Prototype: bool DrawRectangle(POINT pLoc,SIZE szSize,RGBColor_t rgbColor = Rgb::Undefined,RGBColor_t rgbColor2 = Rgb::Undefined);

    // Recangle() -- Put out a filled or outlined rectangle to the window.
    //
    // Rectangle() will display a rectangle to the screen, at point (x,y) with a width and height specified.
    // The Recangle function will generally draw a filled recangle, but can draw a rectangle with a specific PEN and BRUSH (Windows terms).
    //
    // Specifically, the first color is the background color (i.e. Windows BRUSH).  When only the first color is provided, the entire rectangle is filled
    // with this color.
    //
    // If a second color is specified, this is used as the Windows PEN, which is an outline.  The Windows Pen With is typicall 1, but SetPenWidth() can be used to set the width of the pen prior to calling
    // Rectangle() or any other function that uses a Windows PEN.
    //
    //        DWORD dwRed = RGB(255,0,0);        // Or cWindow->GetColor(CDavinci::Colors::Red);  or CDavinci::GetColor("Red");
    //        DWORD dwGreen = RGB(0,255,0);    // Or cWindow->GetColor(CDavinci::Colors::Green);     or GetColor("Green")   (when 'using namespace Davinci')
    //        Rectangle(50,50,400,200,dwRed); 
    //
    // Draw a filled rectangle of color RED.
    //
    //        Rectangle(50,50,400,200,dwRed,dwGreen);
    //
    // Draw a filled rectangle of color RED, with an outlined of color GREEN with the current Pen Width (defaults to 1)
    //
    static bool DrawRectangle(POINT pLoc,SIZE szSize,RGBColor_t rgbColor = Rgb::Undefined,RGBColor_t rgbColor2 = Rgb::Undefined);

    //-->  Original Prototype: bool Rectangle2(int ix,int iy,int iWidth,int iHeight,int iColor,int iColor2 = -1);

    // Ractangle2() -- Used for testing 
    //
    static bool Rectangle2(int ix,int iy,int iWidth,int iHeight,int iColor,int iColor2 = -1);

    //-->  Original Prototype: bool DrawTriangle(POINT v1,POINT v2,POINT v3,int iColor1,int iColor2 = -1);

    // Draw a Triangle on the screen using the three points
    // The third point will connect directly to the first point.
    //
    // The first Color (which can be RGB() or RGBColor_t()) is the internal color.
    // The second color is the outline color (if ommitted, there is no outline).
    // The outline size is determine by the Pen Thickness, which defaults to 1.
    //
    // The Pen Thickness can be changed with SetPenThickness()
    //
    static bool DrawTriangle(POINT v1,POINT v2,POINT v3,int iColor1,int iColor2 = -1);

    //-->  Original Prototype: bool DrawQuadrangle(POINT v1,POINT v2,POINT v3,POINT v4,int iColor1,int iColor2 = -1);

    // Draw a Quadrangle on the screen using the three points
    // The fourth point will connect directly to the first point.
    //
    // The first Color (which can be RGB() or RGBColor_t()) is the internal color.
    // The second color is the outline color (if ommitted, there is no outline).
    // The outline size is determine by the Pen Thickness, which defaults to 1.
    //
    // The Pen Thickness can be changed with SetPenThickness()
    //
    static bool DrawQuadrangle(POINT v1,POINT v2,POINT v3,POINT v4,int iColor1,int iColor2 = -1);

    //-->  Original Prototype: bool DrawPolygon(POINT * pPoints,int iVertices,int iColor1,int iColor2 = -1);

    // Draw a Polygon on the screen using an array of POINT * values.
    // The last point will connect directly to the first point.
    //
    // The first Color (which can be RGB() or RGBColor_t()) is the internal color.
    // The second color is the outline color (if ommitted, there is no outline).
    // The outline size is determine by the Pen Thickness, which defaults to 1.
    //
    // The Pen Thickness can be changed with SetPenThickness()
    //
    static bool DrawPolygon(POINT * pPoints,int iVertices,int iColor1,int iColor2 = -1);

    //-->  Original Prototype: bool DrawCircle(int iX,int iY,int iRadius,int iColor1,int iColor2 = -1);

    // Draw a Circle on the Window
    //
    // The first Color (which can be RGB() or RGBColor_t()) is the internal color.
    // The second color is the outline color (if ommitted, there is no outline).
    // The outline size is determine by the Pen Thickness, which defaults to 1.
    //
    // The Pen Thickness can be changed with SetPenThickness()
    //
    static bool DrawCircle(int iX,int iY,int iRadius,int iColor1,int iColor2 = -1);

    //-->  Original Prototype: bool DrawCircle(int iX,int iY,int iRadius,RGBColor_t rgbColorIn = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined);

    // Draw a Circle on the Window
    //
    // The first Color (which can be RGB() or RGBColor_t()) is the internal color.
    // The second color is the outline color (if ommitted, there is no outline).
    // The outline size is determine by the Pen Thickness, which defaults to 1.
    //
    // The Pen Thickness can be changed with SetPenThickness()
    //
    static bool DrawCircle(int iX,int iY,int iRadius,RGBColor_t rgbColorIn = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined);

    //-->  Original Prototype: bool DrawCircle(POINT pLoc,int iRadius,RGBColor_t rgbColorIn = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined);

    // Draw a Circle on the Window
    //
    // The first Color (which can be RGB() or RGBColor_t()) is the internal color.
    // The second color is the outline color (if ommitted, there is no outline).
    // The outline size is determine by the Pen Thickness, which defaults to 1.
    //
    // The Pen Thickness can be changed with SetPenThickness()
    //
    static bool DrawCircle(POINT pLoc,int iRadius,RGBColor_t rgbColorIn = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined);

    //-->  Original Prototype: bool DrawCircle(POINT pLoc,int iRadius,int iColor1,int iColor2 = -1);

    // Draw a Circle on the Window
    //
    // The first Color (which can be RGB() or RGBColor_t()) is the internal color.
    // The second color is the outline color (if ommitted, there is no outline).
    // The outline size is determine by the Pen Thickness, which defaults to 1.
    //
    // The Pen Thickness can be changed with SetPenThickness()
    //
    static bool DrawCircle(POINT pLoc,int iRadius,int iColor1,int iColor2 = -1);

    //-->  Original Prototype: bool DrawOpenCircle(int iX,int iY,int iRadius,RGBColor_t rgbColor = Rgb::Default,int iPenSize =0);

    // DrawOpenCircle() -- Draws an 'open' circle with only the outline color and no inside color (i.e. the inside
    // is not drawn, only the border). 
    //
    // This draws the border, the size of the current pen.  You can also add the pen size that will be used as an extra parameter, which will
    // override the current pen setting without changing the value of the current pen size
    //
    // See DrawCircle() and SetPenSize() for more information
    //
    static bool DrawOpenCircle(int iX,int iY,int iRadius,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);

    //-->  Original Prototype: bool DrawOpenCircle(int iX,int iY,int iRadius,int iColor,int iPenSize = 0);

    // DrawOpenCircle() -- Draws an 'open' circle with only the outline color and no inside color (i.e. the inside
    // is not drawn, only the border). 
    //
    // This draws the border, the size of the current pen.  You can also add the pen size that will be used as an extra parameter, which will
    // override the current pen setting without changing the value of the current pen size
    //
    // See DrawCircle() and SetPenSize() for more information
    //
    static bool DrawOpenCircle(int iX,int iY,int iRadius,int iColor,int iPenSize = 0);

    //-->  Original Prototype: bool DrawOpenCircle(POINT pLoc,int iRadius,RGBColor_t rgbColor = Rgb::Default,int iPenSize =0 );

    // DrawOpenCircle() -- Draws an 'open' circle with only the outline color and no inside color (i.e. the inside
    // is not drawn, only the border). 
    //
    // This draws the border, the size of the current pen.  You can also add the pen size that will be used as an extra parameter, which will
    // override the current pen setting without changing the value of the current pen size
    //
    // See DrawCircle() and SetPenSize() for more information
    //
    static bool DrawOpenCircle(POINT pLoc,int iRadius,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0 );

    //-->  Original Prototype: bool DrawOpenCircle(POINT pLoc,int iRadius,int iColor,int iPenSize =0 );

    // DrawOpenCircle() -- Draws an 'open' circle with only the outline color and no inside color (i.e. the inside
    // is not drawn, only the border). 
    //
    // This draws the border, the size of the current pen.  You can also add the pen size that will be used as an extra parameter, which will
    // override the current pen setting without changing the value of the current pen size
    //
    // See DrawCircle() and SetPenSize() for more information
    //
    static bool DrawOpenCircle(POINT pLoc,int iRadius,int iColor,int iPenSize = 0 );

    //-->  Original Prototype: bool DrawEllipse(int iX,int iY,int iRadiusX,int iRadiusY,int iColor1,int iColor2 = -1);

    // Draw an Ellipse on the Window
    //
    // The first Color (which can be RGB() or RGBColor_t()) is the internal color.
    // The second color is the outline color (if ommitted, there is no outline).
    // The outline size is determine by the Pen Thickness, which defaults to 1.
    //
    // The Pen Size can be changed with SetPenSize()
    //
    static bool DrawEllipse(int iX,int iY,int iRadiusX,int iRadiusY,int iColor1,int iColor2 = -1);

    //-->  Original Prototype: bool DrawEllipse(int iX,int iY,int iRadiusX,int iRadiusY,RGBColor_t rgbColorIn = Rgb::Default,RGBColor_t rgbColorOut = Rgb::None);

    // Draw a ellipse on the Window
    //
    // The first Color (which can be RGB() or RGBColor_t()) is the internal color.
    // The second color is the outline color (if ommitted, there is no outline).
    // The outline size is determine by the Pen Thickness, which defaults to 1.
    //
    // The Pen Thickness can be changed with SetPenThickness()
    //
    static bool DrawEllipse(int iX,int iY,int iRadiusX,int iRadiusY,RGBColor_t rgbColorIn = Rgb::Default,RGBColor_t rgbColorOut = Rgb::None);

    //-->  Original Prototype: bool DrawEllipse(POINT pLoc,int iRadiusX,int iRadiusY,int iColor1,int iColor2 = -1);

    // Draw a ellipse on the Window
    //
    // The first Color (which can be RGB() or RGBColor_t()) is the internal color.
    // The second color is the outline color (if ommitted, there is no outline).
    // The outline size is determine by the Pen Thickness, which defaults to 1.
    //
    // The Pen Thickness can be changed with SetPenThickness()
    //
    static bool DrawEllipse(POINT pLoc,int iRadiusX,int iRadiusY,int iColor1,int iColor2 = -1);

    //-->  Original Prototype: bool DrawEllipse(POINT pLoc,int iRadiusX,int iRadiusY,RGBColor_t rgbColorIn = Rgb::Default,RGBColor_t rgbColorOut = Rgb::None);

    // Draw a ellipse on the Window
    //
    // The first Color (which can be RGB() or RGBColor_t()) is the internal color.
    // The second color is the outline color (if ommitted, there is no outline).
    // The outline size is determine by the Pen Thickness, which defaults to 1.
    //
    // The Pen Thickness can be changed with SetPenThickness()
    //
    static bool DrawEllipse(POINT pLoc,int iRadiusX,int iRadiusY,RGBColor_t rgbColorIn = Rgb::Default,RGBColor_t rgbColorOut = Rgb::None);

    //-->  Original Prototype: bool DrawOpenEllipse(int iX,int iY,int iRadiusX,int iRadiusY,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);

    // DrawOpenEllipse() -- Draws an 'open' ellipse with only the outline color and no inside color (i.e. the inside
    // is not drawn, only the border). 
    //
    // This draws the border, the size of the current pen.  You can also add the pen size that will be used as an extra parameter, which will
    // override the current pen setting without changing the value of the current pen size
    //
    // See DrawCircle() and SetPenSize() for more information
    //
    static bool DrawOpenEllipse(int iX,int iY,int iRadiusX,int iRadiusY,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);

    //-->  Original Prototype: bool DrawOpenEllipse(int iX,int iY,int iRadiusX,int iRadiusY,int iColor,int iPenSize = 0);

    // DrawOpenEllipse() -- Draws an 'open' ellipse with only the outline color and no inside color (i.e. the inside
    // is not drawn, only the border). 
    //
    // This draws the border, the size of the current pen.  You can also add the pen size that will be used as an extra parameter, which will
    // override the current pen setting without changing the value of the current pen size
    //
    // See DrawCircle() and SetPenSize() for more information
    //
    static bool DrawOpenEllipse(int iX,int iY,int iRadiusX,int iRadiusY,int iColor,int iPenSize = 0);

    //-->  Original Prototype: bool DrawOpenEllipse(POINT pLoc,int iRadiusX,int iRadiusY,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);

    // DrawOpenEllipse() -- Draws an 'open' ellipse with only the outline color and no inside color (i.e. the inside
    // is not drawn, only the border). 
    //
    // This draws the border, the size of the current pen.  You can also add the pen size that will be used as an extra parameter, which will
    // override the current pen setting without changing the value of the current pen size
    //
    // See DrawCircle() and SetPenSize() for more information
    //
    static bool DrawOpenEllipse(POINT pLoc,int iRadiusX,int iRadiusY,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);

    //-->  Original Prototype: bool DrawOpenEllipse(POINT pLoc,int iRadiusX,int iRadiusY,int iColor,int iPenSize = 0);

    // DrawOpenEllipse() -- Draws an 'open' ellipse with only the outline color and no inside color (i.e. the inside
    // is not drawn, only the border). 
    //
    // This draws the border, the size of the current pen.  You can also add the pen size that will be used as an extra parameter, which will
    // override the current pen setting without changing the value of the current pen size
    //
    // See DrawCircle() and SetPenSize() for more information
    //
    static bool DrawOpenEllipse(POINT pLoc,int iRadiusX,int iRadiusY,int iColor,int iPenSize = 0);

    //-->  Original Prototype: bool SetPenColor(int iColor);

    // Set the  color for new lines, circles, rectangles,e tc. using SetPenColor() (or MoveTo()) and then DrawLine() (or LineTo())
    // This allows a default line color to be selected so that functions later drawing lines with SetLinePos()/DrawLine()
    // don't need to set the color. 
    //
    // With all drawing routines, a color may be specified.  This does not change the pen color, drawing the shape in the 
    // color specified, leaving the pen color the same as it was before the call to the function.
    //
    // note: This does not work with DrawLine().  When using DrawLine(), the color must be specifieed.
    //
    // note: This function is in progress and does not work with all drawing routines yet.  For some drawing routines, you may
    // need to specify a color.
    //
    static bool SetPenColor(int iColor);

    //-->  Original Prototype: bool SetPenColor(const char * sColor);

    // Set the  color for new lines, circles, rectangles,e tc. using SetPenColor() (or MoveTo()) and then DrawLine() (or LineTo())
    // This allows a default line color to be selected so that functions later drawing lines with SetLinePos()/DrawLine()
    // don't need to set the color. 
    //
    // With all drawing routines, a color may be specified.  This does not change the pen color, drawing the shape in the 
    // color specified, leaving the pen color the same as it was before the call to the function.
    //
    // note: This does not work with DrawLine().  When using DrawLine(), the color must be specifieed.
    //
    // note: This function is in progress and does not work with all drawing routines yet.  For some drawing routines, you may
    // need to specify a color.
    //
    static bool SetPenColor(const char * sColor);

    //-->  Original Prototype: bool SetPenColor(RGBColor_t rgbColor);

    // Set the DrawLine color for new lines using SetLinePos() (or MoveTo()) and then DrawLine() (or LineTo())
    // This allows a default line color to be selected so that functions later drawing lines with SetLinePos()/DrawLine()
    // don't need to set the color. 
    //
    // note: This does not work with DrawLine().  When using DrawLine(), the color must be specifieed.
    //
    static bool SetPenColor(RGBColor_t rgbColor);

    //-->  Original Prototype: bool SetLinePos(int iX,int iY,int iColor);

    // SetLinePos() -- Set the position for the next LineTo() draw.  This can be used to set the 
    // initial draw position when no line has been previously drawm, or to reset the position when DrawLine()
    // has been used and the draw position is set to the next position. 
    //
    // If a color is specified, LineTo() will use this color.   Otherwise, LineTo() uses the last draw line color
    // or a default color.
    //
    // A color may also be specified to LineTo()
    //
    // MoveTo() may be used instead of SetLinePos() for more compatibility with Windows. 
    // MoveTo() is exactly the same function as SetLinePos(), just with a different name
    //
    static bool SetLinePos(int iX,int iY,int iColor);

    //-->  Original Prototype: bool SetLinePos(int iX,int iY,RGBColor_t rgbColor = Rgb::Undefined);

    // SetLinePos() -- Set the position for the next LineTo() draw.  This can be used to set the 
    // initial draw position when no line has been previously drawm, or to reset the position when DrawLine()
    // has been used and the draw position is set to the next position. 
    //
    // If a color is specified, LineTo() will use this color.   Otherwise, LineTo() uses the last draw line color
    // or a default color.
    //
    // A color may also be specified to LineTo()
    //
    // MoveTo() may be used instead of SetLinePos() for more compatibility with Windows. 
    // MoveTo() is exactly the same function as SetLinePos(), just with a different name
    //
    static bool SetLinePos(int iX,int iY,RGBColor_t rgbColor = Rgb::Undefined);

    //-->  Original Prototype: bool SetLinePos(POINT pLoc,int iColor);

    // SetLinePos() -- Set the position for the next LineTo() draw.  This can be used to set the 
    // initial draw position when no line has been previously drawm, or to reset the position when DrawLine()
    // has been used and the draw position is set to the next position. 
    //
    // If a color is specified, LineTo() will use this color.   Otherwise, LineTo() uses the last draw line color
    // or a default color.
    //
    // A color may also be specified to LineTo()
    //
    // MoveTo() may be used instead of SetLinePos() for more compatibility with Windows. 
    // MoveTo() is exactly the same function as SetLinePos(), just with a different name
    //
    static bool SetLinePos(POINT pLoc,int iColor);

    //-->  Original Prototype: bool SetLinePos(POINT pLoc,RGBColor_t rgbColor = Rgb::Undefined);

    // SetLinePos() -- Set the position for the next LineTo() draw.  This can be used to set the 
    // initial draw position when no line has been previously drawm, or to reset the position when DrawLine()
    // has been used and the draw position is set to the next position. 
    //
    // If a color is specified, LineTo() will use this color.   Otherwise, LineTo() uses the last draw line color
    // or a default color.
    //
    // A color may also be specified to LineTo()
    //
    // MoveTo() may be used instead of SetLinePos() for more compatibility with Windows. 
    // MoveTo() is exactly the same function as SetLinePos(), just with a different name
    //
    static bool SetLinePos(POINT pLoc,RGBColor_t rgbColor = Rgb::Undefined);

    //-->  Original Prototype: bool MoveTo(int iX,int iY,int iColor);

    // MoveTo() -- Set the position for the next LineTo() draw.  This can be used to set the 
    //
    // MoveTo() is the same as SetLinePos() and is provided for convenience (since its shorter and more intuitive)
    //
    // initial draw position when no line has been previously drawm, or to reset the position when DrawLine()
    // has been used and the draw position is set to the next position. 
    //
    // If a color is specified, LineTo() will use this color.   Otherwise, LineTo() uses the last draw line color
    // or a default color.l
    //
    // A color may also be specified to LineTo()
    //
    static bool MoveTo(int iX,int iY,int iColor);

    //-->  Original Prototype: bool MoveTo(int iX,int iY,RGBColor_t rgbColor = Rgb::Undefined);

    // MoveTo() -- Set the position for the next LineTo() draw.  This can be used to set the 
    //
    // MoveTo() is the same as SetLinePos() and is provided for convenience (since its shorter and more intuitive)
    //
    // initial draw position when no line has been previously drawm, or to reset the position when DrawLine()
    // has been used and the draw position is set to the next position. 
    //
    // If a color is specified, LineTo() will use this color.   Otherwise, LineTo() uses the last draw line color
    // or a default color.
    //
    // A color may also be specified to LineTo()
    //
    static bool MoveTo(int iX,int iY,RGBColor_t rgbColor = Rgb::Undefined);

    //-->  Original Prototype: bool MoveTo(POINT pLoc,int iColor);

    // MoveTo() -- Set the position for the next LineTo() draw.  This can be used to set the 
    //
    // MoveTo() is the same as SetLinePos() and is provided for convenience (since its shorter and more intuitive)
    //
    // initial draw position when no line has been previously drawm, or to reset the position when DrawLine()
    // has been used and the draw position is set to the next position. 
    //
    // If a color is specified, LineTo() will use this color.   Otherwise, LineTo() uses the last draw line color
    // or a default color.
    //
    // A color may also be specified to LineTo()
    //
    static bool MoveTo(POINT pLoc,int iColor);

    //-->  Original Prototype: bool MoveTo(POINT pLoc,RGBColor_t rgbColor = Rgb::Undefined);

    // MoveTo() -- Set the position for the next LineTo() draw.  This can be used to set the 
    //
    // MoveTo() is the same as SetLinePos() and is provided for convenience (since its shorter and more intuitive)
    //
    // initial draw position when no line has been previously drawm, or to reset the position when DrawLine()
    // has been used and the draw position is set to the next position. 
    //
    // If a color is specified, LineTo() will use this color.   Otherwise, LineTo() uses the last draw line color
    // or a default color.
    //
    // A color may also be specified to LineTo()
    //
    static bool MoveTo(POINT pLoc,RGBColor_t rgbColor = Rgb::Undefined);

    //-->  Original Prototype: bool LineTo(int iX,int iY,RGBColor_t rgbColor = Rgb::Undefined);

    // Draw a line to the X,Y position specified.    This draws a line from the current position set by 
    // a previous DrawLine() or SetLinePos(). 
    //
    // If no color is specified, the last color used in DrawLine() -- or the color specified in SetLinePos() is used. 
    //
    // If no line has been drawn, use SetLinePos() to Set the draw-line position start (and optional color)
    //
    // LineTo() and DrawLineTo() are the same function.  DrawLineTo() is added for consitency with naming conventions with DrawLine()
    // and other Draw functions
    //
    static bool LineTo(int iX,int iY,RGBColor_t rgbColor = Rgb::Undefined);

    //-->  Original Prototype: bool LineTo(int iX,int iY,int iColor);

    // Draw a line to the X,Y n specified.    This draws a line from the current position set by 
    // a previous DrawLine() or SetLinePos(). 
    //
    // If no color is specified, the last color used in DrawLine() -- or the color specified in SetLinePos() is used. 
    //
    // If no line has been drawn, use SetLinePos() to Set the draw-line position start (and optional color)
    //
    // LineTo() and DrawLineTo() are the same function.  DrawLineTo() is added for consitency with naming conventions with DrawLine()
    // and other Draw functions
    //
    static bool LineTo(int iX,int iY,int iColor);

    //-->  Original Prototype: bool LineTo(POINT pLoc,RGBColor_t rgbColor = Rgb::Undefined);

    // Draw a line to the X,Y n specified.    This draws a line from the current position set by 
    // a previous DrawLine() or SetLinePos(). 
    //
    // If no color is specified, the last color used in DrawLine() -- or the color specified in SetLinePos() is used. 
    //
    // If no line has been drawn, use SetLinePos() to Set the draw-line position start (and optional color)
    //
    // LineTo() and DrawLineTo() are the same function.  DrawLineTo() is added for consitency with naming conventions with DrawLine()
    // and other Draw functions
    //
    static bool LineTo(POINT pLoc,RGBColor_t rgbColor = Rgb::Undefined);

    //-->  Original Prototype: bool LineTo(POINT pLoc,int iColor);

    // Draw a line to the X,Y n specified.    This draws a line from the current position set by 
    // a previous DrawLine() or SetLinePos(). 
    //
    // If no color is specified, the last color used in DrawLine() -- or the color specified in SetLinePos() is used. 
    //
    // If no line has been drawn, use SetLinePos() to Set the draw-line position start (and optional color)
    //
    // LineTo() and DrawLineTo() are the same function.  DrawLineTo() is added for consitency with naming conventions with DrawLine()
    // and other Draw functions
    //
    static bool LineTo(POINT pLoc,int iColor);

    //-->  Original Prototype: bool LineToEx(bool bFirstPoint,int iX,int iY,RGBColor_t rgbColor = Rgb::Undefined);

    // LineToEx() -- Draw a Line to the current Point or Set the current point.
    // LineToEx() is the same as DrawLineTo()
    //
    // if (bFirstPoint) is true, MoveTo() is used to set the point.  When bFirstpoint is false, LineTo() is used instead.
    // This allows loops to not distingiush between first point vs future points.
    //
    // For Example,
    //
    // -->  for(int i=0;i<100;i++) { Cpoint MyPoint = SomeFunction(); cWin.LineToEx(!i,MyPoint); }
    //
    // This will set the point (i.e. MoveTo() when i == 0, and then use LineTo() after that.
    //
    static bool LineToEx(bool bFirstPoint,int iX,int iY,RGBColor_t rgbColor = Rgb::Undefined);

    //-->  Original Prototype: bool LineToEx(bool bFirstPoint,int iX,int iY,int iColor);

    // LineToEx() -- Draw a Line to the current Point or Set the current point.
    // LineToEx() is the same as DrawLineTo()
    //
    // if (bFirstPoint) is true, MoveTo() is used to set the point.  When bFirstpoint is false, LineTo() is used instead.
    // This allows loops to not distingiush between first point vs future points.
    //
    // For Example,
    //
    // -->  for(int i=0;i<100;i++) { Cpoint MyPoint = SomeFunction(); cWin.LineToEx(!i,MyPoint); }
    //
    // This will set the point (i.e. MoveTo() when i == 0, and then use LineTo() after that.
    //
    static bool LineToEx(bool bFirstPoint,int iX,int iY,int iColor);

    //-->  Original Prototype: bool LineToEx(bool bFirstPoint,POINT pLoc,RGBColor_t rgbColor = Rgb::Undefined);

    // LineToEx() -- Draw a Line to the current Point or Set the current point.
    // LineToEx() is the same as DrawLineTo()
    //
    // if (bFirstPoint) is true, MoveTo() is used to set the point.  When bFirstpoint is false, LineTo() is used instead.
    // This allows loops to not distingiush between first point vs future points.
    //
    // For Example,
    //
    // -->  for(int i=0;i<100;i++) { Cpoint MyPoint = SomeFunction(); cWin.LineToEx(!i,MyPoint); }
    //
    // This will set the point (i.e. MoveTo() when i == 0, and then use LineTo() after that.
    //
    static bool LineToEx(bool bFirstPoint,POINT pLoc,RGBColor_t rgbColor = Rgb::Undefined);

    //-->  Original Prototype: bool LineToEx(bool bFirstPoint,POINT pLoc,int iColor);

    // LineToEx() -- Draw a Line to the current Point or Set the current point.
    // LineToEx() is the same as DrawLineTo()
    //
    // if (bFirstPoint) is true, MoveTo() is used to set the point.  When bFirstpoint is false, LineTo() is used instead.
    // This allows loops to not distingiush between first point vs future points.
    //
    // For Example,
    //
    // -->  for(int i=0;i<100;i++) { Cpoint MyPoint = SomeFunction(); cWin.LineToEx(!i,MyPoint); }
    //
    // This will set the point (i.e. MoveTo() when i == 0, and then use LineTo() after that.
    //
    static bool LineToEx(bool bFirstPoint,POINT pLoc,int iColor);

    //-->  Original Prototype: bool DrawLineTo(int iX,int iY,RGBColor_t rgbColor = Rgb::Undefined);

    // Draw a line to the X,Y position specified.    This draws a line from the current position set by 
    // a previous DrawLine() or SetLinePos(). 
    //
    // If no color is specified, the last color used in DrawLine() -- or the color specified in SetLinePos() is used. 
    //
    // If no line has been drawn, use SetLinePos() to Set the draw-line position start (and optional color)
    //
    // LineTo() and DrawLineTo() are the same function.  DrawLineTo() is added for consitency with naming conventions with DrawLine()
    // and other Draw functions
    //
    static bool DrawLineTo(int iX,int iY,RGBColor_t rgbColor = Rgb::Undefined);

    //-->  Original Prototype: bool DrawLineTo(int iX,int iY,int iColor);

    // Draw a line to the X,Y position specified.    This draws a line from the current position set by 
    // a previous DrawLine() or SetLinePos(). 
    //
    // If no color is specified, the last color used in DrawLine() -- or the color specified in SetLinePos() is used. 
    //
    // If no line has been drawn, use SetLinePos() to Set the draw-line position start (and optional color)
    //
    // LineTo() and DrawLineTo() are the same function.  DrawLineTo() is added for consitency with naming conventions with DrawLine()
    // and other Draw functions
    //
    static bool DrawLineTo(int iX,int iY,int iColor);

    //-->  Original Prototype: bool DrawLineTo(POINT pLoc,RGBColor_t rgbColor = Rgb::Undefined);

    // Draw a line to the X,Y position specified.    This draws a line from the current position set by 
    // a previous DrawLine() or SetLinePos(). 
    //
    // If no color is specified, the last color used in DrawLine() -- or the color specified in SetLinePos() is used. 
    //
    // If no line has been drawn, use SetLinePos() to Set the draw-line position start (and optional color)
    //
    // LineTo() and DrawLineTo() are the same function.  DrawLineTo() is added for consitency with naming conventions with DrawLine()
    // and other Draw functions
    //
    static bool DrawLineTo(POINT pLoc,RGBColor_t rgbColor = Rgb::Undefined);

    //-->  Original Prototype: bool DrawLineTo(POINT pLoc,int iColor =-1);

    // Draw a line to the X,Y position specified.    This draws a line from the current position set by 
    // a previous DrawLine() or SetLinePos(). 
    //
    // If no color is specified, the last color used in DrawLine() -- or the color specified in SetLinePos() is used. 
    //
    // If no line has been drawn, use SetLinePos() to Set the draw-line position start (and optional color)
    //
    // LineTo() and DrawLineTo() are the same function.  DrawLineTo() is added for consitency with naming conventions with DrawLine()
    // and other Draw functions
    //
    static bool DrawLineTo(POINT pLoc,int iColor = -1);

    //-->  Original Prototype: bool DrawLineToEx(bool bFirstPoint,int iX,int iY,RGBColor_t rgbColor = Rgb::Undefined);

    // DrawLineToEx() -- Draw a Line to the current Point or Set the current point.
    // DrawLineToEx() is the same as LineToEx()
    //
    // if (bFirstPoint) is true, MoveTo() is used to set the point.  When bFirstpoint is false, LineTo() is used instead.
    // This allows loops to not distingiush between first point vs future points.
    //
    // For Example,
    //
    // -->  for(int i=0;i<100;i++) { Cpoint MyPoint = SomeFunction(); cWin.DrawLineToEx(!i,MyPoint); }
    //
    // This will set the point (i.e. MoveTo() when i == 0, and then use LineTo() after that.
    //
    static bool DrawLineToEx(bool bFirstPoint,int iX,int iY,RGBColor_t rgbColor = Rgb::Undefined);

    //-->  Original Prototype: bool DrawLineToEx(bool bFirstPoint,int iX,int iY,int iColor);

    // DrawLineToEx() -- Draw a Line to the current Point or Set the current point.
    // DrawLineToEx() is the same as LineToEx()
    //
    // if (bFirstPoint) is true, MoveTo() is used to set the point.  When bFirstpoint is false, LineTo() is used instead.
    // This allows loops to not distingiush between first point vs future points.
    //
    // For Example,
    //
    // -->  for(int i=0;i<100;i++) { Cpoint MyPoint = SomeFunction(); cWin.DrawLineToEx(!i,MyPoint); }
    //
    // This will set the point (i.e. MoveTo() when i == 0, and then use LineTo() after that.
    //
    static bool DrawLineToEx(bool bFirstPoint,int iX,int iY,int iColor);

    //-->  Original Prototype: bool DrawLineToEx(bool bFirstPoint,POINT pLoc,RGBColor_t rgbColor = Rgb::Undefined);

    // DrawLineToEx() -- Draw a Line to the current Point or Set the current point.
    // DrawLineToEx() is the same as LineToEx()
    //
    // if (bFirstPoint) is true, MoveTo() is used to set the point.  When bFirstpoint is false, LineTo() is used instead.
    // This allows loops to not distingiush between first point vs future points.
    //
    // For Example,
    //
    // -->  for(int i=0;i<100;i++) { Cpoint MyPoint = SomeFunction(); cWin.DrawLineToEx(!i,MyPoint); }
    //
    // This will set the point (i.e. MoveTo() when i == 0, and then use LineTo() after that.
    //
    static bool DrawLineToEx(bool bFirstPoint,POINT pLoc,RGBColor_t rgbColor = Rgb::Undefined);

    //-->  Original Prototype: bool DrawLineToEx(bool bFirstPoint,POINT pLoc,int iColor);

    // DrawLineToEx() -- Draw a Line to the current Point or Set the current point.
    // DrawLineToEx() is the same as LineToEx()
    //
    // if (bFirstPoint) is true, MoveTo() is used to set the point.  When bFirstpoint is false, LineTo() is used instead.
    // This allows loops to not distingiush between first point vs future points.
    //
    // For Example,
    //
    // -->  for(int i=0;i<100;i++) { Cpoint MyPoint = SomeFunction(); cWin.DrawLineToEx(!i,MyPoint); }
    //
    // This will set the point (i.e. MoveTo() when i == 0, and then use LineTo() after that.
    //
    static bool DrawLineToEx(bool bFirstPoint,POINT pLoc,int iColor);

    //-->  Original Prototype: bool DrawLine(int ix1,int iy1,int ix2,int iy2,int iColor);

    // Draw a line in the window.  
    // This draws a line from (ix1,iy1) to (ix2,iy2) in the given color
    //
    // The color can be RGB() or RGBColor_t
    // The thickness of the line can be changed with SetPenThickness(), which defaults to 1.
    //
    // If the color is omitted, the current line color is used.
    // The line color can be set or changed with SetLineColor() or SetLinePos()
    //
    static bool DrawLine(int ix1,int iy1,int ix2,int iy2,int iColor);

    //-->  Original Prototype: bool DrawLine(POINT p1,POINT p2,int iColor);

    // Draw a line in the window.  
    // This draws a line from (ix1,iy1) to (ix2,iy2) in the given color
    //
    // The color can be RGB() or RGBColor_t
    // The thickness of the line can be changed with SetPenThickness(), which defaults to 1.
    //
    // If the color is omitted, the current line color is used.
    // The line color can be set or changed with SetLineColor() or SetLinePos()
    //
    static bool DrawLine(POINT p1,POINT p2,int iColor);

    //-->  Original Prototype: bool DrawLine(POINT p1,POINT p2,RGBColor_t rgbColor = Rgb::Default);

    // Draw a line in the window.  
    // This draws a line from (ix1,iy1) to (ix2,iy2) in the given color
    //
    // The color can be RGB() or RGBColor_t
    // The thickness of the line can be changed with SetPenThickness(), which defaults to 1.
    //
    // If the color is omitted, the current line color is used.
    // The line color can be set or changed with SetLineColor() or SetLinePos()
    //
    static bool DrawLine(POINT p1,POINT p2,RGBColor_t rgbColor = Rgb::Default);

    //-->  Original Prototype: bool DrawLine(int ix1,int iy1,int ix2,int iy2,RGBColor_t rgbColor = Rgb::Default);

    // Draw a line in the window.  
    // This draws a line from (ix1,iy1) to (ix2,iy2) in the given color
    //
    // The color can be RGB() or RGBColor_t
    // The thickness of the line can be changed with SetPenThickness(), which defaults to 1.
    //
    // If the color is omitted, the current line color is used.
    // The line color can be set or changed with SetLineColor() or SetLinePos()
    //
    static bool DrawLine(int ix1,int iy1,int ix2,int iy2,RGBColor_t rgbColor = Rgb::Default);

    //-->  Original Prototype: bool DrawLine2(int ix1,int iy1,int iWidth,int iHeight,int iColor);

    // Draw a line in the window.  
    // This draws a line from (ix1,iy1) to (ix2,iy2) in the given color
    //
    // The color can be RGB() or RGBColor_t
    // The thickness of the line can be changed with SetPenThickness(), which defaults to 1.
    //
    static bool DrawLine2(int ix1,int iy1,int iWidth,int iHeight,int iColor);

    //-->  Original Prototype: bool DrawLine2(int ix1,int iy1,int iWidth,int iHeight,RGBColor_t rgbColor = Rgb::Default);

    // Draw a line in the window.  
    // This draws a line from (ix1,iy1) to (ix2,iy2) in the given color
    //
    // The color can be RGB() or RGBColor_t
    // The thickness of the line can be changed with SetPenThickness(), which defaults to 1.
    //
    static bool DrawLine2(int ix1,int iy1,int iWidth,int iHeight,RGBColor_t rgbColor = Rgb::Default);

    //-->  Original Prototype: bool DrawLine2(POINT p1,SIZE szDist,int iColor);

    // Draw a line in the window.  
    // This draws a line from (ix1,iy1) to (ix2,iy2) in the given color
    //
    // The color can be RGB() or RGBColor_t
    // The thickness of the line can be changed with SetPenThickness(), which defaults to 1.
    //
    static bool DrawLine2(POINT p1,SIZE szDist,int iColor);

    //-->  Original Prototype: bool DrawLine2(POINT p1,SIZE szDist,RGBColor_t rgbColor = Rgb::Default);

    // Draw a line in the window.  
    // This draws a line from (ix1,iy1) to (ix2,iy2) in the given color
    //
    // The color can be RGB() or RGBColor_t
    // The thickness of the line can be changed with SetPenThickness(), which defaults to 1.
    //
    static bool DrawLine2(POINT p1,SIZE szDist,RGBColor_t rgbColor = Rgb::Default);

    //-->  Original Prototype: int SetPenThickness(int iThickness);

    // SetPenThickness -- Sets the thickness of the 'pen' (i.e. draw line thickness or outline thickness on Cricles, Triangles, etc.)
    //
    // This value defaults to 1, but can be set prior to drawing any control that uses an outline or draws a line.
    // SetPenThickness() must be used again to revert back to the original thickness.  
    // 
    // SetPenThickness() returns the new thickness of the pen -- this useful to ensure the value does not 
    // go below the minimum of 1.
    //
    static int SetPenThickness(int iThickness);

    //-->  Original Prototype: int GetPenThickness();

    // GetPenThickness() -- Returns the size (i.e. thickness) of the current pen)
    //
    static int GetPenThickness();

    //-->  Original Prototype: int SetPenSize(int iThickness);

    // SetPenSize -- Sets the thickness of the 'pen' (i.e. draw line thickness or outline thickness on Cricles, Triangles, etc.)
    //
    // This value defaults to 1, but can be set prior to drawing any control that uses an outline or draws a line.
    // SetPenThickness() must be used again to revert back to the original thickness.  
    // 
    // SetPenThickness() returns the new thickness of the pen -- this useful to ensure the value does not 
    // go below the minimum of 1.
    //
    static int SetPenSize(int iThickness);

    //-->  Original Prototype: int GetPenSize();

    // GetPenSize() -- Returns the size (i.e. thickness) of the current pen)
    //
    static int GetPenSize();

    //-->  Original Prototype: bool DrawPixel(int iX,int iY,DWORD dwColor);

    // DrawPixel() draw a pixel on the screen. 
    //
    // This sets a pixel on the screen and can be useful in prototyping and general drawing.
    // However, note that DrawPixel() used when drawing an entire array (i.e. a bitmap or an image)
    // is very slow! 
    //
    // For images, bitmaps, collections, etc. it is much faster to build a bitmap in memory
    // and then call DrawBitmap() to display multiple pixels at a time.
    //
    static bool DrawPixel(int iX,int iY,DWORD dwColor);

    //-->  Original Prototype: bool DrawPixel(POINT pPoint,DWORD dwColor);

    // DrawPixel() draw a pixel on the screen. 
    //
    // This sets a pixel on the screen and can be useful in prototyping and general drawing.
    // However, note that DrawPixel() used when drawing an entire array (i.e. a bitmap or an image)
    // is very slow! 
    //
    // For images, bitmaps, collections, etc. it is much faster to build a bitmap in memory
    // and then call DrawBitmap() to display multiple pixels at a time.
    //
    static bool DrawPixel(POINT pPoint,DWORD dwColor);

    //-->  Original Prototype: bool DrawPixel(POINT pPoint,RGBColor_t rgbColor);

    // DrawPixel() draw a pixel on the screen. 
    //
    // This sets a pixel on the screen and can be useful in prototyping and general drawing.
    // However, note that DrawPixel() used when drawing an entire array (i.e. a bitmap or an image)
    // is very slow! 
    //
    // For images, bitmaps, collections, etc. it is much faster to build a bitmap in memory
    // and then call DrawBitmap() to display multiple pixels at a time.
    //
    static bool DrawPixel(POINT pPoint,RGBColor_t rgbColor);

    //-->  Original Prototype: bool DrawPixel(int iX,int iY,RGBColor_t rgbColor);

    // DrawPixel() draw a pixel on the screen. 
    //
    // This sets a pixel on the screen and can be useful in prototyping and general drawing.
    // However, note that DrawPixel() used when drawing an entire array (i.e. a bitmap or an image)
    // is very slow! 
    //
    // For images, bitmaps, collections, etc. it is much faster to build a bitmap in memory
    // and then call DrawBitmap() to display multiple pixels at a time.
    //
    static bool DrawPixel(int iX,int iY,RGBColor_t rgbColor);

    //-->  Original Prototype: bool Show(bool bShow = true);

                                                                     // DrawPixel() draw a piel on the screen. 
    // Show() -- Show the window (i.e. make it visible)
    //
    // If the window is not showing on the screen, Show() will make it visible.
    //
    // Show() and Show(true) will show a hidden window
    // Show(false) will Hide() the window
    //
    static bool Show(bool bShow = true);

    //-->  Original Prototype: bool Hide(bool bHide = true);

    // Hide() -- Hide the window (i.e. remove it from the screen without closing it or destroying it)
    //
    // If the window is on the screen Hide() will remove it from the display but not otherwise affect it.
    // 
    // Hide() and Hide(true) will hide the window
    // Hide(false) will Show() the window
    //
    static bool Hide(bool bHide = true);

    //-->  Original Prototype: CSlider &  NewSlider(CSlider * cSlider,int iX,int iY,int iSize,const wchar_t * sLabel = nullptr,const cwfOpt & cwOpt = cwfOpt());

    // Create a New Slider Control
    //
    // This will create a slider control in the current window at (iX,iY)
    // iSize determines the width or height, depending on whether the slider is horizonal or vertical.
    // Horizontal is the default setting.
    //
    // Options:
    //
    // ---> Horizonal()    -- Sets the sliderb to a horizontal slider (default)
    // ---> Vertical()     -- Sets the slider to a vertical slider ($$ still TBD -- results may vary)
    // ---> ShowValue()    -- When set to true, the slider becomes taller to show the value as the slider is moved.
    //                         Use SetSliderHandler() or override the slider class to control the output if the default values are not appropriate.
    // ---> using sLabel   -- This will make the slider taller to show the label underneath the slider.
    // ---> Name()           -- Set the Name of the control
    // ---> ID()             -- Set the ID of the control
    // ---> TextColor()      -- Set the color of the text on the slider
    // ---> fgColor()        -- Same as TextColor()
    // ---> bgColor()        -- Set the backgound color of the slider()  (Default is either transparent or color of the window)
    // ---> ValueColor()     -- Set the text color of the value output
    // ---> Font()           -- Set the font used for the slider value output
    // ---> ValueFont()      -- Set the font used for the output value
    // ---> WinToolTip()     -- Show the "Windows Tooltip", showing the value of the slider as it is moved.
    // ---> WinColors()      -- Use Windows Colors for the slider (rather then current background and foreground colors)
    // ---> EnableFocusBox() -- Shows the focus box when entering data.  Default is to turn this off.
    // ---> Disabled()       -- Disabled the Slider initially 
    // ---> Hidden()         -- Initially Hides the slider.  Useful for setting parameters before showing the slider.
    // ---> Title()          -- Set the title/label (same as using sLabel when calling NewSlider)
    // ---> Label()          -- Same as Title()
    //
    static CSlider & NewSlider(CSlider * cSlider,int iX,int iY,int iSize,const wchar_t * sLabel = nullptr,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CSlider &  NewSlider(CSlider * cSlider,int iX,int iY,int iSize,const char * sLabel = nullptr,const cwfOpt & cwOpt = cwfOpt());

    // Create a New Slider Control
    //
    // This will create a slider control in the current window at (iX,iY)
    // iSize determines the width or height, depending on whether the slider is horizonal or vertical.
    // Horizontal is the default setting.
    //
    // Options:
    //
    // ---> Horizonal()    -- Sets the sliderb to a horizontal slider (default)
    // ---> Vertical()     -- Sets the slider to a vertical slider ($$ still TBD -- results may vary)
    // ---> ShowValue()    -- When set to true, the slider becomes taller to show the value as the slider is moved.
    //                         Use SetSliderHandler() or override the slider class to control the output if the default values are not appropriate.
    // ---> using sLabel   -- This will make the slider taller to show the label underneath the slider.
    // ---> Name()           -- Set the Name of the control
    // ---> ID()             -- Set the ID of the control
    // ---> TextColor()      -- Set the color of the text on the slider
    // ---> fgColor()        -- Same as TextColor()
    // ---> bgColor()        -- Set the backgound color of the slider()  (Default is either transparent or color of the window)
    // ---> ValueColor()     -- Set the text color of the value output
    // ---> Font()           -- Set the font used for the slider value output
    // ---> ValueFont()      -- Set the font used for the output value
    // ---> WinToolTip()     -- Show the "Windows Tooltip", showing the value of the slider as it is moved.
    // ---> WinColors()      -- Use Windows Colors for the slider (rather then current background and foreground colors)
    // ---> EnableFocusBox() -- Shows the focus box when entering data.  Default is to turn this off.
    // ---> Disabled()       -- Disabled the Slider initially 
    // ---> Hidden()         -- Initially Hides the slider.  Useful for setting parameters before showing the slider.
    // ---> Title()          -- Set the title/label (same as using sLabel when calling NewSlider)
    // ---> Label()          -- Same as Title()
    //
    static CSlider & NewSlider(CSlider * cSlider,int iX,int iY,int iSize,const char * sLabel = nullptr,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CSlider &  NewSlider(CSlider * cSlider,int iX,int iY,int iSize,const cwfOpt & cwOpt = cwfOpt());

    // Create a New Slider Control
    //
    // This will create a slider control in the current window at (iX,iY)
    // iSize determines the width or height, depending on whether the slider is horizonal or vertical.
    // Horizontal is the default setting.
    //
    // Options:
    //
    // ---> Horizonal()    -- Sets the sliderb to a horizontal slider (default)
    // ---> Vertical()     -- Sets the slider to a vertical slider ($$ still TBD -- results may vary)
    // ---> ShowValue()    -- When set to true, the slider becomes taller to show the value as the slider is moved.
    //                         Use SetSliderHandler() or override the slider class to control the output if the default values are not appropriate.
    // ---> using sLabel   -- This will make the slider taller to show the label underneath the slider.
    // ---> Name()           -- Set the Name of the control
    // ---> ID()             -- Set the ID of the control
    // ---> TextColor()      -- Set the color of the text on the slider
    // ---> fgColor()        -- Same as TextColor()
    // ---> bgColor()        -- Set the backgound color of the slider()  (Default is either transparent or color of the window)
    // ---> ValueColor()     -- Set the text color of the value output
    // ---> Font()           -- Set the font used for the slider value output
    // ---> ValueFont()      -- Set the font used for the output value
    // ---> WinToolTip()     -- Show the "Windows Tooltip", showing the value of the slider as it is moved.
    // ---> WinColors()      -- Use Windows Colors for the slider (rather then current background and foreground colors)
    // ---> EnableFocusBox() -- Shows the focus box when entering data.  Default is to turn this off.
    // ---> Disabled()       -- Disabled the Slider initially 
    // ---> Hidden()         -- Initially Hides the slider.  Useful for setting parameters before showing the slider.
    // ---> Title()          -- Set the title/label (same as using sLabel when calling NewSlider)
    // ---> Label()          -- Same as Title()
    //
    static CSlider & NewSlider(CSlider * cSlider,int iX,int iY,int iSize,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CSlider &  NewSlider(int iX,int iY,int iSize,const char * sLabel,const cwfOpt & cwOpt = cwfOpt());

    // Create a New Slider Control
    //
    // This will create a slider control in the current window at (iX,iY)
    // iSize determines the width or height, depending on whether the slider is horizonal or vertical.
    // Horizontal is the default setting.
    //
    // Options:
    //
    // ---> Horizonal()    -- Sets the sliderb to a horizontal slider (default)
    // ---> Vertical()     -- Sets the slider to a vertical slider ($$ still TBD -- results may vary)
    // ---> ShowValue()    -- When set to true, the slider becomes taller to show the value as the slider is moved.
    //                         Use SetSliderHandler() or override the slider class to control the output if the default values are not appropriate.
    // ---> using sLabel   -- This will make the slider taller to show the label underneath the slider.
    // ---> Name()           -- Set the Name of the control
    // ---> ID()             -- Set the ID of the control
    // ---> TextColor()      -- Set the color of the text on the slider
    // ---> fgColor()        -- Same as TextColor()
    // ---> bgColor()        -- Set the backgound color of the slider()  (Default is either transparent or color of the window)
    // ---> ValueColor()     -- Set the text color of the value output
    // ---> Font()           -- Set the font used for the slider value output
    // ---> ValueFont()      -- Set the font used for the output value
    // ---> WinToolTip()     -- Show the "Windows Tooltip", showing the value of the slider as it is moved.
    // ---> WinColors()      -- Use Windows Colors for the slider (rather then current background and foreground colors)
    // ---> EnableFocusBox() -- Shows the focus box when entering data.  Default is to turn this off.
    // ---> Disabled()       -- Disabled the Slider initially 
    // ---> Hidden()         -- Initially Hides the slider.  Useful for setting parameters before showing the slider.
    // ---> Title()          -- Set the title/label (same as using sLabel when calling NewSlider)
    // ---> Label()          -- Same as Title()
    //
    static CSlider & NewSlider(int iX,int iY,int iSize,const char * sLabel,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CSlider &  NewSlider(int iX,int iY,int iSize,const wchar_t * sLabel,const cwfOpt & cwOpt = cwfOpt());

    // Create a New Slider Control
    //
    // This will create a slider control in the current window at (iX,iY)
    // iSize determines the width or height, depending on whether the slider is horizonal or vertical.
    // Horizontal is the default setting.
    //
    // Options:
    //
    // ---> Horizonal()    -- Sets the sliderb to a horizontal slider (default)
    // ---> Vertical()     -- Sets the slider to a vertical slider ($$ still TBD -- results may vary)
    // ---> ShowValue()    -- When set to true, the slider becomes taller to show the value as the slider is moved.
    //                         Use SetSliderHandler() or override the slider class to control the output if the default values are not appropriate.
    // ---> using sLabel   -- This will make the slider taller to show the label underneath the slider.
    // ---> Name()           -- Set the Name of the control
    // ---> ID()             -- Set the ID of the control
    // ---> TextColor()      -- Set the color of the text on the slider
    // ---> fgColor()        -- Same as TextColor()
    // ---> bgColor()        -- Set the backgound color of the slider()  (Default is either transparent or color of the window)
    // ---> ValueColor()     -- Set the text color of the value output
    // ---> Font()           -- Set the font used for the slider value output
    // ---> ValueFont()      -- Set the font used for the output value
    // ---> WinToolTip()     -- Show the "Windows Tooltip", showing the value of the slider as it is moved.
    // ---> WinColors()      -- Use Windows Colors for the slider (rather then current background and foreground colors)
    // ---> EnableFocusBox() -- Shows the focus box when entering data.  Default is to turn this off.
    // ---> Disabled()       -- Disabled the Slider initially 
    // ---> Hidden()         -- Initially Hides the slider.  Useful for setting parameters before showing the slider.
    // ---> Title()          -- Set the title/label (same as using sLabel when calling NewSlider)
    // ---> Label()          -- Same as Title()
    //
    static CSlider & NewSlider(int iX,int iY,int iSize,const wchar_t * sLabel,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CSlider &  NewSlider(int iX,int iY,int iSize,const cwfOpt & cwOpt = cwfOpt());

    // Create a New Slider Control
    //
    // This will create a slider control in the current window at (iX,iY)
    // iSize determines the width or height, depending on whether the slider is horizonal or vertical.
    // Horizontal is the default setting.
    //
    // Options:
    //
    // ---> Horizonal()    -- Sets the sliderb to a horizontal slider (default)
    // ---> Vertical()     -- Sets the slider to a vertical slider ($$ still TBD -- results may vary)
    // ---> ShowValue()    -- When set to true, the slider becomes taller to show the value as the slider is moved.
    //                         Use SetSliderHandler() or override the slider class to control the output if the default values are not appropriate.
    // ---> using sLabel   -- This will make the slider taller to show the label underneath the slider.
    // ---> Name()           -- Set the Name of the control
    // ---> ID()             -- Set the ID of the control
    // ---> TextColor()      -- Set the color of the text on the slider
    // ---> fgColor()        -- Same as TextColor()
    // ---> bgColor()        -- Set the backgound color of the slider()  (Default is either transparent or color of the window)
    // ---> ValueColor()     -- Set the text color of the value output
    // ---> Font()           -- Set the font used for the slider value and title text
    // ---> WinToolTip()     -- Show the "Windows Tooltip", showing the value of the slider as it is moved.
    // ---> WinColors()      -- Use Windows Colors for the slider (rather then current background and foreground colors)
    // ---> EnableFocusBox() -- Shows the focus box when entering data.  Default is to turn this off.
    // ---> Disabled()       -- Disabled the Slider initially 
    // ---> Hidden()         -- Initially Hides the slider.  Useful for setting parameters before showing the slider.
    // ---> Title()          -- Set the title/label (same as using sLabel when calling NewSlider)
    // ---> Label()          -- Same as Title()
    //
    static CSlider & NewSlider(int iX,int iY,int iSize,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: void QuickButton(const char * sText = nullptr,const char * sTitleBar = nullptr);

    // QuickButton() -- Put up a quick button and information line. 
    //
    // This is used for quick Input-and-Go buttons, to stop program execution and get a quick button to continue.
    // QuickButton() simply puts up a "press to continue" message with an Ok button.
    //
    // The text can be is changed by setting sText.
    // The ProgramName (if it is set) is set in the title bar, unless sTitleBar is set other text
    //
    // QuickButton() is similar to dialog.Info(), but uses a smaller font.
    // 
    // Multiple lines may be entered with '\n', and long lines are automatically broken into multiple lines
    //
    static void QuickButton(const char * sText = nullptr,const char * sTitleBar = nullptr);

    //-->  Original Prototype: BkMode GetWinBkMode();

    // GetWinBkMode() -- Get the text background mode.
    // BkMode returns eeither BkMode::Transparent or BkMode::Opaque
    //
    // BkMode::Transparent()  --- This is the default.  When text is written to the screen, only the text part is written in the
    //                            foreground color, leaving the blank areas (i.e. text or graphics behind the new text) untouched.
    //
    // BkMode::Opaque         -- When text is written to the window, this will write the text in the foreground color and fill in the background with the
    //                           background color overwriting any text or graphics behind the new text.
    //
    static BkMode GetWinBkMode();

    //-->  Original Prototype: bool SetWinBkMode(BkMode eBkMode);

    // SetWinBkMode() -- Set the text background mode.
    // BkMode returns eeither BkMode::Transparent or BkMode::Opaque
    //
    // BkMode::Transparent()  --- This is the default.  When text is written to the screen, only the text part is written in the
    //                            foreground color, leaving the blank areas (i.e. text or graphics behind the new text) untouched.
    //
    // BkMode::Opaque         -- When text is written to the window, this will write the text in the foreground color and fill in the background with the
    //                           background color overwriting any text or graphics behind the new text.
    //
    static bool SetWinBkMode(BkMode eBkMode);

    //-->  Original Prototype: bool SetBgColor(const char * sColor);

    // Set Background/Text color for text output routines such as Write(), out, printf, putchar, etc.
    //
    // SetBgColor can take the following forms:
    //
    // ---> SetBgColor("Yellow");       // Set a known color via text
    // ---> SetBgColor("MyColor");      // Set a color defined by using MakeColor (using its text name)
    // ---> SetBgColor(MyColor);        // Set a color using a variable MyColor, defined by the return value of MakeColor (i.e. auto MyColor = MakeColor("MyColor",{ 255,0,0 }");
    // ---> SetBgColor({ 255,0,0});     // Set a color using an RGB value (in this case, RED = {255,0,0}, for { red, green, blue }
    // ---> SetBgColor(dwRGBValue);     // Set the background color with a Windows RGB type color, defined by an int or DWORD value, or the macro RGB()
    // ---> SetBgClor(RGB(255,0,0)l     // This is the same as the previous example, but using the Windows RGB macro to create a DWORD based RGB value
    //   
    static bool SetBgColor(const char * sColor);

    //-->  Original Prototype: bool SetBgColor(DWORD dwColor);

    // Set Background/Text color for text output routines such as Write(), out, printf, putchar, etc.
    //
    // SetBgColor can take the following forms:
    //
    // ---> SetBgColor("Yellow");       // Set a known color via text
    // ---> SetBgColor("MyColor");      // Set a color defined by using MakeColor (using its text name)
    // ---> SetBgColor(MyColor);        // Set a color using a variable MyColor, defined by the return value of MakeColor (i.e. auto MyColor = MakeColor("MyColor",{ 255,0,0 }");
    // ---> SetBgColor({ 255,0,0});     // Set a color using an RGB value (in this case, RED = {255,0,0}, for { red, green, blue }
    // ---> SetBgColor(dwRGBValue);     // Set the background color with a Windows RGB type color, defined by an int or DWORD value, or the macro RGB()
    // ---> SetBgClor(RGB(255,0,0)l     // This is the same as the previous example, but using the Windows RGB macro to create a DWORD based RGB value
    //   
    static bool SetBgColor(DWORD dwColor);

    //-->  Original Prototype: bool SetBgColor(RGBColor_t rgbColor);

    // Set Background/Text color for text output routines such as Write(), out, printf, putchar, etc.
    //
    // SetBgColor can take the following forms:
    //
    // ---> SetBgColor("Yellow");       // Set a known color via text
    // ---> SetBgColor("MyColor");      // Set a color defined by using MakeColor (using its text name)
    // ---> SetBgColor(MyColor);        // Set a color using a variable MyColor, defined by the return value of MakeColor (i.e. auto MyColor = MakeColor("MyColor",{ 255,0,0 }");
    // ---> SetBgColor({ 255,0,0});     // Set a color using an RGB value (in this case, RED = {255,0,0}, for { red, green, blue }
    // ---> SetBgColor(dwRGBValue);     // Set the background color with a Windows RGB type color, defined by an int or DWORD value, or the macro RGB()
    // ---> SetBgClor(RGB(255,0,0)l     // This is the same as the previous example, but using the Windows RGB macro to create a DWORD based RGB value
    //   
    static bool SetBgColor(RGBColor_t rgbColor);

    //-->  Original Prototype: bool SetFgColor(const char * sColor);

    // Set Foreground/Text color for text output routines such as Write(), out, printf, putchar, etc.
    //
    // SetFgColor can take the following forms:
    //
    // ---> SetFgColor("Yellow");    // Set a known color via text
    // ---> SetFgColor("MyColor");    // Set a color defined by using MakeColor (using its text name)
    // ---> SetFgColor(MyColor);    // Set a color using a variable MyColor, defined by the return value of MakeColor (i.e. auto MyColor = MakeColor("MyColor",{ 255,0,0 }");
    // ---> SetFgColor({ 255,0,0});        // Set a color using an RGB value (in this case, RED = {255,0,0}, for { red, green, blue }
    // ---> SetFgColor(dwRGBValue);    // Set the foreground color with a Windows RGB type color, defined by an int or DWORD value, or the macro RGB()
    // ---> SetFgClor(RGB(255,0,0)l    // This is the same as the previous example, but using the Windows RGB macro to create a DWORD based RGB value
    //
    static bool SetFgColor(const char * sColor);

    //-->  Original Prototype: bool SetFgColor(DWORD dwColor);

    // Set Foreground/Text color for text output routines such as Write(), out, printf, putchar, etc.
    //
    // SetFgColor can take the following forms:
    //
    // ---> SetFgColor("Yellow");    // Set a known color via text
    // ---> SetFgColor("MyColor");    // Set a color defined by using MakeColor (using its text name)
    // ---> SetFgColor(MyColor);    // Set a color using a variable MyColor, defined by the return value of MakeColor (i.e. auto MyColor = MakeColor("MyColor",{ 255,0,0 }");
    // ---> SetFgColor({ 255,0,0});        // Set a color using an RGB value (in this case, RED = {255,0,0}, for { red, green, blue }
    // ---> SetFgColor(dwRGBValue);    // Set the foreground color with a Windows RGB type color, defined by an int or DWORD value, or the macro RGB()
    // ---> SetFgClor(RGB(255,0,0)l    // This is the same as the previous example, but using the Windows RGB macro to create a DWORD based RGB value
    //
    static bool SetFgColor(DWORD dwColor);

    //-->  Original Prototype: bool SetFgColor(RGBColor_t rgbColor);

    // Set Foreground/Text color for text output routines such as Write(), out, printf, putchar, etc.
    //
    // SetFgColor can take the following forms:
    //
    // ---> SetFgColor("Yellow");    // Set a known color via text
    // ---> SetFgColor("MyColor");    // Set a color defined by using MakeColor (using its text name)
    // ---> SetFgColor(MyColor);    // Set a color using a variable MyColor, defined by the return value of MakeColor (i.e. auto MyColor = MakeColor("MyColor",{ 255,0,0 }");
    // ---> SetFgColor({ 255,0,0});        // Set a color using an RGB value (in this case, RED = {255,0,0}, for { red, green, blue }
    // ---> SetFgColor(dwRGBValue);    // Set the foreground color with a Windows RGB type color, defined by an int or DWORD value, or the macro RGB()
    // ---> SetFgClor(RGB(255,0,0)l    // This is the same as the previous example, but using the Windows RGB macro to create a DWORD based RGB value
    //
    static bool SetFgColor(RGBColor_t rgbColor);

    //-->  Original Prototype: CButton & button(const char * sButtonName);

    // Return a Button & for a button with a Name or an ID. 
    //
    // For example, for this button: NewButton(10,20,"Ok",ID(1))
    // auto& MyButton = button(1) will retrieve the button.
    //
    // Similarly, for this button: NewButton(10,20,"Ok","RobsButton");
    // auto& MyButton = button("RobsButton") will retrieve the button also.
    //
    // Quick functions, though inefficient, can be called using this function without saving the results.
    // For example, button(1).Pressed() calls the Pressed() function of button with ID = 1
    //
    // If the button does not exist (i.e. the ID is incorrect or Name misspelled), an empty button
    // is return and all functions will passively fail (i.e. they won't crash but won't do anything).
    //
    static CButton & button(const char * sButtonName);

    //-->  Original Prototype: CButton & button(int iButtonID);

    // Return a Button & for a button with a Name or an ID. 
    //
    // For example, for this button: NewButton(10,20,"Ok",ID(1))
    // auto& MyButton = button(1) will retrieve the button.
    //
    // Similarly, for this button: NewButton(10,20,"Ok",Name("RobsButton"));
    // auto& MyButton = button("RobsButton") will retrieve the button also.
    //
    // Quick functions, though inefficient, can be called using this function without saving the results.
    // For example, button(1).Pressed() calls the Pressed() function of button with ID = 1
    //
    // If the button does not exist (i.e. the ID is incorrect or Name misspelled), an empty button
    // is return and all functions will passively fail (i.e. they won't crash but won't do anything).
    //
    static CButton & button(int iButtonID);

    //-->  Original Prototype: CEditBox & editbox(const char * sEditboxName);

    // Return an EditBox & for an EditBox with a Name or an ID. 
    //
    // For example, for this EditBox: NewEditBox(10,20,300,ID(1))
    // auto& MyEditBox = editbox(1) will retrieve the editbox.
    //
    // Similarly, for this editbox: NewEditBox(10,20,300,Name("RobsEditBox"))
    // auto& MyEditBox = editbox("RobsEditBox") will retrieve the editbox also.
    //
    // Quick functions, though inefficient, can be called using this function without saving the results.
    // For example, editbox(1).CRPressed() calls the CRPressed() function of editbox with ID = 1
    //
    // If the editbox does not exist (i.e. the ID is incorrect or Name misspelled), an empty editbox
    // is return and all functions will passively fail (i.e. they won't crash but won't do anything).
    //
    static CEditBox & editbox(const char * sEditboxName);

    //-->  Original Prototype: CInputBox & inputbox(const char * sInputboxName) { return editbox(sInputboxName);

    static CInputBox & inputbox(const char * sInputboxName);

    //-->  Original Prototype: CEditBox & editbox(int iEditboxID);

    // Return an EditBox & for an EditBox with a Name or an ID. 
    //
    // For example, for this EditBox: NewEditBox(10,20,300,ID(1))
    // auto& MyEditBox = editbox(1) will retrieve the editbox.
    //
    // Similarly, for this editbox: NewEditBox(10,20,300,Name("RobsEditBox"))
    // auto& MyEditBox = editbox("RobsEditBox") will retrieve the editbox also.
    //
    // Quick functions, though inefficient, can be called using this function without saving the results.
    // For example, editbox(1).CRPressed() calls the CRPressed() function of editbox with ID = 1
    //
    // If the editbox does not exist (i.e. the ID is incorrect or Name misspelled), an empty editbox
    // is return and all functions will passively fail (i.e. they won't crash but won't do anything).
    //
    static CEditBox & editbox(int iEditboxID);

    //-->  Original Prototype: CInputBox & inputbox(int iInputBoxID) { return editbox(iInputBoxID);

    static CInputBox & inputbox(int iInputBoxID);

    //-->  Original Prototype: CSlider & slider(const char * sSliderName);

    // Return a Slider & for a Slider with a Name or an ID. 
    //
    // For example, for this Slider: NewSlider(10,20,200,ID(1))
    // auto& MySlider = slider(1) will retrieve the Slider.
    //
    // Similarly, for this Slider: NewSlider(10,20,200,name("RobsSlider"))
    // auto& MySlider = slider("RobsSlider") will retrieve the slider also.
    //
    // Quick functions, though inefficient, can be called using this function without saving the results.
    // For example, slider(1).Moved() calls the Moved() function of slider with ID = 1
    //
    // If the slider does not exist (i.e. the ID is incorrect or Name misspelled), an empty slider
    // is return and all functions will passively fail (i.e. they won't crash but won't do anything).
    //
    static CSlider & slider(const char * sSliderName);

    //-->  Original Prototype: CSlider & slider(int iSliderID);

    // Return a Slider & for a Slider with a Name or an ID. 
    //
    // For example, for this Slider: NewSlider(10,20,200,ID(1))
    // auto& MySlider = slider(1) will retrieve the Slider.
    //
    // Similarly, for this Slider: NewSlider(10,20,200,name("RobsSlider"))
    // auto& MySlider = slider("RobsSlider") will retrieve the slider also.
    //
    // Quick functions, though inefficient, can be called using this function without saving the results.
    // For example, slider(1).Moved() calls the Moved() function of slider with ID = 1
    //
    // If the slider does not exist (i.e. the ID is incorrect or Name misspelled), an empty slider
    // is return and all functions will passively fail (i.e. they won't crash but won't do anything).
    //
    static CSlider & slider(int iSliderID);

    //-->  Original Prototype: CListBox & listbox(const char * sListBoxName);

    // Return a ListBox & for a ListBox with a Name or an ID. 
    //
    // For example, for this ListBox: NewListBox(10,20,200,400,ID(1))
    // auto& ListBox = listbox(1) will retrieve the listbox.
    //
    // Similarly, for this Slider: NewListBox(10,20,200,name("RobsListBox"))
    // auto& MyListBox = ListBox("RobsListBox") will retrieve the listbox also.
    //
    // Quick functions, though inefficient, can be called using this function without saving the results.
    // For example, listbox(1).ItemSelected() calls the ItemSelected() function of listbox with ID = 1
    //
    // If the listbox does not exist (i.e. the ID is incorrect or Name misspelled), an empty listbox
    // is return and all functions will passively fail (i.e. they won't crash but won't do anything).
    //
    static CListBox & listbox(const char * sListBoxName);

    //-->  Original Prototype: CListBox & listbox(int iListBoxID);

    // Return a ListBox & for a ListBox with a Name or an ID. 
    //
    // For example, for this ListBox: NewListBox(10,20,200,400,ID(1))
    // auto& ListBox = listbox(1) will retrieve the listbox.
    //
    // Similarly, for this Slider: NewListBox(10,20,200,name("RobsListBox"))
    // auto& MyListBox = ListBox("RobsListBox") will retrieve the listbox also.
    //
    // Quick functions, though inefficient, can be called using this function without saving the results.
    // For example, listbox(1).ItemSelected() calls the ItemSelected() function of listbox with ID = 1
    //
    // If the listbox does not exist (i.e. the ID is incorrect or Name misspelled), an empty listbox
    // is return and all functions will passively fail (i.e. they won't crash but won't do anything).
    //
    static CListBox & listbox(int iListBoxID);

    //-->  Original Prototype: CComboBox & combobox(int iComboBoxID);

    static CComboBox & combobox(int iComboBoxID);

    //-->  Original Prototype: CComboBox & combobox(const char * sComboBoxName);

    static CComboBox & combobox(const char * sComboBoxName);

    //-->  Original Prototype: CWindow & window(const char * sWindowName);

  // Return a CWindow & for a Window with a Name or an ID. 
    //
    // For example, for this Window: NewWindow(10,20,800,600,"MyWindow,ID(1))
    // auto& MyWindow = window(1) will retrieve the Window.
    //
    // Similarly, for this Window: NewWindow(10,20,800,600,"MyWindow,Name("RobsWindow"))
    // auto& MyWindow = window("RobsWidow") will retrieve the window also.
    //
    // Quick functions, though inefficient, can be called using this function without saving the results.
    // For example, window(1).MouseMoved() calls the MouseMoved() function of window with ID = 1
    //
    // If the button does not exist (i.e. the ID is incorrect or Name misspelled), an empty button
    // is return and all functions will passively fail (i.e. they won't crash but won't do anything).
    //
    static CWindow & window(const char * sWindowName);

    //-->  Original Prototype: CWindow & window(int iWindowID);

    // Return a CWindow & for a Window with a Name or an ID. 
    //
    // For example, for this Window: NewWindow(10,20,800,600,"MyWindow,ID(1))
    // auto& MyWindow = window(1) will retrieve the Window.
    //
    // Similarly, for this Window: NewWindow(10,20,800,600,"MyWindow,Name("RobsWindow"))
    // auto& MyWindow = window("RobsWidow") will retrieve the window also.
    //
    // Quick functions, though inefficient, can be called using this function without saving the results.
    // For example, window(1).MouseMoved() calls the MouseMoved() function of window with ID = 1
    //
    // If the button does not exist (i.e. the ID is incorrect or Name misspelled), an empty button
    // is return and all functions will passively fail (i.e. they won't crash but won't do anything).
    //
    static CWindow & window(int iWindowID);

    //-->  Original Prototype: bool GetWindowColors(DWORD & dwFgColor,DWORD & dwBgColor);

    // Returns the default Windows window colors.  SageBox has its own defaults for colors, and this
    // returns the colors Windows sets windows to by default.
    //
    static bool GetWindowColors(DWORD & dwFgColor,DWORD & dwBgColor);

    //-->  Original Prototype: bool GetWindowColors(RGBColor_t & rgbFgColor,RGBColor_t & rgbBgColor);

    // Returns the default Windows window colors.  SageBox has its own defaults for colors, and this
    // returns the colors Windows sets windows to by default.
    //
    static bool GetWindowColors(RGBColor_t & rgbFgColor,RGBColor_t & rgbBgColor);

    //-->  Original Prototype: SIZE GetDesktopSize();

    // Get the size of the desktop (i.e. monitor). 
    // This returns the current monitor size (i.e. 1920x1080).  This can help in centering and otherwize
    // placing windows and controls.
    //
    // Currently, this returns the active desktop and will be updated to work with systems with multiple monitors in a future update.
    //
    static SIZE GetDesktopSize();

    //-->  Original Prototype: bool DisplayBitmap(int iX,int iY,RawBitmap32_t & stBitmap);

    // Display a Bitmap on the window.
    // 
    // ** Note: ** -- this displayed aligned bitmaps, where each row must be divisible by 4, in which 
    // case some rows must be padded. 
    //
    // DisplayBitmap() shows a bitmap on the window at the specified (iX,iY) coordinates on the screen.
    // Raw data can be used, in which case the width, height, and memory pointer must also be supplied.
    // RawBitmap_t and CBitmap can also be used, in which case only the iX, and iY parameters are necessary.
    //
    // Note: Negate the height to display the bitmap upside-down.  In the case of RawBitmap_t or CBitmap, put
    // a '-' sign in front of the bitmap structure.  DisplayBitmapR() can also be used.
    //
    // In the case of RawBitmap_t and CBitmap, bad or corrupted bitmaps are not displayed and passed through with 
    // a false return. 
    //
    static bool DisplayBitmap(int iX,int iY,RawBitmap32_t & stBitmap);

    //-->  Original Prototype: bool DisplayBitmap(POINT pLoc,RawBitmap32_t & stBitmap);

    // Display a Bitmap on the window.
    // 
    // ** Note: ** -- this displayed aligned bitmaps, where each row must be divisible by 4, in which 
    // case some rows must be padded. 
    //
    // DisplayBitmap() shows a bitmap on the window at the specified (iX,iY) coordinates on the screen.
    // Raw data can be used, in which case the width, height, and memory pointer must also be supplied.
    // RawBitmap_t and CBitmap can also be used, in which case only the iX, and iY parameters are necessary.
    //
    // Note: Negate the height to display the bitmap upside-down.  In the case of RawBitmap_t or CBitmap, put
    // a '-' sign in front of the bitmap structure.  DisplayBitmapR() can also be used.
    //
    // In the case of RawBitmap_t and CBitmap, bad or corrupted bitmaps are not displayed and passed through with 
    // a false return. 
    //
    static bool DisplayBitmap(POINT pLoc,RawBitmap32_t & stBitmap);

    //-->  Original Prototype: bool DisplayBitmap(RawBitmap32_t & stBitmap);

    // Display a Bitmap on the window.
    // 
    // ** Note: ** -- this displayed aligned bitmaps, where each row must be divisible by 4, in which 
    // case some rows must be padded. 
    //
    // DisplayBitmap() shows a bitmap on the window at the specified (iX,iY) coordinates on the screen.
    // Raw data can be used, in which case the width, height, and memory pointer must also be supplied.
    // RawBitmap_t and CBitmap can also be used, in which case only the iX, and iY parameters are necessary.
    //
    // Note: Negate the height to display the bitmap upside-down.  In the case of RawBitmap_t or CBitmap, put
    // a '-' sign in front of the bitmap structure.  DisplayBitmapR() can also be used.
    //
    // In the case of RawBitmap_t and CBitmap, bad or corrupted bitmaps are not displayed and passed through with 
    // a false return. 
    //
    static bool DisplayBitmap(RawBitmap32_t & stBitmap);

    //-->  Original Prototype: bool DisplayBitmap(int iX,int iY,int iWidth,int iHeight,unsigned char * sMemory);

    // Display a Bitmap on the window.
    // 
    // ** Note: ** -- this displayed aligned bitmaps, where each row must be divisible by 4, in which 
    // case some rows must be padded. 
    //
    // DisplayBitmap() shows a bitmap on the window at the specified (iX,iY) coordinates on the screen.
    // Raw data can be used, in which case the width, height, and memory pointer must also be supplied.
    // RawBitmap_t and CBitmap can also be used, in which case only the iX, and iY parameters are necessary.
    //
    // Note: Negate the height to display the bitmap upside-down.  In the case of RawBitmap_t or CBitmap, put
    // a '-' sign in front of the bitmap structure.  DisplayBitmapR() can also be used.
    //
    // In the case of RawBitmap_t and CBitmap, bad or corrupted bitmaps are not displayed and passed through with 
    // a false return. 
    //
    static bool DisplayBitmap(int iX,int iY,int iWidth,int iHeight,unsigned char * sMemory);

    //-->  Original Prototype: bool DisplayBitmap(POINT pLoc,int iWidth,int iHeight,unsigned char * sMemory);

    // Display a Bitmap on the window.
    // 
    // ** Note: ** -- this displayed aligned bitmaps, where each row must be divisible by 4, in which 
    // case some rows must be padded. 
    //
    // DisplayBitmap() shows a bitmap on the window at the specified (iX,iY) coordinates on the screen.
    // Raw data can be used, in which case the width, height, and memory pointer must also be supplied.
    // RawBitmap_t and CBitmap can also be used, in which case only the iX, and iY parameters are necessary.
    //
    // Note: Negate the height to display the bitmap upside-down.  In the case of RawBitmap_t or CBitmap, put
    // a '-' sign in front of the bitmap structure.  DisplayBitmapR() can also be used.
    //
    // In the case of RawBitmap_t and CBitmap, bad or corrupted bitmaps are not displayed and passed through with 
    // a false return. 
    //
    static bool DisplayBitmap(POINT pLoc,int iWidth,int iHeight,unsigned char * sMemory);

    //-->  Original Prototype: bool DisplayBitmap(int iX,int iY,RawBitmap_t & stBitmap);

    // Display a Bitmap on the window.
    // 
    // ** Note: ** -- this displayed aligned bitmaps, where each row must be divisible by 4, in which 
    // case some rows must be padded. 
    //
    // DisplayBitmap() shows a bitmap on the window at the specified (iX,iY) coordinates on the screen.
    // Raw data can be used, in which case the width, height, and memory pointer must also be supplied.
    // RawBitmap_t and CBitmap can also be used, in which case only the iX, and iY parameters are necessary.
    //
    // Note: Negate the height to display the bitmap upside-down.  In the case of RawBitmap_t or CBitmap, put
    // a '-' sign in front of the bitmap structure.  DisplayBitmapR() can also be used.
    //
    // In the case of RawBitmap_t and CBitmap, bad or corrupted bitmaps are not displayed and passed through with 
    // a false return. 
    //
    static bool DisplayBitmap(int iX,int iY,RawBitmap_t & stBitmap);

    //-->  Original Prototype: bool DisplayBitmap(RawBitmap_t & stBitmap);

    // Display a Bitmap on the window.
    // 
    // ** Note: ** -- this displayed aligned bitmaps, where each row must be divisible by 4, in which 
    // case some rows must be padded. 
    //
    // DisplayBitmap() shows a bitmap on the window at the specified (iX,iY) coordinates on the screen.
    // Raw data can be used, in which case the width, height, and memory pointer must also be supplied.
    // RawBitmap_t and CBitmap can also be used, in which case only the iX, and iY parameters are necessary.
    //
    // Note: Negate the height to display the bitmap upside-down.  In the case of RawBitmap_t or CBitmap, put
    // a '-' sign in front of the bitmap structure.  DisplayBitmapR() can also be used.
    //
    // In the case of RawBitmap_t and CBitmap, bad or corrupted bitmaps are not displayed and passed through with 
    // a false return. 
    //
    static bool DisplayBitmap(RawBitmap_t & stBitmap);

    //-->  Original Prototype: bool DisplayBitmapR(int iX,int iY,int iWidth,int iHeight,unsigned char * sMemory);

    // Display a bitmap upside-down.  This is often useful for bitmaps, as they are typically stored upside-down, and default
    // Windows behavior is to correct this.
    //
    // However, it is common to work with correct bitmaps that will then display upside down. 
    // DisplayBitmapR() will display a bitmap right-side up.  You can also negate the height or 
    // struture (by putting a '-' in front of it) to use the regular DisplayBitmap()
    //
    // See DisplayBitmap() for more details on this function.
    //
    static bool DisplayBitmapR(int iX,int iY,int iWidth,int iHeight,unsigned char * sMemory);

    //-->  Original Prototype: bool DisplayBitmapR(POINT pLoc,int iWidth,int iHeight,unsigned char * sMemory);

    // Display a bitmap upside-down.  This is often useful for bitmaps, as they are typically stored upside-down, and default
    // Windows behavior is to correct this.
    //
    // However, it is common to work with correct bitmaps that will then display upside down. 
    // DisplayBitmapR() will display a bitmap right-side up.  You can also negate the height or 
    // struture (by putting a '-' in front of it) to use the regular DisplayBitmap()
    //
    // See DisplayBitmap() for more details on this function.
    //
    static bool DisplayBitmapR(POINT pLoc,int iWidth,int iHeight,unsigned char * sMemory);

    //-->  Original Prototype: bool DisplayBitmapR(int iX,int iY,RawBitmap_t & stBitmap);

    // Display a bitmap upside-down.  This is often useful for bitmaps, as they are typically stored upside-down, and default
    // Windows behavior is to correct this.
    //
    // However, it is common to work with correct bitmaps that will then display upside down. 
    // DisplayBitmapR() will display a bitmap right-side up.  You can also negate the height or 
    // struture (by putting a '-' in front of it) to use the regular DisplayBitmap()
    //
    // See DisplayBitmap() for more details on this function.
    //
    static bool DisplayBitmapR(int iX,int iY,RawBitmap_t & stBitmap);

    //-->  Original Prototype: bool DisplayBitmapR(POINT pLoc,RawBitmap_t & stBitmap);

    // Display a bitmap upside-down.  This is often useful for bitmaps, as they are typically stored upside-down, and default
    // Windows behavior is to correct this.
    //
    // However, it is common to work with correct bitmaps that will then display upside down. 
    // DisplayBitmapR() will display a bitmap right-side up.  You can also negate the height or 
    // struture (by putting a '-' in front of it) to use the regular DisplayBitmap()
    //
    // See DisplayBitmap() for more details on this function.
    //
    static bool DisplayBitmapR(POINT pLoc,RawBitmap_t & stBitmap);

    //-->  Original Prototype: bool DisplayBitmapR(RawBitmap_t & stBitmap);

    // Display a bitmap upside-down.  This is often useful for bitmaps, as they are typically stored upside-down, and default
    // Windows behavior is to correct this.
    //
    // However, it is common to work with correct bitmaps that will then display upside down. 
    // DisplayBitmapR() will display a bitmap right-side up.  You can also negate the height or 
    // struture (by putting a '-' in front of it) to use the regular DisplayBitmap()
    //
    // See DisplayBitmap() for more details on this function.
    //
    static bool DisplayBitmapR(RawBitmap_t & stBitmap);

    //-->  Original Prototype: bool DisplayBitmap32(int iX,int iY,int iWidth,int iHeight,unsigned char * sMemory);

    //Display a 32-bit bitmap. 
    //
    // This function is the same as DisplayBitmap(), except that it displays a 32-bit bitmap. 
    //
    // iX,iY must be specified for the location of the displayed bitmap.
    // For raw data, Width,Height, and memory pointer must also be displayed.
    //
    // See DisplayBitmap() for more information
    //
    static bool DisplayBitmap32(int iX,int iY,int iWidth,int iHeight,unsigned char * sMemory);

    //-->  Original Prototype: bool DisplayBitmap32(POINT pLoc,int iWidth,int iHeight,unsigned char * sMemory);

    //Display a 32-bit bitmap. 
    //
    // This function is the same as DisplayBitmap(), except that it displays a 32-bit bitmap. 
    //
    // iX,iY must be specified for the location of the displayed bitmap.
    // For raw data, Width,Height, and memory pointer must also be displayed.
    //
    // See DisplayBitmap() for more information
    //
    static bool DisplayBitmap32(POINT pLoc,int iWidth,int iHeight,unsigned char * sMemory);

    //-->  Original Prototype: bool DisplayBitmap32(int iX,int iY,RawBitmap32_t & stBitmap);

    //Display a 32-bit bitmap. 
    //
    // This function is the same as DisplayBitmap(), except that it displays a 32-bit bitmap. 
    //
    // iX,iY must be specified for the location of the displayed bitmap.
    // For raw data, Width,Height, and memory pointer must also be displayed.
    //
    // See DisplayBitmap() for more information
    //
    static bool DisplayBitmap32(int iX,int iY,RawBitmap32_t & stBitmap);

    //-->  Original Prototype: bool DisplayBitmap32(POINT pLoc,RawBitmap32_t & stBitmap);

    //Display a 32-bit bitmap. 
    //
    // This function is the same as DisplayBitmap(), except that it displays a 32-bit bitmap. 
    //
    // iX,iY must be specified for the location of the displayed bitmap.
    // For raw data, Width,Height, and memory pointer must also be displayed.
    //
    // See DisplayBitmap() for more information
    //
    static bool DisplayBitmap32(POINT pLoc,RawBitmap32_t & stBitmap);

    //-->  Original Prototype: bool DisplayBitmap32(RawBitmap32_t & stBitmap);

    //Display a 32-bit bitmap. 
    //
    // This function is the same as DisplayBitmap(), except that it displays a 32-bit bitmap. 
    //
    // iX,iY must be specified for the location of the displayed bitmap.
    // For raw data, Width,Height, and memory pointer must also be displayed.
    //
    // See DisplayBitmap() for more information
    //
    static bool DisplayBitmap32(RawBitmap32_t & stBitmap);

    //-->  Original Prototype: bool BlendBitmap(int iX,int iY,RawBitmap_t & stBitmap);

    // Blend a bitmap with a pre-defined mask
    //
    static bool BlendBitmap(int iX,int iY,RawBitmap_t & stBitmap);

    //-->  Original Prototype: bool BlendBitmap(POINT pLoc,RawBitmap_t & stBitmap);

    // Blend a bitmap with a pre-defined mask
    //
    static bool BlendBitmap(POINT pLoc,RawBitmap_t & stBitmap);

    //-->  Original Prototype: bool BlendBitmap(int iX,int iY,RawBitmap_t & stBitmap,RawBitmap_t & stMask);

    // Blend a bitmap with a pre-defined mask
    //
    static bool BlendBitmap(int iX,int iY,RawBitmap_t & stBitmap,RawBitmap_t & stMask);

    //-->  Original Prototype: bool BlendBitmap(POINT pLoc,RawBitmap_t & stBitmap,RawBitmap_t & stMask);

    // Blend a bitmap with a pre-defined mask
    //
    static bool BlendBitmap(POINT pLoc,RawBitmap_t & stBitmap,RawBitmap_t & stMask);

    //-->  Original Prototype: bool StretchBitmap(unsigned char * sMemory,POINT pDest,SIZE szDest,POINT pSrc, SIZE szSource,SIZE szSourceBitmap = {0,0});

    // StretchBitmap() -- Display a stretched bitmap to the window.  
    //
    // The source bitmap can be stretched to any size, and from any portion of the bitmap.
    // 
    // Important Note: If the szSource (source rectangle from which to draw from the original bitmap) is different from 
    // the size of the bitmap, szSourceBitmap MUST BE INCLUDED with the size of the entire source bitmap.  Otherwise the function
    // will fail.  When specifyin a CBitmap or RawBitmap_t as the source, szSourceBitmap does not need to be filled.
    // szSourceBitmap only needs to be filled when sending raw bitmap data and the source size differs from the source bitmap size.
    //
    static bool StretchBitmap(unsigned char * sMemory,POINT pDest,SIZE szDest,POINT pSrc,SIZE szSource,SIZE szSourceBitmap = {0,0});

    //-->  Original Prototype: bool StretchBitmap(CBitmap & cBitmap,POINT pDest,SIZE szDest);

    static bool StretchBitmap(CBitmap & cBitmap,POINT pDest,SIZE szDest);

    //-->  Original Prototype: bool StretchBitmap(CBitmap & cBitmap,POINT pDest,SIZE szDest,POINT pSrc, SIZE szSource);

    static bool StretchBitmap(CBitmap & cBitmap,POINT pDest,SIZE szDest,POINT pSrc,SIZE szSource);

    //-->  Original Prototype: bool StretchBitmapR(unsigned char * sMemory,POINT pDest,SIZE szDest,POINT pSrc, SIZE szSource,SIZE szSourceBitmap = {0,0});

    static bool StretchBitmapR(unsigned char * sMemory,POINT pDest,SIZE szDest,POINT pSrc,SIZE szSource,SIZE szSourceBitmap = {0,0});

    //-->  Original Prototype: bool StretchBitmapR(CBitmap & cBitmap,POINT pDest,SIZE szDest);

    static bool StretchBitmapR(CBitmap & cBitmap,POINT pDest,SIZE szDest);

    //-->  Original Prototype: bool StretchBitmapR(CBitmap & cBitmap,POINT pDest,SIZE szDest,POINT pSrc, SIZE szSource);

    static bool StretchBitmapR(CBitmap & cBitmap,POINT pDest,SIZE szDest,POINT pSrc,SIZE szSource);

    //-->  Original Prototype: bool DisplayBitmapEx(unsigned char * sMemory,POINT pDest,POINT pSrc, SIZE szSize,SIZE szSourceBitmap);

    static bool DisplayBitmapEx(unsigned char * sMemory,POINT pDest,POINT pSrc,SIZE szSize,SIZE szSourceBitmap);

    //-->  Original Prototype: bool DisplayBitmapEx(CBitmap & cBitmap,POINT pDest,SIZE szSize);

    static bool DisplayBitmapEx(CBitmap & cBitmap,POINT pDest,SIZE szSize);

    //-->  Original Prototype: bool DisplayBitmapEx(CBitmap & cBitmap,POINT pDest,POINT pSrc, SIZE szSize);

    static bool DisplayBitmapEx(CBitmap & cBitmap,POINT pDest,POINT pSrc,SIZE szSize);

    //-->  Original Prototype: bool DisplayBitmapExR(unsigned char * sMemory,POINT pDest,POINT pSrc, SIZE szSize,SIZE szSourceBitmap);

    static bool DisplayBitmapExR(unsigned char * sMemory,POINT pDest,POINT pSrc,SIZE szSize,SIZE szSourceBitmap);

    //-->  Original Prototype: bool DisplayBitmapExR(CBitmap & cBitmap,POINT pDest,SIZE szSize);

    static bool DisplayBitmapExR(CBitmap & cBitmap,POINT pDest,SIZE szSize);

    //-->  Original Prototype: bool DisplayBitmapExR(CBitmap & cBitmap,POINT pDest,POINT pSrc, SIZE szSize);

    static bool DisplayBitmapExR(CBitmap & cBitmap,POINT pDest,POINT pSrc,SIZE szSize);

    //-->  Original Prototype: HFONT PushFont(const char * sFont);

    // PushFont() -- Set and Push a font on the stack, allowing PopFont() to restore the font.
    //
    // This allows a font to be set and the status saved, so that the original font can be restored.
    //
    // For example:
    //
    // PushFont("arial,20");    -- push current font and set new font to Arial, 20
    // <<multiple output functions>>
    // PopFont() -- Pop the font to the original font.
    //
    // PushFont can be used for up to 32 Fonts on the stack at a time
    //
    static HFONT PushFont(const char * sFont);

    //-->  Original Prototype: HFONT PushFont(HFONT hFont = nullptr);

    // PushFont() -- Set and Push a font on the stack, allowing PopFont() to restore the font.
    //
    // This allows a font to be set and the status saved, so that the original font can be restored.
    //
    // For example:
    //
    // PushFont("arial,20");    -- push current font and set new font to Arial, 20
    // <<multiple output functions>>
    // PopFont() -- Pop the font to the original font.
    //
    // PushFont can be used for up to 32 Fonts on the stack at a time
    //
    static HFONT PushFont(HFONT hFont = nullptr);

    //-->  Original Prototype: HFONT PopFont(int iNumPop = 0);

    // PopFont() -- Pop a pushed font and restore the font to the active window font.
    // See PushFont() for more details.
    //
    static HFONT PopFont(int iNumPop = 0);

    //-->  Original Prototype: bool PushColor(DWORD dwFgColor,DWORD dwBgColor = -1);

    // PushColor() -- Push the current Background and Foreground colors for later retrieval.
    // This can also be used to simultaneously set colors.
    //
    // Specifying only one color pushes and pops the foreground color only.
    // See PushBgColor() to push only a background color
    //
    // For example,
    //
    // PushColor("Red","Green") -- Save current colors and set foregroung to RED and Background to GREEN
    // << Perform multiple functions with new color >>
    // PopColor() -- Return the colors to the original color.
    //
    // PushColor can be used for up to 32 colors on the stack at a time
    //
    static bool PushColor(DWORD dwFgColor,DWORD dwBgColor = -1);

    //-->  Original Prototype: bool PushColor(RGBColor_t rgbFgColor = { -1,-1, -1},RGBColor_t rgbBgColor = {-1,-1,-1});

    // PushColor() -- Push the current Background and Foreground colors for later retrieval.
    // This can also be used to simultaneously set colors.
    //
    // Specifying only one color pushes and pops the foreground color only.
    // See PushBgColor() to push only a background color
    //
    // For example,
    //
    // PushColor("Red","Green") -- Save current colors and set foregroung to RED and Background to GREEN
    // << Perform multiple functions with new color >>
    // PopColor() -- Return the colors to the original color.
    //
    // PushColor can be used for up to 32 colors on the stack at a time
    //
    static bool PushColor(RGBColor_t rgbFgColor = { -1,-1, -1},RGBColor_t rgbBgColor = {-1,-1,-1});

    //-->  Original Prototype: bool PushBgColor(DWORD dwFgColor = -1);

    // PushBgColor() -- Push the current Background color for later retrieval.
    // This can also be used to simultaneously set the background color.
    //
    // This function is the same as PushColor() but works on the background color only.
    //
    // See PushColors() for more information.
    //
    static bool PushBgColor(DWORD dwFgColor = -1);

    //-->  Original Prototype: bool PushBgColor(RGBColor_t rgbColor = {-1,-1,-1});

    // PushBgColor() -- Push the current Background color for later retrieval.
    // This can also be used to simultaneously set the background color.
    //
    // This function is the same as PushColor() but works on the background color only.
    //
    // See PushColors() for more information.
    //
    static bool PushBgColor(RGBColor_t rgbColor = {-1,-1,-1});

    //-->  Original Prototype: bool PopColor(int iNumPop = 1);

    // Pop a color or colors pushed on to the PushColor Stack. 
    // See PushColor() for more informtion.
    //
    static bool PopColor(int iNumPop = 1);

    //-->  Original Prototype: RGBColor_t GetDefaultBgColor();

    // GetDefaultBgColor() -- return default Background color for windows created by SageBox
    //
    static RGBColor_t GetDefaultBgColor();

    //-->  Original Prototype: RGBColor_t GetDefaultFgColor();

    // GetDefaultBgColor() -- return default Foreground color for windows created by SageBox
    //
    static RGBColor_t GetDefaultFgColor();

    //-->  Original Prototype: HFONT GetCurrentFont();

    // Return the Current Font. 
    // This returns a Windows HFONT which can be used in all Font functions
    //
    static HFONT GetCurrentFont();

    //-->  Original Prototype: HFONT SetDefaultFont();

    // Set the Font to the Default Font for the Window.
    //
    static HFONT SetDefaultFont();

    //-->  Original Prototype: HFONT GetDefaultFont();

    // Get the Default Font for the window.
    // This only returns the font but does not set it.
    //
    static HFONT GetDefaultFont();

    //-->  Original Prototype: HFONT SetFont(HFONT hFont);

    // Set the Font for the window.
    // This can take forms such as Text and HFONT, as well as fonts named by previous CreateFont() or SetFont() calls.
    //
    // Bold and italic are created for all fonts. 
    //
    // SetFont(MyFont)                  -- Sets the font to the HFONT MyFont Value
    // SetFont("Arial,20")              -- Sets the font to Arial,20
    // MyFont = SetFont("Arial,20","TestFont")  -- Set the font to Arial,20 and name it TestFont.  Store it in MyFont
    // SetFont("TestFont");             -- Set the Font named "TestFont"
    // SetFont(MyFont)                  -- Sets the font to the HFONT MyFont Value
    //
    static HFONT SetFont(HFONT hFont);

    //-->  Original Prototype: HFONT SetFont(int iFontSize);

    // Set the Font for the window.
    // This can take forms such as Text and HFONT, as well as fonts named by previous CreateFont() or SetFont() calls.
    //
    // Bold and italic are created for all fonts. 
    //
    // SetFont(MyFont)                  -- Sets the font to the HFONT MyFont Value
    // SetFont("Arial,20")              -- Sets the font to Arial,20
    // MyFont = SetFont("Arial,20","TestFont")  -- Set the font to Arial,20 and name it TestFont.  Store it in MyFont
    // SetFont("TestFont");             -- Set the Font named "TestFont"
    // SetFont(MyFont)                  -- Sets the font to the HFONT MyFont Value
    //
    static HFONT SetFont(int iFontSize);

    //-->  Original Prototype: HFONT SetFont(const char * sFont,const char * sNewFontName = nullptr,unsigned char * ucStatus = nullptr);

    // Set the Font for the window.
    // This can take forms such as Text and HFONT, as well as fonts named by previous CreateFont() or SetFont() calls.
    //
    // Bold and italic are created for all fonts. 
    //
    // SetFont(MyFont)                  -- Sets the font to the HFONT MyFont Value
    // SetFont("Arial,20")              -- Sets the font to Arial,20
    // MyFont = SetFont("Arial,20","TestFont")  -- Set the font to Arial,20 and name it TestFont.  Store it in MyFont
    // SetFont("TestFont");             -- Set the Font named "TestFont"
    // SetFont(MyFont)                  -- Sets the font to the HFONT MyFont Value
    //
    static HFONT SetFont(const char * sFont,const char * sNewFontName = nullptr,unsigned char * ucStatus = nullptr);

    //-->  Original Prototype: HFONT SetFont(WCHAR * wsFont,WCHAR * wsNewFontName = nullptr,unsigned char * ucStatus = nullptr);

    // GetFont() -- Get a font created through SageBox.
    //
    // Example, GetFont("MyFont") -- retrieves a font created with CreateFont("Arial,20","TestFont"), or SetFont()
    //
    // The Font returned is an HFONT which can be used with all font functions
    //
    static HFONT SetFont(WCHAR * wsFont,WCHAR * wsNewFontName = nullptr,unsigned char * ucStatus = nullptr);

    //-->  Original Prototype: HFONT GetFont(const char * sFont,const char * sNewFontName = nullptr,unsigned char * ucStatus = nullptr);

    // GetFont() -- Get a font created through SageBox.
    //
    // Example, GetFont("MyFont") -- retrieves a font created with CreateFont("Arial,20","TestFont"), or SetFont()
    //
    // The Font returned is an HFONT which can be used with all font functions
    //
    static HFONT GetFont(const char * sFont,const char * sNewFontName = nullptr,unsigned char * ucStatus = nullptr);

    //-->  Original Prototype: HFONT CreateNewFont(const char * sFont,const char * sNewFontName = nullptr,unsigned char * ucStatus = nullptr);

    // Create the Font for the window -- this works the same as SetFont(), creating the font but not setting it in the window.
    // Use SetFont to set the returned font or to Create-and-Set a font simultaneously.
    //
    // This can take forms such as Text and HFONT, as well as fonts named by previous CreateFont() or SetFont() calls.
    //
    // Bold and italic are created for all fonts. 
    //
    // CreateNewFont(MyFont)                            -- Creates the font to the HFONT MyFont Value
    // CreateNewFont("Arial,20")                        -- Creates the font to Arial,20
    // CreateNewFont = SetFont("Arial,20","TestFont")   -- Create the font to Arial,20 and name it TestFont.  Store it in MyFont
    // CreateNewFont("TestFont");                       -- Create the Font named "TestFont"
    // CreateNewFont(MyFont)                            -- Create the font to the HFONT MyFont Value
    //
    static HFONT CreateNewFont(const char * sFont,const char * sNewFontName = nullptr,unsigned char * ucStatus = nullptr);

    //-->  Original Prototype: __forceinline HFONT AddFont(const char * sFont,const char * sNewFontName = nullptr,unsigned char * ucStatus = nullptr) { return CreateNewFont(sFont,sNewFontName,ucStatus);

    // Create the Font for the window (Same as CreateNewFont) -- this works the same as SetFont(), creating the font 
    // but not setting it in the window. Use SetFont to set the returned font or to Create-and-Set a font simultaneously.
    //
    // This can take forms such as Text and HFONT, as well as fonts named by previous AddFont() or SetFont() calls.
    //
    // Bold and italic are created for all fonts. 
    //
    // AddFont(MyFont)                            -- Creates the font to the HFONT MyFont Value
    // AddFont("Arial,20")                        -- Creates the font to Arial,20
    // AddFont = SetFont("Arial,20","TestFont")   -- Create the font to Arial,20 and name it TestFont.  Store it in MyFont
    // AddFont("TestFont");                       -- Create the Font named "TestFont"
    // AddFont(MyFont)                            -- Create the font to the HFONT MyFont Value
    //
    static HFONT AddFont(const char * sFont,const char * sNewFontName = nullptr,unsigned char * ucStatus = nullptr);

    //-->  Original Prototype: bool MouseDoubleClicked();

    // Returns true of the middle mouse button mouse was double clicked.
    // *** This function is still in development and may not work.
    // This is tied to the status of the window an whether or not it will accept double-clicks.
    //
    static bool MouseDoubleClicked();

    //-->  Original Prototype: bool MouseButtonDown();

    // Returns true if the Left Mouse Button is currently pressed.  
    // This is not a mouse event and returns the real-time status of the mouse.
    //
    static bool MouseButtonDown();

    //-->  Original Prototype: bool KeyPressed(char & cKey,Peek peek = Peek::No);

    /// <summary>
    /// If a key has been pressed, KeyPressed() eturns the key (or TRUE with "cKey") filled with the character code for the key pressed.
    /// <para></para>
    /// If a key has not been pressed, Keypressed() returns false without setting "cKey" (or 0 in the case of returning a Key)
    /// <para></para>&#160;&#160;&#160;
    /// Examples:
    /// <para></para>&#160;&#160;&#160;
    ///     char cKey = win.KeyPressed(); // returns 0 if no key pressed or key code
    ///     bool bPressed = win.KeyPressed(cKey);   // returns true if pressed and fills ckey (otherwise cKey is not changed)
    /// <para></para>
    /// This is an event. Therefore, a second call to KeyPressed() will return false until another key is entered (unless "peek" is
    /// set to Peek::Yes
    /// </summary>
    /// <param name="cKey">= (optional) char value to fill)</param>
    /// <param name="peek">= (optional) When Peek:Yes is used, the status for keypress is not changed (will still return as true)</param>
    /// <returns>BOOLEAN (true/false) if cKey included in paramaters; otherwise returns key info (or 0 for no key pressed)</returns>
    static bool KeyPressed(char & cKey,Peek peek = Peek::No);

    //-->  Original Prototype: char KeyPressed(Peek peek = Peek::No);

    /// <summary>
    /// If a key has been pressed, KeyPressed() eturns the key (or TRUE with "cKey") filled with the character code for the key pressed.
    /// <para></para>
    /// If a key has not been pressed, Keypressed() returns false without setting "cKey" (or 0 in the case of returning a Key)
    /// <para></para>&#160;&#160;&#160;
    /// Examples:
    /// <para></para>&#160;&#160;&#160;
    ///     char cKey = win.KeyPressed(); // returns 0 if no key pressed or key code
    ///     bool bPressed = win.KeyPressed(cKey);   // returns true if pressed and fills ckey (otherwise cKey is not changed)
    /// <para></para>
    /// This is an event. Therefore, a second call to KeyPressed() will return false until another key is entered (unless "peek" is
    /// set to Peek::Yes
    /// </summary>
    /// <param name="cKey">= (optional) char value to fill)</param>
    /// <param name="peek">= (optional) When Peek:Yes is used, the status for keypress is not changed (will still return as true)</param>
    /// <returns>BOOLEAN (true/false) if cKey included in paramaters; otherwise returns key info (or 0 for no key pressed)</returns>
    static char KeyPressed(Peek peek = Peek::No);

    //-->  Original Prototype: bool MouseRButtonDown();

    // Returns true if the Right Mouse Button is currently pressed.  
    // This is not a mouse event and returns the real-time status of the mouse.
    //
    static bool MouseRButtonDown();

    //-->  Original Prototype: bool MouseRButtonClicked(POINT & pPoint,Peek peek = Peek::No);

    static bool MouseRButtonClicked(POINT & pPoint,Peek peek = Peek::No);

    //-->  Original Prototype: bool MouseRButtonClicked(Peek peek = Peek::No);

    static bool MouseRButtonClicked(Peek peek = Peek::No);

    //-->  Original Prototype: bool MouseMoved(Peek peek = Peek::No);

    // Returns true if the mouse was moved
    // This is an event and is a one-time only read so that the status is reset 
    // (i.e. the status wont become true again until the next mouse movement)
    //
    // Use MouseMoved(true) to "peek" at the value so that it will be reset.
    // This will allow subsequent reads before it is cleared.  However, it
    // must be read once without bPeek == true in order for the flag to be reset for the next 
    // event.
    //
    // Include a POINT (i.e. MouseMoved(MyPoint)) to get the mouse-movvement cooordinates.
    // You can also use GetMousePos() to retrieve the current mouse cooordinates.
    //
    static bool MouseMoved(Peek peek = Peek::No);

    //-->  Original Prototype: bool MouseMoved(POINT & pPoint,Peek peek = Peek::No);

    // Returns true if the mouse was moved
    // This is an event and is a one-time only read so that the status is reset 
    // (i.e. the status wont become true again until the next mouse movement)
    //
    // Use MouseMoved(true) to "peek" at the value so that it will be reset.
    // This will allow subsequent reads before it is cleared.  However, it
    // must be read once without bPeek == true in order for the flag to be reset for the next 
    // event.
    //
    // Include a POINT (i.e. MouseMoved(MyPoint)) to get the mouse-movvement cooordinates.
    // You can also use GetMousePos() to retrieve the current mouse cooordinates.
    //
    static bool MouseMoved(POINT & pPoint,Peek peek = Peek::No);

    //-->  Original Prototype: int ButtonPressed(Peek peek = Peek::No);

    // ButtonPressed() -- returns the ID of a button that has an active "press" status.
    // When a button is pressed, the individual Button.Pressed() function can be called to determine the press event.
    //
    // With many buttons on the screen at one time, ButtonPressed() can be called to determine a) if a button was press and b) which one, all in one call.
    //
    // In one mode, ButtonPressed() will return either 0 or the ID of the button (note: it is important to NOT used 0 as an ID)
    // In another mode ButtonPressed(ButtonID) will return true of a button was pressed and will fill ButtonID with the ID of the button.
    //
    // When a button is returned, the "pressed" status of that button is set to false to prepare for the next event, unless bPeek is set to true, in
    // which case it is untouched.
    //
    static int ButtonPressed(Peek peek = Peek::No);

    //-->  Original Prototype: bool ButtonPressed(int & iButtonID,Peek peek = Peek::No);

    // ButtonPressed() -- returns the ID of a button that has an active "press" status.
    // When a button is pressed, the individual Button.Pressed() function can be called to determine the press event.
    //
    // With many buttons on the screen at one time, ButtonPressed() can be called to determine a) if a button was press and b) which one, all in one call.
    //
    // In one mode, ButtonPressed() will return either 0 or the ID of the button (note: it is important to NOT used 0 as an ID)
    // In another mode ButtonPressed(ButtonID) will return true of a button was pressed and will fill ButtonID with the ID of the button.
    //
    // When a button is returned, the "pressed" status of that button is set to false to prepare for the next event, unless bPeek is set to true, in
    // which case it is untouched.
    //
    static bool ButtonPressed(int & iButtonID,Peek peek = Peek::No);

    //-->  Original Prototype: bool MouseClicked(Peek peek = Peek::No);

    // MouseClicked() -- returns true if the Left Mouse Button was clicked.
    //
    // This is an event and is a one-time only read so that the status is reset 
    // (i.e. the status wont become true again until the next mouse click)
    //
    // Use MouseClicked(true) to "peek" at the value so that it will be reset.
    // This will allow subsequent reads before it is cleared.  However, it
    // must be read once without bPeek == true in order for the flag to be reset for the next 
    // event.
    //
    // Include a POINT (i.e. MouseClicked(MyPoint)) to get the mouse-click coodrinates.
    // You can also use GetMouseClickPos() to retrieve the last mouse-click coordinates.
    //
    static bool MouseClicked(Peek peek = Peek::No);

    //-->  Original Prototype: bool MouseClicked(POINT & pMouse,Peek peek = Peek::No);

   // MouseClicked() -- returns true if the Left Mouse Button was clicked.
    //
    // This is an event and is a one-time only read so that the status is reset 
    // (i.e. the status wont become true again until the next mouse click)
    //
    // Use MouseClicked(true) to "peek" at the value so that it will be reset.
    // This will allow subsequent reads before it is cleared.  However, it
    // must be read once without bPeek == true in order for the flag to be reset for the next 
    // event.
    //
    // Include a POINT (i.e. MouseClicked(MyPoint)) to get the mouse-click coodrinates.
    // You can also use GetMouseClickPos() to retrieve the last mouse-click coordinates.
    //
    static bool MouseClicked(POINT & pMouse,Peek peek = Peek::No);

    //-->  Original Prototype: bool MouseDragEvent(Peek peek = Peek::No);

    static bool MouseDragEvent(Peek peek = Peek::No);

    //-->  Original Prototype: bool MouseDragEvent(POINT & pMouse,Peek peek = Peek::No);

    static bool MouseDragEvent(POINT & pMouse,Peek peek = Peek::No);

    //-->  Original Prototype: bool MouseDragEnded(Peek peek = Peek::No);

    static bool MouseDragEnded(Peek peek = Peek::No);

    //-->  Original Prototype: bool MouseDragEnded(POINT & pMouse,Peek peek = Peek::No);

    static bool MouseDragEnded(POINT & pMouse,Peek peek = Peek::No);

    //-->  Original Prototype: bool isMouseDragging();

    static bool isMouseDragging();

    //-->  Original Prototype: bool isMouseDragging(POINT & pStartDrag);

    static bool isMouseDragging(POINT & pStartDrag);

    //-->  Original Prototype: POINT GetMouseDragStart();

    static POINT GetMouseDragStart();

    //-->  Original Prototype: bool GetMousePos(int & iMouseX,int & iMouseY);

    // GetMousePos() -- Returns the current mouse coordinates relative to the window
    //
    static bool GetMousePos(int & iMouseX,int & iMouseY);

    //-->  Original Prototype: POINT GetMousePos();

    // GetMousePos() -- Returns the current mouse coordinates relative to the window
    //
    static POINT GetMousePos();

    //-->  Original Prototype: bool GetMouseClickPos(int & iMouseX,int & iMouseY);

    // GetMouseClickPos() -- Returns the last mouse click coordinates.
    //
    // This can be used after a MouseClick() event to return the mouse-click coordinates
    // at the time the mouse was clicked.
    //
    // This works for both left button and right button clicks.
    //
    static bool GetMouseClickPos(int & iMouseX,int & iMouseY);

    //-->  Original Prototype: POINT GetMouseClickPos();

    // GetMouseClickPos() -- Returns the last mouse click coordinates.
    //
    // This can be used after a MouseClick() event to return the mouse-click coordinates
    // at the time the mouse was clicked.
    //
    // This works for both left button and right button clicks.
    //
    static POINT GetMouseClickPos();

    //-->  Original Prototype: bool MouseWheelMoved(Peek peek = Peek::No);

    static bool MouseWheelMoved(Peek peek = Peek::No);

    //-->  Original Prototype: bool MouseWheelMoved(int & iDistance,Peek peek = Peek::No);

    static bool MouseWheelMoved(int & iDistance,Peek peek = Peek::No);

    //-->  Original Prototype: int GetMouseWheelMove(bool bResetEvent = true);

    static int GetMouseWheelMove(bool bResetEvent = true);

    //-->  Original Prototype: bool WindowResized(SIZE & szNewWinSize,Peek peek = Peek::No);

    static bool WindowResized(SIZE & szNewWinSize,Peek peek = Peek::No);

    //-->  Original Prototype: bool WindowResized(Peek peek = Peek::No);

    static bool WindowResized(Peek peek = Peek::No);

    //-->  Original Prototype: ConsoleOp_t SetWritePos(POINT pLoc);

    // SetWritePos() -- Set the output position in the Window for writing text. 
    // 
    // --> When text is written, SageBox keeps track of where to write.
    // --> You can set this value to any coordinate in the Window, and the next text output will occur at this location.
    // -->
    // --> For example, SetPos(0,0) Sets the top-left position of the screen.
    //                  SetPos(100,400) Sets (X,Y) position to 100,400 for the next write. Subsequent "\n" or CRLF will set the position to the next line
    //                  at the leftmost column (X position 0).  You can use console.SetIndent() to change the setting of the X position on "\n"
    //
    // --> Note: using Cls() sets the position to (0,0) (i.e. SetPos(0,0));
    //
    static ConsoleOp_t SetWritePos(POINT pLoc);

    //-->  Original Prototype: ConsoleOp_t SetWritePosX(int iX);

    // SetWritePosX() -- Set the output X position in the window for writing text. 
    //
    // This sets only the X position in the ouput for printf, Write(), etc. -- anything that prints text.
    // The current Y position is not changed.
    //
    // See SetWritePos() for more information; 
    //
    static ConsoleOp_t SetWritePosX(int iX);

    //-->  Original Prototype: void SetBkMode(BkMode eBkType);

    // SetBkMode() -- Set the text background mode.
    // BkMode returns eeither BkMode::Transparent or BkMode::Opaque
    //
    // BkMode::Transparent()  --- This is the default.  When text is written to the screen, only the text part is written in the
    //                            foreground color, leaving the blank areas (i.e. text or graphics behind the new text) untouched.
    //
    // BkMode::Opaque         -- When text is written to the window, this will write the text in the foreground color and fill in the background with the
    //                           background color overwriting any text or graphics behind the new text.
    //
    static void SetBkMode(BkMode eBkType);

    //-->  Original Prototype: BkMode GetBkMode();

    // GetBkMode() -- Get the text background mode.
    // BkMode returns eeither BkMode::Transparent or BkMode::Opaque
    //
    // BkMode::Transparent()  --- This is the default.  When text is written to the screen, only the text part is written in the
    //                            foreground color, leaving the blank areas (i.e. text or graphics behind the new text) untouched.
    //
    // BkMode::Opaque         -- When text is written to the window, this will write the text in the foreground color and fill in the background with the
    //                           background color overwriting any text or graphics behind the new text.
    //
    static BkMode GetBkMode();

    //-->  Original Prototype: bool SetWordWrap(bool bWrap = true);

    // SetWordWrap() -- sets whether text printed to the screen wraps to the next line when exceeding the window's edge. 
    //
    // When ON (SetWordWrap(true)), text printed out to the window wraps to the next line.
    // When OFF, text printed will not wrap around the window and will not appear once it reaches the edge of the widow. 
    //
    // Word wrap is generally used for console-based windows.
    //
    // For general and default windows, Word Wrap is set to OFF by default.  For Text Windows, word wrap is set to ON by default.
    //
    // Word wrap is off for Text Widgets and other text controls and can be turned on only for widgets that have WordWrap() functions of their own.
    //
    // Word wrap is off for Text Widgets and other text controls and can be turned on only for widgets that have WordWrap() functions of their own.
    // When using "console" functions (i.e. MyWidow.console.Write() or MyWindow.console.printf()) word wrap is always on. 
    // When using regular Write(), printf() and other functions, word wrap is only on if it has been set with SetWordWrap()
    //
    static bool SetWordWrap(bool bWrap = true);

    //-->  Original Prototype: bool GetWordWrap();

    // GetWordWrap() -- returns the current word-wrap status for the window (see SetWordWrap());
    //
    static bool GetWordWrap();

    //-->  Original Prototype: void SetProgramName(char * sProgramName);

    // Sets the Program/Application name.  This can also be set when initiating Sagebox, i.e. CSageBox("My Application"); 
    //
    // The Application name is used in various window title bars in SageBox.  With no Program Name set, either the window title is blank or a default
    // text string is used.
    //
    static void SetProgramName(char * sProgramName);

    //-->  Original Prototype: const char * GetProgramName();

    // Gets the program name set in SageBox through either the SageBox constructor (i.e. CSageBox("My Application") or through GetProgramName();
    //
    static const char * GetProgramName();

    //-->  Original Prototype: bool WindowClosing();

    // Returns true if the 'X' button was pressed in the window or the window is closing for some other reason.
    // 
    // By default in Sagebox, main windows do not close when the 'X' button is pressed.  Instead, the WindowClosing() flag
    // is set so that it can be handled by the user's code. 
    //
    // When the Window is closed, it is up to the code to determine whether or not to close the window.  In some cases, a dialog can be used to determine
    // whether or not the user wants to close the window, to save unsaved items, etc.
    //
    // Important Note:  Many blocking Sagebox functions (such as dialogs and other functions that wait for button presses) will exit or fallthrough when 
    // the Window close button has been pressed.
    //
    // See CLoseButtonPressed() to determine if the Window is closing because of a 'X' button press or some other reason.
    //
    static bool WindowClosing();

    //-->  Original Prototype: bool ResetWindowClosing();

    // Reset Window Closing Status.   
    //
    // This resets the Windows Closing status returned by WindowClosing().  Many Sagebox functions will not activate or passthrough when the
    // WindowClosing() status is set to true.
    //
    // This can be used when a user has pressed the close button and CloseButtonPessed() returns true.
    // To cancel the user-based close, call ResetWindowClosing()
    //
    static bool ResetWindowClosing();

    //-->  Original Prototype: bool SetWindowClosing(bool bPressCloseButton = false);

    static bool SetWindowClosing(bool bPressCloseButton = false);

    //-->  Original Prototype: bool PressCloseButton();

    static bool PressCloseButton();

    //-->  Original Prototype: bool PressButtonOnClose(CButton & cButton);

    /// <summary>
    /// Simulates a press on a button when the user closes the window. 
    /// <para></para>
    /// When the users closes the window, a button press is simulated for the button, causing a SageEvent or return from GetEvent().  The button then may be queried for its 
    /// pressed status. 
    /// <para></para>&#160;&#160;&#160;
    /// If a signal is attached to the button, the boolean value that serves as the signal is set to true.
    /// </summary>
    /// <param name="cButton">Button to press when the window is closed by the user</param>
    /// <returns></returns>
    static bool PressButtonOnClose(CButton & cButton);

    //-->  Original Prototype: bool CloseButtonPressed(Peek peek = Peek::No);

    // CloseButtonPressed() -- Returns true of the close buttton was pressed.
    //
    // This is an event and is one-time read function -- i.e. it will return false after subsequent
    // calls until the next time the 'X' Window button is pressed.
    //
    // This can be used with WindowClosing() to determine if the 'X' window button was pressed or the window
    // was closing for some other reason. Use CloseButtonPressed(true) to read the value without resetting it.
    //
    // This can be used to ask the user if they wish to close the window, save unsaved items, etc.
    //
    // Since this is an event, it can be used to reset the WindowClosing() flag by using ResetWindowClosing()
    //
    static bool CloseButtonPressed(Peek peek = Peek::No);

    //-->  Original Prototype: bool PeekCloseButtonPressed();

    // PeekCloseButtonPressed() -- Use to determine if the close button was pressed before 
    //
    // This is a shortcut for CLoseButtonPressed(true)
    // See CloseButtonPressed() for more Informatuion
    //
    static bool PeekCloseButtonPressed();

    //-->  Original Prototype: int getCharWidth();

    // getCharWidth() -- get the average character width for the current window font.
    // For proportional fonts, this returns the average chacter width for the font. 
    // For termainl fonts (i.e. Courier New, etc.) this returns the width for all characters.
    //
    static int getCharWidth();

    //-->  Original Prototype: int getCharHeight();

    // getCharHeight() -- Returns the height of the characters for the current window font.  This is 
    // the lineheight for each line printed. 
    //
    static int getCharHeight();

    //-->  Original Prototype: SIZE getCharSize(const char * sFont);

    // getCharSize() -- returns the average character width and exact height for the current window font
    // used for all text-based input and output functions.
    //
    static SIZE getCharSize(const char * sFont);

    //-->  Original Prototype: SIZE getCharSize(HFONT hFont = nullptr);

    // getCharSize() -- returns the average character width and exact height for the current window font
    // used for all text-based input and output functions.
    //
    static SIZE getCharSize(HFONT hFont = nullptr);

    //-->  Original Prototype: RGBColor_t GetBgColor();

    // GetBgColor() -- Get the current backround color for the window.
    // This color is used for Cls() and all text output routines.
    //
    static RGBColor_t GetBgColor();

    //-->  Original Prototype: RGBColor_t GetFgColor();

    // GetFgColor() -- Get the current foreground (i.e. Text) color for the window.
    // This color is used all text output routines.
    //
    static RGBColor_t GetFgColor();

    //-->  Original Prototype: HDC GetCurDC();

    // Get the current Windows DC (device context) for the window.
    //
    // This is useful for using Windows functions that SageBox may not provide.
    // This can be used with drawing functions or any other output function or
    // Windows bitmap functions, etc. 
    //
    // This is used for raw windows programming outside of Sagebox.
    // Also see GetWindowHandle() to retrieve the Windows handle to the window
    //
    // Note: This generally retrieves the DC to the bitmap that is then printed to 
    // the output.  To get the window DC itself, use SetWindowBuffering(false)
    // to turn off buffering, in which case GetCurDC() will return the DC to the window
    // rather than the bitmap.
    //
    static HDC GetCurDC();

    //-->  Original Prototype: HDC GetDesktopDC();

    // Get the Windows Desktop Device Context of the current window, i.e. the static Device Context.
    //
    // This returns the Windows Device Context (HDC) for the actual Window rather than the bitmap that Sagebox uses
    // for output. 
    // 
    // See GetCurDC() for more information
    //
    static HDC GetDesktopDC();

    //-->  Original Prototype: HDC GetBitmapDC();

    // Get the Windows Device Context for the internal bitmap Sagebox uses for output functions (i.e. Write(), printf(), drawing, etc.)
    //
    // See GetDesktopDC() to get the Windows Device Context for the actual window rather than the internal bitmap (canvas).
    // 
    // See GetCurDC() for more information
    //
    static HDC GetBitmapDC();

    //-->  Original Prototype: POINT GetWritePos();

    // GetWritePos() -- Returns the current X,Y output position for all text-based functions.
    //
    static POINT GetWritePos();

    //-->  Original Prototype: CWindow & BitmapWindow(RawBitmap_t & stBitmap,const  cwfOpt & cwOpt = cwfOpt());

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
    static CWindow & BitmapWindow(RawBitmap_t & stBitmap,const  cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CWindow & BitmapWindow(CBitmap & cBitmap,const  cwfOpt & cwOpt= cwfOpt());

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
    static CWindow & BitmapWindow(CBitmap & cBitmap,const  cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CWindow & BitmapWindow(int iX,int iY,CBitmap & cBitmap,const cwfOpt & cwOpt= cwfOpt());

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
    static CWindow & BitmapWindow(int iX,int iY,CBitmap & cBitmap,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CWindow & BitmapWindow(int iX,int iY,RawBitmap_t & stBitmap,const  cwfOpt & cwOpt= cwfOpt());

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
    static CWindow & BitmapWindow(int iX,int iY,RawBitmap_t & stBitmap,const  cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CWindow & ChildWindow(int iX,int iY,int iWidth,int iHeight,const cwfOpt & cwOpt = cwfOpt());

    // Create a new Child Window within the parent window. 
    // This created an embedded window that is the same as other windows (and, in fact, return CWindow)
    // except that it is not a popup window. 
    //
    // A Border may be added to the window with the option AddBorder()
    // 
    // Child Windows are regular windows, simply embedded. 
    // All CWindow functions (output, display, input, etc.) are available in child windows.
    //
    // Note: As with other windows, a sublassed object may be passed in that overrides 
    // messaging, Main() and other Window components.
    //
    // All Message processing runs through EventLoop() and WaitforEvent() in both the child window
    // and parent window -- that is, events from the child widow can be processed in the parent window.
    //
    // SetMessageHandler() may also be used.
    //
    static CWindow & ChildWindow(int iX,int iY,int iWidth,int iHeight,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CWindow & ChildWindow(POINT pLoc,SIZE szSize,const cwfOpt & cwOpt = cwfOpt());

    static CWindow & ChildWindow(POINT pLoc,SIZE szSize,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CWindow & ChildWindow(CWindow * cWindow,int iX,int iY,int iWidth,int iHeight,const cwfOpt & cwOpt = cwfOpt());

    // Create a new Child Window within the parent window. 
    // This created an embedded window that is the same as other windows (and, in fact, return CWindow)
    // except that it is not a popup window. 
    //
    // A Border may be added to the window with the option AddBorder()
    // 
    // Child Windows are regular windows, simply embedded. 
    // All CWindow functions (output, display, input, etc.) are available in child windows.
    //
    // Note: As with other windows, a sublassed object may be passed in that overrides 
    // messaging, Main() and other Window components.
    //
    // All Message processing runs through EventLoop() and WaitforEvent() in both the child window
    // and parent window -- that is, events from the child widow can be processed in the parent window.
    //
    // SetMessageHandler() may also be used.
    //
    static CWindow & ChildWindow(CWindow * cWindow,int iX,int iY,int iWidth,int iHeight,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CWindow & ChildWindow(CWindow * cWindow,POINT pLoc,SIZE szSize,const cwfOpt & cwOpt = cwfOpt());

    static CWindow & ChildWindow(CWindow * cWindow,POINT pLoc,SIZE szSize,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CWindow & NewWindow(int iX,int iY,int iWidth,int iHeight,const char * sTitle = nullptr,const cwfOpt & cwOpt = cwfOpt());

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
    static CWindow & NewWindow(int iX,int iY,int iWidth,int iHeight,const char * sTitle = nullptr,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CWindow & NewWindow(int iX,int iY,int iWidth,int iHeight,const cwfOpt & cwOpt);

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
    static CWindow & NewWindow(int iX,int iY,int iWidth,int iHeight,const cwfOpt & cwOpt);

    //-->  Original Prototype: CWindow & NewWindow(int iX,int iY,const char * sTitle = nullptr,const cwfOpt & cwOpt = cwfOpt());

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
    static CWindow & NewWindow(int iX,int iY,const char * sTitle = nullptr,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CWindow & NewWindow(int iX,int iY,const cwfOpt & cwOpt);

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
    static CWindow & NewWindow(int iX,int iY,const cwfOpt & cwOpt);

    //-->  Original Prototype: CWindow & NewWindow(CWindow * cWin,int iX,int iY,int iWidth,int iHeight,const char * sTitle = nullptr,const cwfOpt & cwOpt = cwfOpt());

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
    static CWindow & NewWindow(CWindow * cWin,int iX,int iY,int iWidth,int iHeight,const char * sTitle = nullptr,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CWindow & NewWindow(CWindow * cWin,int iX,int iY,int iWidth,int iHeight,const cwfOpt & cwOpt);

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
    static CWindow & NewWindow(CWindow * cWin,int iX,int iY,int iWidth,int iHeight,const cwfOpt & cwOpt);

    //-->  Original Prototype: CWindow & NewWindow(CWindow * cWin,int iX,int iY,const char * sTitle = nullptr,const cwfOpt & cwOpt = cwfOpt());

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
    static CWindow & NewWindow(CWindow * cWin,int iX,int iY,const char * sTitle = nullptr,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CWindow & NewWindow(CWindow * cWin,int iX,int iY,const cwfOpt & cwOpt);

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
    static CWindow & NewWindow(CWindow * cWin,int iX,int iY,const cwfOpt & cwOpt);

    //-->  Original Prototype: RGBColor_t MakeColor(const char * sColor,DWORD rgbColor);

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
    static RGBColor_t MakeColor(const char * sColor,DWORD rgbColor);

    //-->  Original Prototype: RGBColor_t MakeColor(const char * sColor,RGBColor_t rgbColor);

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
    static RGBColor_t MakeColor(const char * sColor,RGBColor_t rgbColor);

    //-->  Original Prototype: RGBColor_t  GetColor(const char * sColor,bool * pColorFound = nullptr);

    // Get a named color.  This returns an RGBColor_t (or DWORD -- see prototypes) of a named color.
    // 
    // Example:
    //
    // auto rgbBlue = GetColor("Blue");                 -- Get RGBColor_t value for Blue
    // RGBColor_t rgbMyColor = GetColor("MyColor");     -- Get RGBColor_t value for "MyColor" created previously.
    //
    static RGBColor_t GetColor(const char * sColor,bool * pColorFound = nullptr);

    //-->  Original Prototype: bool GetColor(const char * sColor,DWORD & rgbColor);

    // Get a named color.  This returns an RGBColor_t (or DWORD -- see prototypes) of a named color.
    // 
    // Example:
    //
    // auto rgbBlue = GetColor("Blue");                 -- Get RGBColor_t value for Blue
    // RGBColor_t rgbMyColor = GetColor("MyColor");     -- Get RGBColor_t value for "MyColor" created previously.
    //
    static bool GetColor(const char * sColor,DWORD & rgbColor);

    //-->  Original Prototype: void SetAutoBuffer(bool bAutoBuffer);

    // SetAutoBuffer() -- Sets the screen to buffer only when necessary.
    // *** This is experimental and meant for faster displays.  See SetWindowBuffering();
    //
    static void SetAutoBuffer(bool bAutoBuffer);

    //-->  Original Prototype: void SetWindowBuffering(bool bBuffer);

    // SetWindowBuffering() -- Sets whether or not the window is bufferred to a bitmap before sending output to the screen. 
    //
    // By default, all SageBox windows are bufferred, meaning that all display actions are performed on a bitmap and then
    // sent to the window -- either immediately or through a schedule update for faster processing.
    //
    // When the Window is covered up and uncovered, or disappears due to other window and reappears, Windows tells Sagebox to repaint
    // the window.  In this case, Sagebox retrieves the bufferred window and displays it to the screen.
    //
    // When Window buffering is turned off, this turns the window into a default Windows, unbufferred window, meaning
    // all output goes directly to the window. 
    //
    // When off, window actions can be much faster or much slower, depending on the actions.
    // But it is now the program's responsibility to intercept WM_PAINT messages and other 
    // Windows messages to ensure the window remains static when Windows needs to repaint certain portions of the window or the entire window.
    //
    // When turning the Buffering back on, Sagebox copies the contents of the window so that Buffering can be turned off for some 
    // specific purposes and easily turned back on with buffering enabled.
    //
    static void SetWindowBuffering(bool bBuffer);

    //-->  Original Prototype: void SetAutoUpdate(bool bAuto = true);

    /// <summary>
    /// Sets the Auto Update Type for the Window
    /// <para></para>&#160;&#160;&#160;
    /// - By Default, Auto Update is turned on.  This sets the udpate to every 10 milliseconds or so, so that not every action drawn has to 
    /// draw the bitmap on the screen, resulting in much faster performance.
    /// <para></para>&#160;&#160;&#160;
    /// - When AutoUpdate() is off, Update() must be called manually to update the screen, which can be cumberome, determining when and where
    /// to sprinkle Update() calls.
    /// <para></para>&#160;&#160;&#160;
    /// - The drawback to Auto Updating is that, in some cases, the last update may not be current if there is a point where program executioin stops.
    /// After most large loops with output, it is a good idea to put a final Update() (without a SleepMS) to ensure the last items output to the
    /// screen are updated.
    /// <para></para>&#160;&#160;&#160;
    /// EventLoop(), WaitforEvent(), getString, GetInteger, etc.  as well as creating new controls -- most Sagebox functions -- call 
    /// Update prior to executing to ensure the screen is up-to-date.
    /// </summary>
    /// <param name="AutoUpdateType update"> = UpdateType.  AutoUpdate(true) sets AutoUpdateType::On, AutoUpdate(false) turns off auto-update</param>
    static void SetAutoUpdate(bool bAuto = true);

    //-->  Original Prototype: void SetAutoUpdate(AutoUpdateType update);

    /// <summary>
    /// Sets the Auto Update Type for the Window
    /// <para></para>&#160;&#160;&#160;
    /// - By Default, Auto Update is turned on.  This sets the udpate to every 10 milliseconds or so, so that not every action drawn has to 
    /// draw the bitmap on the screen, resulting in much faster performance.
    /// <para></para>&#160;&#160;&#160;
    /// - When AutoUpdate() is off, Update() must be called manually to update the screen, which can be cumberome, determining when and where
    /// to sprinkle Update() calls.
    /// <para></para>&#160;&#160;&#160;
    /// - The drawback to Auto Updating is that, in some cases, the last update may not be current if there is a point where program executioin stops.
    /// After most large loops with output, it is a good idea to put a final Update() (without a SleepMS) to ensure the last items output to the
    /// screen are updated.
    /// <para></para>&#160;&#160;&#160;
    /// EventLoop(), WaitforEvent(), getString, GetInteger, etc.  as well as creating new controls -- most Sagebox functions -- call 
    /// Update prior to executing to ensure the screen is up-to-date.
    /// </summary>
    /// <param name="AutoUpdateType update"> = UpdateType.  AutoUpdate(true) sets AutoUpdateType::On, AutoUpdate(false) turns off auto-update</param>
    static void SetAutoUpdate(AutoUpdateType update);

    //-->  Original Prototype: CButton & NewButton(int ix,int iy,int iWidth,int iHeight,const char * sButtonText,const cwfOpt & cwOpt = cwfOpt());

    // Create a new button on the window. 
    //
    // Creates a new button at the (iX,iY) coordinates.  The Width and Height are optional, depending on the needs and type of the button.
    // When these values are omitted, the button is automatially sized.  Negative values add padding to the automatic size calculation.
    // For example, NewButton(10,20,-40,0,"Ok") creates on "OK" button that automatically calculates the width and height of the button 
    // and add 40 pixels on either side of the "OK" for spacing.
    //
    // An overridden class object can be applied to get windows messages.  
    // SetMessageHandler() can also set a message handler without overriding the class.
    //
    // Other options:
    //
    // Style()      -- Set the style of the buttoin,i.e. Style("Panel"), Style("Windows"), Style("medium");
    // SetFgColor()     -- Set the text foreground color
    // SetBgColor()     -- Set the text background color
    // JustCenter()     -- Center the button in the X plane. 
    // See opt:: descriptions for more options
    //
    static CButton & NewButton(int ix,int iy,int iWidth,int iHeight,const char * sButtonText,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CButton & NewButton(int ix,int iy,int iWidth,const char * sButtonText,const cwfOpt & cwOpt = cwfOpt());

    // Create a new button on the window. 
    //
    // Creates a new button at the (iX,iY) coordinates.  The Width and Height are optional, depending on the needs and type of the button.
    // When these values are omitted, the button is automatially sized.  Negative values add padding to the automatic size calculation.
    // For example, NewButton(10,20,-40,0,"Ok") creates on "OK" button that automatically calculates the width and height of the button 
    // and add 40 pixels on either side of the "OK" for spacing.
    //
    // An overridden class object can be applied to get windows messages.  
    // SetMessageHandler() can also set a message handler without overriding the class.
    //
    // Other options:
    //
    // Style()      -- Set the style of the buttoin,i.e. Style("Panel"), Style("Windows"), Style("medium");
    // SetFgColor()     -- Set the text foreground color
    // SetBgColor()     -- Set the text background color
    // JustCenter()     -- Center the button in the X plane. 
    // See opt:: descriptions for more options
    //
    static CButton & NewButton(int ix,int iy,int iWidth,const char * sButtonText,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CButton & NewButton(int ix,int iy,const char * sButtonText,const cwfOpt & cwOpt = cwfOpt());

    // Create a new button on the window. 
    //
    // Creates a new button at the (iX,iY) coordinates.  The Width and Height are optional, depending on the needs and type of the button.
    // When these values are omitted, the button is automatially sized.  Negative values add padding to the automatic size calculation.
    // For example, NewButton(10,20,-40,0,"Ok") creates on "OK" button that automatically calculates the width and height of the button 
    // and add 40 pixels on either side of the "OK" for spacing.
    //
    // An overridden class object can be applied to get windows messages.  
    // SetMessageHandler() can also set a message handler without overriding the class.
    //
    // Other options:
    //
    // Style()      -- Set the style of the buttoin,i.e. Style("Panel"), Style("Windows"), Style("medium");
    // SetFgColor()     -- Set the text foreground color
    // SetBgColor()     -- Set the text background color
    // JustCenter()     -- Center the button in the X plane. 
    // See opt:: descriptions for more options
    //
    static CButton & NewButton(int ix,int iy,const char * sButtonText,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CButton & NewButton(CButton * cUserButton,int ix,int iy,int iWidth,int iHeight,const char * sButtonText,const cwfOpt & cwOpt = cwfOpt());

    // Create a new button on the window. 
    //
    // Creates a new button at the (iX,iY) coordinates.  The Width and Height are optional, depending on the needs and type of the button.
    // When these values are omitted, the button is automatially sized.  Negative values add padding to the automatic size calculation.
    // For example, NewButton(10,20,-40,0,"Ok") creates on "OK" button that automatically calculates the width and height of the button 
    // and add 40 pixels on either side of the "OK" for spacing.
    //
    // An overridden class object can be applied to get windows messages.  
    // SetMessageHandler() can also set a message handler without overriding the class.
    //
    // Other options:
    //
    // Style()      -- Set the style of the buttoin,i.e. Style("Panel"), Style("Windows"), Style("medium");
    // SetFgColor()     -- Set the text foreground color
    // SetBgColor()     -- Set the text background color
    // JustCenter()     -- Center the button in the X plane. 
    // See opt:: descriptions for more options
    //
    static CButton & NewButton(CButton * cUserButton,int ix,int iy,int iWidth,int iHeight,const char * sButtonText,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CButton & NewButton(CButton * cUserButton,int ix,int iy,int iWidth,const char * sButtonText,const cwfOpt & cwOpt = cwfOpt());

    // Create a new button on the window. 
    //
    // Creates a new button at the (iX,iY) coordinates.  The Width and Height are optional, depending on the needs and type of the button.
    // When these values are omitted, the button is automatially sized.  Negative values add padding to the automatic size calculation.
    // For example, NewButton(10,20,-40,0,"Ok") creates on "OK" button that automatically calculates the width and height of the button 
    // and add 40 pixels on either side of the "OK" for spacing.
    //
    // An overridden class object can be applied to get windows messages.  
    // SetMessageHandler() can also set a message handler without overriding the class.
    //
    // Other options:
    //
    // Style()      -- Set the style of the buttoin,i.e. Style("Panel"), Style("Windows"), Style("medium");
    // SetFgColor()     -- Set the text foreground color
    // SetBgColor()     -- Set the text background color
    // JustCenter()     -- Center the button in the X plane. 
    // See opt:: descriptions for more options
    //
    static CButton & NewButton(CButton * cUserButton,int ix,int iy,int iWidth,const char * sButtonText,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CButton & NewButton(CButton * cUserButton,int ix,int iy,const char * sButtonText,const cwfOpt & cwOpt = cwfOpt());

    // Create a new button on the window. 
    //
    // Creates a new button at the (iX,iY) coordinates.  The Width and Height are optional, depending on the needs and type of the button.
    // When these values are omitted, the button is automatially sized.  Negative values add padding to the automatic size calculation.
    // For example, NewButton(10,20,-40,0,"Ok") creates on "OK" button that automatically calculates the width and height of the button 
    // and add 40 pixels on either side of the "OK" for spacing.
    //
    // An overridden class object can be applied to get windows messages.  
    // SetMessageHandler() can also set a message handler without overriding the class.
    //
    // Other options:
    //
    // Style()      -- Set the style of the buttoin,i.e. Style("Panel"), Style("Windows"), Style("medium");
    // SetFgColor()     -- Set the text foreground color
    // SetBgColor()     -- Set the text background color
    // JustCenter()     -- Center the button in the X plane. 
    // See opt:: descriptions for more options
    //
    static CButton & NewButton(CButton * cUserButton,int ix,int iy,const char * sButtonText,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CButton & NewButton(int ix,int iy,int iWidth,int iHeight,const wchar_t * sButtonText,const cwfOpt & cwOpt = cwfOpt());

    // Create a new button on the window. 
    //
    // Creates a new button at the (iX,iY) coordinates.  The Width and Height are optional, depending on the needs and type of the button.
    // When these values are omitted, the button is automatially sized.  Negative values add padding to the automatic size calculation.
    // For example, NewButton(10,20,-40,0,"Ok") creates on "OK" button that automatically calculates the width and height of the button 
    // and add 40 pixels on either side of the "OK" for spacing.
    //
    // An overridden class object can be applied to get windows messages.  
    // SetMessageHandler() can also set a message handler without overriding the class.
    //
    // Other options:
    //
    // Style()      -- Set the style of the buttoin,i.e. Style("Panel"), Style("Windows"), Style("medium");
    // SetFgColor()     -- Set the text foreground color
    // SetBgColor()     -- Set the text background color
    // JustCenter()     -- Center the button in the X plane. 
    // See opt:: descriptions for more options
    //
    static CButton & NewButton(int ix,int iy,int iWidth,int iHeight,const wchar_t * sButtonText,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CButton & NewButton(int ix,int iy,int iWidth,const wchar_t * sButtonText,const cwfOpt & cwOpt = cwfOpt());

    // Create a new button on the window. 
    //
    // Creates a new button at the (iX,iY) coordinates.  The Width and Height are optional, depending on the needs and type of the button.
    // When these values are omitted, the button is automatially sized.  Negative values add padding to the automatic size calculation.
    // For example, NewButton(10,20,-40,0,"Ok") creates on "OK" button that automatically calculates the width and height of the button 
    // and add 40 pixels on either side of the "OK" for spacing.
    //
    // An overridden class object can be applied to get windows messages.  
    // SetMessageHandler() can also set a message handler without overriding the class.
    //
    // Other options:
    //
    // Style()      -- Set the style of the buttoin,i.e. Style("Panel"), Style("Windows"), Style("medium");
    // SetFgColor()     -- Set the text foreground color
    // SetBgColor()     -- Set the text background color
    // JustCenter()     -- Center the button in the X plane. 
    // See opt:: descriptions for more options
    //
    static CButton & NewButton(int ix,int iy,int iWidth,const wchar_t * sButtonText,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CButton & NewButton(int ix,int iy,const wchar_t * sButtonText,const cwfOpt & cwOpt = cwfOpt());

    // Create a new button on the window. 
    //
    // Creates a new button at the (iX,iY) coordinates.  The Width and Height are optional, depending on the needs and type of the button.
    // When these values are omitted, the button is automatially sized.  Negative values add padding to the automatic size calculation.
    // For example, NewButton(10,20,-40,0,"Ok") creates on "OK" button that automatically calculates the width and height of the button 
    // and add 40 pixels on either side of the "OK" for spacing.
    //
    // An overridden class object can be applied to get windows messages.  
    // SetMessageHandler() can also set a message handler without overriding the class.
    //
    // Other options:
    //
    // Style()      -- Set the style of the buttoin,i.e. Style("Panel"), Style("Windows"), Style("medium");
    // SetFgColor()     -- Set the text foreground color
    // SetBgColor()     -- Set the text background color
    // JustCenter()     -- Center the button in the X plane. 
    // See opt:: descriptions for more options
    //
    static CButton & NewButton(int ix,int iy,const wchar_t * sButtonText,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CButton & NewButton(CButton * cUserButton,int ix,int iy,int iWidth,int iHeight,const wchar_t * sButtonText,const cwfOpt & cwOpt = cwfOpt());

    // Create a new button on the window. 
    //
    // Creates a new button at the (iX,iY) coordinates.  The Width and Height are optional, depending on the needs and type of the button.
    // When these values are omitted, the button is automatially sized.  Negative values add padding to the automatic size calculation.
    // For example, NewButton(10,20,-40,0,"Ok") creates on "OK" button that automatically calculates the width and height of the button 
    // and add 40 pixels on either side of the "OK" for spacing.
    //
    // An overridden class object can be applied to get windows messages.  
    // SetMessageHandler() can also set a message handler without overriding the class.
    //
    // Other options:
    //
    // Style()      -- Set the style of the buttoin,i.e. Style("Panel"), Style("Windows"), Style("medium");
    // SetFgColor()     -- Set the text foreground color
    // SetBgColor()     -- Set the text background color
    // JustCenter()     -- Center the button in the X plane. 
    // See opt:: descriptions for more options
    //
    static CButton & NewButton(CButton * cUserButton,int ix,int iy,int iWidth,int iHeight,const wchar_t * sButtonText,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CButton & NewButton(CButton * cUserButton,int ix,int iy,int iWidth,const wchar_t * sButtonText,const cwfOpt & cwOpt = cwfOpt());

    // Create a new button on the window. 
    //
    // Creates a new button at the (iX,iY) coordinates.  The Width and Height are optional, depending on the needs and type of the button.
    // When these values are omitted, the button is automatially sized.  Negative values add padding to the automatic size calculation.
    // For example, NewButton(10,20,-40,0,"Ok") creates on "OK" button that automatically calculates the width and height of the button 
    // and add 40 pixels on either side of the "OK" for spacing.
    //
    // An overridden class object can be applied to get windows messages.  
    // SetMessageHandler() can also set a message handler without overriding the class.
    //
    // Other options:
    //
    // Style()      -- Set the style of the buttoin,i.e. Style("Panel"), Style("Windows"), Style("medium");
    // SetFgColor()     -- Set the text foreground color
    // SetBgColor()     -- Set the text background color
    // JustCenter()     -- Center the button in the X plane. 
    // See opt:: descriptions for more options
    //
    static CButton & NewButton(CButton * cUserButton,int ix,int iy,int iWidth,const wchar_t * sButtonText,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CButton & NewButton(CButton * cUserButton,int ix,int iy,const wchar_t * sButtonText,const cwfOpt & cwOpt = cwfOpt());

    // Create a new button on the window. 
    //
    // Creates a new button at the (iX,iY) coordinates.  The Width and Height are optional, depending on the needs and type of the button.
    // When these values are omitted, the button is automatially sized.  Negative values add padding to the automatic size calculation.
    // For example, NewButton(10,20,-40,0,"Ok") creates on "OK" button that automatically calculates the width and height of the button 
    // and add 40 pixels on either side of the "OK" for spacing.
    //
    // An overridden class object can be applied to get windows messages.  
    // SetMessageHandler() can also set a message handler without overriding the class.
    //
    // Other options:
    //
    // Style()      -- Set the style of the buttoin,i.e. Style("Panel"), Style("Windows"), Style("medium");
    // SetFgColor()     -- Set the text foreground color
    // SetBgColor()     -- Set the text background color
    // JustCenter()     -- Center the button in the X plane. 
    // See opt:: descriptions for more options
    //
    static CButton & NewButton(CButton * cUserButton,int ix,int iy,const wchar_t * sButtonText,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CButton & NewCheckbox(int ix,int iy,int iWidth,int iHeight,const char * sName,const cwfOpt & cwOpt = cwfOpt());

    // Create a new checkbox on the window. 
    //
    // A checkbox is the same as a button, but with a checkbox appearance and "checKed" and "unchecked" status
    //
    // Creates a new button at the (iX,iY) coordinates.  The Width and Height are optional, depending on the needs and type of the button.
    // When these values are omitted, the button is automatially sized.  Negative values add padding to the automatic size calculation.
    // For example, NewButton(10,20,-40,0,"Ok") creates on "OK" button that automatically calculates the width and height of the button 
    // and add 40 pixels on either side of the "OK" for spacing.
    //
    // An overridden class object can be applied to get windows messages.  
    // SetMessageHandler() can also set a message handler without overriding the class.
    //
    // Other options:
    //
    // Style()      -- Set the style of the buttoin,i.e. Style("Panel"), Style("Windows"), Style("medium");
    // SetFgColor()     -- Set the text foreground color
    // SetBgColor()     -- Set the text background color
    // JustCenter()     -- Center the button in the X plane. 
    // See opt:: descriptions for more options
    //
    static CButton & NewCheckbox(int ix,int iy,int iWidth,int iHeight,const char * sName,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CButton & NewCheckbox(int ix,int iy,int iWidth,const char * sName,const cwfOpt & cwOpt = cwfOpt());

    // Create a new checkbox on the window. 
    //
    // A checkbox is the same as a button, but with a checkbox appearance and "checKed" and "unchecked" status
    //
    // Creates a new button at the (iX,iY) coordinates.  The Width and Height are optional, depending on the needs and type of the button.
    // When these values are omitted, the button is automatially sized.  Negative values add padding to the automatic size calculation.
    // For example, NewButton(10,20,-40,0,"Ok") creates on "OK" button that automatically calculates the width and height of the button 
    // and add 40 pixels on either side of the "OK" for spacing.
    //
    // An overridden class object can be applied to get windows messages.  
    // SetMessageHandler() can also set a message handler without overriding the class.
    //
    // Other options:
    //
    // Style()      -- Set the style of the buttoin,i.e. Style("Panel"), Style("Windows"), Style("medium");
    // SetFgColor()     -- Set the text foreground color
    // SetBgColor()     -- Set the text background color
    // JustCenter()     -- Center the button in the X plane. 
    // See opt:: descriptions for more options
    //
    static CButton & NewCheckbox(int ix,int iy,int iWidth,const char * sName,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CButton & NewCheckbox(int ix,int iy,const char * sName,const cwfOpt & cwOpt = cwfOpt());

    // Create a new checkbox on the window. 
    //
    // A checkbox is the same as a button, but with a checkbox appearance and "checKed" and "unchecked" status
    //
    // Creates a new button at the (iX,iY) coordinates.  The Width and Height are optional, depending on the needs and type of the button.
    // When these values are omitted, the button is automatially sized.  Negative values add padding to the automatic size calculation.
    // For example, NewButton(10,20,-40,0,"Ok") creates on "OK" button that automatically calculates the width and height of the button 
    // and add 40 pixels on either side of the "OK" for spacing.
    //
    // An overridden class object can be applied to get windows messages.  
    // SetMessageHandler() can also set a message handler without overriding the class.
    //
    // Other options:
    //
    // Style()      -- Set the style of the buttoin,i.e. Style("Panel"), Style("Windows"), Style("medium");
    // SetFgColor()     -- Set the text foreground color
    // SetBgColor()     -- Set the text background color
    // JustCenter()     -- Center the button in the X plane. 
    // See opt:: descriptions for more options
    //
    static CButton & NewCheckbox(int ix,int iy,const char * sName,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CButton & NewCheckbox(CButton * cUserButton,int ix,int iy,int iWidth,int iHeight,const char * sName,const cwfOpt & cwOpt = cwfOpt());

    // Create a new checkbox on the window. 
    //
    // A checkbox is the same as a button, but with a checkbox appearance and "checKed" and "unchecked" status
    //
    // Creates a new button at the (iX,iY) coordinates.  The Width and Height are optional, depending on the needs and type of the button.
    // When these values are omitted, the button is automatially sized.  Negative values add padding to the automatic size calculation.
    // For example, NewButton(10,20,-40,0,"Ok") creates on "OK" button that automatically calculates the width and height of the button 
    // and add 40 pixels on either side of the "OK" for spacing.
    //
    // An overridden class object can be applied to get windows messages.  
    // SetMessageHandler() can also set a message handler without overriding the class.
    //
    // Other options:
    //
    // Style()      -- Set the style of the buttoin,i.e. Style("Panel"), Style("Windows"), Style("medium");
    // SetFgColor()     -- Set the text foreground color
    // SetBgColor()     -- Set the text background color
    // JustCenter()     -- Center the button in the X plane. 
    // See opt:: descriptions for more options
    //
    static CButton & NewCheckbox(CButton * cUserButton,int ix,int iy,int iWidth,int iHeight,const char * sName,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CButton & NewCheckbox(CButton * cUserButton,int ix,int iy,int iWidth,const char * sName,const cwfOpt & cwOpt = cwfOpt());

    // Create a new checkbox on the window. 
    //
    // A checkbox is the same as a button, but with a checkbox appearance and "checKed" and "unchecked" status
    //
    // Creates a new button at the (iX,iY) coordinates.  The Width and Height are optional, depending on the needs and type of the button.
    // When these values are omitted, the button is automatially sized.  Negative values add padding to the automatic size calculation.
    // For example, NewButton(10,20,-40,0,"Ok") creates on "OK" button that automatically calculates the width and height of the button 
    // and add 40 pixels on either side of the "OK" for spacing.
    //
    // An overridden class object can be applied to get windows messages.  
    // SetMessageHandler() can also set a message handler without overriding the class.
    //
    // Other options:
    //
    // Style()      -- Set the style of the buttoin,i.e. Style("Panel"), Style("Windows"), Style("medium");
    // SetFgColor()     -- Set the text foreground color
    // SetBgColor()     -- Set the text background color
    // JustCenter()     -- Center the button in the X plane. 
    // See opt:: descriptions for more options
    //
    static CButton & NewCheckbox(CButton * cUserButton,int ix,int iy,int iWidth,const char * sName,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CButton & NewCheckbox(CButton * cUserButton,int ix,int iy,const char * sName,const cwfOpt & cwOpt = cwfOpt());

    // Create a new checkbox on the window. 
    //
    // A checkbox is the same as a button, but with a checkbox appearance and "checKed" and "unchecked" status
    //
    // Creates a new button at the (iX,iY) coordinates.  The Width and Height are optional, depending on the needs and type of the button.
    // When these values are omitted, the button is automatially sized.  Negative values add padding to the automatic size calculation.
    // For example, NewButton(10,20,-40,0,"Ok") creates on "OK" button that automatically calculates the width and height of the button 
    // and add 40 pixels on either side of the "OK" for spacing.
    //
    // An overridden class object can be applied to get windows messages.  
    // SetMessageHandler() can also set a message handler without overriding the class.
    //
    // Other options:
    //
    // Style()      -- Set the style of the buttoin,i.e. Style("Panel"), Style("Windows"), Style("medium");
    // SetFgColor()     -- Set the text foreground color
    // SetBgColor()     -- Set the text background color
    // JustCenter()     -- Center the button in the X plane. 
    // See opt:: descriptions for more options
    //
    static CButton & NewCheckbox(CButton * cUserButton,int ix,int iy,const char * sName,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CButton & NewRadioButton(int ix,int iy,int iWidth,int iHeight,const char * sName,const cwfOpt & cwOpt = cwfOpt());

    // Create a new checkbox on the window. 
    //
    // A checkbox is the same as a button, but with a checkbox appearance and "checKed" and "unchecked" status
    //
    // Creates a new button at the (iX,iY) coordinates.  The Width and Height are optional, depending on the needs and type of the button.
    // When these values are omitted, the button is automatially sized.  Negative values add padding to the automatic size calculation.
    // For example, NewButton(10,20,-40,0,"Ok") creates on "OK" button that automatically calculates the width and height of the button 
    // and add 40 pixels on either side of the "OK" for spacing.
    //
    // An overridden class object can be applied to get windows messages.  
    // SetMessageHandler() can also set a message handler without overriding the class.
    //
    // Other options:
    //
    // Style()      -- Set the style of the buttoin,i.e. Style("Panel"), Style("Windows"), Style("medium");
    // SetFgColor()     -- Set the text foreground color
    // SetBgColor()     -- Set the text background color
    // JustCenter()     -- Center the button in the X plane. 
    // See opt:: descriptions for more options
    //
    static CButton & NewRadioButton(int ix,int iy,int iWidth,int iHeight,const char * sName,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CButton & NewRadioButton(int ix,int iy,int iWidth,const char * sName,const cwfOpt & cwOpt = cwfOpt());

    // Create a new checkbox on the window. 
    //
    // A checkbox is the same as a button, but with a checkbox appearance and "checKed" and "unchecked" status
    //
    // Creates a new button at the (iX,iY) coordinates.  The Width and Height are optional, depending on the needs and type of the button.
    // When these values are omitted, the button is automatially sized.  Negative values add padding to the automatic size calculation.
    // For example, NewButton(10,20,-40,0,"Ok") creates on "OK" button that automatically calculates the width and height of the button 
    // and add 40 pixels on either side of the "OK" for spacing.
    //
    // An overridden class object can be applied to get windows messages.  
    // SetMessageHandler() can also set a message handler without overriding the class.
    //
    // Other options:
    //
    // Style()      -- Set the style of the buttoin,i.e. Style("Panel"), Style("Windows"), Style("medium");
    // SetFgColor()     -- Set the text foreground color
    // SetBgColor()     -- Set the text background color
    // JustCenter()     -- Center the button in the X plane. 
    // See opt:: descriptions for more options
    //
    static CButton & NewRadioButton(int ix,int iy,int iWidth,const char * sName,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CButton & NewRadioButton(int ix,int iy,const char * sName,const cwfOpt & cwOpt = cwfOpt());

    // Create a new checkbox on the window. 
    //
    // A checkbox is the same as a button, but with a checkbox appearance and "checKed" and "unchecked" status
    //
    // Creates a new button at the (iX,iY) coordinates.  The Width and Height are optional, depending on the needs and type of the button.
    // When these values are omitted, the button is automatially sized.  Negative values add padding to the automatic size calculation.
    // For example, NewButton(10,20,-40,0,"Ok") creates on "OK" button that automatically calculates the width and height of the button 
    // and add 40 pixels on either side of the "OK" for spacing.
    //
    // An overridden class object can be applied to get windows messages.  
    // SetMessageHandler() can also set a message handler without overriding the class.
    //
    // Other options:
    //
    // Style()      -- Set the style of the buttoin,i.e. Style("Panel"), Style("Windows"), Style("medium");
    // SetFgColor()     -- Set the text foreground color
    // SetBgColor()     -- Set the text background color
    // JustCenter()     -- Center the button in the X plane. 
    // See opt:: descriptions for more options
    //
    static CButton & NewRadioButton(int ix,int iy,const char * sName,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CButton & NewRadioButton(CButton * cUserButton,int ix,int iy,int iWidth,int iHeight,const char * sName,const cwfOpt & cwOpt = cwfOpt());

    // Create a new RadioButton on the window. 
    //
    // A checkbox is the same as a button, but with a checkbox appearance and "checKed" and "unchecked" status, where one
    // Radio button is checked at a time.  Use Group() as options to automatically have SageBox check and uncheck radio button
    // values.  Otherwise, these must be handled individually.
    //
    // Creates a new button at the (iX,iY) coordinates.  The Width and Height are optional, depending on the needs and type of the button.
    // When these values are omitted, the button is automatially sized.  Negative values add padding to the automatic size calculation.
    // For example, NewButton(10,20,-40,0,"Ok") creates on "OK" button that automatically calculates the width and height of the button 
    // and add 40 pixels on either side of the "OK" for spacing.
    //
    // An overridden class object can be applied to get windows messages.  
    // SetMessageHandler() can also set a message handler without overriding the class.
    //
    // Other options:
    //
    // Style()      -- Set the style of the buttoin,i.e. Style("Panel"), Style("Windows"), Style("medium");
    // SetFgColor()     -- Set the text foreground color
    // SetBgColor()     -- Set the text background color
    // JustCenter()     -- Center the button in the X plane. 
    // See opt:: descriptions for more options
    //
    static CButton & NewRadioButton(CButton * cUserButton,int ix,int iy,int iWidth,int iHeight,const char * sName,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CButton & NewRadioButton(CButton * cUserButton,int ix,int iy,int iWidth,const char * sName,const cwfOpt & cwOpt = cwfOpt());

    // Create a new RadioButton on the window. 
    //
    // A checkbox is the same as a button, but with a checkbox appearance and "checKed" and "unchecked" status, where one
    // Radio button is checked at a time.  Use Group() as options to automatically have SageBox check and uncheck radio button
    // values.  Otherwise, these must be handled individually.
    //
    // Creates a new button at the (iX,iY) coordinates.  The Width and Height are optional, depending on the needs and type of the button.
    // When these values are omitted, the button is automatially sized.  Negative values add padding to the automatic size calculation.
    // For example, NewButton(10,20,-40,0,"Ok") creates on "OK" button that automatically calculates the width and height of the button 
    // and add 40 pixels on either side of the "OK" for spacing.
    //
    // An overridden class object can be applied to get windows messages.  
    // SetMessageHandler() can also set a message handler without overriding the class.
    //
    // Other options:
    //
    // Style()      -- Set the style of the buttoin,i.e. Style("Panel"), Style("Windows"), Style("medium");
    // SetFgColor()     -- Set the text foreground color
    // SetBgColor()     -- Set the text background color
    // JustCenter()     -- Center the button in the X plane. 
    // See opt:: descriptions for more options
    //
    static CButton & NewRadioButton(CButton * cUserButton,int ix,int iy,int iWidth,const char * sName,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CButton & NewRadioButton(CButton * cUserButton,int ix,int iy,const char * sName,const cwfOpt & cwOpt = cwfOpt());

    // Create a new RadioButton on the window. 
    //
    // A checkbox is the same as a button, but with a checkbox appearance and "checKed" and "unchecked" status, where one
    // Radio button is checked at a time.  Use Group() as options to automatically have SageBox check and uncheck radio button
    // values.  Otherwise, these must be handled individually.
    //
    // Creates a new button at the (iX,iY) coordinates.  The Width and Height are optional, depending on the needs and type of the button.
    // When these values are omitted, the button is automatially sized.  Negative values add padding to the automatic size calculation.
    // For example, NewButton(10,20,-40,0,"Ok") creates on "OK" button that automatically calculates the width and height of the button 
    // and add 40 pixels on either side of the "OK" for spacing.
    //
    // An overridden class object can be applied to get windows messages.  
    // SetMessageHandler() can also set a message handler without overriding the class.
    //
    // Other options:
    //
    // Style()      -- Set the style of the buttoin,i.e. Style("Panel"), Style("Windows"), Style("medium");
    // SetFgColor()     -- Set the text foreground color
    // SetBgColor()     -- Set the text background color
    // JustCenter()     -- Center the button in the X plane. 
    // See opt:: descriptions for more options
    //
    static CButton & NewRadioButton(CButton * cUserButton,int ix,int iy,const char * sName,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: bool SetWinLocation(POINT pLocation);

    // Sets the location if the window on the desktop
    //
    static bool SetWinLocation(POINT pLocation);

    //-->  Original Prototype: bool SetWinLocation(int iX,int iY);

    // Sets the location if the window on the desktop
    //
    static bool SetWinLocation(int iX,int iY);

    //-->  Original Prototype: POINT GetWinLocation();

    // Gets the location of the window on the desktop
    //
    static POINT GetWinLocation();

    //-->  Original Prototype: HWND GetWindowHandle();

    // Gets the HWND handle to the window that Windows uses and originally assigned to the window.
    // This can be used to perform windows functions directly.
    //
    // Also see GetCurDC()
    //
    static HWND GetWindowHandle();

    //-->  Original Prototype: bool ClearFlags();

    // ** This is experimental **
    // Clears all event flags before entering WaitforEventLoop()
    // Again, experimental and may be deprecated.
    //
    static bool ClearFlags();

    //-->  Original Prototype: SIZE GetWindowSize(bool bFrameSize = false);

    // Returns the Windows Size, either the visible canvas size or actual window size including the frame and title bar.
    //
    // The Windows has three components:  The Window itself (frame, title bar, etc.), visible canvas (the visible part of the window
    // that can be drawn on), and the entire canvas (the entire writeable part of the internal bitmap that may be larger than displayed in the window).
    //
    // GetWindowSize() returns the visible canvas size so that where to put objects may be calculated.
    // GetWindowSize(true) returns the entire visible window size, including the frame, title bar, and borders. 
    //
    // See GetCanvasSize() to retrieve the size of the entire canvas that may exceed the visible window
    //
    static SIZE GetWindowSize(bool bFrameSize = false);

    //-->  Original Prototype: int GetID();

    // Returns the ID assigned to the Window when ID() was used to create the window.
    //
    static int GetID();

    //-->  Original Prototype: const char * GetName();

    // Returns the Name assigned to the window when Name() was used to create the window
    //
    static const char * GetName();

    //-->  Original Prototype: bool SetHoverMsg(const char * sHoverMessage);

    // Sets a hover message to be shown when the mouse hovers over the window
    //
    static bool SetHoverMsg(const char * sHoverMessage);

    //-->  Original Prototype: int    StartX();

    // The the X coordinare of the left part of the window
    //
    static int StartX();

    //-->  Original Prototype: int    StartY();

    // The the Y coordinae of the upper part of the window
    //
    static int StartY();

    //-->  Original Prototype: int    EndX();

    // The the X coordinare of the right part of the last window
    //
    static int EndX();

    //-->  Original Prototype: int    EndY();

    // The the Y coordinare of the bottom part of the window
    //
    static int EndY();

    //-->  Original Prototype: int GetWindowWidth(bool bFrameSize = false);

    // GetWindowWidth() - returns the width of the displayed canvas of the window.
    // Use GetWindoWidth(true) to get the full width of the widow, including the frame.
    // See GetWindowSize() to return both Width and Height in SIZE structure.
    //
    static int GetWindowWidth(bool bFrameSize = false);

    //-->  Original Prototype: int GetWindowHeight(bool bFrameSize = false);

    // GetWindowHeight() - returns the height of the displayed canvas of the window.
    // Use GetWindowHeight(true) to get the full width of the height, including the frame.
    // See GetWindowSize() to return both Width and Height in SIZE structure.
    //
    static int GetWindowHeight(bool bFrameSize = false);

    //-->  Original Prototype: bool isVisible();

    // Returns true if the Window is showing on the desktop.  False is returned if the Window is invisible.
    //
    static bool isVisible();

    //-->  Original Prototype: bool isValid();

    // Returns true of the window object is a valid window.
    // False is returned if not.  This can happen when a window is retrieved that is not valid, or a 
    // Window has been closed. 
    //
    // For example using auto& MyWindow = window("MyWindow") will return an empty window if a window
    // with the Name("MyWindow") does not exist.  In which case, isValid() can be used to determine this is an empty window.
    //
    static bool isValid();

    //-->  Original Prototype: CBitmap GetWindowBitmap(POINT pLoc,SIZE szSize,bool bDesktopView = false);

    // GetWindowBitmap() -- Fills a RawBitmap_t structure with the contents of the window, which can then be used for blending
    // and other functions.
    //
    // The parameter bDesktopView does the following:
    // 
    //      bDesktopView == true         -- Gets the window as seen on the desktop, including any child controls (Widgets, buttons, sub-windows) showing
    //                                      in the window.  This will not capture other windows overlapping the window, and will return the pure desktop bitmap
    //                                      for the window
    //
    //      bDesktopView = false         -- Gets the internal bitmap.  This will only return the graphics output to the bitmap, such as writing (printf, write(), etc.)
    //                                      or drawn items such as lines, bitmaps, etc.  Any child controls (i.e. buttons, sliders, text widgets, etc.) will not be 
    //                                      copied as part of the bitmap and the area underneath the control will be returned in the bitmap.
    //
    static CBitmap GetWindowBitmap(POINT pLoc,SIZE szSize,bool bDesktopView = false);

    //-->  Original Prototype: CBitmap GetWindowBitmap(bool bDesktopView = false);

    // GetWindowBitmap() -- Fills a RawBitmap_t structure with the contents of the window, which can then be used for blending
    // and other functions.
    //
    // The parameter bDesktopView does the following:
    // 
    //      bDesktopView == true         -- Gets the window as seen on the desktop, including any child controls (Widgets, buttons, sub-windows) showing
    //                                      in the window.  This will not capture other windows overlapping the window, and will return the pure desktop bitmap
    //                                      for the window
    //
    //      bDesktopView = false         -- Gets the internal bitmap.  This will only return the graphics output to the bitmap, such as writing (printf, write(), etc.)
    //                                      or drawn items such as lines, bitmaps, etc.  Any child controls (i.e. buttons, sliders, text widgets, etc.) will not be 
    //                                      copied as part of the bitmap and the area underneath the control will be returned in the bitmap.
    //
    static CBitmap GetWindowBitmap(bool bDesktopView = false);

    //-->  Original Prototype: bool SendWindowEvent(CWindow * cWin = nullptr);

    /// <summary>
    /// Sendes a generic message to a window.  This is used to send a Sage Event to a window and wake up a GetEvent() function waiting for a message
    /// <para></para>&#160;&#160;&#160;
    /// This is used when you want to have a message routine check for any status change.  This is sent to the window waiting for a message, either with GetEvent()
    /// or overriding OnSageEvent() in a window message handler.
    /// </summary>
    /// <param name="cWin">The window to receive the wake-up event.  if empty or nullptr, the event is sent to itself.</param>
    /// <returns></returns>
    static bool SendWindowEvent(CWindow * cWin = nullptr);

    //-->  Original Prototype: bool SendWidgetMessage(CWindow * cWin,void * cWidget = nullptr,int iMessage = 0);

    // SendWidgetMessage()
    // 
    // This is used by Widgets to send a generic message that can be intercepted with OnWidgetMessage() and also
    // causes EventLoop() and WaitforEvent() to return so the main procedure can look for WidgetMessages.
    //
    // This is also useful for Widgets to send messages to themselves to process elements in the main
    // thread rather than the calling thread of an interface function with unknown status (i.e. updates, etc.)
    // (this helps avoid multi-threading issues)
    //
    static bool SendWidgetMessage(CWindow * cWin,void * cWidget = nullptr,int iMessage = 0);

    //-->  Original Prototype: bool SendWidgetMessage(HWND hWndParent,void * cWidget = nullptr,int iMessage = 0);

    // SendWidgetMessage()
    // 
    // This is used by Widgets to send a generic message that can be intercepted with OnWidgetMessage() and also
    // causes EventLoop() and WaitforEvent() to return so the main procedure can look for WidgetMessages.
    //
    // This is also useful for Widgets to send messages to themselves to process elements in the main
    // thread rather than the calling thread of an interface function with unknown status (i.e. updates, etc.)
    // (this helps avoid multi-threading issues)
    //
    static bool SendWidgetMessage(HWND hWndParent,void * cWidget = nullptr,int iMessage = 0);

    //-->  Original Prototype: WaitEvent WaitforEvent(const char * sEvent = nullptr);

    // WaitforEvent() -- Wait for a user event, such as a mouse move, mouse click, button press, slider press, or any control or widget event.
    //
    // WaitforEvent() returns for relavent events from the user.  It does not return for all Windows events, only those that affect the running of the
    // program.  All Windows message events can be intercepted by subclassing the window or using SetMessageHandler() to capture all messages.
    //
    // ** This has been superceded by EventLoop() **
    //
    // WaitforEvent() returns if the window is closing with the WaitEvent::WindowClosing status. 
    // 
    // Important Note:  Make sure WindowEvent() does not return until it sees events.  With empty windows or corrupted windows, WaitforEvent()
    // can enter a processor-using wild loop.   When testing, it is a good idea to have printfs() to the window or console to make sure
    // only events are returned and it is not caught in a spining loop. 
    //
    static WaitEvent WaitforEvent(const char * sEvent = nullptr);

    //-->  Original Prototype: WaitEvent WaitforEvent(cwfEvent & cwEvent) { return WaitforEvent(*cwEvent);

    // WaitforEvent() -- Wait for a user event, such as a mouse move, mouse click, button press, slider press, or any control or widget event.
    //
    // WaitforEvent() returns for relavent events from the user.  It does not return for all Windows events, only those that affect the running of the
    // program.  All Windows message events can be intercepted by subclassing the window or using SetMessageHandler() to capture all messages.
    //
    // ** This has been superceded by EventLoop() **
    //
    // WaitforEvent() returns if the window is closing with the WaitEvent::WindowClosing status. 
    // 
    // Important Note:  Make sure WindowEvent() does not return until it sees events.  With empty windows or corrupted windows, WaitforEvent()
    // can enter a processor-using wild loop.   When testing, it is a good idea to have printfs() to the window or console to make sure
    // only events are returned and it is not caught in a spining loop. 
    //
    static WaitEvent WaitforEvent(cwfEvent & cwEvent);

    //-->  Original Prototype: bool EventLoop(WaitEvent * eStatus = nullptr);

    // EventLoop() -- Wait for a user event, such as a mouse move, mouse click, button press, slider press, or any control or widget event.
    //
    // This is the Main Event Loop for procedurally-driven programs to capture events without using event callbacks.
    //
    // EventLoop() returns for relavent events from the user.  It does not return for all Windows events, only those that affect the running of the
    // program.  All Windows message events can be intercepted by subclassing the window or using SetMessageHandler() to capture all messages.
    //
    //
    // EventLoop() returns if the window is closing with the WaitEvent::WindowClosing status. 
    // 
    // Important Note:  Make sure EventLoop() does not return until it sees events.  With empty windows or corrupted windows, EventLoop()
    // can enter a processor-using wild loop.   When testing, it is a good idea to have printfs() to the window or console to make sure
    // only events are returned and it is not caught in a spining loop. 
    //
    static bool EventLoop(WaitEvent * eStatus = nullptr);

    //-->  Original Prototype: bool GetEvent(WaitEvent * eStatus = nullptr);

    // GetEvent() -- Wait for a user event, such as a mouse move, mouse click, button press, slider press, or any control or widget event.
    //
    // This is the Main Event Loop for procedurally-driven programs to capture events without using event callbacks.
    //
    // EventLoop() returns for relavent events from the user.  It does not return for all Windows events, only those that affect the running of the
    // program.  All Windows message events can be intercepted by subclassing the window or using SetMessageHandler() to capture all messages.
    //
    //
    // EventLoop() returns if the window is closing with the WaitEvent::WindowClosing status. 
    // 
    // Important Note:  Make sure EventLoop() does not return until it sees events.  With empty windows or corrupted windows, EventLoop()
    // can enter a processor-using wild loop.   When testing, it is a good idea to have printfs() to the window or console to make sure
    // only events are returned and it is not caught in a spining loop. 
    //
    static bool GetEvent(WaitEvent * eStatus = nullptr);

    //-->  Original Prototype: bool WaitforClick(const char * sMsg = nullptr);

    /// <summary>
    /// Waits for a mouse click in the window.  This is used a simple way to hold up program execution until the user
    /// clicks the mouse.
    /// <para></para>&#160;&#160;&#160;
    /// The window is set as the foreground and active window when this function is called.
    /// --> Note: This function will return if the user closes the window.
    /// </summary>
    /// <param name="sMsg"> = (optional) message to print out as text to the window.</param>
    /// <returns></returns>
    static bool WaitforClick(const char * sMsg = nullptr);

    //-->  Original Prototype: char WaitforKeyPress(const char * sMsg = nullptr);

    /// <summary>
    /// Waits for a keypress from the user and returns the key pressed. This is used as a quick way to pause program execution
    /// until the user presses any key. 
    /// <para></para>&#160;&#160;&#160;
    /// The current window is set as the foreground window when this function is called.
    /// <para></para>
    /// --> Use getchar() for normal input.  WaitforKeyPress() is meant for pausing program execution
    /// --> Use WaitforCRPress() to wait specifically for a Carriage Return
    /// --> Note: This function will return with 0 if the user closes the window.
    /// </summary>
    /// <param name="sMsg"> = (optional) message to print out as text to the window.</param>
    /// <returns></returns>
    static char WaitforKeyPress(const char * sMsg = nullptr);

    //-->  Original Prototype: bool WaitforCRPress(const char * sMsg = nullptr);

    /// <summary>
    /// Waits for a Carriage Return press from the user. This is used as a quick way to pause program execution
    /// until the user presses the carriage return. 
    /// <para></para>&#160;&#160;&#160;
    /// The current window is set as the foreground window when this function is called.
    /// <para></para>
    /// --> Use getchar() for normal input.  WaitforKeyPress() is meant for pausing program execution
    /// --> Use WaitforKey() to wait for any key
    /// --> Note: This function will return with 0 if the user closes the window.
    /// </summary>
    /// <param name="sMsg"> = (optional) message to print out as text to the window.</param>
    /// <returns></returns>
    static bool WaitforCRPress(const char * sMsg = nullptr);

    //-->  Original Prototype: bool WaitforClickOrKey(const char * sMsg = nullptr);

   /// <summary>
    /// Waits for a keypress or mouse click from the user. This is used as a quick way to pause program execution
    /// until the user presses any key. 
    /// <para></para>&#160;&#160;&#160;
    /// The current window is set as the foreground window when this function is called.
    /// <para></para>
    /// --> Use getchar() for normal input.  WaitforKeyPress() is meant for pausing program execution
    /// --> Use WaitforCRPress() to wait specifically for a Carriage Return
    /// --> Note: This function will return with 0 if the user closes the window.
    /// </summary>
    /// <param name="sMsg"> = (optional) message to print out as text to the window.</param>
    /// <returns></returns>
    static bool WaitforClickOrKey(const char * sMsg = nullptr);

    //-->  Original Prototype: int WaitforClose(int iReturnValue = 0);

    // WaitforCLose() -- Wait for the window to close.  
    // This simply calls EventLoop() and only returns if the window is closing, ignoring all other events.
    // This can be a quick method to stop program execution and prevent exiting the program until the window is closed.
    //
    // WaitforClose() returns 0 by default.  if iReturnValue is specifid, this is the value that will be returned from the WaitforClose() call.
    // This is implemented specifically to allow int main-style() returns to avoid a singular return iValue; on the next line.
    // for example, using return WaitforClose(1234) will wait for the window close and return 1234 to the calling process. 
    //
    // If the window is invalid, -1 will be returned.
    //
    static int WaitforClose(int iReturnValue = 0);

    //-->  Original Prototype: bool WaitforMouseClick();

    // WaitforMouseClick() -- Waits for a mouse click in the current window. 
    // This function will return on receipt of a mouseclick or the window close, unless the automatic Window Close is disabled
    // (see DisableClose())
    //
    static bool WaitforMouseClick();

    //-->  Original Prototype: int ExitButton(const char * sText = nullptr);

    // ExitButton() -- Places a "Program Finished. Press Button to Continue" on the bottom of the screen and waits for input before
    // continuing.  This is useful when the program ends, to allow the user to press the button before the window closes.
    //
    // Exit Button return 0
    //
    static int ExitButton(const char * sText = nullptr);

    //-->  Original Prototype: int NoExitMsg(bool bNoExit = true);

    // NoExitMsg() -- Used for Sandbox applications to tell the calling process to not display the exit button w/message on the bottom of the window.
    //
    // NoExitMsg() returns 0;
    //
    static int NoExitMsg(bool bNoExit = true);

    //-->  Original Prototype: int NoExitMsg(int iReturnCode,bool bNoExit=true);

    // NoExitMsg() -- Used for Sandbox applications to tell the calling process to not display the exit button w/message on the bottom of the window.
    //
    // NoExitMsg() returns 0 unless a return code is specified.
    //
    static int NoExitMsg(int iReturnCode,bool bNoExit = true);

    //-->  Original Prototype: bool EnableWindow(bool bEnable = true);

    // EnableWindow() -- Enables or Disables the Window and all controls within the window
    //
    // Disabling the window an be useful to make sure no controls are used within the window, such as when dialog boxes are overlaid on top of the
    // window, or when the contex of the window currently makes no sense.
    //
    // EnableWindow() -- Enable the window
    // EnableWindow(false) -- Disable the window
    //
    static bool EnableWindow(bool bEnable = true);

    //-->  Original Prototype: bool DisableWindow(bool bDisable = true);

    // DisableWindow() -- Enables or Disables the Window and all controls within the window
    //
    // Disabling the window an be useful to make sure no controls are used within the window, such as when dialog boxes are overlaid on top of the
    // window, or when the contex of the window currently makes no sense.
    //
    // DisableWindow() -- Disable the window
    // DisableWindow(false) -- Enable the window
    //
    static bool DisableWindow(bool bDisable = true);

    //-->  Original Prototype: CBitmap CreateBitmap(int iWidth,int iHeight = 1);

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
    static CBitmap CreateBitmap(int iWidth,int iHeight = 1);

    //-->  Original Prototype: CBitmap CreateBitmap(SIZE szBitmapSize);

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
    static CBitmap CreateBitmap(SIZE szBitmapSize);

    //-->  Original Prototype: RawBitmap32_t GetBitmapStruct32(int iWidth,int iHeight = 1);

    // GetBitmapStruct32() -- Returns a 32-bit RawBitmap_t struct with memory for the Width and height.
    // If Height is omitted, a Bitmap structure of height 1 is returned.
    //
    // This is returned as an an bit-per-channel bitmap with 4 channels, the last of which may be
    // used as an Alpha channel.
    //
    // [[nodiscard]] -- Important note: This returns a RawBitmap_t structure which has allocate memory.
    // RawBitmap_t::Delete() must be called to delete this memory.
    //
    // Assign this to CBitmap, such as CBitmap cBitmap = GetWindowBitmap(); 
    // CBitmap will delete this memory automatically.  Otherwise, call RawBitmap_t::Delete() to make sure the memory is deleted
    //
    [[nodiscard]] static RawBitmap32_t GetBitmapStruct32(int iWidth,int iHeight = 1);

    //-->  Original Prototype: CEditBox & NewEditBox(int iX,int iY,int iWidth,int iHeight,cwfOpt & cwOpt);

    // NewEditBox -- Create a new edit box at (iX,iY) of specific width (and, optionally, height)
    // 
    // When the height is omitted, the EditBox is set to the height of the current font.  Otherwise, a much larger
    // height may be entered to create a multi-line edit box.
    //
    // See opt:: for controls that work for the edit box, such as Min(), Max(), Range(), etc.
    //
    // A default set of text may be entered, such as:
    //
    // NewEditbox(10,20,400,0,"This is sample Text"); 
    //
    // Events are returned such as editbox.ReturnedPressed() which is set when the return key is pressed or ESC key is pressed
    // (in which case an empty string is returned).
    //
    static CEditBox & NewEditBox(int iX,int iY,int iWidth,int iHeight,cwfOpt & cwOpt);

    //-->  Original Prototype: CInputBox & NewInputBox(int iX,int iY,int iWidth,int iHeight,cwfOpt & cwOpt) { return NewEditBox(iX,iY,iWidth,iHeight,cwOpt);

    static CInputBox & NewInputBox(int iX,int iY,int iWidth,int iHeight,cwfOpt & cwOpt);

    //-->  Original Prototype: CEditBox & NewEditBox(CEditBox * pObject,int iX,int iY,int iWidth,int iHeight,cwfOpt & cwOpt);

    // NewEditBox -- Create a new edit box at (iX,iY) of specific width (and, optionally, height)
    // 
    // When the height is omitted, the EditBox is set to the height of the current font.  Otherwise, a much larger
    // height may be entered to create a multi-line edit box.
    //
    // See opt:: for controls that work for the edit box, such as Min(), Max(), Range(), etc.
    //
    // A default set of text may be entered, such as:
    //
    // NewEditbox(10,20,400,0,"This is sample Text"); 
    //
    // Events are returned such as editbox.ReturnedPressed() which is set when the return key is pressed or ESC key is pressed
    // (in which case an empty string is returned).
    //
    static CEditBox & NewEditBox(CEditBox * pObject,int iX,int iY,int iWidth,int iHeight,cwfOpt & cwOpt);

    //-->  Original Prototype: CInputBox & NewInputBox(CInputBox * pObject,int iX,int iY,int iWidth,int iHeight,cwfOpt & cwOpt) { return NewEditBox(pObject,iX,iY,iWidth,iHeight,cwOpt);

    static CInputBox & NewInputBox(CInputBox * pObject,int iX,int iY,int iWidth,int iHeight,cwfOpt & cwOpt);

    //-->  Original Prototype: CEditBox & NewEditBox(int iX,int iY,int iWidth,int iHeight,const char * sText = nullptr,const cwfOpt & cwOpt = cwfOpt());

    // NewEditBox -- Create a new edit box at (iX,iY) of specific width (and, optionally, height)
    // 
    // When the height is omitted, the EditBox is set to the height of the current font.  Otherwise, a much larger
    // height may be entered to create a multi-line edit box.
    //
    // See opt:: for controls that work for the edit box, such as Min(), Max(), Range(), etc.
    //
    // A default set of text may be entered, such as:
    //
    // NewEditbox(10,20,400,0,"This is sample Text"); 
    //
    // Events are returned such as editbox.ReturnedPressed() which is set when the return key is pressed or ESC key is pressed
    // (in which case an empty string is returned).
    //
    static CEditBox & NewEditBox(int iX,int iY,int iWidth,int iHeight,const char * sText = nullptr,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CInputBox & NewInputBox(int iX,int iY,int iWidth,int iHeight,const char * sText = nullptr,const cwfOpt & cwOpt = cwfOpt()) { return NewEditBox(iX,iY,iWidth,iHeight,sText,cwOpt);

    static CInputBox & NewInputBox(int iX,int iY,int iWidth,int iHeight,const char * sText = nullptr,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CEditBox & NewEditBox(CEditBox * pObject,int iX,int iY,int iWidth,int iHeight,const char * sText = nullptr,const cwfOpt & cwOpt = cwfOpt());

    // NewEditBox -- Create a new edit box at (iX,iY) of specific width (and, optionally, height)
    // 
    // When the height is omitted, the EditBox is set to the height of the current font.  Otherwise, a much larger
    // height may be entered to create a multi-line edit box.
    //
    // See opt:: for controls that work for the edit box, such as Min(), Max(), Range(), etc.
    //
    // A default set of text may be entered, such as:
    //
    // NewEditbox(10,20,400,0,"This is sample Text"); 
    //
    // Events are returned such as editbox.ReturnedPressed() which is set when the return key is pressed or ESC key is pressed
    // (in which case an empty string is returned).
    //
    static CEditBox & NewEditBox(CEditBox * pObject,int iX,int iY,int iWidth,int iHeight,const char * sText = nullptr,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CInputBox & NewInputBox(CInputBox * pObject,int iX,int iY,int iWidth,int iHeight,const char * sText = nullptr,const cwfOpt & cwOpt = cwfOpt()) { return NewEditBox(pObject,iX,iY,iWidth,iHeight,sText,cwOpt);

    static CInputBox & NewInputBox(CInputBox * pObject,int iX,int iY,int iWidth,int iHeight,const char * sText = nullptr,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: bool CreateButtonStyle(const char * sStyleName,GraphicButtonStyle & stStyle);

    // CreateButtonStyle() -- Allows the creation of personalized, bitmap-based buttons.
    //
    // When the GraphicButtonStyle structure is filled out, the buttons are applied as shown in the GraphicButtonStyle naming.
    // The only button needed is the stNormal button, but it is good to have at least an stPressed, then followed by
    // other buttons to stylize the button display, such as highlighting, etc.
    //
    // note: Buttons are RawBitmap_t.  However, when bitmaps are loaded that are 32-bit bitmaps, the alpha 
    // channel is assigned to a map with RawBitmap_t.
    //
    // Therefore, if 32 bit bitmaps are provided, the buttons will be blended with the background, allowing
    // shaped buttons with transparent background and shadows.
    //
    static bool CreateButtonStyle(const char * sStyleName,GraphicButtonStyle & stStyle);

    //-->  Original Prototype: bool CreateCheckboxStyle(const char * sStyleName,GraphicCheckboxStyle & stStyle);

    // CreateCheckboxStyle() -- Allows the creation of personalized, bitmap-based buttons.
    //
    // When the GraphicCheckboxStyle structure is filled out, the buttons are applied as shown in the GraphicCheckboxStyle naming.
    // The only checkbox needed are the stBormal and stChecked graphics.  Other graphics stylize checkbox display, such as highlighting, etc.
    //
    // note: Checkboxes are RawBitmap_t.  However, when bitmaps are loaded that are 32-bit bitmaps, the alpha 
    // channel is assigned to a map with RawBitmap_t.
    //
    // Therefore, if 32 bit bitmaps are provided, the checkboxes will be blended with the background, allowing
    // shaped checkboxes with transparent background and shadows.
    //
    static bool CreateCheckboxStyle(const char * sStyleName,GraphicCheckboxStyle & stStyle);

    //-->  Original Prototype: bool CaptureMouse(bool bReleaseOnMouseUp = false);

    // CaptureMouse -- Keeps the mouse and keyboard entry tied to the window until the capture is release or
    // changed by Windows.
    //
    // This allows the window or control to keep the mouse input even when the mouse leaves the window.
    // Normally, when the mouse leaves the window, the window no longer gets mouse messages.
    //
    // With drawing and other routines, sometimes keeping the mouse input even ouside the window is important.
    // CaptureMouse() does this.
    //
    // The parameter, "bReleaseOnMouseUp" tells SageBox to release the capture when the user lifts the mouse button up
    // (this is assuming the capture was started because the mouse button is down).  In most cases, the capture is only
    // needed while the mouse button is down, and this provides the convenience of removing it afterwrd.
    // Default for bReleaseOnMouseUp is FALSE
    //
    // *** Otherwise, without "bReleaseOnMouseUp" TRUE, you must release the capture with MouseCapture() ***
    //
    // Note: The mouse capture can be released independently of ReleaseCapture() is MouseCaptured() or
    // catch the OnCaptureChange() Message (which is only called when the capture is released) to 
    // monitor the capture status
    //
    static bool CaptureMouse(bool bReleaseOnMouseUp = false);

    //-->  Original Prototype: bool ReleaseCapture();

    // ReleaseCapture() -- this releases a mouse capture in the current window or control
    // This can be used to release the capture, such as when the mouse button is released. 
    //
    // note: The capture can be released independently.  See isMouseCaptured() for more details
    //
    static bool ReleaseCapture();

    //-->  Original Prototype: bool isMouseCaptured();

    // Returns true of the mouse is capture for the window or control.
    // When CaptureMouse() is used, the mouse input is set to the window, even when the mouse is outside the window.
    // However, the capture can be released by Windows for various reasons.
    // OnCaptureChange() can be intercepted to catch this.  However, when in the EventLoop(),
    // 
    // isMouseCaptured() can be used to determine if the status has changed.
    // 
    // When the EventLoop() is used and MouseCapture() is called, it is a good idea to 
    // put isMouseCaptured() to monitor the status (unless using OnCaptureChanged())
    //
    static bool isMouseCaptured();

    //-->  Original Prototype: bool CaptureReleased(Peek peek = Peek::No);

    // Returns TRUE when the mouse capture has been release.  This only returns true when
    // the mouse capture previously engaged for the window has been released.
    //
    // This returns an event status.  Therefore, the event is reset after the call and will return false 
    // afterward until a subsequent release -- unless peek is set to Peek::No, in which case the status is not reset
    //
    static bool CaptureReleased(Peek peek = Peek::No);

    //-->  Original Prototype: bool Delete();

    // Delete the window.  This closes the window permanently.  All data associated with the window
    // is closed when the parent window is deleted. 
    //
    // If the window is subclassed with an object, this means the memory allocated by this object will not
    // be deleted right away.  Delete() can be overridden to do this, or deletObj() can be used to 
    // physically delete the Window (use carefully)
    //
    static bool Delete();

    //-->  Original Prototype: bool LockProcess();

    // LockProcess() -- Lock the window process for multi-threading capability. (i.e. set Busy status)
    //
    // This uses an Atomic machine code function to lock the process and make it Busy so that multi-threading may
    // occur safely.  LockProcess() uses a single lock (use LockProcess(iMyLock) for more locks) for the window and 
    // does not return until the current lock is Unlocked
    //
    static bool LockProcess();

    //-->  Original Prototype: bool UnlockProcess();

    // UnlockProcess() -- Unlocks the window process for multi-threading capability. (i.e. release Busy status)
    //
    // See LockProcess() for more details
    //
    static bool UnlockProcess();

    //-->  Original Prototype: bool LockProcess(int & iLock);

    // LockProcess(int & iLock) -- Lock the window process for multi-threading capability. (i.e. set Busy status)
    //
    // Important Note: make sure iLock is initially set to 0, otherwise this will cause a hang when
    // LockProcess() is called.
    //
    // This uses a user-specified lock so that multiple locks may be used without causing a hang.
    //
    // This uses an Atomic machine code function to lock the process and make it Busy so that multi-threading may
    // occur safely. 
    //
    // LockProcess(int & iLock) blocks and does not return until iLock is freed.
    //
    static bool LockProcess(int & iLock);

    //-->  Original Prototype: bool UnlockProcess(int & iLock);

    // UnlockProces(int & iLock) -- Unlock a specific lock to lock the process for multi-threading (i.e. release Busy status for the specific lock)
    //
    static bool UnlockProcess(int & iLock);

    //-->  Original Prototype: void SetDebugID(int iID);

    // SetDebugID() -- set a specific ID for the window. 
    //
    // Windows can be hard to debug, especially when there are a number of controls and other window. 
    // When looking at messages and other actions, it can be hard to determine which window or control is 
    // responsible.
    //
    // SetDebugID() can be used to set a specific Debug ID for the window so that it can be
    // trapped in debugging.
    //
    static void SetDebugID(int iID);

    //-->  Original Prototype: int GetDebugID();

    // GetDebugID() -- returns the debug ID set for the window.
    //
    // Set SetDebugID() for more information.
    //
    static int GetDebugID();

    //-->  Original Prototype: CTextWidget & TextWidget(int iX,int iY,int iWidth,int iHeight,const char * sMessage = nullptr,const cwfOpt & cwOpt = cwfOpt());

    static CTextWidget & TextWidget(int iX,int iY,int iWidth,int iHeight,const char * sMessage = nullptr,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CTextWidget & TextWidget(int iX,int iY,int iWidth,int iHeight,cwfOpt & cwOpt);

    static CTextWidget & TextWidget(int iX,int iY,int iWidth,int iHeight,cwfOpt & cwOpt);

    //-->  Original Prototype: CTextWidget & TextWidget(int iX,int iY,const char * sMessage = nullptr,const cwfOpt & cwOpt = cwfOpt());

    static CTextWidget & TextWidget(int iX,int iY,const char * sMessage = nullptr,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: int RegisterWidget(int & iRegistryID);

    // This is used to register a widget with SageBox.
    // The primary use for this is for the widget to register with Sagebox when called by Sagebox.
    // 
    // When Sagebox calls the Widget::Register() function, the widget calls this function,
    // which fille the provided RegistryID with an ID for the widget that it can use to work with dialog boxes
    // created by Sagebox.
    //
    // Note:: this is not a user function and is used by SageBox and Widgets
    // 
    static int RegisterWidget(int & iRegistryID);

    //-->  Original Prototype: CQuickDialog & QuickDialog(const char * sHeader = nullptr,const cwfOpt & cwOpt = CWindow::cwNoOpt);

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
    static CQuickDialog & QuickDialog(const char * sHeader = nullptr,const cwfOpt & cwOpt = CWindow::cwNoOpt);

    //-->  Original Prototype: CQuickDialog & QuickDialog(const cwfOpt & cwOpt);

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
    static CQuickDialog & QuickDialog(const cwfOpt & cwOpt);

    //-->  Original Prototype: CString GetOpenFile(stOpenFileStruct & stFile);

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
    static CString GetOpenFile(stOpenFileStruct & stFile);

    //-->  Original Prototype: CString GetOpenFile(const char * sTypes = nullptr);

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
    static CString GetOpenFile(const char * sTypes = nullptr);

    //-->  Original Prototype: bool GetOpenFile(stOpenFileStruct & stFile,CString & csFilename);

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
    static bool GetOpenFile(stOpenFileStruct & stFile,CString & csFilename);

    //-->  Original Prototype: bool GetOpenFile(const char * sTypes,CString & csFilename);

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
    static bool GetOpenFile(const char * sTypes,CString & csFilename);

    //-->  Original Prototype: bool GetOpenFile(CString & csFilename);

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
    static bool GetOpenFile(CString & csFilename);

    //-->  Original Prototype: CString GetSaveFile(stOpenFileStruct & stFile);

    static CString GetSaveFile(stOpenFileStruct & stFile);

    //-->  Original Prototype: CString GetSaveFile(const char * sTypes = nullptr);

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
    static CString GetSaveFile(const char * sTypes = nullptr);

    //-->  Original Prototype: bool GetSaveFile(stOpenFileStruct & stFile,CString & csFilename);

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
    static bool GetSaveFile(stOpenFileStruct & stFile,CString & csFilename);

    //-->  Original Prototype: bool GetSaveFile(const char * sTypes,CString & csFilename);

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
    static bool GetSaveFile(const char * sTypes,CString & csFilename);

    //-->  Original Prototype: bool GetSaveFile(CString & csFilename);

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
    static bool GetSaveFile(CString & csFilename);

    //-->  Original Prototype: stOpenFileStruct GetOpenFileStruct();

    // GetOpenFileStruct() -- Return an stOpenFileStruct to set for using an GetOpenFile() or GetSaveFile() dialog.
    //
    // This is a shortcut for declaring a structure directly, so that auto& stStruct = GetOpenFileStruct() can be used
    // more easily.
    //
    static stOpenFileStruct GetOpenFileStruct();

    //-->  Original Prototype: void WriteShadow(int iX,int iY,const char * sMessage,const cwfOpt & cwOpt = cwfOpt());

    // WriteShadow() -- Works like Write() (but is more limited), but writes the text with a shadow behind it.
    //
    // See Write() for more information
    //
    static void WriteShadow(int iX,int iY,const char * sMessage,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CMenu CreateMenu(int iBaseMenuValue = 0);

    // CreateMenu() -- Creates a menu that can be added as a main menu or sub menu to the window.
    //
    // Once a menu is created, items can be added through the CMenu class object returned.
    //
    // See CMenu documentation for more information.
    //
    [[nodiscard]] static CMenu CreateMenu(int iBaseMenuValue = 0);

    //-->  Original Prototype: CMenu GetMenu(HMENU hMenu,bool * bFound = nullptr);

    // Find a Windows menu.  If the HMENU (Windows Menu) value is a valid windows menu, a CMenu object is returned for this menu.
    //
    static CMenu GetMenu(HMENU hMenu,bool * bFound = nullptr);

    //-->  Original Prototype: bool GetMenu(HMENU hMenu,CMenu & cMenu);

    // Find a Windows menu.  If the HMENU (Windows Menu) value is a valid windows menu, a CMenu object is returned for this menu.
    //
    static bool GetMenu(HMENU hMenu,CMenu & cMenu);

    //-->  Original Prototype: bool MenuItemSelected(int & iMenuItem,Peek peek = Peek::No);

    // MenuItemSelected() -- This works as an event, and will fill the menu item if it is selected.
    // As an event, subsquent calls will return false and not fill the menu item until another menu item is selected.
    //
    // If the iMenuItem is not included in the call, GetMenuItem() can be used to retrieve the last menu item selected
    //
    static bool MenuItemSelected(int & iMenuItem,Peek peek = Peek::No);

    //-->  Original Prototype: bool MenuItemSelected(Peek peek = Peek::No);

    // MenuItemSelected() -- This works as an event, and will fill the menu item if it is selected.
    // As an event, subsquent calls will return false and not fill the menu item until another menu item is selected.
    //
    // If the iMenuItem is not included in the call, GetMenuItem() can be used to retrieve the last menu item selected
    //
    static bool MenuItemSelected(Peek peek = Peek::No);

    //-->  Original Prototype: int GetMenuItem();

    // Retrieves the last menu item selected.
    // 
    // This is meant to be used right after MenuItemSelected() is called, and will return the last menu item selected.
    //
    // This will continue to return the same menu item until a new menu item is selected.
    //
    static int GetMenuItem();

    //-->  Original Prototype: bool SetCloseButtonMenu(int iMenuItem = 0);

    static bool SetCloseButtonMenu(int iMenuItem = 0);

    //-->  Original Prototype: CBitmap ReadImageFile(const char * sPath,bool * bSuccess = nullptr);

    static CBitmap ReadImageFile(const char * sPath,bool * bSuccess = nullptr);

    //-->  Original Prototype: ImageStatus GetLastImageStatus();

    static ImageStatus GetLastImageStatus();

    //-->  Original Prototype: CBitmap ReadJpegFile(const char * sPath,bool * bSuccess = nullptr);

    // ReadJpegFile -- Read a jpeg file and store it into a CBitmap. 
    // This reads standard 8-bit jpeg (3-channel or monochrome).
    // If the file does not exist or is in an unsupported format, CBitmap will come back empty.
    // Use GetJpegError() to get the status of the last ReadJpegFile() call, which will
    // give information on why the Bitmap came back empty.
    //
    // if (bSuccess) is supplied, it is filled with true for a successful read, otherwise false.
    //
    static CBitmap ReadJpegFile(const char * sPath,bool * bSuccess = nullptr);

    //-->  Original Prototype: CBitmap ReadJpegMem(const unsigned char * sData,int iDataLength,bool * bSuccess);

    // ReadJpegMem -- Read a jpeg file already loaded into memory.
    // This reads standard 8-bit jpeg (3-channel or monochrome).
    // If the memory isin an unsupported format, CBitmap will come back empty.
    // Use GetJpegError() to get the status of the last ReadJpegFile() call, which will
    // give information on why the Bitmap came back empty.
    //
    // The memory and exact JPEG File length must be given.
    //
    // if (bSuccess) is supplied, it is filled with true for a successful read, otherwise false.
    //
    static CBitmap ReadJpegMem(const unsigned char * sData,int iDataLength,bool * bSuccess);

    //-->  Original Prototype: CJpeg::Status GetJpegError();

    // Returns the last Jpeg status.  This will return CJpeg::Status::Ok if there was no error, 
    // or an indication of what went wrong with the last call, such as CJpeg::Status::EmptyFilePath or CJpeg::Status::FileNotFound
    //
    static CJpeg::Status GetJpegError();

    //-->  Original Prototype: bool QuickThumbnail(RawBitmap_t & stBitmap,int iWidth,int iHeight,ThumbType eType = ThumbType::BestFit,const char * sTitle = nullptr);

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
    static bool QuickThumbnail(RawBitmap_t & stBitmap,int iWidth,int iHeight,ThumbType eType = ThumbType::BestFit,const char * sTitle = nullptr);

    //-->  Original Prototype: bool QuickThumbnail(RawBitmap_t & stBitmap,int iWidth,int iHeight,const char * sTitle);

    // Quick thumbnail -- Create and display a window with a thumbnail of bitmap data.
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
    static bool QuickThumbnail(RawBitmap_t & stBitmap,int iWidth,int iHeight,const char * sTitle);

    //-->  Original Prototype: bool QuickThumbnail(CBitmap & cBitmap,int iWidth,int iHeight,ThumbType eType = ThumbType::BestFit,const char * sTitle = nullptr);

    // Quick thumbnail -- Create and display a window with a thumbnail of bitmap data.
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
    static bool QuickThumbnail(CBitmap & cBitmap,int iWidth,int iHeight,ThumbType eType = ThumbType::BestFit,const char * sTitle = nullptr);

    //-->  Original Prototype: bool QuickThumbnail(CBitmap & cBitmap,int iWidth,int iHeight,const char * sTitle);

    // Quick thumbnail -- Create and display a window with a thumbnail of bitmap data.
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
    static bool QuickThumbnail(CBitmap & cBitmap,int iWidth,int iHeight,const char * sTitle);

    //-->  Original Prototype: CBitmap QuickResize(RawBitmap_t & stBitmap,int iWidth,int iHeight,ResizeType eType = ResizeType::BestFit);

    static CBitmap QuickResize(RawBitmap_t & stBitmap,int iWidth,int iHeight,ResizeType eType = ResizeType::BestFit);

    //-->  Original Prototype: CBitmap QuickResize(RawBitmap_t & stBitmap,SIZE szSize,ResizeType eType = ResizeType::BestFit);

    static CBitmap QuickResize(RawBitmap_t & stBitmap,SIZE szSize,ResizeType eType = ResizeType::BestFit);

    //-->  Original Prototype: CString getline(CString & cString,const cwfOpt & cwOpt = cwfOpt());

    // Duplication of C++ console-mode getline()
    //
    // This is the same as GetString() except that the sring input is specified in the function call.
    // This is to duplicate the C++ getline() function.
    //
    // CString and std::string are supported.
    //
    // See GetString() for option usage.
    //
    static CString getline(CString & cString,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CString __getline(CString & cString,const cwfOpt & cwOpt = cwfOpt());

    // Duplication of C++ console-mode getline()
    //
    // This is the same as GetString() except that the sring input is specified in the function call.
    // This is to duplicate the C++ getline() function.
    //
    // CString and std::string are supported.
    //
    // See GetString() for option usage.
    //
    static CString __getline(CString & cString,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CString getline(std::string & cString,const cwfOpt & cwOpt = cwfOpt());

    // Duplication of C++ console-mode getline()
    //
    // This is the same as GetString() except that the sring input is specified in the function call.
    // This is to duplicate the C++ getline() function.
    //
    // CString and std::string are supported.
    //
    // See GetString() for option usage.
    //
    static CString getline(std::string & cString,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CString __getline(std::string & cString,const cwfOpt & cwOpt = cwfOpt());

    // Duplication of C++ console-mode getline()
    //
    // This is the same as GetString() except that the sring input is specified in the function call.
    // This is to duplicate the C++ getline() function.
    //
    // CString and std::string are supported.
    //
    // See GetString() for option usage.
    //
    static CString __getline(std::string & cString,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CString getline(const cwfOpt & cwOpt = cwfOpt());

    // Duplication of C++ console-mode getline()
    //
    // This is the same as GetString() except that the sring input is specified in the function call.
    // This is to duplicate the C++ getline() function.
    //
    // CString and std::string are supported.
    //
    // See GetString() for option usage.
    //
    static CString getline(const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CString __getline(const cwfOpt & cwOpt = cwfOpt());

    // Duplication of C++ console-mode getline()
    //
    // This is the same as GetString() except that the sring input is specified in the function call.
    // This is to duplicate the C++ getline() function.
    //
    // CString and std::string are supported.
    //
    // See GetString() for option usage.
    //
    static CString __getline(const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: int getline(char * sString,int iMaxSize);

    // Duplication of C++ console-mode getline()
    //
    // This is the same as GetString() except that the sring input is specified in the function call.
    // This is to duplicate the C++ getline() function.
    //
    // CString and std::string are supported.
    //
    // See GetString() for option usage.
    //
    static int getline(char * sString,int iMaxSize);

    //-->  Original Prototype: int __getline(char * sString,int iMaxSize);

     // Duplication of C++ console-mode getline()
    //
    // This is the same as GetString() except that the sring input is specified in the function call.
    // This is to duplicate the C++ getline() function.
    //
    // CString and std::string are supported.
    //
    // See GetString() for option usage.
    //
    static int __getline(char * sString,int iMaxSize);

    //-->  Original Prototype: CListBox & NewListBox(int ix,int iy,int iWidth,int iHeight,const cwfOpt & cwOpt = cwfOpt());

    // NewListBox() -- Create a listbox in the window. 
    //
    // A listbox is created at (iX,iY) int the window with the Width and height specified.
    // The Height is adjusted to fit the number of lines available for the current font within the Height given.
    // If only a partial line will fit within the height, the height is reduced to fit the number of lines with no overlap. 
    //
    // The initial colors for the Listbox are the current window's background and foreground (text color).  
    // Use opt::WinColors() to Set the background and text color to default window colors (usually a 
    // white background with black text, but this is dependent on the user's settings). 
    //
    // Use opt::fgColor() or opt::TextColor() to specify a foreground/text color.
    // Use opt::bgColor() to set the background color. 
    // Use opt::Font() to set the font in the listbox. 
    //
    // Use CListBox::AddItem or CListBox::AddItems() to add elements to the listbox.
    //
    // if a CListBox * cListbox object is supplied, this is used as the object so that the listbox may be subclassed to your own class and object.
    // It is recommnded to use CListBox::SetMessageHandler() rather than creating a subclass, as it is easier. 
    //
    static CListBox & NewListBox(int ix,int iy,int iWidth,int iHeight,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CListBox & NewListBox(CListBox * cListBox,int ix,int iy,int iWidth,int iHeight,const cwfOpt & cwOpt = cwfOpt());

    // NewListBox() -- Create a listbox in the window. 
    //
    // A listbox is created at (iX,iY) int the window with the Width and height specified.
    // The Height is adjusted to fit the number of lines available for the current font within the Height given.
    // If only a partial line will fit within the height, the height is reduced to fit the number of lines with no overlap. 
    //
    // The initial colors for the Listbox are the current window's background and foreground (text color).  
    // Use opt::WinColors() to Set the background and text color to default window colors (usually a 
    // white background with black text, but this is dependent on the user's settings). 
    //
    // Use opt::fgColor() or opt::TextColor() to specify a foreground/text color.
    // Use opt::bgColor() to set the background color. 
    // Use opt::Font() to set the font in the listbox. 
    //
    // Use CListBox::AddItem or CListBox::AddItems() to add elements to the listbox.
    //
    // if a CListBox * cListbox object is supplied, this is used as the object so that the listbox may be subclassed to your own class and object.
    // It is recommnded to use CListBox::SetMessageHandler() rather than creating a subclass, as it is easier. 
    //
    static CListBox & NewListBox(CListBox * cListBox,int ix,int iy,int iWidth,int iHeight,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CComboBox &  NewComboBox(int iX,int iY,int iWidth,int iHeight,const cwfOpt & cwOpt = cwfOpt());

    static CComboBox & NewComboBox(int iX,int iY,int iWidth,int iHeight,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CComboBox &  NewComboBox(CComboBox * cComboBox,int iX,int iY,int iWidth,int iHeight,const cwfOpt & cwOpt = cwfOpt());

    static CComboBox & NewComboBox(CComboBox * cComboBox,int iX,int iY,int iWidth,int iHeight,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: bool SetEventWindow(CWindow * cWin = nullptr);

    static bool SetEventWindow(CWindow * cWin = nullptr);

    //-->  Original Prototype: bool SetEventWindow(CWindow & cWin);

    static bool SetEventWindow(CWindow & cWin);

    //-->  Original Prototype: bool SendEventsToParent();

    // SendEventsToParent() -- Send all events to Parent Window while also sending them to the Window itself.
    //
    // This function is mucnh like SetEventWindow() which will tell Sagebox where else to send all events through GetEvent().
    // Child windows send events to their parents (as well as themselves) by default, meaning you can use GetEvent() for the parent window
    // and receive events for the window itself.
    //
    // For windows created with Sagebox (and some other instances), there is no Event Window set, and to receive events through the
    // CSageBox::GetEvent() loop, SendEventsToParent() must be called -- this will send all events to the Sagebox main window (which is hidden)
    //
    static bool SendEventsToParent();

    //-->  Original Prototype: stControlLabel_t AddControlLabel(SizeRect srSize,const char * sText,LabelJust justType,bool bUpdate,const cwfOpt & cwOpt = cwfOpt());

    static stControlLabel_t AddControlLabel(SizeRect srSize,const char * sText,LabelJust justType,bool bUpdate,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: bool DrawLabelBox(POINT pLocation,SIZE szSize,const char * sTitle,bool bUpdate = true,BorderType eBorderType = BorderType::Depressed, LabelJust eLabelType = LabelJust::None,const cwfOpt & cwOpt = cwfOpt());

    static bool DrawLabelBox(POINT pLocation,SIZE szSize,const char * sTitle,bool bUpdate = true,BorderType eBorderType = BorderType::Depressed,LabelJust eLabelType = LabelJust::None,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: bool DrawLabelBox(POINT pLocation,SIZE szSize, const char * sTitle,const cwfOpt & cwOpt);

    static bool DrawLabelBox(POINT pLocation,SIZE szSize,const char * sTitle,const cwfOpt & cwOpt);

    //-->  Original Prototype: bool DrawLabelBox(POINT pLocation,SIZE szSize,const char * sTitle,bool bUpdate,const cwfOpt & cwOpt);

    static bool DrawLabelBox(POINT pLocation,SIZE szSize,const char * sTitle,bool bUpdate,const cwfOpt & cwOpt);

    //-->  Original Prototype: bool HideMenu();

    static bool HideMenu();

    //-->  Original Prototype: bool ShowMenu(CMenu & cMenu);

    static bool ShowMenu(CMenu & cMenu);

    //-->  Original Prototype: bool SetFullScreen(bool bFullscreen = true,bool bForceShow = false);

    static bool SetFullScreen(bool bFullscreen = true,bool bForceShow = false);

    //-->  Original Prototype: CDavinci * GetDavinciMain();

    static CDavinci * GetDavinciMain();

    //-->  Original Prototype: SIZE CreateRadioButtonGroup(int iNumButtons,int iX,int iY,const char * * sButtonNames,int iGroupID, const char * sLabel, const cwfOpt & cwOpt = cwfOpt());

    static SIZE CreateRadioButtonGroup(int iNumButtons,int iX,int iY,const char * * sButtonNames,int iGroupID,const char * sLabel,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: SIZE CreateCheckboxGroup(int iNumButtons,int iX,int iY,const char * * sButtonNames,int iGroupID, const char * sLabel, const cwfOpt & cwOpt = cwfOpt());

    static SIZE CreateCheckboxGroup(int iNumButtons,int iX,int iY,const char * * sButtonNames,int iGroupID,const char * sLabel,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: bool EnablePainting();

    static bool EnablePainting();

    //-->  Original Prototype: bool DisablePainting();

    static bool DisablePainting();

    //-->  Original Prototype: bool DisablePaintingSafe();

    static bool DisablePaintingSafe();

    //-->  Original Prototype: bool DontUpdate(bool bDontUpdate = true,bool bUpdateNow = false);

	/// <summary>
	/// Turns updating of the window off (including Windows WM_PAINT messages) until the next Update() is called or DontUpdate() is called with DontUpdate(false) to turn updates back on.
    /// <para></para>&#160;&#160;&#160;
    /// DontUpdate() is meant to prevent flickering for for local loops that put data to the screen rapidly.  For example, using Cls() and then placing graphics on the screen can
    /// flicker when the automatic update (or Window's WM_PAINT message) happens after the Cls() and all data is output to the window.
    /// <para></para>&#160;&#160;&#160;
    /// Since the next Update() call turns updates back on, and many Sagebox functions call Update() (such as CWindow::GetEvent()), be careful within the 
    /// DontUpdate() loop to not call certain Sagebox functions.
    /// <para></para>&#160;&#160;&#160;Use CWindow::UpdateOnce() to update the window when DontUpdate() is active.  This will update the window one time without restting the DontUpdate() status.
	/// </summary>
	/// <param name="bDontUpdate"> = When true, window updates are turned off; when false, window updating is turned back on</param>
	/// <param name="bUpdateNow"> = When true, an update is performed immediately to make sure the latest window data is showing.  The window is only updated if needed.</param>
	/// <returns>TRUE if Window is valid, FALSE if the Window is not valid.</returns>
    static bool DontUpdate(bool bDontUpdate = true,bool bUpdateNow = false);

    //-->  Original Prototype: bool UpdateOnce();

    /// <summary>
    /// Used to update the window once when updates are off (see DontUpdate())
    /// <para></para>&#160;&#160;&#160;
    /// This function forces an Update even when updates are off, allowing a loop with updates off to update one time
    /// without changing the DontUpdate() status (i.e. no further updates will occur until updates are turned back on).
    /// </summary>
    /// <returns></returns>
    static bool UpdateOnce();

    //-->  Original Prototype: bool SetasTopWindow();

    static bool SetasTopWindow();

    //-->  Original Prototype: bool SetasTopmostWindow();

    static bool SetasTopmostWindow();

    //-->  Original Prototype: bool DrawSimpleDoc(const unsigned char * sPgrData,const cwfOpt & cwOpt = cwfOpt());

    static bool DrawSimpleDoc(const unsigned char * sPgrData,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: bool DrawSimpleDoc(const char * sPgrPath, const cwfOpt & cwOpt = cwfOpt());

    static bool DrawSimpleDoc(const char * sPgrPath,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CWindow * DevGetWindow();

    // DevGetWindow() -- Returns the Window (i.e. CWindow) of the default DevWindow.  This can be used to access window functions for the 
    // the default DevWindow. 
    //
    // For user-created Dev Control Windows, use the CDevControls::GetWindow() function, i.e. cMyDevWindow->GetWindow(); 
    //
    static CWindow * DevGetWindow();

    //-->  Original Prototype: CWindow::WinGroup * DevGetGroup();

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
    static CWindow::WinGroup * DevGetGroup();

    //-->  Original Prototype: CDevControls * DevControlsWindow(int iX = -1, int iY = -1, const cwfOpt & cwOpt = cwfOpt());

    // QuickControlWindow() -- Create a QuickControls Window, allowing for quick creation and automatic placement of
    // controls (buttons, slider, editboxes, etc.) in the window.   This allows quick prototyping and development of non
    // GUI functions with GUI controls. 
    // 
    // See documentationn in CQuickControls.h for more information
    //
    // ** Important note ** the object pointer returned MUST BE DELETED, as it is not a managed object. 
    // Example code tends to use Obj<CQuickControls> cQuickControl = QuickControlWindow() to treat it as a stack object
    // that is automatically deleted when the current function (or class) goes out of scope.
    // (However, since this is usually only used for develpment, leaving it allocated prior to program end causes no problems)
    //
    static CDevControls * DevControlsWindow(int iX = -1,int iY = -1,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CDevControls * DevControlsWindow(const cwfOpt & cwOpt);

    // QuickControlWindow() -- Create a QuickControls Window, allowing for quick creation and automatic placement of
    // controls (buttons, slider, editboxes, etc.) in the window.   This allows quick prototyping and development of non
    // GUI functions with GUI controls. 
    // 
    // See documentationn in CQuickControls.h for more information
    //
    // ** Important note ** the object pointer returned MUST BE DELETED, as it is not a managed object. 
    // Example code tends to use Obj<CQuickControls> cQuickControl = QuickControlWindow() to treat it as a stack object
    // that is automatically deleted when the current function (or class) goes out of scope.
    // (However, since this is usually only used for develpment, leaving it allocated prior to program end causes no problems)
    //
    static CDevControls * DevControlsWindow(const cwfOpt & cwOpt);

    //-->  Original Prototype: CButton & DevButton(const char * sButtonName = nullptr,const cwfOpt & cwOpt = cwfOpt());

	// DevButton() -- Add a button to the Default Dev Control Window.  This accepts all options as normal buttons, but 
	// the default will add a regular button. 
	//
	// The Name used as a title for the button, but is optional. 
	//
    static CButton & DevButton(const char * sButtonName = nullptr,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CButton & DevCheckbox(const char * sCheckboxName = nullptr,const cwfOpt & cwOpt = cwfOpt());

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
    static CButton & DevCheckbox(const char * sCheckboxName = nullptr,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CSlider & DevSlider(const char * sSliderName = nullptr,const cwfOpt & cwOpt = cwfOpt());

    // QuickSlider() -- Add a slider to the Default Dev Controls Window.  The default width is 200 with a 0-100 range.  
    // The Range can be changed with default Slider options, i.e. opt::Range(0,200), for example, to set a range of 0-200.
	// -->
	// The title is displayed beneath the slider, as well as the value. 
	//
    static CSlider & DevSlider(const char * sSliderName = nullptr,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CEditBox & DevEditBox(const char * sEditBoxName = nullptr,const cwfOpt & cwOpt = cwfOpt());

    // DevEditBox() -- Add an EditBox to the default Dev control Window.  The sEditBoxTitle, while optional, will provide a
	// label to the left of the edit box.  The default width is 150 pixels or so, but can be changed with normal EditBox options
	//
    // Note: DevEditBox() and DevInputBox() are the same.  The term Editbox is deprecated and is used for consitency with Windows control names.
    //
    static CEditBox & DevEditBox(const char * sEditBoxName = nullptr,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CEditBox & DevInputBox(const char * sInputBoxName = nullptr,const cwfOpt & cwOpt = cwfOpt()) { return DevEditBox(sInputBoxName,cwOpt);

    // DevInputBox() -- Add an InputBox to the default Dev control Window.  The sInputBoxName, while optional, will provide a
	// label to the left of the Input Box.  The default width is 150 pixels or so, but can be changed with normal InputBox options
	//
    // Note: DevEditBox() and DevInputBox() are the same.  The term Editbox is deprecated and is used for consitency with Windows control names.
    //
    static CEditBox & DevInputBox(const char * sInputBoxName = nullptr,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CComboBox & DevComboBox(const char * sComboBoxName,const cwfOpt & cwOpt = cwfOpt());

    static CComboBox & DevComboBox(const char * sComboBoxName,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CWindow & DevWindow(const char * sTitle,int iNumlines,const cwfOpt & cwOpt = cwfOpt());

    static CWindow & DevWindow(const char * sTitle,int iNumlines,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CWindow & DevWindow(int iNumLines,const cwfOpt & cwOpt = cwfOpt());

    static CWindow & DevWindow(int iNumLines,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CWindow & DevWindow(const cwfOpt & cwOpt = cwfOpt());

    static CWindow & DevWindow(const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CWindow & DevWindow(const char * sTitle);

    static CWindow & DevWindow(const char * sTitle);

    //-->  Original Prototype: CTextWidget & DevText(const char * sText,const cwfOpt & cwOpt  = cwfOpt());

    static CTextWidget & DevText(const char * sText,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CTextWidget & DevText(const char * sText,int iHeight,const cwfOpt & cwOpt  = cwfOpt());

    static CTextWidget & DevText(const char * sText,int iHeight,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CTextWidget & DevText(int iHeight,const cwfOpt & cwOpt  = cwfOpt());

    static CTextWidget & DevText(int iHeight,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CTextWidget & DevText(const cwfOpt & cwOpt  = cwfOpt());

    static CTextWidget & DevText(const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: bool DevAddSection(const char * sSectionName = nullptr,const cwfOpt & cwOpt = cwfOpt());

 	// DevAddSection() -- Adds a text section to the default Dev Controls window, to separate types of controls.
	// You can use opt::fgColor() to set the text color of the section name.
	//
    static bool DevAddSection(const char * sSectionName = nullptr,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: CDevControls * GetDevControlsPtr();

    // GetDevControlsPtr() -- returns the pointer to the default CDevControls object. 
    // *** Important note *** -- this will return NULLPTR until a control is created with
    // QuickButton(), QuickSlider(), etc. The window is not created until a control is created in order
    // to save memory.
    //
    // With the pointer, any CDevContgrols() function can be used, though most are replicated directly
    // through CSageBox functions for ease of use.
    //
    static CDevControls * GetDevControlsPtr();

    //-->  Original Prototype: bool DevControlsTopmost(bool bTopmost = true);

    static bool DevControlsTopmost(bool bTopmost = true);

    //-->  Original Prototype: bool DisableClose(bool bDisable = true);

    /// <summary>
    /// Disable ablility for user to close the window by pressing the upper-right "X" button or 
    /// pressing ALT-F4
    /// <para></para>&#160;&#160;&#160;
    /// CloseButtonPressed() can be used to determine if the user pressed the close button (or ALT-F4), or the window
    /// has otherwise been slated to close by the system.
    /// </summary>
    /// <param name="bDisable">When true, disabled the window from close.  When false, allows the user to close the window</param>
    /// <returns></returns>
    static bool DisableClose(bool bDisable = true);

    //-->  Original Prototype: bool WindowCloseEnabled();

    /// <summary>
    /// Returns true if the window close has been disabled (i.e. ability for the user to close the window)
    /// <para></para>
    /// Use DisableClose() to disable and enable the ability for the user to close the window.
    /// </summary>
    /// <returns>TRUE when Window close is enabled; false if the window-close has been disabled</returns>
    static bool WindowCloseEnabled();

    //-->  Original Prototype: bool CloseWindow();

    static bool CloseWindow();

    //-->  Original Prototype: bool SetClsBitmap(RawBitmap_t & stBitmap,bool bClsNow = true);

    static bool SetClsBitmap(RawBitmap_t & stBitmap,bool bClsNow = true);

    //-->  Original Prototype: bool SetClsBitmap(CBitmap & cBitmap,bool bClsNow = true);

    static bool SetClsBitmap(CBitmap & cBitmap,bool bClsNow = true);

    //-->  Original Prototype: bool ClearClsBitmap();

    static bool ClearClsBitmap();

    //-->  Original Prototype: CBitmap & GetClsBitmap();

    static CBitmap & GetClsBitmap();

    //-->  Original Prototype: CBitmap ReadPgrBitmap(const char * sImageTitle,const char * sPgrFile,bool * bSuccess = nullptr);

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
    static CBitmap ReadPgrBitmap(const char * sImageTitle,const char * sPgrFile,bool * bSuccess = nullptr);

    //-->  Original Prototype: CBitmap ReadPgrBitmap(const char * sImageTitle,const unsigned char * sPGRMemory,bool * bSuccess = nullptr);

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
    static CBitmap ReadPgrBitmap(const char * sImageTitle,const unsigned char * sPGRMemory,bool * bSuccess = nullptr);

    //-->  Original Prototype: bool CreateButtonGroup(int iGroupID,int iNumButtons,const wchar_t * * sButtonNames,POINT pLocation,SIZE szSize,int iColumns = 0, SIZE szSpacing = {-1,-1}, const cwfOpt & cwOpt = cwfOpt());

    static bool CreateButtonGroup(int iGroupID,int iNumButtons,const wchar_t * * sButtonNames,POINT pLocation,SIZE szSize,int iColumns = 0,SIZE szSpacing = {-1,-1},const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: bool CreateButtonGroup(int iGroupID,int iNumButtons,const wchar_t * * sButtonNames,POINT pLocation,int iColumns = 0, const cwfOpt & cwOpt = cwfOpt());

    static bool CreateButtonGroup(int iGroupID,int iNumButtons,const wchar_t * * sButtonNames,POINT pLocation,int iColumns = 0,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: bool CreateButtonGroup(int iGroupID,int iNumButtons,const char * * sButtonNames,POINT pLocation,SIZE szSize,int iColumns = 0, SIZE szSpacing = {-1,-1}, const cwfOpt & cwOpt = cwfOpt());

    static bool CreateButtonGroup(int iGroupID,int iNumButtons,const char * * sButtonNames,POINT pLocation,SIZE szSize,int iColumns = 0,SIZE szSpacing = {-1,-1},const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: bool CreateButtonGroup(int iGroupID,int iNumButtons,const char * * sButtonNames,POINT pLocation,int iColumns = 0, const cwfOpt & cwOpt = cwfOpt());

    static bool CreateButtonGroup(int iGroupID,int iNumButtons,const char * * sButtonNames,POINT pLocation,int iColumns = 0,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: bool CreateButtonGroup(const char * sGroupName,int iNumButtons,const wchar_t * * sButtonNames,POINT pLocation,SIZE szSize,int iColumns = 0, SIZE szSpacing = {-1,-1}, const cwfOpt & cwOpt = cwfOpt());

    static bool CreateButtonGroup(const char * sGroupName,int iNumButtons,const wchar_t * * sButtonNames,POINT pLocation,SIZE szSize,int iColumns = 0,SIZE szSpacing = {-1,-1},const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: bool CreateButtonGroup(const char * sGroupName,int iNumButtons,const char * * sButtonNames,POINT pLocation,SIZE szSize,int iColumns = 0, SIZE szSpacing = {-1,-1}, const cwfOpt & cwOpt = cwfOpt());

    static bool CreateButtonGroup(const char * sGroupName,int iNumButtons,const char * * sButtonNames,POINT pLocation,SIZE szSize,int iColumns = 0,SIZE szSpacing = {-1,-1},const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: bool CreateButtonGroup(const char * sGroupName,int iNumButtons,const char * * sButtonNames,POINT pLocation,int iColumns = 0, const cwfOpt & cwOpt = cwfOpt());

    static bool CreateButtonGroup(const char * sGroupName,int iNumButtons,const char * * sButtonNames,POINT pLocation,int iColumns = 0,const cwfOpt & cwOpt = cwfOpt());

    //-->  Original Prototype: bool SetSignal(SignalEvents event,bool & bSignal);

    static bool SetSignal(SignalEvents event,bool & bSignal);

    //-->  Original Prototype: bool SetSignal(SignalEvents event,Signal & stSignal);

    static bool SetSignal(SignalEvents event,Signal & stSignal);

    //-->  Original Prototype: bool CancelSignal(SignalEvents event);

    static bool CancelSignal(SignalEvents event);

    //-->  Original Prototype: bool UpdateBg(bool bUpdateNow = false);

    static bool UpdateBg(bool bUpdateNow = false);

    //-->  Original Prototype: bool ThreadStopped();

    // Returns true of the main thread is stopped.  
    // Use StartThread() to resume the main thread. 
    //
    // This only applies to the original program thread and will not work for other threads created after
    // the program started.
    //
    static bool ThreadStopped();

    //-->  Original Prototype: ThreadStatus GetThreadStatus();

    // Returns the status of the thread.  ThreadStatus::Running or ThreadStatus::Suspended (if the thread is stopped).
    // This only applies to the original program thread and will not work for other threads created after
    // the program started.
    //
    static ThreadStatus GetThreadStatus();

    //-->  Original Prototype: int StopThread(bool bEndProgramOnClose = false,int iOkReturnValue = 0);

    // Stop the main thread.  This is usually used when ending the main thread and transferring to full event-driven control. 
    //
    // This only applies to the original program thread and will not work for other threads created after
    // the program started.
    //
    // When bEndProgramOnClose == true, the prgoram is ended when the the window the window is closed.
    // Otherwise it is up to the program code to call EndProgram() when responding to an event such as WindowClose, etc.
    //
    // iOkReturnValue is returned when bEndProgramClose == true and the window is closed by the user -- otherwise the program must
    // set the return code in EndProgram().  Since EndProgram is called automatically, iOkReturnValue provides a method to return 
    // an "Ok" or "Window Closed" return value.  The default value (when iOkReturnValue is not specified) is 0.
    // the value iReturnValue is returned from EndProgram() when it wakes up.
    //
    static int StopThread(bool bEndProgramOnClose = false,int iOkReturnValue = 0);

    //-->  Original Prototype: bool ResumeThread(int iValue = 0);

    // Resume the main thread if it is suspended.
    //
    // iValue will be returned by StopThread() when it is awakened. 0 is returned if it was awakened due to the window closing.
    //
    static bool ResumeThread(int iValue = 0);

    //-->  Original Prototype: bool EndProgram(int iValue = 0);

    // End the program when in the main message thread.  This is used when StopThread() has been used to stop the main thread but events are
    // still being handled through the Main Windows Message Thread.
    //
    // EndProgram() sets the window closing status and resumes the main thread.  Typically, the main thread will exit and SageBox will end.
    // However, StopThread() can be used anywhere and does not need to exit immediately.  It can take care of cleanup, memory deallocations, etc. 
    //
    // iValue will be returned by StopThread() when it is awakened. 0 is returned if it was awakened due to the window closing.
    //
    static bool EndProgram(int iValue = 0);

    //-->  Original Prototype: bool EndProgramOnClose(bool bEndOnClose);

    // EndProgramOnClose() -- When in Event-Driven mode and ThreadStop() has been called, this 
    // causes Sagebox to call EndProgram(0) to continue the thread with a window close status.
    // 
    // This can be useful as a way to automatically close the window.  Otherwise, when in event-driven mode (i.e. ThreadStop() has been called),
    // the OnClose() or WM_CLOSE messages need to be handled to call EndProgram() manually (otherwise the program may hang)
    //
    // Important Note: If the Window Close has been disabled this is ignored on Window Close.
    //
    // iReturnValue specifies the returend value on end program (default is 0)
    //
    static bool EndProgramOnClose(bool bEndOnClose);

    //-->  Original Prototype: bool EndProgramOnClose(int iReturnValue = 0);

    // EndProgramOnClose() -- When in Event-Driven mode and ThreadStop() has been called, this 
    // causes Sagebox to call EndProgram(0) to continue the thread with a window close status.
    // 
    // This can be useful as a way to automatically close the window.  Otherwise, when in event-driven mode (i.e. ThreadStop() has been called),
    // the OnClose() or WM_CLOSE messages need to be handled to call EndProgram() manually (otherwise the program may hang)
    //
    // Important Note: If the Window Close has been disabled this is ignored on Window Close.
    //
    // iReturnValue specifies the returend value on end program (default is 0)
    //
    static bool EndProgramOnClose(int iReturnValue = 0);

    //-->  Original Prototype: bool SendtoClipboard();

    // SendtoClipboard() -- Send contents of window to Windows Clipboard as a bitmap (so it may be pasted into other applications)
    //
    // If a Bitmap is included, the bitmap is sent to the clipoard instead.
    //
    // This transfers the visible canvas area only.  It does not copy the window frame or border. 
    //
    static bool SendtoClipboard();

    //-->  Original Prototype: bool SendtoClipboard(const CBitmap & cBitmap);

    // SendtoClipboard() -- Send contents of window to Windows Clipboard as a bitmap (so it may be pasted into other applications)
    //
    // If a Bitmap is included, the bitmap is sent to the clipoard instead.
    //
    // This transfers the visible canvas area only.  It does not copy the window frame or border. 
    //
    static bool SendtoClipboard(const CBitmap & cBitmap);

    //-->  Original Prototype: CString ImportClipboardText(bool * bSuccess = nullptr);

    // ImportClipboardText() -- Returns Text String in the Windows clipboard, if it exists.
    //
    // If there is text within the Windows Clipboard, a CString object will be returned with its contents.
    // Otherwise, an empty CString will be returned.
    //
    // a bSuccess pointer may be included which will be filled with the results (true if text was found, false if the CString returned is empty);
    //
    static CString ImportClipboardText(bool * bSuccess = nullptr);

    //-->  Original Prototype: CStringW ImportClipboardTextW(bool * bSuccess = nullptr);

    // ImportClipboardTextW() -- Returns a Unicode Text String in the Windows clipboard, if it exists.
    //
    // If there is text within the Windows Clipboard, a CStringW object will be returned with its contents.
    // Otherwise, an empty CStringW will be returned.
    //
    // a bSuccess pointer may be included which will be filled with the results (true if text was found, false if the CStringW returned is empty);
    //
    static CStringW ImportClipboardTextW(bool * bSuccess = nullptr);

    //-->  Original Prototype: CBitmap ImportClipboardBitmap(bool * bSuccess = nullptr);

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
    static CBitmap ImportClipboardBitmap(bool * bSuccess = nullptr);

    //-->  Original Prototype: bool isDevWindow() { return m_bDevWindow;

    static bool isDevWindow();

    //-->  Original Prototype: CSageBox * GetSageBox();

    static CSageBox * GetSageBox();

    //-->  Original Prototype: bool CenterInlineFonts(bool bCenterFonts = true);

    // CenterInlineFonts() -- When true, this will center the text when font changes occur in output with {<font>} changes.
    // For example Write("This is a {Arial45}Centered{/} Font" will center the larger font in the middle of the text line when
    // Centering is active.  Otherwise, it will put the font start at and all other text in the line at the same Y value. 
    //
    // This is primarily used for specialized text, such as a TextWidget()
    //
    static bool CenterInlineFonts(bool bCenterFonts = true);

    //-->  Original Prototype: bool SetWindowTitle(const char * sTitle);

    /// <summary>
    /// Sets the title of the Window (shown on the Window Title bar)
    /// </summary>
    /// <param name="sTitle">Name of the Window</param>
    /// <returns>TRUE if Window is valid.  FALSE is the Window is not valid.</returns>
    static bool SetWindowTitle(const char * sTitle);

    //-->  Original Prototype: bool SetWindowTitle(const wchar_t * sTitle);

    /// <summary>
    /// Sets the title of the Window (shown on the Window Title bar)
    /// </summary>
    /// <param name="sTitle">Name of the Window</param>
    /// <returns>TRUE if Window is valid.  FALSE is the Window is not valid.</returns>
    static bool SetWindowTitle(const wchar_t * sTitle);

    //-->  Original Prototype: CString GetVersionInfo();

                                                 /// <summary>
    /// Returns a string with version information for the Sagebox library, as well as current
    /// platform (i.e. 32 bits or 64 bits). 
    /// </summary>
    /// <returns>String with current library version and run-time platform</returns>
    static CString GetVersionInfo();

    //-->  Original Prototype: bool is64BitWindows();

    /// <summary>
    /// returns TRUE of running 64-bit windows, false if 32-it windows. 
    /// </summary>
    /// <returns></returns>
    static bool is64BitWindows();

    // -------------------------------------------------------------------------
    // $Automated_End(QuickCPPH)  -- Automated Interface for Quick CPP functions
    // -------------------------------------------------------------------------
};


    class CQuickCppInit
    {
    public:
        static void CreateQuickCPPWindow(bool bConsoleMode,const char * sTitle,bool bShowProcessWindow = true);
        static void CreateQuickCpp(bool bShowProcessWindow);
        static CSagebox cSagebox; 
        static CWindow * cWin;
    };




};   // namespace Sandbox

// Define the main window interface object. 
//
//      Note: this is a global, rather than being in the Sandbox namespace
//      (i.e. Sandbox::win as opposed to ::win) because Microsoft IDE intellisense 
//      was having a hard time dealing with it and was creating red squiqqly lines rather
//      then interpreting code correctly. 
//
//      Making it global seems to have cured the problem.  "win" will be moved 
//      to the Sandbox namespace if I find a way to clear this issue up (or Microsoft releases an update that
//      fixes it)

// Main Sagebox Quick C++ Window.  This can be used globally to use the main window
// in the Sagebox Quick C++ environment.  Call CreateQuickCpp() to start the environment
//
extern Sandbox::CQuickCpp win; 


namespace std
{
    int __printf(const char * Format,...);
    int __getline(char * sString,int iMaxLen);
    CString __getline(std::string & sString);
    CString __getline(); 
    CString __getline(CString & cString); 
    char __getchar(); 
    int  __putchar(char cChar);
    int __scanf(const char *format, ...);
};


// printf() -- print a message to the main window.  This is the same as std::printf. 
// (this is currently a define, to be changed to a regular function soon)
//

#ifndef __SAGE_SANDBOX_NOREASSIGN__
#ifdef __SAGE_SANDBOX_MERGED__
#define printf __printf
// Use getline() as a replacement for std::getline. 
// --> Note: This function is under construction and does not relate directly to std::getline()
// --> This will be addressed in a future version where std::cin input buffer is overriden
// --> The #define structure is a temporary method to route certain stdio, cin, and cout functions
// --> To the Sagebox Window
//
#define getline __getline 
#define getchar __getchar
#define scanf __scanf
#define putchar __putchar
//#define cout MainWin

#endif
//#define cout MainWin
using namespace Sandbox;
using namespace std;
#endif
#endif // __QuickCpp_h__
