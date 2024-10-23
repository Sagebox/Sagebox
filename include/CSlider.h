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
#include "CSliderHandler.h"
#include "CSageTypes.h"
#include "CPoint.h"
#ifdef kCPP17Functions
#include <optional>
#endif
namespace Sage
{

class CPasWindow;
struct CEControlAction_t;
struct CESlider_t;

using Slider = CSlider;

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
    bool m_bRollover = false;     // When true, the slider can rollover when setting positions below or greater than the minimum
 	void Init(int iControl,CWindow * cDavWindow,CPasWindow * cWinCore);
                               // and maximum values, respectively.  Otherwise, the slider is set to minimum or maximum when values exceed each range (respectively).
public:
	struct WinEvent
	{
		friend CSlider;
	private:
		CSlider * m_cSlider;
	public:

#ifdef kCPP17Functions

        /// <summary>
        /// If the slider has been moved (i.e. a move event), a std::optional&lt;int&gt; is returned with the
        /// position of the slider. 
        /// <para></para>
        /// --> If the slider has not been moved, a std::nullopt is returned.
        /// </summary>
        /// <param name="peek"> -- Set to Peek:Yes to avoid resetting the event (subsequent calls return std::nullopt after first read for each event)</param>
        std::optional<int> opMoved(Peek peek = Peek::No);
#endif
		/// <summary>
		/// If a 'move event' is active, Moved() returns true, otherwise it returns false.
        /// <para></para>
        /// Once a true value is returned Moved() returns false until the next move event, unless bPeek is set to true.
		/// </summary>
		/// <param name="iPosition"> - In one Moved() function form, this is a reference to the current position to combine Moved() and GetPos() to also return the current position.</param>
		/// <param name="bPeek"> - Set to true to prevent resetting of the event flag</param>
		/// <returns>true if move event occurred; false if not or until the next move event.</returns>
		bool Moved(bool bPeek = false);

		/// <summary>
		/// If a 'move event' is active, Moved() returns true, otherwise it returns false.
        /// <para></para>
        /// Once a true value is returned Moved() returns false until the next move event, unless bPeek is set to true.
		/// </summary>
		/// <param name="iPosition"> - In one Moved() function form, this is a reference to the current position to combine Moved() and GetPos() to also return the current position.</param>
		/// <param name="bPeek"> - Set to true to prevent resetting of the event flag</param>
		/// <returns>true if move event occurred; false if not or until the next move event.</returns>
		bool Moved(int & iPosition,bool bPeek = false);

        /// <summary>
        /// Returns true if the handle was double clicked.
        /// <para></para>
        /// This is an EVENT value, which means the value is only true once (and will return FALSE henceforward) until the next doube click.
        /// </summary>
        /// <param name="peek"> - Set to Peek:Yes to avoid the event flag reset (i.e. will contiue to return TRUE without resetting the event status)</param>
        /// <returns>true of the handle was double clicked; false if not or until the next 'HandleDoubleClicked' event</returns>
        bool HandleDoubleClicked(Sage::Peek peek = Sage::Peek::No); 

		/// <summary>
		/// Returns true if an 'EndScroll' occurred -- an 'endscroll' event occurs when the mouse is released from the handle.
        /// <para></para>
        /// This function is useful when you want to process differently (or only) when the handle is released, to avoid reacting to every
        /// movement.  This allows only reacting to when the user has stopped the movement.
        /// <para></para>
        /// --> For example, when moving the slider a preview can be generated, with the full processing occuring when the user has released the slider.
        /// <para></para>
        /// --> This is an event, and is reset when EndScroll() is called, which will always return false until the next event/
		/// </summary>
		/// <param name="bPeek"> - Set to true to prevent resetting of the event flag.</param>
		/// <returns>true if an 'EndScroll' event occurred.  Otherwise returns false, or until the next EndScroll event.</returns>
		bool EndScroll(bool bPeek = false);

		/// <summary>
		/// Returns the current position of the slider.
		/// </summary>
		/// <returns>Current positon of the slider.</returns>
		int GetPos(void);
		
	};

	WinEvent event;     // TBD
    CSlider();
	virtual ~CSlider();

	/// <summary>
	/// Deletes the slider.  This causes the slider to become inactive and is removed from the window.  It is not usable after using Delete()
    /// <para></para>
    /// --> Note: The slider's resources are not deleted until the window is closed.  
    /// <para></para>
    /// --> When deleting and creating sliders, it is creating more resources while the window is active. 
    /// <para></para>
    /// --> Rather than deleting and creating the same slider, use various functions to change the slider's name, values, range, etc.,
    /// to avoid causing resource and memory issues.
	/// </summary>
	/// <returns></returns>
	bool Delete();

//	void SetControlID(int iControl);
    //void Close(void) { gcPasWindow->Slider_Close(m_iControl); }

	/// <summary>
	/// Returns the current position of the slider.
    /// <para></para>
    /// Also see GetPosf() and GetPosff() for floating-point versions.
	/// </summary>
	/// <returns>Current positon of the slider.</returns>
    int GetPos(void);

	/// <summary>
	/// Returns the current position of the slider as a double.
    /// <para></para>
    /// This is useful for sliders with floating-point ranges, or just to return the normal integer result as a double.
	/// </summary>
	/// <returns>Current positon of the slider as a double.</returns>
    double GetPosf(void);

	/// <summary>
	/// Returns the current position of the slider as a float.
    /// <para></para>
    /// This is useful for sliders with floating-point ranges, or just to return the normal integer result as a float.
	/// </summary>
	/// <returns>Current positon of the slider as a float.</returns>
    float GetPosff(void);

	/// <summary>
	/// Sets the position of the slider.
    /// <para></para>
    /// If the positional value is too large or too small, the slider is set to the minimum or maximum range value, respectively
    /// -- unless Rollover is in (see AllowRollover()), in which case the values will rollover in the slider.
    /// <para></para>
    /// See SetPosf() to set float and double values for floating-point sliders.
	/// </summary>
	/// <param name="iPos">Positional value to set the slider.</param>
	/// <returns></returns>
	bool SetPos(int iPos);

	/// <summary>
	/// Sets the position of the slider.
    /// <para></para>
    /// This function can be used to set floating-point (double or float) values to a slider that has a floating-point range.
    /// <para></para>
    /// If the positional value is too large or too small, the slider is set to the minimum or maximum range value, respectively
    /// -- unless Rollover is in (see AllowRollover()), in which case the values will rollover in the slider.
	/// </summary>
	/// <param name="fPos"> - double or float Positional value to set the slider.</param>
	/// <returns></returns>
	bool SetPosf(float fPos);

	/// <summary>
	/// Sets the position of the slider.
    /// <para></para>
    /// This function can be used to set floating-point (double or float) values to a slider that has a floating-point range.
    /// <para></para>
    /// If the positional value is too large or too small, the slider is set to the minimum or maximum range value, respectively
    /// -- unless Rollover is in (see AllowRollover()), in which case the values will rollover in the slider.
	/// </summary>
	/// <param name="fPos"> - double or float Positional value to set the slider.</param>
	/// <returns></returns>
    bool SetPosf(double fPos);

    /// <summary>
    /// Adjust the position of the slider by the iPosAdjust value given.  
    /// <para></para>
    /// For example, AdjustPos(5) will move move the slider from its current position + 5
    /// <para></para>
    /// Similarly, AdjustPos(-5) will move move the slider from its current position 5 positions back. 
    /// <para></para>
    /// --> If the new value is below the minimum or above the maximum range values, the value will be stop at the minimum and maximum value respectively 
    /// -- unless Rollover is on (see AllowRollover()), in which case, the values will rollover in the slider.
    /// <para></para>
    /// --> See AdjustPosf() for usage with floating-point sliders.
    /// </summary>
    /// <param name="iPosAdjust"> - Amount to adjust the slider position</param>
    /// <returns></returns>
    bool AdjustPos(int iPosAdjust); 

    /// <summary>
    /// Adjust the position of the slider by the iPosAdjust value given.  This function can be used with sliders that have floating-point ranges.
    /// <para></para>
    /// For example, AdjustPos(5.0) will move move the slider from its current position + 5
    /// <para></para>
    /// Similarly, AdjustPos(-5.0) will move move the slider from its current position 5 positions back. 
    /// <para></para>
    /// --> If the new value is below the minimum or above the maximum range values, the value will be stop at the minimum and maximum value respectively 
    /// -- unless Rollover is on (see AllowRollover()), in which case, the values will rollover in the slider.
    /// <para></para>
    /// --> See AdjustPosf() for usage with floating-point sliders.
    /// </summary>
    /// <param name="fPosAdjust"> - float or double value to adjust the slider position</param>
    /// <returns></returns>
    bool AdjustPosf(double iPosAdjust); 

    /// <summary>
    /// Adjust the position of the slider by the iPosAdjust value given.  This function can be used with sliders that have floating-point ranges.
    /// <para></para>
    /// For example, AdjustPos(5.0f) will move move the slider from its current position + 5
    /// <para></para>
    /// Similarly, AdjustPos(-5.0f) will move move the slider from its current position 5 positions back. 
    /// <para></para>
    /// --> If the new value is below the minimum or above the maximum range values, the value will be stop at the minimum and maximum value respectively 
    /// -- unless Rollover is on (see AllowRollover()), in which case, the values will rollover in the slider.
    /// <para></para>
    /// --> See AdjustPosf() for usage with floating-point sliders.
    /// </summary>
    /// <param name="fPosAdjust"> - float or double value to adjust the slider position</param>
    /// <returns></returns>
    bool AdjustPosf(float iPosAdjust); 

    /// <summary>
    /// Sets the minimum and maximum range of the slider.
    /// <para></para>
    /// The range can be reversed to set a higher minimum and lower maximum. 
    /// <para></para>
    /// --> See SetRangef() for floating-point versions that can be used with sliders that have a floating-point range.
    /// </summary>
    /// <param name="iMin"> - Minimum value of slider</param>
    /// <param name="iMax"> - Maximum value of slider</param>
    /// <returns></returns>
    bool SetRange(int iMin,int iMax);


    /// <summary>
    /// Sets the minimum and maximum range of the slider.  This can be used to set the range of sliders with 
    /// floating-point ranges.
    /// <para></para>
    /// The range can be reversed to set a higher minimum and lower maximum. 
    /// </summary>
    /// <param name="fMin"> - float or double Minimum value of slider</param>
    /// <param name="fMax"> - float or double Maximum value of slider</param>
    /// <returns></returns>
    bool SetRangef(double fMin,double fMax);
    /// <summary>
    /// Sets the minimum and maximum range of the slider.  This can be used to set the range of sliders with 
    /// floating-point ranges.
    /// <para></para>
    /// The range can be reversed to set a higher minimum and lower maximum. 
    /// </summary>
    /// <param name="fMin"> - float or double Minimum value of slider</param>
    /// <param name="fMax"> - float or double Maximum value of slider</param>
    /// <returns></returns>
    bool SetRangef(float fMin,float fMax);

    /// <summary>
    /// Returns the current range of the slider.
    /// <para></para>
    /// --> See GetRangef() for floating-point version.
    /// </summary>
    /// <returns>SIZE structure with minimum and maximum range.</returns>
    SIZE GetRange(); 

    /// <summary>
    /// Returns the current range of the slider with a floating-point range.
    /// </summary>
    /// <returns>CfPoint structure with double value for minimum and maximum range.</returns>
    CfPoint GetRangef(); 

#ifdef kCPP17Functions
	/// <summary>
	/// If a 'move event' is active, Moved() returns a std::optional&gt;int&gt; with the current slider position.
    /// <para></para>Otherwse std::nullopt is returned.
    /// Once a valid std::optional&lt;int&gt; value is returned Moved() returns std::nullopt until the next move event, unless
    /// Peek is set to Peek::Yes.
    /// <para></para>
    /// --> See opMovedf() and opMovedff() for floating-point versions.
	/// </summary>
	/// <param name="Peek"> - Set to Peek::Yes to prevent resetting of the event flag</param>
	/// <returns>true if move event occurred; false if not or until the next move event.</returns>
    std::optional<int>    opMoved(Peek peek = Peek::No);
 
	/// <summary>
	/// If a 'move event' is active, Moved() returns a std::optional&gt;double&gt; with the current slider position.
    /// <para></para>Otherwse std::nullopt is returned.
    /// <para></para>
    /// --> This function can be used for sliders with floating-point ranges.
    /// <para></para>
    /// Once a valid std::optional&lt;double&gt; value is returned Moved() returns std::nullopt until the next move event, unless
    /// Peek is set to Peek::Yes.
	/// </summary>
	/// <param name="Peek"> - Set to Peek::Yes to prevent resetting of the event flag</param>
	/// <returns>true if move event occurred; false if not or until the next move event.</returns>
    std::optional<double> opMovedf(Peek peek = Peek::No);

	/// <summary>
	/// If a 'move event' is active, Moved() returns a std::optional&gt;float&gt; with the current slider position.
    /// <para></para>Otherwse std::nullopt is returned.
    /// <para></para>
    /// --> This function can be used for sliders with floating-point ranges.
    /// <para></para>
    /// Once a valid std::optional&lt;float&gt; value is returned Moved() returns std::nullopt until the next move event, unless
    /// Peek is set to Peek::Yes.
	/// </summary>
	/// <param name="Peek"> - Set to Peek::Yes to prevent resetting of the event flag</param>
	/// <returns>true if move event occurred; false if not or until the next move event.</returns>
    std::optional<float>  opMovedff(Peek peek = Peek::No);

#endif
	/// <summary>
	/// If a 'move event' is active, Moved() returns true, otherwise it returns false.
    /// <para></para>
    /// Once a true value is returned Moved() returns false until the next move event, unless bPeek is set to true.
    /// <para></para>
    /// --> See Movedf() and Movedff() for floating-point versions.
	/// </summary>
	/// <param name="iPosition"> - In one Moved() function form, this is a reference to the current position to combine Moved() and GetPos() to also return the current position.</param>
	/// <param name="bPeek"> - Set to true to prevent resetting of the event flag</param>
	/// <returns>true if move event occurred; false if not or until the next move event.</returns>
	bool Moved(bool bPeek = false);

	/// <summary>
	/// If a 'move event' is active, Moved() returns true, otherwise it returns false.
    /// <para></para>
    /// Once a true value is returned Moved() returns false until the next move event, unless bPeek is set to true.
    /// <para></para>
    /// --> See Movedf() and Movedff() for floating-point versions.
	/// </summary>
	/// <param name="iPosition"> - In one Moved() function form, this is a reference to the current position to combine Moved() and GetPos() to also return the current position.</param>
	/// <param name="bPeek"> - Set to true to prevent resetting of the event flag</param>
	/// <returns>true if move event occurred; false if not or until the next move event.</returns>
	bool Moved(int & iPosition,bool bPeek = false);

	/// <summary>
	/// If a 'move event' is active, Moved() returns true, otherwise it returns false.
    /// This function can be used to return a double value for sliders with floating-point ranges.
    /// <para></para>
    /// Once a true value is returned Moved() returns false until the next move event, unless bPeek is set to true.
    /// <para></para>
    /// --> See Movedf() and Movedff() for floating-point versions.
	/// </summary>
	/// <param name="fPosition"> - In one Moved() function form, this is a reference to the current position to combine Moved() and GetPos() to also return the current position.</param>
	/// <param name="bPeek"> - Set to true to prevent resetting of the event flag</param>
	/// <returns>true if move event occurred; false if not or until the next move event.</returns>
	bool Movedf(double & fPosition,bool bPeek = false);

	/// <summary>
	/// If a 'move event' is active, Moved() returns true, otherwise it returns false.
    /// This function can be used to return a float value for sliders with floating-point ranges.
    /// <para></para>
    /// Once a true value is returned Moved() returns false until the next move event, unless bPeek is set to true.
    /// <para></para>
    /// --> See Movedf() and Movedff() for floating-point versions.
	/// </summary>
	/// <param name="fPosition"> - In one Moved() function form, this is a reference to the current position to combine Moved() and GetPos() to also return the current position.</param>
	/// <param name="bPeek"> - Set to true to prevent resetting of the event flag</param>
	/// <returns>true if move event occurred; false if not or until the next move event.</returns>
    bool Movedff(float & fPosition,bool bPeek = false);

    /// <summary>
    /// Returns true if the handle was double clicked.
    /// <para></para>
    /// This is an EVENT value, which means the value is only true once (and will return FALSE henceforward) until the next doube click.
    /// </summary>
    /// <param name="peek"> - Set to Peek:Yes to avoid the event flag reset (i.e. will contiue to return TRUE without resetting the event status)</param>
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

	/// <summary>
	/// Sets the text of the slider label.  This can be changed at any time. 
	/// </summary>
	/// <param name="sText"> - New Text for Slider Label</param>
	/// <returns></returns>
	bool SetLabelText(const char * sText);

	/// <summary>
	/// Sets the color of the slider label's text.  This can be changed at any time.
	/// </summary>
	/// <param name="sColor"> - Text name of the new color (i.e. "red") -- can also use symbolic colors like SageColor::Red or PanColor::Red</param>
	/// <returns></returns>
	bool SetLabelColor(const char * sColor);

	/// <summary>
	/// Sets the color of the slider label's text.  This can be changed at any time.
	/// </summary>
	/// <param name="rgbLabelColor"> - Symbolic color name, such as SageColor::Red or PanColor::Red.  Text name can also be used, such as "Red"</param>
	/// <returns></returns>
    bool SetLabelColor(Sage::RGBColor_t rgbLabelColor);

	/// <summary>
	/// Sets the font of the slider's text.  This can be changed at any time, but is usually used when the slider is
    /// first created.
    /// <para></para>
    /// --> opt::Font() can be used in the slider instantiaton to set a specific font in the initial creation call.
	/// </summary>
	/// <param name="sFont"> - Font name of the label font, i.e. "Arial,15", etc.</param>
	/// <returns></returns>
	bool SetTextFont(const char * sFont);

	/// <summary>
	/// Sets the color of the slider's value text. This can be set at any time.
	/// </summary>
	/// <param name="rgbValueColor"> - Symbolic color name, such as SageColor::Red or PanColor::Red.  Text name can also be used, such as "Red"</param>
	/// <returns></returns>
	bool SetValueColor(Sage::RGBColor_t rgbValueColor);

	/// <summary>
	/// Sets the color of the slider's value text. This can be set at any time.
	/// </summary>
	/// <param name="rgbValueColor"> - Text name of the new color (i.e. "red") -- can also use symbolic colors like SageColor::Red or PanColor::Red</param>
	/// <returns></returns>
    bool SetValueColor(const char * sColor);

	/// <summary>
	/// When bShow=false, the slider value is not shown.  This can be used for sliders that are more used for context or 
    /// basic positional references without showing specific values.
    /// <param><param>
    /// --> When bShow=true, the value is shown.
	/// </summary>
	/// <param name="bShow"> - False = don't show slider value.  true = show slider value</param>
	/// <returns></returns>
	bool ShowValue(bool bShow);

	/// <summary>
	/// Enables the slider for usage (when bEnable=true).  This can be used to re-enable a slider after it has been disabled
    /// (i.e. grayed out and locked out for usage).
	/// </summary>
	/// <param name="bEnable"> - When true (default), the slider is enabled.  When false, the slider is disabled.</param>
	/// <returns></returns>
	bool Enable(bool bEnable = true);

	/// <summary>
	/// Disables the slider for usage.  The slider is grayed-out to show its disabled status.
    /// <para></para>
    /// This can be useful to signify when a slider cannot be used during various program periods and states.
    /// <para></para>
    /// --> Use Enable() to renable the slider, or use Disable(false)
	/// </summary>
	/// <param name="bDisable"> - When true (default), the slider is disabled.  When false, the slider is enabled.</param>
	/// <returns></returns>
	bool Disable(bool bDisable = true);

	/// <summary>
	/// Shows the slider if it is hidden (i.e. make is reapper in the window or on the screen).
	/// </summary>
	/// <param name="bShow"> - When true (default), the Slider is shown (made visible).  When false, the slider is hidden.</param>
	/// <returns></returns>
	bool Show(bool bShow = true);

	/// <summary>
	/// Hides the slider from view in the window in which is is placed.
	/// </summary>
	/// <param name="bHide"> - When true (default), the slider is hidden from view.  When true, the slider is shown (made visible).</param>
	/// <returns></returns>
	bool Hide(bool bHide = true);

	/// <summary>
	/// This function may be deprecated and currently does not function. 
    /// <para></para>
    /// --> This is probably a non-used legacy function
	/// </summary>
	/// <param name="rgbColor"></param>
	/// <returns></returns>
	bool SetBgColor(Sage::RGBColor_t rgbColor);

	/// <summary>
	/// This function may be deprecated and currently does not function. 
    /// <para></para>
    /// --> This is probably a non-used legacy function
	/// </summary>
	/// <param name="rgbColor"></param>
	/// <returns></returns>
    bool SetBgColor(DWORD dwColor);
    
    /// <summary>
    /// This function is TBD -- it's generally used for allowing TABBING through multiple controls.
    /// </summary>
    /// <param name="bEnable"> - TBD</param>
    /// <returns></returns>
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

	/// <summary>
	/// Returns the Windows (HWND) window handle of the slider
	/// </summary>
	/// <returns>HWND Windows Handle of the slider</returns>
	HWND GetWindowHandle();

	/// <summary>
	/// Causes the slider information to be recalculated, then redraws the slider.
    /// <para></para>
    /// --> This can be used when Windows operations outside of Sagebox have been used to change the slider (see GetWindowHandle()).
	/// </summary>
	/// <returns></returns>
	bool	RecalculateWindow();

	/// <summary>
	/// Updates the background of the slider when it is in a transparency mode (default)
    /// <para></para>
    /// --> This is used when the contents of the parent window have been changed, allowing the slider background
    ///     to absorb and reflect any changes in the window beneath the slider.
	/// </summary>
	/// <returns></returns>
	bool	UpdateBg();

	/// <summary>
	/// Redraws the slider.  This can be used if there is an issue with slider redrawing on its own, such as 
    /// using specific Windows OS functions, etc. (see GetWindowHandle()).
	/// </summary>
	/// <returns></returns>
	bool	Redraw();

	/// <summary>
	/// This clears all event flags, such as Moved() to false.
	/// </summary>
	/// <returns></returns>
	bool	ClearFlags();

	/// <summary>
	/// Sets the location of the Slider in the window.
	/// </summary>
	/// <param name="iX"> - New X location of Slider</param>
	/// <param name="iY"> - New Y location of Slider</param>
	/// <returns></returns>
	bool	SetLocation(int iX,int iY);

	/// <summary>
	/// Returns the X,Y location of the slider relative to the window in which is it placed.
	/// </summary>
	/// <returns>POINT value with X,Y coordinates of slider, relative to window in which is is placed.</returns>
	POINT GetLocation();

	/// <summary>
	/// Returns the size of the slider window.
	/// </summary>
	/// <returns>SIZE structure with width and height of slider window.</returns>
	SIZE GetWindowSize();

	/// <summary>
	/// Returns X positon of Slider window relative to its location in the window in which it is placed.
	/// </summary>
	/// <returns>X position of Slider window (relative to parent window)</returns>
	int StartX();

	/// <summary>
	/// Returns Y positon of Slider window relative to its location in the window in which it is placed.
	/// </summary>
	/// <returns>Y position of Slider window (relative to parent window)</returns>
    int StartY();

	/// <summary>
	/// Returns ending X positon of Slider window relative to its location in the window in which it is placed.
    /// <para></para>--> Note: the ENDX position is the next position past the window, not the actual X positon of the last slider window pixel.
	/// </summary>
	/// <returns>Ending X position of Slider window (relative to parent window)</returns>
    int EndX();

	/// <summary>
	/// Returns ending Y positon of Slider window relative to its location in the window in which it is placed.
    /// <para></para>--> Note: the ENDY position is the next position past the window, not the actual Y positon of the last slider window pixel.
	/// </summary>
	/// <returns>Ending Y position of Slider window (relative to parent window)</returns>
    int EndY();

    // Returns the width of the slider Window
	int getWidth();

    // Returns the Height of the slider window.
	int getHeight();

    // Returns true of the Slider is visible in the window -- returns false if the Slider is hidden
	bool isVisible();

    // Returns true of the slider is valid, useful slider.  Returns false of the slider is invalid.
    // --> This function is somewhat deprecated.  See isEmpty() for a more comprehensive function.
    // --> isValid() determines that a slider is not corrupt or a null pointer. 
    // isEmpty(), in addition to determing if a slider is valid, also checks is a slider is invalid, but also a 'valid empty slider', which is a null slider used to return
    // a valid slider when no valid slider is found in a condition where a valid slider must be returned.
    // --> For functions returning a Slider reference, always use isEmpty(), as isValid() will return true on an 'empty' slider, since  they are valid sliders that ignore all input.
	bool isValid();

  
    /// <summary>
    /// Returns true of the slider is an 'empty' slider (i.e. valid, but used as an empty container).
    /// <para></para>
    /// --> This can be used for functions that return a slider reference to check for an error in the 
    /// return value -- i.e. isEmpty() is used to determine valid results where a valid slider must be returned
    /// from some function (i.e. GetSlider([id]))
    /// <para></para>
    /// --> This predates C++ ability to return a std::optional reference, where a reference is required, 
    /// but an error condition must also be represented, which, in this case, is an 'empty' slider.
    /// --> This function is the same as isValid()
    /// </summary>
    /// <returns>true if slider is empty. Also returns true of this is nullptr or isValid() slider is otherwise invalid. - returns false if slider is valid and not empty</returns>
    bool isEmpty();

	/// <summary>
	/// Sets the message to be displayed on top of the slider when the mouse hovers on the slider for 1-2 seconds.
    /// <para></para>
    /// This is used as a 'tooltip' message to explain the purpose and action of the slider to the user.
    /// <para></para>
    /// --> When the mouse hovers of the slider for 1-2 seconds, a pop-up message is displayed.
    /// <para></para>
    /// --> Example: mySlider.SetHoverMsg("Use this slider to set the range of the output device.");
	/// </summary>
	/// <param name="sMessage"> - The message to display when the mouse hovers over the slider for 1-2 seconds.</param>
	/// <returns></returns>
	bool SetHoverMsg(const char * sMessage);

    // Returns the ID of the slider.  This is usually set when the slider is created with opt::ID().  When the 
    // ID is not specified in the slider creation, -1 is returned
    // The ID can be used for purposes in other functions.
	int GetID();

    // Returns the text name of a slider of opt::Name() we used in the creation of the slider.
    // This can be useful when referring to the slider in various functions where the name can be used in 
    // a way where it isn't possible to send the slider reference.
    // --> If opt::Name() was not used, an empty string (i.e. "") is returned.
	const char * GetName();

    // TBD -- Currently for internal use.
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
    /// <param name="sPreText"> - Text to add before the value is printed (nullptr for no text)</param>
    /// <param name="sPostText"> - Text to add after the value is printed (nullptr for no text)</param>
    /// <param name="fAdd"> - Value to add to the value after it is multiplied by fMult</param>
    /// <param name="fMult"> - Value by which to multiply the slider position</param>
    /// <param name="bAsFloatingPoint"> - TRUE sets the value as a float-point value.  FALSE (default) displays it as integer</param>
    /// <returns></returns>
    bool SetValueDisplay(const char * sPreText, const char * sPostText); 

    /// <summary>
    /// Sets the display output based on the position of the Slider.  This allows you to automatically display a wide range of options to display the
    /// value as the basic slider positon or through some other formula.
    /// <para></para>
    /// Example: SetValueDisplay(0,1/100,true);  Display 0-1.0 for a generic slider, rather than 0-100
    /// </summary>
    /// <param name="sPreText"> - Text to add before the value is printed (nullptr for no text)</param>
    /// <param name="sPostText"> - Text to add after the value is printed (nullptr for no text)</param>
    /// <param name="fAdd"> - Value to add to the value after it is multiplied by fMult</param>
    /// <param name="fMult"> - Value by which to multiply the slider position</param>
    /// <param name="bAsFloatingPoint"> - TRUE sets the value as a float-point value.  FALSE (default) displays it as integer</param>
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

    /// <summary>
    /// Allows the slider to rollover from max-to-min and min-to-max value when being set programmatically.
    /// <para></para>
    /// By default, when a slider value exceeds the min/max range, the values are set to the minimum and maximum range value, respectively.
    /// <para></para>
    /// When Rollover is on, this will roll the slider value over when values below the minimum and above the maximum are set with GetPos(). 
    /// <para></para>
    /// --> This does not affect the user setting the slider with the mouse; this is not affected and will still stop and the minimum and maximum values when
    /// <para></para>
    /// --> Note: Rollover currently work only with sliders with Integer ranges (i.e. non-floating-point sliders), with a minimum value that is less than the maximum
    /// range value (i.e. range of (50,-50) works, but reversed range of (50,-50) will currently not rollover.
    /// </summary>
    /// <param name="bRollover"> - When true, this allows rollovers to occur when SetPos() is called.  When false, this disables Rollover ability (this is the default slider setting)</param>
    /// <returns></returns>
    bool AllowRollover(bool bRollover = true); 

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
