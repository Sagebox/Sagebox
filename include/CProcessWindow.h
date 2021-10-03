#pragma once

#include "CSageBox.h"
#include "Windowsx.h"

namespace Sage
{

class CProcessWindow
{
public:

    enum class StartMode
    {
        Normal,         // Visible title bar only
        Hidden,
        Debug           // Debug Window showing. 
    };

    enum class Control
    {
        Color       = 1,
        LBG,
        SetX,
        TabPos,
        Div,
        Bold,
        Italic,
        BoldItalic,
        NoControl,
        Tab,            // Used for 0x09 (to form tab stop-style tabs)
        __end
    };

    struct stControl_t
    {
        Control eControl;
        const char * sControl;
        int iExtraData;
    };

    enum class ColorIndex
    {
        Green                   = 0,
        Magenta                     ,
        Red                         ,
        Yellow                      ,
        Blue                        ,
        Cyan                        ,
        Black                       ,
        White                       ,
        Gray                       ,
        LightGray                   ,
        Default                     ,
    };
    enum class UpdateType
    {
        Off,
        OffOnce,
        On,
    };

private:

    struct stButton_t
    {
        CBitmap cNormal;
        CBitmap cPressed;

        POINT   pLoc;
        bool bShow;
    };

    static constexpr int m_iMaxTextLines       = 2000;
    static constexpr int m_iMaxTextWidth       = 300;    // Width including control items. 

    int m_iLinePtr        = -1;     // Internal current line pointer (-1 to force initialization)
    int m_iNumlines       = 0;      // Current line count (always increasing)
    int m_iLastScrollPos  = -1;
    bool m_bCreateNewLine   = false;

    struct stTextLine_t
    {
        int iCurPos;
        bool bDiv;                                      // When true, we're divider
        RgbColor bgColor; 
        char sRawLine[m_iMaxTextWidth+1];                 // Raw line as sent -- $$ This may not be used
        unsigned char sControlLine[m_iMaxTextWidth+2];  // Output control line (+2 to ensure room for 0x00 at end)
    };

    // For now, text controls start with non-printable chars underneath 0x20. 
    // If the list grows >= 32, then sControlLine will either need to be changed to an 
    // <int> type, or StartText/EndText will need to be added. 
    //
    // (i.e. the user nmay want to use any value > 32, so there is no space in an unsigned char format)
    //
    // note: This is also using the assumption that \0x9 (TAB) in the text stream is pre-converted to a tab or tab stop. 
    //
    enum class TextControl
    {
        Color       = 0x01  ,
        Divider             ,
        LineBG              ,
        SetX                ,
        Bold                ,
        Italic              ,
        BoldItalic          ,
        _end,           // i.e. end of control, last "}"
    };

    bool                           m_bActive            = false; // true if instanciated, active, and everything is ok
    bool                           m_bInvalid           = false; // When true, something fatal happened
    bool                           m_bHookTransit       = false;
    bool                           m_bShowInitial       = true;
    bool                           m_bVisible           = false; 
    bool                           m_bDisableMiniWin    = false;
    int                            m_iLinePostSpace     = 3;     // Read in from PGR
    int                            m_iTitleBarHeight    = 10;    // Read in from PGR     
    int                            m_iWinSizeNormal     = 20;    // Read in from PGR

    int                            m_iPrintableLines    = 1;   // Printable line in debug window (calculated)
    int                            m_iDebugRowOffset    = 0; 
    HHOOK                          m_hMouseHook = nullptr;
    
    static CProcessWindow         * m_cHookProcess;
    RgbColor      m_rgbLineNumBg      = { 73 ,73 ,73  };
    RgbColor      m_rgbLineNumFg      = { 190,190,190 };
    HBRUSH        m_hLineNumBrush     = nullptr;
    static constexpr const char  * m_sDebugFont        = "Arial";
    static constexpr int           m_iDebugFontSize    = -12;
    static constexpr const char  * m_sFontLineNum      = "Consolas";
    static constexpr int           m_iFontLineSize     = -11;
    static constexpr int           m_iMoveWindowMaxY   = 35;   // How many pixels in the upper-part of the window are used for NCHItTest
    static constexpr int           m_iWriteLineSize    = 2000;  // Max amount for printf() and other textout functions.
    static constexpr int           m_iTimerID          = 0x1234;
    stTextLine_t * m_stTextLines = nullptr;

    SIZE m_szWinSizes[3];
    SIZE m_szCloseButton;                               // $$ this needs to be a proper button
    bool m_bShowDebug           = false; 
    bool m_bShowLines           = false;
    bool m_bShowDebugInitial    = true;                    // Show Debug window on first debug output when true
    bool m_bFirstWrite          = false;                         // Filled when first write occurs or debug window shown otherwise. 
    bool m_bHiddenOnce          = false;
    bool m_bHelpPressed         = false;
    bool m_bHelpActive          = false; 
    bool m_bTimerActive         = false;
    bool m_bMiniWin     = true;                         // Show Windoiw as title bar only when mouse not in range (not captured)
    bool m_bMouseCaptured = false; 
    bool m_bMovingWindow = false;
    int  m_iScrollbarWidth = 12;
    HWND m_hWndScrollBar = nullptr;
    bool m_bScrollbarVisible = false;
    POINT m_pMoveWinLoc;
    POINT m_pMoveWinStart;

    stButton_t  m_stButtonTerminate; 
    stButton_t  m_stCheckboxShowDebug[2]; 
    stButton_t  m_stCheckboxShowLines[2]; 
    stButton_t  m_stButtonHelp[2]; 
    SizeRect    m_szDebugRect;

    CBitmap         m_cbBackdrop; 
    CBitmap         m_cbDebugWindow; 
    CBitmap         m_cBitmapHelp; 
    CBitmap         m_cBitmapSageboxHeader;
    POINT           m_pDebugWindow;

    RgbColor        m_rgbDebugText; 
    RgbColor        m_rgbDebugBg; 


    enum class DebugFont : int
    {
        Normal,
        Bold,
        Italic,
        BoldItalic,
    };

    struct stFont_t
    {
        HFONT hFont;
        SIZE szMetric; 
        int iCharWidths[256];
        int iMaxLines;
        int iBlankSpace;
    };
    stFont_t      m_stDebugFonts[4]{};
    stFont_t      m_stFontLineNum{};

    char            * m_sWriteLine = nullptr;
    bool        m_bInit                 = false; 
    bool        m_bTerminatePressed     = false;
    UpdateType  m_eUpdateType           = UpdateType::On;

    HWND        m_hWnd          = nullptr;
    HINSTANCE   m_hInstance     = nullptr;
    DWORD       m_dwThreadID;

    HDC         m_hDCDebug          = nullptr;
    HBITMAP     m_hDebugBitmap      = nullptr;
    HBITMAP     m_hMainBitmap       = nullptr;
    HBITMAP     m_hOldMainBitmap    = nullptr;
    HDC         m_hMainDC           = nullptr;
    HBITMAP     m_hOldDebugBitmap   = nullptr;
    HBRUSH      m_hDebugBrush       = nullptr;
    HWND        m_hHelpWindow       = nullptr;
    static constexpr const TCHAR * m_szTitle                = L"Quick C++ Debug Window";    // Title Bar Text
    static constexpr const TCHAR * m_szWindowClass          = L"QUICKCPPDEBUG";
    static constexpr const TCHAR * m_szHelpWindowClass      = L"QUICKCPPDEBUG_HELP";
    ATOM RegisterClass();
    bool APIENTRY WinMain();
    bool InitInstance();
    static LRESULT CALLBACK _WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK _WndProcHelp(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    LRESULT CALLBACK WndProcHelp(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    bool ButtonPressed(stButton_t & stButton,POINT pLoc); 

    bool DisplayBitmap(HDC hDC,CBitmap & cBitmap,POINT pLoc = {});
    bool DisplayBitmap(HWND hwnd,CBitmap & cBitmap,POINT pLoc = {});
    bool DisplayButton(HDC hDC,stButton_t & stButton,bool bPressed);
    bool DrawWindow(HDC hDC);
    bool UpdateDebugWindow();
    bool RedrawDebugWindow(HDC hDC = nullptr); 

    bool SetWindowSize(bool bPositionWindow);
    bool TranslateLine(const char * sText);
    bool InitDataPGR();
    bool InitData();
    bool InitFonts(HDC hDC = nullptr);
    bool InitDebug(HDC hDC); 
    void InitTextLine(stTextLine_t & stTextLine); 
    bool ReleaseMouseHook();

    Control FindControlName(const char * sControl,const stControl_t * stControls,const stControl_t * *stRetControl = nullptr);
    static LRESULT CALLBACK _HandleHook(int nCode, WPARAM wParam, LPARAM lParam);
    void HandleHook(MSLLHOOKSTRUCT & stHook);
    void HandleTimer();
    bool Show(bool bShow);
    bool CaptureMouse();
    bool ReleaseCapture();
    void SendUpdateMsg(bool bForce = false);
    bool WriteText(const char * sText);
    bool DisplayHelpWindow();
    void SetWndTimer();
    void KillWndTimer();
    bool UpdateScrollBar();
    bool ShowScrollBar(bool bShow);
    bool HandleScroll(int iOffset);
    bool GetValidProcess();
public:
    bool StartProcess(HINSTANCE hInstance,StartMode eStartMode); 
    static DWORD WINAPI ThreadProc(LPVOID lpProcess);
   // void printf(const char * Format,...);
    void printf(const char * Format,...);
    bool Write(const char * sText);
    bool isActive() { if (!this) return false; return m_bActive && !m_bInvalid; }
    bool ShowWindow(bool bShow,bool bShowDebug);
    bool isVisible();
    bool _KillTimer(bool bKillTimer);
    bool ShowLineNumbers(bool bShowLineNumbers);
    bool SetUpdateType(UpdateType eType);
    UpdateType GetUpdateType();
    bool Update();
    ~CProcessWindow();


};

}; // namespace Sagebox