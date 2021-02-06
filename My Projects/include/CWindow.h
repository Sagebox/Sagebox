// CWindow.H -- SageProx Project.   Copyright (c) 2020 Rob Nelson, all Rights Reserved.  RobNelsonxx2@gmail.com

// **************************************************************************************************************
// Note: This file is part of the SageBox Alpha release and is under construction
// This file was part of the original code used for private and consulting projects.
//
// Many non-public interface declarations and older C++ version code will be removed in the next updates.
// **************************************************************************************************************

// -------------------------------
// CWindow - CDavinci Window class
// -------------------------------
//
// CDavWndow is the class returned from creating a window in SageBox.  CDavinci is the class returned for the main SageBox Object from which all windows are created.
// CSageBox is a shell around CDavinci.  CDavinci is the main core interface object between the main Windows engine and SageBox functions.
//
// CWindow contains all functions related to SageBox windows, with functions to create controls such as buttons, edit boxes, as well as other funtions such as to display
// bitmaps, basic shapes (rectangles, ellipses, etc.) writing to the windows, etc. 
//
// Windows created through a CWindow object become the child of the window controlled by the initiating object, but otherwise are just another CWindow class, 
// with the only difference being the Windows (i.e. Win32) heirarchy (i.e. Parent->Child) relationship.
//
// various notes with '$$' represent in-progress TODO()-type notes as the interface updated to the next version
// $$PI are declarations being moved out of the public space since they are not used in the public interface for SageBox

//#pragma once   

#if !defined(_CDavWindow_H_)
#define _CDavWindow_H_

#include <string>
#include "SageOpt.h"
#include "CDevString.h"
#include "CString.h"
#include "DialogStruct.h"
#include "EventOpt.h"
#include "CPoint.h"
#include "CDavinci.h"
#include "CMenu.h"
#include "CJpeg.h"
#include "CWindowHandler.h"
#include "CStyleDefaults.h"
#include "Cpaswindow.h"
#include <vector>


namespace Sage
{
    class CButton;
    class CWindow;
    class CDavinci;
    class CEditBox;
    class CListBox;
    class CComboBox;
    class CTextWidget;
    class CDialog;
    class COutDC;
    class CSageBox;
    class CMenu;
    class CDevControls;
    struct CEWindow_t;
    typedef int ConsoleOp_t;        // Experimental

    // Structure used when using the OpenFile/SaveFile functions
    // $$ PI

    struct stOpenFileStruct
    {
    public:
        struct Types
        {
            CStringW csDesc;
            CStringW csTypes;
        };

        CStringW stTitle;
        CStringW stDefaultType; 
        CStringW stDefaultDir;
        std::vector<Types> stTypes;

        void AddType(const char * sDesc,const char * sExt);
        void AddType(const char * sExt);
        void SetDefaultType(const char * sType);
        void SetDefaultDir(const char * sDefaultDir);
        void SetTitle(const char * sTitle);
    };


// ---------------------------------
// CWindow -- The main CWindow class
// ---------------------------------
//
// CWindowHandler is an overridable set of Event callbacks. 
// If CWindowHandler can be override by calling SetMessageHandler(), allowing event handling without
// Subclassing CWindow

class CWindow : public CWindowHandler
{
public:
    struct WinOpt;

    // Class for snapping the window to the main desktop (i.e. correcting overlap)

    enum class Snap
    {
        WarnRed,            // turn window to RED so user knows it went out-of-bounds
        Snap,               // Just place it.
        SnapWarnRed,        // Place it and turn it red.
        NoAction,           // Do nothing. 
    };

   // This private section should disappear altogether in an upcoming update
private:
    friend CDevControls;
    friend CMenu;
    friend WinOpt;
    friend COutDC;
    friend CDialog;
    friend CSageBox;

    CJpeg::Status m_eLastJpegStatus = CJpeg::Status::Ok;                    // Value of last JPEG call through the window (i.e. success, bad file, etc.)
    bool m_bBaseWindow = false;
    ImageStatus m_eLastImageStatus = ImageStatus::Ok;                       // Last Read Image Status (this supercedes JpegStatus)

    int FindDeleter(void * pObject,Deleter_t * stDeleter = nullptr);        // Find any attached objects that want to be deleted when the window is deleted.
    void SetBaseWindow(bool bisBaseWindow = true);                          // Set as a Base Window, a hidden control-Parent Window 
    bool isBaseWindow();                                                    // true = hidden base window, causing differences in Window ownership, child windows, etc.
    bool m_bNextOpenisSave          = false;
    bool m_bPaintDisabled           = false;
    bool m_bFirstEvent              = false;                                // Allows passthrough on GetEvent() on first event call -- this is already deprecated and will be removed.
    CBitmap  m_cClsBitmap;                                                  // Bitmap used to repaint in window when cls() is issued (i.e. when a user has specified Transparent()
                                                                            // on a Child Window or called SetClsBitmap() to set the bitmap for Cls() repaint.
    bool m_bDevWindow               = false;                                // When true, this is a Quick Controls Dev Window, which has a different status for various things.
    bool m_bNoExitMsg               = false;                                // When true, the Exit Button will not be displayed on the bottom of the window for Sandbox Mode Applications (i.e. QuickSandbox, etc.)
    bool m_bTransparent             = false;                                // True when a child widow is transparent (so it can be updated through UpdateBg() or automatically)
    bool InitDevControls();   // Initialize default Dev Controls Window -- added only if used. 
    CDevControls * m_cDevControls = nullptr;    // Not created until first used. 
    void _vprintf(const char * Format,va_list va_args);                                 // Used internally

    // Bitmap Message Handle for BitmapWindow() returned windows
    // The main component is that pressing the 'X' button on the window
    // closes the window automatically (with normal CWindow windows, the 'X' simply sets a status which 
    // the owner has to field to close the window)
    //
    // $$PI -- This belongs in a separate file
    //
    class CBitmapWindowHandler : public CWindowHandler
    {
        CWindow * cWin;

        // Set the main/parent window
        //
        void Init(void * pClassInfo) 
        { 
            cWin = (CWindow *) pClassInfo;
        }

        // When the 'X' button is pressed, or we otherwise received a WM_CLOSE message,
        // Hide the window and then close/destory it when the parent window is closed.
        //
        MsgStatus OnClose()    
        {
            cWin->Hide(); 
            return MsgStatus::Ok; 
        }

        // If the user clicks on the window and AllowDrag() is on, move it to the top of the windows
        // so it doesn't move underneath other windows in the window (this is for embedded windows, not
        // popup bitmap windows).
        //
        // This is done in the non-client area since when AllowDrag() is set to true,
        // this changes all ButtonDown events to the NC ButtonDown messages
        //
        MsgStatus OnNCLButtonDown(int iMouseX,int iMouseY) override
        {
            // SetForegroundWindow(cWin->GetWindowHandle());
            HWND hWnd = cWin->GetWindowHandle();
            SetWindowPos(hWnd,HWND_TOP,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
            SetFocus(hWnd);
            return MsgStatus::Ok;
        }
    };

    // $$ Under Development $$ -- These are placeholders and to be removed (deprecated).
    // They are here to keep them out of the public declarations

    bool CreateButtonGroup(int iGroupID,int iNumButtons,int iX,int iY,int iWidth,int iHeight,const wchar_t * * sButtonNames,int iColumns = 0, SIZE szSpacing = {-1,-1}, const cwfOpt & cwOpt = cwfOpt());
    bool CreateButtonGroup(int iGroupID,int iNumButtons,int iX,int iY,const wchar_t * * sButtonNames,int iColumns = 0, const cwfOpt & cwOpt = cwfOpt());
    bool CreateButtonGroup(int iGroupID,int iNumButtons,int iX,int iY,int iWidth,int iHeight,const char * * sButtonNames,int iColumns = 0, SIZE szSpacing = {-1,-1}, const cwfOpt & cwOpt = cwfOpt());
    bool CreateButtonGroup(int iGroupID,int iNumButtons,int iX,int iY,const char * * sButtonNames,int iColumns = 0, const cwfOpt & cwOpt = cwfOpt());
   

// kAdvPublic:
// This is defalted to private to disallow CWindow class/object copying and deleting.
//
// ** CWindow is a managed object **
//
// the CWindow object is deleted automatically by SageBox when the window is closed.
// However, for subclassed windows with a lot of memory allocation, deleteObj() can be used to delete the
// object on-demand.
//
// kAdvPublic can be set to public, though the Copy Constructor should be monitored, as it could be disruptive.
//
kAdvPublic:                       
    void operator delete(void * ptr)
    { 
        ::operator delete(ptr); 
    };
    // Copy Constructore
    CWindow(const CWindow &p2) 
    {
        FailBox("CWindow::CopyConstructor","In Copy Constructor")   // In debug mode, lets us know if it is ever used
    };

public:
    void AttachDeleter(void * pObject,void (*fDeleter)(void *));    // Attach an object to the window to be deleted when the window is destoyed
                                                                    // This object is responsible for calling RemoveDeleter() when the **original** object
                                                                    // is deleted before the window is deleted (i.e. in its destructor, but only the original and not copies)
    void RemoveDeleter(void * pObject);                             // Remove the object to be deleted when the Window is closed/destroyed
 
    static void deleteObj(CWindow * p)                              // delete a CWindow Object.  This can be used in place of delete cMyWindow (which will cause a compiler error)
    {                                                               // The CWindow object (sublcassed or not) is deleted when the Windows window is closed and destroyed.
        delete p;                                                   // This allows the Windows object to be deleted on demand, which will cascade into deleting all controls and 
    }                                                               // control objects attached or that are children of the window.  Use this function with care.

    friend CDavinci;
friend CButton;
friend CEditBox;
friend ControlGroup;
    
    // Private types for creating private Dialogs (i.e. not part of public SageBox)
    // $$PI
    enum class StringInputType
    {
        String,
        Int,
        Float,
    };
        enum class TempControlType
    {
        Temp,
        Static,
    };


    // -----------------------------------
    // CCIO class -- used as cout and cin
    // -----------------------------------
    // CCIO implements the class used for 'out' and 'in', allowing
    // C++-style input/output streaming, but is more flexible and has more features
    //
    // basically, cWin->out << "This is a String" and in << fMyFloat are the same as C++, but more can be used to control the style if input and output
    // See notes elsewhere about how this is used.
    //
    // $$ this needs to be moved to a separate file
    class CCIO
    {
        friend CWindow;
    private:
        cwfOpt m_cOpt;                          // An options class used to compile options-as-we-go, i.e. out << fgColor("Red") << Font("Arial,40") << "Hello World!")
        CWindow * m_cWin        = nullptr;      // The parent Window (i.e. this main window)
        void ApplyOpt();                        // I'm pretty sure this is unused.
    public:
            // Output operations (most of which an be used in conjunction with IN, i.e.
            // in << "Enter a String" >> MyString; 
        
            CCIO & operator << (cwfOpt & opt);      // Options such as fgColor(), Font(), etc.
            CCIO & operator | (cwfOpt & opt);       // Allow use of '|' for ptions
            CCIO & operator + (cwfOpt & opt);       // Allow use of '+' for options
            CCIO & operator << (char * x);             
            CCIO & operator << (const char * x);
            CCIO & operator << (char x);
            CCIO & operator << (CDevString & cs);  
            CCIO & operator << (std::string & cs); 
            CCIO & operator << (CString & cs); 
            CCIO & operator << (int x); 
            CCIO & operator << (unsigned int x);
            CCIO & operator << (float x);
            CCIO & operator << (double x);
            CCIO & operator << (wchar_t * x); 

            // Input operations, allowing CString, std::string, int, options, and double for inputs. 

            CCIO & operator >> (CString & cs);
            CCIO & operator >> (std::string & cs);
            CCIO & operator >> (int & fValue);
            CCIO & operator >> (cwfOpt & opt); 
            CCIO & operator >> (double & fValue);
    };

    // Another private section 
    // $$PI

private:
    int                           m_iWinClosingCount = 0;   // Used to slow down EventLoop() to 100ms per call when it looks 
                                                            // like no one is responding to the window closing
                                                            // This prevents the loop for using 100% processor time when it has
                                                            // been abandoned.
    int                           m_iAutoWindowLocAddX;
    int                           m_iAutoWindowLocAddY;
    int                           m_iAutoWindowLocXMax;
    int                           m_iAutoWindowLocYMax;
    CBitmapWindowHandler          m_BitmapWindowHandler;
                                  
    bool                          m_bHideOnAllDelete        = false;
    POINT                         m_pLastAutoWindowLoc      = {};
    POINT                         m_pLastAutoWindowLocOrg   = {};
    int                           m_iAutoWindowLocStep      = 0;
    CString                     * m_cTempString             = nullptr;    // Used for various temp uses.
    char                        * m_sOptLine                = nullptr;    // Used for retrieving options.  Do not dealloc;
    int                           m_iWaitEventCount         = 0;
    int                           m_iEventCounter           = 0;
    bool                          m_bSkipEventErrors        = false;

    struct stLastOptions_t
    {
        CUserWindow::stFuncGetInputOptions_t    * stGenericEditBox;
        CUserWindow::stFuncGetInputOptions_t    * stGenericButton;
    };

    stLastOptions_t m_stLastOptions;

    StringInputType m_eStringInputType          = StringInputType::String;
    TempControlType m_eTempControlType          = TempControlType::Temp;

    void                    * m_pTempControlOjbect  = nullptr;
    void                    * m_pTempControl        = nullptr;
    CEditBox                * m_cEditBox            = nullptr;
    CButton                 * m_cButton             = nullptr;
    HWND                      m_hWnd                = nullptr;

    CEControlAction_t       * m_stControl;
    CEWindow_t              * m_stWindow;
    int                       m_iCopyCount;
    int                       m_iControl;   // Control number, used to find the control by value rather than object reference
                                            // This can provide a layer of safety for inactive windows, rather than passing an invalid object reference.
                                            // However, this can be inefficient, since Davinci has to interate through all window objects to find the value.
                                            // For more efficient use, using the object value is much faster, but requires an active/managed object (i.e. the caller
                                            // needs to know the object is valid.

    DialogStruct            * m_stPleaseWait        = nullptr;
    CButton                 * m_cPleaseWaitButton   = nullptr;      // These are deprecated
    CWindow                 * m_cPleaseWaitProgress = nullptr;      // These are deprecated
    int                       m_iProgressBarWidth;
    int                       m_iWindowLock = 0;


    bool TranslateOptColor(char * sColor,DWORD & dwColor);
    void InitWin(int iControl,CPasWindow * cWinCore,CSageBox * cSageBox,CDavinci * cDavinciMain);
    CButton & NewButton(CButton * cUserButton,ButtonType eButtonType,int ix,int iy,int iWidth,int iHeight,const char * sText,const cwfOpt & cwfOpt = cwfOpt());
    CButton & NewButton(CButton * cUserButton,ButtonType eButtonType,int ix,int iy,int iWidth,int iHeight,const wchar_t * sText,const cwfOpt & cwfOpt = cwfOpt());
    int GetOptInt(const char * sFind,bool & bSet);
    bool GetOptInt(const char * sFind,int & iValue);
    bool GetOptColor(const char * sFind,DWORD & dwColor);
    bool GetOptColor(const char * sFind,RGBColor_t & rgbColor);
    double GetOptFloat(const char * sFind,bool & bSet);
    CString GetOptString(const char * sFind,bool * bSet = nullptr);
    bool GetOptString(const char * sFind,CString & stString);
    bool GetOptBool(const char * sFind,bool bDefault = false); 
    void SetOptLine(const char * sOptions);
    POINT GetNewAutoWindowLoc();
    const char * GetInput(const char * sControls = nullptr,const char * sDefaultText = nullptr); 
    bool TranslateButtonStyle(SageString200 & stStyle,char * sStyle,char * sDefaultStyle,CStyleDefaults::ControlType eControlStyle,bool bStrict = false);
    bool AutoUpdate(Sage::UpdateDirty upDateDirty);
    SIZE CreateGenericButtonGroup(bool bRadioButton,int iNumButtons,int iX,int iY,const char * * sButtonNames,int iGroupID, const char * sLabel, const cwfOpt & cwOpt = cwfOpt());

protected:
    friend DialogStruct;
    CUserWindow         * m_cUserWin        = nullptr;  // Main engine Window Interface
    CDavinci            * m_cDavinciMain    = nullptr;  // Interface to CSageBox global functions (Deprecated)
    CSageBox            * m_cSageBox        = nullptr;  // Main SageBox core
    CPasWindow          * m_cWinCore        = nullptr;  // Main core engine interface (CUserWindow's parent)
    CWindow             * m_cParent         = nullptr;  // Our Parent Window, if any


    // -----------------------------------------------------------------------------------------
    // Public Inteface to SageBox -- Available Functions for programming using SageBox functions
    // -----------------------------------------------------------------------------------------

public:
    // Empty controls used for various routine to return the empty controls rather than nullptrs, for fallthrough errors

    static CWindow            m_cEmptyWindow;           // Returned when control not found for any function that returns a conrol reference.
    static CButton            m_cEmptyButton;          
    static CEditBox           m_cEmptyEditBox;         
    static CSlider            m_cEmptySlider;          
    static CListBox           m_cEmptyListBox;         
    static CComboBox          m_cEmptyComboBox;        
    static CDialog            m_cEmptyDialog;          

    // --------------------------------------------------------------------------------------------------------------
    // opt -- allows easy option-finding for writing Widgets and other functions that use options embedded in strings
    // --------------------------------------------------------------------------------------------------------------
    // 
    // At its core, opt is just a set of routines to parse strings, looking for either boolean or assigned options
    // note: the Bool() portion will change in an update to look for bool values, rather than just the declaration.
    // i.e. currently, Transparent() (as an example) puts "Transparent" in the option string, where
    // an upcoming change will cause "Transparent=1" to be used instead). 
    //
    // This shouldn't affect much, as using opt::Transparent() will also change, but fyi.
    //
    // Important Note: Do not use opt with a null window.  This can happen when creating a window and looking for options first.
    //                 Use the parent window's opt functionality to parse for options for a window that is yet to be created.
    //
    struct WinOpt
    {
        friend CWindow;
    private:
        CWindow * cWin;             // main parent window for reference. 
    public:

        // GetID() -- Get ID() and Name() from the string and place it into a cwfOpt object.
        // This allows it to be appended when creating other controls or windows without specifically looking for them.
        // Name() and ID() should be passed when used, so this makes it easy,
        // i.e. CreateWindow(iX,iY,iWidth,iHeight,Title,MyOptions << cParent->opt.GetID());
        //
        cwfOpt              GetID(const char * sOptions); 

        // GetOptInt() -- Find an integer value in a string, i.e. "Offset=1234"
        //
        int                 GetOptInt(const char * sFind,bool & bSet);

        // GetOptInt() -- Find an integer value in a string, i.e. "Offset=1234"
        //
        bool                GetOptInt(const char * sFind,int & iValue);

        // Get a color in the string.  This can take two forms, typically generated by cwfOpt
        // i.e. fgColor="Red" or fgColor=\x123456
        // i.e. fgColor("Red"), fgColor(MyColor), fgColorRGB(255,0,0)), fgColor({255,0,0})
        //
        bool                GetOptColor(const char * sFind,DWORD & dwColor);
 
        // Get a color in the string.  This can take two forms, typically generated by cwfOpt
        // i.e. fgColor="Red" or fgColor=\x123456
        // i.e. fgColor("Red"), fgColor(MyColor), fgColorRGB(255,0,0)), fgColor({255,0,0})
        //
        bool                GetOptColor(const char * sFind,RGBColor_t & rgbColor);

        //Get a float value from a string, i.e. "Radius = 4.5"
        //
        double              GetOptFloat(const char * sFind,bool & bSet);

        // Get a string from the input string, i.e. "MyString=Thisisastring" or "MyString='Thisisastring'
        // or "MyString=\"This is a String\",
        // i.e. opt::Group("MyGroup");
        //
        CString             GetOptString(const char * sFind,bool * bSet = nullptr);

        // Get a string from the input string, i.e. "MyString=Thisisastring" or "MyString='Thisisastring'
        // or "MyString=\"This is a String\",
        // i.e. opt::Group("MyGroup");
        //
        bool                GetOptString(const char * sFind,CString & stString);

        // Get a boolean from the input stream, which is defined by being declared
        // For eample, GetOptBool("Transparent") returns TRUE of Transparent is defined
        // (but not assigned -- "Transparent=<value>" is ignored. 
        // 
        // Returns FALSE if Transparent is not declared
        // Important: Note -- This will be changed to specifically looking for set values, i.e. "Transparent=true", rather than just the declaration
        //
        bool                GetOptBool(const char * sFind,bool bDefault = false);

        // Set the options line.  This used for all Get operations.  If not set, then it will return on a null string or 
        // act on the last known string.  
        //
        // Important: Make sure this is set before calling any Get Operations.
        //
        void                SetOptLine(const char * sOptions);
    };
    WinOpt opt;

    // Returns true if an event has occurred.  This can be used when GetEvent() isn't appropriate (such as in a loop that takes a lot of time)
    // When GetEvent() is called, the Event Pending Status is cleared back to false
    //
    __forceinline bool EventReady() { return (!this || !m_cUserWin) ? false : m_cUserWin->EventPending(); }

    // ClearEvent() -- Clear a specific pending event (whether it is pending or not).  This can be called before GetEvent() to clear the 
    // Event Queue of any specific event. For example, ClearEvent(SageEvent::MouseMove) or ClearEvent(SageEvent::WindowResize). 
    //
    // The latter example can be used to remove a pending window size when a Windows was initially sized with SetWindowSize(), which will
    // set a pending WindowResize() event.
    //
    // Use ClearEvents() to clear all pending events. 
    //
    __forceinline bool ClearEvent(SageEvent eEvent) { return (!this || !m_cUserWin) ? false : m_cUserWin->ClearEvent(eEvent); }
    
    // CLearEvents() -- Clear all pending events.  This can be used to clear any incoming events prior to calling GetEvent(). 
    // This can be useful when a setup, initialization, or other code has caused an unwanted event prior to the GetEvent() loop.
    //
    __forceinline bool ClearEvents() { return ClearEvent(SageEvent::All); }

    // SetMessageHandler() -- Set the message handler for the window.  This overrides the default message handler.
    // This allows events such as OnMouseMove(), OnButton() press, as well as all other Windows Messages to be 
    // acted upon as received by windows. 
    //
    bool SetMessageHandler(CWindowHandler * cHandler,void * pClassInfo = nullptr);

    // SetMessageHandler() -- Set the message handler for the window.  This overrides the default message handler.
    // This allows events such as OnMouseMove(), OnButton() press, as well as all other Windows Messages to be 
    // acted upon as received by windows. 
    //
    bool SetMessageHandler(CWindowHandler & cHandler,void * pClassInfo = nullptr);

    // in -- used as C++ cin, but with more options.
    // use MyWindow.in or just in when in the class the same way as C++.
    // You can also add options, such as:
    // in << "Input a number" >> MyIntValue, 
    // in << "Input a number" << WinColors() << "MyIntValue"  (this removes the box to enter the number, making it the same colors as the window)
    //
    CCIO in;

    // out -- used as C++ cout, but with more options.
    // Use MyWindow.out or just 'out' when in the clasee, the same was as C++
    //
    // out can be used as a powerful method to control outpout by adding controls.  out also works with std::string, CDevString, char *, and CString
    //
    // Examples:
    //
    // --> out << "Hello World";
    // --> out << Font("Arial,40") << "Hello World"
    // --> out << fgColor("Red") << Font("Arial,40") << CenterX() << "Hello World";
    //
    // This allows seamless and easy add-on of options.  After a certain number of options, it can be moved to the Write() function for clarity, if desired, i.e.:
    //
    // --> Write("Hello World", Font("Arial,40") | fgColor("Red") | CenterX()); 
    //
    CCIO out;

    // *** The following is still under-construction and somewhat experimental ***
    //
    // This allows a reference to the window, such as cWin << "Hello World" to print as in cOut. 
    // It is TBD, to make sure it doesn't interfere with other overloading. 

    CCIO & operator << (cwfOpt & opt)           { return out << opt;            }   // Options such as fgColor(), Font(), etc.
    CCIO & operator << (char * x)               { return out << x  ;            }          
    CCIO & operator << (const char * x)         { return out << x  ;            }
    CCIO & operator << (char x)                 { return out << x  ;            }
    CCIO & operator << (CDevString & cs)        { return out << *cs ;           }
    CCIO & operator << (std::string & cs)       { return out << cs.c_str() ;    }
    CCIO & operator << (CString & cs)           { return out << *cs  ;          }
    CCIO & operator << (int x)                  { return out << x  ;            }
    CCIO & operator << (unsigned int x)         { return out << x  ;            }
    CCIO & operator << (float x)                { return out << x  ;            }
    CCIO & operator << (double x)               { return out << x  ;            }
    CCIO & operator << (wchar_t * x)            { return out << x  ;            }
    
    // Input operations, allowing CString, std::string, int, options, and double for inputs. 
    
    CCIO & operator >> (CString & cs)           { return out >> cs      ;   }
    CCIO & operator >> (std::string & cs)       { return out >> cs      ;   }
    CCIO & operator >> (int & fValue)           { return out >> fValue  ;   }
    CCIO & operator >> (cwfOpt & opt)           { return out >> opt     ;   }
    CCIO & operator >> (double & fValue)        { return out >> fValue  ;   }

    // clone of endl in C++ -- it's much faster to just include '\n' in the stream.
    // i.e. out << "Hello World\n!" is much faster than out << "Hello World" << endl
    //
    static constexpr const char * endl = "\n";     

    CWindow();                        // Constructor.  Should only be used by CDavinci or CWindow.

    int GetControlID();                 // Return internal control ID - $$ move to private
    
    // WinMessageBox() -- Bring up a standard Windows Message Box. 
    //
    // This follows the same format as ::MessageBox() in windows and simply calls it in the same way. 
    //
    // This allows more "Windows-like" messages, information, warnings, etc.
    //
    // Example: WinMessageBox("This is the message","This is the title",MB_OK | MB_ICONINFO)
    //
    int WinMessageBox(const char * sMessage,const char * sTitle,unsigned int dwFlags);

    // printf() -- Works the same way as 'C' printf, except you can also put (X,Y) coordinates.
    // printf() can be very useful to quickly print text without using streaming notation,
    //
    // i.e. printf("This is attempt #d\n",++iAttempNo) vs. out << "This is attempt number " << ++iAttemptNo <<  "\n"
    //
    // printf(50,100,"Hello World") will print "Hello World") at (50,100) in the window and set the next write location accordingly.
    //
    void printf(const char * Format,...);

    // printf() -- Works the same way as 'C' printf, except you can also put (X,Y) coordinates.
    // printf() can be very useful to quickly print text without using streaming notation,
    //
    // i.e. printf("This is attempt #d\n",++iAttempNo) vs. out << "This is attempt number " << ++iAttemptNo <<  "\n"
    //
    // printf(50,100,"Hello World") will print "Hello World") at (50,100) in the window and set the next write location accordingly.
    //
    void printf(int iX,int iY,const char * Format,...);
    
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
    void Write(const char * sText,const char * sOptions = nullptr);            
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
    void Write(const char * sText,const cwfOpt & cwOptions);

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
    void Write(int iX,int iY,const char * sText,char * sOptions = nullptr);                             // $$ To be removed

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
    void Write(int iX,int iY,const char * sText,const cwfOpt & cwOptions);
    
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
    void Write(POINT pLoc,const char * sText,const cwfOpt & cwOptions = cwfOpt());

    // Writeln() -- Same as Write() but adds a '\n' at the end of the line for convenience
    void Writeln(const char * sText = nullptr,const char * sOptions = nullptr);        

    // Writeln() -- Same as Write() but adds a '\n' at the end of the line for convenience
    void Writeln(const char * sText,const cwfOpt & cwOptions);                        

    // Writeln() -- Same as Write() but adds a '\n' at the end of the line for convenience
    void Writeln(const cwfOpt & cwOptions);                        


    // Writeln() -- Same as Write() but adds a '\n' at the end of the line for convenience
    void Writeln(int iX,int iY,const char * sText = nullptr,const char * sOptions = nullptr);               // $$ To be removed  

    // Writeln() -- Same as Write() but adds a '\n' at the end of the line for convenience
    void Writeln(int iX,int iY,const char * sText,const cwfOpt & cwOptions);               

    // Writeln() -- Same as Write() but adds a '\n' at the end of the line for convenience

    void Writeln(POINT pLoc,const char * sText,const cwfOpt & cwOptions = cwfOpt());

    // Writeln() -- Same as Write() but adds a '\n' at the end of the line for convenience
    void Writeln(int iX,int iY,const cwfOpt & cwOptions);                        

    // Put a character on the window just as putchar() in regular 'C'.
    // This is useful for Ansii-text and emulation
    // note: This can be useful for terminal emulation when a non-proportional font (i.e. a terminal font)
    //       is used, i.e. "Courier New,14", etc.
    //
    int putchar(char cChar);

    // SetTabPos() -- Set the X position for Write/out/putchar/etc output to the character nth character position.
    // For example. SetTabPos(40) puts the output position (i.e. cursor) at charatcer position 40, based on the average character width.
    // This can be used to align console input boxes, text, etc. 
    // Since most fonts are proportionally spaced, the average character width is used.  In the case of terminal fonts, the character width is
    // the same for all characters and is more predictable
    //
    // This is included as a function from Basic() that is used in a number of programs
    //
    bool SetTabPos(int iPos);

    // ClipWindow() -- Clips the window in the given rectangle, restricting output and drawing to that region.
    // This can be used to restrict write areas, Cls(), Drawing, etc. 
    // For example, a graph can be drawn with the clip and will not exceed the clip area.  This can be very useful and 
    // allow such drawing and output in place of creating a child window to perform the same task.
    // Use  ResetClipWindow() or ClipWindow() (with no paramaters) to remove the clipping region
    //
    // bAllowScroll allows the clip region to scroll based on writing text to the screen with '\n' newlines.
    // Default is to not allow the clipping region to scroll.
    //
    bool ClipWindow(int iX,int iY,int iWidth,int iHeight,bool bAllowScroll = false);

    // ClipWindow() -- Clips the window in the given rectangle, restricting output and drawing to that region.
    // This can be used to restrict write areas, Cls(), Drawing, etc. 
    // For example, a graph can be drawn with the clip and will not exceed the clip area.  This can be very useful and 
    // allow such drawing and output in place of creating a child window to perform the same task.
    // Use  ResetClipWindow() or ClipWindow() (with no paramaters) to remove the clipping region
    //
    // bAllowScroll allows the clip region to scroll based on writing text to the screen with '\n' newlines.
    // Default is to not allow the clipping region to scroll.
    //
    bool ClipWindow(POINT pPoint,SIZE szSize,bool bAllowsScroll = false);

    // ClipWindow() -- Clips the window in the given rectangle, restricting output and drawing to that region.
    // This can be used to restrict write areas, Cls(), Drawing, etc. 
    // For example, a graph can be drawn with the clip and will not exceed the clip area.  This can be very useful and 
    // allow such drawing and output in place of creating a child window to perform the same task.
    // Use  ResetClipWindow() or ClipWindow() (with no paramaters) to remove the clipping region
    //
    bool ClipWindow();

    // ResetClipWindow() -- Resets any clipping region active for the current window.
    //
    bool ResetClipWindow();

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
    bool SetAutoScroll(WindowScroll scroll);
    
    // GetAutoScroll() -- Get the current scroll status
    // This can be used when the window doesn't seem to be scrolling to determine if the AutoScroll setting may be the issue.
    //
    WindowScroll GetAutoScroll();
    
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
    void Cls(DWORD iColor1=-1,DWORD iColor2 = -1);   

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
    void Cls(RGBColor_t rgbColor);    

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
    void Cls(const char * sColor1,const char * sColor2=nullptr);    

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
    void Cls(const char * sColor1,RGBColor_t rgbColor);   

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
    void Cls(RGBColor_t rgbColor,const char * sColor2);    

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
    bool Cls(CBitmap & cBitmap);

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
    void Cls(RGBColor_t rgbColor,RGBColor_t rgbColor2);    

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
    void ClsCanvas(int iColor1=-1,int iColor2 = -1); 

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
    void ClsCanvas(RGBColor_t rgbColor);    

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
    void ClsCanvas(const char * sColor1,const char * sColor2=nullptr);    

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
    void ClsCanvas(const char * sColor1,RGBColor_t rgbColor);    

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
    void ClsCanvas(RGBColor_t rgbColor,const char * sColor2);    

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
    void ClsCanvas(RGBColor_t rgbColor,RGBColor_t rgbColor2);    


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
    void Update(int iUpdateMS=0);

    // ForceUpdate() -- This is deprecated, but may be used in the future when more update/painting options are implemented
    //
    bool ForceUpdate(bool bIfDirty = false);

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
    bool UpdateReady(int iUpdateMS = 10);

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
    bool SetWindowDrag(bool bGrab = true);

    // SetWindowSize() -- Set the size of the window.
    // 
    // This sets the entire size of the window, both the visible portion and the canvas size.
    // See SetCanvasSize() to set a larger canvas than the displayed window size.
    //
    // bInnerSize -- when true, this calculates the window size based on the inner canvas size.
    // When false, this calculates the window (when it is a popup or has a frame) to be the size of the entire window, 
    // making the canvas size smaller.
    //
    bool SetWindowSize(SIZE szSize,bool bInnerSize = false);

    // SetWindowSize() -- Set the size of the window.
    // 
    // This sets the entire size of the window, both the visible portion and the canvas size.
    // See SetCanvasSize() to set a larger canvas than the displayed window size.
    //
    // bInnerSize -- when true, this calculates the window size based on the inner canvas size.
    // When false, this calculates the window (when it is a popup or has a frame) to be the size of the entire window, 
    // making the canvas size smaller.
    //
    bool SetWindowSize(int iWidth,int iHeight,bool bInnerSize = false);

    // GetCanvasSize() -- Get the size of the Window Canvas. 
    // 
    // The Window Canvas is the bitmap that is shown in the window and may be larger than the window (but may never be smaller). 
    // This allows the Window to be resized dynamically, or to use a larger bitmap and move it around in the window, such as for scrolling.
    //
    SIZE GetCanvasSize(); 

    // Set CanvasSize() -- Set the Canvas size of the window
    // This value must be greater than the displayed window canvas or it is ignored.
    //
    // This sets the Canvas size of the window (i.e. the drawable space), which can exceed the size of the window.
    //
    bool SetCanvasSize(SIZE szMaxSize,bool bAllowResizing = true);
    bool SetCanvasSize(SIZE szMinSize,SIZE szSize,bool bAllowResizing = true);
 
    // Set CanvasSize() -- Set the Canvas size of the window
    // This value must be greater than the displayed window canvas or it is ignored.
    //
    // This sets the Canvas size of the window (i.e. the drawable space), which can exceed the size of the window.
    //
    bool SetCanvasSize(int iWidth,int iHeight,bool bAllowResizing = true);
    bool AllowResizing(bool bAllowResizing = true);
    // GetClientSize() -- Get the current client area size (i.e. writeable/drawable part of the window)
    // This value may be larger than the visible window.
    //
    // GetWindowSize(), for example, returns the visible canvas area.
    //
    bool GetClientSize(SIZE & Size);

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
    bool UpdateRegion(RECT & rRegion,int iUpdateMS = 0);

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
    bool UpdateRegion(int iX,int iY,int iWidth,int iHeight,int iUpdateMS = 0);
 
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
    bool UpdateRegion(int iX,int iY,RawBitmap_t stBitmap,int iUpdateMS = 0);

    // GetTextSize() -- Get the text size of the text using the current font.
    //
    // This returns the size the text will use in the window.  This can help with the placement of the text or controls around the text.
    // For example, using auto szSize = GetTextSize("This is some Text"), and then Write((szDesktopSize.cx-szSize.cx)/2,100,MyText)) 
    // will center the text in the X plane.
    //
    SIZE GetTextSize(const wchar_t * sText);
  
    // GetTextSize() -- Get the text size of the text using the current font.
    //
    // This returns the size the text will use in the window.  This can help with the placement of the text or controls around the text.
    // For example, using auto szSize = GetTextSize("This is some Text"), and then Write((szDesktopSize.cx-szSize.cx)/2,100,MyText)) 
    // will center the text in the X plane.
    //
    SIZE GetTextSize(HFONT hFont,const wchar_t * sText);

    // GetTextSize() -- Get the text size of the text using the current font.
    //
    // This returns the size the text will use in the window.  This can help with the placement of the text or controls around the text.
    // For example, using auto szSize = GetTextSize("This is some Text"), and then Write((szDesktopSize.cx-szSize.cx)/2,100,MyText)) 
    // will center the text in the X plane.
    //
    bool GetTextSize(const char * sText,SIZE & Size);

    // GetTextSize() -- Get the text size of the text using the current font.
    //
    // This returns the size the text will use in the window.  This can help with the placement of the text or controls around the text.
    // For example, using auto szSize = GetTextSize("This is some Text"), and then Write((szDesktopSize.cx-szSize.cx)/2,100,MyText)) 
    // will center the text in the X plane.
    //
    SIZE GetTextSize(const char * sText);
    SIZE GetTextSize(HFONT hFont,const char * sText);
    bool GetTextSize(HFONT hFont,const char * sText,SIZE & Size);

    // AddWindowShadow() -- Adds a shadow to the window.  This can be useful for popup-windows or
    // child windows embedded in the current window.
    //
    bool AddWindowShadow();

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
    bool DrawRectangle(int iX,int iY,int iWidth,int iHeight,int iColor,int iColor2 = -1);

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
    bool DrawRectangle(POINT pLoc,SIZE szSize,int iWidth,int iHeight,int iColor,int iColor2 = -1);

    // DrawOpenRectangle() -- Draws an 'open' rectangle with only the outline color and no inside color (i.e. the inside
    // is not drawn, only the border). 
    //
    // This draws the border, the size of the current pen.  You can also add the pen size that will be used as an extra parameter, which will
    // override the current pen setting without changing the value of the current pen size
    //
    // See DrawRectangle() and SetPenSize() for more information
    //
    bool DrawOpenRectangle(int iX,int iY,int iWidth,int iHeight,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);        

    // DrawOpenRectangle() -- Draws an 'open' rectangle with only the outline color and no inside color (i.e. the inside
    // is not drawn, only the border). 
    //
    // This draws the border, the size of the current pen.  You can also add the pen size that will be used as an extra parameter, which will
    // override the current pen setting without changing the value of the current pen size
    //
    // See DrawRectangle() and SetPenSize() for more information
    //
    bool DrawOpenRectangle(int iX,int iY,int iWidth,int iHeight,int iColor,int iPenSize = 0);        

    // DrawOpenRectangle() -- Draws an 'open' rectangle with only the outline color and no inside color (i.e. the inside
    // is not drawn, only the border). 
    //
    // This draws the border, the size of the current pen.  You can also add the pen size that will be used as an extra parameter, which will
    // override the current pen setting without changing the value of the current pen size
    //
    // See DrawRectangle() and SetPenSize() for more information
    //
    bool DrawOpenRectangle(POINT pLoc,SIZE szSize,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);

    // DrawOpenRectangle() -- Draws an 'open' rectangle with only the outline color and no inside color (i.e. the inside
    // is not drawn, only the border). 
    //
    // This draws the border, the size of the current pen.  You can also add the pen size that will be used as an extra parameter, which will
    // override the current pen setting without changing the value of the current pen size
    //
    // See DrawRectangle() and SetPenSize() for more information
    //
    bool DrawOpenRectangle(POINT pLoc,SIZE szSize,int iColor,int iPenSize = 0);

    bool DrawGradient(int ix,int iy,int iWidth,int iHeight,RGBColor_t rgbColor1,RGBColor_t rgbColor2,bool bHorizontal = false);
    bool DrawGradient(POINT pLoc,SIZE szSize,RGBColor_t rgbColor1,RGBColor_t rgbColor2,bool bHorizontal = false);
    bool DrawGradient(RECT rGradientRect,RGBColor_t rgbColor1,RGBColor_t rgbColor2,bool bHorizontal = false);

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
    bool DrawRectangle(int ix,int iy,int iWidth,int iHeight,RGBColor_t rgbColor  = Rgb::Default,RGBColor_t rgbColor2 = Rgb::Undefined);        
    
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
    bool DrawRectangle(POINT pLoc,SIZE szSize,RGBColor_t rgbColor = Rgb::Undefined,RGBColor_t rgbColor2 = Rgb::Undefined);


    // Ractangle2() -- Used for testing 
    //
    bool Rectangle2(int ix,int iy,int iWidth,int iHeight,int iColor,int iColor2 = -1);        

    // Draw a Triangle on the screen using the three points
    // The third point will connect directly to the first point.
    //
    // The first Color (which can be RGB() or RGBColor_t()) is the internal color.
    // The second color is the outline color (if ommitted, there is no outline).
    // The outline size is determine by the Pen Thickness, which defaults to 1.
    //
    // The Pen Thickness can be changed with SetPenThickness()
    //
    bool DrawTriangle(POINT v1,POINT v2,POINT v3,int iColor1,int iColor2 = -1);

    // Draw a Quadrangle on the screen using the three points
    // The fourth point will connect directly to the first point.
    //
    // The first Color (which can be RGB() or RGBColor_t()) is the internal color.
    // The second color is the outline color (if ommitted, there is no outline).
    // The outline size is determine by the Pen Thickness, which defaults to 1.
    //
    // The Pen Thickness can be changed with SetPenThickness()
    //
   bool DrawQuadrangle(POINT v1,POINT v2,POINT v3,POINT v4,int iColor1,int iColor2 = -1);
    
    // Draw a Polygon on the screen using an array of POINT * values.
    // The last point will connect directly to the first point.
    //
    // The first Color (which can be RGB() or RGBColor_t()) is the internal color.
    // The second color is the outline color (if ommitted, there is no outline).
    // The outline size is determine by the Pen Thickness, which defaults to 1.
    //
    // The Pen Thickness can be changed with SetPenThickness()
    //
    bool DrawPolygon(POINT * pPoints,int iVertices,int iColor1,int iColor2 = -1);

    // Draw a Circle on the Window
    //
    // The first Color (which can be RGB() or RGBColor_t()) is the internal color.
    // The second color is the outline color (if ommitted, there is no outline).
    // The outline size is determine by the Pen Thickness, which defaults to 1.
    //
    // The Pen Thickness can be changed with SetPenThickness()
    //
    bool DrawCircle(int iX,int iY,int iRadius,int iColor1,int iColor2 = -1);
 
    // Draw a Circle on the Window
    //
    // The first Color (which can be RGB() or RGBColor_t()) is the internal color.
    // The second color is the outline color (if ommitted, there is no outline).
    // The outline size is determine by the Pen Thickness, which defaults to 1.
    //
    // The Pen Thickness can be changed with SetPenThickness()
    //
    bool DrawCircle(int iX,int iY,int iRadius,RGBColor_t rgbColorIn = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined);

    // Draw a Circle on the Window
    //
    // The first Color (which can be RGB() or RGBColor_t()) is the internal color.
    // The second color is the outline color (if ommitted, there is no outline).
    // The outline size is determine by the Pen Thickness, which defaults to 1.
    //
    // The Pen Thickness can be changed with SetPenThickness()
    //
    bool DrawCircle(POINT pLoc,int iRadius,RGBColor_t rgbColorIn = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined);

    // Draw a Circle on the Window
    //
    // The first Color (which can be RGB() or RGBColor_t()) is the internal color.
    // The second color is the outline color (if ommitted, there is no outline).
    // The outline size is determine by the Pen Thickness, which defaults to 1.
    //
    // The Pen Thickness can be changed with SetPenThickness()
    //
    bool DrawCircle(POINT pLoc,int iRadius,int iColor1,int iColor2 = -1);

    // DrawOpenCircle() -- Draws an 'open' circle with only the outline color and no inside color (i.e. the inside
    // is not drawn, only the border). 
    //
    // This draws the border, the size of the current pen.  You can also add the pen size that will be used as an extra parameter, which will
    // override the current pen setting without changing the value of the current pen size
    //
    // See DrawCircle() and SetPenSize() for more information
    //
    bool DrawOpenCircle(int iX,int iY,int iRadius,RGBColor_t rgbColor = Rgb::Default,int iPenSize =0);

    // DrawOpenCircle() -- Draws an 'open' circle with only the outline color and no inside color (i.e. the inside
    // is not drawn, only the border). 
    //
    // This draws the border, the size of the current pen.  You can also add the pen size that will be used as an extra parameter, which will
    // override the current pen setting without changing the value of the current pen size
    //
    // See DrawCircle() and SetPenSize() for more information
    //
    bool DrawOpenCircle(int iX,int iY,int iRadius,int iColor,int iPenSize = 0);
    
    // DrawOpenCircle() -- Draws an 'open' circle with only the outline color and no inside color (i.e. the inside
    // is not drawn, only the border). 
    //
    // This draws the border, the size of the current pen.  You can also add the pen size that will be used as an extra parameter, which will
    // override the current pen setting without changing the value of the current pen size
    //
    // See DrawCircle() and SetPenSize() for more information
    //
    bool DrawOpenCircle(POINT pLoc,int iRadius,RGBColor_t rgbColor = Rgb::Default,int iPenSize =0 );
    
    // DrawOpenCircle() -- Draws an 'open' circle with only the outline color and no inside color (i.e. the inside
    // is not drawn, only the border). 
    //
    // This draws the border, the size of the current pen.  You can also add the pen size that will be used as an extra parameter, which will
    // override the current pen setting without changing the value of the current pen size
    //
    // See DrawCircle() and SetPenSize() for more information
    //
    bool DrawOpenCircle(POINT pLoc,int iRadius,int iColor,int iPenSize =0 );

    // Draw an Ellipse on the Window
    //
    // The first Color (which can be RGB() or RGBColor_t()) is the internal color.
    // The second color is the outline color (if ommitted, there is no outline).
    // The outline size is determine by the Pen Thickness, which defaults to 1.
    //
    // The Pen Size can be changed with SetPenSize()
    //
    bool DrawEllipse(int iX,int iY,int iRadiusX,int iRadiusY,int iColor1,int iColor2 = -1);
 
    // Draw a ellipse on the Window
    //
    // The first Color (which can be RGB() or RGBColor_t()) is the internal color.
    // The second color is the outline color (if ommitted, there is no outline).
    // The outline size is determine by the Pen Thickness, which defaults to 1.
    //
    // The Pen Thickness can be changed with SetPenThickness()
    //
    bool DrawEllipse(int iX,int iY,int iRadiusX,int iRadiusY,RGBColor_t rgbColorIn = Rgb::Default,RGBColor_t rgbColorOut = Rgb::None);

    // Draw a ellipse on the Window
    //
    // The first Color (which can be RGB() or RGBColor_t()) is the internal color.
    // The second color is the outline color (if ommitted, there is no outline).
    // The outline size is determine by the Pen Thickness, which defaults to 1.
    //
    // The Pen Thickness can be changed with SetPenThickness()
    //
    bool DrawEllipse(POINT pLoc,int iRadiusX,int iRadiusY,int iColor1,int iColor2 = -1);

    // Draw a ellipse on the Window
    //
    // The first Color (which can be RGB() or RGBColor_t()) is the internal color.
    // The second color is the outline color (if ommitted, there is no outline).
    // The outline size is determine by the Pen Thickness, which defaults to 1.
    //
    // The Pen Thickness can be changed with SetPenThickness()
    //
   bool DrawEllipse(POINT pLoc,int iRadiusX,int iRadiusY,RGBColor_t rgbColorIn = Rgb::Default,RGBColor_t rgbColorOut = Rgb::None);

    // DrawOpenEllipse() -- Draws an 'open' ellipse with only the outline color and no inside color (i.e. the inside
    // is not drawn, only the border). 
    //
    // This draws the border, the size of the current pen.  You can also add the pen size that will be used as an extra parameter, which will
    // override the current pen setting without changing the value of the current pen size
    //
    // See DrawCircle() and SetPenSize() for more information
    //
    bool DrawOpenEllipse(int iX,int iY,int iRadiusX,int iRadiusY,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);
    
    // DrawOpenEllipse() -- Draws an 'open' ellipse with only the outline color and no inside color (i.e. the inside
    // is not drawn, only the border). 
    //
    // This draws the border, the size of the current pen.  You can also add the pen size that will be used as an extra parameter, which will
    // override the current pen setting without changing the value of the current pen size
    //
    // See DrawCircle() and SetPenSize() for more information
    //
    bool DrawOpenEllipse(int iX,int iY,int iRadiusX,int iRadiusY,int iColor,int iPenSize = 0);

    // DrawOpenEllipse() -- Draws an 'open' ellipse with only the outline color and no inside color (i.e. the inside
    // is not drawn, only the border). 
    //
    // This draws the border, the size of the current pen.  You can also add the pen size that will be used as an extra parameter, which will
    // override the current pen setting without changing the value of the current pen size
    //
    // See DrawCircle() and SetPenSize() for more information
    //
    bool DrawOpenEllipse(POINT pLoc,int iRadiusX,int iRadiusY,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);

    // DrawOpenEllipse() -- Draws an 'open' ellipse with only the outline color and no inside color (i.e. the inside
    // is not drawn, only the border). 
    //
    // This draws the border, the size of the current pen.  You can also add the pen size that will be used as an extra parameter, which will
    // override the current pen setting without changing the value of the current pen size
    //
    // See DrawCircle() and SetPenSize() for more information
    //
    bool DrawOpenEllipse(POINT pLoc,int iRadiusX,int iRadiusY,int iColor,int iPenSize = 0);

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
    bool SetPenColor(int iColor);

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
    bool SetPenColor(const char * sColor);

    // Set the DrawLine color for new lines using SetLinePos() (or MoveTo()) and then DrawLine() (or LineTo())
    // This allows a default line color to be selected so that functions later drawing lines with SetLinePos()/DrawLine()
    // don't need to set the color. 
    //
    // note: This does not work with DrawLine().  When using DrawLine(), the color must be specifieed.
    //
    bool SetPenColor(RGBColor_t rgbColor);

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
    bool SetLinePos(int iX,int iY,int iColor);

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
    bool SetLinePos(int iX,int iY,RGBColor_t rgbColor = Rgb::Undefined);

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
    bool SetLinePos(POINT pLoc,int iColor);

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
    bool SetLinePos(POINT pLoc,RGBColor_t rgbColor = Rgb::Undefined);

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
    bool MoveTo(int iX,int iY,int iColor);

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
    bool MoveTo(int iX,int iY,RGBColor_t rgbColor = Rgb::Undefined);

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
    bool MoveTo(POINT pLoc,int iColor);

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
    bool MoveTo(POINT pLoc,RGBColor_t rgbColor = Rgb::Undefined);

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
    bool LineTo(int iX,int iY,RGBColor_t rgbColor = Rgb::Undefined); 

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
    bool LineTo(int iX,int iY,int iColor); 

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
    bool LineTo(POINT pLoc,RGBColor_t rgbColor = Rgb::Undefined);

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
    bool LineTo(POINT pLoc,int iColor);

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
    bool LineToEx(bool bFirstPoint,int iX,int iY,RGBColor_t rgbColor = Rgb::Undefined); 

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
    bool LineToEx(bool bFirstPoint,int iX,int iY,int iColor); 

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
    bool LineToEx(bool bFirstPoint,POINT pLoc,RGBColor_t rgbColor = Rgb::Undefined);

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
    bool LineToEx(bool bFirstPoint,POINT pLoc,int iColor);

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
    bool DrawLineTo(int iX,int iY,RGBColor_t rgbColor = Rgb::Undefined); 

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
    bool DrawLineTo(int iX,int iY,int iColor); 

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
    bool DrawLineTo(POINT pLoc,RGBColor_t rgbColor = Rgb::Undefined);

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
    bool DrawLineTo(POINT pLoc,int iColor =-1);

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
    bool DrawLineToEx(bool bFirstPoint,int iX,int iY,RGBColor_t rgbColor = Rgb::Undefined); 

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
    bool DrawLineToEx(bool bFirstPoint,int iX,int iY,int iColor); 

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
    bool DrawLineToEx(bool bFirstPoint,POINT pLoc,RGBColor_t rgbColor = Rgb::Undefined);

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
    bool DrawLineToEx(bool bFirstPoint,POINT pLoc,int iColor);


    // Draw a line in the window.  
    // This draws a line from (ix1,iy1) to (ix2,iy2) in the given color
    //
    // The color can be RGB() or RGBColor_t
    // The thickness of the line can be changed with SetPenThickness(), which defaults to 1.
    //
    // If the color is omitted, the current line color is used.
    // The line color can be set or changed with SetLineColor() or SetLinePos()
    //
    bool DrawLine(int ix1,int iy1,int ix2,int iy2,int iColor);
 
    // Draw a line in the window.  
    // This draws a line from (ix1,iy1) to (ix2,iy2) in the given color
    //
    // The color can be RGB() or RGBColor_t
    // The thickness of the line can be changed with SetPenThickness(), which defaults to 1.
    //
    // If the color is omitted, the current line color is used.
    // The line color can be set or changed with SetLineColor() or SetLinePos()
    //
    bool DrawLine(POINT p1,POINT p2,int iColor);

    // Draw a line in the window.  
    // This draws a line from (ix1,iy1) to (ix2,iy2) in the given color
    //
    // The color can be RGB() or RGBColor_t
    // The thickness of the line can be changed with SetPenThickness(), which defaults to 1.
    //
    // If the color is omitted, the current line color is used.
    // The line color can be set or changed with SetLineColor() or SetLinePos()
    //
    bool DrawLine(POINT p1,POINT p2,RGBColor_t rgbColor = Rgb::Default);


    // Draw a line in the window.  
    // This draws a line from (ix1,iy1) to (ix2,iy2) in the given color
    //
    // The color can be RGB() or RGBColor_t
    // The thickness of the line can be changed with SetPenThickness(), which defaults to 1.
    //
    // If the color is omitted, the current line color is used.
    // The line color can be set or changed with SetLineColor() or SetLinePos()
    //
    bool DrawLine(int ix1,int iy1,int ix2,int iy2,RGBColor_t rgbColor = Rgb::Default);

    // Draw a line in the window.  
    // This draws a line from (ix1,iy1) to (ix2,iy2) in the given color
    //
    // The color can be RGB() or RGBColor_t
    // The thickness of the line can be changed with SetPenThickness(), which defaults to 1.
    //
    bool DrawLine2(int ix1,int iy1,int iWidth,int iHeight,int iColor);

    // Draw a line in the window.  
    // This draws a line from (ix1,iy1) to (ix2,iy2) in the given color
    //
    // The color can be RGB() or RGBColor_t
    // The thickness of the line can be changed with SetPenThickness(), which defaults to 1.
    //
    bool DrawLine2(int ix1,int iy1,int iWidth,int iHeight,RGBColor_t rgbColor = Rgb::Default);
    
    // Draw a line in the window.  
    // This draws a line from (ix1,iy1) to (ix2,iy2) in the given color
    //
    // The color can be RGB() or RGBColor_t
    // The thickness of the line can be changed with SetPenThickness(), which defaults to 1.
    //
    bool DrawLine2(POINT p1,SIZE szDist,int iColor);

    // Draw a line in the window.  
    // This draws a line from (ix1,iy1) to (ix2,iy2) in the given color
    //
    // The color can be RGB() or RGBColor_t
    // The thickness of the line can be changed with SetPenThickness(), which defaults to 1.
    //
    bool DrawLine2(POINT p1,SIZE szDist,RGBColor_t rgbColor = Rgb::Default);

    // SetPenThickness -- Sets the thickness of the 'pen' (i.e. draw line thickness or outline thickness on Cricles, Triangles, etc.)
    //
    // This value defaults to 1, but can be set prior to drawing any control that uses an outline or draws a line.
    // SetPenThickness() must be used again to revert back to the original thickness.  
    // 
    // SetPenThickness() returns the new thickness of the pen -- this useful to ensure the value does not 
    // go below the minimum of 1.
    //
    int SetPenThickness(int iThickness);

    // GetPenThickness() -- Returns the size (i.e. thickness) of the current pen)
    //
    int GetPenThickness();

    // SetPenSize -- Sets the thickness of the 'pen' (i.e. draw line thickness or outline thickness on Cricles, Triangles, etc.)
    //
    // This value defaults to 1, but can be set prior to drawing any control that uses an outline or draws a line.
    // SetPenThickness() must be used again to revert back to the original thickness.  
    // 
    // SetPenThickness() returns the new thickness of the pen -- this useful to ensure the value does not 
    // go below the minimum of 1.
    //
    int SetPenSize(int iThickness);

    // GetPenSize() -- Returns the size (i.e. thickness) of the current pen)
    //
    int GetPenSize();

   // DrawPixel() draw a pixel on the screen. 
    //
    // This sets a pixel on the screen and can be useful in prototyping and general drawing.
    // However, note that DrawPixel() used when drawing an entire array (i.e. a bitmap or an image)
    // is very slow! 
    //
    // For images, bitmaps, collections, etc. it is much faster to build a bitmap in memory
    // and then call DrawBitmap() to display multiple pixels at a time.
    //
    bool DrawPixel(int iX,int iY,DWORD dwColor);
 
    bool DrawPixel(POINT pPoint,DWORD dwColor)          { return DrawPixel(pPoint.x,pPoint.y,dwColor); }
    bool DrawPixel(POINT pPoint,RGBColor_t rgbColor)    { return DrawPixel(pPoint.x,pPoint.y,rgbColor); }

    // DrawPixel() draw a pixel on the screen. 
    //
    // This sets a pixel on the screen and can be useful in prototyping and general drawing.
    // However, note that DrawPixel() used when drawing an entire array (i.e. a bitmap or an image)
    // is very slow! 
    //
    // For images, bitmaps, collections, etc. it is much faster to build a bitmap in memory
    // and then call DrawBitmap() to display multiple pixels at a time.
    //
    bool DrawPixel(int iX,int iY,RGBColor_t rgbColor);               // $$ Form outdated -- rgbColor converts automatically to DWORD
 
                                                                     // DrawPixel() draw a piel on the screen. 
    // Show() -- Show the window (i.e. make it visible)
    //
    // If the window is not showing on the screen, Show() will make it visible.
    //
    // Show() and Show(true) will show a hidden window
    // Show(false) will Hide() the window
    //
    bool Show(bool bShow = true);

    // Hide() -- Hide the window (i.e. remove it from the screen without closing it or destroying it)
    //
    // If the window is on the screen Hide() will remove it from the display but not otherwise affect it.
    // 
    // Hide() and Hide(true) will hide the window
    // Hide(false) will Show() the window
    //
    bool Hide(bool bHide = true);

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
    CSlider &  NewSlider(CSlider * cSlider,int iX,int iY,int iSize,const wchar_t * sLabel = nullptr,const cwfOpt & cwOpt = cwfOpt());

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
    CSlider &  NewSlider(CSlider * cSlider,int iX,int iY,int iSize,const char * sLabel = nullptr,const cwfOpt & cwOpt = cwfOpt());

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
    CSlider &  NewSlider(CSlider * cSlider,int iX,int iY,int iSize,const cwfOpt & cwOpt = cwfOpt());

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
    CSlider &  NewSlider(int iX,int iY,int iSize,const char * sLabel,const cwfOpt & cwOpt = cwfOpt());

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
    CSlider &  NewSlider(int iX,int iY,int iSize,const wchar_t * sLabel,const cwfOpt & cwOpt = cwfOpt());

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
    CSlider &  NewSlider(int iX,int iY,int iSize,const cwfOpt & cwOpt = cwfOpt());


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
    void QuickButton(const char * sText = nullptr,const char * sTitleBar = nullptr);


    // ---------------------
    // win.dialog functions.  
    // ---------------------
    //
    // used as dialog.<function>, i.e. dialog.Info() or dialog.GetFloat()
    // some are replicated in the normal window functions, such as GetInteger(), etc.
    //
    struct WinDialog
    {
        friend CSageBox;
        friend CWindow;
    private:

        CWindow * m_cWin                ;
        CPasWindow * m_cWinCore            ;
        void InitDialog(DialogStruct & stDialog,const char * sTitle,const char * sOptions,DialogStruct::TitleIconType eDefault);
        bool    m_bYesNoCancel;            // For YesNoCancel to call YesNo() and get cancel status
        bool  * m_bPleaseWaitSignal = nullptr; 
        CButtonHandler * m_cPleaseWaitButtonHandler = nullptr;
    public:
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
        void QuickButton(const char * sText = nullptr,const char * sTitleBar = nullptr);

        // WinMessageBox() -- Bring up a standard Windows Message Box. 
        //
        // This follows the same format as ::MessageBox() in windows and simply calls it in the same way. 
        //
        // This allows more "Windows-like" messages, information, warnings, etc.
        //
        // Example: WinMessageBox("This is the message","This is the title",MB_OK | MB_ICONINFO)
        //
        int WinMessageBox(const char * sMessage,const char * sTitle,unsigned int dwFlags);
        int GetInteger(const char * sTitle,bool & bCancelled,const cwfOpt & cwOptions = cwfOpt());
        int GetInteger(const char * sTitle = nullptr,const cwfOpt & cwOptions = cwfOpt());
        bool GetInteger(const char * sTitle,int & iInteger,const cwfOpt & cwOptions = cwfOpt());
        bool GetInteger(int & iInteger,const cwfOpt & cwOptions = cwfOpt());

        double GetFloat(const char * sTitle,bool & bCancelled,const cwfOpt & cwOptions = cwfOpt());
        double GetFloat(const char * sTitle = nullptr,const cwfOpt & cwOptions = cwfOpt());
        bool GetFloat(const char * sTitle,double & fFloat,const cwfOpt & cwOptions = cwfOpt());
        bool GetFloat(double & fFloat,const cwfOpt & cwOptions = cwfOpt());

        CString GetString(const char * sTitle,bool & bCancelled,const cwfOpt & cwOptions = cwfOpt());
        CString GetString(const char * sTitle,const cwfOpt & cwOptions = cwfOpt());

        void Info(const char * sTitle,const cwfOpt & cwOptions = cwfOpt());
        bool YesNo(const char * sTitle,const cwfOpt & cwOptions = cwfOpt());

        DialogResult YesNoCancel(const char * sTitle,const cwfOpt & cwOptions = cwfOpt());
        bool OkCancel(const char * sTitle,const cwfOpt & cwOptions = cwfOpt());

        bool SetPleaseWaitSignal(bool * bSignal); 
        void RemovePleaseWaitSignal();
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
        void PleaseWaitWindow(const char * sText = nullptr,const char * sOptions = nullptr);

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
        void PleaseWaitWindow(const char * sText,cwfOpt cwOptions);
        
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
        void PleaseWaitWindow(cwfOpt cwOptions);

        // ClosePlaseWait() -- Hides and Closes opened Please Wait Window
        //
        void ClosePleaseWait();

        // PleaseWaitCancelled() -- Returns true if the Cancel button was pressed on the Please Wait Window.  False, if not or Please Wait Window is not active.
        //
        bool PleaseWaitCancelled(Peek peek = Peek::No);
        
        // PleaseWaitGetOk() -- Changes "Cancel" button to "Ok" button and waits for the button to be pressed.  This is useful when a function has completed and you want to
        //                      make sure the user knows it is finish.
        //
        // This function returns TRUE when the butto is pressed, if the Cancel button exists (i.e. either "CancelOk" or "HideCancel" was specified).  Otherwise, TRUE is returned immediately
        //
        bool PleaseWaitGetOk();

        // PleaseWaitShowCancel() -- Shows (or Hides) the Cancel button.   If opt::CancelOk() or opt::HideCancel() is set in the PleaseWaitWindow() call, PleaseWaitShowCancel() will
        // hide or show the cancel button. 
        //
        // Input:
        //
        // PleaseWaitShowCancel(true) -- Shows the cancel button
        // PleaseWaitShowCancel(false) -- Hide the cancel button
        //
        bool PleaseWaitShowCancel(bool bShow = true);

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
        void SetPleaseWaitProgress(int iPercent,char * sMessage = nullptr);

        // NewDialog() -- Create a new dialog window. 
        // This returns with a CDialog class that allows you to build a dialog window.
        // A dialog window is the same as a normal window, and you can retrieve the window
        // object with auto& cWin = MyDialog.GetWindow();
        //
        // With the dialog class (i.e. auto& MyDialog = NewDialog()), you can add controls, windows, and widgets, all of which are tracked by the CDialog.
        // This allows flexibility in building dialog windows.
        //
        // Dialogs can act as dialogs with a single purpose, or as regular windows with the dialog managing the controls
        // Use the NoClose() option to use the dialog as a regular window.
        //
        // Dialogs can be used as regular popupwindows or placed within an existing window.
        //
        CDialog & NewDialog(int iX,int iY,int Width = 0,int iHeight = 0,const char * sTitle = nullptr,const cwfOpt & cwOpt = cwfOpt());

        // NewDialog() -- Create a new dialog window. 
        // This returns with a CDialog class that allows you to build a dialog window.
        // A dialog window is the same as a normal window, and you can retrieve the window
        // object with auto& cWin = MyDialog.GetWindow();
        //
        // With the dialog class (i.e. auto& MyDialog = NewDialog()), you can add controls, windows, and widgets, all of which are tracked by the CDialog.
        // This allows flexibility in building dialog windows.
        //
        // Dialogs can act as dialogs with a single purpose, or as regular windows with the dialog managing the controls
        // Use the NoClose() option to use the dialog as a regular window.
        //
        // Dialogs can be used as regular popupwindows or placed within an existing window.
        //
        CDialog & NewDialog(int iX,int iY,int iWidth,int iHeight,const cwfOpt & cwOpt);

                // NewDialog() -- Create a new dialog window. 
        // This returns with a CDialog class that allows you to build a dialog window.
        // A dialog window is the same as a normal window, and you can retrieve the window
        // object with auto& cWin = MyDialog.GetWindow();
        //
        // With the dialog class (i.e. auto& MyDialog = NewDialog()), you can add controls, windows, and widgets, all of which are tracked by the CDialog.
        // This allows flexibility in building dialog windows.
        //
        // Dialogs can act as dialogs with a single purpose, or as regular windows with the dialog managing the controls
        // Use the NoClose() option to use the dialog as a regular window.
        //
        // Dialogs can be used as regular popupwindows or placed within an existing window.
        //
        CDialog & NewDialog(int iX,int iY,const char * sTitle,const cwfOpt & cwOpt = cwfOpt());

        // NewDialog() -- Create a new dialog window. 
        // This returns with a CDialog class that allows you to build a dialog window.
        // A dialog window is the same as a normal window, and you can retrieve the window
        // object with auto& cWin = MyDialog.GetWindow();
        //
        // With the dialog class (i.e. auto& MyDialog = NewDialog()), you can add controls, windows, and widgets, all of which are tracked by the CDialog.
        // This allows flexibility in building dialog windows.
        //
        // Dialogs can act as dialogs with a single purpose, or as regular windows with the dialog managing the controls
        // Use the NoClose() option to use the dialog as a regular window.
        //
        // Dialogs can be used as regular popupwindows or placed within an existing window.
        //
        CDialog & NewDialog(int iX,int iY,const cwfOpt & cwOpt = cwfOpt());
    };

    WinDialog dialog;

    // -----------------------------------------------
    // group functions, i.e. MyWindow.group.function()
    // -----------------------------------------------
    //
    // These functions allow grouped buttons, sliders, and other controls to be controlled at a group level so that
    // the individual controls do not need to be handled separately.
    //
    // Groups are usually referred to by the ID established with the Group() option when the controls are created.
    // There is a CGroup class which is experimental that can contain some group information. 
    //
    // Groups take the place of class callbacks for classes of controls (i.e. buttons of the same type differing only by ID), and 
    // can also be used to direct a button to validate edit boxes.
    //
    struct WinGroup
    {
        friend CDevControls;
        friend CWindow;
    private:
        CWindow * m_cWin                ;
    public:
        // group.isPressed() -- Returns whether or not a button in the group specified was pressed.
        // This has many variations to look at a Group ID number or string, as well as provisions to
        // "peek" at the button-press status so it is not reset.
        //
        // When bPeek is false, this is a one-time only call, returning of a button in the group was pressed.
        // A subsequent call will return false until another button is pressed.
        //
        bool isPressed(const char * sGroup,Peek peek = Peek::No,int * iPressedID = nullptr);

        // group.isPressed() -- Returns whether or not a button in the group specified was pressed.
        // This has many variations to look at a Group ID number or string, as well as provisions to
        // "peek" at the button-press status so it is not reset.
        //
        // When bPeek is false, this is a one-time only call, returning of a button in the group was pressed.
        // A subsequent call will return false until another button is pressed.
        //
        bool isPressed(int iGroup,Peek peek = Peek::No,int * iPressedID = nullptr);

        // group.isPressed() -- Returns whether or not a button in the group specified was pressed.
        // This has many variations to look at a Group ID number or string, as well as provisions to
        // "peek" at the button-press status so it is not reset.
        //
        // When bPeek is false, this is a one-time only call, returning of a button in the group was pressed.
        // A subsequent call will return false until another button is pressed.
        //
        bool isPressed(const char * sGroup,int & iPressedID);

        // group.isPressed() -- Returns whether or not a button in the group specified was pressed.
        // This has many variations to look at a Group ID number or string, as well as provisions to
        // "peek" at the button-press status so it is not reset.
        //
        // When bPeek is false, this is a one-time only call, returning of a button in the group was pressed.
        // A subsequent call will return false until another button is pressed.
        //
        bool isPressed(int iRadioGroup,int & iPressedID);

        bool SliderMoved(int iGroupID,int & iSliderID,int & iSliderValue,Peek peek = Peek::No);
        bool SliderMoved(int iGroupID,SliderStruct & stSlider,Peek peek = Peek::No);
        bool SliderMoved(int iGroupID,Peek peek = Peek::No);

        bool SliderMoved(const char * sGroupID,int & iSliderID,int & iSliderValue,Peek peek = Peek::No);
        bool SliderMoved(const char * sGroupID,SliderStruct & stSlider,Peek peek = Peek::No);
        bool SliderMoved(const char * sGroupID,Peek peek = Peek::No);

#if 0
        // GetPressed() has been deprecated

        // Group::GetPressed()
        //
        // Get the Button ID for a PRESSED button in the group.  
        /// If Remove::Yes is entered, then the value is set to 0, allowing
        // GetPressed(MyGroup,Remove::Yes) to be used for retrieving the button press and checking if any button has been pressed.
        // For example, using if (MyButton = Group.GetPresssed(MyGroup,Remove::Yes)) { .. code .. } gives the button only one time, so that
        // returning a 0 will cause the if() to fail.
        //
        // Note: Using an ID(0) for the button will cause this function to give ambiguous results.  If you use ID(0), you can also use
        // Group.isPressed(MyGroup), which will return TRUE only once if a button is pressed -- you can then use GetPressed() to get the ID for
        // the button press with ID(0).
        //
        // An alternate to the above code line is also if (Group.isPressed(MyGroup,iButtonID) which will return TRUE (and also set the button iD) when
        // a button has been pressed.  Note that this only works once (unless bPeek is set) so that you do not need to reset the button status.
        //
        int GetPressed(const char * sGroup,bool bRemove = false);
 
        // Group::GetPressed()
        //
        // Get the Button ID for a PRESSED button in the group.  
        /// If Remove::Yes is entered, then the value is set to 0, allowing
        // GetPressed(MyGroup,Remove::Yes) to be used for retrieving the button press and checking if any button has been pressed.
        // For example, using if (MyButton = Group.GetPresssed(MyGroup,Remove::Yes)) { .. code .. } gives the button only one time, so that
        // returning a 0 will cause the if() to fail.
        //
        // Note: Using an ID(0) for the button will cause this function to give ambiguous results.  If you use ID(0), you can also use
        // Group.isPressed(MyGroup), which will return TRUE only once if a button is pressed -- you can then use GetPressed() to get the ID for
        // the button press with ID(0).
        //
        // An alternate to the above code line is also if (Group.isPressed(MyGroup,iButtonID) which will return TRUE (and also set the button iD) when
        // a button has been pressed.  Note that this only works once (unless bPeek is set) so that you do not need to reset the button status.
        //
        int GetPressed(int iRadioGroup,bool bRemove = false);
#endif
        // GetChecked() -- returns the button ID of a checked Radio Button
        // This only works for RadioButtons.
        //
        // In a RadioButton group, one button is always checked (as opposed to Checkboxes where any checkbox may
        // be checked or unchecked). 
        //
        // GetChecked() returns the ID if the radio button checked in the group specified.
        //
        int GetChecked(const char * sGroupID);

        // GetChecked() -- returns the button ID of a checked Radio Button
        // This only works for RadioButtons.
        //
        // In a RadioButton group, one button is always checked (as opposed to Checkboxes where any checkbox may
        // be checked or unchecked). 
        //
        // GetChecked() returns the ID if the radio button checked in the group specified.
        //
        int GetChecked(int iGroupID);

        bool SetCheck(int iGroupID,int iControlID); 
        bool SetCheck(const char * sGroupID,int iControlID); 


        // GetGroupID() -- Returns the group ID of a group specified with a string.
        //
        // If the original group is specified with a string (i.e. Group("MyGroup") -- as opposed to a number, i.e. Group(1)),
        // GetGroupID() returns the numeric ID assigned to the group.
        //
        // Using a numeric value in group operations is much faster.  This number can be used instead of the string-based group name
        //
        int GetGroupID(const char * sGroup);

        // GetGroup() -- This returns a ControlGroup Class.  This is experimental and TBD
        //
        ControlGroup GetGroup(const char * sControl);
        // GetGroup() -- This returns a ControlGroup Class.  This is experimental and TBD
        //
        ControlGroup GetGroup(int iControl);

        // CreateGroup() -- Creates a group (This is experimental and in early stages)
        //
        // CreateGroup() pre-creates a group that can be specified when creating a control.  CreateGroup() is not necessary, as
        // the first usage of a Group ID or Name creates the group when it is first seen.
        //
        int CreateGroup(const char * sGroupName,GroupType eGroupType = GroupType::Undefined); 
       SIZE CreateRadioButtonGroup(int iNumButtons,int iX,int iY,const char * * sButtonNames,int iGroupID, const char * sLabel, const cwfOpt & cwOpt = cwfOpt());
       SIZE CreateCheckboxGroup(int iNumButtons,int iX,int iY,const char * * sButtonNames,int iGroupID, const char * sLabel, const cwfOpt & cwOpt = cwfOpt());
       CButton * GetButton(int iGroupID, int iPosition);
       bool Enable(int iGroupID,bool bEnable);
       bool Disable(int iGroupID,bool bDisable);

        bool CreateButtonGroup(int iGroupID,int iNumButtons,int iX,int iY,int iWidth,int iHeight,const wchar_t * * sButtonNames,int iColumns = 0, SIZE szSpacing = {-1,-1}, const cwfOpt & cwOpt = cwfOpt());
        bool CreateButtonGroup(int iGroupID,int iNumButtons,int iX,int iY,const wchar_t * * sButtonNames,int iColumns = 0, const cwfOpt & cwOpt = cwfOpt());
        bool CreateButtonGroup(int iGroupID,int iNumButtons,int iX,int iY,int iWidth,int iHeight,const char * * sButtonNames,int iColumns = 0, SIZE szSpacing = {-1,-1}, const cwfOpt & cwOpt = cwfOpt());
        bool CreateButtonGroup(int iGroupID,int iNumButtons,int iX,int iY,const char * * sButtonNames,int iColumns = 0, const cwfOpt & cwOpt = cwfOpt());



       bool SetHoverMsg(int iGroupID,int iPosition,const char * sMessage); 

    };

    // Location for group functions that can help using buttons, edit boxes, and other controls by grouping them together
    //
    WinGroup group;

    // GetWinBkMode() -- Get the text background mode.
    // BkMode returns eeither BkMode::Transparent or BkMode::Opaque
    //
    // BkMode::Transparent()  --- This is the default.  When text is written to the screen, only the text part is written in the
    //                            foreground color, leaving the blank areas (i.e. text or graphics behind the new text) untouched.
    //
    // BkMode::Opaque         -- When text is written to the window, this will write the text in the foreground color and fill in the background with the
    //                           background color overwriting any text or graphics behind the new text.
    //
    BkMode GetWinBkMode(); 

    // SetWinBkMode() -- Set the text background mode.
    // BkMode returns eeither BkMode::Transparent or BkMode::Opaque
    //
    // BkMode::Transparent()  --- This is the default.  When text is written to the screen, only the text part is written in the
    //                            foreground color, leaving the blank areas (i.e. text or graphics behind the new text) untouched.
    //
    // BkMode::Opaque         -- When text is written to the window, this will write the text in the foreground color and fill in the background with the
    //                           background color overwriting any text or graphics behind the new text.
    //
    bool SetWinBkMode(BkMode eBkMode); 
  
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
    bool SetBgColor(const char * sColor);

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
    bool SetBgColor(DWORD dwColor);

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
    bool SetBgColor(RGBColor_t rgbColor);

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
    bool SetFgColor(const char * sColor);

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
    bool SetFgColor(DWORD dwColor);

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
    bool SetFgColor(RGBColor_t rgbColor);

    // -------------------------
    // event functions
    // -------------------------
    //
    // This is where all event function calls are collected.  In most cases, these can be called without the 'event.' identifier, as they are
    // replicated in the CWindow class.
    //
    // However, using "event." will cause the list of options to appear so that all event possibilities can be seen easily.
    //
    // For example, MyWindow.event.MouseClicked() is the same as MyWindow.MouseClicked(). 
    //
    struct WinEvent
    {
        friend CWindow;
    private:
        CWindow * m_cWin;
    public:
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
        // Use GetMouseClickPos() to retrieve the last mouse-click coordinates.
        //
        bool MouseClicked(Peek peek = Peek::No);
        bool MouseClicked(POINT & pMouse,Peek peek = Peek::No);

        bool MouseDragEvent(Peek peek = Peek::No);
        bool MouseDragEvent(POINT & pMouse,Peek peek = Peek::No);

        bool MouseDragEnded(Peek peek = Peek::No);
        bool MouseDragEnded(POINT & pMouse,Peek peek = Peek::No);

        bool isMouseDragging(); 
        bool isMouseDragging(POINT & pStartDrag); 
        
        POINT GetMouseDragStart();

        // LButtonPressed() -- returns true if the Left Mouse Button was clicked (same as MouseClicked())
        //
        // This is an event and is a one-time only read so that the status is reset 
        // (i.e. the status wont become true again until the next mouse click)
        //
        // Use LButtonPressed(true) to "peek" at the value so that it will be reset.
        // This will allow subsequent reads before it is cleared.  However, it
        // must be read once without bPeek == true in order for the flag to be reset for the next 
        // event.
        //
        // Use GetMouseClickPos() to retrieve the last mouse-click coordinates.
        //
        bool LButtonPressed(Peek peek = Peek::No);

        // RButtonPressed() -- returns true if the Right Mouse Button was clicked
        //
        // This is an event and is a one-time only read so that the status is reset 
        // (i.e. the status wont become true again until the next mouse click)
        //
        // Use RButtonPressed(true) to "peek" at the value so that it will be reset.
        // This will allow subsequent reads before it is cleared.  However, it
        // must be read once without bPeek == true in order for the flag to be reset for the next 
        // event.
        //
        // Use GetMouseClickPos() to retrieve the last mouse-click coordinates.
        //
        bool RButtonPressed(Peek peek = Peek::No);
 
        // LButtonUnpressed() -- returns true when the left button is unclicked.
        //
        // This is an event and is a one-time only read so that the status is reset 
        // (i.e. the status wont become true again until the next mouse un-click)
        //
        // Use LButtonUnpressed(true) to "peek" at the value so that it will be reset.
        // This will allow subsequent reads before it is cleared.  However, it
        // must be read once without bPeek == true in order for the flag to be reset for the next 
        // event.
        //
        bool LButtonUnpressed(Peek peek = Peek::No);

        // RButtonUnpressed() -- returns true when the right button is unclicked.
        //
        // This is an event and is a one-time only read so that the status is reset 
        // (i.e. the status wont become true again until the next mouse un-click)
        //
        // Use LButtonUnpressed(true) to "peek" at the value so that it will be reset.
        // This will allow subsequent reads before it is cleared.  However, it
        // must be read once without bPeek == true in order for the flag to be reset for the next 
        // event.
        //
        bool RButtonUnpressed(Peek peek = Peek::No);

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
        bool MouseMoved(Peek peek = Peek::No);

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
        bool MouseMoved(POINT & pPoint,Peek peek = Peek::No);

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
        int       ButtonPressed(Peek peek = Peek::No)                         ;

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
        bool      ButtonPressed(int & iButtonID,Peek peek = Peek::No)         ;

        // MenuItemSelected() -- This works as an event, and will fill the menu item if it is selected.
        // As an event, subsquent calls will return false and not fill the menu item until another menu item is selected.
        //
        // If the iMenuItem is not included in the call, GetMenuItem() can be used to retrieve the last menu item selected
        //
        bool      MenuItemSelected(int & iMenuItem,Peek peek = Peek::No)      ;

        // MenuItemSelected() -- This works as an event, and will fill the menu item if it is selected.
        // As an event, subsquent calls will return false and not fill the menu item until another menu item is selected.
        //
        // If the iMenuItem is not included in the call, GetMenuItem() can be used to retrieve the last menu item selected
        //
        bool      MenuItemSelected(Peek peek = Peek::No)                      ;

        // Retrieves the last menu item selected.
        // 
        // This is meant to be used right after MenuItemSelected() is called, and will return the last menu item selected.
        //
        // This will continue to return the same menu item until a new menu item is selected.
        //
        int       GetMenuItem()                                             ;     
 
        // Returns true if the Left Mouse Button is currently pressed.  
        // This is not a mouse event and returns the real-time status of the mouse.
        //
        bool MouseButtonDown();

        // Returns true if the Right Mouse Button is currently pressed.  
        // This is not a mouse event and returns the real-time status of the mouse.
        //
        bool MouseRButtonDown();
        bool MouseRButtonClicked(POINT & pPoint,Peek peek = Peek::No);
        bool MouseRButtonClicked(Peek peek);

        // Returns true of the middle mouse button mouse was double clicked.
        // *** This function is still in development and may not work.
        // This is tied to the status of the window an whether or not it will accept double-clicks.
        //
        bool MouseDoubleClicked();     

        // GetMousePos() -- Returns the current mouse coordinates relative to the window
        //
        bool GetMousePos(int & iMouseX,int & iMouseY);

        // GetMousePos() -- Returns the current mouse coordinates relative to the window
        //
        POINT GetMousePos();     

        // GetMouseClickPos() -- Returns the last mouse click coordinates.
        //
        // This can be used after a MouseClick() event to return the mouse-click coordinates
        // at the time the mouse was clicked.
        //
        // This works for both left button and right button clicks.
        //
        bool      GetMouseClickPos(int & iMouseX,int & iMouseY);

        // GetMouseClickPos() -- Returns the last mouse click coordinates.
        //
        // This can be used after a MouseClick() event to return the mouse-click coordinates
        // at the time the mouse was clicked.
        //
        // This works for both left button and right button clicks.
        //
        POINT     GetMouseClickPos();     

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
        bool      CloseButtonPressed(Peek peek = Peek::No);

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
        WaitEvent WaitforEvent(const char * sEvent = nullptr);

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
        bool EventLoop(WaitEvent * eStatus = nullptr);
        
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
        bool GetEvent(WaitEvent * eStatus = nullptr);

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
        int WaitforClose(int iReturnValue = 0);     

        // ExitButton() -- Places a "Program Finished. Press Button to Continue" on the bottom of the screen and waits for input before
        // continuing.  This is useful when the program ends, to allow the user to press the button before the window closes.
        //
        // ExitButton() returns 0
        //
        int ExitButton(const char * sText = nullptr);
 
        // NoExitMsg() -- Used for Sandbox applications to tell the calling process to not display the exit button w/message on the bottom of the window.
        //
        // NoExitMsg() returns 0 unless a return code is specified.
        //
        int NoExitMsg(bool bNoExit=true); 

        // NoExitMsg() -- Used for Sandbox applications to tell the calling process to not display the exit button w/message on the bottom of the window.
        //
        // NoExitMsg() returns 0 unless a return code is specified.
        //
        int NoExitMsg(int iReturnCode,bool bNoExit=true); 

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
        bool      WindowClosing();     

        // Reset Window Closing Status.   
        //
        // This resets the Windows Closing status returned by WindowClosing().  Many Sagebox functions will not activate or passthrough when the
        // WindowClosing() status is set to true.
        //
        // This can be used when a user has pressed the close button and CloseButtonPessed() returns true.
        // To cancel the user-based close, call ResetWindowClosing()
        //
        bool ResetWindowClosing();    

        // Set the Window status as closing.  This causes WindowClosing() to return true. 
        // if bPressCloseButton is TRUE, then this also presses the close button and also sends any signals attached to it. 
        //
        // Pressing the close button through the bPressCloseButtonFlag allows functions to check the one-time press event and react to it
        // one time, rather than WindowClosing() that always remains true unless reset. 
        //
        bool SetWindowClosing(bool bPressCloseButton = false);     

        bool MouseWheelMoved(Peek peek = Peek::No);
        bool MouseWheelMoved(int & iDistance,Peek peek = Peek::No);
        int GetMouseWheelMove(bool bResetEvent = false); 

        bool WindowResized(Peek peek = Peek::No);
        bool WindowResized(SIZE & szNewWinSize,Peek peek = Peek::No);

        // Returns TRUE when the mouse capture has been release.  This only returns true when
        // the mouse capture previously engaged for the window has been released.
        //
        // This returns an event status.  Therefore, the event is reset after the call and will return false 
        // afterward until a subsequent release -- unless peek is set to Peek::No, in which case the status is not reset
        //
        bool CaptureReleased(Peek peek = Peek::No);

        // Returns true if an event has occurred.  This can be used when GetEvent() isn't appropriate (such as in a loop that takes a lot of time)
        // When GetEvent() is called, the Event Pending Status is cleared back to false
        //
        __forceinline bool EventReady() { return !m_cWin ? false : m_cWin->EventReady(); };

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
        bool EndProgramOnClose(bool bEndOnClose); 

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
        bool EndProgramOnClose(int iReturnValue = 0); 

        // ClearEvent() -- Clear a specific pending event (whether it is pending or not).  This can be called before GetEvent() to clear the 
        // Event Queue of any specific event. For example, ClearEvent(SageEvent::MouseMove) or ClearEvent(SageEvent::WindowResize). 
        //
        // The latter example can be used to remove a pending window size when a Windows was initially sized with SetWindowSize(), which will
        // set a pending WindowResize() event.
        //
        // Use ClearEvents() to clear all pending events. 
        //
        __forceinline bool ClearEvent(SageEvent eEvent) { return (!m_cWin) ? false : m_cWin->ClearEvent(eEvent); }
        
        // CLearEvents() -- Clear all pending events.  This can be used to clear any incoming events prior to calling GetEvent(). 
        // This can be useful when a setup, initialization, or other code has caused an unwanted event prior to the GetEvent() loop.
        //
        __forceinline bool ClearEvents() { return ClearEvent(SageEvent::All); }
        
        // SendEventsToParent() -- Send all events to Parent Window while also sending them to the Window itself.
        //
        // This function is mucnh like SetEventWindow() which will tell Sagebox where else to send all events through GetEvent().
        // Child windows send events to their parents (as well as themselves) by default, meaning you can use GetEvent() for the parent window
        // and receive events for the window itself.
        //
        // For windows created with Sagebox (and some other instances), there is no Event Window set, and to receive events through the
        // CSageBox::GetEvent() loop, SendEventsToParent() must be called -- this will send all events to the Sagebox main window (which is hidden)
        //
        bool SendEventsToParent(); 

    };

    WinEvent event;

    // ------------------------
    // Ascii Terminal Functions
    // ------------------------
    // 
    // These functions are meant to provide an Ascii terminal set of functions for CWindow.
    //
    // These are experimental and still in-progress -- they will probably be moved to a class, such as 
    // CTerminal() or somesuch when it is finished.
    //
    struct WinTerm
    {

        friend CWindow;
    private:
        CWindow * m_cWin;
        CPoint m_pTermXY = {};
        CPoint m_pTermPos = {};
    public:
        int putchar(char cChar);
        void InitTerm(int iX,int iY,int iCharWidth,int iNumLines,int iFontSize,const cwfOpt & cwOpt = cwfOpt());
        void InitTerm(int iX,int iY,const cwfOpt & cwOpt = cwfOpt());
        void InitTerm(int iX,int iY,int iFontSize,const cwfOpt & cwOpt = cwfOpt());
        void SetPos(int iX,int iY);
    };

    WinTerm term;

    // -----------------------------
    // bitmap fuction -- in-progress
    // -----------------------------
    //
    // Sagebox has a very large set of raw bitmap functions.  This is a growing collection of the functions available.
    // This collects all of the Sage class bitmap functions that, at the moment, require Sage::<function name>
    //
    // This is still in-progress and will grow on future releases.

    struct WinBitmap
    {
        friend CWindow;
    private:
        CWindow * m_cWin                ;
    public:
        [[nodiscard]] RawBitmap_t CreateBitmap(int iWidth,int iHeight = 1);
        [[nodiscard]] RawBitmap_t ReadBitmap(const char * sPath,bool * bSucceeded = nullptr);

        // Send Contents of Bitmap to the clipboard
        //
        // note: This will be changed to use a CBitmap rather than a RawBitmap_t in the next release. 
        //
        bool SendtoClipboard(RawBitmap_t & stBitmap);
        
        // Send contents of window to the clipboard.
        //
        bool SendtoClipboard(); 

    };

    WinBitmap bitmap;

    // -----------------
    // Console Functions
    // -----------------
    // 
    // This provides a large set of functins used for console-based functions, similar to regular C++ console functions.
    // For example, MyWindow.console.GetString() gets a string directly on the current console output location, as opposed to
    // myWindow.GetString() or MyWindow.dialog.GetString() which brings up a dialog window.
    //
    // These can be used for quick console-based functions that act more like a regular C-based console than windowing functions.
    //
    struct WinConsole
    {
        enum class InputBorder
        {
            None         ,  // Default.  No border on input boxes
            Normal       ,  // Border with an outline
            Thick        ,  // Thisnk border, more like a window
            Reset        ,  // Use this to set default border
            Default      ,  // Use this to set default border
        };

        friend CWindow;
    private:
        CWindow         * m_cWin;
        DWORD             m_dwBgInput;
        DWORD             m_dwFgInput; 
        bool              m_bBgInputSet;
        bool              m_bFgInputSet;
        InputBorder       m_eInputBorder;
        InputBorder       m_eDefaultBorder;

    public:

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
        const char * GetString(const char * sDefault = nullptr,const cwfOpt & cwOpt = cwfOpt());
 
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
        const char * GetString(const cwfOpt & cwOPt);
    
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
        int GetInteger(int iDefault = INT_MAX,const cwfOpt & cwOpt = cwfOpt());

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
        int GetInteger(const cwfOpt & cwOpt);

        // EditBox() -- Set an edit box to get information.  This is returns a control object and does not stop
        // execution waiting for the input.
        //
        // Multiple edit boxes may be placed on the screen, and a button may be assigned to a group of edit boxes for validation.
        //
        // Pressing <CR> or the window will set an edit box event for the control returns.  Empty input is defined by a "" string. 
        // Check return value [0] == 0 for no string entry.
        //
        // Pressing ESC will clear the text entry, remaining in the edit box for more input.  When there is a default set of text, the text will revert to the original default.
        // 
        //Edit boxes can be limited to numbers by setting Min(), Max(), Range() or by explicitly defining NumbersOnly() or FloatsOnly()
        //
        // sLabel Parameter -- this will call Write() to print the text just prior to creating the edit box
        //
        // Parameters in sControl
        //
        // --> WinColors()      -- Sets a color scheme to a the current window colors, blending the box (i.e. there is no input box)
        // --> ColorsBW()       -- (default) Sets a color scheme to a white background with black text, allowing the box to stand out. The default color is the current windows background color
        //                         and text color, showing the input as the same color of the window text displayed in the window.
        //
        // --> Min()            -- Set minimum value.  A value entered below this value will generate an error and prompt the user to enter another value
        // --> Max()            -- Set maximum value.  A value entered below this value will generate an error and prompt the user to enter another value
        // --> Range()          -- Set minimum and maximum value.  A value entered outside this range will generate an error and prompt the user to enter another value
        // --> NumbersOnly      -- Sets numbers only (integer).  Using Min(),Max(), or Range() also sets this value
        // --> FloatsOnly       -- Sets for floating-point numbers only.  Using Min(), Max() or Range() with floating-point values also sets this value
        // --> TextLeft()       -- Justifies Text to the left.  For strings this is the default.  For numbers, TextRight() is the default.
        // --> TextRight()      -- Justifies Text to the right.  For strings this is the default.  For numbers, TextRight() is the default.
        // --> TextCenter()     -- Justifies Text to the center of the input box.  For strings this is the default.  For numbers, TextRight() is the default.
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
        // auto & MyEditbox = MyWindow.console.EditBox() -- Creates a small edit box.  Use Width() or CharWidth() to creae a wider box
        // auto & MyEditBox = MyWindow.console.EditBox("Enter Some Text: ",Width(200));
        // auto & MyEditbox = MyWindow.console.EditBox("Enter a number: ",FloatsOnly());
        // auto & MyEditbox = MyWindow.console.EditBox("Enter a number: ",Range(-10.0,10.0)); (also sets FloatsOnly setting)
        //
        CEditBox & EditBox(const cwfOpt & cwOptions = cwfOpt());                                            

        // EditBox() -- Set an edit box to get information.  This is returns a control object and does not stop
        // execution waiting for the input.
        //
        // Multiple edit boxes may be placed on the screen, and a button may be assigned to a group of edit boxes for validation.
        //
        // Pressing <CR> or the window will set an edit box event for the control returns.  Empty input is defined by a "" string. 
        // Check return value [0] == 0 for no string entry.
        //
        // Pressing ESC will clear the text entry, remaining in the edit box for more input.  When there is a default set of text, the text will revert to the original default.
        // 
        //Edit boxes can be limited to numbers by setting Min(), Max(), Range() or by explicitly defining NumbersOnly() or FloatsOnly()
        //
        // sLabel Parameter -- this will call Write() to print the text just prior to creating the edit box
        //
        // Parameters in sControl
        //
        // --> WinColors()      -- Sets a color scheme to a the current window colors, blending the box (i.e. there is no input box)
        // --> ColorsBW()       -- (default) Sets a color scheme to a white background with black text, allowing the box to stand out. The default color is the current windows background color
        //                         and text color, showing the input as the same color of the window text displayed in the window.
        //
        // --> Min()            -- Set minimum value.  A value entered below this value will generate an error and prompt the user to enter another value
        // --> Max()            -- Set maximum value.  A value entered below this value will generate an error and prompt the user to enter another value
        // --> Range()          -- Set minimum and maximum value.  A value entered outside this range will generate an error and prompt the user to enter another value
        // --> NumbersOnly      -- Sets numbers only (integer).  Using Min(),Max(), or Range() also sets this value
        // --> FloatsOnly       -- Sets for floating-point numbers only.  Using Min(), Max() or Range() with floating-point values also sets this value
        // --> TextLeft()       -- Justifies Text to the left.  For strings this is the default.  For numbers, TextRight() is the default.
        // --> TextRight()      -- Justifies Text to the right.  For strings this is the default.  For numbers, TextRight() is the default.
        // --> TextCenter()     -- Justifies Text to the center of the input box.  For strings this is the default.  For numbers, TextRight() is the default.
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
        // auto & MyEditbox = MyWindow.console.EditBox() -- Creates a small edit box.  Use Width() or CharWidth() to creae a wider box
        // auto & MyEditBox = MyWindow.console.EditBox("Enter Some Text: ",Width(200));
        // auto & MyEditbox = MyWindow.console.EditBox("Enter a number: ",FloatsOnly());
        // auto & MyEditbox = MyWindow.console.EditBox("Enter a number: ",Range(-10.0,10.0)); (also sets FloatsOnly setting)
        //
        CEditBox & EditBox(const char * sLabel,const cwfOpt & cwOptions = cwfOpt());                            

        // EditBox() -- Set an edit box to get information.  This is returns a control object and does not stop
        // execution waiting for the input.
        //
        // Multiple edit boxes may be placed on the screen, and a button may be assigned to a group of edit boxes for validation.
        //
        // Pressing <CR> or the window will set an edit box event for the control returns.  Empty input is defined by a "" string. 
        // Check return value [0] == 0 for no string entry.
        //
        // Pressing ESC will clear the text entry, remaining in the edit box for more input.  When there is a default set of text, the text will revert to the original default.
        // 
        //Edit boxes can be limited to numbers by setting Min(), Max(), Range() or by explicitly defining NumbersOnly() or FloatsOnly()
        //
        // sLabel Parameter -- this will call Write() to print the text just prior to creating the edit box
        //
        // Parameters in sControl
        //
        // --> WinColors()      -- Sets a color scheme to a the current window colors, blending the box (i.e. there is no input box)
        // --> ColorsBW()       -- (default) Sets a color scheme to a white background with black text, allowing the box to stand out. The default color is the current windows background color
        //                         and text color, showing the input as the same color of the window text displayed in the window.
        //
        // --> Min()            -- Set minimum value.  A value entered below this value will generate an error and prompt the user to enter another value
        // --> Max()            -- Set maximum value.  A value entered below this value will generate an error and prompt the user to enter another value
        // --> Range()          -- Set minimum and maximum value.  A value entered outside this range will generate an error and prompt the user to enter another value
        // --> NumbersOnly      -- Sets numbers only (integer).  Using Min(),Max(), or Range() also sets this value
        // --> FloatsOnly       -- Sets for floating-point numbers only.  Using Min(), Max() or Range() with floating-point values also sets this value
        // --> TextLeft()       -- Justifies Text to the left.  For strings this is the default.  For numbers, TextRight() is the default.
        // --> TextRight()      -- Justifies Text to the right.  For strings this is the default.  For numbers, TextRight() is the default.
        // --> TextCenter()     -- Justifies Text to the center of the input box.  For strings this is the default.  For numbers, TextRight() is the default.
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
        // auto & MyEditbox = MyWindow.console.EditBox() -- Creates a small edit box.  Use Width() or CharWidth() to creae a wider box
        // auto & MyEditBox = MyWindow.console.EditBox("Enter Some Text: ",Width(200));
        // auto & MyEditbox = MyWindow.console.EditBox("Enter a number: ",FloatsOnly());
        // auto & MyEditbox = MyWindow.console.EditBox("Enter a number: ",Range(-10.0,10.0)); (also sets FloatsOnly setting)
        //
        CEditBox & EditBox(CEditBox * pObject,const cwfOpt & cwOptions = cwfOpt());        

        // EditBox() -- Set an edit box to get information.  This is returns a control object and does not stop
        // execution waiting for the input.
        //
        // Multiple edit boxes may be placed on the screen, and a button may be assigned to a group of edit boxes for validation.
        //
        // Pressing <CR> or the window will set an edit box event for the control returns.  Empty input is defined by a "" string. 
        // Check return value [0] == 0 for no string entry.
        //
        // Pressing ESC will clear the text entry, remaining in the edit box for more input.  When there is a default set of text, the text will revert to the original default.
        // 
        //Edit boxes can be limited to numbers by setting Min(), Max(), Range() or by explicitly defining NumbersOnly() or FloatsOnly()
        //
        // sLabel Parameter -- this will call Write() to print the text just prior to creating the edit box
        //
        // Parameters in sControl
        //
        // --> WinColors()      -- Sets a color scheme to a the current window colors, blending the box (i.e. there is no input box)
        // --> ColorsBW()       -- (default) Sets a color scheme to a white background with black text, allowing the box to stand out. The default color is the current windows background color
        //                         and text color, showing the input as the same color of the window text displayed in the window.
        //
        // --> Min()            -- Set minimum value.  A value entered below this value will generate an error and prompt the user to enter another value
        // --> Max()            -- Set maximum value.  A value entered below this value will generate an error and prompt the user to enter another value
        // --> Range()          -- Set minimum and maximum value.  A value entered outside this range will generate an error and prompt the user to enter another value
        // --> NumbersOnly      -- Sets numbers only (integer).  Using Min(),Max(), or Range() also sets this value
        // --> FloatsOnly       -- Sets for floating-point numbers only.  Using Min(), Max() or Range() with floating-point values also sets this value
        // --> TextLeft()       -- Justifies Text to the left.  For strings this is the default.  For numbers, TextRight() is the default.
        // --> TextRight()      -- Justifies Text to the right.  For strings this is the default.  For numbers, TextRight() is the default.
        // --> TextCenter()     -- Justifies Text to the center of the input box.  For strings this is the default.  For numbers, TextRight() is the default.
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
        // auto & MyEditbox = MyWindow.console.EditBox() -- Creates a small edit box.  Use Width() or CharWidth() to creae a wider box
        // auto & MyEditBox = MyWindow.console.EditBox("Enter Some Text: ",Width(200));
        // auto & MyEditbox = MyWindow.console.EditBox("Enter a number: ",FloatsOnly());
        // auto & MyEditbox = MyWindow.console.EditBox("Enter a number: ",Range(-10.0,10.0)); (also sets FloatsOnly setting)
        //
        CEditBox & EditBox(CEditBox * pObject,char * sLabel,const cwfOpt & cwOptions = cwfOpt());        

        // InputButton() -- Put a quick button at the current write location (or specified X,Y location). 
        // With no parameters, Input Button puts a quick button with the text "Continue" and waits for input at the current write location.
        //
        // Input Button is a good way to get quick input that stops the program execution and continues when the user presses the button.
        //
        // Using InputButton("My Text") puts the text inside of the button instead of "Contuinue"
        // Using InputButtin(X,Y) will put the button at the specific coordinates.
        //
        // When the user presses the button, the button disappears and the text output will continue at the original Write Position.
        //
        // Justification can also be used:
        //
        // JustCenter() will center the button on the current line
        // JustBottomCenter() will center the button on the bottom line.
        // JustTopCenter() will center the button on the top line.
        // 
        // See all opt:: justification settings for more information.
        void InputButton(int iX,int iY,const char * sButtonText = nullptr,const cwfOpt & cwOptions = cwfOpt());

        // InputButton() -- Put a quick button at the current write location (or specified X,Y location). 
        // With no parameters, Input Button puts a quick button with the text "Continue" and waits for input at the current write location.
        //
        // Input Button is a good way to get quick input that stops the program execution and continues when the user presses the button.
        //
        // Using InputButton("My Text") puts the text inside of the button instead of "Contuinue"
        // Using InputButtin(X,Y) will put the button at the specific coordinates.
        //
        // When the user presses the button, the button disappears and the text output will continue at the original Write Position.
        //
        // Justification can also be used:
        //
        // JustCenter() will center the button on the current line
        // JustBottomCenter() will center the button on the bottom line.
        // JustTopCenter() will center the button on the top line.
        // 
        // See all opt:: justification settings for more information.
        void InputButton(int iX,int iY,cwfOpt & cwOptions);

        // InputButton() -- Put a quick button at the current write location (or specified X,Y location). 
        // With no parameters, Input Button puts a quick button with the text "Continue" and waits for input at the current write location.
        //
        // Input Button is a good way to get quick input that stops the program execution and continues when the user presses the button.
        //
        // Using InputButton("My Text") puts the text inside of the button instead of "Contuinue"
        // Using InputButtin(X,Y) will put the button at the specific coordinates.
        //
        // When the user presses the button, the button disappears and the text output will continue at the original Write Position.
        //
        // Justification can also be used:
        //
        // JustCenter() will center the button on the current line
        // JustBottomCenter() will center the button on the bottom line.
        // JustTopCenter() will center the button on the top line.
        // 
        // See all opt:: justification settings for more information.
        void InputButton(const char * sButtonText = nullptr,const cwfOpt & cwOptions = cwfOpt());

        // InputButton() -- Put a quick button at the current write location (or specified X,Y location). 
        // With no parameters, Input Button puts a quick button with the text "Continue" and waits for input at the current write location.
        //
        // Input Button is a good way to get quick input that stops the program execution and continues when the user presses the button.
        //
        // Using InputButton("My Text") puts the text inside of the button instead of "Contuinue"
        // Using InputButtin(X,Y) will put the button at the specific coordinates.
        //
        // When the user presses the button, the button disappears and the text output will continue at the original Write Position.
        //
        // Justification can also be used:
        //
        // JustCenter() will center the button on the current line
        // JustBottomCenter() will center the button on the bottom line.
        // JustTopCenter() will center the button on the top line.
        // 
        // See all opt:: justification settings for more information.
        void InputButton(cwfOpt & cwOptions);

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
        double getFloat(double fDefault = DBL_MAX,const cwfOpt & cwOpt = cwfOpt());

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
        double getFloat(const cwfOpt & cwOpt = cwfOpt());

        // SetTabPos() -- Set the X position for Write/out/putchar/etc output to the character nth character position.
        // For example. SetTabPos(40) puts the output position (i.e. cursor) at charatcer position 40, based on the average character width.
        // This can be used to align console input boxes, text, etc. 
        // Since most fonts are proportionally spaced, the average character width is used.  In the case of terminal fonts, the character width is
        // the same for all characters and is more predictable
        //
        // This is included as a function from Basic() that is used in a number of programs
        //
        bool SetTabPos(int iPos);                

        // Tab(int iTab) -- Sets the X position <iTab> characters ahead of the current write position. 
        // The characters size is the average character size for the font. 
        //
        // See SetTabPos() to set a specific position.
        //
        bool Tab(int iTab);

        // Sets the Write Output Position to 0 -- same as using SetWritePosX(0).  This is the equivalent to printing a <cr>
        //
        bool CR();

        // ClearLine() -- Clears the current line.  
        // This can be used when writing to a line repeatedly (such as a counter, percent done, etc.) to clear the
        // line before writing.  This helps with transparent modes (i.e. font only overwrites the character portion),
        // and strings that may change in length.
        //
        // This faster than clearing the screen or a wider region.
        //
        // In many cases, a TextWidget is a better option since it doesn't need to be managed and it can just be output to without
        // clearing or managing the background space.  TextWidgets can also be transparent to blend into the background.
        //
        bool ClearLine(int iYPos = -1);

        // AddWritePosX(int iAdd) -- Add <iAdd> pixels to the current X write position for text output.
        // This is simiar to SetTabPos() except the value is pixels. 
        //
        bool AddWritePosX(int iAdd);

        // AddWritePosY(int iAdd) -- Add <iAdd> pixels to the current Y write position for text output.
        //
        bool AddWritePosY(int iAdd);

        // SetInputColors() -- This sets the background and foreground colors of the text inputted through GetString(), GetInteger(), etc.
        // When the input box disappears, the text is reprinted.  
        //
        // These colors allow the inputted text (when printed out after the input box disappears) to be shown in a different color.
        //
        bool SetInputColors(DWORD dwBgColor,DWORD dwFgColor);

        // SetInputColors() -- This sets the background and foreground colors of the text inputted through GetString(), GetInteger(), etc.
        // When the input box disappears, the text is reprinted.  
        //
        // These colors allow the inputted text (when printed out after the input box disappears) to be shown in a different color.
        //
        bool SetInputColors(char * sBgColor,char * sFColor);

        // SetInputFgColor() -- This sets the foreground color of the text inputted through GetString(), GetInteger(), etc.
        // When the input box disappears, the text is reprinted.  
        //
        // This color allows the inputted text (when printed out after the input box disappears) to be shown in a different color.
        //
        bool SetInputFgColor(DWORD dwFgColor);

        // SetInputFgColor() -- This sets the foreground color of the text inputted through GetString(), GetInteger(), etc.
        // When the input box disappears, the text is reprinted.  
        //
        // This color allows the inputted text (when printed out after the input box disappears) to be shown in a different color.
        //
        bool SetInputFgColor(char * sFgColor);
        
        // ResetInputColors() -- Reset the text input (once printed out again after the input box disappears)
        // to console.GetString(), console.GetInteger(), etc. to default.
        //
        // The default colors are the current Window background and foreground color.
        // Set SetInputColors() and SetFgInputColor() for more information.
        //
        bool ResetInputColors();

        // GetBgColor() -- Get the current backround color for the window.
        // This color is used for Cls() and all text output routines.
        //
        RGBColor_t GetBgColor();

        // GetFgColor() -- Get the current foreground (i.e. Text) color for the window.
        // This color is used all text output routines.
        //
        RGBColor_t GetFgColor();

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
        bool SetBgColor(const char * sColor);

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
        bool SetBgColor(DWORD dwColor);

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
        bool SetBgColor(RGBColor_t rgbColor);

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
        bool SetFgColor(const char * sColor);

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
        bool SetFgColor(DWORD dwColor);

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
        bool SetFgColor(RGBColor_t rgbColor);

        // SetWritePosX(int iX) -- Set the current X write position to iX
        // This sets the current X position for all text output routines on the current line.
        //
        bool SetWritePosX(int iX);

        // SetWritePos(int iX,iY) -- Set the current X,Y value for all text output routines to the 
        // specific coordinates.
        //
        bool SetWritePos(int iX,int iY);

        // UpdateLine(iUpdateMS) -- Update the current line. 
        // This can be much faster than using Update(), since Update() updates the entire screen.
        // This can also be used with UpdateReady(iUpdateMS) to determine whether to draw the part of the line prior
        // to updating.
        //
        // iUpdateMS() will only uddate the line every <iUpateMS> milliseconds.  Therefore, one last  UpdateLine() or Update() (without iUpdateMS) will be
        // necessary to ensure the latest output.
        //
        bool UpdateLine(int iUpdateMS = 0);


        // This sets the default Input border for the input boxes in GetString, GetInteger, etc. functions.
        // The options may be:
        //
        // "None"      
        // "Normal"
        // "Thick"    
        // "Default"   
        //
        // Example: SetInputBorder("Thick");
        //
        bool SetInputBorder(char * sBorderType);

        // This sets the default Input border for the input boxes in GetString, GetInteger, etc. functions.
        // The options may be:
        //
        // InputBorder:None      
        // InputBorder:Normal    
        // InputBorder:Thick     
        // InputBorder:Default   
        //
        // Example: SetInputBorder(InputBorder::Normal);
        //
        bool SetInputBorder(InputBorder eBorder);

        // GetWindowColors() -- Get the current Window foreground (i.e. Text) and background colors.
        // These are the colors used for all text output and input functions.
        //
        bool GetWindowColors(DWORD & dwFgColor,DWORD & dwBgColor);

        // GetWindowColors() -- Get the current Window foreground (i.e. Text) and background colors.
        // These are the colors used for all text output and input functions.
        //
        bool GetWindowColors(RGBColor_t & rgbFgColor,RGBColor_t & rgbBgColor);

        // Return the Current Font. 
        // This returns a Windows HFONT which can be used in all Font functions
        //
        HFONT GetCurrentFont();

        // Set the Font to the Default Font for the Window.
        //
        HFONT SetDefaultFont();

        // Get the Default Font for the window.
        // This only returns the font but does not set it.
        //
        HFONT GetDefaultFont();

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
        HFONT SetFont(HFONT hFont);    

        // Set the Font for the window.
        // This can take forms such as Text and HFONT, as well as fonts named by previous CreateFont() or SetFont() calls.
        //
        // Bold and italic are created for all fonts. 
        //
        // SetFont(MyFont)                  -- Sets the font to the HFONT MyFont Value
        // SetFont("Arial,20")              -- Sets and Creates (if it does not exist) the font to Arial,20
        // MyFont = SetFont("Arial,20","TestFont")  -- Set and Create the font to Arial,20 and name it TestFont.  Store it in MyFont
        // SetFont("TestFont");             -- Set the Font named "TestFont"
        // SetFont(MyFont)                  -- Sets the font to the HFONT MyFont Value
        //
        HFONT SetFont(const char * sFont,const char * sNewFontName = nullptr,unsigned char * ucStatus = nullptr);    

        // Set the Font for the window.
        // This can take forms such as Text and HFONT, as well as fonts named by previous CreateFont() or SetFont() calls.
        //
        // Bold and italic are created for all fonts. 
        //
        // SetFont(MyFont)                  -- Sets the font to the HFONT MyFont Value
        // SetFont("Arial,20")              -- Sets and Creates (if it does not exist) the font to Arial,20
        // MyFont = SetFont("Arial,20","TestFont")  -- Set and Create the font to Arial,20 and name it TestFont.  Store it in MyFont
        // SetFont("TestFont");             -- Set the Font named "TestFont"
        // SetFont(MyFont)                  -- Sets the font to the HFONT MyFont Value
        //
        HFONT SetFont(WCHAR * wsFont,WCHAR * wsNewFontName = nullptr,unsigned char * ucStatus = nullptr);

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
        HFONT CreateNewFont(const char * sFont,const char * sNewFontName = nullptr,unsigned char * ucStatus = nullptr);    
    
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
        __forceinline HFONT AddFont(const char * sFont,const char * sNewFontName = nullptr,unsigned char * ucStatus = nullptr)
                                                                            { return CreateNewFont(sFont,sNewFontName,ucStatus); }

        // GetFont() -- Get a font created through SageBox.
        //
        // Example, GetFont("MyFont") -- retrieves a font created with CreateFont("Arial,20","TestFont"), or SetFont()
        //
        // The Font returned is an HFONT which can be used with all font functions
        //
        HFONT GetFont(const char * sFont,const char * sNewFontName = nullptr,unsigned char * ucStatus = nullptr);    
        
        // SetBkMode() -- Set the text background mode.
        // BkMode returns eeither BkMode::Transparent or BkMode::Opaque
        //
        // BkMode::Transparent()  --- This is the default.  When text is written to the screen, only the text part is written in the
        //                            foreground color, leaving the blank areas (i.e. text or graphics behind the new text) untouched.
        //
        // BkMode::Opaque         -- When text is written to the window, this will write the text in the foreground color and fill in the background with the
        //                           background color overwriting any text or graphics behind the new text.
        //
        void SetBkMode(BkMode eBkType);

        // GetBkMode() -- Get the text background mode.
        // BkMode returns eeither BkMode::Transparent or BkMode::Opaque
        //
        // BkMode::Transparent()  --- This is the default.  When text is written to the screen, only the text part is written in the
        //                            foreground color, leaving the blank areas (i.e. text or graphics behind the new text) untouched.
        //
        // BkMode::Opaque         -- When text is written to the window, this will write the text in the foreground color and fill in the background with the
        //                           background color overwriting any text or graphics behind the new text.
        //
        BkMode GetBkMode();
        
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
        // When using "console" functions (i.e. MyWidow.console.Write() or MyWindow.console.printf()) word wrap is always on. 
        // When using regular Write(), printf() and other functions, word wrap is only on if it has been set with SetWordWrap()
        //
        bool SetWordWrap(bool bWrap = true);

        // GetWordWrap() -- returns the current word-wrap status for the window (see SetWordWrap());
        //
        bool GetWordWrap();

        // getCharWidth() -- get the average character width for the current window font.
        // For proportional fonts, this returns the average chacter width for the font. 
        // For termainl fonts (i.e. Courier New, etc.) this returns the width for all characters.
        //
        int getCharWidth();

        // getCharHeight() -- Returns the height of the characters for the current window font.  This is 
        // the lineheight for each line printed. 
        //
        int getCharHeight();

        // getCharSize() -- returns the average character width and exact height for the current window font
        // used for all text-based input and output functions.
        //
        SIZE getCharSize();

        // GetWritePos() -- Returns the current X,Y output position for all text-based functions.
        //
        bool GetWritePos(POINT & pLocation);

        // GetWritePos() -- Returns the current X,Y output position for all text-based functions.
        //
        POINT GetWritePos();

        // WinMessageBox() -- Bring up a standard Windows Message Box. 
        //
        // This follows the same format as ::MessageBox() in windows and simply calls it in the same way. 
        //
        // This allows more "Windows-like" messages, information, warnings, etc.
        //
        // Example: WinMessageBox("This is the message","This is the title",MB_OK | MB_ICONINFO)
        //
        int WinMessageBox(const char * sMessage,const char * sTitle,unsigned int dwFlags);
    
        // printf() -- Works the same way as 'C' printf, except you can also put (X,Y) coordinates.
        // printf() can be very useful to quickly print text without using streaming notation,
        //
        // i.e. printf("This is attempt #d\n",++iAttempNo) vs. out << "This is attempt number " << ++iAttemptNo <<  "\n"
        //
        // printf(50,100,"Hello World") will print "Hello World") at (50,100) in the window and set the next write location accordingly.
        //
        void printf(const char * Format,...);

        // printf() -- Works the same way as 'C' printf, except you can also put (X,Y) coordinates.
        // printf() can be very useful to quickly print text without using streaming notation,
        //
        // i.e. printf("This is attempt #d\n",++iAttempNo) vs. out << "This is attempt number " << ++iAttemptNo <<  "\n"
        //
        // printf(50,100,"Hello World") will print "Hello World") at (50,100) in the window and set the next write location accordingly.
        //
        void printf(int iX,int iY,const char * Format,...);

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
        void Write(const char * sText,const char * sOptions = nullptr);            

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
        void Write(const char * sText,cwfOpt & cwOptions);

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
        void Write(int iX,int iY,const char * sText,char * sOptions = nullptr);

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
        void Write(int iX,int iY,const char * sText,cwfOpt & cwOptions);

        // Writeln() -- Same as Write() but adds a '\n' at the end of the line for convenience
        void Writeln(const char * sText = nullptr,const char * sOptions = nullptr);    

        // Writeln() -- Same as Write() but adds a '\n' at the end of the line for convenience
        void Writeln(const char * sText,cwfOpt & cwOptions);         

        // Writeln() -- Same as Write() but adds a '\n' at the end of the line for convenience
        void Writeln(cwfOpt & cwOptions);                        

        // Writeln() -- Same as Write() but adds a '\n' at the end of the line for convenience
        void Writeln(int iX,int iY,const char * sText = nullptr,const char * sOptions = nullptr);   

        // Writeln() -- Same as Write() but adds a '\n' at the end of the line for convenience
        void Writeln(int iX,int iY,const char * sText,cwfOpt & cwOptions);                       

        // Writeln() -- Same as Write() but adds a '\n' at the end of the line for convenience
        void Writeln(int iX,int iY,cwfOpt & cwOptions);                        

        // ClipWindow() -- Clips the window in the given rectangle, restricting output and drawing to that region.
        // This can be used to restrict write areas, Cls(), Drawing, etc. 
        // For example, a graph can be drawn with the clip and will not exceed the clip area.  This can be very useful and 
        // allow such drawing and output in place of creating a child window to perform the same task.
        // Use  ResetClipWindow() or ClipWindow() (with no paramaters) to remove the clipping region
        //
        bool ClipWindow(int iX,int iY,int iWidth,int iHeight,bool bAllowScroll = false);

        // ClipWindow() -- Clips the window in the given rectangle, restricting output and drawing to that region.
        // This can be used to restrict write areas, Cls(), Drawing, etc. 
        // For example, a graph can be drawn with the clip and will not exceed the clip area.  This can be very useful and 
        // allow such drawing and output in place of creating a child window to perform the same task.
        // Use  ResetClipWindow() or ClipWindow() (with no paramaters) to remove the clipping region
        //
        bool ClipWindow(POINT pPoint,SIZE szSize,bool bAllowScroll = false);

        // ClipWindow() -- Clips the window in the given rectangle, restricting output and drawing to that region.
        // This can be used to restrict write areas, Cls(), Drawing, etc. 
        // For example, a graph can be drawn with the clip and will not exceed the clip area.  This can be very useful and 
        // allow such drawing and output in place of creating a child window to perform the same task.
        // Use  ResetClipWindow() or ClipWindow() (with no paramaters) to remove the clipping region
        //
        bool ClipWindow();

        // ResetClipWindow() -- Resets any clipping region active for the current window.
        //
        bool ResetClipWindow();

        // SetAutoScroll() -- Set Automatic Window Scrolling 
        // Note: This function is experimental and may not be working
        //
        bool SetAutoScroll(WindowScroll scroll);
    
        // GetAutoScroll() -- This function is deprecated and not used.
        //
        WindowScroll GetAutoScroll();
    
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
        void Cls(int iColor1=-1,int iColor2 = -1);    

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
        void Cls(RGBColor_t rgbColor);    

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
        void Cls(RGBColor_t rgbColor,RGBColor_t rgbColor2);    

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
        void Cls(const char * sColor1,const char * sColor2=nullptr);    

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
        void Cls(const char * sColor1,RGBColor_t rgbColor);

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
        void Cls(RGBColor_t rgbColor,const char * sColor2);


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
        void Update(int iUpdateMS=0);

        // GetTextSize() -- Get the text size of the text using the current font.
        //
        // This returns the size the text will use in the window.  This can help with the placement of the text or controls around the text.
        // For example, using auto szSize = GetTextSize("This is some Text"), and then Write((szDesktopSize.cx-szSize.cx)/2,100,MyText)) 
        // will center the text in the X plane.
        //
    //    bool GetTextSize(wchar_t * sText,SIZE & szSize); -- deprecated

        // GetTextSize() -- Get the text size of the text using the current font.
        //
        // This returns the size the text will use in the window.  This can help with the placement of the text or controls around the text.
        // For example, using auto szSize = GetTextSize("This is some Text"), and then Write((szDesktopSize.cx-szSize.cx)/2,100,MyText)) 
        // will center the text in the X plane.
        //
        bool GetTextSize(const char * sText,SIZE & szSize);

        // GetTextSize() -- Get the text size of the text using the current font.
        //
        // This returns the size the text will use in the window.  This can help with the placement of the text or controls around the text.
        // For example, using auto szSize = GetTextSize("This is some Text"), and then Write((szDesktopSize.cx-szSize.cx)/2,100,MyText)) 
        // will center the text in the X plane.
        //
        SIZE GetTextSize(const char * sText);

        // SetIndent() -- Sets the indent for text printing functions when a cr/lf "\n" is returned.
        // This allows informally indenting printing.  
        //
        // This only affects where the X position in the console output gets reset to when the "\n" is seen.
        // All other write(), printf(), SetPos(), etc. functions work as normal. 
        // Use SetIndent()  (without arguments to reset it). 
        //
        void SetIndent(int iIndent = 0);
    };

    // console-based functions -- These function work on the main window similarly to C/C++ console functions, 
    // blending with and facilitating text input and output.
    //
    WinConsole console;

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
    CButton & button(const char * sButtonName);

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
    CButton & button(int iButtonID);

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
    CEditBox & editbox(const char * sEditboxName);

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
    CEditBox & editbox(int iEditboxID);
    
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
    CSlider & slider(const char * sSliderName);

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
    CSlider & slider(int iSliderID);
    
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
    CListBox & listbox(const char * sListBoxName);

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
    CListBox & listbox(int iListBoxID);

    CComboBox & combobox(int iComboBoxID);
    CComboBox & combobox(const char * sComboBoxName);
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
    CWindow & window(const char * sWindowName);

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
    CWindow & window(int iWindowID);

    // Returns the default Windows window colors.  SageBox has its own defaults for colors, and this
    // returns the colors Windows sets windows to by default.
    //
    bool GetWindowColors(DWORD & dwFgColor,DWORD & dwBgColor);

    // Returns the default Windows window colors.  SageBox has its own defaults for colors, and this
    // returns the colors Windows sets windows to by default.
    //
    bool GetWindowColors(RGBColor_t & rgbFgColor,RGBColor_t & rgbBgColor);

    // Get the size of the desktop (i.e. monitor). 
    // This returns the current monitor size (i.e. 1920x1080).  This can help in centering and otherwize
    // placing windows and controls.
    //
    // Currently, this returns the active desktop and will be updated to work with systems with multiple monitors in a future update.
    //
    SIZE GetDesktopSize();

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
    bool DisplayBitmap(int iX,int iY,RawBitmap32_t & stBitmap);    
 
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
    bool DisplayBitmap(RawBitmap32_t & stBitmap);    

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
    bool DisplayBitmap(int iX,int iY,int iWidth,int iHeight,unsigned char * sMemory);    

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
    bool DisplayBitmap(int iX,int iY,RawBitmap_t & stBitmap);    
 
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
    bool DisplayBitmap(RawBitmap_t & stBitmap);    

    // Display a bitmap upside-down.  This is often useful for bitmaps, as they are typically stored upside-down, and default
    // Windows behavior is to correct this.
    //
    // However, it is common to work with correct bitmaps that will then display upside down. 
    // DisplayBitmapR() will display a bitmap right-side up.  You can also negate the height or 
    // struture (by putting a '-' in front of it) to use the regular DisplayBitmap()
    //
    // See DisplayBitmap() for more details on this function.
    //
    bool DisplayBitmapR(int iX,int iY,int iWidth,int iHeight,unsigned char * sMemory);    

    // Display a bitmap upside-down.  This is often useful for bitmaps, as they are typically stored upside-down, and default
    // Windows behavior is to correct this.
    //
    // However, it is common to work with correct bitmaps that will then display upside down. 
    // DisplayBitmapR() will display a bitmap right-side up.  You can also negate the height or 
    // struture (by putting a '-' in front of it) to use the regular DisplayBitmap()
    //
    // See DisplayBitmap() for more details on this function.
    //
    bool DisplayBitmapR(int iX,int iY,RawBitmap_t & stBitmap);    
    
    // Display a bitmap upside-down.  This is often useful for bitmaps, as they are typically stored upside-down, and default
    // Windows behavior is to correct this.
    //
    // However, it is common to work with correct bitmaps that will then display upside down. 
    // DisplayBitmapR() will display a bitmap right-side up.  You can also negate the height or 
    // struture (by putting a '-' in front of it) to use the regular DisplayBitmap()
    //
    // See DisplayBitmap() for more details on this function.
    //
    bool DisplayBitmapR(RawBitmap_t & stBitmap);    

    //Display a 32-bit bitmap. 
    //
    // This function is the same as DisplayBitmap(), except that it displays a 32-bit bitmap. 
    //
    // iX,iY must be specified for the location of the displayed bitmap.
    // For raw data, Width,Height, and memory pointer must also be displayed.
    //
    // See DisplayBitmap() for more information
    //
    bool DisplayBitmap32(int iX,int iY,int iWidth,int iHeight,unsigned char * sMemory);    

    //Display a 32-bit bitmap. 
    //
    // This function is the same as DisplayBitmap(), except that it displays a 32-bit bitmap. 
    //
    // iX,iY must be specified for the location of the displayed bitmap.
    // For raw data, Width,Height, and memory pointer must also be displayed.
    //
    // See DisplayBitmap() for more information
    //
    bool DisplayBitmap32(int iX,int iY,RawBitmap32_t & stBitmap);    
 
    //Display a 32-bit bitmap. 
    //
    // This function is the same as DisplayBitmap(), except that it displays a 32-bit bitmap. 
    //
    // iX,iY must be specified for the location of the displayed bitmap.
    // For raw data, Width,Height, and memory pointer must also be displayed.
    //
    // See DisplayBitmap() for more information
    //
    bool DisplayBitmap32(RawBitmap32_t & stBitmap);    
 
    // Blend a bitmap with a pre-defined mask
    //
    bool BlendBitmap(int iX,int iY,RawBitmap_t & stBitmap);

    // Blend a bitmap with a pre-defined mask
    //
    bool BlendBitmap(int iX,int iY,RawBitmap_t & stBitmap,RawBitmap_t & stMask);

    // StretchBitmap() -- Display a stretched bitmap to the window.  
    //
    // The source bitmap can be stretched to any size, and from any portion of the bitmap.
    // 
    // Important Note: If the szSource (source rectangle from which to draw from the original bitmap) is different from 
    // the size of the bitmap, szSourceBitmap MUST BE INCLUDED with the size of the entire source bitmap.  Otherwise the function
    // will fail.  When specifyin a CBitmap or RawBitmap_t as the source, szSourceBitmap does not need to be filled.
    // szSourceBitmap only needs to be filled when sending raw bitmap data and the source size differs from the source bitmap size.
    //
    bool StretchBitmap(unsigned char * sMemory,POINT pDest,SIZE szDest,POINT pSrc, SIZE szSource,SIZE szSourceBitmap = {0,0});
    bool StretchBitmap(CBitmap & cBitmap,POINT pDest,SIZE szDest); 
    bool StretchBitmap(CBitmap & cBitmap,POINT pDest,SIZE szDest,POINT pSrc, SIZE szSource);

    bool StretchBitmapR(unsigned char * sMemory,POINT pDest,SIZE szDest,POINT pSrc, SIZE szSource,SIZE szSourceBitmap = {0,0});
    bool StretchBitmapR(CBitmap & cBitmap,POINT pDest,SIZE szDest); 
    bool StretchBitmapR(CBitmap & cBitmap,POINT pDest,SIZE szDest,POINT pSrc, SIZE szSource);

    bool DisplayBitmapEx(unsigned char * sMemory,POINT pDest,POINT pSrc, SIZE szSize,SIZE szSourceBitmap);
    bool DisplayBitmapEx(CBitmap & cBitmap,POINT pDest,SIZE szSize); 
    bool DisplayBitmapEx(CBitmap & cBitmap,POINT pDest,POINT pSrc, SIZE szSize);

    bool DisplayBitmapExR(unsigned char * sMemory,POINT pDest,POINT pSrc, SIZE szSize,SIZE szSourceBitmap);
    bool DisplayBitmapExR(CBitmap & cBitmap,POINT pDest,SIZE szSize); 
    bool DisplayBitmapExR(CBitmap & cBitmap,POINT pDest,POINT pSrc, SIZE szSize);


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
    HFONT PushFont(const char * sFont);

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
    HFONT PushFont(HFONT hFont = nullptr);

    // PopFont() -- Pop a pushed font and restore the font to the active window font.
    // See PushFont() for more details.
    //
    HFONT PopFont(int iNumPop = 0); 

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
    bool PushColor(DWORD dwFgColor,DWORD dwBgColor = -1);

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
    bool PushColor(RGBColor_t rgbFgColor = { -1,-1, -1},RGBColor_t rgbBgColor = {-1,-1,-1});

    // PushBgColor() -- Push the current Background color for later retrieval.
    // This can also be used to simultaneously set the background color.
    //
    // This function is the same as PushColor() but works on the background color only.
    //
    // See PushColors() for more information.
    //
    bool PushBgColor(DWORD dwFgColor = -1);

    // PushBgColor() -- Push the current Background color for later retrieval.
    // This can also be used to simultaneously set the background color.
    //
    // This function is the same as PushColor() but works on the background color only.
    //
    // See PushColors() for more information.
    //
    bool PushBgColor(RGBColor_t rgbColor = {-1,-1,-1});

    // Pop a color or colors pushed on to the PushColor Stack. 
    // See PushColor() for more informtion.
    //
    bool PopColor(int iNumPop = 1);

    // GetDefaultBgColor() -- return default Background color for windows created by SageBox
    //
    RGBColor_t GetDefaultBgColor();

    // GetDefaultBgColor() -- return default Foreground color for windows created by SageBox
    //
    RGBColor_t GetDefaultFgColor();
 
    // Return the Current Font. 
    // This returns a Windows HFONT which can be used in all Font functions
    //
    HFONT GetCurrentFont();

    // Set the Font to the Default Font for the Window.
    //
    HFONT SetDefaultFont();

    // Get the Default Font for the window.
    // This only returns the font but does not set it.
    //
    HFONT GetDefaultFont();

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
    HFONT SetFont(HFONT hFont);    

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
    HFONT SetFont(const char * sFont,const char * sNewFontName = nullptr,unsigned char * ucStatus = nullptr);    

    // GetFont() -- Get a font created through SageBox.
    //
    // Example, GetFont("MyFont") -- retrieves a font created with CreateFont("Arial,20","TestFont"), or SetFont()
    //
    // The Font returned is an HFONT which can be used with all font functions
    //
    HFONT SetFont(WCHAR * wsFont,WCHAR * wsNewFontName = nullptr,unsigned char * ucStatus = nullptr);

    // GetFont() -- Get a font created through SageBox.
    //
    // Example, GetFont("MyFont") -- retrieves a font created with CreateFont("Arial,20","TestFont"), or SetFont()
    //
    // The Font returned is an HFONT which can be used with all font functions
    //
    HFONT GetFont(const char * sFont,const char * sNewFontName = nullptr,unsigned char * ucStatus = nullptr);    

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
    HFONT CreateNewFont(const char * sFont,const char * sNewFontName = nullptr,unsigned char * ucStatus = nullptr);    

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
    __forceinline HFONT AddFont(const char * sFont,const char * sNewFontName = nullptr,unsigned char * ucStatus = nullptr)
                                                                            { return CreateNewFont(sFont,sNewFontName,ucStatus); }
    // Returns true of the middle mouse button mouse was double clicked.
    // *** This function is still in development and may not work.
    // This is tied to the status of the window an whether or not it will accept double-clicks.
    //
    bool MouseDoubleClicked();

    // Returns true if the Left Mouse Button is currently pressed.  
    // This is not a mouse event and returns the real-time status of the mouse.
    //
    bool MouseButtonDown();

    // Returns true if the Right Mouse Button is currently pressed.  
    // This is not a mouse event and returns the real-time status of the mouse.
    //
    bool MouseRButtonDown();
    bool MouseRButtonClicked(POINT & pPoint,Peek peek = Peek::No);
    bool MouseRButtonClicked(Peek peek = Peek::No);

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
    bool MouseMoved(Peek peek = Peek::No);

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
    bool MouseMoved(POINT & pPoint,Peek peek = Peek::No);


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
    int ButtonPressed(Peek peek = Peek::No);

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
    bool ButtonPressed(int & iButtonID,Peek peek = Peek::No);

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
    bool MouseClicked(Peek peek = Peek::No);

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
    bool MouseClicked(POINT & pMouse,Peek peek = Peek::No);
    
    bool MouseDragEvent(Peek peek = Peek::No);
    bool MouseDragEvent(POINT & pMouse,Peek peek = Peek::No);

    bool MouseDragEnded(Peek peek = Peek::No);
    bool MouseDragEnded(POINT & pMouse,Peek peek = Peek::No);

    bool isMouseDragging(); 
    bool isMouseDragging(POINT & pStartDrag); 

    POINT GetMouseDragStart();

    // GetMousePos() -- Returns the current mouse coordinates relative to the window
    //
    bool GetMousePos(int & iMouseX,int & iMouseY); 

    // GetMousePos() -- Returns the current mouse coordinates relative to the window
    //
    POINT GetMousePos();

    // GetMouseClickPos() -- Returns the last mouse click coordinates.
    //
    // This can be used after a MouseClick() event to return the mouse-click coordinates
    // at the time the mouse was clicked.
    //
    // This works for both left button and right button clicks.
    //
    bool GetMouseClickPos(int & iMouseX,int & iMouseY); 

    // GetMouseClickPos() -- Returns the last mouse click coordinates.
    //
    // This can be used after a MouseClick() event to return the mouse-click coordinates
    // at the time the mouse was clicked.
    //
    // This works for both left button and right button clicks.
    //
    POINT GetMouseClickPos();

    bool MouseWheelMoved(Peek peek = Peek::No);
    bool MouseWheelMoved(int & iDistance,Peek peek = Peek::No);
    int GetMouseWheelMove(bool bResetEvent = true);
    bool WindowResized(SIZE & szNewWinSize,Peek peek = Peek::No);
    bool WindowResized(Peek peek = Peek::No);

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
    ConsoleOp_t SetWritePos(int iX,int iY);

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
    ConsoleOp_t SetWritePos(POINT pLoc);


    // SetWritePosX() -- Set the output X position in the window for writing text. 
    //
    // This sets only the X position in the ouput for printf, Write(), etc. -- anything that prints text.
    // The current Y position is not changed.
    //
    // See SetWritePos() for more information; 
    //
    ConsoleOp_t SetWritePosX(int iX);

    // SetBkMode() -- Set the text background mode.
    // BkMode returns eeither BkMode::Transparent or BkMode::Opaque
    //
    // BkMode::Transparent()  --- This is the default.  When text is written to the screen, only the text part is written in the
    //                            foreground color, leaving the blank areas (i.e. text or graphics behind the new text) untouched.
    //
    // BkMode::Opaque         -- When text is written to the window, this will write the text in the foreground color and fill in the background with the
    //                           background color overwriting any text or graphics behind the new text.
    //
    void SetBkMode(BkMode eBkType);

    // GetBkMode() -- Get the text background mode.
    // BkMode returns eeither BkMode::Transparent or BkMode::Opaque
    //
    // BkMode::Transparent()  --- This is the default.  When text is written to the screen, only the text part is written in the
    //                            foreground color, leaving the blank areas (i.e. text or graphics behind the new text) untouched.
    //
    // BkMode::Opaque         -- When text is written to the window, this will write the text in the foreground color and fill in the background with the
    //                           background color overwriting any text or graphics behind the new text.
    //
    BkMode GetBkMode();

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
    bool SetWordWrap(bool bWrap = true);

    // GetWordWrap() -- returns the current word-wrap status for the window (see SetWordWrap());
    //
    bool GetWordWrap();

    // Sets the Program/Application name.  This can also be set when initiating Sagebox, i.e. CSageBox("My Application"); 
    //
    // The Application name is used in various window title bars in SageBox.  With no Program Name set, either the window title is blank or a default
    // text string is used.
    //
    void SetProgramName(char * sProgramName);

    // Gets the program name set in SageBox through either the SageBox constructor (i.e. CSageBox("My Application") or through GetProgramName();
    //
    const char * GetProgramName();

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
    bool WindowClosing();

    // Reset Window Closing Status.   
    //
    // This resets the Windows Closing status returned by WindowClosing().  Many Sagebox functions will not activate or passthrough when the
    // WindowClosing() status is set to true.
    //
    // This can be used when a user has pressed the close button and CloseButtonPessed() returns true.
    // To cancel the user-based close, call ResetWindowClosing()
    //
    bool ResetWindowClosing();
    bool SetWindowClosing(bool bPressCloseButton = false);
    bool PressCloseButton(); 

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
    bool CloseButtonPressed(Peek peek = Peek::No);

    // PeekCloseButtonPressed() -- Use to determine if the close button was pressed before 
    //
    // This is a shortcut for CLoseButtonPressed(true)
    // See CloseButtonPressed() for more Informatuion
    //
    bool PeekCloseButtonPressed();

    // getCharWidth() -- get the average character width for the current window font.
    // For proportional fonts, this returns the average chacter width for the font. 
    // For termainl fonts (i.e. Courier New, etc.) this returns the width for all characters.
    //
    int getCharWidth();

    // getCharHeight() -- Returns the height of the characters for the current window font.  This is 
    // the lineheight for each line printed. 
    //
    int getCharHeight();

    // getCharSize() -- returns the average character width and exact height for the current window font
    // used for all text-based input and output functions.
    //
    SIZE getCharSize(const char * sFont);

    // getCharSize() -- returns the average character width and exact height for the current window font
    // used for all text-based input and output functions.
    //
    SIZE getCharSize(HFONT hFont = nullptr);

    // GetBgColor() -- Get the current backround color for the window.
    // This color is used for Cls() and all text output routines.
    //
    RGBColor_t GetBgColor();

    // GetFgColor() -- Get the current foreground (i.e. Text) color for the window.
    // This color is used all text output routines.
    //
    RGBColor_t GetFgColor();

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
    HDC GetCurDC();

    // Get the Windows Desktop Device Context of the current window, i.e. the static Device Context.
    //
    // This returns the Windows Device Context (HDC) for the actual Window rather than the bitmap that Sagebox uses
    // for output. 
    // 
    // See GetCurDC() for more information
    //
    HDC GetDesktopDC();

    // Get the Windows Device Context for the internal bitmap Sagebox uses for output functions (i.e. Write(), printf(), drawing, etc.)
    //
    // See GetDesktopDC() to get the Windows Device Context for the actual window rather than the internal bitmap (canvas).
    // 
    // See GetCurDC() for more information
    //
    HDC GetBitmapDC();


    // GetWritePos() -- Returns the current X,Y output position for all text-based functions.
    //
    bool GetWritePos(POINT & pLocation);

    // GetWritePos() -- Returns the current X,Y output position for all text-based functions.
    //
    POINT GetWritePos();

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
    CWindow & BitmapWindow(RawBitmap_t & stBitmap,const  cwfOpt & cwOpt = cwfOpt());

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
    CWindow & BitmapWindow(CBitmap & cBitmap,const  cwfOpt & cwOpt= cwfOpt());

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
    CWindow & BitmapWindow(int iX,int iY,CBitmap & cBitmap,const cwfOpt & cwOpt= cwfOpt());

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
    CWindow & BitmapWindow(int iX,int iY,RawBitmap_t & stBitmap,const  cwfOpt & cwOpt= cwfOpt());

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
    CWindow & ChildWindow(int iX,int iY,int iWidth,int iHeight,const cwfOpt & cwOpt = cwfOpt());
    CWindow & ChildWindow(POINT pLoc,SIZE szSize,const cwfOpt & cwOpt = cwfOpt());

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
    CWindow & ChildWindow(CWindow * cWindow,int iX,int iY,int iWidth,int iHeight,const cwfOpt & cwOpt = cwfOpt());
    CWindow & ChildWindow(CWindow * cWindow,POINT pLoc,SIZE szSize,const cwfOpt & cwOpt = cwfOpt());

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
    CWindow & NewWindow(int iX,int iY,int iWidth,int iHeight,const char * sTitle = nullptr,const cwfOpt & cwOpt = cwfOpt());

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
    CWindow & NewWindow(int iX,int iY,int iWidth,int iHeight,const cwfOpt & cwOpt);

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
    CWindow & NewWindow(int iX,int iY,const char * sTitle = nullptr,const cwfOpt & cwOpt = cwfOpt());

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
    CWindow & NewWindow(int iX,int iY,const cwfOpt & cwOpt);


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
    CWindow & NewWindow(CWindow * cWin,int iX,int iY,int iWidth,int iHeight,const char * sTitle = nullptr,const cwfOpt & cwOpt = cwfOpt());

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
    CWindow & NewWindow(CWindow * cWin,int iX,int iY,int iWidth,int iHeight,const cwfOpt & cwOpt);

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
    CWindow & NewWindow(CWindow * cWin,int iX,int iY,const char * sTitle = nullptr,const cwfOpt & cwOpt = cwfOpt());

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
    CWindow & NewWindow(CWindow * cWin,int iX,int iY,const cwfOpt & cwOpt);


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
    RGBColor_t MakeColor(const char * sColor,DWORD rgbColor);

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
    RGBColor_t MakeColor(const char * sColor,RGBColor_t rgbColor);

    // Get a named color.  This returns an RGBColor_t (or DWORD -- see prototypes) of a named color.
    // 
    // Example:
    //
    // auto rgbBlue = GetColor("Blue");                 -- Get RGBColor_t value for Blue
    // RGBColor_t rgbMyColor = GetColor("MyColor");     -- Get RGBColor_t value for "MyColor" created previously.
    //
    RGBColor_t  GetColor(const char * sColor,bool * pColorFound = nullptr);

    // Get a named color.  This returns an RGBColor_t (or DWORD -- see prototypes) of a named color.
    // 
    // Example:
    //
    // auto rgbBlue = GetColor("Blue");                 -- Get RGBColor_t value for Blue
    // RGBColor_t rgbMyColor = GetColor("MyColor");     -- Get RGBColor_t value for "MyColor" created previously.
    //
    bool GetColor(const char * sColor,DWORD & rgbColor);



    // SetAutoBuffer() -- Sets the screen to buffer only when necessary.
    // *** This is experimental and meant for faster displays.  See SetWindowBuffering();
    //
    void SetAutoBuffer(bool bAutoBuffer);

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
    void SetWindowBuffering(bool bBuffer);


    // Set the Auto Update for the Window.
    //
    // By Default, Auto Update is turned on.  This sets the udpate to every 10 milliseconds or so, so that not every action drawn has to 
    // draw the bitmap on the screen, resulting in much faster performance.
    //
    // WHen AutoUpdate() is off, Update() must be called manually to update the screen, which can be cumberome, determining when and where
    // to sprinkle Update() calls.
    //
    // The drawback to Auto Updating is that, in some cases, the last update may not be current if there is a point where program executioin stops.
    // After most large loops with output, it is a good idea to put a final Update() (without a SleepMS) to ensure the last items output to the
    // screen are updated.
    //
    // EventLoop(), WaitforEvent(), getString, GetInteger, etc.  as well as creating new controls -- most Sagebox functions -- call 
    // Update prior to executing to ensure the screen is up-to-date.
    //
    void SetAutoUpdate(bool bAuto = true);
    void SetAutoUpdate(AutoUpdateType update);
 
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
    CButton & NewButton(int ix,int iy,int iWidth,int iHeight,const char * sButtonText,const cwfOpt & cwOpt = cwfOpt());

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
    CButton & NewButton(int ix,int iy,int iWidth,const char * sButtonText,const cwfOpt & cwOpt = cwfOpt());

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
    CButton & NewButton(int ix,int iy,const char * sButtonText,const cwfOpt & cwOpt = cwfOpt());

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
    CButton & NewButton(CButton * cUserButton,int ix,int iy,int iWidth,int iHeight,const char * sButtonText,const cwfOpt & cwOpt = cwfOpt());

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
    CButton & NewButton(CButton * cUserButton,int ix,int iy,int iWidth,const char * sButtonText,const cwfOpt & cwOpt = cwfOpt());

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
    CButton & NewButton(CButton * cUserButton,int ix,int iy,const char * sButtonText,const cwfOpt & cwOpt = cwfOpt());

    // Wchar_t

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
    CButton & NewButton(int ix,int iy,int iWidth,int iHeight,const wchar_t * sButtonText,const cwfOpt & cwOpt = cwfOpt());

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
    CButton & NewButton(int ix,int iy,int iWidth,const wchar_t * sButtonText,const cwfOpt & cwOpt = cwfOpt());

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
    CButton & NewButton(int ix,int iy,const wchar_t * sButtonText,const cwfOpt & cwOpt = cwfOpt());

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
    CButton & NewButton(CButton * cUserButton,int ix,int iy,int iWidth,int iHeight,const wchar_t * sButtonText,const cwfOpt & cwOpt = cwfOpt());

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
    CButton & NewButton(CButton * cUserButton,int ix,int iy,int iWidth,const wchar_t * sButtonText,const cwfOpt & cwOpt = cwfOpt());

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
    CButton & NewButton(CButton * cUserButton,int ix,int iy,const wchar_t * sButtonText,const cwfOpt & cwOpt = cwfOpt());

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
    CButton & NewCheckbox(int ix,int iy,int iWidth,int iHeight,const char * sName,const cwfOpt & cwOpt = cwfOpt());

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
    CButton & NewCheckbox(int ix,int iy,int iWidth,const char * sName,const cwfOpt & cwOpt = cwfOpt());

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
    CButton & NewCheckbox(int ix,int iy,const char * sName,const cwfOpt & cwOpt = cwfOpt());

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
    CButton & NewCheckbox(CButton * cUserButton,int ix,int iy,int iWidth,int iHeight,const char * sName,const cwfOpt & cwOpt = cwfOpt());

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
    CButton & NewCheckbox(CButton * cUserButton,int ix,int iy,int iWidth,const char * sName,const cwfOpt & cwOpt = cwfOpt());

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
    CButton & NewCheckbox(CButton * cUserButton,int ix,int iy,const char * sName,const cwfOpt & cwOpt = cwfOpt());

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
    CButton & NewRadioButton(int ix,int iy,int iWidth,int iHeight,const char * sName,const cwfOpt & cwOpt = cwfOpt());

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
    CButton & NewRadioButton(int ix,int iy,int iWidth,const char * sName,const cwfOpt & cwOpt = cwfOpt());

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
    CButton & NewRadioButton(int ix,int iy,const char * sName,const cwfOpt & cwOpt = cwfOpt());

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
    CButton & NewRadioButton(CButton * cUserButton,int ix,int iy,int iWidth,int iHeight,const char * sName,const cwfOpt & cwOpt = cwfOpt());

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
    CButton & NewRadioButton(CButton * cUserButton,int ix,int iy,int iWidth,const char * sName,const cwfOpt & cwOpt = cwfOpt());

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
    CButton & NewRadioButton(CButton * cUserButton,int ix,int iy,const char * sName,const cwfOpt & cwOpt = cwfOpt()) ;

    // Sets the location if the window on the desktop
    //
    bool SetWinLocation(POINT pLocation);

    // Sets the location if the window on the desktop
    //
    bool SetWinLocation(int iX,int iY);

    // Gets the location of the window on the desktop
    //
    POINT GetWinLocation();

    // Gets the HWND handle to the window that Windows uses and originally assigned to the window.
    // This can be used to perform windows functions directly.
    //
    // Also see GetCurDC()
    //
    HWND GetWindowHandle();        

    // ** This is experimental **
    // Clears all event flags before entering WaitforEventLoop()
    // Again, experimental and may be deprecated.
    //
    bool ClearFlags();

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
    SIZE GetWindowSize(bool bFrameSize = false);

    // Returns the ID assigned to the Window when ID() was used to create the window.
    //
    int GetID();

    // Returns the Name assigned to the window when Name() was used to create the window
    //
    const char * GetName();

    // Sets a hover message to be shown when the mouse hovers over the window
    //
    bool SetHoverMsg(const char * sHoverMessage);

    // The the X coordinare of the left part of the window
    //
    int    StartX();

    // The the Y coordinae of the upper part of the window
    //
    int    StartY();
   
    // The the X coordinare of the right part of the last window
    //
    int    EndX();

    // The the Y coordinare of the bottom part of the window
    //
    int    EndY();

    // GetWindowWidth() - returns the width of the displayed canvas of the window.
    // Use GetWindoWidth(true) to get the full width of the widow, including the frame.
    // See GetWindowSize() to return both Width and Height in SIZE structure.
    //
    int GetWindowWidth(bool bFrameSize = false);

    // GetWindowHeight() - returns the height of the displayed canvas of the window.
    // Use GetWindowHeight(true) to get the full width of the height, including the frame.
    // See GetWindowSize() to return both Width and Height in SIZE structure.
    //
    int GetWindowHeight(bool bFrameSize = false);

    // Returns true if the Window is showing on the desktop.  False is returned if the Window is invisible.
    //
    bool isVisible();

    // Returns true of the window object is a valid window.
    // False is returned if not.  This can happen when a window is retrieved that is not valid, or a 
    // Window has been closed. 
    //
    // For example using auto& MyWindow = window("MyWindow") will return an empty window if a window
    // with the Name("MyWindow") does not exist.  In which case, isValid() can be used to determine this is an empty window.
    //
    bool isValid();


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
    CBitmap GetWindowBitmap(POINT pLoc,SIZE szSize,bool bDesktopView = false);
    
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
    CBitmap GetWindowBitmap(bool bDesktopView = false);

    // SendWidgetMessage()
    // 
    // This is used by Widgets to send a generic message that can be intercepted with OnWidgetMessage() and also
    // causes EventLoop() and WaitforEvent() to return so the main procedure can look for WidgetMessages.
    //
    // This is also useful for Widgets to send messages to themselves to process elements in the main
    // thread rather than the calling thread of an interface function with unknown status (i.e. updates, etc.)
    // (this helps avoid multi-threading issues)
    //
    bool SendWidgetMessage(CWindow * cWin,void * cWidget = nullptr,int iMessage = 0);

    // SendWidgetMessage()
    // 
    // This is used by Widgets to send a generic message that can be intercepted with OnWidgetMessage() and also
    // causes EventLoop() and WaitforEvent() to return so the main procedure can look for WidgetMessages.
    //
    // This is also useful for Widgets to send messages to themselves to process elements in the main
    // thread rather than the calling thread of an interface function with unknown status (i.e. updates, etc.)
    // (this helps avoid multi-threading issues)
    //
    bool SendWidgetMessage(HWND hWndParent,void * cWidget = nullptr,int iMessage = 0);

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
    WaitEvent WaitforEvent(const char * sEvent = nullptr);

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
    WaitEvent WaitforEvent(cwfEvent & cwEvent) { return WaitforEvent(*cwEvent); }
 
    
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
    bool EventLoop(WaitEvent * eStatus = nullptr); 
 
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
    bool GetEvent(WaitEvent * eStatus = nullptr);

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
    int WaitforClose(int iReturnValue = 0);
    
    // WaitforMouseClick() -- Waits for a mouse click in the current window. 
    // This function will return on receipt of a mouseclick or the window close, unless the automatic Window Close is disabled
    // (see DisableClose())
    //
    bool WaitforMouseClick();    // false return means window is closing down

    // ExitButton() -- Places a "Program Finished. Press Button to Continue" on the bottom of the screen and waits for input before
    // continuing.  This is useful when the program ends, to allow the user to press the button before the window closes.
    //
    // Exit Button return 0
    //
    int ExitButton(const char * sText = nullptr);
 
    // NoExitMsg() -- Used for Sandbox applications to tell the calling process to not display the exit button w/message on the bottom of the window.
    //
    // NoExitMsg() returns 0;
    //
    int NoExitMsg(bool bNoExit = true); 

    // NoExitMsg() -- Used for Sandbox applications to tell the calling process to not display the exit button w/message on the bottom of the window.
    //
    // NoExitMsg() returns 0 unless a return code is specified.
    //
    int NoExitMsg(int iReturnCode,bool bNoExit=true); 

    // EnableWindow() -- Enables or Disables the Window and all controls within the window
    //
    // Disabling the window an be useful to make sure no controls are used within the window, such as when dialog boxes are overlaid on top of the
    // window, or when the contex of the window currently makes no sense.
    //
    // EnableWindow() -- Enable the window
    // EnableWindow(false) -- Disable the window
    //
    bool EnableWindow(bool bEnable = true);

    // DisableWindow() -- Enables or Disables the Window and all controls within the window
    //
    // Disabling the window an be useful to make sure no controls are used within the window, such as when dialog boxes are overlaid on top of the
    // window, or when the contex of the window currently makes no sense.
    //
    // DisableWindow() -- Disable the window
    // DisableWindow(false) -- Enable the window
    //
    bool DisableWindow(bool bDisable = true);

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
    CBitmap CreateBitmap(int iWidth,int iHeight = 1);

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
    CBitmap CreateBitmap(SIZE szBitmapSize);
 
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
    [[nodiscard]] RawBitmap32_t GetBitmapStruct32(int iWidth,int iHeight = 1);

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
    CEditBox & NewEditBox(int iX,int iY,int iWidth,int iHeight,cwfOpt & cwOpt);

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
    CEditBox & NewEditBox(CEditBox * pObject,int iX,int iY,int iWidth,int iHeight,cwfOpt & cwOpt);

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
    CEditBox & NewEditBox(int iX,int iY,int iWidth,int iHeight,const char * sText = nullptr,const cwfOpt & cwOpt = cwfOpt());

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
    CEditBox & NewEditBox(CEditBox * pObject,int iX,int iY,int iWidth,int iHeight,const char * sText = nullptr,const cwfOpt & cwOpt = cwfOpt());

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
    bool CreateButtonStyle(const char * sStyleName,GraphicButtonStyle & stStyle);

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
    bool CreateCheckboxStyle(const char * sStyleName,GraphicCheckboxStyle & stStyle);

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
    bool CaptureMouse(bool bReleaseOnMouseUp = false); 

    // ReleaseCapture() -- this releases a mouse capture in the current window or control
    // This can be used to release the capture, such as when the mouse button is released. 
    //
    // note: The capture can be released independently.  See isMouseCaptured() for more details
    //
    bool ReleaseCapture();

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
    bool isMouseCaptured();

    // Returns TRUE when the mouse capture has been release.  This only returns true when
    // the mouse capture previously engaged for the window has been released.
    //
    // This returns an event status.  Therefore, the event is reset after the call and will return false 
    // afterward until a subsequent release -- unless peek is set to Peek::No, in which case the status is not reset
    //
    bool CaptureReleased(Peek peek = Peek::No);

    // Delete the window.  This closes the window permanently.  All data associated with the window
    // is closed when the parent window is deleted. 
    //
    // If the window is subclassed with an object, this means the memory allocated by this object will not
    // be deleted right away.  Delete() can be overridden to do this, or deletObj() can be used to 
    // physically delete the Window (use carefully)
    //
    bool Delete();

    // LockProcess() -- Lock the window process for multi-threading capability. (i.e. set Busy status)
    //
    // This uses an Atomic machine code function to lock the process and make it Busy so that multi-threading may
    // occur safely.  LockProcess() uses a single lock (use LockProcess(iMyLock) for more locks) for the window and 
    // does not return until the current lock is Unlocked
    //
    bool LockProcess();

    // UnlockProcess() -- Unlocks the window process for multi-threading capability. (i.e. release Busy status)
    //
    // See LockProcess() for more details
    //
    bool UnlockProcess();


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
    bool LockProcess(int & iLock);

    // UnlockProces(int & iLock) -- Unlock a specific lock to lock the process for multi-threading (i.e. release Busy status for the specific lock)
    //
    bool UnlockProcess(int & iLock);

    // SetDebugID() -- set a specific ID for the window. 
    //
    // Windows can be hard to debug, especially when there are a number of controls and other window. 
    // When looking at messages and other actions, it can be hard to determine which window or control is 
    // responsible.
    //
    // SetDebugID() can be used to set a specific Debug ID for the window so that it can be
    // trapped in debugging.
    //
    void SetDebugID(int iID);

    // GetDebugID() -- returns the debug ID set for the window.
    //
    // Set SetDebugID() for more information.
    //
    int GetDebugID();
    virtual ~CWindow();        // Destructor.  Closes the window, deleting all child controls and windows (but not their user-created objects).


    // Main function for subclassed windows.
    // This can be called by user-code, and is call automatically by the CSageBox::Main() function as the 
    // point of entry for a Window class, allowing use of windows functions as regular C code (rather than using the object name)
    //
    // For example, with Main() inside of the window, functions like:
    //
    // out<< "Hello World"
    // Write("Hello World\n");
    // DrawRectangle()
    //
    // can be used instead of:
    //
    // MyWindow.out<< "Hello World"
    // MyWindow.Write("Hello World\n");
    // MyWindow.DrawRectangle()
    //
    virtual void Main() { };

    // This can be called by user code to initial a window sublass.
    // for example:
    //
    // ---> auto& cNewWindow = NewWindow(new CMyWindow,100,200); 
    // ---> cNewWindow.InitialUpdate(this);    
    //
    // to initialize the window class with the current 'this' pointer.
    //
    virtual void InitialUpdate(void * vpData = nullptr) { };


    /* TextWidget() -- Creates a persistent Text Widget Windows to write Text
    
    A Text Widget is a widow that that is placed on the current window.   This protects the text, and the
    Text Widget can then be moved, re-written, and update. 

    A Text Widget does not need to be update or managed.  When you write the Text Widget, any centering, colors, and fonts are remembered.
    This can be useful when writing multiple text objects to the window.

    An alternative to using a Text Widget is the stanard "out" streaming function, Write() and printf() that can write directly to the window.
    
    Input:

    iX,iY          - The X and Y position to place the Widget in the window.  If you use any Just or Center settings, these numbers turn into the offset
                    from the position calculated.  Note: This is individual for each item. For example, using iX = -20 and JustRight() will put the
                    Text Widget ending 20 pixels from the window.  This is it does not need to be on the edge.
                    Using iY = 0 and CenterY(-50) will bring the window up a little from the center of the window, which can look nicer.

    Width,Height  - The Width and Height of the Text Widget.  These can be omitted entirely, or set to 0, in which case the Width and Height are calculated for the selected font
                    Using a negative number adds the positive value to the calculated size for that dimension (i.e. Width or Height).
                
                    For example, using -20 for the Width value will add 20 pixels to the left and right value of the size calculate for the text.

                    note:  This only appens during initialization.   Width and Height do not change once the Text Widget is created, except specifically through an access function TBD.

    sMessage      - The initial text for the TextWidget.  This may be omitted, and set later with the Write() function.

    cwOpt           - These are the options (discussed below).  For example, Font(), fgColor(), etc.  See below.

    --> Functions in Text Widget.  Use the autocomplete and hover over the function to get a description.
    --> Options (set in cwOpt)
    -->
    --> Hidden()     - Initially Hide the TextWidget.  This can be useful for setting up the widget before showing it, or giving a max-sized initial text that is not shown.
    --> Show()       - Show the Widget if it is hidden
    --> AllowDrag()  - Allow the user to move the window with the mouse
    --> FgColor()     - Set the forerground color
    --> BgColor()     - Set the backround color
    --> Font()         - Set the font for the Text Widget

    --> Window Placement: Standard window placement options (see auto-complete in opt:: settings) -- Center(),CenterX(),CenterY(),JustLeft(),JustRight(),JustTop(),JustBottom(), etc.
    --> Text Placement: Standard window placement options (see auto-complete in opt:: settings) -- TextCenter(),TextCenterX(),TextCenterY(),TextLeft(),TextRight(),TextTop(),TextBottom(), etc.
    -->
    --> note: default for text placement is Centered in the Y dimension, and Left in the X dimension.
    */
    CTextWidget & TextWidget(int iX,int iY,int iWidth,int iHeight,const char * sMessage = nullptr,const cwfOpt & cwOpt = cwfOpt());
    /* TextWidget() -- Creates a persistent Text Widget Windows to write Text
    
    A Text Widget is a widow that that is placed on the current window.   This protects the text, and the
    Text Widget can then be moved, re-written, and update. 

    A Text Widget does not need to be update or managed.  When you write the Text Widget, any centering, colors, and fonts are remembered.
    This can be useful when writing multiple text objects to the window.

    An alternative to using a Text Widget is the stanard "out" streaming function, Write() and printf() that can write directly to the window.
    
    Input:

    iX,iY          - The X and Y position to place the Widget in the window.  If you use any Just or Center settings, these numbers turn into the offset
                    from the position calculated.  Note: This is individual for each item. For example, using iX = -20 and JustRight() will put the
                    Text Widget ending 20 pixels from the window.  This is it does not need to be on the edge.
                    Using iY = 0 and CenterY(-50) will bring the window up a little from the center of the window, which can look nicer.

    Width,Height  - The Width and Height of the Text Widget.  These can be omitted entirely, or set to 0, in which case the Width and Height are calculated for the selected font
                    Using a negative number adds the positive value to the calculated size for that dimension (i.e. Width or Height).
                
                    For example, using -20 for the Width value will add 20 pixels to the left and right value of the size calculate for the text.

                    note:  This only appens during initialization.   Width and Height do not change once the Text Widget is created, except specifically through an access function TBD.

    sMessage      - The initial text for the TextWidget.  This may be omitted, and set later with the Write() function.

    cwOpt           - These are the options (discussed below).  For example, Font(), fgColor(), etc.  See below.

    --> Functions in Text Widget.  Use the autocomplete and hover over the function to get a description.
    --> Options (set in cwOpt)
    -->
    --> Hidden()     - Initially Hide the TextWidget.  This can be useful for setting up the widget before showing it, or giving a max-sized initial text that is not shown.
    --> Show()       - Show the Widget if it is hidden
    --> AllowDrag()  - Allow the user to move the window with the mouse
    --> FgColor()     - Set the forerground color
    --> BgColor()     - Set the backround color
    --> Font()         - Set the font for the Text Widget

    --> Window Placement: Standard window placement options (see auto-complete in opt:: settings) -- Center(),CenterX(),CenterY(),JustLeft(),JustRight(),JustTop(),JustBottom(), etc.
    --> Text Placement: Standard window placement options (see auto-complete in opt:: settings) -- TextCenter(),TextCenterX(),TextCenterY(),TextLeft(),TextRight(),TextTop(),TextBottom(), etc.
    -->
    --> note: default for text placement is Centered in the Y dimension, and Left in the X dimension.
    */    CTextWidget & TextWidget(int iX,int iY,int iWidth,int iHeight,cwfOpt & cwOpt);
    /* TextWidget() -- Creates a persistent Text Widget Windows to write Text
    
    A Text Widget is a widow that that is placed on the current window.   This protects the text, and the
    Text Widget can then be moved, re-written, and update. 

    A Text Widget does not need to be update or managed.  When you write the Text Widget, any centering, colors, and fonts are remembered.
    This can be useful when writing multiple text objects to the window.

    An alternative to using a Text Widget is the stanard "out" streaming function, Write() and printf() that can write directly to the window.
    
    Input:

    iX,iY          - The X and Y position to place the Widget in the window.  If you use any Just or Center settings, these numbers turn into the offset
                    from the position calculated.  Note: This is individual for each item. For example, using iX = -20 and JustRight() will put the
                    Text Widget ending 20 pixels from the window.  This is it does not need to be on the edge.
                    Using iY = 0 and CenterY(-50) will bring the window up a little from the center of the window, which can look nicer.

    Width,Height  - The Width and Height of the Text Widget.  These can be omitted entirely, or set to 0, in which case the Width and Height are calculated for the selected font
                    Using a negative number adds the positive value to the calculated size for that dimension (i.e. Width or Height).
                
                    For example, using -20 for the Width value will add 20 pixels to the left and right value of the size calculate for the text.

                    note:  This only appens during initialization.   Width and Height do not change once the Text Widget is created, except specifically through an access function TBD.

    sMessage      - The initial text for the TextWidget.  This may be omitted, and set later with the Write() function.

    cwOpt           - These are the options (discussed below).  For example, Font(), fgColor(), etc.  See below.

    --> Functions in Text Widget.  Use the autocomplete and hover over the function to get a description.
    --> Options (set in cwOpt)
    -->
    --> Hidden()     - Initially Hide the TextWidget.  This can be useful for setting up the widget before showing it, or giving a max-sized initial text that is not shown.
    --> Show()       - Show the Widget if it is hidden
    --> AllowDrag()  - Allow the user to move the window with the mouse
    --> FgColor()     - Set the forerground color
    --> BgColor()     - Set the backround color
    --> Font()         - Set the font for the Text Widget

    --> Window Placement: Standard window placement options (see auto-complete in opt:: settings) -- Center(),CenterX(),CenterY(),JustLeft(),JustRight(),JustTop(),JustBottom(), etc.
    --> Text Placement: Standard window placement options (see auto-complete in opt:: settings) -- TextCenter(),TextCenterX(),TextCenterY(),TextLeft(),TextRight(),TextTop(),TextBottom(), etc.
    -->
    --> note: default for text placement is Centered in the Y dimension, and Left in the X dimension.
    */
    CTextWidget & TextWidget(int iX,int iY,const char * sMessage = nullptr,const cwfOpt & cwOpt = cwfOpt());

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
    bool SnaptoWin(CWindow * cWin,int iPadX = 0,int iPadY = 0,Snap eAction = Snap::Snap,CWidget * cWidget = nullptr);
    bool SnaptoDesktop(int iPadX = 0,int iPadY = 0,Snap eAction = Snap::Snap,CWidget * cWidget = nullptr);

    // NewDialog() -- Create a new dialog window. 
    // This returns with a CDialog class that allows you to build a dialog window.
    // A dialog window is the same as a normal window, and you can retrieve the window
    // object with auto& cWin = MyDialog.GetWindow();
    //
    // With the dialog class (i.e. auto& MyDialog = NewDialog()), you can add controls, windows, and widgets, all of which are tracked by the CDialog.
    // This allows flexibility in building dialog windows.
    //
    // Dialogs can act as dialogs with a single purpose, or as regular windows with the dialog managing the controls
    // Use the NoClose() option to use the dialog as a regular window.
    //
    // Dialogs can be used as regular popupwindows or placed within an existing window.
    //
    CDialog & NewDialog(int iX,int iY,int Width = 0,int iHeight = 0,const char * sTitle = nullptr,const cwfOpt & cwOpt = cwfOpt());

    // NewDialog() -- Create a new dialog window. 
    // This returns with a CDialog class that allows you to build a dialog window.
    // A dialog window is the same as a normal window, and you can retrieve the window
    // object with auto& cWin = MyDialog.GetWindow();
    //
    // With the dialog class (i.e. auto& MyDialog = NewDialog()), you can add controls, windows, and widgets, all of which are tracked by the CDialog.
    // This allows flexibility in building dialog windows.
    //
    // Dialogs can act as dialogs with a single purpose, or as regular windows with the dialog managing the controls
    // Use the NoClose() option to use the dialog as a regular window.
    //
    // Dialogs can be used as regular popupwindows or placed within an existing window.
    //
    CDialog & NewDialog(int iX,int iY,int iWidth,int iHeight,const cwfOpt & cwOpt);

    // NewDialog() -- Create a new dialog window. 
    // This returns with a CDialog class that allows you to build a dialog window.
    // A dialog window is the same as a normal window, and you can retrieve the window
    // object with auto& cWin = MyDialog.GetWindow();
    //
    // With the dialog class (i.e. auto& MyDialog = NewDialog()), you can add controls, windows, and widgets, all of which are tracked by the CDialog.
    // This allows flexibility in building dialog windows.
    //
    // Dialogs can act as dialogs with a single purpose, or as regular windows with the dialog managing the controls
    // Use the NoClose() option to use the dialog as a regular window.
    //
    // Dialogs can be used as regular popupwindows or placed within an existing window.
    //
    CDialog & NewDialog(int iX,int iY,const char * sTitle,const cwfOpt & cwOpt = cwfOpt());

    // NewDialog() -- Create a new dialog window. 
    // This returns with a CDialog class that allows you to build a dialog window.
    // A dialog window is the same as a normal window, and you can retrieve the window
    // object with auto& cWin = MyDialog.GetWindow();
    //
    // With the dialog class (i.e. auto& MyDialog = NewDialog()), you can add controls, windows, and widgets, all of which are tracked by the CDialog.
    // This allows flexibility in building dialog windows.
    //
    // Dialogs can act as dialogs with a single purpose, or as regular windows with the dialog managing the controls
    // Use the NoClose() option to use the dialog as a regular window.
    //
    // Dialogs can be used as regular popupwindows or placed within an existing window.
    //
    CDialog & NewDialog(int iX,int iY,const cwfOpt & cwOpt = cwfOpt());

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
    CString GetOpenFile(stOpenFileStruct & stFile);
 
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
    CString GetOpenFile(const char * sTypes = nullptr);
    
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
    bool GetOpenFile(stOpenFileStruct & stFile,CString & csFilename);

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
    bool GetOpenFile(const char * sTypes,CString & csFilename);

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
    bool GetOpenFile(CString & csFilename);


    CString GetSaveFile(stOpenFileStruct & stFile);
 
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
    CString GetSaveFile(const char * sTypes = nullptr);
    
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
    bool GetSaveFile(stOpenFileStruct & stFile,CString & csFilename);

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
    bool GetSaveFile(const char * sTypes,CString & csFilename);

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
    bool GetSaveFile(CString & csFilename);


    // GetOpenFileStruct() -- Return an stOpenFileStruct to set for using an GetOpenFile() or GetSaveFile() dialog.
    //
    // This is a shortcut for declaring a structure directly, so that auto& stStruct = GetOpenFileStruct() can be used
    // more easily.
    //
    stOpenFileStruct GetOpenFileStruct();

    // WriteShadow() -- Works like Write() (but is more limited), but writes the text with a shadow behind it.
    //
    // See Write() for more information
    //
    void WriteShadow(int iX,int iY,const char * sMessage,const cwfOpt & cwOpt = cwfOpt());


    // CreateMenu() -- Creates a menu that can be added as a main menu or sub menu to the window.
    //
    // Once a menu is created, items can be added through the CMenu class object returned.
    //
    // See CMenu documentation for more information.
    //
    [[nodiscard]] CMenu CreateMenu(int iBaseMenuValue = 0);

    // Find a Windows menu.  If the HMENU (Windows Menu) value is a valid windows menu, a CMenu object is returned for this menu.
    //
    CMenu GetMenu(HMENU hMenu,bool * bFound = nullptr);

    // Find a Windows menu.  If the HMENU (Windows Menu) value is a valid windows menu, a CMenu object is returned for this menu.
    //
    bool GetMenu(HMENU hMenu,CMenu & cMenu);

    // MenuItemSelected() -- This works as an event, and will fill the menu item if it is selected.
    // As an event, subsquent calls will return false and not fill the menu item until another menu item is selected.
    //
    // If the iMenuItem is not included in the call, GetMenuItem() can be used to retrieve the last menu item selected
    //
    bool MenuItemSelected(int & iMenuItem,Peek peek = Peek::No);

    // MenuItemSelected() -- This works as an event, and will fill the menu item if it is selected.
    // As an event, subsquent calls will return false and not fill the menu item until another menu item is selected.
    //
    // If the iMenuItem is not included in the call, GetMenuItem() can be used to retrieve the last menu item selected
    //
    bool MenuItemSelected(Peek peek = Peek::No);

    // Retrieves the last menu item selected.
    // 
    // This is meant to be used right after MenuItemSelected() is called, and will return the last menu item selected.
    //
    // This will continue to return the same menu item until a new menu item is selected.
    //
    int GetMenuItem();

    bool SetCloseButtonMenu(int iMenuItem = 0);


    CBitmap ReadImageFile(const char * sPath,bool * bSuccess = nullptr);
    ImageStatus GetLastImageStatus();

    // ReadJpegFile -- Read a jpeg file and store it into a CBitmap. 
    // This reads standard 8-bit jpeg (3-channel or monochrome).
    // If the file does not exist or is in an unsupported format, CBitmap will come back empty.
    // Use GetJpegError() to get the status of the last ReadJpegFile() call, which will
    // give information on why the Bitmap came back empty.
    //
    // if (bSuccess) is supplied, it is filled with true for a successful read, otherwise false.
    //
    CBitmap ReadJpegFile(const char * sPath,bool * bSuccess = nullptr);

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
    CBitmap ReadJpegMem(const unsigned char * sData,int iDataLength,bool * bSuccess);

    // Returns the last Jpeg status.  This will return CJpeg::Status::Ok if there was no error, 
    // or an indication of what went wrong with the last call, such as CJpeg::Status::EmptyFilePath or CJpeg::Status::FileNotFound
    //
    CJpeg::Status GetJpegError();

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
    bool QuickThumbnail(RawBitmap_t & stBitmap,int iWidth,int iHeight,ThumbType eType = ThumbType::BestFit,const char * sTitle = nullptr);
 
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
    bool QuickThumbnail(RawBitmap_t & stBitmap,int iWidth,int iHeight,const char * sTitle);

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
    bool QuickThumbnail(CBitmap & cBitmap,int iWidth,int iHeight,ThumbType eType = ThumbType::BestFit,const char * sTitle = nullptr);

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
    bool QuickThumbnail(CBitmap & cBitmap,int iWidth,int iHeight,const char * sTitle);

    CBitmap QuickResize(RawBitmap_t & stBitmap,int iWidth,int iHeight,ResizeType eType = ResizeType::BestFit);
    CBitmap QuickResize(RawBitmap_t & stBitmap,SIZE szSize,ResizeType eType = ResizeType::BestFit);


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
    CListBox & NewListBox(int ix,int iy,int iWidth,int iHeight,const cwfOpt & cwOpt = cwfOpt());
 
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
    CListBox & NewListBox(CListBox * cListBox,int ix,int iy,int iWidth,int iHeight,const cwfOpt & cwOpt = cwfOpt());


    CComboBox &  NewComboBox(int iX,int iY,int iWidth,int iHeight,const cwfOpt & cwOpt = cwfOpt()); 
    CComboBox &  NewComboBox(CComboBox * cComboBox,int iX,int iY,int iWidth,int iHeight,const cwfOpt & cwOpt = cwfOpt()); 

    bool SetEventWindow(CWindow * cWin = nullptr);
    bool SetEventWindow(CWindow & cWin);

    // SendEventsToParent() -- Send all events to Parent Window while also sending them to the Window itself.
    //
    // This function is mucnh like SetEventWindow() which will tell Sagebox where else to send all events through GetEvent().
    // Child windows send events to their parents (as well as themselves) by default, meaning you can use GetEvent() for the parent window
    // and receive events for the window itself.
    //
    // For windows created with Sagebox (and some other instances), there is no Event Window set, and to receive events through the
    // CSageBox::GetEvent() loop, SendEventsToParent() must be called -- this will send all events to the Sagebox main window (which is hidden)
    //
    bool SendEventsToParent(); 

    bool AddControlLabel(SizeRect srSize,const char * sText,LabelJust justType,bool bUpdate,const cwfOpt & cwOpt = cwfOpt());
   
    bool DrawLabelBox(POINT pLocation,SIZE szSize,const char * sTitle,bool bUpdate = true,BorderType eBorderType = BorderType::Depressed, LabelJust eLabelType = LabelJust::None,const cwfOpt & cwOpt = cwfOpt());
    bool DrawLabelBox(POINT pLocation,SIZE szSize, const char * sTitle,const cwfOpt & cwOpt);
    bool DrawLabelBox(POINT pLocation,SIZE szSize,const char * sTitle,bool bUpdate,const cwfOpt & cwOpt);
    bool HideMenu();
    bool ShowMenu(CMenu & cMenu);

    LONG SetFullScreen(bool bFullscreen = true,LONG dwPrevStyle = 0,bool bForceShow = false);

    CDavinci * GetDavinciMain();

    SIZE CreateRadioButtonGroup(int iNumButtons,int iX,int iY,const char * * sButtonNames,int iGroupID, const char * sLabel, const cwfOpt & cwOpt = cwfOpt());
    SIZE CreateCheckboxGroup(int iNumButtons,int iX,int iY,const char * * sButtonNames,int iGroupID, const char * sLabel, const cwfOpt & cwOpt = cwfOpt());

	bool EnablePainting();
	bool DisablePainting();
	bool DisablePaintingSafe();
	bool DontUpdate(bool bDontUpdate = true);
    bool SetasTopWindow();
    bool SetasTopmostWindow();
    bool DrawSimpleDoc(const unsigned char * sPgrData,const cwfOpt & cwOpt = cwfOpt()); 
    bool DrawSimpleDoc(const char * sPgrPath, const cwfOpt & cwOpt = cwfOpt());

    // DevGetWindow() -- Returns the Window (i.e. CWindow) of the default DevWindow.  This can be used to access window functions for the 
    // the default DevWindow. 
    //
    // For user-created Dev Control Windows, use the CDevControls::GetWindow() function, i.e. cMyDevWindow->GetWindow(); 
    //
    CWindow * DevGetWindow();

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
    CWindow::WinGroup * DevGetGroup();

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
    CDevControls * DevControlsWindow(int iX = -1, int iY = -1, const cwfOpt & cwOpt = cwfOpt());

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
    CDevControls * DevControlsWindow(const cwfOpt & cwOpt);

	// DevButton() -- Add a button to the Default Dev Control Window.  This accepts all options as normal buttons, but 
	// the default will add a regular button. 
	//
	// The Name used as a title for the button, but is optional. 
	//
    CButton & DevButton(const char * sButtonName = nullptr,const cwfOpt & cwOpt = cwfOpt());

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
    CButton & DevCheckbox(const char * sCheckboxName = nullptr,const cwfOpt & cwOpt = cwfOpt());

    // QuickSlider() -- Add a slider to the Default Dev Controls Window.  The default width is 200 with a 0-100 range.  
    // The Range can be changed with default Slider options, i.e. opt::Range(0,200), for example, to set a range of 0-200.
	// -->
	// The title is displayed beneath the slider, as well as the value. 
	//
    CSlider & DevSlider(const char * sSliderName = nullptr,const cwfOpt & cwOpt = cwfOpt());

    // AddEditBox() -- Add an EditBox to the default Dev control Window.  The sEditBoxTitle, while optional, will provide a
	// label to the left of the edit box.  The default width is 150 pixels or so, but can be changed with normal EditBox options
	//
    CEditBox & DevEditBox(const char * sEditBoxName = nullptr,const cwfOpt & cwOpt = cwfOpt());
  	CComboBox & DevComboBox(const char * sComboBoxName,const cwfOpt & cwOpt = cwfOpt());
	CWindow & DevWindow(const char * sTitle,int iNumlines,const cwfOpt & cwOpt = cwfOpt());
	CWindow & DevWindow(int iNumLines,const cwfOpt & cwOpt = cwfOpt());
	CWindow & DevWindow(const cwfOpt & cwOpt = cwfOpt());
	CWindow & DevWindow(const char * sTitle);

    CTextWidget & DevText(const char * sText,const cwfOpt & cwOpt  = cwfOpt());
    CTextWidget & DevText(const char * sText,int iHeight,const cwfOpt & cwOpt  = cwfOpt());
    CTextWidget & DevText(int iHeight,const cwfOpt & cwOpt  = cwfOpt());
    CTextWidget & DevText(const cwfOpt & cwOpt  = cwfOpt());


 	// DevAddSection() -- Adds a text section to the default Dev Controls window, to separate types of controls.
	// You can use opt::fgColor() to set the text color of the section name.
	//
    bool DevAddSection(const char * sSectionName = nullptr,const cwfOpt & cwOpt = cwfOpt());

    // GetDevControlsPtr() -- returns the pointer to the default CDevControls object. 
    // *** Important note *** -- this will return NULLPTR until a control is created with
    // QuickButton(), QuickSlider(), etc. The window is not created until a control is created in order
    // to save memory.
    //
    // With the pointer, any CDevContgrols() function can be used, though most are replicated directly
    // through CSageBox functions for ease of use.
    //
    CDevControls * GetDevControlsPtr();
 
    bool DevControlsTopmost(bool bTopmost = true);

    bool DisableClose(bool bDisable = true);
    bool CloseWindow();

    bool SetClsBitmap(RawBitmap_t & stBitmap,bool bClsNow = true);
    bool SetClsBitmap(CBitmap & cBitmap,bool bClsNow = true);
    bool ClearClsBitmap();
    CBitmap & GetClsBitmap();
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
	CBitmap ReadPgrBitmap(const char * sImageTitle,const char * sPgrFile,bool * bSuccess = nullptr);

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
	CBitmap ReadPgrBitmap(const char * sImageTitle,const unsigned char * sPGRMemory,bool * bSuccess = nullptr);
    

    bool CreateButtonGroup(int iGroupID,int iNumButtons,const wchar_t * * sButtonNames,POINT pLocation,SIZE szSize,int iColumns = 0, SIZE szSpacing = {-1,-1}, const cwfOpt & cwOpt = cwfOpt())
    {
        return CreateButtonGroup(iGroupID,iNumButtons,pLocation.x,pLocation.y,szSize.cx,szSize.cy,sButtonNames,iColumns,szSpacing,cwOpt);
    }
    bool CreateButtonGroup(int iGroupID,int iNumButtons,const wchar_t * * sButtonNames,POINT pLocation,int iColumns = 0, const cwfOpt & cwOpt = cwfOpt())
    {
        return CreateButtonGroup(iGroupID,iNumButtons,pLocation.x,pLocation.y,sButtonNames,iColumns,cwOpt);
    }

    bool CreateButtonGroup(int iGroupID,int iNumButtons,const char * * sButtonNames,POINT pLocation,SIZE szSize,int iColumns = 0, SIZE szSpacing = {-1,-1}, const cwfOpt & cwOpt = cwfOpt())
    {
        return CreateButtonGroup(iGroupID,iNumButtons,pLocation.x,pLocation.y,szSize.cx,szSize.cy,sButtonNames,iColumns,szSpacing,cwOpt);
    }
    bool CreateButtonGroup(int iGroupID,int iNumButtons,const char * * sButtonNames,POINT pLocation,int iColumns = 0, const cwfOpt & cwOpt = cwfOpt())
    {
        return CreateButtonGroup(iGroupID,iNumButtons,pLocation.x,pLocation.y,sButtonNames,iColumns,cwOpt);
    }

    
    bool SetSignal(SignalEvents event,bool & bSignal);
    bool SetSignal(SignalEvents event,Signal & stSignal);
    bool CancelSignal(SignalEvents event);
    bool UpdateBg(bool bUpdateNow = false);

    // Returns true of the main thread is stopped.  
    // Use StartThread() to resume the main thread. 
    //
    // This only applies to the original program thread and will not work for other threads created after
    // the program started.
    //
    bool ThreadStopped();
    
    // Returns the status of the thread.  ThreadStatus::Running or ThreadStatus::Suspended (if the thread is stopped).
    // This only applies to the original program thread and will not work for other threads created after
    // the program started.
    //
    ThreadStatus GetThreadStatus();

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
    int StopThread(bool bEndProgramOnClose = false,int iOkReturnValue = 0);

    // Resume the main thread if it is suspended.
    //
    // iValue will be returned by StopThread() when it is awakened. 0 is returned if it was awakened due to the window closing.
    //
    bool ResumeThread(int iValue = 0);

    // End the program when in the main message thread.  This is used when StopThread() has been used to stop the main thread but events are
    // still being handled through the Main Windows Message Thread.
    //
    // EndProgram() sets the window closing status and resumes the main thread.  Typically, the main thread will exit and SageBox will end.
    // However, StopThread() can be used anywhere and does not need to exit immediately.  It can take care of cleanup, memory deallocations, etc. 
    //
    // iValue will be returned by StopThread() when it is awakened. 0 is returned if it was awakened due to the window closing.
    //
    bool EndProgram(int iValue = 0);

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
    bool EndProgramOnClose(bool bEndOnClose); 
    
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
    bool EndProgramOnClose(int iReturnValue = 0); 

    // SendtoClipboard() -- Send contents of window to Windows Clipboard as a bitmap (so it may be pasted into other applications)
    //
    // If a Bitmap is included, the bitmap is sent to the clipoard instead.
    //
    // This transfers the visible canvas area only.  It does not copy the window frame or border. 
    //
    bool SendtoClipboard();

    // SendtoClipboard() -- Send contents of window to Windows Clipboard as a bitmap (so it may be pasted into other applications)
    //
    // If a Bitmap is included, the bitmap is sent to the clipoard instead.
    //
    // This transfers the visible canvas area only.  It does not copy the window frame or border. 
    //
    bool SendtoClipboard(CBitmap & cBitmap);


    // ImportClipboardText() -- Returns Text String in the Windows clipboard, if it exists.
    //
    // If there is text within the Windows Clipboard, a CString object will be returned with its contents.
    // Otherwise, an empty CString will be returned.
    //
    // a bSuccess pointer may be included which will be filled with the results (true if text was found, false if the CString returned is empty);
    //
    CString ImportClipboardText(bool * bSuccess = nullptr);

    // ImportClipboardTextW() -- Returns a Unicode Text String in the Windows clipboard, if it exists.
    //
    // If there is text within the Windows Clipboard, a CStringW object will be returned with its contents.
    // Otherwise, an empty CStringW will be returned.
    //
    // a bSuccess pointer may be included which will be filled with the results (true if text was found, false if the CStringW returned is empty);
    //
    CStringW ImportClipboardTextW(bool * bSuccess = nullptr);

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
    CBitmap ImportClipboardBitmap(bool * bSuccess = nullptr);

    bool isDevWindow() { return m_bDevWindow; }
    CSageBox * GetSageBox();

    // CenterInlineFonts() -- When true, this will center the text when font changes occur in output with {<font>} changes.
    // For example Write("This is a {Arial45}Centered{/} Font" will center the larger font in the middle of the text line when
    // Centering is active.  Otherwise, it will put the font start at and all other text in the line at the same Y value. 
    //
    // This is primarily used for specialized text, such as a TextWidget()
    //
    bool CenterInlineFonts(bool bCenterFonts = true);


};
}; // namespae Sage

#endif // _CDavWindow_H_

 