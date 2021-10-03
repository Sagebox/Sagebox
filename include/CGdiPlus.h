#pragma once
#include "Sage.h"

// CGdiPlus -- A GDI+ Passthrough to help make functionality easier.  This can be used in place of a Gdiplus::Graphic object, or inconjunction with one. 
// You can also just call cWin.GetGdiGraphic() to get a Gdiplus::Graphic object (which will also initialize GDIPlus for the window)
//
// You can also simply used GDI Plus without Sagebox, by initializing the GDI Plus object with the Sagebox Window's DC obtained by calling CWindow::GetCurDC(),
// and then using ForceUpdate() instead of Update(), since Sagebox won't know if the window needs refreshed. 
//
// ************ This module is experimental and in-progress -- there are only a few functions imported at the moment **************
//
namespace Sage
{
 class CWindow;
class CGdiPlus
{
    using GdiReal = Gdiplus::REAL; 
    static constexpr Gdiplus::Status kInvalidError = Gdiplus::Status::WrongState;
    bool m_bInvalid = true; 
    Gdiplus::Graphics & m_cGraphic; 
    CWindow & m_cWin;
    Gdiplus::SolidBrush * m_cBrush; 
    Gdiplus::SolidBrush * m_cBgBrush; 
    Gdiplus::Pen * m_cPen;
public:
    Gdiplus::Graphics & GetGraphicObject();
    CGdiPlus(CWindow & cWin);
    Gdiplus::Status TranslateTransform(Gdiplus::MatrixOrder order = Gdiplus::MatrixOrderPrepend);
    Gdiplus::Status TranslateTransform(double dx,double dy,Gdiplus::MatrixOrder order = Gdiplus::MatrixOrderPrepend);
    Gdiplus::Status TranslateTransform(int iX,int iY,Gdiplus::MatrixOrder order = Gdiplus::MatrixOrderPrepend);
    Gdiplus::Status ResetTransform();
    Gdiplus::Status RotateTransform(double fAngle,IN Gdiplus::MatrixOrder order = Gdiplus::MatrixOrderPrepend);
    Gdiplus::Status RotateTransform(int iAngle,IN Gdiplus::MatrixOrder order = Gdiplus::MatrixOrderPrepend);
    Gdiplus::Status Clear(const CRgb & cColor = CRgb::EmptyColor);
   
    Gdiplus::Status FillRectangle(int iX,int iY,int iWidth,int iHeight,const CRgb & cColor = CRgb::EmptyColor); 
    Gdiplus::Status SetSmoothingMode(Gdiplus::SmoothingMode smoothingMode);

    Gdiplus::Status SetBrushColor(CRgb & cColor); 
    Gdiplus::Status SetPenColor(CRgb & cColor);
    Gdiplus::Status SetSolidColor(CRgb & cColor); 
    Gdiplus::Status SetBgColor(CRgb & cColor); 
};
}