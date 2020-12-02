//File copyright(c) 2020, Rob Nelson.  All rights reserved.   robnelsonxx2@gmail.com
//
//#pragma once

#if !defined(_CDavEditBox_H_)
#define _CDavEditBox_H_
#include "CDavinci.h"
#include "CEditBoxHandler.h"
#include "CPoint.h"

// -----------------
// CEditBox Class
// -----------------
//
// This class is returned when an Edit Box is created and contains most edit box functions.
//
// The object returned is managed by Davinci and shouldn't be deleted.  Use the Delete() funtion
// to remove the Edit Box.
//
// Some Examples:
//
// CEditBox & MyEditBox = MyWin.CreateEditBox(10,50)		-- Creates an edit box at (10,50) in the  the window.
// auto MyEditBox = MyWin.CreateEditBox(10,50)				-- Does the same thing, just without specifying the type
// auto pMyEditBox = &MyWin.CreateEditBox(10,50)			-- Also creates an edit box at (10,50), but assigns it to a pointer so it is more easily passed.
//															   This is useful when specifying it in a class.  Do not delete this pointer (it will cause a compiler error anyway).
// Overloading
//
// You can overload the EditBox class to use your own functions and trap Window messages (i.e. WM_PAINT, etc.).
// With the overload, you can then give this to the CreateEditBox() function.  You can then use all of the EditBox
// functions as well as any you care to override, such as message and valiate functions.
//
// Example:
//
// CRobEditBox : public CEditBox
// {
//		Status Validate(const char * sText) override;
// };
//
// Setting a Message Handler
//
// TBD
//

namespace Sage
{
class CArrowBox;

class CEditBox : public CEditBoxHandler
{
kAdvPublic:		// Public or --Adanced define
				// delete() operator is set to private.  The created EditBox is managed by Davinci, so no
				// other process should delete it.   This will cause a compiler error if it is used.
				//
	void operator delete(void *) { };
	CEditBox(const CEditBox &p);						// Copy constructor to force error on copy (since its a managed object)

public:
	static void deleteObj(CEditBox * p) 
	{ 
		delete p; 
	}

private:
	void EnableInvalidate(bool bEnable,bool bRefresh = false);		// Enable/Disable widow redraws.  Used internally.
	bool			m_bArrowBox		= false;
	bool			m_bLabelPending = false;
	CString			m_csLabelFont;
	CString			m_csLabel;
	RGBColor_t		m_rgbLabelColor;
	int				m_iLabelX = 0;
	LabelJust		m_eLabelJustType = LabelJust::None;

	CArrowBox	* m_cArrowBox	= nullptr;

	bool SetLabelPending(LabelJust eLabelJust,int iX,const char * sLabel,const char * sFont,RGBColor_t rgbColor); 

public:
	friend CWindow;
	int					  m_iControl	;		// Davinci Control ID
	int					  m_iWinControl	;		// Parent Window Control ID
	CWindow				* m_cDavWin		;		// Pointer to Parent Window object
	CPasWindow			* m_cWinCore	;		// Pointer to Core internal functions
    CEControlAction_t	* m_stControl	;		// A Copy of the internal control for easy-access
	int					  m_iCopyCount	;		// Just some debug/statistic keeping.  A value of 0 suggests we're the original 
												// object, but it is not used internally. 

	void Init(int iControl,CWindow * cDavWindow,CPasWindow * cWinCore);		// Initialize the class/object
	bool SetValidateMsg(char * sMsg) { return true; }							// $$ Not Sure -- this may not be used.
	int GetControlID();															// Internal Use.  $$ May not be used

public:
//	CEditBox(const CEditBox &p);						// Copy constructor		- $$ Debug/Testing
//	CEditBox & operator = (CEditBox & p2);			// Assignment operator, i.e. EditBoxCopy = EditBox, so we can track it.	- $$ Debug/Testing

	CEditBox();

	// Set an external message handler object.
	// This can be used to trap all WM_ windows messages without explicitly subclassing the
	// EditBox class. 
	//
	bool SetMessageHandler(CEditBoxHandler & cEditBoxHandler,void * pClassInfo = nullptr);																		
																	
	// Get text displaying in the edit box.  returns nullptr or "" if nothing exists.
	//
	const char * GetText()			;	

	// Get text displaying in the edit box (in Unicode);
	//
	const wchar_t * GetTextW()		;				

	// Get the length of the text string in the Edit Box text areas
	//
	int	GetTextLength()			;

	// Set the text in the edit box
	//
	bool	SetText(const char * sText)	;
	bool SetText(int iValue);
	bool SetText(double fValue);
	double GetFloat(bool * bOk = nullptr);
	bool GetFloat(double & fValue,double fSetValue = DBL_MIN);
	int GetInteger(bool * bOk = nullptr);
	bool GetInteger(int & iValue,int iSetValue = INT_MIN);

	// Clear the text in the Edit Box
	//
	bool ClearText()				;

	// Show the Edit Box Window (i.e. make it visible)
	//
	bool Show(bool bShow = true);

	// Hide the Edit Box Window
	//
	bool Hide(bool bHide = true);
	bool Enable(bool bEnable = true);
	bool Disable(bool bDisable = true);

	// Was a return Pressed?  True if so, false it not.
	// With Peek = Sage::Peek::No  (default), the "return pressed" flag is reset. This will cause subsequent values to return false
	// until the Return Key is pressed again.
	// 
	// Use ReturnPressed(Sage::Peek::Yes) to keep from resetting the flag
	//
	bool ReturnPressed(bool bPeek = false);
	bool ReturnPressed(double & fValue,bool bPeek = false);
	bool ReturnPressed(int & fValue,bool bPeek = false);
	bool MouseWheelMoved(bool bPeek = false);
	bool MouseWheelMoved(int & iMouseWheelValue,bool bPeek = false);
	int GetMouseWheelValue();

	// $$ TBD
	//
    int SetCharCallback(int (*MyFunc)(CEditBox *,WPARAM & , LPARAM & ,void*),void * vpCallbackData = NULL);

	// $$ TBD
	//
    int SetValidateCallback(int (*MyFunc)(CEditBox *,WPARAM & ,void*),void * vpCallbackData = NULL);


	bool SetMouseWheel(double fAdd,double fSub,double fMin = 0,double fMax = 0);
	bool SetMouseWheel(int iAdd,int iSub,int iMin = 0,int iMax = 0);



	// Set the Font in the Edit Box Window.  This may change the size of the Edit Box. 
	// This is usually used just after creating the Edit Box in a hidden state.  You can also use
	// "Setfont=<fontname or type>" or opt::SetFont(<fontname or type>) to set it in the initial call.
	// --> Example: SetFont("Arial,12"), or SetFont("RobsPrivateFont"), or SetFont(hMyFont) (where HFONT MyFont = <Windows Font>
	//
	bool SetFont(char * sFont);

	// Sets the keyboard focus to the Edit Box -- this places the input caret in the editbox so the user does 
	// not have to click on it to start editing.
	//
	bool SetFocus();

	// Set the Font in the Edit Box Window.  This may change the size of the Edit Box. 
	// This is usually used just after creating the Edit Box in a hidden state.  You can also use
	// "Setfont=<fontname or type>" or opt::SetFont(<fontname or type>) to set it in the initial call.
	// --> Example: SetFont("Arial,12"), or SetFont("RobsPrivateFont"), or SetFont(hMyFont) (where HFONT MyFont = <Windows Font>
	//
	bool SetFont(HFONT hFont);

	// Set the size of the Edit Box Window.  This is usually set right after creating the Edit Box in a hidden state.
	// The initial creation determines the size based on the Font being used and other input options.  SetWindowSize()
	// can be used to set a specific size for the window. 
	// --> You can also specifically set the size in CreateEditBox() by giving the X,Y location and Width,Height as parameters.
	// --> See CreateEditBox() in CDavWindow header file.
	//
	bool SetWindowSize(int iWidth,int iHeight=-1); 

	// $$ TBD
	bool SetParentColors(DWORD * dwBgColor = nullptr,DWORD * dwFgColor = nullptr);

	// Set Text Color in the Edit Box.  This can also be specified when the box is created in the options, i.e. "SetFgColor=MyColor" or opt::fgColor(MyColor)
	//
	bool SetFgColor(DWORD dwColor);

	// Set Background Color in the Edit Box.  This can also be specified when the box is created in the options, i.e. "SetBgColor=MyColor" or opt::bgColor(MyColor)
	//
	bool SetBgColor(DWORD dwColor);

	// Set Minimum and Maximum values for the Edit Box.  This accepts integer, float, and double (you only need one of them).  
	// When set, the input to the Edit Box will be validated.  If the number in the edit box is not within the range specified, an error box will appear
	// telling the user to enter a number in the correct range
	//
	bool SetRange(int iMin,int iMax);

	// Set Minimum and Maximum values for the Edit Box.  This accepts integer, float, and double (you only need one of them).  
	// When set, the input to the Edit Box will be validated.  If the number in the edit box is not within the range specified, an error box will appear
	// telling the user to enter a number in the correct range
	//
	bool SetRange(double fMin,double fMax);

	// Set Minimum and Maximum values for the Edit Box.  This accepts integer, float, and double (you only need one of them).  
	// When set, the input to the Edit Box will be validated.  If the number in the edit box is not within the range specified, an error box will appear
	// telling the user to enter a number in the correct range
	//
	bool SetRange(float fMin,float fMax);

	// Set Minimum Value for Edit Box.  If Return Key or Validate Button is pressed, and the value is less than the minimmum, an error message will appear informing
	// the user to enter a number greater than the minimum.  Max values does not need to be set. 
	//
	bool SetMin(int iMin);

	// Set Minimum Value for Edit Box.  If Return Key or Validate Button is pressed, and the value is less than the minimmum, an error message will appear informing
	// the user to enter a number greater than the minimum.  Max values does not need to be set. 
	//
	bool SetMin(double fMin);

	// Set Minimum Value for Edit Box.  If Return Key or Validate Button is pressed, and the value is less than the minimmum, an error message will appear informing
	// the user to enter a number greater than the minimum.  Max values does not need to be set. 
	//
	bool SetMin(float fMin);

	// Set Maximum Value for Edit Box.  If Return Key or Validate Button is pressed, and the value is greater than the maximum, an error message will appear informing
	// the user to enter a number less than the maximum.  Min values does not need to be set. 
	//
	bool SetMax(int iMax);

	// Set Maximum Value for Edit Box.  If Return Key or Validate Button is pressed, and the value is greater than the maximum, an error message will appear informing
	// the user to enter a number less than the maximum.  Min values does not need to be set. 
	//
	bool SetMax(double fMax);

	// Set Maximum Value for Edit Box.  If Return Key or Validate Button is pressed, and the value is greater than the maximum, an error message will appear informing
	// the user to enter a number less than the maximum.  Min values does not need to be set. 
	//
	bool SetMax(float fMax);

	// Remove minimum or maximum restrictions. This will remove any automatic validation on the edit box.
	//
	bool ResetMinMax();

	// Used in the overloaded Validate function.  This will set the error strings to display in the error box when
	// the a user-written validate function is used. 
	//
	bool SetErrorStrings(char * sErrString,char * sErrHeader);

	// Delete the Edit Box.  Since the edit box object is managed by Davinci, you can remove the edit box from the window
	// by specifying EditBox.Delete().   The Edit Box is automatically deleted when the parent window is closed.
	//
	// Typically returns TRUE -- only returns FALSE of the Edit Box is invalid or corrupt (which should never happen)
	//
	bool Delete();

	// UserDelete() -- called from Delete() to allow your overloaded class to respond to the Delete() call on the control.
	//
	// Using UserDelete() as a virtual function.
	//
	// UserDelete() gives your overload the chance to delete and memory allocated at the time you decide to delete the control.  The actual control (and, therefore, destructor)
	// is not called until the Parent WIndow is closed.  This will leave any memory allocated still allocated until this time, even if the control is no longer in use before the 
	// parent window is closed.
	//
	// UserDelete() should be idempotent (i.e. can be called multiple times with the same effect)
	//
	// In your overloaded class, UserDelete() should only be called once, but should be prepared to be called multiple times, just to 
	// anticipate programming errors and whatever Davinci decides to do under the hood.  That is, since Delete() can legally be called
	// multiple times (and, therefore, UserDelete()), UserDelete() should account for this.
	//
	// Make sure any memory pointers deallocated are set to nullptr, and checked prior to deletion (i.e. if (myMemPtr) delete MyMemPtr; MyMemPtr = nullptr; ), so that
	// subseqent calls will have no effect. 
	// 
	// Otherwise, you can wait for the destruction of the object to delete any memory allocated in the overloaded class.
	//
	virtual void UserDelete() { }

	bool ClearFlags();
	bool SetLocation(int iX,int iY);
	POINT GetLocation();
	SIZE GetWindowSize();
	int GetID();
	const char * GetName();

	SizeRect GetBoxSizeRect();
	SizeRect GetArrowBoxSizeRect();
	HWND GetWindowHandle();
	bool SetHoverMsg(const char * sHoverMessage);
	int	StartX();
	int	StartY();
	int	EndX();
	int	EndY();
	int getWidth();
	int getHeight();
	SIZE getSize();
	bool isVisible();
	bool isValid();
	bool AttachArrowBox(CWindow * cParent);
	bool SetSignal(bool * pSignal,char * pTextSignal = nullptr,int iTextLength = 0);
	bool Redraw();
	bool SetSignal(EditBoxSignal & stSignal);
	bool ResetSignal();
	bool UpdateBg(bool bUpdateNow = true);
	// virtual destructor -- allows overloaded objects cast as CEditBox to delete properly.
	//
	virtual ~CEditBox();
};

}; // Namespace Sage

#endif	// _CDavEditBox_H_