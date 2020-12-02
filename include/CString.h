//#pragma once
#if !defined(_CSageString_H_)
#define _CSageString_H_
//#pragma warning( disable : 4996) 
#include <Windows.h>
#include <string>

namespace Sage
{
class CString
{
	class CStringW;

private:
	int m_iMaxSize		= 300; 
	int m_iMinAllocSize	= 1000;
	int m_iAllocSize;
	char * s;
	char sStatic[301];
	char * sAlloc;
	int iPlace;
public:
	char * Return(int x);


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
//	operator void * () const { return (void *) s; }
	void Init()
	{
		iPlace				= 0; 
		sStatic[0]			= 0; 
		sStatic[m_iMaxSize] = 0;
		s = (char *) sStatic;
		m_iAllocSize		= 0;
		sAlloc				= nullptr;
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

};


class CStringW
{
private:
	int m_iMaxSize		= 300; 
	int m_iMinAllocSize	= 1000;
	int m_iAllocSize;
	wchar_t * s;
	wchar_t sStatic[301];
	wchar_t * sAlloc;
	int iPlace;
public:
	wchar_t * Return(int x);


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
	}
	~CStringW()
	{
		if (sAlloc) free(sAlloc);
	}
	CStringW() 
	{ 
		Init();
	}
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