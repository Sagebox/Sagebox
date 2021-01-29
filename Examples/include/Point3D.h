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

#if !defined(_Point3D_h_)
#define _Point3D_h_ 

#include "CDavinci.h"		// Include Davinci references
#include <cmath>

//using namespace std;
//using namespace SageWin;

// Point3d_t - Create a vector-based point structure.  This allows us to use
// vector functions rather than specifying the same action for each X,Y,Z. 
//
namespace Sage
{
struct Point3D_t
{
	double fX,fY,fZ;
	Point3D_t operator + (double fValue) { return { fX+fValue, fY+fValue, fZ+fValue }; }
	Point3D_t operator + (Point3D_t const &obj) { return { fX+obj.fX, fY+obj.fY, fZ+obj.fZ }; }
	Point3D_t operator - (Point3D_t const &obj) { Point3D_t pTemp; pTemp.fX = fX - obj.fX; pTemp.fY = fY - obj.fY; pTemp.fZ = fZ - obj.fZ; return pTemp; }
	Point3D_t operator ^ (Point3D_t const &obj) { Point3D_t pTemp; pTemp.fX = fY*obj.fZ - fZ*obj.fY; 
										pTemp.fY = fZ*obj.fX - fX*obj.fZ; pTemp.fZ = fX*obj.fY - fY*obj.fX; return pTemp; }	// Cross Product
	Point3D_t operator / (double fValue) { Point3D_t pTemp; pTemp.fX = fX/fValue; pTemp.fY = fY/fValue; pTemp.fZ = fZ/fValue; return pTemp; }
	Point3D_t & operator /= (double fValue) { fX /= fValue; fY /= fValue; fZ /= fValue; return *this; }
	Point3D_t operator * (double fValue) { Point3D_t pTemp; pTemp.fX = fX*fValue; pTemp.fY = fY*fValue; pTemp.fZ = fZ*fValue; return pTemp; }
	Point3D_t & operator *= (double fValue) { fX *= fValue; fY *= fValue; fZ *= fValue; return *this; }
	Point3D_t operator * (Point3D_t const &obj) { return { fX*obj.fX, fY*obj.fY, fZ*obj.fZ }; }
	double operator | (Point3D_t const &obj) { return fX*obj.fX + fY*obj.fY + fZ*obj.fZ; }	// Dot Product
	double Mag() { return std::sqrt(fX*fX + fY*fY + fZ*fZ); }									// Magnitude of vector
	Point3D_t Normalize() { double fMag = Mag(); if (fMag) { fX /= fMag; fY /= fMag; fZ /= fMag; } return *this; }	// Normalize itself (as a quick method)
	Point3D_t RotateX(double fAngle) { return { fX,fY *cos(fAngle) - fZ*sin(fAngle),fY *sin(fAngle) + fZ*cos(fAngle) }; }	// Rotate point around X-Axis
	Point3D_t RotateY(double fAngle) { return { fZ *sin(fAngle) + fX*cos(fAngle),fY, fZ *cos(fAngle) - fX*sin(fAngle) }; }	// Rotate point around Y-Axis
	Point3D_t RotateZ(double fAngle) { return { fX *cos(fAngle) - fY*sin(fAngle),fX *sin(fAngle) + fY*cos(fAngle),fZ }; }	// Rotate point around Y-Axis
	__forceinline RGBColor_t toRGB() { return RGBColor_t{(int) fX,(int) fY,(int) fZ}; };
	__forceinline DWORD toColorRef() { return RGB((int) fX,(int) fY,(int) fZ); };
    operator POINT() const { POINT p = { (int) fX,(int) fY }; return p; };
    operator CfPoint() const { CfPoint p = { fX,fY }; return p; };

};
}; // namespace Sage
#endif		// _Point3D_h_