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

#if !defined(_CDAVINCI_H_)
#define _CDAVINCI_H_
#include <windows.h>
#include "Cpaswindow.h"
#include "Sage.h"
#include "CPgr.h"
#include "CButtonBuilder.h"
#include <cfloat>
#include "CControlStyles.h"
#include "DialogStruct.h"
#include "SageOpt.h"
#include "ControlGroup.h"
#include "CEditBox.h"
#include "CString.h"
#include "CButton.h"
#include "CSlider.h"
#include "CListBox.h"
#include "CComboBox.h"
#include "CWindow.h"
#include "CRawBitmap.h"
#include <string>
#include <vector>

namespace Sage
{

class CWindow;
class CButton;
class CEditBox;
class CDavBitmap;
class CSageBox;

struct DavBitmap_t;

struct BitmapColor24_t
{
	unsigned char ucGreen;
	unsigned char ucBlue;
	unsigned char ucRed;
};
class CHoverWindow;

// CDavinci Class - Main object for Davinci GUI.  Typically created only once per application. 
//
// CDavinci creates the main window thread used for Davinci.  From the CDavinci class created, all Davinci Windows, child controls and windows are created and managed.
//
// CDavinci creates a window which can be used as a status window.  It is initially hidden, and its use is TBD.
//
// Note: There are a number of window functions (i.e. Cls(), Write(), Update(), etc.) that parallel CDavWIndow functions.
//       This is a legacy issue and should be ignored.  Most functions in the original CDavinci class have not been re-implemented here, 
//       and the ones that remain are left to use the CDavinci window as a status window.
//		 In an update, the status window will change to a CWindow member, such as cStatusWin, which then can be used 
//       as any window, with the purpose of tracking Davinci issues and general information.
//
class CDavinci
{
private:

	struct WidgetRegistry_t
	{
		int iRegistryID;
		int * iRegAddr;
	};
	std::vector<WidgetRegistry_t>	m_stWidgetRegistry;

	DialogStruct m_stDefaultDialog;

	CUserWindow	* m_cWin = nullptr;
	const char * m_sDefaultWindowName = "Davinci Status Window";	// $$ change to constexpr
	CPasWindow * m_cWinCore = nullptr;		// Main engine and interface for Win32/Windows functions used by Davinci Windows and Controls 
	bool AddControlStyle(Sage::stGraphicBitmap_t & stBitmap); 
	bool AddControlStyle(Sage::stGraphicButton_t & stButton); 
	bool AddControlStyle(Sage::stComponentBitmap_t & stButton); 
	bool AddControlStyle(Sage::ControlStyles eStyle,Sage::ControlSubStyles eSubStyle,char * sName,void * stStyle,int iSize);
	void CreateDefaultDialog();
	void LoadDefaultResources();
	void RegisterWidgets();
	void LoadWindowsSlider();
	void CreateHoverWindow();
	static bool HoverFunction(void * pDavinci,bool bShow,const char * pHoverData);
public:
	void SetSageBox(CSageBox * cSageBox);
	HWND m_hConsoleWindow = (HWND) nullptr;
	bool SetHoverFunction(CEControlAction_t * stControl);
	CHoverWindow * m_cHoverWindow = nullptr;
	
	bool ValidateControl(HWND hWnd);

	std::vector<Sage::Deleter_t>	m_vDeleters;

	int FindDeleter(void * pObject,Sage::Deleter_t * stDeleter = nullptr);
	void AttachDeleter(CWindow * cWin,void * pObject,void (*fDeleter)(void *));
	void RemoveDeleter(void * pObject);
	void CallDeleters(CWindow * cWin);
	DialogStruct * GetDefaultDialog();
	CControlStyles::stControlStyles_t * GetControlStyle(Sage::ControlStyles,char * sName);
	CPasWindow * GetWinCore();
public:
	// $$ Legacy functionality.  These are left in Davinci for now, for use with the private window Davinci creates for itself, but is also used
	//    as a status window.  This will eventually change to a CWindow * m_cStatusWin that can then be used as a DavWindow object.
	//    This is TBD, as the status window will have some limitations and may be converted to a CTextWindow rather than a CWindow. 
	//    Again, TBD.  For now, just ignore the following. See notes on CWindow counterparts for usage information.
	//

	bool m_DebugOn = false;
	void Show();
	void Cls(int iColor1=-1,int iColor2 = -1);
	void Cls(Sage::RGBColor_t rgbColor1, Sage::RGBColor_t rgbColor2  = { -1, -1, -1});
	void Update();
	void Write(char * sLine);
	bool MakeColor(char * sColor,DWORD rgbColor);
	DWORD GetColor(char * sColor,bool * pColorFound = nullptr);
	bool GetColor(char * sColor,DWORD & rgbColor);
	int Rectangle(int ix,int iy,int iWidth,int iHeight,int iColor,int iColor2 = -1);
	Sage::SageString m_stProgramName = {};
	CSageBox * m_cSageBox = nullptr;
public:
	CDavinci(HWND m_hWnd,HINSTANCE m_hInstance,int ix=0,int iy=0,int iWidth=800,int iHeight=400,const char * sName=nullptr,char * sControl = NULL);
	~CDavinci();
	CWindow & NewWindow(int ix,int iy,int iWidth,int iHeight,char * sTitle,const char * sControl=NULL);
	CWindow & NewWindow(CWindow * cWin,int ix,int iy,int iWidth,int iHeight,char * sTitle,const char * sControl=NULL);
	CWindow & NewWindow(int ix,int iy,int iWidth,int iHeight,char * sTitle,cwfOpt & cwOpt);
	CWindow & NewWindow(CWindow * cWin,int ix,int iy,int iWidth,int iHeight,char * sTitle,cwfOpt & cwOpt);
	CDavBitmap		GetBitmapObject(int iWidth,int iHeight = 1);
	[[nodiscard]] Sage::RawBitmap_t		GetBitmapStruct(int iWidth,int iHeight = 1);
	 [[nodiscard]] Sage::RawBitmap32_t	GetBitmapStruct32(int iWidth,int iHeight = 1);
	char *  LoadButtonStyle(char * sFile,char * sAltName = nullptr,CSagePGR * cMemPGR = nullptr);
	char *  LoadButtonStyle(unsigned char * sData,int iLength,char * sAltName = nullptr);
	char *  LoadBitmapStyle(char * sFile,char * sAltName = nullptr,CSagePGR * cMemPGR = nullptr);
	char *  LoadBitmapStyle(unsigned char * sData,int iLength,char * sAltName = nullptr);
	bool SetStyleAlias(Sage::ControlStyles eStyle,char * sStyle,char * sAlias);
	bool ResetStyleAlias(Sage::ControlStyles eStyle,char * sStyle);

	bool CreateGraphicButtonStyle(const char * sStyleName,Sage::GraphicButtonStyle & stStyle);
	bool CreateGraphicCheckboxStyle(const char * sStyleName,Sage::GraphicCheckboxStyle & stStyle);
	bool LoadGraphicButtonStyle(CSagePGR * cPGR,char * sAltName = nullptr);
	bool LoadGraphicBitmapStyle(CSagePGR * cPGR,char * sAltName = nullptr);
	bool LoadComponentButtonStyle(CSagePGR * cPGR,char * sAltName = nullptr);
	void SetProgramName(const char * sProgramName) { m_stProgramName = sProgramName; }
	char * GetProgramName() {return *m_stProgramName; }

	
	int RegisterWidget(int & iRegistryID);
	CBitmap ReadPgrBitmap(const char * sImageTitle,const char * sPgrPath,bool * bSuccess = nullptr);
	CBitmap ReadPgrBitmap(const char * sImageTitle,const unsigned char * sPGRMemory,bool * bSuccess = nullptr);
	HWND GetConsoleWindow() { return m_hConsoleWindow; }
};


#define QUICK_FUNCTIONS



struct DavBitmap_t
{
friend CWindow;
friend CDavinci;
private:
	bool m_bCopyPointer = false;		
public:
	int iBits					= 0;
	int iWidth					= 0;
	int iHeight					= 0;
	int iWidthBytes				= 0;
	int iTotalSize				= 0;
	unsigned char * sData		= nullptr;
	Sage::RGBColor24 * stRGB	= nullptr;	// This is a map to sData in RGB form that can be referred to more easily.

	DavBitmap_t(const DavBitmap_t & obj);
	bool ZeroData(); 
	DavBitmap_t() { }
	~DavBitmap_t();
};


class CDavBitmap
{
	friend CDavinci;
private:
	int kMaxWidth	= 5000;
	int kMaxHeight	= 5000;

	bool CreateBitmap(int iWidth,int iHeight);
	bool m_bActive				= false;
	CWindow * m_cDavWindow		= nullptr;

public:
	int iBitmapWidth		= 0;
	int iBitmapHeight		= 0;
	int iWidthBytes			= 0;
	unsigned char * sBitmapData = NULL;
	CDavBitmap(CWindow * cWindow);
	~CDavBitmap();
};

}; // namespace Sage
#endif


