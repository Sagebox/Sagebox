//#pragma once

#if !defined(_CRawBitmap_H_)
#define _CRawBitmap_H_
#include "Sage.h"

namespace Sage
{

class CSageBitmap;
using CBitmap = CSageBitmap;
class CFloatBitmap
{
public:
	FloatBitmap_t fBitmap;
	FloatBitmap_t & operator *() { return fBitmap; }

	CFloatBitmap(CSageBitmap & cBitmap);
	CFloatBitmap(RawBitmap_t & stBitmap);
	CFloatBitmap(FloatBitmap_t & fBitmap) { this->fBitmap = fBitmap; };
	CFloatBitmap(int iWidth,int iHeight) { fBitmap = CreateFloatBitmap(iWidth,iHeight); }

	CFloatBitmap() 
	{ 
		fBitmap.Init();
	}
	[[nodiscard]] RawBitmap_t ConverttoBitmap() { return fBitmap.ConverttoBitmap(); }
	[[nodiscard]] RawBitmap_t ConverttoBitmap(RawBitmap_t & stBitmap) { return fBitmap.ConverttoBitmap(stBitmap); }
	CFloatBitmap & operator = (CSageBitmap stBitmap);
	CFloatBitmap & operator = (SIZE szSize) { fBitmap.Delete(); fBitmap = CreateFloatBitmap(szSize.cx,szSize.cy); return * this; };
	CFloatBitmap & operator = (FloatBitmap_t fBitmap) { this->fBitmap.Delete(); this->fBitmap = fBitmap; return *this; }
	CFloatBitmap & operator = (RawBitmap_t stBitmap) 
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
    float * * GetMemPtr() { return fBitmap.f; }
	__forceinline SIZE GetSize() { return {fBitmap.iWidth, fBitmap.iHeight };}
	__forceinline int GetWidth() { return fBitmap.iWidth; }
	__forceinline int GetHeight() { return fBitmap.iHeight; }
	operator FloatBitmap_t & () const { return (FloatBitmap_t &) fBitmap; };

};
class CFloatBitmapM
{
public:
	FloatBitmapM_t fBitmap;
	FloatBitmapM_t & operator *() { return fBitmap; }

//	CFloatBitmapM(CSageBitmap & cBitmap);
//	CFloatBitmapM(RawBitmap_t & stBitmap);
	CFloatBitmapM(int iWidth,int iHeight) { fBitmap = CreateFloatMBitmap(iWidth,iHeight); };
	CFloatBitmapM(const SIZE szSize) { fBitmap = CreateFloatMBitmap(szSize.cx,szSize.cy); };

//	CFloatBitmapM(FloatBitmapM_t & fBitmap) { this->fBitmap = fBitmap; };
	CFloatBitmapM(FloatBitmapM_t fBitmap) { this->fBitmap = fBitmap; };
	CFloatBitmapM() 
	{ 
		fBitmap = {}; 
	}
	bool Invert() { return fBitmap.Invert(); };
	bool Multiply(CFloatBitmapM & cSource,POINT pDestStart = { 0,0 }, POINT pSourceStart = { 0,0 }, SIZE szSize = { 0,0 })
	{
		return fBitmap.Multiply(*cSource,pDestStart,pSourceStart,szSize);
	}
	[[nodiscard]] RawBitmap_t ConverttoBitmap() { return fBitmap.ConverttoBitmap(); }
	[[nodiscard]] RawBitmap_t ConverttoBitmap(RawBitmap_t & stBitmap) { return fBitmap.ConverttoBitmap(stBitmap); }
//	CFloatBitmapM & operator = (CSageBitmap stBitmap);

	CFloatBitmapM & operator = (FloatBitmapM_t fBitmap) { this->fBitmap.Delete(); this->fBitmap = fBitmap; return *this; }
	CFloatBitmapM & operator = (SIZE szSize) { this->fBitmap.Delete(); this->fBitmap = CreateFloatMBitmap(szSize.cx,szSize.cy);; return *this; }
//	CFloatBitmapM & operator = (RawBitmap_t stBitmap) 
//	{ 
//		fBitmap.Delete(); this->fBitmap = stBitmap.ConverttoFloat(); return *this; 
//	}
	bool isValid()
	{
		return (fBitmap.isValid());
	}
	~CFloatBitmapM() 
	{ 
		fBitmap.Delete(); 
	};
	void Delete() { fBitmap.Delete(); };

    float * GetMem() { return fBitmap.fPixels; }
	__forceinline SIZE GetSize() { return {fBitmap.iWidth, fBitmap.iHeight };}
	__forceinline int GetWidth() { return fBitmap.iWidth; }
	__forceinline int GetHeight() { return fBitmap.iHeight; }
	operator FloatBitmap_t & () const { return (FloatBitmap_t &) fBitmap; };

};

class CSageBitmap
{
public:
	RawBitmap_t stBitmap;
	int iCurrentLine = 0;

	__forceinline  unsigned char *  SetCurrentLine(int iLine) { iCurrentLine = iLine >= stBitmap.iHeight ? stBitmap.iHeight-1 : iLine < 0 ? iLine : iLine;
		return stBitmap.stMem ? stBitmap.stMem + iCurrentLine*stBitmap.iWidthBytes : nullptr; }

	__forceinline  unsigned char * NextLine()
	{
		if (!stBitmap.stMem) return nullptr;

		if (iCurrentLine < -1 || iCurrentLine >= stBitmap.iHeight-1) iCurrentLine = stBitmap.iHeight-2;
		iCurrentLine++;
		return stBitmap.stMem ? stBitmap.stMem + iCurrentLine * stBitmap.iWidthBytes : nullptr;
	}

	__forceinline  void SetPixel(int iX,int iY,RGBColor_t rgbColor) 
	{ 
		if (!stBitmap.stMem || iX < 0 || iX >= stBitmap.iWidth || iY < 0 || iY >= stBitmap.iHeight) return;
		unsigned char * sTemp = stBitmap.stMem + iY*stBitmap.iWidthBytes + iX*3;
		*sTemp++ = rgbColor.iBlue;
		*sTemp++ = rgbColor.iGreen;
		*sTemp++ = rgbColor.iRed;
	}			
	__forceinline  void SetPixel(int iX,RGBColor_t rgbColor) 
	{ 
		if (!stBitmap.stMem || iX < 0 || iX >= stBitmap.iWidth || iCurrentLine < 0 || iCurrentLine >= stBitmap.iHeight) return;
		unsigned char * sTemp = stBitmap.stMem + iCurrentLine*stBitmap.iWidthBytes + iX*3;
		*sTemp++ = rgbColor.iBlue;
		*sTemp++ = rgbColor.iGreen;
		*sTemp++ = rgbColor.iRed;
	}			
	__forceinline  void SetFastPixel(int iX,int iY,RGBColor_t rgbColor) 
	{ 
		unsigned char * sTemp = stBitmap.stMem + iY*stBitmap.iWidthBytes + iX*3;
		*sTemp++ = rgbColor.iBlue;
		*sTemp++ = rgbColor.iGreen;
		*sTemp++ = rgbColor.iRed;
	}	
	__forceinline  void SetFastPixel(int iX,RGBColor_t rgbColor) 
	{ 
		unsigned char * sTemp = stBitmap.stMem + iCurrentLine*stBitmap.iWidthBytes + iX*3;
		*sTemp++ = rgbColor.iBlue;
		*sTemp++ = rgbColor.iGreen;
		*sTemp++ = rgbColor.iRed;
	}	

	__forceinline  RGBColor_t GetPixel(int iX,int iY) 
	{ 
		if (!stBitmap.stMem || iX < 0 || iX >= stBitmap.iWidth || iY < 0 || iY >= stBitmap.iHeight) return { 0,0,0};
		unsigned char * sTemp = stBitmap.stMem + iY*stBitmap.iWidthBytes + iX*3;
		return { (int) sTemp[2], (int) sTemp[1], (int) sTemp[0] };
	}			

	__forceinline  RGBColor_t GetFastPixel(int iX,int iY,RGBColor_t rgbColor) 
	{ 
		unsigned char * sTemp = stBitmap.stMem + iY*stBitmap.iWidthBytes + iX*3;
		return { (int) sTemp[2], (int) sTemp[1], (int) sTemp[0] };
	}			

//	CSageBitmap(RawBitmap_t & stBitmap) { this->stBitmap = stBitmap; };
	CSageBitmap(RawBitmap_t stBitmap) { this->stBitmap = stBitmap; };
	CSageBitmap(int iWidth,int iHeight) { this->stBitmap = Sage::CreateBitmap(iWidth,iHeight); }
	CSageBitmap(SIZE szSize) { this->stBitmap = Sage::CreateBitmap(szSize.cx,szSize.cy); }
	CSageBitmap() 
	{ 
		stBitmap = {}; 
	}

	CSageBitmap(CFloatBitmap & cBitmap);
	CSageBitmap(CFloatBitmapM & cBitmap);
//	CSageBitmap & operator = (RawBitmap_t & stBitmap) { this->stBitmap.Delete(); this->stBitmap = stBitmap; return *this; }
	CSageBitmap & operator = (RawBitmap_t stBitmap) { this->stBitmap.Delete(); this->stBitmap = stBitmap; return *this; }
	CSageBitmap & operator = (SIZE sz) { this->stBitmap.Delete(); this->stBitmap = Sage::CreateBitmap((int) sz.cx,(int) sz.cy); return *this; }
	CSageBitmap & operator = (CSageBitmap csBitmap) 
	{ this->stBitmap.Delete();
		CopyFrom(csBitmap);  return *this; }
//	CSageBitmap & operator = (FloatBitmap_t fBitmap) 
//	{ 
//		stBitmap.Delete(); this->stBitmap = fBitmap.ConverttoBitmap(); return *this; 
//	}
	CSageBitmap & operator = (CFloatBitmap & fBitmap) 
	{ 
		stBitmap.Delete(); this->stBitmap = fBitmap.ConverttoBitmap(); return *this; 
	}

//	CSageBitmap & operator = (FloatBitmapM_t fBitmap) 
//	{ 
//		stBitmap.Delete(); this->stBitmap = fBitmap.ConverttoBitmap(); return *this; 
//	}
	CSageBitmap & operator = (FloatBitmapM_t & fBitmap) 
	{ 
		stBitmap.Delete(); this->stBitmap = fBitmap.ConverttoBitmap(); return *this; 
	}
	CSageBitmap & operator = (CFloatBitmapM & fBitmap) 
	{ 
		stBitmap.Delete(); this->stBitmap = fBitmap.ConverttoBitmap(); return *this; 
	}

	RawBitmap_t & operator *() { return stBitmap; }
	~CSageBitmap() 
	{ 
//		printf("Deleting bitmap.. size = %d,%d\n",stBitmap.iWidth,stBitmap.iHeight);	// Debug
		stBitmap.Delete(); 
	};
	__forceinline SIZE GetSize() { return { stBitmap.iWidth, stBitmap.iHeight }; }
	__forceinline int GetWidth() { return stBitmap.iWidth; }
	__forceinline int GetHeight() { return stBitmap.iHeight; }
	__forceinline int GetWidthBytes() { return stBitmap.iWidthBytes; }
	__forceinline int GetOverhang() { return stBitmap.iOverHang; }

    unsigned char * GetMem() { return stBitmap.stMem; }
	bool ReverseBitmap() { return stBitmap.ReverseBitmap(); }
	bool ApplyMaskGraphic(RawBitmap_t & stBackground,RawBitmap_t & stDest) { return stBitmap.ApplyMaskGraphic(stBackground,stDest); }
	bool ApplyMaskGraphic(CSageBitmap & cBackground,CSageBitmap & cDest) { return stBitmap.ApplyMaskGraphic(*cBackground,*cDest); }
		
	bool ApplyMaskGraphic(POINT pSourceStart, RawBitmap_t & stDest, POINT pDestStart, SIZE & szSize) { return stBitmap.ApplyMaskGraphic(pSourceStart,stDest,pDestStart,szSize); }
	bool ApplyMaskGraphic(POINT pSourceStart, CSageBitmap & cDest, POINT pDestStart, SIZE & szSize) { return stBitmap.ApplyMaskGraphic(pSourceStart,*cDest,pDestStart,szSize); }
	bool ApplyMaskGraphic(CSageBitmap & cDest, const POINT pDestStart = { 0,0 }) { return stBitmap.ApplyMaskGraphic({ 0,0} ,*cDest,pDestStart,stBitmap.GetSize()); }

	bool ApplyMaskGraphic(RawBitmap_t & stSource, RawBitmap_t & stBackground,RawBitmap_t & stDest) { return stBitmap.ApplyMaskGraphic(stSource,stBackground,stDest); }
	bool ApplyMaskGraphic(CSageBitmap & cSource,CSageBitmap & cBackground,CSageBitmap & cDest) { return stBitmap.ApplyMaskGraphic(*cSource,*cBackground,*cDest); }


	bool ApplyMaskGraphicR(const POINT pSourceStart, RawBitmap_t & stDest,const POINT pDestStart,const SIZE & szSize) { return stBitmap.ApplyMaskGraphicR(pSourceStart,stDest,pDestStart,szSize); }
	bool ApplyMaskGraphicR(const POINT pSourceStart, CSageBitmap & cDest,const POINT pDestStart, const SIZE & szSize) { return stBitmap.ApplyMaskGraphicR(pSourceStart,*cDest,pDestStart,szSize); }
	bool ApplyMaskGraphicR(CSageBitmap & cDest, const POINT pDestStart) { return stBitmap.ApplyMaskGraphicR({ 0,0} ,*cDest,pDestStart,stBitmap.GetSize()); }
	bool ApplyMaskColor(RGBColor_t rgbColor	,RawBitmap_t stMask,POINT pMaskStart, POINT pDestStart, SIZE szSize) { return stBitmap.ApplyMaskColor(rgbColor,stMask,pMaskStart,pDestStart,szSize);	}
	bool ApplyMaskColorR(RGBColor_t rgbColor,RawBitmap_t stMask,POINT pMaskStart,POINT pDestStart, SIZE szSize) { return stBitmap.ApplyMaskColorR(rgbColor,stMask,pMaskStart,pDestStart,szSize);	}
	bool ApplyMaskColor(RGBColor_t rgbColor	,RawBitmap_t stMask,POINT pDestStart) { return stBitmap.ApplyMaskColor(rgbColor,stMask,pDestStart); }
	bool ApplyMaskColorR(RGBColor_t rgbColor,RawBitmap_t stMask,POINT pDestStart) { return stBitmap.ApplyMaskColorR(rgbColor,stMask,pDestStart); }

	bool ApplyMaskColor(RGBColor_t	 rgbColor,	CSageBitmap & cMask,POINT pMaskStart,	POINT pDestStart, SIZE szSize = { 0,0 }) { return stBitmap.ApplyMaskColor(rgbColor,*cMask,pMaskStart,pDestStart,szSize);	}
	bool ApplyMaskColorR(RGBColor_t	 rgbColor,	CSageBitmap & cMask,POINT pMaskStart,	POINT pDestStart, SIZE szSize = { 0,0 }) { return stBitmap.ApplyMaskColorR(rgbColor,*cMask,pMaskStart,pDestStart,szSize);	}
	bool ApplyMaskColor(RGBColor_t	 rgbColor,	CSageBitmap & cMask,POINT pDestStart = { 0,0 }) { return stBitmap.ApplyMaskColor(rgbColor,*cMask,pDestStart); }
	bool ApplyMaskColorR(RGBColor_t  rgbColor,	CSageBitmap & cMask,POINT pDestStart = { 0,0 }) { return stBitmap.ApplyMaskColorR(rgbColor,*cMask,pDestStart); }


	bool CopyFrom(RawBitmap_t & stSource, POINT pDestStart = {0,0} , POINT pSourceStart = { 0,0 }, SIZE szSize = {0,0}) { return stBitmap.CopyFrom(stSource,pDestStart,pSourceStart,szSize); }		
	bool CopyFrom(CSageBitmap & cSource, POINT pDestStart = {0,0} , POINT pSourceStart = { 0,0 }, SIZE szSize = {0,0}) { return stBitmap.CopyFrom(*cSource,pDestStart,pSourceStart,szSize); }		
		
	bool Copyto(RawBitmap_t & stDest, POINT pSourceStart = { 0,0}, POINT pDestStart = { 0,0 }, SIZE szSize = { 0,0 }) { return stBitmap.Copyto(stDest,pSourceStart,pDestStart,szSize); }
	bool Copyto(CSageBitmap & cDest,POINT pSourceStart = { 0,0}, POINT pDestStart = { 0,0 }, SIZE szSize = { 0,0 }) { return Copyto(*cDest,pSourceStart,pDestStart,szSize); }
		
	// FillColor() -- Fill the entire bitmap, or a section of it, with a specific color
	// Using FillColo(Color) with no other parameters fills the entire bitmap.
	// Using pStart and szSize to set a start location and rectangular area allows only a certain area to be filled with the supplied color.
	//
	bool __forceinline FillColor(RGBColor_t rgbColor, POINT pStart = { 0,0 }, SIZE szSize = { 0,0 }) { return stBitmap.FillColor(*rgbColor,pStart,szSize); }

	// FillColor() -- Fill the entire bitmap, or a section of it, with a specific color
	// Using FillColo(Color) with no other parameters fills the entire bitmap.
	// Using pStart and szSize to set a start location and rectangular area allows only a certain area to be filled with the supplied color.
	//
	bool __forceinline FillColor(DWORD dwColor, POINT pStart = { 0,0 }, SIZE szSize = { 0,0 }) { return stBitmap.FillColor(dwColor,pStart,szSize); }

	void Delete(){ stBitmap.Delete(); }
	RawBitmap_t  Clean(){ RawBitmap_t stOld = stBitmap; stBitmap.Clean(); return stOld; }
	bool CreateBitmap(int iWidth,int iHeight);

	RawBitmap_t operator - ()
	{
		RawBitmap_t x = stBitmap;
		x.iHeight = - x.iHeight;
		return x;
	}


	CSageBitmap(const CSageBitmap &p2)
	{
		stBitmap = {};
		CopyFrom(p2);

	}

	// isEmpty() -- Returns true of the bitmap is empty or invalid. 
	// This is the opposite of isValid() (i.e. !isValid()). 
	//
	__forceinline bool isEmpty() { return !isValid(); };

	// isValid() returns true of the bitmap is valid
	// If the bitmap does not exist or the width/height do not match 
	// the size of memory allocation or there are other inconsitencies, FALSE is returned
	// as the bitmap is not valid, even if it exists in memory
	//
	bool isValid()
	{
		return (this != nullptr && stBitmap.stMem != nullptr && !stBitmap.isInvalid()); 
	}
	operator unsigned char * () const { return stBitmap.stMem; };
	operator RawBitmap_t & () const { return (RawBitmap_t &) stBitmap; };

};
}; // namesapace Sage
#endif //_CRawBitmap_H_