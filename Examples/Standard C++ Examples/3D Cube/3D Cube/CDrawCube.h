// CDrawCube.h -- Copyright(c) 2020, 2021 Rob Nelson.  robnelsonxx2@gmail.com -- all rights reserved.
// This file, information, and process within are for personal use only and may not be distributed without permission.
// Please modify, copy, do whatever you want for personal uses.  For professional, distribution or commercial uses, 
// contact the e-mail address above

// *****************************************
// SageBox 3D Interactive Cube Demonstration
// *****************************************
//
// See notes in CDrawCube.cpp for more information
//

#if !defined(_DrawCube_h_)
#define _DrawCube_h_

#include "SageBox.h"

class DrawCube
{
private:
    // Structure to store date for each side of the cube

    struct stPoly_t 
    {
        POINT v1;
        POINT v2;
        POINT v3;
        POINT v4;
        RGBColor_t rgbColor;
        double fMax;
        operator POINT * () const { return (POINT *) &v1; };
    };

private:
    CWindow * m_cWin;           // Main Davinci/SageWin Window

    // Some hard-coded defaults, which can be more variable for less specific demo-like applications

    int m_iMidX         = 425;
    int m_iMidY         = 300;
    double m_fMag       = 90;   // Current Magnification
    double m_fScreenZ   = 201;

    bool m_bSemiTransparent = false;    // Box is Semi-Transparent (when not in Wireframe)

    stPoly_t m_stAllPolys[6];   // Storage for the output polygons
    Point3D_t m_fCube[8]{};     // Out basic cube definition
    Point3D_t m_fOut[8]{};      // Output points (rotate by whichever axes)
    Point3D_t m_fRot[8]{};      // Output points (rotate by whichever axes)

    double m_fRotX = 0;         // Rotation on X axis (i.e. Y & Z rotate around Z axis, X does not change)
    double m_fRotY = 0;         // Rotation on Y axis (i.e. X & Z rotate around Z axis, Y does not change)
    double m_fRotZ = 0;         // Rotation on Z axis (i.e. X & Y rotate around Z axis, Z does not change)

    static constexpr double m_fEyeDistance = 1000;    // Eye Distance.  Make shorter for more distorted cube (i.e. as if your eye is right up against it)

    void FillPoly(stPoly_t & stPoly,int i1,int i2,int i3,int i4,Point3D_t pColor);
    void RotatePoints();

    void AddReflections(int i1,int i2,int i3,Point3D_t & pColor);
    void CreateCubePoints();
    void DrawWireframe();
    void DrawSurfaces();
    void Create2DPoints();
    void SortPolygons();    // Sort the polygons shapes. 
    __forceinline Point3D_t sqrt(const Point3D_t & p) { return { std::sqrt(p.fX), std::sqrt(p.fY), std::sqrt(p.fZ) }; };
    __forceinline Point3D_t min3d(const Point3D_t & p,double fValue) { return { min(p.fX,fValue), min(p.fY,fValue), min(p.fZ,fValue) }; }

public:
    DrawCube(CWindow * cWindow);
    void Main(); 
};
#endif        // _DrawCube_h_