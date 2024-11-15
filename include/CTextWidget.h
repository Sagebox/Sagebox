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
#if !defined(_CTextWidget_H_)
#define _CTextWidget_H_
#include "CDavinci.h"
#include "CWindow.h"

namespace Sage
{

class CDialog;

class CTextWidget : protected CWindow
{
	class CWidget : public Sage::CWidget
	{
		bool bTransparent = false;
	public:
		CTextWidget * m_cText;

		CWidget(CTextWidget * cText);

		CWindow * GetWindow()					override;
		bool SetLocation(int iX,int iY)			override;
		bool isValid()							override;
		bool Show(bool bShow)					override;
		bool Hide(bool bHide)					override;
		int GetID()								override;
		const char * GetName()					override;
		bool RecalcWindow()						override;
		bool UpdateBg(bool bUpdate = true)		override;
	};
friend CWindow;
friend CWidget;
friend CDavinci;

	SageControlDeletePrivate
private:

	
	enum Message
	{
		MsgUpdate,
	};

	enum class JustX
	{
		Left,
		Right,
		Center,
	};
	enum class JustY
	{
		Top,
		Bottom,
		Center,
	};

	enum class WinJustY
	{
		Top,
		Bottom,
		Center,
		None,
	};
	enum class WinJustX
	{
		Left,
		Right,
		Center,
		None,
	};
private:
    static constexpr    int kMaxPrintfStringSize = 2048;    // Max size of supported printf() strings.
    Mem<char>           m_sPrintfString;                    // Used for printf(); allocated on demand.
	bool				m_bWriteShadow = false;
	static int			m_iRegistryValue;
	static CTextWidget	m_cEmpty;

	CString			* m_csString;
	CBitmap		    * m_cParentBg		= nullptr;
	void			* m_pOrgObject		= nullptr;
	bool			  m_bTransparent	= false;
	WinJustX		  m_eWinJustX		= WinJustX::None;
	WinJustY		  m_eWinJustY		= WinJustY::None;
	JustX			  m_eJustX			= JustX::Left;
	JustY			  m_eJustY			= JustY::Center;
	int				  m_iOffsetY		= 0;
	CWindow			* m_cParent			= nullptr;
	CWindow			* m_cWin			= nullptr;
	CDavinci		* m_cDavinciMain	= nullptr;
	bool			  m_bValid			= false;
    double            m_fBlend          = 0;    // Blend percent of output -- 0 = transparent, 100 = fully opaque					  
	int					m_iWinOffsetY		= 0;
	int					m_iWidth			= 0;
	int					m_iHeight			= 0;
	int					m_iX				= 0;
	int					m_iY				= 0;
	int					m_iOrgX				= 0;
	int					m_iOrgY				= 0;
	int					m_iPadX				= 0;    // Amount to adjust the window placement left or right (-value for left)
	int					m_iPadY				= 0;    // Amount to adjust the window placement up or down (-value for up)
	int					m_iUpdateMS			= 0;
	bool				m_bHide				= false;
	bool				m_bUpdatesDisabled	= false;
	bool				m_bAllowMove		= false;
	Sage::RGBColor_t	m_rgbBgColor;
	Sage::RGBColor_t	m_rgbFgColor;

	CWidget				* m_cWidget = nullptr;

	static int Register(CDavinci & cSageBox);
private:
	MsgStatus OnWidgetMessage(void * cWidget,int iMessage);
	MsgStatus OnMove(int iX,int iY) override;
	void LocUpdate();
	void LocCls();
	void Init(const char * sInitialText,const kwOpt & keywords);
	void GetBackground();
	bool CalcWindow();
	bool Redraw();
	void LocalWrite(const char * sText);
public:
	CTextWidget(CWindow * cParent,int iX,int iY,int iWidth,int iHeight,CString & cText); 
	CTextWidget(CWindow * cParent,int iX,int iY,int iWidth,int iHeight,CString & cText,const kwOpt & keywords); 
//	CTextWidget(CWindow * cParent,int iX,int iY,int iWidth,int iHeight,CString & cText,const char * sControls); 
	~CTextWidget();
	//CTextWidget(CWindow * cParent,int iX,int iY,int iWidth,int iHeight,const char * sText=nullptr); 
	CTextWidget(CWindow * cParent,int iX,int iY,int iWidth,int iHeight,const char * sText = nullptr,const kwOpt & keywords = kw::none);  
//	CTextWidget(CWindow * cParent,int iX,int iY,int iWidth,int iHeight,const char * sText=nullptr,const char * sControls = nullptr); 
	void Write(const char * sText,const kwOpt & keywords);
	void Write(const char * sText);
	void Write(CString & cString);
	__inline void Write(int iValue) { Write(CString() << iValue); }
	__inline void Write(double fValue) { Write(CString() << fValue); }

	void WriteShadow(const char * sText,const kwOpt & keywords);
	void WriteShadow(const char * sText);
	void WriteShadow(CString & cString);
	void Show(bool bShow = true);
	void Hide(bool bHide = true);
	void SetLocation(int iX,int iY);

    // SetBlend() -- Sets the blending percent of the textwidget
    //
    // 0 = Fully Transparent, 100 = fully opaque
    //
    // note: This sets the blend value but does not update the text widget. 
    //       Updating the Text Widget must be done separately. 
    //
    bool SetBlend(double fBlendPercent); 
	bool UpdateBg(bool bUpdate = true);
	void SetFont(char * sFont);
	void SetFont(HFONT hFont);
	void SetBgColor(Sage::RGBColor_t rgbColor);
	void SetFgColor(Sage::RGBColor_t rgbColor);
	//void SetBgColor(DWORD dwColor);
	//void SetFgColor(DWORD dwColor);
	void SetFastMode(bool bFastMode=true);
	void EnableUpdate(bool bEnable);
	void DisableUpdate();
	void Update();
	void Delete();
	int EndY();
	int EndX();
	int Width();
	int Height();
	SIZE GetWindowSize(); 
    bool SetWindowSize(int iWidth,int iHeight);
    bool SetWindowSize(SIZE szSize);
    POINT GetLocation();
	Sage::CWidget * GetWidgetObj() { return m_cWidget; }
	static CTextWidget & GetEmptyObject() { return m_cEmpty; }

    /// <summary>
    /// Prints to the Text window in the same manner as printf(). 
    /// </summary>
    void printf(const char * Format,...);

    /// <summary>
    /// Prints to the Text window in the same manner as printf(). 
    /// <para></para>
    /// --> __printf() is the same as printf() and is currently used for test code.  Use printf() instead.
    /// </summary>
     void __printf(const char * Format,...) ;

	// Basic functions that all Widgets need.  These are TBD and will probably
	// be moved to a separate class such as CWidget so processes can get the information
	// without explicitly knowing the type of control.
	//
	// Right now, the control type is needed.

	int GetID();
	const char * GetName();

    // Overloads to allow things like MyTextWidget << "output string"

    CTextWidget & operator << (CString & cString);
    CTextWidget & operator << (const char * sMessage);
    CTextWidget & operator << (std::string sString);

    CTextWidget & operator << (char x)                ;
    CTextWidget & operator << (int x)                 ;
    CTextWidget & operator << (unsigned int x)        ;
    CTextWidget & operator << (float x)               ;
    CTextWidget & operator << (double x)              ;

};
}; // namespace Sage
#endif // _CTextWidget_H_
