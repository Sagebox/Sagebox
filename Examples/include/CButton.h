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

#if !defined(_CDavButton_H_)
#define _CDavButton_H_
#include <windows.h>
#include "Sage.h"
#include "SageOpt.h"
#include "CButtonHandler.h"

// ----------------
// CButton Class
// ----------------
//
// This class is returned when a Button, Checkbox, or RadioButton is created and contains most edit box functions.
//
// The object returned is managed by SageWin and shouldn't be deleted.  Use the Delete() funtion
// to remove the button.
//
// You can manage this yourself and use delete() to delete the control, though it is not necessary.  All
// Created controles are deleted automatically when the parent Window closes.  It is safe, however, to 
// use delete on the control -- you may have to set a specific compile directive to cause the delete() operator to
// become public (TBD)
//
// Some Examples:
//
// CSageButton & MyButton = MyWin.CreateButton(10,50,"This a Button")		-- Creates an Button at (10,50) in the the window with the Text "This is a button"
// auto MyButton = MyWin.CreateButton(10,50,"This a Button")				-- Does the same thing, just without specifying the type
// auto pMyButton = MyWin.CreateButton(10,50,"This a Button")				-- Also creates a button at (10,50), but assigns it to a pointer so it is more easily passed between functions.
//																			   This is useful when specifying it in a class. 
// MyWin.CreateButton(10,50,"This is a button",Name("MyButton"))			-- This creates a button without assigning it.  It can be accessed later with the functon MyWin.button("MyButton)",
//																			   which will return the reference to the created button.
// Overloading
//
// You can overload the Button class to use your own functions and trap Window messages (i.e. WM_PAINT, etc.).
// With the overload, you can then give this to the CreateButton() function.  You can then use all of the EditBox
// functions as well as any you care to override, such as message and valiate functions.
//
// Example:
//
// CRobEditBox : public CSageButton
// {
//		Status Validate(const char * sText) override;
// };
//
// Setting a Message Handler
//
// TBD
//
// External classes and structures used internally


namespace Sage
{

class CPasWindow			;
struct CEControlAction_t	;
struct CEButton_t			;
class CButton				;
class CWindow				;

class CButton : public CButtonHandler
{
friend CWindow;

kAdvPublic:		// Public or --Adanced define
				// delete() operator is set to private.  The created EditBox is managed by Davinci, so no
				// other process should delete it.   This will cause a compiler error if it is used.
				//
	void operator delete(void * ptr) 
	{ 
		::operator delete(ptr); 
	};
	CButton(const CButton &p);						// Copy constructor to force error on copy (since its a managed object)

public:
	static void deleteObj(CButton * p) 
	{ 
		delete p; 
	}
private:
	class CWidget : public Sage::CWidget
	{
		CButton * m_cObj;
	public:
		CWidget(CButton * cObj);
		CWindow * GetWindow()			;
		bool SetLocation(int iX,int iY)		;
		bool isValid()						;
		bool Show(bool bShow)				;
		bool Hide(bool Hide)				;
		int GetID()							;
		const char * GetName()				;
	};												

	CWidget * m_cWidget = nullptr;
	void Init(int iControl,CWindow * cDavWindow,CPasWindow * cWinCore);

	CEControlAction_t	*		m_stControl	;		// Internal control structure containing data for button
	CEButton_t			*		m_stButton	;		// More direct routine to control structure (i.e. actual button)
	CWindow				*		m_cDavWin	;		// Pointer to parent Window
	CPasWindow			*		m_cWinCore	;		// Pointer to core function class

	int m_iCopyCount	;	// Used for debugging & tracking
	int m_iControl		;	// Internal Control ID
	int m_iWinControl	;	// Internal Conrol ID for parent window
public:

	struct WinEvent
	{
		friend CButton;
	private:
		CButton * m_cButton;
	public:
		bool Pressed(Peek peek = Peek::No);
		bool Pressed(bool & bChecked,Peek peek = Peek::No);

		// Checked() -- For checkboxes, this returns TRUE if the checkbox is checked, FALSE if it is unchecked.
		// To determine WHEN the checkbox is checked, this is equivalent to a "press" (and "unpress") action -- use Pressed() to determine when the
		// checkbox is pressed ans UnPressed() to determine when the mouse button is unpressed.
		// -->
		//  ** Important note ** 
		// This is not an event and will always return the status (it doesn't get reset on the call).  It is provided here for clarity. 
		// MyButton.Checked() can also be used to get the 'checked' status
		//
		bool Checked();
		bool PeekPress();
		bool DoubleClicked(Peek peek = Peek::No);
		bool Unpressed(Peek peek = Peek::No);
		bool RadioGroupPressed(Peek peek = Peek::No);
		bool RadioGroupPressed(int & iButton,Peek peek = Peek::No);

		// RadioGroupChecked() - Returns the currently selected RadioButton that is selected in the radio button group (since one must always be selected)
		// -->
		//  ** Important note ** 
		// This is not an event and will always return the status (it doesn't get reset on the call).  It is provided here for clarity. 
		// MyButton.RadioGroupChecked() can also be used to get the 'checked' box for the radio button group
		//
		int RadioGroupChecked();
	};

	WinEvent event;
	// Window Rect of Button Window
	//
	RECT WindowRect();

	// Pressed() -- Returns true of the button was pressed, false if it wasn't.
	// This function is a one--time-per press, resetting the "pressed" flag after being called. This means subsequent calls to Presed() will return false until the
	// button is pressed again.
	//
	// Specify Sage::Peek:Yes to "peek" at the value.  This will no reset the value, allowing the call to be made again for the same press.
	//
	bool Pressed(Peek peek = Peek::No);
	bool Pressed(bool & bChecked,Peek peek = Peek::No);

	// PeekPress() -- Returns TRUE of the button was pressed, false if it has not been pressed. This will not reset the flag and will always return TRUE until the flag is reset.
	// This function is a shortcut for Pressed(Sage::Peek:Yes).
	//
	int PeekPress();

	// Was the button double clicked?
	//
	// This function is a one--time-per press, resetting the "pressed" flag after being called. This means subsequent calls to Presed() will return false until the
	// button is pressed again.
	//
	int DoubleClicked(Peek peek = Peek::No);

	bool SetHoldPress(int iRepeatMS = 0,int iHoverMS = 0); 
	bool ReleaseHoldPress(); 
	


	// TBD
	//
	virtual void HandleButton(void);

	// CSageButton constructor
	//
	CButton();

	// CSageButton Destructor
	//
	// Usually, you can use MyButton.Delete() to delete the button.  However, for some advanced applications, you can manage the button yourself
	// and delete the button (you may have to set a compilation flag to make the delete() function public)
	//
	virtual ~CButton();

	// UnPressed() -- When the button is pressed, a "pressed" flag is set.  When the mouse is released, an "UnPressed" flag is set. This functio will return true
	// when the button is "unpressed".
	//
	bool Unpressed(Peek peek = Peek::No);
	
	// ComboBoxPressed() -- This is TBD, as the ComboBoxes are scheduled for the next release.
	//
	bool ComboBoxPressed();

	// SetComboBoxPress() -- This is TBD, as the ComboBoxes are scheduled for the next release.
	//
	bool SetComboBoxPress(int bValue);

	// Checked() -- For checkboxes, this returns TRUE if the checkbox is checked, FALSE if it is unchecked.
	// To determine WHEN the checkbox is checked, this is equivalent to a "press" (and "unpress") action -- use Pressed() to determine when the
	// checkbox is pressed ans UnPressed() to determine when the mouse button is unpressed.
	//
	bool Checked();

	// SetCheck() -- Set the check status of the checkbox.  When TRUE, the checkbox is set to checked. When FALSE, the checkbox is set to UnChecked
	//
	bool SetCheck(bool bCheckStatus);

	// Show() -- Show the button.  If the button is hidden, Show() will cause it to display.  You may also specify Show(false) to hide it from the Window display.
	//
	bool Show(bool bShow = true);

	// Hide() -- Hide the button.  This will cause the button to disappear from the window, but remain active.  You may also specify Hide(false) to show the Button on the Window if it is hidden.
	//
	bool Hide(bool bHide = true);

	// SetFocus() -- Sets the focus for the button.  This sets the Windows keyboard focus for the button.  This allows the button to receive keys, so you can look for keys 
	// related to the button.  For example, if the user pressed "CR" in the button, this is accepted as a button "press".  However, this only works when the focus is on the 
	// button window.
	//
	// This is useful for "Continue"-type buttons, such as:
	//
	// --> auto MyButton = CreateButton(10,"Continue");
	// --> MyButton.SetFocus();
	//
	// This will set the keyboard focus so the user may also enter a carriage return to continue, as well as press the button.
	// Without SetFocus(), the focus remains on the Window and the button has no way to look for keys.
	//
	// $$ This may change in a future update with various functins such as WaitforPress(), which may also field keys within the entire window.
	//
	bool SetFocus();

	// Disable() -- Disable the button.  This will cause the button (or checkbox) to become disabled, accepting no input.  In most cases, this will also gray-out the button to 
	// give a disabled appearance
	//
	bool Disable(bool bDisable = true);
	
	// Enable() -- Enable a button or checkbox that has been disabled.
	//
	bool Enable(bool bEnable = true);

	// RadioGroupPressed() - Acts just like button.Press(), but returns the button press status of any button in a named group.  This allows addressing multiple Radio buttons with one call.
	// RadioGroupPressed() returns with false if no button is pressed. Like Press(), this is a one-time function and will clear the flag, resulting in a FALSE return on subsequent calls until another button
	// in the group is pressed.  Use RadioGroupPressed(Sage::Peek::Yes) to "peek" at the value, which will no reset the flag.
	//
	// Parameters:
	//
	//
	// peek     -- Default is false (i.e. Sage::Peek:No), which resets the flag.  Specifying Sage::Peek::Yes will "peek" at the value and return its status without setting it to false afterwards.
	// iButton  -- When specified, fills pButtonID with the ControlID (set with ID() option) for the button pressed.  This allows a quick if (RadioGroupPressed(iButton)) { ... iValue = button ID } 
	//               to determine the status and value in one statement, where a GroupPressed() and then checking all button values would otherwise be necessary.
	//
	bool RadioGroupPressed(Peek peek = Peek::No);

	// RadioGroupPressed() - Acts just like button.Press(), but returns the button press status of any button in a named group.  This allows addressing multiple Radio buttons with one call.
	// RadioGroupPressed() returns with false if no button is pressed. Like Press(), this is a one-time function and will clear the flag, resulting in a FALSE return on subsequent calls until another button
	// in the group is pressed.  Use RadioGroupPressed(Sage::Peek::Yes) to "peek" at the value, which will no reset the flag.
	//
	// Parameters:
	//
	//
	// peek     -- Default is false (i.e. Sage::Peek:No), which resets the flag.  Specifying Sage::Peek::Yes will "peek" at the value and return its status without setting it to false afterwards.
	// iButton  -- When specified, fills pButtonID with the ControlID (set with ID() option) for the button pressed.  This allows a quick if (RadioGroupPressed(iButton)) { ... iValue = button ID } 
	//               to determine the status and value in one statement, where otherwise a GroupPressed() and then calling iButton = RadioGroupChecked() would be required.
	//
	bool RadioGroupPressed(int & iButto,Peek peek = Peek::No);


	// RadioGroupChecked() - Returns the currently selected RadioButton that is selected in the radio button group (since one must always be selected)
	//
	int RadioGroupChecked();

	// Set the RadioGroup ID of this button (assumed to be a RadioButton).  This is usually set in the creation of the radiobutton (i.e. CreateRadioButon(ix,iy,"Name",RadioGroup(1)), but can be set after the 
	// radiobutton is created. 
	//
	// Note: this is the button's ID within the group, not the ID of the group itself.
	//
	bool SetRadioGroupID(int iValue);

	// WaitforPress() - Wait for the button to be pressed.  This blocks the program until one of two condiions are met:
	//
	// 1. The button is pressed.
	// 2. The Window is closing.  
	//
	// This returns TRUE if the button is pressed.
	// This returns FALSE if the Window is closing (whether or not the button was pressed) or if the button is invalid.
	//
	// Input
	//
	// iWaitMS	-- Number of milliseconds to wait before contining.  For example, WaitforPress(2000) waits for two seconds and returns false if the button was not press in that time.
	//
	// note -- $$ iWaitMS is currently inactive, this is to be added soon.
	//
	bool WaitforPress(int iWaitMS = 0);
	
	// WaitforUnpress() - Wait for the button to be un-pressed (i.e. the mouse button is released after a press).  This blocks the program until one of two condiions are met:
	//
	// 1. The button is "unpressed".
	// 2. The Window is closing.  
	//
	// This returns TRUE if the button is unpressed.
	// This returns FALSE if the Window is closing (whether or not the button was unpressed) or if the button is invalid.
	//
	// Input
	//
	// iWaitMS	-- Number of milliseconds to wait before contining.  For example, WaitforUnpress(2000) waits for two seconds and returns false if the button was not press in that time.
	//
	// note -- $$ iWaitMS is currently inactive, this is to be added soon.
	//
	bool WaitforUnpress(int iWaitMS = 0);


	// SetText() -- Set the text of the button.  This can be changed dynamically, such as changing "Cancel" to "Ok" to reflect a change in status.
	// This may change the size of the button or clip text that is longer than the button.   You can add padding to the button by specifying a negative Width value or specifing Pad(<value>)
	// or "Pad=<value>" when the button is created to ensure a button that fits the widest text and will not change size.
	//
	// You can also specify a specific width that fits all planned text, which will disallow the button from changing size. 
	//
	bool SetText(const char * sText);
	bool SetText(const wchar_t * sText);


	// Sets the size of the button.  This sets the width and/or height of the button.  This causes the values specified to be static, meaning the button size should never change after this point, even when
	// the button text is changed.  If no width/height is initially specified, the button is sized according to the selectef Font and length of the button text.  This leaves the width and height in a 
	// "floating" state, which may cause the button size to change according to any changes to the text.  Setting a static width and height (which can be done by specifying in the call to create the button)
	// will keep the button at the same size even when text is changed.
	//
	bool SetButtonSize(int iWidth,int iHeight=0);
	
	// GetText() -- Get the text on the button.   This will return the text showing on the button.  In the case of a graphic button with no text, either nullptr or "" is returned.
	//
	const char * GetText();

	// Delete() -- Delete the button. 
	//
	// This removes the button from the window and deactivates it as a SageWin control.
	//
	bool Delete();

	// Redraw() -- Redraw the button.  Technically, it sends an Invalidate() call to Windows to force a Windows-level redraw of the button. 
	// 
	bool Redraw();

	// ForcePress() -- Cause the button to appear pressed when Pressed() is called.  This is useful when calling functions that look for this and you want to
	// have it react to the press.  This allows multiple methods to activate an action by routing it through the button press.   
	//
	// This will also call any overload or callback for the ButtonPress() message (i.e. it will send a ButtonPress() message that can be handled by a Button overload
	// or Handler class)
	//
	// For example, if a process is cancelled for an external reason, using CancelButton.ForcePress() can let the Cancel Button handler deal with the cancellation.
	//
	bool ForcePress();

	// SetFgColor() -- Set the button foreground color
	//
	// This is somewhat TBD, but works for windows-style buttons. 
	//
	bool SetFgColor(int iColor);

	// SetBgColor -- Set the button background color.  
	//
	// This is TBD.  It works well for Windows buttons, but is not yet completelty defined for non-Windows (i.e. SageWin default) buttons.
	//
	bool SetBgColor(int iColor);

	bool SetStyle(const char * sStyle);
	bool SetDefaultStyle();
	bool SetMessageHandler(CButtonHandler * cHandler,void * pClassInfo = nullptr);
	bool SetMessageHandler(CButtonHandler & cHandler,void * pClassInfo = nullptr);

	// SetFont() -- Sets the current font in the button. 
	//
	// This is usually done at the time the button is created by specifying a font int the button call (i.e. CreateButton(x,y,"Button Text",Font(MyFont)), but can be 
	// set afterwards.
	//
	bool SetFont(char * sFont);

	// This is usually done at the time the button is created by specifying a font int the button call (i.e. CreateButton(x,y,"Button Text",Font(MyFont)), but can be 
	// set afterwards.
	//
	bool SetFont(HFONT hFont);

	bool ClearFlags();
	bool SetLocation(int iX,int iY);
	POINT GetLocation();
	SIZE GetWindowSize();
	int	StartX();
	int	StartY();
	int	EndX();
	int	EndY();
	int getWidth();
	int getHeight();
	bool isVisible();
	bool isValid();
	void SetDebugID(int iDebugID);
	int GetDebugID();
	void UpdateBg(bool bUpdateNow = true);
	int GetID();
	const char * GetName();
	Sage::CWidget * GetWidgetObj();

	int GetControlID();
	bool SetHoverMsg(const char * sMessage);
	bool SetSignal(ButtonSignal & stSignal);
	bool SetSignal(bool * bSignal,bool * bSignalData = nullptr);

    // SendCloseMessage() -- Closes Parent Window when the button is pressed. 
    //
    // SendCloseMessage() sends a WM_CLOSE message to the parent window (or the window specified). 
    // If no window is specified, the Button's parent window is chosen by default.
    //
    // Use SendCloseMessage(false) to turn off the window close message.
    // For programmatic/logic uses, use SendCloseMessage(bSendCloseMsg,cWin) to specify on/off and a window in the case bSendCloseMsg == true
    //
    // In the case of a Quick Controls DevWindow, SendCloseMessage() may be used -- the window closure will affect the Dev Window's owner and
    // not the Dev Window itself.
    //
    // The button's reference is returns so that the button may be assigned after usage, i.e. MyButton = NewButton().SendCloseMessage(),
    // as a quick way to create the button and assign it as a window-closing button in one line.
    //
    CButton & SendCloseMessage(CWindow * cWin = nullptr);

    // SendCloseMessage() -- Closes Parent Window when the button is pressed. 
    //
    // SendCloseMessage() sends a WM_CLOSE message to the parent window (or the window specified). 
    // If no window is specified, the Button's parent window is chosen by default.
    //
    // Use SendCloseMessage(false) to turn off the window close message.
    // For programmatic/logic uses, use SendCloseMessage(bSendCloseMsg,cWin) to specify on/off and a window in the case bSendCloseMsg == true
    //
    // In the case of a Quick Controls DevWindow, SendCloseMessage() may be used -- the window closure will affect the Dev Window's owner and
    // not the Dev Window itself.
    //
    // The button's reference is returns so that the button may be assigned after usage, i.e. MyButton = NewButton().SendCloseMessage(),
    // as a quick way to create the button and assign it as a window-closing button in one line.
    //
    CButton & SendCloseMessage(bool bSendCloseMsg,CWindow * cWin = nullptr);

};
}; // namespace Sage
#endif // _CDavButton_H_