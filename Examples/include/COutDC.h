// This file copyright(c) 2021 Rob Nelson, All Rights Reserved.    E-mail rob@projectsagebox.com for more information.
//

// --------------------------------------------------------------------------------------------------------------
// PRELIMINARY VERSION.  This file has been brought into the Sagebox project from the original sources and has 
// not yet been commented for Sagebox, or properly formatted (I used tabs and am converting to spaces).
// --------------------------------------------------------------------------------------------------------------
//
// ****** UNDER CONSTRUCTION ******
//
// This file is still under construction and may not yet include specifics, awaiting proper testing and integration into Sagebox.
//

//#pragma once

#if !defined(_COutDC_H_)
#define _COutDC_H_
#include <Windows.h>
#include "Sage.h"

namespace Sage
{

class CPasWindow;
class CWindow;
class COutDC
{
private:
	bool m_bInvalid = false;		// Used for user-based functions to avoid crashing
	CPasWindow	* m_cMain;
	int			m_iWidth			= 0;
	int			m_iHeight			= 0;
	HBITMAP		m_hOldBitmap		= (HBITMAP) nullptr;
	HBITMAP		m_hNewBitmap		= (HBITMAP) nullptr;
	HDC m_hdc = (HDC) nullptr;
	HDC m_horgDC = (HDC) nullptr;

	BITMAPINFOHEADER CreateBitmapInfoHeader32(int iWidth,int iHeight);
	BITMAPINFOHEADER CreateBitmapInfoHeader24(int iWidth,int iHeight);
	[[nodiscard]] Sage::RawBitmap32_t GetDCBitmap();
	[[nodiscard]] Sage::RawBitmap_t GetDCBitmap24();
	void ClearBitmap();
	bool WriteHDCText(int iX,int iY,char * sText,HDC hdc,Sage::RGBColor_t fgColor);
	bool WriteHDCText(int iX,int iY,wchar_t * sText,HDC hdc,Sage::RGBColor_t fgColor);
	void Init(CPasWindow * cMain,HDC hDC,int iWidth,int iHeight);
public:
	COutDC(CPasWindow * cMain,HDC hdc,int iWidth=0,int iHeight=0);
	COutDC(CWindow * cWin,int iWidth=0,int iHeight=0);
	[[nodiscard]] Sage::RawBitmap32_t CreateTextPlate(wchar_t * sText,int iX,int iY,HFONT hMyFont);
	[[nodiscard]] Sage::RawBitmap32_t CreateTextPlate(char  * sText,int iX,int iY,HFONT hMyFont);
	[[nodiscard]] Sage::RawBitmap32_t CreateTransparencyPlate(HDC hSourceDC,int iX,int iY);
	[[nodiscard]] Sage::RawBitmap_t CreateTransparencyPlate24(HDC hSourceDC,int iX,int iY);
	[[nodiscard]] Sage::RawBitmap_t CreateTransparencyPlate24(CWindow * cWin,int iX = 0,int iY = 0);
	SIZE GetTextSize(char * sText,HFONT hFont);
	SIZE GetTextSize(wchar_t * sText,HFONT hFont);
	bool SetSize(int iWidth,int iHeight);
	~COutDC();
};

}; // namespace Sage
#endif //_COutDC_H_