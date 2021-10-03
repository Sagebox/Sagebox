#pragma once

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

#pragma once
#include <Windows.h>


#if !defined(_CSageSize_H_)
#define _CSageSize_H_

// CSize.cpp -- work in progress

#pragma warning(push)
#pragma warning(disable : 26495)

namespace Sage
{

struct Point3D_t;

struct CSize
{
	int cx;
	int cy;

	CSize & operator *= (CSize & p2)
	{
		cx *= p2.cx;
		cy *= p2.cy;

		return { *this };
	}

	CSize & operator *= (SIZE & p2)
	{
		cx *= p2.cx;
		cy *= p2.cy;

		return { *this };
	}
	CSize operator * (const SIZE & p2)     // $ SIZE
	{

		return { cx * p2.cx,cy*p2.cy };
	}
	__forceinline CSize operator * (int iValue) { return { cx * iValue,cy * iValue }; }
	__forceinline CSize operator / (int iValue) { return { cx / iValue,cy / iValue }; }
	__forceinline CSize operator + (int iValue) { return { cx + iValue,cy + iValue }; }
	__forceinline CSize operator - (int iValue) { return { cx - iValue,cy - iValue }; }

	__forceinline CSize & operator *= (int iValue) { cx *= iValue; cy *= iValue; return *this; }
	__forceinline CSize & operator /= (int iValue) { cx /= iValue; cy /= iValue; return *this; }
	__forceinline CSize & operator += (int iValue) { cx += iValue; cy += iValue; return *this; }
	__forceinline CSize & operator -= (int iValue) { cx -= iValue; cy -= iValue; return *this; }

	
	CSize operator * (const CSize & p2)
	{

		return { cx * p2.cx,cy*p2.cy };
	}
	CSize operator + (const CSize & p2)
	{
		return { cx + p2.cx, cy+p2.cy };
	}
	CSize operator + (const SIZE & p2)
	{
		return { cx + p2.cx, cy+p2.cy };
	}

	CSize & operator += (const CSize & p2)
	{
		cx += p2.cx;
		cy += p2.cy;
		return *this;
	}
	CSize & operator += (const SIZE & p2)
	{
		cx += p2.cx;
		cy += p2.cy;
		return *this;
	}

	CSize operator - (const CSize & p2)
	{
		return { cx - p2.cx, cy-p2.cy };
	}
	CSize & operator -= (const CSize & p2)
	{
		cx -= p2.cx;
		cy -= p2.cy;
		return *this;
	}
    CSize operator = (Point3D_t & p) const;

	SIZE operator * () { return { cx,cy }; }
	CSize(int iX,int iY)
	{
		cx = iX;
		cy = iY;
	}
	CSize(const SIZE p)
	{
		cx = p.cx;
		cy = p.cy;
	}

	operator SIZE() const { SIZE p = { cx,cy }; return p; };
    CSize & operator = (const CSize & p2)
    {
        cx = p2.cx;
        cy = p2.cy;
        return *this;
    }
 
    CSize(const CSize & p2)
    {
        cx = p2.cx;
        cy = p2.cy;
    }
   CSize & operator = (CSize && p2) noexcept
    {
        cx = p2.cx;
        cy = p2.cy;
        return * this;
    }
    CSize(CSize && p2) noexcept
    {
        cx = p2.cx;
        cy = p2.cy;
    }
    CSize(const Point3D_t & p);
	CSize() { }
    CSize operator = (Point3D_t & p);

    __forceinline CSize _min(CSize & c) { return {min(cx,c.cx),min(cy,c.cy)}; }
    __forceinline CSize _max(CSize & c) { return {max(cx,c.cx),max(cy,c.cy)}; }
    __forceinline CSize _min(SIZE & c) { return {min(cx,c.cx),min(cy,c.cy)}; }
    __forceinline CSize _max(SIZE & c) { return {max(cx,c.cx),max(cy,c.cy)}; }
};

struct CfSize
{
	double cx;
	double cy;

	CfSize & operator = (const CSize & p)
	{
		cx = (double) p.cx;
		cy = (double) p.cy;
		return (*this);
	}
	CfSize & operator = (const CfSize & p)
	{
		cx = (double) p.cx;
		cy = (double) p.cy;
		return (*this);
	}
	CfSize operator + (const CfSize & p2)
	{
		return { cx + p2.cx, cy+p2.cy };
	}
	CfSize operator / (const CfSize & p2)
	{
		return { cx / p2.cx, cy/p2.cy };
	}
	CfSize & operator /= (const CfSize & p2)
	{
		cx /= p2.cx;
		cy /= p2.cy;
		return *this;
	}

	CfSize operator - (const CfSize & p2)
	{
		return { cx - p2.cx, cy-p2.cy };
	}
	CfSize operator - (const CSize & p2)
	{
		return { cx - (double) p2.cx, cy- (double) p2.cy };
	}
	CfSize & operator += (const CfSize & p2)
	{
		cx += p2.cx;
		cy += p2.cy;
		return *this;
	}
	CfSize & operator *= (const CfSize & p2)
	{
		cx *= p2.cx;
		cy *= p2.cy;
		return *this;
	}
	CfSize operator * (const CfSize & p2)
	{
		return { cx * p2.cx, cy * p2.cy };
	}
    CfSize & operator -= (const CfSize & p2)
	{
		cx -= p2.cx;
		cy -= p2.cy;
		return *this;
	}
    CfSize(const CfSize & p2)
    {
        cx = p2.cx;
        cy = p2.cy;
    }
    
    CfSize(CfSize && p2) noexcept
    {
        cx = p2.cx;
        cy = p2.cy;
    }
    CfSize(CSize & p2) noexcept
    {
        cx = p2.cx;
        cy = p2.cy;
    }
    CfSize(SIZE & p2) noexcept
    {
         cx = p2.cx;
         cy = p2.cy;
     }
    CfSize operator = (Point3D_t & p);

	__forceinline CfSize operator * (int iValue) { return { cx * (double) iValue,cy * (double) iValue }; }
	__forceinline CfSize operator / (int iValue) { return { cx / (double) iValue,cy / (double) iValue }; }
	__forceinline CfSize operator + (int iValue) { return { cx + (double) iValue,cy + (double) iValue }; }
	__forceinline CfSize operator - (int iValue) { return { cx - (double) iValue,cy - (double) iValue }; }

	__forceinline CfSize & operator *= (int iValue) { cx *= (double) iValue; cy *= (double) iValue; return *this; }
	__forceinline CfSize & operator /= (int iValue) { cx /= (double) iValue; cy /= (double) iValue; return *this; }
	__forceinline CfSize & operator += (int iValue) { cx += (double) iValue; cy += (double) iValue; return *this; }
	__forceinline CfSize & operator -= (int iValue) { cx -= (double) iValue; cy -= (double) iValue; return *this; }

	__forceinline CfSize operator * (double fValue) { return { cx * fValue,cy * fValue }; }
	__forceinline CfSize operator / (double fValue) { return { cx / fValue,cy / fValue }; }
	__forceinline CfSize operator + (double fValue) { return { cx + fValue,cy + fValue }; }
	__forceinline CfSize operator - (double fValue) { return { cx - fValue,cy - fValue }; }

	__forceinline CfSize & operator *= (double fValue) { cx *= fValue; cy *= fValue; return *this; }
	__forceinline CfSize & operator /= (double fValue) { cx /= fValue; cy /= fValue; return *this; }
	__forceinline CfSize & operator += (double fValue) { cx += fValue; cy += fValue; return *this; }
	__forceinline CfSize & operator -= (double fValue) { cx -= fValue; cy -= fValue; return *this; }

    CfSize(const Point3D_t & p);
	CfSize() { };
	CfSize(double fx,double fy) { cx = fx; cy = fy; };
	CfSize(const CSize & p)
	{
		cx = (double) p.cx;
		cy = (double) p.cy;
	}
	CfSize(const SIZE & p)
	{
		cx = (double) p.cx;
		cy = (double) p.cy;
	}
    CfSize & operator =(CfSize && p2) noexcept
    {
        cx = p2.cx;
        cy = p2.cy;
        return * this;
    }
	
	SIZE operator * () { return { (int) cx,(int) cy }; }
	operator SIZE() const { SIZE p = { (int) cx,(int) cy }; return p; };
};


}; // namespace Sage

#pragma warning(pop)

#endif // _CSageSize_H_

