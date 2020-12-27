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
#if !defined(_SageString_H_)
#define _SageString_H_
namespace Sage
{
struct SageStringW;
struct SageString
{
	int iStringSize;
	int iMemSize;
	char * spData;
public:
	void Init();
	void Delete();
	void Clear();
	void CloneSelf();
	char * GetPureStr();
	char * GetMem(int iSize);
	char * SetString(char * sString);
	char * SetString(wchar_t * sString);
	char * AddString(char * sString);
	char * operator * ();
	 operator char * ();
	 operator const char * ();
	SageString & operator = (SageStringW & sString);
	SageString & operator = (char * sString);
	SageString & operator = (const char * sString);
	SageString & operator += (char * sString);
	SageString & operator += (SageString & stString);

};


struct SageStringW
{
	int iStringSize;
	int iMemSize;
	wchar_t * spData;
public:
	void Init();
	void Delete();
	void Clear();

	void CloneSelf();
	wchar_t * GetPureStr();
	wchar_t * GetMem(int iSize);
	wchar_t * SetString(char * sString);
	wchar_t * SetString(wchar_t * sString);
	wchar_t * AddString(char * sString);
	wchar_t * AddString(wchar_t * sString);
	wchar_t * operator * ();
	operator wchar_t * () ;
	operator const wchar_t * ();
	
	SageStringW & operator = (char * sString);
	SageStringW & operator = (const char * sString);
	SageStringW & operator += (char * sString);
	SageStringW & operator += (SageString & stString);

	SageStringW & operator = (wchar_t * sString);
	SageStringW & operator = (const wchar_t * sString);
	SageStringW & operator += (wchar_t * sString);
	SageStringW & operator += (SageStringW & stString);
};


}; // namepace Sage

#endif // _SageString_H_