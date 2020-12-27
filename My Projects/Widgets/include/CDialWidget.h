//#pragma once

#if !defined(_CDialWidget_H_)
#define _CDialWidget_H_
#include "CSageBox.h"
#include "CQuickMenu.h"


class CDialWidget : protected CWindow
{
    SageControlDelete
private:
    enum class DebugMode
    {
        None        = 0,
        ShowData    = 1,
        Graph       = 2,
    };

    int             m_iWindowX;
    int             m_iWindowY;
    bool            m_bHide ;
    CQuickMenu    & m_cQuickMenu;
    CWindow       * m_cParentWin    = nullptr;
    CWindow       * m_cWin          = nullptr;
    SIZE            m_szWinSize;
    POINT           m_pTextStart;
    POINT           m_pCenter;
    bool            m_bShowDegrees  = false;
    int             m_iRadius;
    double          m_fRadius;
    int             m_iRadiusRange;

    double          m_fSelectAngle;
    double          m_fSelectFx;
    double          m_fSelectFy;
    DebugMode       m_DebugMode = DebugMode::None;
    
    int             m_iValue            = 0;
    double          m_fValue            = 0;
    int             m_iMinValue         = 0;
    int             m_iMaxValue         = 1000;
    bool            m_bValid            = false;

    double          m_fExtMin           = 0.0;
    double          m_fExtMax           = 500;
    int             m_iDigitWidth       = 0;        // Show floating point digits?  0 = no, > 0 = digit width
    bool            m_bButtonDown       = false;
    int             m_iLastMousePosX    = 0;
    int             m_iLastMousePosY    = 0;
    int             m_iIncResolution    = 10;
    bool            m_bValueChanged     = false;
    bool            m_bSettingValue     = false;
    int             m_iLockMisses        = 0;

    CString m_csFont; 
    Sage::RGBColor_t m_rgbGlow;
    Sage::RGBColor_t m_rgbDialText;
    Sage::RGBColor_t m_rgbDegree;

    SIZE m_szTextSize;
    double m_fMinAngle;
    double m_fMaxAngle;

    CBitmap m_cFaceOut;
    CBitmap m_cFaceScr;
    CBitmap m_cFaceOrg;
    CBitmap m_cRingMask;
    CBitmap m_cRingHigh;
    CBitmap m_cCalcMask;

    CBitmap m_cHandleGraphic;
    CBitmap m_cHandleGlowMask;

    SIZE m_szHandleSize;
    POINT m_pHandleCenter;

    CButton * m_cLeftButton     = nullptr;    // We don't have to delete these.  We can, but it is unnecessary.
    CButton * m_cRightButton    = nullptr;    // They are deleted by the window when it closes.
    struct Button_t
    {
        CBitmap cNormal;
        CBitmap cHigh;
        CBitmap cPressed;

    };
    POINT           m_pLeftButtonPos;
    POINT           m_pRightButtonPos;

    Button_t        m_stLeftButton;
    Button_t        m_stRightButton;

    void Draw(bool bRecalcImage);
    void TextDraw();
    void ApplyMask();
    void ApplyMaskDebug();
    void CalcMask();
    void CreateButtons();
    void CalcImage();
    double ValuetoAngle();
    void CalcValues();
    void DebugOutput();
    void SetValid(bool bValid,const char * sErr = nullptr);
    void GetParentBg();
    void ReadPGR();
    void SendParentMsg();
private:
    MsgStatus OnMouseMove(int iMouseX,int iMouseY)     override;
    MsgStatus OnLButtonDown(int iMouseX,int iMouseY) override;
    MsgStatus OnRButtonDown(int iMouseX,int iMouseY) override;
    MsgStatus OnLButtonUp(int iMouseX,int iMouseY) override;
    MsgStatus OnCaptureChanged() override;
    MsgStatus OnWidgetMessage(void * vpWidget,int iMessage) override;
    MsgStatus OnSageEvent() override;
    MsgStatus OnMove(int iX,int iY) override;
    bool LockProcessBusy() override;

public:
    class Event
    {
        friend CDialWidget;
    private:
        CDialWidget * cDial;
    public:
        bool ValueChanged(Sage::Peek peek = Sage::Peek::No);
        bool ValueChanged(double & fValue);
    };

    Event event;
    CDialWidget(CWindow * cParent,int iWindowX,int iWindowY,bool bHide = false);
    CDialWidget(CWindow * cParent,POINT pLoc,bool bHide = false);

    void Init();
    void Show();
    int GetRawValue();
    void GetRawRange(int & iMin,int & iMax);

    double GetValue();
    bool ValueChanged(Sage::Peek peek = Sage::Peek::No);
    bool ValueChanged(double & fValue);
    bool SetRange(double fMin,double fMax);
    bool SetDegreeDisplay(bool bDisplay);
    bool ShowFraction(bool bShow);
    bool SetLocation(int iX,int iY);
    bool SetValue(double fValue);
    SIZE GetWindowSize();
    POINT GetWinLocation();
};
#endif // _CDialWidget_H_

