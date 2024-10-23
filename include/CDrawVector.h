#pragma once
 
// ************************************************************
// Draw Vector Class -- THIS DOCUMENTATION IS IN-PROGRESS (TBD)
// ************************************************************
// Include vector keyword defintions.
//
// These keywords are used as a sample of how to use keywords in various ways: 
//
// 1. Functional Keywords -- the same keyword type used through Sagebox
// 2. Functional Keywords starting with '_'.  This allows using namespace _vkw2 so keywords can start with underlines, 
//    such as DrawVector(p1,p2,10,_Color("red") + _Angle(35)), for example, without cluttering the overall namespace.
// 3. Packed-Parameter Keywords (namespace vkw2) -- example: DrawVector(p1,p2,10,Color="red",Angle=35), etc. 
//    Without using namespace vkw2, it would be DrawVector(p1,p2,10,vkw2::Color="red",vkw2::Angle=35)
// 4. Packed-Parameter Keywords with underlines (namespace _vkw2) - Allows "using namespace _vkw2" using underlines to more obviously specify keywords,
//    e.g. DrawVector(p1,p2,10,_Color="red",_Angle=35), etc.
// 
#include "vkw_keyfuncs.h"
#include <map>
namespace Sage
{
namespace vkw
{
    extern const vkwType::ckw none;
}
class CDrawVector
{
     using vkwOpt = vkwType::ckw;

private:
    enum class VType
    {
        Top,
        Mid,
        Bot,
    };
    enum class HType
    {
        Left,
        CenterLeft,
        Center,
        Right,
        CenterRight,
    }; 

    struct stCapTypeTable_t
    {
        LineCapType eLineType;

        const char * sLineText; 
    };

    static const std::map<std::string,vkwType::TextSizeType>    m_mapTextSizes; 
    static const std::map<std::string,vkwType::LabelAngleType>  m_mapLabelAngleType;
    static std::map<std::string,Sage::LineCapType>              mapLineCapTypes;    // New Method  
    static const stCapTypeTable_t                               stCapTypes[18];     // Old method, still used for single cap specifications

    static constexpr double kDefaultAnchorCapMult = 2.0; 
    static constexpr double kDefaultCapMult = 1.0; 

    LineCapType kDefaultCapBeg = LineCapType::Round;
    LineCapType kDefaultCapEnd = LineCapType::ArrowAnchor;

    Gdiplus::Graphics * m_cGdi = nullptr; 

    double m_fDefaultCapSize = 1.0; 

    static constexpr int m_iMinFontSize = 15; 
    static constexpr int m_iMaxFontSize = 35;
    static constexpr const char * m_sBaseFont = "Arial,";

    double CalcFont() { return 0.6791*m_fLineSize + 14.32; }
    
    vkwType::LabelJustType kDefaultLabelJust = vkwType::LabelJustType::TopCenter;
    vkwType::TextSizeType kDefaultTextSize = vkwType::TextSizeType::Default;


    static constexpr double kFontSizeMult[7] = { .6, .8, 1.0, 1.15, 1.25, 1.4, 1.9 };


    std::optional<RgbColor> m_opShowCenter; 

    CRgbColor m_rgbStartCap          = SageColorA::White();  // If no color is found, these resolve to the line color
    CRgbColor m_rgbEndCap            = SageColorA::White(); 
    CRgbColor m_rgbStartCapBorder    = SageColorA::White();  
    CRgbColor m_rgbEndCapBorder      = SageColorA::White(); 
    CRgbColor m_rgbLineColor         = SageColorA::White();

 //   const char * m_sStartCapColor = nullptr;
 //   const char * m_sEndCapColor = nullptr;

 //   std::optional<vkwType::stDualColorStruct_t> m_opRgbStartCapColor; 
 //   std::optional<vkwType::stDualColorStruct_t> m_opRgbEndCapColor; 

    std::optional<CfPoint> m_opSetCenter;
    const char * m_sStartCap = nullptr;
    const char * m_sEndCap = nullptr;
    const char * m_sTitle = nullptr;
    CString m_csUserFont; 
    HFONT m_hUserFont = (HFONT) nullptr;
 //   std::optional<double> m_opStartCapSize;
 //   std::optional<double> m_opEndCapSize;

 //   double m_fCapMultInBeg = kDefaultAnchorCapMult;      // User-incoming cap size. 
 //   double m_fCapMultInEnd = kDefaultAnchorCapMult;      // User-incoming cap size. 
    double m_fLineSize = 0;
    double m_fCapMultBeg = kDefaultAnchorCapMult;
    double m_fCapMultEnd = kDefaultAnchorCapMult;
    LineCapType    m_eEndCap           = kDefaultCapEnd;
    LineCapType    m_eStartCap         = kDefaultCapBeg;
    vkwType::LabelJustType  m_eLabelJustType    = kDefaultLabelJust;
    vkwType::TextSizeType   m_eTextSize         = kDefaultTextSize;

    bool m_bLabelAngleStatic                = false;
    bool m_bKeepLabelUp                     = false;
    vkwType::LabelAngleType m_eLabelAngleType   = vkwType::LabelAngleType::Default;
 //   const char * m_sLineColor = nullptr;
    int m_iOpacity          = 255; 
    int m_iTextOpacity      = 255; 
    int m_iLabelOffsetX     = 0;
    int m_iLabelOffsetY     = 0;

    RgbColor m_rgbLabelColor = SageColor::White;

    double m_fFontMult      = 1.0;
    CfPoint m_cVecCoord1 = { 0,0 };
    CfPoint m_cVecCoord2 = { 0,0 };

    CfPoint m_cOrgVecCoord1 = { 0,0 };
    CfPoint m_cOrgVecCoord2 = { 0,0 };

    double m_fLineAngle = 0;        // Absolute angle of line after any rotations - $$ m_fLineAngle is not named properly.  It is too confused with the angle of the line as input.
    double m_fInputAngle = 0;       // Line Angle as specified by the calling function.

    // Line Draw Members

   Gdiplus::AdjustableArrowCap * m_capBegArrow     = nullptr;
   Gdiplus::AdjustableArrowCap * m_capEndArrow     = nullptr;
   Gdiplus::CustomLineCap      * m_capBegCircle    = nullptr;
   Gdiplus::CustomLineCap      * m_capEndCircle    = nullptr;

   double m_fPrevBegLCapMult = std::numeric_limits<double>::lowest(); 
   double m_fPrevEndLCapMult = std::numeric_limits<double>::lowest(); 
   double m_fPrevBegRCapMult = std::numeric_limits<double>::lowest(); 
   double m_fPrevEndRCapMult = std::numeric_limits<double>::lowest(); 

   Gdiplus::LineCap m_eCapBegPrev = (Gdiplus::LineCap) -1;
   Gdiplus::LineCap m_eCapEndPrev = (Gdiplus::LineCap) -1;

   Gdiplus::GraphicsPath  * m_CapBegPath = nullptr;
   Gdiplus::GraphicsPath  * m_CapEndPath = nullptr;

    bool m_bAttachBeg   = false; 
    bool m_bAttachEnd   = false; 

    bool m_bAltShapeBeg = false; 
    bool m_bAltShapeEnd = false; 
    bool m_bUniformColorsBeg = false;
    bool m_bUniformColorsEnd = false;
    Gdiplus::LineCap m_eCapBegGDI = Gdiplus::LineCap::LineCapFlat; 
    Gdiplus::LineCap m_eCapEndGDI = Gdiplus::LineCap::LineCapFlat; 

//  Gdiplus::CustomLineCap * m_LineCapBeg = new Gdiplus::CustomLineCap(nullptr,nullptr); // startCap(); //m_CapBegPath, nullptr);  
//  Gdiplus::CustomLineCap * m_LineCapEnd = new Gdiplus::CustomLineCap(nullptr,nullptr); // startCap(); //m_CapBegPath, nullptr);  
//

private:
    CWindow & m_cWin; 
private:
    std::optional<LineCapType> FindStringCapType(const char * sCapString);
    RgbGradient FindDualColor(const char * sColorString);
    bool GetCapColors(std::optional<vkwType::stDualColorStruct_t> opRgbStartCapColor,const char * sStartCapColor,
                                   std::optional<vkwType::stDualColorStruct_t> opRgbEndCapColor,const char * sEndCapColor);
    void SetBegCapColor(CRgbColor rgbColor,const char * sColor); 
    void SetEndCapColor(CRgbColor rgbColor,const char * sColor); 

    bool GetLineColor(std::optional<RgbColor> rgbLineColor,const char * sLineColor);
    bool SetBegCapType(LineCapType eCapType,const char * sType);
    bool SetEndCapType(LineCapType eCapType,const char * sType);
    bool GetCapSizes(std::optional<double> fCapMultInBeg,std::optional<double> fCapMultInEnd);
    void GetArrowCap(Gdiplus::LineCap eCapType,Gdiplus::AdjustableArrowCap * &cap,double fMultCap,double & fPrevMultCap,const char * sDebug = nullptr);
    void GetCircleCap(Gdiplus::LineCap eCapType,Gdiplus::LineCap & eOldCap,Gdiplus::CustomLineCap * &cap,Gdiplus::GraphicsPath * capPath,double fMultCap,double & fPrevMultCap,const char * sDebug = nullptr);
    void SetupStandardGDI(CfPoint cVecCoord1,CfPoint cVecCoord2); 
    void DrawLabel();
    void DrawLabel_save1();
    void DrawLabel_old();
    void PrepareKeywordTypeString(CString & csString);
    void GetCapLengthOffsets(HType eHType,double & fLeftX,double & fRightX);
    std::tuple<CDrawVector::VType,CDrawVector::HType>  GetLabelJustTypes();
    void RotateOrgLine(double fAngle);

    void RestoreOrgPoints();
    void SetDefaultCapMult();
    bool DrawLocal();

    // Constructor initialization to serve both types of constructors (object keywords and packed-parameter keywords);
    //
    void InitObject(double fLineSize,CfPoint p1,CfPoint p2); 

    // ********************************************************************
    // Internal versionf of functions using packed-parameter keywords
    // (template versions in the public declarations call these functions)
    // ********************************************************************

    bool Draw(const vkwType::KeyValuesPtr & keys);              // Draw function using packed parameter keywords
    bool SetOptions(const vkwType::KeyValuesPtr & keys);        // Set options used packed-parameter keywords

    // Process incoming LineCaps() for Strings and LineCapType values, respectively. 

    void SetCapsfromMultiString(const char * sCapTypes,std::optional<LineCapType> & eBegCap,std::optional<LineCapType> & eEndCap);
    void SetCapsFromDualCapOption(std::optional<vkwType::stLineCaps_t> & opLineCaps,std::optional<LineCapType> & eBegCap,std::optional<LineCapType> & eEndCap);
public:
    CDrawVector(CWindow & cWin);

    // ***********************************************************************************
    // Function using Object-Based Function Keywords - e.g. Draw(Color("red") + Angle(45))
    // ***********************************************************************************

    CDrawVector(CWindow & cWin,const CfPoint p1,const CfPoint p2,double fLineSize,const vkwOpt & keywords = vkw::none); // Simple consctuctor
    bool SetOptions(const vkwOpt & keywords);                                                                               // Set various options
    bool Draw(const vkwOpt & keywords = vkw::none);                                                                         // Draw the vector with optional options.

    // ***************************************************************************************
    // Functions using Classical Packed-Parameter Keywords - e.g. Draw(Color="red",Angle = 45)
    // ***************************************************************************************

    // Note:  non-keyword functions (i.e. Simple Draw() vs. Draw(<keywords>) do not exist.  Since the Function-Based keywords are supported as an example,
    //        these functions already support no-keyword versions, and these can be called instead. 
    //        Normal usage (when not an example like this class) is to provide these functions for cases where keywords are not used. 
 
    // Template for constructor using packed-parameter keywords. 
    //
    template <class... Args>
    CDrawVector(CWindow & cWin,const CfPoint p1,const CfPoint p2, double fLineSize,const Args&... kwArgs) : m_cWin(cWin)
    {
        InitObject(fLineSize,p1,p2);                            // Do common things for all constructors
        SetOptions(vkwType::pkw::FillKeyValues(kwArgs...));     // Set options via packed-parameter method
    }


    // Template for Set options with packed-parameter keywords
    //
    template <class... Args>
    bool SetOptions( const Args&... kwArgs)
    {
        return SetOptions(vkwType::pkw::FillKeyValues(kwArgs...));
    }  

    // Template for Draw (i.e. Draw Vector) function using packed-parameter functions. 
    //
    template <class... Args>
    bool Draw( const Args&... kwArgs)
    {
       return Draw(vkwType::pkw::FillKeyValues(kwArgs...));
    }


    ~CDrawVector();

    // Static vector to draw a vector without the need to instantiate the CDrawVector class
    //
    // e.g. CDrawVector::DrawVector(cWin,p1,p2,fLineSize,[optional keywords])
    //
    // This is the same function as using cWin.DrawVector(), i.e. CWindow::DrawVector().
    //
    static bool DrawVector(CWindow & cWin,const CfPoint p1,const CfPoint p2,double fLineSize,const vkwOpt & keywords);

    // Parameter functions, replicated with kw:: keywords
    //


    /// <summary>
    /// Keeps the vector's label right-side up. As the vector rotates and the label's orientation, using SetLabelAlwaysUp() will keep the label
    /// right-side up when the label's angle is such that the label would otherwise appear upside-down.
    /// </summary>
    /// <returns></returns>
    bool SetLabelAlwaysUp(bool bAlwaysUp = true);

    /// <summary>
    /// Sets the label of the angle with 3 options: "horizontal" (default), "vertical", or "vertical 180".  "vertical" and vertical 180" make the label
    /// appear sideways on the line, with "vertial 180" reversing it's direction, with the start of the text appearing away from the line.
    /// <para></para>
    /// --> Add the term "static" to keep the label the exact orientation regardless of line angle, e.g. SetLabelAngle("horizontal,static") will cause
    /// the text to display at 0 degrees relative to the screen rather than the line itself.
    /// --> Text can be used, e.g. SetLabelAngle("vertical,static")
    /// <para></para>
    /// --> vkwType::LabelAngleType can also ge used, e.g. SetLabelAngle(vkwType::LabelAngleType::Vertical) or SetLabelAngle(vkwType::LabelAngleType::Horizontal,true) 
    /// </summary>
    /// <param name="AngleType"> - Angle type, either text (e..g. "vertical")</param>
    /// <param name="bStaticAngle"> - [optional] true to make angle static when using vkwType::LabelAngleType, e.g. SetLabelAngle(vkwType::LabelAngleType::Horizontal,true) </param>
    /// <returns></returns>
    bool SetLabelAngle(const char * sAngleType);

    /// <summary>
    /// Sets the label of the angle with 3 options: "horizontal" (default), "vertical", or "vertical 180".  "vertical" and vertical 180" make the label
    /// appear sideways on the line, with "vertial 180" reversing it's direction, with the start of the text appearing away from the line.
    /// <para></para>
    /// --> Add the term "static" to keep the label the exact orientation regardless of line angle, e.g. SetLabelAngle("horizontal,static") will cause
    /// the text to display at 0 degrees relative to the screen rather than the line itself.
    /// --> Text can be used, e.g. SetLabelAngle("vertical,static")
    /// <para></para>
    /// --> vkwType::LabelAngleType can also ge used, e.g. SetLabelAngle(vkwType::LabelAngleType::Vertical) or SetLabelAngle(vkwType::LabelAngleType::Horizontal,true) 
    /// </summary>
    /// <param name="AngleType"> - Angle type, either text (e..g. "vertical")</param>
    /// <param name="bStaticAngle"> - [optional] true to make angle static when using vkwType::LabelAngleType, e.g. SetLabelAngle(vkwType::LabelAngleType::Horizontal,true) </param>
    /// <returns></returns>
    bool SetLabelAngle(vkwType::LabelAngleType angleType,bool bStaticAngle = false);
   

    /// <summary>
    /// Sets the label justification on the drawn vector. 
    /// <para></para>
    /// These include vertical (top, middle, bottom), as well as horizontal: Left, Left Center, Center, Right Center, Right
    /// <para></para>
    /// --> LabelJustTypes can be used, e.g. LabelJust(vkwType::LabelJustType::BottomRight)
    /// <para></para>
    /// --> Strings can also be used, e.g. LabelJust("bottom right"), LabelJust("middle left center"), etc.
    /// </summary>
    /// <param name="labelJust"> - Label Justification, either a string or vkwType::LabelJustType</param>
    /// <returns></returns>
    bool SetLabelJust(const char * sLabelJustType);

    /// <summary>
    /// Sets the label justification on the drawn vector. 
    /// <para></para>
    /// These include vertical (top, middle, bottom), as well as horizontal: Left, Left Center, Center, Right Center, Right
    /// <para></para>
    /// --> LabelJustTypes can be used, e.g. SetLabelJust(vkwType::LabelJustType::BottomRight)
    /// <para></para>
    /// --> Strings can also be used, e.g. SetLabelJust("bottom right"), LabelJust("middle left center"), etc.
    /// </summary>
    /// <param name="labelJust"> - Label Justification, either a string or vkwType::LabelJustType</param>
    /// <returns></returns>
    bool SetLabelJust(vkwType::LabelJustType eJustType);
   
    /// <summary>
    /// Sets the text size based on the default font, from xxsmall to xxlarge. 
    /// <para></para>
    /// When using the default font, the font will scale to a smaller or larger size based on the line width.
    /// <para></para>
    /// --> You can further control the size by specifying the following values:
    /// <para></para>
    /// xxsmall, xsmall, small, medium, large, xlarge, and xxlarge.  using "default" sets to the default, which is 'small'
    /// --> TextSizeType can be used, e.g. SetTextSize(vkwType::TextSizeType::XXSmall)
    /// <para></para>
    /// --> Strings can also be used, e.g. SetTextSize("xxsmall"); 
    /// <para></para>
    /// ** See: SetLabelFont() which can be used to specificy a specific font (which does not scale based on line thickness).
    /// </summary>
    /// <param name="textSize"> - size of the font relative to default font and line thickness.</param>
    bool SetTextSize(const char * sTextSize);

    /// <summary>
    /// Sets the text size based on the default font, from xxsmall to xxlarge. 
    /// <para></para>
    /// When using the default font, the font will scale to a smaller or larger size based on the line width.
    /// <para></para>
    /// --> You can further control the size by specifying the following values:
    /// <para></para>
    /// xxsmall, xsmall, small, medium, large, xlarge, and xxlarge.  using "default" sets to the default, which is 'small'
    /// --> TextSizeType can be used, e.g. TextSize(vkwType::TextSizeType::XXSmall)
    /// <para></para>
    /// --> Strings can also be used, e.g. TextSize("xxsmall"); 
    /// <para></para>
    /// ** See: SetLabelFont() which can be used to specificy a specific font (which does not scale based on line thickness).
    /// </summary>
    /// <param name="textSize"> - size of the font relative to default font and line thickness.</param>
    bool SetTextSize(vkwType::TextSizeType eTextSize);
   
    // Sets Cap Type for start of vector line.  
    // Use Sage::LineCapType::  (i.e. SetBegCap(LineCapType::Arrow))
    // or use strings, such as SetBegCap("arrow"); 
    //
    // --> Valid types are Flat, Arrow, Round, Square, Diamond, ArrowAnchor, RoundAnchor, SquareAnchor, and DiamondAnchor
    //
    // Default for start cap is Round. Default for EndCap is ArrowAnchor. 
    //
    // 'Anchor' types are larger.  Non-anchor types are the same size of the line.  To set Cap sizes, use Anchor Types.
    // See SetCapSize(), SetBegCapSize() and SetEndCapSize()
    //
    bool SetBegCap(LineCapType eBegCapType);

    // Sets Cap Type for start of vector line.  
    // Use Sage::LineCapType::  (i.e. SetBegCap(LineCapType::Arrow))
    // or use strings, such as SetBegCap("arrow"); 
    //
    // --> Valid types are Flat, Arrow, Round, Square, Diamond, ArrowAnchor, RoundAnchor, SquareAnchor, and DiamondAnchor
    //
    // Default for start cap is Round. Default for EndCap is ArrowAnchor. 
    //
    // 'Anchor' types are larger.  Non-anchor types are the same size of the line.  To set Cap sizes, use Anchor Types.
    // See SetCapSize(), SetBegCapSize() and SetEndCapSize()
    //
    bool SetBegCap(const char * sBegCapType);

    // Sets Cap Type for end of vector line.  
    // Use Sage::LineCapType::  (i.e. SetEndCap(LineCapType::Arrow))
    // or use strings, such as SetEndCap("arrow"); 
    //
    // --> Valid types are Flat, Arrow, Round, Square, Diamond, ArrowAnchor, RoundAnchor, SquareAnchor, and DiamondAnchor
    //
    // Default for start cap is Round. Default for EndCap is ArrowAnchor. 
    //
    // 'Anchor' types are larger.  Non-anchor types are the same size of the line.  To set Cap sizes, use Anchor Types.
    // See SetCapSize(), SetBegCapSize() and SetEndCapSize()
    //
    bool SetEndCap(LineCapType eEndCapType);

    // Sets Cap Type for end of vector line.  
    // Use Sage::LineCapType::  (i.e. SetEndCap(LineCapType::Arrow))
    // or use strings, such as SetEndCap("arrow"); 
    //
    // --> Valid types are Flat, Arrow, Round, Square, Diamond, ArrowAnchor, RoundAnchor, SquareAnchor, and DiamondAnchor
    //
    // Default for start cap is Round. Default for EndCap is ArrowAnchor. 
    //
    // 'Anchor' types are larger.  Non-anchor types are the same size of the line.  To set Cap sizes, use Anchor Types.
    // See SetCapSize(), SetBegCapSize() and SetEndCapSize()
    //
    bool SetEndCap(const char * sEndCapType);

    /// <summary>
    /// Sets the color of the Start Cap of the line.  When not specified, the color is the same color as the vector line itself.
    /// <para></para>
    /// --> Rgb Colors may be used, e.g. SetBegCapColor(PanColor::Red), SetBegCapColor(MyColor), SetBegCapColor(RgbColor{0,0,128}), etc.
    /// <para></para>
    /// --> Strings may be used as well, e.g. SetBegCapColor("red")
    /// </summary>
    /// <returns></returns>
	bool SetBegCapColor(const Sage::CRgbColor rgbColor);
#if 0
    /// <summary>
    /// Sets the color of the Start Cap of the line.  When not specified, the color is the same color as the vector line itself.
    /// <para></para>
    /// --> Rgb Colors may be used, e.g. SetBegCapColor(PanColor::Red), SetBegCapColor(MyColor), SetBegCapColor(RgbColor{0,0,128}), etc.
    /// <para></para>
    /// --> Strings may be used as well, e.g. SetBegCapColor("red")
    /// </summary>
    /// <returns></returns>
	bool SetBegCapColor(const char * sCapColor);
#endif
    /// <summary>
    /// Sets the color of the End Cap of the line.  When not specified, the color is the same color as the vector line itself.
    /// <para></para>
    /// --> Rgb Colors may be used, e.g. SetEndCapColor(PanColor::Red), SetEndCapColor(MyColor), SetEndCapColor(RgbColor{0,0,128}), etc.
    /// <para></para>
    /// --> Strings may be used as well, e.g. SetEndCapColor("red")
    /// </summary>
    /// <returns></returns>
 	bool SetEndCapColor(const Sage::CRgbColor rgbColor);
#if 0
    /// <summary>
    /// Sets the color of the End Cap of the line.  When not specified, the color is the same color as the vector line itself.
    /// <para></para>
    /// --> Rgb Colors may be used, e.g. SetEndCapColor(PanColor::Red), SetEndCapColor(MyColor), SetEndCapColor(RgbColor{0,0,128}), etc.
    /// <para></para>
    /// --> Strings may be used as well, e.g. SetEndCapColor("red")
    /// </summary>
    /// <returns></returns>
    bool SetEndCapColor(const char * sCapColor);
#endif
#if 0
    /// <summary>
    /// Sets the color of the vector line and end caps (if any).
    /// <para></para>
    /// --> Start and End Caps may be set with SetBegCap() and SetEndCap() keywords.  Defaults are round and ArrowAnchor, respectively.
    /// <para></para>
    /// --> Rgb Colors may be used, e.g. SetLineColor(PanColor::Red), SetLineColor(MyColor), SetLineColor(RgbColor{0,0,128}), etc.
    /// <para></para>
    /// --> Strings may be used as well, e.g. SetLineColor("red")
    /// <para></para>
    /// </summary>
    /// <param name="Color"> - Color of the line, either as text (e.g. "red") or as an RgbColor</param>
    /// <returns></returns>
 	bool SetLineColor(const char * sColor);
#endif
    /// <summary>
    /// Sets the color of the vector line and end caps (if any).
    /// <para></para>
    /// --> Start and End Caps may be set with SetBegCap() and SetEndCap() keywords.  Defaults are round and ArrowAnchor, respectively.
    /// <para></para>
    /// --> Rgb Colors may be used, e.g. SetLineColor(PanColor::Red), SetLineColor(MyColor), SetLineColor(RgbColor{0,0,128}), etc.
    /// <para></para>
    /// --> Strings may be used as well, e.g. SetLineColor("red")
    /// <para></para>
    /// </summary>
    /// <param name="Color"> - Color of the line, either as text (e.g. "red") or as an RgbColor</param>
    /// <returns></returns>
	bool SetLineColor(const Sage::CRgbColor rgbColor);

    /// <summary>
    /// Sets the size of the start cap of the line. This function sets a multplier based on the line size.
    /// Default value is 1.0, with a minimum of .5 for the multiplier.
    /// <para></para>
    /// As the line width grows, the anchor size will also grow, applying this multiplier, or using the default value of 1.0.
    /// <para></para>
    /// --> This only works with Anchor types, e.g. ArrowAnchor, CircleAnchor, etc.
    /// <para></para>
    /// Non-anchor types are static, adjusting to the line size.
    /// <para></para>
    /// ** note: See SetCapSize() to set the cap size of Start and End Caps together with one keyword.
    /// </summary>
    /// <param name="CapSize"> - Multiplier for default (1.0) cap size.</param>
	bool SetBegCapSize(int iCapSize);

    /// <summary>
    /// Sets the size of the start cap of the line. This function sets a multplier based on the line size.
    /// Default value is 1.0, with a minimum of .5 for the multiplier.
    /// <para></para>
    /// As the line width grows, the anchor size will also grow, applying this multiplier, or using the default value of 1.0.
    /// <para></para>
    /// --> This only works with Anchor types, e.g. ArrowAnchor, CircleAnchor, etc.
    /// <para></para>
    /// Non-anchor types are static, adjusting to the line size.
    /// <para></para>
    /// ** note: See SetCapSize() to set the cap size of Start and End Caps together with one keyword.
    /// </summary>
    /// <param name="CapSize"> - Multiplier for default (1.0) cap size.</param>
    bool SetBegCapSize(float fCapSize);

    /// <summary>
    /// Sets the size of the start cap of the line. This function sets a multplier based on the line size.
    /// Default value is 1.0, with a minimum of .5 for the multiplier.
    /// <para></para>
    /// As the line width grows, the anchor size will also grow, applying this multiplier, or using the default value of 1.0.
    /// <para></para>
    /// --> This only works with Anchor types, e.g. ArrowAnchor, CircleAnchor, etc.
    /// <para></para>
    /// Non-anchor types are static, adjusting to the line size.
    /// <para></para>
    /// ** note: See SetCapSize() to set the cap size of Start and End Caps together with one keyword.
    /// </summary>
    /// <param name="CapSize"> - Multiplier for default (1.0) cap size.</param>
    bool SetBegCapSize(double fCapSize);

    /// <summary>
    /// Sets the size of the end cap of the line. This function sets a multplier based on the line size.
    /// Default value is 1.0, with a minimum of .5 for the multiplier.
    /// <para></para>
    /// As the line width grows, the anchor size will also grow, applying this multiplier, or using the default value of 1.0.
    /// <para></para>
    /// --> This only works with Anchor types, e.g. ArrowAnchor, CircleAnchor, etc.
    /// <para></para>
    /// Non-anchor types are static, adjusting to the line size.
    /// <para></para>
    /// ** note: See SetCapSize() to set the cap size of Start and End Caps together with one keyword.
    /// </summary>
    /// <param name="CapSize"> - Multiplier for default (1.0) cap size.</param>
	bool SetEndCapSize(int iCapSize);

    /// <summary>
    /// Sets the size of the end cap of the line. This function sets a multplier based on the line size.
    /// Default value is 1.0, with a minimum of .5 for the multiplier.
    /// <para></para>
    /// As the line width grows, the anchor size will also grow, applying this multiplier, or using the default value of 1.0.
    /// <para></para>
    /// --> This only works with Anchor types, e.g. ArrowAnchor, CircleAnchor, etc.
    /// <para></para>
    /// Non-anchor types are static, adjusting to the line size.
    /// <para></para>
    /// ** note: See SetCapSize() to set the cap size of Start and End Caps together with one keyword.
    /// </summary>
    /// <param name="CapSize"> - Multiplier for default (1.0) cap size.</param>
	bool SetEndCapSize(float fCapSize);

    /// <summary>
    /// Sets the size of the end cap of the line. This function sets a multplier based on the line size.
    /// Default value is 1.0, with a minimum of .5 for the multiplier.
    /// <para></para>
    /// As the line width grows, the anchor size will also grow, applying this multiplier, or using the default value of 1.0.
    /// <para></para>
    /// --> This only works with Anchor types, e.g. ArrowAnchor, CircleAnchor, etc.
    /// <para></para>
    /// Non-anchor types are static, adjusting to the line size.
    /// <para></para>
    /// ** note: See SetCapSize() to set the cap size of Start and End Caps together with one keyword.
    /// </summary>
    /// <param name="CapSize"> - Multiplier for default (1.0) cap size.</param>
    bool SetEndCapSize(double fCapSize);


    /// <summary>
    /// Sets the size of the start and end cap of the line. This function sets a multplier based on the line size.
    /// Default value is 1.0, with a minimum of .5 for the multiplier.
    /// <para></para>
    /// As the line width grows, the anchor size will also grow, applying this multiplier, or using the default value of 1.0.
    /// <para></para>
    /// --> This only works with Anchor types, e.g. ArrowAnchor, CircleAnchor, etc.
    /// <para></para>
    /// Non-anchor types are static, adjusting to the line size.
    /// <para></para>
    /// ** note: See SetBegCapSize() and SetEndCapSize() to set cap sizes for Start and End Caps individually.
    /// </summary>
	bool SetCapSize(int iCapSize);

    /// <summary>
    /// Sets the size of the start and end cap of the line. This function sets a multplier based on the line size.
    /// Default value is 1.0, with a minimum of .5 for the multiplier.
    /// <para></para>
    /// As the line width grows, the anchor size will also grow, applying this multiplier, or using the default value of 1.0.
    /// <para></para>
    /// --> This only works with Anchor types, e.g. ArrowAnchor, CircleAnchor, etc.
    /// <para></para>
    /// Non-anchor types are static, adjusting to the line size.
    /// <para></para>
    /// ** note: See SetBegCapSize() and SetEndCapSize() to set cap sizes for Start and End Caps individually.
    /// </summary>
    bool SetCapSize(float fCapSize);

    /// <summary>
    /// Sets the size of the start and end cap of the line. This function sets a multplier based on the line size.
    /// Default value is 1.0, with a minimum of .5 for the multiplier.
    /// <para></para>
    /// As the line width grows, the anchor size will also grow, applying this multiplier, or using the default value of 1.0.
    /// <para></para>
    /// --> This only works with Anchor types, e.g. ArrowAnchor, CircleAnchor, etc.
    /// <para></para>
    /// Non-anchor types are static, adjusting to the line size.
    /// <para></para>
    /// ** note: See SetBegCapSize() and SetEndCapSize() to set cap sizes for Start and End Caps individually.
    /// </summary>
    bool SetCapSize(double fCapSize);

    /// <summary>
    /// Sets the opacity of the vector line, 0-255.  This includes start and end caps if used.
    /// <para></para>
    /// --> The label is also set to this opacity unless SetLabelOpacity() is used to individually set the label opacity.
    /// </summary>
    /// <param name="iOpacity"> - Opacity value of the line (and label). 0 = completely transparent.  255 = compeltely opaque.</param>
    /// <returns></returns>
 	bool SetOpacity(int iOpacity);

    /// <summary>
    /// Sets the label/title of the vector line.
    /// <para></para>
    /// --> The Vector label is displayed in a default font.  
    /// <para></para>
    /// --> By default, the label is centered above the vector line.
    /// <para></para>
    /// --> Use SetLabelFont() to set a specific font, or SetTextSize() to set small, large,etc. (default is small).
    /// <para></para>
    /// </summary>
    /// <param name="sText"> - Label Text</param>
    /// <returns></returns>
    bool SetLabel(const char * sText);

    /// <summary>
    /// Sets the angle (in degrees) for the vector.
    /// <para></para>
    /// This calculates the center of the vector and rotates it by the specified angle.
    /// <para></para>
    /// --> Use SetAngleRad() to use Radians. 
    /// <para></para>
    /// --> This is useful when using SetCenter() to orient the vector around a specific point.
    /// <para></para>
    /// ** note: SetAngleDeg() and SetAngle() are the same function. SetAngle() defaults to degrees.
    /// </summary>
    /// <param name="Angle"> - Angle (in degrees) to rotate the vector</param>
    /// <returns></returns>
	bool SetAngleDeg(int iAngle);

    /// <summary>
    /// Sets the angle (in degrees) for the vector.
    /// <para></para>
    /// This calculates the center of the vector and rotates it by the specified angle.
    /// <para></para>
    /// --> Use SetAngleRad() to use Radians. 
    /// <para></para>
    /// --> This is useful when using SetCenter() to orient the vector around a specific point.
    /// <para></para>
    /// ** note: SetAngleDeg() and SetAngle() are the same function. SetAngle() defaults to degrees.
    /// </summary>
    /// <param name="Angle"> - Angle (in degrees) to rotate the vector</param>
    /// <returns></returns>
	bool SetAngleDeg(float fAngle);

    /// <summary>
    /// Sets the angle (in degrees) for the vector.
    /// <para></para>
    /// This calculates the center of the vector and rotates it by the specified angle.
    /// <para></para>
    /// --> Use SetAngleRad() to use Radians. 
    /// <para></para>
    /// --> This is useful when using SetCenter() to orient the vector around a specific point.
    /// <para></para>
    /// ** note: SetAngleDeg() and SetAngle() are the same function. SetAngle() defaults to degrees.
    /// </summary>
    /// <param name="Angle"> - Angle (in degrees) to rotate the vector</param>
    /// <returns></returns>
    bool SetAngleDeg(double fAngle);

    /// <summary>
    /// Sets the angle (in degrees) for the vector.
    /// <para></para>
    /// This calculates the center of the vector and rotates it by the specified angle.
    /// <para></para>
    /// --> Use SetAngleRad() to use Radians. 
    /// <para></para>
    /// --> This is useful when using SetCenter() to orient the vector around a specific point.
    /// <para></para>
    /// ** note: SetAngleDeg() and SetAngle() are the same function. SetAngle() defaults to degrees.
    /// </summary>
    /// <param name="Angle"> - Angle (in degrees) to rotate the vector</param>
    /// <returns></returns>
    bool SetAngle(int iAngle);

    /// <summary>
    /// Sets the angle (in degrees) for the vector.
    /// <para></para>
    /// This calculates the center of the vector and rotates it by the specified angle.
    /// <para></para>
    /// --> Use SetAngleRad() to use Radians. 
    /// <para></para>
    /// --> This is useful when using SetCenter() to orient the vector around a specific point.
    /// <para></para>
    /// ** note: SetAngleDeg() and SetAngle() are the same function. SetAngle() defaults to degrees.
    /// </summary>
    /// <param name="Angle"> - Angle (in degrees) to rotate the vector</param>
    /// <returns></returns>
    bool SetAngle(float fAngle);

    /// <summary>
    /// Sets the angle (in degrees) for the vector.
    /// <para></para>
    /// This calculates the center of the vector and rotates it by the specified angle.
    /// <para></para>
    /// --> Use SetAngleRad() to use Radians. 
    /// <para></para>
    /// --> This is useful when using SetCenter() to orient the vector around a specific point.
    /// <para></para>
    /// ** note: SetAngleDeg() and SetAngle() are the same function. SetAngle() defaults to degrees.
    /// </summary>
    /// <param name="Angle"> - Angle (in degrees) to rotate the vector</param>
    /// <returns></returns>
    bool SetAngle(double fAngle);

    /// <summary>
    /// Sets the angle (in radians) for the vector.
    /// <para></para>
    /// This calculates the center of the vector and rotates it by the specified angle.
    /// <para></para>
    /// --> Use SetAngle() or SetAngleDeg() to use Degrees. 
    /// <para></para>
    /// --> This is useful when using SetCenter() to orient the vector around a specific point.
    /// </summary>
    /// <param name="Angle"> - Angle (in radians) to rotate the vector</param>
    /// <returns></returns>
    bool SetAngleRad(float fAngle);

    /// <summary>
    /// Sets the angle (in radians) for the vector.
    /// <para></para>
    /// This calculates the center of the vector and rotates it by the specified angle.
    /// <para></para>
    /// --> Use SetAngle() or SetAngleDeg() to use Degrees. 
    /// <para></para>
    /// --> This is useful when using SetCenter() to orient the vector around a specific point.
    /// </summary>
    /// <param name="Angle"> - Angle (in radians) to rotate the vector</param>
    /// <returns></returns>
    bool SetAngleRad(double fAngle);

    /// <summary>
    /// Sets the vector label/title font to a specific font, either text,integer, or HGont as input.
    /// <para></para>
    /// --> Example:  SetLabelFont(50), SetLabelFont("Arial,100"), SetLabelFont("MyFont"), SetLabelFont(hMyFont)
    /// <para></para>
    /// --> See: SetTextSize() to set small, xsmall, large, etc. (default is 'small') using the default font.
    /// </summary>
    /// <param name="Font"> - Text, Integer, or Hfont font.</param>
    /// <returns></returns>
	bool SetLabelFont(HFONT hFont);

    /// <summary>
    /// Sets the vector label/title font to a specific font, either text,integer, or HGont as input.
    /// <para></para>
    /// --> Example:  SetLabelFont(50), SetLabelFont("Arial,100"), SetLabelFont("MyFont"), SetLabelFont(hMyFont)
    /// <para></para>
    /// --> See: SetTextSize() to set small, xsmall, large, etc. (default is 'small') using the default font.
    /// </summary>
    /// <param name="Font"> - Text, Integer, or Hfont font.</param>
    /// <returns></returns>
	bool SetLabelFont(int iFontSize);

    /// <summary>
    /// Sets the vector label/title font to a specific font, either text,integer, or HGont as input.
    /// <para></para>
    /// --> Example:  SetLabelFont(50), SetLabelFont("Arial,100"), SetLabelFont("MyFont"), SetLabelFont(hMyFont)
    /// <para></para>
    /// --> See: SetTextSize() to set small, xsmall, large, etc. (default is 'small') using the default font.
    /// </summary>
    /// <param name="Font"> - Text, Integer, or Hfont font.</param>
    /// <returns></returns>
    bool SetLabelFont(const char * sFont);

    /// <summary>
    /// Adjusts the label text of the vector left or right by a number of pixels.
    /// <para></para>
    /// --> Negative values move the label to the left, positive values moving the label to the right.
    /// <para></para>
    /// --> Movement is relative to the line, with left the first vector coordinate and right being the second.
    /// <para></para>
    /// --> See: SetLabelPadY() to move the label up or down (relative to the vector's orientation).
    /// <para></para>
    /// ** note: SetLabelPad(), SetLabelPadX() are the same function.
    /// </summary>
    /// <param name="iPadX"></param>
    /// <returns></returns>
	bool SetLabelPad(int iPadX,int iPadY);

    /// <summary>
    /// Adjusts the label text of the vector left or right by a number of pixels.
    /// <para></para>
    /// --> Negative values move the label to the left, positive values moving the label to the right.
    /// <para></para>
    /// --> Movement is relative to the line, with left the first vector coordinate and right being the second.
    /// <para></para>
    /// --> See: SetLabelPadY() to move the label up or down (relative to the vector's orientation).
    /// <para></para>
    /// ** note: SetLabelPad(), SetLabelPadX() are the same function.
    /// </summary>
    /// <param name="iPadX"></param>
    /// <returns></returns>
    bool SetLabelPadX(int iPadX);

    /// <summary>
    /// Adjusts the label text of the vector up or down by a number of pixels.
    /// <para></para>
    /// --> Negative values move the label up, positive values moving the label down.
    /// <para></para>
    /// --> Movement is relative to the line, with up relative to the center moving away from the line, and down moving towards the line.
    /// <para></para>
    /// --> See: SetLabelPadX() to move the label left or right (relative to the vector's orientation).
    /// </summary>
    /// </summary>
    /// <param name="iPadY"></param>
    /// <returns></returns>
	bool SetLabelPadY(int iPadY);

    /// <summary>
    /// Sets the opacity of the label. 
    /// The label will inherit the opacity of the line by default.  If the line's opacity is changed, the label also changes
    /// to the same opacity level unless LabelOpacity is used.
    /// <para></para>
    /// --> LabelOpacity() provides a way to have a different opacity for the label text than the lines opacity.
    /// <para></para>
    /// </summary>
    /// <param name="iOpacity"> - Opacity level 0-255. 0 is completely tranparent, 255 is completely opaque (default is 255)</param>
    bool SetLabelOpacity(int iOpacity);

    /// <summary>
    /// Sets the color of the label text.  
    /// By default, the label color is white. 
    /// <para></para>
    /// Using SetLabelColor() changes the color of the text of the label.
    /// <para></para>
    /// --> Rgb Colors may be used, e.g. SetLabelColor(PanColor::Red), SetLabelColor(MyColor), SetLabelColor(RgbColor{0,0,128}), etc.
    /// <para></para>
    /// --> Strings may be used as well, e.g. SetLabelColor("red")
    /// </summary>
    /// <param name="rgbColor"> - Color to set the label text.</param>
    /// <returns></returns>
    bool SetLabelColor(RgbColor rgbColor);

    /// <summary>
    /// Sets the color of the label text.  
    /// By default, the label color is white. 
    /// <para></para>
    /// Using SetLabelColor() changes the color of the text of the label.
    /// <para></para>
    /// --> Rgb Colors may be used, e.g. SetLabelColor(PanColor::Red), SetLabelColor(MyColor), SetLabelColor(RgbColor{0,0,128}), etc.
    /// <para></para>
    /// --> Strings may be used as well, e.g. SetLabelColor("red")
    /// </summary>
    /// <param name="rgbColor"> - Color to set the label text.</param>
    /// <returns></returns>
	bool SetLabelColor(const char * sColor);

    /// <summary>
    /// Sets the center of the vector.  This allows vectors of a length and orientation to be specified, without a specific point of origin.
    /// <para></para>
    /// SetCenter() sets the center of the vector.  The original angle between the two input points and any other angle changes is kept and
    /// the new start and end points are calculated based on the center input.
    /// <para></para>
    /// This allows non-descript and generic vectors to be defined and re-defined. 
    /// </summary>
    /// <param name="pCenter"> - Center of vector in screen coordinates.</param>
    bool SetCenter(POINT pCenter);

    /// <summary>
    /// Sets the center of the vector.  This allows vectors of a length and orientation to be specified, without a specific point of origin.
    /// <para></para>
    /// SetCenter() sets the center of the vector.  The original angle between the two input points and any other angle changes is kept and
    /// the new start and end points are calculated based on the center input.
    /// <para></para>
    /// This allows non-descript and generic vectors to be defined and re-defined. 
    /// </summary>
    /// <param name="pCenter"> - Center of vector in screen coordinates.</param>
    bool SetCenter(CfPoint pCenter);

    /// <summary>
    /// Puts a 10-pixel diameter filled circle in the center of the vector.  This allows ensuring the center of the vector is where it
    /// is intended.
    /// <para></para>
    /// Generally, ShowCenter() shows where the DrawVector() function calculated the center of the vector. 
    /// <para></para>
    /// --> The default color for the filled circle is Cyan, but can be changed.
    /// <para></para>
    /// --> Rgb Colors may be used, e.g. ShowCenter(PanColor::Red), ShowCenter(MyColor), ShowCenter(RgbColor{0,0,128}), etc.
    /// <para></para>
    /// --> Strings may be used as well, e.g. ShowCenter("red")
    /// </summary>
    /// <param name="sColor"> - [optional] Color to set the ShowCenter() filled circle</param>
    /// <returns></returns>
	bool ShowCenter(const char * sColor);
	bool ShowCenter(RgbColor rgbColor = SageColor::Cyan);
	
    /// <summary>
    /// Sets the line thickness of the vector. This can be used to change the line size prior to redrawing a vector.
    /// </summary>
    /// <param name="fLineSize"></param>
    /// <returns></returns>
    bool SetLinesize(double fLineSize);

    /// <summary>
    /// Sets the vector points p1 and p2 of the vector.  This can be used to change the p1 and p2 vector points prior to redrawing a vector.
    /// </summary>
    /// <param name="p1"> - first vector coordinate</param>
    /// <param name="p2"> - secont vector coordinate</param>
    /// <returns></returns>
    bool SetVecPoints(const CfPoint p1,const CfPoint p2);
};

} // namespace Sage
