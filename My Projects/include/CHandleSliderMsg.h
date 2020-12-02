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