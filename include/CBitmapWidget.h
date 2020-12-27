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
#if !defined(_CBitmapWidget_H_)
#define _CBitmapWidget_H_
#include "CDavinci.h"

namespace Sage
{

class CBitmapWidget : private CWindow
{
	SageControlDelete
private:
	bool	m_bValid = false;
	CBitmap * m_cBitmap = nullptr;
	int			m_iX;
	int			m_iY;
	int			m_iWidth;
	int			m_iHeight;
	bool		m_bBitmapValid = false;
	CWindow	* m_cParent = nullptr;
	CWindow	* m_cWin = nullptr;
	bool		m_bMouseMoved;
	POINT		m_szMousePos;
	void Init();
private:
	MsgStatus OnMouseMove(int iMouseX,int iMouseY);

public:
	CBitmapWidget(CWindow * cParent,int iX,int iY,CBitmap & cBitmap);
	bool MouseMoved(); 
	bool MouseMoved(POINT & pMouse);
	bool Update(CBitmap & cBitmap);
	POINT GetMousePos();

};
}; // namespace Sage
#endif // _CBitmapWidget_H_