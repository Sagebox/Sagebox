//#pragma once

#if !defined(_CRawBitmap_H_)
#define _CRawBitmap_H_
#include "Sage.h"


class CSageBitmap;
class CFloatBitmap
{
public:
	Sage::FloatBitmap_t fBitmap;
	Sage::FloatBitmap_t & operator *() { return fBitmap; }

	CFloatBitmap(CSageBitmap & cBitmap);
	CFloatBitmap(Sage::RawBitmap_t & stBitmap);
	CFloatBitmap(Sage::FloatBitmap_t & fBitmap) { this->fBitmap = fBitmap; };
	CFloatBitmap() 
	{ 
		fBitmap = {}; 
	}
	[[nodiscard]] Sage::RawBitmap_t ConverttoBitmap() { return fBitmap.ConverttoBitmap(); }
	[[nodiscard]] Sage::RawBitmap_t ConverttoBitmap(Sage::RawBitmap_t & stBitmap) { return fBitmap.ConverttoBitmap(stBitmap); }
	CFloatBitmap & operator = (CSageBitmap stBitmap);

	CFloatBitmap & operator = (Sage::FloatBitmap_t fBitmap) { this->fBitmap.Delete(); this->fBitmap = fBitmap; return *this; }
	CFloatBitmap & operator = (Sage::RawBitmap_t stBitmap) 
	{ 
		fBitmap.Delete(); this->fBitmap = stBitmap.ConverttoFloat(); return *this; 
	}
	bool isValid()
	{
		return (fBitmap.isValid());
	}
	~CFloatBitmap() 
	{ 
		fBitmap.Delete(); 
	};
	SIZE GetSize() { return {fBitmap.iWidth, fBitmap.iHeight };}
	int GetWidth() { return fBitmap.iWidth; }
	int GetHeight() { return fBitmap.iHeight; }
	operator Sage::FloatBitmap_t & () const { return (Sage::FloatBitmap_t &) fBitmap; };

};

class CSageBitmap
{
public:
	Sage::RawBitmap_t stBitmap;

	CSageBitmap(Sage::RawBitmap_t & stBitmap) { this->stBitmap = stBitmap; };
	CSageBitmap() 
	{ 
		stBitmap = {}; 
	}

	CSageBitmap(CFloatBitmap & cBitmap);
	//CSageBitmap & operator = (RawBitmap_t & stBitmap) { this->stBitmap.Delete(); this->stBitmap = stBitmap; return *this; }
	CSageBitmap & operator = (Sage::RawBitmap_t stBitmap) { this->stBitmap.Delete(); this->stBitmap = stBitmap; return *this; }
	CSageBitmap & operator = (CSageBitmap csBitmap) 
	{ this->stBitmap.Delete();
		CopyFrom(csBitmap);  return *this; }
	CSageBitmap & operator = (Sage::FloatBitmap_t fBitmap) 
	{ 
		stBitmap.Delete(); this->stBitmap = fBitmap.ConverttoBitmap(); return *this; 
	}
	CSageBitmap & operator = (CFloatBitmap & fBitmap) 
	{ 
		stBitmap.Delete(); this->stBitmap = fBitmap.ConverttoBitmap(); return *this; 
	}
	Sage::RawBitmap_t & operator *() { return stBitmap; }
	~CSageBitmap() 
	{ 
		stBitmap.Delete(); 
	};
	SIZE GetSize() { return stBitmap.GetSize(); }
	int GetWidth() { return stBitmap.iWidth; }
	int GetHeight() { return stBitmap.iHeight; }
	int GetWidthBytes() { return stBitmap.iWidthBytes; }
	bool ApplyMaskGraphic(Sage::RawBitmap_t & stBackground,Sage::RawBitmap_t & stDest) { return stBitmap.ApplyMaskGraphic(stBackground,stDest); }
	bool ApplyMaskGraphic(CSageBitmap & cBackground,CSageBitmap & cDest) { return stBitmap.ApplyMaskGraphic(*cBackground,*cDest); }
		
	bool ApplyMaskGraphic(POINT pSourceStart, Sage::RawBitmap_t & stDest, POINT pDestStart, SIZE & szSize) { return stBitmap.ApplyMaskGraphic(pSourceStart,stDest,pDestStart,szSize); }
	bool ApplyMaskGraphic(POINT pSourceStart, CSageBitmap & cDest, POINT pDestStart, SIZE & szSize) { return stBitmap.ApplyMaskGraphic(pSourceStart,*cDest,pDestStart,szSize); }
	bool ApplyMaskGraphic(CSageBitmap & cDest, const POINT pDestStart = { 0,0 }) { return stBitmap.ApplyMaskGraphic({ 0,0} ,*cDest,pDestStart,stBitmap.GetSize()); }

	bool ApplyMaskGraphic(Sage::RawBitmap_t & stSource, Sage::RawBitmap_t & stBackground,Sage::RawBitmap_t & stDest) { return stBitmap.ApplyMaskGraphic(stSource,stBackground,stDest); }
	bool ApplyMaskGraphic(CSageBitmap & cSource,CSageBitmap & cBackground,CSageBitmap & cDest) { return stBitmap.ApplyMaskGraphic(*cSource,*cBackground,*cDest); }


	bool ApplyMaskGraphicR(const POINT pSourceStart, Sage::RawBitmap_t & stDest,const POINT pDestStart,const SIZE & szSize) { return stBitmap.ApplyMaskGraphicR(pSourceStart,stDest,pDestStart,szSize); }
	bool ApplyMaskGraphicR(const POINT pSourceStart, CSageBitmap & cDest,const POINT pDestStart, const SIZE & szSize) { return stBitmap.ApplyMaskGraphicR(pSourceStart,*cDest,pDestStart,szSize); }
	bool ApplyMaskGraphicR(CSageBitmap & cDest, const POINT pDestStart) { return stBitmap.ApplyMaskGraphicR({ 0,0} ,*cDest,pDestStart,stBitmap.GetSize()); }
	bool ApplyMaskColor(Sage::RGBColor_t rgbColor	,Sage::RawBitmap_t stMask,POINT pMaskStart, POINT pDestStart, SIZE szSize) { return stBitmap.ApplyMaskColor(rgbColor,stMask,pMaskStart,pDestStart,szSize);	}
	bool ApplyMaskColorR(Sage::RGBColor_t rgbColor,Sage::RawBitmap_t stMask,POINT pMaskStart,POINT pDestStart, SIZE szSize) { return stBitmap.ApplyMaskColorR(rgbColor,stMask,pMaskStart,pDestStart,szSize);	}
	bool ApplyMaskColor(Sage::RGBColor_t rgbColor	,Sage::RawBitmap_t stMask,POINT pDestStart) { return stBitmap.ApplyMaskColor(rgbColor,stMask,pDestStart); }
	bool ApplyMaskColorR(Sage::RGBColor_t rgbColor,Sage::RawBitmap_t stMask,POINT pDestStart) { return stBitmap.ApplyMaskColorR(rgbColor,stMask,pDestStart); }

	bool ApplyMaskColor(Sage::RGBColor_t	 rgbColor,	CSageBitmap & cMask,POINT pMaskStart,	POINT pDestStart, SIZE szSize = { 0,0 }) { return stBitmap.ApplyMaskColor(rgbColor,*cMask,pMaskStart,pDestStart,szSize);	}
	bool ApplyMaskColorR(Sage::RGBColor_t	 rgbColor,	CSageBitmap & cMask,POINT pMaskStart,	POINT pDestStart, SIZE szSize = { 0,0 }) { return stBitmap.ApplyMaskColorR(rgbColor,*cMask,pMaskStart,pDestStart,szSize);	}
	bool ApplyMaskColor(Sage::RGBColor_t	 rgbColor,	CSageBitmap & cMask,POINT pDestStart = { 0,0 }) { return stBitmap.ApplyMaskColor(rgbColor,*cMask,pDestStart); }
	bool ApplyMaskColorR(Sage::RGBColor_t  rgbColor,	CSageBitmap & cMask,POINT pDestStart = { 0,0 }) { return stBitmap.ApplyMaskColorR(rgbColor,*cMask,pDestStart); }


	bool CopyFrom(Sage::RawBitmap_t & stSource, POINT pDestStart = {0,0} , POINT pSourceStart = { 0,0 }, SIZE szSize = {0,0}) { return stBitmap.CopyFrom(stSource,pDestStart,pSourceStart,szSize); }		
	bool CopyFrom(CSageBitmap & cSource, POINT pDestStart = {0,0} , POINT pSourceStart = { 0,0 }, SIZE szSize = {0,0}) { return stBitmap.CopyFrom(*cSource,pDestStart,pSourceStart,szSize); }		
		
	bool Copyto(Sage::RawBitmap_t & stDest, POINT pSourceStart = { 0,0}, POINT pDestStart = { 0,0 }, SIZE szSize = { 0,0 }) { return stBitmap.Copyto(stDest,pSourceStart,pDestStart,szSize); }
	bool Copyto(CSageBitmap & cDest,POINT pSourceStart = { 0,0}, POINT pDestStart = { 0,0 }, SIZE szSize = { 0,0 }) { return Copyto(*cDest,pSourceStart,pDestStart,szSize); }
		
	bool FillColor(Sage::RGBColor_t rgbColor, POINT pStart = { 0,0 }, SIZE szSize = { 0,0 }) { return stBitmap.FillColor(*rgbColor,pStart,szSize); }
	bool FillColor(DWORD dwColor, POINT pStart = { 0,0 }, SIZE szSize = { 0,0 }) { return stBitmap.FillColor(dwColor,pStart,szSize); }

	void Delete(){ stBitmap.Delete(); }
	bool CreateBitmap(int iWidth,int iHeight);

	Sage::RawBitmap_t operator - ()
	{
		Sage::RawBitmap_t x = stBitmap;
		x.iHeight = - x.iHeight;
		return x;
	}


	CSageBitmap(const CSageBitmap &p2)
	{
		stBitmap = {};
		CopyFrom(p2);

	}
	bool isValid()
	{
		return (stBitmap.stMem != nullptr && !stBitmap.isInvalid()); 
	}
	operator unsigned char * () const { return stBitmap.stMem; };
	operator Sage::RawBitmap_t & () const { return (Sage::RawBitmap_t &) stBitmap; };

};
#endif //_CRawBitmap_H_