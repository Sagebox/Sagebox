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
#include "SimdClass.h"

using namespace std;
//using namespace SageWin;

// Point3d_t - Create a vector-based point structure.  This allows us to use
// vector functions rather than specifying the same action for each X,Y,Z. 
//
namespace Sage
{

struct Point3Df_t
{
	float fX,fY,fZ;
	__SageInline Point3Df_t operator + (float fValue) const { return { fX+fValue, fY+fValue, fZ+fValue }; }
	__SageInline Point3Df_t operator + (Point3Df_t const &obj) const { return { fX+obj.fX, fY+obj.fY, fZ+obj.fZ }; }
	__SageInline Point3Df_t & operator += (Point3Df_t const &obj) { fX += obj.fX; fY += obj.fY; fZ += obj.fZ; return *this; }
	__SageInline Point3Df_t & operator -= (Point3Df_t const &obj) { fX -= obj.fX; fY -= obj.fY; fZ -= obj.fZ; return *this; }
	__SageInline Point3Df_t & operator += (float fValue) { fX+=fValue; fY+=fValue; fZ-=fValue; return *this; }
	__SageInline Point3Df_t & operator -= (float fValue) { fX-=fValue; fY-=fValue; fZ-=fValue; return *this; }
	__SageInline Point3Df_t operator - (Point3Df_t const &obj) const { Point3Df_t pTemp; pTemp.fX = fX - obj.fX; pTemp.fY = fY - obj.fY; pTemp.fZ = fZ - obj.fZ; return pTemp; }
	__SageInline Point3Df_t operator ^ (Point3Df_t const &obj) const { Point3Df_t pTemp; pTemp.fX = fY*obj.fZ - fZ*obj.fY; 
										pTemp.fY = fZ*obj.fX - fX*obj.fZ; pTemp.fZ = fX*obj.fY - fY*obj.fX; return pTemp; }	// Cross Product
	__SageInline Point3Df_t operator / (float fValue) const { Point3Df_t pTemp; pTemp.fX = fX/fValue; pTemp.fY = fY/fValue; pTemp.fZ = fZ/fValue; return pTemp; }
	__SageInline Point3Df_t & operator /= (float fValue) { fX /= fValue; fY /= fValue; fZ /= fValue; return *this; }
	__SageInline Point3Df_t operator / (Point3Df_t const &obj) const { Point3Df_t pTemp; pTemp.fX = fX/obj.fX; pTemp.fY = fY/obj.fY; pTemp.fZ = fZ/obj.fZ; return pTemp; }
	__SageInline Point3Df_t & operator /=  (Point3Df_t const &obj) { fX /= obj.fX; fY /= obj.fY; fZ /= obj.fZ; return *this;; }

    __SageInline Point3Df_t operator - () const { return { -fX, -fY, -fZ }; }
	__SageInline Point3Df_t operator * (float fValue) const { Point3Df_t pTemp; pTemp.fX = fX*fValue; pTemp.fY = fY*fValue; pTemp.fZ = fZ*fValue; return pTemp; }
	__SageInline Point3Df_t & operator *= (float fValue) { fX *= fValue; fY *= fValue; fZ *= fValue; return *this; }
	__SageInline Point3Df_t operator * (Point3Df_t const &obj) const { return { fX*obj.fX, fY*obj.fY, fZ*obj.fZ }; }
	__SageInline float operator | (Point3Df_t const &obj) const { return fX*obj.fX + fY*obj.fY + fZ*obj.fZ; }	// Dot Product
	__SageInline float Mag() const { return std::sqrt(fX*fX + fY*fY + fZ*fZ); }							// Magnitude of vector
	__SageInline float MagSq() const { return fX*fX + fY*fY + fZ*fZ; }									// Magnitude of vector ^2
    __SageInline Point3Df_t Normalize() const { float fMag = Mag(); return fMag ? Point3Df_t{ fX / fMag, fY / fMag, fZ / fMag } : Point3Df_t{ 0,0,0}; }	// Normalize
	__SageInline Point3Df_t & selfNormalize() { float fMag = Mag(); if (fMag) { fX /= fMag; fY /= fMag; fZ /= fMag; } return *this; }	// Normalize itself (as a quick method)
	__SageInline Point3Df_t RotateX(float fAngle) const { return { fX,fY *cosf(fAngle) - fZ*sinf(fAngle),fY *sinf(fAngle) + fZ*cosf(fAngle) }; }	    // Rotate point around X-Axis
	__SageInline Point3Df_t RotateY(float fAngle) const { return { fZ *sinf(fAngle) + fX*cosf(fAngle),fY, fZ *cosf(fAngle) - fX*sinf(fAngle) }; }	// Rotate point around Y-Axis
	__SageInline Point3Df_t RotateZ(float fAngle) const { return { fX *cosf(fAngle) - fY*sinf(fAngle),fX *sinf(fAngle) + fY*cosf(fAngle),fZ }; }	    // Rotate point around Z-Axis
	__SageInline Point3Df_t & selfRotateX(float fAngle) { float fY2 = fY *cosf(fAngle) - fZ*sinf(fAngle); fZ = fY *sinf(fAngle) + fZ*cosf(fAngle); fY = fY2; return *this; }	// Rotate point around X-Axis
	__SageInline Point3Df_t & selfRotateY(float fAngle) { float fX2 = fZ *sinf(fAngle) + fX*cosf(fAngle); fZ = fZ *cosf(fAngle) - fX*sinf(fAngle); fX = fX2; return *this; }	// Rotate point around Y-Axis
	__SageInline Point3Df_t & selfRotateZ(float fAngle) { float fX2 = fX *cosf(fAngle) - fY*sinf(fAngle); fY = fX *sinf(fAngle) + fY*cosf(fAngle); fX = fX2; return *this; }	// Rotate point around Z-Axis
	__SageInline RGBColor_t toRGB() const { return RGBColor_t{(int) fX,(int) fY,(int) fZ}; };
	__SageInline DWORD toColorRef() const { return RGB((int) fX,(int) fY,(int) fZ); };
    operator POINT() const { POINT p = { (int) fX,(int) fY }; return p; };
    operator CfPoint() const { CfPoint p = { fX,fY }; return p; };
    operator CPoint() const { CPoint p = { (int) fX,(int) fY }; return p; };
    operator Point3Df_t() const { Point3Df_t{ (float) fX,(float) fY, (float) fZ }; };

    CPoint toCPoint() { return CPoint{(decltype(CPoint::x)) fX,(decltype(CPoint::y)) fY }; }
    CfPoint toCfPoint() { return CfPoint{(decltype(CfPoint::x)) fX,(decltype(CfPoint::y)) fY }; }

   static Point3Df_t fromRgb(const RgbColor & rgbColor) { return {  (decltype(Point3Df_t::fX)) rgbColor.iRed/255.0f, 
                                                                    (decltype(Point3Df_t::fY)) rgbColor.iGreen/255.0f, 
                                                                    (decltype(Point3Df_t::fZ)) rgbColor.iBlue/255.0f }; };

};

struct Point3D_t
{
	double fX,fY,fZ;
	__SageInline Point3D_t operator + (double fValue) const { return { fX+fValue, fY+fValue, fZ+fValue }; }
	__SageInline Point3D_t operator + (Point3D_t const &obj) const { return { fX+obj.fX, fY+obj.fY, fZ+obj.fZ }; }
	__SageInline Point3D_t & operator += (Point3D_t const &obj) { fX += obj.fX; fY += obj.fY; fZ += obj.fZ; return *this; }
	__SageInline Point3D_t & operator -= (Point3D_t const &obj) { fX -= obj.fX; fY -= obj.fY; fZ -= obj.fZ; return *this; }
	__SageInline Point3D_t & operator += (double fValue) { fX+=fValue; fY+=fValue; fZ-=fValue; return *this; }
	__SageInline Point3D_t & operator -= (double fValue) { fX-=fValue; fY-=fValue; fZ-=fValue; return *this; }
	__SageInline Point3D_t operator - (Point3D_t const &obj) const { Point3D_t pTemp; pTemp.fX = fX - obj.fX; pTemp.fY = fY - obj.fY; pTemp.fZ = fZ - obj.fZ; return pTemp; }
	__SageInline Point3D_t operator ^ (Point3D_t const &obj) const { Point3D_t pTemp; pTemp.fX = fY*obj.fZ - fZ*obj.fY; 
										pTemp.fY = fZ*obj.fX - fX*obj.fZ; pTemp.fZ = fX*obj.fY - fY*obj.fX; return pTemp; }	// Cross Product
	__SageInline Point3D_t operator / (double fValue) const { Point3D_t pTemp; pTemp.fX = fX/fValue; pTemp.fY = fY/fValue; pTemp.fZ = fZ/fValue; return pTemp; }
	__SageInline Point3D_t & operator /= (double fValue) { fX /= fValue; fY /= fValue; fZ /= fValue; return *this; }
	__SageInline Point3D_t operator / (Point3D_t const &obj) const { Point3D_t pTemp; pTemp.fX = fX/obj.fX; pTemp.fY = fY/obj.fY; pTemp.fZ = fZ/obj.fZ; return pTemp; }
	__SageInline Point3D_t & operator /=  (Point3D_t const &obj) { fX /= obj.fX; fY /= obj.fY; fZ /= obj.fZ; return *this;; }

    __SageInline Point3D_t operator - () const { return { -fX, -fY, -fZ }; }
	__SageInline Point3D_t operator * (double fValue) const { Point3D_t pTemp; pTemp.fX = fX*fValue; pTemp.fY = fY*fValue; pTemp.fZ = fZ*fValue; return pTemp; }
	__SageInline Point3D_t & operator *= (double fValue) { fX *= fValue; fY *= fValue; fZ *= fValue; return *this; }
	__SageInline Point3D_t operator * (Point3D_t const &obj) const { return { fX*obj.fX, fY*obj.fY, fZ*obj.fZ }; }
	__SageInline double operator | (Point3D_t const &obj) const { return fX*obj.fX + fY*obj.fY + fZ*obj.fZ; }	// Dot Product
	__SageInline double Mag() const { return std::sqrt(fX*fX + fY*fY + fZ*fZ); }							// Magnitude of vector
	__SageInline double MagSq() const { return fX*fX + fY*fY + fZ*fZ; }									// Magnitude of vector ^2
    __SageInline Point3D_t Normalize() const { double fMag = Mag(); return fMag ? Point3D_t{ fX / fMag, fY / fMag, fZ / fMag } : Point3D_t{ 0,0,0}; }	// Normalize
	__SageInline Point3D_t & selfNormalize() { double fMag = Mag(); if (fMag) { fX /= fMag; fY /= fMag; fZ /= fMag; } return *this; }	// Normalize itself (as a quick method)
	__SageInline Point3D_t RotateX(double fAngle) const { return { fX,fY *cos(fAngle) - fZ*sin(fAngle),fY *sin(fAngle) + fZ*cos(fAngle) }; }	    // Rotate point around X-Axis
	__SageInline Point3D_t RotateY(double fAngle) const { return { fZ *sin(fAngle) + fX*cos(fAngle),fY, fZ *cos(fAngle) - fX*sin(fAngle) }; }	// Rotate point around Y-Axis
	__SageInline Point3D_t RotateZ(double fAngle) const { return { fX *cos(fAngle) - fY*sin(fAngle),fX *sin(fAngle) + fY*cos(fAngle),fZ }; }	    // Rotate point around Z-Axis
	__SageInline Point3D_t RotateXYZ(Point3D_t pAngles) const { return RotateX(pAngles.fX).selfRotateY(pAngles.fY).selfRotateZ(pAngles.fZ); }	    // Rotate point around Z-Axis
	__SageInline Point3D_t & selfRotateX(double fAngle) { double fY2 = fY *cos(fAngle) - fZ*sin(fAngle); fZ = fY *sin(fAngle) + fZ*cos(fAngle); fY = fY2; return *this; }	// Rotate point around X-Axis
	__SageInline Point3D_t & selfRotateY(double fAngle) { double fX2 = fZ *sin(fAngle) + fX*cos(fAngle); fZ = fZ *cos(fAngle) - fX*sin(fAngle); fX = fX2; return *this; }	// Rotate point around Y-Axis
	__SageInline Point3D_t & selfRotateZ(double fAngle) { double fX2 = fX *cos(fAngle) - fY*sin(fAngle); fY = fX *sin(fAngle) + fY*cos(fAngle); fX = fX2; return *this; }	// Rotate point around Z-Axis
	__SageInline Point3D_t & selfRotateXYZ(Point3D_t pAngles) { selfRotateX(pAngles.fX).selfRotateY(pAngles.fY).selfRotateZ(pAngles.fZ); return *this; }	// Rotate point around X-Axis
	__SageInline RGBColor_t toRGB() const { return RGBColor_t{(int) fX,(int) fY,(int) fZ}; };
	__SageInline DWORD toColorRef() const { return RGB((int) fX,(int) fY,(int) fZ); };
    operator POINT() const { POINT p = { (int) fX,(int) fY }; return p; };
    operator CfPoint() const { CfPoint p = { fX,fY }; return p; };
    operator CPoint() const { CPoint p = { (int) fX,(int) fY }; return p; };

    CPoint toCPoint() { return CPoint{(decltype(CPoint::x)) fX,(decltype(CPoint::y)) fY }; }
    CfPoint toCfPoint() { return CfPoint{(decltype(CfPoint::x)) fX,(decltype(CfPoint::y)) fY }; }
    static Point3D_t fromRgb(const RgbColor & rgbColor) { return {  (decltype(Point3D_t::fX)) rgbColor.iRed/255.0, 
                                                                    (decltype(Point3D_t::fY)) rgbColor.iGreen/255.0, 
                                                                    (decltype(Point3D_t::fZ)) rgbColor.iBlue/255.0 }; }

};

//#ifdef SAGEBOX_AVX

struct Point3DfSimd4 : public Simd128
{

public:
	__m128 x;
	__m128 y;
	__m128 z;
private:
    using nType = decltype(x.m128_f32[0]);                      // i.e. native type; float, double, etc. 

    __forceinline __m128 nVec(nType f) const { return Vecf(f); };     // i.e. native vector
public:
    __SageInline Point3DfSimd4(const __m128 & _x,const __m128 & _y,__m128 _z)
    {
        x = _x; y = _y; z = _z; 
    }
    __SageInline Point3DfSimd4(Point3DfSimd4 && p2) noexcept                 { x = p2.x; y = p2.y; z = p2.z; }
    __SageInline Point3DfSimd4(Point3DfSimd4 & p2) noexcept                  { x = p2.x; y = p2.y; z = p2.z; }
    __SageInline Point3DfSimd4 & operator =(Point3DfSimd4 && p2) noexcept    { x = p2.x, y = p2.y; z = p2.z; return * this; }

	__SageInline Point3DfSimd4 operator + (Point3DfSimd4 const &obj) const { return { Add(x,obj.x), Add(y,obj.y), Add(z,obj.z) }; }
	__SageInline Point3DfSimd4 operator - (Point3DfSimd4 const &obj) const { return { Sub(x,obj.x), Sub(y,obj.y), Sub(z,obj.z) }; }
	__SageInline Point3DfSimd4 operator * (Point3DfSimd4 const &obj) const { return { Mul(x,obj.x), Mul(y,obj.y), Mul(z,obj.z) }; }
	__SageInline Point3DfSimd4 operator / (Point3DfSimd4 const &obj) const { return { Div(x,obj.x), Div(y,obj.y), Div(z,obj.z) }; }

	__SageInline Point3DfSimd4 & operator += (Point3DfSimd4 const &obj) { x = Add(x,obj.x); y = Add(y,obj.y); z = Add(z,obj.z); return *this; }
	__SageInline Point3DfSimd4 & operator -= (Point3DfSimd4 const &obj) { x = Sub(x,obj.x); y = Sub(y,obj.y); z = Sub(z,obj.z); return *this; }
	__SageInline Point3DfSimd4 & operator *= (Point3DfSimd4 const &obj) { x = Mul(x,obj.x); y = Mul(y,obj.y); z = Mul(z,obj.z); return *this; }
	__SageInline Point3DfSimd4 & operator /= (Point3DfSimd4 const &obj) { x = Div(x,obj.x); y = Div(y,obj.y); z = Div(z,obj.z); return *this; }

    Point3DfSimd4() {}

    // Dot product, i.e. a | b

    __SageInline __m128 operator | (Point3DfSimd4 const &obj) const { return Add(Add(Mul(x,obj.x), Mul(y,obj.y)), Mul(z,obj.z)); }

    // Cross Product, i.e. a ^ b

	__SageInline Point3DfSimd4 operator ^ (Point3DfSimd4 &obj) 
    { 
        Point3DfSimd4 pTemp; 
        pTemp.x = Sub(Mul(y,obj.z),Mul(z,obj.y)); 
        pTemp.y = Sub(Mul(z,obj.x),Mul(x,obj.z)); 
        pTemp.z = Sub(Mul(x,obj.y),Mul(y,obj.x)); 
        return pTemp; 
    }	

	__SageInline Point3DfSimd4 operator + (nType fValue) const { auto vTemp = nVec(fValue); return { Add(x,vTemp), Add(y,vTemp), Add(z,vTemp) }; }
	__SageInline Point3DfSimd4 operator - (nType fValue) const { auto vTemp = nVec(fValue); return { Sub(x,vTemp), Sub(y,vTemp), Sub(z,vTemp) }; }
	__SageInline Point3DfSimd4 operator * (nType fValue) const { auto vTemp = nVec(fValue); return { Mul(x,vTemp), Mul(y,vTemp), Mul(z,vTemp) }; }
	__SageInline Point3DfSimd4 operator / (nType fValue) const { auto vTemp = nVec(fValue); return { Div(x,vTemp), Div(y,vTemp), Div(z,vTemp) }; }

	__SageInline Point3DfSimd4 & operator += (nType fValue) { auto vTemp = nVec(fValue); x = Add(x,vTemp); y = Add(y,vTemp); z = Add(z,vTemp); return *this; }
	__SageInline Point3DfSimd4 & operator -= (nType fValue) { auto vTemp = nVec(fValue); x = Sub(x,vTemp); y = Sub(y,vTemp); z = Sub(z,vTemp); return *this; }
	__SageInline Point3DfSimd4 & operator *= (nType fValue) { auto vTemp = nVec(fValue); x = Mul(x,vTemp); y = Mul(y,vTemp); z = Mul(z,vTemp); return *this; }
	__SageInline Point3DfSimd4 & operator /= (nType fValue) { auto vTemp = nVec(fValue); x = Div(x,vTemp); y = Div(y,vTemp); z = Div(z,vTemp); return *this; }


	__SageInline __m128 Mag() const { return Sqrt(Add(Add(Mul(x,x),Mul(y,y)),Mul(z,z))); }		// Magnitude of vector // Note: AVX512 has an instruction for this
	__SageInline __m128 MagSq() const { return Add(Add(Mul(x,x),Mul(y,y)),Mul(z,z)); 	 }		// Magnitude of vector ^2


    __SageInline Point3DfSimd4 Normalize() const
    { 
        auto fMag = Mag();  // We'll just assume the risk of divide-by-zero for now; later we an convert this to a masked operation, at least for 
                            // SSE/AVX platforms that support AVX512-based masked instructions

        return { Div(x,fMag), Div(y,fMag), Div(z,fMag) };
    }

    __SageInline Point3DfSimd4 & selfNormalize() 
    { 
        auto fMag = Mag();  // We'll just assume the risk of divide-by-zero for now; later we an convert this to a masked operation, at least for 
                            // SSE/AVX platforms that support AVX512-based masked instructions

        x = Div(x,fMag); y = Div(y,fMag); z = Div(z,fMag);
        return (*this);
    }
    
    __SageInline Point3DfSimd4 RotateX(nType fAngle) const { auto fCos = Vecf(cosf(fAngle)); auto fSin = Vecf(sinf(fAngle)); 
                                                             return { x, Sub(Mul(y,fCos), Mul(x,fSin)), Add(Mul(y,fSin), Mul(z,fCos)) }; }	    // Rotate point around X-Axis

    __SageInline Point3DfSimd4 RotateX(__m128 fAngle) const { auto fCos = Cos(fAngle); auto fSin = Sin(fAngle); 
                                                            return { x, Sub(Mul(y,fCos), Mul(x,fSin)), Add(Mul(y,fSin), Mul(z,fCos)) }; }	    // Rotate point around X-Axis

    __SageInline Point3DfSimd4 & selfRotateX(__m128 fAngle) { auto fCos = Cos(fAngle); auto fSin = Sin(fAngle); 
                                                              auto pTempY = Sub(Mul(y,fCos), Mul(x,fSin));
                                                              z = Add(Mul(y,fSin), Mul(z,fCos));
                                                              y = pTempY;
                                                              return *this; 
                                                             }

    __SageInline Point3DfSimd4 & selfRotateX(nType fAngle) {  auto fCos = Vecf(cosf(fAngle)); auto fSin = Vecf(sinf(fAngle)); 
                                                              auto pTempY = Sub(Mul(y,fCos), Mul(x,fSin));
                                                              z = Add(Mul(y,fSin), Mul(z,fCos));
                                                              y = pTempY;
                                                              return *this; 
                                                            }

    __SageInline Point3DfSimd4 RotateY(nType fAngle) const { auto fCos = Vecf(cosf(fAngle)); auto fSin = Vecf(sinf(fAngle)); 
                                                             return {  Add(Mul(z,fSin), Mul(x,fCos)), y, Sub(Mul(z,fCos),Mul(x,fSin)) }; }	    // Rotate point around Y-Axis


    __SageInline Point3DfSimd4 RotateY(__m128 fAngle) const { auto fCos = Cos(fAngle); auto fSin = Sin(fAngle); 
                                                             return {  Add(Mul(z,fSin), Mul(x,fCos)), y, Sub(Mul(z,fCos),Mul(x,fSin)) }; }	    // Rotate point around Y-Axis


    __SageInline Point3DfSimd4 & selfRotateY(nType fAngle) { auto fCos = Vecf(cosf(fAngle)); auto fSin = Vecf(sinf(fAngle)); 
                                                             auto pTempX =  Add(Mul(z,fSin), Mul(x,fCos));
                                                             z = Sub(Mul(z,fCos),Mul(x,fSin));
                                                             x = pTempX;
                                                             return *this; 
                                                            }

    __SageInline Point3DfSimd4 & selfRotateY(__m128 fAngle) { auto fCos = Cos(fAngle); auto fSin = Sin(fAngle); 
                                                             auto pTempX =  Add(Mul(z,fSin), Mul(x,fCos));
                                                             z = Sub(Mul(z,fCos),Mul(x,fSin));
                                                             x = pTempX;
                                                             return *this; 
                                                            }


   __SageInline Point3DfSimd4 RotateZ(nType fAngle) const { auto fCos = Vecf(cosf(fAngle)); auto fSin = Vecf(sinf(fAngle)); 
                                                            return { Sub(Mul(x,fCos),Mul(y,fSin)), Add(Mul(x,fSin),Mul(y,fCos)), z };}

   __SageInline Point3DfSimd4 RotateZ(__m128 fAngle) const { auto fCos = Cos(fAngle); auto fSin = Sin(fAngle); 
                                                            return { Sub(Mul(x,fCos),Mul(y,fSin)), Add(Mul(x,fSin),Mul(y,fCos)), z };}

  __SageInline Point3DfSimd4 & selfRotateZ(nType fAngle) { auto fCos = Vecf(cosf(fAngle)); auto fSin = Vecf(sinf(fAngle)); 
                                                                 auto pTempX = Sub(Mul(x,fCos),Mul(y,fSin));
                                                                 y = Add(Mul(x,fSin),Mul(y,fCos));
                                                                 x = pTempX;
                                                                 return *this;
                                                               }
  
  __SageInline Point3DfSimd4 & selfRotateZ(__m128 fAngle)  { auto fCos = Cos(fAngle); auto fSin = Sin(fAngle); 
                                                                  auto pTempX = Sub(Mul(x,fCos),Mul(y,fSin));
                                                                  y = Add(Mul(x,fSin),Mul(y,fCos));
                                                                  x = pTempX;
                                                                  return *this;
                                                                }
 


  __SageInline Point3DfSimd4 operator - () const { return { Xor(x,Vecf(-0.0)), Xor(y,Vecf(-0.0)), Xor(z,Vecf(-0.0)) }; }

};


struct Point3DfSimd8 : public Simd256
{

public:
	__m256 fX;
	__m256 fY;
	__m256 fZ;
private:
    using nType = decltype(fX.m256_f32[0]);                      // i.e. native type; float, double, etc. 

    __forceinline __m256 nVec(nType f) const { return Vecf(f); };     // i.e. native vector
public:
    __SageInline Point3DfSimd8(const __m256 & _x,const __m256 & _y,__m256 _z)
    {
        fX = _x; fY = _y; fZ = _z; 
    }
    __SageInline Point3DfSimd8(Point3DfSimd8 && p2) noexcept                 { fX = p2.fX; fY = p2.fY; fZ = p2.fZ; }
    __SageInline Point3DfSimd8(Point3DfSimd8 & p2) noexcept                  { fX = p2.fX; fY = p2.fY; fZ = p2.fZ; }
    __SageInline Point3DfSimd8 & operator =(Point3DfSimd8 && p2) noexcept    { fX = p2.fX, fY = p2.fY; fZ = p2.fZ; return * this; }

	__SageInline Point3DfSimd8 operator + (Point3DfSimd8 const &obj) const { return { Add(fX,obj.fX), Add(fY,obj.fY), Add(fZ,obj.fZ) }; }
	__SageInline Point3DfSimd8 operator - (Point3DfSimd8 const &obj) const { return { Sub(fX,obj.fX), Sub(fY,obj.fY), Sub(fZ,obj.fZ) }; }
	__SageInline Point3DfSimd8 operator * (Point3DfSimd8 const &obj) const { return { Mul(fX,obj.fX), Mul(fY,obj.fY), Mul(fZ,obj.fZ) }; }
	__SageInline Point3DfSimd8 operator / (Point3DfSimd8 const &obj) const { return { Div(fX,obj.fX), Div(fY,obj.fY), Div(fZ,obj.fZ) }; }

    __SageInline Point3DfSimd8 & operator += (Point3DfSimd8 const &obj) { fX = Add(fX,obj.fX); fY = Add(fY,obj.fY); fZ = Add(fZ,obj.fZ); return *this; }
	__SageInline Point3DfSimd8 & operator -= (Point3DfSimd8 const &obj) { fX = Sub(fX,obj.fX); fY = Sub(fY,obj.fY); fZ = Sub(fZ,obj.fZ); return *this; }
	__SageInline Point3DfSimd8 & operator *= (Point3DfSimd8 const &obj) { fX = Mul(fX,obj.fX); fY = Mul(fY,obj.fY); fZ = Mul(fZ,obj.fZ); return *this; }
	__SageInline Point3DfSimd8 & operator /= (Point3DfSimd8 const &obj) { fX = Div(fX,obj.fX); fY = Div(fY,obj.fY); fZ = Div(fZ,obj.fZ); return *this; }

    Point3DfSimd8() {}

    // Dot product, i.e. a | b

    __SageInline __m256 operator | (Point3DfSimd8 const &obj) const { return Add(Add(Mul(fX,obj.fX), Mul(fY,obj.fY)), Mul(fZ,obj.fZ)); }

    // Cross Product, i.e. a ^ b

	__SageInline Point3DfSimd8 operator ^ (Point3DfSimd8 &obj) 
    { 
        Point3DfSimd8 pTemp; 
        pTemp.fX = Sub(Mul(fY,obj.fZ),Mul(fZ,obj.fY)); 
        pTemp.fY = Sub(Mul(fZ,obj.fX),Mul(fX,obj.fZ)); 
        pTemp.fZ = Sub(Mul(fX,obj.fY),Mul(fY,obj.fX)); 
        return pTemp; 
    }	

	__SageInline Point3DfSimd8 operator + (nType fValue) const { auto vTemp = nVec(fValue); return { Add(fX,vTemp), Add(fY,vTemp), Add(fZ,vTemp) }; }
	__SageInline Point3DfSimd8 operator - (nType fValue) const { auto vTemp = nVec(fValue); return { Sub(fX,vTemp), Sub(fY,vTemp), Sub(fZ,vTemp) }; }
	__SageInline Point3DfSimd8 operator * (nType fValue) const { auto vTemp = nVec(fValue); return { Mul(fX,vTemp), Mul(fY,vTemp), Mul(fZ,vTemp) }; }
	__SageInline Point3DfSimd8 operator / (nType fValue) const { auto vTemp = nVec(fValue); return { Div(fX,vTemp), Div(fY,vTemp), Div(fZ,vTemp) }; }

	__SageInline Point3DfSimd8 & operator += (nType fValue) { auto vTemp = nVec(fValue); fX = Add(fX,vTemp); fY = Add(fY,vTemp); fZ = Add(fZ,vTemp); return *this; }
	__SageInline Point3DfSimd8 & operator -= (nType fValue) { auto vTemp = nVec(fValue); fX = Sub(fX,vTemp); fY = Sub(fY,vTemp); fZ = Sub(fZ,vTemp); return *this; }
	__SageInline Point3DfSimd8 & operator *= (nType fValue) { auto vTemp = nVec(fValue); fX = Mul(fX,vTemp); fY = Mul(fY,vTemp); fZ = Mul(fZ,vTemp); return *this; }
	__SageInline Point3DfSimd8 & operator /= (nType fValue) { auto vTemp = nVec(fValue); fX = Div(fX,vTemp); fY = Div(fY,vTemp); fZ = Div(fZ,vTemp); return *this; }

	__SageInline Point3DfSimd8 operator + (Point3DfSimd8 & obj) const {  return { Add(fX,obj.fX), Add(fY,obj.fY), Add(fZ,obj.fZ) }; }
	__SageInline Point3DfSimd8 operator - (Point3DfSimd8 & obj) const {  return { Sub(fX,obj.fX), Sub(fY,obj.fY), Sub(fZ,obj.fZ) }; }
	__SageInline Point3DfSimd8 operator * (Point3DfSimd8 & obj) const {  return { Mul(fX,obj.fX), Mul(fY,obj.fY), Mul(fZ,obj.fZ) }; }
	__SageInline Point3DfSimd8 operator / (Point3DfSimd8 & obj) const {  return { Div(fX,obj.fX), Div(fY,obj.fY), Div(fZ,obj.fZ) }; }

	__SageInline Point3DfSimd8 & operator += (Point3DfSimd8 & obj)  { fX = Add(fX,obj.fX); fY = Add(fY,obj.fY); fZ = Add(fZ,obj.fZ); return *this; }
	__SageInline Point3DfSimd8 & operator -= (Point3DfSimd8 & obj)  { fX = Sub(fX,obj.fX); fY = Sub(fY,obj.fY); fZ = Sub(fZ,obj.fZ); return *this; }
	__SageInline Point3DfSimd8 & operator *= (Point3DfSimd8 & obj)  { fX = Mul(fX,obj.fX); fY = Mul(fY,obj.fY); fZ = Mul(fZ,obj.fZ); return *this; }
	__SageInline Point3DfSimd8 & operator /= (Point3DfSimd8 & obj)  { fX = Div(fX,obj.fX); fY = Div(fY,obj.fY); fZ = Div(fZ,obj.fZ); return *this; }

	__SageInline Point3DfSimd8 operator + (__m256 & mValue) const {  return { Add(fX,mValue), Add(fY,mValue), Add(fZ,mValue) }; }
	__SageInline Point3DfSimd8 operator - (__m256 & mValue) const {  return { Sub(fX,mValue), Sub(fY,mValue), Sub(fZ,mValue) }; }
	__SageInline Point3DfSimd8 operator * (__m256 & mValue) const {  return { Mul(fX,mValue), Mul(fY,mValue), Mul(fZ,mValue) }; }
	__SageInline Point3DfSimd8 operator / (__m256 & mValue) const {  return { Div(fX,mValue), Div(fY,mValue), Div(fZ,mValue) }; }

	__SageInline Point3DfSimd8 & operator += (__m256 & mValue)  { fX = Add(fX,mValue); fY = Add(fY,mValue); fZ = Add(fZ,mValue); return *this; }
	__SageInline Point3DfSimd8 & operator -= (__m256 & mValue)  { fX = Sub(fX,mValue); fY = Sub(fY,mValue); fZ = Sub(fZ,mValue); return *this; }
	__SageInline Point3DfSimd8 & operator *= (__m256 & mValue)  { fX = Mul(fX,mValue); fY = Mul(fY,mValue); fZ = Mul(fZ,mValue); return *this; }
	__SageInline Point3DfSimd8 & operator /= (__m256 & mValue)  { fX = Div(fX,mValue); fY = Div(fY,mValue); fZ = Div(fZ,mValue); return *this; }

	__SageInline Point3DfSimd8 operator + (Point3Df_t & pValue) const {  return { Add(fX,nVec(pValue.fX)), Add(fY,nVec(pValue.fY)), Add(fZ,nVec(pValue.fZ)) }; }
	__SageInline Point3DfSimd8 operator - (Point3Df_t & pValue) const {  return { Sub(fX,nVec(pValue.fX)), Sub(fY,nVec(pValue.fY)), Sub(fZ,nVec(pValue.fZ)) }; }
	__SageInline Point3DfSimd8 operator * (Point3Df_t & pValue) const {  return { Mul(fX,nVec(pValue.fX)), Mul(fY,nVec(pValue.fY)), Mul(fZ,nVec(pValue.fZ)) }; }
	__SageInline Point3DfSimd8 operator / (Point3Df_t & pValue) const {  return { Div(fX,nVec(pValue.fX)), Div(fY,nVec(pValue.fY)), Div(fZ,nVec(pValue.fZ)) }; }

	__SageInline Point3DfSimd8 & operator += (Point3Df_t & pValue)  { fX = Add(fX,nVec(pValue.fX)); fY = Add(fY,nVec(pValue.fY)); fZ = Add(fZ,nVec(pValue.fZ)); return *this; }
	__SageInline Point3DfSimd8 & operator -= (Point3Df_t & pValue)  { fX = Sub(fX,nVec(pValue.fX)); fY = Sub(fY,nVec(pValue.fY)); fZ = Sub(fZ,nVec(pValue.fZ)); return *this; }
	__SageInline Point3DfSimd8 & operator *= (Point3Df_t & pValue)  { fX = Mul(fX,nVec(pValue.fX)); fY = Mul(fY,nVec(pValue.fY)); fZ = Mul(fZ,nVec(pValue.fZ)); return *this; }
	__SageInline Point3DfSimd8 & operator /= (Point3Df_t & pValue)  { fX = Div(fX,nVec(pValue.fX)); fY = Div(fY,nVec(pValue.fY)); fZ = Div(fZ,nVec(pValue.fZ)); return *this; }


	__SageInline __m256 Mag() const { return Sqrt(Add(Add(Mul(fX,fX),Mul(fY,fY)),Mul(fZ,fZ))); }		// Magnitude of vector // Note: AVX512 has an instruction for this
	__SageInline __m256 MagSq() const { return Add(Add(Mul(fX,fX),Mul(fY,fY)),Mul(fZ,fZ)); 	 }		// Magnitude of vector ^2


    __SageInline Point3DfSimd8 Normalize() const
    { 
        auto fMag = Mag();  // We'll just assume the risk of divide-by-zero for now; later we an convert this to a masked operation, at least for 
                            // SSE/AVX platforms that support AVX512-based masked instructions

        return { Div(fX,fMag), Div(fY,fMag), Div(fZ,fMag) };
    }

    __SageInline Point3DfSimd8 & selfNormalize() 
    { 
        auto fMag = Mag();  // We'll just assume the risk of divide-by-zero for now; later we an convert this to a masked operation, at least for 
                            // SSE/AVX platforms that support AVX512-based masked instructions

        fX = Div(fX,fMag); fY = Div(fY,fMag); fZ = Div(fZ,fMag);
        return (*this);
    }
    
    __SageInline Point3DfSimd8 RotateX(nType fAngle) const { auto fCos = Vecf(cosf(fAngle)); auto fSin = Vecf(sinf(fAngle)); 
                                                             return { fX, Sub(Mul(fY,fCos), Mul(fX,fSin)), Add(Mul(fY,fSin), Mul(fZ,fCos)) }; }	    // Rotate point around X-Axis

    __SageInline Point3DfSimd8 RotateX(__m256 fAngle) const { auto fCos = Cos(fAngle); auto fSin = Sin(fAngle); 
                                                            return { fX, Sub(Mul(fY,fCos), Mul(fX,fSin)), Add(Mul(fY,fSin), Mul(fZ,fCos)) }; }	    // Rotate point around X-Axis

    __SageInline Point3DfSimd8 & selfRotateX(__m256 fAngle) { auto fCos = Cos(fAngle); auto fSin = Sin(fAngle); 
                                                              auto pTempY = Sub(Mul(fY,fCos), Mul(fX,fSin));
                                                              fZ = Add(Mul(fY,fSin), Mul(fZ,fCos));
                                                              fY = pTempY;
                                                              return *this; 
                                                             }

    __SageInline Point3DfSimd8 & selfRotateX(nType fAngle) {  auto fCos = Vecf(cosf(fAngle)); auto fSin = Vecf(sinf(fAngle)); 
                                                              auto pTempY = Sub(Mul(fY,fCos), Mul(fX,fSin));
                                                              fZ = Add(Mul(fY,fSin), Mul(fZ,fCos));
                                                              fY = pTempY;
                                                              return *this; 
                                                            }

    __SageInline Point3DfSimd8 RotateY(nType fAngle) const { auto fCos = Vecf(cosf(fAngle)); auto fSin = Vecf(sinf(fAngle)); 
                                                             return {  Add(Mul(fZ,fSin), Mul(fX,fCos)), fY, Sub(Mul(fZ,fCos),Mul(fX,fSin)) }; }	    // Rotate point around Y-Axis


    __SageInline Point3DfSimd8 RotateY(__m256 fAngle) const { auto fCos = Cos(fAngle); auto fSin = Sin(fAngle); 
                                                             return {  Add(Mul(fZ,fSin), Mul(fX,fCos)), fY, Sub(Mul(fZ,fCos),Mul(fX,fSin)) }; }	    // Rotate point around Y-Axis


    __SageInline Point3DfSimd8 & selfRotateY(nType fAngle) { auto fCos = Vecf(cosf(fAngle)); auto fSin = Vecf(sinf(fAngle)); 
                                                             auto pTempX =  Add(Mul(fZ,fSin), Mul(fX,fCos));
                                                             fZ = Sub(Mul(fZ,fCos),Mul(fX,fSin));
                                                             fX = pTempX;
                                                             return *this; 
                                                            }

    __SageInline Point3DfSimd8 & selfRotateY(__m256 fAngle) { auto fCos = Cos(fAngle); auto fSin = Sin(fAngle); 
                                                             auto pTempX =  Add(Mul(fZ,fSin), Mul(fX,fCos));
                                                             fZ = Sub(Mul(fZ,fCos),Mul(fX,fSin));
                                                             fX = pTempX;
                                                             return *this; 
                                                            }


   __SageInline Point3DfSimd8 RotateZ(nType fAngle) const { auto fCos = Vecf(cosf(fAngle)); auto fSin = Vecf(sinf(fAngle)); 
                                                            return { Sub(Mul(fX,fCos),Mul(fY,fSin)), Add(Mul(fX,fSin),Mul(fY,fCos)), fZ };}

   __SageInline Point3DfSimd8 RotateZ(__m256 fAngle) const { auto fCos = Cos(fAngle); auto fSin = Sin(fAngle); 
                                                            return { Sub(Mul(fX,fCos),Mul(fY,fSin)), Add(Mul(fX,fSin),Mul(fY,fCos)), fZ };}

  __SageInline Point3DfSimd8 & selfRotateZ(nType fAngle) { auto fCos = Vecf(cosf(fAngle)); auto fSin = Vecf(sinf(fAngle)); 
                                                                 auto pTempX = Sub(Mul(fX,fCos),Mul(fY,fSin));
                                                                 fY = Add(Mul(fX,fSin),Mul(fY,fCos));
                                                                 fX = pTempX;
                                                                 return *this;
                                                               }
  
  __SageInline Point3DfSimd8 & selfRotateZ(__m256 fAngle)  { auto fCos = Cos(fAngle); auto fSin = Sin(fAngle); 
                                                                  auto pTempX = Sub(Mul(fX,fCos),Mul(fY,fSin));
                                                                  fY = Add(Mul(fX,fSin),Mul(fY,fCos));
                                                                  fX = pTempX;
                                                                  return *this;
                                                                }
 


  __SageInline Point3DfSimd8 operator - () const { return { Xor(fX,Vecf(-0.0)), Xor(fY,Vecf(-0.0)), Xor(fZ,Vecf(-0.0)) }; }

};

struct Point3DfSimd16 : public Simd512
{

public:
	__m512 fX;
	__m512 fY;
	__m512 fZ;
private:
    using nType = decltype(fX.m512_f32[0]);                      // i.e. native type; float, double, etc. 

    __forceinline __m512 nVec(nType f) const { return Vecf(f); };     // i.e. native vector
public:
    __SageInline Point3DfSimd16(const __m512 & _x,const __m512 & _y,__m512 _z)
    {
        fX = _x; fY = _y; fZ = _z; 
    }
    __SageInline Point3DfSimd16(Point3DfSimd16 && p2) noexcept                 { fX = p2.fX; fY = p2.fY; fZ = p2.fZ; }
    __SageInline Point3DfSimd16(Point3DfSimd16 & p2) noexcept                  { fX = p2.fX; fY = p2.fY; fZ = p2.fZ; }
    __SageInline Point3DfSimd16 & operator =(Point3DfSimd16 && p2) noexcept    { fX = p2.fX, fY = p2.fY; fZ = p2.fZ; return * this; }

	__SageInline Point3DfSimd16 operator + (Point3DfSimd16 const &obj) const { return { Add(fX,obj.fX), Add(fY,obj.fY), Add(fZ,obj.fZ) }; }
	__SageInline Point3DfSimd16 operator - (Point3DfSimd16 const &obj) const { return { Sub(fX,obj.fX), Sub(fY,obj.fY), Sub(fZ,obj.fZ) }; }
	__SageInline Point3DfSimd16 operator * (Point3DfSimd16 const &obj) const { return { Mul(fX,obj.fX), Mul(fY,obj.fY), Mul(fZ,obj.fZ) }; }
	__SageInline Point3DfSimd16 operator / (Point3DfSimd16 const &obj) const { return { Div(fX,obj.fX), Div(fY,obj.fY), Div(fZ,obj.fZ) }; }

    __SageInline Point3DfSimd16 & operator += (Point3DfSimd16 const &obj) { fX = Add(fX,obj.fX); fY = Add(fY,obj.fY); fZ = Add(fZ,obj.fZ); return *this; }
	__SageInline Point3DfSimd16 & operator -= (Point3DfSimd16 const &obj) { fX = Sub(fX,obj.fX); fY = Sub(fY,obj.fY); fZ = Sub(fZ,obj.fZ); return *this; }
	__SageInline Point3DfSimd16 & operator *= (Point3DfSimd16 const &obj) { fX = Mul(fX,obj.fX); fY = Mul(fY,obj.fY); fZ = Mul(fZ,obj.fZ); return *this; }
	__SageInline Point3DfSimd16 & operator /= (Point3DfSimd16 const &obj) { fX = Div(fX,obj.fX); fY = Div(fY,obj.fY); fZ = Div(fZ,obj.fZ); return *this; }

    Point3DfSimd16() {}

    // Dot product, i.e. a | b

    __SageInline __m512 operator | (Point3DfSimd16 const &obj) const { return Add(Add(Mul(fX,obj.fX), Mul(fY,obj.fY)), Mul(fZ,obj.fZ)); }

    // Cross Product, i.e. a ^ b

	__SageInline Point3DfSimd16 operator ^ (Point3DfSimd16 &obj) 
    { 
        Point3DfSimd16 pTemp; 
        pTemp.fX = Sub(Mul(fY,obj.fZ),Mul(fZ,obj.fY)); 
        pTemp.fY = Sub(Mul(fZ,obj.fX),Mul(fX,obj.fZ)); 
        pTemp.fZ = Sub(Mul(fX,obj.fY),Mul(fY,obj.fX)); 
        return pTemp; 
    }	

	__SageInline Point3DfSimd16 operator + (nType fValue) const { auto vTemp = nVec(fValue); return { Add(fX,vTemp), Add(fY,vTemp), Add(fZ,vTemp) }; }
	__SageInline Point3DfSimd16 operator - (nType fValue) const { auto vTemp = nVec(fValue); return { Sub(fX,vTemp), Sub(fY,vTemp), Sub(fZ,vTemp) }; }
	__SageInline Point3DfSimd16 operator * (nType fValue) const { auto vTemp = nVec(fValue); return { Mul(fX,vTemp), Mul(fY,vTemp), Mul(fZ,vTemp) }; }
	__SageInline Point3DfSimd16 operator / (nType fValue) const { auto vTemp = nVec(fValue); return { Div(fX,vTemp), Div(fY,vTemp), Div(fZ,vTemp) }; }

	__SageInline Point3DfSimd16 & operator += (nType fValue) { auto vTemp = nVec(fValue); fX = Add(fX,vTemp); fY = Add(fY,vTemp); fZ = Add(fZ,vTemp); return *this; }
	__SageInline Point3DfSimd16 & operator -= (nType fValue) { auto vTemp = nVec(fValue); fX = Sub(fX,vTemp); fY = Sub(fY,vTemp); fZ = Sub(fZ,vTemp); return *this; }
	__SageInline Point3DfSimd16 & operator *= (nType fValue) { auto vTemp = nVec(fValue); fX = Mul(fX,vTemp); fY = Mul(fY,vTemp); fZ = Mul(fZ,vTemp); return *this; }
	__SageInline Point3DfSimd16 & operator /= (nType fValue) { auto vTemp = nVec(fValue); fX = Div(fX,vTemp); fY = Div(fY,vTemp); fZ = Div(fZ,vTemp); return *this; }

	__SageInline Point3DfSimd16 operator + (Point3DfSimd16 & obj) const {  return { Add(fX,obj.fX), Add(fY,obj.fY), Add(fZ,obj.fZ) }; }
	__SageInline Point3DfSimd16 operator - (Point3DfSimd16 & obj) const {  return { Sub(fX,obj.fX), Sub(fY,obj.fY), Sub(fZ,obj.fZ) }; }
	__SageInline Point3DfSimd16 operator * (Point3DfSimd16 & obj) const {  return { Mul(fX,obj.fX), Mul(fY,obj.fY), Mul(fZ,obj.fZ) }; }
	__SageInline Point3DfSimd16 operator / (Point3DfSimd16 & obj) const {  return { Div(fX,obj.fX), Div(fY,obj.fY), Div(fZ,obj.fZ) }; }

	__SageInline Point3DfSimd16 & operator += (Point3DfSimd16 & obj)  { fX = Add(fX,obj.fX); fY = Add(fY,obj.fY); fZ = Add(fZ,obj.fZ); return *this; }
	__SageInline Point3DfSimd16 & operator -= (Point3DfSimd16 & obj)  { fX = Sub(fX,obj.fX); fY = Sub(fY,obj.fY); fZ = Sub(fZ,obj.fZ); return *this; }
	__SageInline Point3DfSimd16 & operator *= (Point3DfSimd16 & obj)  { fX = Mul(fX,obj.fX); fY = Mul(fY,obj.fY); fZ = Mul(fZ,obj.fZ); return *this; }
	__SageInline Point3DfSimd16 & operator /= (Point3DfSimd16 & obj)  { fX = Div(fX,obj.fX); fY = Div(fY,obj.fY); fZ = Div(fZ,obj.fZ); return *this; }

	__SageInline Point3DfSimd16 operator + (__m512 & mValue) const {  return { Add(fX,mValue), Add(fY,mValue), Add(fZ,mValue) }; }
	__SageInline Point3DfSimd16 operator - (__m512 & mValue) const {  return { Sub(fX,mValue), Sub(fY,mValue), Sub(fZ,mValue) }; }
	__SageInline Point3DfSimd16 operator * (__m512 & mValue) const {  return { Mul(fX,mValue), Mul(fY,mValue), Mul(fZ,mValue) }; }
	__SageInline Point3DfSimd16 operator / (__m512 & mValue) const {  return { Div(fX,mValue), Div(fY,mValue), Div(fZ,mValue) }; }

	__SageInline Point3DfSimd16 & operator += (__m512 & mValue)  { fX = Add(fX,mValue); fY = Add(fY,mValue); fZ = Add(fZ,mValue); return *this; }
	__SageInline Point3DfSimd16 & operator -= (__m512 & mValue)  { fX = Sub(fX,mValue); fY = Sub(fY,mValue); fZ = Sub(fZ,mValue); return *this; }
	__SageInline Point3DfSimd16 & operator *= (__m512 & mValue)  { fX = Mul(fX,mValue); fY = Mul(fY,mValue); fZ = Mul(fZ,mValue); return *this; }
	__SageInline Point3DfSimd16 & operator /= (__m512 & mValue)  { fX = Div(fX,mValue); fY = Div(fY,mValue); fZ = Div(fZ,mValue); return *this; }

	__SageInline Point3DfSimd16 operator + (Point3Df_t & pValue) const {  return { Add(fX,nVec(pValue.fX)), Add(fY,nVec(pValue.fY)), Add(fZ,nVec(pValue.fZ)) }; }
	__SageInline Point3DfSimd16 operator - (Point3Df_t & pValue) const {  return { Sub(fX,nVec(pValue.fX)), Sub(fY,nVec(pValue.fY)), Sub(fZ,nVec(pValue.fZ)) }; }
	__SageInline Point3DfSimd16 operator * (Point3Df_t & pValue) const {  return { Mul(fX,nVec(pValue.fX)), Mul(fY,nVec(pValue.fY)), Mul(fZ,nVec(pValue.fZ)) }; }
	__SageInline Point3DfSimd16 operator / (Point3Df_t & pValue) const {  return { Div(fX,nVec(pValue.fX)), Div(fY,nVec(pValue.fY)), Div(fZ,nVec(pValue.fZ)) }; }

	__SageInline Point3DfSimd16 & operator += (Point3Df_t & pValue)  { fX = Add(fX,nVec(pValue.fX)); fY = Add(fY,nVec(pValue.fY)); fZ = Add(fZ,nVec(pValue.fZ)); return *this; }
	__SageInline Point3DfSimd16 & operator -= (Point3Df_t & pValue)  { fX = Sub(fX,nVec(pValue.fX)); fY = Sub(fY,nVec(pValue.fY)); fZ = Sub(fZ,nVec(pValue.fZ)); return *this; }
	__SageInline Point3DfSimd16 & operator *= (Point3Df_t & pValue)  { fX = Mul(fX,nVec(pValue.fX)); fY = Mul(fY,nVec(pValue.fY)); fZ = Mul(fZ,nVec(pValue.fZ)); return *this; }
	__SageInline Point3DfSimd16 & operator /= (Point3Df_t & pValue)  { fX = Div(fX,nVec(pValue.fX)); fY = Div(fY,nVec(pValue.fY)); fZ = Div(fZ,nVec(pValue.fZ)); return *this; }


	__SageInline __m512 Mag() const { return Sqrt(Add(Add(Mul(fX,fX),Mul(fY,fY)),Mul(fZ,fZ))); }		// Magnitude of vector // Note: AVX512 has an instruction for this
	__SageInline __m512 MagSq() const { return Add(Add(Mul(fX,fX),Mul(fY,fY)),Mul(fZ,fZ)); 	 }		// Magnitude of vector ^2


    __SageInline Point3DfSimd16 Normalize() const
    { 
        auto fMag = Mag();  // We'll just assume the risk of divide-by-zero for now; later we an convert this to a masked operation, at least for 
                            // SSE/AVX platforms that support AVX512-based masked instructions

        return { Div(fX,fMag), Div(fY,fMag), Div(fZ,fMag) };
    }

    __SageInline Point3DfSimd16 & selfNormalize() 
    { 
        auto fMag = Mag();  // We'll just assume the risk of divide-by-zero for now; later we an convert this to a masked operation, at least for 
                            // SSE/AVX platforms that support AVX512-based masked instructions

        fX = Div(fX,fMag); fY = Div(fY,fMag); fZ = Div(fZ,fMag);
        return (*this);
    }
    
    __SageInline Point3DfSimd16 RotateX(nType fAngle) const { auto fCos = Vecf(cosf(fAngle)); auto fSin = Vecf(sinf(fAngle)); 
                                                             return { fX, Sub(Mul(fY,fCos), Mul(fX,fSin)), Add(Mul(fY,fSin), Mul(fZ,fCos)) }; }	    // Rotate point around X-Axis

    __SageInline Point3DfSimd16 RotateX(__m512 fAngle) const { auto fCos = Cos(fAngle); auto fSin = Sin(fAngle); 
                                                            return { fX, Sub(Mul(fY,fCos), Mul(fX,fSin)), Add(Mul(fY,fSin), Mul(fZ,fCos)) }; }	    // Rotate point around X-Axis

    __SageInline Point3DfSimd16 & selfRotateX(__m512 fAngle) { auto fCos = Cos(fAngle); auto fSin = Sin(fAngle); 
                                                              auto pTempY = Sub(Mul(fY,fCos), Mul(fX,fSin));
                                                              fZ = Add(Mul(fY,fSin), Mul(fZ,fCos));
                                                              fY = pTempY;
                                                              return *this; 
                                                             }

    __SageInline Point3DfSimd16 & selfRotateX(nType fAngle) {  auto fCos = Vecf(cosf(fAngle)); auto fSin = Vecf(sinf(fAngle)); 
                                                              auto pTempY = Sub(Mul(fY,fCos), Mul(fX,fSin));
                                                              fZ = Add(Mul(fY,fSin), Mul(fZ,fCos));
                                                              fY = pTempY;
                                                              return *this; 
                                                            }

    __SageInline Point3DfSimd16 RotateY(nType fAngle) const { auto fCos = Vecf(cosf(fAngle)); auto fSin = Vecf(sinf(fAngle)); 
                                                             return {  Add(Mul(fZ,fSin), Mul(fX,fCos)), fY, Sub(Mul(fZ,fCos),Mul(fX,fSin)) }; }	    // Rotate point around Y-Axis


    __SageInline Point3DfSimd16 RotateY(__m512 fAngle) const { auto fCos = Cos(fAngle); auto fSin = Sin(fAngle); 
                                                             return {  Add(Mul(fZ,fSin), Mul(fX,fCos)), fY, Sub(Mul(fZ,fCos),Mul(fX,fSin)) }; }	    // Rotate point around Y-Axis


    __SageInline Point3DfSimd16 & selfRotateY(nType fAngle) { auto fCos = Vecf(cosf(fAngle)); auto fSin = Vecf(sinf(fAngle)); 
                                                             auto pTempX =  Add(Mul(fZ,fSin), Mul(fX,fCos));
                                                             fZ = Sub(Mul(fZ,fCos),Mul(fX,fSin));
                                                             fX = pTempX;
                                                             return *this; 
                                                            }

    __SageInline Point3DfSimd16 & selfRotateY(__m512 fAngle) { auto fCos = Cos(fAngle); auto fSin = Sin(fAngle); 
                                                             auto pTempX =  Add(Mul(fZ,fSin), Mul(fX,fCos));
                                                             fZ = Sub(Mul(fZ,fCos),Mul(fX,fSin));
                                                             fX = pTempX;
                                                             return *this; 
                                                            }


   __SageInline Point3DfSimd16 RotateZ(nType fAngle) const { auto fCos = Vecf(cosf(fAngle)); auto fSin = Vecf(sinf(fAngle)); 
                                                            return { Sub(Mul(fX,fCos),Mul(fY,fSin)), Add(Mul(fX,fSin),Mul(fY,fCos)), fZ };}

   __SageInline Point3DfSimd16 RotateZ(__m512 fAngle) const { auto fCos = Cos(fAngle); auto fSin = Sin(fAngle); 
                                                            return { Sub(Mul(fX,fCos),Mul(fY,fSin)), Add(Mul(fX,fSin),Mul(fY,fCos)), fZ };}

  __SageInline Point3DfSimd16 & selfRotateZ(nType fAngle) { auto fCos = Vecf(cosf(fAngle)); auto fSin = Vecf(sinf(fAngle)); 
                                                                 auto pTempX = Sub(Mul(fX,fCos),Mul(fY,fSin));
                                                                 fY = Add(Mul(fX,fSin),Mul(fY,fCos));
                                                                 fX = pTempX;
                                                                 return *this;
                                                               }
  
  __SageInline Point3DfSimd16 & selfRotateZ(__m512 fAngle)  { auto fCos = Cos(fAngle); auto fSin = Sin(fAngle); 
                                                                  auto pTempX = Sub(Mul(fX,fCos),Mul(fY,fSin));
                                                                  fY = Add(Mul(fX,fSin),Mul(fY,fCos));
                                                                  fX = pTempX;
                                                                  return *this;
                                                                }
 


  __SageInline Point3DfSimd16 operator - () const { return { Xor(fX,Vecf(-0.0)), Xor(fY,Vecf(-0.0)), Xor(fZ,Vecf(-0.0)) }; }

};
}; // namespace Sage
#endif		// _Point3D_h_