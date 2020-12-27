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
#ifdef _LOCALTEST
#if !defined(_CLEDDisplayWidget_H_)
#define _CLEDDisplayWidget_H_


#include "CSageBox.h"


class CLCDDisplay1238Widget : protected CWindow
{
	SageControlDelete
public:
		enum class Mode
		{
			PlainLed = 0,
			BlueLed,
		};

		struct Led_t
		{
			int iWidth;
			int iHeight;
			CBitmap cBitmaps[13];
		};
private:
		enum LCDPLace
		{
			colon = 10,
			blank = 11,
			dash = 12,
		};	
		bool m_bInvalid = true; 

		// "hardware"-dependent values

		static constexpr int	kMaxValue = 999999;
		static constexpr int	kMinValue = 0;
private:
#define kMaxDigits 6
	int				  m_iNumModes = 2;	// $$ constexpr
	int				  m_iMaxDigits = kMaxDigits;	//  constexpr
	int				  m_iWindowX;
	int				  m_iWindowY;
	int				  m_iWinWidth;
	int				  m_iWinHeight;
	CWindow		* m_cParent = nullptr;
	CWindow		* m_cWin	= nullptr;
	bool			  m_bTransparent;
	bool			  m_bHidden;
	Mode			  m_eMode;
	Led_t			  m_stLeds[2] = {};
	Led_t			* m_stCurLed = nullptr;
	POINT			  m_pTextLoc;
	CBitmap		  m_cBmpEmulatorFace;
	CBitmap		  m_cBmpBackground;
	CBitmap		  m_cBmpOutput;	
	bool			  m_bFastMode;
	int				  m_iUpdateMS = 0;
	Sage::RGBColor_t  m_rgbBgColor;
	bool			  m_bAllowMove;
	int				  m_iValue = 0;
	int				  m_iMinValue;
	int				  m_iMaxValue;
	bool			  m_bInitialUpdate = false;
private:
	MsgStatus OnMove(int iX,int iY) override;
	MsgStatus OnWidgetMessage(void * cWidget,int iMessage);
private:
	void Init(CWindow * cParent, int iX,int iY,const char * sControls);
	void LocalSetDefaults();
	void ReadPGR();
	bool PrintValue();
	void FillBackground();
	bool Redraw(bool bForce = false);
	void CalcImage();
	void ReadOptions(const char * sOptions);
	bool DisplayBlank();
	void ForceRedraw();

public:
	CLCDDisplay1238Widget(CWindow * cParent, int iX,int iY, int iInitialValue, cwfOpt & cwOpt = cwfOpt());
	CLCDDisplay1238Widget(CWindow * cParent,POINT pLoc, int iInitialValue, cwfOpt & cwOpt = cwfOpt());
	virtual void SetDefaults()	{ }

	// Access Functions

	void SetFastMode(bool bFastMode);
	void SetLocation(int iX,int iY);	// Set the location of the window
	void AllowDrag(bool bAllowDrag);	// Allow movement by placing mouse on window and moving it. 
	void UpdateLast();					// Update last value (for last update when FastMode = true)
	int GetValue(); 
	bool SetValue(int iValue); 
	void UpdateBackground();
	void Show(bool bShow = true);
	void Hide(bool bHide = true);
	void SetMode(Mode mode);
	SIZE GetWindowSize();
	POINT GetWinLocation();
	SizeRect GetSizeRect(); 

	void debugSetUpdateMS(int iUpdateMS); 
};

#endif // _CLEDDisplayWidget_H_
#endif // _LOCALTEST