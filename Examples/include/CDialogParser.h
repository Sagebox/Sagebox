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

#if !defined(_CDialogParser_H_)
#define _CDialogParser_H_
#include "CString.h"
#include <vector>
namespace CSageScript { class CCompLink; }
namespace Sage
{
class CSageScipt;

class CWidget;
class CDialogParser
{
public:
	enum class ControlType
	{
		Unknown,
		Button,
		EditBox,
		Window,
		Slider,
		TextWidget,
		BitmapWidget,
		Bitmap,
		ListBox,
		Widget,
	};

	struct stControl_t
	{
		ControlType controlType;
		int iX;
		int iY;
		int iWidth;
		int iHeight;
		int ID;
		CString csName;
		CString csOptions;
		CString csText;
		CWidget * cWidget;
		void * pObject;
		HWND  hWnd;	// $$ This will probably change once the CWidget issues get more solidified.
						//  hwnd is currently used selecively, for WIndows and Widgets to get the actual window size.
						// For other controls, it is known that we can use the Client Size, so the Window is left as a nullptr

	};
private:
	enum class OptionNames
	{
		X,
		Y,
		W,
		H,
		Width,
		Height,
		Name,
		ID,
		Transparent,
		Hidden,
		fgColor,
		bgColor,
		Style,
		Text,
	};
	enum OptValueType
	{
		Num,
		String,
		Undefined,
	};
	struct stValue_t
	{
			
		char * sValueName;
		char * sValueString;

		int iValue; 

		OptValueType valueType;
		OptionNames optionName;
			
		stValue_t & Init()
		{
			*this = {};
			valueType = OptValueType::Undefined;
			return *this;
		}
	};


	struct stOptDefine_t
	{
		enum class stOptDefineType
		{
			Int,
			String,
			Bool,
			Color,
		};
		const char * sName;
		stOptDefineType optType;
		OptionNames optionName;
		stValue_t stValue;
		stOptDefine_t * GetItem(OptionNames option)
		{
			stOptDefine_t * stTemp = this;	// Note this assumes this is called with the main array, not a sub-item

			while(stTemp->sName)
			{
				if (stTemp->optionName == option) return stTemp->stValue.valueType != OptValueType::Undefined ? stTemp : nullptr;
				stTemp++;
			}
			return nullptr;
		}
	};


	std::vector<stControl_t> m_stControls;

	CSageScript::CCompLink * m_cComp;
	struct stControlTypes_t
	{
		ControlType controlType;
		CString csName;
	};

	struct stOptNames_t
	{
		CString csName;
	};

	struct stStockColors_t
	{
		CString csName;
	};
	std::vector<stControlTypes_t>	m_stControlTypes;
	std::vector<stStockColors_t>	m_stStockColors;

	stOptDefine_t  * m_stOptDefines = nullptr;
	const char * * m_sStockColors	= nullptr;

	char m_sControlNameStr[2000]	= {};
	bool m_bEOF						= false;
	bool m_bError					= false;
	void InitDefaults();
	void SetEOF() { m_bEOF = true; };
	bool isEOF() { return m_bEOF; }
	void ProcessDeclaration();
	bool ValidateDeclaration(stControl_t & stControl);
	stValue_t getOptValue(const char * sName,OptionNames optionName) 	;
	stValue_t getOptBool(const char * sName,OptionNames optionName) 	;
	stValue_t getOptInt(const char * sName,OptionNames optionName) 		;
	stValue_t getOptColor(const char * sName,OptionNames optionName) 	;
	stOptDefine_t * getOption(OptionNames option) { return m_stOptDefines[0].GetItem(option); };

	char * ReverseWhiteSpace(char * sPointer,char * sBegin);
	char * SkipToWhiteSpace(char * sPointer);
	char * SkipWhiteSpace(char * sPointer);
	char * SkiptoBreak(char * sPointer,char cBreak);



public:
	CDialogParser();
	~CDialogParser();

	bool Init(char * sPointer);
	CDialogParser::stControlTypes_t * FindControlName();
	char * FindSegment();
	void Reverse() { m_sPointer = m_sPrevPointer; }

	__int64 m_iDialogDiff = 0;
	char * m_sControlName		= nullptr;
	char * m_sPointer			= nullptr;
	CString m_sDialog		;
	CString m_sOrgDialog	;
	char * m_sPrevPointer		= nullptr;
	char * m_sSegment			= nullptr;
	char * m_sPrevSegment		= nullptr;

	bool CompileDialog();
	int GetNumControls() { return (int) m_stControls.size(); }
	std::vector<stControl_t> & GetControls() { return m_stControls; }

};
}; // namespace Sage
#endif // _CDialogParser_H_