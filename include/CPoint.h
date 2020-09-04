//#pragma once
#include <Windows.h>
#if !defined(_CSagePoint_H_)
#define _CSagePoint_H_

// CPoint.cpp -- work in progress

namespace Sage
{
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
	CPoint operator * (SIZE & p2)
	{

		return { x * p2.cx,y*p2.cy };
	}
	
	CPoint operator * (CPoint & p2)
	{

		return { x * p2.x,y*p2.y };
	}
	CPoint operator + (CPoint & p2)
	{
		return { x + p2.x, y+p2.y };
	}
	CPoint operator + (POINT & p2)
	{
		return { x + p2.x, y+p2.y };
	}
	CPoint operator / (int p)
	{
		return { x / p, y/p};
	}
	CPoint & operator += (CPoint & p2)
	{
		x += p2.x;
		y += p2.y;
		return *this;
	}
	CPoint operator - (CPoint & p2)
	{
		return { x - p2.x, y-p2.y };
	}
	CPoint & operator -= (CPoint & p2)
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
	CPoint(POINT p)
	{
		x = p.x;
		y = p.y;
	}
	CPoint(SIZE sz)
	{
		x = sz.cx;
		y = sz.cy;
	}
	operator POINT() const { POINT p = { x,y }; return p; };
	operator SIZE() const { SIZE p = { x,y }; return p; };
	CPoint() { }
};

struct CSagefPoint
{
	double x;
	double y;

	CSagefPoint & operator = (CPoint & p)
	{
		x = (double) p.x;
		y = (double) p.y;
		return (*this);
	}
	CSagefPoint operator + (CSagefPoint & p2)
	{
		return { x + p2.x, y+p2.y };
	}
	CSagefPoint operator / (CSagefPoint & p2)
	{
		return { x / p2.x, y/p2.y };
	}
	CSagefPoint & operator /= (CSagefPoint & p2)
	{
		x /= p2.x;
		y /= p2.y;
		return *this;
	}
	CSagefPoint operator / (double fDiv)
	{
		return { x /fDiv, y/fDiv };
	}
	CSagefPoint & operator /= (double fDiv)
	{
		x /= fDiv;
		y /= fDiv;
		return *this;
	}
	CSagefPoint operator * (double fMul)
	{
		return { x * fMul, y*fMul };
	}
	CSagefPoint operator - (CSagefPoint & p2)
	{
		return { x - p2.x, y-p2.y };
	}
	CSagefPoint operator - (CPoint & p2)
	{
		return { x - (double) p2.x, y- (double) p2.y };
	}
	CSagefPoint & operator += (CSagefPoint & p2)
	{
		x += p2.x;
		y += p2.y;
		return *this;
	}
CSagefPoint & operator -= (CSagefPoint & p2)
	{
		x -= p2.x;
		y -= p2.y;
		return *this;
	}

	CSagefPoint() { };
	CSagefPoint(double fx,double fy) { x = fx; y = fy; };
	CSagefPoint(CPoint & p)
	{
		x = (double) p.x;
		y = (double) p.y;
	}
	CSagefPoint(POINT & p)
	{
		x = (double) p.x;
		y = (double) p.y;
	}
	CSagefPoint(SIZE & szSize)
	{
		x = (double) szSize.cx;
		y = (double) szSize.cy;
	}
	POINT operator * () { return { (int) x,(int) y }; }
};
}; // namespace Sage
#endif // _CSagePoint_H_
