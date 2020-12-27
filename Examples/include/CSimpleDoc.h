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


#if !defined(_CSimpleDoc_H_)
#define _CSimpleDoc_H_
#include "CSageBox.h"
#include "cparsehtml.h"

namespace Sage
{
class CSimpleDoc
{
private:
	static void SimpleDocButtonDeleter(void * pData);
	class ButtonHandler : public CButtonHandler
	{
		CWindow * cWin = nullptr;
		void Init(void * pClassInfo) override;
		MsgStatus OnPressed();
	};

	static void SimpleDocWinDeleter(void * pData);
	class WindowHandler : public CWindowHandler
    {
        CWindow * cWin;
        void Init(void * pClassInfo);
        MsgStatus OnClose() ;
    };


private:
    WindowHandler         * m_WindowHandler = nullptr;
    ButtonHandler         * m_ButtonHandler = nullptr;

	CHtmlImages m_cHtmlImages;
	CWindow * m_cWin = nullptr;
	CWindow * m_cWinHtml = nullptr;
	CButton * m_cButtonHtml = nullptr;

	bool m_bAddButton = true; 
//	bool m_bAddButtonHandler = true; // Bad programming 101 here. 

	unsigned char * m_sHtml = nullptr;
public:
	enum class Placement
	{
		CenterX,
		CenterY,
		CenterXY,
		None,
	};
	CSimpleDoc(CWindow * cWin,const char * sHtmlFile);
	bool AddImage(const char * sPath,Mem<unsigned char> & stImageData);
	bool MainDraw(const char * sTitle,int iWidth,int iHeight,bool bModal,Placement placement,bool bAllowDrag);
	bool Draw(const char * sTitle,int iWidth,int iHeight,const cwfOpt & cwOpt = cwfOpt());
};

#endif // _CSimpleDoc_H_
}; // namespace Sage

