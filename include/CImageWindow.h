#pragma once

#include "Sagebox.h"


namespace Sage
{
class CGenBitmapWindow;

class CImageWin 
{
private:
    static constexpr const char     * kOptName_FillZoom     = "FillZoom";
    static constexpr const char     * kOptName_Percent      = "Percent";
    static constexpr const char     * kOptName_ZoomBox      = "ZoomBox";

public:
    class opt
    {
    public:
        static inline cwfOpt FillZoom() { return Sage::opt::Bool(kOptName_FillZoom); }
        static inline cwfOpt Percent(int iPercent) { return Sage::opt::Int(kOptName_Percent,iPercent); }
        static inline cwfOpt ZoomBox() { return Sage::opt::Bool(kOptName_ZoomBox); }
    };

private:
    static constexpr SIZE kDefaultWinSize   = { INT_MIN,INT_MIN };
    static constexpr POINT kDefaultWinLoc   = { INT_MIN,INT_MIN };

    bool m_bManuallyDeleted         = false;
    bool m_bWindowCloseEvent        = false;
    CGenBitmapWindow * m_cGen       = nullptr; 
    CDataStore * m_cDataStore       = nullptr;
    int m_iGenBitmapControlID       = 0;

    void Init(CWindow * cWin,Sage::CBitmap & cBitmap,POINT pWinLoc,SIZE szWinSize,const cwfOpt & cwOpt);
public:
    CImageWin(CBitmap & cBitmap,const cwfOpt & cwOpt = cwfOpt())                              ;
    CImageWin(CBitmap & cBitmap,SIZE szWinSize,const cwfOpt & cwOpt = cwfOpt())               ;
    CImageWin(CBitmap & cBitmap,POINT pWinLoc,const cwfOpt & cwOpt = cwfOpt())                ;
    CImageWin(CBitmap & cBitmap,POINT pWinLoc,SIZE szWinSize,const cwfOpt & cwOpt = cwfOpt()) ;
    bool Delete();  // Manually delete vs. automatic delete.
    bool WindowClosed(); 
    bool WindowCloseEvent(); 
    bool WaitforClose();
    bool isValid();
    
    static int WindowCount();
    static bool WaitforCloseAny();
    static bool WaitforCloseAll();
    static bool CloseAll();
    static void ShowInstructions();

    ~CImageWin();
};
} // namespace Sage