#pragma once
#include "CSageBox.h"
#if !defined(_CBarGraph_H_)
#define _CBarGraph_H_

class CBarGraph
{
private:
   struct stBarGraph_t
    {
        struct stBit_t
        {
            RawBitmap_t stTopLeft		;
            RawBitmap_t stTopMiddle	    ;
            RawBitmap_t stTopRight	    ;
            RawBitmap_t stTopShadow	    ;
            RawBitmap_t stLeft		    ;
            RawBitmap_t stMiddle		;
            RawBitmap_t stRight		    ;
            RawBitmap_t stShadowRight   ;
        };

        stBit_t         stBit           ;
        int     iMinWidth               ;       // Less than width just puts a color rectangle;

        ~stBarGraph_t();
        float      fHue;

    };

    bool        m_bDrawShadow = true;
    int         m_iWinWidth = 0;
    int         m_iWinHeight = 0;
    bool        m_bInvalid = true;
    CWindow   * m_cWin;
    stBarGraph_t m_stBarGraph;

    SIZE m_szTopSize       = {};
    SIZE m_szRightSize     = {};
    SIZE m_szMiddleSize    = {};
    SIZE m_szMinSize       = {};
    struct BarType_t
    {
        SIZE szSize;        // Width/Height
        int     iStartX;    // Start location (may be set automatically)
    };

    int m_iBarListEntries   = 0;
    std::vector<BarType_t> m_vBarList; 

    CBitmap m_cBitmapWin; 
    CBitmap m_cBitmapOut;     

    static constexpr int m_iPadX = 50; 
    static constexpr int m_iPadY = 20; 

    bool LoadPGRFile();
    bool Initialize();
  
public:
    CBarGraph(CWindow & cWin);
    CBarGraph();
    bool Init(CWindow & cWin); 

    bool DrawBar(int iX,int iY,int iWidth,int iHeight,RGBColor_t rgbColor,bool bUpdate = false); 
    bool Test();

};
#endif // _CBarGraph_H_


