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
	bool isPressed(Peek peek = Peek::No);
	bool isPressed(int & iPressedID,Peek peek = Peek::No);
    int GetCheckedButton();
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
}; // namespace Sage
#endif // _ControlGroup_H_