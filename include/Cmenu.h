//#pragma once

#if !defined(_CMenu_H_)
#define _CMenu_H_

#include "CWindow.h"

namespace Sage
{

class CMenu
{
private:
	CWindow * m_cWin = nullptr;
	CPasWindow * m_cWinCore = nullptr;
	bool m_bPopup = false;
	int m_iControl = 0;
public:
	CMenu(CWindow * cWin,int iControl);

	HMENU hMenu;
	int AddMenuItem(const char * sItem,int iItemID,const char * sControl = nullptr);
	int AddMenuItem(const char * sItem,CMenu * cMenu,const char * sControl=NULL);
	int AddMenuItem(const char * sItem,CMenu & cMenu,const char * sControl=NULL);
	int AddSeparator();
	int ActivateMenu(void);
	HMENU GetMenuHandle(void);
	void RemoveMenu();
	static CMenu GetMenu(HMENU hMenu);
	void EnableItem(int iItemNumber,bool bEnable);
	void EnableItem(CMenu & cMenu,bool bEnable);


};
}; // namespace Sage

#endif // _CMenu_H_
