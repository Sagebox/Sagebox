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

#if !defined(_CRawBitmap_H_)
#define _CRawBitmap_H_
#include "Sage.h"

#pragma warning(push)
#pragma warning(disable : 26451)    // Mute Microsoft "warning" about overflow (which, imo, is far too broad)
namespace Sage
{

class CBitmap;
//using CSageBitmap = CBitmap;
class CFloatBitmap
{
public:
	FloatBitmap_t fBitmap;
	FloatBitmap_t & operator *() { return fBitmap; }

	CFloatBitmap(CBitmap & cBitmap);
	CFloatBitmap(RawBitmap_t & stBitmap);
	CFloatBitmap(FloatBitmap_t & fBitmap) { this->fBitmap = fBitmap; };
	CFloatBitmap(int iWidth,int iHeight) { fBitmap = CreateFloatBitmap(iWidth,iHeight); }

	CFloatBitmap() 
	{ 
		fBitmap.Init();
	}
    CFloatBitmap(CFloatBitmap && p2) noexcept
    {
        fBitmap = p2.fBitmap;
        p2.fBitmap.Clean();
    }
	[[nodiscard]] RawBitmap_t ConverttoBitmap() { return fBitmap.ConverttoBitmap(); }
	[[nodiscard]] RawBitmap_t ConverttoBitmap(RawBitmap_t & stBitmap) { return fBitmap.ConverttoBitmap(stBitmap); }
	CFloatBitmap & operator = (CBitmap stBitmap);
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

//	CFloatBitmapM(CBitmap & cBitmap);
//	CFloatBitmapM(RawBitmap_t & stBitmap);
	CFloatBitmapM(int iWidth,int iHeight) { fBitmap = CreateFloatMBitmap(iWidth,iHeight); };
	CFloatBitmapM(const SIZE szSize) { fBitmap = CreateFloatMBitmap(szSize.cx,szSize.cy); };

//	CFloatBitmapM(FloatBitmapM_t & fBitmap) { this->fBitmap = fBitmap; };
	CFloatBitmapM(FloatBitmapM_t fBitmap) { this->fBitmap = fBitmap; };
	CFloatBitmapM() 
	{ 
		fBitmap = {}; 
	}
    CFloatBitmapM(CFloatBitmapM && p2) noexcept
    {
        fBitmap = p2.fBitmap;
        p2.fBitmap.Clean();
    }
	bool Invert() { return fBitmap.Invert(); };
	bool Multiply(CFloatBitmapM & cSource,POINT pDestStart = { 0,0 }, POINT pSourceStart = { 0,0 }, SIZE szSize = { 0,0 })
	{
		return fBitmap.Multiply(*cSource,pDestStart,pSourceStart,szSize);
	}
	[[nodiscard]] RawBitmap_t ConverttoBitmap() { return fBitmap.ConverttoBitmap(); }
	[[nodiscard]] RawBitmap_t ConverttoBitmap(RawBitmap_t & stBitmap) { return fBitmap.ConverttoBitmap(stBitmap); }
//	CFloatBitmapM & operator = (CBitmap stBitmap);

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

class CBitmap
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
	__forceinline  void SetPixel(int iX,int iY,DWORD dwColor)
    {
		if (!stBitmap.stMem || iX < 0 || iX >= stBitmap.iWidth || iY < 0 || iY >= stBitmap.iHeight) return;
		unsigned char * sTemp = stBitmap.stMem + iY*stBitmap.iWidthBytes + iX*3;
		*sTemp++ = GetBValue(dwColor);
		*sTemp++ = GetGValue(dwColor);
		*sTemp++ = GetRValue(dwColor);

    }

	__forceinline  void SetPixel(int iX,DWORD dwColor)
    {
		if (!stBitmap.stMem || iX < 0 || iX >= stBitmap.iWidth || iCurrentLine < 0 || iCurrentLine >= stBitmap.iHeight) return;
		unsigned char * sTemp = stBitmap.stMem + iCurrentLine*stBitmap.iWidthBytes + iX*3;
		*sTemp++ = GetBValue(dwColor);
		*sTemp++ = GetGValue(dwColor);
		*sTemp++ = GetRValue(dwColor);
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

	__forceinline  void SetFastPixel(int iX,int iY,DWORD dwColor) 
	{ 
		unsigned char * sTemp = stBitmap.stMem + iY*stBitmap.iWidthBytes + iX*3;
		*sTemp++ = GetBValue(dwColor);
		*sTemp++ = GetGValue(dwColor);
		*sTemp++ = GetRValue(dwColor);
	}	
	__forceinline  void SetFastPixel(int iX,DWORD dwColor) 
	{ 
		unsigned char * sTemp = stBitmap.stMem + iCurrentLine*stBitmap.iWidthBytes + iX*3;
		*sTemp++ = GetBValue(dwColor);
		*sTemp++ = GetGValue(dwColor);
		*sTemp++ = GetRValue(dwColor);
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

	__forceinline  RGBColor_t GetPixel(int iX,int iY = 0) 
	{ 
		if (!stBitmap.stMem || iX < 0 || iX >= stBitmap.iWidth || iY < 0 || iY >= stBitmap.iHeight) return { 0,0,0};
		unsigned char * sTemp = stBitmap.stMem + iY*stBitmap.iWidthBytes + iX*3;
		return { (int) sTemp[2], (int) sTemp[1], (int) sTemp[0] };
	}			
	/// <summary>
	/// Returns pixel bounded to the bitmap size (i.e. won't overflow).  May be a little slower than GetPixel(), which returns a 0 pixel (i.e. black)
    /// when boundaries are exceeded (but may be quicker).
    /// <para></para>
    /// Use GetPixel() when boundaries are expected to be honored/
    /// <para></para>
    /// Use GetFastPixel() for a fast version -- unchecked GetPixel() that does not check memory or bounds.
	/// </summary>
	/// <returns></returns>
	__forceinline  RGBColor_t GetPixelBounded(int iX,int iY = 0) 
	{ 
        if (!stBitmap.stMem) return { 0,0,0 };
        if (iX < 0) iX = 0;
        if (iY < 0) iY = 0;
        if (iY >= stBitmap.iHeight) iY = stBitmap.iHeight-1;
        if (iX >= stBitmap.iWidth) iX = stBitmap.iWidth-1;

        unsigned char * sTemp = stBitmap.stMem + iY*stBitmap.iWidthBytes + iX*3;
		return { (int) sTemp[2], (int) sTemp[1], (int) sTemp[0] };
	}			
	/// <summary>
	/// Returns pixel bounded to the bitmap size (i.e. won't overflow).  May be a little slower than GetPixel(), which returns a 0 pixel (i.e. black)
    /// when boundaries are exceeded (but may be quicker).
    /// <para></para>
    /// Use GetPixel() when boundaries are expected to be honored/
    /// <para></para>
    /// Use GetFastPixel() for a fast version -- unchecked GetPixel() that does not check memory or bounds.
	/// </summary>
	/// <returns></returns>
	__forceinline  RGBColor_t GetPixelBounded(POINT p) 
	{ 
        if (!stBitmap.stMem) return { 0,0,0 };
        if (p.x < 0) p.x = 0;
        if (p.y < 0) p.y  = 0;
        if (p.y  >= stBitmap.iHeight) p.y  = stBitmap.iHeight-1;
        if (p.x >= stBitmap.iWidth) p.x = stBitmap.iWidth-1;

        unsigned char * sTemp = stBitmap.stMem + p.y*stBitmap.iWidthBytes + p.x*3;
		return { (int) sTemp[2], (int) sTemp[1], (int) sTemp[0] };
	}			

	__forceinline  RGBColor_t GetPixel(POINT p) 
	{ 
		if (!stBitmap.stMem || p.x < 0 || p.x >= stBitmap.iWidth || p.y < 0 || p.y >= stBitmap.iHeight) return { 0,0,0};
		unsigned char * sTemp = stBitmap.stMem + p.y*stBitmap.iWidthBytes + p.x*3;
		return { (int) sTemp[2], (int) sTemp[1], (int) sTemp[0] };
	}			

	__forceinline  RGBColor_t GetFastPixel(int iX,int iY = 0) 
	{ 
		unsigned char * sTemp = stBitmap.stMem + iY*stBitmap.iWidthBytes + iX*3;
		return { (int) sTemp[2], (int) sTemp[1], (int) sTemp[0] };
	}			
	__forceinline  RGBColor_t GetFastPixel(POINT p) 
	{ 
		unsigned char * sTemp = stBitmap.stMem + p.y*stBitmap.iWidthBytes + p.x*3;
		return { (int) sTemp[2], (int) sTemp[1], (int) sTemp[0] };
	}			

//	CBitmap(RawBitmap_t & stBitmap) { this->stBitmap = stBitmap; };
	CBitmap(RawBitmap_t stBitmap) { this->stBitmap = stBitmap; };
	CBitmap(int iWidth,int iHeight) { this->stBitmap = Sage::CreateBitmap(iWidth,iHeight); }
	CBitmap(SIZE szSize) { this->stBitmap = Sage::CreateBitmap(szSize.cx,szSize.cy); }
	CBitmap() 
	{ 
		stBitmap = {}; 
	}

	CBitmap(CFloatBitmap & cBitmap);
	CBitmap(CFloatBitmapM & cBitmap);
//	CBitmap & operator = (RawBitmap_t & stBitmap) { this->stBitmap.Delete(); this->stBitmap = stBitmap; return *this; }
	CBitmap & operator = (RawBitmap_t stBitmap) { this->stBitmap.Delete(); this->stBitmap = stBitmap; return *this; }
	CBitmap & operator = (SIZE sz) { this->stBitmap.Delete(); this->stBitmap = Sage::CreateBitmap((int) sz.cx,(int) sz.cy); return *this; }
	CBitmap & operator = (CBitmap csBitmap) 
	{ this->stBitmap.Delete();
		CopyFrom(csBitmap);  return *this; }
//	CBitmap & operator = (FloatBitmap_t fBitmap) 
//	{ 
//		stBitmap.Delete(); this->stBitmap = fBitmap.ConverttoBitmap(); return *this; 
//	}
	CBitmap & operator = (CFloatBitmap & fBitmap) 
	{ 
		stBitmap.Delete(); this->stBitmap = fBitmap.ConverttoBitmap(); return *this; 
	}

//	CBitmap & operator = (FloatBitmapM_t fBitmap) 
//	{ 
//		stBitmap.Delete(); this->stBitmap = fBitmap.ConverttoBitmap(); return *this; 
//	}
	CBitmap & operator = (FloatBitmapM_t & fBitmap) 
	{ 
		stBitmap.Delete(); this->stBitmap = fBitmap.ConverttoBitmap(); return *this; 
	}
	CBitmap & operator = (CFloatBitmapM & fBitmap) 
	{ 
		stBitmap.Delete(); this->stBitmap = fBitmap.ConverttoBitmap(); return *this; 
	}

    /// <summary>
    /// Blends a color using the Alpha channel.  If no Alpha Channel exists, the bitmap is not changed.
    /// <para></para>
    /// --> If no color is given, the color defaults to Black (i.e. {0,0,0} or RGB(0))
    /// </summary>
    /// <param name="rgbColor">Color to fill with Alpha Channel Mask</param>
    /// <returns>Reference to input bitmap</returns>
    CBitmap & BlendAlphaColor(RGBColor_t rgbColor); 

    /// <summary>
    /// Blends a color using the Alpha channel.  If no Alpha Channel exists, the bitmap is not changed.
    /// <para></para>
    /// --> If no color is given, the color defaults to Black (i.e. {0,0,0} or RGB(0))
    /// </summary>
    /// <param name="rgbColor">Color to fill with Alpha Channel Mask</param>
    /// <returns>Reference to input bitmap</returns>
   CBitmap & BlendAlphaColor(COLORREF rgbColor = 0); 

    CBitmap(CBitmap && p2) noexcept
    {
        stBitmap = p2.stBitmap;
        iCurrentLine = p2.iCurrentLine;
        p2.stBitmap.Clean();
    }
	RawBitmap_t & operator *() { return stBitmap; }
	~CBitmap() 
	{ 
//		printf("Deleting bitmap.. size = %d,%d\n",stBitmap.iWidth,stBitmap.iHeight);	// Debug
		stBitmap.Delete(); 
	};

    /// <summary>
    /// Swaps red and blue channels.  This supports BGR uses, such as GPU uses or libraries and other routines that may  look for BGRA
    /// <para></para>
    /// The CBitmap (and RawBitmap32) types are RGB in memory, unless converted with this function to BGR.
    /// <para></para>
    /// --> Note: Once converted, all bitmap functions will still work, with the RED and BLUE channels reversed.
    /// </summary>
    /// <returns>true of successful, false if not successful (i.e. invalid bitmap, etc.)</returns>
    bool SwapBlueRedInline();

	__forceinline SIZE GetSize() { return { stBitmap.iWidth, stBitmap.iHeight }; }
    __forceinline int GetTotalPixels() { return stBitmap.iWidth*stBitmap.iHeight; };
	__forceinline int GetWidth() { return stBitmap.iWidth; }
	__forceinline int GetHeight() { return stBitmap.iHeight; }
	__forceinline int GetWidthBytes() { return stBitmap.iWidthBytes; }
	__forceinline int GetOverhang() { return stBitmap.iOverHang; }

    unsigned char * GetMem() { return stBitmap.stMem; }

    // ReverseBitmapInline() -- Turns the bitmap upside-down inline (i.e. the same bitmap).  This is helpful for many operations, since
    // Windows bitmaps are upside-down in memory.  This makes the bitmap right-side up and easier to deal with. 
    //
    // This returns the current bitmap (which has now been turned upside-down). 
    //
	CBitmap & ReverseBitmapInline() { stBitmap.ReverseBitmap(); return *this; }

    // ReverseBitmap() -- Turns the bitmap upside-down inline (i.e. the same bitmap).  This is helpful for many operations, since
    // Windows bitmaps are upside-down in memory.  This makes the bitmap right-side up and easier to deal with. 
    //
    // This functions returns a new CBitmap, leaving the current bitmap untouched. 
    // 
    // note; ReverseBitmapInline() can be used to reverse the bitmap without creating a new one.
    //
	CBitmap ReverseBitmap();

	bool ApplyMaskGraphic(RawBitmap_t & stBackground,RawBitmap_t & stDest) { return stBitmap.ApplyMaskGraphic(stBackground,stDest); }
	bool ApplyMaskGraphic(CBitmap & cBackground,CBitmap & cDest) { return stBitmap.ApplyMaskGraphic(*cBackground,*cDest); }
		
	bool ApplyMaskGraphic(POINT pSourceStart, RawBitmap_t & stDest, POINT pDestStart, SIZE & szSize) { return stBitmap.ApplyMaskGraphic(pSourceStart,stDest,pDestStart,szSize); }
	bool ApplyMaskGraphic(POINT pSourceStart, CBitmap & cDest, POINT pDestStart, SIZE & szSize) { return stBitmap.ApplyMaskGraphic(pSourceStart,*cDest,pDestStart,szSize); }
	bool ApplyMaskGraphic(CBitmap & cDest, const POINT pDestStart = { 0,0 }) { return stBitmap.ApplyMaskGraphic({ 0,0} ,*cDest,pDestStart,stBitmap.GetSize()); }

	bool ApplyMaskGraphic(RawBitmap_t & stSource, RawBitmap_t & stBackground,RawBitmap_t & stDest) { return stBitmap.ApplyMaskGraphic(stSource,stBackground,stDest); }
	bool ApplyMaskGraphic(CBitmap & cSource,CBitmap & cBackground,CBitmap & cDest) { return stBitmap.ApplyMaskGraphic(*cSource,*cBackground,*cDest); }


	bool ApplyMaskGraphicR(const POINT pSourceStart, RawBitmap_t & stDest,const POINT pDestStart,const SIZE & szSize) { return stBitmap.ApplyMaskGraphicR(pSourceStart,stDest,pDestStart,szSize); }
	bool ApplyMaskGraphicR(const POINT pSourceStart, CBitmap & cDest,const POINT pDestStart, const SIZE & szSize) { return stBitmap.ApplyMaskGraphicR(pSourceStart,*cDest,pDestStart,szSize); }
	bool ApplyMaskGraphicR(CBitmap & cDest, const POINT pDestStart) { return stBitmap.ApplyMaskGraphicR({ 0,0} ,*cDest,pDestStart,stBitmap.GetSize()); }
	bool ApplyMaskColor(RGBColor_t rgbColor	,RawBitmap_t stMask,POINT pMaskStart, POINT pDestStart, SIZE szSize) { return stBitmap.ApplyMaskColor(rgbColor,stMask,pMaskStart,pDestStart,szSize);	}
	bool ApplyMaskColorR(RGBColor_t rgbColor,RawBitmap_t stMask,POINT pMaskStart,POINT pDestStart, SIZE szSize) { return stBitmap.ApplyMaskColorR(rgbColor,stMask,pMaskStart,pDestStart,szSize);	}
	bool ApplyMaskColor(RGBColor_t rgbColor	,RawBitmap_t stMask,POINT pDestStart) { return stBitmap.ApplyMaskColor(rgbColor,stMask,pDestStart); }
	bool ApplyMaskColorR(RGBColor_t rgbColor,RawBitmap_t stMask,POINT pDestStart) { return stBitmap.ApplyMaskColorR(rgbColor,stMask,pDestStart); }

	bool ApplyMaskColor(RGBColor_t	 rgbColor,	CBitmap & cMask,POINT pMaskStart,	POINT pDestStart, SIZE szSize = { 0,0 }) { return stBitmap.ApplyMaskColor(rgbColor,*cMask,pMaskStart,pDestStart,szSize);	}
	bool ApplyMaskColorR(RGBColor_t	 rgbColor,	CBitmap & cMask,POINT pMaskStart,	POINT pDestStart, SIZE szSize = { 0,0 }) { return stBitmap.ApplyMaskColorR(rgbColor,*cMask,pMaskStart,pDestStart,szSize);	}
	bool ApplyMaskColor(RGBColor_t	 rgbColor,	CBitmap & cMask,POINT pDestStart = { 0,0 }) { return stBitmap.ApplyMaskColor(rgbColor,*cMask,pDestStart); }
	bool ApplyMaskColorR(RGBColor_t  rgbColor,	CBitmap & cMask,POINT pDestStart = { 0,0 }) { return stBitmap.ApplyMaskColorR(rgbColor,*cMask,pDestStart); }


	bool CopyFrom(RawBitmap_t & stSource, POINT pDestStart = {0,0} , POINT pSourceStart = { 0,0 }, SIZE szSize = {0,0}) { return stBitmap.CopyFrom(stSource,pDestStart,pSourceStart,szSize); }		
	bool CopyFrom(CBitmap & cSource, POINT pDestStart = {0,0} , POINT pSourceStart = { 0,0 }, SIZE szSize = {0,0}) { return stBitmap.CopyFrom(*cSource,pDestStart,pSourceStart,szSize); }		
		
	bool Copyto(RawBitmap_t & stDest, POINT pSourceStart = { 0,0}, POINT pDestStart = { 0,0 }, SIZE szSize = { 0,0 }) { return stBitmap.Copyto(stDest,pSourceStart,pDestStart,szSize); }
	bool Copyto(CBitmap & cDest,POINT pSourceStart = { 0,0}, POINT pDestStart = { 0,0 }, SIZE szSize = { 0,0 }) { return Copyto(*cDest,pSourceStart,pDestStart,szSize); }
		
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
	/// <summary>
	/// Initialize the Bitmap to an empty bitmap without deallocating the bitmap contained inside or any mask that may be active.
    /// <para></para>
    /// A RawBitmap_t is returned that contains the bitmap data so that it can be manipulated directly.
    /// <para></para>
    /// This function is used when the need arises to use the bitmap data directly outside of a Cbitmap.
    /// <para></para>
    /// *** You must deallocate any bitmap data (Bitmap and Mask) that is active yourself once this function is used
	/// </summary>
	/// <returns>RawBitmap_t of bitmap data contained in the CBitmap before it was initialized</returns>
	[[nodiscard]] RawBitmap_t  Clean(){ RawBitmap_t stOld = stBitmap; stBitmap.Clean(); return stOld; }
    
    // GaussianBlurStd() -- Blur the image with a Gaussian Blur of the given Radius (fRadius)
    //
    // If no output bitmap is given, a CBitmap object is returned with the blurred image. 
    // In this case, bError can also be input to be filled to check for error, OR the return bitmap can be checked for Empty() is !isValid() status to determine an error. 
    //
    // When an output bitmap is given, it must be either empty or the same size as the the bitmap in the current CBitmap object. 
    // TRUE is returned if the blur was successful, FALSE if there was an error.  If the output bitmap was empty, then Empty() or !isValid() may be called to determine an error status
    // If the output bitmap already exists, it will remain the same as it was before the called (and not a good indicator of an error)
    //
    // To blur the bitmap and keep the result in the same bitmap, use MyBitmap.GaussianBlurStd(MyBitmap); -- note that this creates a separate bitmap, blurs the original into this new bitmap as an output,
    // copies the result into the current bitmap, and then deletes the temporary bitmap.   i.e. it doesn't save memory allocation or time by blurring itself (as with some other funtions)
    //
    // ** note: This function is currenty using a slow method for a Gaussian blur until the multi-threading SSE versions are ready
    //
    bool GaussianBlurStd(CBitmap & cOutput,double fRadius); 

    // GaussianBlurStd() -- Blur the image with a Gaussian Blur of the given Radius (fRadius)
    //
    // If no output bitmap is given, a CBitmap object is returned with the blurred image. 
    // In this case, bError can also be input to be filled to check for error, OR the return bitmap can be checked for Empty() is !isValid() status to determine an error. 
    //
    // When an output bitmap is given, it must be either empty or the same size as the the bitmap in the current CBitmap object. 
    // TRUE is returned if the blur was successful, FALSE if there was an error.  If the output bitmap was empty, then Empty() or !isValid() may be called to determine an error status
    // If the output bitmap already exists, it will remain the same as it was before the called (and not a good indicator of an error)
    //
    // To blur the bitmap and keep the result in the same bitmap, use MyBitmap.GaussianBlurStd(MyBitmap); -- note that this creates a separate bitmap, blurs the original into this new bitmap as an output,
    // copies the result into the current bitmap, and then deletes the temporary bitmap.   i.e. it doesn't save memory allocation or time by blurring itself (as with some other funtions)
    //
    // ** note: This function is currenty using a slow method for a Gaussian blur until the multi-threading SSE versions are ready
    //
    CBitmap GaussianBlurStd(double fRadius,bool * bError = nullptr);


    // CreateBitmap() -- Replaces current bitmap and memory with an uninitialized bitmap of input size.
    // This replaces any current bitmap that already exists, but will not duplicae an existing mask
    // (a mask is a special property of CBitmap)
    // 
    // The new bitmap contains uninitialized memory.  Use Clear() to initially clear it.
    //
	bool CreateBitmap(int iWidth,int iHeight);

    // Normalize() -- Normalize the Black and White point levels. 
    //
    // This is similar to 'Level' or 'Auto levels' in image processing.
    // Normalize() will set the black and white points to the lowest highest points found in the image,
    // adding contrast to items with a lower white point or higher black point than 255 and 0, respectively.
    //
    // If no output bitmap is supplied, the normalize operation is performed on the current bitmap in the CBitmap object.
    // Otherwise, the Normalize() function is processed on the output bitmap, leaving the CBitmap object bitmap untouched.
    //
    // *** This function is in-progress ***
    //
    // Currently, the Normalize works on the Grayscale translation of the image (i.e. the image is converted to grayscale to determine
    // the black and white points, as opposed to setting them individually per-channel)
    //
    // The inputs fUpperThreshold and fLowerThreshold are TBD
    //
    bool Normalize(double fUpperThreshold = 1,double fLowerThreshold = 0);

    // Normalize() -- Normalize the Black and White point levels. 
    //
    // This is similar to 'Level' or 'Auto levels' in image processing.
    // Normalize() will set the black and white points to the lowest highest points found in the image,
    // adding contrast to items with a lower white point or higher black point than 255 and 0, respectively.
    //
    // If no output bitmap is supplied, the normalize operation is performed on the current bitmap in the CBitmap object.
    // Otherwise, the Normalize() function is processed on the output bitmap, leaving the CBitmap object bitmap untouched.
    //
    // *** This function is in-progress ***
    //
    // Currently, the Normalize works on the Grayscale translation of the image (i.e. the image is converted to grayscale to determine
    // the black and white points, as opposed to setting them individually per-channel)
    //
    // The inputs fUpperThreshold and fLowerThreshold are TBD
    //
    bool Normalize(CBitmap & cOutput,double fUpperThreshold = 1,double fLowerThreshold = 0);

    // CreateBitmap() -- Replaces current bitmap and memory with an uninitialized bitmap of input size.
    // This replaces any current bitmap that already exists, but will not duplicae an existing mask
    // (a mask is a special property of CBitmap)
    // 
    // The new bitmap contains uninitialized memory.  Use Clear() to initially clear it.
    //
	bool CreateBitmap(SIZE szSize);

	RawBitmap_t operator - ()
	{
		RawBitmap_t x = stBitmap;
		x.iHeight = - x.iHeight;
		return x;
	}


	CBitmap(const CBitmap &p2)
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
	bool isValid() const
	{
		return (this != nullptr && stBitmap.stMem != nullptr && !stBitmap.isInvalid()); 
	}
	operator unsigned char * () const { return stBitmap.stMem; };
	operator RawBitmap_t & () const { return (RawBitmap_t &) stBitmap; };

};
}; // namesapace Sage
#pragma warning(pop)
#endif //_CRawBitmap_H_