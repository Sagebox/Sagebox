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


#define __SageGDIPlusSupport

#include <string>
#include "SageOpt.h"
#include "CDevString.h"
#include "CString.h"
#include "DialogStruct.h"
#include "EventOpt.h"
#include "CPoint.h"
#include "CSize.h"
#include "CDavinci.h"
#include "CMenu.h"
#include "CJpeg.h"
#include "CWindowHandler.h"
#include "CStyleDefaults.h"
#include "Cpaswindow.h"
#include "Point3D.h"
#include "CDataStore.h"
#include <tuple>
#include <optional>
#include <array>
#include <map>
#include "CQuickForm.h"
#ifdef __SageGDIPlusSupport
#include <gdiplus.h>
#include "CGdiPlus.h"
#endif

#include <vector>

#include "WinConio.h"

namespace Sandbox
{
    class CQuickCpp;
};

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
    class CQuickDialog;

    using CInputBox = CEditBox;

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
    friend Sandbox::CQuickCpp;
    friend CQuickDialog;
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

    //inline static cwfOpt      cwNoOpt;          // Empty cwfOpt() that can be used as a default rather than specifying cwOpt() in the
    //                                            // function prototype.  Most (if not all ) uses of cwfOpt & cwOpt = cwfOpt() are deprecated
    //                                            // and are to be replaced with cwfOpt & cwOpt = cwNoOpt;
    //

    static cwfOpt      cwNoOpt;                 // $$ moved from static to keep more compatibility with C++11 (inline is C++17) for the first few release, to
                                                //    allow better compatibility with existing projects.  Will probably be restored once C++17 (or C++20) becomes
                                                //    a requirement.

    LONG                m_dwKnownStyle;         // Last known Window style.  Used for SetFullScreen(false) restoration

   // This private section should disappear altogether in an upcoming update
private:

    // Window Title Widget & Information for Child Windows with external titles.
    //
    //  Note -- The Window Titles for Child Windows are in-progress and may not be completely functional
    // (such as adapting to window size changes and movements)
    //
    struct WindowTitle_t
    {
        CTextWidget     * cWidget;        // Title Widget 
        bool              bHidden;        // Showing/not showing.
    };


    struct QuickFormType_t
    {
        QuickFormType eType;
        CDataStore::DataStore_t stDatastore;
        QuickFormEngine * pQuickForm;
    };

    // Memory allocated for various Window items, to avoid using stack memory.
    struct WindowPackage_t
    {
        WindowTitle_t   stWindowTitle;
        int             iLastSysMenu;
        bool            bPlacedSysMenuBorder;   // i.e. the one Sagebox adds
        bool            bMagicWindow;
        QuickFormType_t stQuickFormType;
    };
    static constexpr const char * kDefaultChildTitleFont = "17";                  // Font to set Child Title Widgets to when font is not specified in the options.
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
    bool m_bShowInitial             = false;                                // Internal use, subject to change
    bool m_bShowInitialized         = false;                                // Internal use, subject to change

    WindowPackage_t * m_WindowPackage    = nullptr;                         // Allocated memory window used for various items to keep from using the stack. 
    unsigned char * m_sLastKnownDIBMem   = nullptr;                         // Last Known DIB section memory installed by user ($$ This is temporary and will be removed)
    SIZE m_szLastKnownDIBSize       = {};                                   // Last Known DIB section size installed by user ($$ This is temporary and will be removed)

    bool InitDevControls();   // Initialize default Dev Controls Window -- added only if used. 
    CDevControls * m_cDevControls = nullptr;    // Not created until first used. 
    void _vprintf(const char * Format,va_list va_args);                                 // Used internally
    void _vprintf(const cwfOpt & cwOpt,const char * Format,va_list va_args);                                 // Used internally

    void DestructWindowPackage();                                           // Handle Destruction of Window Package Items & Window Package Memory itself
#ifdef __SageGDIPlusSupport
    static Gdiplus::Graphics m_cEmptyGDI;           // Empty GDI for invalid windows, etc. -- something to return that the GDI can deal with as an object with a NULL HDC
    Gdiplus::Graphics * m_cGDI = nullptr;           // The one and only GDI graphics object, though the user can create their own.
#endif

    // Bitmap Message Handle for BitmapWindow() returned windows
    // The main component is that pressing the 'X' button on the window
    // closes the window automatically (with normal CWindow windows, the 'X' simply sets a status which 
    // the owner has to field to close the window)
    //
    // $$PI -- This belongs in a separate file
    //
    class CBitmapWindowHandler : public CWindowHandler
    {
        CWindow * cWin = nullptr;

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

    bool CreateButtonGroup(const char * sGroupName,int iGroupID,int iNumButtons,int iX,int iY,int iWidth,int iHeight,const wchar_t * * sButtonNames,int iColumns = 0, SIZE szSpacing = {-1,-1}, const cwfOpt & cwOpt = cwfOpt());
    bool CreateButtonGroup(const char * sGroupName,int iGroupID,int iNumButtons,int iX,int iY,const wchar_t * * sButtonNames,int iColumns = 0, const cwfOpt & cwOpt = cwfOpt());
    bool CreateButtonGroup(const char * sGroupName,int iGroupID,int iNumButtons,int iX,int iY,int iWidth,int iHeight,const char * * sButtonNames,int iColumns = 0, SIZE szSpacing = {-1,-1}, const cwfOpt & cwOpt = cwfOpt());
    bool CreateButtonGroup(const char * sGroupName,int iGroupID,int iNumButtons,int iX,int iY,const char * * sButtonNames,int iColumns = 0, const cwfOpt & cwOpt = cwfOpt());
    stControlLabel_t CalcControlLabel(SizeRect srSize,const char * sText,LabelJust justType,bool bDraw,const cwfOpt & cwOpt = cwfOpt());
    

// kAdvPublic:
// This is defaulted to private to disallow CWindow class/object copying and deleting.
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
    // Copy Constructor
    CWindow(const CWindow &p2) 
    {
        FailBox("CWindow::CopyConstructor","In Copy Constructor")   // In debug mode, lets us know if it is ever used
    };

    // Internal public values (removed from public interface) (TBD)
    //
public:
    // _SetShowInitial() -- Internal Use only.  
    //
    void _SetCheckShowInitial(bool bShowInitial = true); 

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
    CString                     * m_cTempString2            = nullptr;    // Used for various temp uses.
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
    HMENU                     m_hMenu               = nullptr;  // Last known menu

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
    int GetOptInt(const char * sFind);  // An oversight for not making bool & bSet a pointer.
    bool GetOptInt(const char * sFind,int & iValue);

    long long GetOptLongLong(const char * sFind,bool & bSet);
    long long GetOptLongLong(const char * sFind);  // An oversight for not making bool & bSet a pointer.
    bool GetOptLongLong(const char * sFind,long long & iValue);
    long GetOptLong(const char * sFind,bool & bSet);
    long GetOptLong(const char * sFind);  // An oversight for not making bool & bSet a pointer.
    bool GetOptLong(const char * sFind, long & iValue);
    bool GetOptColor(const char * sFind,DWORD & dwColor);
    bool GetOptColor(const char * sFind,RGBColor_t & rgbColor);

    double GetOptFloat(const char * sFind,bool & bSet);
    bool GetOptFloat(const char * sFind,double & fValue);
    CString GetOptString(const char * sFind,bool * bSet = nullptr);
    bool GetOptString(const char * sFind,CString & stString);
    bool GetOptBool(const char * sFind,bool bDefault = false); 

    std::optional<std::array<long,2>> GetOptLongPair(const char * sFind);
    std::optional<SIZE> GetOptSize(const char * sFind);  
    bool GetOptSize(const char * sFind,SIZE * szSize); 
    SIZE GetOptSize(const char * sFind,SIZE szDefault);  

    std::optional<POINT> GetOptPoint(const char * sFind);  
    bool GetOptPoint(const char * sFind,POINT * pPOint); 
    POINT GetOptPoint(const char * sFind,POINT pDefault);  
    const char * GetOptPointer(const char * sFind); 


    void SetOptLine(const char * sOptions);
    POINT GetNewAutoWindowLoc();
    const char * GetInput(const char * sControls = nullptr,const char * sDefaultText = nullptr); 
    bool TranslateButtonStyle(SageString200 & stStyle,char * sStyle,char * sDefaultStyle,CStyleDefaults::ControlType eControlStyle,bool bStrict = false);
    bool _AutoUpdate(Sage::UpdateDirty upDateDirty);
    SIZE CreateGenericButtonGroup(bool bRadioButton,int iNumButtons,int iX,int iY,const char * * sButtonNames,int iGroupID, const char * sLabel, const cwfOpt & cwOpt = cwfOpt());
        
    

protected:
    friend DialogStruct;
    bool                  m_bActive         = false;    // Is the Window Active? (If not, it acts as if it is deleted)
    CUserWindow         * m_cUserWin        = nullptr;  // Main engine Window Interface
    CDavinci            * m_cDavinciMain    = nullptr;  // Interface to CSageBox global functions (Deprecated)
    CSageBox            * m_cSageBox        = nullptr;  // Main SageBox core
    CPasWindow          * m_cWinCore        = nullptr;  // Main core engine interface (CUserWindow's parent)
    CWindow             * m_cParent         = nullptr;  // Our Parent Window, if any


    // -----------------------------------------------------------------------------------------
    // Public Inteface to SageBox -- Available Functions for programming using SageBox functions
    // -----------------------------------------------------------------------------------------

public:
   // Empty controls used for various routine to retu rn the empty controls rather than nullptrs, for fallthrough errors

    static CWindow            m_cEmptyWindow;           // Returned when control not found for any function that returns a conrol reference.
    static CButton            m_cEmptyButton;          
    static CEditBox           m_cEmptyEditBox;         
    static CSlider            m_cEmptySlider;          
    static CListBox           m_cEmptyListBox;         
    static CComboBox          m_cEmptyComboBox;        
    static CDialog            m_cEmptyDialog;          
    
    // Returns Main Sagebox Root Hidden Window
    //
    static CWindow * GetSageWindow(); 

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
        int                 GetOptInt(const char * sFind);
        std::optional<int>  GetInt(const char * sFind);
        int GetInt(const char * sFind,int iDefault);
        std::optional<double>  GetFloat(const char * sFind);
        double GetFloat(const char * sFind,double fDefault);
        int GetOptIntOr(const char * sFind,int iDefault); 

        std::optional<long>  GetLong(const char * sFind);
        long GetLong(const char * sFind,long iDefault);
        long GetOptLongOr(const char * sFind,long iDefault); 

        // GetOptInt() -- Find an integer value in a string, i.e. "Offset=1234"
        //
        bool                GetOptInt(const char * sFind,int & iValue);

        // GetOptLongLong() -- Find an integer value in a string, i.e. "Offset=1234"
        //
        long long GetOptLongLong(const char * sFind,bool & bSet);

        // GetOptLongLong() -- Find an integer value in a string, i.e. "Offset=1234"
        //
        long long GetOptLongLong(const char * sFind);

        // GetOptLongLong() -- Find an longlong value in a string, i.e. "Offset=1234"
        //
        bool GetOptLongLong(const char * sFind,long long & iValue);

        // GetOptLong() -- Find an integer value in a string, i.e. "Offset=1234"
        //
        long GetOptLong(const char * sFind,bool & bSet);

        // GetOptLong() -- Find an integer value in a string, i.e. "Offset=1234"
        //
        long GetOptLong(const char * sFind);

         // GetOptLong() -- Find an longlong value in a string, i.e. "Offset=1234"
        //
        bool GetOptLong(const char * sFind, long & iValue);

        // Get a color in the string.  This can take two forms, typically generated by cwfOpt
        // i.e. fgColor="Red" or fgColor=\x123456
        // i.e. fgColor("Red"), fgColor(MyColor), fgColorRGB(255,0,0)), fgColor({255,0,0})
        //
        bool GetOptColor(const char * sFind,DWORD & dwColor);
        
 
        // Get a color in the string.  This can take two forms, typically generated by cwfOpt
        // i.e. fgColor="Red" or fgColor=\x123456
        // i.e. fgColor("Red"), fgColor(MyColor), fgColorRGB(255,0,0)), fgColor({255,0,0})
        //
        bool                GetOptColor(const char * sFind,RGBColor_t & rgbColor);
        std::optional<RgbColor> GetColor(const char * sFind); 
        RgbColor GetColor(const char * sFind,RgbColor rgbDefault); 

        //Get a float value from a string, i.e. "Radius = 4.5"
        //
        double              GetOptFloat(const char * sFind,bool & bSet);
        
        //Get a float value from a string, i.e. "Radius = 4.5"
        //
        bool GetOptFloat(const char * sFind,double & fValue);

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
        CString             GetString(const char * sFind,const char * sDefault = nullptr);
        bool                GetString(const char * sFind,CString * cOutString);

        // Get a boolean from the input stream, which is defined by being declared
        // For example, GetOptBool("Transparent") returns TRUE of Transparent is defined
        // (but not assigned -- "Transparent=<value>" is ignored. 
        // 
        // Returns FALSE if Transparent is not declared
        // Important: Note -- This will be changed to specifically looking for set values, i.e. "Transparent=true", rather than just the declaration
        //
        bool                GetOptBool(const char * sFind,bool bDefault = false);

        std::optional<std::array<long,2>> GetOptLongPair(const char * sFind);
        std::optional<SIZE> GetOptSize(const char * sFind);  
        bool GetOptSize(const char * sFind,SIZE * szSize); 
        SIZE GetOptSize(const char * sFind,SIZE szDefault);  

        std::optional<POINT> GetOptPoint(const char * sFind);  
        bool GetOptPoint(const char * sFind,POINT * pPOint); 
        POINT GetOptPoint(const char * sFind,POINT pDefault);  

        const char * GetOptPointer(const char * sFind); 

        template<typename eType>
        eType GetMapEnum(const char * sValName,std::map<std::string,eType> & inMap,eType eDefault = eType::Unknown)
        {
            auto eReturn = eDefault;
            if (EmptyString(sValName)) return eReturn;
            auto sTemp = CWindow::GetSageWindow()->opt.GetOptString(sValName); 
            if (!EmptyString(sTemp))
            {
                auto c = inMap.find(*sTemp); 
                if (c != inMap.end()) eReturn = c->second; 
            }
            return eReturn;
        }
        template<typename eType>
        eType GetMapBool(std::map<std::string,eType> & inMap,eType eDefault = eType::Unknown)
        {
            auto eReturn = eDefault;
            for (auto & name : inMap) 
            {
                if (GetOptBool(name.first.c_str()))
                {
                    eReturn = name.second;
                    break;
                }
            }
 
            return eReturn;
        }
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
    __forceinline bool EventPending(Peek peek = Peek::No) { return (!this || !m_cUserWin) ? false : m_cUserWin->EventPending(peek); }                                     // $QC

    // ClearEvent() -- Clear a specific pending event (whether it is pending or not).  This can be called before GetEvent() to clear the 
    // Event Queue of any specific event. For example, ClearEvent(SageEvent::MouseMove) or ClearEvent(SageEvent::WindowResize). 
    //
    // The latter example can be used to remove a pending window size when a Windows was initially sized with SetWindowSize(), which will
    // set a pending WindowResize() event.
    //
    // Use ClearEvents() to clear all pending events. 
    //
    __forceinline bool ClearEvent(SageEvent eEvent) { return (!this || !m_cUserWin) ? false : m_cUserWin->ClearEvent(eEvent); }                 // $QC
    
    // CLearEvents() -- Clear all pending events.  This can be used to clear any incoming events prior to calling GetEvent(). 
    // This can be useful when a setup, initialization, or other code has caused an unwanted event prior to the GetEvent() loop.
    //
    __forceinline bool ClearEvents() { return ClearEvent(SageEvent::All); }                                                                     // $QC

    // SetMessageHandler() -- Set the message handler for the window.  This overrides the default message handler.
    // This allows events such as OnMouseMove(), OnButton() press, as well as all other Windows Messages to be 
    // acted upon as received by windows. 
    //
    bool SetMessageHandler(CWindowHandler * cHandler,void * pClassInfo = nullptr);                                                              // $QC       

    // SetMessageHandler() -- Set the message handler for the window.  This overrides the default message handler.
    // This allows events such as OnMouseMove(), OnButton() press, as well as all other Windows Messages to be 
    // acted upon as received by windows. 
    //
    bool SetMessageHandler(CWindowHandler & cHandler,void * pClassInfo = nullptr);                                                              // $QC

    // $$ Temporary until AddHandler() and RemoveHandle() become available.  TBD
    bool __SetAdminHandler(CWindowHandler * cHandler,void * pClassInfo = nullptr);                                                              // $QC       
    // $$ Temporary until AddHandler() and RemoveHandle() become available.  TBD
    bool __SetAdminHandler(CWindowHandler & cHandler,void * pClassInfo = nullptr);                                                              // $QC


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
    int WinMessageBox(const char * sMessage,const char * sTitle = nullptr,unsigned int dwFlags = MB_OK);                                                          // $QC

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
    int GetInteger(const char * sTitle,bool & bCancelled,const cwfOpt & cwOptions = cwfOpt());              // $QC

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
    int GetInteger(const char * sTitle = nullptr,const cwfOpt & cwOptions = cwfOpt());                      // $QC

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
    bool GetInteger(const char * sTitle,int & iInteger,const cwfOpt & cwOptions = cwfOpt());                // $QC

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
    bool GetInteger(int & iInteger,const cwfOpt & cwOptions = cwfOpt());                                    // $QC

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
    int GetInteger(const cwfOpt & cwOptions);                                                               // $QC

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
    double GetFloat(const char * sTitle,bool & bCancelled,const cwfOpt & cwOptions = cwfOpt());             // $QC

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
    double GetFloat(const char * sTitle = nullptr,const cwfOpt & cwOptions = cwfOpt());                     // $QC

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
    bool GetFloat(const char * sTitle,double & fFloat,const cwfOpt & cwOptions = cwfOpt());                 // $QC

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
    bool GetFloat(double & fFloat,const cwfOpt & cwOptions = cwfOpt());                                     // $QC

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
    double GetFloat(const cwfOpt & cwOptions);                                                              // $QC
	
    // Dialog Info/Question Windows

    // Documentation TBD
	void InfoWindow(const char * sTitle,const cwfOpt & cwOptions = cwfOpt());                                       // $QC
    // Documentation TBD
	bool YesNoWindow(const char * sTitle,const cwfOpt & cwOptions = cwfOpt());                                      // $QC
    // Documentation TBD
	Sage::DialogResult YesNoCancelWindow(const char * sTitle,const cwfOpt & cwOptions = cwfOpt());                  // $QC
    // Documentation TBD
	bool OkCancelWindow(const char * sTitle,const cwfOpt & cwOptions = cwfOpt());                                   // $QC



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
    void printf(const cwfOpt & cwOpt,const char * Format,...);


    // printf() -- Works the same way as 'C' printf, except you can also put (X,Y) coordinates.
    // printf() can be very useful to quickly print text without using streaming notation,
    //
    // i.e. printf("This is attempt #d\n",++iAttempNo) vs. out << "This is attempt number " << ++iAttemptNo <<  "\n"
    //
    // printf(50,100,"Hello World") will print "Hello World") at (50,100) in the window and set the next write location accordingly.
    //
    void printf(int iX,int iY,const char * Format,...);

    // printf() -- Works the same way as 'C' printf, except you can also put (X,Y) coordinates.
    // printf() can be very useful to quickly print text without using streaming notation,
    //
    // i.e. printf("This is attempt #d\n",++iAttempNo) vs. out << "This is attempt number " << ++iAttemptNo <<  "\n"
    //
    // printf(50,100,"Hello World") will print "Hello World") at (50,100) in the window and set the next write location accordingly.
    //
    void printf(int iX,int iY,const cwfOpt & cwOpt,const char * Format,...);
    
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
    void Write(const char * sText,const char * sOptions = nullptr);     // $QCC

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
    void Write(const char * sText,const cwfOpt & cwOptions);                                                                                                    // $QCC

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
    void Write(int iX,int iY,const char * sText,const cwfOpt & cwOptions);                                                                                      // $QCC
    
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
    void Write(POINT pLoc,const char * sText,const cwfOpt & cwOptions = cwfOpt());                                                                              // $QCC

    // Writeln() -- Same as Write() but adds a '\n' at the end of the line for convenience
    void Writeln(const char * sText = nullptr,const char * sOptions = nullptr);                                                                                 // $QCC 

    // Writeln() -- Same as Write() but adds a '\n' at the end of the line for convenience
    void Writeln(const char * sText,const cwfOpt & cwOptions);                                                                                                  // $QCC     

    // Writeln() -- Same as Write() but adds a '\n' at the end of the line for convenience
    void Writeln(const cwfOpt & cwOptions);                                                                                                                     // $QCC  


    // Writeln() -- Same as Write() but adds a '\n' at the end of the line for convenience
    void Writeln(int iX,int iY,const char * sText = nullptr,const char * sOptions = nullptr);               // $$ To be removed  

    // Writeln() -- Same as Write() but adds a '\n' at the end of the line for convenience
    void Writeln(int iX,int iY,const char * sText,const cwfOpt & cwOptions);                                                                                    // $QCC               

    // Writeln() -- Same as Write() but adds a '\n' at the end of the line for convenience

    void Writeln(POINT pLoc,const char * sText,const cwfOpt & cwOptions = cwfOpt());                                                                            // $QCC

    // Writeln() -- Same as Write() but adds a '\n' at the end of the line for convenience
    void Writeln(int iX,int iY,const cwfOpt & cwOptions);                                                                                                       // $QCC

    // Put a character on the window just as putchar() in regular 'C'.
    // This is useful for Ansii-text and emulation
    // note: This can be useful for terminal emulation when a non-proportional font (i.e. a terminal font)
    //       is used, i.e. "Courier New,14", etc.
    //
    int putchar(char cChar);                                                                                                                                    // $QCC $QCNS
                                                                                                                                                                
    // Put a character on the window just as putchar() in regular 'C'.
    // This is useful for Ansii-text and emulation
    // note: This can be useful for terminal emulation when a non-proportional font (i.e. a terminal font)
    //       is used, i.e. "Courier New,14", etc.
    //
    int __putchar(char cChar);                                                                                                                                    // $QCC $QCNS

    // SetTabPos() -- Set the X position for Write/out/putchar/etc output to the character nth character position.
    // For example. SetTabPos(40) puts the output position (i.e. cursor) at charatcer position 40, based on the average character width.
    // This can be used to align console input boxes, text, etc. 
    // Since most fonts are proportionally spaced, the average character width is used.  In the case of terminal fonts, the character width is
    // the same for all characters and is more predictable
    //
    // This is included as a function from Basic() that is used in a number of programs
    //
    bool SetTabPos(int iPos);                                                                                                                                   // $QC

    // ClipWindow() -- Clips the window in the given rectangle, restricting output and drawing to that region.
    // This can be used to restrict write areas, Cls(), Drawing, etc. 
    // For example, a graph can be drawn with the clip and will not exceed the clip area.  This can be very useful and 
    // allow such drawing and output in place of creating a child window to perform the same task.
    // Use  ResetClipWindow() or ClipWindow() (with no paramaters) to remove the clipping region
    //
    // bAllowScroll allows the clip region to scroll based on writing text to the screen with '\n' newlines.
    // Default is to not allow the clipping region to scroll.
    //
    bool ClipWindow(int iX,int iY,int iWidth,int iHeight,bool bAllowScroll = false);                                                                            // $QC

    // ClipWindow() -- Clips the window in the given rectangle, restricting output and drawing to that region.
    // This can be used to restrict write areas, Cls(), Drawing, etc. 
    // For example, a graph can be drawn with the clip and will not exceed the clip area.  This can be very useful and 
    // allow such drawing and output in place of creating a child window to perform the same task.
    // Use  ResetClipWindow() or ClipWindow() (with no paramaters) to remove the clipping region
    //
    // bAllowScroll allows the clip region to scroll based on writing text to the screen with '\n' newlines.
    // Default is to not allow the clipping region to scroll.
    //
    bool ClipWindow(POINT pPoint,SIZE szSize,bool bAllowsScroll = false);                                                                                       // $QC

    // ClipWindow() -- Clips the window in the given rectangle, restricting output and drawing to that region.
    // This can be used to restrict write areas, Cls(), Drawing, etc. 
    // For example, a graph can be drawn with the clip and will not exceed the clip area.  This can be very useful and 
    // allow such drawing and output in place of creating a child window to perform the same task.
    // Use  ResetClipWindow() or ClipWindow() (with no paramaters) to remove the clipping region
    //
    bool ClipWindow();                                                                                                                                          // $QC

    // ResetClipWindow() -- Resets any clipping region active for the current window.
    //
    bool ResetClipWindow();                                                                                                                                     // $QC

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
    bool SetAutoScroll(WindowScroll scroll);                                                                                                                    // $QC
    
    // GetAutoScroll() -- Get the current scroll status
    // This can be used when the window doesn't seem to be scrolling to determine if the AutoScroll setting may be the issue.
    //
    WindowScroll GetAutoScroll();                                                                                                                               // $QC
    
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
    void Cls(DWORD iColor1=-1,DWORD iColor2 = -1);                                                                                                              // $QCC          

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
    void Cls(RGBColor_t rgbColor);                                                                                                                              // $QCC    

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
    void Cls(const char * sColor1,const char * sColor2=nullptr);                                                                                                // $QCC    

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
    void Cls(const char * sColor1,RGBColor_t rgbColor);                                                                                                         // $QCC

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
    void Cls(RGBColor_t rgbColor,const char * sColor2);                                                                                                         // $QCC   

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
    bool Cls(CBitmap & cBitmap);                                                                                                                                // $QCC

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
    void Cls(RGBColor_t rgbColor,RGBColor_t rgbColor2);                                                                                                         // $QCC    

    // Clears the window and also calls DontUpdate() to turn off Updates until Update() is called.
    // Update is called prior to the Cls().  This is useful for real-time displays.  See Cls() for more information
    //
    void ClsHold(DWORD iColor1=-1,DWORD iColor2 = -1);                                                                                                              // $QCC          

    // Clears the window and also calls DontUpdate() to turn off Updates until Update() is called.
    // Update is called prior to the Cls().  This is useful for real-time displays.  See Cls() for more information
    //
    void ClsHold(RGBColor_t rgbColor);                                                                                                                              // $QCC    

    // Clears the window and also calls DontUpdate() to turn off Updates until Update() is called.
    // Update is called prior to the Cls().  This is useful for real-time displays.  See Cls() for more information
    //
    void ClsHold(const char * sColor1,const char * sColor2=nullptr);                                                                                                // $QCC    

    // Clears the window and also calls DontUpdate() to turn off Updates until Update() is called.
    // Update is called prior to the Cls().  This is useful for real-time displays.  See Cls() for more information
    //
    bool ClsHold(CBitmap & cBitmap);                                                                                                                                // $QCC
    
    // Clears the window and also calls DontUpdate() to turn off Updates until Update() is called.
    // Update is called prior to the Cls().  This is useful for real-time displays.  See Cls() for more information
    //
    void ClsHold(RGBColor_t rgbColor,RGBColor_t rgbColor2);                                                                                                         // $QCC

    // Clears the window's entire canvas and also calls DontUpdate() to turn off Updates until Update() is called.
    // Update is called prior to the Cls().  This is useful for real-time displays.  See Cls() for more information
    //
    void ClsCanvasHold(DWORD iColor1=-1,DWORD iColor2 = -1);                                                                                                              // $QCC          
    
    // Clears the window's entire canvas and also calls DontUpdate() to turn off Updates until Update() is called.
    // Update is called prior to the Cls().  This is useful for real-time displays.  See Cls() for more information
    //
    void ClsCanvasHold(RGBColor_t rgbColor);                                                                                                                              // $QCC    
    
    // Clears the window's entire canvas and also calls DontUpdate() to turn off Updates until Update() is called.
    // Update is called prior to the Cls().  This is useful for real-time displays.  See Cls() for more information
    //
    void ClsCanvasHold(const char * sColor1,const char * sColor2=nullptr);                                                                                                // $QCC    
    
    // Clears the window's entire canvas and also calls DontUpdate() to turn off Updates until Update() is called.
    // Update is called prior to the Cls().  This is useful for real-time displays.  See Cls() for more information
    //
    void ClsCanvasHold(RGBColor_t rgbColor,RGBColor_t rgbColor2);                                                                                                         // $QCC

    
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
    void ClsCanvas(int iColor1=-1,int iColor2 = -1);                                                                                                            // $QCC

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
    void ClsCanvas(RGBColor_t rgbColor);                                                                                                                        // $QCC

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
    void ClsCanvas(const char * sColor1,const char * sColor2=nullptr);                                                                                          // $QCC 

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
    void ClsCanvas(const char * sColor1,RGBColor_t rgbColor);                                                                                                   // $QCC

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
    void ClsCanvas(RGBColor_t rgbColor,const char * sColor2);                                                                                                   // $QCC

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
    void ClsCanvas(RGBColor_t rgbColor,RGBColor_t rgbColor2);                                                                                                   // $QCC


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
    void Update(int iUpdateMS=0);                                                                                                                           // $QC

    /// <summary>
    /// Updates (repaints the window with any changes made to the window since the last update) all windows.
    /// <para></para>
    /// This function will not update windows with an AutoUpdate status of DontUpdate.
    /// <para></para>
    /// This function will update windows with a temporary DontUpdate() status. 
    /// </summary>
    /// <param name="iUpdateMS"></param>
    /// <returns></returns>
    bool UpdateAll(int iUpdateMS = 0);                                                                                                                      // $QC
    bool UpdateAll(bool bForced);                                                                                                                      // $QC

    // ForceUpdate() -- This is deprecated, but may be used in the future when more update/painting options are implemented
    //
    bool __ForceUpdate(bool bIfDirty = false);
    bool ForceUpdate();

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
    bool UpdateReady(int iUpdateMS = 10);                                                                                                                   // $QC

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
    bool SetWindowDrag(bool bGrab = true);                                                                                                                  // $QC

    // SetWindowSize() -- Set the size of the window.
    // 
    // This sets the entire size of the window, both the visible portion and the canvas size.
    // See SetCanvasSize() to set a larger canvas than the displayed window size.
    //
    // bInnerSize -- when true, this calculates the window size based on the inner canvas size.
    // When false, this calculates the window (when it is a popup or has a frame) to be the size of the entire window, 
    // making the canvas size smaller.
    //
    bool SetWindowSize(SIZE szSize,bool bInnerSize = false);                                                                                                // $QC

    // SetWindowSize() -- Set the size of the window.
    // 
    // This sets the entire size of the window, both the visible portion and the canvas size.
    // See SetCanvasSize() to set a larger canvas than the displayed window size.
    //
    // bInnerSize -- when true, this calculates the window size based on the inner canvas size.
    // When false, this calculates the window (when it is a popup or has a frame) to be the size of the entire window, 
    // making the canvas size smaller.
    //
    bool SetWindowSize(int iWidth,int iHeight,bool bInnerSize = false);                                                                                     // $QC

    // GetCanvasSize() -- Get the size of the Window Canvas. 
    // 
    // The Window Canvas is the bitmap that is shown in the window and may be larger than the window (but may never be smaller). 
    // This allows the Window to be resized dynamically, or to use a larger bitmap and move it around in the window, such as for scrolling.
    //
    SIZE GetCanvasSize();                                                                                                                                   // $QC

    // Set CanvasSize() -- Set the Canvas size of the window
    // This value must be greater than the displayed window canvas or it is ignored.
    //
    // This sets the Canvas size of the window (i.e. the drawable space), which can exceed the size of the window.
    //
    bool SetCanvasSize(SIZE szMaxSize,bool bAllowResizing = true);                                                                                          // $QC
    bool SetCanvasSize(SIZE szMinSize,SIZE szSize,bool bAllowResizing = true);                                                                              // $QC
 
    // Set CanvasSize() -- Set the Canvas size of the window
    // This value must be greater than the displayed window canvas or it is ignored.
    //
    // This sets the Canvas size of the window (i.e. the drawable space), which can exceed the size of the window.
    //
    bool SetCanvasSize(int iWidth,int iHeight,bool bAllowResizing = true);                                                                                  // $QC

    bool SetMaxWinSize(SIZE szMaxSize); 
    bool SetMaxWinSize(SIZE szMinSize,SIZE szMaxSize); 

    bool SetMinWinSize(SIZE szMinSize); 



    bool AllowResizing(bool bAllowResizing = true);                                                                                                         // $QC
    // GetClientSize() -- Get the current client area size (i.e. writeable/drawable part of the window)
    // This value may be larger than the visible window.
    //
    // GetWindowSize(), for example, returns the visible canvas area.
    //
    bool GetClientSize(SIZE & Size);                                                                                                                        // $QC

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
    bool UpdateRegion(RECT & rRegion,int iUpdateMS = 0);                                                                                                    // $QC

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
    bool UpdateRegion(int iX,int iY,int iWidth,int iHeight,int iUpdateMS = 0);                                                                              // $QC
 
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
    bool UpdateRegion(int iX,int iY,RawBitmap_t stBitmap,int iUpdateMS = 0);                                                                                // $QC

    // GetTextSize() -- Get the text size of the text using the current font.
    //
    // This returns the size the text will use in the window.  This can help with the placement of the text or controls around the text.
    // For example, using auto szSize = GetTextSize("This is some Text"), and then Write((szDesktopSize.cx-szSize.cx)/2,100,MyText)) 
    // will center the text in the X plane.
    //
    SIZE GetTextSize(const wchar_t * sText);                                                                                                                // $QC
  
    // GetTextSize() -- Get the text size of the text using the current font.
    //
    // This returns the size the text will use in the window.  This can help with the placement of the text or controls around the text.
    // For example, using auto szSize = GetTextSize("This is some Text"), and then Write((szDesktopSize.cx-szSize.cx)/2,100,MyText)) 
    // will center the text in the X plane.
    //
    SIZE GetTextSize(HFONT hFont,const wchar_t * sText);                                                                                                    // $QC

    // GetTextSize() -- Get the text size of the text using the current font.
    //
    // This returns the size the text will use in the window.  This can help with the placement of the text or controls around the text.
    // For example, using auto szSize = GetTextSize("This is some Text"), and then Write((szDesktopSize.cx-szSize.cx)/2,100,MyText)) 
    // will center the text in the X plane.
    //
    bool GetTextSize(const char * sText,SIZE & Size);                                                                                                       // $QC

    // GetTextSize() -- Get the text size of the text using the current font.
    //
    // This returns the size the text will use in the window.  This can help with the placement of the text or controls around the text.
    // For example, using auto szSize = GetTextSize("This is some Text"), and then Write((szDesktopSize.cx-szSize.cx)/2,100,MyText)) 
    // will center the text in the X plane.
    //
    SIZE GetTextSize(const char * sText);                                                                                                                  // $QC
    SIZE GetTextSize(HFONT hFont,const char * sText);                                                                                                      // $QC
    bool GetTextSize(HFONT hFont,const char * sText,SIZE & Size);                                                                                          // $QC
    SIZE GetTextSize(const char * sFont,const char * sText);                                                                                               // $QC
    bool GetTextSize(const char * sFont,const char * sText,SIZE & Size);                                                                                   // $QC

    SIZE GetTextSize(const char * sFont,const wchar_t * sText);                                                                                             // $QC
    SIZE GetTextSize(int iFontSize,const wchar_t * sText);                                                                                                  // $QC
    SIZE GetTextSize(int iFontSize,const char * sText);                                                                                                     // $QC
    bool GetTextSize(int iFontSize,const char * sText,SIZE & Size);                                                                                         // $QC

    // AddWindowShadow() -- Adds a shadow to the window.  This can be useful for popup-windows or
    // child windows embedded in the current window.
    //
    bool AddWindowShadow();                                                                                                                              // $QC

    bool DrawSphere(int iX,int iY,double fRadius,RgbColor fgColor,Point3D_t pAngles = {0,0,0},double fShininess = 50); 
    bool DrawSphere(int iX,int iY,double fRadius,RgbColor fgColor,double fShininess); 
    bool DrawSphere(POINT pLoc,double fRadius,RgbColor fgColor,Point3D_t pAngles = {0,0,0},double fShininess = 50); 
    bool DrawSphere(POINT pLoc,double fRadius,RgbColor fgColor,double fShininess); 

    /// #DrawRectangle
    /// <summary>
    /// Draws an open/wireframe Rectangle on the screen at starting point (x,y) with a width and height of (iWidth,iHeight)<para></para>
    /// See FillRectangle() to draw a filled Rectangle vs. an open/wireframe Rectangle<para></para>
    /// Note: x,y, Radius/Size, and PenSize values must all be of same type (int,float,double)<para></para>
    /// Note: Point, cfPoint, and cfPointf pair/vector arrays may also be used.  PenSize must be of same type as pair (i.e. int, float, double.)<para></para>
    /// --> This drawing function responds to current opacity and transformations and will anti-alias output results.<para></para>
    /// --> This drawing function is high-precision where float and double values may be used for positioning and size.<para></para>
    /// --> Hint: use an odd-size PenSize for sharper edges.<para></para>
    /// </summary>
    /// <param name="iX">- Location of the X center of the rectangle in the window.  See various overloads for different types of X,Y input.</param>
    /// <param name="iY">- Location of the Y center of the rectangle in the window.</param>
    /// <param name="iWidth">- Width (in pixels) of the rectangle in the X dimension. See various overloads for different types of Width,Height input.</param>
    /// <param name="iHeight">- Radius (in pixels) of the rectangle in the Y dimension.</param>
    /// <param name="rgbColor">- Color of the rectangle</param>
    /// <param name="iPenSize">- [optional] Pen size for the rectangle color.  If the PenSize is not specified, the current default pen size is used.</param>
    /// <returns></returns>
    bool DrawRectangle(int iX,int iY,int iWidth,int iHeight,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);                                       // $QCC #DrawRectangle 
    bool DrawRectangle(POINT pLoc,SIZE szSize,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);                                                     // $QCC #DrawRectangle
    bool DrawRectangle(float iX,float iY,float iWidth,float iHeight,RGBColor_t rgbColor = Rgb::Default,float fPenSize = 0);                             // $QCC #DrawRectangle 
    bool DrawRectangle(double iX,double iY,double iWidth,double iHeight,RGBColor_t rgbColor = Rgb::Default,double fPenSize = 0);                        // $QCC #DrawRectangle 
    bool DrawRectangle(CfPoint fLoc,CfPoint fSize,RGBColor_t rgbColor = Rgb::Default,double fPenSize = 0);                                              // $QCC #DrawRectangle 
    bool DrawRectangle(CfPointf fLoc,CfPointf fSize,RGBColor_t rgbColor = Rgb::Default,float fPenSize = 0);                                             // $QCC #DrawRectangle 

    /// #FillRectangle
    /// <summary>
    /// Draws a filled Rectangle on the screen at starting point (x,y) with a width and height of (iWidth,iHeight)<para></para>
    /// See DrawRectangle() to draw an open/wireframe Rectangle vs. a filled Rectangle<para></para>
    /// Note: x,y, Radius/Size, and PenSize values must all be of same type (int,float,double)<para></para>
    /// Note: Point, cfPoint, and cfPointf pair/vector arrays may also be used.  PenSize must be of same type as pair (i.e. int, float, double.)<para></para>
    /// --> This drawing function responds to current opacity and transformations and will anti-alias output results.<para></para>
    /// --> This drawing function is high-precision where float and double values may be used for positioning and size.<para></para>
    /// --> Hint: use an odd-size PenSize for sharper edges.<para></para>
    /// </summary>
    /// <param name="iX">- Location of the X center of the rectangle in the window.  See various overloads for different types of X,Y input.</param>
    /// <param name="iY">- Location of the Y center of the rectangle in the window.</param>
    /// <param name="iWidth">- Width (in pixels) of the rectangle in the X dimension. See various overloads for different types of Width,Height input.</param>
    /// <param name="iHeight">- Radius (in pixels) of the rectangle in the Y dimension.</param>
    /// <param name="rgbColorIn">- Color of the interior of the rectangle</param>
    /// <param name="rgbColorOut">- [optional] Color of the rectangle border</param>
    /// <param name="iPenSize">- [optional] Pen size for the rectangle border.  If the PenSize is not specified, the current default pen size is used.</param>
    /// <returns></returns>
    bool FillRectangle(int iX,int iY,int iWidth,int iHeight,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,int iPenSize = 0);                // $QCC #FillRectangle
    bool FillRectangle(POINT pLoc,SIZE szSize,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,int iPenSize = 0);                              // $QCC #FillRectangle
    bool FillRectangle(float iX,float iY,float iWidth,float iHeight,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,float fPenSize = 0);      // $QCC #FillRectangle   
    bool FillRectangle(double iX,double iY,double iWidth,double iHeight,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,double fPenSize = 0); // $QCC #FillRectangle   
    bool FillRectangle(CfPoint fLoc,CfPoint fSize,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,double fPenSize = 0);                       // $QCC #FillRectangle   
    bool FillRectangle(CfPointf fLoc,CfPointf fSize,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,float fPenSize = 0);                      // $QCC #FillRectangle   

    bool DrawFilledRectangle(int iX,int iY,int iWidth,int iHeight,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,int iPenSize = 0);                  // $QCC #FillRectangle
    bool DrawFilledRectangle(POINT pLoc,SIZE szSize,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,int iPenSize = 0);                                // $QCC #FillRectangle
    bool DrawFilledRectangle(float iX,float iY,float iWidth,float iHeight,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,float fPenSize = 0);        // $QCC #FillRectangle      
    bool DrawFilledRectangle(double iX,double iY,double iWidth,double iHeight,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,double fPenSize = 0);   // $QCC #FillRectangle      
    bool DrawFilledRectangle(CfPoint fLoc,CfPoint fSize,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,double fPenSize = 0);                         // $QCC #FillRectangle      
    bool DrawFilledRectangle(CfPointf fLoc,CfPointf fSize,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,float fPenSize = 0);                        // $QCC #FillRectangle      

    /// #DrawFilledRectangleFast
    /// <summary>
    /// Draws a 'fast' filled Rectangle on the screen at starting point (x,y) with a width and height of (iWidth,iHeight)<para></para>
    /// See DrawRectangleFast() to draw an open/wireframe Rectangle vs. a filled Rectangle<para></para>
    /// Note: x,y, Radius/Size, and PenSize values must all be of same type (int,float,double)<para></para>
    /// Note: POINT arrays may also be used.  PenSize must be of same type as pair (i.e. int, float, double.)<para></para>
    /// --> This drawing function is a 'fast' function that does not use anti-aliasing and is about 10x faster than
    /// non-fast drawing functions.  These functions are useful for general drawing and do not respond to opacity or transformations.<para></para>
    /// </summary>
    /// <param name="iX">- Location of the X center of the rectangle in the window.  See various overloads for different types of X,Y input.</param>
    /// <param name="iY">- Location of the Y center of the rectangle in the window.</param>
    /// <param name="iWidth">- Width (in pixels) of the rectangle in the X dimension. See various overloads for different types of Width,Height input.</param>
    /// <param name="iHeight">- Radius (in pixels) of the rectangle in the Y dimension.</param>
    /// <param name="rgbColorIn">- Color of the interior of the rectangle</param>
    /// <param name="rgbColorOut">- [optional] Color of the rectangle border</param>
    /// <param name="iPenSize">- [optional] Pen size for the rectangle border.  If the PenSize is not specified, the current default pen size is used.</param>
    /// <returns></returns>
    bool DrawFilledRectangleFast(int iX,int iY,int iWidth,int iHeight,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,int iPenSize = 0); // $QCC #DrawFilledRectangleFast
    bool DrawFilledRectangleFast(POINT pLoc,SIZE szSize,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,int iPenSize = 0);               // $QCC #DrawFilledRectangleFast
   
    bool FillRectangleFast(int iX,int iY,int iWidth,int iHeight,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,int iPenSize = 0);       // $QCC #DrawFilledRectangleFast
    bool FillRectangleFast(POINT pLoc,SIZE szSize,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,int iPenSize = 0);                     // $QCC #DrawFilledRectangleFast
    
    /// #DrawRectangleFast
    /// <summary>
    /// Draws an open/wireframe Rectangle on the screen at starting point (x,y) with a width and height of (iWidth,iHeight)<para></para>
    /// See FillRectangleFast() to draw a filled Rectangle vs. an open/wireframe Rectangle<para></para>
    /// Note: x,y, Radius/Size, and PenSize values must all be of same type (int,float,double)<para></para>
    /// Note: POINT arrays may also be used.  PenSize must be of same type as pair (i.e. int, float, double.)<para></para>
    /// --> This drawing function is a 'fast' function that does not use anti-aliasing and is about 10x faster than
    /// non-fast drawing functions.  These functions are useful for general drawing and do not respond to opacity or transformations.<para></para>
    /// </summary>
    /// <param name="iX">- Location of the X center of the rectangle in the window.  See various overloads for different types of X,Y input.</param>
    /// <param name="iY">- Location of the Y center of the rectangle in the window.</param>
    /// <param name="iWidth">- Width (in pixels) of the rectangle in the X dimension. See various overloads for different types of Width,Height input.</param>
    /// <param name="iHeight">- Radius (in pixels) of the rectangle in the Y dimension.</param>
    /// <param name="rgbColor">- Color of the rectangle</param>
    /// <param name="iPenSize">- [optional] Pen size for the rectangle color.  If the PenSize is not specified, the current default pen size is used.</param>
    /// <returns></returns>
    bool DrawRectangleFast(int iX,int iY,int iWidth,int iHeight,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);   // $QCC #DrawRectangleFast    
    bool DrawRectangleFast(POINT pLoc,SIZE szSize,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);                 // $QCC #DrawRectangleFast
   
    // Triangle functions

    /// #DrawTriangle
    /// <summary>
    /// Draws an open/wireframe Triangle on the screen at starting point (x,y) with vertexes v1,v2,and v3<para></para>
    /// See FillTriangle() to draw a filled Triangle vs. an open/wireframe Triangle<para></para>
    /// Note: Vertex and PenSize values must all be of same type (int,float,double)<para></para>
    /// Note: Point, cfPoint, and cfPointf pair/vector arrays may also be used.  PenSize must be of same type as pair (i.e. int, float, double.)<para></para>
    /// --> This drawing function responds to current opacity and transformations and will anti-alias output results.<para></para>
    /// --> This drawing function is high-precision where float and double values may be used for positioning and size.<para></para>
    /// --> Hint: use an odd-size PenSize for sharper edges.<para></para>
    /// </summary>
    /// <param name="v1">- (x,y) vertex 1 of the Triangle.</param>
    /// <param name="v2">- (x,y) vertex 2 of the Triangle.</param>
    /// <param name="v3">- (x,y) vertex 3 of the Triangle.</param>
    /// <param name="rgbColor">- Color of the triangle</param>
    /// <param name="iPenSize">- [optional] Pen size for the triangle color.  If the PenSize is not specified, the current default pen size is used.</param>
    /// <returns></returns>
    bool DrawTriangle(POINT v1,POINT v2,POINT v3,RgbColor rgbColor,int iPenSize = 0);                                                               // $QCC #DrawTriangle
    bool DrawTriangle(CfPoint v1,CfPoint v2,CfPoint v3,RgbColor rgbColor,double iPenSize = 0);                                                      // $QCC #DrawTriangle
    bool DrawTriangle(CfPointf v1,CfPointf v2,CfPointf v3,RgbColor rgbColor,float iPenSize = 0);                                                    // $QCC #DrawTriangle
                                                                                                                                                            
    /// #FillTriangle
    /// <summary>
    /// Draws a filled Triangle on the screen at starting point (x,y) with vertexes v1,v2,and v3<para></para>
    /// See DrawTriangle() to draw an open/wireframe Triangle vs. a filled Triangle an open/wireframe Triangle<para></para>
    /// Note: Vertex and PenSize values must all be of same type (int,float,double)<para></para>
    /// Note: POINT, cfPoint, and cfPointf pair/vector arrays may also be used.  PenSize must be of same type as pair (i.e. int, float, double.)<para></para>
    /// --> This drawing function responds to current opacity and transformations and will anti-alias output results.<para></para>
    /// --> This drawing function is high-precision where float and double values may be used for positioning and size.<para></para>
    /// --> Hint: use an odd-size PenSize for sharper edges.<para></para>
    /// </summary>
    /// <param name="v1">- (x,y) vertex 1 of the Triangle.</param>
    /// <param name="v2">- (x,y) vertex 2 of the Triangle.</param>
    /// <param name="v3">- (x,y) vertex 3 of the Triangle.</param>
    /// <param name="rgbColorIn">- Color of the triangle's interior</param>
    /// <param name="rgbColorOut">- [optional] Color of the triangle's border</param>
    /// <param name="iPenSize">- [optional] Pen size for the border color if specified.  If the PenSize is not specified, the current default pen size is used.</param>
    /// <returns></returns>
    bool FillTriangle(POINT v1,POINT v2,POINT v3,RgbColor rgbColorIn, RgbColor rgbColorOut = Rgb::Undefined,int iPenSize = 0);                      // $QCC #FillTriangle
    bool FillTriangle(CfPoint v1,CfPoint v2,CfPoint v3,RgbColor rgbColorIn, RgbColor rgbColorOut = Rgb::Undefined,double iPenSize = 0);             // $QCC #FillTriangle
    bool FillTriangle(CfPointf v1,CfPointf v2,CfPointf v3,RgbColor rgbColorIn, RgbColor rgbColorOut = Rgb::Undefined,float iPenSize = 0);           // $QCC #FillTriangle

    bool DrawFilledTriangle(POINT v1,POINT v2,POINT v3,RgbColor rgbColorIn, RgbColor rgbColorOut = Rgb::Undefined,int iPenSize = 0);                // $QCC #FillTriangle
    bool DrawFilledTriangle(CfPoint v1,CfPoint v2,CfPoint v3,RgbColor rgbColorIn, RgbColor rgbColorOut = Rgb::Undefined,double iPenSize = 0);       // $QCC #FillTriangle
    bool DrawFilledTriangle(CfPointf v1,CfPointf v2,CfPointf v3,RgbColor rgbColorIn, RgbColor rgbColorOut = Rgb::Undefined,float iPenSize = 0);     // $QCC #FillTriangle

    /// #FillTriangleFast
    /// <summary>
    /// Draws a 'fast' filled Triangle on the screen at starting point (x,y) with vertexes v1,v2,and v3<para></para>
    /// See DrawTriangleFast() to draw an open/wireframe Triangle vs. a filled Triangle an open/wireframe Triangle<para></para>
    /// Note: POINT arrays may also be used.  PenSize must be of same type as pair (i.e. int, float, double.)<para></para>
    /// --> This drawing function is a 'fast' function that does not use anti-aliasing and is about 10x faster than
    /// non-fast drawing functions.  These functions are useful for general drawing and do not respond to opacity or transformations.<para></para>
    /// </summary>
    /// <param name="v1">- (x,y) vertex 1 of the Triangle.</param>
    /// <param name="v2">- (x,y) vertex 2 of the Triangle.</param>
    /// <param name="v3">- (x,y) vertex 3 of the Triangle.</param>
    /// <param name="rgbColorIn">- Color of the triangle's interior</param>
    /// <param name="rgbColorOut">- [optional] Color of the triangle's border</param>
    /// <param name="iPenSize">- [optional] Pen size for the border color if specified.  If the PenSize is not specified, the current default pen size is used.</param>
    /// <returns></returns>
    bool DrawFilledTriangleFast(POINT v1,POINT v2,POINT v3,RgbColor rgbColorIn, RgbColor rgbColorOut = Rgb::Undefined,int iPenSize = 0);            // $QCC #FillTriangleFast
    bool FillTriangleFast(POINT v1,POINT v2,POINT v3,RgbColor rgbColorIn, RgbColor rgbColorOut = Rgb::Undefined,int iPenSize = 0);                  // $QCC #FillTriangleFast

    /// #DrawTriangleFast
    /// <summary>
    /// Draws a 'fast' open/wireframe Triangle on the screen at starting point (x,y) with vertexes v1,v2,and v3<para></para>
    /// See FillTriangle() to draw a filled Triangle vs. an open/wireframe Triangle<para></para>
    /// Note: POINT arrays may also be used.  PenSize must be of same type as pair (i.e. int, float, double.)<para></para>
    /// --> This drawing function is a 'fast' function that does not use anti-aliasing and is about 10x faster than
    /// non-fast drawing functions.  These functions are useful for general drawing and do not respond to opacity or transformations.<para></para>
    /// </summary>
    /// <param name="v1">- (x,y) vertex 1 of the Triangle.</param>
    /// <param name="v2">- (x,y) vertex 2 of the Triangle.</param>
    /// <param name="v3">- (x,y) vertex 3 of the Triangle.</param>
    /// <param name="rgbColor">- Color of the triangle</param>
    /// <param name="iPenSize">- [optional] Pen size for the triangle color.  If the PenSize is not specified, the current default pen size is used.</param>
    /// <returns></returns>
    bool DrawTriangleFast(POINT v1,POINT v2,POINT v3,RgbColor rgbColor,int iPenSize = 0);  // $QCC #DrawTriangleFast

    // QuadRangle Functions
  
    /// #DrawQuadrangle
    /// <summary>
    /// Draws an open/wireframe Quadrangle on the screen at starting point (x,y) with vertexes v1,v2,v3, and v4.<para></para>
    /// See FillQuadrangle() to draw a filled Quadrangle vs. an open/wireframe Quadrangle<para></para>
    /// Note: Vertex and PenSize values must all be of same type (int,float,double)<para></para>
    /// Note: Point, cfPoint, and cfPointf pair/vector arrays may also be used.  PenSize must be of same type as pair (i.e. int, float, double.)<para></para>
    /// --> This drawing function responds to current opacity and transformations and will anti-alias output results.<para></para>
    /// --> This drawing function is high-precision where float and double values may be used for positioning and size.<para></para>
    /// --> Hint: use an odd-size PenSize for sharper edges.<para></para>
    /// </summary>
    /// <param name="v1">- (x,y) vertex 1 of the quadrangle.</param>
    /// <param name="v2">- (x,y) vertex 2 of the quadrangle.</param>
    /// <param name="v3">- (x,y) vertex 3 of the quadrangle.</param>
    /// <param name="v4">- (x,y) vertex 4 of the quadrangle.</param>
    /// <param name="rgbColor">- Color of the quadrangle</param>
    /// <param name="iPenSize">- [optional] Pen size for the quadrangle color.  If the PenSize is not specified, the current default pen size is used.</param>
    /// <returns></returns>
    bool DrawQuadrangle(POINT v1,POINT v2,POINT v3,POINT v4,RgbColor rgbColor,int iPenSize = 0);                                                                // $QCC #DrawQuadrangle
    bool DrawQuadrangle(CfPoint v1,CfPoint v2,CfPoint v3,CfPoint v4,RgbColor rgbColor,double iPenSize = 0);                                                     // $QCC #DrawQuadrangle
    bool DrawQuadrangle(CfPointf v1,CfPointf v2,CfPointf v3,CfPointf v4,RgbColor rgbColor,float iPenSize = 0);                                                  // $QCC #DrawQuadrangle

    /// #FillQuadrangle
    /// <summary>
    /// Draws a filled Quadrangle on the screen at starting point (x,y) with with vertexes v1,v2,v3, and v4.<para></para>
    /// See DrawQuadrangle() to draw an open/wireframe Quadrangle vs. a filled Quadrangle an open/wireframe Quadrangle<para></para>
    /// Note: Vertex and PenSize values must all be of same type (int,float,double)<para></para>
    /// Note: POINT, cfPoint, and cfPointf pair/vector arrays may also be used.  PenSize must be of same type as pair (i.e. int, float, double.)<para></para>
    /// --> This drawing function responds to current opacity and transformations and will anti-alias output results.<para></para>
    /// --> This drawing function is high-precision where float and double values may be used for positioning and size.<para></para>
    /// --> Hint: use an odd-size PenSize for sharper edges.<para></para>
    /// </summary>
    /// <param name="v1">- (x,y) vertex 1 of the quadrangle.</param>
    /// <param name="v2">- (x,y) vertex 2 of the quadrangle.</param>
    /// <param name="v3">- (x,y) vertex 3 of the quadrangle.</param>
    /// <param name="v4">- (x,y) vertex 4 of the quadrangle.</param>
    /// <param name="rgbColorIn">- Color of the quadrangle's interior</param>
    /// <param name="rgbColorOut">- [optional] Color of the quadrangle's border</param>
    /// <param name="iPenSize">- [optional] Pen size for the border color if specified.  If the PenSize is not specified, the current default pen size is used.</param>
    /// <returns></returns>
    bool FillQuadrangle(POINT v1,POINT v2,POINT v3,POINT v4,RgbColor rgbColorIn, RgbColor rgbColorOut = Rgb::Undefined,int iPenSize = 0);                       // $QCC #FillQuadrangle
    bool FillQuadrangle(CfPoint v1,CfPoint v2,CfPoint v3,CfPoint v4, RgbColor rgbColorIn, RgbColor rgbColorOut = Rgb::Undefined,double iPenSize = 0);           // $QCC #FillQuadrangle
    bool FillQuadrangle(CfPointf v1,CfPointf v2,CfPointf v3,CfPointf v4, RgbColor rgbColorIn, RgbColor rgbColorOut = Rgb::Undefined,float iPenSize = 0);        // $QCC #FillQuadrangle

    bool DrawFilledQuadrangle(POINT v1,POINT v2,POINT v3,POINT v4,RgbColor rgbColorIn, RgbColor rgbColorOut = Rgb::Undefined,int iPenSize = 0);                 // $QCC #FillQuadrangle
    bool DrawFilledQuadrangle(CfPoint v1,CfPoint v2,CfPoint v3,CfPoint v4,RgbColor rgbColorIn, RgbColor rgbColorOut = Rgb::Undefined,double iPenSize = 0);      // $QCC #FillQuadrangle
    bool DrawFilledQuadrangle(CfPointf v1,CfPointf v2,CfPointf v3,CfPointf v4,RgbColor rgbColorIn, RgbColor rgbColorOut = Rgb::Undefined,float iPenSize = 0);   // $QCC #FillQuadrangle

    /// #FillQuadrangleFast
    /// <summary>
    /// Draws a 'fast' filled Quadrangle on the screen at starting point (x,y) with vertexes v1,v2,v3, and v4.<para></para>
    /// See DrawQuadrangleFast() to draw an open/wireframe Quadrangle vs. a filled Quadrangle an open/wireframe Quadrangle<para></para>
    /// Note: POINT arrays may also be used.  PenSize must be of same type as pair (i.e. int, float, double.)<para></para>
    /// --> This drawing function is a 'fast' function that does not use anti-aliasing and is about 10x faster than
    /// non-fast drawing functions.  These functions are useful for general drawing and do not respond to opacity or transformations.<para></para>
    /// </summary>
    /// <param name="v1">- (x,y) vertex 1 of the quadrangle.</param>
    /// <param name="v2">- (x,y) vertex 2 of the quadrangle.</param>
    /// <param name="v3">- (x,y) vertex 3 of the quadrangle.</param>
    /// <param name="v4">- (x,y) vertex 4 of the quadrangle.</param>
    /// <param name="rgbColorIn">- Color of the quadrangle's interior</param>
    /// <param name="rgbColorOut">- [optional] Color of the quadrangle's border</param>
    /// <param name="iPenSize">- [optional] Pen size for the border color if specified.  If the PenSize is not specified, the current default pen size is used.</param>
    /// <returns></returns>
    bool DrawFilledQuadrangleFast(POINT v1,POINT v2,POINT v3,POINT v4,RgbColor rgbColorIn, RgbColor rgbColorOut = Rgb::Undefined,int iPenSize = 0);     // $QCC #FillQuadrangleFast
    bool FillQuadrangleFast(POINT v1,POINT v2,POINT v3,POINT v4,RgbColor rgbColorIn, RgbColor rgbColorOut = Rgb::Undefined,int iPenSize = 0);           // $QCC #FillQuadrangleFast
 
    /// #DrawQuadrangleFast
    /// <summary>
    /// Draws a 'fast' open/wireframe Quadrangle on the screen at starting point (x,y) with vertexes v1,v2,v3, and v4.<para></para>
    /// See FillQuadrangle() to draw a filled Quadrangle vs. an open/wireframe TriQuadrangleangle<para></para>
    /// Note: POINT arrays may also be used.  PenSize must be of same type as pair (i.e. int, float, double.)<para></para>
    /// --> This drawing function is a 'fast' function that does not use anti-aliasing and is about 10x faster than
    /// non-fast drawing functions.  These functions are useful for general drawing and do not respond to opacity or transformations.<para></para>
    /// </summary>
    /// <param name="v1">- (x,y) vertex 1 of the quadrangle.</param>
    /// <param name="v2">- (x,y) vertex 2 of the quadrangle.</param>
    /// <param name="v3">- (x,y) vertex 3 of the quadrangle.</param>
    /// <param name="rgbColor">- Color of the quadrangle</param>
    /// <param name="iPenSize">- [optional] Pen size for the quadrangle color.  If the PenSize is not specified, the current default pen size is used.</param>
    /// <returns></returns>
    bool DrawQuadrangleFast(POINT v1,POINT v2,POINT v3,POINT v4,RgbColor rgbColor,int iPenSize = 0);  // $QCC #DrawQuadrangleFast

    /// #DrawPolygon
    /// <summary>
    /// Draws an open/wireframe Polygon on the screen at starting point (x,y) with vertexes defined in pPoints<para></para>
    /// See FillPolygon() to draw a filled Polygon vs. an open/wireframe Polygon<para></para>
    /// Note: vertex and PenSize values must all be of same type (int,float,double)<para></para>
    /// Note: Point, cfPoint, and cfPointf pair/vector arrays may also be used.  PenSize must be of same type as pair (i.e. int, float, double.)<para></para>
    /// --> This drawing function responds to current opacity and transformations and will anti-alias output results.<para></para>
    /// --> This drawing function is high-precision where float and double values may be used for positioning and size.<para></para>
    /// --> Hint: use an odd-size PenSize for sharper edges.<para></para>
    /// </summary>
    /// <param name="pPoints">- POINT, cPoint, cPointf, or cfPointf array of vertices</param>
    /// <param name="iVertices">- Number of vertices in pPoints.</param>
    /// <param name="v3">- (x,y) vertex 3 of the polygon.</param>
    /// <param name="rgbColor">- Color of the polygon</param>
    /// <param name="iPenSize">- [optional] Pen size for the polygon color.  If the PenSize is not specified, the current default pen size is used.</param>
    /// <returns></returns>
    bool DrawPolygon(POINT * pPoints,int iVertices,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);                                                                        // $QCC
    bool DrawPolygon(CfPointf * pPoints,int iVertices,RGBColor_t rgbColor = Rgb::Default,float iPenSize = 0);                                                                   // $QCC
    bool DrawPolygon(CPoint * pPoints,int iVertices,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);                                                                       // $QCC
    bool DrawPolygon(CfPoint * pPoints,int iVertices,RGBColor_t rgbColor = Rgb::Default,double iPenSize = 0);                                                                   // $QCC
    bool DrawPolygon(Gdiplus::Point * pPoints,int iVertices,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);                                                               // $QCC
    bool DrawPolygon(Gdiplus::PointF * pPoints,int iVertices,RGBColor_t rgbColor = Rgb::Default,float iPenSize = 0);                                                            // $QCC

    /// #DrawFilledPolygon
    /// <summary>
    /// Draws an filled Polygon on the screen at starting point (x,y) with vertexes defined in pPoints<para></para>
    /// See DrawPolygon() to draw an open/wireframe Polygon vs. a filled Polygon<para></para>
    /// Note: vertex and PenSize values must all be of same type (int,float,double)<para></para>
    /// Note: Point, cfPoint, and cfPointf pair/vector arrays may also be used.  PenSize must be of same type as pair (i.e. int, float, double.)<para></para>
    /// --> This drawing function responds to current opacity and transformations and will anti-alias output results.<para></para>
    /// --> This drawing function is high-precision where float and double values may be used for positioning and size.<para></para>
    /// --> Hint: use an odd-size PenSize for sharper edges.<para></para>
    /// </summary>
    /// <param name="pPoints">- POINT, cPoint, cPointf, or cfPointf array of vertices</param>
    /// <param name="iVertices">- Number of vertices in pPoints.</param>
    /// <param name="v3">- (x,y) vertex 3 of the polygon.</param>
    /// <param name="rgbColorIn">- Color of the quadrangle's interior</param>
    /// <param name="rgbColorOut">- [optional] Color of the quadrangle's border</param>
    /// <param name="iPenSize">- [optional] Pen size for the border color if specified.  If the PenSize is not specified, the current default pen size is used.</param>
    /// <returns></returns>
    bool DrawFilledPolygon(POINT * pPoints,int iVertices,RGBColor_t rgbColorIn = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined,int iPenSize = 0);                // $QCC #DrawFilledPolygon
    bool DrawFilledPolygon(CPoint * pPoints,int iVertices,RGBColor_t rgbColorIn = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined,int iPenSize = 0);               // $QCC #DrawFilledPolygon
    bool DrawFilledPolygon(CfPointf * pPoints,int iVertices,RGBColor_t rgbColorIn = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined,float iPenSize = 0);           // $QCC #DrawFilledPolygon
    bool DrawFilledPolygon(CfPoint * pPoints,int iVertices,RGBColor_t rgbColorIn = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined,double iPenSize = 0);           // $QCC #DrawFilledPolygon
    bool DrawFilledPolygon(Gdiplus::Point * pPoints,int iVertices,RGBColor_t rgbColorIn = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined,int iPenSize = 0);       // $QCC #DrawFilledPolygon
    bool DrawFilledPolygon(Gdiplus::PointF * pPoints,int iVertices,RGBColor_t rgbColorIn = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined,float iPenSize = 0);    // $QCC #DrawFilledPolygon
   
    bool FillPolygon(POINT * pPoints,int iVertices,RGBColor_t rgbColorIn = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined,int iPenSize = 0);                      // $QCC #DrawFilledPolygon
    bool FillPolygon(CPoint * pPoints,int iVertices,RGBColor_t rgbColorIn = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined,int iPenSize = 0);                     // $QCC #DrawFilledPolygon
    bool FillPolygon(CfPointf * pPoints,int iVertices,RGBColor_t rgbColorIn = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined,float iPenSize = 0);                 // $QCC #DrawFilledPolygon
    bool FillPolygon(CfPoint * pPoints,int iVertices,RGBColor_t rgbColorIn = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined,double iPenSize = 0);                 // $QCC #DrawFilledPolygon
    bool FillPolygon(Gdiplus::Point * pPoints,int iVertices,RGBColor_t rgbColorIn = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined,int iPenSize = 0);             // $QCC #DrawFilledPolygon
    bool FillPolygon(Gdiplus::PointF * pPoints,int iVertices,RGBColor_t rgbColorIn = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined,float iPenSize = 0);          // $QCC #DrawFilledPolygon
   
    /// #DrawPolygonFast
    /// <summary>
    /// Draws a 'fast' open/wireframe Polygon on the screen at starting point (x,y) with vertexes defined in pPoints<para></para>
    /// See FillPolygonFast() to draw a filled Polygon vs. an open/wireframe Polygon<para></para>
    /// Note: POINT arrays may also be used.  PenSize must be of same type as pair (i.e. int, float, double.)<para></para>
    /// --> This drawing function is a 'fast' function that does not use anti-aliasing and is about 10x faster than
    /// non-fast drawing functions.  These functions are useful for general drawing and do not respond to opacity or transformations.<para></para>
    /// </summary>
    /// <param name="pPoints">- POINT, cPoint, cPointf, or cfPointf array of vertices</param>
    /// <param name="iVertices">- Number of vertices in pPoints.</param>
    /// <param name="v3">- (x,y) vertex 3 of the polygon.</param>
    /// <param name="rgbColor">- Color of the polygon</param>
    /// <param name="iPenSize">- [optional] Pen size for the polygon color.  If the PenSize is not specified, the current default pen size is used.</param>
    /// <returns></returns>
    bool DrawPolygonFast(POINT * pPoints,int iVertices,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);                                                    // $QCC #DrawPolygonFast
    bool DrawPolygonFast(CPoint * pPoints,int iVertices,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);                                                   // $QCC #DrawPolygonFast
    bool DrawPolygonFast(CfPoint * pPoints,int iVertices,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);                                                  // $QCC #DrawPolygonFast

    bool DrawFilledPolygonFast(POINT * pPoints,int iVertices,RGBColor_t rgbColor = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined,int iPenSize = 0);      // $QCC #DrawPolygonFast
    bool DrawFilledPolygonFast(CPoint * pPoints,int iVertices,RGBColor_t rgbColor = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined,int iPenSize = 0);     // $QCC #DrawPolygonFast
    bool DrawFilledPolygonFast(CfPoint * pPoints,int iVertices,RGBColor_t rgbColor = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined,int iPenSize = 0);    // $QCC #DrawPolygonFast

    /// #FillPolygonFast
    /// <summary>
    /// Draws a 'fast' filled Polygon on the screen at starting point (x,y) with vertexes defined in pPoints<para></para>
    /// See DrawPolygonFast() to draw an open/wireframe Polygon vs. a filled Polygon<para></para>
    /// Note: POINT arrays may also be used.  PenSize must be of same type as pair (i.e. int, float, double.)<para></para>
    /// --> This drawing function is a 'fast' function that does not use anti-aliasing and is about 10x faster than
    /// non-fast drawing functions.  These functions are useful for general drawing and do not respond to opacity or transformations.<para></para>
    /// </summary>
    /// <param name="pPoints">- POINT, cPoint, cPointf, or cfPointf array of vertices</param>
    /// <param name="iVertices">- Number of vertices in pPoints.</param>
    /// <param name="v3">- (x,y) vertex 3 of the polygon.</param>
    /// <param name="rgbColorIn">- Color of the quadrangle's interior</param>
    /// <param name="rgbColorOut">- [optional] Color of the quadrangle's border</param>
    /// <param name="iPenSize">- [optional] Pen size for the border color if specified.  If the PenSize is not specified, the current default pen size is used.</param>
    /// <returns></returns>
    bool FillPolygonFast(POINT * pPoints,int iVertices,RGBColor_t rgbColor = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined,int iPenSize = 0);      // $QCC #FillPolygonFast
    bool FillPolygonFast(CPoint * pPoints,int iVertices,RGBColor_t rgbColor = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined,int iPenSize = 0);     // $QCC #FillPolygonFast
    bool FillPolygonFast(CfPoint * pPoints,int iVertices,RGBColor_t rgbColor = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined,int iPenSize = 0);    // $QCC #FillPolygonFast

    // Circle Drawing Functions

    /// #DrawCircle
    /// <summary>
    /// Draws an open/wireframe Circle on the screen at starting point (x,y) with a width and height of (radiusX, radiusY)<para></para>
    /// See FillCircle() to draw a filled Circle vs. an open/wireframe Circle<para></para>
    /// Note: x,y, width,height, and PenSize values must all be of same type (int,float,double)<para></para>
    /// Note: Point, cfPoint, and cfPointf pair/vector arrays may also be used.  PenSize must be of same type as pair (i.e. int, float, double.)<para></para>
    /// --> This drawing function responds to current opacity and transformations and will anti-alias output results.<para></para>
    /// --> This drawing function is high-precision where float and double values may be used for positioning and size.<para></para>
    /// --> Hint: use an odd-size PenSize for sharper edges.<para></para>
    /// </summary>
    /// <param name="iX">- Location of the X center of the circle in the window.  See various overloads for different types of X,Y input.</param>
    /// <param name="iY">- Location of the Y center of the circle in the window.</param>
    /// <param name="iRadius">- Radius (in pixels) of the circle.</param>
    /// <param name="rgbColor">- Color of the circle</param>
    /// <param name="iPenSize">- [optional] Pen size for the circle color.  If the PenSize is not specified, the current default pen size is used.</param>
    /// <returns></returns>
    bool DrawCircle(int iX,int iY,int iRadius,RgbColor rgbColor = Rgb::Default,int iPenSize =0);                  // $QCC  #DrawCircle
    bool DrawCircle(POINT pLoc,int iRadius,RgbColor rgbColor = Rgb::Default,int iPenSize =0 );                    // $QCC  #DrawCircle
    bool DrawCircle(double iX,double iY,double iRadius,RgbColor rgbColor = Rgb::Default,double fPenSize = 0);     // $QCC  #DrawCircle
    bool DrawCircle(float iX,float iY,float iRadius,RgbColor rgbColor = Rgb::Default,float fPenSize = 0);         // $QCC  #DrawCircle
    bool DrawCircle(CfPoint fLoc,double iRadius,RgbColor rgbColor = Rgb::Default,double fPenSize = 0);            // $QCC  #DrawCircle
    bool DrawCircle(CfPointf fLoc,float iRadius,RgbColor rgbColor = Rgb::Default,float fPenSize = 0);             // $QCC  #DrawCircle

    /// #FillCircle
    /// <summary>
    /// Draws a filled Circle on the screen at starting point (x,y) with a width and height of (radiusX, radiusY)<para></para>
    /// See DrawCircle() to draw an open/wireframe Circle vs. a filled Circle an open/wireframe Circle<para></para>
    /// Note: x,y, width,height, and PenSize values must all be of same type (int,float,double)<para></para>
    /// Note: POINT, cfPoint, and cfPointf pair/vector arrays may also be used.  PenSize must be of same type as pair (i.e. int, float, double.)<para></para>
    /// --> This drawing function responds to current opacity and transformations and will anti-alias output results.<para></para>
    /// --> This drawing function is high-precision where float and double values may be used for positioning and size.<para></para>
    /// --> Hint: use an odd-size PenSize for sharper edges.<para></para>
    /// </summary>
    /// <param name="iX">- Location of the X center of the circle in the window.  See various overloads for different types of X,Y input.</param>
    /// <param name="iY">- Location of the Y center of the circle in the window.</param>
    /// <param name="iRadius">- Radius (in pixels) of the circle.</param>
    /// <param name="rgbColorIn">- Color of the circle's interior</param>
    /// <param name="rgbColorOut">- [optional] Color of the circle's border</param>
    /// <param name="iPenSize">- [optional] Pen size for the border color if specified.  If the PenSize is not specified, the current default pen size is used.</param>
    /// <returns></returns>
    bool FillCircle(int iX,int iY,int iRadius,RgbColor rgbColorIn = Rgb::Default,RgbColor rgbColorOut = Rgb::Undefined,int iPenSize = 0);          // $QCC #FillCircle
    bool FillCircle(POINT pLoc,int iRadius,RgbColor rgbColorIn = Rgb::Default,RgbColor rgbColorOut = Rgb::Undefined,int iPenSize = 0);             // $QCC #FillCircle
    bool FillCircle(float iX,float iY,float iRadius,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,float fPenSize = 0);                 // $QCC #FillCircle  
    bool FillCircle(double iX,double iY,double iRadius,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,double fPenSize = 0);             // $QCC #FillCircle  
    bool FillCircle(CfPoint fLoc,double iRadius,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,double fPenSize = 0);                    // $QCC #FillCircle  
    bool FillCircle(CfPointf fLoc,float iRadius,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,float fPenSize = 0);                     // $QCC #FillCircle  

    /// #DrawFilledCircle
    /// <summary>
    /// Draws a filled Circle on the screen at starting point (x,y) with a width and height of (radiusX, radiusY)<para></para>
    /// See DrawCircle() to draw an open/wireframe Circle vs. a filled Circle an open/wireframe Circle<para></para>
    /// Note: x,y, width,height, and PenSize values must all be of same type (int,float,double)<para></para>
    /// Note: POINT, cfPoint, and cfPointf pair/vector arrays may also be used.  PenSize must be of same type as pair (i.e. int, float, double.)<para></para>
    /// --> This drawing function responds to current opacity and transformations and will anti-alias output results.<para></para>
    /// --> This drawing function is high-precision where float and double values may be used for positioning and size.<para></para>
    /// --> Hint: use an odd-size PenSize for sharper edges.<para></para>
    /// </summary>
    /// <param name="iX">- Location of the X center of the circle in the window.  See various overloads for different types of X,Y input.</param>
    /// <param name="iY">- Location of the Y center of the circle in the window.</param>
    /// <param name="iRadius">- Radius (in pixels) of the circle.</param>
    /// <param name="rgbColorIn">- Color of the circle's interior</param>
    /// <param name="rgbColorOut">- [optional] Color of the circle's border</param>
    /// <param name="iPenSize">- [optional] Pen size for the border color if specified.  If the PenSize is not specified, the current default pen size is used.</param>
    /// <returns></returns>
    bool DrawFilledCircle(int iX,int iY,int iRadius,RgbColor rgbColorIn = Rgb::Default,RgbColor rgbColorOut = Rgb::Undefined,int iPenSize = 0);              // $QCC  #DrawFilledCircle
    bool DrawFilledCircle(POINT pLoc,int iRadius,RgbColor rgbColorIn = Rgb::Default,RgbColor rgbColorOut = Rgb::Undefined,int iPenSize = 0);                 // $QCC  #DrawFilledCircle
    bool DrawFilledCircle(float iX,float iY,float iRadius,RgbColor rgbColorIn = Rgb::Default,RgbColor rgbColorOut = Rgb::Undefined,float iPenSize = 0);      // $QCC  #DrawFilledCircle  
    bool DrawFilledCircle(double iX,double iY,double iRadius,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,double fPenSize = 0);                 // $QCC  #DrawFilledCircle  
    bool DrawFilledCircle(CfPoint fLoc,double iRadius,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,double fPenSize = 0);                        // $QCC  #DrawFilledCircle  
    bool DrawFilledCircle(CfPointf fLoc,float iRadius,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,float fPenSize = 0);                         // $QCC  #DrawFilledCircle  

    /// #DrawFilledCircleFast
    /// <summary>
    /// Draws a 'fast' filled Circle on the screen at starting point (x,y) with a width and height of (radiusX, radiusY)<para></para>
    /// See DrawCircleFast() to draw an open/wireframe Circle vs. a filled Circle an open/wireframe Circle<para></para>
    /// Note: POINT arrays may also be used.  PenSize must be of same type as pair (i.e. int, float, double.)<para></para>
    /// --> This drawing function is a 'fast' function that does not use anti-aliasing and is about 10x faster than
    /// non-fast drawing functions.  These functions are useful for general drawing and do not respond to opacity or transformations.<para></para>
    /// </summary>
    /// <param name="iX">- Location of the X center of the circle in the window.  See various overloads for different types of X,Y input.</param>
    /// <param name="iY">- Location of the Y center of the circle in the window.</param>
    /// <param name="iRadius">- Radius (in pixels) of the circle.</param>
    /// <param name="rgbColorIn">- Color of the circle's interior</param>
    /// <param name="rgbColorOut">- [optional] Color of the circle's border</param>
    /// <param name="iPenSize">- [optional] Pen size for the border color if specified.  If the PenSize is not specified, the current default pen size is used.</param>
    /// <returns></returns>
    bool DrawFilledCircleFast(int iX,int iY,int iRadius,RgbColor rgbColorIn = Rgb::Default,RgbColor rgbColorOut = Rgb::Undefined,int iPenSize = 0);   // $QCC   #DrawFilledCircleFast
    bool DrawFilledCircleFast(POINT pLoc,int iRadius,RgbColor rgbColorIn = Rgb::Default,RgbColor rgbColorOut = Rgb::Undefined,int iPenSize = 0);      // $QCC   #DrawFilledCircleFast
   

    bool FillCircleFast(int iX,int iY,int iRadius,RgbColor rgbColorIn = Rgb::Default,RgbColor rgbColorOut = Rgb::Undefined,int iPenSize = 0);  // $QCC   #DrawFilledCircleFast
    bool FillCircleFast(POINT pLoc,int iRadius,RgbColor rgbColorIn = Rgb::Default,RgbColor rgbColorOut = Rgb::Undefined,int iPenSize = 0);     // $QCC   #DrawFilledCircleFast

    /// #DrawCircleFast
    /// <summary>
    /// Draws a 'fast' open/wireframe Circle on the screen at starting point (x,y) with a width and height of (radiusX, radiusY)<para></para>
    /// See DrawCircleFast() to draw an open/wireframe Circle vs. a filled Circle an open/wireframe Circle<para></para>
    /// Note: POINT arrays may also be used.  PenSize must be of same type as pair (i.e. int, float, double.)<para></para>
    /// --> This drawing function is a 'fast' function that does not use anti-aliasing and is about 10x faster than
    /// non-fast drawing functions.  These functions are useful for general drawing and do not respond to opacity or transformations.<para></para>
    /// </summary>
    /// <param name="iX">- Location of the X center of the circle in the window.  See various overloads for different types of X,Y input.</param>
    /// <param name="iY">- Location of the Y center of the circle in the window.</param>
    /// <param name="iRadius">- Radius (in pixels) of the circle.</param>
    /// <param name="rgbColor">- Color of the circle</param>
    /// <param name="iPenSize">- [optional] Pen size for the border color if specified.  If the PenSize is not specified, the current default pen size is used.</param>
    /// <returns></returns>
    bool DrawCircleFast(int iX,int iY,int iRadius,RgbColor rgbColor = Rgb::Default,int iPenSize =0);      // $QCC  #DrawCircleFast
    bool DrawCircleFast(POINT pLoc,int iRadius,RgbColor rgbColor = Rgb::Default,int iPenSize =0 );        // $QCC  #DrawCircleFast
   
    // Draw Ellipse Functions

    /// #DrawEllipse
    /// <summary>
    /// Draws an open/wireframe Ellipse on the screen at starting point (x,y) with a width and height of (radiusX, radiusY)<para></para>
    /// See FillEllipse() to draw a filled Ellipse vs. an open/wireframe Ellipse<para></para>
    /// Note: x,y, Radius/Size, and PenSize values must all be of same type (int,float,double)<para></para>
    /// Note: Point, cfPoint, and cfPointf pair/vector arrays may also be used.  PenSize must be of same type as pair (i.e. int, float, double.)<para></para>
    /// --> This drawing function responds to current opacity and transformations and will anti-alias output results.<para></para>
    /// --> This drawing function is high-precision where float and double values may be used for positioning and size.<para></para>
    /// --> Hint: use an odd-size PenSize for sharper edges.<para></para>
    /// </summary>
    /// <param name="iX">- Location of the X center of the ellipse in the window.  See various overloads for different types of X,Y input.</param>
    /// <param name="iY">- Location of the Y center of the ellipse in the window.</param>
    /// <param name="iRadiusX">- Width/Radius (in pixels) of the ellipse in the X dimension. See various overloads for different types of RadiusX,RadiusY input.</param>
    /// <param name="iRadiusY">- Radius (in pixels) of the ellipse in the Y dimension.</param>
    /// <param name="rgbColor">- Color of the ellipse</param>
    /// <param name="iPenSize">- [optional] Pen size for the ellipse color.  If the PenSize is not specified, the current default pen size is used.</param>
    /// <returns></returns>
    bool DrawEllipse(int iX,int iY,int iRadiusX,int iRadiusY,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);                       // $QCC #DrawEllipse
    bool DrawEllipse(POINT pLoc,int iRadiusX,int iRadiusY,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);                          // $QCC #DrawEllipse
    bool DrawEllipse(double iX,double iY,double iRadiusX,double iRadiusY,RGBColor_t rgbColor = Rgb::Default,double fPenSize = 0);        // $QCC #DrawEllipse   
    bool DrawEllipse(float iX,float iY,float iRadiusX,float iRadiusY,RGBColor_t rgbColor = Rgb::Default,float fPenSize = 0);             // $QCC #DrawEllipse   
    bool DrawEllipse(CfPoint fLoc,CfPoint fSize,RGBColor_t rgbColor = Rgb::Default,double fPenSize = 0);                                 // $QCC #DrawEllipse   
    bool DrawEllipse(CfPointf fLoc,CfPointf fSize,RGBColor_t rgbColor = Rgb::Default,float fPenSize = 0);                                // $QCC #DrawEllipse    

    /// #FillEllipse
    /// <summary>
    /// Draws a filled Ellipse on the screen at starting point (x,y) with a width and height of (radiusX, radiusY)<para></para>
    /// See Ellipse() to draw an open/wireframe Ellipse vs.a filled  Ellipse<para></para>
    /// Note: x,y, Radius/Size, and PenSize values must all be of same type (int,float,double)<para></para>
    /// Note: Point, cfPoint, and cfPointf pair/vector arrays may also be used.  PenSize must be of same type as pair (i.e. int, float, double.)<para></para>
    /// --> This drawing function responds to current opacity and transformations and will anti-alias output results.<para></para>
    /// --> This drawing function is high-precision where float and double values may be used for positioning and size.<para></para>
    /// --> Hint: use an odd-size PenSize for sharper edges.<para></para>
    /// </summary>
    /// <param name="iX">- Location of the X center of the ellipse in the window.  See various overloads for different types of X,Y input.</param>
    /// <param name="iY">- Location of the Y center of the ellipse in the window.</param>
    /// <param name="iRadiusX">- Width/Radius (in pixels) of the ellipse in the X dimension. See various overloads for different types of RadiusX,RadiusY input.</param>
    /// <param name="iRadiusY">- Radius (in pixels) of the ellipse in the Y dimension.</param>
    /// <param name="rgbColorIn">- Color of the interior of the ellipse</param>
    /// <param name="rgbColorIn">- [optional[ Color of the border of the ellipse</param>
    /// <param name="iPenSize">- [optional] Pen size for the ellipse border.  If the PenSize is not specified, the current default pen size is used.</param>
    /// <returns></returns>
    bool FillEllipse(int iX,int iY,int iRadiusX,int iRadiusY,RGBColor_t rgbColorIn = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined,int iPenSize = 0); // $QCC #FillEllipse
    bool FillEllipse(POINT pLoc,int iRadiusX,int iRadiusY,RGBColor_t rgbColorIn = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined,int iPenSize = 0);    // $QCC #FillEllipse
    bool FillEllipse(float iX,float iY,float iRadiusX,float iRadiusY,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,float fPenSize = 0);          // $QCC #FillEllipse  
    bool FillEllipse(double iX,double iY,double iRadiusX,double iRadiusY,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,double fPenSize = 0);     // $QCC #FillEllipse  
    bool FillEllipse(CfPoint fLoc,CfPoint fSize,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,double fPenSize = 0);                              // $QCC #FillEllipse  
    bool FillEllipse(CfPointf fLoc,CfPointf fSize,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,float fPenSize = 0);                             // $QCC #FillEllipse  

    bool DrawFilledEllipse(int iX,int iY,int iRadiusX,int iRadiusY,RGBColor_t rgbColorIn = Rgb::Default,RGBColor_t rgbColorOut = Rgb::None,int iPenSize = 0);       // $QCC #FillEllipse
    bool DrawFilledEllipse(POINT pLoc,int iRadiusX,int iRadiusY,RGBColor_t rgbColorIn = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined,int iPenSize = 0);     // $QCC #FillEllipse
    bool DrawFilledEllipse(float iX,float iY,float iRadiusX,float iRadiusY,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,float fPenSize = 0);           // $QCC #FillEllipse   
    bool DrawFilledEllipse(double iX,double iY,double iRadiusX,double iRadiusY,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,double fPenSize = 0);      // $QCC #FillEllipse   
    bool DrawFilledEllipse(CfPoint fLoc,CfPoint fSize,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,double fPenSize = 0);                               // $QCC #FillEllipse   
    bool DrawFilledEllipse(CfPointf fLoc,CfPointf fSize,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,float fPenSize = 0);                              // $QCC #FillEllipse   

    /// #DrawEllipseFast
    /// <summary>
    /// Draws a 'fast' open/wireframe Ellipse on the screen at starting point (x,y) with a width and height of (radiusX, radiusY)<para></para>
    /// See FillEllipseFast() to draw a filled Ellipse vs. an open/wireframe Ellipse<para></para>
    /// Note: x,y, Radius/Size, and PenSize values must all be of same type (int,float,double)<para></para>
    /// Note: POINT arrays may also be used.  PenSize must be of same type as pair (i.e. int, float, double.)<para></para>
    /// --> This drawing function is a 'fast' function that does not use anti-aliasing and is about 10x faster than
    /// non-fast drawing functions.  These functions are useful for general drawing and do not respond to opacity or transformations.<para></para>
    /// </summary>
    /// <param name="iX">- Location of the X center of the ellipse in the window.  See various overloads for different types of X,Y input.</param>
    /// <param name="iY">- Location of the Y center of the ellipse in the window.</param>
    /// <param name="iRadiusX">- Width/Radius (in pixels) of the ellipse in the X dimension. See various overloads for different types of RadiusX,RadiusY input.</param>
    /// <param name="iRadiusY">- Radius (in pixels) of the ellipse in the Y dimension.</param>
    /// <param name="rgbColor">- Color of the ellipse</param>
    /// <param name="iPenSize">- [optional] Pen size for the ellipse color.  If the PenSize is not specified, the current default pen size is used.</param>
    /// <returns></returns>
    bool DrawEllipseFast(int iX,int iY,int iRadiusX,int iRadiusY,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);      // $QCC #DrawEllipseFast
    bool DrawEllipseFast(POINT pLoc,int iRadiusX,int iRadiusY,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);         // $QCC #DrawEllipseFast

    /// #FillEllipseFast
    /// <summary>
    /// Draws a 'fast' filled Ellipse on the screen at starting point (x,y) with a width and height of (radiusX, radiusY)<para></para>
    /// See Ellipse() to draw an open/wireframe Ellipse vs.a filled  Ellipse<para></para>
    /// Note: x,y, Radius/Size, and PenSize values must all be of same type (int,float,double)<para></para>
    /// Note: POINT arrays may also be used.  PenSize must be of same type as pair (i.e. int, float, double.)<para></para>
    /// --> This drawing function is a 'fast' function that does not use anti-aliasing and is about 10x faster than
    /// non-fast drawing functions.  These functions are useful for general drawing and do not respond to opacity or transformations.<para></para>
    /// </summary>
    /// <param name="iX">- Location of the X center of the ellipse in the window.  See various overloads for different types of X,Y input.</param>
    /// <param name="iY">- Location of the Y center of the ellipse in the window.</param>
    /// <param name="iRadiusX">- Width/Radius (in pixels) of the ellipse in the X dimension. See various overloads for different types of RadiusX,RadiusY input.</param>
    /// <param name="iRadiusY">- Radius (in pixels) of the ellipse in the Y dimension.</param>
    /// <param name="rgbColorIn">- Color of the interior of the ellipse</param>
    /// <param name="rgbColorIn">- [optional[ Color of the border of the ellipse</param>
    /// <param name="iPenSize">- [optional] Pen size for the ellipse border.  If the PenSize is not specified, the current default pen size is used.</param>
    /// <returns></returns>
    bool FillEllipseFast(int iX,int iY,int iRadiusX,int iRadiusY,RGBColor_t rgbColorIn = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined,int iPenSize = 0);        // $QCC #FillEllipseFast
    bool FillEllipseFast(POINT pLoc,int iRadiusX,int iRadiusY,RGBColor_t rgbColorIn = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined,int iPenSize = 0);           // $QCC #FillEllipseFast
   
    bool DrawFilledEllipseFast(int iX,int iY,int iRadiusX,int iRadiusY,RGBColor_t rgbColorIn = Rgb::Default,RGBColor_t rgbColorOut = Rgb::None,int iPenSize = 0);       // $QCC #FillEllipseFast
    bool DrawFilledEllipseFast(POINT pLoc,int iRadiusX,int iRadiusY,RGBColor_t rgbColorIn = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined,int iPenSize = 0);     // $QCC #FillEllipseFast
     
    
    bool DrawGradient(int ix,int iy,int iWidth,int iHeight,RGBColor_t rgbColor1,RGBColor_t rgbColor2,bool bHorizontal = false); // $QCC
    bool DrawGradient(POINT pLoc,SIZE szSize,RGBColor_t rgbColor1,RGBColor_t rgbColor2,bool bHorizontal = false);               // $QCC
    bool DrawGradient(RECT rGradientRect,RGBColor_t rgbColor1,RGBColor_t rgbColor2,bool bHorizontal = false);                   // $QCC


    // Ractangle2() -- Used for testing 
    //
    bool Rectangle2(int ix,int iy,int iWidth,int iHeight,int iColor,int iColor2 = -1);        // $QCC  

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
    bool SetPenColor(int iColor);  // $QC

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
    bool SetPenColor(const char * sColor);  // $QC

    // Set the DrawLine color for new lines using SetLinePos() (or MoveTo()) and then DrawLine() (or LineTo())
    // This allows a default line color to be selected so that functions later drawing lines with SetLinePos()/DrawLine()
    // don't need to set the color. 
    //
    // note: This does not work with DrawLine().  When using DrawLine(), the color must be specifieed.
    //
    bool SetPenColor(RGBColor_t rgbColor);                                                                  // $QC

    bool MoveTo(int iX,int iY,RGBColor_t rgbColor = Rgb::Undefined);                                        // $QC
    bool MoveTo(POINT pLoc,RGBColor_t rgbColor = Rgb::Undefined);                                           // $QC

    bool MoveTo(float iX,float iY,RGBColor_t rgbColor = Rgb::Undefined);                                    // $QC
    bool MoveTo(double iX,double iY,RGBColor_t rgbColor = Rgb::Undefined);                                  // $QC
    bool MoveTo(CfPointf pLoc,RGBColor_t rgbColor = Rgb::Undefined);                                        // $QC
    bool MoveTo(CfPoint pLoc,RGBColor_t rgbColor = Rgb::Undefined);                                         // $QC

    bool DrawLineToFast(int iX,int iY,RGBColor_t rgbColor  = Rgb::Undefined,int iPenSize = 0);              // $QCC
    bool DrawLineToFast(POINT pLoc,RGBColor_t rgbColor  = Rgb::Undefined,int iPenSize =0);                  // $QCC
  
    bool DrawLineTo(POINT p1,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);                          // $QCC
    bool DrawLineTo(int ix1,int iy1,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);                   // $QCC
    bool DrawLineTo(float ix1,float iy1,RGBColor_t rgbColor = Rgb::Default,float iPenSize = 0);             // $QCC
    bool DrawLineTo(double ix1,double iy1,RGBColor_t rgbColor = Rgb::Default,double iPenSize = 0);          // $QCC
    bool DrawLineTo(const CfPointf p1,RGBColor_t rgbColor = Rgb::Default,float iPenSize = 0);               // $QCC
    bool DrawLineTo(const CfPoint p1,RGBColor_t rgbColor = Rgb::Default,double iPenSize = 0);               // $QCC


    bool DrawLineToExFast(bool bFirstPoint,int iX,int iY,RGBColor_t rgbColor);                                          // $QCC
    bool DrawLineToExFast(bool bFirstPoint,POINT pLoc,RGBColor_t rgbColor);                                             // $QCC

    bool DrawLineToEx(bool bFirstPoint,POINT p1,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);                   // $QCC
    bool DrawLineToEx(bool bFirstPoint,int ix1,int iy1,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);            // $QCC
    bool DrawLineToEx(bool bFirstPoint,float ix1,float iy1,RGBColor_t rgbColor = Rgb::Default,float iPenSize = 0);      // $QCC
    bool DrawLineToEx(bool bFirstPoint,double ix1,double iy1,RGBColor_t rgbColor = Rgb::Default,double iPenSize = 0);   // $QCC
    bool DrawLineToEx(bool bFirstPoint,const CfPointf p1,RGBColor_t rgbColor = Rgb::Default,float iPenSize = 0);        // $QCC
    bool DrawLineToEx(bool bFirstPoint,const CfPoint p1,RGBColor_t rgbColor = Rgb::Default,double iPenSize = 0);        // $QCC


    bool DrawLineFast(POINT p1,POINT p2,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);                               // $QCC
    bool DrawLineFast(int ix1,int iy1,int ix2,int iy2,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);                 // $QCC

    bool DrawLine(POINT p1,POINT p2,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);                                   // $QCC
    bool DrawLine(int ix1,int iy1,int ix2,int iy2,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);                     // $QCC
    bool DrawLine(float ix1,float iy1,float ix2,float iy2,RGBColor_t rgbColor = Rgb::Default,float iPenSize = 0);           // $QCC
    bool DrawLine(double ix1,double iy1,double ix2,double iy2,RGBColor_t rgbColor = Rgb::Default,double iPenSize = 0);      // $QCC
    bool DrawLine(const CfPointf p1,const CfPointf p2,RGBColor_t rgbColor = Rgb::Default,float iPenSize = 0);               // $QCC
    bool DrawLine(const CfPoint p1,const CfPoint p2,RGBColor_t rgbColor = Rgb::Default,double iPenSize = 0);                // $QCC


    // Draw a line in the window.  
    // This draws a line from (ix1,iy1) to (ix2,iy2) in the given color
    //
    // The color can be RGB() or RGBColor_t
    // The thickness of the line can be changed with SetPenThickness(), which defaults to 1.
    //
    bool DrawLine2(int ix1,int iy1,int iWidth,int iHeight,RGBColor_t rgbColor = Rgb::Default);       // $QCC
  

    // Draw a line in the window.  
    // This draws a line from (ix1,iy1) to (ix2,iy2) in the given color
    //
    // The color can be RGB() or RGBColor_t
    // The thickness of the line can be changed with SetPenThickness(), which defaults to 1.
    //
    bool DrawLine2(POINT p1,SIZE szDist,RGBColor_t rgbColor = Rgb::Default);         // $QCC

    // SetPenThickness -- Sets the thickness of the 'pen' (i.e. draw line thickness or outline thickness on Cricles, Triangles, etc.)
    //
    // This value defaults to 1, but can be set prior to drawing any control that uses an outline or draws a line.
    // SetPenThickness() must be used again to revert back to the original thickness.  
    // 
    // SetPenThickness() returns the new thickness of the pen -- this useful to ensure the value does not 
    // go below the minimum of 1.
    //
    int SetPenThickness(int iThickness); // $QC

    // GetPenThickness() -- Returns the size (i.e. thickness) of the current pen)
    //
    int GetPenThickness(); // $QC

    // SetPenSize -- Sets the thickness of the 'pen' (i.e. draw line thickness or outline thickness on Cricles, Triangles, etc.)
    //
    // This value defaults to 1, but can be set prior to drawing any control that uses an outline or draws a line.
    // SetPenThickness() must be used again to revert back to the original thickness.  
    // 
    // SetPenThickness() returns the new thickness of the pen -- this useful to ensure the value does not 
    // go below the minimum of 1.
    //
    int SetPenSize(int iThickness); // $QC

    /// <summary>
    /// Sets the opacity for shapes drawn with GDI functions, such as Circle(), FillCircle(), Triangle, etc.
    /// </summary>
    /// <param name="iOpacity">- Opacity value (0 = transparent, 100 (or 255 is bAsByteValue is true) is completely opaque.  Default is fully opaque</param>
    /// <param name="bAsByteValue">When false, the Opacity value is used as a percentage.  When true, Opacity value is used as a BYTE value 0-255 to set the value directly.</param>
    /// <returns></returns>
    bool SetDrawOpacity(int iOpacity,bool bAsByteValue = false);                // $QC

    /// <summary>
    /// Sets the opacity for shapes drawn with GDI functions, such as Circle(), FillCircle(), Triangle, etc.
    /// </summary>
    /// <param name="iOpacity">- Opacity value (0 = transparent, 100 (or 255 is bAsByteValue is true) is completely opaque.  Default is fully opaque</param>
    /// <param name="bAsByteValue">When false, the Opacity value is used as a percentage.  When true, Opacity value is used as a BYTE value 0-255 to set the value directly.</param>
    /// <returns></returns>
    bool SetDrawOpacity(float fOpacity,bool bAsByteValue = false);              // $QC

    /// <summary>
    /// Sets the opacity for shapes drawn with GDI functions, such as Circle(), FillCircle(), Triangle, etc.
    /// </summary>
    /// <param name="iOpacity">- Opacity value (0 = transparent, 100 (or 255 is bAsByteValue is true) is completely opaque.  Default is fully opaque</param>
    /// <param name="bAsByteValue">When false, the Opacity value is used as a percentage.  When true, Opacity value is used as a BYTE value 0-255 to set the value directly.</param>
    /// <returns></returns>
    bool SetDrawOpacity(double fOpacity,bool bAsByteValue = false);             // $QC

    /// <summary>
    /// Returns the current Opacity for GDI-based draw functions.  Value is from 0-255 (fully transparent to fully opaque)
    /// </summary>
    /// <returns></returns>
    int GetDrawOpacity();

    /// <summary>
    /// Sets the rotational transform for objects drawn with with GDI functions, such as Circle(), FillCircle(), Triangle, etc.
    /// <para>
    /// When used, the object will be rotated by --iAngle-- degrees. 
    /// </para><para>--> Use DrawResetTransform when finished with transformations
    /// </para><para>--> Transformations are processed in order called.  calling DrawRotateTransform() then DrawTranslateTransform() vs. 
    /// </para><para>DrawTranslateTransform() then DrawRotateTransform() will result in different behavior. 
    /// </para><para>--> Typical order is DrawTranslateTransform() to set the center of the transform, then DrawRotateTransform() to rotate on that center.
    /// </para>
    /// </summary>
    /// <param name="iAngle">Angle to rotate in degrees</param>
    /// <returns></returns>
    bool DrawRotateTransform(int iAngle);                                       // $QC
 
    /// <summary>
    /// Sets the rotational transform for objects drawn with with GDI functions, such as Circle(), FillCircle(), Triangle, etc.
    /// <para>
    /// When used, the object will be rotated by --iAngle-- degrees. 
    /// </para><para>--> Use DrawResetTransform when finished with transformations
    /// </para><para>--> Transformations are processed in order called.  calling DrawRotateTransform() then DrawTranslateTransform() vs. 
    /// </para><para>DrawTranslateTransform() then DrawRotateTransform() will result in different behavior. 
    /// </para><para>--> Typical order is DrawTranslateTransform() to set the center of the transform, then DrawRotateTransform() to rotate on that center.
    /// </para>
    /// </summary>
    /// <param name="iAngle">Angle to rotate in degrees</param>
    /// <returns></returns>
    bool DrawRotateTransform(double fAngle);                                    // $QC

    /// <summary>
    /// Sets the rotational transform for objects drawn with with GDI functions, such as Circle(), FillCircle(), Triangle, etc.
    /// <para>
    /// When used, the object will be rotated by --iAngle-- degrees. 
    /// </para><para>--> Use DrawResetTransform when finished with transformations
    /// </para><para>--> Transformations are processed in order called.  calling DrawRotateTransform() then DrawTranslateTransform() vs. 
    /// </para><para>DrawTranslateTransform() then DrawRotateTransform() will result in different behavior. 
    /// </para><para>--> Typical order is DrawTranslateTransform() to set the center of the transform, then DrawRotateTransform() to rotate on that center.
    /// </para>
    /// </summary>
    /// <param name="iAngle">Angle to rotate in degrees</param>
    /// <returns></returns>
    bool DrawRotateTransform(float iAngle);                                     // $QC

    /// <summary>
    /// Resets all transforms for GDI-based object (i.e. objects drawn with with GDI functions, such as Circle(), FillCircle(), Triangle, etc.)
    /// </summary>
    /// <returns></returns>
    bool DrawResetTransform();                                                  // $QC

    /// <summary>
    /// Sets the positional transform for objects drawn with with GDI functions, such as Circle(), FillCircle(), Triangle, etc.
    /// <para>
    /// When used, the object drawn at (0,0) will now be drawn at (X,y) and all coordinates used will transform (0,0) into X and Y
    /// </para><para>--> Use DrawResetTransform when finished with transformations
    /// </para><para>--> Transformations are processed in order called.  calling DrawRotateTransform() then DrawTranslateTransform() vs. 
    /// </para><para>DrawTranslateTransform() then DrawRotateTransform() will result in different behavior. 
    /// </para><para>--> Typical order is DrawTranslateTransform() to set the center of the transform, then DrawRotateTransform() to rotate on that center.
    /// </para>
    /// </summary>
    /// <param name="iAngle">Angle to rotate in degrees</param>
    /// <returns></returns>    
    bool DrawTranslateTransform(int iX,int iY);                                 // $QC  
    
    /// <summary>
    /// Sets the positional transform for objects drawn with with GDI functions, such as Circle(), FillCircle(), Triangle, etc.
    /// <para>
    /// When used, the object drawn at (0,0) will now be drawn at (X,y) and all coordinates used will transform (0,0) into X and Y
    /// </para><para>--> Use DrawResetTransform when finished with transformations
    /// </para><para>--> Transformations are processed in order called.  calling DrawRotateTransform() then DrawTranslateTransform() vs. 
    /// </para><para>DrawTranslateTransform() then DrawRotateTransform() will result in different behavior. 
    /// </para><para>--> Typical order is DrawTranslateTransform() to set the center of the transform, then DrawRotateTransform() to rotate on that center.
    /// </para>
    /// </summary>
    /// <param name="iAngle">Angle to rotate in degrees</param>
    /// <returns></returns>    
    bool DrawTranslateTransform(float iX,float iY);                             // $QC
 
    /// <summary>
    /// Sets the positional transform for objects drawn with with GDI functions, such as Circle(), FillCircle(), Triangle, etc.
    /// <para>
    /// When used, the object drawn at (0,0) will now be drawn at (X,y) and all coordinates used will transform (0,0) into X and Y
    /// </para><para>--> Use DrawResetTransform when finished with transformations
    /// </para><para>--> Transformations are processed in order called.  calling DrawRotateTransform() then DrawTranslateTransform() vs. 
    /// </para><para>DrawTranslateTransform() then DrawRotateTransform() will result in different behavior. 
    /// </para><para>--> Typical order is DrawTranslateTransform() to set the center of the transform, then DrawRotateTransform() to rotate on that center.
    /// </para>
    /// </summary>
    /// <param name="iAngle">Angle to rotate in degrees</param>
    /// <returns></returns>
    bool DrawTranslateTransform(double iX,double iY);                           // $QC

    /// <summary>
    /// Sets the positional transform for objects drawn with with GDI functions, such as Circle(), FillCircle(), Triangle, etc.
    /// <para>
    /// When used, the object drawn at (0,0) will now be drawn at (X,y) and all coordinates used will transform (0,0) into X and Y
    /// </para><para>--> Use DrawResetTransform when finished with transformations
    /// </para><para>--> Transformations are processed in order called.  calling DrawRotateTransform() then DrawTranslateTransform() vs. 
    /// </para><para>DrawTranslateTransform() then DrawRotateTransform() will result in different behavior. 
    /// </para><para>--> Typical order is DrawTranslateTransform() to set the center of the transform, then DrawRotateTransform() to rotate on that center.
    /// </para>
    /// </summary>
    /// <param name="iAngle">Angle to rotate in degrees</param>
    /// <returns></returns>
    bool DrawTranslateTransform(POINT pLoc);                                    // $QC

    /// <summary>
    /// Sets the positional transform for objects drawn with with GDI functions, such as Circle(), FillCircle(), Triangle, etc.
    /// <para>
    /// When used, the object drawn at (0,0) will now be drawn at (X,y) and all coordinates used will transform (0,0) into X and Y
    /// </para><para>--> Use DrawResetTransform when finished with transformations
    /// </para><para>--> Transformations are processed in order called.  calling DrawRotateTransform() then DrawTranslateTransform() vs. 
    /// </para><para>DrawTranslateTransform() then DrawRotateTransform() will result in different behavior. 
    /// </para><para>--> Typical order is DrawTranslateTransform() to set the center of the transform, then DrawRotateTransform() to rotate on that center.
    /// </para>
    /// </summary>
    /// <param name="iAngle">Angle to rotate in degrees</param>
    /// <returns></returns>
    bool DrawTranslateTransform(CfPointf pLoc);                                 // $QC

    /// <summary>
    /// Sets the positional transform for objects drawn with with GDI functions, such as Circle(), FillCircle(), Triangle, etc.
    /// <para>
    /// When used, the object drawn at (0,0) will now be drawn at (X,y) and all coordinates used will transform (0,0) into X and Y
    /// </para><para>--> Use DrawResetTransform when finished with transformations
    /// </para><para>--> Transformations are processed in order called.  calling DrawRotateTransform() then DrawTranslateTransform() vs. 
    /// </para><para>DrawTranslateTransform() then DrawRotateTransform() will result in different behavior. 
    /// </para><para>--> Typical order is DrawTranslateTransform() to set the center of the transform, then DrawRotateTransform() to rotate on that center.
    /// </para>
    /// </summary>
    /// <param name="iAngle">Angle to rotate in degrees</param>
    /// <returns></returns>
    bool DrawTranslateTransform(CfPoint pLoc);                                  // $QC


    // GetPenSize() -- Returns the size (i.e. thickness) of the current pen)
    //
    int GetPenSize(); // $QC

    // DrawPixel() draw a pixel on the screen. 
    //
    // This sets a pixel on the screen and can be useful in prototyping and general drawing.
    // However, note that DrawPixel() used when drawing an entire array (i.e. a bitmap or an image)
    // is very slow! 
    //
    // For images, bitmaps, collections, etc. it is much faster to build a bitmap in memory
    // and then call DrawBitmap() to display multiple pixels at a time.
    //
    bool DrawPixel(int iX,int iY,DWORD dwColor);        // $QCC
 
    // DrawPixel() draw a pixel on the screen. 
    //
    // This sets a pixel on the screen and can be useful in prototyping and general drawing.
    // However, note that DrawPixel() used when drawing an entire array (i.e. a bitmap or an image)
    // is very slow! 
    //
    // For images, bitmaps, collections, etc. it is much faster to build a bitmap in memory
    // and then call DrawBitmap() to display multiple pixels at a time.
    //
    bool DrawPixel(POINT pPoint,DWORD dwColor);          // $QCC
    
    // DrawPixel() draw a pixel on the screen. 
    //
    // This sets a pixel on the screen and can be useful in prototyping and general drawing.
    // However, note that DrawPixel() used when drawing an entire array (i.e. a bitmap or an image)
    // is very slow! 
    //
    // For images, bitmaps, collections, etc. it is much faster to build a bitmap in memory
    // and then call DrawBitmap() to display multiple pixels at a time.
    //
    bool DrawPixel(POINT pPoint,RGBColor_t rgbColor);    // $QCC

    // DrawPixel() draw a pixel on the screen. 
    //
    // This sets a pixel on the screen and can be useful in prototyping and general drawing.
    // However, note that DrawPixel() used when drawing an entire array (i.e. a bitmap or an image)
    // is very slow! 
    //
    // For images, bitmaps, collections, etc. it is much faster to build a bitmap in memory
    // and then call DrawBitmap() to display multiple pixels at a time.
    //
    bool DrawPixel(int iX,int iY,RGBColor_t rgbColor);            // $QCC
 
                                                                     // DrawPixel() draw a piel on the screen. 
    // Show() -- Show the window (i.e. make it visible)
    //
    // If the window is not showing on the screen, Show() will make it visible.
    //
    // Show() and Show(true) will show a hidden window
    // Show(false) will Hide() the window
    //
    bool Show(bool bShow = true);   // $QCS

    // Hide() -- Hide the window (i.e. remove it from the screen without closing it or destroying it)
    //
    // If the window is on the screen Hide() will remove it from the display but not otherwise affect it.
    // 
    // Hide() and Hide(true) will hide the window
    // Hide(false) will Show() the window
    //
    bool Hide(bool bHide = true);   // $QCS

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
    CSlider &  NewSlider(CSlider * cSlider,int iX,int iY,int iSize,const wchar_t * sLabel = nullptr,const cwfOpt & cwOpt = cwfOpt());   // $QC

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
    CSlider &  NewSlider(CSlider * cSlider,int iX,int iY,int iSize,const char * sLabel = nullptr,const cwfOpt & cwOpt = cwfOpt()); // $QC

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
    CSlider &  NewSlider(CSlider * cSlider,int iX,int iY,int iSize,const cwfOpt & cwOpt = cwfOpt()); // $QC

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
    CSlider &  NewSlider(int iX,int iY,int iSize,const char * sLabel,const cwfOpt & cwOpt = cwfOpt()); // $QC

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
    CSlider &  NewSlider(int iX,int iY,int iSize,const wchar_t * sLabel,const cwfOpt & cwOpt = cwfOpt()); // $QC

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
    CSlider &  NewSlider(int iX,int iY,int iSize,const cwfOpt & cwOpt = cwfOpt()); // $QC

    // Float Slider Variatons of NewSlider 

    /// #NewfSlider
    /// <summary>
    /// Creates a float-point slider.  This is the same as NewSlider() except that the values are floating point, with a default range of 0.0-1.0.<para></para>
    /// Use Slider functions GetPosf(), SetPosf() to get/set values, and SetRangef() to change the range.<para></para>
    /// .<para></para>
    /// With options, use opt::SetRangef() to set floating point ranges. <para></para>
    /// --> Note: You can also use the NewSlider() function with the opt::AsFloat() option to set it as a floating points slider.<para></para>
    /// </summary>
    /// <param name="iX">- X Location of Slider</param>
    /// <param name="iY">- Y Location of Slider</param>
    /// <param name="iSize">Width of Slider (use 0 for default value)</param>
    /// <param name="sLabel">- [optional] label of slider</param>
    /// <param name="cwOpt">- [optional] other options such as Title(), Rangef(), Default(), etc.</param>
    /// <returns></returns>
    CSlider &  NewSliderf(int iX,int iY,int iSize,const char * sLabel,const cwfOpt & cwOpt = cwfOpt());         // $QC #NewfSlider
    CSlider &  NewSliderf(int iX,int iY,int iSize,const wchar_t * sLabel,const cwfOpt & cwOpt = cwfOpt());      // $QC #NewfSlider
    CSlider &  NewSliderf(int iX,int iY,int iSize,const cwfOpt & cwOpt = cwfOpt());                             // $QC #NewfSlider
                                                                                        
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
    void QuickButton(const char * sText = nullptr,const char * sTitleBar = nullptr); // $QC


    struct WinDraw
    {
    private:
        friend CWindow;
        CWindow * m_cWin                    = nullptr;
        CPasWindow * m_cWinCore             = nullptr;
    public:

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


        // Circle Functions

        bool FillCircle(int iX,int iY,int iRadius,RGBColor_t rgbColorIn = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined,int iPenSize = 0);    
        bool FillCircle(POINT pLoc,int iRadius,RGBColor_t rgbColorIn = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined,int iPenSize = 0);       
        bool FillCircle(float iX,float iY,float iRadius,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,float fPenSize = 0);         
        bool FillCircle(double iX,double iY,double iRadius,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,double fPenSize = 0);     
        bool FillCircle(CfPoint fLoc,double iRadius,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,double fPenSize = 0);            
        bool FillCircle(CfPointf fLoc,float iRadius,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,float fPenSize = 0);                   
        
        bool FillCircleFast(int iX,int iY,int iRadius,RGBColor_t rgbColorIn = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined,int iPenSize = 0);
        bool FillCircleFast(POINT pLoc,int iRadius,RGBColor_t rgbColorIn = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined,int iPenSize = 0);   

        bool DrawCircle(int iX,int iY,int iRadius,RGBColor_t rgbColor = Rgb::Default,int iPenSize =0);                                               
        bool DrawCircle(POINT pLoc,int iRadius,RGBColor_t rgbColor = Rgb::Default,int iPenSize =0 );                                                 
        bool DrawCircle(double iX,double iY,double iRadius,RGBColor_t rgbColor = Rgb::Default,double fPenSize = 0);                                  
        bool DrawCircle(float iX,float iY,float iRadius,RGBColor_t rgbColor = Rgb::Default,float fPenSize = 0);                                      
        bool DrawCircle(CfPoint fLoc,double iRadius,RGBColor_t rgbColor = Rgb::Default,double fPenSize = 0);                                         
        bool DrawCircle(CfPointf fLoc,float iRadius,RGBColor_t rgbColor = Rgb::Default,float fPenSize = 0);                                          


        bool DrawCircleFast(int iX,int iY,int iRadius,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);      
        bool DrawCircleFast(POINT pLoc,int iRadius,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);        

        // Ellipse Functions
 
        bool FillEllipse(int iX,int iY,int iRadiusX,int iRadiusY,RGBColor_t rgbColorIn = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined,int iPenSize = 0);     
        bool FillEllipse(POINT pLoc,int iRadiusX,int iRadiusY,RGBColor_t rgbColorIn = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined,int iPenSize = 0);        
        bool FillEllipse(float iX,float iY,float iRadiusX,float iRadiusY,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,float fPenSize = 0);              
        bool FillEllipse(double iX,double iY,double iRadiusX,double iRadiusY,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,double fPenSize = 0);         
        bool FillEllipse(CfPoint fLoc,CfPoint fSize,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,double fPenSize = 0);                                  
        bool FillEllipse(CfPointf fLoc,CfPointf fSize,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,float fPenSize = 0);                                 

        bool FillEllipseFast(int iX,int iY,int iRadiusX,int iRadiusY,RGBColor_t rgbColorIn = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined,int iPenSize = 0); 
        bool FillEllipseFast(POINT pLoc,int iRadiusX,int iRadiusY,RGBColor_t rgbColorIn = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined,int iPenSize = 0); 
    
        bool DrawEllipse(int iX,int iY,int iRadiusX,int iRadiusY,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);                                               
        bool DrawEllipse(POINT pLoc,int iRadiusX,int iRadiusY,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);                                                  
        bool DrawEllipse(double iX,double iY,double iRadiusX,double iRadiusY,RGBColor_t rgbColor = Rgb::Default,double fPenSize = 0);                                
        bool DrawEllipse(float iX,float iY,float iRadiusX,float iRadiusY,RGBColor_t rgbColor = Rgb::Default,float fPenSize = 0);                                     
        bool DrawEllipse(CfPoint fLoc,CfPoint fSize,RGBColor_t rgbColor = Rgb::Default,double fPenSize = 0);                                                         
        bool DrawEllipse(CfPointf fLoc,CfPointf fSize,RGBColor_t rgbColor = Rgb::Default,float fPenSize = 0);                                                        


        bool DrawEllipseFast(int iX,int iY,int iRadiusX,int iRadiusY,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0); 
        bool DrawEllipseFast(POINT pLoc,int iRadiusX,int iRadiusY,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0); 

        // Rectangle Functions

        bool DrawRectangle(int iX,int iY,int iWidth,int iHeight,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);                                       
        bool DrawRectangle(POINT pLoc,SIZE szSize,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);                                                     
        bool DrawRectangle(float iX,float iY,float iWidth,float iHeight,RGBColor_t rgbColor = Rgb::Default,float fPenSize = 0);                             
        bool DrawRectangle(double iX,double iY,double iWidth,double iHeight,RGBColor_t rgbColor = Rgb::Default,double fPenSize = 0);                        
        bool DrawRectangle(CfPoint fLoc,CfPoint fSize,RGBColor_t rgbColor = Rgb::Default,double fPenSize = 0);                                              
        bool DrawRectangle(CfPointf fLoc,CfPointf fSize,RGBColor_t rgbColor = Rgb::Default,float fPenSize = 0);                                             

        bool FillRectangle(int iX,int iY,int iWidth,int iHeight,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,int iPenSize = 0);                
        bool FillRectangle(POINT pLoc,SIZE szSize,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,int iPenSize = 0);                              
        bool FillRectangle(float iX,float iY,float iWidth,float iHeight,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,float fPenSize = 0);      
        bool FillRectangle(double iX,double iY,double iWidth,double iHeight,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,double fPenSize = 0); 
        bool FillRectangle(CfPoint fLoc,CfPoint fSize,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,double fPenSize = 0);                       
        bool FillRectangle(CfPointf fLoc,CfPointf fSize,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,float fPenSize = 0);                      

        bool FillRectangleFast(int iX,int iY,int iWidth,int iHeight,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,int iPenSize = 0);            
        bool FillRectangleFast(POINT pLoc,SIZE szSize,RgbColor rgbColorIn,RgbColor rgbColorOut = Rgb::Undefined,int iPenSize = 0);                          
        
        bool DrawRectangleFast(int iX,int iY,int iWidth,int iHeight,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);                                   
        bool DrawRectangleFast(POINT pLoc,SIZE szSize,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);                                                 


        // Triangle Draw Functions

        bool DrawTriangle(POINT v1,POINT v2,POINT v3,RgbColor rgbColor,int iPenSize = 0);                                                      
        bool DrawTriangle(CfPoint v1,CfPoint v2,CfPoint v3,RgbColor rgbColor,double iPenSize = 0);                                             
        bool DrawTriangle(CfPointf v1,CfPointf v2,CfPointf v3,RgbColor rgbColor,float iPenSize = 0);                                           

        bool FillTriangle(POINT v1,POINT v2,POINT v3,RgbColor rgbColorIn, RgbColor rgbColorOut = Rgb::Undefined,int iPenSize = 0);             
        bool FillTriangle(CfPoint v1,CfPoint v2,CfPoint v3,RgbColor rgbColorIn, RgbColor rgbColorOut = Rgb::Undefined,double iPenSize = 0);    
        bool FillTriangle(CfPointf v1,CfPointf v2,CfPointf v3,RgbColor rgbColorIn, RgbColor rgbColorOut = Rgb::Undefined,float iPenSize = 0);  

        bool FillTriangleFast(POINT v1,POINT v2,POINT v3,RgbColor rgbColorIn, RgbColor rgbColorOut = Rgb::Undefined,int iPenSize = 0);         
        bool DrawTriangleFast(POINT v1,POINT v2,POINT v3,RgbColor rgbColor,int iPenSize = 0); 

        // Quadrangle Draw Functions

        bool DrawQuadrangle(POINT v1,POINT v2,POINT v3,POINT v4,RgbColor rgbColor,int iPenSize = 0);                                                      
        bool DrawQuadrangle(CfPoint v1,CfPoint v2,CfPoint v3,CfPoint v4,RgbColor rgbColor,double iPenSize = 0);                                             
        bool DrawQuadrangle(CfPointf v1,CfPointf v2,CfPointf v3,CfPointf v4,RgbColor rgbColor,float iPenSize = 0);                                           

        bool FillQuadrangle(POINT v1,POINT v2,POINT v3,POINT v4,RgbColor rgbColorIn, RgbColor rgbColorOut = Rgb::Undefined,int iPenSize = 0);             
        bool FillQuadrangle(CfPoint v1,CfPoint v2,CfPoint v3,CfPoint v4,RgbColor rgbColorIn, RgbColor rgbColorOut = Rgb::Undefined,double iPenSize = 0);    
        bool FillQuadrangle(CfPointf v1,CfPointf v2,CfPointf v3,CfPointf v4,RgbColor rgbColorIn, RgbColor rgbColorOut = Rgb::Undefined,float iPenSize = 0);  

        bool FillQuadrangleFast(POINT v1,POINT v2,POINT v3,POINT v4,RgbColor rgbColorIn, RgbColor rgbColorOut = Rgb::Undefined,int iPenSize = 0);         
        bool DrawQuadrangleFast(POINT v1,POINT v2,POINT v3,POINT v4,RgbColor rgbColor,int iPenSize = 0); 

        // Polygon functions

        bool DrawPolygon(POINT * pPoints,int iVertices,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);                                                                       
        bool DrawPolygon(CfPointf * pPoints,int iVertices,RGBColor_t rgbColor = Rgb::Default,float iPenSize = 0);                                                                  
        bool DrawPolygon(CPoint * pPoints,int iVertices,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);                                                                      
        bool DrawPolygon(CfPoint * pPoints,int iVertices,RGBColor_t rgbColor = Rgb::Default,double iPenSize = 0);                                                                  
        bool DrawPolygon(Gdiplus::Point * pPoints,int iVertices,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);                                                              
        bool DrawPolygon(Gdiplus::PointF * pPoints,int iVertices,RGBColor_t rgbColor = Rgb::Default,float iPenSize = 0);                                                           

        bool FillPolygon(POINT * pPoints,int iVertices,RGBColor_t rgbColor = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined,int iPenSize = 0);                         
        bool FillPolygon(CPoint * pPoints,int iVertices,RGBColor_t rgbColor = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined,int iPenSize = 0);                        
        bool FillPolygon(CfPointf * pPoints,int iVertices,RGBColor_t rgbColor = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined,float iPenSize = 0);                    
        bool FillPolygon(CfPoint * pPoints,int iVertices,RGBColor_t rgbColor = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined,double iPenSize = 0);                    
        bool FillPolygon(Gdiplus::Point * pPoints,int iVertices,RGBColor_t rgbColor = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined,int iPenSize = 0);                
        bool FillPolygon(Gdiplus::PointF * pPoints,int iVertices,RGBColor_t rgbColor = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined,float iPenSize = 0);             
   
        bool DrawPolygonFast(POINT * pPoints,int iVertices,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);                                                                   
        bool DrawPolygonFast(CPoint * pPoints,int iVertices,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);                                                                  
        bool DrawPolygonFast(CfPoint * pPoints,int iVertices,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);                                                                 


        bool FillPolygonFast(POINT * pPoints,int iVertices,RGBColor_t rgbColor = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined,int iPenSize = 0);                     
        bool FillPolygonFast(CPoint * pPoints,int iVertices,RGBColor_t rgbColor = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined,int iPenSize = 0);                    
        bool FillPolygonFast(CfPoint * pPoints,int iVertices,RGBColor_t rgbColor = Rgb::Default,RGBColor_t rgbColorOut = Rgb::Undefined,int iPenSize = 0);                   


        // Line Draw Functions

        bool MoveTo(int iX,int iY,RGBColor_t rgbColor = Rgb::Undefined);                                       
        bool MoveTo(POINT pLoc,RGBColor_t rgbColor = Rgb::Undefined);                                          

        bool MoveTo(float iX,float iY,RGBColor_t rgbColor = Rgb::Undefined);                                   
        bool MoveTo(double iX,double iY,RGBColor_t rgbColor = Rgb::Undefined);                                 
        bool MoveTo(CfPointf pLoc,RGBColor_t rgbColor = Rgb::Undefined);                                       
        bool MoveTo(CfPoint pLoc,RGBColor_t rgbColor = Rgb::Undefined);                                        

        bool LineToFast(int iX,int iY,RGBColor_t rgbColor  = Rgb::Undefined,int iPenSize = 0);             
        bool LineToFast(POINT pLoc,RGBColor_t rgbColor  = Rgb::Undefined,int iPenSize =0);                 
  
        bool LineTo(POINT p1,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);                         
        bool LineTo(int ix1,int iy1,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);                  
        bool LineTo(float ix1,float iy1,RGBColor_t rgbColor = Rgb::Default,float iPenSize = 0);            
        bool LineTo(double ix1,double iy1,RGBColor_t rgbColor = Rgb::Default,double iPenSize = 0);         
        bool LineTo(const CfPointf p1,RGBColor_t rgbColor = Rgb::Default,float iPenSize = 0);              
        bool LineTo(const CfPoint p1,RGBColor_t rgbColor = Rgb::Default,double iPenSize = 0);              


        bool LineToExFast(bool bFirstPoint,int iX,int iY,RGBColor_t rgbColor);                                         
        bool LineToExFast(bool bFirstPoint,POINT pLoc,RGBColor_t rgbColor);                                            

        bool LineToEx(bool bFirstPoint,POINT p1,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);                  
        bool LineToEx(bool bFirstPoint,int ix1,int iy1,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);           
        bool LineToEx(bool bFirstPoint,float ix1,float iy1,RGBColor_t rgbColor = Rgb::Default,float iPenSize = 0);     
        bool LineToEx(bool bFirstPoint,double ix1,double iy1,RGBColor_t rgbColor = Rgb::Default,double iPenSize = 0);  
        bool LineToEx(bool bFirstPoint,const CfPointf p1,RGBColor_t rgbColor = Rgb::Default,float iPenSize = 0);       
        bool LineToEx(bool bFirstPoint,const CfPoint p1,RGBColor_t rgbColor = Rgb::Default,double iPenSize = 0);       


        bool DrawLineFast(POINT p1,POINT p2,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);                              
        bool DrawLineFast(int ix1,int iy1,int ix2,int iy2,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);                

        bool DrawLine(POINT p1,POINT p2,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);                                  
        bool DrawLine(int ix1,int iy1,int ix2,int iy2,RGBColor_t rgbColor = Rgb::Default,int iPenSize = 0);                    
        bool DrawLine(float ix1,float iy1,float ix2,float iy2,RGBColor_t rgbColor = Rgb::Default,float iPenSize = 0);          
        bool DrawLine(double ix1,double iy1,double ix2,double iy2,RGBColor_t rgbColor = Rgb::Default,double iPenSize = 0);     
        bool DrawLine(const CfPointf p1,const CfPointf p2,RGBColor_t rgbColor = Rgb::Default,float iPenSize = 0);              
        bool DrawLine(const CfPoint p1,const CfPoint p2,RGBColor_t rgbColor = Rgb::Default,double iPenSize = 0);               

        // GDI Transform functions

        /// <summary>
        /// Sets the opacity for shapes drawn with GDI functions, such as Circle(), FillCircle(), Triangle, etc.
        /// </summary>
        /// <param name="iOpacity">- Opacity value (0 = transparent, 100 (or 255 is bAsByteValue is true) is completely opaque.  Default is fully opaque</param>
        /// <param name="bAsByteValue">When false, the Opacity value is used as a percentage.  When true, Opacity value is used as a BYTE value 0-255 to set the value directly.</param>
        /// <returns></returns>
        bool SetDrawOpacity(int iOpacity,bool bAsByteValue = false);

        /// <summary>
        /// Sets the opacity for shapes drawn with GDI functions, such as Circle(), FillCircle(), Triangle, etc.
        /// </summary>
        /// <param name="iOpacity">- Opacity value (0 = transparent, 100 (or 255 is bAsByteValue is true) is completely opaque.  Default is fully opaque</param>
        /// <param name="bAsByteValue">When false, the Opacity value is used as a percentage.  When true, Opacity value is used as a BYTE value 0-255 to set the value directly.</param>
        /// <returns></returns>
        bool SetDrawOpacity(float fOpacity,bool bAsByteValue = false);

        /// <summary>
        /// Sets the opacity for shapes drawn with GDI functions, such as Circle(), FillCircle(), Triangle, etc.
        /// </summary>
        /// <param name="iOpacity">- Opacity value (0 = transparent, 100 (or 255 is bAsByteValue is true) is completely opaque.  Default is fully opaque</param>
        /// <param name="bAsByteValue">When false, the Opacity value is used as a percentage.  When true, Opacity value is used as a BYTE value 0-255 to set the value directly.</param>
        /// <returns></returns>
        bool SetDrawOpacity(double fOpacity,bool bAsByteValue = false);

        /// <summary>
        /// Returns the current Opacity for GDI-based draw functions.  Value is from 0-255 (fully transparent to fully opaque)
        /// </summary>
        /// <returns></returns>
        int GetDrawOpacity();

        /// <summary>
        /// Sets the rotational transform for objects drawn with with GDI functions, such as Circle(), FillCircle(), Triangle, etc.
        /// <para>
        /// When used, the object will be rotated by --iAngle-- degrees. 
        /// </para><para>--> Use DrawResetTransform when finished with transformations
        /// </para><para>--> Transformations are processed in order called.  calling DrawRotateTransform() then DrawTranslateTransform() vs. 
        /// </para><para>DrawTranslateTransform() then DrawRotateTransform() will result in different behavior. 
        /// </para><para>--> Typical order is DrawTranslateTransform() to set the center of the transform, then DrawRotateTransform() to rotate on that center.
        /// </para>
        /// </summary>
        /// <param name="iAngle">Angle to rotate in degrees</param>
        /// <returns></returns>
        bool RotateTransform(int iAngle);
 
        /// <summary>
        /// Sets the rotational transform for objects drawn with with GDI functions, such as Circle(), FillCircle(), Triangle, etc.
        /// <para>
        /// When used, the object will be rotated by --iAngle-- degrees. 
        /// </para><para>--> Use DrawResetTransform when finished with transformations
        /// </para><para>--> Transformations are processed in order called.  calling DrawRotateTransform() then DrawTranslateTransform() vs. 
        /// </para><para>DrawTranslateTransform() then DrawRotateTransform() will result in different behavior. 
        /// </para><para>--> Typical order is DrawTranslateTransform() to set the center of the transform, then DrawRotateTransform() to rotate on that center.
        /// </para>
        /// </summary>
        /// <param name="iAngle">Angle to rotate in degrees</param>
        /// <returns></returns>
        bool RotateTransform(double fAngle);

        /// <summary>
        /// Sets the rotational transform for objects drawn with with GDI functions, such as Circle(), FillCircle(), Triangle, etc.
        /// <para>
        /// When used, the object will be rotated by --iAngle-- degrees. 
        /// </para><para>--> Use DrawResetTransform when finished with transformations
        /// </para><para>--> Transformations are processed in order called.  calling DrawRotateTransform() then DrawTranslateTransform() vs. 
        /// </para><para>DrawTranslateTransform() then DrawRotateTransform() will result in different behavior. 
        /// </para><para>--> Typical order is DrawTranslateTransform() to set the center of the transform, then DrawRotateTransform() to rotate on that center.
        /// </para>
        /// </summary>
        /// <param name="iAngle">Angle to rotate in degrees</param>
        /// <returns></returns>
        bool RotateTransform(float iAngle);

        /// <summary>
        /// Resets all transforms for GDI-based object (i.e. objects drawn with with GDI functions, such as Circle(), FillCircle(), Triangle, etc.)
        /// </summary>
        /// <returns></returns>
        bool ResetTransform();

        /// <summary>
        /// Sets the positional transform for objects drawn with with GDI functions, such as Circle(), FillCircle(), Triangle, etc.
        /// <para>
        /// When used, the object drawn at (0,0) will now be drawn at (X,y) and all coordinates used will transform (0,0) into X and Y
        /// </para><para>--> Use DrawResetTransform when finished with transformations
        /// </para><para>--> Transformations are processed in order called.  calling DrawRotateTransform() then DrawTranslateTransform() vs. 
        /// </para><para>DrawTranslateTransform() then DrawRotateTransform() will result in different behavior. 
        /// </para><para>--> Typical order is DrawTranslateTransform() to set the center of the transform, then DrawRotateTransform() to rotate on that center.
        /// </para>
        /// </summary>
        /// <param name="iAngle">Angle to rotate in degrees</param>
        /// <returns></returns>    
        bool TranslateTransform(int iX,int iY);

        /// <summary>
        /// Sets the positional transform for objects drawn with with GDI functions, such as Circle(), FillCircle(), Triangle, etc.
        /// <para>
        /// When used, the object drawn at (0,0) will now be drawn at (X,y) and all coordinates used will transform (0,0) into X and Y
        /// </para><para>--> Use DrawResetTransform when finished with transformations
        /// </para><para>--> Transformations are processed in order called.  calling DrawRotateTransform() then DrawTranslateTransform() vs. 
        /// </para><para>DrawTranslateTransform() then DrawRotateTransform() will result in different behavior. 
        /// </para><para>--> Typical order is DrawTranslateTransform() to set the center of the transform, then DrawRotateTransform() to rotate on that center.
        /// </para>
        /// </summary>
        /// <param name="iAngle">Angle to rotate in degrees</param>
        /// <returns></returns>    
        bool TranslateTransform(float iX,float iY);
 
        /// <summary>
        /// Sets the positional transform for objects drawn with with GDI functions, such as Circle(), FillCircle(), Triangle, etc.
        /// <para>
        /// When used, the object drawn at (0,0) will now be drawn at (X,y) and all coordinates used will transform (0,0) into X and Y
        /// </para><para>--> Use DrawResetTransform when finished with transformations
        /// </para><para>--> Transformations are processed in order called.  calling DrawRotateTransform() then DrawTranslateTransform() vs. 
        /// </para><para>DrawTranslateTransform() then DrawRotateTransform() will result in different behavior. 
        /// </para><para>--> Typical order is DrawTranslateTransform() to set the center of the transform, then DrawRotateTransform() to rotate on that center.
        /// </para>
        /// </summary>
        /// <param name="iAngle">Angle to rotate in degrees</param>
        /// <returns></returns>  
        bool TranslateTransform(double iX,double iY);

        /// <summary>
        /// Sets the positional transform for objects drawn with with GDI functions, such as Circle(), FillCircle(), Triangle, etc.
        /// <para>
        /// When used, the object drawn at (0,0) will now be drawn at (X,y) and all coordinates used will transform (0,0) into X and Y
        /// </para><para>--> Use DrawResetTransform when finished with transformations
        /// </para><para>--> Transformations are processed in order called.  calling DrawRotateTransform() then DrawTranslateTransform() vs. 
        /// </para><para>DrawTranslateTransform() then DrawRotateTransform() will result in different behavior. 
        /// </para><para>--> Typical order is DrawTranslateTransform() to set the center of the transform, then DrawRotateTransform() to rotate on that center.
        /// </para>
        /// </summary>
        /// <param name="iAngle">Angle to rotate in degrees</param>
        /// <returns></returns>
        bool TranslateTransform(POINT pLoc);

        /// <summary>
        /// Sets the positional transform for objects drawn with with GDI functions, such as Circle(), FillCircle(), Triangle, etc.
        /// <para>
        /// When used, the object drawn at (0,0) will now be drawn at (X,y) and all coordinates used will transform (0,0) into X and Y
        /// </para><para>--> Use DrawResetTransform when finished with transformations
        /// </para><para>--> Transformations are processed in order called.  calling DrawRotateTransform() then DrawTranslateTransform() vs. 
        /// </para><para>DrawTranslateTransform() then DrawRotateTransform() will result in different behavior. 
        /// </para><para>--> Typical order is DrawTranslateTransform() to set the center of the transform, then DrawRotateTransform() to rotate on that center.
        /// </para>
        /// </summary>
        /// <param name="iAngle">Angle to rotate in degrees</param>
        /// <returns></returns>
        bool TranslateTransform(CfPointf pLoc);

        /// <summary>
        /// Sets the positional transform for objects drawn with with GDI functions, such as Circle(), FillCircle(), Triangle, etc.
        /// <para>
        /// When used, the object drawn at (0,0) will now be drawn at (X,y) and all coordinates used will transform (0,0) into X and Y
        /// </para><para>--> Use DrawResetTransform when finished with transformations
        /// </para><para>--> Transformations are processed in order called.  calling DrawRotateTransform() then DrawTranslateTransform() vs. 
        /// </para><para>DrawTranslateTransform() then DrawRotateTransform() will result in different behavior. 
        /// </para><para>--> Typical order is DrawTranslateTransform() to set the center of the transform, then DrawRotateTransform() to rotate on that center.
        /// </para>
        /// </summary>
        /// <param name="iAngle">Angle to rotate in degrees</param>
        /// <returns></returns>
        bool TranslateTransform(CfPoint pLoc);



    };

    WinDraw draw;
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

        CWindow * m_cWin                    = nullptr;
        CPasWindow * m_cWinCore             = nullptr;
        void InitDialog(DialogStruct & stDialog,const char * sTitle,const char * sOptions,DialogStruct::TitleIconType eDefault);
        bool    m_bYesNoCancel = false;            // For YesNoCancel to call YesNo() and get cancel status
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
        int WinMessageBox(const char * sMessage,const char * sTitle = nullptr,unsigned int dwFlags = MB_OK);
        int GetInteger(const char * sTitle,bool & bCancelled,const cwfOpt & cwOptions = cwfOpt());
        int GetInteger(const char * sTitle = nullptr,const cwfOpt & cwOptions = cwfOpt());
        bool GetInteger(const char * sTitle,int & iInteger,const cwfOpt & cwOptions = cwfOpt());
        bool GetInteger(int & iInteger,const cwfOpt & cwOptions = cwfOpt());
        int GetInteger(const cwfOpt & cwOptions);

        double GetFloat(const char * sTitle,bool & bCancelled,const cwfOpt & cwOptions = cwfOpt());
        double GetFloat(const char * sTitle = nullptr,const cwfOpt & cwOptions = cwfOpt());
        bool GetFloat(const char * sTitle,double & fFloat,const cwfOpt & cwOptions = cwfOpt());
        bool GetFloat(double & fFloat,const cwfOpt & cwOptions = cwfOpt());
        double GetFloat(const cwfOpt & cwOptions);

        CString GetString(const char * sTitle,bool & bCancelled,const cwfOpt & cwOptions = cwfOpt());
        CString GetString(const char * sTitle,const cwfOpt & cwOptions = cwfOpt());

        void InfoWindow(const char * sTitle,const cwfOpt & cwOptions = cwfOpt());
        bool YesNoWindow(const char * sTitle,const cwfOpt & cwOptions = cwfOpt());

        DialogResult YesNoCancelWindow(const char * sTitle,const cwfOpt & cwOptions = cwfOpt());
        bool OkCancelWindow(const char * sTitle,const cwfOpt & cwOptions = cwfOpt());

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
#if 0
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

        /// <summary>
        /// returns TRUE of the checkbox number is checked (i.e. 0,1,2, depending on position in the group).  returns FALSE if the checkbox is not checked.
        /// <para></para>&#160;&#160;&#160;
        /// This function is useful for checkboxes only.  For Radio Buttons, use GetChecked() to get the value of the filled radio button.
        /// &#160;&#160;&#160;
        /// This returns the checked status only.  To determine if a specific checkbox button was pressed, is isPressed() with the checkbox ID.
        /// </summary>
        /// <param name="sGroupID"> = Name of chexkbox group (can also be a number)</param>
        /// <param name="iCheckboxID"> = Position of Checkbox in group (in order it was added, starting with 0,1,2, etc.)</param>
        /// <returns>TRUE if checkbox ID is checked. Returns FALSE if it is not checked.</returns>
        bool isChecked(const char * sGroupID,int iCheckboxID);

         /// <summary>
        /// returns TRUE of the checkbox number is checked (i.e. 0,1,2, depending on position in the group).  returns FALSE if the checkbox is not checked.
        /// <para></para>&#160;&#160;&#160;
        /// This function is useful for checkboxes only.  For Radio Buttons, use GetChecked() to get the value of the filled radio button.
        /// &#160;&#160;&#160;
        /// This returns the checked status only.  To determine if a specific checkbox button was pressed, is isPressed() with the checkbox ID.
        /// </summary>
        /// <param name="sGroupID"> = Name of chexkbox group (can also be a number)</param>
        /// <param name="iCheckboxID"> = Position of Checkbox in group (in order it was added, starting with 0,1,2, etc.)</param>
        /// <returns>TRUE if checkbox ID is checked. Returns FALSE if it is not checked.</returns>
        bool isChecked(int isGroupID,int iCheckboxID);

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
        ButtonGroup GetButtonGroup(const char * sControl);
        // GetGroup() -- This returns a ControlGroup Class.  This is experimental and TBD
        //
        ControlGroup GetGroup(int iControl);
        ButtonGroup GetButtonGroup(int iControl);

        // CreateGroup() -- Creates a group (This is experimental and in early stages)
        //
        // CreateGroup() pre-creates a group that can be specified when creating a control.  CreateGroup() is not necessary, as
        // the first usage of a Group ID or Name creates the group when it is first seen.
        //
        int CreateGroup(const char * sGroupName,GroupType eGroupType = GroupType::Undefined); 
       SIZE CreateRadioButtonGroup(int iNumButtons,int iX,int iY,const char * * sButtonNames,int iGroupID, const char * sLabel, const cwfOpt & cwOpt = cwfOpt());
       SIZE CreateCheckboxGroup(int iNumButtons,int iX,int iY,const char * * sButtonNames,int iGroupID, const char * sLabel, const cwfOpt & cwOpt = cwfOpt());
       CButton * GetButton(int iGroupID, int iPosition);
       CButton * GetButton(const char * sGroupID, int iPosition);
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
    BkMode GetWinBkMode(); // $QC

    // SetWinBkMode() -- Set the text background mode.
    // BkMode returns eeither BkMode::Transparent or BkMode::Opaque
    //
    // BkMode::Transparent()  --- This is the default.  When text is written to the screen, only the text part is written in the
    //                            foreground color, leaving the blank areas (i.e. text or graphics behind the new text) untouched.
    //
    // BkMode::Opaque         -- When text is written to the window, this will write the text in the foreground color and fill in the background with the
    //                           background color overwriting any text or graphics behind the new text.
    //
    bool SetWinBkMode(BkMode eBkMode);  // $QC
  
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
    bool SetBgColor(const char * sColor); // $QC

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
    bool SetBgColor(DWORD dwColor); // $QC

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
    bool SetBgColor(RGBColor_t rgbColor); // $QC

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
    bool SetFgColor(const char * sColor); // $QC

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
    bool SetFgColor(DWORD dwColor); // $QC

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
    bool SetFgColor(RGBColor_t rgbColor); // $QC

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

        bool MouseUnclicked(Peek peek = Peek::No); 
        bool MouseUnclicked(POINT & pMouse,Peek peek = Peek::No); 

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
        std::optional<int> MenuItemSelected(Peek peek = Peek::No)                      ;

        // Retrieves the last menu item selected.
        // 
        // This is meant to be used right after MenuItemSelected() is called, and will return the last menu item selected.
        //
        // This will continue to return the same menu item until a new menu item is selected.
        //
        int       GetSelectedMenuItem()                                             ;     
 
        // Returns true if the Left Mouse Button is currently pressed.  
        // This is not a mouse event and returns the real-time status of the mouse.
        //

        /// <summary>
        /// Returns true if the Left Mouse Button is currently pressed. 
        /// <para></para>
        /// This is not a mouse event and returns the real-time status of the mouse.
        /// </summary>
        /// <param name="pMouse"> - (optional POINT) fills pMouse with current mouse location </param>
        /// <returns></returns>
        bool MouseButtonDown();

        /// <summary>
        /// Returns true if the Left Mouse Button is currently pressed. 
        /// <para></para>
        /// This is not a mouse event and returns the real-time status of the mouse.
        /// </summary>
        /// <param name="pMouse"> - (optional POINT) fills pMouse with current mouse location </param>
        /// <returns></returns>
        bool MouseButtonDown(POINT & pMouse);

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
        bool KeyPressed(char & cKey,Peek peek = Peek::No);

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
        char KeyPressed(Peek peek = Peek::No); 

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
        bool PressButtonOnClose(CButton & cButton);

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

        /// <summary>
        /// Waits for a mouse click in the window.  This is used a simple way to hold up program execution until the user
        /// clicks the mouse.
        /// <para></para>&#160;&#160;&#160;
        /// The window is set as the foreground and active window when this function is called.
        /// --> Note: This function will return if the user closes the window.
        /// </summary>
        /// <param name="sMsg"> = (optional) message to print out as text to the window.</param>
        /// <returns></returns>
        bool WaitforClick(const char * sMsg = nullptr);

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
        char WaitforKeyPress(const char * sMsg = nullptr); 

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
        bool WaitforCRPress(const char * sMsg = nullptr); 

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
        bool WaitforClickOrKey(const char * sMsg = nullptr); 

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

#ifdef kCPP17Functions
        std::optional<int> MouseWheelMoved(Peek peek = Peek::No);
#else
        bool MouseWheelMoved(Peek peek = Peek::No);
#endif
        bool MouseWheelMoved(int & iDistance,Peek peek = Peek::No);
        int GetMouseWheelMove(bool bResetEvent = false); 

        bool WindowResized(Peek peek = Peek::No);
        bool WindowResized(SIZE & szNewWinSize,Peek peek = Peek::No);

        bool WindowResizing(Peek peek = Peek::No);
        bool WindowResizing(SIZE & szNewWinSize,Peek peek = Peek::No);

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
        __forceinline bool EventPending(Peek peek = Peek::No) { return !m_cWin ? false : m_cWin->EventPending(peek); };

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
        CWindow * m_cWin = nullptr;
        CPoint m_pTermXY = {};
        CPoint m_pTermPos = {};
    public:
        int putchar(char cChar);
        int __putchar(char cChar);
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
        CBitmap CreateBitmap(int iWidth,int iHeight = 1);
        CBitmap ReadBitmap(const char * sPath,bool * bSucceeded = nullptr);
        bool WriteBitmap(const char * sFile,CBitmap & cBitmap); 

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
    WinConio conio; 

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
    // Important Note: As this changes, so does WinDebug in CSagebox.  They are separate but parallel, except for the CSagebox exceptions at the BOTTOM
    // of the class interface. 
    //
    struct WinDebug
    {
        friend CWindow;
    private:
        CWindow         * m_cWin = nullptr;
    public:
        WinDebug & operator << (const char * x)        ;
        WinDebug & operator << (char x)                ;
        WinDebug & operator << (std::string & cs)      ;
        WinDebug & operator << (CString & cs)          ;
        WinDebug & operator << (int x)                 ;
        WinDebug & operator << (unsigned int x)        ;
        WinDebug & operator << (float x)               ;
        WinDebug & operator << (double x)              ;

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
       bool SetUpdate(UpdateType eType);
       bool Update();


        bool SetGlobalString(int iIndex,const char * sString);
        bool SetGlobalValue(int iIndex,const char * sString);
        bool SetGlobalValue(int iIndex,long long iValue);
        bool SetGlobalValue(int iIndex,double fValue);
        bool SetGlobalValue(int iIndex,int iValue);
        bool SetGlobalValue(int iIndex,unsigned int iValue);
        bool SetGlobalValue(int iIndex,unsigned long long uiValue);
        bool SetGlobalValue(int iIndex, void* vPointer);
        bool SetGlobalValue(int iIndex, bool bValue);

        bool SetGlobalString(const char * sName,const char * sString);
        bool SetGlobalValue(const char * sName,const char * sString);
        bool SetGlobalValue(const char * sName,long long iValue);
        bool SetGlobalValue(const char * sName,double fValue);
        bool SetGlobalValue(const char * sName,int iValue);
        bool SetGlobalValue(const char * sName,unsigned int iValue);
        bool SetGlobalValue(const char * sName,unsigned long long uiValue);
        bool SetGlobalValue(const char * sName,void * vPointer);
        bool SetGlobalValue(const char *, bool bValue);

        const char * GetGlobalString(int iIndex);
        long long GetGlobalValue(int iIndex);
        double GetGlobalDouble(int iIndex);
        int GetGlobalInt(int iIndex);
        unsigned int GetGlobalUInt(int iIndex);
        unsigned long long GetGlobalULongLong(int iIndex);
        void * GetGlobalPointer(int iIndex);
        bool GetGlobalBool(int iIndex);

        const char * GetGlobalString(const char * sName);
        long long GetGlobalValue(const char * sName);
        double GetGlobalDouble(const char * sName);
        int GetGlobalInt(const char * sName);
        unsigned int GetGlobalUInt(const char * sName);
        unsigned long long GetGlobalULongLong(const char * sName);
        void * GetGlobalPointer(const char * sName);
        bool GetGlobalBool(const char * sName);

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
        //      Init(WinDebug::InitType::Debug)         -- Brings up the Process Control Window with the Debug Window showing. This will
        //                                                 not auto-minimize as it does when the "Show Debug" switch is off. 
        //      Init(WinDebug::InitType::DebugLineNumbers)         -- Same as WinDebug::InitType::Debug, but with line numbers turn on
        //                                                            (see WinDebug:ShowLineNumbers())
        //
        // Init() is not necessary for debug output -- the first call to any debug output routine will automatically initalize
        // and display the Process Control Window and its debug window. 
        //
        // returns TRUE if Process Window is active. 
        //
        bool Init(bool bVisible = false);
        
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
        //      Init(WinDebug::InitType::Debug)         -- Brings up the Process Control Window with the Debug Window showing. This will
        //                                                 not auto-minimize as it does when the "Show Debug" switch is off. 
        //      Init(WinDebug::InitType::DebugLineNumbers)         -- Same as WinDebug::InitType::Debug, but with line numbers turn on
        //                                                            (see WinDebug:ShowLineNumbers())
        //
        // Init() is not necessary for debug output -- the first call to any debug output routine will automatically initalize
        // and display the Process Control Window and its debug window. 
        //
        // returns TRUE if Process Window is active. 
        //
        bool Init(InitType eInitType);
        
        
        // Show() -- Show or Hide the Process Control Window
        //
        bool Show(bool bShow = true); 

        // Hide() -- Hide or Show the Process Control Window
        //
        bool Hide(bool bHide = true);

        // printf() -- print out to the debug window, just as in std::printf()
        //
        bool printf(const char * Format,...);
// __Sandbox__Except__ deprecated (for now)
//#ifdef __Sandbox__Except__
        // printf() -- print out to the debug window, just as in std::printf()
        //
        bool __printf(const char * Format,...);
//#endif
        // Write() -- Write out to the debug window.  Can be quicker than using printf()
        //
        bool Write(const char * sText);

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
        bool KillTimer(bool bKillTimer = true);

        // ShowLineNumbers() -- Shows line numbers in the debug display.
        //
        // The Debug window defaults to ShowLineNumbers off.  You can use 
        // Init(CWindow::WinDebug::InitType::DebugLineNumbers) to initialize the window with line numbers on.
        //
        // Use ShowLineNumbers(true) turn turn line numbers on
        // Use ShowLineNumbers(false) to turn line numbers off.
        //
        bool ShowLineNumbers(bool bShowLineNumbers = true);

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
        bool SetControlCBreak(bool bActive);
    };

    WinDebug debug;

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
        // GetString() will return immediately of WindowClosing() is true, allowing multiple console input funcoctions in the code, as they will all fall through so you may
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
        CString getline(CString & cString,const cwfOpt & cwOpt = cwfOpt());
        CString __getline(CString & cString,const cwfOpt & cwOpt = cwfOpt());
 
        // Duplication of C++ console-mode getline()
        //
        // This is the same as GetString() except that the sring input is specified in the function call.
        // This is to duplicate the C++ getline() function.
        //
        // CString and std::string are supported.
        //
        // See GetString() for option usage.
        //
        CString getline(std::string & cString,const cwfOpt & cwOpt = cwfOpt());

        // Duplication of C++ console-mode getline()
        //
        // This is the same as GetString() except that the sring input is specified in the function call.
        // This is to duplicate the C++ getline() function.
        //
        // CString and std::string are supported.
        //
        // See GetString() for option usage.
        //
        CString __getline(std::string & cString,const cwfOpt & cwOpt = cwfOpt());
        
        // Duplication of C++ console-mode getline()
        //
        // This is the same as GetString() except that the sring input is specified in the function call.
        // This is to duplicate the C++ getline() function.
        //
        // CString and std::string are supported.
        //
        // See GetString() for option usage.
        //
        CString getline(const cwfOpt & cwOpt = cwfOpt());
        CString __getline(const cwfOpt & cwOpt = cwfOpt());

        // Duplication of C++ console-mode getline()
        //
        // This is the same as GetString() except that the sring input is specified in the function call.
        // This is to duplicate the C++ getline() function.
        //
        // CString and std::string are supported.
        //
        // See GetString() for option usage.
        //
        int getline(char * sString,int iMaxLen);
        int __getline(char * sString,int iMaxLen);

        int scanf(const char *format, ...);
        int __scanf(const char *format, ...);
        char getchar();
        char __getchar();

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
        int GetInteger(const char * sText,const cwfOpt & cwOpt = cwfOpt());

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

        // NewEditBox() -- Set an edit box to get information.  This is returns a control object and does not stop
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
        // auto & MyEditbox = MyWindow.console.NewEditBox() -- Creates a small edit box.  Use Width() or CharWidth() to creae a wider box
        // auto & MyEditBox = MyWindow.console.NewEditBox("Enter Some Text: ",Width(200));
        // auto & MyEditbox = MyWindow.console.NewEditBox("Enter a number: ",FloatsOnly());
        // auto & MyEditbox = MyWindow.console.NewEditBox("Enter a number: ",Range(-10.0,10.0)); (also sets FloatsOnly setting)
        //
        CEditBox & NewEditBox(const cwfOpt & cwOptions = cwfOpt());                                            
        CInputBox & NewInputBox(const cwfOpt & cwOptions = cwfOpt()) { return NewEditBox(cwOptions); } // $$moveme

        // NewEditBox() -- Set an edit box to get information.  This is returns a control object and does not stop
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
        // auto & MyEditbox = MyWindow.console.NewEditBox() -- Creates a small edit box.  Use Width() or CharWidth() to creae a wider box
        // auto & MyEditBox = MyWindow.console.NewEditBox("Enter Some Text: ",Width(200));
        // auto & MyEditbox = MyWindow.console.NewEditBox("Enter a number: ",FloatsOnly());
        // auto & MyEditbox = MyWindow.console.NewEditBox("Enter a number: ",Range(-10.0,10.0)); (also sets FloatsOnly setting)
        //
        CEditBox & NewEditBox(const char * sLabel,const cwfOpt & cwOptions = cwfOpt());                            
        CInputBox & NewInputBox(const char * sLabel,const cwfOpt & cwOptions = cwfOpt()) { return NewEditBox(sLabel,cwOptions); } // $$moveme                          

        // NewEditBox() -- Set an edit box to get information.  This is returns a control object and does not stop
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
        // auto & MyEditbox = MyWindow.console.NewEditBox() -- Creates a small edit box.  Use Width() or CharWidth() to creae a wider box
        // auto & MyEditBox = MyWindow.console.NewEditBox("Enter Some Text: ",Width(200));
        // auto & MyEditbox = MyWindow.console.NewEditBox("Enter a number: ",FloatsOnly());
        // auto & MyEditbox = MyWindow.console.NewEditBox("Enter a number: ",Range(-10.0,10.0)); (also sets FloatsOnly setting)
        //
        CEditBox & NewEditBox(CEditBox * pObject,const cwfOpt & cwOptions = cwfOpt());        
        CInputBox & NewInputBox(CInputBox * pObject,const cwfOpt & cwOptions = cwfOpt()) { return NewEditBox(pObject,cwOptions); } // $$moveme     

        // NewEditBox() -- Set an edit box to get information.  This is returns a control object and does not stop
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
        // auto & MyEditbox = MyWindow.console.NewEditBox() -- Creates a small edit box.  Use Width() or CharWidth() to creae a wider box
        // auto & MyEditBox = MyWindow.console.NewEditBox("Enter Some Text: ",Width(200));
        // auto & MyEditbox = MyWindow.console.NewEditBox("Enter a number: ",FloatsOnly());
        // auto & MyEditbox = MyWindow.console.NewEditBox("Enter a number: ",Range(-10.0,10.0)); (also sets FloatsOnly setting)
        //
        CEditBox & NewEditBox(CEditBox * pObject,char * sLabel,const cwfOpt & cwOptions = cwfOpt());        
        CInputBox & NewInputBox(CInputBox * pObject,char * sLabel,const cwfOpt & cwOptions = cwfOpt()) { return NewEditBox(pObject,sLabel,cwOptions); }    //$$moveme      

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
        double GetFloat(const char * sText,const cwfOpt & cwOpt = cwfOpt());

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
        double GetFloat(const cwfOpt & cwOpt = cwfOpt());

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
        // SetFont("Arial,20")              -- Sets the font to Arial,20
        // MyFont = SetFont("Arial,20","TestFont")  -- Set the font to Arial,20 and name it TestFont.  Store it in MyFont
        // SetFont("TestFont");             -- Set the Font named "TestFont"
        // SetFont(MyFont)                  -- Sets the font to the HFONT MyFont Value
        //
        HFONT SetFont(int iFontSize);    

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
        int WinMessageBox(const char * sMessage,const char * sTitle = nullptr,unsigned int dwFlags = MB_OK);
    
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
        void printf(const cwfOpt & cwOpt,const char * Format,...);

        // printf() -- Works the same way as 'C' printf, except you can also put (X,Y) coordinates.
        // printf() can be very useful to quickly print text without using streaming notation,
        //
        // i.e. printf("This is attempt #d\n",++iAttempNo) vs. out << "This is attempt number " << ++iAttemptNo <<  "\n"
        //
        // printf(50,100,"Hello World") will print "Hello World") at (50,100) in the window and set the next write location accordingly.
        //
        void printf(int iX,int iY,const char * Format,...);
 
        // printf() -- Works the same way as 'C' printf, except you can also put (X,Y) coordinates.
        // printf() can be very useful to quickly print text without using streaming notation,
        //
        // i.e. printf("This is attempt #d\n",++iAttempNo) vs. out << "This is attempt number " << ++iAttemptNo <<  "\n"
        //
        // printf(50,100,"Hello World") will print "Hello World") at (50,100) in the window and set the next write location accordingly.
        //
        void printf(int iX,int iY,const cwfOpt & cwOpt,const char * Format,...);

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
// __Sandbox__Except__ deprecated (for now)
//
//#ifdef __Sandbox__Except__
   
        // printf() -- Works the same way as 'C' printf, except you can also put (X,Y) coordinates.
        // printf() can be very useful to quickly print text without using streaming notation,
        //
        // i.e. printf("This is attempt #d\n",++iAttempNo) vs. out << "This is attempt number " << ++iAttemptNo <<  "\n"
        //
        // printf(50,100,"Hello World") will print "Hello World") at (50,100) in the window and set the next write location accordingly.
        //
        void __printf(const char * Format,...);

        // printf() -- Works the same way as 'C' printf, except you can also put (X,Y) coordinates.
        // printf() can be very useful to quickly print text without using streaming notation,
        //
        // i.e. printf("This is attempt #d\n",++iAttempNo) vs. out << "This is attempt number " << ++iAttemptNo <<  "\n"
        //
        // printf(50,100,"Hello World") will print "Hello World") at (50,100) in the window and set the next write location accordingly.
        //
        void __printf(const cwfOpt & cwOpt,const char * Format,...);

        // printf() -- Works the same way as 'C' printf, except you can also put (X,Y) coordinates.
        // printf() can be very useful to quickly print text without using streaming notation,
        //
        // i.e. printf("This is attempt #d\n",++iAttempNo) vs. out << "This is attempt number " << ++iAttemptNo <<  "\n"
        //
        // printf(50,100,"Hello World") will print "Hello World") at (50,100) in the window and set the next write location accordingly.
        //
        void __printf(int iX,int iY,const char * Format,...);
 
        // printf() -- Works the same way as 'C' printf, except you can also put (X,Y) coordinates.
        // printf() can be very useful to quickly print text without using streaming notation,
        //
        // i.e. printf("This is attempt #d\n",++iAttempNo) vs. out << "This is attempt number " << ++iAttemptNo <<  "\n"
        //
        // printf(50,100,"Hello World") will print "Hello World") at (50,100) in the window and set the next write location accordingly.
        //
        void __printf(int iX,int iY,const cwfOpt & cwOpt,const char * Format,...);

        /// <summary>
        /// Waits for a mouse click in the window.  This is used a simple way to hold up program execution until the user
        /// clicks the mouse.
        /// <para></para>&#160;&#160;&#160;
        /// The window is set as the foreground and active window when this function is called.
        /// --> Note: This function will return if the user closes the window.
        /// </summary>
        /// <param name="sMsg"> = (optional) message to print out as text to the window.</param>
        /// <returns></returns>
        bool WaitforClick(const char * sMsg = nullptr);

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
        char WaitforKeyPress(const char * sMsg = nullptr); 

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
        bool WaitforCRPress(const char * sMsg = nullptr); 

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
        bool WaitforClickOrKey(const char * sMsg = nullptr); 

//#endif


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
    CButton & button(const char * sButtonName); // $QC

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
    CButton & button(int iButtonID); // $QC

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
    CEditBox & editbox(const char * sEditboxName); // $QC
    CInputBox & inputbox(const char * sInputboxName) { return editbox(sInputboxName); }              // $QC //$$moveme

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
    CEditBox & editbox(int iEditboxID); // $QC
    CInputBox & inputbox(int iInputBoxID) { return editbox(iInputBoxID); }; // $QC
    
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
    CSlider & slider(const char * sSliderName); // $QC

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
    CSlider & slider(int iSliderID); // $QC
    
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
    CListBox & listbox(const char * sListBoxName); // $QC

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
    CListBox & listbox(int iListBoxID); // $QC

    CComboBox & combobox(int iComboBoxID); // $QC
    CComboBox & combobox(const char * sComboBoxName); // $QC
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
    CWindow & window(const char * sWindowName); // $QC

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
    CWindow & window(int iWindowID); // $QC

    // Returns the default Windows window colors.  SageBox has its own defaults for colors, and this
    // returns the colors Windows sets windows to by default.
    //
    bool GetWindowColors(DWORD & dwFgColor,DWORD & dwBgColor); // $QC

    // Returns the default Windows window colors.  SageBox has its own defaults for colors, and this
    // returns the colors Windows sets windows to by default.
    //
    bool GetWindowColors(RGBColor_t & rgbFgColor,RGBColor_t & rgbBgColor); // $QC

    // Get the size of the desktop (i.e. monitor). 
    // This returns the current monitor size (i.e. 1920x1080).  This can help in centering and otherwize
    // placing windows and controls.
    //
    // Currently, this returns the active desktop and will be updated to work with systems with multiple monitors in a future update.
    //
    SIZE GetDesktopSize(); // $QC

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
    bool DisplayBitmap(int iX,int iY,RawBitmap32_t & stBitmap);     // $QCC

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
    bool DisplayBitmap(POINT pLoc,RawBitmap32_t & stBitmap);     // $QCC
 
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
    bool DisplayBitmap(RawBitmap32_t & stBitmap);     // $QCC

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
    bool DisplayBitmap(int iX,int iY,int iWidth,int iHeight,unsigned char * sMemory);     // $QCC

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
    bool DisplayBitmap(POINT pLoc,int iWidth,int iHeight,unsigned char * sMemory);     // $QCC

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
    bool DisplayBitmap(int iX,int iY,RawBitmap_t & stBitmap);     // $QCC

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
    bool DisplayBitmap(POINT pLoc,RawBitmap_t & stBitmap);     // $QCC
 
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
    bool DisplayBitmap(RawBitmap_t & stBitmap);     // $QCC

    // Display a bitmap upside-down.  This is often useful for bitmaps, as they are typically stored upside-down, and default
    // Windows behavior is to correct this.
    //
    // However, it is common to work with correct bitmaps that will then display upside down. 
    // DisplayBitmapR() will display a bitmap right-side up.  You can also negate the height or 
    // struture (by putting a '-' in front of it) to use the regular DisplayBitmap()
    //
    // See DisplayBitmap() for more details on this function.
    //
    bool DisplayBitmapR(int iX,int iY,int iWidth,int iHeight,unsigned char * sMemory);     // $QCC

    // Display a bitmap upside-down.  This is often useful for bitmaps, as they are typically stored upside-down, and default
    // Windows behavior is to correct this.
    //
    // However, it is common to work with correct bitmaps that will then display upside down. 
    // DisplayBitmapR() will display a bitmap right-side up.  You can also negate the height or 
    // struture (by putting a '-' in front of it) to use the regular DisplayBitmap()
    //
    // See DisplayBitmap() for more details on this function.
    //
    bool DisplayBitmapR(POINT pLoc,int iWidth,int iHeight,unsigned char * sMemory);     // $QCC

    // Display a bitmap upside-down.  This is often useful for bitmaps, as they are typically stored upside-down, and default
    // Windows behavior is to correct this.
    //
    // However, it is common to work with correct bitmaps that will then display upside down. 
    // DisplayBitmapR() will display a bitmap right-side up.  You can also negate the height or 
    // struture (by putting a '-' in front of it) to use the regular DisplayBitmap()
    //
    // See DisplayBitmap() for more details on this function.
    //
    bool DisplayBitmapR(int iX,int iY,RawBitmap_t & stBitmap);     // $QCC
    
    // Display a bitmap upside-down.  This is often useful for bitmaps, as they are typically stored upside-down, and default
    // Windows behavior is to correct this.
    //
    // However, it is common to work with correct bitmaps that will then display upside down. 
    // DisplayBitmapR() will display a bitmap right-side up.  You can also negate the height or 
    // struture (by putting a '-' in front of it) to use the regular DisplayBitmap()
    //
    // See DisplayBitmap() for more details on this function.
    //
    bool DisplayBitmapR(POINT pLoc,RawBitmap_t & stBitmap);     // $QCC
    
    // Display a bitmap upside-down.  This is often useful for bitmaps, as they are typically stored upside-down, and default
    // Windows behavior is to correct this.
    //
    // However, it is common to work with correct bitmaps that will then display upside down. 
    // DisplayBitmapR() will display a bitmap right-side up.  You can also negate the height or 
    // struture (by putting a '-' in front of it) to use the regular DisplayBitmap()
    //
    // See DisplayBitmap() for more details on this function.
    //
    bool DisplayBitmapR(RawBitmap_t & stBitmap);     // $QCC

    // --- 32-Bit Display Bitmap -- 

    // Display a bitmap upside-down.  This is often useful for bitmaps, as they are typically stored upside-down, and default
    // Windows behavior is to correct this.
    //
    // However, it is common to work with correct bitmaps that will then display upside down. 
    // DisplayBitmapR() will display a bitmap right-side up.  You can also negate the height or 
    // struture (by putting a '-' in front of it) to use the regular DisplayBitmap()
    //
    // See DisplayBitmap() for more details on this function.
    //
    bool DisplayBitmapR(int iX,int iY,RawBitmap32_t & stBitmap);     // $QCC
    
    // Display a bitmap upside-down.  This is often useful for bitmaps, as they are typically stored upside-down, and default
    // Windows behavior is to correct this.
    //
    // However, it is common to work with correct bitmaps that will then display upside down. 
    // DisplayBitmapR() will display a bitmap right-side up.  You can also negate the height or 
    // struture (by putting a '-' in front of it) to use the regular DisplayBitmap()
    //
    // See DisplayBitmap() for more details on this function.
    //
    bool DisplayBitmapR(POINT pLoc,RawBitmap32_t & stBitmap);     // $QCC
    
    // Display a bitmap upside-down.  This is often useful for bitmaps, as they are typically stored upside-down, and default
    // Windows behavior is to correct this.
    //
    // However, it is common to work with correct bitmaps that will then display upside down. 
    // DisplayBitmapR() will display a bitmap right-side up.  You can also negate the height or 
    // struture (by putting a '-' in front of it) to use the regular DisplayBitmap()
    //
    // See DisplayBitmap() for more details on this function.
    //
    bool DisplayBitmapR(RawBitmap32_t & stBitmap);     // $QCC

    // -------------------------------

    //Display a 32-bit bitmap. 
    //
    // This function is the same as DisplayBitmap(), except that it displays a 32-bit bitmap. 
    //
    // iX,iY must be specified for the location of the displayed bitmap.
    // For raw data, Width,Height, and memory pointer must also be displayed.
    //
    // See DisplayBitmap() for more information
    //
    bool DisplayBitmap32(int iX,int iY,int iWidth,int iHeight,unsigned char * sMemory);    // $QCC

    //Display a 32-bit bitmap. 
    //
    // This function is the same as DisplayBitmap(), except that it displays a 32-bit bitmap. 
    //
    // iX,iY must be specified for the location of the displayed bitmap.
    // For raw data, Width,Height, and memory pointer must also be displayed.
    //
    // See DisplayBitmap() for more information
    //
    bool DisplayBitmap32(POINT pLoc,int iWidth,int iHeight,unsigned char * sMemory);     // $QCC

    //Display a 32-bit bitmap. 
    //
    // This function is the same as DisplayBitmap(), except that it displays a 32-bit bitmap. 
    //
    // iX,iY must be specified for the location of the displayed bitmap.
    // For raw data, Width,Height, and memory pointer must also be displayed.
    //
    // See DisplayBitmap() for more information
    //
    bool DisplayBitmap32(int iX,int iY,RawBitmap32_t & stBitmap);     // $QCC
    
    //Display a 32-bit bitmap. 
    //
    // This function is the same as DisplayBitmap(), except that it displays a 32-bit bitmap. 
    //
    // iX,iY must be specified for the location of the displayed bitmap.
    // For raw data, Width,Height, and memory pointer must also be displayed.
    //
    // See DisplayBitmap() for more information
    //
    bool DisplayBitmap32(POINT pLoc,RawBitmap32_t & stBitmap);     // $QCC
 
    //Display a 32-bit bitmap. 
    //
    // This function is the same as DisplayBitmap(), except that it displays a 32-bit bitmap. 
    //
    // iX,iY must be specified for the location of the displayed bitmap.
    // For raw data, Width,Height, and memory pointer must also be displayed.
    //
    // See DisplayBitmap() for more information
    //
    bool DisplayBitmap32(RawBitmap32_t & stBitmap);     // $QCC
 
    // Blend a bitmap with a pre-defined mask
    //
    bool BlendBitmap(int iX,int iY,RawBitmap_t & stBitmap); // $QCC
    // Blend a bitmap with a pre-defined mask
    //
    bool BlendBitmap(POINT pLoc,RawBitmap_t & stBitmap); // $QCC

    // Blend a bitmap with a pre-defined mask
    //
    bool BlendBitmap(int iX,int iY,RawBitmap_t & stBitmap,RawBitmap_t & stMask); // $QCC

    // Blend a bitmap with a pre-defined mask
    //
    bool BlendBitmap(POINT pLoc,RawBitmap_t & stBitmap,RawBitmap_t & stMask); // $QCC

    // Deprecated -- Use BlendBitmap32()
    //
    bool BlendBitmapEx(Sage::RawBitmap32_t & stSource,POINT pDest,SIZE szDest); // $QCC

    // Deprecated -- Use BlendBitmapREx()
    //
    bool BlendBitmapREx(Sage::RawBitmap32_t & stSource,POINT pDest,SIZE szDest); // $QCC

    bool BlendBitmap32(Sage::RawBitmap32_t & stSource,const POINT pDest,const SIZE szDest = {}); // $QCC
    bool BlendBitmap32(const unsigned char * sBitmap,const SIZE szSize,const POINT pDest,const SIZE szDest = {}); // $QCC
    bool BlendBitmap32R(Sage::RawBitmap32_t & stSource,const POINT pDest,const SIZE szDest = {}); // $QCC
    bool BlendBitmap32R(const unsigned char * sBitmap,const SIZE szSize, const POINT pDest,const SIZE szDest = {}); // $QCC

    bool BlendDIB(HBITMAP hBitmap,const SIZE szSize,const POINT pDest,const SIZE szDest = {});

    /// <summary>
    /// Displays a transformed bitmap, that can be rotated, scaled (zoom), or flipped in any direction. Masks are also supported.
    /// <para></para>
    /// Note that the position (iX, iY, or the POINT pLoc) is the center of the image.  Rotation also occurs about the center of the image.
    /// <para></para>
    /// --> This function creates a CBMPTransform.  For multiple operations, create a CBmpBitmap transform object locally -- rotation and scale operatons can be
    /// used multiple times without allocating memory on each call (allocated memory is freed when the CBmpBitmap object goes out of scope)
    /// <para></para>
    /// --> *** note *** -- TransformBitmap() and CBmpBitmap are in an experimental stage. 
    /// </summary>
    /// <param name="iX">X location of center of image (you can also use a POINT to specify center (x,y)</param>
    /// <param name="iY">Y location of center of image (when a POINT is not used instead)</param>
    /// <param name="cBitmap">Bitmap to be displayed</param>
    /// <param name="fAngle">Angle of rotation</param>
    /// <param name="fZoom">Zoom factor</param>
    /// <param name="eFlip">Flip type, i.e. horizonal, vertical, etc.</param>
    /// <returns></returns>
    bool TransformBitmap(int iX,int iY,CBitmap & cBitmap,double fAngle,double fZoom = 1.0,FlipType eFlip = FlipType::None);

    /// <summary>
    /// Displays a transformed bitmap, that can be rotated, scaled (zoom), or flipped in any direction. Masks are also supported.
    /// <para></para>
    /// Note that the position (iX, iY, or the POINT pLoc) is the center of the image.  Rotation also occurs about the center of the image.
    /// <para></para>
    /// --> This function creates a CBMPTransform.  For multiple operations, create a CBmpBitmap transform object locally -- rotation and scale operatons can be
    /// used multiple times without allocating memory on each call (allocated memory is freed when the CBmpBitmap object goes out of scope)
    /// <para></para>
    /// --> *** note *** -- TransformBitmap() and CBmpBitmap are in an experimental stage. 
    /// </summary>
    /// <param name="iX">X location of center of image (you can also use a POINT to specify center (x,y)</param>
    /// <param name="iY">Y location of center of image (when a POINT is not used instead)</param>
    /// <param name="cBitmap">Bitmap to be displayed</param>
    /// <param name="fAngle">Angle of rotation</param>
    /// <param name="fZoom">Zoom factor</param>
    /// <param name="eFlip">Flip type, i.e. horizonal, vertical, etc.</param>
    /// <returns></returns>
    bool TransformBitmap(POINT pLoc,CBitmap & cBitmap,double fAngle,double fZoom = 1.0,FlipType eFlip = FlipType::None);

    // StretchBitmap() -- Display a stretched bitmap to the window.  
    //
    // The source bitmap can be stretched to any size, and from any portion of the bitmap.
    // 
    // Important Note: If the szSource (source rectangle from which to draw from the original bitmap) is different from 
    // the size of the bitmap, szSourceBitmap MUST BE INCLUDED with the size of the entire source bitmap.  Otherwise the function
    // will fail.  When specifyin a CBitmap or RawBitmap_t as the source, szSourceBitmap does not need to be filled.
    // szSourceBitmap only needs to be filled when sending raw bitmap data and the source size differs from the source bitmap size.
    //
    bool StretchBitmap(unsigned char * sMemory,POINT pDest,SIZE szDest,POINT pSrc, SIZE szSource,SIZE szSourceBitmap = {0,0}); // $QCC
    bool StretchBitmap(CBitmap & cBitmap,POINT pDest,SIZE szDest);  // $QCC
    bool StretchBitmap(CBitmap & cBitmap,POINT pDest,SIZE szDest,POINT pSrc, SIZE szSource); // $QCC

    bool StretchBitmapR(unsigned char * sMemory,POINT pDest,SIZE szDest,POINT pSrc, SIZE szSource,SIZE szSourceBitmap = {0,0}); // $QCC
    bool StretchBitmapR(CBitmap & cBitmap,POINT pDest,SIZE szDest);  // $QCC
    bool StretchBitmapR(CBitmap & cBitmap,POINT pDest,SIZE szDest,POINT pSrc, SIZE szSource); // $QCC

    bool StretchBitmap32(unsigned char * sMemory,POINT pDest,SIZE szDest,POINT pSrc, SIZE szSource,SIZE szSourceBitmap = {0,0}); // $QCC
    bool StretchBitmap32(RawBitmap32_t & cBitmap,POINT pDest,SIZE szDest);  // $QCC
    bool StretchBitmap32(RawBitmap32_t & cBitmap,POINT pDest,SIZE szDest,POINT pSrc, SIZE szSource); // $QCC

    bool StretchBitmap32R(unsigned char * sMemory,POINT pDest,SIZE szDest,POINT pSrc, SIZE szSource,SIZE szSourceBitmap = {0,0}); // $QCC
    bool StretchBitmap32R(RawBitmap32_t & cBitmap,POINT pDest,SIZE szDest);  // $QCC
    bool StretchBitmap32R(RawBitmap32_t & cBitmap,POINT pDest,SIZE szDest,POINT pSrc, SIZE szSource); // $QCC

    bool StretchBitmap(RawBitmap32_t & cBitmap,POINT pDest,SIZE szDest);  // $QCC
    bool StretchBitmap(RawBitmap32_t & cBitmap,POINT pDest,SIZE szDest,POINT pSrc, SIZE szSource); // $QCC

    bool StretchBitmapR(RawBitmap32_t & cBitmap,POINT pDest,SIZE szDest);  // $QCC
    bool StretchBitmapR(RawBitmap32_t & cBitmap,POINT pDest,SIZE szDest,POINT pSrc, SIZE szSource); // $QCC



    bool DisplayBitmapEx(unsigned char * sMemory,POINT pDest,POINT pSrc, SIZE szSize,SIZE szSourceBitmap); // $QCC
    bool DisplayBitmapEx(CBitmap & cBitmap,POINT pDest,SIZE szSize);  // $QCC
    bool DisplayBitmapEx(CBitmap & cBitmap,POINT pDest,POINT pSrc, SIZE szSize); // $QCC

    bool DisplayBitmapExR(unsigned char * sMemory,POINT pDest,POINT pSrc, SIZE szSize,SIZE szSourceBitmap); // $QCC
    bool DisplayBitmapExR(CBitmap & cBitmap,POINT pDest,SIZE szSize);  // $QCC
    bool DisplayBitmapExR(CBitmap & cBitmap,POINT pDest,POINT pSrc, SIZE szSize); // $QCC


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
    HFONT PushFont(const char * sFont); // $QC

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
    HFONT PushFont(HFONT hFont = nullptr); // $QC

    // PopFont() -- Pop a pushed font and restore the font to the active window font.
    // See PushFont() for more details.
    //
    HFONT PopFont(int iNumPop = 0);  // $QC

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
    bool PushColor(DWORD dwFgColor,DWORD dwBgColor = -1); // $QC

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
    bool PushColor(RGBColor_t rgbFgColor = { -1,-1, -1},RGBColor_t rgbBgColor = {-1,-1,-1}); // $QC

    // PushBgColor() -- Push the current Background color for later retrieval.
    // This can also be used to simultaneously set the background color.
    //
    // This function is the same as PushColor() but works on the background color only.
    //
    // See PushColors() for more information.
    //
    bool PushBgColor(DWORD dwFgColor = -1); // $QC

    // PushBgColor() -- Push the current Background color for later retrieval.
    // This can also be used to simultaneously set the background color.
    //
    // This function is the same as PushColor() but works on the background color only.
    //
    // See PushColors() for more information.
    //
    bool PushBgColor(RGBColor_t rgbColor = {-1,-1,-1}); // $QC

    // Pop a color or colors pushed on to the PushColor Stack. 
    // See PushColor() for more informtion.
    //
    bool PopColor(int iNumPop = 1); // $QC

    // GetDefaultBgColor() -- return default Background color for windows created by SageBox
    //
    RGBColor_t GetDefaultBgColor(); // $QC

    // GetDefaultBgColor() -- return default Foreground color for windows created by SageBox
    //
    RGBColor_t GetDefaultFgColor(); // $QC
 
    // Return the Current Font. 
    // This returns a Windows HFONT which can be used in all Font functions
    //
    HFONT GetCurrentFont(); // $QC

    // Set the Font to the Default Font for the Window.
    //
    HFONT SetDefaultFont(); // $QC

    // Get the Default Font for the window.
    // This only returns the font but does not set it.
    //
    HFONT GetDefaultFont(); // $QC

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
    HFONT SetFont(HFONT hFont);    // $QC

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
    HFONT SetFont(int iFontSize);    // $QC

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
    HFONT SetFont(const char * sFont,const char * sNewFontName = nullptr,unsigned char * ucStatus = nullptr);       // $QC

    // GetFont() -- Get a font created through SageBox.
    //
    // Example, GetFont("MyFont") -- retrieves a font created with CreateFont("Arial,20","TestFont"), or SetFont()
    //
    // The Font returned is an HFONT which can be used with all font functions
    //
    HFONT SetFont(WCHAR * wsFont,WCHAR * wsNewFontName = nullptr,unsigned char * ucStatus = nullptr);       // $QC

    // GetFont() -- Get a font created through SageBox.
    //
    // Example, GetFont("MyFont") -- retrieves a font created with CreateFont("Arial,20","TestFont"), or SetFont()
    //
    // The Font returned is an HFONT which can be used with all font functions
    //
    HFONT GetFont(const char * sFont,const char * sNewFontName = nullptr,unsigned char * ucStatus = nullptr);     // $QC

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
    HFONT CreateNewFont(const char * sFont,const char * sNewFontName = nullptr,unsigned char * ucStatus = nullptr);     // $QC

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
    __forceinline HFONT AddFont(const char * sFont,const char * sNewFontName = nullptr,unsigned char * ucStatus = nullptr) { return CreateNewFont(sFont,sNewFontName,ucStatus); }   // $QC
    // Returns true of the middle mouse button mouse was double clicked.
    // *** This function is still in development and may not work.
    // This is tied to the status of the window an whether or not it will accept double-clicks.
    //
    bool MouseDoubleClicked(); // $QCC

    /// <summary>
    /// Returns true if the Left Mouse Button is currently pressed. 
    /// <para></para>
    /// This is not a mouse event and returns the real-time status of the mouse.
    /// </summary>
    /// <param name="pMouse"> - (optional POINT) fills pMouse with current mouse location </param>
    /// <returns></returns>
    bool MouseButtonDown(); // $QCC

    /// <summary>
    /// Returns true if the Left Mouse Button is currently pressed. 
    /// <para></para>
    /// This is not a mouse event and returns the real-time status of the mouse.
    /// </summary>
    /// <param name="pMouse"> - (optional POINT) fills pMouse with current mouse location </param>
    /// <returns></returns>
    bool MouseButtonDown(POINT & pMouse); // $QCC

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
    bool KeyPressed(char & cKey,Peek peek = Peek::No);  // $QCC

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
    char KeyPressed(Peek peek = Peek::No); // $QCC

    // Returns true if the Right Mouse Button is currently pressed.  
    // This is not a mouse event and returns the real-time status of the mouse.
    //
    bool MouseRButtonDown(); // $QCC
    bool MouseRButtonClicked(POINT & pPoint,Peek peek = Peek::No); // $QCC
    bool MouseRButtonClicked(Peek peek = Peek::No); // $QCC

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
    bool MouseMoved(Peek peek = Peek::No); // $QCC

    // Experimental
    //
    std::optional<POINT> MouseMoved17(Peek peek = Peek::No)
    {
        std::optional<POINT> p;
        POINT pMouse;
        if (MouseMoved(pMouse,peek)) p = pMouse;
        return p;
    }

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
    bool MouseMoved(POINT & pPoint,Peek peek = Peek::No); // $QCC


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
    int ButtonPressed(Peek peek = Peek::No); // $QCC

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
    bool ButtonPressed(int & iButtonID,Peek peek = Peek::No); // $QCC

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
    bool MouseClicked(Peek peek = Peek::No); // $QCC

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
    bool MouseClicked(POINT & pMouse,Peek peek = Peek::No); // $QCC
  
    bool MouseUnclicked(Peek peek = Peek::No); 
    bool MouseUnclicked(POINT & pMouse,Peek peek = Peek::No); 

    /// <summary>
    /// Returns true of a Mouse Drag Event has occurred.  This is an event.  Therefore, true is returned once per mouse drag event.  False is returned
    /// afterward until the next mouse drag event.
    /// <para></para>
    /// A "drag event" is defined by the movement of the mouse while the left button is pressed.
    /// <para></para>
    /// MouseDrag event start, current, and previous position can be found through other MouseDrag functions
    /// <para></para>
    /// </summary>
    /// <param name="pMouse"> - (optional) Include POINT pMouse to fill the mouse position of the drag event when TRUE is returned.</param>
    /// <param name="bIncludeClick"> - (optional) Includes the initial button click as a Drag Event (so ButtonClicked() does not need to be called separately)</param>
    /// <param name="peek"> - (optional) including Peek::Yes will cause the event flag to remain unset, allowing the next call to return a TRUE status.</param>
    /// <returns></returns>
    bool MouseDragEvent(Peek peek = Peek::No); // $QCC

    /// <summary>
    /// Returns true of a Mouse Drag Event has occurred.  This is an event.  Therefore, true is returned once per mouse drag event.  False is returned
    /// afterward until the next mouse drag event.
    /// <para></para>
    /// A "drag event" is defined by the movement of the mouse while the left button is pressed.
    /// <para></para>
    /// MouseDrag event start, current, and previous position can be found through other MouseDrag functions
    /// <para></para>
    /// </summary>
    /// <param name="pMouse"> - (optional) Include POINT pMouse to fill the mouse position of the drag event when TRUE is returned.</param>
    /// <param name="bIncludeClick"> - (optional) Includes the initial button click as a Drag Event (so ButtonClicked() does not need to be called separately)</param>
    /// <param name="peek"> - (optional) including Peek::Yes will cause the event flag to remain unset, allowing the next call to return a TRUE status.</param>
    /// <returns></returns>
    bool MouseDragEvent(bool bIncludeClick,Peek peek = Peek::No); // $QCC

    /// <summary>
    /// Returns true of a Mouse Drag Event has occurred.  This is an event.  Therefore, true is returned once per mouse drag event.  False is returned
    /// afterward until the next mouse drag event.
    /// <para></para>
    /// A "drag event" is defined by the movement of the mouse while the left button is pressed.
    /// <para></para>
    /// MouseDrag event start, current, and previous position can be found through other MouseDrag functions
    /// <para></para>
    /// </summary>
    /// <param name="pMouse"> - (optional) Include POINT pMouse to fill the mouse position of the drag event when TRUE is returned.</param>
    /// <param name="bIncludeClick"> - (optional) Includes the initial button click as a Drag Event (so ButtonClicked() does not need to be called separately)</param>
    /// <param name="peek"> - (optional) including Peek::Yes will cause the event flag to remain unset, allowing the next call to return a TRUE status.</param>
    /// <returns></returns>
    bool MouseDragEvent(POINT & pMouse,Peek peek = Peek::No); // $QCC

    /// <summary>
    /// Returns true of a Mouse Drag Event has occurred.  This is an event.  Therefore, true is returned once per mouse drag event.  False is returned
    /// afterward until the next mouse drag event.
    /// <para></para>
    /// A "drag event" is defined by the movement of the mouse while the left button is pressed.
    /// <para></para>
    /// MouseDrag event start, current, and previous position can be found through other MouseDrag functions
    /// <para></para>
    /// </summary>
    /// <param name="pMouse"> - (optional) Include POINT pMouse to fill the mouse position of the drag event when TRUE is returned.</param>
    /// <param name="bIncludeClick"> - (optional) Includes the initial button click as a Drag Event (so ButtonClicked() does not need to be called separately)</param>
    /// <param name="peek"> - (optional) including Peek::Yes will cause the event flag to remain unset, allowing the next call to return a TRUE status.</param>
    /// <returns></returns>
    bool MouseDragEvent(POINT & pMouse,bool bIncludeClick,Peek peek = Peek::No); // $QCC

    /// <summary>
    /// Returns the last (current) Mouse Drag Event mouse position, i.e. the position the mouse was at when the mouse moved while the left mouse button was pressed.
    /// <para></para>
    /// If the mouse button was clicked with no subsequent mousemove, both MouseDragPos() and MouseDragPrev() will return the mouse position of the mouse button click.
    /// <para></para>
    /// --> See MouseDragPrev() to return the mouse position before the current position.
    /// <para></para>
    /// --> *** Important *** Call MouseDragPrev() only once per cycle and before MouseDragPos(), as each call to MouseDragPrev() increments the 
    /// "previous" point to the same point as MouseDragPos()
    /// </summary>
    /// <returns>Mouse position of current/last Drag Event</returns>
    POINT GetMouseDragPos();      // $QCC
    POINT MouseDragPos();      // $QCC

    /// <summary>
    /// Returns the previous (one before current) Mouse Drag Event mouse position.
    /// <para></para>
    /// If the mouse button was clicked with no subsequent mousemove, both MouseDragPos() and MouseDragPrev() will return the mouse position of the mouse button click.
    /// <para></para>
    /// --> See MouseDragPos() to return the mouse position of the current (i.e. last) Drag Event
    /// <para></para>
    /// --> In combination with MouseDragPos(), this allows connecting points from mouse movements.  When a mouse drag event occurs, the 
    /// current positon in MouseDragPos() is copied to MouseDragPrev().
    /// <para></para>
    /// --> *** Important *** Call MouseDragPrev() only once per cycle and before MouseDragPos(), as each call to MouseDragPrev() increments the 
    /// "previous" point to the same point as MouseDragPos()
    /// </summary>
    /// <returns>Mouse position of previous Drag Event</returns>
    POINT GetMouseDragPrev();      // $QCC
    POINT MouseDragPrev();      // $QCC

    bool MouseDragEnded(Peek peek = Peek::No); // $QCC
    bool MouseDragEnded(POINT & pMouse,Peek peek = Peek::No); // $QCC

    bool isMouseDragging();  // $QCC
    bool isMouseDragging(POINT & pStartDrag);  // $QCC

    bool StartMouseDrag(POINT pPoint); // $QCC

    bool MouseDragReset();  // $QCC

    POINT GetMouseDragStart(); // $QCC
    POINT MouseDragStart(); // $QCC

    // GetMousePos() -- Returns the current mouse coordinates relative to the window
    //
    bool GetMousePos(int & iMouseX,int & iMouseY);  // $QCC

    // GetMousePos() -- Returns the current mouse coordinates relative to the window
    //
    POINT GetMousePos(); // $QCC

    // GetMouseClickPos() -- Returns the last mouse click coordinates.
    //
    // This can be used after a MouseClick() event to return the mouse-click coordinates
    // at the time the mouse was clicked.
    //
    // This works for both left button and right button clicks.
    //
    bool GetMouseClickPos(int & iMouseX,int & iMouseY);  // $QCC

    // GetMouseClickPos() -- Returns the last mouse click coordinates.
    //
    // This can be used after a MouseClick() event to return the mouse-click coordinates
    // at the time the mouse was clicked.
    //
    // This works for both left button and right button clicks.
    //
    POINT GetMouseClickPos(); // $QCC


#ifdef kCPP17Functions
        std::optional<int> MouseWheelMoved(Peek peek = Peek::No);       // $QCC
#else
    bool MouseWheelMoved(Peek peek = Peek::No); // $QCC
#endif
    bool MouseWheelMoved(int & iDistance,Peek peek = Peek::No); // $QCC
    int GetMouseWheelMove(bool bResetEvent = true); // $QCC
    bool WindowResized(SIZE & szNewWinSize,Peek peek = Peek::No); // $QC
    bool WindowResized(Peek peek = Peek::No); // $QC
    bool WindowResizing(Peek peek = Peek::No);
    bool WindowResizing(SIZE & szNewWinSize,Peek peek = Peek::No);

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
    ConsoleOp_t SetWritePos(POINT pLoc); // $QC


    // SetWritePosX() -- Set the output X position in the window for writing text. 
    //
    // This sets only the X position in the ouput for printf, Write(), etc. -- anything that prints text.
    // The current Y position is not changed.
    //
    // See SetWritePos() for more information; 
    //
    ConsoleOp_t SetWritePosX(int iX); // $QC

    // SetWriteIndent() -- Sets the indent for text printing functions when a cr/lf "\n" is returned.
    // This allows informally indenting printing.  
    //
    // This only affects where the X position in the console output gets reset to when the "\n" is seen.
    // All other write(), printf(), SetPos(), etc. functions work as normal. 
    // Use SetIndent()  (without arguments to reset it). 
    //
    void SetWriteIndent(int iIndent = 0);

    /// <summary>
    /// Sets the vertical padding for each line written in the window. 
    /// <para></para>
    /// This allows adding vertical space per-line when writing out to the window.
    /// </summary>
    /// <param name="iPad"></param>
    void SetWritePadding(int iPad);

    // SetBkMode() -- Set the text background mode.
    // BkMode returns eeither BkMode::Transparent or BkMode::Opaque
    //
    // BkMode::Transparent()  --- This is the default.  When text is written to the screen, only the text part is written in the
    //                            foreground color, leaving the blank areas (i.e. text or graphics behind the new text) untouched.
    //
    // BkMode::Opaque         -- When text is written to the window, this will write the text in the foreground color and fill in the background with the
    //                           background color overwriting any text or graphics behind the new text.
    //
    void SetBkMode(BkMode eBkType); // $QC

    // GetBkMode() -- Get the text background mode.
    // BkMode returns eeither BkMode::Transparent or BkMode::Opaque
    //
    // BkMode::Transparent()  --- This is the default.  When text is written to the screen, only the text part is written in the
    //                            foreground color, leaving the blank areas (i.e. text or graphics behind the new text) untouched.
    //
    // BkMode::Opaque         -- When text is written to the window, this will write the text in the foreground color and fill in the background with the
    //                           background color overwriting any text or graphics behind the new text.
    //
    BkMode GetBkMode(); // $QC

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
    bool SetWordWrap(bool bWrap = true); // $QC

    // GetWordWrap() -- returns the current word-wrap status for the window (see SetWordWrap());
    //
    bool GetWordWrap(); // $QC

    // Sets the Program/Application name.  This can also be set when initiating Sagebox, i.e. CSageBox("My Application"); 
    //
    // The Application name is used in various window title bars in SageBox.  With no Program Name set, either the window title is blank or a default
    // text string is used.
    //
    void SetProgramName(char * sProgramName); // $QC

    // Gets the program name set in SageBox through either the SageBox constructor (i.e. CSageBox("My Application") or through GetProgramName();
    //
    const char * GetProgramName(); // $QC

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
    bool WindowClosing();       // $QC

    // Reset Window Closing Status.   
    //
    // This resets the Windows Closing status returned by WindowClosing().  Many Sagebox functions will not activate or passthrough when the
    // WindowClosing() status is set to true.
    //
    // This can be used when a user has pressed the close button and CloseButtonPessed() returns true.
    // To cancel the user-based close, call ResetWindowClosing()
    //
    bool ResetWindowClosing(); // $QC
    bool SetWindowClosing(bool bPressCloseButton = false); // $QC
    bool PressCloseButton();  // $QC

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
    bool PressButtonOnClose(CButton & cButton); // $QC

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
    bool CloseButtonPressed(Peek peek = Peek::No); // $QCC

    // PeekCloseButtonPressed() -- Use to determine if the close button was pressed before 
    //
    // This is a shortcut for CLoseButtonPressed(true)
    // See CloseButtonPressed() for more Informatuion
    //
    bool PeekCloseButtonPressed(); // $QCC

    // getCharWidth() -- get the average character width for the current window font.
    // For proportional fonts, this returns the average chacter width for the font. 
    // For termainl fonts (i.e. Courier New, etc.) this returns the width for all characters.
    //
    int getCharWidth(); // $QC

    // getCharHeight() -- Returns the height of the characters for the current window font.  This is 
    // the lineheight for each line printed. 
    //
    int getCharHeight(); // $QC

    // getCharSize() -- returns the average character width and exact height for the current window font
    // used for all text-based input and output functions.
    //
    SIZE getCharSize(const char * sFont); // $QC

    // getCharSize() -- returns the average character width and exact height for the current window font
    // used for all text-based input and output functions.
    //
    SIZE getCharSize(HFONT hFont = nullptr); // $QC

    // GetBgColor() -- Get the current backround color for the window.
    // This color is used for Cls() and all text output routines.
    //
    RGBColor_t GetBgColor(); // $QC

    // GetFgColor() -- Get the current foreground (i.e. Text) color for the window.
    // This color is used all text output routines.
    //
    RGBColor_t GetFgColor(); // $QC

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
    HDC GetCurDC(); // $QC

    // Get the Windows Desktop Device Context of the current window, i.e. the static Device Context.
    //
    // This returns the Windows Device Context (HDC) for the actual Window rather than the bitmap that Sagebox uses
    // for output. 
    // 
    // See GetCurDC() for more information
    //
    HDC GetDesktopDC(); // $QC

    // Get the Windows Device Context for the internal bitmap Sagebox uses for output functions (i.e. Write(), printf(), drawing, etc.)
    //
    // See GetDesktopDC() to get the Windows Device Context for the actual window rather than the internal bitmap (canvas).
    // 
    // See GetCurDC() for more information
    //
    HDC GetBitmapDC(); // $QC

    /// <summary>
    /// Returns the Windows Bitmap handle for the Bitmap currently selected in the window client area.
    /// <para></para>
    /// Also See: GetBitmapDC();
    /// </summary>
    /// <returns></returns>
    HBITMAP GetBitmapHandle(); // $QC

    // GetWritePos() -- Returns the current X,Y output position for all text-based functions.
    //
    bool GetWritePos(POINT & pLocation);

    // GetWritePos() -- Returns the current X,Y output position for all text-based functions.
    //
    POINT GetWritePos(); // $QC

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
    CWindow & _BitmapWindow(RawBitmap_t & stBitmap,const  cwfOpt & cwOpt = cwfOpt()); // $QC

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
    CWindow & _BitmapWindow(CBitmap & cBitmap,const  cwfOpt & cwOpt= cwfOpt()); // $QC

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
    CWindow & _BitmapWindow(int iX,int iY,CBitmap & cBitmap,const cwfOpt & cwOpt= cwfOpt()); // $QC

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
    CWindow & _BitmapWindow(POINT pLoc,CBitmap & cBitmap,const cwfOpt & cwOpt= cwfOpt()); // $QC

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
    CWindow & _BitmapWindow(int iX,int iY,RawBitmap_t & stBitmap,const  cwfOpt & cwOpt= cwfOpt()); // $QC

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
    CWindow & _BitmapWindow(POINT pLoc,RawBitmap_t & stBitmap,const  cwfOpt & cwOpt= cwfOpt()); // $QC

    // -----------------------------------------------------------------------------------------
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
    CWindow & _BitmapWindowR(RawBitmap_t & stBitmap,const  cwfOpt & cwOpt = cwfOpt()); // $QC

    // BitmapWindowR() -- Create a window (popup or embedded) designed to show bitmaps.
    //                     (Bitmap displays upside-down compared to BitmapWindow() call)
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
    CWindow & _BitmapWindowR(CBitmap & cBitmap,const  cwfOpt & cwOpt= cwfOpt()); // $QC

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
    CWindow & _BitmapWindowR(int iX,int iY,CBitmap & cBitmap,const cwfOpt & cwOpt= cwfOpt()); // $QC

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
    CWindow & _BitmapWindowR(POINT pLoc,CBitmap & cBitmap,const cwfOpt & cwOpt= cwfOpt()); // $QC

    // BitmapWindowR() -- Create a window (popup or embedded) designed to show bitmaps.
    //                    (Bitmap displays upside-down compared to BitmapWindow() call)
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
    CWindow & _BitmapWindowR(int iX,int iY,RawBitmap_t & stBitmap,const  cwfOpt & cwOpt= cwfOpt()); // $QC

    // BitmapWindowR() -- Create a window (popup or embedded) designed to show bitmaps.
    //                    (Bitmap displays upside-down compared to BitmapWindow() call)
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
    CWindow & _BitmapWindowR(POINT pLoc,RawBitmap_t & stBitmap,const  cwfOpt & cwOpt= cwfOpt()); // $QC

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
    CWindow & ChildWindow(int iX,int iY,int iWidth,int iHeight,const cwfOpt & cwOpt = cwfOpt());             // $QC
    CWindow & ChildWindow(POINT pLoc,SIZE szSize,const cwfOpt & cwOpt = cwfOpt());                           // $QC

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
    CWindow & ChildWindow(CWindow * cWindow,int iX,int iY,int iWidth,int iHeight,const cwfOpt & cwOpt = cwfOpt());      // $QC
    CWindow & ChildWindow(CWindow * cWindow,POINT pLoc,SIZE szSize,const cwfOpt & cwOpt = cwfOpt());                    // $QC

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
    CWindow & NewWindow(int iX,int iY,int iWidth,int iHeight,const char * sTitle = nullptr,const cwfOpt & cwOpt = cwfOpt()); // $QC

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
    CWindow & NewWindow(int iX,int iY,int iWidth,int iHeight,const cwfOpt & cwOpt); // $QC

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
    CWindow & NewWindow(int iX,int iY,const char * sTitle = nullptr,const cwfOpt & cwOpt = cwfOpt()); // $QC

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
    CWindow & NewWindow(int iX,int iY,const cwfOpt & cwOpt); // $QC


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
    CWindow & NewWindow(CWindow * cWin,int iX,int iY,int iWidth,int iHeight,const char * sTitle = nullptr,const cwfOpt & cwOpt = cwfOpt()); // $QC

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
    CWindow & NewWindow(CWindow * cWin,int iX,int iY,int iWidth,int iHeight,const cwfOpt & cwOpt); // $QC

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
    CWindow & NewWindow(CWindow * cWin,int iX,int iY,const char * sTitle = nullptr,const cwfOpt & cwOpt = cwfOpt()); // $QC

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
    CWindow & NewWindow(CWindow * cWin,int iX,int iY,const cwfOpt & cwOpt); // $QC


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
    RGBColor_t MakeColor(const char * sColor,DWORD rgbColor); // $QC

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
    RGBColor_t MakeColor(const char * sColor,RGBColor_t rgbColor); // $QC

    // Get a named color.  This returns an RGBColor_t (or DWORD -- see prototypes) of a named color.
    // 
    // Example:
    //
    // auto rgbBlue = GetColor("Blue");                 -- Get RGBColor_t value for Blue
    // RGBColor_t rgbMyColor = GetColor("MyColor");     -- Get RGBColor_t value for "MyColor" created previously.
    //
    RGBColor_t  GetColor(const char * sColor,bool * pColorFound = nullptr); // $QC

    // Get a named color.  This returns an RGBColor_t (or DWORD -- see prototypes) of a named color.
    // 
    // Example:
    //
    // auto rgbBlue = GetColor("Blue");                 -- Get RGBColor_t value for Blue
    // RGBColor_t rgbMyColor = GetColor("MyColor");     -- Get RGBColor_t value for "MyColor" created previously.
    //
    bool GetColor(const char * sColor,DWORD & rgbColor); // $QC



    // SetAutoBuffer() -- Sets the screen to buffer only when necessary.
    // *** This is experimental and meant for faster displays.  See SetWindowBuffering();
    //
    void SetAutoBuffer(bool bAutoBuffer); // $QC

    // SetDirectDraw() -- Sets whether or not the window is bufferred to a bitmap before sending output to the screen. 
    //
    // By default, all SageBox windows are bufferred (i.e. Direct Draw is false), meaning that all display actions are performed on a bitmap and then
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
    void SetDirectDraw(bool bDirectDraw); // $QC
    void ThunkDirectDraw();

    bool isDirectDraw();                    // $QC
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
    void SetAutoUpdate(bool bAuto = true); // $QC

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
    void SetAutoUpdate(AutoUpdateType update); // $QC
 
    bool UpdatesOn();

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
    CButton & NewButton(int ix,int iy,int iWidth,int iHeight,const char * sButtonText,const cwfOpt & cwOpt = cwfOpt()); // $QC

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
    CButton & NewButton(int ix,int iy,int iWidth,const char * sButtonText,const cwfOpt & cwOpt = cwfOpt()); // $QC

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
    CButton & NewButton(int ix,int iy,const char * sButtonText,const cwfOpt & cwOpt = cwfOpt()); // $QC

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
    CButton & NewButton(CButton * cUserButton,int ix,int iy,int iWidth,int iHeight,const char * sButtonText,const cwfOpt & cwOpt = cwfOpt()); // $QC

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
    CButton & NewButton(CButton * cUserButton,int ix,int iy,int iWidth,const char * sButtonText,const cwfOpt & cwOpt = cwfOpt()); // $QC

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
    CButton & NewButton(CButton * cUserButton,int ix,int iy,const char * sButtonText,const cwfOpt & cwOpt = cwfOpt()); // $QC

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
    CButton & NewButton(int ix,int iy,int iWidth,int iHeight,const wchar_t * sButtonText,const cwfOpt & cwOpt = cwfOpt()); // $QC

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
    CButton & NewButton(int ix,int iy,int iWidth,const wchar_t * sButtonText,const cwfOpt & cwOpt = cwfOpt()); // $QC

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
    CButton & NewButton(int ix,int iy,const wchar_t * sButtonText,const cwfOpt & cwOpt = cwfOpt()); // $QC

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
    CButton & NewButton(CButton * cUserButton,int ix,int iy,int iWidth,int iHeight,const wchar_t * sButtonText,const cwfOpt & cwOpt = cwfOpt()); // $QC

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
    CButton & NewButton(CButton * cUserButton,int ix,int iy,int iWidth,const wchar_t * sButtonText,const cwfOpt & cwOpt = cwfOpt()); // $QC

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
    CButton & NewButton(CButton * cUserButton,int ix,int iy,const wchar_t * sButtonText,const cwfOpt & cwOpt = cwfOpt()); // $QC

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
    CButton & NewCheckbox(int ix,int iy,int iWidth,int iHeight,const char * sName,const cwfOpt & cwOpt = cwfOpt()); // $QC

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
    CButton & NewCheckbox(int ix,int iy,int iWidth,const char * sName,const cwfOpt & cwOpt = cwfOpt()); // $QC

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
    CButton & NewCheckbox(int ix,int iy,const char * sName,const cwfOpt & cwOpt = cwfOpt()); // $QC

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
    CButton & NewCheckbox(CButton * cUserButton,int ix,int iy,int iWidth,int iHeight,const char * sName,const cwfOpt & cwOpt = cwfOpt()); // $QC

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
    CButton & NewCheckbox(CButton * cUserButton,int ix,int iy,int iWidth,const char * sName,const cwfOpt & cwOpt = cwfOpt()); // $QC

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
    CButton & NewCheckbox(CButton * cUserButton,int ix,int iy,const char * sName,const cwfOpt & cwOpt = cwfOpt()); // $QC

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
    CButton & NewRadioButton(int ix,int iy,int iWidth,int iHeight,const char * sName,const cwfOpt & cwOpt = cwfOpt()); // $QC

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
    CButton & NewRadioButton(int ix,int iy,int iWidth,const char * sName,const cwfOpt & cwOpt = cwfOpt()); // $QC

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
    CButton & NewRadioButton(int ix,int iy,const char * sName,const cwfOpt & cwOpt = cwfOpt()); // $QC

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
    CButton & NewRadioButton(CButton * cUserButton,int ix,int iy,int iWidth,int iHeight,const char * sName,const cwfOpt & cwOpt = cwfOpt()); // $QC

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
    CButton & NewRadioButton(CButton * cUserButton,int ix,int iy,int iWidth,const char * sName,const cwfOpt & cwOpt = cwfOpt()); // $QC

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
    CButton & NewRadioButton(CButton * cUserButton,int ix,int iy,const char * sName,const cwfOpt & cwOpt = cwfOpt()); // $QC

    // Sets the location if the window on the desktop
    //
    bool SetWinLocation(POINT pLocation); // $QC

    // Sets the location if the window on the desktop
    //
    bool SetWinLocation(int iX,int iY); // $QC

    // Gets the location of the window on the desktop
    //
    POINT GetWinLocation(); // $QC

    // Gets the HWND handle to the window that Windows uses and originally assigned to the window.
    // This can be used to perform windows functions directly.
    //
    // Also see GetCurDC()
    //
    HWND GetWindowHandle();         // $QC

    // ** This is experimental **
    // Clears all event flags before entering WaitforEventLoop()
    // Again, experimental and may be deprecated.
    //
    bool ClearFlags(); // $QC

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
    SIZE GetWindowSize(bool bFrameSize = false);    // $QC

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
    SIZE Size(bool bFrameSize = false);    // $QC
    // Returns the ID assigned to the Window when ID() was used to create the window.
    //
    int GetID(); // $QC

    // Returns the Name assigned to the window when Name() was used to create the window
    //
    const char * GetName(); // $QC

    // Sets a hover message to be shown when the mouse hovers over the window
    //
    bool SetHoverMsg(const char * sHoverMessage); // $QC

    // The the X coordinare of the left part of the window
    //
    int    StartX(); // $QC

    // The the Y coordinae of the upper part of the window
    //
    int    StartY(); // $QC
   
    // The the X coordinare of the right part of the last window
    //
    int    EndX(); // $QC

    // The the Y coordinare of the bottom part of the window
    //
    int    EndY(); // $QC

    // GetWindowWidth() - returns the width of the displayed canvas of the window.
    // Use GetWindoWidth(true) to get the full width of the widow, including the frame.
    // See GetWindowSize() to return both Width and Height in SIZE structure.
    //
    int GetWindowWidth(bool bFrameSize = false);    // $QC

    // GetWindowWidth() - returns the width of the displayed canvas of the window.
    // Use GetWindoWidth(true) to get the full width of the widow, including the frame.
    // See GetWindowSize() to return both Width and Height in SIZE structure.
    //
    int Width(bool bFrameSize = false);    // $QC

    // GetWindowHeight() - returns the height of the displayed canvas of the window.
    // Use GetWindowHeight(true) to get the full width of the height, including the frame.
    // See GetWindowSize() to return both Width and Height in SIZE structure.
    //
    int GetWindowHeight(bool bFrameSize = false);   // $QC

   // GetWindowHeight() - returns the height of the displayed canvas of the window.
    // Use GetWindowHeight(true) to get the full width of the height, including the frame.
    // See GetWindowSize() to return both Width and Height in SIZE structure.
    //
    int Height(bool bFrameSize = false);   // $QC

    // Returns true if the Window is showing on the desktop.  False is returned if the Window is invisible.
    //
    bool isVisible(); // $QC

    // Returns true of the window object is a valid window.
    // False is returned if not.  This can happen when a window is retrieved that is not valid, or a 
    // Window has been closed. 
    //
    // For example using auto& MyWindow = window("MyWindow") will return an empty window if a window
    // with the Name("MyWindow") does not exist.  In which case, isValid() can be used to determine this is an empty window.
    //
    // isValid() and isWindowValid() are the samne function.
    //
    bool isValid(); // $QC

   // Returns true of the window object is a valid window.
    // False is returned if not.  This can happen when a window is retrieved that is not valid, or a 
    // Window has been closed. 
    //
    // For example using auto& MyWindow = window("MyWindow") will return an empty window if a window
    // with the Name("MyWindow") does not exist.  In which case, isValid() can be used to determine this is an empty window.
    //
    // isValid() and isWindowValid() are the samne function.
    //
    __forceinline bool isWindowValid() { return isValid(); }; // $QC

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
    CBitmap GetWindowBitmap(POINT pLoc,SIZE szSize,bool bDesktopView = false); // $QC
    
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
    CBitmap GetWindowBitmap(bool bDesktopView = false); // $QC


    /// <summary>
    /// Sendes a generic message to a window.  This is used to send a Sage Event to a window and wake up a GetEvent() function waiting for a message
    /// <para></para>&#160;&#160;&#160;
    /// This is used when you want to have a message routine check for any status change.  This is sent to the window waiting for a message, either with GetEvent()
    /// or overriding OnSageEvent() in a window message handler.
    /// </summary>
    /// <param name="cWin">The window to receive the wake-up event.  if empty or nullptr, the event is sent to itself.</param>
    /// <returns></returns>
    bool SendWindowEvent(CWindow * cWin = nullptr); // $QC

    // SendWidgetMessage()
    // 
    // This is used by Widgets to send a generic message that can be intercepted with OnWidgetMessage() and also
    // causes EventLoop() and WaitforEvent() to return so the main procedure can look for WidgetMessages.
    //
    // This is also useful for Widgets to send messages to themselves to process elements in the main
    // thread rather than the calling thread of an interface function with unknown status (i.e. updates, etc.)
    // (this helps avoid multi-threading issues)
    //
    bool SendWidgetMessage(CWindow * cWin,void * cWidget = nullptr,int iMessage = 0); // $QC

    // SendWidgetMessage()
    // 
    // This is used by Widgets to send a generic message that can be intercepted with OnWidgetMessage() and also
    // causes EventLoop() and WaitforEvent() to return so the main procedure can look for WidgetMessages.
    //
    // This is also useful for Widgets to send messages to themselves to process elements in the main
    // thread rather than the calling thread of an interface function with unknown status (i.e. updates, etc.)
    // (this helps avoid multi-threading issues)
    //
    bool SendWidgetMessage(HWND hWndParent,void * cWidget = nullptr,int iMessage = 0); // $QC

    // PostWidgetMessage()
    // 
    // This is used by Widgets to send a generic message that can be intercepted with OnWidgetMessage() and also
    // causes EventLoop() and WaitforEvent() to return so the main procedure can look for WidgetMessages.
    //
    // This is also useful for Widgets to send messages to themselves to process elements in the main
    // thread rather than the calling thread of an interface function with unknown status (i.e. updates, etc.)
    // (this helps avoid multi-threading issues)
    //
    bool PostWidgetMessage(CWindow * cWin,void * cWidget = nullptr,int iMessage = 0); // $QC

    // PostWidgetMessage()
    // 
    // This is used by Widgets to send a generic message that can be intercepted with OnWidgetMessage() and also
    // causes EventLoop() and WaitforEvent() to return so the main procedure can look for WidgetMessages.
    //
    // This is also useful for Widgets to send messages to themselves to process elements in the main
    // thread rather than the calling thread of an interface function with unknown status (i.e. updates, etc.)
    // (this helps avoid multi-threading issues)
    //
    bool PostWidgetMessage(HWND hWndParent,void * cWidget = nullptr,int iMessage = 0); // $QC


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
    WaitEvent WaitforEvent(const char * sEvent = nullptr); // $QCC

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
    WaitEvent WaitforEvent(cwfEvent & cwEvent) { return WaitforEvent(*cwEvent); } // $QCC
 
    
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
    bool EventLoop(WaitEvent * eStatus = nullptr);  // $QCC
 
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
    bool GetEvent(WaitEvent * eStatus = nullptr); // $QCC

    /// <summary>
    /// Waits for a mouse click in the window.  This is used a simple way to hold up program execution until the user
    /// clicks the mouse.
    /// <para></para>&#160;&#160;&#160;
    /// The window is set as the foreground and active window when this function is called.
    /// --> Note: This function will return if the user closes the window.
    /// </summary>
    /// <param name="sMsg"> = (optional) message to print out as text to the window.</param>
    /// <returns></returns>
    bool WaitforClick(const char * sMsg = nullptr);     // $QCC

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
    char WaitforKeyPress(const char * sMsg = nullptr);        // $QCC

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
    bool WaitforCRPress(const char * sMsg = nullptr);         // $QCC

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
    bool WaitforClickOrKey(const char * sMsg = nullptr);      // $QCC

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
    int WaitforClose(int iReturnValue = 0);     // $QCC
    
    // WaitforMouseClick() -- Waits for a mouse click in the current window. 
    // This function will return on receipt of a mouseclick or the window close, unless the automatic Window Close is disabled
    // (see DisableClose())
    //
    bool WaitforMouseClick();     // $QCC // false return means window is closing down

    // ExitButton() -- Places a "Program Finished. Press Button to Continue" on the bottom of the screen and waits for input before
    // continuing.  This is useful when the program ends, to allow the user to press the button before the window closes.
    //
    // Exit Button return 0
    //
    int ExitButton(const char * sText = nullptr); // $QCC 
 
    // NoExitMsg() -- Used for Sandbox applications to tell the calling process to not display the exit button w/message on the bottom of the window.
    //
    // NoExitMsg() returns 0;
    //
    int NoExitMsg(bool bNoExit = true); // $QC $QCNS

    // NoExitMsg() -- Used for Sandbox applications to tell the calling process to not display the exit button w/message on the bottom of the window.
    //
    // NoExitMsg() returns 0 unless a return code is specified.
    //
    int NoExitMsg(int iReturnCode,bool bNoExit=true);  // $QC $QCNS

    // EnableWindow() -- Enables or Disables the Window and all controls within the window
    //
    // Disabling the window an be useful to make sure no controls are used within the window, such as when dialog boxes are overlaid on top of the
    // window, or when the contex of the window currently makes no sense.
    //
    // EnableWindow() -- Enable the window
    // EnableWindow(false) -- Disable the window
    //
    bool EnableWindow(bool bEnable = true); // $QC

    // DisableWindow() -- Enables or Disables the Window and all controls within the window
    //
    // Disabling the window an be useful to make sure no controls are used within the window, such as when dialog boxes are overlaid on top of the
    // window, or when the contex of the window currently makes no sense.
    //
    // DisableWindow() -- Disable the window
    // DisableWindow(false) -- Enable the window
    //
    bool DisableWindow(bool bDisable = true); // $QC

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
    CBitmap CreateBitmap(int iWidth,int iHeight = 1); // $QC

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
    CBitmap CreateBitmap(SIZE szBitmapSize); // $QC
 
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
    [[nodiscard]] RawBitmap32_t GetBitmapStruct32(int iWidth,int iHeight = 1); // $QC

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
    CEditBox & NewEditBox(int iX,int iY,int iWidth,int iHeight,cwfOpt & cwOpt); // $QC
    CInputBox & NewInputBox(int iX,int iY,int iWidth,int iHeight,cwfOpt & cwOpt); // $QC // $$moveme
   
    CEditBox & NewEditBox(int iX,int iY,int iWidth,cwfOpt & cwOpt); // $QC
    CInputBox & NewInputBox(int iX,int iY,int iWidth,cwfOpt & cwOpt); // $QC // $$movme


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
    CEditBox & NewEditBox(CEditBox * pObject,int iX,int iY,int iWidth,int iHeight,cwfOpt & cwOpt); // $QC
    CInputBox & NewInputBox(CInputBox * pObject,int iX,int iY,int iWidth,int iHeight,cwfOpt & cwOpt) { return NewEditBox(pObject,iX,iY,iWidth,iHeight,cwOpt); }  // $QC // $$movme

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
    CEditBox & NewEditBox(int iX,int iY,int iWidth,int iHeight,const char * sText = nullptr,const cwfOpt & cwOpt = cwfOpt()); // $QC
    CInputBox & NewInputBox(int iX,int iY,int iWidth,int iHeight,const char * sText = nullptr,const cwfOpt & cwOpt = cwfOpt());  // $QC //$$moveme

    CEditBox & NewEditBox(int iX,int iY,int iWidth,const char * sText = nullptr,const cwfOpt & cwOpt = cwfOpt()); // $QC
    CInputBox & NewInputBox(int iX,int iY,int iWidth,const char * sText = nullptr,const cwfOpt & cwOpt = cwfOpt());  // $QC //$$moveme

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
    CEditBox & NewEditBox(CEditBox * pObject,int iX,int iY,int iWidth,int iHeight = 0,const char * sText = nullptr,const cwfOpt & cwOpt = cwfOpt()); // $QC
    CInputBox & NewInputBox(CInputBox * pObject,int iX,int iY,int iWidth,int iHeight = 0,const char * sText = nullptr,const cwfOpt & cwOpt = cwfOpt()) { return NewEditBox(pObject,iX,iY,iWidth,iHeight,sText,cwOpt); }  // $QC //$$moveme

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
    bool CreateButtonStyle(const char * sStyleName,GraphicButtonStyle & stStyle); // $QC

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
    bool CreateCheckboxStyle(const char * sStyleName,GraphicCheckboxStyle & stStyle); // $QC

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
    bool CaptureMouse(bool bReleaseOnMouseUp = true);  // $QCC

    // ReleaseCapture() -- this releases a mouse capture in the current window or control
    // This can be used to release the capture, such as when the mouse button is released. 
    //
    // note: The capture can be released independently.  See isMouseCaptured() for more details
    //
    bool ReleaseCapture(); // $QCC

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
    bool isMouseCaptured(); // $QC

    // Returns TRUE when the mouse capture has been release.  This only returns true when
    // the mouse capture previously engaged for the window has been released.
    //
    // This returns an event status.  Therefore, the event is reset after the call and will return false 
    // afterward until a subsequent release -- unless peek is set to Peek::No, in which case the status is not reset
    //
    bool CaptureReleased(Peek peek = Peek::No); // $QC

    // Delete the window.  This closes the window permanently.  All data associated with the window
    // is closed when the parent window is deleted. 
    //
    // If the window is subclassed with an object, this means the memory allocated by this object will not
    // be deleted right away.  Delete() can be overridden to do this, or deletObj() can be used to 
    // physically delete the Window (use carefully)
    //
    bool Delete(); // $QC

    // LockProcess() -- Lock the window process for multi-threading capability. (i.e. set Busy status)
    //
    // This uses an Atomic machine code function to lock the process and make it Busy so that multi-threading may
    // occur safely.  LockProcess() uses a single lock (use LockProcess(iMyLock) for more locks) for the window and 
    // does not return until the current lock is Unlocked
    //
    bool LockProcess(); // $QC

    // UnlockProcess() -- Unlocks the window process for multi-threading capability. (i.e. release Busy status)
    //
    // See LockProcess() for more details
    //
    bool UnlockProcess(); // $QC


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
    bool LockProcess(int & iLock); // $QC

    // UnlockProces(int & iLock) -- Unlock a specific lock to lock the process for multi-threading (i.e. release Busy status for the specific lock)
    //
    bool UnlockProcess(int & iLock); // $QC

    // SetDebugID() -- set a specific ID for the window. 
    //
    // Windows can be hard to debug, especially when there are a number of controls and other window. 
    // When looking at messages and other actions, it can be hard to determine which window or control is 
    // responsible.
    //
    // SetDebugID() can be used to set a specific Debug ID for the window so that it can be
    // trapped in debugging.
    //
    void SetDebugID(int iID); // $QC

    // GetDebugID() -- returns the debug ID set for the window.
    //
    // Set SetDebugID() for more information.
    //
    int GetDebugID(); // $QC
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
    CTextWidget & TextWidget(int iX,int iY,int iWidth,int iHeight,const char * sMessage = nullptr,const cwfOpt & cwOpt = cwfOpt()); // $QC

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
    CTextWidget & TextWidget(int iX,int iY,int iWidth,int iHeight,cwfOpt & cwOpt); // $QC
    
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
    CTextWidget & TextWidget(int iX,int iY,const char * sMessage = nullptr,const cwfOpt & cwOpt = cwfOpt()); // $QC

    // This is used to register a widget with SageBox.
    // The primary use for this is for the widget to register with Sagebox when called by Sagebox.
    // 
    // When Sagebox calls the Widget::Register() function, the widget calls this function,
    // which fille the provided RegistryID with an ID for the widget that it can use to work with dialog boxes
    // created by Sagebox.
    //
    // Note:: this is not a user function and is used by SageBox and Widgets
    // 

    CTextWidget & TextWidget(const char * sMessage,const cwfOpt & cwOpt = cwfOpt());        // $QC

    int RegisterWidget(int & iRegistryID); // $QC

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
    bool SnaptoWin(CWindow * cWin,int iPadX = 0,int iPadY = 0,Snap eAction = Snap::Snap,CWidget * cWidget = nullptr);       // $Q_C
    bool SnaptoDesktop(int iPadX = 0,int iPadY = 0,Snap eAction = Snap::Snap,CWidget * cWidget = nullptr);                  // $Q_C
#if 0
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
    CQuickDialog & QuickDialog(const char * sHeader = nullptr,const cwfOpt & cwOpt = CWindow::cwNoOpt); // $QC

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
    CQuickDialog & QuickDialog(const cwfOpt & cwOpt); // $QC

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
    CString GetOpenFile(stOpenFileStruct & stFile); // $QC
 
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
    CString GetOpenFile(const char * sTypes = nullptr); // $QC
    
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
    bool GetOpenFile(stOpenFileStruct & stFile,CString & csFilename); // $QC

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
    bool GetOpenFile(const char * sTypes,CString & csFilename); // $QC

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
    bool GetOpenFile(CString & csFilename); // $QC


    CString GetSaveFile(stOpenFileStruct & stFile); // $QC
 
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
    CString GetSaveFile(const char * sTypes = nullptr); // $QC
    
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
    bool GetSaveFile(stOpenFileStruct & stFile,CString & csFilename); // $QC

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
    bool GetSaveFile(const char * sTypes,CString & csFilename); // $QC

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
    bool GetSaveFile(CString & csFilename); // $QC


    // GetOpenFileStruct() -- Return an stOpenFileStruct to set for using an GetOpenFile() or GetSaveFile() dialog.
    //
    // This is a shortcut for declaring a structure directly, so that auto& stStruct = GetOpenFileStruct() can be used
    // more easily.
    //
    stOpenFileStruct GetOpenFileStruct(); // $QC

    // WriteShadow() -- Works like Write() (but is more limited), but writes the text with a shadow behind it.
    //
    // See Write() for more information
    //
    void WriteShadow(int iX,int iY,const char * sMessage,const cwfOpt & cwOpt = cwfOpt()); // $QCC

    // Retrieves the last menu item selected.
    // 
    // This is meant to be used right after MenuItemSelected() is called, and will return the last menu item selected.
    //
    // This will continue to return the same menu item until a new menu item is selected.
    //
    int GetSelectedMenuItem();                                                                      // $QC

    bool CreateMenu(const char * sMenu,const cwfOpt & cwOpt = CWindow::cwNoOpt);                                                            // $QC
    bool QuickMenu(const char * sMenu,const cwfOpt & cwOpt = CWindow::cwNoOpt);                                                            // $QC
    
    int GetMenuItemID(const char * sMenu);                                                          // $QC
    bool isMenuIDSelected(const char * sMenu);                                                        // $QC
    bool isMenuIDSelected(int iMenuID) { return this->GetSelectedMenuItem() == iMenuID; }             // $QC

    bool EnableMenuItem(int iMenuID,bool bEnable = true);                                           // $QC
    bool EnableMenuItem(const char * sMenu,bool bEnable = true);                                    // $QC

    bool DisableMenuItem(int iMenuID,bool bDisable = true);                                           // $QC
    bool DisableMenuItem(const char * sMenu,bool bDisable = true);                                    // $QC

    bool SetMenuItemCheck(int iMenuID,bool bChecked);
    bool SetMenuItemCheck(const char * sMenu,bool bChecked);

    bool HideMenu(bool bHide = true);                   //  $QCC
    bool ShowMenu(bool bShow = true);                   //  $QCC
    bool ShowMenu(CMenu & cMenu,bool bShow = true);     //  $QCC
    bool HideMenu(CMenu & cMenu,bool bHide = true);     //  $QCC

    void PrintMenuItems();                              // $QC

    // CreateMenu() -- Creates a menu that can be added as a main menu or sub menu to the window.
    //
    // Once a menu is created, items can be added through the CMenu class object returned.
    //
    // See CMenu documentation for more information.
    //
    [[nodiscard]] CMenu CreateMenu(int iBaseMenuValue = 0); // $QC

    // Find a Windows menu.  If the HMENU (Windows Menu) value is a valid windows menu, a CMenu object is returned for this menu.
    //
    CMenu GetMenu(HMENU hMenu,bool * bFound = nullptr); // $QC

    // Find a Windows menu.  If the HMENU (Windows Menu) value is a valid windows menu, a CMenu object is returned for this menu.
    //
    bool GetMenu(HMENU hMenu,CMenu & cMenu); // $QC

    // MenuItemSelected() -- This works as an event, and will fill the menu item if it is selected.
    // As an event, subsquent calls will return false and not fill the menu item until another menu item is selected.
    //
    // If the iMenuItem is not included in the call, GetMenuItem() can be used to retrieve the last menu item selected
    //
    bool MenuItemSelected(int & iMenuItem,Peek peek = Peek::No); // $QC

    // MenuItemSelected() -- This works as an event, and will fill the menu item if it is selected.
    // As an event, subsquent calls will return false and not fill the menu item until another menu item is selected.
    //
    // If the iMenuItem is not included in the call, GetMenuItem() can be used to retrieve the last menu item selected
    //
    std::optional<int> MenuItemSelected(Peek peek = Peek::No); // $QC

    bool SetCloseButtonMenu(int iMenuItem = 0); // $QC

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
    CBitmap ReadImageFile(const char * sPath,bool * bSuccess = nullptr); // $QC

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
     CBitmap ReadImageFile(std::string & sPath,bool * bSuccess = nullptr); // $QC

    CBitmap32 ReadImageFile32(const char * sPath,bool * bSuccess = nullptr); // $QC
    CBitmap32 ReadImageFile32(std::string & sPath,bool * bSuccess = nullptr); // $QC

    /// <summary>
    /// Reads an image in memory and returns a CBitmap.  If the Bitmap is not valid, then there was an error. 
    /// <para></para>
    /// Use GetLastImageStatus() to determine error type.
    /// <para></para>
    /// Memory and Memory Size, Vector, or Mem classes may be used instead.  See function prototypes. 
    /// <para></para>
    /// --> Image Types Supported: BMP, Jpeg, and PNG.
    /// </summary>
    /// <param name="sMem">Start of Image Memory</param>
    /// <param name="tMemLength">Length of Memory</param>
    /// <param name="bSuccess">(optional) filled with true/false if image was read successfully.</param>
    /// <returns></returns>
    CBitmap ReadImageMem(const unsigned char * sMem,size_t tMemLength,bool * bSuccess = nullptr); // $QC

    /// <summary>
    /// Reads an image in memory and returns a CBitmap.  If the Bitmap is not valid, then there was an error. 
    /// <para></para>
    /// Use GetLastImageStatus() to determine error type.
    /// <para></para>
    /// Memory and Memory Size, Vector, or Mem classes may be used instead.  See function prototypes. 
    /// <para></para>
    /// --> Image Types Supported: BMP, Jpeg, and PNG.
    /// </summary>
    /// <param name="sMem">Start of Image Memory</param>
    /// <param name="tMemLength">Length of Memory</param>
    /// <param name="bSuccess">(optional) filled with true/false if image was read successfully.</param>
    /// <returns></returns>
    CBitmap ReadImageMem(Mem<unsigned char> & cMem,bool * bSuccess = nullptr); // $QC

    /// <summary>
    /// Reads an image in memory and returns a CBitmap.  If the Bitmap is not valid, then there was an error. 
    /// <para></para>
    /// Use GetLastImageStatus() to determine error type.
    /// <para></para>
    /// Memory and Memory Size, Vector, or Mem classes may be used instead.  See function prototypes. 
    /// <para></para>
    /// --> Image Types Supported: BMP, Jpeg, and PNG.
    /// </summary>
    /// <param name="sMem">Start of Image Memory</param>
    /// <param name="tMemLength">Length of Memory</param>
    /// <param name="bSuccess">(optional) filled with true/false if image was read successfully.</param>
    /// <returns></returns>
    CBitmap ReadImageMem(std::vector<unsigned char> & vMem,bool * bSuccess = nullptr); // $QC


    ImageStatus GetLastImageStatus(); // $QC

    // ReadJpegFile -- Read a jpeg file and store it into a CBitmap. 
    // This reads standard 8-bit jpeg (3-channel or monochrome).
    // If the file does not exist or is in an unsupported format, CBitmap will come back empty.
    // Use GetJpegError() to get the status of the last ReadJpegFile() call, which will
    // give information on why the Bitmap came back empty.
    //
    // if (bSuccess) is supplied, it is filled with true for a successful read, otherwise false.
    //
    CBitmap ReadJpegFile(const char * sPath,bool * bSuccess = nullptr); // $QC

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
    CBitmap ReadJpegMem(const unsigned char * sData,int iDataLength,bool * bSuccess); // $QC

    // Returns the last Jpeg status.  This will return CJpeg::Status::Ok if there was no error, 
    // or an indication of what went wrong with the last call, such as CJpeg::Status::EmptyFilePath or CJpeg::Status::FileNotFound
    //
    CJpeg::Status GetJpegError(); // $QC

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
    bool QuickThumbnail(RawBitmap_t & stBitmap,int iWidth,int iHeight,ThumbType eType = ThumbType::BestFit,const char * sTitle = nullptr); // $QC
 
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
    bool QuickThumbnail(RawBitmap_t & stBitmap,int iWidth,int iHeight,const char * sTitle); // $QC

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
    bool QuickThumbnail(CBitmap & cBitmap,int iWidth,int iHeight,ThumbType eType = ThumbType::BestFit,const char * sTitle = nullptr); // $QC

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
    bool QuickThumbnail(CBitmap & cBitmap,int iWidth,int iHeight,const char * sTitle); // $QC

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
    bool QuickThumbnail(POINT pLoc,RawBitmap_t & stBitmap,int iWidth,int iHeight,ThumbType eType = ThumbType::BestFit,const char * sTitle = nullptr); // $QC
 
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
    bool QuickThumbnail(POINT pLoc,RawBitmap_t & stBitmap,int iWidth,int iHeight,const char * sTitle); // $QC

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
    bool QuickThumbnail(POINT pLoc,CBitmap & cBitmap,int iWidth,int iHeight,ThumbType eType = ThumbType::BestFit,const char * sTitle = nullptr); // $QC

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
    bool QuickThumbnail(POINT pLoc,CBitmap & cBitmap,int iWidth,int iHeight,const char * sTitle); // $QC

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
    bool QuickThumbnail(int iX,int iY,RawBitmap_t & stBitmap,int iWidth,int iHeight,ThumbType eType = ThumbType::BestFit,const char * sTitle = nullptr); // $QC
 
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
    bool QuickThumbnail(int iX,int iY,RawBitmap_t & stBitmap,int iWidth,int iHeight,const char * sTitle); // $QC

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
    bool QuickThumbnail(int iX,int iY,CBitmap & cBitmap,int iWidth,int iHeight,ThumbType eType = ThumbType::BestFit,const char * sTitle = nullptr); // $QC

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
    bool QuickThumbnail(int iX,int iY,CBitmap & cBitmap,int iWidth,int iHeight,const char * sTitle); // $QC

    CBitmap QuickResize(RawBitmap_t & stBitmap,int iWidth,int iHeight,ResizeType eType = ResizeType::BestFit); // $QC
    CBitmap QuickResize(RawBitmap_t & stBitmap,SIZE szSize,ResizeType eType = ResizeType::BestFit); // $QC


    // Duplication of C++ console-mode getline()
    //
    // This is the same as GetString() except that the sring input is specified in the function call.
    // This is to duplicate the C++ getline() function.
    //
    // CString and std::string are supported.
    //
    // See GetString() for option usage.
    //
    CString getline(CString & cString,const cwfOpt & cwOpt = cwfOpt()); // $QCC $QCNS

    // Duplication of C++ console-mode getline()
    //
    // This is the same as GetString() except that the sring input is specified in the function call.
    // This is to duplicate the C++ getline() function.
    //
    // CString and std::string are supported.
    //
    // See GetString() for option usage.
    //
    CString __getline(CString & cString,const cwfOpt & cwOpt = cwfOpt()); // $QCC $QCNS

    // Duplication of C++ console-mode getline()
    //
    // This is the same as GetString() except that the sring input is specified in the function call.
    // This is to duplicate the C++ getline() function.
    //
    // CString and std::string are supported.
    //
    // See GetString() for option usage.
    //
    CString getline(std::string & cString,const cwfOpt & cwOpt = cwfOpt()); // $QCC $QCNS

    // Duplication of C++ console-mode getline()
    //
    // This is the same as GetString() except that the sring input is specified in the function call.
    // This is to duplicate the C++ getline() function.
    //
    // CString and std::string are supported.
    //
    // See GetString() for option usage.
    //
    CString __getline(std::string & cString,const cwfOpt & cwOpt = cwfOpt()); // $QCC $QCNS

    // Duplication of C++ console-mode getline()
    //
    // This is the same as GetString() except that the sring input is specified in the function call.
    // This is to duplicate the C++ getline() function.
    //
    // CString and std::string are supported.
    //
    // See GetString() for option usage.
    //
    CString getline(const cwfOpt & cwOpt = cwfOpt()); // $QCC $QCNS

    // Duplication of C++ console-mode getline()
    //
    // This is the same as GetString() except that the sring input is specified in the function call.
    // This is to duplicate the C++ getline() function.
    //
    // CString and std::string are supported.
    //
    // See GetString() for option usage.
    //
    CString __getline(const cwfOpt & cwOpt = cwfOpt()); // $QCC $QCNS

    // Duplication of C++ console-mode getline()
    //
    // This is the same as GetString() except that the sring input is specified in the function call.
    // This is to duplicate the C++ getline() function.
    //
    // CString and std::string are supported.
    //
    // See GetString() for option usage.
    //
    int getline(char * sString,int iMaxSize); // $QCC $QCNS

     // Duplication of C++ console-mode getline()
    //
    // This is the same as GetString() except that the sring input is specified in the function call.
    // This is to duplicate the C++ getline() function.
    //
    // CString and std::string are supported.
    //
    // See GetString() for option usage.
    //
    int __getline(char * sString,int iMaxSize); // $QCC $QCNS

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
    CListBox & NewListBox(int ix,int iy,int iWidth,int iHeight,const cwfOpt & cwOpt = cwfOpt()); // $QC
    CListBox & NewListBox(int ix,int iy,int iWidth,int iHeight,const char * sItemList,const cwfOpt & cwOpt = cwfOpt()); // $QC
 
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
    CListBox & NewListBox(CListBox * cListBox,int ix,int iy,int iWidth,int iHeight,const cwfOpt & cwOpt = cwfOpt()); // $QC
    CListBox & NewListBox(CListBox * cListBox,int ix,int iy,int iWidth,int iHeight,const char * sItemList,const cwfOpt & cwOpt = cwfOpt()); // $QC


    CComboBox &  NewComboBox(int iX,int iY,int iWidth,const cwfOpt & cwOpt = cwfOpt());  // $QC
    CComboBox &  NewComboBox(int iX,int iY,int iWidth,const char * sItemList,const cwfOpt & cwOpt = cwfOpt());  // $QC
    CComboBox &  NewComboBox(CComboBox * cComboBox,int iX,int iY,int iWidth,const cwfOpt & cwOpt = cwfOpt());  // $QC
    CComboBox &  NewComboBox(CComboBox * cComboBox,int iX,int iY,int iWidth,const char * sItemList,const cwfOpt & cwOpt = cwfOpt());  // $QC

    bool SetEventWindow(CWindow * cWin = nullptr); // $QC
    bool SetEventWindow(CWindow & cWin); // $QC

    // SendEventsToParent() -- Send all events to Parent Window while also sending them to the Window itself.
    //
    // This function is mucnh like SetEventWindow() which will tell Sagebox where else to send all events through GetEvent().
    // Child windows send events to their parents (as well as themselves) by default, meaning you can use GetEvent() for the parent window
    // and receive events for the window itself.
    //
    // For windows created with Sagebox (and some other instances), there is no Event Window set, and to receive events through the
    // CSageBox::GetEvent() loop, SendEventsToParent() must be called -- this will send all events to the Sagebox main window (which is hidden)
    //
    bool SendEventsToParent();  // $QC

    /// <summary>
    /// Returns parent window.  Useful for Chld Windows and other window operations.
    /// </summary>
    /// <returns>CWindow pointer to parent window.  nullptr if there is no parent.</returns>
    CWindow * GetParentWindow(); 

    stControlLabel_t AddControlLabel(SizeRect srSize,const char * sText,LabelJust justType,bool bUpdate,const cwfOpt & cwOpt = cwfOpt()); // $QC
   
    bool DrawLabelBox(POINT pLocation,SIZE szSize,const char * sTitle,bool bUpdate = true,BorderType eBorderType = BorderType::Depressed, LabelJust eLabelType = LabelJust::None,const cwfOpt & cwOpt = cwfOpt()); // $QCC
    bool DrawLabelBox(POINT pLocation,SIZE szSize, const char * sTitle,const cwfOpt & cwOpt); // $QCC
    bool DrawLabelBox(POINT pLocation,SIZE szSize,const char * sTitle,bool bUpdate,const cwfOpt & cwOpt); // $QCC
 
    bool SetFullScreen(bool bFullscreen = true,bool bForceShow = false); // $QC

    CDavinci * GetDavinciMain(); // $QC

    SIZE CreateRadioButtonGroup(int iNumButtons,int iX,int iY,const char * * sButtonNames,int iGroupID, const char * sLabel, const cwfOpt & cwOpt = cwfOpt()); // $QC
    SIZE CreateCheckboxGroup(int iNumButtons,int iX,int iY,const char * * sButtonNames,int iGroupID, const char * sLabel, const cwfOpt & cwOpt = cwfOpt()); // $QC

	bool EnablePainting(); // $QC
	bool DisablePainting(); // $QC
	bool DisablePaintingSafe(); // $QC

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
	bool DontUpdate(bool bDontUpdate = true,bool bUpdateNow = false); // $QC

    /// <summary>
    /// Used to update the window once when updates are off (see DontUpdate())
    /// <para></para>&#160;&#160;&#160;
    /// This function forces an Update even when updates are off, allowing a loop with updates off to update one time
    /// without changing the DontUpdate() status (i.e. no further updates will occur until updates are turned back on).
    /// </summary>
    /// <returns></returns>
    bool UpdateOnce(); // $QC

    bool SetasTopWindow(); // $QC
    bool SetasTopmostWindow(bool bTopMost = true); // $QC
    bool SetasBottomWindow(); // $QC

    /// <summary>
    /// Sets window as the top window in the Z-Order of all windows created by Sagebox.
    /// <para></para>
    /// See WindowTopMost() to make window a 'topmost' window, which will persist in the z-order 
    /// </summary>
    /// <returns></returns>
    bool WindowTop();            // $QC

    /// <summary>
    /// Set Window as a topmost window.  This will being the window the top of all windows and persist
    /// as the topmost window even when other windows are activated.
    /// <para></para>
    /// See WindowTop() to bring the window to the top of the z-order with it being persistent.
    /// </summary>
    /// <param name="bTopMost"> - when true, this sets the window as a persistent topmost window.  When False, this turns off the topmost status.</param>
    /// <returns></returns>
    bool WindowTopMost(bool bTopMost = true);        // $QC

    /// <summary>
    /// Mazimize the window on the desktop. 
    /// </summary>
    /// <returns></returns>
    bool WindowMaximize();       // $QC

    /// <summary>
    /// Minimize the Window in the desktop.
    /// </summary>
    /// <returns></returns>
    bool WindowMinimize();       // $QC



    bool DrawSimpleDoc(const unsigned char * sPgrData,const cwfOpt & cwOpt = cwfOpt());  // $QCC
    bool DrawSimpleDoc(const char * sPgrPath, const cwfOpt & cwOpt = cwfOpt()); // $QCC

    // DevGetWindow() -- Returns the Window (i.e. CWindow) of the default DevWindow.  This can be used to access window functions for the 
    // the default DevWindow. 
    //
    // For user-created Dev Control Windows, use the CDevControls::GetWindow() function, i.e. cMyDevWindow->GetWindow(); 
    //
    CWindow * DevGetWindow(); // $QC

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
    CWindow::WinGroup * DevGetGroup(); // $QC

    // Create a Dev Controls Window, allowing for quick creation and automatic placement of
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
    CDevControls & NewDevWindow(int iX, int iY, const cwfOpt & cwOpt = CWindow::cwNoOpt); // $QC

    // Create a Dev Controls Window, allowing for quick creation and automatic placement of
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
    CDevControls & NewDevWindow(POINT pLoc, const cwfOpt & cwOpt = CWindow::cwNoOpt); // $QC

    // Create a Dev Controls Window, allowing for quick creation and automatic placement of
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
    CDevControls & NewDevWindow(const cwfOpt & cwOpt = CWindow::cwNoOpt); // $QC


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
    bool DevSetBgColor(RgbColor rgbColor1,RgbColor rgbColor2 = Sage::Rgb::Undefined,bool bDrawBar = true);  // # DevSetBgColor
    bool DevSetBgColor(RgbColor rgbColor1,bool bDrawBar);                                                   // # DevSetBgColor
    bool DevSetBgColor(const char * sColor1,const char * sColor2 = nullptr,bool bDrawBar = true);           // # DevSetBgColor
    bool DevSetBgColor(const char * sColor1,bool bDrawBar);                                                 // # DevSetBgColor

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
    bool DevSetBgBitmap(CBitmap & cBitmap,bool bDrawBar = true,const cwfOpt & cwOpt = cwfOpt());       // # DevSetBgBitmap
    bool DevSetBgBitmap(const char * sBitmap,bool bDrawBar = true,const cwfOpt & cwOpt = cwfOpt());    // # DevSetBgBitmap
    bool DevSetBgBitmap(CBitmap & cBitmap,const cwfOpt & cwOpt);                                       // # DevSetBgBitmap
    bool DevSetBgBitmap(const char * sBitmap,const cwfOpt & cwOpt);                                    // # DevSetBgBitmap

    /// <summary>
    /// When bAllowClose = false, Disables the Dev Window from closing on its own -- the program must close it purposely. <para></para>
    /// This will disable the 'x' from appearing when the Dev Window is the only window open, so that the  <para></para>
    /// user cannot close it with the 'x' button. <para></para>
    /// . <para></para>
    /// This function must be called before AllowClose() is called, otherwise results may be unpredictable. <para></para>
    /// Note: to manually add the 'x' (and Close Button), use AllowClose().
    /// </summary>
    /// <returns></returns>
    bool DevAllowAutoClose(bool bAllowClose = true); 

    /// <summary>
    /// Sets the Y position of the next control added.
    /// </summary>
    bool DevSetNextY(int iY);


	// DevButton() -- Add a button to the Default Dev Control Window.  This accepts all options as normal buttons, but 
	// the default will add a regular button. 
	//
	// The Name used as a title for the button, but is optional. 
	//
    CButton & DevButton(const char * sButtonName = nullptr,const cwfOpt & cwOpt = cwfOpt()); // $QC

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
    CButton & DevCheckbox(const char * sCheckboxName = nullptr,const cwfOpt & cwOpt = cwfOpt()); // $QC


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
    ButtonGroup DevRadioButtons(const char * sButtonNames,const cwfOpt & cwOpt = cwfOpt());

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
    ButtonGroup DevRadioButtons(const char * * sButtonNames,const cwfOpt & cwOpt = cwfOpt());

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
    ButtonGroup DevRadioButtons(int iNumButtons,const char * * sButtonNames,const cwfOpt & cwOpt = cwfOpt());

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
    ButtonGroup DevRadioButtons(std::vector<char *> & vButtonNames,const cwfOpt & cwOpt = cwfOpt());

    ButtonGroup DevCheckboxGroup(const char * sButtonNames,const cwfOpt & cwOpt = cwfOpt());
    ButtonGroup DevCheckboxGroup(const char * * sButtonNames,const cwfOpt & cwOpt = cwfOpt());
    ButtonGroup DevCheckboxGroup(int iNumButtons,const char * * sButtonNames,const cwfOpt & cwOpt = cwfOpt());
    ButtonGroup DevCheckboxGroup(std::vector<char *> & vButtonNames,const cwfOpt & cwOpt = cwfOpt());

    // DevSlider() -- Add a slider to the Default Dev Controls Window.  The default width is 200 with a 0-100 range.  
    // The Range can be changed with default Slider options, i.e. opt::Range(0,200), for example, to set a range of 0-200.
	// -->
	// The title is displayed beneath the slider, as well as the value. 
	//
    CSlider & DevSlider(const char * sSliderName = nullptr,const cwfOpt & cwOpt = cwfOpt()); // $QC

    // DevSliderf() -- Add a floating-point slider to the Default Dev Controls Window.  The default width is 200 with a 0-100 range.  
    // The Range can be changed with default Slider options, i.e. opt::Range(0,200), for example, to set a range of 0-200.
	// -->
	// The title is displayed beneath the slider, as well as the value. 
	//
    CSlider & DevSliderf(const char * sSliderName = nullptr,const cwfOpt & cwOpt = cwfOpt()); // $QC

    // DevEditBox() -- Add an EditBox to the default Dev control Window.  The sEditBoxTitle, while optional, will provide a
	// label to the left of the edit box.  The default width is 150 pixels or so, but can be changed with normal EditBox options
	//
    // Note: DevEditBox() and DevInputBox() are the same.  The term Editbox is deprecated and is used for consitency with Windows control names.
    //
    CEditBox & DevEditBox(const char * sEditBoxName = nullptr,const cwfOpt & cwOpt = cwfOpt()); // $QC
    
    // DevInputBox() -- Add an InputBox to the default Dev control Window.  The sInputBoxName, while optional, will provide a
	// label to the left of the Input Box.  The default width is 150 pixels or so, but can be changed with normal InputBox options
	//
    // Note: DevEditBox() and DevInputBox() are the same.  The term Editbox is deprecated and is used for consitency with Windows control names.
    //
    CEditBox & DevInputBox(const char * sInputBoxName = nullptr,const cwfOpt & cwOpt = cwfOpt()) { return DevEditBox(sInputBoxName,cwOpt); } // $QC //$$moveme
  	CComboBox & DevComboBox(const char * sComboBoxName,const cwfOpt & cwOpt = cwfOpt()); // $QC
  	CComboBox & DevComboBox(const cwfOpt & cwOpt = cwfOpt()); // $QC
	CWindow & DevWindow(const char * sTitle,int iNumlines,const cwfOpt & cwOpt = cwfOpt()); // $QC
	CWindow & DevWindow(int iNumLines,const cwfOpt & cwOpt = cwfOpt()); // $QC
	CWindow & DevWindow(const cwfOpt & cwOpt = cwfOpt()); // $QC
	CWindow & DevWindow(const char * sTitle,const cwfOpt & cwOpt = cwfOpt()); // $QC

    CTextWidget & DevText(const char * sText,const cwfOpt & cwOpt  = cwfOpt()); // $QC
    CTextWidget & DevText(const char * sText,int iHeight,const cwfOpt & cwOpt  = cwfOpt()); // $QC
    CTextWidget & DevText(int iHeight,const cwfOpt & cwOpt  = cwfOpt()); // $QC
    CTextWidget & DevText(const cwfOpt & cwOpt  = cwfOpt()); // $QC

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
    bool DevBitmap(CBitmap & cBitmap,bool bDrawTopBar = true);                                                               /// # DevBitmap
    bool DevBitmap(CBitmap & cBitmap,const char * sText,bool bDrawTopBar = true,const cwfOpt & cwOpt = CWindow::cwNoOpt);    /// # DevBitmap
    bool DevBitmap(CBitmap & cBitmap,const char * sText,const cwfOpt & cwOpt);                                               /// # DevBitmap

    bool DevBitmap(const char * sPath,bool bDrawTopBar = true);                                                               /// # DevBitmap
    bool DevBitmap(const char * sPath,const char * sText,bool bDrawTopBar = true,const cwfOpt & cwOpt = CWindow::cwNoOpt);    /// # DevBitmap
    bool DevBitmap(const char * sPath,const char * sText,const cwfOpt & cwOpt);                                               /// # DevBitmap

    /// <summary>
    /// Auto-hides the DevWindow when the user presses the 'x' button or close button.  This does not destroy the devwindow, and only hides it.
    /// <para></para>
    /// Once Hidden, the DevWindow can be shown again with a call to Show()
    /// </summary>
    /// <param name="bAutoHide"> - True to auto-hide window; false to turn auti-hide off</param>
    /// <param name="bAddCloseButton"> -- True to create a close button.  Otherwise only the 'x' appears in the upper-right corner.</param>
    /// <returns></returns>
    bool DevWindowAutoHide(bool bAutoHide = true,bool bAddCloseButton = false); // $QC

    /// <summary>
    /// Shows the DevWindow if it is hidden.  
    /// </summary>
    /// <param name="bShow">True to show the Dev Window. False to hide the DevWindow</param>
    /// <returns></returns>
    bool DevWindowShow(bool bShow = true);  // $QC

    /// <summary>
    /// Hides the DevWindow if it is visible.  
    /// </summary>
    /// <param name="bShow">True to hide the Dev Window. False to show the DevWindow</param>
    /// <returns></returns>
    bool DevWindowHide(bool bHide = true); // $QC

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
    bool DevWindowCloseEvent(bool bAddCloseButton = false); // $QC

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
    bool DevAllowClose(bool bAllowClose = true,bool bAddCloseButton = false);   // $QC

    /// <summary>
    /// Sets the window to close automatically when there are no other windows open. <para></para>
    /// By default, the Dev Window is a 'primary' window and won't close when <para></para>
    /// functions such as WaitPending() or GetEvent() are used.<para></para>
    /// .<para></para>
    /// When set to false (default), the window won't close until it is closed by the user. 
    /// or the program exits.
    /// </summary>
    /// <param name="bAutoClose">when true, the dev window will close automatically.  When false, the user must close it.</param>
    bool DevAutoClose(bool bAutoClose = true);
  
    /// <summary>
    /// Sets the location of the Dev Window
    /// </summary>
    bool DevSetLocation(int iX,int iY);

    /// <summary>
    /// Sets the location of the Dev Window
    /// </summary>
    bool DevSetLocation(POINT pLoc);

    /// <summary>
    /// Returns TRUE if the "Close" Button or 'X' has been pressed (both are added by DevAllowClose()).
    /// <para></para>
    /// This can be used as a quick way to close an application that only has a Dev Window and no other window except the Console Window.
    /// </summary>
    /// <returns>true if the user has attempted to close the window.</returns>
    /// <param name="bAddCloseButton">when TRUE adds a "Close" button on first usage.  Otherwise only the 'X' is placed on the right-top of the window for closure.</param>
    /// <returns>true if the user has attempted to close the window.</returns>
    bool DevWindowClosed(bool bAddCloseButton = false);     // $QC

 	// DevAddSection() -- Adds a text section to the default Dev Controls window, to separate types of controls.
	// You can use opt::fgColor() to set the text color of the section name.
	//
    bool DevAddSection(const char * sSectionName = nullptr,const cwfOpt & cwOpt = cwfOpt()); // $QC

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
    bool DevWaitEvent(bool bReturnFalseOnClose = true); // $QC

    // GetDevControlsPtr() -- returns the pointer to the default CDevControls object. 
    // *** Important note *** -- this will return NULLPTR until a control is created with
    // QuickButton(), QuickSlider(), etc. The window is not created until a control is created in order
    // to save memory.
    //
    // With the pointer, any CDevContgrols() function can be used, though most are replicated directly
    // through CSageBox functions for ease of use.
    //
    CDevControls * GetDevControlsPtr(); // $QC
 
    bool DevWindowTopmost(bool bTopmost = true); // $QC

    /// <summary>
    /// Disable ablility for user to close the window by pressing the upper-right "X" button or 
    /// pressing ALT-F4
    /// <para></para>&#160;&#160;&#160;
    /// CloseButtonPressed() can be used to determine if the user pressed the close button (or ALT-F4), or the window
    /// has otherwise been slated to close by the system.
    /// </summary>
    /// <param name="bDisable">When true, disabled the window from close.  When false, allows the user to close the window</param>
    /// <returns></returns>
    bool DisableClose(bool bDisable = true); // $QC

    /// <summary>
    /// Returns true if the window close has been disabled (i.e. ability for the user to close the window)
    /// <para></para>
    /// Use DisableClose() to disable and enable the ability for the user to close the window.
    /// </summary>
    /// <returns>TRUE when Window close is enabled; false if the window-close has been disabled</returns>
    bool WindowCloseEnabled();      // $QC

    bool CloseWindow(); // $QC

    bool SetClsBitmap(RawBitmap_t & stBitmap,bool bClsNow = true); // $QC
    bool SetClsBitmap(CBitmap & cBitmap,bool bClsNow = true); // $QC
    bool SetClsBitmap(char * sBitmap,bool bClsNow = true); // $QC
    bool ClearClsBitmap(); // $QC
    CBitmap & GetClsBitmap(); // $QC
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
	CBitmap ReadPgrBitmap(const char * sImageTitle,const char * sPgrFile,bool * bSuccess = nullptr); // $QC

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
	CBitmap ReadPgrBitmap(const char * sImageTitle,const unsigned char * sPGRMemory,bool * bSuccess = nullptr); // $QC
    

    bool CreateButtonGroup(int iGroupID,int iNumButtons,const wchar_t * * sButtonNames,POINT pLocation,SIZE szSize,int iColumns = 0, SIZE szSpacing = {-1,-1}, const cwfOpt & cwOpt = cwfOpt()); // $QC
    bool CreateButtonGroup(int iGroupID,int iNumButtons,const wchar_t * * sButtonNames,POINT pLocation,int iColumns = 0, const cwfOpt & cwOpt = cwfOpt()); // $QC
    bool CreateButtonGroup(int iGroupID,int iNumButtons,const char * * sButtonNames,POINT pLocation,SIZE szSize,int iColumns = 0, SIZE szSpacing = {-1,-1}, const cwfOpt & cwOpt = cwfOpt()); // $QC
    bool CreateButtonGroup(int iGroupID,int iNumButtons,const char * * sButtonNames,POINT pLocation,int iColumns = 0, const cwfOpt & cwOpt = cwfOpt()); // $QC
    bool CreateButtonGroup(const char * sGroupName,int iNumButtons,const wchar_t * * sButtonNames,POINT pLocation,SIZE szSize,int iColumns = 0, SIZE szSpacing = {-1,-1}, const cwfOpt & cwOpt = cwfOpt()); // $QC
    bool CreateButtonGroup(const char * sGroupName,int iNumButtons,const wchar_t * * sButtonNames,POINT pLocation,int iColumns = 0, const cwfOpt & cwOpt = cwfOpt());
    bool CreateButtonGroup(const char * sGroupName,int iNumButtons,const char * * sButtonNames,POINT pLocation,SIZE szSize,int iColumns = 0, SIZE szSpacing = {-1,-1}, const cwfOpt & cwOpt = cwfOpt()); // $QC
    bool CreateButtonGroup(const char * sGroupName,int iNumButtons,const char * * sButtonNames,POINT pLocation,int iColumns = 0, const cwfOpt & cwOpt = cwfOpt()); // $QC
     
    bool SetSignal(SignalEvents event,bool & bSignal); // $QC
    bool SetSignal(SignalEvents event,Signal & stSignal); // $QC
    bool CancelSignal(SignalEvents event); // $QC
    bool UpdateBg(bool bUpdateNow = false); // $QC

    // Returns true of the main thread is stopped.  
    // Use StartThread() to resume the main thread. 
    //
    // This only applies to the original program thread and will not work for other threads created after
    // the program started.
    //
    bool ThreadStopped(); // $QC
    
    // Returns the status of the thread.  ThreadStatus::Running or ThreadStatus::Suspended (if the thread is stopped).
    // This only applies to the original program thread and will not work for other threads created after
    // the program started.
    //
    ThreadStatus GetThreadStatus(); // $QC

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
    int StopThread(bool bEndProgramOnClose = false,int iOkReturnValue = 0); // $QC

    // Resume the main thread if it is suspended.
    //
    // iValue will be returned by StopThread() when it is awakened. 0 is returned if it was awakened due to the window closing.
    //
    bool ResumeThread(int iValue = 0); // $QC

    // End the program when in the main message thread.  This is used when StopThread() has been used to stop the main thread but events are
    // still being handled through the Main Windows Message Thread.
    //
    // EndProgram() sets the window closing status and resumes the main thread.  Typically, the main thread will exit and SageBox will end.
    // However, StopThread() can be used anywhere and does not need to exit immediately.  It can take care of cleanup, memory deallocations, etc. 
    //
    // iValue will be returned by StopThread() when it is awakened. 0 is returned if it was awakened due to the window closing.
    //
    bool EndProgram(int iValue = 0); // $QC

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
    bool EndProgramOnClose(bool bEndOnClose);  // $QC
    
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
    bool EndProgramOnClose(int iReturnValue = 0);  // $QC

    // SendtoClipboard() -- Send contents of window to Windows Clipboard as a bitmap (so it may be pasted into other applications)
    //
    // If a Bitmap is included, the bitmap is sent to the clipoard instead.
    //
    // This transfers the visible canvas area only.  It does not copy the window frame or border. 
    //
    bool SendtoClipboard(); // $QC

    // SendtoClipboard() -- Send contents of window to Windows Clipboard as a bitmap (so it may be pasted into other applications)
    //
    // If a Bitmap is included, the bitmap is sent to the clipoard instead.
    //
    // This transfers the visible canvas area only.  It does not copy the window frame or border. 
    //
    bool SendtoClipboard(const CBitmap & cBitmap); // $QC


    // ImportClipboardText() -- Returns Text String in the Windows clipboard, if it exists.
    //
    // If there is text within the Windows Clipboard, a CString object will be returned with its contents.
    // Otherwise, an empty CString will be returned.
    //
    // a bSuccess pointer may be included which will be filled with the results (true if text was found, false if the CString returned is empty);
    //
    CString ImportClipboardText(bool * bSuccess = nullptr); // $QC

    // ImportClipboardTextW() -- Returns a Unicode Text String in the Windows clipboard, if it exists.
    //
    // If there is text within the Windows Clipboard, a CStringW object will be returned with its contents.
    // Otherwise, an empty CStringW will be returned.
    //
    // a bSuccess pointer may be included which will be filled with the results (true if text was found, false if the CStringW returned is empty);
    //
    CStringW ImportClipboardTextW(bool * bSuccess = nullptr); // $QC

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
    CBitmap ImportClipboardBitmap(bool * bSuccess = nullptr); // $QC

    bool isDevWindow() { return m_bDevWindow; } // $QC
    CSageBox * GetSageBox(); // $QC

    // CenterInlineFonts() -- When true, this will center the text when font changes occur in output with {<font>} changes.
    // For example Write("This is a {Arial45}Centered{/} Font" will center the larger font in the middle of the text line when
    // Centering is active.  Otherwise, it will put the font start at and all other text in the line at the same Y value. 
    //
    // This is primarily used for specialized text, such as a TextWidget()
    //
    bool CenterInlineFonts(bool bCenterFonts = true); // $QC


    // Functions with the same name as STDIO funcitons rerouted in Sandbox mode. 
    // We only need them during the compile for the source code; otherwise they conflict. 


// __Sandbox__Except__ deprecated (for now)
//#ifdef __Sandbox__Except__

    // printf() -- Works the same way as 'C' printf, except you can also put (X,Y) coordinates.
    // printf() can be very useful to quickly print text without using streaming notation,
    //
    // i.e. printf("This is attempt #d\n",++iAttempNo) vs. out << "This is attempt number " << ++iAttemptNo <<  "\n"
    //
    // printf(50,100,"Hello World") will print "Hello World") at (50,100) in the window and set the next write location accordingly.
    //
    void __printf(const char * Format,...);

    // printf() -- Works the same way as 'C' printf, except you can also put (X,Y) coordinates.
    // printf() can be very useful to quickly print text without using streaming notation,
    //
    // i.e. printf("This is attempt #d\n",++iAttempNo) vs. out << "This is attempt number " << ++iAttemptNo <<  "\n"
    //
    // printf(50,100,"Hello World") will print "Hello World") at (50,100) in the window and set the next write location accordingly.
    //
    void __printf(const cwfOpt & cwOpt,const char * Format,...);


    // printf() -- Works the same way as 'C' printf, except you can also put (X,Y) coordinates.
    // printf() can be very useful to quickly print text without using streaming notation,
    //
    // i.e. printf("This is attempt #d\n",++iAttempNo) vs. out << "This is attempt number " << ++iAttemptNo <<  "\n"
    //
    // printf(50,100,"Hello World") will print "Hello World") at (50,100) in the window and set the next write location accordingly.
    //
    void __printf(int iX,int iY,const char * Format,...);

    // printf() -- Works the same way as 'C' printf, except you can also put (X,Y) coordinates.
    // printf() can be very useful to quickly print text without using streaming notation,
    //
    // i.e. printf("This is attempt #d\n",++iAttempNo) vs. out << "This is attempt number " << ++iAttemptNo <<  "\n"
    //
    // printf(50,100,"Hello World") will print "Hello World") at (50,100) in the window and set the next write location accordingly.
    //
    void __printf(int iX,int iY,const cwfOpt & cwOpt,const char * Format,...);
    /// <summary>
    /// Sets the title of the Window (shown on the Window Title bar)
    /// </summary>
    /// <param name="sTitle">Name of the Window</param>
    /// <returns>TRUE if Window is valid.  FALSE is the Window is not valid.</returns>
    bool SetWindowTitle(const char * sTitle); // $QC

    /// <summary>
    /// Sets the title of the Window (shown on the Window Title bar)
    /// </summary>
    /// <param name="sTitle">Name of the Window</param>
    /// <returns>TRUE if Window is valid.  FALSE is the Window is not valid.</returns>
    bool SetWindowTitle(const wchar_t * sTitle); // $QC

                                                 /// <summary>
    /// Returns a string with version information for the Sagebox library, as well as current
    /// platform (i.e. 32 bits or 64 bits). 
    /// </summary>
    /// <returns>String with current library version and run-time platform</returns>
    CString GetVersionInfo();   // $QC

    /// <summary>
    /// returns TRUE of running 64-bit windows, false if 32-it windows. 
    /// </summary>
    /// <returns></returns>
    bool is64BitWindows();  // $QC

#ifdef __SageGDIPlusSupport
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
    bool InitGdiPlus(); // $QC

    Gdiplus::Graphics & GetGdiGraphics();   // $QC
    bool SetGdiGraphics(Gdiplus::Graphics * cGdi) { if (!this) return false; m_cGDI = cGdi; return true; };
#endif
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
    bool VsyncWait(VsyncType vSync = VsyncType::Auto);   // $QCC

    /// <summary>
    /// Returns TRUE if the VsyncWait and VsyncThread capabilities are accurate (i.e. if DirectDraw() was initialized and can look for the Vertical ReSync)
    /// <para></para>&#160;&#160;&#160;
    /// --> Note this will initialize DirectDraw() and put the application in a performance mode, using more processing time (i.e. it assumes you are using graphics by checking its status)
    /// <para></para>
    /// Use this as a DEBUG function to check the validity of Vsync operations (once true, then there is no need to check)
    /// </summary>
    /// <returns></returns>
    bool VsyncIsValid();    // $QC

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
    /// <param name="peek">Does not change the event status if Peek == Peek::Yes</param>
    /// <returns></returns>
    bool VsyncReady(Peek peek = Peek::No);  // $QCC

    /// <summary>
    /// Starts the main Vsync thread to send a VsyncReady() event to your window with the Vertical Retrace occurs on your monitor. 
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
    /// <returns></returns>
    bool VsyncStartThread();    // $QC

    /// <summary>
    /// Ends the Vsync Retrace Thread.  Use this when finished with your graphics loop using the vertical retrace. 
    /// <para></para>
    /// --> The VSync Thread is passive and is ok to leave running, as it will not take up too much processing time. 
    /// </summary>
    /// <returns></returns>
    bool VsyncEndThread();  // $QC

    /// <summary>
    /// Selects a new Windows Bitmap into the device context. This does not delete the original bitmap, which is still available for re-selection with a null parameter.
    /// <para></para>&#160;&#160;&#160;
    /// --> Note: Once a new bitmap is selected, the bitmap will not be automatically resized when window sizes change.  The bitmap can be sized to the maximum possible
    /// window size, or can be re-allocated on WM_SIZE messages from Windows by trapping these messages with a Window Handler (i.e. CWindowHandler).
    /// <para></para>
    /// Use SelectNewBitmap() with a nullptr or SelectOriginalBitmap() to revert back to the original bitmap created by Sagebox when the window was created. 
    /// <para></para>
    /// </summary>
    /// <param name="hBitmap">Windows HANDLE to the bitmap</param>
    /// <returns></returns>
    bool SelectNewBitmap(HBITMAP hBitmap);          // $QC

    bool SelectOriginalBitmap();                    // $QC

    std::tuple<HBITMAP,unsigned char *> CreateDIBitmap32(bool bUpsideDown);         // $QC     
    std::tuple<HBITMAP,unsigned char *> CreateDIBitmap32(SIZE szSize = {},bool bUpsideDown = false);         // $QC     
    std::tuple<HBITMAP,unsigned char *> CreateDIBitmap32(int iWidth,int iHeight,bool bUpsideDown = false);            // $QC

    bool SetDirtyFlag(bool bDirty = true);                                          // $QC
    bool SetDirtyFlagUpdate();                                          // $QC


    /// <summary>
    /// Waits for OS to Paint Window in progress, and locks the OS from Painting the Window until UnLockPaint() is called.
    /// <para></para>
    /// LockPaint() can be called to prevent the OS from updating the window while another thread (i.e. the procedural thread controlling the window)
    /// is also using GUI functions.  This is usually taken care of by Sagebox, but this can also be used when calling Windows functions directly.
    /// <para></para>
    /// --> Using LockPaint() keeps the OS message thread from conflicting with a main procedural thread when performing GUI functions.
    /// <para></para>
    /// Importante Note: Use UnlockPaint() to unlock when finished, otherwise your program will hang.
    /// </summary>
    /// <returns></returns>
    bool LockPaint();       // $QC

    /// <summary>
    /// Unlocks the OS Paint Lock.  see LockPaint() for more details.
    /// </summary>
    /// <returns></returns>
    bool UnlockPaint();     // $QC

   
    /// <summary>
    /// Used for functions that want to return an empty window vs. a live window (i.e. if an error occurred). 
    /// <para></para>
    /// This is typically used for Widgets and other processes that return created windows.  EmptWindow() can be returned as a safe-to-use window that will not perform any actions.
    /// <para></para>
    /// ---> use isWindowValid() (i.e. MyWindow.isWindowValid()) to determine if a window is a valid window 
    /// <para></para>
    /// ---> The window returned by GetEmptyWindow() is invalid, but can be used as an 'empty window' that won't perform actions, but will simply pass through ok.
    /// </summary>
    /// <returns>Reference to safe-to-use Empty Windows.</returns>
    static CWindow & GetEmptyWindow() { return m_cEmptyWindow; } // $QC

    __forceinline unsigned char * GetLastKnownDIBMem()    { return m_sLastKnownDIBMem; }    // $QC      -- $$ This is a temorary function and will be removed; for internal use.
    __forceinline SIZE GetLastKnownDIBSize()    { return m_szLastKnownDIBSize; }            // $QC      -- $$ This is a temorary function and will be removed; for internal use.

    bool TitleWidgetAdd(const char * sTitle,const cwfOpt & cwOpt = cwfOpt());// $QC
    bool TitleWidgetUpdate(const char * sTitle = nullptr);// $QC
    bool TitleWidgetSetText(const char * sTitle);// $QC
    bool TitleWidgetShow(bool bShow = true);// $QC
    bool TitleWidgetHide(bool bHide = true); // $QC
    bool TitleWidgetRemove();// $QC
    CTextWidget & TitleWidgetGet();// $QC

    /// <summary>
    /// Sets the Z-position of the console window directly behind the Window. 
    /// <para></para>
    /// Also see Sagebox::ConsoleSetBottom() and Sagebox::ConsoleBringForward()
    /// </summary>
    /// <returns></returns>
    bool ConsoleSetBehind(); // $QC


    /// <summary>
    /// Internal function, used for creating and maintaining Widgets in a safe environment
    /// </summary>
    /// <returns></returns>
    CDataStore * __GetDataStoreClassPointer();    // $QC

    CDataStore * __RegisterandIncrementDataStore(int & iControlID,CDataStore::DataStore_t * & stDataStore); // $QC

    int AddSysMenuItem(const char * sSysMenuItem);
    void InsertSysMenuBorder(int iMenuID);

    bool __WindowSetAsPrimary(bool bAsPrimary = true); 
    Events & GetEventStruct(); 

    /// <summary>
    /// Sets RealTime status for Window. <para></para>
    /// This sets the status of the Window for real-time drawing, includes setting the high-resolution timer and turning off Auto Updates to the Window.
    /// </summary>
    void __SetRealTime();
  
    /// <summary>
    /// Internal Use.<para></para>
    /// Sets the window as a "magic window" which has properties that allow it to automatically appear, disappear, and perform other functions based on the usage<para></para>
    /// of the window.<para></para>
    /// .<para></para>
    /// --> This is used for scripting languages like Python and only works with outside processes.  This is not a C++-based or useable function.<para></para>
    /// --> This function is for internal use. 
    /// </summary>
    /// <param name="bMagicWindow"></param>
    bool __SetMagicWindow(bool bMagicWindow);

    QuickFormType __GetQuickFormType();
    bool __isQuickFormWindow();
    CQuickForm GetQuickForm();

    /// <summary>
    /// Tells the Window to close (hide itself) automatically when the user presses the 'X' or otherwise purposely closes the window.
    /// <para></para>
    /// When set to off (false), the Window will stay displayed until the close event is handled by the program and the window is deleted or hidden manually.
    /// <para></para>
    /// This is the default behavor for top-level (i.e. Main) windows.  Use HideOnClose(false) to turn this off.
    /// <para></para>
    /// --> When the window is closed by the user, it is hidden.  A WindowClose event is active, and the Window may be shown again by calling the Window's show() function.
    /// </summary>
    /// <param name="bHideOnClose">set to 'true' to hide on close; 'false' to not hide when the user closes the window.</param>
    /// <returns></returns>
    bool HideOnClose(bool bHideOnClose = true);


    /// <summary>
    /// Returns true if the window has been designated as a RealTime or DirectDraw Window.
    /// </summary>
    /// <returns></returns>
    bool isRealTimeWindow();
 
//#endif

};
}; // namespace Sage

#endif // _CDavWindow_H_

 