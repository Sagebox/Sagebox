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

#if !defined(_CControlsStyles_h_)
#define _CControlsStyles_h_
#include "windows.h"
#include "Sage.h"
//#pragma warning( disable : 4996) 


class CControlStyles
{
public:
struct stControlStyles_t
{
	// SubStyle structure -- each substyle can has a name and corresponding value with an optional data pointer
	struct stSubStyles_t
	{
		char sName[50];
		int iValue;
		bool bFound;
		void * vpData;
	};
	stSubStyles_t * stSubStyles;
	Sage::ControlStyles eStyle;
	Sage::ControlSubStyles eSubStyle;
	bool bSubStyleError;	// When a subtyle is searched, when true, this signals that a substyle reference included but not found.
							// In this case, it will return the control default conrol style to make the error passive, and it is up to the 
							// caller to determine any course of action with regard to a not-found on a specified substyle
	int iNumSubStyles;		// For each style, there may be substyles, such as MyButton:Dark, or MyButton:Large:Dark -- no substyle can be named the same. 
	char sName[100];
	char sAlias[100];		// Internal davinci alias name, for defaults, etc.  i.e. MyCompButtonWhite1 can be aliased as "DefaultCheckbox", etc.
							// Also, the button name does not need to be explicitly known, so MyButtonName = LoadButtonStyle(...)
							// followed with SetControlStyleAlias(MyButtonName,"RobsButton1", etc.
							// Alias will then supercede name, except for query functions such as GetInternalName(), etc.
							// $$ also add funcition isAliased() to return whether or not the reference name is the same as the Alias (i.e. if Alias exists). TBD
	union StyleTypes_t
	{
		Sage::stGraphicButton_t stGraphicButton	;
		Sage::stGraphicBitmap_t stGraphicBitmap	;
		Sage::stComponentBitmap_t stComponentButton	;	// Component Button or Checkbox
	};

	StyleTypes_t stStyle;

	stControlStyles_t * stNext; 
public:
	bool AddSubStyle(int iStyleValue,char * sStyleName,void * vpData = nullptr);

	void Init()
	{
		iNumSubStyles = 0;
		stSubStyles = nullptr;
		eStyle = Sage::ControlStyles::Uninitialized;
		eSubStyle = Sage::ControlSubStyles::Unknown;
		sName[0] = 0;
	}
	void Delete()
	{
		if (stSubStyles) free(stSubStyles);
		stSubStyles = nullptr;
	}
};

private:
	stControlStyles_t		* m_stControlStyles = nullptr;
	int m_iNumControlStyles = 0;

public:
	bool AddControlStyle(Sage::stGraphicButton_t & stButton); 
	bool AddControlStyle(Sage::stComponentBitmap_t & stButton); 
	bool AddControlStyle(Sage::ControlStyles eStyle,Sage::ControlSubStyles eSubStyle,char * sName,void * stStyle,int iSize);
	bool SetStyleAlias(Sage::ControlStyles eStyle,char * sStyle,char * sAlias);
	stControlStyles_t * GetControlStyle(Sage::ControlStyles,char * sName);

	int GetNumControlStyles() { return m_iNumControlStyles; }
	stControlStyles_t * GetControlStyleStruct() {return m_stControlStyles; }

	CControlStyles();
	~CControlStyles();
};
#endif // _CControlsStyles_h_