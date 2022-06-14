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

#include <windows.h>
#include "Sage.h"
#include "SageOpt.h"
#include "CSliderHandler.h"
#include "CSageTypes.h"
#ifdef kCPP17Functions
#include <optional>
#endif
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

#ifdef kCPP17Functions
        std::optional<int> Moved(Peek peek = Peek::No);
#else
		bool Moved(bool bPeek = false);
#endif
		bool Moved(int & iPosition,bool bPeek = false);

        /// <summary>
        /// Returns true if the handle was double clicked.
        /// <para></para>
        /// This is an EVENT value, which means the value is only true once (and will return FALSE henceforward) until the next doube click.
        /// </summary>
        /// <param name="peek">Set to Peek:Yes to avoid the event flag reset (i.e. will contiue to return TRUE without resetting the event status)</param>
        /// <returns></returns>
        bool HandleDoubleClicked(Sage::Peek peek = Sage::Peek::No); 

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
    double GetPosf(void);
    float GetPosff(void);
	bool SetPos(int iPos);
	bool SetPosf(float fPos);
	bool SetPosf(double fPos);

    bool SetRange(int iMin,int iMax);
    bool SetRangef(double fMin,double fMax);
    bool SetRangef(float fMin,float fMax);

#ifdef kCPP17Functions
    std::optional<int> Moved(Peek peek = Peek::No);
    std::optional<double> Movedf(Peek peek = Peek::No);
    std::optional<float> Movedff(Peek peek = Peek::No);

#else
	bool Moved(bool bPeek = false);
#endif
	bool Moved(int & iPosition,bool bPeek = false);
	bool Movedf(double & fPosition,bool bPeek = false);
	bool Movedff(float & fPosition,bool bPeek = false);

    /// <summary>
    /// Returns true if the handle was double clicked.
    /// <para></para>
    /// This is an EVENT value, which means the value is only true once (and will return FALSE henceforward) until the next doube click.
    /// </summary>
    /// <param name="peek">Set to Peek:Yes to avoid the event flag reset (i.e. will contiue to return TRUE without resetting the event status)</param>
    /// <returns></returns>
    bool HandleDoubleClicked(Sage::Peek peek = Sage::Peek::No); 

    /// <summary>
    /// Cause Moved() event status to come back true (just once) to force or fake a slider movement.
    /// <para></para>
    /// This is useful when wanting to enter a slider event code section based on the a TRUE return for the
    /// move event without changing the slider value.
    /// </summary>
    /// <returns></returns>
    bool ForceMove(); 

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

	bool SetSignal(bool * bSignal,int * iSignalPos = nullptr);
	bool SetSignal(SliderSignal & stSignal);

    /// <summary>
    /// Sets the display output based on the position of the Slider.  This allows you to automatically display a wide range of options to display the
    /// value as the basic slider positon or through some other formula.
    /// <para></para>
    /// Example: SetValueDisplay(0,1/100,true);  Display 0-1.0 for a generic slider, rather than 0-100
    /// </summary>
    /// <param name="sPreText">Text to add before the value is printed (nullptr for no text)</param>
    /// <param name="sPostText">Text to add after the value is printed (nullptr for no text)</param>
    /// <param name="fAdd">Value to add to the value after it is multiplied by fMult</param>
    /// <param name="fMult">Value by which to multiply the slider position</param>
    /// <param name="bAsFloatingPoint">TRUE sets the value as a float-point value.  FALSE (default) displays it as integer</param>
    /// <returns></returns>
    bool SetValueDisplay(const char * sPreText, const char * sPostText); 

    /// <summary>
    /// Sets the display output based on the position of the Slider.  This allows you to automatically display a wide range of options to display the
    /// value as the basic slider positon or through some other formula.
    /// <para></para>
    /// Example: SetValueDisplay(0,1/100,true);  Display 0-1.0 for a generic slider, rather than 0-100
    /// </summary>
    /// <param name="sPreText">Text to add before the value is printed (nullptr for no text)</param>
    /// <param name="sPostText">Text to add after the value is printed (nullptr for no text)</param>
    /// <param name="fAdd">Value to add to the value after it is multiplied by fMult</param>
    /// <param name="fMult">Value by which to multiply the slider position</param>
    /// <param name="bAsFloatingPoint">TRUE sets the value as a float-point value.  FALSE (default) displays it as integer</param>
    /// <returns></returns>
    bool SetValueDisplay(const char * sPreText, const char * sPostText,double fMinValue,double fMaxValue,bool bAsFloatingPoint = false); 
 
    /// <summary>
    /// Sets the display output based on the position of the Slider.  This allows you to automatically display a wide range of options to display the
    /// value as the basic slider positon or through some other formula.
    /// <para></para>
    /// Example: SetValueDisplay(0,1/100,true);  Display 0-1.0 for a generic slider, rather than 0-100
    /// </summary>
    /// <param name="sPreText">- Text to add before the value is printed (nullptr for no text)</param>
    /// <param name="sPostText">- Text to add after the value is printed (nullptr for no text)</param>
    /// <param name="fAdd">- Value to add to the value after it is multiplied by fMult</param>
    /// <param name="fMult">- Value by which to multiply the slider position</param>
    /// <param name="bAsFloatingPoint">- TRUE sets the value as a float-point value.  FALSE (default) displays it as integer</param>
    /// <returns></returns>
     bool SetValueDisplay(double fMinValue,double fMaxValue,bool bAsFloatingPoint = false); 

    /// <summary>
    /// Returns the value calculated for the display if SetValueDisplay() was used.  This allows an easy way to obtain the displayed Slider Value as an integer
    /// value without having to relculate it yourself.
    /// <para></para>
    /// --> Use GetPos() to get the slider position between 0 and the Range set initially.
    /// <para></para>
    /// --> If SetValueDisplay() has not been called, the slider position is returned (which corresponds to the displayed value)
    /// </summary>
    /// <returns>int value of displayed slider value</returns>
    int GetValueDisplay();

    /// <summary>
    /// Returns the value calculated for the display if SetValueDisplay() was used.  This allows an easy way to obtain the displayed Slider Value as float-point
    /// value without having to relculate it yourself.
    /// --> Use GetPos() to get the slider position between 0 and the Range set initially.
    /// <para></para>
    /// --> If SetValueDisplay() has not been called, the slider position is returned (which corresponds to the displayed value)
    /// </summary>
    /// <returns>double value of displayed slider value</returns>
    double GetValueDisplayf();


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
