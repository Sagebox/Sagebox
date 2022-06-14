#pragma once

#include "Sagebox.h"


namespace Sage::CodeStarters
{
    class ImageZoomerBeforeAfter;
}

namespace Sage
{
    

class CBeforeAfterImage
{
private:
    CodeStarters::ImageZoomerBeforeAfter * m_cImage = nullptr;
    CDataStore::DataStore_t * m_stDataStore = nullptr;
    bool m_bWindowCloseEvent = false;
public:

    class opt
    {
    public:
        static cwfOpt AfterTitle(const char * sTitle) { return Sage::opt::Str("AfterTitle",sTitle); }; 
        static cwfOpt BeforeTitle(const char * sTitle) { return Sage::opt::Str("BeforeTitle",sTitle); }; 
        static cwfOpt Percent(int iPercent) { return Sage::opt::Int("Percent",iPercent); }; 
        static cwfOpt Maximize(bool bMaximize = true) { return Sage::opt::Bool("Maximize",bMaximize); }; 
    };
    CBeforeAfterImage(const char * sTitle,CBitmap & cBefore,CBitmap & cAfter,const cwfOpt & cwOpt = cwfOpt());
    CBeforeAfterImage(CBitmap & cBefore,CBitmap & cAfter,const cwfOpt & cwOpt = cwfOpt());
    ~CBeforeAfterImage();
    CBeforeAfterImage(const CBeforeAfterImage &p1);
    bool isValid();
    bool Update(CBitmap & cAfterBitmap);
    bool WindowClosed();
    bool WindowCloseEvent();
    bool WaitforClose();
    bool DeleteOnClose();
    bool HideOnClose();
    bool Delete();
    static void ShowInstructions();

};

} // namespace Sage

