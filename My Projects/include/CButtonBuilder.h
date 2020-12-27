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
#include "CPgr.h"
#include "COutDC.h"
#include "Windows.h"
#include "CpasWindow.h"
#if !defined(_CButtonBuilder_H_)
#define _CButtonBuilder_H_

namespace Sage
{

class COutDC;


class CButtonBuilder
{
public:
	enum class Transparent
	{
		Yes,
		No,
	};
protected:
	CPasWindow * m_cMain;
	int		m_iPaddingX		    = 0;
	int		m_iPaddingY		    = 0;
	bool	m_bDebug			= false;
	int		m_iTextStartX 		= 0;
	int		m_iTextStartY 		= 0;
	int		m_iTextHeight 		= 0;
	int		m_iTextWidth		= 0;
	Sage::stButtonBitmaps_t * m_stNewBitmap = nullptr;
	Sage::stComponentBitmap_t * m_stNew = nullptr;
	Sage::stComponentBitmap_t * m_stInput = nullptr;
	Sage::stGraphicBitmap_t * m_stGraphicBitmap = nullptr;

	Sage::RawBitmap_t m_stBackground = {};
	Sage::RGBColor_t m_rgbBackgroundColor;
	wchar_t * m_sButtonText = nullptr;
	SIZE m_szText;
	HFONT m_hFont = (HFONT) nullptr;

//	Sage::RawBitmap_t	m_stTransparentPlate = {};
	int m_iBitmapSections = 4;		// For now: Normal, High, Pressed, Disabled -- will be adding Checked and Disabled Checked
	enum class ControlType
	{
		Button,
		Checkbox,
		ProgressBar,		// $$ Possible expansion
		Unknown,
	};
	ControlType m_eControlType = ControlType::Unknown;
	char * m_sCompSection = nullptr;		// Name of section in PGR file with button information
	bool ReplicateBitmapLineVert(Sage::RawBitmap_t & stSource,Sage::RawBitmap_t & stDest,int iStartX,int iLength);
	bool FillComponentSections(Sage::stComponentBitmap_t & stInput,Sage::stComponentBitmap_t & stNew,int iTextBlockWidth);
	bool FillCheckboxSections(Sage::stComponentBitmap_t & stInput,Sage::stComponentBitmap_t & stNew);
	bool CreateComponentSectionBitmaps(Sage::stComponentBitmap_t & stInput,Sage::stComponentBitmap_t & stNew);
	Sage::stComponentBitmap_t::stSection & GetSection(int iSection,Sage::stComponentBitmap_t & stInput);
	void RepairComponentBitmaps(Sage::stButtonBitmaps_t & stBitmap,Sage::stComponentBitmap_t & stComp);
	void RepairGraphicBitmaps(Sage::stButtonBitmaps_t & stBitmap,Sage::stGraphicButton_t & stButton);

	bool CreateTextButton(COutDC & cOutDC);
	bool CreateTextCheckbox(COutDC & cOutDC);
	bool CreateText(COutDC & cOutDC);
	void SetControlType(Sage::stComponentBitmap_t::ControlType eType);
	bool MergeComponentBitmap(Sage::RawBitmap_t & stBitmap,Sage::stComponentBitmap_t & stComp,Sage::stComponentBitmap_t::stSection & stSectionInput);

	enum class ShadowType
	{
		Normal,
		Pressed,
		High,
	};
	bool MergeGraphicBitmap(Sage::RawBitmap_t & stBitmap,Sage::stGraphicButton_t & stButton,Sage::RawBitmap_t & stInputBitmap,Sage::RawBitmap_t & stPlate,SIZE & szPlateOffset,ShadowType shadow);


	bool CreateComponentBitmaps(Sage::stButtonBitmaps_t & stBitmap,Sage::stComponentBitmap_t & stComp);
	bool CreateGraphicBitmaps(Sage::stButtonBitmaps_t & stBitmap,Sage::stGraphicButton_t & stButton,int iPlate = -1);
	bool CreateGraphicBitmaps(Sage::RawBitmap_t & stOut,Sage::stGraphicBitmap_t & stBitmap);

	//bool BuildTransparentPlate(HDC hSourceDC,int iX,int iY,stComponentBitmap_t & stComp);
//	bool ResetTransparentPlate() { m_stTransparentPlate.Delete(); return true; }
public:
	void SetDebug(bool bDebug) { m_bDebug = bDebug; }
	bool CreateComponentButton(CUserWindow * cWin,Sage::stComponentBitmap_t & stInput,Sage::stComponentBitmap_t & stNew,Sage::stButtonBitmaps_t & stNewBitmap,Sage::RGBColor_t & rgbBackground,
		Transparent eTransparent,wchar_t * sButtonText,HFONT hFont,bool bOverrideFont,char * sStyle,int iPaddingX,int iX,int iY,int iWidth = 0, int iHeight = 0);

	bool CreateGraphicButton(CUserWindow * cWin,Sage::stGraphicButton_t & stInput,Sage::stGraphicButton_t & stNew, Sage::stButtonBitmaps_t & stNewBitmap,Sage::RGBColor_t & rgbBackground,
			Transparent eTransparent,wchar_t * sButtonText,HFONT hFont,int iPlate,int iX,int iY);

	bool CreateComponentCheckbox(CUserWindow * cWin,Sage::stComponentBitmap_t & stInput,Sage::stComponentBitmap_t & stNew,Sage::stButtonBitmaps_t & stNewBitmap,Sage::RGBColor_t & rgbBackground,
		Transparent eTransparent,wchar_t * sButtonText,HFONT hFont,bool bOverrideFont,char * sStyle,SIZE szPadding,int iX,int iY,int iWidth = 0, int iHeight = 0);

	bool CreateGraphicBitmap(CUserWindow * cWin,Sage::stGraphicBitmap_t & stInput,Sage::RawBitmap_t & stOutput,int iX,int iY);



	bool CreateComponentButton(Sage::stComponentBitmap_t & stInput,Sage::stComponentBitmap_t & stNew,int iTextBlockWidth,int iStaticWidth = 0);
	bool CreateComponentCheckbox(Sage::stComponentBitmap_t & stInput,Sage::stComponentBitmap_t & stNew,SIZE szText,int iStaticWidth,int iStaticHeight);

	bool ReadComponentBitmapPGR(Sage::stComponentBitmap_t & stInput,char * sPath,char * sAltName = nullptr);
	bool ReadComponentBitmapPGR(Sage::stComponentBitmap_t & stInput,CSagePGR * cPGR,char * sAltName = nullptr);
	bool ReadGraphicButtonPGR(CPasWindow * cCore,Sage::stGraphicButton_t & stButton,CSagePGR * cPGR,char * sAltName = nullptr);
	bool ReadGraphicBitmapPGR(CPasWindow * cCore,Sage::stGraphicBitmap_t & stBitmap,CSagePGR * cPGR,char * sAltName = nullptr);
	bool CreateGraphicButton(const char * sStyleName,Sage::GraphicButtonStyle & stStyle,Sage::stGraphicButton_t & stButton);
	bool CreateGraphicCheckbox(const char * sStyleName,Sage::GraphicCheckboxStyle & stStyle,Sage::stGraphicButton_t & stButton);

	CButtonBuilder(CPasWindow * cMain) {m_cMain = cMain; };
	~CButtonBuilder()
	{
		
		m_stBackground.Delete();
	//	m_stTransparentPlate.Delete();
	}
};

}; // namespace Sage

#endif //_CButtonBuilder_H_