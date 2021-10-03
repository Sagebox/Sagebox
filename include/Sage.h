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
#if !defined(__CSage_H__)
#define __CSage_H__
#pragma warning(push)
#pragma warning(disable : 26451)    // Mute Microsoft "warning" about overflow (which, imo, is far too broad)
#define EmptyString(_x) (!_x || !*_x)

#define kStdMsg(_x)		nullptr
#define kStdPassThrough nullptr			// i.e. "@", or some other token to refer to passing the last error 
#define stdNoMsg		nullptr

#define __SageInline  __forceinline
#define SupportGDI

#ifdef SupportGDI
#include <windows.h>
#include <gdiplus.h>
#endif

#include "CMemClass.h"
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
	Slider,
	Undefined,
};

// Supported Image Types. 
//
// PNG, TIFF, Support to be added soon.
//
enum class ImageType
{
    Jpeg,
    Bitmap,
    PNG,
    Unknown,
};
enum class ImageStatus
{
    Ok,
    EmptyPath,
    NotFound,
    TooLarge,
    UnspportedFormat,
    Corrupted,
    InputMemoryEmpty,
    UnknownError
};


#ifdef _WIN64
#define stdInt	//__int64
#else
#define stdInt //__int64
#endif

#define stdTry		bool bError = false; const char *___sErrMsg = nullptr; try{
#define stdAssert(_x,_msg) {  if (!(stdInt (_x))) {/*  Sage::StopHere();*/ throw((const char *) _msg); } }
#define stdCatch }catch(const char * sErrMsg) {___sErrMsg = sErrMsg; bError = true; }
#define stdCatcher }catch(const char * sErrMsg) 
#define stdCatchSage }catch(const char * __y) {__y; bError = true; Sage::sLastError = __y; }
#define stdReturnMsg(_x) _x = ___sErrMsg; return (!bError); 
#define stdReturn return (!bError); 
#define stdAssertL stdAssert


// stdAsserts using gotos, to avoid using exceptions.

#if 0
#define stdTry		bool bError = false; char *___sErrMsg = nullptr;{ 
#define stdAssert(_x,_msg) {  if (!((int) (_x))) { Sage::StopHere();___sErrMsg = _msg; bError = true; goto __Err;; } }
#define stdCatch  } __Err:
#define stdAssertL(_x,_msg) _x		// versions for stdAssert embedded in Lambdas
#define stdCatchSage } __Err:  
#endif
//#pragma warning( disable : 4996) 

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

namespace Sage
{
    using SageEventHookFunc = bool(*)(HWND hwnd,void * pData,int64_t ullExtra);

    class CBitmap; 
    class CSlider;
    class CButton;
	constexpr const char * BoolString(bool bValue) { return bValue ? "true" : "false"; };
	constexpr const char * BoolStringU(bool bValue) { return bValue ? "True" : "False"; };
	constexpr const char * BoolStringUU(bool bValue) { return bValue ? "TRUE" : "FALSE"; };
	constexpr const char * BoolStringY(bool bValue) { return bValue ? "yes" : "no"; };
	constexpr const char * BoolStringYU(bool bValue) { return bValue ? "Yes" : "No"; };
	constexpr const char * BoolStringYUU(bool bValue) { return bValue ? "YES" : "NO"; };

    // Keyboard accelerator messages -- still to be completely defined. 
    // Right now, it a placeholder for the ^C keyboard accelerator when the Control-C Exit option is on
    // 
    // It's place here in case it interefere's with anyone's accelerator OR if the accelerator is replaced, then 
    // it can be added back in as Sage::AcelMessage::CtrlCBreak

    enum class AcelMessage : int
    {
        CtrlCBreak      = 0x1785,     // An arbitrary number 
    };

    // Temporary Structure TBD
    //
    struct stControlLabel_t
    {
        HFONT hFont; 
        POINT pLoc;
        SIZE szSize; 
    };

    static constexpr int defColor = -1;

    enum class SysMenuItems
    {
        SageboxID             = 0x10,     // generic Sagebox Info, versioning
        TerminateProcess      = 0x20,     // Terminate Program (i.e. for programs that hang or are unresponsive)
        ShowProcessWin        = 0x30,     // Show process window debug (creates it if non-existent)
    };
	enum class Peek : bool
	{
		No = false,
		Yes = true,
	};
	enum class ThreadStatus
    {
        Running, 
        Suspended,
		Unknown,
    };


    // SageEvent -- a list of all events that pass through GetEvent() (possibly others) 
    //
    // *** This is in-progress ***
    //
    // This enum class is used to express single or multiple events when communicating with Sagebox for various reasons.
    // For example, when clearing the event queue, ClearCent(SageEvent::WindowResize) can be used to remove a pending window resize without
    // affecting other events in the queue. 
    //
    // ClearEvent(SageEvent::All) or ClearEvents() can be used to clear any pending event in the queue. 
    //
    // Note:  When ClearEvent() is used, it clears the events for the event request, but it does not clear the pending status.
    //        Therefore, if an event is in the queue, GetEvent() will still return, but there will be no event found. 
    // 
    // ClearEvent() is not applicable to a Window Close Pending.  This must be reset independently with ResetWindowClosing()
    //
    //
    // Possible Additions include Event groups such as "AnyMouse", "AnyEditBox", "AnyButton", etc. to specify any type of event -- for example AnyMouse would
    // include any mouse movement, presses, unpresses, etc.  -- This is TBD
    //
    enum class SageEvent
    {
        All,
        WindowResize,
        ButtonPress,
        ButtonUnpress,
        MouseMoved,
        MouseLButtonDown,
        MouseRButtonDown,
        CloseWindowPressed,

        // This list in-progress.  Not all events may be processed with ClearEvent() -- WindowResize is currently the only one intended, but others will follow. 

    };

	// Window Event Signals that can be processed.
	//
	// $$ Right now, ONLY WindowClose is supported. 
	//

	enum class SignalEvents
	{
		// Set Signal to true if WindowClose Button pressed or Window Close status is true
		//
		WindowClose,

		// Expected to be supported
//		MouseWheel,		
//		MouseClick,
//		MouseRClick,
//		MouseMove,
//		others TBD
		NoEvent,	
	};



	// An Signal Event structure.
	//
	// This is still being developed and may change.
	// stEventSignal has some routines and some data it can also set for more advanced events.
	// For instance, MouseWheel() will set the iData value for the MouseWheel value. 
	//
	// pPtr is still TBD and may be removed if there is no useful purpose for it.  For now, its a placeholder.
	//
	// All Event signals can also be given a simple bool and an altdata pointer to fill these value rather than the 
	// Event Signal.  For example, a Windows Close event signal can just fill a simple bWindowsClosed or bAbort signal used by
	// multiple signals.  
	// 
	// stEventSignal is used for clean code and self-documenting code, but is not required to use signals.
	//
	// Signals are still under development.
	//
	struct Signal
	{
		bool bSignal;		// Set true when the event occurs 
		int iData;			// any additional data set by the event
		void * pPtr;		// any addition Pointer set by the event
		__forceinline bool GetSignal()	{ bool bReturn = bSignal; bSignal = false; return bReturn; };
		__forceinline int GetData()		{ return iData; };
		__forceinline void * GetPtr()	{ return pPtr; };
	};
	struct SliderSignal
	{
		bool bMoved;
		int iPosition;
		constexpr bool GetSignal() { bool bReturn = bMoved; bMoved = false; return bReturn; };
		constexpr int GetPos() { return iPosition; };
	};

    typedef void(*SliderSignalCallback)(SliderSignal & stSignal,void * pData)  ;

    class CSignal
    {
    public:
        HWND hWnd;   
        HWND hWndParent;   
    };
    // CSliderSignal -- contsainer class for SLiderSignal.  See notes for the "signal" class object in CSlider.cpp
    //
    // Using signals is a much faster way to access control data and events than using the regular class, which must validate 
    // the control. 
    //
    // Using the default signal (i.e. in the control class) guarantees safe pointer access as long as the control exists.
    // If a new signal is created to bypass the control's internal signal, then it is up to the user-program to make sure
    // the Signals are not used if the control is deleted. 
    //
    // Documentation TBD
    //
    class CSliderSignal : public CSignal
    {
    private:
        CSlider * cSlider;
   public:
        void Init(CSlider & cSlider);
        SliderSignal stSignal; 
        CSliderSignal(CSlider & cSlider);
        CSliderSignal();
        ~CSliderSignal();

        bool KillSignal();
        bool RestoreSignal();
        __forceinline bool Moved(Peek peek = Peek::No) { bool bReturn = (bool) stSignal.bMoved; stSignal.bMoved &= *(bool *)&peek; return bReturn; }
        __forceinline bool Moved2(Peek peek = Peek::No) { bool bReturn = (int) stSignal.bMoved; if (peek == Peek::No) stSignal.bMoved = false; return bReturn; }
        __forceinline bool Moved(int & iPosition,Peek peek = Peek::No) { bool bReturn = (bool)stSignal.bMoved; if (bReturn) iPosition = stSignal.iPosition;  stSignal.bMoved &= *(bool *)&peek; return bReturn; }
        __forceinline bool GetPos() { return stSignal.iPosition; }
        
        // Setcallback() -- Sets a call back for the event signal.  This is a low-level function for easy prototyping
        // An equivalent and safer method is to override the button class and override the event functions.
        //
        // This can be much faster to prototype development, but is not meant as permanent code in the program.
        //
        // pData is your object or whatever data you want to send the signal function.  It can be omitted.
        //
        // The call back function should be in this form: 
        //
        //void SliderButtonCallback(SliderSignal & stSignal,void * pData) { ... code here ... }
        //
        bool SetCallback(SliderSignalCallback fCallback,void * pPrivateData = nullptr);
    };
	struct EditBoxSignal
	{
		bool bCRPressed; 
		char sText[40];
		constexpr bool GetSignal() { bool bReturn = bCRPressed; bCRPressed = false; return bReturn; };
		constexpr const char * GetText() { return (const char *) sText; }
	};

    struct SliderStruct
    {
        bool bMoved;
        int iPosition;
        int iID; 
    };
struct ButtonSignal
{
	// bPressed -- true when the button is pressed, until reset.  See GetSignal()
	//
	bool bPressed;
	
	// bChecked -- true of checkbox or radiobutton is chcked or active (i.e. radio button set)
	// --> important note -- This vsalue is only filled (and valid) when the checkbox or radiobutton is pressed.
	// Initial settings or changes outside of the actual press with the Signal active are not registered.
	//
	bool bChecked; 

	// Returns true if Button was pressed, returning false for subsequent calls until the button is pressed again.
	// This is the equivalent of checking bPressed and then setting it to false directly afterwards, ensuring only
	// one TRUE reurn per-button press.
	// 
	// --> GetSignal(MyButtonSignal.bPressed) can be used instead of the member function.
	// 
	constexpr bool GetSignal() { bool bReturn = bPressed; bPressed = false; return bReturn; };
    __forceinline bool Pressed(Peek peek = Peek::No) { bool bReturn = (bool) bPressed; bPressed &= *( bool *)&peek; return bReturn; }
    __forceinline bool Checked(Peek peek = Peek::No) { bool bReturn = (bool) bChecked; bChecked &= *( bool *)&peek; return bReturn; }

};

typedef void(*ButtonSignalCallback)(ButtonSignal & stSignal,void * pData)  ;

// CButtonSignal -- contsainer class for ButtonSignal.  See notes for the "signal" class object in Cbutton.cpp
//
// Using signals is a much faster way to access control data and events than using the regular class, which must validate 
// the control. 
//
// Using the default signal (i.e. in the control class) guarantees safe pointer access as long as the control exists.
// If a new signal is created to bypass the control's internal signal, then it is up to the user-program to make sure
// the Signals are not used if the control is deleted. 
//
// Documentation TBD
//
class CButtonSignal : public CSignal
    {
    private:
        CButton * cButton;
   public:
       void Init(CButton & cButton);
        ButtonSignal stSignal; 
        CButtonSignal(CButton & cButton);
        CButtonSignal();
        ~CButtonSignal();

        bool KillSignal();
        bool RestoreSignal();
        __forceinline bool Pressed(Peek peek = Peek::No) { bool bReturn = (bool) stSignal.bPressed; stSignal.bPressed &= *( bool *)&peek; return bReturn; }
        __forceinline bool Pressed2(Peek peek = Peek::No) { bool bReturn = (bool) stSignal.bPressed; if (peek == Peek::No) stSignal.bPressed = false; return bReturn; }
        __forceinline bool Checked(Peek peek = Peek::No) { bool bReturn = (bool) stSignal.bChecked; stSignal.bChecked &= *( bool *)&peek; return bReturn; }

        // Setcallback() -- Sets a call back for the event signal.  This is a low-level function for easy prototyping
        // An equivalent and safer method is to override the button class and override the event functions.
        //
        // This can be much faster to prototype development, but is not meant as permanent code in the program.
        //
        // pData is your object or whatever data you want to send the signal function.  It can be omitted.
        //
        // The call back function should be in this form: 
        //
        //void ButtonCallback(ButtonSignal & stSignal,void * pData) { ... code here ... }
        //
        bool SetCallback(ButtonSignalCallback fCallback,void * pPrivateData = nullptr);

    };
constexpr bool GetSignal(SliderSignal stSignal) { bool bReturn = stSignal.bMoved; stSignal.bMoved = false; return bReturn; };
constexpr bool GetSignal(ButtonSignal stSignal) { bool bReturn = stSignal.bPressed; stSignal.bPressed = false; return bReturn; };
constexpr bool GetSignal(EditBoxSignal stSignal) { bool bReturn = stSignal.bCRPressed; stSignal.bCRPressed = false; return bReturn; };
constexpr bool GetSignal(bool & bSignal) { bool bReturn = bSignal; bSignal = false; return bReturn; };
enum class BorderType
{
	Line,
	Depressed,
	None,
}; 
enum class LabelJust
{
	None,
	Top,
	Bottom,
	Left,
	Right,
	SetXPos,
};	

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
enum class ResizeType
	{
		Exact		   ,        // Sizes to exact size specified
		BestFit		   ,        // Sets size to whichever proportional dimension fits within max width/height given, but will only shrink the image (will not enlarge)
		BestExactFit   ,        // Sets size to whichever proportional dimension fits within max width/height given; this will enlarge or shrink the image to fit.
		ExactWidth     ,        // Sets image to the width specified, keeping the image proportions (width is ignored)
		ExactHeight	   ,        // Sets image to the height specified, keeping the image proportions (width is ignored)
		Percentage	   ,        // Sets the image to the percentage specified (specified in the Width parameter)
		MaxWidth	   ,        // Sets image to the height width, keeping the image proportions (width is ignored), but will only shrink the image (it won't enlarge it)
		MaxHeight	   ,        // Sets image to the height specified, keeping the image proportions (width is ignored), but will only shrink the image (it won't enlarge it)
	};	
struct Deleter_t
{
	void * pObject;
	void  (*fDeleter)(void *);
	CWindow * cWindow;
};

class CWidget
{
	bool bTransparent = false;					// True if the widget is currently set as Transparent
public:
	virtual CWindow * GetWindow()				{ return nullptr;		}
	virtual bool SetLocation(int iX,int iY)		{ return false;			}	// This needs to return true if processed
	virtual bool isValid()						= 0;
	virtual bool Show(bool bShow)				{ return false;			}
	virtual bool Hide(bool Hide)				{ return false;			}
	virtual int GetID()							{ return -1;			}
	virtual const char * GetName()				{ return nullptr;		}
	virtual bool RecalcWindow()					{ return false;			}	// This needs to return true if processed
	virtual bool UpdateBg(bool bUpdate = true)  { return false;			}	// Update background if transparent 
	virtual bool isTransparent()				{ return false;			}	// Widget returns true if it is transparent (not if it supports it)
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
	struct RGBColorA_t;
    class CRgb; 

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
        bool bSendPoll;
	};

	void FailBoxMsg(char * sFunctionName,char * sError);

	struct RGBColor24
	{
	__forceinline RGBColor24 & operator *= (const RGBColor24 & r2) {	Red		= (unsigned char) ((int) Red*(int) r2.Red/255);
																		Green	= (unsigned char) ((int) Green*(int) r2.Green/255); 
																		Blue	= (unsigned char) ((int) Blue*(int) r2.Blue/255); 
																		return *this; }
	__forceinline RGBColor24 operator * (const RGBColor24 & r2) { return {	(unsigned char) ((int) Red*(int) r2.Red/255), 
																			(unsigned char) ((int) Green*(int) r2.Green/255), 
																			(unsigned char) ((int) Blue*(int) r2.Blue/255) }; }

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

	// Auto Update Type:
	//
	// On: Updates 10-20 millisconds.  Use an extra Update() at the end of all routines to make sure it is updated.
	// Off:  Update() must be used to update the window.
	// Immediate:  Updates after any action that outputs to the screen, no matter how small. 
	//             *** Warning: Immediate can slow down your program.  Use for diagnostics, status windows or other places where you don't want to do an Update(), but 
	//             *** also don't care about it performing an Update() at every step
    //
    // $$ DO NOT CHANGE THE ORDER OR DELETE ITEMS -- Look at uses of AutoUpdateType as integers, such as CPasWindow::UserCreateWindow(), etc.
    //
	enum class AutoUpdateType
	{
		//  Update() must be used to update the window.
		//
		Off,

		// Updates 10-20 millisconds.  Use an extra Update() at the end of all routines to make sure it is updated.
		//
		On,

        // Same as On, but installs a timer to ensure update status.  This is primarily used for Quick C++-type applications.
        OnTime,

		// Updates after any action that outputs to the screen, no matter how small. 
		// *** Warning: Immediate can slow down your program.  Use for diagnostics, status windows or other places where you don't want to do an Update(), but 
		// *** also don't care about it performing an Update() at every step
		//
		Immediate, 



	};
 

    
    // Names opt:: values that can be referred to internally to ensure proper spelling 
    // since they are passed as text.   Mostly, these are internal values that are not public
    // in opt:: and need to be specified directly.
    //
    // note:  At some point, all opt:: names are expected to be replaced with an integer identifier.
    //
    class OptName
    {
    public:
        static constexpr const char * __CenterX         = "__CenterX";      // Internal X range for centering (overriding default window size range
        static constexpr const char * __CenterY         = "__CenterY";      // Internal Y range for centering (overriding default window size range
        static constexpr const char * CenterXY          = "CenterXY";      
        static constexpr const char * CenterX           = "JustCenterX";   
        static constexpr const char * CenterY           = "JustCenterY";   
        static constexpr const char * Center            = "CenterY";   
        static constexpr const char * Checkbox          = "Checkbox";       // $$ May not be part of opt::namespace and used as opt::Str("<id>"), i.e. opt::str("checkbox")., etc.
        static constexpr const char * RadioButton       = "RadioButton";    // $$ May not be part of opt::namespace and used as opt::Str("<id>"), i.e. opt::str("checkbox")., etc.
        static constexpr const char * JustCenter        = "JustCenter";    
        static constexpr const char * JustCenterX       = "JustCenterX";   
        static constexpr const char * JustCenterY       = "JustCenterY";   
        static constexpr const char * JustRight         = "JustRight";     
        static constexpr const char * JustLeft          = "JustLeft";      
        static constexpr const char * JustTopRight      = "JustTopRight";  
        static constexpr const char * JustTopLeft       = "JustTopLeft";   
        static constexpr const char * JustBottomRight   = "JustBottomRight"; 
        static constexpr const char * JustBottomLeft    = "JustBottomLeft";  
        static constexpr const char * JustBottomCenter  = "JustBottomCenter";
        static constexpr const char * JustTopCenter     = "JustTopCenter";   
        static constexpr const char * JustTop           = "JustTop";         
        static constexpr const char * JustBottom        = "JustBottom";    
        static constexpr const char * TextCenterX       = "TextCenterX";   
        static constexpr const char * TextCenterY       = "TextCenterY";   
        static constexpr const char * TextCenter        = "TextCenter";   
        static constexpr const char * TextCenterXY      = "TextCenterXY";   
        static constexpr const char * TextRight         = "TextRight";   
        static constexpr const char * TextLeft          = "TextLeft";   
        static constexpr const char * TextTop           = "TextTop";   
        static constexpr const char * TextBottom        = "TextBottom";   
        static constexpr const char * Default           = "Default";   
        static constexpr const char * Horz              = "Horz";   
        static constexpr const char * Vert              = "Vert";   
        static constexpr const char * NoClose           = "NoClose";   
        static constexpr const char * Label             = "Label";   
        static constexpr const char * NoCancel          = "NoCancel";   
        static constexpr const char * NoAutoHide        = "NoAutoHide";   
        static constexpr const char * NoSizing          = "NoSizing";   
        static constexpr const char * ToolWindow        = "ToolWindow";   
        static constexpr const char * Modal             = "Modal";   
        static constexpr const char * NoSysMenu         = "NoSysMenu";   
        static constexpr const char * AllowDrag         = "AllowDrag";   
        static constexpr const char * XPos              = "XPos";   
        static constexpr const char * YPos              = "YPos";   
        static constexpr const char * XYPos             = "GotoXY";   
        static constexpr const char * WinLoc            = "GotoXY";   
        static constexpr const char * GotoXY            = "GotoXY";   
        static constexpr const char * PadX              = "PadX";   
        static constexpr const char * PadY              = "PadY";   
        static constexpr const char * cbTitleCell       = "cbTitleCell";
        static constexpr const char * Title             = "Title";
        static constexpr const char * CenterWin         = "CenterWin";
        static constexpr const char * CenterDesktop     = "CenterDesktop";
        static constexpr const char * MinValue          = "MinValue";
        static constexpr const char * MaxValue          = "MaxValue";
        static constexpr const char * Checked           = "Checked";
        static constexpr const char * Width             = "Width";
        static constexpr const char * Height            = "Height";
        static constexpr const char * Columns           = "Columns";
        static constexpr const char * Rows              = "Rows";
        static constexpr const char * Hidden            = "Hidden";
        static constexpr const char * Style             = "Style";
        static constexpr const char * fgColor           = "fgColor";
        static constexpr const char * bgColor           = "bgColor";
        static constexpr const char * _DefaultTitle     = "_DefaultTitle";      // Internal use


    };  
	// This is deprecated

	enum class AutoUpdateOld
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
using RgbColor = RGBColor_t;
using RgbColorA = RGBColorA_t;

struct RGBColor_t
{
public:
	int iRed;
	int iGreen;
	int iBlue;
public:
	__forceinline RGBColor_t operator * (const RGBColor_t & r2) { return { iRed*r2.iRed/255, iGreen*r2.iGreen/255, iBlue*r2.iBlue/255 }; }
	__forceinline RGBColor_t operator + (const RGBColor_t & r2) { return { iRed + r2.iRed, iGreen + r2.iGreen, iBlue + r2.iBlue }; }
	__forceinline RGBColor_t operator - (const RGBColor_t & r2) { return { iRed +- r2.iRed, iGreen - r2.iGreen, iBlue - r2.iBlue }; }
	__forceinline RGBColor_t & operator *= (const RGBColor_t & r2) { iRed = iRed*r2.iRed/255; iGreen = iGreen*r2.iGreen/255; iBlue = iBlue*r2.iBlue/255; return *this; }
	__forceinline RGBColor_t operator * (int iValue) { return { iRed*iValue, iGreen*iValue, iBlue*iValue }; }
	__forceinline RGBColor_t operator * (double fValue) { return { (int) ((double)iRed*fValue),  (int) ((double)iGreen*fValue),  (int) ((double)iBlue*fValue) }; }
    __forceinline RGBColor_t & operator *= (int iValue) { iRed *= iValue, iGreen *= iValue, iBlue *= iValue; return *this; }
	__forceinline RGBColor_t operator / (int iValue) { return { iRed/iValue, iGreen/iValue, iBlue/iValue }; }
    __forceinline RGBColor_t & operator /= (int iValue) { iRed /= iValue, iGreen /= iValue, iBlue /= iValue; return *this; }

	__forceinline operator int () { return (int) RGB(iRed,iGreen,iBlue); }
	__forceinline operator DWORD () { return (DWORD) RGB(iRed,iGreen,iBlue); }
   	operator RGBColorA_t ();

#ifdef SupportGDI
   	__forceinline operator  Gdiplus::Color () { return Gdiplus::Color(iRed,iGreen,iBlue); }
    operator CRgb (); 

#endif

	__forceinline DWORD toRGB() { return RGB(iRed,iGreen,iBlue); }
	__forceinline RGBColor24 toRGB24() { return RGBColor24{(unsigned char) iRed,(unsigned char) iGreen,(unsigned char) iBlue}; }
	__forceinline RGBColor_t & fromRGB(DWORD dwColor) { iRed = GetRValue(dwColor); iGreen = GetGValue(dwColor); iBlue = GetBValue(dwColor); return *this; }
	__forceinline DWORD operator * () { return RGB(iRed,iGreen,iBlue);  }
	__forceinline RGBColor_t & operator = (DWORD dwColor) { return fromRGB(dwColor);  }
	
	/// <summary>
	/// Set color as undefined (all values = -1). This is used to determine lack of color, to use default, or otherwise as a signal that there is no color assigned.
    /// <para></para>&#160;&#160;&#160;
    /// Init() and SetUndefined() are the same function.
	/// </summary>
	/// <returns>Current color (with Red, Green, Blue set to -1)</returns>
	RGBColor_t & Init();

	/// <summary>
	/// Set color as undefined (all values = -1). This is used to determine lack of color, to use default, or otherwise as a signal that there is no color assigned.
    /// <para></para>&#160;&#160;&#160;
    /// Init() and SetUndefined() are the same function.
	/// </summary>
	/// <returns>Current color (with Red, Green, Blue set to -1)</returns>
	RGBColor_t & SetUndefined();
	bool Undefined();
    __forceinline RGBColor_t fromGray(int iGray) { return { iGray,iGray,iGray }; }
    __forceinline RGBColor_t fromGray(double fGray) { return { (int) fGray,(int) fGray,(int) fGray }; }
    __forceinline RGBColor_t & toGray() { iRed = iGreen = iBlue = IntGray(); return *this; }
    __forceinline RGBColor_t & toLABGray() { iRed = iGreen = iBlue = (int) (255.0*LabGray()); return *this; }
	__forceinline int IntGray() { return (iRed + iGreen + iBlue)/3; };
	__forceinline double Gray() { return (double) ((int)(iRed+iGreen+iBlue))/3.0; }
	double LabGray();


	__forceinline void Clip() { iRed = max(0,min(255,iRed)); iGreen = max(0,min(255,iGreen)); iBlue = max(0,min(255,iBlue)); }
};

struct RGBColorA_t
{
public:
	int iRed;
	int iGreen;
	int iBlue;
	int iAlpha;
public:
	__forceinline RGBColorA_t operator * (const RGBColorA_t & r2) { return { iRed*r2.iRed/255, iGreen*r2.iGreen/255, iBlue*r2.iBlue/255 }; }
	__forceinline RGBColorA_t & operator *= (const RGBColorA_t & r2) { iRed = iRed*r2.iRed/255; iGreen = iGreen*r2.iGreen/255; iBlue = iBlue*r2.iBlue/255; return *this; }
	__forceinline RGBColorA_t operator * (int iValue) { return { iRed*iValue, iGreen*iValue, iBlue*iValue }; }
    __forceinline RGBColorA_t & operator *= (int iValue) { iRed *= iValue, iGreen *= iValue, iBlue *= iValue; return *this; }
	__forceinline RGBColorA_t operator / (int iValue) { return { iRed/iValue, iGreen/iValue, iBlue/iValue }; }
    __forceinline RGBColorA_t & operator /= (int iValue) { iRed /= iValue, iGreen /= iValue, iBlue /= iValue; return *this; }

	__forceinline operator int () { return (int) RGB(iRed,iGreen,iBlue); }
	__forceinline operator DWORD () { return (DWORD) RGB(iRed,iGreen,iBlue); }
    __forceinline operator RGBColor_t () { return RGBColor_t{iRed,iGreen,iBlue}; }

#ifdef SupportGDI
   	__forceinline operator  Gdiplus::Color () { return Gdiplus::Color(iAlpha,iRed,iGreen,iBlue); }
#endif

	__forceinline DWORD toRGB() { return RGB(iRed,iGreen,iBlue); }
	__forceinline RGBColor24 toRGB24() { return RGBColor24{(unsigned char) iRed,(unsigned char) iGreen,(unsigned char) iBlue}; }
	__forceinline RGBColorA_t & fromRGB(DWORD dwColor) { iRed = GetRValue(dwColor); iGreen = GetGValue(dwColor); iBlue = GetBValue(dwColor); return *this; }
	__forceinline DWORD operator * () { return RGB(iRed,iGreen,iBlue);  }
	__forceinline RGBColorA_t & operator = (DWORD dwColor) { return fromRGB(dwColor);  }
	
	/// <summary>
	/// Set color as undefined (all values = -1). This is used to determine lack of color, to use default, or otherwise as a signal that there is no color assigned.
    /// <para></para>&#160;&#160;&#160;
    /// Init() and SetUndefined() are the same function.
	/// </summary>
	/// <returns>Current color (with Red, Green, Blue set to -1)</returns>
	RGBColorA_t & Init();

	/// <summary>
	/// Set color as undefined (all values = -1). This is used to determine lack of color, to use default, or otherwise as a signal that there is no color assigned.
    /// <para></para>&#160;&#160;&#160;
    /// Init() and SetUndefined() are the same function.
	/// </summary>
	/// <returns>Current color (with Red, Green, Blue set to -1)</returns>
	RGBColorA_t & SetUndefined();
	bool Undefined();
    __forceinline RGBColorA_t fromGray(int iGray) { return { iGray,iGray,iGray,iAlpha }; }
    __forceinline RGBColorA_t fromGray(double fGray) { return { (int) fGray,(int) fGray,(int) fGray, iAlpha }; }
    __forceinline RGBColorA_t & toGray() { iRed = iGreen = iBlue = IntGray(); return *this; }
    __forceinline RGBColorA_t & toLABGray() { iRed = iGreen = iBlue = (int) (255.0*LabGray()); return *this; }
	__forceinline int IntGray() { return (iRed + iGreen + iBlue)/3; };
	__forceinline double Gray() { return (double) ((int)(iRed+iGreen+iBlue))/3.0; }
	double LabGray();


	__forceinline void Clip() { iAlpha = max(0,min(255,iAlpha)); iRed = max(0,min(255,iRed)); iGreen = max(0,min(255,iGreen)); iBlue = max(0,min(255,iBlue)); }
};

#ifdef SupportGDI

class CRgb : public Gdiplus::Color
{

    bool m_bUndefined = false;
public:
    static const CRgb EmptyColor;     // Empty color for undefined references (i.e. defaults)
    CRgb(int iRed,int iGreen,int iBlue) : Color(iRed,iGreen,iBlue) {};
    CRgb(int iRed,int iGreen,int iBlue,int iAlpha) : Color(iAlpha,iRed,iGreen,iBlue) {};
    CRgb(Gdiplus::ARGB argb) : Color(argb) {};
    __forceinline operator  RgbColor () const { return RgbColor{ (int) GetR(),(int) GetG(),(int) GetB() }; }
    __forceinline operator  RgbColorA () const { return RgbColorA{ (int) GetR(),(int) GetG(),(int) GetB(),(int) GetA() }; }
    CRgb() { m_bUndefined = true; }
    bool isUndefined()const { return m_bUndefined; }
};
#endif
	// Gradient -- used two store two colors to form a gradient
	//
	struct Gradient
	{
		RGBColor_t rgbColor1;		// Top or Left Color
		RGBColor_t rgbColor2;		// Bottom or Right Color
		bool bMono;					// When true, only the top color is used (i.e. it's just storage for the top color)
		bool bHorizontal;
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
		bool bAlignedMem;	// True of memory aligned
							// $$ This is temporary until all legacy functions
							// are verified to use aligned memory

        void Clean();
		void Delete();
		void Init();
		bool isValid();

		FloatBitmap_t & operator = (SIZE szSize);

		[[nodiscard]] RawBitmap_t ConverttoBitmap();
		[[nodiscard]] RawBitmap_t ConverttoBitmap(RawBitmap_t & stSource);
		float **  operator * ();

	};
	[[nodiscard]] FloatBitmap_t CreateFloatBitmap(int iWidth,int iHeight,bool * bError = nullptr);
	[[nodiscard]] FloatBitmap_t CreateFloatBitmap(const SIZE szSize,bool * bError = nullptr);
	[[nodiscard]] FloatBitmap_t CreateFloatBitmap(float * fpMem,int iWidth,int iHeight,bool * bError = nullptr);

	struct FloatBitmapM_t
	{
		float * fPixels;
		int iWidth;
		int iHeight;
		int iTotalSize;

		void Delete();
        void Clean();
		void Init();
		bool isValid();
		bool Invert();
		bool Multiply(FloatBitmapM_t & stSource,POINT pDestStart = { 0,0 }, POINT pSourceStart = { 0,0 }, SIZE szSize = { 0,0 });

		[[nodiscard]] RawBitmap_t ConverttoBitmap();
		[[nodiscard]] RawBitmap_t ConverttoBitmap(RawBitmap_t & stSource);
		float *  operator * ();

	};

	struct RawBitmap_t
	{
		int iWidth;
		int iHeight;
		int iWidthBytes;
		int iOverHang;
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

        /// <summary>
        /// Swaps red and blue channels.  This supports BGR uses, such as GPU uses or libraries and other routines that may  look for BGRA
        /// <para></para>
        /// The CBitmap (and RawBitmap32) types are RGB in memory, unless converted with this function to BGR.
        /// <para></para>
        /// --> Note: Once converted, all bitmap functions will still work, with the RED and BLUE channels reversed.
        /// </summary>
        /// <returns>true of successful, false if not successful (i.e. invalid bitmap, etc.)</returns>
        bool SwapRedBlueInline();

		bool CopyFrom(RawBitmap_t & stSource, POINT pDestStart = {0,0} , POINT pSourceStart = { 0,0 }, SIZE szSize = {0,0});
	//	bool Copyto(RawBitmap_t & stDest);
		bool Copyto(RawBitmap_t & stDest, POINT pSourceStart = { 0,0}, POINT pDestStart = { 0,0 }, SIZE szSize = { 0,0 });
		bool FillColor(Sage::RGBColor_t rgbColor, POINT pStart = { 0,0 }, SIZE szSize = { 0,0 });
		bool FillColor(DWORD dwColor, POINT pStart = { 0,0 }, SIZE szSize = { 0,0 });

		bool ReverseBitmap();



		void DeleteMask();
		void UpdateMaskMem();
		bool MasktoBmp(RawBitmap_t & stDest);

		// Returns true of the bitmap data is invalid (i.e. corrupted), but does not determine if there is an active bitmap.
		// Use isEmpty() to determine of a bitmap is empty, or use isValid() to combine isInvalid() + isEmpty()
		//
		bool isInvalid() const;
		
		// Returns true of there is a valid bitmap contained in the structure. 
		// Returns true if the bitmap is empty (i.e. v.s. isInvalid() which only returns true of the data is corrupted).
		//
		bool isValid();
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
		RawBitmap32_t operator - ();      // Return RawBitmap32_t with -Height (for displaying bitmap upside-down)
        unsigned char * operator * ();
        SIZE GetSize();
        bool isEmpty();
        bool isInvalid() const;
        bool isValid();
        /// <summary>
        /// Swaps red and blue channels.  This supports BGRA uses, such as GPU uses or libraries and other routines that may  look for BGRA
        /// <para></para>
        /// The CBitmap32 (and RawBitmap32_t) types are RGBA in memory, unless converted with this function to BGR.
        /// <para></para>
        /// --> Note: Once converted, all bitmap functions will still work, with the RED and BLUE channels reversed.
        /// </summary>
        /// <returns>true of successful, false if not successful (i.e. invalid bitmap, etc.)</returns>
        bool SwapRedBlueInline();

		void Delete();
		void Clean();	// Use carefully
		bool ClearBitmap();
		int Init();
        static RawBitmap32_t CreateFrom(CBitmap & cBitmap);
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
	SageString200 & operator = (const char * sString);
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

	[[nodiscard]] RawBitmap_t ReadBitmap(const char * sFile, bool * bSucceeded = nullptr);

	bool SaveBitmap(const char * sFile,RawBitmap_t & stBitmap); 
	bool WriteBitmap(const char * sFile,RawBitmap_t & stBitmap); 

	[[nodiscard]] RawBitmap32_t BMPtoBitmap32(const unsigned char * sBMP);
	[[nodiscard]] RawBitmap_t BMPtoBitmap(const unsigned char * sBMP);
	bool BMPtoBitmap(const unsigned char * sBMP,RawBitmap_t & stBitmap);
	void DeleteBitmap(Sage::RawBitmap_t & stBitmap);
	void DeleteBitmap(Sage::RawBitmap32_t & stBitmap);

	[[nodiscard]] FloatBitmapM_t CreateFloatMBitmap(int iWidth,int iHeight,bool * bError = nullptr);
	[[nodiscard]] FloatBitmapM_t CreateFloatMBitmap(const SIZE szSize,bool * bError = nullptr);
	[[nodiscard]] FloatBitmapM_t CreateBitmap(float * fpMem,int iWidth,int iHeight);


	[[nodiscard]] RawBitmap_t CreateBitmap(unsigned char * sBitmapMem,int iWidth,int iHeight);
	[[nodiscard]] RawBitmap_t CreateBitmap(int iWidth,int iHeight,bool * bError = nullptr);
	[[nodiscard]] RawBitmap_t CreateBitmap(SIZE szSize,bool * bError = nullptr);
	[[nodiscard]] RawBitmap32_t CreateBitmap32(unsigned char * sBitmapMem,int iWidth,int iHeight);
	[[nodiscard]] RawBitmap32_t CreateBitmap32(int iWidth,int iHeight,bool * bError = nullptr);
	[[nodiscard]] RawBitmap32_t CreateBitmap32(SIZE szSize,bool * bError = nullptr);
	bool CreateBitmap(int iWidth,int iHeight,RawBitmap_t & stBitmap);
	bool CreateBitmap(RawBitmap_t & stSource,RawBitmap_t & stBitmap);
	[[nodiscard]] RawBitmap_t CreateBitmap(RawBitmap_t & stSource,bool * bSuccess = nullptr);
	[[nodiscard]] RawBitmap_t CreateBitmap(CBitmap & cBitmap,bool * bSuccess = nullptr);
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
	bool Multiply(Sage::FloatBitmapM_t & stDest, Sage::FloatBitmapM_t & stSource,POINT pDestStart = { 0,0 }, POINT pSourceStart = { 0,0 }, SIZE szSize = { 0,0 });

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
	[[nodiscard]] Mem<char> ReadTextFile(const char * sFile,bool bNullTerminate = false,bool * bSuccess = nullptr);
	[[nodiscard]] Mem<unsigned char> ReadBinaryFile(const char * sFile,bool * bSuccess = nullptr);
	[[nodiscard]] Mem<unsigned char> ReadBinaryFile(const char * sFile,int iMaxSize,ImageStatus * eStatus = nullptr);
	CBitmap ReadJpegFile(const char * sPath,bool * bSuccess = nullptr);
	CBitmap ReadJpegMem(const unsigned char * sData,int iDataLength,bool * bSuccess = nullptr);

    // Console Functions

    // ErrorExit() -- Used to exit program quickly with an optional message.
    //
    // ErrorExit() simply prints an optional message and exits with the error-code specified or -1
    // This is a function that is used for a convenient exit of a program that is in a state of development.
    //
    // This exits the program directly with no clean up, memory deallocations, and so-forth.
    //
    // For a Windows program, ErrorExit() is meant for development or an emergency exit.
    // For a console-mode program, since the message is printed to the console-mode window, ErrorExit() can 
    // also be used as a solid and permanent way to exit a program on an error.
    //
    // In a Windows program, a Windows Message Box appears with the error message.
    // In a Console Mode program, the error message is written to the console window.
    //
    void ErrorExit(const char * sErrMsg,int iExitCode = -1);

    /// <summary>
    /// Local function to return integer converted to ascii in decimal (base 10) format. 
    /// </summary>
    /// <returns></returns>
    const char * itoadec(int iNumber, char * sOutput); 

    // ErrorExit() -- Used to exit program quickly with an optional message.
    //
    // ErrorExit() simply prints an optional message and exits with the error-code specified or -1
    // This is a function that is used for a convenient exit of a program that is in a state of development.
    //
    // This exits the program directly with no clean up, memory deallocations, and so-forth.
    //
    // For a Windows program, ErrorExit() is meant for development or an emergency exit.
    // For a console-mode program, since the message is printed to the console-mode window, ErrorExit() can 
    // also be used as a solid and permanent way to exit a program on an error.
    //
    // In a Windows program, a Windows Message Box appears with the error message.
    // In a Console Mode program, the error message is written to the console window.
    //
    void ErrorExit(int iExitCode = -1);

    // WinErrorExit() -- Used to exit program quickly with an optional message.
    //
    // ErrorExit() simply prints an optional message and exits with the error-code specified or -1
    // This is a function that is used for a convenient exit of a program that is in a state of development.
    //
    // This exits the program directly with no clean up, memory deallocations, and so-forth.
    //
    // For a Windows program, ErrorExit() is meant for development or an emergency exit.
    // For a console-mode program, since the message is printed to the console-mode window, ErrorExit() can 
    // also be used as a solid and permanent way to exit a program on an error.
    //
    // In a Windows program, a Windows Message Box appears with the error message.
    // In a Console Mode program, the error message is written to the console window.
    //
    void WinErrorExit(const char * sErrMsg,int iExitCode = -1);

    // WinErrorExit() -- Used to exit program quickly with an optional message.
    //
    // ErrorExit() simply prints an optional message and exits with the error-code specified or -1
    // This is a function that is used for a convenient exit of a program that is in a state of development.
    //
    // This exits the program directly with no clean up, memory deallocations, and so-forth.
    //
    // For a Windows program, ErrorExit() is meant for development or an emergency exit.
    // For a console-mode program, since the message is printed to the console-mode window, ErrorExit() can 
    // also be used as a solid and permanent way to exit a program on an error.
    //
    // In a Windows program, a Windows Message Box appears with the error message.
    // In a Console Mode program, the error message is written to the console window.
    //
    void WinErrorExit(int iExitCode = -1);

    CString ShowExceptMsg(const char * sTitle, const char * sMsg,const char * sFile,unsigned int sLine);

    namespace Rgb
    {
        static constexpr RGBColor_t Transparent = RGBColor_t{-1,-1,-1};     // Use to specify transparency for Circle(), Rectangle(), etc.
        static constexpr RGBColor_t None        = RGBColor_t{-1,-1,-1};     // Used to specify non-existent color, such as Pen color for Circle(), Rectangle(), etc.
        static constexpr RGBColor_t Undefined   = RGBColor_t{-1,-1,-1};     // Used to specify undefined color (i.e. defaults and such).
        static constexpr RGBColor_t Default     = RGBColor_t{-1,-1,-1};     // Used to specify default color (i.e. DrawLine() or LineTo()
    };

    // WinX has been co-opted by the use of "Win" in Quick C++
	namespace WinX
	{
		void ScreentoClient(HWND hWndParent,RECT & rRect);
	} // namespace Sage::Win

   // Sagebox Initialization.  This is in its preliminary stages and will be built up over time. 
    struct SageboxInit
    {
        public:
            AutoUpdateType  eAutoUpdateType = Sage::AutoUpdateType::On;
            bool bWordWrap = false;
            bool bControlCExitDisabled = false; 
            RgbColor        bgColor;                // TBD - Background color for created windows
            RgbColor        bgColor2;               // TBD - Secondary color of the default background is a gradient
            RgbColor        fgColor;                // TBD - Foreground color for created windows (i.e. text color
            const char    * sFont = "Arial,13";       // Default font for created windows

            // Functions called from other Sagebox functions.  Note that these are defaults and not current to any window
           
            bool GetWordWrap() { return bWordWrap; };
            AutoUpdateType GetAutoUpdateType() { return eAutoUpdateType; }
            const char * GetFont() { return sFont; }
    };

    // WinConio Structure -- Functions for Console Window Output 
    // ---------------------------------------------------------
    //
    // WinConio (referred to as "conio" in code) functions can be used to put out formatted text with colors and control the Console Mode window. 
    //
    // "{}"-style modifiers may be used to control the output, such as setting text colors, background colors, and underlined text. 
    //
    // For example,
    //
    //  conio.printf("This {r}word{/} was written in red")          -- writes the word "word" in red text .
    //  conio.printf("This {bg=y}{r}word{/}{/} was written in red") -- writes the word "word" in red text with a yellow background.
    //  conio.printf("This {rev}word{/}{/} was written in reverse") -- writes the word "word" in reversed text 
    //  conio.printf("This {u}word{/}{/} was written underlined")   -- writes the word "word" in underlined text.
    //
    // --> Note: WinConio "{}"-style modifiers are not 100% compatible with other Sagebox/Quick C++ modifiers.
    //           For example colors like "yellow" are typically compatible, but "{rev}" (to reverse text)" is only used for WinConio.
    //
    // --> Note: WinConio is useful for the Console Window only.  It does not print out to stdio.  
    //     Therefore, if stdio is re-routed conio will still print the the Console Window.
    //     if a Console Window does not exist (such as a pure Windows program or Quick C++ project with no console mode window), WinnConio functions
    //     will have no effect.
    //
    // --> Note: Tabs are set with tabstops every 4 spaces, and can be used for formatting.  See also "x=" below to set specific X character 
    //           settings for aligned text.
    //
    // A complete list of "{}"-style modifiers for WinConIo
    // ----------------------------------------------------
    //
    //      black       - color black (text or background)    
    //      darkblue    - color dark blue (text or background)       
    //      db          - color dark blue (text or background)       
    //      darkgreen   - color dark green (text or background)       
    //      dg          - color dark green (text or background)       
    //      darkcyan    - color dark cyan (text or background)       
    //      dc          - color dark cyan (text or background)       
    //      darkRed     - color dark red(text or background)      
    //      dr          - color dark red (text or background)       
    //      darkpurple  - color dark purple (text or background)      
    //      dp          - color (text or background)       
    //      darkmagenta - color dark purple (text or background)      
    //      dm          - color dark magenta (text or background)       
    //      darkYellow  - color dark yellow (text or background)       
    //      dy          - color dark yellow (text or background)       
    //      gray        - color gray (text or background)       
    //      grey        - color gray (text or background)       
    //      blue        - color blue (text or background)       
    //      b           - color blue (text or background)       
    //      green       - color green (text or background)       
    //      g           - color green (text or background)       
    //      cyan        - color cyan (text or background)       
    //      c           - color cyan (text or background)       
    //      red         - color red (text or background)       
    //      r           - color red (text or background)       
    //      purple      - color purple (text or background)       
    //      p           - color purple (text or background)       
    //      magenta     - color magenta (text or background)       
    //      m           - color m (text or background)       
    //      Yellow      - color yellow (text or background)       
    //      y           - color yellow (text or background)       
    //      w           - color white (text or background)       
    //      white       - color white (text or background)       
    //      u           - underline text
    //      vl          - left bar on character (i.e. vertical left)
    //      vr          - right bar on character (i.e. vertical top)
    //      ht          - topline text (i.e. underline, but on top) (i.e. horizontal top)
    //      hb          - same as underline (i.e. horizontal bottom)
    //      rev         - reverse text colors (foreground color = background color; background color = foreground color)
    //      bg=         - Set background color next text (i.e. {bg=blue})
    //      lbg=        - Set background color for entire line (i.e. {lbg=blue})
    //      X=          - Set X character position in line (for aligning text), i.e. "This is at {x=40} column 40"
    //
    struct WinConio
    {
        WinConio & operator << (const char * x)        ;
        WinConio & operator << (char x)                ;
        WinConio & operator << (std::string & cs)      ;
        WinConio & operator << (CString & cs)          ;
        WinConio & operator << (int x)                 ;
        WinConio & operator << (unsigned int x)        ;
        WinConio & operator << (float x)               ;
        WinConio & operator << (double x)              ;
        /// <summary>
        /// Write text to the console window.  "{}"-style modifiers may be used, such as conio.Write("This {r}{u}word{/}{/} is red and underlined"); 
        /// <para></para>&#160;&#160;&#160;You can also specify X,Y coordinates beforehand, such as:
        /// <para></para>&#160;&#160;&#160;&#160;&#160;&#160;
        /// Write(10,30,"This message was written at character location 10,30"); 
        /// <para></para>&#160;&#160;&#160;&#160;&#160;&#160;
        /// Write(MyPoint,"This message was written at character location 10,30"); 
        /// <para></para>
        /// --> Note: the maximum length of input text is roughly 2k, but may be increased in a future update
        /// </summary>
        /// <param name="sText"> = text to write to the window</param>
        /// <returns></returns>
        bool Write(const char * sText)                          ;

        /// <summary>
        /// Write text to the console window.  "{}"-style modifiers may be used, such as conio.Write("This {r}{u}word{/}{/} is red and underlined"); 
        /// <para></para>&#160;&#160;&#160;You can also specify X,Y coordinates beforehand, such as:
        /// <para></para>&#160;&#160;&#160;&#160;&#160;&#160;
        /// Write(10,30,"This message was written at character location 10,30"); 
        /// <para></para>&#160;&#160;&#160;&#160;&#160;&#160;
        /// Write(MyPoint,"This message was written at character location 10,30"); 
        /// <para></para>
        /// --> Note: the maximum length of input text is roughly 2k, but may be increased in a future update
        /// </summary>
        /// <param name="sText"> = text to write to the window</param>
        /// <returns></returns>
        bool Write(int iX,int iY,const char * sText)            ;

        /// <summary>
        /// Write text to the console window.  "{}"-style modifiers may be used, such as conio.Write("This {r}{u}word{/}{/} is red and underlined"); 
        /// <para></para>&#160;&#160;&#160;You can also specify X,Y coordinates beforehand, such as:
        /// <para></para>&#160;&#160;&#160;&#160;&#160;&#160;
        /// Write(10,30,"This message was written at character location 10,30"); 
        /// <para></para>&#160;&#160;&#160;&#160;&#160;&#160;
        /// Write(MyPoint,"This message was written at character location 10,30"); 
        /// <para></para>
        /// --> Note: the maximum length of input text is roughly 2k, but may be increased in a future update
        /// </summary>
        /// <param name="sText"> = text to write to the window</param>
        /// <returns></returns>
        bool Write(POINT pLoc,const char * sText)               ;

        /// <summary>
        /// Sets the font size of the Console Window.  This change the font for all characters in the Consolle Window simultaneously.
        /// <para></para>
        /// Typically, 14-16 is a good number, where numbers less than 14 start to represent smaller fonts, and numbers larger than 16 represent big fonts.
        /// <para></para>&#160;&#160;&#160;
        /// When setting the font, this will not keep the available characters in the console window the same, keeping the Console Window the same physical size.  Therefore, use SetFont() before setting the window size. 
        /// <para></para>&#160;&#160;&#160;
        /// You can also use the MouseWheel when pressing the control-key -- this will keep the number of characters in the window the same while changing the size of the Console Window.  This method will keep text and other
        /// aligned text still centered and aligned, where using SetFont() will affect any centering and text alignment.
        /// </summary>
        /// <param name="iSize">= Size of the new font</param>
        /// <returns></returns>
        bool SetFontSize(int iSize)                             ;

        /// <summary>
        /// Sets the Text (i.e. foreground) color of text in the Console Window. This affects the next printed characters.
        /// <para></para>&#160;&#160;&#160;
        /// Use a text color, such as "White", "blue", "db" (for darkblue"), "darkred", etc.  to set the text color. 
        /// <para></para>You can also use a number such as 0x04 (for red), etc. The value of the numbers can be obtained by searching the internet for "Win32 Console Mode Text Colors". 
        /// <para></para>GetFgColor() can be used to retrieve the current color, which returns it as a numerical value specific to the Console Window.
        /// <para></para>--> Note: You can embed the text color with conio.printf() or Write(), such as conio.printf("This {red}word{/} is written in red.")
        /// </summary>
        /// <param name="iColor">= The color code (0x00-0x0F) to set for the text color</param>
        /// <param name="sColor">The color name to set for the text color</param>
        /// <returns></returns>
        bool SetFgColor(int iColor)                             ;

        /// <summary>
        /// Sets the Text (i.e. foreground) color of text in the Console Window. This affects the next printed characters.
        /// <para></para>&#160;&#160;&#160;
        /// Use a text color, such as "White", "blue", "db" (for darkblue"), "darkred", etc.  to set the text color. 
        /// <para></para>You can also use a number such as 0x04 (for red), etc. The value of the numbers can be obtained by searching the internet for "Win32 Console Mode Text Colors". 
        /// <para></para>GetFgColor() can be used to retrieve the current color, which returns it as a numerical value specific to the Console Window.
        /// <para></para>--> Note: You can embed the text color with conio.printf() or Write(), such as conio.printf("This {red}word{/} is written in red.")
        /// </summary>
        /// <param name="iColor">= The color code (0x00-0x0F) to set for the text color</param>
        /// <param name="sColor">The color name to set for the text color</param>
        /// <returns></returns>
        bool SetFgColor(const char * sColor)                    ;

        /// <summary>
        /// Sets the Background color of text in the Console Window. This affects the next printed characters.
        /// <para></para>&#160;&#160;&#160;
        /// Use a text color, such as "White", "blue", "db" (for darkblue"), "darkred", etc.  to set the text color. 
        /// <para></para>You can also use a number such as 0x04 (for red), etc. The value of the numbers can be obtained by searching the internet for "Win32 Console Mode Text Colors". 
        /// <para></para>GetBgColor() can be used to retrieve the current color, which returns it as a numerical value specific to the Console Window.
        /// <para></para>--> Note: You can embed the background color with conio.printf() or Write(), such as conio.printf("This {bg=blue}word{/} has a blue background.")
        /// <para></para>--> Note: You can also use "{rev}some text{/}" as a quick way to reverse the colors for selected text.
        /// </summary>
        /// <param name="iColor">= The color code (0x00-0x0F) to set for the background color</param>
        /// <param name="sColor">The color name to set for the background color</param>
        /// <returns></returns>
        bool SetBgColor(int iColor,bool bFillBg = true)         ;

        /// <summary>
        /// Sets the Background color of text in the Console Window. This affects the next printed characters.
        /// <para></para>&#160;&#160;&#160;
        /// Use a text color, such as "White", "blue", "db" (for darkblue"), "darkred", etc.  to set the text color. 
        /// <para></para>You can also use a number such as 0x04 (for red), etc. The value of the numbers can be obtained by searching the internet for "Win32 Console Mode Text Colors". 
        /// <para></para>GetBgColor() can be used to retrieve the current color, which returns it as a numerical value specific to the Console Window.
        /// <para></para>--> Note: You can embed the background color with conio.printf() or Write(), such as conio.printf("This {bg=blue}word{/} has a blue background.")
        /// <para></para>--> Note: You can also use "{rev}some text{/}" as a quick way to reverse the colors for selected text.
        /// </summary>
        /// <param name="iColor">= The color code (0x00-0x0F) to set for the background color</param>
        /// <param name="sColor">The color name to set for the background color</param>
        /// <returns></returns>
        bool SetBgColor(const char * sColor,bool bFillBg = true);

        /// <summary>
        /// Sets the title of the Console Window in the Title Bar. Use "" or nullptr for no title.
        /// </summary>
        /// <param name="sTitle"></param>
        /// <returns></returns>
        bool SetWindowTitle(const char * sTitle);

        /// <summary>
        /// Clears the console window with the current (or specified background color), then sets the cursor to {0,0} (unless bResetCursor = false)
        /// <para></para>&#160;&#160;&#160;
        /// If a color is specified (with a color value or color name), the background text color is also set to this color.
        /// <para></para>
        /// Examples
        /// <para></para>&#160;&#160;&#160; Cls("blue"); -- Clears the screen to blue, sets cursor at {0,0}
        /// <para></para>&#160;&#160;&#160; Cls(0x10);  -- Clears the screen to blue, does not reset cursor
        /// <para></para>&#160;&#160;&#160; Cls();      -- Clears the screen with current BG color, sets cursor to {0,0}
        /// </summary>
        /// <param name="iColor"> = Sets the CLS background color (optional)</param>
        /// <param name="sColor"> = Sets the CLS background color by name (optional)</param>
        /// <param name="bResetCursor">= Sets the cursor to {0,0}} (default) (optional)</param>
        /// <returns></returns>
        bool Cls(bool bResetCursor = true)                      ;

        /// <summary>
        /// Clears the console window with the current (or specified background color), then sets the cursor to {0,0} (unless bResetCursor = false)
        /// <para></para>&#160;&#160;&#160;
        /// If a color is specified (with a color value or color name), the background text color is also set to this color.
        /// <para></para>
        /// Examples
        /// <para></para>&#160;&#160;&#160; Cls("blue"); -- Clears the screen to blue, sets cursor at {0,0}
        /// <para></para>&#160;&#160;&#160; Cls(0x10);  -- Clears the screen to blue, does not reset cursor
        /// <para></para>&#160;&#160;&#160; Cls();      -- Clears the screen with current BG color, sets cursor to {0,0}
        /// </summary>
        /// <param name="iColor"> = Sets the CLS background color (optional)</param>
        /// <param name="sColor"> = Sets the CLS background color by name (optional)</param>
        /// <param name="bResetCursor">= Sets the cursor to {0,0}} (default) (optional)</param>
        /// <returns></returns>
        bool Cls(int iColor,bool bResetCursor = true)           ;

        /// <summary>
        /// Clears the console window with the current (or specified background color), then sets the cursor to {0,0} (unless bResetCursor = false)
        /// <para></para>&#160;&#160;&#160;
        /// If a color is specified (with a color value or color name), the background text color is also set to this color.
        /// <para></para>
        /// Examples
        /// <para></para>&#160;&#160;&#160; Cls("blue"); -- Clears the screen to blue, sets cursor at {0,0}
        /// <para></para>&#160;&#160;&#160; Cls(0x10);  -- Clears the screen to blue, does not reset cursor
        /// <para></para>&#160;&#160;&#160; Cls();      -- Clears the screen with current BG color, sets cursor to {0,0}
        /// </summary>
        /// <param name="iColor"> = Sets the CLS background color (optional)</param>
        /// <param name="sColor"> = Sets the CLS background color by name (optional)</param>
        /// <param name="bResetCursor">= Sets the cursor to {0,0}} (default) (optional)</param>
        /// <returns></returns>
        bool Cls(const char * sColor,bool bResetCursor = true)  ;

        /// <summary>
        /// Sets the background color of the entire line.  This will set the background color without erasing text to the color
        /// specified, which may be a numeric background value, or a string for the color (i.e. "Blue","darkblue","red", etc.)
        /// </summary>
        /// <param name="sColor">Name of the color for the background</param>
        /// <param name="iColor">Numeric code for the background color</param>
        /// <returns></returns>
        bool SetBgLine(const char * sColor)                     ;

        /// <summary>
        /// Sets the background color of the entire line.  This will set the background color without erasing text to the color
        /// specified, which may be a numeric background value, or a string for the color (i.e. "Blue","darkblue","red", etc.)
        /// </summary>
        /// <param name="sColor">Name of the color for the background</param>
        /// <param name="iColor">Numeric code for the background color</param>
        /// <returns></returns>
        bool SetBgLine(int iColor)                              ;


        /// <summary>
        /// Print to the console window, just as printf(), but with some options.  "{}"-style modifiers may be used, such as conio.printf("This {r}{u}word{/}{/} is red and underlined"); 
        /// <para></para>&#160;&#160;&#160;You can also specify X,Y coordinates beforehand, such as:
        /// <para></para>&#160;&#160;&#160;&#160;&#160;&#160;
        /// conio.printf(10,30,"This message was written at character location 10,30"); 
        /// <para></para>&#160;&#160;&#160;&#160;&#160;&#160;
        /// conio.printf(MyPoint,"This message was written at character location %d,%d",iX,iY); 
        /// <para></para>
        /// --> Note: the maximum length of input text is roughly 2k, but may be increased in a future update
        /// </summary>
        /// <param name="sText"> = text to write to the window</param>
        /// <param name="..."> = standard printf parameters for formatted text (i.e. variadic arguments)</param>
        /// <returns></returns>
        bool printf(const char * Format,...)                    ;

        /// <summary>
        /// Print to the console window, just as printf(), but with some options.  "{}"-style modifiers may be used, such as conio.printf("This {r}{u}word{/}{/} is red and underlined"); 
        /// <para></para>&#160;&#160;&#160;You can also specify X,Y coordinates beforehand, such as:
        /// <para></para>&#160;&#160;&#160;&#160;&#160;&#160;
        /// conio.printf(10,30,"This message was written at character location 10,30"); 
        /// <para></para>&#160;&#160;&#160;&#160;&#160;&#160;
        /// conio.printf(MyPoint,"This message was written at character location %d,%d",iX,iY); 
        /// <para></para>
        /// --> Note: the maximum length of input text is roughly 2k, but may be increased in a future update
        /// </summary>
        /// <param name="sText"> = text to write to the window</param>
        /// <param name="..."> = standard printf parameters for formatted text (i.e. variadic arguments)</param>
        /// <returns></returns>
        bool printf(int iX,int iY,const char * Format,...)      ;

        /// <summary>
        /// Print to the console window, just as printf(), but with some options.  "{}"-style modifiers may be used, such as conio.printf("This {r}{u}word{/}{/} is red and underlined"); 
        /// <para></para>&#160;&#160;&#160;You can also specify X,Y coordinates beforehand, such as:
        /// <para></para>&#160;&#160;&#160;&#160;&#160;&#160;
        /// conio.printf(10,30,"This message was written at character location 10,30"); 
        /// <para></para>&#160;&#160;&#160;&#160;&#160;&#160;
        /// conio.printf(MyPoint,"This message was written at character location %d,%d",iX,iY); 
        /// <para></para>
        /// --> Note: the maximum length of input text is roughly 2k, but may be increased in a future update
        /// </summary>
        /// <param name="sText"> = text to write to the window</param>
        /// <param name="..."> = standard printf parameters for formatted text (i.e. variadic arguments)</param>
        /// <returns></returns>
        bool printf(POINT pLoc,const char * Format,...)         ;

        /// <summary>
        /// Print to the console window, just as printf(), but with some options.  "{}"-style modifiers may be used, such as conio.printf("This {r}{u}word{/}{/} is red and underlined"); 
        /// <para></para>&#160;&#160;&#160;You can also specify X,Y coordinates beforehand, such as:
        /// <para></para>&#160;&#160;&#160;&#160;&#160;&#160;
        /// conio.printf(10,30,"This message was written at character location 10,30"); 
        /// <para></para>&#160;&#160;&#160;&#160;&#160;&#160;
        /// conio.printf(MyPoint,"This message was written at character location %d,%d",iX,iY); 
        /// <para></para>
        /// --> Note: the maximum length of input text is roughly 2k, but may be increased in a future update
        /// </summary>
        /// <param name="sText"> = text to write to the window</param>
        /// <param name="..."> = standard printf parameters for formatted text (i.e. variadic arguments)</param>
        /// <returns></returns>
        bool __printf(const char * Format,...)                    ;

        /// <summary>
        /// Print to the console window, just as printf(), but with some options.  "{}"-style modifiers may be used, such as conio.printf("This {r}{u}word{/}{/} is red and underlined"); 
        /// <para></para>&#160;&#160;&#160;You can also specify X,Y coordinates beforehand, such as:
        /// <para></para>&#160;&#160;&#160;&#160;&#160;&#160;
        /// conio.printf(10,30,"This message was written at character location 10,30"); 
        /// <para></para>&#160;&#160;&#160;&#160;&#160;&#160;
        /// conio.printf(MyPoint,"This message was written at character location %d,%d",iX,iY); 
        /// <para></para>
        /// --> Note: the maximum length of input text is roughly 2k, but may be increased in a future update
        /// </summary>
        /// <param name="sText"> = text to write to the window</param>
        /// <param name="..."> = standard printf parameters for formatted text (i.e. variadic arguments)</param>
        /// <returns></returns>
        bool __printf(int iX,int iY,const char * Format,...)      ;

        /// <summary>
        /// Print to the console window, just as printf(), but with some options.  "{}"-style modifiers may be used, such as conio.printf("This {r}{u}word{/}{/} is red and underlined"); 
        /// <para></para>&#160;&#160;&#160;You can also specify X,Y coordinates beforehand, such as:
        /// <para></para>&#160;&#160;&#160;&#160;&#160;&#160;
        /// conio.printf(10,30,"This message was written at character location 10,30"); 
        /// <para></para>&#160;&#160;&#160;&#160;&#160;&#160;
        /// conio.printf(MyPoint,"This message was written at character location %d,%d",iX,iY); 
        /// <para></para>
        /// --> Note: the maximum length of input text is roughly 2k, but may be increased in a future update
        /// </summary>
        /// <param name="sText"> = text to write to the window</param>
        /// <param name="..."> = standard printf parameters for formatted text (i.e. variadic arguments)</param>
        /// <returns></returns>
        bool __printf(POINT pLoc,const char * Format,...)         ;

        /// <summary>
        /// Returns background color.  This is the text attribute ANDed with 0xF0 to form only the background color
        /// </summary>
        /// <returns>Console-Based background color</returns>
        WORD GetBgColor()                                       ;
        
        /// <summary>
        /// Returns foreground color.  This is the text attribute ANDed with 0x0F to form only the foreground color
        /// </summary>
        /// <returns>Console-Based foreground color</returns>
        WORD GetFgColor()                                       ;

        /// <summary>
        /// Returns the console-mode text attribute.  This includes the background color, text color, and other attributes (such as underline, etc.)
        /// <para></para>&#160;&#160;&#160;AND the attribute with 0xF0 to obtain the background color; AND with 0x0F to obtaint the foreground color.
        /// </summary>
        /// <returns>Console attribute color</returns>
        WORD GetTextAttribute()                                 ;

        /// <summary>
        /// Set the Cursor to the (X,Y) character position in the Console Window.
        /// <para></para>&#160;&#160;&#160;    
        /// the X and Y position are character positions rather than pixel positions.
        /// <para></para>
        /// Example: GotoXY(10,20); -- Sets the cursor at character position 10,20 in the console window.
        /// </summary>
        /// <param name="iX">X position to set Cursor</param>
        /// <param name="iY">Y position to set Cursor</param>
        /// <returns></returns>
        bool GotoXY(int iX, int iY)                             ;

        /// <summary>
        /// Returns a CString with the names of colors that can be used in the "{}" Write() or printf() functions.
        /// <para></para>&#160;&#160;&#160;    
        /// For example, "printf("This is {red}red{/} or {r}red{/}") puts both "red" words in the color red on the console window.
        /// </summary>
        /// <returns></returns>
        CString GetColorNames()                                 ;

        /// <summary>
        /// Causes the console window to become visible or hidden, dependig on bShow paramater
        /// </summary>
        /// <param name="bShow"> -> true = Show console window.  false = Hide console window</param>
        /// <returns></returns>
        bool Show(bool bShow = true);

        /// <summary>
        /// Causes the console window to become hidden or visible, depending on the bHide paramater
        /// </summary>
        /// <param name="bHide"> -> true = Hide console window.  true = Show console window</param>
        /// <returns></returns>
        bool Hide(bool bHide = true);

        /// <summary>
        /// Returns TRUE if the Console Window is Visible, FALSE if the console window is hidden
        /// </summary>
        /// <returns></returns>
        bool isWindowVisible();

        /// <summary>
        /// Sets the size of the console window in pixels.
        /// <para></para>&#160;&#160;&#160;
        /// The window is sized to the nearest multiple of the character width and height, and is the internal size of the window, not the entire window (i.e. title bar, frame, etc.).
        /// <para></para>
        /// Use (iX,iY) or a SIZE value to set the window size.
        /// </summary>
        /// <param name="iWidth">Width of Window (in pixels)</param>
        /// <param name="iHeight">Height of Window (in pixels)</param>
        /// <param name="SetMaxX">When TRUE, sets the window width to remove the bottom scrollbar.  When FALSE, the max character width is not changed.</param>
        /// <returns></returns>
        bool SetWindowSize(int iWidth,int iHeight,bool SetMaxX = true); 

        /// <summary>
        /// Sets the size of the console window in pixels.
        /// <para></para>&#160;&#160;&#160;
        /// The window is sized to the nearest multiple of the character width and height, and is the internal size of the window, not the entire window (i.e. title bar, frame, etc.).
        /// <para></para>
        /// Use (iX,iY) or a SIZE value to set the window size.
        /// </summary>
        /// <param name="szSize">SIZE structure with the new window size.</param>
        /// <param name="SetMaxX">When TRUE, sets the window width to remove the bottom scrollbar.  When FALSE, the max character width is not changed.</param>
        /// <returns></returns>
        bool SetWindowSize(SIZE szSize,bool SetMaxX = true); 

        /// <summary>
        /// Set the location of the Console Widow on the desktop.
        /// <para></para>&#160;&#160;&#160;
        /// Use (iX,iY) or a POINT value to set the size (i.e. SetWIndowSize(myPoint)); 
        ///
        /// </summary>
        /// <param name="iX">X location of Console Window</param>
        /// <param name="iY">Y location of Console Window</param>
        /// <returns></returns>
        bool SetWinLocation(int iX,int iY); 

        /// <summary>
        /// Set the location of the Console Widow on the desktop.
        /// <para></para>&#160;&#160;&#160;
        /// Use (iX,iY) or a POINT value to set the size (i.e. SetWIndowSize(myPoint)); 
        ///
        /// </summary>
        /// <param name="pLoc">POINT structure with the new window location</param>
        /// <returns></returns>
        bool SetWinLocation(POINT pLoc); 

        /// <summary>
        /// Brings the Window to the top of the screen, above all other windows.  This can be useful when using a Sagebox, Quick C++ or other window with your program, to ensure
        /// the Console Window is showing above the other windows.  Use SetTopWindow() to bring the Console Window to the top permanently (i.e. can't be overlapped again)
        /// </summary>
        /// <returns></returns>
        bool SetTopWindow(); 

        /// <summary>
        /// Sets the Console Window as "topmost", which means no other window can overlap it.  Other windows (including from other applications) will be underneath the console
        /// window until the program ends or SetTopmost(false) is called to remove the TopMost status.
        /// </summary>
        /// <param name="bTopMost"> = when TRUE, sets the Console Window as "topmost".  When FALSE, resets the status so other window may overlap the console window</param>
        /// <returns></returns>
        bool SetTopmost(bool bTopMost = true); 

        /// <summary>
        /// Gets the character width of the Console Window.  This can be used for centering or otherwise aligning text.
        /// <para></para>&#160;&#160;&#160;
        /// For example, using code like conio.printf((GetMaxCharWidth()-TextWidth),-1,"This is the Text");
        /// <para></para>
        /// Will center the text on the current Y position in the console window.
        /// <para></para>&#160;&#160;&#160;--> Note: the text width is the width without and "{}" fields.  If the text has any encoded fields, the length of the text
        /// must be measured separately.
        /// </summary>
        /// <returns>Character width of Console Window</returns>
        int GetMaxCharWidth();

        /// <summary>
        /// Shows the Console Window's cursor if it is hidden.  This can also HIDE the cursor with ShowCursor(false);
        /// </summary>
        /// <param name="bShow">when TRUE, shows the cursor.  When FALSE, hides the cursor.</param>
        /// <returns></returns>
        bool ShowCursor(bool bShow = true);

        /// <summary>
        /// Hides the Console Window's cursor.  This can also SHOW the cursor with HideCursor(false);
        /// </summary>
        /// <param name="bHide">when TRUE, hides the cursor.  When FALSE, shoows the cursor.</param>
        /// <returns></returns>
        bool HideCursor(bool bHide = true);

        /// <summary>
        /// Gets an integer from the console and returns an int value. 
        /// Returns 0 if a number is not entered.
        /// <para></para>
        ///     The buffer is cleared before and after entry for error, providing for sage number entry that can be validated in a loop.
        /// </summary>
        /// <returns>Integer entered or 0 if error or non-number value entered.</returns>
        int GetInteger(); 

        /// <summary>
        /// Gets a long integer from the console and returns a long value. 
        /// Returns 0 if a number is not entered.
        /// <para></para>
        ///     The buffer is cleared before and after entry for error, providing for sage number entry that can be validated in a loop.
        /// </summary>
        /// <returns>Long value entered or 0 if error or non-number value entered.</returns>
        long GetLong();

        /// <summary>
        /// Gets a 64-bit 'long long' type from the console and returns a 'long long' value. 
        /// Returns 0 if a number is not entered.
        /// <para></para>
        ///     The buffer is cleared before and after entry for error, providing for sage number entry that can be validated in a loop.
        /// <para>Same as GetNumber()</para>
        /// </summary>
        /// <returns>64-bit Long Long value entered or 0 if error or non-number value entered.</returns>
        long long GetLongLong(); 


        /// <summary>
        /// Gets a 64-bit 'long long' type from the console and returns a 'long long' value. 
        /// Returns 0 if a number is not entered.
        /// <para></para>
        ///     The buffer is cleared before and after entry for error, providing for sage number entry that can be validated in a loop.
        /// <para>Same as GetLongLong()</para>
        /// </summary>
        /// <returns>64-bit Long Long value entered or 0 if error or non-number value entered.</returns>
        long long GetNumber();

        /// <summary>
        /// Gets a double value from the console and returns the double value. 
        /// Returns 0 if a number is not entered.
        /// <para></para>
        ///     The buffer is cleared before and after entry for error, providing for sage number entry that can be validated in a loop.
        /// </summary>
        /// <returns>Double value entered or 0 if error or non-number value entered.</returns>
        double GetFloat(); 

        /// <summary>
        /// Gets a text line from the user and returns it as a CString object.
        /// <para></para>This provides a safe way to get a string of any length from the user.  
        /// </summary>
        /// <param name="iMax">Maximum string length (default is 2000)</param>
        /// <returns>CString of user entry, or an emptry CString if there was an error or no entry from the user.</returns>
        CString getline(int iMax = 2000); 

        /// <summary>
        /// Clears the cin input buffer. 
        /// </summary>
        /// <param name="bOnErrorOnly">when FALSE (default), the cin buffer is cleared. When TRUE, ths cin buffer is cleared only if it is an error state</param>
        void ClearCin(bool bOnErrorOnly = false);


    };
    class CConsoleIO;
    extern CConsoleIO      _sage_m_cConio;


    extern SageboxInit m_stSageInit;       // Set the default.


}	// namespace Sage
#include "SageColors.h"     // Must be included after Sage.H 
#pragma warning(pop)
#endif // __CSage_H__