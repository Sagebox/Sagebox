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
#if !defined(_CSageString_H_)
#define _CSageString_H_
//#pragma warning( disable : 4996) 
#include <Windows.h>
#include <string>

namespace Sage
{
class CString;
class CStringW;
using CStr = CString;
using CStrW = CStringW;
class CString
{
	class CStringW;
public:
    struct csFloatType
    {
        const char * sFloatStyle;
    };

private:
    // Since Init() is in the interface, this can be changed to set a different default style for all CString() usage.
    static constexpr const char * kDefaultFloatStyle = "%g";
	int m_iMaxSize		= 300; 
	int m_iMinAllocSize	= 1000;
	int m_iAllocSize;
	char * s;
	char sStatic[301];
    char sFloatStyle[10];   // String for the float style, to override the default
                            // This allows the caller to set how floating-point values are output. 
	char * sAlloc;
	int iPlace;
    bool bDefaultFloatStyle;
    void SetFloatStyle(csFloatType csStyle);
public:
	char * Return(int x);

    // CString::fs() -- Set the floating-point output style. 
    //
    // This can be used via streaming, i.e. CString() << "This is a number " << fs("%.10f") << MyNumber; 
    // It can be used anywhere or anytime with the current CString(), but does not take effect for numbers preceding the 
    // fs() call. 
    //
    // Note: When using the default style, 0 is represented as "0" and trailing 0's are removed.
    // Setting the float style, this no longer occurs to support the new style.  This also means that setting the style
    // to the same string as the default style will also disable the post-formatting.
    //
    // Use fs() to restore the default style. 
    //
    CString & fs(const char * sStyle = nullptr) { SetFloatStyle({sStyle}); return *this; }


    // GetDefaultFloatStyle() -- returns the default floating-point style for CString floating-point output.
    // Use CString::fs() to set the floating-point style. 
    //
    const char * GetDefaultFloatStyle() { return kDefaultFloatStyle; };

//    __forceinline CString & FloatStyle(const char * sStyle) { return _fsX(sStyle); } // Inlined so we don't do a double call

	CString & operator << (std::string & x)		{ AddString((char *) x.c_str()); return((CString &) *this); }
//	CString & operator << (char * x)			{ AddString(x); return((CString &) *this); }
	CString & operator << (const char * x)		{ AddString((char *) x); return((CString &) *this); }
	CString & operator << (wchar_t * x)			{ AddString(x); return((CString &) *this); }
	CString & operator << (const wchar_t * x)	{ AddString((wchar_t *) x); return((CString &) *this); }
	CString & operator << (int x)				{ AddNumber(x); return((CString &) *this); }
	CString & operator << (long x)				{ AddNumber((int) x); return((CString &) *this); }
	CString & operator << (unsigned int x)		{ AddNumber((int) x); return((CString &) *this); }
	CString & operator << (DWORD x)				{ AddNumber((unsigned int) x); return((CString &) *this); }
	CString & operator << (double x2)			{ AddDouble(x2); return((CString &) *this); }
	CString & operator << (csFloatType csft)	{ SetFloatStyle(csft); return *this; }

	CString & operator >> (std::string & x)		{ StartString((char *) x.c_str()); return((CString &) *this); }
	CString & operator >> (char * x)			{ StartString(x); return((CString &) *this); }
	CString & operator >> (const char * x)		{ StartString((char *) x); return((CString &) *this); }
	CString & operator >> (wchar_t * x)			{ StartString(x); return((CString &) *this); }
	CString & operator >> (const wchar_t * x)	{ StartString((wchar_t *) x); return((CString &) *this); }
	CString & operator >> (int x)				{ StartString(x); return((CString &) *this); }
	CString & operator >> (long x)				{ StartString((int) x); return((CString &) *this); }
	CString & operator >> (unsigned int x)		{ StartString((int) x); return((CString &) *this); }
	CString & operator >> (DWORD x)				{ StartString((unsigned int) x); return((CString &) *this); }
	CString & operator >> (double x2)			{ iPlace = 0; AddDouble(x2); return((CString &) *this); }

	char * operator * () { return (char *) s; };
	const char * str() { return s; };
	char * c_str() { return s; };
	const char * cc_str() { return (const char *)  s; };
	char * GetBuffer() { return s; };
	bool isEmpty(){ return  s && *s ? false : true; };

	char * operator * (int x) { return(Return(x)); }
//	char * operator * (float x) { return(Return((int) x)); }
//	CString & operator = (char * x)			{ if (x == s) return *this; ClearString(); return *this << x; }
	CString & operator = (const char * x)	{ if (x == s) return *this; ClearString(); return *this << x; }
	CString & operator = (std::string & x)	{	ClearString(); return *this << x; }
	CString & operator = (const CString & x)		{	ClearString(); return *this << x; }
	CString & operator = (int x)			{	ClearString(); return *this << x; }
	CString & operator = (unsigned int x)	{	ClearString(); return *this << x; }
	CString & operator = (DWORD x)			{	ClearString(); return *this << x; }

	operator const char * () const { return (const char *) s; }
	operator char * () const { return (char *) s; }
	
	void Init()
	{
		iPlace				= 0; 
		sStatic[0]			= 0; 
		sStatic[m_iMaxSize] = 0;
		s = (char *) sStatic;
		m_iAllocSize		= 0;
		sAlloc				= nullptr;
        strcpy(sFloatStyle,kDefaultFloatStyle);
        bDefaultFloatStyle = true;
	}
	~CString()
	{
		if (sAlloc) free(sAlloc);
	}
	CString() 
	{ 
		Init();
	}
	CString(int iMem)
	{
		AllocateMem(iMem);
	}

	CString(const char * sString) 
	{
		Init();
		AddString((char *) sString);  
	}
	CString(const wchar_t * sString)
	{
		Init();
		AddString((wchar_t *) sString);
	}
	CString(const CString &p2);

    CString(CString && p2) noexcept;
  
	int getMemSize()
	{
		return m_iAllocSize ? m_iAllocSize : m_iMaxSize;
	}
	bool AllocateMem(int iMemSize);
	void ClearString()
	{
		iPlace = 0; 
		sStatic[0] = 0;
		if (sAlloc) *sAlloc = 0;
	}
  
	char * UnEmptyStr()  { return (*s) ? s : nullptr; }
	char * PureStr()  { return (*s) ? s : nullptr; }

	void StartString(char * sString); 
	void StartString(wchar_t * sString); 
	void StartString(int iValue); 
	void StartString(unsigned int uiValue); 
	void AddString(char * sString); 
	void AddString(wchar_t * sString); 
	void AddNumber(int x); 
	void AddNumber(unsigned int x); 
    void AddDouble(double x2);
    const char * printf(const char * Format,...);
};


class CStringW
{
public:
    struct csFloatType
    {
        const char * sFloatStyle;
    };
private:
    static constexpr const char * kDefaultFloatStyle = "%g";
	int m_iMaxSize		= 300; 
	int m_iMinAllocSize	= 1000;
	int m_iAllocSize;
	wchar_t * s;
	wchar_t sStatic[301];
    char sFloatStyle[10];   // String for the float style, to override the default

	wchar_t * sAlloc;
	int iPlace;
    bool bDefaultFloatStyle;
    void SetFloatStyle(csFloatType csStyle);
public:
	wchar_t * Return(int x);

    // CStringW::fs() -- Set the floating-point output style. 
    //
    // This can be used via streaming, i.e. CStringW() << "This is a number " << fs("%.10f") << MyNumber; 
    // It can be used anywhere or anytime with the current CStringW(), but does not take effect for numbers preceding the 
    // fs() call. 
    //
    // Note: When using the default style, 0 is represented as "0" and trailing 0's are removed.
    // Setting the float style, this no longer occurs to support the new style.  This also means that setting the style
    // to the same string as the default style will also disable the post-formatting.
    //
    // Use fs() to restore the default style. 
    //
    // Note: fs() takes a char value, not wchar_t -- i.e. fs(".4f") works, but not fs(L".4f");
    //
    CStringW & fs(const char * sStyle = nullptr) { SetFloatStyle({sStyle}); return *this; }

	CStringW & operator << (std::string & x)	{ AddString((wchar_t *) x.c_str()); return((CStringW &) *this); }
	CStringW & operator << (wchar_t * x)		{ AddString(x); return((CStringW &) *this); }
	CStringW & operator << (const wchar_t * x)	{ AddString((wchar_t *) x); return((CStringW &) *this); }
	CStringW & operator << (char * x)			{ AddString(x); return((CStringW &) *this); }
	CStringW & operator << (const char * x)		{ AddString((char *) x); return((CStringW &) *this); }
	CStringW & operator << (int x)				{ AddNumber(x); return((CStringW &) *this); }
	CStringW & operator << (unsigned int x)		{ AddNumber((int) x); return((CStringW &) *this); }
	CStringW & operator << (DWORD x)			{ AddNumber((unsigned int) x); return((CStringW &) *this); }

	CStringW & operator >> (std::string & x)	{ StartString((wchar_t *) x.c_str()); return((CStringW &) *this); }
	CStringW & operator >> (wchar_t * x)		{ StartString(x); return((CStringW &) *this); }
	CStringW & operator >> (const wchar_t * x)	{ StartString((wchar_t *) x); return((CStringW &) *this); }
	CStringW & operator >> (char * x)			{ StartString(x); return((CStringW &) *this); }
	CStringW & operator >> (const char * x)		{ StartString((char *) x); return((CStringW &) *this); }
	CStringW & operator >> (int x)				{ StartString(x); return((CStringW &) *this); }
	CStringW & operator >> (unsigned int x)		{ StartString((int) x); return((CStringW &) *this); }
	CStringW & operator >> (DWORD x)			{ StartString((unsigned int) x); return((CStringW &) *this); }


//	CDevString & operator << (COLORREF x) { AddNumber((int) x); return((CDevString &) *this); }
	wchar_t * operator * () { return (wchar_t *) s; };

	CStringW & operator << (double x2) {  AddDouble(x2); return((CStringW &) *this); }
	CStringW & operator << (csFloatType csft)	{ SetFloatStyle(csft); return *this; }
	CStringW & operator >> (double x2) { iPlace = 0; AddDouble(x2); return((CStringW &) *this); }
	wchar_t * operator * (int x) { return(Return(x)); }
//	char * operator * (float x) { return(Return((int) x)); }
	CStringW & operator = (wchar_t * x)			{ if (x == s) return *this;	ClearString();	return *this << x; }
	CStringW & operator = (const wchar_t * x)	{ if (x == s) return *this;	ClearString();	return *this << (wchar_t *) x; }

	CStringW & operator = (char * x)			{  ClearString();	return *this << x; }
	CStringW & operator = (const char * x)		{  ClearString();	return *this << (char *) x; }
	CStringW & operator = (std::string & x)	{	ClearString();	return *this << x; }
	CStringW & operator = (int x)			{	ClearString();	return *this << x; }
	CStringW & operator = (unsigned int x)	{	ClearString();	return *this << x; }
	CStringW & operator = (DWORD x)			{	ClearString();	return *this << x; }

	operator const wchar_t * () const { return (const wchar_t *) s; }
	operator wchar_t * () const { return (wchar_t *) s; }
	void Init()
	{
		iPlace				= 0; 
		sStatic[0]			= 0; 
		sStatic[m_iMaxSize] = 0;
		s = (wchar_t *) sStatic;
		m_iAllocSize		= 0;
		sAlloc				= nullptr;
        strcpy(sFloatStyle,kDefaultFloatStyle);
        bDefaultFloatStyle = true;
	}
	~CStringW()
	{
		if (sAlloc) free(sAlloc);
	}
	CStringW() 
	{ 
		Init();
	}
    CStringW(CStringW && p2) noexcept;

	CStringW(const wchar_t * sString) 
	{
		Init();
		AddString((wchar_t *) sString);  
	}
		CStringW(const char * sString) 
	{
		Init();
		AddString((char *) sString);  
	}

	CStringW(const CStringW &p2);

	int getMemSize()
	{
		return m_iAllocSize ? m_iAllocSize : m_iMaxSize;
	}
	bool AllocateMem(int iMemSize);
	void ClearString()
	{
		iPlace = 0; 
		sStatic[0] = 0;
		if (sAlloc) *sAlloc = 0;
	}
  
	wchar_t * UnEmptyStr()  { return (*s) ? s : nullptr; }
	bool isEmpty(){ return  s && *s ? false : true; };

	void StartString(wchar_t * sString); 
	void StartString(char * sString); 
	void StartString(int iValue); 
	void StartString(unsigned int uiValue); 
	void AddString(wchar_t * sString); 
	void AddString(char * sString); 
	void AddNumber(int x); 
	void AddNumber(unsigned int x); 
    void AddDouble(double x2);

};
}; // namespace Sage
#endif	// _CSageString_H_