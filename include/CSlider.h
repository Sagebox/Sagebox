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
friend CSliderSignal;
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
	struct WinEvent
	{
		friend CSlider;
	private:
		CSlider * m_cSlider;
	public:
		bool Moved(bool bPeek = false);
		bool Moved(int & iPosition,bool bPeek = false);
		bool EndScroll(bool bPeek = false);
		int GetPos(void);
		
	};

	WinEvent event;
    CSlider();
	virtual ~CSlider();
	bool Delete();
	void Init(int iControl,CWindow * cDavWindow,CPasWindow * cWinCore);

//	void SetControlID(int iControl);
    //void Close(void) { gcPasWindow->Slider_Close(m_iControl); }
    int GetPos(void);
	bool SetPos(int iPos);

    bool SetRange(int iMin,int iMax);
	bool Moved(bool bPeek = false);
	bool Moved(int & iPosition,bool bPeek = false);
	bool EndScroll(bool bPeek = false);

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

	// SetFineControl() -- When TRUE, the Mouse Wheels will increment the slider value by 1 or -1, allowing finer control
	// When FALSE (default setting), the Mouse Wheel increments the slider by a larger value so that the
	// entire length of the slider and be traversed fairly quickly.
	//
	// --> iIncrement sets the increment for each Mouse Wheel (i.e. SetFineControl(5) will move the slider value or or down by 5.
	// --> The default is one (i.e. SetFineControl() is the same as SetFineControl(true,1) or SetFineControl(1);
	//
	bool SetFineControl(bool bFineControl = true,int iIncrement = 1);

	// SetFineControl() -- When TRUE, the Mouse Wheels will increment the slider value by 1 or -1, allowing finer control
	// When FALSE (default setting), the Mouse Wheel increments the slider by a larger value so that the
	// entire length of the slider and be traversed fairly quickly.
	//
	// --> iIncrement sets the increment for each Mouse Wheel (i.e. SetFineControl(5) will move the slider value or or down by 5.
	// --> The default is one (i.e. SetFineControl() is the same as SetFineControl(true,1) or SetFineControl(1);
	//
	bool SetFineControl(int iIncrement);

	HWND	GetWindowHandle();
	bool	RecalculateWindow();
	bool	UpdateBg();
	bool	Redraw();
	bool	ClearFlags();
	bool	SetLocation(int iX,int iY);
	POINT	GetLocation();
	SIZE	GetWindowSize();
	int		StartX();
	int		StartY();
	int		EndX();
	int		EndY();
	int		getWidth();
	int		getHeight();
	bool	isVisible();
	bool	isValid();
	bool	SetHoverMsg(const char * sMessage);
	int GetID();
	const char * GetName();
	Sage::CWidget * GetWidgetObj();

	//int EndScrollActive() { return gcPasWindow->Slider_EndScrollActive(m_iControl); }
	//void SetWantEndScrollOnly(int bWantEndScroll) { gcPasWindow->Slider_SetWantEndScrollOnly(m_iControl,bWantEndScroll); }

	bool SetMessageHandler(CSliderHandler * cHandler,void * pClassInfo = nullptr);
	bool SetMessageHandler(CSliderHandler & cHandler,void * pClassInfo = nullptr);

	bool SetSignal(bool * bSignal,int * iSignalPos);
	bool SetSignal(SliderSignal & stSignal);

    // signal -- this is used to access Sliider data and events without using the normal CSlider call process.
    //           This makes getting presses and status changes much quicker. 
    //
    // The signal can be accessed through it's class (CSliderSignal) or thorugh the stSignal in the class directly.
    // The addresses of the stSignal data may be passed as references for direct acess in tight loops. 
    //
    // Access is guaranteed to be safe as long as the CSlider object exists. 
    //
    // You can create a new signal that will stay local (and does not need to be dereferenced).  Only
    // one signal is available at a time -- creating a new signal will deactivate the CSlider signal.
    // The signal may be restored with signal.RestoreSignal() or deactivated with signal KillSignal().
    //
    CSliderSignal signal;
};
}; // namespace Sage
#endif _CDavSlider_H_