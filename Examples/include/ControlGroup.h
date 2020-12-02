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
	char			*	m_sOptString = nullptr;
	int					m_iControlGroup;
	CWindow		*	m_cParentWin;
	CDavinci		*	m_cDavinci;

	void FreeOptString() { if (m_sOptString) free(m_sOptString); m_sOptString = nullptr; };

protected:
	friend CWindow;
	ControlGroup(int iControlGroup,CWindow * cWin, CDavinci * cDavinci);
public:
	~ControlGroup() { FreeOptString(); }
	ControlGroup() { m_iControlGroup = 0; };
//	int GetPressed(bool bRemove = false);	-- deprecated
	bool isPressed(Peek peek = Peek::No);
	bool isPressed(int & iPressedID,Peek peek = Peek::No);
	bool Valid() { return m_iControlGroup != 0; }
	inline int getGroupID() { return m_iControlGroup; }
	char * getGroupName(); 
	char * GetOptString() { return m_sOptString; }
	bool SetOptions(char * sOpString);
	bool SetOptions(cwfOpt & cwOptions);
};
}; // namespace Sage
#endif // _ControlGroup_H_