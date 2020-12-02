//#pragma once
#if !defined(_CTextWidget_H_)
#define _CTextWidget_H_
#include "CDavinci.h"

namespace Sage
{

class CDialog;

class CTextWidget : CWindow
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
	bool				m_bWriteShadow = false;
	static int			m_iRegistryValue;
	static CTextWidget	m_cEmpty;

	CString			* m_csString;
	CSageBitmap		* m_cParentBg		= nullptr;
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
					  
	int					m_iWinOffsetY		= 0;
	int					m_iWidth			= 0;
	int					m_iHeight			= 0;
	int					m_iX				= 0;
	int					m_iY				= 0;
	int					m_iOrgX				= 0;
	int					m_iOrgY				= 0;
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
	void Init(const char * sInitialText,const char * sControls);
	void GetBackground();
	bool CalcWindow();
	bool Redraw();
	void LocalWrite(const char * sText);
public:
	CTextWidget(CWindow * cParent,int iX,int iY,int iWidth,int iHeight,CString & cText); 
	CTextWidget(CWindow * cParent,int iX,int iY,int iWidth,int iHeight,CString & cText,cwfOpt & cwOpt); 
	CTextWidget(CWindow * cParent,int iX,int iY,int iWidth,int iHeight,CString & cText,const char * sControls); \
	~CTextWidget();
	//CTextWidget(CWindow * cParent,int iX,int iY,int iWidth,int iHeight,const char * sText=nullptr); 
	CTextWidget(CWindow * cParent,int iX,int iY,int iWidth,int iHeight,const char * sText,cwfOpt & cwOpt); 
	CTextWidget(CWindow * cParent,int iX,int iY,int iWidth,int iHeight,const char * sText=nullptr,const char * sControls = nullptr); 
	void Write(const char * sText,cwfOpt & cwOpt);
	void Write(const char * sText);
	void Write(CString & cString);
	__inline void Write(int iValue) { Write(CString() << iValue); }
	__inline void Write(double fValue) { Write(CString() << fValue); }

	void WriteShadow(const char * sText,cwfOpt & cwOpt);
	void WriteShadow(const char * sText);
	void WriteShadow(CString & cString);
	void Show(bool bShow = true);
	void Hide(bool bHide = true);
	void SetLocation(int iX,int iY);
	bool UpdateBg(bool bUpdate = true);
	void SetFont(char * sFont);
	void SetFont(HFONT hFont);
	void SetBgColor(Sage::RGBColor_t rgbColor);
	void SetFgColor(Sage::RGBColor_t rgbColor);
	void SetBgColor(DWORD dwColor);
	void SetFgColor(DWORD dwColor);
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
	Sage::CWidget * GetWidgetObj() { return m_cWidget; }
	static CTextWidget & GetEmptyObject() { return m_cEmpty; }
	// Basic functions that all Widgets need.  These are TBD and will probably
	// be moved to a separate class such as CWidget so processes can get the information
	// without explicitly knowing the type of control.
	//
	// Right now, the control type is needed.

	int GetID() { return m_cWin->GetID(); }
	const char * GetName() { return m_cWin->GetName(); }




};
}; // namespace Sage
#endif // _CTextWidget_H_
