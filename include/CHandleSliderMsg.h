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

#if !defined(_HandleSliderMsg_H_)
#define _HandleSliderMsg_H_
#include <Windows.h>

namespace Sage
{
class CPasWindow;
class CUserWindow;
struct CEControlAction_t;
struct CESlider_t;
class CHandleSliderMsg
{
public:
	enum class Return
	{
		Value,
		DefProc,
		Continue,
		MessageChanged,
	};
	HWND				  m_hParent;
	HWND				  hWnd;
	WPARAM				  wParam;
	LPARAM				  lParam;
	UINT				  Message;
	CEControlAction_t	& stControl;
	CEControlAction_t	* stParentControl;

	CESlider_t			& stSlider;
	UINT				  m_uiReturn;				// Return value set by WM_Message functions.  TRUE return returns this value, otherwise we return defproc
	CUserWindow			* m_cWin;
	bool				  m_bInParent;
public:
	CPasWindow	* m_cWinCore;
	LRESULT HandleMessage(CEControlAction_t & stControl,HWND hWnd,UINT Message,WPARAM wParam,LPARAM lParam);
	LRESULT HandleNotify(HWND hWnd,WPARAM wParam,LPARAM lParam);

	// WM_Message functions

	Return MouseWheel();
	Return MouseHover();
	Return MouseMove();
	Return Destroy();
	Return SetFocus();
	Return ParentNotify();	// This handled WM_NOTIFY from the parent window. See notes where it is used.

	CHandleSliderMsg(CPasWindow * cWinCore,CEControlAction_t & stControl);
};
}; // namespace Sage
#endif // _HandleEditboxMsg_H_