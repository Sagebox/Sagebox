#pragma once

#include "Sage.h"

// ------------------------
// Sagebox CBitmap 32 class
// ------------------------
// 
// Specifics:
// 
//      ● Bitmap32 is in RGBA format (not BGRA) not aligned on a 4-byte boundary
// 
//          ● Format is subject to change via testing (i.e. GPU Texture considerations, etc.)
//          ● With different SIMD sizes (SSE, AVX, AVX512) there is no reason to stick with the 4-byte boundary
//          ● There is no longer processing-time penalty for SIMD loads on a non-aligned boundary
//      ● TBD is a separated R, G, B version (i.e. different memory for each channel)
//          ● This can be very useful for high-speed operations, but needs testing especially with regard to 
//            GPU usage which would require tranlating and copying every frame (TBD)

// This class is in its preliminary state:
//
//      ● Most commented out elements are TO DO items, copied from the CBitmap header and not implemented
//      ● Some functions may not work as expected until more items are implemented
//      ● Items are implemented as needed for various demos, requests, etc. 
//
namespace Sage
{
class CBitmap32
{
public:
    static constexpr int kPixelWidth = 4;
    RawBitmap32_t stBitmap{};
	int iCurrentLine = 0;

	_sageinline unsigned char *  SetCurrentLine(int iLine) { iCurrentLine = iLine >= stBitmap.iHeight ? stBitmap.iHeight-1 : iLine < 0 ? iLine : iLine;
		return stBitmap.stMem ? stBitmap.stMem + iCurrentLine*stBitmap.iWidthBytes : nullptr; }

	_sageinline unsigned char * NextLine()
	{
		if (!stBitmap.stMem) return nullptr;

		if (iCurrentLine < -1 || iCurrentLine >= stBitmap.iHeight-1) iCurrentLine = stBitmap.iHeight-2;
		iCurrentLine++;
		return stBitmap.stMem ? stBitmap.stMem + iCurrentLine * stBitmap.iWidthBytes : nullptr;
	}
	_sageinline void SetPixel(int iX,int iY,DWORD dwColor)
    {
		if (!stBitmap.stMem || iX < 0 || iX >= stBitmap.iWidth || iY < 0 || iY >= stBitmap.iHeight) return;
		unsigned char * sTemp = stBitmap.stMem + iY*stBitmap.iWidthBytes + iX*kPixelWidth;
		*sTemp++ = GetBValue(dwColor);
		*sTemp++ = GetGValue(dwColor);
		*sTemp++ = GetRValue(dwColor);

    }

	_sageinline void SetPixel(int iX,DWORD dwColor)
    {
		if (!stBitmap.stMem || iX < 0 || iX >= stBitmap.iWidth || iCurrentLine < 0 || iCurrentLine >= stBitmap.iHeight) return;
		unsigned char * sTemp = stBitmap.stMem + iCurrentLine*stBitmap.iWidthBytes + iX*kPixelWidth;
		*sTemp++ = GetBValue(dwColor);
		*sTemp++ = GetGValue(dwColor);
		*sTemp++ = GetRValue(dwColor);
    }

	_sageinline void SetPixel(int iX,int iY,RGBColor_t rgbColor) 
	{ 
		if (!stBitmap.stMem || iX < 0 || iX >= stBitmap.iWidth || iY < 0 || iY >= stBitmap.iHeight) return;
		unsigned char * sTemp = stBitmap.stMem + iY*stBitmap.iWidthBytes + iX*kPixelWidth;
		*sTemp++ = rgbColor.iBlue;
		*sTemp++ = rgbColor.iGreen;
		*sTemp++ = rgbColor.iRed;
	}			
	_sageinline void SetPixel(int iX,RGBColor_t rgbColor) 
	{ 
		if (!stBitmap.stMem || iX < 0 || iX >= stBitmap.iWidth || iCurrentLine < 0 || iCurrentLine >= stBitmap.iHeight) return;
		unsigned char * sTemp = stBitmap.stMem + iCurrentLine*stBitmap.iWidthBytes + iX*kPixelWidth;
		*sTemp++ = rgbColor.iBlue;
		*sTemp++ = rgbColor.iGreen;
		*sTemp++ = rgbColor.iRed;
	}		

	_sageinline void SetFastPixel(int iX,int iY,DWORD dwColor) 
	{ 
		unsigned char * sTemp = stBitmap.stMem + iY*stBitmap.iWidthBytes + iX*kPixelWidth;
		*sTemp++ = GetBValue(dwColor);
		*sTemp++ = GetGValue(dwColor);
		*sTemp++ = GetRValue(dwColor);
	}	
	_sageinline void SetFastPixel(int iX,DWORD dwColor) 
	{ 
		unsigned char * sTemp = stBitmap.stMem + iCurrentLine*stBitmap.iWidthBytes + iX*kPixelWidth;
		*sTemp++ = GetBValue(dwColor);
		*sTemp++ = GetGValue(dwColor);
		*sTemp++ = GetRValue(dwColor);
	}	


	_sageinline void SetFastPixel(int iX,int iY,RGBColor_t rgbColor) 
	{ 
		unsigned char * sTemp = stBitmap.stMem + iY*stBitmap.iWidthBytes + iX*kPixelWidth;
		*sTemp++ = rgbColor.iBlue;
		*sTemp++ = rgbColor.iGreen;
		*sTemp++ = rgbColor.iRed;
	}	
	_sageinline void SetFastPixel(int iX,RGBColor_t rgbColor) 
	{ 
		unsigned char * sTemp = stBitmap.stMem + iCurrentLine*stBitmap.iWidthBytes + iX*kPixelWidth;
		*sTemp++ = rgbColor.iBlue;
		*sTemp++ = rgbColor.iGreen;
		*sTemp++ = rgbColor.iRed;
	}	

	_sageinline RGBColor_t GetPixel(int iX,int iY = 0) 
	{ 
		if (!stBitmap.stMem || iX < 0 || iX >= stBitmap.iWidth || iY < 0 || iY >= stBitmap.iHeight) return { 0,0,0};
		unsigned char * sTemp = stBitmap.stMem + iY*stBitmap.iWidthBytes + iX*kPixelWidth;
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
	_sageinline RGBColor_t GetPixelBounded(int iX,int iY = 0) 
	{ 
        if (!stBitmap.stMem) return { 0,0,0 };
        if (iX < 0) iX = 0;
        if (iY < 0) iY = 0;
        if (iY >= stBitmap.iHeight) iY = stBitmap.iHeight-1;
        if (iX >= stBitmap.iWidth) iX = stBitmap.iWidth-1;

        unsigned char * sTemp = stBitmap.stMem + iY*stBitmap.iWidthBytes + iX*kPixelWidth;
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
	_sageinline RGBColor_t GetPixelBounded(POINT p) 
	{ 
        if (!stBitmap.stMem) return { 0,0,0 };
        if (p.x < 0) p.x = 0;
        if (p.y < 0) p.y  = 0;
        if (p.y  >= stBitmap.iHeight) p.y  = stBitmap.iHeight-1;
        if (p.x >= stBitmap.iWidth) p.x = stBitmap.iWidth-1;

        unsigned char * sTemp = stBitmap.stMem + p.y*stBitmap.iWidthBytes + p.x*kPixelWidth;
		return { (int) sTemp[2], (int) sTemp[1], (int) sTemp[0] };
	}			

	_sageinline RGBColor_t GetPixel(POINT p) 
	{ 
		if (!stBitmap.stMem || p.x < 0 || p.x >= stBitmap.iWidth || p.y < 0 || p.y >= stBitmap.iHeight) return { 0,0,0};
		unsigned char * sTemp = stBitmap.stMem + p.y*stBitmap.iWidthBytes + p.x*3;
		return { (int) sTemp[2], (int) sTemp[1], (int) sTemp[0] };
	}			

	_sageinline RGBColor_t GetFastPixel(int iX,int iY = 0) 
	{ 
		unsigned char * sTemp = stBitmap.stMem + iY*stBitmap.iWidthBytes + iX*kPixelWidth;
		return { (int) sTemp[2], (int) sTemp[1], (int) sTemp[0] };
	}			
	_sageinline RGBColor_t GetFastPixel(POINT p) 
	{ 
		unsigned char * sTemp = stBitmap.stMem + p.y*stBitmap.iWidthBytes + p.x*kPixelWidth;
		return { (int) sTemp[2], (int) sTemp[1], (int) sTemp[0] };
	}			

	CBitmap32(RawBitmap32_t stBitmap) { this->stBitmap = stBitmap; };
	CBitmap32(int iWidth,int iHeight) { this->stBitmap = Sage::CreateBitmap32(iWidth,iHeight); }
	CBitmap32(SIZE szSize) { this->stBitmap = Sage::CreateBitmap32(szSize.cx,szSize.cy); }
	CBitmap32() 
	{ 
		stBitmap = {}; 
	}

//	CBitmap32(CFloatBitmap & cBitmap);       $$ CBitmap32 TBD
//	CBitmap32(CFloatBitmapM & cBitmap);      $$ CBitmap32 TBD

    CBitmap32 & operator = (RawBitmap32_t stBitmap) { this->stBitmap.Delete(); this->stBitmap = stBitmap; return *this; }
	CBitmap32 & operator = (SIZE sz) { this->stBitmap.Delete(); this->stBitmap = Sage::CreateBitmap32((int) sz.cx,(int) sz.cy); return *this; }
	CBitmap32 & operator = (const CBitmap32 & csBitmap) 
	{ 
        if (!stBitmap.isValid() || csBitmap.GetWidth() != stBitmap.iWidth || csBitmap.GetHeight() != stBitmap.iHeight)
            stBitmap.Delete();
		CopyFrom(csBitmap);  return *this; 
    }

//    CBitmap & operator = (CFloatBitmap & fBitmap)                                         $$ CBitmap32 TBD
//	{                                                                                       $$ CBitmap32 TBD
//		stBitmap.Delete(); this->stBitmap = fBitmap.ConverttoBitmap(); return *this;        $$ CBitmap32 TBD
//	}                                                                                       $$ CBitmap32 TBD

//	CBitmap & operator = (FloatBitmapM_t & fBitmap)                                         $$ CBitmap32 TBD
//	{                                                                                       $$ CBitmap32 TBD
//		stBitmap.Delete(); this->stBitmap = fBitmap.ConverttoBitmap(); return *this;        $$ CBitmap32 TBD
//	}

//	CBitmap & operator = (CFloatBitmapM & fBitmap)                                          $$ CBitmap32 TBD
//	{                                                                                       $$ CBitmap32 TBD
//		stBitmap.Delete(); this->stBitmap = fBitmap.ConverttoBitmap(); return *this;        $$ CBitmap32 TBD
//	}

    /// <summary>
    /// Blends a color using the Alpha channel.  If no Alpha Channel exists, the bitmap is not changed.
    /// <para></para>
    /// --> If no color is given, the color defaults to Black (i.e. {0,0,0} or RGB(0))
    /// </summary>
    /// <param name="rgbColor">Color to fill with Alpha Channel Mask</param>
    /// <returns>Reference to input bitmap</returns>
 //   CBitmap & BlendAlphaColor(RGBColor_t rgbColor);                                       $$ CBitmap32 TBD

    /// <summary>
    /// Blends a color using the Alpha channel.  If no Alpha Channel exists, the bitmap is not changed.
    /// <para></para>
    /// --> If no color is given, the color defaults to Black (i.e. {0,0,0} or RGB(0))
    /// </summary>
    /// <param name="rgbColor">Color to fill with Alpha Channel Mask</param>
    /// <returns>Reference to input bitmap</returns>
//   CBitmap & BlendAlphaColor(COLORREF rgbColor = 0);                                      $$ CBitmap32 TBD

    CBitmap32(CBitmap32 && p2) noexcept
    {
        stBitmap = p2.stBitmap;
        iCurrentLine = p2.iCurrentLine;
        p2.stBitmap.Clean();
    }
	RawBitmap32_t & operator *() { return stBitmap; }
	~CBitmap32() 
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
 //   bool SwapBlueRedInline();                                                             $$ CBitmap32 TBD

	/// <summary>
	/// Returns the Width and Height dimensions of the bitmap in memory. 
    /// <para></para> {0,0} is returned if the bitmap is not valid or does not exist.
	/// </summary>
	/// <returns>SIZE structure with the Width and Height dimensions of the bitmap</returns>
    _sageinline SIZE GetSize() { return this ? SIZE{ stBitmap.iWidth, stBitmap.iHeight } : SIZE{ 0,0 }; }

    _sageinline int GetTotalPixels() { return stBitmap.iWidth*stBitmap.iHeight; };
	/// <summary>
	/// Returns the Width of the bitmap in memory.  0 is returned if the bitmap is empty or invalid.
	/// </summary>
	/// <returns>Returns the Width of the bitmap in memory.  0 is returned if the bitmap is empty or invalid.</returns>
	_sageinline int GetWidth() const { return this ? stBitmap.iWidth : 0; }

	/// <summary>
	/// Returns the Width of the bitmap in memory.  0 is returned if the bitmap is empty or invalid.
	/// </summary>
	/// <returns>Returns the Width of the bitmap in memory.  0 is returned if the bitmap is empty or invalid.</returns>
	_sageinline int Width() const { return this ? stBitmap.iWidth : 0; }

	/// <summary>
	/// Returns the Height of the bitmap in memory.  0 is returned if the bitmap is empty or invalid.
	/// </summary>
	/// <returns>Returns the Height of the bitmap in memory.  0 is returned if the bitmap is empty or invalid.</returns>
    _sageinline int GetHeight() const { return this ? stBitmap.iHeight : 0; }

	/// <summary>
	/// Returns the Height of the bitmap in memory.  0 is returned if the bitmap is empty or invalid.
	/// </summary>
	/// <returns>Returns the Height of the bitmap in memory.  0 is returned if the bitmap is empty or invalid.</returns>
    _sageinline int Height() const { return this ? stBitmap.iHeight : 0; }

    _sageinline int GetWidthBytes() const { return this ? stBitmap.iWidthBytes : 0; }
    _sageinline int GetRowStride() const { GetWidthBytes(); }
    _sageinline int RowStride() const { GetWidthBytes(); }
    _sageinline int GetChannelStride() const { return this ? 4 : 0; }     // Split RGB not supported for unisgned char (yet anyway).
    _sageinline int ChannelStride() const { return GetChannelStride(); }


    /// <summary>
    /// Returns the (unsigned char *) memory pointer to the RGB bitmap in memory.
    /// <para></para>
    /// --> You can also use "**Bitmap" or simply use the Bitmap name for automatic conversion.
    /// <para></para>
    /// --> nullptr is returned if there is no bitmap.
    /// <para></para>
    /// --> CBitmap is a standard RGB bitmap with each row aligned by 4 bytes.
    /// <para></para>
    /// --> Use GetMaskMem() to get the mask memory, when a mask exists.
    /// </summary>
    /// <returns>(unsigned char *) memory pointer to the bitmap in memory</returns>
    _sageinline unsigned char * GetMem() { return stBitmap.stMem; }

    // ReverseBitmapInline() -- Turns the bitmap upside-down inline (i.e. the same bitmap).  This is helpful for many operations, since
    // Windows bitmaps are upside-down in memory.  This makes the bitmap right-side up and easier to deal with. 
    //
    // This returns the current bitmap (which has now been turned upside-down). 
    //
//	CBitmap & ReverseBitmapInline() { stBitmap.ReverseBitmap(); return *this; }                             $$ CBitmap32 TBD

    // ReverseBitmap() -- Turns the bitmap upside-down inline (i.e. the same bitmap).  This is helpful for many operations, since
    // Windows bitmaps are upside-down in memory.  This makes the bitmap right-side up and easier to deal with. 
    //
    // This functions returns a new CBitmap, leaving the current bitmap untouched. 
    // 
    // note; ReverseBitmapInline() can be used to reverse the bitmap without creating a new one.
    //
	//CBitmap ReverseBitmap();                                                                              $$ CBitmap32 TBD

   // ReverseBitmapInline() -- Turns the bitmap upside-down inline (i.e. the same bitmap).  This is helpful for many operations, since
    // Windows bitmaps are upside-down in memory.  This makes the bitmap right-side up and easier to deal with. 
    //
    // This returns the current bitmap (which has now been turned upside-down). 
    //
	CBitmap32 & ReverseBitmapInline() { stBitmap.ReverseBitmap(); return *this; }

    // ReverseBitmap() -- Turns the bitmap upside-down inline (i.e. the same bitmap).  This is helpful for many operations, since
    // Windows bitmaps are upside-down in memory.  This makes the bitmap right-side up and easier to deal with. 
    //
    // This functions returns a new CBitmap, leaving the current bitmap untouched. 
    // 
    // note; ReverseBitmapInline() can be used to reverse the bitmap without creating a new one.
    //
	CBitmap32 ReverseBitmap();




    // ********************************* MASK FUNCTIONS NOT YET IMPLEMENTED FOR CBitmap32

#if 0
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
#endif

	bool CopyFrom(RawBitmap32_t & stSource, POINT pDestStart = {0,0} , POINT pSourceStart = { 0,0 }, SIZE szSize = {0,0}) { return stBitmap.CopyFrom(stSource,pDestStart,pSourceStart,szSize); }		
	bool CopyFrom(CBitmap32 & cSource, POINT pDestStart = {0,0} , POINT pSourceStart = { 0,0 }, SIZE szSize = {0,0}) { return stBitmap.CopyFrom(*cSource,pDestStart,pSourceStart,szSize); }		
		
	bool Copyto(RawBitmap32_t & stDest, POINT pSourceStart = { 0,0}, POINT pDestStart = { 0,0 }, SIZE szSize = { 0,0 }) { return stBitmap.Copyto(stDest,pSourceStart,pDestStart,szSize); }
	bool Copyto(CBitmap32 & cDest,POINT pSourceStart = { 0,0}, POINT pDestStart = { 0,0 }, SIZE szSize = { 0,0 }) { return Copyto(*cDest,pSourceStart,pDestStart,szSize); }
		
    /// <summary>
    /// Clears the entire bitmap, seting all mask values to 0xFF
    /// <para></para>
    /// --> Use FillColor() to clear the bitmap to a color with affecting the mask values
    /// </summary>
    /// <returns></returns>
    bool Clear(); 

    /// <summary>
    /// Sets all mask values to 0xFF by default or to the value set. 
    /// </summary>
    /// <param name="MaskValue">Mask value to place into mask</param>
    /// <returns></returns>
    bool ClearMask(unsigned char MaskValue = 0xFF); 

	// FillColor() -- Fill the entire bitmap, or a section of it, with a specific color
	// Using FillColo(Color) with no other parameters fills the entire bitmap.
	// Using pStart and szSize to set a start location and rectangular area allows only a certain area to be filled with the supplied color.
	//
	bool _sageinline FillColor(CRgbColor rgbColor, POINT pStart = { 0,0 }, SIZE szSize = { 0,0 }) { return stBitmap.FillColor(rgbColor,pStart,szSize); }
	bool _sageinline FillColorA(CRgbColor rgbColor, int iMaskValue,POINT pStart = { 0,0 }, SIZE szSize = { 0,0 }) { return stBitmap.FillColorA(rgbColor,iMaskValue,pStart,szSize); }
	bool _sageinline FillColorA(RgbColorA rgbColor, POINT pStart = { 0,0 }, SIZE szSize = { 0,0 }) { return stBitmap.FillColor(rgbColor,pStart,szSize); }

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
	[[nodiscard]] RawBitmap32_t  Clean(){ RawBitmap32_t stOld = stBitmap; stBitmap.Clean(); return stOld; }
    
 


    // CreateBitmap() -- Replaces current bitmap and memory with an uninitialized bitmap of input size.
    // This replaces any current bitmap that already exists, but will not duplicae an existing mask
    // (a mask is a special property of CBitmap)
    // 
    // The new bitmap contains uninitialized memory.  Use Clear() to initially clear it.
    //
	bool CreateBitmap(int iWidth,int iHeight);

 
    // CreateBitmap() -- Replaces current bitmap and memory with an uninitialized bitmap of input size.
    // This replaces any current bitmap that already exists, but will not duplicae an existing mask
    // (a mask is a special property of CBitmap)
    // 
    // The new bitmap contains uninitialized memory.  Use Clear() to initially clear it.
    //
	bool CreateBitmap(SIZE szSize);


	CBitmap32(const CBitmap32 &p2)
	{
  
	    stBitmap = {};
		CopyFrom(p2);

	}

	// isEmpty() -- Returns true of the bitmap is empty or invalid. 
	// This is the opposite of isValid() (i.e. !isValid()). 
	//
	_sageinline bool isEmpty() { return !isValid(); };

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
	operator RawBitmap32_t & () const { return (RawBitmap32_t &) stBitmap; };   // $$ This needs tested

};
} // namespace Sage