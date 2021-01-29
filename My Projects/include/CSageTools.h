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

#if !defined(_CSageTools_h_)

#define _CSageTools_h_

#include "CDavinci.h"
#include "Sage.h"
#include "Point3D.h"
#include <cmath>

namespace Sage
{
    class CSageTools;
    using CTools = CSageTools;

class CSageTools
{
public:
	static void RGBtoHSV(double& fH, double& fS, double& fV,int iRed,int iGreen,int iBlue);
	static void HSVtoRGB(double fH, double fS, double fV,int & iRed, int & iGreen, int & iBlue);

	static void HSVtoRGB(HSLColor_t stHSL,RGBColor24 & rgbColor);
	static void HSVtoRGB(HSLColor_t stHSL,RGBColor_t & rgbColor);

	static RGBColor_t HSVtoRGB(HSLColor_t stHSL);
	static HSLColor_t RGBtoHSV(RGBColor_t stRGB);

	static void HSLtoRGB(HSLColor_t stHSL,RGBColor24 & rgbColor);
	static void HSLtoRGB(HSLColor_t stHSL,RGBColor_t & rgbColor);
	static RGBColor_t HSLtoRGB(HSLColor_t stHSL);
	static void HSLtoRGB(double fH,double fS,double fL,int &iRed,int &iGreen,int &iBlue);
	static void HSLtoRGB(double fH,double fS,double fL,RGBColor24 & rgbColor);
	static void HSLtoRGB(double fH,double fS,double fL,RGBColor_t & rgbColor);
	static void RGBtoHSL(int iRed,int iGreen,int iBlue,double &frH,double &frS,double &frL);
    static HSLColor_t RGBtoHSL(RGBColor_t rgbColor); 
    static HSLColor_t RGBtoHSL(RGBColor24 rgbColor); 
	static bool ResizeLanzcos(CBitmap & cSource,CBitmap & cDest); 
	static bool ResizeLanzcos(RawBitmap_t & stSource,RawBitmap_t & stDest); 
	static bool ResizeLanzcos(int iWidth,int iHeight,int iNewWidth,int iNewHeight,unsigned char * sSource,unsigned char * sDest);
	static CBitmap ResizeLanzcos(RawBitmap_t & stSource,int iNewWidth,int iNewHeight,bool * bSuccess = nullptr);
	static CBitmap ResizeLanzcos(CBitmap & cSource,int iNewWidth,int iNewHeight,bool * bSuccess = nullptr);

	static CBitmap BilinearResize(RawBitmap_t & stSource,int iNewWidth,int iNewHeight,bool * bSuccess = nullptr);
	static CBitmap BilinearResize(CBitmap & cSource,int iNewWidth,int iNewHeight,bool * bSuccess = nullptr);
	static bool BilinearResize(int iOrgWidth,int iOrgHeight,int iNewWidth,int iNewHeight,const unsigned char *sInputMem,unsigned char * sOutputMem);
    static bool GaussianBlurStd(CBitmap & cInput,CBitmap & cOutput,double fRadius);
    static CBitmap GaussianBlurStd(CBitmap & cInput,double fRadius,bool * bRetError = nullptr);
    static bool NormalizeBitmap(RawBitmap_t & stInput,RawBitmap_t & stOutput,double fUpperThreshold = 1,double fLowerThreshold = 0);
};
};// namespace Sage
#endif	// _CSageTools_h_