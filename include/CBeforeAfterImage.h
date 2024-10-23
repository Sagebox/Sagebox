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


    CBeforeAfterImage(const char * sTitle,CBitmap & cBefore,CBitmap & cAfter,const kwOpt & keywords = kw::none);
    CBeforeAfterImage(CBitmap & cBefore,CBitmap & cAfter,const kwOpt & keywords = kw::none);
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

