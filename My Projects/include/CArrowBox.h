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

	CSageBitmap m_cUpMask				;
	CSageBitmap m_cUpShadowMask			;
	CSageBitmap m_cUpBevelMask			;
	CSageBitmap m_cDnMask				;
	CSageBitmap m_cDnShadowMask			;
	CSageBitmap m_cDnBevelMask			;
	CSageBitmap m_cUpPressedMask		;
	CSageBitmap m_cUpPressedBevelMask	;
	CSageBitmap m_cDnPressedMask		;
	CSageBitmap m_cDnPressedBevelMask	;

	CSageBitmap m_cUpGraphic;
	CSageBitmap m_cDnGraphic;

	CSageBitmap m_cUpPrGraphic;
	CSageBitmap m_cDnPrGraphic;

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

	void MergeBevel(CSageBitmap cGraphic,CSageBitmap cBevelMask); 
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