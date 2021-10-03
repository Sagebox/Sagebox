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
#if !defined(_ControlGroup_H_)
#define _ControlGroup_H_

#include "SageOpt.h"
#include "Sage.h"
namespace Sage
{

class CDavinci;
class CWindow;
class cwfOpt;
class ControlGroup
{
private:
	char			*	m_sOptString    = nullptr;
	int					m_iControlGroup;
	CWindow		    *	m_cParentWin    = nullptr;
	CDavinci		*	m_cDavinci      = nullptr;

	void FreeOptString() { if (m_sOptString) free(m_sOptString); m_sOptString = nullptr; };

protected:
	friend CWindow;
	ControlGroup(int iControlGroup,CWindow * cWin, CDavinci * cDavinci);
public:
    static ControlGroup m_cEmptyGroup; 

	~ControlGroup() { FreeOptString(); }
	ControlGroup();
//	int GetPressed(bool bRemove = false);	-- deprecated
    
	/// <summary>
	/// When int iPressedID is given, isPressed(iPressedID) returns TRUE when one of the control buttons has been pressed, filling
    /// iPressedID with the value.
    /// <para></para>&#160;&#160;&#160;
    /// When iPressedID is omitted, isPressed() returns -1 when no button has been pressed.  Otherwise, the 0-based index of the button pressed is returned.
    /// <para></para>
    /// --> The index returned is the 0-based index of the button int the group, in order they were added.
    /// --> Peek (i.e. Peek::yes) can be added to Peek get the result without resetting the event.
    /// <para></para>
    /// --> Important Note: This is an event query. Therefore, it will only return a true value once (unless Peek is used), resetting the the
    /// event to false on subsequent calls until another button is pressed.
	/// </summary>
	/// <param name="iPressedID">- (optional) Integer value to fill with 0-based index of button pressed when an event is active (otherwise it is not changed)</param>
	/// <param name="peek">- Use Peek::Yes to avoid resetting the event flag</param>
	/// <returns>TRUE when button is pressed (when supplying the event reference, i.e. iPressedID); otherwise returns the button pressed or -1 when no event is active.</returns>
	bool isPressed(Peek peek = Peek::No);

	/// <summary>
	/// When int iPressedID is given, isPressed(iPressedID) returns TRUE when one of the control buttons has been pressed, filling
    /// iPressedID with the value.
    /// <para></para>&#160;&#160;&#160;
    /// When iPressedID is omitted, isPressed() returns -1 when no button has been pressed.  Otherwise, the 0-based index of the button pressed is returned.
    /// <para></para>
    /// --> The index returned is the 0-based index of the button int the group, in order they were added.
    /// --> Peek (i.e. Peek::yes) can be added to Peek get the result without resetting the event.
    /// <para></para>
    /// --> Important Note: This is an event query. Therefore, it will only return a true value once (unless Peek is used), resetting the the
    /// event to false on subsequent calls until another button is pressed.
	/// </summary>
	/// <param name="iPressedID">- (optional) Integer value to fill with 0-based index of button pressed when an event is active (otherwise it is not changed)</param>
	/// <param name="peek">- Use Peek::Yes to avoid resetting the event flag</param>
	/// <returns>TRUE when button is pressed (when supplying the event reference, i.e. iPressedID); otherwise returns the button pressed or -1 when no event is active.</returns>
	bool isPressed(int & iPressedID,Peek peek = Peek::No);
    int GetCheckedButton();
    CButton & GetButton(int iPosition);

    bool isChecked(int iCheckboxID);
    bool SetCheck(int iCheckboxID); 

	bool Valid() { return m_iControlGroup != 0; }
	inline int getGroupID() { return m_iControlGroup; }
	char * getGroupName(); 
	char * GetOptString() { return m_sOptString; }
	bool SetOptions(char * sOpString);
	bool SetOptions(cwfOpt & cwOptions);
    static ControlGroup & GetEmptyGroup();
    CWindow & GetParentWindow();
};

class ButtonGroup : public ControlGroup
{
protected:
	friend CWindow;
    ButtonGroup(int iControlGroup,CWindow * cWin, CDavinci * cDavinci) : ControlGroup(iControlGroup,cWin,cDavinci) {}
public:
    ButtonGroup() : ControlGroup() { }
    __forceinline CButton & operator [](int i) { return GetButton(i); }

};

}; // namespace Sage
#endif // _ControlGroup_H_