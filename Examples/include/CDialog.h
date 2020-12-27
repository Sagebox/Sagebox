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

#if !defined(_CDialog_H_)
#define _CDialog_H_
#include "CSageBox.h"
#include "CBitmapWidget.h"
#include "CDialogParser.h"

#include <vector>

namespace Sage
{

class CDialogParser;
class CDialog
{
kAdvPublic:		// Public or --Adanced define
				// delete() operator is set to private.  The created EditBox is managed by Davinci, so no
				// other process should delete it.   This will cause a compiler error if it is used.
				//
	void operator delete(void * ptr) 
	{ 
		::operator delete(ptr); 
	};
	CDialog(const CDialog &p2) {
	 FailBox("CDialog::CopyConstructor","In Copy Constructor") };
private:
	class CDialogWindow : public CWindow
	{
		SageControlDelete;
		CDialog * m_cDialog = nullptr;
	public:
		CDialogWindow(CDialog * cDialog) { m_cDialog = cDialog; };
		MsgStatus OnClose() override;

		~CDialogWindow()
		{
			return;
		}
	};
friend CDialogWindow;
friend CWindow;
	std::vector<CDialogParser::stControl_t>	m_stControls;

	CBitmap m_cBackground;
	CDialogParser cDialog;
	bool		m_bDisabled			= false;	// i.e. faux-closed
	bool		m_bDisableClose		= false;
	int			m_iWinX				= 0;
	int			m_iWinY				= 0;
	int			m_iOffsetX			= 0;
	int			m_iOffsetY			= 0;
	int			m_iPadX				= 0;
	int			m_iPadY				= 0;
	int			m_iWidth			= 0;
	int			m_iHeight			= 0;
	bool		m_bClosePending		= false; // User pressed the close button, but close is disabled. 
											 // This will cause a return of true on dialog.WindowClosing, but not for the actual window

	int			m_iMinX = MAXINT;
	int			m_iMinY = MAXINT; 
	int			m_iMaxX = MININT;
	int			m_iMaxY = MININT;
	CString		m_csControls;
	CString		m_csWinTitle;

	bool m_bTransparent			= false;
	bool m_bAutoCalcWidth		= false;
	bool m_bAutoCalcHeight		= false;
	bool m_bValid				= false;
	bool m_bAutoSize			= false;
	bool m_bBaseWindow			= false;
	bool m_bCenterX				= false;
	bool m_bCenterY				= false;

	CWindow	* m_cParent			= nullptr;
	CWindow  * m_cWin			= nullptr;

	CDialog		* m_pOrgObject = nullptr;
	CButton		& CreateButton(CButton * cButton,int iX,int iY,int iWidth,int iHeight,const char * sText,const cwfOpt & cwOpt = cwfOpt());
	CButton		& CreateButton(CButton * cButton,int iX,int iY,int iWidth,int iHeight,const wchar_t * sText,const cwfOpt & cwOpt = cwfOpt());
	CButton		& CreateCheckbox(CButton * cButton,int iX,int iY,int iWidth,int iHeight,const char * sText,const cwfOpt & cwOpt = cwfOpt());
	CButton		& CreateCheckbox(CButton * cButton,int iX,int iY,int iWidth,int iHeight,const wchar_t * sText,const cwfOpt & cwOpt = cwfOpt());
	CButton		& CreateRadioButton(CButton * cButton,int iX,int iY,int iWidth,int iHeight,const char * sText,const cwfOpt & cwOpt = cwfOpt());
	CButton		& CreateRadioButton(CButton * cButton,int iX,int iY,int iWidth,int iHeight,const wchar_t * sText,const cwfOpt & cwOpt = cwfOpt());
	CTextWidget & CreateTextWidget(int iX,int iY,int iWidth,int iHeight,const char * sText,const cwfOpt & cwOpt = cwfOpt());
	CEditBox	& CreateEditBox(CEditBox * cEditBox,int iX,int iY,int iWidth,int iHeight,const char * sText,const cwfOpt & cwOpt = cwfOpt());

	void SetOrgObject(CDialog * cDialog) { m_pOrgObject = cDialog; }
	void CreateScriptControls();
	void LocCls();
	void GetBackground();
	void RecalcControls();
	void Disable();
public:
	static void deleteObj(CDialog * p) 
	{ 
		delete p; 
	}
	CDialog(CWindow * cParent,CWindow * cOutputWindow);
	~CDialog();
	bool Init(int iX,int iY,int iWidth,int iHeight,const char * sTitle, const char * sControls);
	bool Compile(const char * sDialog);

	CButton & NewButton(int iX,int iY,int iWidth,int iHeight,const char * sText = nullptr,const cwfOpt & cwOpt = cwfOpt());
	CButton & NewButton(int iX,int iY,int iWidth,int iHeightconst ,const cwfOpt & cwOpt = cwfOpt());
	CButton & NewButton(int iX,int iY,const char * sText = nullptr,const cwfOpt & cwOpt = cwfOpt());
	CButton & NewButton(int iX,int iY,const cwfOpt & cwOpt = cwfOpt());

	CButton & NewButton(int iX,int iY,int iWidth,int iHeight,const wchar_t * sText,const cwfOpt & cwOpt = cwfOpt());
	CButton & NewButton(int iX,int iY,const wchar_t * sText,const cwfOpt & cwOpt = cwfOpt());
	
	CButton & NewButton(CButton * cButton,int iX,int iY,int iWidth,int iHeight,const char * sText = nullptr,const cwfOpt & cwOpt = cwfOpt());
	CButton & NewButton(CButton * cButton,int iX,int iY,int iWidth,int iHeight,const cwfOpt & cwOpt = cwfOpt());
	CButton & NewButton(CButton * cButton,int iX,int iY,const char * sText = nullptr,const cwfOpt & cwOpt = cwfOpt());
	CButton & NewButton(CButton * cButton,int iX,int iY,const cwfOpt & cwOpt = cwfOpt());

	CButton & NewButton(CButton * cButton,int iX,int iY,int iWidth,int iHeight,const wchar_t * sText,const cwfOpt & cwOpt = cwfOpt());
	CButton & NewButton(CButton * cButton,int iX,int iY,const wchar_t * sText,const cwfOpt & cwOpt = cwfOpt());
	
	// Sub-Classable versions

	CButton & NewCheckbox(int iX,int iY,int iWidth,int iHeight,const char * sText = nullptr,const cwfOpt & cwOpt = cwfOpt());
	CButton & NewCheckbox(int iX,int iY,int iWidth,int iHeight,const cwfOpt & cwOpt = cwfOpt());
	CButton & NewCheckbox(int iX,int iY,const char * sText = nullptr,const cwfOpt & cwOpt = cwfOpt());
	CButton & NewCheckbox(int iX,int iY,const cwfOpt & cwOpt = cwfOpt());

	CButton & NewCheckbox(int iX,int iY,int iWidth,int iHeight,const wchar_t * sText,const cwfOpt & cwOpt = cwfOpt());
	CButton & NewCheckbox(int iX,int iY,const wchar_t * sText,const cwfOpt & cwOpt = cwfOpt());

	// Sub-Classable versions

	CButton & NewCheckbox(CButton * cButton,int iX,int iY,int iWidth,int iHeight,const char * sText = nullptr,const cwfOpt & cwOpt = cwfOpt());
	CButton & NewCheckbox(CButton * cButton,int iX,int iY,int iWidth,int iHeight,const cwfOpt & cwOpt = cwfOpt());
	CButton & NewCheckbox(CButton * cButton,int iX,int iY,const char * sText = nullptr,const cwfOpt & cwOpt = cwfOpt());
	CButton & NewCheckbox(CButton * cButton,int iX,int iY,const cwfOpt & cwOpt = cwfOpt());

	CButton & NewCheckbox(CButton * cButton,int iX,int iY,int iWidth,int iHeight,const wchar_t * sText,const cwfOpt & cwOpt = cwfOpt());
	CButton & NewCheckbox(CButton * cButton,int iX,int iY,const wchar_t * sText,const cwfOpt & cwOpt = cwfOpt());


	CButton & NewRadioButton(int iX,int iY,int iWidth,int iHeight,const char * sText = nullptr,const cwfOpt & cwOpt = cwfOpt());
	CButton & NewRadioButton(int iX,int iY,int iWidth,int iHeight,const cwfOpt & cwOpt = cwfOpt());
	CButton & NewRadioButton(int iX,int iY,const char * sText = nullptr,const cwfOpt & cwOpt = cwfOpt());
	CButton & NewRadioButton(int iX,int iY,const cwfOpt & cwOpt = cwfOpt());

	CButton & NewRadioButton(int iX,int iY,int iWidth,int iHeight,const wchar_t * sText,const cwfOpt & cwOpt = cwfOpt());
	CButton & NewRadioButton(int iX,int iY,const wchar_t * sText,const cwfOpt & cwOpt = cwfOpt());
	
	// Sub-Classable versions
	
	CButton & NewRadioButton(CButton * cButton,int iX,int iY,int iWidth,int iHeight,const char * sText = nullptr,const cwfOpt & cwOpt = cwfOpt());
	CButton & NewRadioButton(CButton * cButton,int iX,int iY,int iWidth,int iHeight,const cwfOpt & cwOpt = cwfOpt());
	CButton & NewRadioButton(CButton * cButton,int iX,int iY,const char * sText = nullptr,const cwfOpt & cwOpt = cwfOpt());
	CButton & NewRadioButton(CButton * cButton,int iX,int iY,const cwfOpt & cwOpt = cwfOpt());

	CButton & NewRadioButton(CButton * cButton,int iX,int iY,int iWidth,int iHeight,const wchar_t * sText,const cwfOpt & cwOpt = cwfOpt());
	CButton & NewRadioButton(CButton * cButton,int iX,int iY,const wchar_t * sText,const cwfOpt & cwOpt = cwfOpt());

	// Currently no method to sub-class the Text Widget

	CTextWidget & NewTextWidget(int iX,int iY,int iWidth,int iHeight,const char * sText = nullptr,const cwfOpt & cwOpt = cwfOpt());
	CTextWidget & NewTextWidget(int iX,int iY,int iWidth,int iHeight,const cwfOpt & cwOpt = cwfOpt());
	CTextWidget & NewTextWidget(int iX,int iY,const char * sText = nullptr,const cwfOpt & cwOpt = cwfOpt());
	CTextWidget & NewTextWidget(int iX,int iY,const cwfOpt cwOpt = cwfOpt());


	CWindow & NewBitmapWindow(int iX,int iY,CBitmap & cBitmap,const cwfOpt & cwOpt = cwfOpt());
	CWindow & NewBitmapWindow(int iX,int iY,Sage::RawBitmap_t & cBitmap,const cwfOpt & cwOpt = cwfOpt());



	CEditBox & NewEditBox(int iX,int iY,int iWidth,int iHeight,const char * sText = nullptr,const cwfOpt & cwOpt = cwfOpt());
	CEditBox & NewEditBox(int iX,int iY,int iWidth,int iHeight,const cwfOpt & cwOpt = cwfOpt());
	CEditBox & NewEditBox(int iX,int iY,const char * sText = nullptr,const cwfOpt & cwOpt = cwfOpt());
	CEditBox & NewEditBox(int iX,int iY,const cwfOpt cwOpt = cwfOpt());

	// Sub-Classable versions

	CEditBox & NewEditBox(CEditBox * cEditBox,int iX,int iY,int iWidth,int iHeight,const char * sText = nullptr,const cwfOpt & cwOpt = cwfOpt());
	CEditBox & NewEditBox(CEditBox * cEditBox,int iX,int iY,int iWidth,int iHeight,const cwfOpt & cwOpt = cwfOpt());
	CEditBox & NewEditBox(CEditBox * cEditBox,int iX,int iY,const char * sText = nullptr,const cwfOpt & cwOpt = cwfOpt());
	CEditBox & NewEditBox(CEditBox * cEditBox,int iX,int iY,const cwfOpt cwOpt = cwfOpt());



	CSlider	& NewSlider(int iX,int iY,int iWidth,const char * sLabel = nullptr,const cwfOpt & cwOpt = cwfOpt());
	CSlider	& NewSlider(int iX,int iY,int iWidth,const wchar_t * sText,const cwfOpt & cwOpt = cwfOpt());
	CSlider	& NewSlider(int iX,int iY,int iWidth,const cwfOpt & cwOpt = cwfOpt());

	// Sub-Classable versions

	CSlider	& NewSlider(CSlider * cSlider,int iX,int iY,int iWidth,const char * sLabel = nullptr,const cwfOpt & cwOpt = cwfOpt());
	CSlider	& NewSlider(CSlider * cSlider,int iX,int iY,int iWidth,const wchar_t * sText,const cwfOpt & cwOpt = cwfOpt());
	CSlider	& NewSlider(CSlider * cSlider,int iX,int iY,int iWidth,const cwfOpt & cwOpt = cwfOpt());

	CWindow & NewWindow(int iX,int iY,int iWidth,int iHeight,const cwfOpt & cwOpt);

	// Instantiate the dialog box.  By default, this also shows the Dialog Box.
	// When bShow == false (i.e. dialog.Go(false)), the dialog box stays hidden so that changes
	// can be made (i.e. background, drawing objects, resizing, etc.) before the dialog box is shown.
	// Use dialog.Show() to make the dialog box visible.
	//
	bool Go(bool bShow = true);
	void AutoSize();
	CButton & GetButton(int iID);
	CButton & GetButton(const char * sName);

	CSlider & GetSlider(int iID);
	CSlider & GetSlider(const char * sName);

	CEditBox & GetEditBox(int iID);
	CEditBox & GetEditBox(const char * sName);

	CWindow & GetWindow(int iID);
	CWindow & GetWindow(const char * sName);
	CWindow & GetWindow() { return *m_cWin; }

	// Same as GetWindow() -- incuded for clarity

	CWindow & GetBitmapWindow(int iID);
	CWindow & GetBitmapWindow(const char * sName);

	SIZE GetPadding() { return { m_iPadX, m_iPadY }; }
	void AttachWidget(int iRegistryValue,CWidget * cWidget);

	CTextWidget & GetTextWidget(int iID);
	CTextWidget & GetTextWidget(const char * sName);

	Sage::CWidget * GetWidget(int iRegistryValue,int iID);
	Sage::CWidget * GetWidget(int iRegistryValue,const char * sName);

	bool isButtonPressed(int iGroup,int & iButton);
	bool isButtonPressed(const char * sGroup,int & iButton);
//	int isButtonPressed(int iGroup);		-- These two are deprecated
//	int isButtonPressed(const char * sGroup); 

	// Get the currently selected radio button in a group of radio buttons.
	// This returns the ID if the button set with ID() or the 1-based ID set
	// automatically when Group() is specified, in order of creation.
	//
	int GetSelectedRadioButton(int iGroup);

	// Get the currently selected radio button in a group of radio buttons.
	// This returns the ID if the button set with ID() or the 1-based ID set
	// automatically when Group() is specified, in order of creation.
	//
	int GetSelectedRadioButton(const char * sGroup);
	bool Show(bool bShow = true);
	bool Hide(bool bHide = true);
	void Delete();
	void DisableClose(bool bDisable); 
	bool WindowClosing();
	bool ClosePressed(bool bPeek = false);


};

}; // namespace Sage
#endif // _CDialog_H_