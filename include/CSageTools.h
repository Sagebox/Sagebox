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

class CSageTools
{
public:
	static void RGBtoHSV(double& fH, double& fS, double& fV,int iRed,int iGreen,int iBlue);
	static void HSVtoRGB(double fH, double fS, double fV,int & iRed, int & iGreen, int & iBlue);

	static void HSVtoRGB(Sage::HSLColor_t stHSL,Sage::RGBColor24 & rgbColor);
	static void HSVtoRGB(Sage::HSLColor_t stHSL,Sage::RGBColor_t & rgbColor);

	static Sage::RGBColor_t HSVtoRGB(Sage::HSLColor_t stHSL);

	static void HSLtoRGB(Sage::HSLColor_t stHSL,Sage::RGBColor24 & rgbColor);
	static void HSLtoRGB(Sage::HSLColor_t stHSL,Sage::RGBColor_t & rgbColor);
	static Sage::RGBColor_t HSLtoRGB(Sage::HSLColor_t stHSL);
	static void HSLtoRGB(double fH,double fS,double fL,int &iRed,int &iGreen,int &iBlue);
	static void HSLtoRGB(double fH,double fS,double fL,Sage::RGBColor24 & rgbColor);
	static void HSLtoRGB(double fH,double fS,double fL,Sage::RGBColor_t & rgbColor);
	static void RGBtoHSL(int iRed,int iGreen,int iBlue,double &frH,double &frS,double &frL);
	static bool ResizeLanzcos(Sage::CBitmap & cSource,Sage::CBitmap & cDest); 
	static bool ResizeLanzcos(Sage::RawBitmap_t & stSource,Sage::RawBitmap_t & stDest); 
	static bool ResizeLanzcos(int iWidth,int iHeight,int iNewWidth,int iNewHeight,unsigned char * sSource,unsigned char * sDest);
	static Sage::CBitmap ResizeLanzcos(Sage::RawBitmap_t & stSource,int iNewWidth,int iNewHeight,bool * bSuccess = nullptr);
	static Sage::CBitmap ResizeLanzcos(Sage::CBitmap & cSource,int iNewWidth,int iNewHeight,bool * bSuccess = nullptr);

	static Sage::CBitmap BilinearResize(Sage::RawBitmap_t & stSource,int iNewWidth,int iNewHeight,bool * bSuccess = nullptr);
	static Sage::CBitmap BilinearResize(Sage::CBitmap & cSource,int iNewWidth,int iNewHeight,bool * bSuccess = nullptr);
	static bool BilinearResize(int iOrgWidth,int iOrgHeight,int iNewWidth,int iNewHeight,const unsigned char *sInputMem,unsigned char * sOutputMem);
};

#endif	// _CSageTools_h_