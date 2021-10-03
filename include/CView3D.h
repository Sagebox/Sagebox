#pragma once
#ifdef _LOCALTEST

#include "Sage.h"
#include "Point3D.h"

namespace Sage
{
struct CView3D
{
private:
__forceinline void Mul3x3(Point3D_t * pMat3a,Point3D_t * pMat3b,Point3D_t * pResult)
{
    double a11 = pMat3a[0].fX;  double a21 = pMat3a[1].fX; double a31 = pMat3a[2].fX;
    double a12 = pMat3a[0].fY;  double a22 = pMat3a[1].fY; double a32 = pMat3a[2].fY;
    double a13 = pMat3a[0].fZ;  double a23 = pMat3a[1].fZ; double a33 = pMat3a[2].fZ;

    double b11 = pMat3b[0].fX;  double b21 = pMat3b[1].fX; double b31 = pMat3b[2].fX;
    double b12 = pMat3b[0].fY;  double b22 = pMat3b[1].fY; double b32 = pMat3b[2].fY;
    double b13 = pMat3b[0].fZ;  double b23 = pMat3b[1].fZ; double b33 = pMat3b[2].fZ;



    pResult[0].fX = a11*b11 + a12*b21 + a13*b31; pResult[0].fY = a11*b12 + a12*b22 + a13*b32; pResult[0].fZ = a11*b13 + a12*b23 + a13*b33;
    pResult[1].fX = a21*b11 + a22*b21 + a23*b31; pResult[1].fY = a21*b12 + a22*b22 + a23*b32; pResult[1].fZ = a21*b13 + a22*b23 + a23*b33;
    pResult[2].fX = a31*b11 + a32*b21 + a33*b31; pResult[2].fY = a31*b12 + a32*b22 + a33*b32; pResult[2].fZ = a31*b13 + a32*b23 + a33*b33;

}

public:


    static constexpr Point3D_t     pViewInit { 0,0, 1000000 }; // This is an actual number not an angle
    static constexpr Point3D_t     pLightInit { 0,0, 1 }; // This is an actual number not an angle
    Point3D_t       pViewer = pViewInit;
    Point3D_t       pLight { 0,0,1 };       // Facing object directly (this is also a specific number not an angle)
    Point3D_t       pLightNormalized { 0,0,1 };       // Facing object directly (this is also a specific number not an angle)

    double          m_fViewDiv;
    Point3D_t       pViewerNormal;
    double          fViewDistance = 0;
    double          m_fBaseHeight; 
    double          m_fViewAngle;
    double          m_fTanAngle;



    Point3D_t       pRotX;      // Rotation Translation Matrix
    Point3D_t       pRotY;
    Point3D_t       pRotZ;
    Point3D_t       TranslateView(const Point3D_t & pPoint) const;
    Point3D_t       Translateto2D(const Point3D_t & pPoint) const; 
    Point3D_t       Translateto2DV(const Point3D_t & pPoint) const; 
    double          Get2DMul(const Point3D_t & pPoint) const; 
    double          Get2DMulV(const Point3D_t & pPoint) const; 
    void SetViewpoint(const Point3D_t & pViewer);
    void SetViewerAngles(double fPan,double fPitch,double fRoll); 

    void SetLightPos(const Point3D_t pLight); 
    void SetViewAngle(double fAngle)
    {
        if (fAngle <= 0) fAngle = 45*3.14159/180; 
        if (fAngle >= 89) fAngle = 89; 

        m_fViewAngle = fAngle;
        m_fTanAngle = tan(fAngle);
        m_fViewDiv = m_fBaseHeight/m_fTanAngle;
        m_fTanAngle = 1;    // Remove to keep points at 0 in the Z plane the same size as the window when viewing distance is the 
                            // Same height
    }
    void Init(double fHeight,double fAngle = 45*3.14159/180)
    {
        // Set the reference divisor for the angle we want. 

        if (fAngle <= 0) fAngle = 45*3.14159/180; 
        if (fHeight <= 0) fHeight = 100; 

        m_fBaseHeight = fHeight;

        SetViewAngle(fAngle);
        SetViewpoint(pViewInit);    
        SetLightPos(pLightInit);
    }
    CView3D(double fHeight = 0,double fAngle = 45*3.14159/180)
    {
        Init(fHeight,fAngle); 
    }
    Point3D_t Translate2DPoint(const Point3D_t & pPoint = {})
    {
   Point3D_t pNew = TranslateView(pPoint);
    double fDist = fViewDistance/m_fTanAngle-pNew.fZ;
    double fMul = fDist ? fabs(m_fViewDiv/fDist) : 0; //*fMultProjection;

    return { pNew.fX*fMul, pNew.fY*fMul, fMul };

    }
};

} // namespace Sage

#endif // _LOCALTEST