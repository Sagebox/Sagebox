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
#include <cmath>

// If SDL Support is enabled, include SDL.H
// If SDL has been included elsewhere, this is not needed.
//
// --> This assumes the SDL path is already set in the project. 
// 
#if defined(SageSDLSupport)
#include <SDL.h>
#endif

#if !defined(_CSagePoint_H_)
#define _CSagePoint_H_

// CPoint.cpp -- work in progress

#pragma warning(push)
#pragma warning(disable : 26495)
#include "SimdClass.h"
namespace Sage
{

struct Point3D_t;
struct Point3Df_t;
struct CfPoint;
struct CfPointf;
struct CSize;

// CSize struct -- right now, this structure exists for the sole purpose of converting to SIZE 
// more easily, so that CSize(x,y) can be entered on a parameter list, for instance.
//
// This will grow over time to support CPOINT conversions as well as being added to CPOINT.
//
#if 0
// deprecated

struct CSize
{
public:
    int cx;
    int cy;
    CSize(int cx,int cy) { this->cx = cx; this->cy = cy; }
	operator SIZE() const { SIZE p = { cx,cy }; return p; };
};
#endif

struct CPoint
{
	int x;
	int y;

	__forceinline CPoint & operator *= (CPoint & p2)
	{
		x *= p2.x;
		y *= p2.y;

		return { *this };
	}

	__forceinline CPoint operator * (int iValue) const { return { x * iValue,y * iValue }; }
	__forceinline CPoint operator / (int iValue) const { return { x / iValue,y / iValue }; }
	__forceinline CPoint operator + (int iValue) const { return { x + iValue,y + iValue }; }
	__forceinline CPoint operator - (int iValue) const { return { x - iValue,y - iValue }; }

	__forceinline CPoint & operator *= (int iValue) { x *= iValue; y *= iValue; return *this; }
	__forceinline CPoint & operator /= (int iValue) { x /= iValue; y /= iValue; return *this; }
	__forceinline CPoint & operator += (int iValue) { x += iValue; y += iValue; return *this; }
	__forceinline CPoint & operator -= (int iValue) { x -= iValue; y -= iValue; return *this; }

    // All non-POINT types are deprecated to avoid collisions and make programming more streamlined.
    // All relavent types (CSize, CPoint, CfPoint, etc.) resolve to POINT. 
    // The only outsider is SIZE, which would be nice to support, but it's probably better to enforce POINT vs SIZE types, especially since SIZE is size_t, and 
    // POINT types are currently integer (e.g. integer vs. long), making an error more appropriate here viz. loss of precision with no announced warning.

//	__forceinline CPoint operator * (const CPoint & p2) { return { x * p2.x, y * p2.y }; }
//	__forceinline CPoint operator / (const CPoint & p2) { return { x / p2.x, y / p2.y }; }
//	__forceinline CPoint operator + (const CPoint & p2) { return { x + p2.x, y + p2.y }; }
//	__forceinline CPoint operator - (const CPoint & p2) { return { x - p2.x, y - p2.y }; }
//
//	__forceinline CPoint & operator *= (const CPoint & p2) { x *= p2.x; y *= p2.y; return *this; }
//	__forceinline CPoint & operator /= (const CPoint & p2) { x /= p2.x; y /= p2.y; return *this; }
//	__forceinline CPoint & operator += (const CPoint & p2) { x += p2.x; y += p2.y; return *this; }
//	__forceinline CPoint & operator -= (const CPoint & p2) { x -= p2.x; y -= p2.y; return *this; }

	__forceinline CPoint operator * (const POINT & p2) const { return { x * p2.x, y * p2.y }; }
	__forceinline CPoint operator / (const POINT & p2) const { return { x / p2.x, y / p2.y }; }
	__forceinline CPoint operator + (const POINT & p2) const { return { x + p2.x, y + p2.y }; }
	__forceinline CPoint operator - (const POINT & p2) const { return { x - p2.x, y - p2.y }; }

	__forceinline CPoint & operator *= (const POINT & p2) { x *= p2.x; y *= p2.y; return *this; }
	__forceinline CPoint & operator /= (const POINT & p2) { x /= p2.x; y /= p2.y; return *this; }
	__forceinline CPoint & operator += (const POINT & p2) { x += p2.x; y += p2.y; return *this; }
	__forceinline CPoint & operator -= (const POINT & p2) { x -= p2.x; y -= p2.y; return *this; }

//	__forceinline CPoint operator * (const SIZE & p2) { return { x * p2.cx, y * p2.cy }; }
//	__forceinline CPoint operator / (const SIZE & p2) { return { x / p2.cx, y / p2.cy }; }
//	__forceinline CPoint operator + (const SIZE & p2) { return { x + p2.cx, y + p2.cy }; }
//	__forceinline CPoint operator - (const SIZE & p2) { return { x - p2.cx, y - p2.cy }; }
//
//	__forceinline CPoint & operator *= (const SIZE & p2) { x *= p2.cx; y *= p2.cy; return *this; }
//	__forceinline CPoint & operator /= (const SIZE & p2) { x /= p2.cx; y /= p2.cy; return *this; }
//	__forceinline CPoint & operator += (const SIZE & p2) { x += p2.cx; y += p2.cy; return *this; }
//	__forceinline CPoint & operator -= (const SIZE & p2) { x -= p2.cx; y -= p2.cy; return *this; }

    
    
     __forceinline CPoint operator = (Point3D_t & p);
     __forceinline CPoint operator = (Point3Df_t & p);

	__forceinline POINT operator * () { return { x,y }; }
	__forceinline CPoint(int iX,int iY) { x = iX;    y = iY;    }
	__forceinline CPoint(const POINT & p) { x = p.x;   y = p.y;   }
    __forceinline CPoint(const SIZE & sz) { x = sz.cx; y = sz.cy; }

	__forceinline operator POINT() const { POINT p = { x,y }; return p; };
	__forceinline operator SIZE() const { SIZE p = { x,y }; return p; };

#ifdef SupportGDI
    __forceinline operator Gdiplus::Point() const {  return { (int) x, (int) y }; }

#endif
    __forceinline CPoint & operator = (const CPoint & p2)
    {
        x = p2.x;
        y = p2.y;
        return *this;
    }
 
    __forceinline CPoint(const CPoint & p2)
    {
        x = p2.x;
        y = p2.y;
    }
    __forceinline CPoint & operator = (CPoint && p2) noexcept
    {
        x = p2.x;
        y = p2.y;
        return * this;
    }
    __forceinline CPoint(CPoint && p2) noexcept
    {
        x = p2.x;
        y = p2.y;
    }
    CPoint(const CSize & p);
    CPoint(const Point3D_t & p);
    CPoint(const Point3Df_t & p);
    CPoint(const CfPointf & p);
    CPoint(const CfPoint & p);
    
    CPoint RotateXY(double fAngle) const { return { (decltype(x)) ((double) x *cos(fAngle) - (double) y*sin(fAngle)), (decltype(y)) ((double) x *sin(fAngle) + (double) y*cos(fAngle)) }; };  
    CPoint & SelfRotateXY(double fAngle) { auto fTemp = (double) x *cos(fAngle) - (double) y*sin(fAngle); y =  (decltype(y)) (x*sin(fAngle) + (double) y*cos(fAngle)); x = (decltype(x)) fTemp; return *this; }

    double Mag() const { return sqrt((double) x*(double) x+(double) y*(double) y); }
    double MagSq() const { return ((double) x*(double) x+(double) y*(double) y); }

    CPoint & SelfAbs()  { x = abs(x); y = abs(y); return *this; }

    __forceinline bool operator == (const CPoint & p2) { x == p2.x && y == p2.y; }
    __forceinline bool operator != (const CPoint & p2) { x != p2.x || y != p2.y; }


	CPoint() = default;
    CPoint operator-()  { return { -x, -y }; }

    __forceinline CPoint _min(CPoint & c) { return {min(x,c.x),min(y,c.y)}; }
    __forceinline CPoint _max(CPoint & c) { return {max(x,c.x),max(y,c.y)}; }
    __forceinline CPoint _min(POINT & c) { return {min(x,c.x),min(y,c.y)}; }
    __forceinline CPoint _max(POINT & c) { return {max(x,c.x),max(y,c.y)}; }
    __forceinline CPoint _min(SIZE & c) { return {min(x,c.cx),min(y,c.cy)}; }
    __forceinline CPoint _max(SIZE & c) { return {max(x,c.cx),max(y,c.cy)}; }

    __forceinline bool WithinRect(CPoint tl, CPoint br) {  return (x >= tl.x && y >= tl.y && x < br.x && y < br.y); }

};

struct CfPointf;
struct CfPoint
{
	double x;
	double y;

	__forceinline CfPoint & operator = (const CPoint & p)
	{
		x = (double) p.x;
		y = (double) p.y;
		return (*this);
	}
	__forceinline CfPoint & operator = (const CfPoint & p)
	{
		x = (double) p.x;
		y = (double) p.y;
		return (*this);
	}
	__forceinline CfPoint operator + (const CfPoint & p2) const
	{
		return { x + p2.x, y+p2.y };
	}
	__forceinline CfPoint operator / (const CfPoint & p2)  const
	{
		return { x / p2.x, y/p2.y };
	}
	__forceinline CfPoint & operator /= (const CfPoint & p2)
	{
		x /= p2.x;
		y /= p2.y;
		return *this;
	}

	__forceinline CfPoint operator - (const CfPoint & p2) const
	{
		return { x - p2.x, y-p2.y };
	}
	__forceinline CfPoint operator - (const CPoint & p2) const
	{
		return { x - (double) p2.x, y- (double) p2.y };
	}
	__forceinline CfPoint & operator += (const CfPoint & p2)
	{
		x += p2.x;
		y += p2.y;
		return *this;
	}
	__forceinline CfPoint & operator *= (const CfPoint & p2)
	{
		x *= p2.x;
		y *= p2.y;
		return *this;
	}
	__forceinline CfPoint operator * (const CfPoint & p2)  const
	{
		return { x * p2.x, y * p2.y };
	}
    __forceinline CfPoint & operator -= (const CfPoint & p2)
	{
		x -= p2.x;
		y -= p2.y;
		return *this;
	}
    __forceinline CfPoint(const CfPoint & p2)
    {
        x = p2.x;
        y = p2.y;
    }
    
    __forceinline CfPoint(CfPoint && p2) noexcept
    {
        x = p2.x;
        y = p2.y;
    }
    __forceinline CfPoint(CPoint & p2) noexcept
    {
        x = p2.x;
        y = p2.y;
    }
    __forceinline CfPoint(POINT & p2) noexcept
    {
         x = p2.x;
         y = p2.y;
     }
    CfPoint operator = (Point3D_t& p);
    CfPoint operator = (Point3Df_t& p);

	__forceinline CfPoint operator * (int iValue) { return { x * (double) iValue,y * (double) iValue }; }
	__forceinline CfPoint operator / (int iValue) { return { x / (double) iValue,y / (double) iValue }; }
	__forceinline CfPoint operator + (int iValue) { return { x + (double) iValue,y + (double) iValue }; }
	__forceinline CfPoint operator - (int iValue) { return { x - (double) iValue,y - (double) iValue }; }

	__forceinline CfPoint & operator *= (int iValue) { x *= (double) iValue; y *= (double) iValue; return *this; }
	__forceinline CfPoint & operator /= (int iValue) { x /= (double) iValue; y /= (double) iValue; return *this; }
	__forceinline CfPoint & operator += (int iValue) { x += (double) iValue; y += (double) iValue; return *this; }
	__forceinline CfPoint & operator -= (int iValue) { x -= (double) iValue; y -= (double) iValue; return *this; }

	__forceinline CfPoint operator * (double fValue) { return { x * fValue,y * fValue }; }
	__forceinline CfPoint operator / (double fValue) { return { x / fValue,y / fValue }; }
	__forceinline CfPoint operator + (double fValue) { return { x + fValue,y + fValue }; }
	__forceinline CfPoint operator - (double fValue) { return { x - fValue,y - fValue }; }

	__forceinline CfPoint & operator *= (double fValue) { x *= fValue; y *= fValue; return *this; }
	__forceinline CfPoint & operator /= (double fValue) { x /= fValue; y /= fValue; return *this; }
	__forceinline CfPoint & operator += (double fValue) { x += fValue; y += fValue; return *this; }
	__forceinline CfPoint & operator -= (double fValue) { x -= fValue; y -= fValue; return *this; }
    

    CfPoint RotateXY(double fAngle) const { return { x *cos(fAngle) - y*sin(fAngle), x *sin(fAngle) + y*cos(fAngle) }; };  
    CfPoint & SelfRotateXY(double fAngle) { auto fTemp = x *cos(fAngle) - y*sin(fAngle); y =  x* sin(fAngle) + y*cos(fAngle); x = fTemp; return *this; }

    double Mag() const { return sqrt(x*x+y*y); }
    double MagSq() const { return (x*x+y*y); }

    CfPoint & SelfAbs()  { x = fabs(x); y = fabs(y); return *this; }

    CfPoint(const Point3D_t & p);
    CfPoint(const Point3Df_t & p);
    CfPoint(const CfPointf & p);
	CfPoint() = default;
	CfPoint(double fx,double fy)
    { 
        x = fx; y = fy; 
    };
	__forceinline CfPoint(const CPoint & p)
	{
		x = (double) p.x;
		y = (double) p.y;
	}
	__forceinline CfPoint(const POINT & p)
	{
		x = (double) p.x;
		y = (double) p.y;
	}
    __forceinline CfPoint & operator =(CfPoint && p2) noexcept
    {
        x = p2.x;
        y = p2.y;
        return * this;
    }
	__forceinline CfPoint(const SIZE & szSize)
	{
		x = (double) szSize.cx;
		y = (double) szSize.cy;
	}

    __forceinline bool operator == (const CfPoint & p2) { x == p2.x && y == p2.y; }
    __forceinline bool operator != (const CfPoint & p2) { x != p2.x || y != p2.y; }

	__forceinline POINT operator * () { return { (int) x,(int) y }; }
	__forceinline operator POINT() const { POINT p = { (int) x,(int) y }; return p; };

    __forceinline bool WithinRect(CfPoint tl, CfPoint br) {  return (x >= tl.x && y >= tl.y && x < br.x && y < br.y); }
    operator Point3D_t() const;
    operator Point3Df_t() const;
    CfPoint operator-()  { return { -x, -y }; }

   // operator CfPointf() const;
};


struct CfPointf
{
    // $$ Do not change the order of these values, their type, or add any other values here unless:
    //      1. Change C#/VB DrawPolygon() functions to not cast to CfPointf(), which is currently being used in an unsafe fashion to 
    //         handle an array of float(x,y) values as DrawPolygon() functons have no support for a raw float(x,y) structure, e.g. POINTF
	float x;
	float y;

	__forceinline CfPointf & operator = (const CPoint & p)
	{
		x = (float) p.x;
		y = (float) p.y;
		return (*this);
	}
	__forceinline CfPointf & operator = (const CfPointf & p)
	{
		x = (float) p.x;
		y = (float) p.y;
		return (*this);
	}
	__forceinline CfPointf & operator = (const CfPoint & p)
	{
		x = (float) p.x;
		y = (float) p.y;
		return (*this);
	}
	__forceinline CfPointf operator + (const CfPointf & p2) const
	{
		return { x + p2.x, y+p2.y };
	}
	__forceinline CfPointf operator / (const CfPointf & p2) const
	{
		return { x / p2.x, y/p2.y };
	}
	__forceinline CfPointf & operator /= (const CfPointf & p2)
	{
		x /= p2.x;
		y /= p2.y;
		return *this;
	}

	__forceinline CfPointf operator - (const CfPointf & p2) const
	{
		return { x - p2.x, y-p2.y };
	}
	__forceinline CfPointf operator - (const CPoint & p2) const
	{
		return { x - (double) p2.x, y- (double) p2.y };
	}
	__forceinline CfPointf & operator += (const CfPointf & p2) 
	{
		x += p2.x;
		y += p2.y;
		return *this;
	}
	__forceinline CfPointf & operator *= (const CfPointf & p2)
	{
		x *= p2.x;
		y *= p2.y;
		return *this;
	}
	__forceinline CfPointf operator * (const CfPointf & p2) const
	{
		return { x * p2.x, y * p2.y };
	}
    __forceinline CfPointf & operator -= (const CfPointf & p2)
	{
		x -= p2.x;
		y -= p2.y;
		return *this;
	}
    __forceinline CfPointf(const CfPoint & p2)
    {
        x = (decltype(x)) p2.x;
        y = (decltype(y)) p2.y;
    }
    __forceinline CfPointf(const CfPointf & p2)
    {
        x = (decltype(x)) p2.x;
        y = (decltype(y)) p2.y;
    }  
    __forceinline CfPointf(CfPointf && p2) noexcept
    {
        x = p2.x;
        y = p2.y;
    }
    __forceinline CfPointf(CPoint & p2) noexcept
    {
        x = (decltype(x)) p2.x;
        y = (decltype(y)) p2.y;
    }
    __forceinline CfPointf(POINT & p2) noexcept
    {
         x = (decltype(x)) p2.x;
         y = (decltype(y)) p2.y;
     }
    __forceinline CfPointf operator = (Point3D_t & p);
    __forceinline CfPointf operator = (Point3Df_t & p);

	__forceinline CfPointf operator * (int iValue) const { return { x * (float) iValue,y * (float) iValue }; }
	__forceinline CfPointf operator / (int iValue) const { return { x / (float) iValue,y / (float) iValue }; }
	__forceinline CfPointf operator + (int iValue) const { return { x + (float) iValue,y + (float) iValue }; }
	__forceinline CfPointf operator - (int iValue) const { return { x - (float) iValue,y - (float) iValue }; }

	__forceinline CfPointf & operator *= (int iValue) { x *= (float) iValue; y *= (float) iValue; return *this; }
	__forceinline CfPointf & operator /= (int iValue) { x /= (float) iValue; y /= (float) iValue; return *this; }
	__forceinline CfPointf & operator += (int iValue) { x += (float) iValue; y += (float) iValue; return *this; }
	__forceinline CfPointf & operator -= (int iValue) { x -= (float) iValue; y -= (float) iValue; return *this; }

	__forceinline CfPointf operator * (float fValue) const { return { x * fValue,y * fValue }; }
	__forceinline CfPointf operator / (float fValue) const { return { x / fValue,y / fValue }; }
	__forceinline CfPointf operator + (float fValue) const { return { x + fValue,y + fValue }; }
	__forceinline CfPointf operator - (float fValue) const { return { x - fValue,y - fValue }; }

	__forceinline CfPointf operator * (double fValue) const { return { x * (float) fValue,y * (float) fValue }; }
	__forceinline CfPointf operator / (double fValue) const { return { x / (float) fValue,y / (float) fValue }; }
	__forceinline CfPointf operator + (double fValue) const { return { x + (float) fValue,y + (float) fValue }; }
	__forceinline CfPointf operator - (double fValue) const { return { x - (float) fValue,y - (float) fValue }; }

	__forceinline CfPointf & operator *= (float fValue) { x *= fValue; y *= fValue; return *this; }
	__forceinline CfPointf & operator /= (float fValue) { x /= fValue; y /= fValue; return *this; }
	__forceinline CfPointf & operator += (float fValue) { x += fValue; y += fValue; return *this; }
	__forceinline CfPointf & operator -= (float fValue) { x -= fValue; y -= fValue; return *this; }


    CfPointf(const Point3D_t & p);
    CfPointf(const Point3Df_t & p);
	CfPointf() = default;
	CfPointf(double fx,double fy) { x = (decltype(x)) fx;y = (decltype(x)) fy; };
    CfPointf operator-()  { return { -x, -y }; }


    CfPointf RotateXY(float fAngle) const { return { x *cos(fAngle) - y*sin(fAngle), x *sin(fAngle) + y*cos(fAngle) }; };  
    CfPointf & SelfRotateXY(float fAngle) { auto fTemp = x *cos(fAngle) - y*sin(fAngle); y =  x*sin(fAngle) + y*cos(fAngle); x = fTemp; return *this; }

    float Mag() const { return sqrt(x*x+y*y); }
    float MagSq() const { return (x*x+y*y); }

    CfPointf & SelfAbs()  { x = fabs(x); y = fabs(y); return *this; }

	__forceinline CfPointf(const CPoint & p) 
	{
		x = (decltype(x)) p.x;
		y = (decltype(y)) p.y;
	}
	__forceinline CfPointf(const POINT & p)
	{
		x = (decltype(x)) p.x;
		y = (decltype(y)) p.y;
	}
    __forceinline CfPointf & operator =(CfPointf && p2) noexcept
    {
        x = p2.x;
        y = p2.y;
        return * this;
    }
	__forceinline CfPointf(const SIZE & szSize)
	{
		x = (decltype(x)) szSize.cx;
		y = (decltype(y)) szSize.cy;
	}

    __forceinline bool operator == (const CfPointf & p2) { x == p2.x && y == p2.y; }
    __forceinline bool operator != (const CfPointf & p2) { x != p2.x || y != p2.y; }

	__forceinline POINT operator * () { return { (int) x,(int) y }; }
	__forceinline operator POINT() const { POINT p = { (int) x,(int) y }; return p; };
    __forceinline bool WithinRect(CfPointf tl, CfPointf br) {  return (x >= tl.x && y >= tl.y && x < br.x && y < br.y); }
    operator Point3D_t() const;
    operator Point3Df_t() const;

};

//#ifdef SAGEBOX_AVX

struct CfPointSimd4 : public Simd128
{
public:
	__m128 x;
	__m128 y;
private:
    using nType = decltype(x.m128_f32[0]);                      // i.e. native type; float, double, etc. 
    __forceinline __m128 nVec(nType f) { return Vecf(f); };     // i.e. native vector
public:
    __forceinline CfPointSimd4(const __m128 & _x,const __m128 & _y)         { x = _x; y = _y; }
    __forceinline CfPointSimd4(CfPointSimd4 && p2) noexcept                 { x = p2.x; y = p2.y; }
    __forceinline CfPointSimd4(CfPointSimd4 & p2) noexcept                  { x = p2.x; y = p2.y; }
    __forceinline CfPointSimd4 & operator =(CfPointSimd4 && p2) noexcept    { x = p2.x, y = p2.y; return * this; }

	__forceinline CfPointSimd4 operator +  (const CfPointSimd4 & p2) const { return {Add(x,p2.x), Add(x,p2.y) }; }
	__forceinline CfPointSimd4 operator -  (const CfPointSimd4 & p2) const { return {Sub(x,p2.x), Sub(x,p2.y) }; }
	__forceinline CfPointSimd4 operator *  (const CfPointSimd4 & p2) const { return {Mul(x,p2.x), Mul(x,p2.y) }; }
	__forceinline CfPointSimd4 operator /  (const CfPointSimd4 & p2) const { return {Div(x,p2.x), Div(x,p2.y) }; }
    __forceinline CfPointSimd4 operator += (const CfPointSimd4 & p2)       {  x = Add(x,p2.x); y = Add(x,p2.y); return *this; }
    __forceinline CfPointSimd4 operator -= (const CfPointSimd4 & p2)       {  x = Sub(x,p2.x); y = Sub(x,p2.y); return *this; }
    __forceinline CfPointSimd4 operator *= (const CfPointSimd4 & p2)       {  x = Mul(x,p2.x); y = Mul(x,p2.y); return *this; }
    __forceinline CfPointSimd4 operator /= (const CfPointSimd4 & p2)       {  x = Div(x,p2.x); y = Div(x,p2.y); return *this; }

	__forceinline CfPointSimd4 operator + (int iValue) { auto vTemp = nVec((nType) iValue); return { Add(x ,vTemp), Add(y, vTemp) }; }
	__forceinline CfPointSimd4 operator - (int iValue) { auto vTemp = nVec((nType) iValue); return { Sub(x ,vTemp), Sub(y, vTemp) }; }
	__forceinline CfPointSimd4 operator * (int iValue) { auto vTemp = nVec((nType) iValue); return { Mul(x ,vTemp), Mul(y, vTemp) }; }
	__forceinline CfPointSimd4 operator / (int iValue) { auto vTemp = nVec((nType) iValue); return { Div(x ,vTemp), Div(y, vTemp) }; }

	__forceinline CfPointSimd4 & operator += (int iValue) { auto vTemp = nVec((nType) iValue); x = Add(x ,vTemp); y = Add(y, vTemp); return * this; }
	__forceinline CfPointSimd4 & operator -= (int iValue) { auto vTemp = nVec((nType) iValue); x = Sub(x ,vTemp); y = Sub(y, vTemp); return * this; }
	__forceinline CfPointSimd4 & operator *= (int iValue) { auto vTemp = nVec((nType) iValue); x = Mul(x ,vTemp); y = Mul(y, vTemp); return * this; }
	__forceinline CfPointSimd4 & operator /= (int iValue) { auto vTemp = nVec((nType) iValue); x = Div(x ,vTemp); y = Div(y, vTemp); return * this; }

	__forceinline CfPointSimd4 operator + (nType fValue) { auto vTemp = nVec(fValue); return { Add(x ,vTemp), Add(y, vTemp) }; }
	__forceinline CfPointSimd4 operator - (nType fValue) { auto vTemp = nVec(fValue); return { Sub(x ,vTemp), Sub(y, vTemp) }; }
	__forceinline CfPointSimd4 operator * (nType fValue) { auto vTemp = nVec(fValue); return { Mul(x ,vTemp), Mul(y, vTemp) }; }
	__forceinline CfPointSimd4 operator / (nType fValue) { auto vTemp = nVec(fValue); return { Div(x ,vTemp), Div(y, vTemp) }; }

	__forceinline CfPointSimd4 & operator += (nType fValue) { auto vTemp = nVec(fValue); x = Add(x ,vTemp); y = Add(y, vTemp); return * this; }
	__forceinline CfPointSimd4 & operator -= (nType fValue) { auto vTemp = nVec(fValue); x = Sub(x ,vTemp); y = Sub(y, vTemp); return * this; }
	__forceinline CfPointSimd4 & operator *= (nType fValue) { auto vTemp = nVec(fValue); x = Mul(x ,vTemp); y = Mul(y, vTemp); return * this; }
	__forceinline CfPointSimd4 & operator /= (nType fValue) { auto vTemp = nVec(fValue); x = Div(x ,vTemp); y = Div(y, vTemp); return * this; }



 
	CfPointSimd4() { };
};


struct CfPointSimd8 : public Simd256
{
public:
	__m256 x;
	__m256 y;
private:
    using nType = decltype(x.m256_f32[0]);                      // i.e. native type; float, double, etc. 
    __forceinline __m256 nVec(nType f) const { return Vecf(f); };     // i.e. native vector
public:
    __forceinline CfPointSimd8(const __m256 & _x,const __m256 & _y)         { x = _x; y = _y; }
    __forceinline CfPointSimd8(CfPointSimd8 && p2) noexcept                 { x = p2.x; y = p2.y; }
    __forceinline CfPointSimd8(CfPointSimd8 & p2) noexcept                  { x = p2.x; y = p2.y; }
    __forceinline CfPointSimd8 & operator =(CfPointSimd8 && p2) noexcept    { x = p2.x, y = p2.y; return * this; }

	__forceinline CfPointSimd8 operator +  (const CfPointSimd8 & p2) const { return {Add(x,p2.x), Add(x,p2.y) }; }
	__forceinline CfPointSimd8 operator -  (const CfPointSimd8 & p2) const { return {Sub(x,p2.x), Sub(x,p2.y) }; }
	__forceinline CfPointSimd8 operator *  (const CfPointSimd8 & p2) const { return {Mul(x,p2.x), Mul(x,p2.y) }; }
	__forceinline CfPointSimd8 operator /  (const CfPointSimd8 & p2) const { return {Div(x,p2.x), Div(x,p2.y) }; }
 
    __forceinline CfPointSimd8 operator += (const CfPointSimd8 & p2)       {  x = Add(x,p2.x); y = Add(x,p2.y); return *this; }
    __forceinline CfPointSimd8 operator -= (const CfPointSimd8 & p2)       {  x = Sub(x,p2.x); y = Sub(x,p2.y); return *this; }
    __forceinline CfPointSimd8 operator *= (const CfPointSimd8 & p2)       {  x = Mul(x,p2.x); y = Mul(x,p2.y); return *this; }
    __forceinline CfPointSimd8 operator /= (const CfPointSimd8 & p2)       {  x = Div(x,p2.x); y = Div(x,p2.y); return *this; }

	__forceinline CfPointSimd8 operator + (int iValue) const { auto vTemp = nVec((nType) iValue); return { Add(x ,vTemp), Add(y, vTemp) }; }
	__forceinline CfPointSimd8 operator - (int iValue) const { auto vTemp = nVec((nType) iValue); return { Sub(x ,vTemp), Sub(y, vTemp) }; }
	__forceinline CfPointSimd8 operator * (int iValue) const { auto vTemp = nVec((nType) iValue); return { Mul(x ,vTemp), Mul(y, vTemp) }; }
	__forceinline CfPointSimd8 operator / (int iValue) const { auto vTemp = nVec((nType) iValue); return { Div(x ,vTemp), Div(y, vTemp) }; }

	__forceinline CfPointSimd8 & operator += (int iValue) { auto vTemp = nVec((nType) iValue); x = Add(x ,vTemp); y = Add(y, vTemp); return * this; }
	__forceinline CfPointSimd8 & operator -= (int iValue) { auto vTemp = nVec((nType) iValue); x = Sub(x ,vTemp); y = Sub(y, vTemp); return * this; }
	__forceinline CfPointSimd8 & operator *= (int iValue) { auto vTemp = nVec((nType) iValue); x = Mul(x ,vTemp); y = Mul(y, vTemp); return * this; }
	__forceinline CfPointSimd8 & operator /= (int iValue) { auto vTemp = nVec((nType) iValue); x = Div(x ,vTemp); y = Div(y, vTemp); return * this; }

	__forceinline CfPointSimd8 operator + (nType fValue) const { auto vTemp = nVec(fValue); return { Add(x ,vTemp), Add(y, vTemp) }; }
	__forceinline CfPointSimd8 operator - (nType fValue) const { auto vTemp = nVec(fValue); return { Sub(x ,vTemp), Sub(y, vTemp) }; }
	__forceinline CfPointSimd8 operator * (nType fValue) const { auto vTemp = nVec(fValue); return { Mul(x ,vTemp), Mul(y, vTemp) }; }
	__forceinline CfPointSimd8 operator / (nType fValue) const { auto vTemp = nVec(fValue); return { Div(x ,vTemp), Div(y, vTemp) }; }

	__forceinline CfPointSimd8 & operator += (nType fValue) { auto vTemp = nVec(fValue); x = Add(x ,vTemp); y = Add(y, vTemp); return * this; }
	__forceinline CfPointSimd8 & operator -= (nType fValue) { auto vTemp = nVec(fValue); x = Sub(x ,vTemp); y = Sub(y, vTemp); return * this; }
	__forceinline CfPointSimd8 & operator *= (nType fValue) { auto vTemp = nVec(fValue); x = Mul(x ,vTemp); y = Mul(y, vTemp); return * this; }
	__forceinline CfPointSimd8 & operator /= (nType fValue) { auto vTemp = nVec(fValue); x = Div(x ,vTemp); y = Div(y, vTemp); return * this; }

	__forceinline CfPointSimd8 operator + (__m256 mValue) const { return { Add(x ,mValue), Add(y, mValue) }; }
	__forceinline CfPointSimd8 operator - (__m256 mValue) const { return { Sub(x ,mValue), Sub(y, mValue) }; }
	__forceinline CfPointSimd8 operator * (__m256 mValue) const { return { Mul(x ,mValue), Mul(y, mValue) }; }
	__forceinline CfPointSimd8 operator / (__m256 mValue) const { return { Div(x ,mValue), Div(y, mValue) }; }

	__forceinline CfPointSimd8 & operator += (__m256 mValue) { x = Add(x ,mValue); y = Add(y, mValue); return * this; }
	__forceinline CfPointSimd8 & operator -= (__m256 mValue) { x = Sub(x ,mValue); y = Sub(y, mValue); return * this; }
	__forceinline CfPointSimd8 & operator *= (__m256 mValue) { x = Mul(x ,mValue); y = Mul(y, mValue); return * this; }
	__forceinline CfPointSimd8 & operator /= (__m256 mValue) { x = Div(x ,mValue); y = Div(y, mValue); return * this; }



 
	CfPointSimd8() { };
};


struct CfPointSimd16: public Simd512
{
public:
	__m512 x;
	__m512 y;
private:
    using nType = decltype(x.m512_f32[0]);                      // i.e. native type; float, double, etc. 
    __forceinline __m512 nVec(nType f) const { return Vecf(f); };     // i.e. native vector
public:
    __forceinline CfPointSimd16(const __m512 & _x,const __m512 & _y)         { x = _x; y = _y; }
    __forceinline CfPointSimd16(CfPointSimd16 && p2) noexcept                 { x = p2.x; y = p2.y; }
    __forceinline CfPointSimd16(CfPointSimd16 & p2) noexcept                  { x = p2.x; y = p2.y; }
    __forceinline CfPointSimd16 & operator =(CfPointSimd16 && p2) noexcept    { x = p2.x, y = p2.y; return * this; }

	__forceinline CfPointSimd16 operator +  (const CfPointSimd16 & p2) const { return {Add(x,p2.x), Add(x,p2.y) }; }
	__forceinline CfPointSimd16 operator -  (const CfPointSimd16 & p2) const { return {Sub(x,p2.x), Sub(x,p2.y) }; }
	__forceinline CfPointSimd16 operator *  (const CfPointSimd16 & p2) const { return {Mul(x,p2.x), Mul(x,p2.y) }; }
	__forceinline CfPointSimd16 operator /  (const CfPointSimd16 & p2) const { return {Div(x,p2.x), Div(x,p2.y) }; }
 
    __forceinline CfPointSimd16 operator += (const CfPointSimd16 & p2)       {  x = Add(x,p2.x); y = Add(x,p2.y); return *this; }
    __forceinline CfPointSimd16 operator -= (const CfPointSimd16 & p2)       {  x = Sub(x,p2.x); y = Sub(x,p2.y); return *this; }
    __forceinline CfPointSimd16 operator *= (const CfPointSimd16 & p2)       {  x = Mul(x,p2.x); y = Mul(x,p2.y); return *this; }
    __forceinline CfPointSimd16 operator /= (const CfPointSimd16 & p2)       {  x = Div(x,p2.x); y = Div(x,p2.y); return *this; }

	__forceinline CfPointSimd16 operator + (int iValue) const { auto vTemp = nVec((nType) iValue); return { Add(x ,vTemp), Add(y, vTemp) }; }
	__forceinline CfPointSimd16 operator - (int iValue) const { auto vTemp = nVec((nType) iValue); return { Sub(x ,vTemp), Sub(y, vTemp) }; }
	__forceinline CfPointSimd16 operator * (int iValue) const { auto vTemp = nVec((nType) iValue); return { Mul(x ,vTemp), Mul(y, vTemp) }; }
	__forceinline CfPointSimd16 operator / (int iValue) const { auto vTemp = nVec((nType) iValue); return { Div(x ,vTemp), Div(y, vTemp) }; }

	__forceinline CfPointSimd16 & operator += (int iValue) { auto vTemp = nVec((nType) iValue); x = Add(x ,vTemp); y = Add(y, vTemp); return * this; }
	__forceinline CfPointSimd16 & operator -= (int iValue) { auto vTemp = nVec((nType) iValue); x = Sub(x ,vTemp); y = Sub(y, vTemp); return * this; }
	__forceinline CfPointSimd16 & operator *= (int iValue) { auto vTemp = nVec((nType) iValue); x = Mul(x ,vTemp); y = Mul(y, vTemp); return * this; }
	__forceinline CfPointSimd16 & operator /= (int iValue) { auto vTemp = nVec((nType) iValue); x = Div(x ,vTemp); y = Div(y, vTemp); return * this; }

	__forceinline CfPointSimd16 operator + (nType fValue) const { auto vTemp = nVec(fValue); return { Add(x ,vTemp), Add(y, vTemp) }; }
	__forceinline CfPointSimd16 operator - (nType fValue) const { auto vTemp = nVec(fValue); return { Sub(x ,vTemp), Sub(y, vTemp) }; }
	__forceinline CfPointSimd16 operator * (nType fValue) const { auto vTemp = nVec(fValue); return { Mul(x ,vTemp), Mul(y, vTemp) }; }
	__forceinline CfPointSimd16 operator / (nType fValue) const { auto vTemp = nVec(fValue); return { Div(x ,vTemp), Div(y, vTemp) }; }

	__forceinline CfPointSimd16 & operator += (nType fValue) { auto vTemp = nVec(fValue); x = Add(x ,vTemp); y = Add(y, vTemp); return * this; }
	__forceinline CfPointSimd16 & operator -= (nType fValue) { auto vTemp = nVec(fValue); x = Sub(x ,vTemp); y = Sub(y, vTemp); return * this; }
	__forceinline CfPointSimd16 & operator *= (nType fValue) { auto vTemp = nVec(fValue); x = Mul(x ,vTemp); y = Mul(y, vTemp); return * this; }
	__forceinline CfPointSimd16 & operator /= (nType fValue) { auto vTemp = nVec(fValue); x = Div(x ,vTemp); y = Div(y, vTemp); return * this; }

	__forceinline CfPointSimd16 operator + (__m512 mValue) const { return { Add(x ,mValue), Add(y, mValue) }; }
	__forceinline CfPointSimd16 operator - (__m512 mValue) const { return { Sub(x ,mValue), Sub(y, mValue) }; }
	__forceinline CfPointSimd16 operator * (__m512 mValue) const { return { Mul(x ,mValue), Mul(y, mValue) }; }
	__forceinline CfPointSimd16 operator / (__m512 mValue) const { return { Div(x ,mValue), Div(y, mValue) }; }

	__forceinline CfPointSimd16 & operator += (__m512 mValue) { x = Add(x ,mValue); y = Add(y, mValue); return * this; }
	__forceinline CfPointSimd16 & operator -= (__m512 mValue) { x = Sub(x ,mValue); y = Sub(y, mValue); return * this; }
	__forceinline CfPointSimd16 & operator *= (__m512 mValue) { x = Mul(x ,mValue); y = Mul(y, mValue); return * this; }
	__forceinline CfPointSimd16 & operator /= (__m512 mValue) { x = Div(x ,mValue); y = Div(y, mValue); return * this; }



 
	CfPointSimd16() { };
};
//#endif SAGEBOX_AVX

class SizeRect
{
public:
	POINT loc;
	SIZE size;
	__forceinline bool Empty() { return !loc.x && !loc.y && !size.cx && !size.cy; };
	__forceinline bool EmptySize() {return !size.cx && !size.cy; };
	__forceinline bool EmptyPoint() {return !loc.x && !loc.y; };
	__forceinline bool EmptyAny() { return (!size.cx && !size.cy) || ( !loc.x && !loc.y); };
	__forceinline operator POINT() const { POINT p = { loc.x,loc.y }; return p; };
	__forceinline operator CPoint() const { CPoint p = { loc.x,loc.y }; return p; };
	__forceinline operator SIZE() const { SIZE p = { size.cx,size.cy }; return p; };
	__forceinline operator RECT() const { RECT p = { loc.x,loc.y,loc.x+size.cx,loc.y+size.cy }; return p; };
    __forceinline SizeRect operator = (const RECT & p) { return { (int) p.left, (int) p.top, (int) (p.left-p.right), (int) (p.bottom-p.top) }; }
    __forceinline static SizeRect fromRect(const RECT & p) { return { (int) p.left, (int) p.top, (int) (p.left-p.right), (int) (p.bottom-p.top) }; }

#ifdef SupportGDI
    __forceinline operator Gdiplus::Rect() const { return { (int) loc.x, (int) loc.y, (int) size.cx,  (int) size.cy }; }
#endif
#ifdef SDLCALL
    operator SDL_Rect() { SDL_Rect p = { loc.x,loc.y,size.cx,size.cy }; return p; };
#endif
    SizeRect() = default; 
    SizeRect(POINT loc,SIZE size) { this->loc = loc; this->size = size; }
    SizeRect(int x,int y,int width,int height) { loc = POINT{x,y}; size = SIZE{width,height}; }

    __forceinline SizeRect operator + (const SizeRect & sr) const { return SizeRect(loc.x + sr.loc.x,loc.y + sr.loc.y,size.cx + sr.size.cx,size.cy + sr.size.cy); }
    __forceinline SizeRect operator - (const SizeRect & sr) const { return SizeRect(loc.x - sr.loc.x,loc.y - sr.loc.y,size.cx - sr.size.cx,size.cy - sr.size.cy); }
    __forceinline SizeRect operator * (const SizeRect & sr) const { return SizeRect(loc.x * sr.loc.x,loc.y * sr.loc.y,size.cx * sr.size.cx,size.cy * sr.size.cy); }
    __forceinline SizeRect operator / (const SizeRect & sr) const { return SizeRect(loc.x / sr.loc.x,loc.y / sr.loc.y,size.cx / sr.size.cx,size.cy / sr.size.cy); }

    __forceinline SizeRect & operator += (const SizeRect & sr) { loc.x += sr.loc.x; loc.y += sr.loc.y; size.cx += sr.size.cx; size.cy += sr.size.cy; return *this; }
    __forceinline SizeRect & operator -= (const SizeRect & sr) { loc.x -= sr.loc.x; loc.y -= sr.loc.y; size.cx -= sr.size.cx; size.cy -= sr.size.cy; return *this; }
    __forceinline SizeRect & operator *= (const SizeRect & sr) { loc.x *= sr.loc.x; loc.y *= sr.loc.y; size.cx *= sr.size.cx; size.cy *= sr.size.cy; return *this; }
    __forceinline SizeRect & operator /= (const SizeRect & sr) { loc.x /= sr.loc.x; loc.y /= sr.loc.y; size.cx /= sr.size.cx; size.cy /= sr.size.cy; return *this; }

    __forceinline SizeRect operator + (int value) const { return SizeRect(loc.x + value,loc.y + value,size.cx + value,size.cy + value); }
    __forceinline SizeRect operator - (int value) const { return SizeRect(loc.x - value,loc.y - value,size.cx - value,size.cy - value); }
    __forceinline SizeRect operator * (int value) const { return SizeRect(loc.x * value,loc.y * value,size.cx * value,size.cy * value); }
    __forceinline SizeRect operator / (int value) const { return SizeRect(loc.x / value,loc.y / value,size.cx / value,size.cy / value); }

    __forceinline SizeRect & operator += (int value) { loc.x += value; loc.y += value; size.cx += value; size.cy += value; return *this; }
    __forceinline SizeRect & operator -= (int value) { loc.x -= value; loc.y -= value; size.cx -= value; size.cy -= value; return *this; }
    __forceinline SizeRect & operator *= (int value) { loc.x *= value; loc.y *= value; size.cx *= value; size.cy *= value; return *this; }
    __forceinline SizeRect & operator /= (int value) { loc.x /= value; loc.y /= value; size.cx /= value; size.cy /= value; return *this; }

    __forceinline SizeRect operator + (const POINT & sr) const { return SizeRect(loc.x + sr.x,loc.y + sr.y,size.cx,size.cy); }
    __forceinline SizeRect operator - (const POINT & sr) const { return SizeRect(loc.x - sr.x,loc.y - sr.y,size.cx,size.cy); }
    __forceinline SizeRect operator * (const POINT & sr) const { return SizeRect(loc.x * sr.x,loc.y * sr.y,size.cx,size.cy); }
    __forceinline SizeRect operator / (const POINT & sr) const { return SizeRect(loc.x / sr.x,loc.y / sr.y,size.cx,size.cy); }

    __forceinline SizeRect & operator += (const POINT & sr) { loc.x += sr.x; loc.y += sr.y; return *this; }
    __forceinline SizeRect & operator -= (const POINT & sr) { loc.x -= sr.x; loc.y -= sr.y; return *this; }
    __forceinline SizeRect & operator *= (const POINT & sr) { loc.x *= sr.x; loc.y *= sr.y; return *this; }
    __forceinline SizeRect & operator /= (const POINT & sr) { loc.x /= sr.x; loc.y /= sr.y; return *this; }

    __forceinline SizeRect operator + (const SIZE & sr) const { return SizeRect(loc.x,loc.y,size.cx + sr.cx,size.cy + sr.cy); }
    __forceinline SizeRect operator - (const SIZE & sr) const { return SizeRect(loc.x,loc.y,size.cx - sr.cx,size.cy - sr.cy); }
    __forceinline SizeRect operator * (const SIZE & sr) const { return SizeRect(loc.x,loc.y,size.cx * sr.cx,size.cy * sr.cy); }
    __forceinline SizeRect operator / (const SIZE & sr) const { return SizeRect(loc.x,loc.y,size.cx / sr.cx,size.cy / sr.cy); }

    __forceinline SizeRect & operator += (const SIZE & sr) { size.cx += sr.cx; size.cy += sr.cy; return *this; }
    __forceinline SizeRect & operator -= (const SIZE & sr) { size.cx -= sr.cx; size.cy -= sr.cy; return *this; }
    __forceinline SizeRect & operator *= (const SIZE & sr) { size.cx *= sr.cx; size.cy *= sr.cy; return *this; }
    __forceinline SizeRect & operator /= (const SIZE & sr) { size.cx /= sr.cx; size.cy /= sr.cy; return *this; }

    __forceinline bool operator == (const SizeRect & sr) { loc.x == sr.loc.x && loc.y == sr.loc.y && size.cx == sr.size.cx && size.cy == sr.size.cy; }
    __forceinline bool operator != (const SizeRect & sr) { loc.x != sr.loc.x || loc.y != sr.loc.y || size.cx != sr.size.cx || size.cy != sr.size.cy; }


};

}; // namespace Sage

Sage::CfPoint abs(Sage::CfPoint cfPoint);
Sage::CfPointf abs(Sage::CfPointf cfPoint);
Sage::CPoint abs(Sage::CPoint cPoint);

#pragma warning(pop)

#endif // _CSagePoint_H_
