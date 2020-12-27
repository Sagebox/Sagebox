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
#if !defined(_CSyleDefaults_h_)
#define _CSyleDefaults_h_
#include <Windows.h>
#include "Sage.h"
//#pragma warning( disable : 4996) 

namespace Sage
{
class CStyleDefaults
{
public:
	enum class ControlType
	{
		Button,
		Checkbox,
		RadioButton,
		EditBox,
		ListBox,
		ComboBox,
	};

private:
	bool m_bBusy = false;
	int m_iNumDefaults = 0;
	const int m_iMaxControls = 10;
	struct stDefaults_t
	{
		int iNumControls;				// Number of controls defined in the default
		char sDefaultName[50];			// i.e. "default", "defaultSmall", etc.
		char sDefaultDefault[50];		// Where to look for a class type if the default name was found, but control name wasn't
										// For example, DefaultSmall for a RadioButton may not exist, so it will default to "Default" and try to find 
										// a "default" radiobutton.  This allows generic types such as Default, DefaultLarge, DefaultSmall, etc. to exist without
										// worrying the user whether or not the control exists for that default type. 
		
										// If the default name is not found (i.e. someone entered "defaultXL") and it doesn't exist as a class, then any request is returned as a failure,
										// and it is up to the calling function to decide what the default behavior is for not-found or not-specified in terms of the class name
					
										// note: It is typically always assumed (by the program-at-large) that "default" exists -- this must be established by the system during initalization.
		struct stControl_t
		{
			ControlType eControlType;	
			char sControlName[50];
		};

		stControl_t stControls[10];		// Up to 10 different control types, 50 chars max. 
	};

	stDefaults_t	* m_stDefaultClasses = nullptr; 

public:
	bool AddDefaultClass(char * sDefaultClassName,bool AutoDefault = false);
	bool AddControl(char * sDefaultClassName,ControlType eControlType,char * sControlName); 
	bool SetNotFoundDefault(char * sDefaultClassName,char * sDefaultName);
	Sage::SageString200 GetControlName(char * sClassName,ControlType eControlType);


	~CStyleDefaults();
};
}; // namespace Sage
#endif //_CSyleDefaults_h_