//#pragma once
#if !defined(__CSage_H__)
#define __CSage_H__


// SageBox defines for .DLL output. 
// This is currently in-process. 

#ifdef SageDLL
#define dllvirt    virtual     // Make all interface functons virtual for returns objects. 

// Defines for virtual returns -- these are not used by must be defined by the DLL client

#define dllvoid {}
#define dllbool { return false; }
#define dllfloat { return 0.0; }
#define doolint { return 0; }
#else
#define dllvirt
#define dllvoid
#define dllbool
#define dllint
#define dllfloat
#endif

enum class GroupType
{
	Generic,
	RadioButton,
	EditBox,
	Undefined,
};

static constexpr int defColor = -1;
#define EmptyString(_x) (!_x || !*_x)

#define kStdMsg(_x)		nullptr
#define kStdPassThrough nullptr			// i.e. "@", or some other token to refer to passing the last error 
#define stdNoMsg		nullptr

#ifdef _WIN64
#define stdInt __int64
#else
#define stdInt int
#endif

#define stdTry		bool bError = false; char *___sErrMsg = nullptr; try{
#define stdAssert(_x,_msg) {  if (!((stdInt) (_x))) {/*  Sage::StopHere();*/ throw((char *) _msg); } }
#define stdCatch }catch(char * __y) {__y; bError = true; }
#define stdCatchSage }catch(char * __y) {__y; bError = true; Sage::sLastError = __y; }
#define stdAssertL stdAssert

// stdAsserts using gotos, to avoid using exceptions.

#if 0
#define stdTry		bool bError = false; char *___sErrMsg = nullptr;{ 
#define stdAssert(_x,_msg) {  if (!((int) (_x))) { Sage::StopHere();___sErrMsg = _msg; bError = true; goto __Err;; } }
#define stdCatch  } __Err:
#define stdAssertL(_x,_msg) _x		// versions for stdAssert embedded in Lambdas
#define stdCatchSage } __Err:  
#endif
#pragma warning( disable : 4996) 

 #define SageControlDelete public: void operator delete(void * ptr ) { ::operator delete(ptr); }
 #define SageControlDeletePrivate private: void operator delete(void * ptr ) { ::operator delete(ptr); }



// kAdvPublic:
// This is defalted to private to disallow CWindow class/object copying and deleting.
//
// ** CWindow and Controls are managed objects **
//
// the Cwindow or Control object is deleted automatically by SageBox when the window is closed.
// However, for subclassed windows with a lot of memory allocation, deleteObj() can be used to delete the
// object on-demand.
//
// kAdvPublic can be set to public, though the Copy Constructor should be monitored, as it could be disruptive.
//
#if defined(_CSageAdvanced)
	#define kAdvPublic	public
#else
	#define kAdvPublic	private
#endif
#include <Windows.h>
#include "CDevString.h"
#include "Sagestring.h"
#if 0
#define FailBox(_FunctionName,_Error) Sage::FailBoxMsg((char *) _FunctionName,(char *) _Error);
#else
#define FailBox(_FunctionName,_Error) { }
#endif

class CSageBitmap; 
namespace Sage
{
	

class CWindow;
enum class ThumbType
	{
		BestFit		   ,
		BestExactFit   ,
		ExactWidth	   ,
		ExactHeight	   ,
		Percentage	   ,
		MaxWidth	   ,
		MaxHeight	   ,
	};	
struct Deleter_t
{
	void * pObject;
	void  (*fDeleter)(void *);
	CWindow * cWindow;
};

class CWidget
{
public:
	virtual CWindow * GetWindow()				{ return nullptr;		}
	virtual bool SetLocation(int iX,int iY)		{ return false;			}	// This needs to return true if processed
	virtual bool isValid()						= 0;
	virtual bool Show(bool bShow)				{ return false;			}
	virtual bool Hide(bool Hide)				{ return false;			}
	virtual int GetID()							{ return -1;			}
	virtual const char * GetName()				{ return nullptr;		}
	virtual bool RecalcWindow()					{ return false;			}	// This needs to return true if processed
};

	struct fPoint_t
	{
		double fx;
		double fy;
	};

	struct fSize_t
	{
		double fx;
		double fy;
	};
	enum class ControlKey
	{
		Shift,
		Control,
		Menu,
		Pause,
		Capital,
		End,
		Home,
		Left,
		Up,
		Right,
		Down,
		Insert,
		Delete,
		Unknown,
	};

	struct RGBColor_t;
	enum class WidgetMsg
	{
		LButtonDown			,
		LButtonUp			,
		RButtonDown			,
		RButtonUp			,
		MButtonDown			,
		Char				,
		MouseWheel			,
		MouseMove			,
		MNButtonUp			,
		UnPressed			,

		Generic				,
	};
	struct WidgetPackage_t
	{
		void * cWidget;
		int iMessage;
	};

	void FailBoxMsg(char * sFunctionName,char * sError);

	struct RGBColor24
	{
		unsigned char Blue;
		unsigned char Green;
		unsigned char Red;
		DWORD toRGB() { return RGB(Red,Green,Blue); }
		RGBColor24 & fromRGB(DWORD dwColor) { Red = GetRValue(dwColor); Green = GetGValue(dwColor); Blue = GetBValue(dwColor); return *this; }
		DWORD operator * () { return toRGB();  }
		RGBColor24 & operator = (DWORD dwColor) { return fromRGB(dwColor);  }
	};
	struct RGBColor32
	{
		unsigned char Blue;
		unsigned char Green;
		unsigned char Red;
		unsigned char Mask;
		DWORD toRGB() { return RGB(Red,Green,Blue); }
		RGBColor32 & fromRGB(DWORD dwColor) { Red = GetRValue(dwColor); Green = GetGValue(dwColor); Blue = GetBValue(dwColor); Mask = 0xFF; return *this; }
		DWORD operator * () { return toRGB();  }
		RGBColor32 & operator = (DWORD dwColor) { return fromRGB(dwColor);  }
	};

typedef RGBColor24* pRGB24;
typedef RGBColor32* pRGB32;

	// WaitForEvent() Return values. 
	//
	// This is mostly TBD, showing some return issues with WaitForEvent.  Use some of these values in conjunction with parameters sent to WaitforEvent
	//
	enum class WaitEvent
	{
		EventFound,		// Event was found, everything is ok
		Error,			// Unknown Error
		TimerError,		// WaitforEvent is processing too fast -- probably returning witout waiting for events.  Use ShowTimerError() to force pop-up window to show the error without checking.
		TimeOut,		// Timed out based on MaxTimer setting (not a timer).  All events should still be checked. This can be used to end the loop if nothing happens in a specified period of time.
		WindowInvalid,	// The Window is Invalid -- this will turn into a TimerError after a certain point.  If you are having trouble with TimerError() also turn on ShowErrors()
						// i.e. "WaitforEvent(event::ShowErrors()) (ShowErrors also turns on ShowTimerError())
		WindowClosing	// The Window is closing.  There may be events to process, but this signals the Windows has been closed by the user or some process.  Use cWin->WindowClosing() to get the status
	};

	enum class BkMode
	{
		Transparent,
		Opaque
	};
	enum UpdateDirty
	{
		No = false,
		Yes = true,
	};

	enum class AutoUpdate
	{
		Disabled,
		Always,
		Passive,
	};
	enum class WindowScroll
	{
		Enabled,				// Causes the window to scroll when text written on the last line of the display
		Disabled,				// When text is written to the bottom of the screen, the window won't scroll.  This helps controls (i.e. buttons, sliders, etc.) keep the same background
		Auto,					// This is TBD and its effect is currently undefined.  use ENABLED or DISABLED only.
	};

	enum class Peek
	{
		No,
		Yes,
	};
	struct RawBitmap_t;
	struct RawBitmap32_t;
	void freeBitmap(Sage::RawBitmap_t & stBitmap);
	void freeBitmap(Sage::RawBitmap32_t & stBitmap);

	typedef bool PeekType2;
	extern const Sage::PeekType2 PeekX;
	extern const Sage::PeekType2 NoPeekX;

	enum class DialogResult
	{
		No		= 0,
		Yes		= 1,
		Ok		= 1,
		Cancel	= 2,
	};
	enum class Remove
	{
		No = false,
		Yes = true,
	};

struct HSLColor_t
{
public:
	double fH;
	double fS;
	double fL;

	void SetUndefined() { fH = fS = fL = -1; }
	bool Undefined() { return fH < 0 || fH > 1 || fS < 0 || fS > 1 || fL < 0 || fL > 1; }
};
struct RGBColor_t
{
public:
	int iRed;
	int iGreen;
	int iBlue;
public:
	DWORD toRGB();
	RGBColor_t & fromRGB(DWORD dwColor);
	DWORD operator * ();
	RGBColor_t & operator = (DWORD dwColor);
	operator int () { return (int) RGB(iRed,iGreen,iBlue); }
	operator DWORD () { return (DWORD) RGB(iRed,iGreen,iBlue); }
	RGBColor_t & Init();
	bool Undefined();
};

	struct FloatBitmap_t
	{
		float * fRed;
		float * fBlue;
		float * fGreen;
		float * f[3];
		int iWidth;
		int iHeight;
		int iTotalSize;

		void Delete();
		void Init();
		bool isValid();

		[[nodiscard]] RawBitmap_t ConverttoBitmap();
		[[nodiscard]] RawBitmap_t ConverttoBitmap(RawBitmap_t & stSource);
		float **  operator * ();

	};

	struct RawBitmap_t
	{
		int iWidth;
		int iHeight;
		int iWidthBytes;
		int iTotalSize;
		unsigned char * sMask;
		unsigned char * stMem;
		Sage::RGBColor24 * stRGB;	// This is a map to sData in RGB form that can be referred to more easily.
		void Delete();
		void Clean();
		int Init();
		bool ClearBitmap();
		unsigned char * operator * ();
		RawBitmap_t operator - ();

		// Check if empty -- it's only empty if everything is 0. Otherwise, it might be used as a bitmap or simply uninitialized

		bool isEmpty();
		bool ApplyMaskColor(RawBitmap_t & stDest,DWORD dwColor);
		bool ApplyMaskGraphic(Sage::RawBitmap_t & stBackground,Sage::RawBitmap_t & stDest);
		bool ApplyMaskGraphic(Sage::RawBitmap_t & stSource, Sage::RawBitmap_t & stBackground,Sage::RawBitmap_t & stDest);
		bool ApplyMaskGraphic(POINT pSourceStart, RawBitmap_t & stDest, POINT pDestStart, const SIZE & szSize); 
		bool ApplyMaskGraphicR(POINT pSourceStart, RawBitmap_t & stDest, POINT pDestStart, const SIZE & szSize); 
		bool ApplyMaskColor(RGBColor_t rgbColor,Sage::RawBitmap_t stMask,POINT pMaskStart, POINT pDestStart, SIZE szSize);
		bool ApplyMaskColorR(RGBColor_t rgbColor,Sage::RawBitmap_t stMask,POINT pMaskStart,POINT pDestStart, SIZE szSize);
		bool ApplyMaskColor(RGBColor_t rgbColor,Sage::RawBitmap_t stMask,POINT pDestStart);
		bool ApplyMaskColorR(RGBColor_t rgbColor,Sage::RawBitmap_t stMask,POINT pDestStart);

		bool CopyFrom(RawBitmap_t & stSource, POINT pDestStart = {0,0} , POINT pSourceStart = { 0,0 }, SIZE szSize = {0,0});
	//	bool Copyto(RawBitmap_t & stDest);
		bool Copyto(RawBitmap_t & stDest, POINT pSourceStart = { 0,0}, POINT pDestStart = { 0,0 }, SIZE szSize = { 0,0 });
		bool FillColor(Sage::RGBColor_t rgbColor, POINT pStart = { 0,0 }, SIZE szSize = { 0,0 });
		bool FillColor(DWORD dwColor, POINT pStart = { 0,0 }, SIZE szSize = { 0,0 });

		bool ReverseBitmap();



		void DeleteMask();
		void UpdateMaskMem();
		bool MasktoBmp(RawBitmap_t & stDest);
		bool isInvalid();
		SIZE GetSize();
		FloatBitmap_t ConverttoFloat();
		FloatBitmap_t ConverttoFloat(FloatBitmap_t & fBitmap);
	};

	struct GraphicButtonStyle
	{
		RawBitmap_t	stNormal		;
		RawBitmap_t	stHigh			;
		RawBitmap_t	stPressed		;
		RawBitmap_t	stDisabled		;

		// Values below this point TBD

		POINT szTextAreaStart;
		SIZE szTextAreaSize;
		char * sDefaultFont;
	};
	
	struct GraphicCheckboxStyle
	{
		RawBitmap_t	stNormal				;
		RawBitmap_t	stHigh					;
		RawBitmap_t	stChecked				;
		RawBitmap_t	stCheckedHigh			;
		RawBitmap_t	stDisabled				;
		RawBitmap_t	stDisabledChecked		;

		// Values below this point TBD

		POINT szTextAreaStart;
		SIZE szTextAreaSize;
		char * sDefaultFont;
	};
	struct RawBitmap32_t
	{
		int iWidth;
		int iHeight;
		int iWidthBytes;
		int iTotalSize;
		unsigned char * stMem;
		Sage::RGBColor32 * stRGB;	// This is a map to sData in RGB form that can be referred to more easily.

		void Delete();
		void Clean();	// Use carefully
		bool ClearBitmap();
		int Init();
	};
	enum class ButtonType
	{
		Button,
		Checkbox,
		RadioButton,
	};


struct SageString200
{
	char sStatic[201];
	bool bAlloc;
	int iStringSize;
	int iMemSize;
	char * spData;

public:
	char * GetMem(int iSize);
	char * SetString(char * sString);
	char * AddString(char * sString);
	void Delete();
	void Clear();

	char * operator * ();
	SageString200 & operator = (char * sString);
	SageString200 & operator += (char * sString);
	SageString200 & operator += (SageString200 & stString);
	SageString200 & operator <<  (char * sString);
};
struct stGraphicPlate_t
{
	char sName[100];
	int iPressMove;			// How for does it move when pressed?
	bool bStaticPress;		// Does it stay put when check (checkboxes only)
	Sage::RawBitmap_t		stBitmapNormal			;
	Sage::RawBitmap_t		stBitmapHigh			;
	Sage::RawBitmap_t		stBitmapPressed			;
	Sage::RawBitmap_t		stBitmapCheckedHigh		;
	Sage::RawBitmap_t		stBitmapDisabled		;
	Sage::RawBitmap_t		stBitmapDisabledChecked	;

	void Delete()
	{
		stBitmapNormal				 .Delete();
		stBitmapHigh				 .Delete();
		stBitmapPressed				 .Delete();
		stBitmapCheckedHigh			 .Delete();
		stBitmapDisabled			 .Delete();
		stBitmapDisabledChecked		 .Delete();
	}
};

enum class ButtonStyle
{
	StaticButton,
	Checkbox,
};

struct stButtonBitmaps_t
{
	Sage::RawBitmap_t	stNormal			;
	Sage::RawBitmap_t	stHigh				;
	Sage::RawBitmap_t	stPressed			;
	Sage::RawBitmap_t	stDisabled			;
	Sage::RawBitmap_t	stCheckedHigh		;
	Sage::RawBitmap_t	stDisabledChecked	;
public:
	void Delete();
	void Init();
};

struct stComponentBitmap_t
{
	enum class ControlType
	{
		Unknown,
		Button,
		Checkbox,
		GraphicButton,		// An Oversight -- the idea of ComponentBitmap and GraphicButton need to be combined 
							// Or the ControlType class needs to moved out to something more generic.  For now, this is just
							// to fit it in for the addition of the Graphic Button type
		GraphicBitmap,
	};
	Sage::SageString stName;
	Sage::SageString stOrgName;
	char sDefaultStyle[50];
	char sDefaultColorStyle[50];
	bool bDefaultStyle;
	ControlType eControlType;
	Sage::RawBitmap_t	stBackground; 
	stButtonBitmaps_t stBitmaps;

	struct stColorStyles_t
	{
		char sStyleName[50];
		Sage::RGBColor_t rgbButton			;
		Sage::RGBColor_t rgbOutline			;
		Sage::RGBColor_t rgbTextNormal		;
		Sage::RGBColor_t rgbTextHigh		;
		Sage::RGBColor_t rgbTextPressed		;
		Sage::RGBColor_t rgbTextDisabled	;
	};
	int iMaxColorStyles;
	int iColorStyles;
	stColorStyles_t * stColorStyles;
	struct stSection
	{
		SIZE szTextOffset;
		Sage::RGBColor_t rgbStrokeColor			;				// $$ This and others, TBD
		Sage::RawBitmap_t stBitmapStroke		;
		Sage::RawBitmap_t stBitmapGlow			;
		Sage::RawBitmap_t stBitmapGloss			;
		Sage::RawBitmap_t stBitmapGraphic		;
		Sage::RawBitmap_t stBitmapGraphicMask	;
		Sage::RawBitmap_t stBitmapOutline		;
		Sage::RawBitmap_t stBitmapShadow		;
		Sage::RawBitmap_t stBitmapButton		;
	public:
		void Delete();
		void Init();
	};

	stSection stNormal;
	stSection stHigh;
	stSection stPressed;
	stSection stCheckedHigh;
	stSection stDisabled;
	stSection stDisabledChecked;


	int iDefaultPad;
	int iDefaultPadY;
	char sDefaultFont[100];

	int iLeftWidth;			// Width of Bitmap part to the left
	int iRightWidth;		// Width of Bitmap part to the right
	int iMinWidth;			// Minimum with of bitmap (Left/Right width may ary from Text Area)

	// These are for reference & easy acesss, and are filled on the PGR Load. The absolute reference is any of the loaded bitmaps.

	int iBitmapWidth		;
	int iBitmapHeight		;
	int iBitmapWidthBytes	;

	// Colors are also for reference, storage purposes -- not part of the PGR

	Sage::RGBColor_t rgbButtonColor			;
	Sage::RGBColor_t rgbOutlineColor		;
	Sage::RGBColor_t rgbStrokeColor			;
	Sage::RGBColor_t rgbBackgroundColor		;

	// More just-for-reference items

	Sage::RGBColor_t	rgbTextNormal			;
	Sage::RGBColor_t	rgbTextHigh				;
	Sage::RGBColor_t	rgbTextPressed			;
	Sage::RGBColor_t	rgbTextDisabled			;
	struct stZone
	{
		SIZE szTopLeft;
		SIZE szSize;
	};
	stZone stTextZone;
	stZone stMiddleZone;

public:
	void Delete();
	void Init();
	void SetBitmaps(stButtonBitmaps_t & stBitmaps);
	void SetColors(Sage::RGBColor_t rgbButtonColor, Sage::RGBColor_t rgbStrokeColor, Sage::RGBColor_t rgbBackgroundColor);
	void SetBackgroundColor(Sage::RGBColor_t rgbBackgroundColor);
	bool SetColorStyle(char * sColorStyle);
	bool SetBackgroundBitmap(Sage::RawBitmap32_t & stSource,bool bDeleteSource = false);
};
struct stGraphicBitmap_t
{
	SageString stName;
	SageString stOrgName;
	Sage::RawBitmap_t			stBitmapGraphic		   ;
	Sage::RawBitmap_t			stBitmapMask		   ;
	Sage::RawBitmap_t			stBitmapShadowMask	   ;

	Sage::RawBitmap_t			stOutput			   ;
	POINT pCenter;
public:
	void Delete()
	{
		stBitmapGraphic		.Delete();
		stBitmapMask		.Delete();
		stBitmapShadowMask	.Delete();
		
		stOutput			.Delete();
		stName				.Delete();
		stOrgName			.Delete();
	}
};
struct stGraphicButton_t
{
	bool bIsCopy;				// When true, this is a copy so Delete won't delete certain things
	int iUsePlate;					// Dynamically used to reference a plate when specifying a button and potential plate. 
	bool bUsePlate;					// If plate was specified
	ButtonStyle eButtonStyle;
	Sage::SageString stName; 
	Sage::SageString stOrgName; 
	char		sFont[101];

	// Note: Add any new bitmaps, allocated structures, etc. to CopyClean(), etc.

	Sage::stButtonBitmaps_t	stButtonBitmaps				;

	Sage::RawBitmap_t			stBitmapNormal			;
	Sage::RawBitmap_t			stBitmapHigh			;
	Sage::RawBitmap_t			stBitmapPressed			;
	Sage::RawBitmap_t			stBitmapCheckedHigh		;
	Sage::RawBitmap_t			stBitmapDisabled		;
	Sage::RawBitmap_t			stBitmapDisabledChecked	;
	Sage::RawBitmap_t			stBitmapShadowMask		;
	Sage::RawBitmap_t			stBitmapMask			;
	Sage::RawBitmap_t			stBitmapMaskPressed		;
	Sage::RawBitmap_t			stBitmapMaskHigh		;

	bool bCheckbox;				// Checkbox or RadioButton
	int iBitmapWidth;
	int iBitmapHeight;
	int iMouseZoneX;
	int iMouseZoneY;
	int iMouseZoneWidth;
	int iMouseZoneHeight;

	int iTextRegionStartX;
	int iTextRegionStartY;
	int iTextRegionWidth;
	int iTextRegionHeight;
	int iTextRegionPressMove;
	int iTextZoneStaticPress;

	DWORD dwTextNormal			;
	DWORD dwTextHighlighted		;
	DWORD dwTextPressed			;
	DWORD dwTextCheckedHigh		;
	DWORD dwTextDisabled		;

	bool bAllowText;
	int iGraphicPlates			;	 // Number of graphic plates defined by the user.
									 // pre-defined 0 (or omission) = the button with no plate placement.


	int iGraphicPlateStartX		;
	int iGraphicPlateStartY		;

	stGraphicPlate_t		* stGraphicPlates;
	void DeletePlates();
	void DeleteBitmaps();
	void DeleteAll();
	int GetPlateID(char * sPlate);	// Returns -1 for no plate
	void SetButtonBitmaps(Sage::stButtonBitmaps_t & stBitmaps);
	void CopyClean(Sage::stGraphicButton_t & stButton);
};


enum class ControlStyles
{
	Uninitialized,
	Button,
	Bitmap,
//	Checkbox,
};
enum class ControlSubStyles
{
	Unknown,
	GraphicButton,
	ComponentButton,
	ComponentCheckbox,
	GraphicBitmap,
	RGB32Bitmap,
};

	[[nodiscard]] RawBitmap_t ReadBitmap(char * sFile, bool * bSucceeded = nullptr);

	[[nodiscard]] RawBitmap32_t BMPtoBitmap32(unsigned char * sBMP);
	[[nodiscard]] RawBitmap_t BMPtoBitmap(unsigned char * sBMP);
	bool BMPtoBitmap(unsigned char * sBMP,RawBitmap_t & stBitmap);
	void DeleteBitmap(Sage::RawBitmap_t & stBitmap);
	void DeleteBitmap(Sage::RawBitmap32_t & stBitmap);
	[[nodiscard]] RawBitmap_t CreateBitmap(unsigned char * sBitmapMem,int iWidth,int iHeight);
	[[nodiscard]] RawBitmap_t CreateBitmap(int iWidth,int iHeight,bool * bError = nullptr);
	[[nodiscard]] RawBitmap_t CreateBitmap(SIZE szSize,bool * bError = nullptr);
	[[nodiscard]] RawBitmap32_t CreateBitmap32(unsigned char * sBitmapMem,int iWidth,int iHeight);
	[[nodiscard]] RawBitmap32_t CreateBitmap32(int iWidth,int iHeight,bool * bError = nullptr);
	bool CreateBitmap(int iWidth,int iHeight,RawBitmap_t & stBitmap);
	bool CreateBitmap(RawBitmap_t & stSource,RawBitmap_t & stBitmap);
	[[nodiscard]] RawBitmap_t CreateBitmap(RawBitmap_t & stSource,bool * bSuccess = nullptr);
	[[nodiscard]] RawBitmap_t CreateBitmap(CSageBitmap & cBitmap,bool * bSuccess = nullptr);
	bool CreateBitmap(int iWidth,int iHeight,RawBitmap32_t & stBitmap);
	bool CreateBitmap(RawBitmap32_t & stSource,RawBitmap32_t & stBitmap);
	bool FillBitmap(RawBitmap_t & stBitmap,RGBColor_t rgbColor);
	bool CopyBitmap(RawBitmap_t & stSource,POINT pSourceStart, RawBitmap_t & stDest, POINT pDestStart, const SIZE & szSize); 
	bool CopyBitmap(RawBitmap_t & stSource,RawBitmap_t & stDest); 
	bool MasktoBmp(RawBitmap_t & stSource,RawBitmap_t & stDest); 
	[[nodiscard]] RawBitmap_t MasktoBmp(RawBitmap_t & stSource,bool * bSuccess = nullptr); 
	bool CloneBitmap(RawBitmap_t & stSource,RawBitmap_t & stDest); 
	bool CloneBitmap(RawBitmap32_t & stSource,RawBitmap_t & stDest); 
	bool ApplyMaskColor(Sage::RawBitmap_t & stSource,Sage::RawBitmap_t & stDest,Sage::RawBitmap_t stMask,Sage::RGBColor_t rgbColor);
	bool ApplyMaskColor(Sage::RawBitmap_t & stSource,Sage::RawBitmap_t & stDest,DWORD dwColor);
	
	
	
	bool ApplyMaskGraphic8(Sage::RawBitmap_t & stSource,Sage::RawBitmap_t & stBackground,Sage::RawBitmap_t & stDest,Sage::RawBitmap_t stMask);
	bool ApplyMaskGraphic(Sage::RawBitmap_t & stSource,Sage::RawBitmap_t & stBackground,Sage::RawBitmap_t & stDest,Sage::RawBitmap_t stMask);
	bool ApplyMaskGraphic(Sage::RawBitmap_t & stSource,Sage::RawBitmap_t & stBackground,Sage::RawBitmap_t & stDest);
	bool ApplyMaskColor(RGBColor_t rgbColor,Sage::RawBitmap_t stMask,POINT pMaskStart, RawBitmap_t & stDest, POINT pDestStart, const SIZE & szSize); 
	bool ApplyMaskColorR(RGBColor_t rgbColor,Sage::RawBitmap_t stMask,POINT pMaskStart, RawBitmap_t & stDest, POINT pDestStart, const SIZE & szSize); 
	
	bool ApplyMaskColor(RGBColor_t rgbColor,Sage::RawBitmap_t stMask, RawBitmap_t & stDest, POINT pDestStart); 
	bool ApplyMaskColorR(RGBColor_t rgbColor,Sage::RawBitmap_t stMask, RawBitmap_t & stDest, POINT pDestStart); 

	bool ApplyMaskGraphic(RawBitmap_t & stSource,POINT pSourceStart, RawBitmap_t & stDest, POINT pDestStart, const SIZE & szSize); 
	bool ApplyMaskGraphicR(RawBitmap_t & stSource,POINT pSourceStart, RawBitmap_t & stDest, POINT pDestStart, const SIZE & szSize); 
	int CopyStringMax(const char * sSource,char * sDest,int iMaxLen);
	void StopHere();
	bool toHex(char * sText,DWORD & dwValue);
	unsigned int toHex(char * sText,bool * bFound);
	char * SkipWhiteSpace2(char * sPlace);
	char * SkipAlphaNumeric(char * sPlace);
	char * SkipTrailingSpace(char * sPlace);
	bool StripWhiteSpace(char * sSource,char * sDest);
	bool StripLeadingCRLF(char * sSource,char * sDest);
	int FindIntValues(char * sString,long * iValues,int iMaxValues);
	bool ReplicateBitmapLineVert(Sage::RawBitmap_t & stSource,Sage::RawBitmap_t & stDest,int iStartSourceX,int iStartDestX,int iLength);
	extern RGBColor_t rgbUndefined;
	BITMAPINFOHEADER CreateBitmapInfoHeader32(int iWidth,int iHeight);
	BITMAPINFOHEADER CreateBitmapInfoHeader24(int iWidth,int iHeight);
	BITMAPINFO CreateBitmapInfo24(int iWidth,int iHeight);
	BITMAPINFO CreateBitmapInfo32(int iWidth,int iHeight);
	int FindIntValues(char * sString,long * iValues,int iMaxValues);
	int FindIntValues(char * sString,int * iValues,int iMaxValues);
	namespace Win
	{
		void ScreentoClient(HWND hWndParent,RECT & rRect);
	} // namespace Sage::Win
}	// namespace Sage

#endif // __CSage_H__