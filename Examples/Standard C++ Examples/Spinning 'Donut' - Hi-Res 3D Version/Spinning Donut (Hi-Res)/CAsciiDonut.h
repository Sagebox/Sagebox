// 3d Ascii Donut -- All parts but original source Copyright(c) 2020, 2021 Rob Nelson.  robnelsonxx2@gmail.com -- all rights reserved.
// This file, information, and process within are for personal use only and may not be distributed without permission.
// Please modify, copy, do whatever you want for personal uses.  For professional, distribution or commercial uses, 
// contact the e-mail address above

    // **************************************** DEPRECATED *****************************************
    // 
    // ** Note: This program is a prototype of using a subclassed window, as well as creating a Sagebox object 
    //          vs. calling using static functions
    //
    // ** Mostly, this has been deprecated and is not a good example of how to use Sagebox, but it does 
    //    show an approach where you can:
    //
    //       1. Control when Sagebox gets insantiated and when it gets deleted (it gets deleted when you delete the object created, vs. 
    //          an unknown order when using funcition statically).  As long as the Sagebox object is created before any Sagebox global functions are used,
    //          Sagebox will be deleted when the object is deleted vs. at program termination.
    //
    //       2. Using a sub-classed window.  In this case, the window functions are used as regular functions (which is really deprecated at this point).
    //          This example was left in as most windowing packages tend to work within the subclassed window.  For various reasons, I don't think
    //          this is a good approach in today's programming world.
    //
    // ** This example will probably be changed to the current format of working with static Sagebox functions and the Window as a separate object.



// **************************
// 3D Ascii Donut Source Code
// **************************
//
// See notes in main.cpp for more information
//

#pragma once

#if !defined(_CAsciiDonut_h_)
#define _CAsciiDonut_h_

#include "SageBox.h"
#include "CColorWheelWidget.h"

typedef RGBColor24 RGB24;

class CAsciiDonut : public CWindow        
{                                            
    SageControlDelete        
private:
    unsigned char m_ucColorTable[257*4];    // Color table for each intensity element in the output map
                                            // of the donut calculation loop.
    CBitmap cBitmapBg;                      // 1-pixel wide bitmap with the background of the window.
    Sage::CPoint m_szWindowSize;

    Mem<double> m_fSinIMem = 4000;          // this gets the memory required for the Sin/Cos tables
    Mem<double> m_fCosIMem = 4000;          // (i.e. 4000 elements of each).
    Mem<double> m_fSinJMem = 4000;          // We can check the validity of it later with
    Mem<double> m_fCosJMem = 4000;          // an isValid() call, but we don't worry about it here.

    CBitmap m_cOut;                         // Output bitmap for the donut
    Sage::CPoint m_pInsetSize;              // Size if the area the donut traverses -- this helps so we 
                                            // only need to worry about Z-Buffer, bitmap, and other memory
                                            // in that area only, and not the entire window.
    Sage::CPoint m_pOffset;    
private:
    void FillColorTable(Sage::HSLColor_t stHSL);    // Fill the color table for the donut (i.e. red, blue, etc.), based on Hue
    void FillBgTable();                             // Fill the color table for the color of line in the window background
    void CalcBitmap(int * pMap);                    // Calculate the output bitmap based on the intensity map (i.e. reflection/difusion), etc.
    void CalcSinCosTable();                         // Calculate tables for sin/cos used multiple times to make things *much* faster
public:
    void Main();                                    // Main called from SageBox, as a SandBox-type class (i.e. derived from CWindow).        
};

#endif // _CAsciiDonut_h_