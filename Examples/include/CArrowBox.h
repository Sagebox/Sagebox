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

#pragma once
#include "CSageBox.h"

#if !defined(_CArrowBox_H_)
#define _CArrowBox_H_
namespace Sage{



class CArrowBox
{
private:
	class CArrowHandler : public Sage::CButtonHandler
	{
	public:
		enum class Direction
		{
			Up,
			Down,
		};

		Direction m_eDirection;
		HWND m_hWnd = nullptr;

		CArrowHandler(Direction direction,HWND hWnd) : CButtonHandler()
		{
			m_eDirection = direction;
			m_hWnd = hWnd;
		}


		MsgStatus OnPressed() override;
	};
	CArrowBox(const CWindow &p2) { }		// To prevent object copying.  There should be only one version.
private:
	CArrowHandler * m_HandlerUp = nullptr;
	CArrowHandler * m_HandlerDn = nullptr;

	CDavinci * m_cDavinciMain = nullptr;

	bool m_bInvalid = true;
	CWindow * m_cWin = nullptr;
	CWindow * m_cParentWin = nullptr;
	CEditBox * m_cEditBox = nullptr;
	CButton * m_cButtonUp	  = nullptr;
	CButton * m_cButtonDown	  = nullptr;

	CBitmap m_cUpMask				;
	CBitmap m_cUpShadowMask			;
	CBitmap m_cUpBevelMask			;
	CBitmap m_cDnMask				;
	CBitmap m_cDnShadowMask			;
	CBitmap m_cDnBevelMask			;
	CBitmap m_cUpPressedMask		;
	CBitmap m_cUpPressedBevelMask	;
	CBitmap m_cDnPressedMask		;
	CBitmap m_cDnPressedBevelMask	;

	CBitmap m_cUpGraphic;
	CBitmap m_cDnGraphic;

	CBitmap m_cUpPrGraphic;
	CBitmap m_cDnPrGraphic;

	static constexpr const char * sButtonStyleUp = "CArrowBox_ButtonUp";
	static constexpr const char * sButtonStyleDn = "CArrowBox_ButtonDn";

	static constexpr RGBColor_t rgbBlack = { 0,0,0};
	static constexpr RGBColor_t rgbWhite = { 255,255,255 };
	void CalcEditBoxWindow();

	int m_iX = 0;			
	int m_iY = 0;			
	int m_iWindowWidth = 0;
	int m_iWindowHeight = 0;
	int m_iBitmapWidth;
	int m_iBitmapHeight;
	int m_iWidthBytes;
	int m_iOdd;
	void ReadBitmaps();
	void CreateButtonBitmaps();
	void CreateButtonStyle();

	void MergeBevel(CBitmap cGraphic,CBitmap cBevelMask); 
	void Init(CWindow * cParentWin,CEditBox * cEditBox);
protected:
friend CEditBox;
	void RemoveDeleter();
	CArrowBox(CWindow * cParentWin,CEditBox * cEditBox) { Init(cParentWin, cEditBox);	}
public:
	CArrowBox(CWindow * cParentWin) { m_cParentWin = cParentWin; }
	~CArrowBox();

	bool isValid() { return !m_bInvalid; };
	bool Show(bool bShow = true);
	bool Hide(bool bHide = true);
	bool UpdateBg(bool bUpdateNow = true);
	SIZE GetWindowSize();
	POINT GetLocation();
};
}; // namespace Sage
#endif // _CArrowBox_H_