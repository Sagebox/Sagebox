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
#if !defined(_CDevString_H_)
#define _CDevString_H_
//#pragma warning( disable : 4996) 
#include <Windows.h>
class CDevString
{
public:
	int m_iMaxSize = 1200; 
	char s[1201];
	int iPlace;
public:
	char * Return(int x);
	CDevString & operator << (const char * x) { AddString(x); return((CDevString &) *this); }
	CDevString & operator >> (const char * x) { StartString(x); return((CDevString &) *this); }
	CDevString & operator << (const wchar_t * x) { AddString(x); return((CDevString &) *this); }
	CDevString & operator >> (const wchar_t * x) { StartString(x); return((CDevString &) *this); }
	CDevString & operator >> (int x) { StartString(x); return((CDevString &) *this); }
	CDevString & operator << (int x) { AddNumber(x); return((CDevString &) *this); }
	CDevString & operator << (unsigned int x) { AddNumber((int) x); return((CDevString &) *this); }
//	CDevString & operator << (COLORREF x) { AddNumber((int) x); return((CDevString &) *this); }
	char * operator * () { return (char *) s; };
//	const char * operator * () { return (const char *) s; };
	CDevString & operator << (DWORD x) { AddNumber((unsigned int) x); return((CDevString &) *this); }
	CDevString & operator >> (DWORD x) {  StartString((unsigned int) x); return((CDevString &) *this); }

	CDevString & operator << (double x2) { AddDouble(x2); return((CDevString &) *this); }
	char * operator * (int x) { return(Return(x)); }
//	char * operator * (float x) { return(Return((int) x)); }
	operator char * () { return (char *) s; }
	CDevString() { iPlace = 0; s[0] = 0; s[1200] = 0; }
	CDevString(char * sString) { iPlace = 0; s[0] = 0; s[1200] = 0; AddString(sString);  }
	CDevString(const char * sString) { iPlace = 0; s[0] = 0; s[1200] = 0; AddString(sString);  }

	void StartString(const char * sString); 
	void StartString(const wchar_t * sString); 
	void StartString(int iValue); 
	void StartString(unsigned int uiValue); 
	void AddString(const char * sString); 
	void AddString(const wchar_t * sString); 
	void AddNumber(int x); 
	void AddNumber(unsigned int x); 
    void AddDouble(double x2);

};
#endif	// _CDevString_H_