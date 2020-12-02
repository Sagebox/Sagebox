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

	static void HSVtoRGB(Sage::HSLColor_t & stHSL,Sage::RGBColor24 & rgbColor);
	static void HSVtoRGB(Sage::HSLColor_t & stHSL,Sage::RGBColor_t & rgbColor);


	static void HSLtoRGB(Sage::HSLColor_t & stHSL,Sage::RGBColor24 & rgbColor);
	static void HSLtoRGB(Sage::HSLColor_t & stHSL,Sage::RGBColor_t & rgbColor);
	static void HSLtoRGB(double fH,double fS,double fL,int &iRed,int &iGreen,int &iBlue);
	static void HSLtoRGB(double fH,double fS,double fL,Sage::RGBColor24 & rgbColor);
	static void HSLtoRGB(double fH,double fS,double fL,Sage::RGBColor_t & rgbColor);
	static void RGBtoHSL(int iRed,int iGreen,int iBlue,double &frH,double &frS,double &frL);
	static bool ResizeLanzcos(CSageBitmap & cSource,CSageBitmap & cDest); 
	static bool ResizeLanzcos(Sage::RawBitmap_t & stSource,Sage::RawBitmap_t & stDest); 
	static bool ResizeLanzcos(int iWidth,int iHeight,int iNewWidth,int iNewHeight,unsigned char * sSource,unsigned char * sDest);
	static CSageBitmap ResizeLanzcos(Sage::RawBitmap_t & stSource,int iNewWidth,int iNewHeight,bool * bSuccess = nullptr);
	static CSageBitmap ResizeLanzcos(CSageBitmap & cSource,int iNewWidth,int iNewHeight,bool * bSuccess = nullptr);

	static CSageBitmap BilinearResize(Sage::RawBitmap_t & stSource,int iNewWidth,int iNewHeight,bool * bSuccess = nullptr);
	static CSageBitmap BilinearResize(CSageBitmap & cSource,int iNewWidth,int iNewHeight,bool * bSuccess = nullptr);
	static bool BilinearResize(int iOrgWidth,int iOrgHeight,int iNewWidth,int iNewHeight,const unsigned char *sInputMem,unsigned char * sOutputMem);
};

#endif	// _CSageTools_h_