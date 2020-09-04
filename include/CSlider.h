//File copyright(c) 2020, Rob Nelson.  All rights reserved.   robnelsonxx2@gmail.com
//
//#pragma once

#if !defined(_CDavSlider_H_)
#define _CDavSlider_H_
#include <windows.h>
#include "Sage.h"
#include "SageOpt.h"
#include "CSliderHandler.h"

namespace Sage
{

class CPasWindow;
struct CEControlAction_t;
struct CESlider_t;

class CSlider : CSliderHandler
{
	class CWidget : public Sage::CWidget
	{
		CSlider * m_cObj;
	public:
		CWidget(CSlider * cObj);
		// * GetWindow()				;
		bool SetLocation(int iX,int iY)		;
		bool isValid()						;
		bool Show(bool bShow)				;
		bool Hide(bool Hide)				;
		int GetID()							;
		const char * GetName()				;
	};												


friend CWindow;
kAdvPublic:		// Public or --Adanced define
				// delete() operator is set to private.  The created EditBox is managed by Davinci, so no
				// other process should delete it.   This will cause a compiler error if it is used.
				//
	void operator delete(void * ptr) 
	{ 
		::operator delete(ptr); 
	};
	CSlider(const CSlider &p);						// Copy constructor to force error on copy (since its a managed object)
public:
	static void deleteObj(CSlider * p) 
	{ 
		delete p; 
	}
private:
	CWidget				*		m_cWidget = nullptr;
	CEControlAction_t	*		m_stControl	;		// Internal control structure containing data for button
	CESlider_t			*		m_stSlider	;		// More direct routine to control structure (i.e. actual button)
	CWindow			*		m_cDavWin	;		// Pointer to parent Window
	CPasWindow			*		m_cWinCore	;		// Pointer to core function class

	int m_iCopyCount	;	// Used for debugging & tracking
	int m_iControl		;	// Internal Control ID
	int m_iWinControl	;	// Internal Conrol ID for parent window

	void * stPrivStruct = nullptr;
public:
    CSlider();
	virtual ~CSlider();
	bool Delete();
	void Init(int iControl,CWindow * cDavWindow,CPasWindow * cWinCore);

//	void SetControlID(int iControl);
    //void Close(void) { gcPasWindow->Slider_Close(m_iControl); }
    int GetPos(void);
    bool SetRange(int iMin,int iMax);
	bool Moved(Sage::Peek peek = Sage::Peek::No);
	bool EndScroll(Sage::Peek peek = Sage::Peek::No);

   // void SetPos(int iPos) { gcPasWindow->Slider_SetPos(m_iControl,iPos); }
	//int SetLocation(int iX,int iY) { return gcPasWindow->Slider_SetLocation(m_iControl,iX,iY); }
	//void Show(int bShow = 1) { gcPasWindow->Slider_ShowSlider(m_iControl,bShow); };
	//void Hide(int bHide = 1) { gcPasWindow->Slider_HideSlider(m_iControl,bHide); }
	bool SetLabelText(char * sText);
	bool SetLabelColor(DWORD dwLabelColor);
	bool SetLabelColor(Sage::RGBColor_t rgbLabelColor);
	bool SetTextFont(char * sFont);
	bool SetValueColor(Sage::RGBColor_t rgbValueColor);
	bool SetValueColor(DWORD dwValueColor);
	bool ShowValue(bool bShow);
	bool Enable(bool bEnable);
	bool Disable(bool bDisable);
	bool Show(bool bShow = true);
	bool Hide(bool bHide = true);
	bool SetBgColor(Sage::RGBColor_t rgbColor);
	bool SetBgColor(DWORD dwColor);
	bool EnableFocusBox(bool bEnable = true);

	HWND	GetWindowHandle();
	bool	RecalculateWindow();
	bool	ClearFlags();
	bool	SetLocation(int iX,int iY);
	POINT	GetLocation();
	SIZE	GetWindowSize();
	bool	SetHoverMsg(const char * sHoverMessage);
	int		StartX();
	int		StartY();
	int		EndX();
	int		EndY();
	int		getWidth();
	int		getHeight();
	bool	isVisible();
	bool	isValid();

	int GetID();
	const char * GetName();
	Sage::CWidget * GetWidgetObj();

	//int EndScrollActive() { return gcPasWindow->Slider_EndScrollActive(m_iControl); }
	//void SetWantEndScrollOnly(int bWantEndScroll) { gcPasWindow->Slider_SetWantEndScrollOnly(m_iControl,bWantEndScroll); }

	bool SetMessageHandler(CSliderHandler * cHandler,void * pClassInfo = nullptr);
	bool SetMessageHandler(CSliderHandler & cHandler,void * pClassInfo = nullptr);

};
}; // namespace Sage
#endif _CDavSlider_H_