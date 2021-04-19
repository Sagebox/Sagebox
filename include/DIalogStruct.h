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
#if !defined(_DialogStruct_H_)
#define _DialogStruct_H_

#include <cfloat>
#include <cmath>
#include "CString.h"
namespace Sage
{

class CPasWindow;
enum class DialogJustify
	{
		Left,
		Right,
		Center,
		Top,
		Bottom,
	};

enum class DialogItem
		{
			H1,
			H2,
			H3,
			SubTitle,
			Label,
			Title,
			TitleIcon,
			EditBox,
			Button,
			AddSpace,
			Unknown,
		};

	 
class CButton;
class CWindow;
class CEditBox;

class DialogStruct
{
public:
	enum class TitleIconType
	{
		Info,
		Input,
		Question,
		Warning,
		Stop,
		User,
		None,
	};
	struct Title
	{
		SIZE szTextSize;
		POINT pPosition;
		Sage::SageString stTitle;
	public:
		void Delete()
		{
			stTitle.Delete();
		}
	};

	struct TitleIcon
	{
		TitleIconType				  eType;
		Sage::SageString			  stName;
		Sage::RawBitmap_t			* stBitmap;
		Sage::stGraphicBitmap_t	* stGraphicBitmap;
		int iWidth;
		int iHeight;
	public:
		void Delete()
		{
			stName.Delete();
			stBitmap = nullptr;
		}
	};

	enum class EditBoxType
	{
		String,
		Integer,
		Float,
	};
	struct EditBox
	{
		EditBoxType	eEditBoxType;
		Sage::SageString stDefaultText;
		Sage::SageString stOptString;
		Sage::SageString stName;
		Sage::SageString stText;
		CPasWindow	* m_cWinCore;
		int			m_iControl;
		int			bNoCancel;
		int			iMinRange;
		int			iMaxRange;
		double		fMinRange;
		double		fMaxRange;
		int			iDefault;
		double		fDefault;
		bool		bAllowBlankCR; 
		bool		bMinRangeSet;
		bool		bMaxRangeSet;
		bool		bDefaultSet;
		SIZE		szEditBoxSize;
		POINT		pPosition;
		CEditBox	* cEditBox;
	public:
		void Delete();
		void SetMinValue(int iValue,bool bSet = true);
		void SetMaxValue(int iValue,bool bSet = true);
		void SetDefault(int iValue,bool bSet = true);
		void SetMinValue(double fValue,bool bSet = true);
		void SetMaxValue(double fValue,bool bSet = true);
		void SetDefault(double fValue,bool bSet = true);
		void SetNoCancel(bool bValue);
	};

	enum class ButtonType
	{
		Ok,
		Cancel,
		Yes,
		No,
		Retry,
		Unknown,
	};
	struct Button
	{
		ButtonType	eButtonType;
		Sage::SageString stAltText; 
		CButton * cButton;
		CPasWindow	* m_cWinCore;
		int m_iControl;
		SIZE szButtonSize;
		POINT pPosition;
	public:
		void Delete();
	};

	// WindowSizes -- how many widths to try to get the best ratio, which is TBD

	int			m_iMaxWindowSizes = 4;
	int			m_iWindowSizes[4]; 
	SIZE		m_stWindowSizes[4];
	ButtonType	m_eButtonPress;
	double		m_fBestWidthRatio	=	4;	// i.e. W = 2*H. or 4/2, etc. ratio.  This may be TBD
	bool		m_bTitleIcon = false;
	int			m_iTitleIconWidth;
	int			m_iTitleIconHeight;
	int			m_iTitleIconMarginX = 3;	// $$ TBD

	struct AddSpace
	{
		int iPixelsX;
		int iPixelsY;
		int iPosY;
		Sage::SageString stName;
	public:
		void Delete() { stName.Delete(); }

	};
	struct Label
	{
		DialogItem				eItemType		;
		Sage::SageString		stLabelText		;
		DialogJustify			eJustify		;
		HFONT					hFont			;
		SIZE					szMetrics		;
		Sage::RGBColor_t		rgbText			;
		RECT					rDrawRect		;
		POINT					pDrawPoint		;
		int						iDrawHeight		;
	public:

		void Delete()
		{
			stLabelText.Delete();
		}
};
	int m_iNumItems     = 0;
	int m_iNumButtons   = 0;	    // Used, since buttons are only used at the bottom (as opposed to checkboxes, etc.)
						            // with specific types: Cancel, Ok, Yes, No, and a few others. 
						            // Different Wording can be used, such as CancelButton = "Abort", but the basic types are the above, 
						            // and all button presses will/should close the window. 
	int m_iNumEditBoxes = 0 ;
	struct Items_t
	{
			
		DialogItem eItemType;
		union Data_t
		{
			Label		stLabel;
			Button		stButton;
			EditBox		stEditBox;
			Title		stTitle;
			TitleIcon	stTitleIcon;
			AddSpace	stAddSpace;
		};

		Data_t stData;
	};

	const char * m_sEditBoxGroup = "CSageEditBox_EditboxGroup";
	Sage::SageString m_stBarHeader = {};	
	Sage::SageString m_stErrMessage = {};			  
	int			  m_iX					= 0;
	int			  m_iY					= 0;
	int			  m_iWidth				= 0;
	int			  m_iHeight				= 0;
	int			  m_iMaxWidth			= 0;
	int			  m_iMaxHeight			= 0;
				  
	int			  m_iTotalButtonWidth	= 0;
	int			  m_iButtonMargin		= 26;	// $$ TBD
	int			  m_iTextMarginX		= 10;
	Items_t		* m_stItems;
		
	Sage::RGBColor_t	  m_rgbBarColor;
	int					  m_iBarSize;
	HFONT				  m_hH1HeaderFont;
	HFONT				  m_hH2HeaderFont;
	HFONT				  m_hH3HeaderFont;
	HFONT				  m_hSubTitleFont;
	HFONT				  m_hTitleFont;
	TEXTMETRICA			  m_szH1HeaderMetric;
	TEXTMETRICA			  m_szH2HeaderMetric;
	int					  m_iHeaderMargin;
	int					  m_iDefaultButtonWidth			= 100;			// $$ TBD
	int					  m_iDefaultEditBoxWidth		= 250;			// $$ TBD
	HFONT				  m_hDefaultEditBoxFont			= {};
	int					  m_iH2HeaderMargin;
	int					  m_iCalcWindowHeight;
	Sage::RGBColor_t	  m_rgbBackground;
	Sage::RGBColor_t	  m_rgbBackground2;
						  
	Sage::RGBColor_t	  m_rgbHeader;
	Sage::RGBColor_t	  m_rgbH2Header;
	Sage::RGBColor_t	  m_rgbH3Header;
	Sage::RGBColor_t	  m_rgbSubTitle;
	Sage::RGBColor_t	  m_rgbTitle;

	int					  m_iControl = 0;
	CWindow			    * m_cWin		    = nullptr;
	CWindow			    * m_cParentWin	    = nullptr;
	CPasWindow			* m_cWinCore	    = nullptr;
	HDC					  m_hDC			    = nullptr;
	bool				  m_bModal		    = false;
	bool				  m_bClosing	    = false;
	bool				  m_bCenterWindow   = false;
public:
	void Init();
	ButtonType GetButtonPress();
	bool ButtonPressed(ButtonType eButton);
	CButton * GetButtonObject(ButtonType eButton);
	bool InitWindow(CWindow * cParentWin = nullptr,bool bModal = false);
	bool WaitforEvent();
	bool HideWindow();		
	void SetDefaults();
	void DeleteItems()
	{
		if (m_stItems)
		{
			for (int i=0;i<m_iNumItems;i++)
			{
				// Delete all data for eac structure type
				switch(m_stItems[i].eItemType)
				{
					case DialogItem::H1			: 
					case DialogItem::H2			: 
					case DialogItem::H3			: 
					case DialogItem::Label		: 
					case DialogItem::SubTitle	: m_stItems[i].stData.stLabel.Delete(); break;
					case DialogItem::Button		: m_stItems[i].stData.stButton.Delete(); break;
					case DialogItem::Title		: m_stItems[i].stData.stTitle.Delete(); break;
					case DialogItem::EditBox	: m_stItems[i].stData.stEditBox.Delete(); break;
					case DialogItem::TitleIcon	: m_stItems[i].stData.stTitleIcon.Delete(); break;
					case DialogItem::AddSpace	: m_stItems[i].stData.stAddSpace.Delete(); break;
				}
			}
			free(m_stItems);
		}
		m_stItems = nullptr;
		m_iNumItems = 0;
	}
	~DialogStruct();

	DialogStruct()
	{
		Init();
		SetDefaults();
	}

	bool AddItem(DialogItem eItemType,void * vpItem);
	char * GetText(Button stButton);
	POINT GetPos(const char * sName);
	DialogStruct::EditBox * GetEditBox(const char * sName);
	HFONT SetFont(Label & stLabel);
	Sage::RGBColor_t SetTextColor(Label & stLabel);
	bool AddButton(ButtonType eButtonType, const char * sAltText = nullptr);
	bool AddEditBox(EditBoxType eEditBoxType, const char * sName,const char * sDefaultText = nullptr,const char * sOptString = nullptr);
	bool ValidateEditBox(EditBox & stEditBox);
	bool AddLabel(DialogItem eItemType,const char * sLabel,DialogJustify eJustify);
	bool AddVerticalSpace(int iPixels,const char * sName = nullptr);
	bool AddTitleIcon(TitleIconType eType,const char * sUserName = nullptr);
	bool AddTitle(const char * sTitle);

	char * GetText(const char * sName);
	POINT GetPointX(HDC hdc,Label & stLabel,RECT & rRect,int iDialogY);
	bool CalcWindow();
	bool CalcButtons();
	bool DrawWindow(CPasWindow * cWinCore,bool bShow = true);
	bool Closing() { return m_bClosing; }
	void SetCenterWindow(bool bCenterWindow) { m_bCenterWindow = bCenterWindow; }
};

}; // namespae Sage
#endif // _DialogStruct_H_