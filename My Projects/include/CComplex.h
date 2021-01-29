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

#if !defined(_CComplex_H_)
#define _CComplex_H_
namespace Sage
{
struct CComplex
{

	double fR;
	double fI;

	__forceinline CComplex operator * (const CComplex & p2) { return { fR*p2.fR - fI*p2.fI, fR*p2.fI + fI*p2.fR }; };
	__forceinline CComplex & operator *= (const CComplex & p2) {  double fTemp = fR*p2.fR - fI*p2.fI; fI = fR*p2.fI + fI*p2.fR; fR = fTemp; return *this; };
	__forceinline CComplex operator * (double fVal) { return { fR*fVal, fI*fVal }; };
	__forceinline CComplex & operator *= (double fVal) {  fR *= fVal; fI *= fVal; return *this; };
	__forceinline CComplex operator / (double fVal) { return { fR/fVal, fI/fVal }; };
	__forceinline CComplex & operator /= (double fVal) { fR/=fVal; fI/=fVal; return *this; };
	__forceinline CComplex operator - (double fVal) { return { fR - fVal, fI }; };
    __forceinline CComplex & operator -= (double fVal) { fR -= fVal; return *this; }
	__forceinline CComplex operator + (double fVal) { return { fR + fVal, fI }; };
	__forceinline CComplex & operator += (double fVal) { fR += fVal; return *this; };
	__forceinline CComplex operator + (const CComplex & p2) { return { fR + p2.fR, fI + p2.fI }; }
	__forceinline CComplex & operator += (const CComplex & p2) { fR += p2.fR; fI += p2.fI; return *this; }
	__forceinline CComplex operator - (const CComplex & p2) { return { fR - p2.fR, fI - p2.fI }; }
	__forceinline CComplex & operator -= (const CComplex & p2) { fR -= p2.fR; fI -= p2.fI; return *this; }
	__forceinline double abs() { return sqrt(fR*fR + fI*fI); };
	__forceinline double absSq() { return fR*fR + fI*fI; };
	__forceinline CComplex sq() { return { fR*fR - fI*fI,  2*fR*fI };};

	__forceinline CComplex operator / (const CComplex & p2)
	{
		double fDiv = p2.fR * p2.fR + p2.fI*p2.fI;
		if (!fDiv) return { 0,0 };
		return { (fR*p2.fR + fI*p2.fI)/fDiv, (fI*p2.fR - fR*p2.fI)/fDiv };
	}
	__forceinline CComplex & operator /= (const CComplex & p2)
	{
		double fDiv = p2.fR * p2.fR + p2.fI*p2.fI;
        if (!fDiv) { fR = fI = 0; return *this; }
        double fTemp = (fR*p2.fR + fI*p2.fI)/fDiv;
        fI = (fI*p2.fR - fR*p2.fI)/fDiv;
        fR = fTemp;
        return *this;
	}
	__forceinline CComplex & Normalize() { double fValue = abs(); if (!fValue) *this = { 0,0 }; else *this = *this/fValue; return *this; }

};

}; // namespace Sage

#endif // _CComplex_H_