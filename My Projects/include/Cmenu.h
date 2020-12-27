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

#if !defined(_CMenu_H_)
#define _CMenu_H_

#include "CWindow.h"
#include <vector>
namespace Sage
{

class CMenu
{
private:
	CWindow			* m_cWin		= nullptr;
	CPasWindow		* m_cWinCore	= nullptr;
	bool m_bPopup = false;
	int m_iControl = 0;
	int m_iBaseMenuValue = 0;
	int m_iLastMenuValue = -1;
	int m_iNumValues = 0;

	// MenuItems_t TBD -- some values may not be used

	struct MenuItems_t
	{
		int iMenuValue; 
		bool bEnabled; 
		bool bValid;
	};
	std::vector<MenuItems_t> m_vMenuItems; 

public:
	CMenu();
	CMenu(CWindow * cWin,int iControl,int iBaseMenuValue = 0);

	HMENU hMenu = (HMENU) nullptr;
	bool SetCheck(int iMenuID,bool bCheck = true); 
	bool SetActiveCheck(int iValue);
	bool MenuItemSelected(int iMenuItem,int & iMenuValue);
	bool CheckBoolItem(int iMenuItem,int iMenuItemCheck,bool & bValue);

	int AddMenuItem(const char * sItem,int iItemID = -1,const char * sControl = nullptr);
	int AddMenuItem(const char * sItem,CMenu * cMenu,const char * sControl=NULL);
	int AddMenuItem(const char * sItem,CMenu & cMenu,const char * sControl=NULL);

	bool SetCloseButtonItem(int iMenuItem = 0);
	int AddSeparator();
	int ActivateMenu(bool AdjustClientArea = false);
	HMENU GetMenuHandle(void);
	void RemoveMenu();
	static CMenu GetMenu(HMENU hMenu);
	void EnableItem(int iItemNumber,bool bEnable);
	void EnableItem(CMenu & cMenu,bool bEnable);
};
}; // namespace Sage

#endif // _CMenu_H_
