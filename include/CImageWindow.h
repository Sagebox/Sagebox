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


private:
    static constexpr SIZE kDefaultWinSize   = { INT_MIN,INT_MIN };
    static constexpr POINT kDefaultWinLoc   = { INT_MIN,INT_MIN };

    bool m_bManuallyDeleted         = false;
    bool m_bWindowCloseEvent        = false;
    CGenBitmapWindow * m_cGen       = nullptr; 
    CDataStore * m_cDataStore       = nullptr;
    int m_iGenBitmapControlID       = 0;

    void Init(CWindow * cWin,Sage::CBitmap & cBitmap,POINT pWinLoc,SIZE szWinSize,const kwOpt & keywords);
public:
    CImageWin(CBitmap & cBitmap,const kwOpt & keywords = kw::none)                              ;
    CImageWin(CBitmap & cBitmap,SIZE szWinSize,const kwOpt & keywords = kw::none)               ;
    CImageWin(CBitmap & cBitmap,POINT pWinLoc,const kwOpt & keywords = kw::none)                ;
    CImageWin(CBitmap & cBitmap,POINT pWinLoc,SIZE szWinSize,const kwOpt & keywords = kw::none) ;
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