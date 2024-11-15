// SageOpt.H -- SageProx Project.   Copyright (c) 2020 Rob Nelson, all Rights Reserved.  RobNelsonxx2@gmail.com

// **************************************************************************************************************
// Note: This file is part of the SageBox Alpha release and is under construction
// This file was part of the original code used for private and consulting projects.
//
// Many non-public interface declarations and older C++ version code will be removed in the next updates.
// **************************************************************************************************************

// --------------------------------------------------
// SageOpt.H -- cwfOpt and opt namespace declarations
// --------------------------------------------------
//
// The opt namespace is used for compiling options in the function call for many windows functions in SageBox.
// Formerly, string were used, and have been changed to cwfOpt(). 
//
// Many functions still accept strings, but are being moved to accept the cwfOpt() class only. 
//
// The idea is that there are many options for controls, but usualy the controls start off simply and grow through development.
// 
// For example, a TextWidget can be created as simple as TextWidget(100,200,"This is my Text"); 
//
// As the programming continues, then we want to set the foreground color, then possibly the font, transparency, and Drag ability.
//
// These can be added as they are encountered, such as 
//
//		TextWidget(100,200,"This is my Text",fgColor("red")) to set a text color (or, more properly: opt::fgColor("red"));
//
// Then the font can be changed
//
//		TextWidget(100,200,"This is my Text",fgColor("red") << Font(MyFont)) 
//
// the '|' is more commonly used in examples:
//
//		TextWidget(100,200,"This is my Text",fgColor("red") | Font(MyFont)) 
//
// To the final:
//
//		TextWidget(100,200,"This is my Text",fgColor("red") | Font(MyFont) | Transparent() | AllowDrag()) 
//
// Using this method prevents large amounts of code after declaration:
//
//		auto& widget = TextWidhet(100,200,"This is my Text");
//		widget.SetFgColor("red");
//		widget.SetTransparent();
//		widget.AllowDrag()
//		widget.SetFont(MyFont);
//		widget.show()
//
// all of which would be necessary for the same line:
//
//		TextWidget(100,200,"This is my Text",fgColor("red") | Font(MyFont) | Transparent() | AllowDrag()) 
//
// For widgets that do not support opt:: defines below, str() may be used, such as:
//
//		auto& MyWidget = SomeNewWidget(100,200,"This is a widget",str("WidgetOption=<someoption>"),
//
// to allow the Widget to parse an option while allowing the option to come through the original call.
// Widgets can also accept direct calls.  In these cases, the Widget or control should be initially hidden with opt::Hidden() 
// to avoid these changes showing on the screen, with Widget.Show() called once the changes are made.
//
// All opt:: types are commented and should show up in the intellisense for IDEs that support it.
//
// Using repeated options:
//
// Options can be assigned before a call for multiple controls with the same options.
//
// For example, a series of buttons with the same values:
//
//		cwfOpt MyOpt = fgColor("red") | Style("Panel") | Group(MyGroup);
//
//		CreateButton(100,200,"Button1",MyOpt);
//		CreateButton(100,240,"Button2",MyOpt);
//		CreateButton(100,280,"Button3",MyOpt);
//
// For buttons with differences, APPEND cwfOpt at the End.
// For efficiency, the cwOpt is changed as it is compiled.  Only the cwOpt() at the end is changed
//
// For example,
//
//		CreateButton100,320,"Button4",bgColor("green") | MyOpt) will use the options above and also add a green backround color.
//
// However,
//
//		CreateButton100,320,"Button4",MyOpt | bgColor("green")) will use the options above and also add a green backround color.
// 
// will accomplish the same goal, but corrup MyOpt by adding the green option to te MyOpt object.
//

//#pragma once
#if !defined(_SageOpt_h_)
#define _SageOpt_h_
#include <windows.h>
#include "ControlGroup.h"
#include "Sage.h"

// $$$$ NOTE $$$$$
//
// The defOpt approach is a workaround for MSVC es.84 warning (C26444) --- that you cannot get around even if you try to shut it off. 
// This is an error, contradicting valid C++ programming, and something Microsoft needs to fix at some point.
// 
// Normally, you can just ignore these warnings, but since the opt:: structure is uniqitous in SageBox, this causes green, squigly underlines whenever it is used.
//
// So, the fix is to have a large array of re-useable cwfOpt() objects that can be reused for the pre-defined types. 
//
// Since the use of pre-defined opt:: functions (i.e. fgColor(), Transparent(), etc.) is discreet, usually no more than 3-6 at a time, the 
// list is just recycled. 
//
// Though not how I would like to implment this; In the real world, an overlap would only occur with a runaway program where it wouldn't make a difference anyway.
//


// --------------------------------------
// This file is still under construction
// --------------------------------------

#define defOpt (*cwfOpt::Temp())
#define optRet cwfOpt &
namespace Sage
{

class cwfOpt
{
private:
	static constexpr int iMaxArray = 5000;	// Size of recycled array
	char sOpt[101];     // $$ This needs to be a constexpr throughout the code. 
	char * spOpt		= nullptr;
	int iLength			= 0;
	int ipLength		= 0;

	void AddOptString(const char * sOptString);
	static cwfOpt m_OptArray[iMaxArray];			// Global array of recycled cwfOpt() objects for pre-defined opt:: calls
	static int m_iMaxArray;
	static int m_iArrayIndex;
#ifdef _WIN64
	static volatile __int64 m_iProcessLock64;						// Lock used creating Temp() cwfOpt() objects due to Microsft Compiler Warning error
#else
	static int m_iProcessLock;						// Lock used creating Temp() cwfOpt() objects due to Microsft Compiler Warning error
#endif
	static void LockProcess();						// Used when allocating memory to allow multi-threading. 
	static void UnlockProcess(); 
	void InitTemp();
public:

	static cwfOpt * Temp();
	static cwfOpt m_cEmpty; 
	static cwfOpt * GetEmptyObj() { return &m_cEmpty; };	// This should only be used for default Empty on prototypes or in case of allocation error.
	const char * s();

	const char * GetString()const { return spOpt ? spOpt : &sOpt[0]; }
	void SetMemNull() { spOpt = nullptr; }
	optRet literal(const char * sString,const char * sDefintion = nullptr);
	optRet Bool(const char * sString,bool bValue = true);
	optRet Int(const char * sString,int iValue);
	optRet Long(const char * sString,long iValue);
	optRet Float(const char * sString,double fValue);
//	optRet StrStr(const char * sString,const char * sDefinition);
    optRet NoOpt();
	optRet Str(const char * sString,const char * sDefintion = nullptr);
	optRet S(const char * sString,const char * sDefintion = nullptr);
	optRet Name(const char * sName);
	optRet Border();
	optRet ThickBorder();
	optRet Recessed();
	optRet RecessedBorder();
	optRet Title(const char * sTitle);
//    optRet TitleFont(const char * sFont);

    optRet cbTitleCell(const char * sTitle);
	optRet Label(const char * sLabel,int just);
	optRet LabelX(const char * sLabel,int iPos);
	optRet LabelTop(const char * sLabel)	;
	optRet LabelBottom(const char * sLabel)	;
	optRet LabelLeft(const char * sLabel,int iSpacing)	;
	optRet LabelRight(const char * sLabel,int iSpacing)	;
	optRet LabelFont(const char * sFont);
	optRet LabelColor(const char * sFgColor);
	optRet LabelColor(DWORD dwFgColor);
	optRet LabelColor(Sage::RGBColor_t rgbColor);
	optRet SetIcon(CBitmap & cBitmap,int just);
    optRet SageIcon(int iIcon,int just); 
	optRet BorderColor(const char * sFgColor);
	optRet BorderColor(DWORD dwFgColor);
	optRet BorderColor(Sage::RGBColor_t rgbColor);

	optRet Style(const char * sStyle);
	optRet Style(const char * & sStyle,const char * & sOptions);
	optRet Font(const char * sFont);
	optRet FontHigh(const char * sFont);
	optRet FontChecked(const char * sFont);
	optRet ValueFont(const char * sFont);
	optRet Group(const char * sName);
	optRet Group_ID(const char * sName,int iControlID);
	optRet GroupID_ID(int iGroupID,int iControlID);
	optRet ColorsBW();
    optRet AsFloat();
	optRet Horz();
	optRet Vert();
	optRet Horizontal();
	optRet Vertical();
	optRet ShowValue();
	optRet WinTooltip();
	optRet EnableFocusBox()	;
	optRet Disabled()		;
	optRet Plate(const char * sPlate);
	optRet bgColor(const char * sBgColor);
	optRet bgColor(DWORD dwBgColor);
	optRet bgColor2(const char * sBgColor);
	optRet bgColor2(DWORD dwBgColor);
	optRet bgHigh(const char * sBgColor);
	optRet bgHigh(DWORD dwBgColor);
	optRet bgChecked(const char * sBgColor);
	optRet bgChecked(DWORD dwBgColor);
	optRet fgColor(const char * sFgColor);
	optRet fgColor(DWORD dwFgColor);
	optRet fgColor(Sage::RGBColor_t rgbColor);
	optRet fgHigh(const char * sFgColor);
	optRet fgHigh(DWORD dwFgColor);
	optRet fgHigh(Sage::RGBColor_t rgbColor);
	optRet fgChecked(const char * sFgColor);
	optRet fgChecked(DWORD dwFgColor);
	optRet fgChecked(Sage::RGBColor_t rgbColor);
	optRet TextColor(Sage::RGBColor_t rgbColor);
	optRet TextColor(const char * sTextColor);
	optRet TextColor(DWORD dwTextColor);
	optRet ValueColor(const char * sValueColor);
	optRet ValueColor(DWORD dwValueColor);
	optRet Color(const char * sColor);
	optRet Color(DWORD dwColor);
	optRet MinMax(double fMin,double fMax);
	optRet MinMax(float fMin,float fMax);
	optRet MinMax(int iMin,int iMax);
	optRet MinValue(int iValue);
	optRet MaxValue(int iValue);
	optRet MinValue(double fValue);
	optRet MaxValue(double fValue);
	optRet Default(int iValue);
	optRet Default(bool bValue);
	optRet Default(double fValue);
	optRet Default(const char * sString);
    optRet Checked(bool bChecked);
	optRet CharWidth(int iRange);
	optRet Width(int iWidth);
	optRet Height(int iHeight);
	optRet Rows(int iRows);
	optRet Columns(int iColumns);
	optRet AllowScroll();
	optRet WinColors(bool bValue);
	optRet VScroll(bool bValue);
	optRet HScroll(bool bValue);
	optRet MultiLine(bool bValue);
	optRet WantReturn(bool bValue);
	optRet Newline();
	optRet Newline(int iLines);
	optRet GroupID(int iGroupID);
	optRet ControlID(int iControlID);
	optRet UseLast();
	optRet UseEventThread();
    optRet Fullscreen();
	optRet IconInfo()		;
	optRet IconWarning()	;
	optRet IconStop()		;
	optRet IconNone()		;
	optRet NoCancel();
	optRet EmptyOk();
	optRet NoBlanks();
	optRet CancelOk();
	optRet Modal();
	optRet NoAutoUpdate();
	optRet DirectDraw();
	optRet RealTime();
    optRet QuickCpp();
    optRet DebugMode();
	optRet AutoUpdate(int iAutoUpdate);
	optRet CenterWin();
	optRet CenterDesktop();
	optRet SetTopmost();
	optRet HideCancel();
	optRet ProgressBar();
	optRet Transparent();
	optRet Transparent(int iBlendValue);
	optRet Blend(int iBlendValue);
	optRet Hidden();
	optRet NoAutoHide();
	optRet FastMode();
	optRet Center();
	optRet CenterX();
	optRet CenterX(int iRange);
	optRet CenterY();
	optRet CenterY(int iRange);
	optRet AddBorder();
    optRet NoResize();
    optRet ResizeOk();
    optRet Reversed();
	optRet InnerSize();
	optRet Resizeable();
	optRet SetSize(int iWidth,int iHeight);
	optRet MaxSize(int iWidth,int iHeight);
	optRet NoBorder();
	optRet NoScrollbar();
	optRet PadX(int iPad)		;
	optRet PadY(int iPad)		;
	optRet OffsetX(int iOffset)		;
	optRet OffsetY(int iOffset)		;


	optRet TextCenter();
	optRet TextCenter(int iRange);
	optRet TextCenterX();
	optRet TextCenterX(int iRange);
	optRet TextCenterY();
	optRet TextCenterY(int iRange);
	optRet TextCenterXY();
	optRet TextCenterXY(int iRangeX,int iRangeY);
	optRet TextRight();
	optRet TextLeft();
	optRet TextTop();
	optRet TextBottom();

    optRet Embed();
	optRet Popup();
	optRet NoClose();
    optRet NoSizing();
    optRet NoSysMenu();
	optRet AddShadow();
	optRet AllowDrag();

	optRet ReadOnly();
	optRet NumbersOnly();
	optRet FloatsOnly();

	optRet JustCenter();
	optRet JustCenterX();
	optRet JustCenterX(int iRange);
	optRet JustCenterY();
	optRet JustCenterY(int iRange);
	optRet JustRight();
	optRet JustLeft();
	optRet JustTopRight();
	optRet JustTopLeft();
	optRet JustBottomRight();
	optRet JustBottomLeft();
	optRet JustBottomCenter();
	optRet JustTopCenter();

	optRet JustRight(int iOffset);
	optRet JustTop();
	optRet JustBottom();

	optRet CenterXY(int iOffset=0);
	optRet CenterXY(int iRangeX,int iRangeY);
	optRet XPos(int iPos);
	optRet YPos(int iPos);
	optRet TabPos(int iPos);
	optRet Tab(int iPos);
	optRet GotoXY(int iX,int iY);
	optRet ValidateGroup(const char * sGroup);
	optRet ValidateGroup(ControlGroup & cGroup);
	optRet Group(ControlGroup & cGroup,int iID = INT_MAX);
	optRet _NonPrimary();

	cwfOpt & operator + (const cwfOpt & Opt) { AddOpt(Opt); return((cwfOpt &) *this); }
	cwfOpt & operator << (const cwfOpt & Opt) { AddOpt(Opt); return((cwfOpt &) *this); }
//	cwfOpt & operator << (const cwfOpt & Opt) { AddOpt(Opt); return((cwfOpt &) *this); }
	cwfOpt & operator | (const cwfOpt & Opt) { AddOpt(Opt); return((cwfOpt &) *this); }
	cwfOpt & operator , (const cwfOpt & Opt) { AddOpt(Opt); return((cwfOpt &) *this); }
	const char * operator * () const { return spOpt && *spOpt ? spOpt : (const char *) sOpt; }
	cwfOpt(const cwfOpt & opt2);
    cwfOpt & operator = (cwfOpt && p2) noexcept;
    cwfOpt(cwfOpt && p2) noexcept;

	inline bool Active() { return iLength > 0 || ipLength > 0; }
	void ClearMem() 
	{ 
		if (spOpt) free(spOpt);
		sOpt[0] = 0;;
		iLength = ipLength = 0;
		spOpt = nullptr;
	}
	~cwfOpt() { ClearMem(); }
	void AddOpt(const cwfOpt & wfOpt);
	cwfOpt() { sOpt[0] = 0; sOpt[1] = 0; spOpt = nullptr; ipLength=0; }
};

namespace opt
{
	// Sets a background gradient for window or control by specifying two colors: the top and bottom color. 
	// --> note: this is experimental and not supported in all controls or window types
	//
	static optRet bgGradient(const char * sColor1,const char * sColor2) { return defOpt.bgColor(sColor1) << defOpt.bgColor2(sColor2); }

	// --> note: this is experimental and not supported in all controls or window types
	//
	static optRet bgGradient(Sage::RGBColor_t rgbColor1,RGBColor_t rgbColor2) { return defOpt.bgColor(*rgbColor1) << defOpt.bgColor2(*rgbColor2); }

	//---> Set the background color for this control.  Enter any valid SageWin named color or RGB() value
	static optRet bgColor(DWORD dwColor) { return defOpt.bgColor(dwColor); }  ;	

	//---> Set the background color for this control.  Enter any valid SageWin named color or RGB() value
	static optRet bgColor(const char * sColor,const char * sColor2 = nullptr) 
    {
        if (!sColor2) return defOpt.bgColor(sColor); 
        return defOpt.bgColor(CString() << sColor << "," << sColor2); // Gradient
    }  ;	

	//---> Set the background color for this control.  Enter any valid SageWin named color or RGB() value
	static optRet bgColor(Sage::RGBColor_t rgbColor,Sage::RGBColor_t rgbColor2 = Rgb::Undefined) 
    { 
        if (rgbColor2.Undefined()) return defOpt.bgColor(*rgbColor); 
        return defOpt.bgColor(*rgbColor) << defOpt.bgColor2(*rgbColor2);        // Gradient
    };	

	// --> Sets the background color for controls like Buttons (i.e. Style("Window") or Style("Panel") when the mouse is over the control.
	// For example, with Windows-style or Panel-Style buttons, this will cause the color of the button to change to the bgHColor() specified
	// as the mouse is moved over the control and the button is pressed.
	// When the mouse moves out of the button, the normal background color is restored.
	//
	static optRet bgHigh(const char * sColor) { return defOpt.bgHigh(sColor); }  ;	

	// --> Sets the background color for controls like Buttons (i.e. Style("Window") or Style("Panel") when the mouse is over the control.
	// For example, with Windows-style or Panel-Style buttons, this will cause the color of the button to change to the bgHColor() specified
	// as the mouse is moved over the control and the button is pressed.
	// When the mouse moves out of the button, the normal background color is restored.
	//
	static optRet bgHigh(Sage::RGBColor_t rgbColor) { return defOpt.bgHigh(*rgbColor); }  ;	

	// Set the background color of a "checked" background (used for Panel Type checkboxes and radioboxes)
	// Since checkmark doesn't appear, and sometimes there is no border, the color can be an indication that it is a
	// selected item (or, in the case of a Radio Button, the selected item)
	//
	static optRet bgChecked(const char * sColor) { return defOpt.bgChecked(sColor); }  ;	

	// Set the background color of a "checked" background (used for Panel Type checkboxes and radioboxes)
	// Since checkmark doesn't appear, and sometimes there is no border, the color can be an indication that it is a
	// selected item (or, in the case of a Radio Button, the selected item)
	//
	static optRet bgChecked(Sage::RGBColor_t rgbColor) { return defOpt.bgChecked(*rgbColor); }  ;	

	//-----> Set the text color the control, Widget, or Window. Enter any valid SageWin named color or RGB() value
	//
	// Examples:
	//
	// --> fgColor("yellow")
	// --> fgColor("MyRed")				(where MyRed was made by calling MakeColor("MyRed",<<color>>), i.e. MakeColor("MyRed",RGB(255,50,50));
	// --> fgColor(MyColor)				(where DWORD MyColor), i.e. DWORD MyColor = RGB(255,50,50);
	// --> fgColor(RGB(255,50,50));
	// --> fgColor(*rgbMyColor)			(where rgbColor_t MyColor)		// i.e. MyColor = { 255,50,50 };
	//
	static optRet fgColor(const char * sColor) { return defOpt.fgColor(sColor); }  ;	
	//-----> Set the text color the control, Widget, or Window. Enter any valid SageWin named color or RGB() value
	//
	// Examples:
	//
	// --> fgColor("yellow")
	// --> fgColor("MyRed")				(where MyRed was made by calling MakeColor("MyRed",<<color>>), i.e. MakeColor("MyRed",RGB(255,50,50));
	// --> fgColor(MyColor)				(where DWORD MyColor), i.e. DWORD MyColor = RGB(255,50,50);
	// --> fgColor(RGB(255,50,50));
	// --> fgColor(*rgbMyColor)			(where rgbColor_t MyColor)		// i.e. MyColor = { 255,50,50 };
	//
	static optRet fgColor(DWORD dwColor) { return defOpt.fgColor(dwColor); }  ;	
	//-----> Set the text color the control, Widget, or Window. Enter any valid SageWin named color or RGB() value
	//
	// Examples:
	//
	// --> fgColor("yellow")
	// --> fgColor("MyRed")				(where MyRed was made by calling MakeColor("MyRed",<<color>>), i.e. MakeColor("MyRed",RGB(255,50,50));
	// --> fgColor(MyColor)				(where DWORD MyColor), i.e. DWORD MyColor = RGB(255,50,50);
	// --> fgColor(RGB(255,50,50));
	// --> fgColor(*rgbMyColor)			(where rgbColor_t MyColor)		// i.e. MyColor = { 255,50,50 };
	//
	static optRet fgColor(Sage::RGBColor_t rgbColor) { return defOpt.fgColor(*rgbColor); }  ;	

	// Set the foreground (Text) color for the control.  This is usually used with Panel Type checkboxes and radiobuttons,
	// But can be used for any control that accepts it.
	//
	static optRet fgHigh(const char * sColor) { return defOpt.fgHigh(sColor); }  ;	

	// Set the foreground (Text) color for the control.  This is usually used with Panel Type checkboxes and radiobuttons,
	// But can be used for any control that accepts it.
	//
	static optRet fgHigh(DWORD dwColor) { return defOpt.fgHigh(dwColor); }  ;	

	// Set the foreground (Text) color for the control.  This is usually used with Panel Type checkboxes and radiobuttons,
	// But can be used for any control that accepts it.
	//
	static optRet fgHigh(Sage::RGBColor_t rgbColor) { return defOpt.fgHigh(*rgbColor); }  ;	

	// Set the foreground (Text) color for the slected checkbox or radiobox (i.e. "checked" status).
	//
	// Since checkmark doesn't appear, and sometimes there is no border, the color can be an indication that it is a
	// selected item (or, in the case of a Radio Button, the selected item)
	//
	static optRet fgChecked(DWORD dwColor) { return defOpt.fgChecked(dwColor); }  ;	

	// Set the foreground (Text) color for the slected checkbox or radiobox (i.e. "checked" status).
	//
	// Since checkmark doesn't appear, and sometimes there is no border, the color can be an indication that it is a
	// selected item (or, in the case of a Radio Button, the selected item)
	//
	static optRet fgChecked(Sage::RGBColor_t rgbColor) { return defOpt.fgChecked(*rgbColor); }  ;	

	// Set the TextColor of a control.  This is the same as fgColor()
	// 
	// See fgColor() for more information.  Text-based, RGB, and RGBColor_t, and variables may be used to set the color
	//
	static optRet TextColor(const char * sColor) { return defOpt.fgColor(sColor); }  ;	

	// Set the TextColor of a control.  This is the same as fgColor()
	// 
	// See fgColor() for more information.  Text-based, RGB, and RGBColor_t, and variables may be used to set the color
	//
	static optRet TextColor(DWORD dwColor) { return defOpt.fgColor(dwColor); }  ;	

    // Set the TextColor of a control.  This is the same as fgColor()
	// 
	// See fgColor() for more information.  Text-based, RGB, and RGBColor_t, and variables may be used to set the color
	//
	static optRet TextColor(Sage::RGBColor_t rgbColor) { return defOpt.fgColor(*rgbColor); }  ;	

	// Set the color for the Value display in a slider.  The option ShowValue() must be used to enable the value to display.
	//
	static optRet ValueColor(const char * sColor) { return defOpt.ValueColor(sColor); }  ;	

	// Set the color for the Value display in a slider.  The option ShowValue() must be used to enable the value to display.
	//
	static optRet ValueColor(DWORD dwColor) { return defOpt.ValueColor(dwColor); }  ;	

	// This is probably deprecated, but usually sets the Backround color of a control
	// Expect it to disappear and to be replaced with bgColor() at some point.
	//
	static optRet Color(const char * sColor) { return defOpt.Color(sColor); }  ;	
	// This is probably deprecated, but usually sets the Backround color of a control
	// Expect it to disappear and to be replaced with bgColor() at some point.
	//

	static optRet Color(DWORD dwColor) { return defOpt.Color(dwColor); }  ;	
	// Set a Minimum value allowed (for integer or floating-point).  A dialog box appears with
	// min/max info if a value outisde of the range is entered.
	//
	// Example:  GetGloat(fDefault,MinValue(0)); 
	//
	static optRet MinValue(int iValue) { return defOpt.MinValue(iValue); }  ;		

    // Set a Minimum value allowed (for integer or floating-point).  A dialog box appears with
	// min/max info if a value outisde of the range is entered.
	//
	// Example:  GetGloat(fDefault,MinValue(0)); 
	//
	static optRet MinValue(bool bValue) { return defOpt.MinValue(bValue ? 1 : 0); }  ;		

	// Set a Minimum value allowed (for integer or floating-point).  A dialog box appears with
	// min/max info if a value outisde of the range is entered.
	//
	// Example:  GetGloat(fDefault,MinValue(0)); 
	//
	static optRet MinValue(double fValue) { return defOpt.MinValue(fValue); }  ;		
	
	// Set the Default value for a control.  This is usually used for getting values such as integers, floats, or
	// strings.
	//
	// Example: GetString(MyString,opt::Default("This is the default")
	// GetFloat(MyFloat,Default(1.5))
	//
	// Typically, the default can be set in the call itself without using an option.
	//
	static optRet Default(int iDefault) { return defOpt.Default(iDefault); }  ;	

	// Set the Default value for a control.  This is usually used for getting values such as integers, floats, or
	// strings.
	//
	// Example: GetString(MyString,opt::Default("This is the default")
	// GetFloat(MyFloat,Default(1.5))
	//
	// Typically, the default can be set in the call itself without using an option.
	//
	static optRet Default(double fDefault) { return defOpt.Default(fDefault); }  ;	

	// Set the Default value for a control.  This is usually used for getting values such as integers, floats, or
	// strings.
	//
	// Example: GetString(MyString,opt::Default("This is the default")
	// GetFloat(MyFloat,Default(1.5))
	//
	// Typically, the default can be set in the call itself without using an option.
	//
	static optRet Default(const char * sString) { return defOpt.Default(sString); }  ;	

    /// <summary>
    /// When creating a checkbox, Checked() (or Checked(true)) will set the checkbox as initially checked. 
    ///<para></para>
    /// Checked(false) sets the checkbox as unchecked -- howver, this is not needed since the checkbox defaults to unchecked.
    /// </summary>
    /// <param name="bChecked"></param>
    /// <returns></returns>
    static optRet Checked(bool bChecked = true) { return defOpt.Checked(bChecked); }

	// Set a Maximum value allowed (for integer or floating-point).  A dialog box appears with
	// min/max info if a value outisde of the range is entered.
	//
	// Example:  GetGloat(fDefault,MaxValue(100)); 
	//
	static optRet MaxValue(int iValue) { return defOpt.MaxValue(iValue); }  ;	

	// Set a Maximum value allowed (for integer or floating-point).  A dialog box appears with
	// min/max info if a value outisde of the range is entered.
	//
	// Example:  GetGloat(fDefault,MaxValue(100)); 
	//
	static optRet MaxValue(double fValue) { return defOpt.MaxValue(fValue); }  ;	

	// Set the name of the control.  This can be used to find the control later, without saving the 
	// returned object pointer.
	//
	// ID() can be used for the same result and is more efficient, though name() can be easier to remember,
	// such as "MyButton" or "RangeSlider"
	//
	// Example:
	//
	// CreateButton(10,10,"Ok",Name("MyButton");
	//
	// --> elsehwere:
	//
	// auto& MyButton = cWin.button("MyButton");
	// 
	// also, button("MyButton").Pressed() (i.e. any function) can be used for quick access, though it is inefficient.
	//
	static optRet Name(const char * sName) { return defOpt.Name(sName); }  ;

	// Adds a border to the control, such as a Window or Input Control (i.e. string, integer, etc.). 
	// This is deprecated and will be changed to AddBorder()
	//
	static optRet Border() { return defOpt.Border(); }  ;	

	// Disable the control or window from being resized by the user by grabbing the edges of the window/control.
	// Windows and some controls are resizeable by default.   Using NoResize() disables this when using the mouse. 
    // The Window or Control can still be resized programmatically via the SetWindowSize() function for the specific window/control.
	//
	static optRet NoResize() { return defOpt.NoResize(); }  ;	

	// Disable the control or window from being resized by the user by grabbing the edges of the window/control.
	// Windows and some controls are resizeable by default.   Using NoResize() disables this when using the mouse. 
    // The Window or Control can still be resized programmatically via the SetWindowSize() function for the specific window/control.
	//
	static optRet ResizeOk() { return defOpt.ResizeOk(); }  ;	

	// This will reverse the bitmap (i.e. display it upside-down) in functions, widgets, and created windows that display a bitmap as part 
    // of its initialization.   
    // 
	// Windows bitmap standard is to bring in the bitmap upside-down, which causes Sagebox to reverse the bitmap to show it right-side-up.
    // for bitmaps than come in right-side-up, adding the Reversed() option will cause Sagebox to display the bitmap correctly.
    //
    // With most functions, you can attach an R on the end to do this automatically, i.e. DisplayBitmap() vs. DisplayBitmapR()
    //
	static optRet Reversed() { return defOpt.Reversed(); }  ;	

	// Adds a thick border to the control, such as a Window or Input Control (i.e. string, integer, etc.). 
	// This is deprecated and will be changed to AddBorder()
	//
	static optRet ThickBorder() { return defOpt.ThickBorder(); }  ;	

	// Adds a recessed border to the control, such as a Window or Input Control (i.e. string, integer, etc.). 
	// This is deprecated and will be changed to AddBorder()
	//
    // Note: Recessed() and RecessedBorder() are the same function.  Recessed() is provided as a shorter option name.
	static optRet Recessed() { return defOpt.Recessed(); }  ;	

	// Adds a recessed border to the control, such as a Window or Input Control (i.e. string, integer, etc.). 
	// This is deprecated and will be changed to AddBorder()
	//
    // Note: Recessed() and RecessedBorder() are the same function.  Recessed() is provided as a shorter option name.
	static optRet RecessedBorder() { return defOpt.Recessed(); }  ;	

	// Adds a title to a control, such as a slider, input dialog title bard, and other controls.
	//
	// Without a title, many controls/dialog show the ProgramName() in the title bar of the program name is set.
	// Otherwise, this value is left blank or set to a default that is descriptive of the control.
	//
	// Title() and Label() are the same.
	//
	static optRet Title(const char * sTitle) { return defOpt.Title(sTitle); }  ;	

	static optRet cbTitleCell(const char * sTitle) { return defOpt.cbTitleCell(sTitle); }  ;	

	// Adds a title to a control, such as a slider, input dialog title bard, and other controls.
	//
	// Without a title, many controls/dialog show the ProgramName() in the title bar of the program name is set.
	// Otherwise, this value is left blank or set to a default that is descriptive of the control.
	//
	// Title() and Label() are the same.
	//
	static optRet Label(const char * sLabel,LabelJust just = LabelJust::Default)			{ return defOpt.Label(sLabel,(int) just);			}  ;	
	static optRet LabelX(const char * sLabel,int iPos)	{ return defOpt.LabelX(sLabel,iPos);		}  ;	
	static optRet LabelTop(const char * sLabel)			{ return defOpt.LabelTop(sLabel);		}  ;	
	static optRet LabelBottom(const char * sLabel)		{ return defOpt.LabelBottom(sLabel);	}  ;	
	static optRet LabelLeft(const char * sLabel,int iSpacing = 0)		{ return defOpt.LabelLeft(sLabel,iSpacing);		}  ;	
	static optRet LabelRight(const char * sLabel,int iSpacing =0 )		{ return defOpt.LabelRight(sLabel,iSpacing);		}  ;	
	static optRet LabelFont(const char * sFont)			{ return defOpt.LabelFont(sFont);			}  ;	
	static optRet LabelFont(int iFontSize) { char sTemp[20]; itoadec(iFontSize > 0 ? iFontSize : 2,sTemp); return defOpt.LabelFont(sTemp); }  ;

	static optRet SetIcon(CBitmap & cBitmap,TextJust just = TextJust::Default)			{ return defOpt.SetIcon(cBitmap,(int) just);			}  ;	

//	static optRet TitleFont(const char * sFont)			{ return defOpt.TitleFont(sFont);			}  ;	
//	static optRet TitleFont(int iFontSize) { char sTemp[20]; itoadec(iFontSize > 0 ? iFontSize : 2,sTemp); return defOpt.TitleFont(sTemp); }  ;

    // Sets Sagebox icon rather than a specified bitmap for various demos and displays. 
    //
    static optRet SageIcon(Sage::SageIconType Icon = Sage::SageIconType::Green32,TextJust just = TextJust::Default) { return defOpt.SageIcon((int) Icon,(int) just); } 

	static optRet LabelColor(const char * sColor) { return defOpt.LabelColor(sColor); }  ;	
	static optRet LabelColor(DWORD dwColor) { return defOpt.LabelColor(dwColor); }  ;	
	static optRet LabelColor(Sage::RGBColor_t rgbColor) { return defOpt.LabelColor(*rgbColor); }  ;	

	static optRet BorderColor(const char * sColor) { return defOpt.BorderColor(sColor); }  ;	
	static optRet BorderColor(DWORD dwColor) { return defOpt.BorderColor(dwColor); }  ;	
	static optRet BorderColor(Sage::RGBColor_t rgbColor) { return defOpt.BorderColor(*rgbColor); }  ;	

	// Sets a Style of a button, slider, or other control.
	// Styles can be pre-defined styles or ones created through loading graphics and setting the style with
	// SageBox, such as SetButtonStyle()
	//
	// Examples:
	//
	// CreateButton(50,50,"Ok",Style("medium")  -- sets a larger button
	// CreateButton(50,50,"Ok",Style("medium:red")  -- sets a larger button in red (where gray, blue, red, light gray, dark, are pre-defined styles)
	// CreateButton(50,50,"Ok",Style("red")  -- sets a default button in red (where gray, blue, red, light gray, dark, are pre-defined styles)
	// CreateButton(50,50,"Ok",Style("Panel")  -- sets a Panel button
	// CreateButton(50,50,"Ok",Style("Windows")  -- sets a Windows-style button
	// CreateButton(50,50,"Ok",Style("MyStyle") -- uses a style registered with SageBox
	// CreateRadioButton(50,50m,"MyStyle") -- Also uses a style registered with Sagebox when a button label is not required.
	//
	static optRet Style(const char * sStyle) { return defOpt.Style(sStyle); }  ;	

	// Sets a Style of a button, slider, or other control.
	// Styles can be pre-defined styles or ones created through loading graphics and setting the style with
	// SageBox, such as SetButtonStyle()
	//
	// Examples:
	//
	// CreateButton(50,50,"Ok",Style("medium")  -- sets a larger button
	// CreateButton(50,50,"Ok",Style("medium:red")  -- sets a larger button in red (where gray, blue, red, light gray, dark, are pre-defined styles)
	// CreateButton(50,50,"Ok",Style("red")  -- sets a default button in red (where gray, blue, red, light gray, dark, are pre-defined styles)
	// CreateButton(50,50,"Ok",Style("Panel")  -- sets a Panel button
	// CreateButton(50,50,"Ok",Style("Windows")  -- sets a Windows-style button
	// CreateButton(50,50,"Ok",Style("MyStyle") -- uses a style registered with SageBox
	// CreateRadioButton(50,50m,"MyStyle") -- Also uses a style registered with Sagebox when a button label is not required.
	//
	static optRet Style(const char * sStyle,const char * sOptions) { return defOpt.Style(sStyle,sOptions); }  ;	

	// Font() -- Set the Text Font for the current Control, Widget, or Window
	// When a plain integer is used, i.e. Font(15), it is set to the default font with a size of the value, i.e. "Arial,15"
	//
	// ---> Form Examples: 
	// --->    Font("Arial,20")  -- Set the font to "Arial,20".  You can also add italic an bold, i.e. Font("Arial,20,bold,italic");
	// --->    Font("MyFont")    -- Set the font by a previously created font named "MyFont"
	// --->    Font(MyFont)      -- Set the font with a standard Windows HFONT returned by CreateFont(), GetFont(), or SetFont().
	// 
	static optRet Font(const char * sFont) { return defOpt.Font(sFont); }  ;

    // Font() -- Set the Text Font for the current Control, Widget, or Window.  
	// When a plain integer is used, i.e. Font(15), it is set to the default font with a size of the value, i.e. "Arial,15"
    //
	// ---> Form Examples: 
	// --->    Font("Arial,20")  -- Set the font to "Arial,20".  You can also add italic an bold, i.e. Font("Arial,20,bold,italic");
	// --->    Font("MyFont")    -- Set the font by a previously created font named "MyFont"
	// --->    Font(MyFont)      -- Set the font with a standard Windows HFONT returned by CreateFont(), GetFont(), or SetFont().
	// 
	static optRet Font(int iFontSize) { char sTemp[20]; itoadec(iFontSize > 0 ? iFontSize : 2,sTemp); return defOpt.Font(sTemp); }  ;

	// Sets the Font for the highlighted button, checkbox or radiobox -- used for Panel Type buttons.
	// See Font() for more information how to set the Font.  Example: FontHigh("Arial,40") or FontHigh(MyFont)
	//
	static optRet FontHigh(const char * sFont) { return defOpt.FontHigh(sFont); }  ;	

	// Sets the Font for the checked button, checkbox or radiobox -- used for Panel Type buttons.
	// See Font() for more information how to set the Font.  Example: FontHigh("Arial,40") or FontHigh(MyFont)
	//
	static optRet FontChecked(const char * sFont) { return defOpt.FontChecked(sFont); }  ;	

	// This is deprecated
	static optRet ValueFont(const char * sFont) { return defOpt.ValueFont(sFont); }  ;	

	// A little TBD still yet.  This sets a plate on a pre-defined Graphic Button or Checkbox style, 
	// allowing a general graphic, shadow, glow, etc. for the button with different plate for the button interior.
	// These are pre-defined by the style and must be known.
	//
	// Example: CreateButton(50,100,"MyButtonStyle",Plate("GreenLight") -- for a "MyButtonStyle" with a plate named "GreenLight");
	//
	static optRet Plate(const char * sPlate) { return defOpt.Plate(sPlate); }  ;	

	// Set the width of an input control in terms of average character width (i.e. CharWidth)
	// In an input, this can set the range of the input.  For example., CharWidth(50) sets the size of the input box
	// to the size of 50 average characters.  
	// 
	// Since fonts are proportional, this is only an estimate.
	// Width() can also be used to set a specific width.
	//
	static optRet CharWidth(int iCharWidth) { return defOpt.CharWidth(iCharWidth); }  ;		

	// Set the width (in pixels) of a control or input box.
	// This sets the width of the input box of a control such as GetString(). 
	//
	static optRet Width(int iWidth) { return defOpt.Width(iWidth); }  ;	

	/// <summary>
	/// 
	/// </summary>
	/// <param name="iHeight"></param>
	/// <returns></returns>
	static optRet Height(int iHeight) { return defOpt.Height(iHeight); }  ;		

	/// <summary>
	/// 
	/// </summary>
	/// <param name="iRows"></param>
	/// <returns></returns>
	static optRet Rows(int iRows) { return defOpt.Rows(iRows); }  ;		

	/// <summary>
	/// 
	/// </summary>
	/// <param name="iColumns"></param>
	/// <returns></returns>
	static optRet Columns(int iColumns) { return defOpt.Columns(iColumns); }  ;		

	// Allows scrolling in an input box.  This allows the text to roll in the box when the box is smaller than the input
	// text.  Otherwise, the text is lmited to the size of the input box.
	//
	static optRet AllowScroll() { return defOpt.AllowScroll(); }  ;

	// Sets the Windows colors for the control.
	// $$ This conrol is a little TBD.  In some controls, it sets the color of the current window (such as with input boxes, i.e.
	// console.GetString(), console.GetInteger(), etc.), and in others, it sets the standard Windows Colors, such as with
	// CreateButton() and CreateSlider()
	//
	// See the individual control (or just try it) for more information
	// This will be addressed in a future update. 
	//
	static optRet WinColors(bool bWinColors = true) { return defOpt.WinColors(bWinColors); }  ;

	/// <summary>
	/// Sets a vertical scroll bar on an applicable control (i.e. an edit box)
	/// </summary>
    static optRet VScroll(bool bScroll = true)          { return defOpt.VScroll(bScroll); };

	/// <summary>
	/// Sets a horizontal scroll bar on an applicable control (i.e. an edit box)
	/// </summary>
	static optRet HScroll(bool bScroll = true)          { return defOpt.HScroll(bScroll); };

	/// <summary>
	/// Sets an edit box to accept multiple lines (the defualt is one line).
    /// <para></para>
    /// When multiple lines are used, a button or some other method must be used to determine when the user has signaled sending (or finishing) with the edit box.
	/// </summary>
	/// <returns></returns>
	static optRet MultiLine(bool bMultiLine = true)     { return defOpt.MultiLine(bMultiLine); };

	/// <summary>
	/// In a multi-line edit box (where the "MultiLine" option has been used), this tells the edit box to send a PressReturned() message when the 
    /// Return Key is pressed.  Othewise, the multi-line edit box will accept the return key as input and start a new line in the edit box.
	/// </summary>
	/// <param name="bWantReturn"></param>
	/// <returns></returns>
	static optRet WantReturn(bool bWantReturn = true)   { return defOpt.WantReturn(bWantReturn); };


	// This is deprecated and will be removed.
	static optRet Newline() { return defOpt.Newline(); }  ;
	// This is deprecated and will be removed.
	static optRet Newline(int iLines) { return defOpt.Newline(iLines); }  ;

	// Can be used in a console input function (i.e. console.GetInput(), console.getFloat(), etc.) to set the position of the 
	// input box.  This allows the boxes to be easily lined up with text to the left and aligned boxes to the right.
	//
	static optRet XPos(int iPos) { return defOpt.XPos(iPos); }  ;		

    // Can be used in a console input function (i.e. console.GetInput(), console.getFloat(), etc.) to set the position of the 
	// input box.  This allows the boxes to be easily lined up with text to the left and aligned boxes to the right.
	//
	static optRet YPos(int iPos) { return defOpt.YPos(iPos); }  ;		

	// Can be used in a console input function (i.e. console.GetInput(), console.getFloat(), etc.) to set the position of the 
	// input box.  TabPos is sets (aerage character width of font)*size entered in the TabPos.  For example, TabPos(50)
	// Sets the X position at X = 50 aerage characters for the font.
	// 
	// This allows the boxes to be easily lined up with text to the left and aligned boxes to the right.
	//
	static optRet TabPos(int iPos) { return defOpt.TabPos(iPos); }  ;	

	// Not sure what this does -- it's probably deprecated
	//
	static optRet Tab(int iPos) { return defOpt.Tab(iPos); }  ;		

	// Sets the (X,Y) position for functions like Write() prior to writing.
	// 
	// Example, out << GotoXY(100,200) << "Hello World"
	// Writes "Hello World" at position (100,200);
	// Write("Hello World",GotoXY(100,200)) does the same thing
	//
	static optRet GotoXY(int iX,int iY) { return defOpt.GotoXY(iX,iY); }  ;		

	/// <summary>
	/// Sets the location of a window when creating a window or new control.
    /// <para></para>
    /// --> Note this function is in-progress and may not be supported on all functions.
    /// --> Most functions allow setting the Window location in the main function call. 
    /// <para></para>&#160;&#160;&#160;
    /// WinLoc() is the equivalent of XYPos()
	/// </summary>
	/// <param name="iX">X Location of the Window or Control</param>
	/// <param name="iY">Y Location of the Window or Control</param>
	/// <returns></returns>
	static optRet WinLoc(int iX,int iY) { return defOpt.GotoXY(iX,iY); }  ;	

	/// <summary>
	/// Sets the location of a window when creating a window or new control.
    /// <para></para>
    /// --> Note this function is in-progress and may not be supported on all functions.
    /// --> Most functions allow setting the Window location in the main function call. 
    /// <para></para>&#160;&#160;&#160;
    /// WinLoc() is the equivalent of XYPos()
	/// </summary>
	/// <param name="iX">X Location of the Window or Control</param>
	/// <param name="iY">Y Location of the Window or Control</param>
	/// <returns></returns>
	static optRet XYPos(int iX,int iY) { return defOpt.GotoXY(iX,iY); }  ;		

	// Sets the range of an input box for integers or floats, to the range specified.
	// If the range is exceeded, an error message comes up and prompts the user to enter a value in the range.
	// 
	// Example: GetFloat(Range(-5,5)) -- gets a floating point value in the range of -5 to 5.
	// Use Min() or Max() for ranges on the end, such as GetFloat(Min(0)) to prohibit negative values
	// while allowing any positive value.
	//
	static optRet Range(int iMin,int iMax) { return defOpt.MinMax(iMin,iMax); }  ;		

	// Sets the range of an input box for integers or floats, to the range specified.
	// If the range is exceeded, an error message comes up and prompts the user to enter a value in the range.
	// 
	// Example: GetFloat(Range(-5,5)) -- gets a floating point value in the range of -5 to 5.
	// Use Min() or Max() for ranges on the end, such as GetFloat(Min(0)) to prohibit negative values
	// while allowing any positive value.
	//
	static optRet Range(double fMin,double fMax) { return defOpt.MinMax(fMin,fMax); }  ;		

	// Use the last known options.  This is deprecated.
	// Instead, set the options in a cwfOpt value, such as:
	//
	// cwfOpt cwOpt = fgColor("Red")  | Transparent() | CenterXY(), etc. 
	// and then use cwOpt as the input. 
	//
	static optRet UseLast() { return defOpt.UseLast(); }  ;	

	// Sets the floating-point range of an input box for integers or floats, to the range specified.
	// If the range is exceeded, an error message comes up and prompts the user to enter a value in the range.
	// 
	// Example: GetFloat(Range(-5,5)) -- gets a floating point value in the range of -5 to 5.
	// Use Min() or Max() for ranges on the end, such as GetFloat(Min(0)) to prohibit negative values
	// while allowing any positive value.
	//
	static optRet Rangef(double fMin,double fMax) { return defOpt.MinMax(fMin,fMax); }  ;		

	// Sets the floating-point range of an input box for integers or floats, to the range specified.
	// If the range is exceeded, an error message comes up and prompts the user to enter a value in the range.
	// 
	// Example: GetFloat(Range(-5,5)) -- gets a floating point value in the range of -5 to 5.
	// Use Min() or Max() for ranges on the end, such as GetFloat(Min(0)) to prohibit negative values
	// while allowing any positive value.
	//
	static optRet Rangef(float fMin,float fMax) { return defOpt.MinMax(fMin,fMax); }  ;		

	/// <summary>
	/// Tells the widget or process to use the Windows/OS Event Thread rather than pumping the 
    /// widget through the procedural GetEvent() loop. 
	/// </summary>
	/// <returns></returns>
	static optRet UseEventThread() { return defOpt.UseEventThread(); }  ;	

	/// <summary>
	/// Causes the Widget, Window, or other control/item to create a fullscreen window
    /// rather than a window with a border. 
    /// 
    /// This window will use the entire desktop, and you should provide a way to close the window
    /// since the 'X' button and other menu items will not be present.
    /// 
	/// </summary>
	/// <returns></returns>
	static optRet Fullscreen() { return defOpt.Fullscreen(); }  ;	

	// Apply the Info Icon to the dialog box -- currently not functional
	//
	static optRet IconInfo()	{ return defOpt.IconInfo();		}		;
	// Apply the Info Icon to the dialog box -- currently not functional
	//
	static optRet IconWarning()	{ return defOpt.IconWarning();	}		;
	// Apply the Info Icon to the dialog box -- currently not functional
	//
	static optRet IconStop()	{ return defOpt.IconStop();	}		;

	// Apply the No Icon to the dialog box -- currently not functional
	//
	static optRet IconNone()	{ return defOpt.IconNone();		}		;


	// Add X pixels to the size of control. For example,
	// CreateButton(10,50,"OK",PadX(20)) will add 20 pixels to the size of the button to make the button wider with "OK" in the center.
	//
	static optRet PadX(int iPad) { return defOpt.PadX(iPad); }  ;		

	// Add Y pixels to the size of control. For example,
	// CreateButton(10,50,"OK",PadX(20)) will add 20 pixels to the size of the button to make the button wider with "OK" in the center.
	//
	// PadY() only works with certain controls.  For example, standard buttons work with PadX() but not PadY(), where
	// Panel Buttons and Windows Buttons (i.e. Style("Panel") and Style("Windows") respond do the PadY(), though 
	// specifiying an explicit width and height is a better option.
	//
	static optRet PadY(int iPad) { return defOpt.PadY(iPad); }  ;		

	// Add X Pixels to the location of the control.  This helps when keeping items off of the edges,giving them some space.
	// For example, TextWidget(0,0,"This a text widget",opt::JustTopRight,opt::OffsetX(-5)) will move the text widget 5 pixels from 
	// the edge of the window to give it some space. 
	//
	static optRet OffsetX(int iOffset) { return defOpt.OffsetX(iOffset); }  ;		

	// Add Y Pixels to the location of the control.  This helps when keeping items off of the edges,giving them some space.
	// For example, TextWidget(0,10,"This a text widget",opt::JustBottomCenter,opt::OffsetX(-5)) will move the text widget 5 pixels from 
	// the bottom of the window, making it look more natural with some space before the bottom edge.
	//
	static optRet OffsetY(int iOffset) { return defOpt.OffsetY(iOffset); }  ;		

	/// <summary>
	/// Can be used in dialog boxes such as GetInteger(), GetFloat,QuickDialog(), etc. to elminate the cancel button.
	/// This will insist the user to enter a value before continuing with no way to cancel the input dialog.
    /// <para></para>&#160;&#160;&#160;
	/// If the user closes the window, the dialog box will return the default value, even when NoCancel() is used.
    /// <para></para>&#160;&#160;&#160;
    /// --> Use opt::Modal() to disable the parent window until the dialog box is closed.
    /// </summary>
	/// <returns></returns>
	static optRet NoCancel() { return defOpt.NoCancel(); }  ;	

    static optRet EmptyOk() { return defOpt.EmptyOk(); };
    static optRet NoBlanks() { return defOpt.NoBlanks(); } 
	// This allows the user to Cancel certain controls.  
	// This used when creating a PleaseWaitWindow() to allow the user to cancel, which can then be 
	// detected by polling the please wait window.
	//
	static optRet CancelOk() { return defOpt.CancelOk(); }  ;

	// Specifies that the new window opened will disabled the parent window until the new window is dismissed
	// Used for message boxes, document windows, and other windows with any form of input (buttons, sliders, edit boxes, etc.).
	// For example, opening an InfoWindow with Modal() option will not allow input to any other window 
	// until the OK button is pressed.
	//
	// When Modal() is not specified, other windows may also be used (depending on the function).
	//
	static optRet Modal() { return defOpt.Modal(); }  ;	

	/// <summary>
	/// Sets Quick C++ mode for Sagebox, for easier development and more automatic processes, such as updating windows. 
    /// <para></para>
    /// --> See Sagebox::SetQuickCpp() for more information
    /// <para></para>
    /// This mode initializes the debug window as hidden, so that the program can be terminated easier (moving the mouse to the upper-right corner activates the debug window),
    /// sets timer-based updates for all windows (every 20ms), turns on the word-wrap and
    /// allows control-C to terminate the program in any window.
    /// <para></para>
    /// --> Note this is a global setting and is meant to be used only once in the initial Sagebox::NewWindow() creation.  Sagebox::SetQuickCpp() can be called directly to perform the same function.
	/// </summary>
	/// <returns></returns>
	static optRet QuickCpp() { return defOpt.QuickCpp(); }  ;	

	/// <summary>
	/// Sets Debug mode for Sagebox, for easier development and debugging, such as updating windows. 
    /// <para></para>
    /// --> See Sagebox::SetDebugMode() for more information
    /// <para></para>
    /// This mode initializes the debug window as hidden  (moving the mouse to the upper-right corner activates the debug window), and allows control-C to terminate the program in any window.
    /// <para></para>
    /// --> Note this is a global setting and is meant to be used only once in the initial Sagebox::NewWindow() creation.  Sagebox::SetDebugMode() can be called directly to perform the same function.
	/// </summary>
	/// <returns></returns>
    static optRet DebugMode() { return defOpt.DebugMode(); }  ;	
    
	// Sets AutoUpdate to False when creating a Window.  When AutoUpdate = False, you must
	// update the window when appropriate.  
	//
	// Default behavior for new windows is AutoUpdate = On, which automatically updates the window
	// (though one Update() is sometimes required at the end of large outputs to make sure the Window is fully updated)
	//
	static optRet NoAutoUpdate() { return defOpt.NoAutoUpdate(); }  ;	
    
	/// <summary>
	/// Sets the window for direct output to the display bitmap rather than buffer the output to a separate bitmap,
    /// which then displays when an update occurs -- this will not paint direfctly to the window, making the program responsible for PAINT messages from the OS
    /// 
    /// This also Signals that a window will be used for Real Time drawing, typically based on the vertical retrace. 
    /// <para></para> 
    /// <para>.</para> 
    /// --> sets AutoUpdate off for the Window (Window must be automatically updated) <para></para>
    /// --> Creates memory and sets other environmental factors more suitable to real-time graphics display <para></para>
    /// --> Also see RealTime() option to set up a RealTime display status without setting a direct output to the window.<para></para>
    /// <para>.</para> 
    /// Note: This function is still TBD and is meant to work with real-time graphics as well as the GPU. 
    /// When this option is used, Auto Updating of the window is also turned off. 
	/// </summary>
	/// <returns></returns>	
    static optRet DirectDraw() { return defOpt.DirectDraw(); }  ;	
  
	/// <summary>
	/// Signals that a window will be used for Real Time drawing, typically based on the vertical retrace. 
    /// <para></para> 
    /// <para>.</para> 
    /// --> sets AutoUpdate off for the Window (Window must be automatically updated) <para></para>
    /// --> Creates memory and sets other environmental factors more suitable to real-time graphics display <para></para>
    /// --> Also see DirectDraw() option<para></para>
    /// <para>.</para> 
    /// Note: This function is still TBD and is meant to work with real-time graphics as well as the GPU. 
    /// When this option is used, Auto Updating of the window is also turned off. 
	/// </summary>
	/// <returns></returns>
	static optRet RealTime() { return defOpt.RealTime(); }  ;	

    /// <summary>
    /// Sets the AutoUpdate Type to the type specified, such as No Update, On, Immediate, OnTime, etc.
    /// </summary>
    /// <param name="eUpdateType"> = Auto Update Type</param>
    /// <returns></returns>
    static optRet AutoUpdate(Sage::AutoUpdateType eUpdateType) { return defOpt.AutoUpdate((int) eUpdateType); };

    /// <summary>
    /// Sets the AutoUpdate Type to update on a timer so that the update for the window is automatic (i.e. Update() does not need to be called)
    /// <para></para>
    /// This is the same as AutoUpdate(AutoUpdateType::OnTime), which will update the screen every 20ms or so.
    /// <para></para>
    /// --> Timer-based updates are for general-purpose usage. Update() can also be used for faster/immediate updates() when desired, even when the window is being updated via a timer.
    /// </summary>
    /// <returns></returns>
    static optRet WinTimerUpdate() { return defOpt.AutoUpdate((int) Sage::AutoUpdateType::OnTime); };

	// When Creating a Dialog (i.e. dialog.Info(), etc.) or Dialog Window, CenterWindow()
	// causes the newly created window to be centered in the parent window creating it. 
	//
	// Otherwise, the window is put somewhere on the screen, or, in the case of dialog windows and
	// dialog functions, centered on the screen rather than the parent window.
	//
	static optRet CenterWin() { return defOpt.CenterWin(); }  ;	

    /// <summary>
    /// Centers the object on the desktop (such as a new window, or dialog). 
    /// <para></para>
    /// Some functions will center in the parent window, using CenterDesktop() will cause the
    /// window to be centered in the Desktop instead.
    /// <para></para>
    /// --> Note: this functon is still in progress and may not work on all functions that create windows.
    /// <para></para>
    /// --> QuickDialog() is currently the only funciton that suppirts CenterDesktop()
    /// </summary>
    /// <returns></returns>
    static optRet CenterDesktop() { return defOpt.CenterDesktop(); }  ;

	// Specifies window as "TopMost", keeping the window at the top of all windows.  This prevents other windows from 
	// overlapping the window.  This is useful for windows that need to be on top at all times so they do not become
	// obscured by other windows
	//
	static optRet SetTopmost() { return defOpt.SetTopmost(); };
	// Hides the cancel button on dialog.  This is used for the PleaseWaitWindow() to remove the cancel button
	// after a certain point -- i.e. after a point of no return. 
	//
	static optRet HideCancel() { return defOpt.HideCancel(); }  ;	

	// Add a progress bar to the PleaseWaitWindow(),
	// i.e. PleaseWaitWindow(ProgressBar());
	//
	static optRet ProgressBar() { return defOpt.ProgressBar(); }  ;	

	// Causes Text, Control, or Widget to have a transparent background.
	// --->This will cause the background of the parent window to show through, allowing the Text, control or widget to blend in.
	// --->When this option is not active, the text, control or widget will have a solid background.  For most windows, which have a solid background, this is fine.
	// --->However, with textures, bitmaps, and gradients, this will allow blending in a seamless manner.
	//
    // Adding a blend percent sets the blending percent of the object, where
    // 0 = fully opaque and 100 = fully transparent. 
    //
	static optRet Transparent() { return defOpt.Transparent(); }  ;	
	// Causes Text, Control, or Widget to have a transparent background.
	// --->This will cause the background of the parent window to show through, allowing the Text, control or widget to blend in.
	// --->When this option is not active, the text, control or widget will have a solid background.  For most windows, which have a solid background, this is fine.
	// --->However, with textures, bitmaps, and gradients, this will allow blending in a seamless manner.
	//
    // Adding a blend percent sets the blending percent of the object, where
    // 100 = fully opaque and 0 = fully transparent. 
    //
	static optRet Transparent(int iBlendPercent) { return defOpt.Transparent(iBlendPercent); }  ;	

    // Sets the blending percent of the control or target object. 
    // 0 = fully transparent, 100 = fully opaque.
    // This only work controls or widgets where it supported, for example:
    //
    // --> TextWidget(100,200,0,0,"This is a 75% visible line of text",Blend(75)); 
    //
    // Transparent() may be used to combine blending and transparency of the control, such as:
    //
    // --> TextWidget(100,200,0,0,"This is a 25% visible line of text",Transparent(25)); 
    //
	static optRet Blend(int iBlendPercent) { return defOpt.Blend(iBlendPercent); }  ;	

	// Keeps the control, widget, or window hidden as it is created.
	// By default, all Windows, Widgets, and Controls appear upon creation. 
	// Hidden() will keep the Window/Control hidden, allowing for changes to appearance and behavior before they
	// show in the screen.
	//
	// Use <control>.Show() (i.e. myWindow.Show()) to show the Window, Control or Widget that is hidden.
	//
	static optRet Hidden() { return defOpt.Hidden(); }  ;	

	/// <summary>
	/// Tell various functions (such as QuickDialog) not to auto hide the window when done.
    /// <para></para>
    /// When this is used, it is the calling program's responsibility to hide the window when finished. 
    /// <para></para>&#160;&#160;&#160;
    /// This is useful for Quick Dialogs (and some other functions, TBD) to validate data after the user pressed OK, to
    /// re-enter the loop without creating a flash when hiding and re-showing the window.
	/// </summary>
	/// <returns></returns>
	static optRet NoAutoHide() { return defOpt.NoAutoHide(); }  ;	
	
    // Sets FastMode() for widgets.  
	// This is a powerful function allowing widgets to update only when
	// the system is ready for another update (around 10ms-20ms), rather than 
	// re-drawing the control every time a value is changed.
	//
	// When this is used, the widget will require one last update
	// at the end to ensure drawing of the last known value. 
	// 
	// This is usually called LastUpdate(), telling the widget
	// to draw and forcibly update the last known value
	//
	static optRet FastMode() { return defOpt.FastMode(); }  ;	

	// --> Sets the ID of the control, window or Widget
	//
	// This gives the control an identifier than can be used to find it or control it later without needing the control object pointer.
	// This can be used for Groups or for general ID.
	//
	// note: Avoid using 0 for an ID, as this conflicts with some functions mixing a boolean return with 
	// a control value. 
	// 
	// With Groups, a 1-based ID is automatically assigned for each member in the group, numbered in order of declaration.
	//
	// For example, to find a button with a control:
	//
	// CreateButton(10,20,"OK",ID(10));
	// Later on, auto& MyButton = button(10) can be used to get the button pointer, or button(10).Pressed()  (i.e. any function) can be used to quickly access
	// the button without knowing the control pointer value.
	//
	static optRet ID(int iID)					{ return defOpt.ControlID(iID);				}  ;		

	// Set a common group for a Button, Checkbox, RadioButton, Slider, or other control
	// This can allow group actions, such as checking button presses in the group rather than 
	// checking each control.
	//
	// Groups, in a sense, take the place of using a button class override to check the state.
	// With a Group, functions such as group.GetButtonPress(iGroupID) can give you the 
	// ID of the button pressed within the group.
	//
	// If the Group is set for radio buttons, this will automatically change the checks of the radio buttons as they are selected.  Otherwise, this
	// must be done individually. 
	//
	// Similarly, with input boxes, buttons can be assigned to accept an "OK" for all edit boxes and call the
	// validation routines for all inputs, only continuing when no error exists.
	//
	// A Group can be a number or a string value. 
	// There are also experimeting CGroup() class values that can be used, as well
	//
	// When a Group is designaed, an ID() is automatically assigned to the control.  This ID is 1-based (i.e. starting with 1) 
	// and is by order of creation -- if the order of creation of the controls is changed, this will also change the automatically
	// assigned ID.  
	//
	// If an ID is set explicitly, this ID is used instead.
	//
	// Example:
	//
	// CreateRadioButton(10,50,"MyButton",Group(1)) - Set button as Group(1), this also sets ID(1) automatically.
	// CreateRadioButton(10,50,"MyButton2",Group(1)) - Set button as Group(1), this also sets ID(2) automatically.
	// CreateButton(100,200,"test Button",Group("MyGroup") | ID(1234) -- Puts button in Group("MyGroup") with ID(1234)
	//
	// While strings can be used for Groups, numeric values are faster and easier to use.
	//
	static optRet Group(const char * sGroup)			{ return defOpt.Group(sGroup);				}  ;	

	// Set a common group for a Button, Checkbox, RadioButton, Slider, or other control
	// This can allow group actions, such as checking button presses in the group rather than 
	// checking each control.
	//
	// Groups, in a sense, take the place of using a button class override to check the state.
	// With a Group, functions such as group.GetButtonPress(iGroupID) can give you the 
	// ID of the button pressed within the group.
	//
	// If the Group is set for radio buttons, this will automatically change the checks of the radio buttons as they are selected.  Otherwise, this
	// must be done individually. 
	//
	// Similarly, with input boxes, buttons can be assigned to accept an "OK" for all edit boxes and call the
	// validation routines for all inputs, only continuing when no error exists.
	//
	// A Group can be a number or a string value. 
	// There are also experimeting CGroup() class values that can be used, as well
	//
	// When a Group is designaed, an ID() is automatically assigned to the control.  This ID is 1-based (i.e. starting with 1) 
	// and is by order of creation -- if the order of creation of the controls is changed, this will also change the automatically
	// assigned ID.  
	//
	// If an ID is set explicitly, this ID is used instead.
	//
	// Example:
	//
	// CreateRadioButton(10,50,"MyButton",Group(1)) - Set button as Group(1), this also sets ID(1) automatically.
	// CreateRadioButton(10,50,"MyButton2",Group(1)) - Set button as Group(1), this also sets ID(2) automatically.
	// CreateButton(100,200,"test Button",Group("MyGroup") | ID(1234) -- Puts button in Group("MyGroup") with ID(1234)
	//
	// While strings can be used for Groups, numeric values are faster and easier to use.
	//
	static optRet Group(const char * sGroup,int iID)	{ return defOpt.Group_ID(sGroup,iID);			}  ;	

	// Set a common group for a Button, Checkbox, RadioButton, Slider, or other control
	// This can allow group actions, such as checking button presses in the group rather than 
	// checking each control.
	//
	// Groups, in a sense, take the place of using a button class override to check the state.
	// With a Group, functions such as group.GetButtonPress(iGroupID) can give you the 
	// ID of the button pressed within the group.
	//
	// If the Group is set for radio buttons, this will automatically change the checks of the radio buttons as they are selected.  Otherwise, this
	// must be done individually. 
	//
	// Similarly, with input boxes, buttons can be assigned to accept an "OK" for all edit boxes and call the
	// validation routines for all inputs, only continuing when no error exists.
	//
	// A Group can be a number or a string value. 
	// There are also experimeting CGroup() class values that can be used, as well
	//
	// When a Group is designaed, an ID() is automatically assigned to the control.  This ID is 1-based (i.e. starting with 1) 
	// and is by order of creation -- if the order of creation of the controls is changed, this will also change the automatically
	// assigned ID.  
	//
	// If an ID is set explicitly, this ID is used instead.
	//
	// Example:
	//
	// CreateRadioButton(10,50,"MyButton",Group(1)) - Set button as Group(1), this also sets ID(1) automatically.
	// CreateRadioButton(10,50,"MyButton2",Group(1)) - Set button as Group(1), this also sets ID(2) automatically.
	// CreateButton(100,200,"test Button",Group("MyGroup") | ID(1234) -- Puts button in Group("MyGroup") with ID(1234)
	//
	// While strings can be used for Groups, numeric values are faster and easier to use.
	//
	static optRet Group(int iGroupID)			{ return defOpt.GroupID(iGroupID);			}  ;	

	// Set a common group for a Button, Checkbox, RadioButton, Slider, or other control
	// This can allow group actions, such as checking button presses in the group rather than 
	// checking each control.
	//
	// Groups, in a sense, take the place of using a button class override to check the state.
	// With a Group, functions such as group.GetButtonPress(iGroupID) can give you the 
	// ID of the button pressed within the group.
	//
	// If the Group is set for radio buttons, this will automatically change the checks of the radio buttons as they are selected.  Otherwise, this
	// must be done individually. 
	//
	// Similarly, with input boxes, buttons can be assigned to accept an "OK" for all edit boxes and call the
	// validation routines for all inputs, only continuing when no error exists.
	//
	// A Group can be a number or a string value. 
	// There are also experimeting CGroup() class values that can be used, as well
	//
	// When a Group is designaed, an ID() is automatically assigned to the control.  This ID is 1-based (i.e. starting with 1) 
	// and is by order of creation -- if the order of creation of the controls is changed, this will also change the automatically
	// assigned ID.  
	//
	// If an ID is set explicitly, this ID is used instead.
	//
	// Example:
	//
	// CreateRadioButton(10,50,"MyButton",Group(1)) - Set button as Group(1), this also sets ID(1) automatically.
	// CreateRadioButton(10,50,"MyButton2",Group(1)) - Set button as Group(1), this also sets ID(2) automatically.
	// CreateButton(100,200,"test Button",Group("MyGroup") | ID(1234) -- Puts button in Group("MyGroup") with ID(1234)
	//
	// While strings can be used for Groups, numeric values are faster and easier to use.
	//
	static optRet Group(int iGroupID,int iID)	{ return defOpt.GroupID_ID(iGroupID,iID);		}  ;	

	// Set a common group for a Button, Checkbox, RadioButton, Slider, or other control
	// This can allow group actions, such as checking button presses in the group rather than 
	// checking each control.
	//
	// Groups, in a sense, take the place of using a button class override to check the state.
	// With a Group, functions such as group.GetButtonPress(iGroupID) can give you the 
	// ID of the button pressed within the group.
	//
	// If the Group is set for radio buttons, this will automatically change the checks of the radio buttons as they are selected.  Otherwise, this
	// must be done individually. 
	//
	// Similarly, with input boxes, buttons can be assigned to accept an "OK" for all edit boxes and call the
	// validation routines for all inputs, only continuing when no error exists.
	//
	// A Group can be a number or a string value. 
	// There are also experimeting CGroup() class values that can be used, as well
	//
	// When a Group is designaed, an ID() is automatically assigned to the control.  This ID is 1-based (i.e. starting with 1) 
	// and is by order of creation -- if the order of creation of the controls is changed, this will also change the automatically
	// assigned ID.  
	//
	// If an ID is set explicitly, this ID is used instead.
	//
	// Example:
	//
	// CreateRadioButton(10,50,"MyButton",Group(1)) - Set button as Group(1), this also sets ID(1) automatically.
	// CreateRadioButton(10,50,"MyButton2",Group(1)) - Set button as Group(1), this also sets ID(2) automatically.
	// CreateButton(100,200,"test Button",Group("MyGroup") | ID(1234) -- Puts button in Group("MyGroup") with ID(1234)
	//
	// While strings can be used for Groups, numeric values are faster and easier to use.
	//
	static optRet Group(ControlGroup & cGroup)	{ return defOpt.Group(cGroup);		}  ;	

	// Set a common group for a Button, Checkbox, RadioButton, Slider, or other control
	// This can allow group actions, such as checking button presses in the group rather than 
	// checking each control.
	//
	// Groups, in a sense, take the place of using a button class override to check the state.
	// With a Group, functions such as group.GetButtonPress(iGroupID) can give you the 
	// ID of the button pressed within the group.
	//
	// If the Group is set for radio buttons, this will automatically change the checks of the radio buttons as they are selected.  Otherwise, this
	// must be done individually. 
	//
	// Similarly, with input boxes, buttons can be assigned to accept an "OK" for all edit boxes and call the
	// validation routines for all inputs, only continuing when no error exists.
	//
	// A Group can be a number or a string value. 
	// There are also experimeting CGroup() class values that can be used, as well
	//
	// When a Group is designaed, an ID() is automatically assigned to the control.  This ID is 1-based (i.e. starting with 1) 
	// and is by order of creation -- if the order of creation of the controls is changed, this will also change the automatically
	// assigned ID.  
	//
	// If an ID is set explicitly, this ID is used instead.
	//
	// Example:
	//
	// CreateRadioButton(10,50,"MyButton",Group(1)) - Set button as Group(1), this also sets ID(1) automatically.
	// CreateRadioButton(10,50,"MyButton2",Group(1)) - Set button as Group(1), this also sets ID(2) automatically.
	// CreateButton(100,200,"test Button",Group("MyGroup") | ID(1234) -- Puts button in Group("MyGroup") with ID(1234)
	//
	// While strings can be used for Groups, numeric values are faster and easier to use.
	//
	static optRet Group(ControlGroup & cGroup,int iID)	{ return defOpt.Group(cGroup,iID);		}  ;	

	// --> Used for Buttons.  Tells the button that when pressed, it should validate the Edit Boxes in the group specified.   The Validate() function for
	// --> each edit box is called.  If there is no Validate function, the Min/Max ranges, entry types (i.e. integer, float, string), and blank-lines allowed/disallowed
	// --> are checked.  If these checks fail, a dialog box is displayed telling the user to enter the correct information -- the keyboard focus is then set for the
	// --> first edit box to fail the check.
	// --> The virtual Validate() function in the CEditBox class, when overridden, can return a specific message to display for the failure. 
	// Example:
	//             auto MyButton = Console.Button("Press OK to Continue",opt::ValidateGroup("MyEditBoxGroup"));
	// (This causes the button to validate any edit box with a group name "MyEditBoxGroup"). 
	//             auto MyButton = Console.Button("Press OK to Continue",opt::ValidateGroup(MyEditBoxGroup));
	// (This does the same thing, but uses a pre-defined group string, value, or group object)
	//
	// Where it is useful:
	//
	//        *** This function is useful when you have multiple edit boxes and want to continue execution when they are filled.  These can be validated with no extra validation code or
	//        *** validation checking when using the values -- by assigning ValidateGroup() to a button, and then setting min/max and other requirments on each Edit Box.
	//        *** Once the code is working properly, you can later add your own validation code.
	//        *** 
	//        *** When there is only one edit box, the validation occurs when the user presses ENTER.  When there are multiple edit boxes, these values can be filled and there is no
	//        *** easy way to validate all boxes unless you look for each ENTER on each edit box and process them individually.  ValidateGroup() allows this to happen all at once with
	//        *** no additional code.
	//
	static optRet ValidateGroup(const char * sGroup)	{ return defOpt.ValidateGroup(sGroup);		}  ;	

	// --> Used for Buttons.  Tells the button that when pressed, it should validate the Edit Boxes in the group specified.   The Validate() function for
	// --> each edit box is called.  If there is no Validate function, the Min/Max ranges, entry types (i.e. integer, float, string), and blank-lines allowed/disallowed
	// --> are checked.  If these checks fail, a dialog box is displayed telling the user to enter the correct information -- the keyboard focus is then set for the
	// --> first edit box to fail the check.
	// --> The virtual Validate() function in the CEditBox class, when overridden, can return a specific message to display for the failure. 
	// Example:
	//             auto MyButton = Console.Button("Press OK to Continue",opt::ValidateGroup("MyEditBoxGroup"));
	// (This causes the button to validate any edit box with a group name "MyEditBoxGroup"). 
	//             auto MyButton = Console.Button("Press OK to Continue",opt::ValidateGroup(MyEditBoxGroup));
	// (This does the same thing, but uses a pre-defined group string, value, or group object)
	//
	// Where it is useful:
	//
	//        *** This function is useful when you have multiple edit boxes and want to continue execution when they are filled.  These can be validated with no extra validation code or
	//        *** validation checking when using the values -- by assigning ValidateGroup() to a button, and then setting min/max and other requirments on each Edit Box.
	//        *** Once the code is working properly, you can later add your own validation code.
	//        *** 
	//        *** When there is only one edit box, the validation occurs when the user presses ENTER.  When there are multiple edit boxes, these values can be filled and there is no
	//        *** easy way to validate all boxes unless you look for each ENTER on each edit box and process them individually.  ValidateGroup() allows this to happen all at once with
	//        *** no additional code.
	//
	static optRet ValidateGroup(ControlGroup & cGroup)	{ return defOpt.ValidateGroup(cGroup);		}  ;	

	// --> Centers the control, Widget, or Window. horizontally in the window (on the current line or Y position in the window).
	// --> For vertical centering, use CenterY().   To center in entire window, use CenterXY()
	// Example:
	//          MyWindow.GetInputButton("Press to continue",opt::Center() | opt::WinColors());
	//          (This centers the button on the current output/console line and using the current window colors for the button)
	//
	static optRet Center()	{ return defOpt.Center();		}  ;		

	/// <summary>
	/// Centers the control, Widget, or Window. horizontally in the window (on the current line or Y position in the window).
	/// For vertical centering, use CenterY().   To center in entire window, use CenterXY()
    /// <para></para>&#160;&#160;&#160;
	/// Example:
    /// <para></para>&#160;&#160;&#160;&#160;&#160;&#160;
	///          MyWindow.GetInputButton("Press to continue",opt::CenterX() | opt::WinColors());
    /// <para></para>&#160;&#160;&#160;&#160;&#160;&#160;
	///          (This centers the button on the current output/console line and using the current window colors for the button
	/// </summary>
	/// <param name="iRange"> = Consider iRange as centering width range rather than window size.</param>
	/// <returns></returns>
	static optRet CenterX()	{ return defOpt.CenterX();	}  ;		

	/// <summary>
	/// Centers the control, Widget, or Window. horizontally in the window (on the current line or Y position in the window).
	/// For vertical centering, use CenterY().   To center in entire window, use CenterXY()
    /// <para></para>&#160;&#160;&#160;
	/// Example:
    /// <para></para>&#160;&#160;&#160;&#160;&#160;&#160;
	///          MyWindow.GetInputButton("Press to continue",opt::CenterX(300) | opt::WinColors());
    /// <para></para>&#160;&#160;&#160;&#160;&#160;&#160;
	///          (This centers the button on the current output/console line and using the current window colors for the button
	/// </summary>
	/// <param name="iRange"> = Consider iRange as centering width range rather than window size.</param>
	/// <returns></returns>
	static optRet CenterX(int iRange)	{ return defOpt.CenterX(iRange);	}  ;		

	// Centers the control, widget, or window in the window in the Y dimension
	//
	static optRet CenterY() { return defOpt.CenterY();	}  ;		
	// Centers the control, widget, or window in the window in the Y dimension
	//
	static optRet CenterY(int iRange) { return defOpt.CenterY(iRange);	}  ;		

	// Adds a border to the input box, window, or other control that uses borders
	//
	static optRet AddBorder() { return defOpt.AddBorder();	}  ;		

	// When creating a new window with a frame or border, InnerSize() creates the window with the
	// canvas size set to the input width/height.  Otherwise, the Width/Height is the size of the
	// entire window, including the frame or border
	//
	static optRet InnerSize() { return defOpt.InnerSize();	}  ;		

	// Allows the window to be resized by the user by dragging the edge of the window and maximizing the window.
	// By default, once the window is created, the size cannot be changed by the user. 
	//
	// When true, make sure the window has the canvas size to support larger sizes.  The window will not
	// be allowed to size greater than the original size of greater canvas set set in the program
	//
	static optRet Resizeable() { return defOpt.Resizeable(); };

	/// <summary>
	/// Sets the size of the control, widget, or functional item. 
    /// When available, this can be used when the Size is not parameter in the function call.
	/// </summary>
	/// <param name="iWidth"> - Width of element</param>
	/// <param name="iHeight"> - Height of element</param>
	/// <returns></returns>
	static optRet SetSize(int iWidth,int iHeight)	{ return defOpt.SetSize(iWidth,iHeight);	}  ;		

    /// <summary>
	/// Sets the size of the control, widget, or functional item. 
    /// When available, this can be used when the Size is not parameter in the function call.
    /// </summary>
    /// <param name="pSize"> - SIZE of element</param>
    /// <returns></returns>
    static optRet SetSize(SIZE pSize)	    { return defOpt.SetSize(pSize.cx,pSize.cy);	}  ;		

	/// <summary>
	/// Sets the maximum size of the control, widget, or functional item. 
    /// When available, this can be used when the Size is not parameter in the function call.
	/// </summary>
	/// <param name="iWidth"> - Width of element</param>
	/// <param name="iHeight"> - Height of element</param>
	/// <returns></returns>
	static optRet MaxSize(int iWidth,int iHeight)	{ return defOpt.SetSize(iWidth,iHeight);	}  ;		

    /// <summary>
	/// Sets the maximum size of the control, widget, or functional item. 
    /// When available, this can be used when the Size is not parameter in the function call.
    /// </summary>
    /// <param name="pSize"> - SIZE of element</param>
    /// <returns></returns>
    static optRet MaxSize(SIZE pSize)	    { return defOpt.SetSize(pSize.cx,pSize.cy);	}  ;		


	// Tells the Window, Control, input box, or Widget to no use a border even when it is the default
	//
	static optRet NoBorder() { return defOpt.NoBorder();	}  ;		
	
	// Don't add a scrollbar on a control where it may be a default (such as a Listbox).
	//
	static optRet NoScrollbar() { return defOpt.NoScrollbar();	}  ;		


	// CenterXY() -- Centers the Control, Widget, or new Window in the current Window.
	//               This is can also be used as JustCenter()
	//
	// ---> In some cases, this will also center Text inside of a control when there is no other context.
	// ---> For example, Write("Hello World",CenterXY()) will center the text in the window.
	//
	// For most text (such as with a TextWidget), use TextCenterXY() instead of CenterXY()
	//
	static optRet CenterXY(int iOffset=0) { return defOpt.CenterXY(iOffset); }  ;		
	
    // CenterXY() -- Centers the Control, Widget, or new Window in the current Window.
	//               This is can also be used as JustCenter()
	//
	// ---> In some cases, this will also center Text inside of a control when there is no other context.
	// ---> For example, Write("Hello World",CenterXY()) will center the text in the window.
	//
	// For most text (such as with a TextWidget), use TextCenterXY() instead of CenterXY()
	//
	static optRet CenterXY(int iRangeX,int iRangeY) { return defOpt.CenterXY(iRangeX,iRangeY); }  ;		

	// Add a Shadow to the control, window, or widget. 
	//
	static optRet AddShadow()				{ return defOpt.AddShadow(); }  ;

	// --> Allows the user to move the control or Widget (in Controls and Widgets that support it).
	//     This allows the user to move the Control or Widget on the screen with the mouse, usually
	//     with the Control-Key pressed, but also sometimes with normal mouse movement if there is no 
	//     other reason to use the mouse on the Control or Widget.
	//
	// --> Some controls may also support using the right mouse button (TBD)
	static optRet AllowDrag()				{ return defOpt.AllowDrag()		; }  ;		

	// When creating a BitmapWindow or other types of windows, this creates a separate "popup" window.
	// Otherwise, the window is embedded in the current window at the (X,Y) position specified.
	//
	static optRet Popup()			{ return defOpt.Popup()			; }  ;	

    /// <summary>
    /// Tells a function that creates a window to embed the window into the parent window, rather than as its own popup-styyle window.
    /// </summary>
    /// <returns></returns>
    static optRet Embed()            { return defOpt.Embed()        ; } ;

	/// <summary>
	/// Disallows sizing of a window (i.e. through NewWindow(), ChildWindow(), etc.).  By default, the user may resize the window up to the canvas size by dragging the window's edges. 
    /// <para></para>
    /// ---> When NoSizing() is used as an option, the window cannot be resized by the user (but can be resized by the program).  This does not include Minimize
    /// and Maximize controls.  Use NoSysMenu() to eliminate these options.
	/// </summary>
	/// <returns></returns>
	static optRet NoSizing()			{ return defOpt.NoSizing()	; }  ;	

 	/// <summary>
	/// Removes Minimize and Maximize buttons on a new window (i.e. NewWindow(), ChildWindow(), etc).  By default, the user may minimize and maximize a window. 
    /// This removes the buttons to allow minimizing and maximizing. 
    /// <para></para>
    /// ---> Use this option with NoSizing() to also remove the ability for the user to dynamically size the window by grabbing its edges.
	/// </summary>
	/// <returns></returns>
	static optRet NoSysMenu()			{ return defOpt.NoSysMenu()	; }  ;	
   

	// Disable automatic closing for dialogs and windows
	// When a dialo window is created, by default, pressing the 'X' close button in the top-right of the window
	// will close the dialog.
	//
	// NoClose() will keep the window open and set a ClosePending() flag.
	// The OnClose() can also be intercepted in the Window Handler.
	//
	static optRet NoClose()			{ return defOpt.NoClose()	; }  ;	

	// In an EditBox (or other text-based control) ReadOnly makes the text non-editable and only for display. 
	//
	static optRet ReadOnly()				{ return defOpt.ReadOnly()	; }  ;		

	// In an EditBox, NumbersOnly() limits the text input to only integer numbers. All other text is ignored.  See FloatsOnly() for floating-point numbers.
	//
	// --->
	// note: This also automatically sets the test justification for an EditBox to TextRight(). This can be changed by explicitly setting another for, i.e. TextCenter() or TextLeft();
	//
	static optRet NumbersOnly()				{ return defOpt.NumbersOnly()	; }  ;	

	// In an Edit Box, FloatsOnly() limits the text to floating point numbersm allowing a '.',numbers and the '-' sign.  All other text is ignored.
	// See NumbersOnly() to limit text to integer numbers (i.e. without the '.' needed for floating-point numbers.
	// --->
	// note: This also automatically sets the test justification for an EditBox to TextRight(). This can be changed by explicitly setting another for, i.e. TextCenter() or TextLeft();
	//
	static optRet FloatsOnly()				{ return defOpt.FloatsOnly()	; }  ;		


	// --> Centers the Control, Widget, or Window in the X plane.
	// Use the X position in the function call to set an offset from this position.
	//	   This centers the X dimension only.  To Center the Y dimension, use JustCenterY()
	//     Use JustCenterXY() to center text in both X and Y dimensions
	static optRet JustCenter()				{ return defOpt.JustCenter(); }  ;		

	// --> Centers the Control, Widget, or Window in the X plane.
	// Use the X position in the function call to set an offset from this position.
	//	   This centers the X dimension only.  To Center the Y dimension, use JustCenterY()
	//     Use JustCenterXY() to center text in both X and Y dimensions
	static optRet JustCenterX()				{ return defOpt.JustCenterX(); }  ;		

	// --> Centers the Control, Widget, or Window in the X plane.
	// Use the X position in the function call to set an offset from this position.
	//	   This centers the X dimension only.  To Center the Y dimension, use JustCenterY()
	//     Use JustCenterXY() to center text in both X and Y dimensions
	static optRet JustCenterX(int iRange)				{ return defOpt.JustCenterX(iRange); }  ;		

	// --> Centers the Control, Widget, or Window in the Y dimension.
	// Use the Y position in the function call to set an offset from this position.
	//	   This centers the Y dimension only.  To Center the X dimension, use JustCenterX()
	//     Use JustCenterXY() to center text in both X and Y dimensions
	static optRet JustCenterY()				{ return defOpt.JustCenterY(); }  ;		

	// --> Centers the Control, Widget, or Window in the Y dimension.
	// Use the Y position in the function call to set an offset from this position.
	//	   This centers the Y dimension only.  To Center the X dimension, use JustCenterX()
	//     Use JustCenterXY() to center text in both X and Y dimensions
	static optRet JustCenterY(int iRange)				{ return defOpt.JustCenterY(iRange); }  ;		

    // ---> Sets the Control, Window, or Widget to the top of the window.
	// Use the Y position in the function call to set an offset from this position.
	//
	static optRet JustTop()					{ return defOpt.JustTop(); }  ;		

	// ---> Sets the Control, Window, or Widget to the bottom of the window.
	// Use the Y position in the function call to set an offset from this position.
	//
	static optRet JustBottom()				{ return defOpt.JustBottom(); }  ;		

	// ---> Sets the Control, Window, or Widget to the right of the window.
	// Use the X position in the function call to set an offset from this position.
	//
	static optRet JustRight()				{ return defOpt.JustRight(); }  ;		

	// ---> Sets the Control, Window, or Widget to the right of the window.
	// Use the X position in the function call to set an offset from this position.
	//
	static optRet JustRight(int iOffset)	{ return defOpt.JustRight(iOffset); };

	// ---> Sets the Control, Window, or Widget to the left of the window.
	// Use the X position in the function call to set an offset from this position.
	//
	static optRet JustLeft()				{ return defOpt.JustLeft(); }  ;		

	// ---> Sets the Control, Window, or Widget to the top-right of the window.
	// Use the X and Yposition in the function call to set an offset from this position.
	//
	static optRet JustTopRight()			{ return defOpt.JustTopRight(); }  ;		

	// ---> Sets the Control, Window, or Widget to the top-left of the window.
	// Use the X and Yposition in the function call to set an offset from this position.
	//
	static optRet JustTopLeft()				{ return defOpt.JustTopLeft(); }  ;		

	// ---> Sets the Control, Window, or Widget to the bottom-right of the window.
	// Use the X and Yposition in the function call to set an offset from this position.
	//
	static optRet JustBottomRight()			{ return defOpt.JustBottomRight(); }  ;		

	// ---> Sets the Control, Window, or Widget to the bottom-left of the window.
	// Use the X and Yposition in the function call to set an offset from this position.
	//
	static optRet JustBottomLeft()			{ return defOpt.JustBottomLeft(); }  ;		

	// ---> Sets the Control, Window, or Widget to the bottom-center of the window.
	// Use the X and Yposition in the function call to set an offset from this position.
	//
	static optRet JustBottomCenter()		{ return defOpt.JustBottomCenter(); }  ;		

	// ---> Sets the Control, Window, or Widget to the top-center of the window.
	// Use the X and Yposition in the function call to set an offset from this position.
	//
	static optRet JustTopCenter()			{ return defOpt.JustTopCenter(); }  ;		

    // TextCenter() -- Centers the text inside of a Control, Widget, or Window in the X axis
    //
	// This only centers in the X dimension.  Use TextCenterY() to cener in the Y dimension, or CenterXY() for both.
	//
	static optRet TextCenter()				{ return defOpt.TextCenter();		}  ;		

    // TextCenter() -- Centers the text inside of a Control, Widget, or Window in the X axis
    //
	// This only centers in the X dimension.  Use TextCenterY() to cener in the Y dimension, or CenterXY() for both.
	//
	static optRet TextCenter(int iRange)				{ return defOpt.TextCenter();		}  ;		

    // TextCenterX() -- Centers the text inside of a Control, Widget, or Window in the X axis
	//					This can also be used as TextCenterX()
	//
	// This only centers in the X dimension.  Use TextCenterY() to cener in the Y dimension, or CenterXY() for both.
	//
	static optRet TextCenterX()				{ return defOpt.TextCenterX();	}  ;		

    // TextCenterX() -- Centers the text inside of a Control, Widget, or Window in the X axis
	//
	// This only centers in the X dimension.  Use TextCenterY() to cener in the Y dimension, or CenterXY() for both.
	//
	static optRet TextCenterX(int iRange)	{ return defOpt.TextCenterX(iRange);	}  ;		

	// TextCenterY() -- Centers the text inside of a Control, Widget, or Window in the Y axis
	//
	// This only centers in the Y dimension.  Use TextCenterX() to cener in the X dimension, or CenterXY() for both.
	//
	static optRet TextCenterY()				{ return defOpt.TextCenterY();	}  ;		

    // TextCenterY() -- Centers the text inside of a Control, Widget, or Window in the Y axis
	//
	// This only centers in the Y dimension.  Use TextCenterX() to cener in the X dimension, or CenterXY() for both.
	//
	static optRet TextCenterY(int iRange)	{ return defOpt.TextCenterY(iRange);	}  ;		

    // TextCenterXY() -- Centers the text inside of a Control, Widget, or Window on both axes (X and Y)
	//					This can also be used as TextCenterX()
	//
	// This only centers in the Y dimension.  Use TextCenterX() to cener in the X dimension, or CenterXY() for both.
	//
	static optRet TextCenterXY()				{ return defOpt.TextCenterXY();	}  ;		
 
    // TextCenterXY() -- Centers the text inside of a Control, Widget, or Window on both axes (X and Y)
	//					This can also be used as TextCenterX()
	//
	// This only centers in the Y dimension.  Use TextCenterX() to cener in the X dimension, or CenterXY() for both.
	//
	static optRet TextCenterXY(int iRangeX,int iRangeY)	{ return defOpt.TextCenterXY(iRangeX,iRangeY);	}  ;		

	// TextTop() -- Sets the text to the top of the Control or WIndow
	//
	static optRet TextTop()					{ return defOpt.TextTop();		}  ;		

	// TextTop() -- Sets the text to the bottom of the Control or WIndow
	//
	static optRet TextBottom()				{ return defOpt.TextBottom();		}  ;		

	// TextTop() -- Sets the text to the right of the Control or iIndow
	//
	static optRet TextRight()				{ return defOpt.TextRight();		}  ;		

	// TextLeft() -- Sets the text to the left of the Control or iIndow
	//
	static optRet TextLeft()				{ return defOpt.TextLeft();		}  ;		

	// --> Sets Background as WHITE and Text Colors as BLACK (a macro for "colors=bw").  Used for Edit Boxes tha otherwise detault to the
	// --> current window background and color to blend in.  This allows the box to stand out in the window.
	// --> Example: EditBox(wfOpt::CharWidth(20) | wfOpt::ColorsBW()) -- Create an Editbox with Char Width of 50, and BW background/text color.
	// --> This can also be entered as EditBox("charwidth=20,colors=bw");
	//
	static optRet ColorsBW() { return defOpt.ColorsBW(); }  ;		

	// Send a literal string to the optioins
	// This is deprecated.  Use str() instead.
	//
	static optRet literal(const char * sString) { return defOpt.literal(sString); }  ;	

    // Send a literal string to the optioins
	// This is deprecated.  Use str() instead.
	//
	static optRet lit(const char * sString) { return defOpt.literal(sString); }  ;	

	// Sends a literal string to the Window, Control, or Widget to set options that may not exist int the opt::namespace
	// For example, MyWiget(100,200,opt::Transparent() | opt::str("WidgetOption=Value") can tell the widget to perform an action
	// that cannot be done through opt() commands.
	//
	static optRet Str(const char * sString,const char * sDefinition = nullptr) { return defOpt.literal(sString,sDefinition); }  ;	

	// Sends a literal string to the Window, Control, or Widget to set options that may not exist int the opt::namespace
	// For example, MyWiget(100,200,opt::Transparent() | opt::str("WidgetOption=Value") can tell the widget to perform an action
	// that cannot be done through opt() commands.
	//
	static optRet S(const char * sString,const char * sDefinition = nullptr) { return defOpt.literal(sString,sDefinition); }  ;	

    static optRet Bool(const char * sString,bool bValue = true) { return defOpt.Bool(sString,bValue); } 
    static optRet Int(const char * sString,int iValue) { return defOpt.Int(sString,iValue); } 
    static optRet Long(const char * sString,int iValue) { return defOpt.Long(sString,iValue); } 
    static optRet Float(const char * sString,double fValue) { return defOpt.Float(sString,fValue); } 
//    static optRet StrStr(const char * sString,const char * sDefinition) { return defOpt.StrStr(sString,sDefinition); } 

    // Returns a blank options object (cwfOpt) 
    //
    static optRet NoOpt() { return defOpt.NoOpt(); }  // Return empty options

	// For Sliders, Horz() declares a horizontal slider.  This is the default.
	//
	static optRet Horz()			{ return defOpt.Horz();			};

	/// <summary>
	/// For Sliders, AsFloat() declares the Slider as a floating-point slider with a default range of 0.0-1.0. <para></para>
    /// Use the slider funtions GetPosf(), SetPosf(), SetRangef() for floating-point functions. <para></para>
    /// .<para></para>
    /// NewSliderf() can be used instead of specifying AsFloat.   Use opt::SetRangef() to set floating point range.
	/// </summary>
	/// <returns></returns>
	static optRet AsFloat()			{ return defOpt.AsFloat();			};

	// For Sliders, Horizontal() declares a horizontal slider.  This is the default.
	//
	static optRet Horizontal()		{ return defOpt.Horizontal();		};

	// For Sliders, Vert() declares a vertical slider. 
	//
	static optRet Vert()			{ return defOpt.Vert();			};

	// For Sliders, Vertical() declares a vertical slider. 
	//
	static optRet Vertical()		{ return defOpt.Vertical();		};

	// For sliders, ShowValue() causes the slider to show the value in the slider.
	// This creates a larger slider.  This can also be used with a Title set for the slider to
	// show the vlaue name.
	//
	// The Slider class can be overridden (or using a Message Handler), and GetPosString() can be defined to return the string of the value
	// when it does not correspond with the internal value.
	//
	static optRet ShowValue()		{ return defOpt.ShowValue();		};

	// WinToolTip.  For Windows slider, this will show the internal value above the handle when the handle is moved.
	//
	static optRet WinTooltip()		{ return defOpt.WinTooltip();		};

	// For Input boxes and general edit boxes, this will show the focus box defined by window. Otherwise it is hidden.
	//
	static optRet EnableFocusBox()	{ return defOpt.EnableFocusBox();	};

	// This initially disabled a control upon creation.  Use Enable() or Disable() function calls for controls that support this.
	//
	static optRet Disabled()		{ return defOpt.Disabled();		};

    // A low-level options - sets a NewWindow() call to be a non-primary window (which means it isn't counted in the Primary/Main Window count and will
    // allow GetEvent() and program end determination to not count this window.
    //
    // Otherwise, as a primary window, GetEvent() will not return if the window is open and visible.
    //
	static optRet _NonPrimary()				{ return defOpt._NonPrimary(); }  ;		


};
namespace kw = opt;

namespace imgOpt
{
    /// <summary>
    /// Sets the size of the image viewer on the desktop.  For example, 50% will use half the width and height for the image view window.
    /// <para></para>
    /// --> This is an optional parameter.
    /// <para></para>
    /// Percent(100) will maximize the window, using the entire desktop.
    /// <para></para>
    /// Percent can be between 25% and 100%. The default is approximately 75% of the desktop space, depending on the function.
    /// <para></para>
    /// --> Example:  Sagebox::ViewImage("My Image.jpg",Percent(50));
    /// </summary>
    /// <param name="iPercent"></param>
    /// <returns></returns>
    cwfOpt Percent(int iPercent)            ;

    /// <summary>
    /// Sets the "after" title for the Before & After Image Viewer.
    /// <para></para>
    /// This is an optional parameter.  If not used, a default label or no label will be chosen. 
    /// </summary>
    /// <param name="sTitle">Title/Label for After (i.e. resultant image) Window</param>
    /// <returns></returns>    cwfOpt BeforeTitle(const char * sTitle) ;
    cwfOpt AfterTitle(const char * sTitle)  ;

    /// <summary>
    /// Sets the image view to maximized, using the entire desktop to show the image. 
    /// </summary>
    /// <param name="bMaximized">True (default) for maximized</param>
    /// <returns></returns>
    cwfOpt Maximize(bool bMaximize = true)       ;

    /// <summary>
    /// Adds a ZoomBox for the image viewer for images using ViewImage()
    /// <para></para>
    /// The Zoom Box makes it easy to navigate, and zoom in and out of an image.   
    /// <para></para>
    /// The Zoom Box will change to the new window when a different window using the ImageView() function gains the focus.
    /// </summary>
    /// <param name="bZoombox"></param>
    /// <returns></returns>
    cwfOpt ZoomBox(bool bZoombox = true)           ;

    /// <summary>
    /// Tells the ImageView() function to fill the image in the window if the image is size is less than the available space in the window.
    /// </summary>
    /// <param name="bFillZoom"></param>
    /// <returns></returns>
     cwfOpt FillZoom(bool bFillZoom = true)         ;

     /// <summary>
     /// When an ImgView/BeforeAfter window is created, WaitforWindowClose() will display the window
     /// <para></para>
     /// and wait for the user to close the window before returning. 
     /// <para></para>--> This is the same as calling the object's WaitforClose() function just after creating the window.
     /// <para></para>Using imgOpt::WaitforClose() allows display of the object, waiting for a close, without creating an object (i.e. fire-and-forget).
     /// </summary>
     /// <param name="bWaitforClose"></param>
     /// <returns></returns>
     cwfOpt WaitforClose(bool bWaitforClose = true); 
};
}; // namespace Sage

#endif // _SageOpt_h_