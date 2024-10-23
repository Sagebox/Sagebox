#pragma once

#if !defined(_CLCDDisplayWidget_H_)
#define _CLCDDisplayWidget_H_


#include "CSageBox.h"
#include "CDataStore.h"

class CLcdDisplayWidget 
{
    SageControlDelete
public:
    static constexpr const char * kWinTitle = "Sagebox LCD Widget";
        enum class Mode
        {
            PlainLed = 0,
            BlueLed,
        };

        struct Led_t
        {
            int iWidth;
            int iHeight;
            CBitmap cBitmaps[13];
        };
private:
        CDataStore * m_cDataStore = nullptr;
        int m_iWinControlID = 0;
        int m_iPopupControlID = 0;
        CDataStore::DataStore_t * m_stPopupDataStore = nullptr;
        CDataStore::DataStore_t * m_stWinDataStore = nullptr;
        class WindowHandler : public CWindowHandler
        {
            CLcdDisplayWidget * cLcd = nullptr;
        public:
            virtual void Init(void * pClassInfo) override;
            virtual MsgStatus OnMove(int iX,int iY) override;
            virtual MsgStatus OnWidgetMessage(void * cWidget,int iMessage) override;

        };
        enum LCDPLace
        {
            colon = 10,
            blank = 11,
            dash = 12,
        };    
        bool m_bInvalid = true; 

        // "hardware"-dependent values

        static constexpr int    kMaxValue = 999999;
        static constexpr int    kMinValue = 0;
        WindowHandler m_WindowHandler;
private:
#define kMaxDigits 6
    int                 m_iNumModes = 2;    // $$ constexpr
    int                 m_iMaxDigits = kMaxDigits;    //  constexpr
    int                 m_iPopupX = 0;
    int                 m_iPopupY = 0;
    int                 m_iWindowX = 0;
    int                 m_iWindowY = 0;
    int                 m_iWinWidth;
    int                 m_iWinHeight;
    CWindow           * m_cParent           = nullptr;
    CWindow           * m_cWin              = nullptr;
    bool                m_bTransparent;
    bool                m_bHidden;
    bool                m_bCenterX;         // Center LCD Window in Parent Window X
    bool                m_bCenterY;         // Center LCD Window in Parent Window Y
    int                 m_iPadX;            // X Pixel Placement Adjust for automatic placements (i.e. Center or CenterX)
    int                 m_iPadY;            // Y Pixel Placement Adjust for automatic placements (i.e. CenterY)
    bool                m_bPopup;           // Popup an LCD Widget Window
    Mode                m_eMode;
    Led_t               m_stLeds[2] = {};
    Led_t             * m_stCurLed          = nullptr;
    POINT               m_pTextLoc;
    CBitmap             m_cBmpEmulatorFace;
    CBitmap             m_cBmpBackground;
    CBitmap             m_cBmpOutput;    
    bool                m_bFastMode;
    int                 m_iUpdateMS = 0;
    Sage::RGBColor_t    m_rgbBgColor;
    bool                m_bAllowMove;
    int                 m_iValue = 0;
    int                 m_iMinValue;
    int                 m_iMaxValue;
    bool                m_bInitialUpdate    = false;
    CWindow           * m_cPopupWin         = nullptr;
    CTextWidget       * m_cTextWidget       = nullptr;
    CPoint              m_pPopupLoc = { 5, 5 };   

    CString             m_csTitle;          
    CString             m_csUserTitle;           
    CString             m_csText;           
    int                 m_iTextFont = 21;
    SIZE                m_stTextSize{};

private:
private:
    void Init(CWindow * cParent, int iX,int iY,const kwOpt & keywords);
    void LocalSetDefaults();
    void ReadPGR();
    bool PrintValue();
    void FillBackground();
    bool Redraw(bool bForce = false);
    void CalcImage();
    void ReadOptions(const kwOpt & keywords);
    bool DisplayBlank();
    void ForceRedraw();
    bool CreatePopupWindow();
    bool InitPopupWindow();

public:
    CLcdDisplayWidget(CWindow * cParent, int iX,int iY, int iInitialValue = 0, const kwOpt & keywords = kw::none);
    CLcdDisplayWidget(CWindow * cParent,POINT pLoc, int iInitialValue = 0, const kwOpt & keywords = kw::none);
    CLcdDisplayWidget(int iX,int iY, int iInitialValue = 0, const kwOpt & keywords = kw::none);
    CLcdDisplayWidget(POINT pLoc, int iInitialValue = 0, const kwOpt & keywords = kw::none);
    ~CLcdDisplayWidget();

    virtual void SetDefaults()    { }

    // Access Functions

    void SetFastMode(bool bFastMode);
    void SetLocation(int iX,int iY);    // Set the location of the window
    void AllowDrag(bool bAllowDrag);    // Allow movement by placing mouse on window and moving it. 
    void UpdateLast();                    // Update last value (for last update when FastMode = true)
    int GetValue(); 
    bool SetValue(int iValue); 
    void UpdateBackground();
    void Show(bool bShow = true);
    void Hide(bool bHide = true);
    void SetMode(Mode mode);
    SIZE GetWindowSize();
    POINT GetWinLocation();
    SizeRect GetSizeRect(); 

    /// <summary>
    /// If the LCD Window is a popup window, WindowClosing() returns true of the window was closed (i.e. 'X' button pressed) or it is closing
    /// for some other reason.  
    /// <para></para>
    /// --> This is not an event, and will return TRUE on successive calls after returning TRUE the first time.
    /// <para></para>
    /// --> If the LCD Window is not a popup window, WindowClosing() always returns false.
    /// </summary>
    /// <returns></returns>
    bool WindowClosing();

    void debugSetUpdateMS(int iUpdateMS); 
};

#endif // _CLCDDisplayWidget_H_

