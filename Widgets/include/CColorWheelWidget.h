//#pragma once
#if !defined(_CColorWheelWidget_H_2)
#define _CColorWheelWidget_H_2

#include "CSageBox.h"
#include "CPoint.h"
#include "CSageTools.h"

using namespace opt;

// Note: These should be moved to the .cpp file

#define pi 3.14159265358979
#define Rad90 (pi/2.0);
#define Rad180 (pi)
#define Rad360 (2.0*pi)
#define Rad(_deg) (_deg*pi/180)
#define Deg(_rad) (_rad*180/pi)

class CColorWheelWidget : private CWindow
{
public:
    static int m_iRegistryValue;
    static int Register(CSageBox & cSageBox);
public:
    class CWidget : public Sage::CWidget
    {
    public:
        CColorWheelWidget * m_cObj = nullptr;

        CWidget(CColorWheelWidget * m_cObj);

        CWindow * GetWindow();
        bool SetLocation(int iX,int iY);
        bool isValid();
        bool Show(bool bShow);
        bool Hide(bool bHide);
        int GetID();
        const char * GetName();

    };
    SageControlDelete
public:
        struct Event
        {
            friend CColorWheelWidget;
        private:
            CColorWheelWidget * cWidget;
        public:
            bool ValueChanged(bool bPeek = false);
            bool ValueChanged(Sage::RGBColor_t & rgbColor);

        };
private:
    static CColorWheelWidget m_cEmpty;

    CWidget     m_cWidget;
    CWindow   * m_cWin      = nullptr;;
    bool        m_bValid    = false;
    CWindow   * m_cParent;
    int         m_iWindowX;
    int         m_iWindowY;
    bool        m_bJustLeft;
    bool        m_bJustRight;
    bool        m_bJustBottomRight;
    bool        m_bJustBottomLeft;
    bool        m_bJustTopRight;
    bool        m_bJustTopLeft;

    RGBColor_t     m_rgbHardColor;      // When defined, this is a preset color (such as when SetRgbColor() is called)
                                        // and is returned for the value until the color is changed again.  This is
                                        // Becsause we can't be precise with the display as it doesn't have the resolution.
    HSLColor_t m_hslHardColor;          // HSL version
   
    enum class MouseGrab
    {
        BigCircle,
        SmallCircle,
        None,
    };

    MouseGrab m_eMouseGrab;
    MouseGrab m_eMouseHigh;
    struct RingBitmaps
    {
        POINT pCenter;
        CBitmap cRingMask;
        CBitmap cGlowMask;
        CBitmap cShadowMask;
    };

    SIZE        m_szTriangleSize;
    POINT       m_pTriangleStart;
    int         m_iWheelRadius; 
    int         m_iCircleWidth;
    int         m_iCircleRadius;
    double      m_fWheelRadius; 
    CPoint      m_pWheelCenter;
    POINT       m_pLastMouse;
    POINT       m_pSelectMouse;
    CfPoint     m_fpWheelCenter;
    CfPoint     m_fpBigRingCenter;

    RingBitmaps m_stBigRing;
    RingBitmaps m_stSmallRing;

    CBitmap     m_cShadowMask;
    CBitmap     m_cWheelMask;
    CBitmap     m_cPlane;    
    CBitmap     m_cTriMask;
    CBitmap     m_cTriPlane;

    CBitmap     m_cBackground;
    CBitmap     m_cOutput;
    CBitmap     m_cParentBg;

    Sage::RGBColor_t m_rgbBg;
    Sage::RGBColor_t m_rgbMaskColor;

    CPoint      m_pTriPoint[3];
    CfPoint     m_fRingLoc;
    double      m_fAngleLeft;
    double      m_fAngleRight;
    double      m_fAngleTop;
    double      m_fSelectAngle;

    int         m_iWinWidth;
    int         m_iWinHeight;
    int         m_iBitmapWidth;
    int         m_iBitmapHeight;
    int         m_iBitmapWidthBytes;

    Sage::HSLColor_t m_hslSelect;
    bool m_bTransparent;

    bool    m_bShowDebug           = false;
    bool    m_bValueChanged        = false;
    bool    m_bHighlighted         = false;


    void Init(CWindow * cParent,int iX,int iY,const char * sControl = nullptr);
    void SetDefaults();
    void ReadPGR();
    void CreateWheel();
    void CreateTriangle(double fHue);
    void FillBackground();
    void BuildOutput();
    void AddBigRing(double fAngle);
    void AddSmallRing(POINT pMouse);
    void CalcValues(bool bSendMessage);
    void Redraw();
    void SetOptions(const char * sControls);

    CfPoint GetIntersect(CPoint & p1, CPoint & p2);
    CfPoint DrawEdgeCircle(POINT & pMouse);
    double toAngle(double fx,double fy);

    // Event overloads
private:
    MsgStatus OnMouseMove(int iMouseX,int iMouseY) override;
    MsgStatus OnLButtonDown(int iMouseX,int iMouseY) override;
    MsgStatus OnLButtonUp(int iMouseX,int iMouseY) override;
    MsgStatus OnCaptureChanged() override;
    MsgStatus OnWidgetMessage(void * cWidget,int iMessage);
    MsgStatus OnMove(int iX,int iY);

    void RedrawThreaded();
protected:
    virtual void OnValueChange() { }    // Override function to get value changes     

public:
    CColorWheelWidget(CWindow * cParent,int iX,int iY);
    CColorWheelWidget(CWindow * cParent,int iX,int iY,const cwfOpt & cwOpt);
    ~CColorWheelWidget();
    static CColorWheelWidget & AttachNew(CDialog & cDialog,int iX,int iY,const cwfOpt & cwOpt = cwfOpt());
    static CColorWheelWidget & GetEmptyObject();
    static CColorWheelWidget & Get(CDialog & cDialog,int iD);
    static CColorWheelWidget & Get(CDialog & cDialog,const char * sName);

    // Access Functions

    Sage::RGBColor_t GetRGBValues();
    Sage::HSLColor_t GetHSLValues();
    bool ValueChanged(bool bPeek = false);
    bool ValueChanged(Sage::RGBColor_t & rgbColor);
    void SetDebug(bool bDebug);
    void SetLocation(int iX,int iY);
    void Show(bool bShow = true);
    void Hide(bool bHide = true);
    void UpdateBg(bool bRedraw = true);
    void Update();
    bool isValid();
    bool SetRGBValue(RGBColor_t rgbColor);
    SIZE GetWindowSize();
    POINT GetWindowPos();
    Event event;                                        // Event functions, but can be used directly (i.e. ValueChanged())

};
#endif // _CColorWheelWidget_H_2