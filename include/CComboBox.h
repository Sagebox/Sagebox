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
//File copyright(c) 2020, Rob Nelson.  All rights reserved.   robnelsonxx2@gmail.com
//
//#pragma once

#if !defined(_CDavComboBox_H_)
#define _CDavComboBox_H_
#include "CDavinci.h"
#include "CComboBoxHandler.h"


// --------------
// CListBox Class
// --------------
//
//
// This class is returned when a List Box is created and contains most list box functions.
//
// The object returned is managed by SageBox and shouldn't be deleted.  Use the Delete() funtion
// to remove the Edit Box.
//
// Some Examples:
//
// CEditBox & MyListBox = MyWin.NewistBox(10,50,400,200)		-- Creates a list box at (10,50) in  the window, 400 pixels wide by (200 to the nearest character line)

// auto MyListBox =  MyWin.NewistBox(10,50,400,200)				-- Does the same thing, just without specifying the type
//
namespace Sage
{

class CComboBox;
using CCombobox = CComboBox;
class CComboBox : public CComboBoxHandler
{
kAdvPublic:		// Public or --Adanced define
				// delete() operator is set to private.  The created ListBox is managed by SageBox, so no
				// other process should delete it.   This will cause a compiler error if it is used.
				//
	void operator delete(void *) { };
	CComboBox(const CComboBox &p);						// Copy constructor to force error on copy (since its a managed object)

public:
	static void deleteObj(CComboBox * p) 
	{ 
		delete p; 
	}
private:
	void EnableInvalidate(bool bEnable,bool bRefresh = false);		// Enable/Disable widow redraws.  Used internally.

	class CWidget : public Sage::CWidget
	{
		CComboBox * m_cObj;
	public:
		CWidget(CComboBox * cObj);
		CWindow * GetWindow()				;
		bool SetLocation(int iX,int iY)		;
		bool isValid()						;
		bool Show(bool bShow)				;
		bool Hide(bool Hide)				;
		int GetID()							;
		const char * GetName()				;
	};												

	CWidget * m_cWidget = nullptr;
public:
	friend CWindow;
	int					  m_iControl	;		// SageBox Control ID
	int					  m_iWinControl	;		// Parent Window Control ID
	CWindow				* m_cDavWin		;		// Pointer to Parent Window object
	CPasWindow			* m_cWinCore	;		// Pointer to Core internal functions
    CEControlAction_t	* m_stControl	;		// A Copy of the internal control for easy-access
	int					  m_iCopyCount	;		// Just some debug/statistic keeping.  A value of 0 suggests we're the original 
												// object, but it is not used internally. 
    HWND                  m_hWnd = nullptr;
	void Init(int iControl,CWindow * cDavWindow,CPasWindow * cWinCore);		// Initialize the class/object

public:

	CComboBox();

    CComboBox(CComboBox && p) noexcept
    {
        int gg = 1;
    }
	// Show the List Box Window (i.e. make it visible)
	//
	bool Show(bool bShow = true);

	// Hide the List Box Window
	//
	bool Hide(bool bHide = true);

	// Set the Font in the List Box Window.  
	// This is usually used just after creating the List Box in a hidden state.  You can also use
	// "Setfont=<fontname or type>" or opt::SetFont(<fontname or type>) to set it in the initial call.
	// --> Example: SetFont("Arial,12"), or SetFont("RobsPrivateFont"), or SetFont(hMyFont) (where HFONT MyFont = <Windows Font>
	//
	bool SetFont(const char * sFont);

	// Set the Font in the List Box Window.  
	// This is usually used just after creating the List Box in a hidden state.  You can also use
	// "Setfont=<fontname or type>" or opt::SetFont(<fontname or type>) to set it in the initial call.
	// --> Example: SetFont("Arial,12"), or SetFont("RobsPrivateFont"), or SetFont(hMyFont) (where HFONT MyFont = <Windows Font>
	//
	bool SetFont(HFONT hFont);

	// Set the size of the List Box Window.  This is usually set right after creating the List Box in a hidden state.
	// The initial creation determines the size based on the Font being used and other input options.  SetWindowSize()
	// can be used to set a specific size for the window. 
	// --> You can also specifically set the size in NewListBox() by giving the X,Y location and Width,Height as parameters.
	//
	bool SetWindowSize(int iWidth,int iHeight); 

	// Set the size of the List Box Window.  This is usually set right after creating the List Box in a hidden state.
	// The initial creation determines the size based on the Font being used and other input options.  SetWindowSize()
	// can be used to set a specific size for the window. 
	// --> You can also specifically set the size in NewListBox() by giving the X,Y location and Width,Height as parameters.
	//
	bool SetWindowSize(SIZE szSize);

	// Set Text Color in the List Box.  This can also be specified when the box is created in the options, i.e. "SetFgColor=MyColor" or opt::fgColor(MyColor)
	//
	bool SetFgColor(DWORD dwColor);
	// Set Text Color in the List Box.  This can also be specified when the box is created in the options, i.e. "SetFgColor=MyColor" or opt::fgColor(MyColor)
	//

	bool SetFgColor(const char * sColor);

	// Set Background Color in the List Box.  This can also be specified when the box is created in the options, i.e. "SetBgColor=MyColor" or opt::bgColor(MyColor)
	//
	bool SetBgColor(DWORD dwColor);

	// Set Background Color in the List Box.  This can also be specified when the box is created in the options, i.e. "SetBgColor=MyColor" or opt::bgColor(MyColor)
	//
	bool SetBgColor(const char * sColor);



	// Delete the Lit Box.  Since the list box object is managed by SageBox, you can remove the list box from the window
	// by specifying ListBox.Delete().   The List Box is automatically deleted when the parent window is closed.
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

	// Set the location of the ListBox Window
	//
	bool SetLocation(int iX,int iY);

	// Set the location of the ListBox Window
	//
	bool SetLocation(POINT pLoc);

	// Get the location of the ListBox window relative to the parent window
	//
	POINT GetLocation();

	// Get the location of the ListBox window relative to the parent window
	//
	// By default, GetWindowSize() returns the size of the interior (i.e. canvas space) of the listbox.
	//
	// Use GetWindowSize(true) to get the entire size of the window, including any frames, menus,
	// or borders.
	//
	// 
	SIZE GetWindowSize(bool bFrameSize = false);

	// Get the rectangular points of the window.
	//
	// By default GetWindowRect() returns the RECT of the interior (i.e. canvas space) of the listbox,
	// 0-based for the client area o the window.
	//
	// Use GetWindowRect(true) to get the entire RECT of the window, including any frames, menus,
	// or borders -- the coordinates are relatie to the parent window
	// 
	RECT GetWindowRect(bool bFrameSize = false);

	// Get the ID of the control set with ID() in the original listbox creation i.e. opt::ID(<id_value>);
	//
	int GetID();

	// Get the name of the control set with Name() in the original listbox creation, i.e. opt::Name("MyListBox");
	//
	const char * GetName();

	// Get the left X position of the Listbox window
	//
	int	StartX();

	// Get the top Y position of the Listbox window
	//
	int	StartY();

	// Get the End X position of the Listbox window
	//
	int	EndX();

	// Get the Bottom Y position of the Listbox window
	//
	int	EndY();

	// Get the width of the ListBox
	//
	int GetWidth();

	// Get the height of the ListBox
	//
	int GetHeight();

	// Returns TRUE if the listbox is visible, FALSE if the listbox is hidden
	//
	bool isVisible();

	// Redraws the ComboBox
	//
	bool Redraw();

	// Returns TRUE of this is a valid listbox.  FALSE if this is an empty listbox (i.e. bad listbox) or it has been deleted from the window.
	//
	bool isValid();
	
    /// <summary>
    /// Returns Windows handle to Listbox window. 
    /// </summary>
    HWND GetWindowHandle();

    /// <summary>
    /// Invalidates listbox window and forces a redraw.
    /// </summary>
    /// <param name="bErase">when TRUE, erases background first. May be useful for Non-client area, but can also cause a flicker.</param>
    /// <returns></returns>
    bool Invalidate(bool bErase = false); 

	bool SetMessageHandler(CComboBoxHandler * cMessageHandler,void * pClassInfo = nullptr);
	bool SetSelection(int iSelection);
	bool AddItem(const char * sItem);
    bool AddItems(const char * * sItems); 
    bool AddItems(const char * sItems); 
    bool AddItems(int iNumItems,const char * * sItems); 

	bool ItemSelected(int & iItem,bool bPeek = true);
	bool ItemSelected(bool bPeek = true);
	int GetItemSelected();
    CString GetText(int iItem);
	int GetNumItems();
	bool ClearList();
	
    bool SetTitleCell(const char * sTitle); 


	// virtual destructor -- allows overloaded objects cast as CListBox to delete properly.
	//
	virtual ~CComboBox();
};

}; // Namespace Sage

#endif	// _CDavComboBox_H_