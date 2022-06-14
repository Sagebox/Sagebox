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

#include "SageBox.h"
#include "CColorWheelWidget.h"

typedef RGBColor24 RGB24;

class CAsciiDonut   
{                                            
    SageControlDelete        
private:
    static inline unsigned char m_ucColorTable[257*4];      // Color table for each intensity element in the output map
                                                            // of the donut calculation loop.
    static inline CBitmap cBitmapBg;                        // 1-pixel wide bitmap with the background of the window.
    static inline CPoint m_szWindowSize;

    static inline Mem<double> m_fSinIMem = 4000;            // this gets the memory required for the Sin/Cos tables
    static inline Mem<double> m_fCosIMem = 4000;            // (i.e. 4000 elements of each).
    static inline Mem<double> m_fSinJMem = 4000;            // We can check the validity of it later with
    static inline Mem<double> m_fCosJMem = 4000;            // an isValid() call, but we don't worry about it here.

    static inline CBitmap m_cOut;                           // Output bitmap for the donut
    static inline CPoint m_pInsetSize;                      // Size if the area the donut traverses -- this helps so we 
                                                            // only need to worry about Z-Buffer, bitmap, and other memory
                                                            // in that area only, and not the entire window.
    static inline CPoint m_pOffset;    
    static inline CWindow   * m_cWin = nullptr;
private:
    static void FillColorTable(Sage::HSLColor_t stHSL);     // Fill the color table for the donut (i.e. red, blue, etc.), based on Hue
    static void FillBgTable();                              // Fill the color table for the color of line in the window background
    static void CalcBitmap(int * pMap);                     // Calculate the output bitmap based on the intensity map (i.e. reflection/difusion), etc.
    static void CalcSinCosTable();                          // Calculate tables for sin/cos used multiple times to make things *much* faster
public:
    static int Main();                                     // Main called from SageBox, as a SandBox-type class (i.e. derived from CWindow).        
};

