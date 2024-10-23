#pragma once
namespace Sage
{

    class CWindow;

    namespace kw
    {
        extern const kwType::ckw none;
    }
   
    using kwOpt = kwType::ckw;


// Mouse Region Class 
//
// This class gives the ability to track multiple mouse points or regions, and can optionally Auto Draw the mouse regions for ease-of-use.
//
// In many programs, mouse areas are use to move around object.  The MouseRegion class allows easy tracking of the areas, eliminating most or all of the need to
// manage these areas in the program's code.
//
//
// Example: Moving Bezier Points in a curve
// 
// A bezier curve with N points can use the MouseRegion to define the points and allow the user to move the points easily. 
// 
// The bezier curve points and control points can be added and moved by the user.  These can be added in the same Mouse Region Object or in multiple
// Mouse Region Object (e.g. One Mouse Region object for the curve points and another for the control points)
//
// Optimally, only one Mouse Region object is defined for both curve points and control points, because:
// 
//     1. Points can be added wholesale, but also individually.  Thus, curve points and control points can be defined as different sizes
//        (e.g. smaller points for control points and larger ones for curve points, or vice-versa)
//     2. Each Point added can have a different size or set of boundaries, as well as other characteristics.
//     3. In the case of the Auto Draw functionality (which draws the mouse regions with one function call), mouse region display
//        characteristics can be set as a group or individually.
// 
//        In the case of curve points vs. control points, this means that the two different types can have different displays,
//        including shape (e.g. square vs. circle), color, size, etc.
//
// Other Comments (Work in Progress)
//
//
// 1. Points vs. Regions.
//    a. Points.  Points can be added via AddPoint() or AddPoints() calls.  Points are considered a central point with an width and height radius from the center.
//    b. Regions.  Regions are considered specific regions in an explicit rectangle, using AddRegion() or AddRegions()  
//
//    The difference is that the size of the point (i.e. width and height) extends left and right, up and down from the point specified,
//    Where the point specified in the AddRegion() call is the upper-left of the rectangle. 
//
// 2. AutoDraw.  Auto Draw is optional.  Call AutoDraw() to draw all mouse points.
//    AutoDraw for specific Mouse Points or Regions can be disbled by setting kw::AutoDraw("off")/AutoDraw("disabled")
// 
// 3. TBD -- See comments over functions in the class interface
//
class MouseRegion
{
private:
    // Shapes for Auto Draw.
    // 
    // Currently Circle and Rectangle/Square are supported.  Later, other shapes may be added, such as Triangles, or whatever makes sense.
    //  
    enum class ShapeType
    {
        Rectangle,
        Circle,
        None,
        Unknown,
    };

    // AutoDraw profile stored generically and for each point/region
    //
    struct AutoDrawDef
    {
        ShapeType eShape;           // Shape (e.g. circle, square, etc.)
        RgbColorA rgbColor[3];      // main color - Normal, highlighted, selected
        RgbColorA rgbPenColor[3];   // Pen Color (if pen size > 0) - Normal, highlighted, selected
        float fPenSize[3];          // PenSize - 0 for no outline (default) - Normal, highlighted, Selected
        bool bDisabled;             // Auto Draw is disabled for this point or region.
    };

    // Auto Draw Defaults until changed for all points generically or for individual points.
    //
    static inline AutoDrawDef m_stAutoDrawDefaults = 
    {
        ShapeType::Rectangle,                                                                   // Default Shape
        { SageColorA::Cyan(170), SageColorA::LightBlue(212), SageColorA::LightBlue(212) },      // Default Internal Colors
   //     { SageColorA::Cyan(212), SageColorA::Cyan(212), SageColorA::Cyan(212) },      // Default Internal Colors
        { SageColorA::White(200), SageColorA::White(200), SageColorA::White(200) },             // Default PenColors
        0.0f,0.0f,0.0f,                                                                         // Default PenSize (0 = no pen)
        false                                                                                   // Auto Draw for point/region is disabled when true
    };

    static inline AutoDrawDef m_stOrgAutoDrawDefaults = m_stAutoDrawDefaults;                   // Set initial defaults

    static constexpr int kMaxMouseRegions = 1000;                                               // Current maxium number of mouse points.
                                                                                                // Later to be changed to something more dynamic.
    // Tracking the mouse events so we know what to report at any given time.
    //  
    struct EventTrack_t
    {
       int ulMouseMoved;
       int ulClicked;           // Mouse was clicked on a point or region
       int ulHighlight;         // Mouse is over a point or region
       int ulSelected;          // Mouse is pressed and moving a point or region
       int ulRegionMoved;       // Not sure how this is used.

       bool bNewHighlight;      // Highlight has changed from previous highlight
       bool bDragEvent;         // Mouse is currently moving a mouse point or region
       bool bDragEventEnd;      // Mouse button was unpressed, ending the movement of a point or region
    };

    CWindow & m_cWin;           // Attached Window for Mouse Region
    
    EventTrack_t m_stEvents{}; 

    // Storage for each mouse point or region added. 
    //
    class InternalRegion
    {
    public: 
        bool    bDontPromote            = false;
        bool    bHighlighted            = false;
        bool    bPressed                = false;
        bool    bOutlineHighlighted     = false;
        bool    bOutlinePressed         = false; 
        bool    bBoundingRect           = false;
        bool    bBoundtoWindow          = false;
        bool    bOOB                    = false;

        SizeRect srBoundingRect{};

        float   fx                      = 0;        // Current inbound X and Y (i.e. if boundaries have been established and point has been stopped at the edge)
        float   fy                      = 0;
        bool    bisOOB                  = false;    // Mouse is dragging an object that has hit its boundary, and the mouse is exceeding this boundary
        CfPointf cfOOB                  = {};       // Point where object would be without its boundary (or where the mouse is in relation to the stopped object that is OOB)
        float   fxOrg                   = 0;        // Starting points when the user started moving a mouse point or region
        float   fyOrg                   = 0;
        float   fWidth                  = 0;        // Width and Height of mouse region or point
        float   fHeight                 = 0;
        int     iUserID                 = 0;        // UserID given to the individual point with kw::ID()
        bool    bPointType              = false;    // When true, this region was added as a "point" with a radius of width & height.  Otherwise it is an explicit region.
        
        InternalRegion * pDisplayPrev           = nullptr;  // Previous Point or Region in the display order (nullptr means this point is at the top)
        InternalRegion * pDisplayNext           = nullptr;  // Next Point or Region in display order (nullptr means its at the bottom)
        CString csUserID{};                         // $$ This is probably deprecated
        int ulInternalID                = 0;        // Internal ID tracking
        int iPhysicalIndex              = 0;        // Internal ID tracking ($$ This is deprecated)
        bool bAutoDraw                  = false;    // Does this Point have an AutoDraw display when AutoDraw() is called?
        bool bDisplayOOB                = false;    // When AutoDraw() is called and this is true, this means the object is outside of the window and would 
                                                    // be invisible and impossible to re-grab.  AutoDraw() keeps the ojbect slightly in the window to allow it to
                                                    // to be grabbed and moved -- this does not reflect the point position, and just limits the display so it doesn't
                                                    // completely disappear from the window.
        CfPointf cfDisplayOOB           = {};       // If we are out of bounds in the display, this is where to look when the user grabs the box/circle to move it
        AutoDrawDef stAutoDraw;                     // Auto Draw definitions for this point or region.
    };



    InternalRegion * m_pDisplayTop = nullptr;   // Top of display order, descending linked list. 
    InternalRegion * m_pDisplayBot = nullptr;   // Bottom of display order, descending linked list.  $$ not sure this is used any longer.

    CfPointf m_cfCurDrag{};         // Current Drag Position for point or region currently being moved.
    CfPointf m_cfLastDrag{};        // Previous Drag Position for point or region currently being moved.
    CfPointf m_cfStartDrag{};       // Original/Starting Drag Position for point or region currently being moved.

    int m_iCurDrag      = -1;   
    int m_iCurDragID    = 0;

    std::vector<InternalRegion> m_vRegions; // Region or Point storage

    bool    m_bDisabled             = false;
    bool    m_bValid                = false;
    int     m_iActiveRegions        = 0;
    int     m_iLastClickCount       = 0; // Counters are used to keep track of whethe or not 
    int     m_iLastMouseMoveCount   = 0; // mouse status has changed, without affecting the stored mouse status itself.
    int     m_iLastUnclickCount     = 0;

    bool    m_bPreprocess           = true;     // Useful for startup/changes to look for/exclude mouse over highlighted region
    bool    m_bDefaultBindtoWindow  = false;    // Make the window the bounding box (this is dynamic and will change with window-size changes)
    int m_ulCurID         = 0;
    int m_ulCurHighlight  = -1;
    int m_ulCurSelected   = -1;

    bool m_bKillPassedEvents    = true;
    bool m_bMouseDown           = false; 
    bool m_bEventReady          = false;
    bool m_bMovePending         = false; 
    bool m_bSelectPending       = false; 
    bool m_bAutoGradient        = false;
    bool m_bDefaultBoundBox     = false;

    SizeRect m_srDefaultBoundBox  = {};
    CfPointf m_cfPending;


    bool m_bSelectionAutoFocus = true; 

    int m_iLastSelectedDisplay = -1;        // When not -1, this is the last selected value that can be obtained via GetLastSelected()
                                            // This can be useful to track which item has a permanent status/highlight to it via being selected.
                                            // This value is valid when a point or region is not being moved.  When a point or region is being moved
                                            // this value is the index of that region/point, the same as m_iSelected.
                                            //
                                            // m_iSelected returns to -1 when the mouse is released, signifying that there is no object currently being moved.
                                            // m_iLastSelectedDisplay essentially keeps a static memory of the last selected item.
                                            //
                                            // Use ResetSelected() return this value to -1 (i.e. no last selected)

    int m_iLastHighlight = -1;              // Same thing with the above m_iLastSelectedDisplay, except for highlighted item. 

    // I have no idea what these three functions do.  They seemed important at the time, so I am sure I will return to them

    POINT AdjusttoSizeRect(int iRegion,SizeRect rRect);
    POINT AdjusttoRect(int iRegion,RECT rRect);
    POINT AdjusttoSizeRect(InternalRegion & r,SizeRect rRect,bool * bAdjusted = nullptr);

    void EndMouseDrag();
    AutoDrawDef ParseAutoDrawString(const char * sString,AutoDrawDef * stDefault = nullptr);

    InternalRegion * FindRegion(int iRegion);       // $$ Deprecated
    bool BringtoTop(InternalRegion & r);            // Brings the point/region to the top of the display order.
    
    MouseRegion(const MouseRegion &p);	    // Privatize copy-constructor to keep copies from happening
                                            // more specifically, to cause a compiler error on auto mr = cWin.GetMouseRegion(), vs. 
                                            // auto& mr = cWin.GetMouseRegion;
     bool _SetRegion(int iIndex,POINT pLoc,SIZE szSize,bool bPointType);
public:

     // Returned Point structure for each mouse region or point.
     //
     // When the Region has been defined as a 'point', the X & Y "loc" position reflects its central "point" position.
     // When the Region has been defined as a Region, the X & Y "loc" position is the upper-left corner of the region rectangle.
    struct Point
    {
        CPoint      loc;            // Current location (either "point" location or upper-left of rectangular region)
        CfPointf    cfLoc;          // floating-point version of Point location
        CPoint      OOBLoc;         // Out-of-bounds position.  This is the position of the Region or Point if it had no boundaries
                                    // This can be used to determine when the mouse dragging this point has exceeded the bounds, and by how far.
                                    // --> If no boundaries have been established for the point or region, then loc and OOBLoc are the same value.
        CfPointf    cfOOBLoc;       // Floating-Point version of the OOB location.
        int         userID;         // UserID sent via kw::ID() (0 if no ID was sent)
        int         index;          // index of point in the array -- not as added, as this can change when points are removed and inserted above this point.
        bool        bValid;         // ** Important:  In some cases, such as GetSelected(), bValid will be false if there was no selected point.
                                    //                There are some cases where a call may ask for an index or other status that is not valid.
                                    //                This will always return the first point in the array, but set bValid = false to indicate a failure.
                                    //
                                    // For most uses, the returned point is assumed to be valid.  bValid can be checked for a "true" status just to be sure.
        SizeRect    physRegion;     // Physical region of mouse region or point -- when it is a region-style point, this is loc and size.
                                    // when this is a point-style point, this is the adjusted loc (for RadiusX and RadiusY) and size to form the 
                                    // physical region
    };
    
    struct Region
    {
        SizeRect    region;         // Current region
        SizeRect    regionOOB;      // Current region
        int         userID;         // UserID sent via kw::ID() (0 if no ID was sent)
        int         index;          // index of point in the array -- not as added, as this can change when points are removed and inserted above this point.
        bool        bValid;         // ** Important:  In some cases, such as GetSelected(), bValid will be false if there was no selected point.
                                    //                There are some cases where a call may ask for an index or other status that is not valid.
                                    //                This will always return the first point in the array, but set bValid = false to indicate a failure.
                                    //
                                    // For most uses, the returned point is assumed to be valid.  bValid can be checked for a "true" status just to be sure.
     };
    
    struct DragInfo
    {
        int iIndex;
        CPoint pCur;
        CPoint pCurOOB;
        CPoint pLast;
        CPoint ptart;
        CfPointf cfCur;
        CfPointf cfCurOOB;
        CfPointf cfLast;
        CfPointf cStart;
    };
public:

    /// <summary>
    /// Constructor.  Use kw:: Keywords to set some controls.
    /// <para></para>
    /// - kw::AutoDraw() -- which can set different settings for the auto draw. Until documentation is more filled out, see examples of kw::AutoDraw() usage.
    /// <para></para>
    /// - kw::BoundBox() -- Sets the boundaries/limitations for the region or point.  Input is { left,right,width,height }
    /// <para></para>
    /// - Other keywords currently TBD.
    /// </summary>
    /// <param name="cWin"> - Window to use with the Mouse Regions</param>
    /// <param name="keywords"> - [optional] various keywords</param>
    MouseRegion(CWindow & cWin,const kwOpt & keywords = kw::none);


    __forceinline Region GetRegion(int iIndex)
    {
        if (!this || iIndex < 0 || iIndex > m_iActiveRegions)
        {
            Region region{};
            region.bValid = false;
            return region;
        }

        Region r;   // No need to initialize since we're going to fill all of it. 

        auto& v = m_vRegions[iIndex];

        r.userID        = v.iUserID;
        r.index         = v.ulInternalID;
        r.region        = { (int) v.fx, (int) v.fy, (int) v.fWidth, (int) v.fHeight };
        r.regionOOB     = { (int) v.cfOOB.x, (int) v.cfOOB.y, (int) v.fWidth, (int) v.fHeight };
        r.bValid        = true;
        return r;
    }
    __forceinline Point GetPoint(int iIndex)
    {
        if (!this || iIndex < 0 || iIndex > m_iActiveRegions)
        {
            Point pPoint{};
            pPoint.bValid = false;
            return pPoint;
        }

        Point p;   // No need to initialize since we're going to fill all of it. 

        auto& r = m_vRegions[iIndex];
        using ftype = decltype(r.fWidth);
        using cftype = decltype(p.cfLoc);

        CfPointf fAdjust = r.bPointType ? cftype{r.fWidth/(ftype) 2.0,r.fHeight/(ftype) 2.0} : cftype{(ftype) 0,(ftype) 0};
   
        p.cfLoc         = cftype{r.fx,r.fy} + fAdjust;
        p.loc           = p.cfLoc;  
        p.cfOOBLoc      = r.cfOOB + fAdjust;
        p.OOBLoc        = p.cfOOBLoc; 
        p.userID        = r.iUserID;
        p.index         = r.ulInternalID;
        p.physRegion    = { (int) r.fx, (int) r.fy, (int) r.fWidth, (int) r.fHeight };
        p.bValid        = true;
        return p;
    }

    __forceinline Point GetSelectedPoint(bool bIncludeLastKnown = true)
    {
        // If there is nothing current selected (i.e. currently being moved), but there has been one selected and we're 
        // tracking static selected points, then we look at this point when nothing is current selected.
        // 
        // There should never be a case where the last selected display value is a different valid index than the currently selected value.
        //
        return GetPoint(bIncludeLastKnown ? m_ulCurSelected < 0 ? m_iLastSelectedDisplay : m_ulCurSelected : m_ulCurSelected);   // if nothing is selected, Invalid is returned.
    }

    /// <summary>
    /// This is a debug function that prints out the current display order for points/regions in the order if their index. 
    /// This will print the previous and next order and the UserID (if submitted with kw::ID()). 
    /// 
    /// This function can help in a debug process to ensure that the order of display precendence is as expected.
    /// This will print to the console window. 
    /// </summary>
    /// <returns></returns>
    bool PrintDisplayOrder();

    /// <summary>
    /// Add Regions of a std::array of SizeRect with N members.  This adds a set of regions (i.e. rectangle with { left, right, width height } ). 
    /// <para></para>
    /// See AddPoints() to add Points as opposed to Regions.  Also see AddRegion() to individually add regions.
    /// <para></para>
    /// --> Regions and Points may be added as std::array, std::vector, or pointer to an array of SizeRect values.
    /// </summary>
    /// <typeparam name="N"> - number of values in the std::array</typeparam>
    /// <param name="pArray"> - std::array of SizeRect of a specific size.</param>
    /// <param name="keywords"> - [optional] various useful keywords, such as kw::AutoDraw(), etc.</param>
    /// <returns>The integer currently returned has no value and is deprecated.</returns>
    template<int N>
    int AddRegions(std::array<SizeRect,N> pArray,const kwOpt & keywords = kw::none)
    {
        for (int i=0;i<N;i++)
        {
            AddRegion(pArray[i].loc,pArray[i].size,keywords);
        }
        return 0;
    }


    /// <summary>
    /// Add Regions of a std::vector of SizeRect.  This adds a set of regions (i.e. rectangle with { left, right, width height } ). 
    /// <para></para>
    /// See AddPoints() to add Points as opposed to Regions.  Also see AddRegion() to individually add regions.
    /// <para></para>
    /// --> Regions and Points may be added as std::array, std::vector, or pointer to an array of SizeRect values.
    /// </summary>
    /// <param name="pArray"> - std::vector of SizeRect.</param>
    /// <param name="keywords"> - [optional] various useful keywords, such as kw::AutoDraw(), etc.</param>
    /// <returns>The integer currently returned has no value and is deprecated.</returns>
    int AddRegions(const std::vector<SizeRect> pArray,const kwOpt & keywords = kw::none)
    {
        int N = (int) size(pArray);
        for (int i=0;i<N;i++)
        {
            AddRegion(pArray[i].loc,pArray[i].size,keywords);
        }
        return 0;
    }


    /// <summary>
    /// Add Regions from an Array of SizeRect.  This adds a set of regions (i.e. rectangle with { left, right, width height } ). 
    /// <para></para>
    /// See AddPoints() to add Points as opposed to Regions.  Also see AddRegion() to individually add regions.
    /// <para></para>
    /// --> Regions and Points may be added as std::array, std::vector, or pointer to an array of SizeRect values.
    /// </summary>
    /// <param name="pArray"> - pointer to an array of SizeRect.</param>
    /// <param name="iSize"> - number of values in the array.</param>
    /// <param name="keywords"> - [optional] various useful keywords, such as kw::AutoDraw(), etc.</param>
    /// <returns>The integer currently returned has no value and is deprecated.</returns>
    int AddRegions(const SizeRect * pArray,int iSize,const kwOpt & keywords = kw::none)
    {
        for (int i=0;i<iSize;i++)
        {
            AddRegion(pArray[i].loc,pArray[i].size,keywords);
        }
        return 0;
    }

    /// <summary>
    /// Adds a singular mouse region (or area) to the Mouse Region object. This aa region (i.e. rectangle with { left, right, width height } ). 
    /// <para></para>
    /// See AddPoint() to add a Point as opposed to Regions.  Also see AddRegions() to add multiple Regions simultaneously with arrays, std::array or std::vector.
    /// <para></para>
    /// --> SizeRect may be used instead of pLoc and szSize, e.g. AddRegion(MySizeRect) vs. AddRegion(loc,size);
    /// </summary>
    /// <param name="pLoc"> - Location of upper-left of the region. --> This may also be a SizeRect with the { left, top, width, height } values.</param>
    /// <param name="szSize"> - Width and Height of the region --> Not Used if the first parameter is a SizeRect</param>
    /// <param name="keywords"> - [optional] various useful keywords, such as kw::AutoDraw(), etc.</param>
    /// <returns>The integer currently returned has no value and is deprecated.</returns>
    int AddRegion(const POINT pLoc,const SIZE szSize,const kwOpt & keywords = kw::none); 

    /// <summary>
    /// Adds a singular mouse region (or area) to the Mouse Region object. This aa region (i.e. rectangle with { left, right, width height } ). 
    /// <para></para>
    /// See AddPoint() to add a Point as opposed to Regions.  Also see AddRegions() to add multiple Regions simultaneously with arrays, std::array or std::vector.
    /// <para></para>
    /// --> SizeRect may be used instead of pLoc and szSize, e.g. AddRegion(MySizeRect) vs. AddRegion(loc,size);
    /// </summary>
    /// <param name="pLoc"> - Location of upper-left of the region. --> This may also be a SizeRect with the { left, top, width, height } values.</param>
    /// <param name="szSize"> - Width and Height of the region --> Not Used if the first parameter is a SizeRect</param>
    /// <param name="keywords"> - [optional] various useful keywords, such as kw::AutoDraw(), etc.</param>
    /// <returns>The integer currently returned has no value and is deprecated.</returns>
    int AddRegion(const SizeRect & sizeRect,const kwOpt & keywords = kw::none); 

    /// <summary>
    /// Add Points of a std::array of SizeRect with N members.  This adds a set of points (i.e. SizeRect with center x,y point and radiusX and radius Y},
    /// e.g. AddPoints({x,y,radiusX,radiusY}) ). 
    /// <para></para>
    /// See AddRegions() to add Regions as opposed to Points.  Also see AddPoint() to individually add points.
    /// <para></para>
    /// --> Regions and Points may be added as std::array, std::vector, or pointer to an array of SizeRect values.
    /// </summary>
    /// <typeparam name="N"> - number of values in the std::array</typeparam>
    /// <param name="pArray"> - std::array of SizeRect of a specific size.</param>
    /// <param name="keywords"> - [optional] various useful keywords, such as kw::AutoDraw(), etc.</param>
    /// <returns>The integer currently returned has no value and is deprecated.</returns>
    template<int N>
    int AddPoints(std::array<SizeRect,N> pArray,const kwOpt & keywords = kw::none)
    {
        for (int i=0;i<N;i++)
        {
            AddPoint(pArray[i].loc,pArray[i].size,keywords);
        }
        return 0;
    }

    /// <summary>
    /// Add Points of a std::vector of SizeRect.  This adds a set of points (i.e. SizeRect with center x,y point and radiusX and radius Y},
    /// e.g. AddPoints({x,y,radiusX,radiusY}) ). 
    /// <para></para>
    /// See AddRegions() to add Regions as opposed to Points.  Also see AddPoint() to individually add points.
    /// <para></para>
    /// --> Regions and Points may be added as std::array, std::vector, or pointer to an array of SizeRect values.
    /// </summary>
    /// <param name="pArray"> - std::vector of SizeRect.</param>
    /// <param name="keywords"> - [optional] various useful keywords, such as kw::AutoDraw(), etc.</param>
    /// <returns>The integer currently returned has no value and is deprecated.</returns>
    int AddPoints(const std::vector<SizeRect> pArray,const kwOpt & keywords = kw::none)
    {
        int N = (int) size(pArray);
        for (int i=0;i<N;i++)
        {
            AddPoint(pArray[i].loc,pArray[i].size,keywords);
        }
        return 0;
    }
    /// <summary>
    /// Add Points from an array of SizeRect.  This adds a set of points (i.e. SizeRect with center x,y point and radiusX and radius Y},
    /// e.g. AddPoints({x,y,radiusX,radiusY}) ). 
    /// <para></para>
    /// See AddRegions() to add Regions as opposed to Points.  Also see AddPoint() to individually add points.
    /// <para></para>
    /// --> Regions and Points may be added as std::array, std::vector, or pointer to an array of SizeRect values.
    /// </summary>
    /// <param name="pArray"> - pointer to an array of SizeRect.</param>
    /// <param name="iSize"> - number of values in the array.</param>
    /// <param name="keywords"> - [optional] various useful keywords, such as kw::AutoDraw(), etc.</param>
    /// <returns>The integer currently returned has no value and is deprecated.</returns>
    int AddPoints(const SizeRect * pArray,int iSize,const kwOpt & keywords = kw::none)
    {
        for (int i=0;i<iSize;i++)
        {
            AddPoint(pArray[i].loc,pArray[i].size,keywords);
        }
        return 0;
    }

    /// <summary>
    /// Adds a singular mouse point to the Mouse Region object.  This adds a set of points (i.e. SizeRect with center x,y point and radiusX and radius Y},
    /// e.g. AddPoints({x,y,radiusX,radiusY}) ). 
    /// <para></para>
    /// See AddRegion() to add a Region as opposed to Points.  Also see AddPoints() to add multiple Points simultaneously with arrays, std::array or std::vector.
    /// <para></para>
    /// --> SizeRect may be used instead of pLoc and szSize, e.g. AddPoint(MySizeRect) vs. AddPoint(loc,size);
    /// </summary>
    /// <param name="pLoc"> - Location of point (i.e. center point). --> This may also be a SizeRect with the { x,y, radiusX, radiusY } values.</param>
    /// <param name="szSize"> - RadiusX and RadiusY of the point --> Not Used if the first parameter is a SizeRect</param>
    /// <param name="keywords"> - [optional] various useful keywords, such as kw::AutoDraw(), etc.</param>
    /// <returns>The integer currently returned has no value and is deprecated.</returns>
    int AddPoint(const POINT pLoc,const SIZE szSize,const kwOpt & keywords = kw::none); 
 
    /// <summary>
    /// Adds a singular mouse point to the Mouse Region object.  This adds a set of points (i.e. SizeRect with center x,y point and radiusX and radius Y},
    /// e.g. AddPoints({x,y,radiusX,radiusY}) ). 
    /// <para></para>
    /// See AddRegion() to add a Region as opposed to Points.  Also see AddPoints() to add multiple Points simultaneously with arrays, std::array or std::vector.
    /// <para></para>
    /// --> SizeRect may be used instead of pLoc and szSize, e.g. AddPoint(MySizeRect) vs. AddPoint(loc,size);
    /// </summary>
    /// <param name="pLoc"> - Location of point (i.e. center point). --> This may also be a SizeRect with the { x,y, radiusX, radiusY } values.</param>
    /// <param name="szSize"> - RadiusX and RadiusY of the point --> Not Used if the first parameter is a SizeRect</param>
    /// <param name="keywords"> - [optional] various useful keywords, such as kw::AutoDraw(), etc.</param>
    /// <returns>The integer currently returned has no value and is deprecated.</returns>
    int AddPoint(const SizeRect & sizeRect,const kwOpt & keywords = kw::none); 

    /// <summary>
    /// Sets the region location and size.  See SetPoint() to change a point-type mouse region.
    /// <para></para>
    /// --> This does not change the display order of the region/point.
    /// <para></para>
    /// --> The location given sets the upper-left of the region (see SetPoint() to set a point-type center location) 
    /// <para></para>
    /// --> Location and Size may be used individually, or a SizeRect may be used to set location and size.  
    /// <para></para>
    /// ** note:  SetRegion() and SetPoint() can be used with any type of mouse point or region. The 'point'- or 'region'-type status will be adjusted accordingly.  
    /// </summary>
    /// <param name="iIndex"> - Index of mouse region/point to change.</param>
    /// <param name="pLoc"> - New location of mouse region/point (or a SizeRect with location and size together)</param>
    /// <param name="szSize"> - New Size of mouse region (not used if SizeRect is used.  Check function prototypes).</param>
    /// <returns></returns>
    bool SetRegion(int iIndex,POINT pLoc,SIZE szSize);

    /// <summary>
    /// Sets the region location and size.  See SetPoint() to change a point-type mouse region.
    /// <para></para>
    /// --> This does not change the display order of the region/point.
    /// <para></para>
    /// --> The location given sets the upper-left of the region (see SetPoint() to set a point-type center location) 
    /// <para></para>
    /// --> Location and Size may be used individually, or a SizeRect may be used to set location and size.  
    /// <para></para>
    /// ** note:  SetRegion() and SetPoint() can be used with any type of mouse point or region. The 'point'- or 'region'-type status will be adjusted accordingly.  
    /// </summary>
    /// <param name="iIndex"> - Index of mouse region/point to change.</param>
    /// <param name="pLoc"> - New location of mouse region/point (or a SizeRect with location and size together)</param>
    /// <param name="szSize"> - New Size of mouse region (not used if SizeRect is used.  Check function prototypes).</param>
    /// <returns></returns>
    bool SetRegion(int iIndex,SizeRect szRect);

    /// <summary>
    /// Sets the point-type region location and size.  See SetRegion() to change a region-type mouse region.
    /// <para></para>
    /// --> This does not change the display order of the region/point.
    /// <para></para>
    /// --> The location sets the center point, with width/height the radius in each direction.  See SetRegion() to set a region-type mouse region.  
    /// <para></para>
    /// --> Location and Size may be used individually, or a SizeRect may be used to set location and size.  
    /// <para></para>
    /// ** note:  SetRegion() and SetPoint() can be used with any type of mouse point or region. The 'point'- or 'region'-type status will be adjusted accordingly.  
    /// </summary>
    /// <param name="iIndex"> - Index of mouse region/point to change.</param>
    /// <param name="pLoc"> - New location of mouse region/point (or a SizeRect with location and size together)</param>
    /// <param name="szSize"> - New Size of mouse region (not used if SizeRect is used.  Check function prototypes).</param>
    /// <returns></returns>
    bool SetPoint(int iIndex,POINT pLoc,SIZE szSize);
    
    /// <summary>
    /// Sets the point-type region location and size.  See SetRegion() to change a region-type mouse region.
    /// <para></para>
    /// --> This does not change the display order of the region/point.
    /// <para></para>
    /// --> The location sets the center point, with width/height the radius in each direction.  See SetRegion() to set a region-type mouse region.  
    /// <para></para>
    /// --> Location and Size may be used individually, or a SizeRect may be used to set location and size.  
    /// <para></para>
    /// ** note:  SetRegion() and SetPoint() can be used with any type of mouse point or region. The 'point'- or 'region'-type status will be adjusted accordingly.  
    /// </summary>
    /// <param name="iIndex"> - Index of mouse region/point to change.</param>
    /// <param name="pLoc"> - New location of mouse region/point (or a SizeRect with location and size together)</param>
    /// <param name="szSize"> - New Size of mouse region (not used if SizeRect is used.  Check function prototypes).</param>
    /// <returns></returns>
    bool SetPoint(int iIndex,SizeRect szRect);

    /// <summary>
    /// Sets the current positon of the mouse region/point.
    /// <para></para>
    /// --> This does not change the size or shape of the region or point, and only sets it's position.
    /// <para></para>
    /// --> This does not change the display order of the region/point.
    /// <para></para>
    /// --> For point-type mouse regions, this sets the center point.  For region-type mouse regions, this sets the upper-left corner of the region.
    /// </summary>
    /// <param name="iIndex">- Index of mouse region</param>
    /// <param name="pLoc">- New position</param>
    /// <returns> Returns true if ok.  returns false if an invalid index was used as input.</returns>
    bool SetPos(int iIndex,const POINT pLoc);

    /// <summary>
    /// This looks at the current mouse status to update the current Mouse Region status for all points / regions.
    /// <para></para>
    /// This is used to update the movement, highlight, selection of points/regions based on what has happened to the mouse since the last time it was called.
    /// <para></para>
    /// --> This function must be called for every GetEvent() (or event callback) to update the mouse points, and should be called in the main event loop
    /// before looking for updated events or values from the Mouse Region.
    /// <para></para>
    /// --> This function is best called directly after a GetEvent() call or upon entry into an OnSageEvent() or Sage Event callback function.
    /// <para></para>
    /// ** Important Note **:  Do not call this function for the Mouse Region obtained from the window, e.g. win.GetMouseRegion().  The Mouse Region obtained from
    /// the window has a special status and automatically updates itself after every GetEvent() or before any Sage Event call or callback function. 
    /// </summary>
    void UpdatePoints();

    /// <summary>
    /// Returns true of the highlight Changed since the last call to HighlightChanged().
    /// <para></para>
    /// This function will return the index to the new highlighted index if the highlight has changed.
    /// <para></para>
    /// ** note: This is an event call.  Subsquent calls will return false (i.e. std::nullptr) once a true is returned, until the highlight changes again.
    /// </summary>
    /// <param name="peek"> - [optional] Set to Peek::Yes to not reset the event (i.e. still return true on subsequent calls)</param>
    /// <returns>std::optional&lt;int&gt;.  This value is std::nullptr if the highlight did not change.  Otherwise, it has the index of the highlighted point or region.</returns>
    std::optional<int> HighlightChanged(Peek peek = Peek::No);

    /// <summary>
    /// Returns true of the selection Changed since the last call to SelectionChanged() or a new selection has occurred..
    /// <para></para>
    /// --> Note:  The selection occurs when the mouse clicked on region or point.  The selection value remains active while the point/region is being moved. 
    /// See GetLastSelected() to get the last selected item even after it is no longer being moved.
    /// <para></para>
    /// This function will return the index to the new selected item index if the selection index has changed.
    /// <para></para>
    /// ** note: This is an event call.  Subsquent calls will return false (i.e. std::nullptr) once a true is returned, until the selection changes again.
    /// <para></para>
    /// --> Note: Unlike HighlightChanged(), an index of  -1 will not be returned to indicate that a selection has ended. Use GetCurrentSelection() to determine
    /// if there is a currently selected item. 
    /// </summary>
    /// <param name="peek"> - [optional] Set to Peek::Yes to not reset the event (i.e. still return true on subsequent calls)</param>
    /// <returns>std::optional&lt;int&gt;.  This value is std::nullptr if the selection did not change.  Otherwise, it has the index of the selected point or region.</returns>
    std::optional<int> SelectionChanged(Peek peek = Peek::No);
 
    /// <summary>
    /// Sets the AutoDraw() characteristics of the given index.
    /// <para></para>
    /// Use AutoDraw() with no index to set the characteristics of all points or regions simultaneously.  When using the index value, the changes only affect the current index's
    /// AutoDraw characteristics.
    /// <para></para>
    /// --&gt; Documentation for this feature is still in development.  See various example for usage information. &lt;---
    /// </summary>
    /// <param name="iIndex"> - Index of point or region</param>
    /// <param name="sAutoDraw"> - New AutoDraw characteristics, e.g. "disabled", "color=blue", "shape=square,colors=red,blue,green", etc.</param>
    /// <returns></returns>
    bool SetAutoDraw(int iIndex,const char * sAutoDraw);

    /// <summary>
    /// Sets the AutoDraw() characteristics of all points/regions simultaneously.  This also sets the default AutoDraw characteristics of new point/regions added 
    /// after this function call.
    /// <para></para>
    /// Use AutoDraw() with an index to set the AutoDraw characteristics of a single point or region.
    /// <para></para>
    /// --&gt; Documentation for this feature is still in development.  See various example for usage information. &lt;---
    /// </summary>
    /// <param name="sAutoDraw"> - New AutoDraw characteristics, e.g. "disabled", "color=blue", "shape=square,colors=red,blue,green", etc.</param>
    /// <returns></returns>
    bool SetAutoDraw(const char * sAutoDraw);

    /// <summary>
    /// Sets keyword-based options for the Mouse Region object. This can be useful to change options when using different or new sets of points. 
    /// This function can be useful when re-using the Mouse Region obtained from the window (e.g. win.GetMouseRegion()), when using mouse points for different
    /// types of processes in the same window.  (you can also create a new and separate MouseRegion by declaring a MouseRegion object).
    /// </summary>
    /// <param name="keywords"> - Various keywords to set as options, e.g. kw::AutoDraw("red,green,blue,shape=circle"), etc.</param>
    /// <returns></returns>
    bool SetOptions(const kwOpt & keywords);

    /// <summary>
    /// Returns the index of the currently highlighted item. 
    /// <para></para>
    /// The currently highlighted item is the item that the mouse is currently over or has selected. 
    /// <para></para>
    /// --> note: When there is no highlighted item or the mouse is no longer over the previous item, a -1 is returned to indicate there is no current highlight.
    /// <para></para>
    /// --> See: GetLastHighlighted() to get the last and/or current highlighted item. 
    /// </summary>
    /// <returns>If the mouse is currently within a region (whether it is moving the region/point or not), the index of the mouse region/point is returned.
    /// Otherwise -1 is returned to indicate there is no highlighted item.</returns>
    int GetCurrentHighlight() { return m_ulCurHighlight; }

    /// <summary>
    /// Returns the index of the currently selected item. 
    /// <para></para>
    /// --> Note: The currently selected item is active only if the mouse is currently moving a point or region, or has clicked on a point or region and
    /// the mouse button has not yet been unclicked.  Once the mouse is unclicked, the item is no longer selected and -1 is returned.
    /// <para></para>
    /// --> See GetLastSelected() to return the value of the most recently selected item even after the mouse has been released.
    /// <para></para>
    /// --> note: When there is no selected item or the mouse is no in contact with the mouse region/point, a -1 is returned to indicate there is no currently selected item.
    /// </summary>
    /// <returns>If the mouse is currently moving or otherwise has clicked on an object (without having unclicked the mous yet, whether it is moving the region/point or not),
    /// the index of the point/region is returned. Otherwise -1 is returned to indicate there is no selected item.</returns>
    int GetCurrentSelection() { return m_ulCurSelected; }

    /// <summary>
    /// Returns the index of the last selected item (or the currently selected item of the mouse is moving or otherwise pressed on a region/point).
    /// <para></para>
    /// ** Note: GetLastSelected() returns the last selected item, which also includes an item that is currently selected (i.e. currently being moved by the mouse)
    /// <para></para>
    /// GetLastSelected() returns the last item that was selected (or -1 if there is no previous selection).  This differs from GetCurrentSelection() which only
    /// returns an index value if the mouse is currently pressed on a selected item (which then returns to -1 when the mouse button is released).
    /// <para></para>
    /// --> GetLastSelected() will return the value of the last selection, unless ResetSelected() is called or there has not yet been a selected item. This is also useful
    /// when using AutoDraw().
    /// <para></para>
    /// --> Example:  kw::AutoDraw("red,yellow,green") sets the selected color as green, which
    /// will remain green even when the mouse has stopped moving the item -- this occurs when the selected color (third color) is different than the highlighted color
    /// (second color), showing the last selected index as green.
    /// </summary>
    /// <returns></returns>
    int GetLastSelected() { return  m_iLastSelectedDisplay < m_iActiveRegions ? m_iLastSelectedDisplay : -1; }


    /// <summary>
    /// Returns the index of the last highlighted item (or the currently highlighted item).
    /// <para></para>
    /// GetLastHighlighted() returns the last item that was highlighted (or -1 if there is no previous selection).  This differs from GetCurrentHighlight() which only
    /// returns an index value if the mouse is currently over highlighted item (which then returns to -1 when the mouse is no longer over the point/region).
    /// </summary>
    /// <returns></returns>
    int GetLastHighlighted() { return m_iLastHighlight < m_iActiveRegions ? m_iLastHighlight : -1; }

    /// <summary>
    /// Sets the bounding box for the mouse region or point.
    /// <para></para>
    /// --> When a bounding box is used, the mouse point or region cannot exceed the boundaries set inside the box.
    /// <para></para>
    /// This function uses a SizeRect: the input for region is { left, top, width, height}.  For Points, the input is { x,y,radiusX,RadiusY }
    /// <para></para>
    /// To use a RECT structure, it can be converted to a size rect with SizeRect::fromRect(myRect)
    /// <para></para>
    /// --> With a Point-type index (i.e. AddPoint() was used), the center x,y value is restricted to the bounding box, allowing the radiusX and radiusY width/height
    /// to exceed the bounding box.  Reduce the Bounding Box as needed to include these values with Point-type indexes. 
    /// <para></para>
    /// ** Note:  kw::BoundBox() may be used generically or with individual points when they are added, e.g. AddPoint(myPoint,kw::BoundingBox({100,200,800,400}))
    /// </summary>
    /// <param name="iIndex"> - Index of point or region</param>
    /// <param name="srRect"> - SizeRect of { x,y, RadiusX, RadiusY } for bounding box area.</param>
    /// <returns></returns>
    bool SetBoundBox(int iIndex,SizeRect srRect);

    /// <summary>
    /// Removes a bounding box constraint from a mouse region or point.
    /// <para></para>
    /// See SetBoundBox() to set a bounding box for a mouse region or point. Or use kw::SetBoundBox().
    /// </summary>
    /// <param name="iIndex"></param>
    /// <returns></returns>
    bool RemoveBoundBox(int iIndex);

    /// <summary>
    /// Draws all points or regions to the window. The program may handle the display of points itself by processing events, which can be much more flexible.
    /// <para></para>
    /// AutoDraw() is provided as a way to quickly display the mouse points/regions as they are used, moved, highlighted, etc.
    /// <para></para>
    /// --> AutoDraw() can be used for most programs. However, as programs grow and require more extensive display, AutoDraw() can be omitted. 
    /// Also, individual points/regions can be set to off, allowing some to use AutoDraw() and others to be bypassed so that the program can draw the point itself.
    /// example: SetAutoDraw(index,"off");
    /// <para></para>
    /// --&gt; Documentation for this feature is still in development.  See various example for usage information. &lt;---
    /// </summary>
    /// <returns></returns>
    bool AutoDraw();

    /// <summary>
    /// Calls the AutoDraw() function for only the index provided.
    /// <para></para>
    /// Where AutoDraw() updates all Mouse Region elements in the AutoDraw display, AutoDrawIndex() updates only the indexed element provided.
    /// This allows some control over the output, allowing for text and other additives to the auto-draw display before moving to the next
    /// element.
    /// <para></para>
    /// See AutoDraw() documentation for more information.
    /// </summary>
    /// <param name="iIndex"> - Index of element to auto-draw</param>
    /// <returns></returns>
    bool AutoDrawIndex(int iIndex);

    /// <summary>
    /// Resets the last selected point value to -1.
    /// <para></para>
    /// The 'last selected point' can be used to keep a static value for selected point, even after it has been released from being moved or selected with the mouse.
    /// <para></para>
    /// --> The 'selected point' (i.e. GetSelected()) is the point that is being moved or otherwise the mouse has clicked on without yet unclicking the button.
    /// <para></para>
    ///  --> This resets the last selected point (i.e. GetLastSelected())
    /// <para></para>
    /// --> ResetSelected() also resets the last highlighted index (i.e. GetLastHighlighted())
    /// <para></para>
    /// ** Note: Once the mouse is released from a selection, this value is stored as the 'last selected point' for later reference.  
    /// This 'last selected' value remains the same until a new region or point is selected, or ResetSelected() is called to reset it to -1 (no selection)
    /// <para></para>
    /// See: GetLastSelected() to get the last selected item (vs. GetSelected())
    /// </summary>
    /// <returns></returns>
    bool ResetSelected(); 
 
    /// <summary>
    /// Returns a DragInfo structure if there is a mouse point or region that is being moved by the mouse, or the mouse has otherwise clicked on the region/point
    /// while not yet releasing the mouse button (i.e. the mouse buttin is pressed on the object).
    /// <para></para>
    /// The returned DragInfo object returns the current location of the region or point, the last point, and the original point when it was first grabbed by the mouse.
    /// <para></para>
    /// The DragInfo structure also contains the index of the currently selected mouse region/point (i.e. the region/point being moved).
    /// <para></para>
    /// ** note: This is an event call.  Subsquent calls will return false (i.e. std::nullptr) once a true is returned, until the mouse moves again.
    /// </summary>
    /// <param name="peek"> - [optional] Set to Peek::Yes to not reset the event (i.e. still return true on subsequent calls)</param>
    /// <returns>DragInfo object if there is a new MouseDragEvent to report.  Otherwise std::nullopt is returned.</returns>
    std::optional<DragInfo> MouseDragEvent(Peek peek = Peek::No);

    /// <summary>
    /// Returns a DragInfo structure if a mouse/region being dragged by the mouse (or was clicked upon) has been released.
    /// <para></para>
    /// A Drag Event ends when the mouse is released from a selected region/point.
    /// <para></para>
    /// The returned DragInfo object returns the current location of the region or point, the last point, and the original point when it was first grabbed by the mouse.
    /// <para></para>
    /// The DragInfo structure also contains the index of the currently selected mouse region/point (i.e. the region/point being moved).
    /// <para></para>
    /// ** note: This is an event call.  Subsquent calls will return false (i.e. std::nullptr) once a true is returned, until the mouse moves again.
    /// </summary>
    /// <param name="peek"> - [optional] Set to Peek::Yes to not reset the event (i.e. still return true on subsequent calls)</param>
    /// <returns>DragInfo object if there is a new MouseDragEnded to report.  Otherwise std::nullopt is returned.</returns>
    std::optional<DragInfo> MouseDragEnded(Peek peek = Peek::No);

#if 0
    /// <summary>
    /// Sets the position of the mouse point/region.
    /// <para></para>
    /// Note:  For region-type indexes (set with AddRegion()) the location is the upper-left point of the region.
    /// <para></para>
    /// For point-type indexes (set with AddPoint()) the location is the (x,y) of the central point.
    /// </summary>
    /// <param name="iIndex"> - Index of mouse point or region</param>
    /// <param name="pLoc"> - New locaiton for region/point.</param>
    /// <returns></returns>
    bool SetPos(int iIndex,POINT pLoc);
#endif
    // This function is still in development and debug.
    POINT GetDisplayPos(int iPos);

    // This function is still in development and debug.
    int GetDisplayPosID(int iID);

    // This function is still in development and debug.
    int GetDisplayPosIndex(int iIndex);


    /// <summary>
    /// Returns the mouse region/point index corresponding to the user ID as set with kw::ID()
    /// <para></para>
    /// -1 is returned if no valid index was found
    /// </summary>
    /// <param name="iUserID"> - User ID as given with kw::ID() on point creation</param>
    /// <returns>Index of point that has this user ID.  -1 is returned if no point was found.</returns>
    int GetUserIDIndex(int iUserID);

    /// <summary>
    /// Returns userID (if specified with kw::ID()) corresponding to the mouse region/point index given as input.
    /// <para></para>
    /// -1 is returned if the index was not a valid mouse region/point.
    /// <para></para>
    /// </summary>
    /// <param name="index"> - index of the mouse region/point from which to obtain its user ID.</param>
    /// <returns>User ID of the mouse point/region if ID was established with kw::ID(). Otherwise 0 is returned..  -1 is returned if the index given was invalid.</returns>
    int GetIndexUserID(int iIndex);

    /// <summary>
    /// Clears all points/regions from the Mouse Region object, leaving 0 points/regions.
    /// <para></para> 
    /// Once cleared, new additions start at index 0 and forward.
    /// </summary>
    /// <returns></returns>
    bool ResetPoints();

    /// <summary>
    /// Gets the display index iteratively, in the display order.
    /// <para></para> 
    /// --> When Mouse Region items are selected, they are moved to the top of the display order (unless kw::DontPromote() is used to prevent it).
    /// Thus, the index order vs. the display order may not be the same. 
    /// <para></para> 
    /// To iterate through the display order, GetDisplayIndex() can be called until a -1 is returned to signal there are no more display items:
    /// <para></para> 
    /// Example:
    /// <para></para> 
    /// --> int iIndex = -1;
    /// <para></para> 
    /// --> while (GetDisplayIndex(iIndex)) { perform_display_operation_on_index() };
    /// </summary>
    /// <param name="iIndex"> - Input index.  First value must be -1</param>
    /// <returns>True if a value index is filled in iIndex.  Returns False if there are no more indexes to display.</returns>
    bool GetDisplayIndex(int & iIndex); 

    /// <summary>
    /// Returns true if an event is ready to be processed, such as a Mouse Drag event, etc. 
    /// <para></para>
    /// --> This is an Event-type function, which will return false once a 'true' is returned for the same event, and until another event is ready
    /// <para></para>
    /// --> Use peek=Peek::Yes to avoid resetting the event status.
    /// <para></para>
    /// The event status is only for the EventReady() call itself and not any of the Mouse Region events that have occurred.
    /// Looking for specific events (e.g. Mouse Drag Event) will return true or false for their
    /// respective event, even if EventReady() has been called.
    /// <para></para>
    /// EventReady() can be useful to only process Mouse Regions if there is a reason to do so (i.e. a Mouse Region event has occurred). 
    /// This can be useful in separating mouse events from Mouse Region events, to allow using the mouse in areas not within mouse region points/regions.
    /// </summary>
    /// <param name="peek"> - [optional] use Peek::Yes to avoid resetting the event status (i.e. true will be returned again for the same event)</param>
    /// <returns></returns>
    bool EventReady(Peek peek = Peek::No);

    /// <summary>
    /// By default, mouse movements, clicks, unclicks, are not passed as general events when the Mouse Region has determined a Mouse Region event has 
    /// occured.
    /// <para></para>
    /// --> For example, if the main event loop looks for mouse movements or clicks, these will not be reported if the mouse is over or is currently
    /// moving a mouse point or region. This helps to react only to mouse events not within mouse regions.
    /// <para></para>
    /// Sometimes it may be useful to get the mouse events as regions are being moved or highlighted. 
    /// <para></para>
    /// --> Using KillPassedEvents(false) will pass these events through, so the same mouse events that Mouse Region used will also be available to the main
    /// event loop. 
    /// </summary>
    /// <param name="bKillEvents"> - True to not pass Mouse Region mouse events to the main event loop.  False to pass the events for use with the main event loop.</param>
    /// <returns></returns>
    bool KillPassedEvents(bool bKillEvents = true); 

    /// <summary>
    /// Returns the number of points and regions in the Mouse Region object.
    /// </summary>
    /// <returns></returns>
    __forceinline int GetNumIndexes() { return !this ? 0 : m_iActiveRegions; }
};

} // namespace Sage
