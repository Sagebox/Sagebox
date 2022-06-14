#pragma once

#include "Sagebox.h"

namespace Sage
{

class CBmpTransform
{
private:
    HBITMAP m_hBitmap       = (HBITMAP) nullptr;
    HBITMAP m_hBitmapMask   = (HBITMAP) nullptr; 
    HGDIOBJ m_hOldBitmap    = (HGDIOBJ) nullptr; 
    HDC m_hDC               = (HDC) nullptr;
    bool m_bError           = true;
    int  m_iWidth           = 0;
    int  m_iHeight          = 0;
    bool CreateTransform(CWindow & cWin,CBitmap & cBitmap,int iThreshold);



public:

    __forceinline HDC GetDC() { return m_hDC; }
    __forceinline HBITMAP GetMask() { return m_hBitmapMask; }
    __forceinline int GetWidth() { return m_bError ? 0 : m_iWidth; }
    __forceinline int GetHeight() { return m_bError ? 0 : m_iHeight; }

    __forceinline SIZE GetSize() { return m_bError ? SIZE{} : SIZE{ m_iWidth, m_iHeight}; }
    bool DisplayTransform(CWindow & cWin,int iX,int iY,double fAngle,double fZoom = 1.0,FlipType eFlip = FlipType::None);
    bool DisplayTransform(CWindow & cWin,POINT pLoc,double fAngle,double fZoom = 1.0,FlipType eFlip = FlipType::None);
    bool isValid() { return this != nullptr && !m_bError; }

    CBmpTransform(CWindow & cWin,CBitmap & cBitmap,int iThreshold = 127)
    {
        CreateTransform(cWin,cBitmap,iThreshold); 
    }
    static bool DisplayTransform(CWindow & cWin,int iX,int iY,CBitmap & cBitmap,double fAngle,double fZoom = 1.0,FlipType eFlip = FlipType::None);
    static bool DisplayTransform(CWindow & cWin,POINT pLoc,CBitmap & cBitmap,double fAngle,double fZoom = 1.0,FlipType eFlip = FlipType::None);

    ~CBmpTransform()
    {
        if (m_hDC)            SelectObject(m_hDC,m_hOldBitmap);
        if (m_hBitmapMask)    DeleteObject(m_hBitmapMask);
        if (m_hBitmap)        DeleteObject(m_hBitmap);
        if (m_hDC)            DeleteDC(m_hDC); 
    }
};

} // namespace Sage