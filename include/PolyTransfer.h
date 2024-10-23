#pragma once
namespace Sage
{
class PolyTransfer_t
{
    static inline constexpr int kMaxPolygonSideTotalCount = 10000;    // Maximum sides allowed for polygon draws, otherwise polygon is ignored. 
    static inline constexpr int kMaxPolygonSideStackCount = 500;      // Up to 500 for stack usage, otherwise space is allocated.
    struct stPolyMem_t
    {

        CfPointf cfPoints[kMaxPolygonSideStackCount];
        Gdiplus::PointF gdiPoints[kMaxPolygonSideStackCount+1];
    };
    stPolyMem_t stPolyMem; 
    stPolyMem_t * stPolyMem2 = nullptr; 

    CfPointf * cfOrgPoints = nullptr;
    bool bValid = false;
    int iVertices = 0;
public:
    CfPointf * cf           = nullptr; 
    Gdiplus::PointF * gdi   = nullptr;
    template<typename T>
    PolyTransfer_t(T * cfIn,int iNumVertices);
//    PolyTransfer_t(CfPoint * cfIn,int iNumVertices);
//    PolyTransfer_t(CPoint * cfIn,int iNumVertices);
//    PolyTransfer_t(POINT * cfIn,int iNumVertices);
    ~PolyTransfer_t();
    bool isValid() const; 
    int getNumVertices() { return iVertices; } 
};

} // namespace Sage