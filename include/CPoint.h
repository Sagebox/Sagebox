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
#if !defined(_CSagePoint_H_)
#define _CSagePoint_H_

// CPoint.cpp -- work in progress

namespace Sage
{

struct SizeRect
{
public:
	POINT loc;
	SIZE size;
	bool Empty() { return !loc.x && !loc.y && !size.cx && !size.cy; };
	bool EmptySize() {return !size.cx && !size.cy; };
	bool EmptyPoint() {return !loc.x && !loc.y; };
	bool EmptyAny() { return (!size.cx && !size.cy) || ( !loc.x && !loc.y); };
};

struct CPoint
{
	int x;
	int y;

	CPoint & operator *= (CPoint & p2)
	{
		x *= p2.x;
		y *= p2.y;

		return { *this };
	}
	CPoint & operator *= (POINT & p2)
	{
		x *= p2.x;
		y *= p2.y;

		return { *this };
	}	
	CPoint & operator *= (SIZE & p2)
	{
		x *= p2.cx;
		y *= p2.cy;

		return { *this };
	}
	CPoint operator * (const SIZE & p2)
	{

		return { x * p2.cx,y*p2.cy };
	}
	CPoint operator * (int iValue) { return { x * iValue,y * iValue }; }
	CPoint operator / (int iValue) { return { x / iValue,y / iValue }; }
	CPoint operator + (int iValue) { return { x + iValue,y + iValue }; }
	CPoint operator - (int iValue) { return { x - iValue,y - iValue }; }

	CPoint & operator *= (int iValue) { x *= iValue; y *= iValue; return *this; }
	CPoint & operator /= (int iValue) { x /= iValue; y /= iValue; return *this; }
	CPoint & operator += (int iValue) { x += iValue; y += iValue; return *this; }
	CPoint & operator -= (int iValue) { x -= iValue; y -= iValue; return *this; }

	CPoint operator * (const POINT & p2)
	{

		return { x * p2.x,y*p2.y };
	}
	
	CPoint operator * (const CPoint & p2)
	{

		return { x * p2.x,y*p2.y };
	}
	CPoint operator + (const CPoint & p2)
	{
		return { x + p2.x, y+p2.y };
	}
	CPoint operator + (const POINT & p2)
	{
		return { x + p2.x, y+p2.y };
	}

	CPoint & operator += (const CPoint & p2)
	{
		x += p2.x;
		y += p2.y;
		return *this;
	}
	CPoint & operator += (const POINT & p2)
	{
		x += p2.x;
		y += p2.y;
		return *this;
	}
	CPoint & operator += (const SIZE & p2)
	{
		x += p2.cx;
		y += p2.cy;
		return *this;
	}
	CPoint operator - (const CPoint & p2)
	{
		return { x - p2.x, y-p2.y };
	}
	CPoint & operator -= (const CPoint & p2)
	{
		x -= p2.x;
		y -= p2.y;
		return *this;
	}

	POINT operator * () { return { x,y }; }
	CPoint(int iX,int iY)
	{
		x = iX;
		y = iY;
	}
	CPoint(const POINT p)
	{
		x = p.x;
		y = p.y;
	}
	CPoint(const SIZE sz)
	{
		x = sz.cx;
		y = sz.cy;
	}
	operator POINT() const { POINT p = { x,y }; return p; };
	operator SIZE() const { SIZE p = { x,y }; return p; };
    CPoint & operator = (const CPoint & p2)
    {
        x = p2.x;
        y = p2.y;
        return *this;
    }
 
    CPoint(const CPoint & p2)
    {
        x = p2.x;
        y = p2.y;
    }
   CPoint & operator = (CPoint && p2) noexcept
    {
        x = p2.x;
        y = p2.y;
        return * this;
    }
    CPoint(CPoint && p2) noexcept
    {
        x = p2.x;
        y = p2.y;
    }
	CPoint() { }
};

struct CfPoint
{
	double x;
	double y;

	CfPoint & operator = (const CPoint & p)
	{
		x = (double) p.x;
		y = (double) p.y;
		return (*this);
	}
	CfPoint & operator = (const CfPoint & p)
	{
		x = (double) p.x;
		y = (double) p.y;
		return (*this);
	}
	CfPoint operator + (const CfPoint & p2)
	{
		return { x + p2.x, y+p2.y };
	}
	CfPoint operator / (const CfPoint & p2)
	{
		return { x / p2.x, y/p2.y };
	}
	CfPoint & operator /= (const CfPoint & p2)
	{
		x /= p2.x;
		y /= p2.y;
		return *this;
	}
	CfPoint operator / (double fDiv)
	{
		return { x /fDiv, y/fDiv };
	}
	CfPoint & operator /= (double fDiv)
	{
		x /= fDiv;
		y /= fDiv;
		return *this;
	}
	CfPoint & operator *= (double fMul)
	{
		x *= fMul;
		y *= fMul;
		return *this;
	}
	CfPoint operator * (double fMul)
	{
		return { x * fMul, y*fMul };
	}
	CfPoint operator - (const CfPoint & p2)
	{
		return { x - p2.x, y-p2.y };
	}
	CfPoint operator - (const CPoint & p2)
	{
		return { x - (double) p2.x, y- (double) p2.y };
	}
	CfPoint & operator += (const CfPoint & p2)
	{
		x += p2.x;
		y += p2.y;
		return *this;
	}
	CfPoint & operator *= (const CfPoint & p2)
	{
		x *= p2.x;
		y *= p2.y;
		return *this;
	}
	CfPoint operator * (const CfPoint & p2)
	{
		return { x * p2.x, y * p2.y };
	}
    CfPoint & operator -= (const CfPoint & p2)
	{
		x -= p2.x;
		y -= p2.y;
		return *this;
	}
    CfPoint(const CfPoint & p2)
    {
        x = p2.x;
        y = p2.y;
    }
    
    CfPoint(CfPoint && p2) noexcept
    {
        x = p2.x;
        y = p2.y;
    }

	CfPoint() { };
	CfPoint(double fx,double fy) { x = fx; y = fy; };
	CfPoint(const CPoint & p)
	{
		x = (double) p.x;
		y = (double) p.y;
	}
	CfPoint(const POINT & p)
	{
		x = (double) p.x;
		y = (double) p.y;
	}
    CfPoint & operator =(CfPoint && p2)
    {
        x = p2.x;
        y = p2.y;
        return * this;
    }
	CfPoint(const SIZE & szSize)
	{
		x = (double) szSize.cx;
		y = (double) szSize.cy;
	}
	POINT operator * () { return { (int) x,(int) y }; }
};
}; // namespace Sage
#endif // _CSagePoint_H_
