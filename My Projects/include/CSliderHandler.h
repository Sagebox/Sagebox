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
#include <Windows.h>

#if !defined(_CSliderHandler_H)
#define _CSliderHandler_H

namespace Sage
{

class CSlider;
class CPasWindow;
class CUserWindow;

struct CEControlAction_t;
class CSliderHandler
{
public:
	enum class WinMsgStatus
	{
		Ok,				// Continue as Normal, returns to Windows whatever Sage has decided for this message
		RetValue,		// Returns the value set in uiReturnValue
		RetDefProc,		// Returns DefProc, as if message didn't happen
		CallAgain,		// Calls at the bottom of the processing of the message, so issues can be handled related to 
						// Sage processing of the message.
						// If CallAgain is returned on the second, post-process call, it is seen as "Ok"
	};
	enum class MsgStatus
	{
		Ignore,			// Ignore any internal processing of this message. Acts as if it didn't happen
		Ok,				// Continue as normal, setting flags such as Pressed(), UnPressed(), etc.
		DontProcess,	// Don't set flags for controls -- Pressed(), UnPressed(), etc, go unfilled.
		CallAgain,		// Calls at the botom of Sage processing.  This will process all pending issues but won't set flags (i.e. Pressed(), etc.)
						// m_bPostProcess = TRUE for the top-level call and FALSE for the bottom level call (which only occurs when CallAgain is returned on the first call)
						// If CallAgain is returned on the second, post-process call, it is seen as "Ok"
	};
private:
	friend CEControlAction_t;		// Allow direct access for faster processing
	CPasWindow * m_cWinCore = nullptr;
	int m_iControlID;
	void * m_pClassInfo = nullptr;
	const char * m_sControlName;
	bool m_bPostProcess;		// Initially FALSE.  When CallAgain is returned, this will add a second call at the bottom of the processing 
								// When it will be set to true.
	int m_iInHandler = false;	// This used when the handler is being deleted and their are active controls that use the handler. 
								// This allows the Delete to wait to make sure we don't delete the memory when we're using it. 
	static bool m_bHandlerTimeout;	// Used for debugging to see if we have any case where the destructor encountered a lengthy wait 
										// to come out of the handler (i.e. m_bInHandler == true).  This looks for bugs, but also 
										// a potential case where some unseen lock issue occurs. 
	CSlider * m_cSlider;
	virtual void HandlerTimeOut() { };			// This is just for debugging to see if there is ever a timeout.  Used for testing.
public:
	virtual void Init(void * pClassInfo)					{ } // Set info to your class type, i.e. MyClassPtr = (MyClass *) pClassInfo, or MyClassRef = & (MyCLass *) pClassInfo
	virtual MsgStatus OnParentNotify(NMHDR * nmh)			{ return MsgStatus::Ok; }	// Continue as normal
	virtual MsgStatus OnPosChange(int iPos,int iOldPos)		{ return MsgStatus::Ok; }
	virtual MsgStatus OnEndScroll(int iPos,int iOldPos)		{ return MsgStatus::Ok; }

	virtual WinMsgStatus OnWinMessage(unsigned int uiMessage,WPARAM wParam,LPARAM lParam,unsigned int & uiReturnCode) { return WinMsgStatus::Ok; }
	virtual const char * GetPosString(int iPosition) { return nullptr; }	

	void SetPostProcess(bool bPostProcess);
	void SetCore(CPasWindow * cWinCore,void * pClassInfo) { m_cWinCore = cWinCore; m_pClassInfo = pClassInfo; Init(pClassInfo); }
	void SetValues(int iControlID,const char * sControlName,void * vpData,bool bPostProcess = false);
	CSliderHandler();
	virtual ~CSliderHandler();
};

}; // namespace Sage
#endif // _CSliderHandler_H