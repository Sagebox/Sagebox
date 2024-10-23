#include "stdafx.h"
#include "Sagebox.h"
#include "MouseRegions.h"

#include "keywords\opt2_keyfuncs.h"

namespace Sage
{
// $$ Old version -- deprecated (probably)
#if 0
MouseRegion::Region * MouseRegion::FindRegion(int iRegion)
{
    if (!this || m_iActiveRegions <= 0) return nullptr;
    decltype(&m_vRegions[0]) region = nullptr;

    for (auto& r : m_vRegions)
        if (r.ulInternalID == iRegion) return &r; 

    return nullptr;
}
#endif
MouseRegion::Region * MouseRegion::FindRegion(int iRegion)
{
    if (!this || iRegion >= m_iActiveRegions) return nullptr;
    return &m_vRegions[iRegion];
}

bool MouseRegion::RemoveBoundBox(int iIndex)
{
    if (!this || iIndex < 0 || iIndex >= m_iActiveRegions) return false;
    m_vRegions[iIndex].bBoundingRect = false;
    return true;
}
#if 0
bool MouseRegion::SetBoundRect(int iIndex,RECT rRect)
{
    using sType = decltype(std::declval<SizeRect>().size.cx);
    using lType = decltype(std::declval<SizeRect>().loc.x);




    return SetBoundBox(iIndex,{(lType) rRect.left, (lType) rRect.top, (sType) abs(rRect.right-rRect.left), (sType) abs(rRect.bottom-rRect.top) });
}
#endif
bool MouseRegion::SetBoundBox(int iIndex,SizeRect srRect)
{
    if (!this || iIndex < 0 || iIndex >= m_iActiveRegions) return false;

    m_vRegions[iIndex].bBoundingRect = srRect.size.cx == 0 || srRect.size.cy == 0 ? false : true; 

    m_vRegions[iIndex].srBoundingRect = srRect; 
    return true;
}


#if 0

// Old version, pre-display-order pointers

bool MouseRegion::BringtoTop(int iRegion)
{
    if (!this || m_iActiveRegions <= 0) return nullptr;
    if (iRegion < 0) return false;

    bool bFound = false;

    // $$ lock here

    for (int i=0;i<m_iActiveRegions;i++)
    {
        if (m_vRegions[i].ulInternalID == iRegion)
        {
            if (i)
            {
                if (m_iCurDrag == i) m_iCurDrag = 0;
                Region r = m_vRegions[i];
                m_vRegions.erase(m_vRegions.begin()+i); 
                m_vRegions.insert(m_vRegions.begin(),r); 
            }

            bFound = true;
            break;
        }
    }
    return bFound;
}
#endif
POINT MouseRegion::GetDisplayPos(int iPos)
{
    iPos = m_iActiveRegions-iPos-1;
    if (!this || iPos < 0 || iPos >= this->m_iActiveRegions) return { 0, 0 };

    auto pPos = m_pDisplayTop;

    int iCount = 0;
    while (pPos)
    {
        if (iCount == iPos) return POINT{(int) pPos->fx,(int) pPos->fy}; 
        pPos = pPos->pDisplayNext;
        iCount++;

    }
    return {0,0};
}
int MouseRegion::GetDisplayPosID(int iPos)
{
    iPos = m_iActiveRegions-iPos-1;
    if (!this || iPos < 0 || iPos >= this->m_iActiveRegions) return 0;

    auto pPos = m_pDisplayTop;

    int iCount = 0;
    while (pPos)
    {
        if (iCount == iPos) return pPos->iUserID;
        pPos = pPos->pDisplayNext;
        iCount++;
    }
    return 0;


}

bool MouseRegion::ResetPoints()
{
    m_iActiveRegions = 0;
    m_vRegions.clear();
    return true;
}
int MouseRegion::GetDisplayPosIndex(int iIndex)
{
    iIndex = m_iActiveRegions-iIndex-1;
    if (!this || iIndex < 0 || iIndex >= this->m_iActiveRegions) return 0;

    auto pPos = m_pDisplayTop;

    int iCount = 0;
    while (pPos)
    {
        if (iCount == iIndex) return pPos->iPhysicalIndex;
        pPos = pPos->pDisplayNext;
        iCount++;
    }
    return 0;
}
bool MouseRegion::BringtoTop(Region & r)
{
    if (!this) return false;
    if (m_pDisplayTop == &r) return true;
    if (m_iActiveRegions < 1)
    {
        // Should really never need this. 

        m_pDisplayTop = &r; 
        r.pDisplayNext = nullptr;
        r.pDisplayPrev = nullptr;
        return false; 
    }
    if (r.pDisplayPrev)
    {
        int gg = 1;
        r.pDisplayPrev->pDisplayNext = r.pDisplayNext;
    }
    if (r.pDisplayNext) r.pDisplayNext->pDisplayPrev = r.pDisplayPrev;
    r.pDisplayPrev = nullptr; 
    r.pDisplayNext = m_pDisplayTop;
    
    if (m_pDisplayTop)
    {
        m_pDisplayTop->pDisplayPrev = &r; 
    }

    m_pDisplayTop = &r;
    return true;
}
#if 1
POINT MouseRegion::AdjusttoRect(int iRegion,RECT rRect)
{
    using sType = decltype(std::declval<SizeRect>().size.cx);
    using lType = decltype(std::declval<SizeRect>().loc.x);

    return AdjusttoSizeRect(iRegion,{(lType) rRect.left, (lType) rRect.top, (sType) abs(rRect.right-rRect.left), (sType) abs(rRect.bottom-rRect.top) });

}
POINT MouseRegion::AdjusttoSizeRect(int iRegion,SizeRect sizeRect)
{
    auto r = FindRegion(iRegion);
    if (!r) return {};

    return AdjusttoSizeRect(*r,sizeRect);
    
}
#endif
#if 1
POINT MouseRegion::AdjusttoSizeRect(Region & r,SizeRect sizeRect,bool * bAdjusted)
{
    auto fx      = (float) sizeRect.loc.x;
    auto fy      = (float) sizeRect.loc.y;
    auto fWidth  = (float) sizeRect.size.cx;
    auto fHeight = (float) sizeRect.size.cy;
    
    if (r.bPointType)
    {
        fx -= r.fWidth/2.0f;
        fWidth += r.fWidth;
        fy -= r.fHeight/2.0f;
        fHeight += r.fHeight;
    }
    bool bSet = false; 
    float fxNew = r.fx;
    float fyNew = r.fy; 

    if (r.fx < fx) { fxNew = fx; bSet = true; }
    if (r.fy < fy) { fyNew = fy; bSet = true; }

    if (r.fx + r.fWidth  > fx+fWidth)  { fxNew = fx+fWidth-r.fWidth-1;    bSet = true; }
    if (r.fy + r.fHeight > fy+fHeight) { fyNew = fy+fHeight-r.fHeight-1;  bSet = true; }

    if (bSet)
    {
        if (m_iCurDrag < 0)
        {
            r.fxOrg = r.fx = fxNew;
            r.fyOrg = r.fy = fyNew;
            m_bMovePending = true;
        }
        else
        {
            r.fx = m_cfCurDrag.x = fxNew;
            r.fy = m_cfCurDrag.y = fyNew;
            m_bMovePending = true; 
            m_cfPending = { fxNew, fyNew };
        }
    }
    if (bAdjusted) *bAdjusted = bSet;
    return { (decltype(std::declval<POINT>().x)) r.fx, (decltype(std::declval<POINT>().x)) r.fy };

}
#endif
bool MouseRegion::SetPos(int iIndex,POINT pLoc)
{
    if (!this || iIndex < 0 || iIndex >= m_iActiveRegions) return false;

    auto & region = m_vRegions[iIndex];

    // Only set if we're not dragging.  Otherwise set it as pending to set after drag has ended

    using ftype = decltype(region.fx);

    auto fx = (ftype) pLoc.x;
    auto fy = (ftype) pLoc.y;

    if (region.bPointType)
    {
        fx -= region.fWidth/(ftype) 2.0;
        fy -= region.fHeight/(ftype) 2.0;
    }

    if (m_iCurDrag < 0)
    {
        region.fx = fx;
        region.fy = fy;

        region.fxOrg = region.fx;
        region.fyOrg = region.fy; 
    }
    else
    {
        m_bMovePending = true; 

        m_cfPending = { fx, fy };

    }
    // Adjust any dragging going on

    //  m_cfCurDrag     = { region->fx, region->fy }; 
    //  m_cfLastDrag    = m_cfCurDrag;
    //  m_cfStartDrag   = m_cWin.GetMousePos(); 
        
    return true;
}

void MouseRegion::EndMouseDrag()
{
    if (m_iCurDrag >= 0)
    {
        auto &r = m_vRegions[m_iCurDrag];
        r.fxOrg = r.fx;
        r.fyOrg = r.fy; 
        m_iCurDrag = -1;
        m_ulCurSelected = -1;
        m_stEvents.bDragEventEnd = true;
        m_bEventReady = true;

        if (m_bMovePending)
        {
            r.fx = r.fxOrg = m_cfPending.x; 
            r.fy = r.fyOrg = m_cfPending.y; 

            m_bMovePending = false;
        }
    }
};
void MouseRegion::UpdatePoints()
{
    auto& v                 = m_vRegions;
    auto pMouse             = m_cWin.GetMousePos();
   
    auto iClickCounter      = Sagebox::GetCounterMouseClick(); 
    auto iUnclickCounter    = Sagebox::GetCounterMouseUnclick(); 
    auto iMouseMoveCounter  = Sagebox::GetCounterMouseMove(); 

    bool bMoved             = false;
    bool bClicked           = false;
    bool bUnclick           = false;

    if (iUnclickCounter != m_iLastUnclickCount)
    {
        m_iLastUnclickCount = iUnclickCounter;
          //  SageDebug::printf("UNclick Counter = {p}%d\n",iUnclickCounter); 

        m_bMouseDown = false; 
        bUnclick = true;
        
    }
    bool bSetCapture = false;
    if (iClickCounter      != m_iLastClickCount)
    {
        bClicked    = true;
        m_bMouseDown = m_cWin.MouseButtonDown() ? true : false;
        bSetCapture = true;
        m_iLastClickCount = iClickCounter;
    //    SageDebug::printf("Click Counter = {p}%d\n",iClickCounter); 
    }
    if (iMouseMoveCounter != m_iLastMouseMoveCount)
    {
        bMoved = true;
        m_iLastMouseMoveCount = iMouseMoveCounter;
   //     SageDebug::printf("Mouse Move = {p}%d\n",m_iLastMouseMoveCount); 
    }

    bool bProcess = bClicked || bMoved || bUnclick || m_bPreprocess;

    // If we're in a mouse drag, then process the current selection or release it if the mouse is no longer down. 


    bool bInMouseDrag = bMoved && m_iCurDrag >= 0 && m_bMouseDown;

    if (bUnclick || !m_bMouseDown)
    {
        // Set Drag Event Ended

        if (bInMouseDrag)
        {
            m_stEvents.bDragEventEnd    = true;
            m_stEvents.bDragEvent       = false;    // $$ should already be false
            m_bEventReady               = true;
        }

        EndMouseDrag();
        bInMouseDrag = false;
        if (bUnclick) bProcess = false;
    }

    // Process any current drag

    if (bInMouseDrag && bMoved)
    {
        bProcess = false;   // Don't process further.
        auto &r = m_vRegions[m_iCurDrag];

        m_cfLastDrag = m_cfCurDrag;
        m_cfCurDrag  = CfPointf(r.fxOrg,r.fyOrg) + (CfPointf) pMouse -  m_cfStartDrag;

        m_stEvents.bDragEvent = true;
        m_bEventReady = true;

        r.fx = m_cfCurDrag.x;
        r.fy = m_cfCurDrag.y; 

        m_bMovePending = false; // Negate any override

        r.cfOOB = { r.fx, r.fy };
     
        r.bisOOB = false;

        if (r.bBoundingRect || r.bBoundtoWindow)
        {
            if (r.bBoundtoWindow && !r.bBoundingRect) 
            {
                auto szTemp = m_cWin.___internal__GetLastKnownWindowClientSize();
                r.srBoundingRect = { 0,0, szTemp.cx,szTemp.cy };
            }
            CfPointf cfOld = { r.fx, r.fy };
            AdjusttoSizeRect(r,r.srBoundingRect,&r.bisOOB);  // This must be after m_bMovePending set to false above, as it sets is back if necessary.

            if (r.bisOOB) r.cfOOB = cfOld;
        }
//        SageDebug::printf("{g}{%.2f,%.2f}, {y}{%.2f,%.2f}\n",r.fx,r.fy,r.cfOOB.x,r.cfOOB.y);

    }
    
    if (bProcess)
    {
        bool bFoundRegion = false;

        auto * pr = m_pDisplayTop; 

        //for (int i=0;i<m_iActiveRegions;i++)
        while (pr)
        {
            auto& r = *pr; //m_vRegions[i];

            // Search for new highlighted region

            int iX      = (decltype(iX))        r.fx;
            int iY      = (decltype(iY))        r.fy; 
            int iWidth  = (decltype(iWidth))    r.fWidth; 
            int iHeight = (decltype(iHeight))   r.fHeight;
        
            if (r.bDisplayOOB)
            { 
                iX = (int) r.cfDisplayOOB.x;
                iY = (int) r.cfDisplayOOB.y;
            }
            if (pMouse.x >= iX && pMouse.x < iX + iWidth &&
                pMouse.y >= iY && pMouse.y < iY + iHeight)
            {
                bFoundRegion = true;
                if (m_ulCurHighlight != r.ulInternalID)
                {
                  //  SageDebug::printf("Highlight = {cyan}%d,%d\n",i,(int) r.ulInternalID); 
                    m_ulCurHighlight = r.ulInternalID;
                    m_stEvents.ulHighlight = r.ulInternalID;
                    m_stEvents.bNewHighlight = true;
                    m_bEventReady = true;

                }
                if (bMoved)
                {
                  //  SageDebug::printf("Mouse Move = {y}%d,%d (down = {c}%s)\n",i,(int) r.ulInternalID,Sage::BoolStringY(m_bMouseDown)); 
                    m_stEvents.ulMouseMoved = r.ulInternalID;

 
                }
                if (bClicked)
                {
                    // Probably overkill referring to bSetCapture
                    //
                    if (bSetCapture)
                    {
                        m_cWin.CaptureMouse();
                        //SageDebug::printf("Captured mouse\n"); 

                    }

                    if (r.bDisplayOOB)
                    {
                        r.fx = r.cfDisplayOOB.x;
                        r.fy = r.cfDisplayOOB.y;

                        r.fxOrg = r.fx;
                        r.fyOrg = r.fy;
                    }
                    r.bisOOB = false; // if the object was out-of-bounds, then set it to the current point.
                    r.bDisplayOOB = false;
                    r.cfOOB = { r.fx, r.fy };
                    // SageDebug::printf("Mouse Click = {r}%d,%d\n",i,(int) r.ulInternalID); 
                    m_stEvents.ulClicked    = r.ulInternalID;
                    m_stEvents.ulSelected   = r.ulInternalID;

                    m_iCurDrag  = r.iPhysicalIndex;
                    m_iCurDragID = r.ulInternalID;
                    m_cfCurDrag     = { r.fx, r.fy };
                    m_cfStartDrag   = pMouse;
                    m_cfLastDrag    = m_cfCurDrag;

                    m_ulCurHighlight = -1;    // Remove highlight when selected
                    m_ulCurSelected             = r.ulInternalID;
                    m_iLastSelectedDisplay      = m_ulCurSelected;
                    m_stEvents.bNewHighlight    = true;
                    m_stEvents.bDragEvent       = true;
                    m_bEventReady               = true;
                    m_bSelectPending            = true;

                    if (m_bSelectionAutoFocus) BringtoTop(r);
                }

                break;
            }
            pr = pr->pDisplayNext;
        }

        if (bFoundRegion)
        {
            int gg = 1;
        }
        if (!bFoundRegion)
        {

            if (bClicked || bUnclick)
            {
                if (m_iCurDrag >= 0)
                {
                    // this should never happen -- $$ Check if this occurs here. msg or something.
                    m_stEvents.bDragEventEnd = true;
                    m_stEvents.bDragEvent = false;
                    m_bEventReady = true;
                }
                m_iCurDrag = -1; 
            }
            if (m_ulCurHighlight >= 0) 
            {
                m_bEventReady = true;
                m_stEvents.bNewHighlight = true;
                m_stEvents.ulHighlight = -1;
            }
            m_ulCurHighlight = -1;
            m_ulCurSelected = -1;
        }
    } 
    m_bPreprocess = false;
}
int MouseRegion::AddPoint(const SizeRect & sizeRect,const kwOpt & keywords)
{
    return AddPoint(sizeRect.loc,sizeRect.size,keywords);
}

int MouseRegion::AddPoint(const POINT pLoc,const SIZE szSize,const kwOpt & keywords)
{
    if (m_iActiveRegions >= kMaxMouseRegions) return -1; 
   
    // Stop certain processes -- Mouse dragging, etc.

    auto keys = keywords.FillKeyValues();        // Link together all 

    auto iUserID = kwType::ckw::Get(keys._ID,0); 

    auto opBoundingSR = kwType::ckw::Get(keys._BoundingSizeRect2);

    if (!opBoundingSR && m_bDefaultBoundBox) opBoundingSR = m_srDefaultBoundBox;

   // auto opBoundingR  = kwType::ckw::Get(keys._BoundingRect2);

    EndMouseDrag();

    Region rg; 
    using fType = decltype(rg.fWidth); 

    rg.bBoundtoWindow        = kwType::ckw::Get(keys._mouse_region_key_mrBindtoWindow,m_bDefaultBindtoWindow); 
    rg.fWidth                = (fType) (szSize.cx*2);
    rg.fHeight               = (fType) (szSize.cy*2);
    rg.fx                    = (fType) pLoc.x-(fType) szSize.cx;
    rg.fy                    = (fType) pLoc.y-(fType) szSize.cy;
    rg.ulInternalID          = m_ulCurID++;
    rg.iUserID               = iUserID;
    rg.bPointType            = true;
 
    rg.fxOrg = rg.fx;
    rg.fyOrg = rg.fy; 
    rg.iPhysicalIndex       = m_iActiveRegions;

    rg.stAutoDraw           = m_stAutoDrawDefaults; // Set default Auto Draw Components (to be used later)
    rg.bAutoDraw            = true; // $$ Temporary
    m_iActiveRegions++;

    CString csAutoDraw = kwType::ckw::Get(keys._mouse_region_key_mrAutoDraw,(const char *) nullptr); 

    if (!csAutoDraw.isEmpty())
    {
        rg.stAutoDraw = ParseAutoDrawString(*csAutoDraw); 
    }

    m_vRegions.emplace_back(rg); 

    // Add this to the bottom of the display order. 

    auto * pv = &m_vRegions[m_iActiveRegions-1];

    pv->pDisplayPrev = nullptr;
    if (m_pDisplayTop) m_pDisplayTop->pDisplayPrev = pv; 
    pv->pDisplayNext = m_pDisplayTop;
   // if (m_pDisplayTop) pv->pDisplayPrev = pv;

    m_pDisplayTop = pv; 
    

    if (opBoundingSR) SetBoundBox(rg.ulInternalID ,*opBoundingSR);
    //if (opBoundingR ) SetBoundingRect    (rg.ulInternalID ,*opBoundingR );

    UpdatePoints();  // Reset or Add any highlighting, etc. for new addition

    return rg.ulInternalID; 
}
bool MouseRegion::PrintDisplayOrder()
{
    auto pDisplay = m_pDisplayTop;

    int i = 0;
    Sagebox::conio.printf("{bg=darkblue}-------------------- Display Order --------------------\n");
    while (pDisplay)
    {
        Sagebox::conio.printf("%.2d: UserID = %d (prev = {g}%d{}, next = {g}%d)\n",i++,pDisplay->iUserID,
            pDisplay->pDisplayPrev ? pDisplay->pDisplayPrev->iUserID : -1,
            pDisplay->pDisplayNext ? pDisplay->pDisplayNext->iUserID : -1);
        pDisplay = pDisplay->pDisplayNext; 

        if (i > 100) break;
    }
    return true;
}

bool MouseRegion::ResetSelected()
{
    if (!this) return false;
    m_iLastSelectedDisplay = -1;
    return true;
}
bool MouseRegion::AutoDraw()
{
    return AutoDrawIndex((std::numeric_limits<int>::min)());
}
bool MouseRegion::AutoDrawIndex(int iIndex)
{
    if (!this || m_iActiveRegions < 0) return false;

    if (!m_iActiveRegions) return true; 

    int iCheckIndex = iIndex == (std::numeric_limits<int>::min)() ? -1 : iIndex;
    // Display regions in reverse order of position.
    // 

    auto pos = m_pDisplayTop;

    while(pos->pDisplayNext) pos = pos->pDisplayNext;

    if (m_ulCurSelected > 0) m_iLastSelectedDisplay = m_ulCurSelected;

    while (pos)
    {
        if (iCheckIndex > 0 && pos->ulInternalID != iIndex) 
        {
            pos = pos->pDisplayPrev;
            continue;
        }

        int iCurSelected = m_ulCurSelected;

        // If the selected color is different than the highlighted color, then display the selected color even if it is not currently selected (i.e. last selected color)
        if (pos->stAutoDraw.rgbColor[1] != pos->stAutoDraw.rgbColor[2]) iCurSelected = m_iLastSelectedDisplay;

        if (pos->stAutoDraw.bDisabled)
        {
            pos = pos->pDisplayPrev;
            continue;
        }
        int iIndex = pos->iPhysicalIndex;
        RgbColorA rgbColor = iIndex == (int) this->m_ulCurHighlight ? pos->stAutoDraw.rgbColor[1] : iIndex == (int) iCurSelected ? pos->stAutoDraw.rgbColor[2] : 
            pos->stAutoDraw.rgbColor[0];

        RgbColorA rgbPenColor = iIndex == (int) this->m_ulCurHighlight ? pos->stAutoDraw.rgbPenColor[1] : iIndex == (int) iCurSelected ? pos->stAutoDraw.rgbPenColor[2] : 
            pos->stAutoDraw.rgbPenColor[0];

        float fPenSize = iIndex == (int) this->m_ulCurHighlight ? pos->stAutoDraw.fPenSize[1] : iIndex == (int) iCurSelected ? pos->stAutoDraw.fPenSize[2] : 
            pos->stAutoDraw.fPenSize[0];

         
        if (rgbColor.iAlpha > 0)        // i.e. invisible
        {
            auto szWinSize = m_cWin.___internal__GetLastKnownWindowClientSize();
            auto fx = pos->fx;
            auto fy = pos->fy; 
            auto fWidth = pos->fWidth;
            auto fHeight = pos->fHeight;

            bool bDisplayOOB = false;

            if ((int) fx > szWinSize.cx-5) { fx = (float) (szWinSize.cx-5);  bDisplayOOB = true;  }
            if ((int) fy > szWinSize.cy-5) { fy = (float) (szWinSize.cy-5);  bDisplayOOB = true;  }

            if (fx + fWidth < 5.0f) { fx = -fWidth+5.0f;  bDisplayOOB = true; }
            if (fy + fWidth < 5.0f) { fy = -fHeight+5.0f; bDisplayOOB = true; }
            pos->bDisplayOOB = bDisplayOOB;
            pos->cfDisplayOOB = { fx, fy };

            // Convert circles to squares when display OOB, since circles can be hidden in corners. 

            auto eShape = bDisplayOOB ? ShapeType::Rectangle : pos->stAutoDraw.eShape;
            switch(eShape)
            {
                case ShapeType::Circle:
                    m_cWin.FillEllipse_f(fx+fWidth/2.0f,fy+fHeight/2.0f,fWidth/2.0f,fHeight/2.0f,rgbColor,kw::AutoGradient(m_bAutoGradient)); 
                    if (fPenSize > 0)
                    m_cWin.DrawEllipse_f(fx+fWidth/2.0f,fy+fHeight/2.0f,fWidth/2.0f,fHeight/2.0f,rgbPenColor,kw::PenSize(fPenSize)); 
                    break;
                default:
                    m_cWin.FillRectangle_f(fx,fy,fWidth,fHeight,rgbColor,kw::AutoGradient(m_bAutoGradient)); 
                    if (fPenSize > 0)
                    m_cWin.DrawRectangle_f(fx,fy,fWidth,fHeight,rgbPenColor,kw::PenSize(fPenSize)); 
                    break;
            }
        }
#if 1
#endif
        iIndex--;
        pos = pos->pDisplayPrev; 
    } 
    return true;
}
int MouseRegion::AddRegion(const SizeRect & sizeRect,const kwOpt & keywords)
{
    return AddRegion(sizeRect.loc,sizeRect.size,keywords);
}

int MouseRegion::AddRegion(const POINT pLoc,const SIZE szSize,const kwOpt & keywords) 
{
    if (m_iActiveRegions >= kMaxMouseRegions) return -1; 

    // Stop certain processes -- Mouse dragging, etc.

    auto keys = keywords.FillKeyValues();        // Link together all

    auto iUserID = kwType::ckw::Get(keys._ID,0); 

    auto opBoundingSR = kwType::ckw::Get(keys._BoundingSizeRect2);
    if (!opBoundingSR && m_bDefaultBoundBox) opBoundingSR = m_srDefaultBoundBox;

    EndMouseDrag();

    Region rg; 
    using fType = decltype(rg.fWidth); 

    rg.bBoundtoWindow        = kwType::ckw::Get(keys._mouse_region_key_mrBindtoWindow,m_bDefaultBindtoWindow); 
    rg.fx                    = (fType) pLoc.x;
    rg.fy                    = (fType) pLoc.y;
    rg.fWidth                = (fType) szSize.cx;
    rg.fHeight               = (fType) szSize.cy;
    rg.ulInternalID          = m_ulCurID++;
    rg.iUserID               = iUserID;

    rg.fxOrg = rg.fx;
    rg.fyOrg = rg.fy; 
    rg.iPhysicalIndex       = m_iActiveRegions;

    rg.stAutoDraw           = m_stAutoDrawDefaults; // Set default Auto Draw Components (to be used later)
    rg.bAutoDraw            = true; // $$ Temporary
    m_iActiveRegions++;

    CString csAutoDraw = kwType::ckw::Get(keys._mouse_region_key_mrAutoDraw,(const char *) nullptr); 

    if (!csAutoDraw.isEmpty())
    {
        rg.stAutoDraw = ParseAutoDrawString(*csAutoDraw); 
    }

    m_vRegions.emplace_back(rg); 

    // Add this to the bottom of the display order. 

    auto * pv = &m_vRegions[m_iActiveRegions-1];

    pv->pDisplayPrev = nullptr;
    if (m_pDisplayTop) m_pDisplayTop->pDisplayPrev = pv; 
    pv->pDisplayNext = m_pDisplayTop;
   // if (m_pDisplayTop) pv->pDisplayPrev = pv;

    m_pDisplayTop = pv; 
    


    if (opBoundingSR) SetBoundBox(rg.ulInternalID ,*opBoundingSR);
  //  if (opBoundingR ) SetBoundingRect    (rg.ulInternalID ,*opBoundingR );

    UpdatePoints();  // Reset or Add any highlighting, etc. for new addition

    return rg.ulInternalID; 
}
bool MouseRegion::SetAutoDraw(const char * sAutoDraw)
{
    if (!this) return false;
    if (EmptyString(sAutoDraw))
    {
        m_stAutoDrawDefaults = m_stOrgAutoDrawDefaults;
    }

    if (!EmptyString(sAutoDraw)) m_stAutoDrawDefaults = ParseAutoDrawString(sAutoDraw);
    for (int i=0;i<m_iActiveRegions;i++) m_vRegions[i].stAutoDraw = m_stAutoDrawDefaults;
    return true;
}

bool MouseRegion::SetAutoDraw(int iIndex,const char * sAutoDraw)
{
    if (!this || iIndex < 0 || iIndex >= m_iActiveRegions) return false;
    if (EmptyString(sAutoDraw))
    {
        m_vRegions[iIndex].stAutoDraw = m_stAutoDrawDefaults;
        return true;
    }

    m_vRegions[iIndex].stAutoDraw = ParseAutoDrawString(sAutoDraw,&m_vRegions[iIndex].stAutoDraw);
    return true;
}

MouseRegion::AutoDrawDef MouseRegion::ParseAutoDrawString(const char * sString,AutoDrawDef * stDefault)
{
    auto stDefaults = stDefault ? *stDefault : m_stAutoDrawDefaults;
    if (!sString || !*sString) return stDefaults; 

    // Parse through the string to change any defaults. 
    //
    // --> There really is no error-handling here, as it's all pass-through
    //
    //     However, there may be some facility later to help with error-processing and with Python
    //     (i.e. putting out parsing errors to the Sage Debug window or Python warnings, respectively)

    CString csString = sString;

    int iLength = (int) strlen(sString); 
    
    csString.MakeLowerCase();

    auto vStrings = csString.Split(';'); 

    enum class AutoDrawCommands
    {
        Shape,
        Color,
        Size,
        PenColor,
        PenSize,
        NotFound,
        Default,
        Disabled,
        Enabled,
        None,
    };
    std::map<std::string,AutoDrawCommands> mapAutoDrawCommands = 
    {
        { "shape"           , AutoDrawCommands::Shape  },
        { "shapes"          , AutoDrawCommands::Shape  },
        { "color"           , AutoDrawCommands::Color  },
        { "colors"          , AutoDrawCommands::Color  },
        { "pencolor"        , AutoDrawCommands::PenColor     },
        { "pencolors"       , AutoDrawCommands::PenColor     },
        { "bordercolor"     , AutoDrawCommands::PenColor     },
        { "bordercolors"    , AutoDrawCommands::PenColor     },
        { "pensize"         , AutoDrawCommands::PenSize     },
        { "pensizes"        , AutoDrawCommands::PenSize     },
        { "bordersize"      , AutoDrawCommands::PenSize     },
        { "bordersizes"      , AutoDrawCommands::PenSize     },
        { "size"            , AutoDrawCommands::Size   },
        { "sizes"           , AutoDrawCommands::Size       },
        { "hide"            , AutoDrawCommands::Disabled       },
        { "hidden"          , AutoDrawCommands::Disabled       },
        { "disabled"        , AutoDrawCommands::Disabled       },
        { "disable"         , AutoDrawCommands::Disabled       },
        { "off"             , AutoDrawCommands::Disabled       },
        { "enable"          , AutoDrawCommands::Enabled       },
        { "on"              , AutoDrawCommands::Enabled       },
        { "default"         , AutoDrawCommands::Default       },
        { "none"            , AutoDrawCommands::None       },
    };

    std::map<std::string,ShapeType> mapShapeType = 
    {
        { "square"      , ShapeType::Rectangle  },
        { "rectangle"   , ShapeType::Rectangle  },
        { "circle"      , ShapeType::Circle     },
  //      { "triangle"    , ShapeType::Triangle   },
        { "none"        , ShapeType::None       },
    };

    int iNumStrings = (int) vStrings.size();

    CString csTemp = sString;

    auto SkipAssignment = [](char * & sString)
    {
        if (!sString || !*sString) return;
        while (*sString <= 0x20 || *sString == '=') sString++;
    };

    auto FindSkipSub = []( char * & sString,const char cFind)
    {
        if (!sString || !*sString) return CString();;

        while(*sString && *sString <= 0x20) sString++;
        auto sOrgString = sString;
        while(*sString && *sString != cFind)
        {
            auto c = *sString++;
            if (c == '{') { while(*sString && *sString != '}') sString++; }
            else
            if (c == '"') { while(*sString && *sString != '"') sString++; }
            else
            if (c == '\'') { while(*sString && *sString != '\'') sString++; }
            else
            if (c == '[') { while(*sString && *sString != ']') sString++; }
            else
            if (c == '(') { while(*sString && *sString != ')') sString++; }
        }

        return CString().CopyString(sOrgString,(int) (sString-sOrgString)).StripWhiteSpace(); 
    };
    for (int i=0;i<iNumStrings;i++)
    {
        bool bBadString = false;

        auto ePreShape = ShapeType::Unknown;;
        CRgbColor cPreRgbColor = RgbVal::Undefined; 


        CString cString = vStrings[i];
        cString.SimplifyWhitespace();   // keep some whitespace so we can work with people forgetting '=', e.g. "shape square" vs. "shape = square"

        cString.Remove('_');    // Desnake
        auto * sPlace = *cString;   // If we find a name
        auto * sOrgPlace = sPlace; // If we don't find a valid name. 

        auto cVar = cString.GetVarName(sPlace); 

        // If we don't find something that qualifies as a 'var', then the entire line is not useful
        // as we don't have any type of elements here that would mean anything (just yet anyway)
         
        if (cVar.isEmpty())
        {
            // it's possible we have a color starting with '{')
            if (*sPlace == '{')
            {
                cVar = FindSkipSub(sPlace,',');
            }
        }
        if (cVar.isEmpty()) continue; // Just skip it, since we don't intend to generate errors. 
        auto iter = mapAutoDrawCommands.find(*cVar);

        // If we found a command, then we can just move to the assignment, with out without '=' 
        auto eCommand = AutoDrawCommands::NotFound; 

        if (iter != mapAutoDrawCommands.end())
        {

            eCommand = (*iter).second;
            // skip past any '='

            SkipAssignment(sPlace); 

#if 0
            auto sEnd = CString::SkipAlphaNumeric(sPlace);

            CString sTemp;
            sTemp.CopyString(sPlace,(int) (sEnd-sPlace)); 
            sEnd = sPlace; 

            // Look for shape in the map

            sTemp.MakeLowerCase();
            if (ePreShape != ShapeType::Unknown)
            {
                // $$ set shape here

                // No specified standalone shape here. 
            }
            else
            {
                auto iter = mapShapeType.find(*sTemp); 
                if (iter != mapShapeType.end())
                {
                    auto eNewShape = (*iter).second;
                
                    // Not currently reacting to 'none' shape

                    if (eNewShape != ShapeType::None)
                    {
                        // $$ set shape for all modes here. 
                    }
                    else
                    {
                        // We have an error here, to which we could react, but probably just do nothing,
                    }
                }
            }
#endif
        }

        
        if (eCommand == AutoDrawCommands::NotFound)
        {
            
            CString csString = sPlace;
            csString.RemoveWhitespace(); 


                // Look for a set shape names. 

            auto iter = mapShapeType.find(*cVar);

            if (iter != mapShapeType.end())
            {
                eCommand = AutoDrawCommands::Shape;
                ePreShape = (*iter).second;
            }
            else
            {
                // If we don't have set of shape names, look for a color
                //
                // For a color, we need to isolate by skipping sub blocks. 
                // e.g. red(128) or {1,2,3}(56), so we need to skip 
                // () and {} blocks.  In our case, any generic block is fine. 
                // 
                // We're looking for a ',' outside of sub blocks. 
                //
                // Something like FindNoSub(','), which will generically skip past "",'',{},(),'[]' while looking for a character. 
                // 
                //  
      
                auto sSavePlace = sOrgPlace;
                auto csTemp = FindSkipSub(sOrgPlace,','); 
                csTemp.RemoveWhitespace();
      
                if (!csTemp.isEmpty())
                {
                    sPlace = sOrgPlace;
                    // Attempt to resolve a color

                    auto cRgb = CRgbColor(csTemp); 

                    if (!cRgb.rgbColor.Undefined())
                    {
                       eCommand = AutoDrawCommands::Color; 
                       cPreRgbColor = cRgb;
                    }
                    else
                    {
                       int gg = 1;
                    }
                }
                else
                {
                    int gg = 1;
                }
                int gg = 1;
            }
        }

        int gg2 = 1;
        // if we don't find (or have not interpolated) a command, then we are in error. 
        //
        // If we have a command, then we parse each command as we can. 
        //
        // For Shapes and Colors, we may have an incoming shape or color. 
        //
        if (eCommand != AutoDrawCommands::NotFound)
        {
            if (eCommand == AutoDrawCommands::Disabled)
            {
                stDefaults.bDisabled = true;
            }
             if (eCommand == AutoDrawCommands::Enabled)
            {
                stDefaults.bDisabled = false;
            }
            if (eCommand == AutoDrawCommands::Shape)
            {
                // Look for just one shape to describe all modes. 
                // 
                // none and default are looked at, but really have no effect, since there is not point to them. 
                // --> default is just a "no entry" and "none" means no autodraw, for which the user could just kill the auto draw. 

                if (ePreShape != ShapeType::Unknown)
                {
                    stDefaults.eShape = ePreShape;
                }
                else
                {
                    auto sTemp = CString::SkipAlphaNumeric(sPlace);
                    CString cs;
                    cs.RemoveWhitespace();  // Should have none, but you never know
                    cs.CopyString(sPlace,(int) (sTemp-sPlace)); 
                    auto iter = mapShapeType.find(*cs);

                    if (iter != mapShapeType.end()) stDefaults.eShape = (*iter).second;
                }
            }

            if (eCommand == AutoDrawCommands::PenSize)
            {
                CString csString = sPlace;
                csString.RemoveWhitespace();
                sPlace = *csString;

                int iSizes = 0;
                do
                {
                    if (*sPlace != ',')
                    {
                        if (std::isalpha(*sPlace))
                        {
                            bBadString = true;
                            continue;
                        }
 
                        auto csSizeString = FindSkipSub(sPlace,',');
                        if (!csSizeString.isEmpty())
                        {
                           stDefaults.fPenSize[iSizes] = (std::remove_all_extents<decltype(stDefaults.fPenSize)>::type) atof(*csSizeString);
                        }
                        iSizes++;
                        if (*sPlace == ',' && (std::isalpha(sPlace[1]))) sPlace++;    // natural comma as opposed to empty comma
                    }
                    else
                    {
                        sPlace++;   // Move to next size
                        iSizes++;
                    }
                } while (!bBadString && *sPlace && iSizes < 3);

                if (iSizes)
                {
                    if (iSizes < 2) stDefaults.fPenSize[1] = stDefaults.fPenSize[0];
                    if (iSizes < 3) stDefaults.fPenSize[2] = stDefaults.fPenSize[1];
                }
            }
            if (eCommand == AutoDrawCommands::Color || eCommand == AutoDrawCommands::PenColor)
            {
                bool bPenColor = eCommand == AutoDrawCommands::PenColor;
                // Look for up to three sucessive colors
                // including default none and a ',' to designate default color. 


                CString csString = sPlace;
                csString.RemoveWhitespace();
                sPlace = *csString;

                // Look for up to 3 colors. 

                int iColors = 0;

                if (cPreRgbColor.rgbColor != RgbVal::Undefined)
                {
                    if (*sPlace == ',' && (std::isalpha(sPlace[1]))) sPlace++;
                    stDefaults.rgbColor[iColors]      = RgbA(cPreRgbColor,cPreRgbColor.iAlpha[0]); 
                    iColors++;

                    // Set color [iColor] here.
                }
                // If we have an incoming color, we are either at the end or at a commma, and we want to set the colors to 1, since we already have one. 


                do
                {
                    if (!*sPlace) break;
                    // If sPlace is at a ',', then we're signifying default color. 

                    if (*sPlace != ',')
                    {
                        auto sOrgPlace = sPlace;
                        auto csColorString = FindSkipSub(sPlace,',');
                        if (!strcmp(*csColorString,"none"))
                        {
                           // For now, just set the color to Black(0)
                           
                            bPenColor   ? stDefaults.rgbPenColor[iColors]   = SageColorA::Black(0)
                                        : stDefaults.rgbColor[iColors]      = SageColorA::Black(0); 
                        }
                        else
                        if (!strcmp(*csColorString,"default"))
                        {
                            // Do nothing
                        }
                        else
                        {
                            CRgbColor cRgbColor = *csColorString;
                            
                            if (cRgbColor.rgbColor != RgbVal::Undefined)
                            {
                                bPenColor   ? stDefaults.rgbPenColor[iColors]   = RgbA(cRgbColor,cRgbColor.iAlpha[0])
                                            : stDefaults.rgbColor[iColors]      = RgbA(cRgbColor,cRgbColor.iAlpha[0]); 
                            }
                            else
                            {
                                sPlace = sOrgPlace;
                                bBadString = true;
                                continue;
                            }
                            // Look for a valid color.
                        }
                        iColors++;  // Advance color count regardless.  If there was an error, it is handled elsewhere
                        if (*sPlace == ',' && (std::isalpha(sPlace[1]))) sPlace++;    // natural comma as opposed to empty comma
                    }
                    else 
                    {
                        sPlace++;   // Move to next color
                        iColors++;
                    }
                } while (!bBadString && *sPlace && iColors < 3);

                // Assign subsequent colors to the same color if they were left blank
                if (iColors)
                {
                    if (bPenColor)
                    {
                        if (iColors < 2) stDefaults.rgbPenColor[1] = stDefaults.rgbPenColor[0];
                        if (iColors < 3) stDefaults.rgbPenColor[2] = stDefaults.rgbPenColor[1];
                    }
                    else
                    {
                        if (iColors < 2) stDefaults.rgbColor[1] = stDefaults.rgbColor[0];
                        if (iColors < 3) stDefaults.rgbColor[2] = stDefaults.rgbColor[1];
                    }
                }
            }
        }

        int gg = 1;

        if (*sPlace == ',' && std::isalpha(sPlace[1]))
        {
            sPlace++;
            bBadString = true;
        }

        if (bBadString && sPlace && std::isalpha(*sPlace))
        {
            vStrings[i] = sPlace;
            i--;
        }
       // break;
    }
    int gg = 1;
    return stDefaults;
}

bool MouseRegion::SetOptions(const kwOpt & keywords)
{
    if (!this) return false;
    auto keys = keywords.FillKeyValues();        // Link together all
    m_bSelectionAutoFocus = kwType::ckw::Get(keys._SelectionAutoFocus,m_bSelectionAutoFocus);
    m_bAutoGradient = kwType::ckw::GetBool(keys._AutoGradientBool);
    CString csAutoDraw = kwType::ckw::Get(keys._mouse_region_key_mrAutoDraw,(const char *) nullptr); 
    m_bDefaultBoundBox = false;
    if (auto opBoundingSR = kwType::ckw::Get(keys._BoundingSizeRect2))
    {
        m_bDefaultBoundBox = true;
        m_srDefaultBoundBox = *opBoundingSR;
    }

    
    kwType::ckw::GetBool_if(keys._mouse_region_key_mrBindtoWindow,m_bDefaultBindtoWindow); 
    if (!csAutoDraw.isEmpty())
    {
        m_stAutoDrawDefaults = ParseAutoDrawString(*csAutoDraw); 
    }
    return true;
}
MouseRegion::MouseRegion(CWindow & cWin,const kwOpt & keywords) : m_cWin(cWin)
{
    m_bValid = m_cWin.isValid(); 
    m_vRegions.reserve(kMaxMouseRegions); 
    SetOptions(keywords);
}

std::optional<int> MouseRegion::HighlightChanged(Peek peek)
{
    auto bChanged = m_stEvents.bNewHighlight; 
    if (peek == Peek::No) m_stEvents.bNewHighlight = false;

    return bChanged ? m_ulCurHighlight : (std::optional<int>) std::nullopt;
}

std::optional<int> MouseRegion::SelectionChanged(Peek peek)
{
    auto bChanged = m_bSelectPending; 
    if (peek == Peek::No) m_bSelectPending = false;

    return bChanged ? m_ulCurSelected : (std::optional<int>) std::nullopt;
}

std::optional<MouseRegion::DragInfo> MouseRegion::MouseDragEvent(Peek peek)
{
    if (!this || m_vRegions.size() == 0 || m_iCurDragID < 0 || m_iCurDragID >= m_iActiveRegions) return std::nullopt;
    auto bChanged = m_stEvents.bDragEvent; 
    if (peek == Peek::No) m_stEvents.bDragEvent = false;

    // If we're a point type, adjust mouse point outputs accordingly. 

    auto cfCurDrag      = m_cfCurDrag;
    auto cfLastDrag     = m_cfLastDrag;
    auto cfStartDrag    = m_cfStartDrag;

    auto & v = m_vRegions[m_iCurDragID];

    auto cfOOBDrag      = v.cfOOB;

    using ftype = decltype(v.fWidth);
    using cftype = decltype(cfCurDrag);
    if (v.bPointType)
    {
        cfCurDrag   += cftype{v.fWidth/(ftype) 2.0,v.fHeight/(ftype) 2.0};
        cfLastDrag  += cftype{v.fWidth/(ftype) 2.0,v.fHeight/(ftype) 2.0};
        cfStartDrag += cftype{v.fWidth/(ftype) 2.0,v.fHeight/(ftype) 2.0};
        cfOOBDrag   += cftype{v.fWidth/(ftype) 2.0,v.fHeight/(ftype) 2.0};
    }

    DragInfo drag =  { m_iCurDragID, cfCurDrag, cfOOBDrag, cfLastDrag, cfStartDrag, cfCurDrag, cfOOBDrag, cfLastDrag, cfStartDrag };
    return bChanged ? drag : (std::optional<DragInfo>) std::nullopt;
}

std::optional<MouseRegion::DragInfo> MouseRegion::MouseDragEnded(Peek peek)
{
    if (!this || m_vRegions.size() == 0 || m_iCurDragID < 0 || m_iCurDragID >= m_iActiveRegions) return std::nullopt;
    auto bChanged = m_stEvents.bDragEventEnd; 
    if (peek == Peek::No) m_stEvents.bDragEventEnd = false;

    auto cfCurDrag      = m_cfCurDrag;
    auto cfLastDrag     = m_cfLastDrag;
    auto cfStartDrag    = m_cfStartDrag;

    auto & v = m_vRegions[m_iCurDragID];
    using ftype = decltype(v.fWidth);

    if (v.bPointType)
    {
        cfCurDrag   += decltype(cfCurDrag){v.fWidth/(ftype) 2.0,v.fHeight/(ftype) 2.0};
        cfLastDrag  += decltype(cfCurDrag){v.fWidth/(ftype) 2.0,v.fHeight/(ftype) 2.0};
        cfStartDrag += decltype(cfCurDrag){v.fWidth/(ftype) 2.0,v.fHeight/(ftype) 2.0};
    }



    DragInfo drag =  { m_iCurDragID, cfCurDrag, cfLastDrag, cfStartDrag, cfCurDrag, cfLastDrag, cfStartDrag };

    return bChanged ? drag : (std::optional<DragInfo>) std::nullopt;
}

}; // namespace Sage