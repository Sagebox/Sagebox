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
	CSageBitmap * m_cBitmap = nullptr;
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
	CBitmapWidget(CWindow * cParent,int iX,int iY,CSageBitmap & cBitmap);
	bool MouseMoved(); 
	bool MouseMoved(POINT & pMouse);
	bool Update(CSageBitmap & cBitmap);
	POINT GetMousePos();

};
}; // namespace Sage
#endif // _CBitmapWidget_H_