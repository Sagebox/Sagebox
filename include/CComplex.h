// This file copyright(c) 2021 Rob Nelson, All Rights Reserved.    E-mail rob@projectsagebox.com for more information.
//

// --------------------------------------------------------------------------------------------------------------
// PRELIMINARY VERSION.  This file has been brought into the Sagebox project realom the original sources and has 
// not yet been commented for Sagebox, or properly formatted (I used tabs and am converting to spaces).
// --------------------------------------------------------------------------------------------------------------
//
// ****** UNDER CONSTRUCTION ******
//
// This file is still under construction and may not yet include specifics, awaiting proper testing and integration into Sagebox.
//

#pragma once

#if !defined(_CComplex_H_)
#define _CComplex_H_
namespace Sage
{
class CComplex
{
public:
	double real;
	double imag;

	__forceinline CComplex operator * (const CComplex & p2) { return { real*p2.real - imag*p2.imag, real*p2.imag + imag*p2.real }; };
	__forceinline CComplex & operator *= (const CComplex & p2) {  double fTemp = real*p2.real - imag*p2.imag; imag = real*p2.imag + imag*p2.real; real = fTemp; return *this; };
	__forceinline CComplex operator * (double fVal) { return { real*fVal, imag*fVal }; };
	__forceinline CComplex & operator *= (double fVal) {  real *= fVal; imag *= fVal; return *this; };
	__forceinline CComplex operator / (double fVal) { return { real/fVal, imag/fVal }; };
	__forceinline CComplex & operator /= (double fVal) { real/=fVal; imag/=fVal; return *this; };
	__forceinline CComplex operator - (double fVal) { return { real - fVal, imag }; };
    __forceinline CComplex & operator -= (double fVal) { real -= fVal; return *this; }
	__forceinline CComplex operator + (double fVal) { return { real + fVal, imag }; };
	__forceinline CComplex & operator += (double fVal) { real += fVal; return *this; };
	__forceinline CComplex operator + (const CComplex & p2) { return { real + p2.real, imag + p2.imag }; }
	__forceinline CComplex & operator += (const CComplex & p2) { real += p2.real; imag += p2.imag; return *this; }
	__forceinline CComplex operator - (const CComplex & p2) { return { real - p2.real, imag - p2.imag }; }
	__forceinline CComplex & operator -= (const CComplex & p2) { real -= p2.real; imag -= p2.imag; return *this; }
	__forceinline double abs() const { return sqrt(real*real + imag*imag); };
	__forceinline double absSq() const { return real*real + imag*imag; };
	__forceinline CComplex sq() const { return { real*real - imag*imag,  2*real*imag };};

	__forceinline CComplex operator / (const CComplex & p2)
	{
		double fDiv = p2.real * p2.real + p2.imag*p2.imag;
		if (!fDiv) return { 0,0 };
		return { (real*p2.real + imag*p2.imag)/fDiv, (imag*p2.real - real*p2.imag)/fDiv };
	}
	__forceinline CComplex & operator /= (const CComplex & p2)
	{
		double fDiv = p2.real * p2.real + p2.imag*p2.imag;
        if (!fDiv) { real = imag = 0; return *this; }
        double fTemp = (real*p2.real + imag*p2.imag)/fDiv;
        imag = (imag*p2.real - real*p2.imag)/fDiv;
        real = fTemp;
        return *this;
	}
	__forceinline CComplex & Normalize() { double fValue = abs(); if (!fValue) { real = imag = 0.0; } else *this = *this/fValue; return *this; }
    CComplex() =  default; 
    CComplex(double real,double imag) { this->real = real; this->imag = imag; } 
    CComplex(float real,float imag) { this->real = (double) real; this->imag = (double) imag; } 
    CComplex(int real,int imag) { this->real = (double) real; this->imag = (double) imag; } 
    friend auto operator<<(std::ostream& os, CComplex const& m) -> std::ostream& { return os << "[ " << m.real << ", " << m.imag << " ]"; }

};


class CComplexf
{
public:
	float real;
	float imag;

	__forceinline CComplexf operator * (const CComplexf & p2) { return { real*p2.real - imag*p2.imag, real*p2.imag + imag*p2.real }; };
	__forceinline CComplexf & operator *= (const CComplexf & p2) {  float fTemp = real*p2.real - imag*p2.imag; imag = real*p2.imag + imag*p2.real; real = fTemp; return *this; };
	__forceinline CComplexf operator * (float fVal) { return { real*fVal, imag*fVal }; };
	__forceinline CComplexf & operator *= (float fVal) {  real *= fVal; imag *= fVal; return *this; };
	__forceinline CComplexf operator / (float fVal) { return { real/fVal, imag/fVal }; };
	__forceinline CComplexf & operator /= (float fVal) { real/=fVal; imag/=fVal; return *this; };
	__forceinline CComplexf operator - (float fVal) { return { real - fVal, imag }; };
    __forceinline CComplexf & operator -= (float fVal) { real -= fVal; return *this; }
	__forceinline CComplexf operator + (float fVal) { return { real + fVal, imag }; };
	__forceinline CComplexf & operator += (float fVal) { real += fVal; return *this; };
	__forceinline CComplexf operator + (const CComplexf & p2) { return { real + p2.real, imag + p2.imag }; }
	__forceinline CComplexf & operator += (const CComplexf & p2) { real += p2.real; imag += p2.imag; return *this; }
	__forceinline CComplexf operator - (const CComplexf & p2) { return { real - p2.real, imag - p2.imag }; }
	__forceinline CComplexf & operator -= (const CComplexf & p2) { real -= p2.real; imag -= p2.imag; return *this; }
	__forceinline float abs() const { return sqrt(real*real + imag*imag); };
	__forceinline float absSq() const { return real*real + imag*imag; };
	__forceinline CComplexf sq() const { return { real*real - imag*imag,  2*real*imag };};

	__forceinline CComplexf operator / (const CComplexf & p2)
	{
		float fDiv = p2.real * p2.real + p2.imag*p2.imag;
		if (!fDiv) return { 0,0 };
		return { (real*p2.real + imag*p2.imag)/fDiv, (imag*p2.real - real*p2.imag)/fDiv };
	}
	__forceinline CComplexf & operator /= (const CComplexf & p2)
	{
		float fDiv = p2.real * p2.real + p2.imag*p2.imag;
        if (!fDiv) { real = imag = 0; return *this; }
        float fTemp = (real*p2.real + imag*p2.imag)/fDiv;
        imag = (imag*p2.real - real*p2.imag)/fDiv;
        real = fTemp;
        return *this;
	}
	__forceinline CComplexf & Normalize() { float fValue = abs(); if (!fValue) { real = imag = 0.0f; } else *this = *this/fValue; return *this; }
    CComplexf() =  default; 
    CComplexf(double real,double imag) { this->real = (float) real; this->imag = (float) imag; } 
    CComplexf(float real,float imag) { this->real = (float) real; this->imag = (float) imag; } 
    CComplexf(int real,int imag) { this->real = (float) real; this->imag = (float) imag; } 
    friend auto operator<<(std::ostream& os, CComplexf const& m) -> std::ostream& { return os << "[ " << m.real << ", " << m.imag << " ]"; }

};
}; // namespace Sage
__forceinline float abs(const Sage::CComplexf & num) { return num.abs(); }
__forceinline double abs(const Sage::CComplex & num) { return num.abs(); }


#endif // _CComplex_H_