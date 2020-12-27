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
#if !defined(_CQuickMenu_h_)
#define _CQuickMenu_h_
#include "CDavinci.h"
#include <vector>

namespace Sage
{
class CQuickMenu : public CWindow
{
	struct event_t
	{
		friend CQuickMenu;
	private:
		bool bWindowClosed;
		bool bItemSelected;
		CQuickMenu * cMenu;
	public:
		bool WindowClosed(Sage::Peek peek = Sage::Peek::No);
		bool ItemSelected(Sage::Peek peek = Sage::Peek::No);
		int GetItem(Sage::Peek peek = Sage::Peek::No);
	};


	SageControlDelete
private:
	bool	m_bAutoClose			;
	bool	m_bRightClickCloses		;
	char  * m_sDefaultFont			;	
	int		m_iMaxItems				;	
	int		m_iMaxWidth				;	
	int		m_iPadY					;	
	int		m_iPadLeftX				;	
	int		m_iPadRightX			;	
	bool	m_bShowing			= false;
	int		m_iStaticWidth		= 0;
	int		m_iRowWidth			= 0;
	int		m_iRowHeight		= 0;
	int		m_iTextHeight		= 0;
	int		m_iWinWidth;
	int		m_iWinHeight;
	int		m_iHighItem			= 0;
	int		m_iItemSelected;
	int		m_iRangeX;		
	int		m_iRangeY;		
	bool	m_bWindowClosed		= false;
	int		m_iWinX		;
	int		m_iWinY		;

	Sage::RGBColor_t m_rgbNormFg	;
	Sage::RGBColor_t m_rgbNormBg	;

	Sage::RGBColor_t m_rgbHighFg	;
	Sage::RGBColor_t m_rgbHighBg	;
	
	Sage::RGBColor_t m_rgbBgColor	;


private:
	struct Item_t
	{
		// Currently, this contains one item, but is set as a structure as it will probably grow into either
		// a more sophisticated routine, or used for a larger routine that borrows this cod.e
		CString csItem;
	};

	std::vector<Item_t> m_vItems;


	CString m_sItems;
	int m_iNumItems = 0;
	int m_iVisibleItems = 0; 


	CWindow * m_cWin = nullptr;
	CWindow * m_cParent = nullptr;
	int m_iWidth;
	int m_iHeight;
	void CalculateWindow();
	void DrawWindow();
	void RedrawWindow();
	void SetDefaults();
	void SelectItem();

	// Message overrides
public:
	CWindowHandler::MsgStatus OnMouseMove(int iMouseX,int iMouseY) override;
	CWindowHandler::MsgStatus OnLButtonDown(int iMouseX,int iMouseY) override;
	CWindowHandler::MsgStatus OnRButtonDown(int iMouseX,int iMouseY) override;
	CWindowHandler::MsgStatus OnCaptureChanged() override;
	CWindowHandler::MsgStatus OnControlKey(Sage::ControlKey key,int iCount) override;
	CWindowHandler::MsgStatus OnChar(char cKey,int iCount) override;
	CWindowHandler::MsgStatus OnMouseWheel(int iDelta,int iX,int iY) override;

public:
	CQuickMenu();
	void Init(CWindow * cWin,CWindow * cParentWin);
	void ClearItems()
	{
		m_iNumItems = 0;
		m_sItems.ClearString();
	}
	event_t event = {};
	int AddItem(const char * sItem);
	int getItemCount() { return m_iNumItems; }
	void Show(int iX, int iY);
	void Hide(bool bSendMessage = false); 
	int getSelectedItem(bool bClearReady = false);
	bool WindowClosed();
	const char * GetItemText(int iItem);	// Useful for debugging
	void DeleteItems();

};

}; // namespace Sage
#endif //_CQuickMenu_h_