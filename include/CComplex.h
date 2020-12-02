#pragma once

#if !defined(_CComplex_H_)
#define _CComplex_H_
namespace Sage
{
struct CComplex
{

	double fR;
	double fI;

	//__forceinline CeComplex operator * (const CeComplex & p2) { return ComplexMult(*this,p2); }
	__forceinline CComplex operator * (const CComplex & p2) { return { fR*p2.fR - fI*p2.fI, fR*p2.fI + fI*p2.fR }; };
	__forceinline CComplex operator * (double fVal) { return { fR*fVal, fI*fVal }; };
	__forceinline CComplex operator / (double fVal) { return { fR/fVal, fI/fVal }; };
	__forceinline CComplex operator - (double fVal) { return { fR - fVal, fI }; };
	__forceinline CComplex operator + (double fVal) { return { fR + fVal, fI }; };
	__forceinline CComplex operator + (const CComplex & p2) { return { fR + p2.fR, fI + p2.fI }; }
	__forceinline CComplex operator - (const CComplex & p2) { return { fR - p2.fR, fI - p2.fI }; }
	__forceinline double abs() { return sqrt(fR*fR + fI*fI); };
	__forceinline double absSq() { return fR*fR + fI*fI; };
	__forceinline CComplex sq() { return { fR*fR - fI*fI,  2*fR*fI };};

	__forceinline CComplex operator / (const CComplex & p2)
	{
		double fDiv = p2.fR * p2.fR + p2.fI*p2.fI;
		if (!fDiv) return { 0,0 };
		return { (fR*p2.fR + fI*p2.fI)/fDiv, (fI*p2.fR - fR*p2.fI)/fDiv };
	}
	__forceinline CComplex & Normalize() { double fValue = abs(); if (!fValue) *this = { 0,0 }; else *this = *this/fValue; return *this; }

};

}; // namespace Sage

#endif // _CComplex_H_