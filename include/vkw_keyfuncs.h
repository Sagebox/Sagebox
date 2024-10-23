#pragma once

// ---------------------------------------------------------------------------------------------
// Keyword definitions for Function-Based Keywords.  This is the type of keyword used in Sagebox
// ---------------------------------------------------------------------------------------------

// This file contains the Functional-Based keywords vkw (main usage) and _vkw (example underlined usage)
// --> See vkw_keyfuncs_packed.h for packed-parameter usage, implemented as an example of such usage.

// 
// Other types are implemented as examples of using keywords in different ways.
//
// The following are the types of keywords implemented in the CDrawVector project.
//
//  ---- in vkw_keyfuncs.h (this file) ---
//
// 1. Functional Keywords (vkw), e.g. MyFunction(parm1,parm2,kw::keyword1(value) + kw::keyword2(value)).  <-- this file (this is the main usage)
// 2. Functional Keywords underlined (_vkw).  This is used as an example of using underlines to specify keywords, making 'using statements' easier,
//    example:
//
//      using namespace _vkw
//      MyFunction(parm1,parm2,_keyword1(value) + _keyword2(value))
//      
//  ---- in vkw_keyfuncs_packed.h ---
// 
// 3. Packed-Parameter Keywords (vkw2).  This is used as an example of using 'classic' assignment-based keywords, such as 'Color="Red"'
//    vs. Color("red"), for example.  
// 
//    Packed-Parameter keywords allow for using ',' between keyword usages, rather than streaming as with '+'
//
//    The equivalent usage for the example #1 above is: 
//
//      MyFunction(parm1,parm2,kw::keyword1 = value, kw::keyword2 =value).
//
//    or, to remove the kw:: namespace, allowing for a sense of true classic keywords as seen in scripting languages: 
// 
//      using namespace vkw2;
// 
//      MyFunction(parm1,parm2,keyword1 = value, keyword2 =value).
// 
// 
// 4. Packed-Parameter keywords underlined (_vkw2).  Same as the vkw2 space but with underlines, to make 
//    the 'using namespace _vkw2' a little easier and to more easily understand where keywords are being used:
// 
//      Example:
// 
//          using namespace _vkw2;
// 
//          MyFunction(parm1,parm2,_keyword1 = value, _keyword2 =value).
// 
// ------------------------------------------------------------------------------ 
// The following documentation is from the Ckwargs project original documentation
// ------------------------------------------------------------------------------ 

// ******************************** Example my_keyfuncs.h file **********************************************************
//
// Example KeyFunc Usage: AddBorder, Range(1,10), BorderSize(5)
// 
// --------------------------
// Keyword-Style Example File
// --------------------------
// 
// This file shows setting up function-based keyword-sytyle keywords (i.e. AddBorder, Range(1,10), BorderSize(5), etc.)
// 
// See the file my_keywords.h for setting up canonical, assignment-based keywords (i.e. AddBorder=true, 
// Range = { 1, 10}, BorderSize=5); 
// 
// my_keyfuncs.h and my_keywords.h are generally not meant to be used together -- the idea is to choose which format you
// want for your keywords and use one of these files as a starting template.
// 
// -------------------------------
// namespace vs. class (or struct)
// -------------------------------
// 
// note: keep in mind that namespaces can also be declared as classes, which can be easier because:
// 
//   1. as a namespace, inlined functions declared as static are duplicated for all
//      source files that include "my_keywords.h"
//   2. This can be prevented by moving the code from inline to a separate
//      C++ file, but does require the work to do so
// 
//   --> A recommendation is to just use static functions in the namespacee or class at first,
//       and then decide whether the project is large enough to support the extra work.
// 
// --> Namespaces vs. classes for the kw and kf namespaces below are only useful if you want to unscope keywords, to use
//     keywords like 'BorderSize=50' vs. 'kw::BorderSize=50' by specifying "using namespace kw" in the function or at the top of the file.
//  
// --> struct can also be used instead of a class, and doesn't require 'public' after the class definition.
// 
// ***********************************************************************************************************************

#include "vkw_ckwargs.h"   
#include "vkw_keyfuncs_packed.h"
// ----------------------------------------------------------------
// Function-Based Keywords namespace/class/struct for program usage
// ----------------------------------------------------------------
// 
// When kf:: is entered, the intellisense will show all available keywords and will also show any comments 
//
// For this example, only the last two functions are commented, i.e. kf::Text will show intellisense information
// about the keyword.
// 
// Note that the namespace below is 'kf' where it is 'kw' in my_keywords.h.  This is just nomenclature (i.e. 'kw' for
// 'keywords' and 'kf' for keyfunctions) -- normally both forms wouldn't be used together, so naming would not be an issue.


// Declare namespace for our keyword functions
// 
// 'kf' is an arbitrarily-named namespace, class, or struct you can set here or include in a class. 
// 
// the 'namespace kf' below can be a struct or a class, and is used as a namespace below
// to use examples where the keyword is unscoped, i.e. 'BorderSize(123)' vs. 'kw::BorderSize(123)', which
// is not currently possible in C++ with a class or struct.
//
// To use 'namespace kf' as a class or struct, i.e. 'struct kf', change
// 'extern struct' statements to 'static struct' -- a class definition will also require 'public:' as the first declaration.

namespace Sage
{
    using vkwOpt = vkwType::ckw;

namespace vkw  // rename to whatever fits your program.  Or bring it in as a class/struct into a class.
{
     extern const vkwType::ckw none;

    // Sets Cap Type for start of vector line.  
    // Use Sage::LineCapType::  (i.e. StartCap(LineCapType::Arrow))
    // or use strings, such as BegCap("arrow"); 
    //
    // --> Valid types are Flat, Arrow, Round, Square, Diamond, ArrowAnchor, RoundAnchor, SquareAnchor, and DiamondAnchor
    //
    // Default for start cap is Round. Default for EndCap is ArrowAnchor. 
    //
    // 'Anchor' types are larger.  Non-anchor types are the same size of the line.  To set Cap sizes, use Anchor Types.
    // See SetCapSize(), BegCapSize() and EndCapSize()
    vkwType::ckw BegCap(const char * sText);

    // Sets Cap Type for start of vector line.  
    // Use Sage::LineCapType::  (i.e. StartCap(LineCapType::Arrow))
    // or use strings, such as BegCap("arrow"); 
    //
    // --> Valid types are Flat, Arrow, Round, Square, Diamond, ArrowAnchor, RoundAnchor, SquareAnchor, and DiamondAnchor
    //
    // Default for start cap is Round. Default for EndCap is ArrowAnchor. 
    //
    // 'Anchor' types are larger.  Non-anchor types are the same size of the line.  To set Cap sizes, use Anchor Types.
    // See SetCapSize(), BegCapSize() and EndCapSize()
    vkwType::ckw BegCap(LineCapType eCapType);

    // Sets Cap Type for end of vector line.  
    // Use Sage::LineCapType::  (i.e. StartCap(LineCapType::Arrow))
    // or use strings, such as EndCap("arrow"); 
    //
    // --> Valid types are Flat, Arrow, Round, Square, Diamond, ArrowAnchor, RoundAnchor, SquareAnchor, and DiamondAnchor
    //
    // Default for start cap is Round. Default for EndCap is ArrowAnchor. 
    //
    // 'Anchor' types are larger.  Non-anchor types are the same size of the line.  To set Cap sizes, use Anchor Types.
    // See SetCapSize(), BegCapSize() and EndCapSize()
    vkwType::ckw EndCap(const char * sText);

    // Sets Cap Type for end of vector line.  
    // Use Sage::LineCapType::  (i.e. StartCap(LineCapType::Arrow))
    // or use strings, such as EndCap("arrow"); 
    //
    // --> Valid types are Flat, Arrow, Round, Square, Diamond, ArrowAnchor, RoundAnchor, SquareAnchor, and DiamondAnchor
    //
    // Default for start cap is Round. Default for EndCap is ArrowAnchor. 
    //
    // 'Anchor' types are larger.  Non-anchor types are the same size of the line.  To set Cap sizes, use Anchor Types.
    // See SetCapSize(), BegCapSize() and EndCapSize()
    vkwType::ckw EndCap(LineCapType eCapType)      ;

    /// <summary>
    /// Sets the color of the Start Cap of the line.  When not specified, the color is the same color as the vector line itself.
    /// <para></para>
    /// --> Rgb Colors may be used, e.g. BegCapColor(PanColor::Red), BegCapColor(MyColor), BegCapColor(RgbColor{0,0,128}), etc.
    /// <para></para>
    /// --> Strings may be used as well, e.g. BegCapColor("red")
    /// </summary>
    /// <returns></returns>
    vkwType::ckw BegCapColor(const Sage::RgbColor rgbColor);

    /// <summary>
    /// Sets the color of the Start Cap of the line.  When not specified, the color is the same color as the vector line itself.
    /// <para></para>
    /// --> Rgb Colors may be used, e.g. BegCapColor(PanColor::Red), BegCapColor(MyColor), BegCapColor(RgbColor{0,0,128}), etc.
    /// <para></para>
    /// --> Strings may be used as well, e.g. BegCapColor("red")
    /// </summary>
    /// <returns></returns>
    vkwType::ckw BegCapColor(const char * sCapColor);
 
    /// <summary>
    /// Sets the color of the End Cap of the line.  When not specified, the color is the same color as the vector line itself.
    /// <para></para>
    /// --> Rgb Colors may be used, e.g. EndCapColor(PanColor::Red), EndCapColor(MyColor), EndCapColor(RgbColor{0,0,128}), etc.
    /// <para></para>
    /// --> Strings may be used as well, e.g. EndCapColor("red")
    /// </summary>
    /// <returns></returns>
    vkwType::ckw EndCapColor(const Sage::RgbColor rgbColor);

    /// <summary>
    /// Sets the color of the End Cap of the line.  When not specified, the color is the same color as the vector line itself.
    /// <para></para>
    /// --> Rgb Colors may be used, e.g. EndCapColor(PanColor::Red), EndCapColor(MyColor), EndCapColor(RgbColor{0,0,128}), etc.
    /// <para></para>
    /// --> Strings may be used as well, e.g. EndCapColor("red")
    /// </summary>
    /// <returns></returns>
    vkwType::ckw EndCapColor(const char * sCapColor)      ;

    /// <summary>
    /// Sets the color of the vector line and end caps (if any).
    /// <para></para>
    /// --> Start and End Caps may be set with BegCap() and EndCap() keywords.  Defaults are round and ArrowAnchor, respectively.
    /// <para></para>
    /// --> Rgb Colors may be used, e.g. Color(PanColor::Red), Color(MyColor), Color(RgbColor{0,0,128}), etc.
    /// <para></para>
    /// --> Strings may be used as well, e.g. Color("red")
    /// <para></para>
    /// ** Note: Color and LineColor() are the same function.
    /// </summary>
    /// <param name="sColor"></param>
    /// <returns></returns>
    vkwType::ckw Color(const char * sColor);

    /// <summary>
    /// Sets the color of the vector line and end caps (if any).
    /// <para></para>
    /// --> Start and End Caps may be set with BegCap() and EndCap() keywords.  Defaults are round and ArrowAnchor, respectively.
    /// <para></para>
    /// --> Rgb Colors may be used, e.g. Color(PanColor::Red), Color(MyColor), Color(RgbColor{0,0,128}), etc.
    /// <para></para>
    /// --> Strings may be used as well, e.g. Color("red")
    /// <para></para>
    /// ** Note: Color and LineColor() are the same function.
    /// </summary>
    /// <param name="sColor"></param>
    /// <returns></returns>
    vkwType::ckw Color(const Sage::RgbColor rgbColor);

    /// <summary>
    /// Sets the color of the vector line and end caps (if any).
    /// <para></para>
    /// --> Start and End Caps may be set with BegCap() and EndCap() keywords.  Defaults are round and ArrowAnchor, respectively.
    /// <para></para>
    /// --> Rgb Colors may be used, e.g. LineColor(PanColor::Red), LineColor(MyColor), LineColor(RgbColor{0,0,128}), etc.
    /// <para></para>
    /// --> Strings may be used as well, e.g. LineColor("red")
    /// <para></para>
    /// ** Note: Color and LineColor() are the same function.
    /// </summary>
    /// <param name="sColor"></param>
    /// <returns></returns>
    vkwType::ckw LineColor(const char * sColor);

    /// <summary>
    /// Sets the color of the vector line and end caps (if any).
    /// <para></para>
    /// --> Start and End Caps may be set with BegCap() and EndCap() keywords.  Defaults are round and ArrowAnchor, respectively.
    /// <para></para>
    /// --> Rgb Colors may be used, e.g. LineColor(PanColor::Red), LineColor(MyColor), LineColor(RgbColor{0,0,128}), etc.
    /// <para></para>
    /// --> Strings may be used as well, e.g. LineColor("red")
    /// <para></para>
    /// ** Note: Color and LineColor() are the same function.
    /// </summary>
    /// <param name="sColor"></param>
    /// <returns></returns>
    vkwType::ckw LineColor(const Sage::RgbColor rgbColor);

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
    vkwType::ckw BegCapSize(int iCapSize);

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
    vkwType::ckw BegCapSize(float fCapSize);

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
    vkwType::ckw BegCapSize(double fCapSize);

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
    vkwType::ckw EndCapSize(int iCapSize);

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
    vkwType::ckw EndCapSize(float fCapSize);

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
    vkwType::ckw EndCapSize(double fCapSize);

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
    /// ** note: See BegCapSize() and EndCapSize() to set cap sizes for Start and End Caps individually.
    /// </summary>
    vkwType::ckw CapSize(int iCapSize);

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
    /// ** note: See BegCapSize() and EndCapSize() to set cap sizes for Start and End Caps individually.
    /// </summary>
    vkwType::ckw CapSize(float fCapSize);

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
    /// ** note: See BegCapSize() and EndCapSize() to set cap sizes for Start and End Caps individually.
    /// </summary>
    vkwType::ckw CapSize(double fCapSize)   ;

    /// <summary>
    /// Sets the opacity of the vector line, 0-255.  This includes start and end caps if used.
    /// <para></para>
    /// --> The label is also set to this opacity unless SetLabelOpacity() is used to individually set the label opacity.
    /// </summary>
    /// <param name="iOpacity"> - Opacity value of the line (and label). 0 = completely transparent.  255 = compeltely opaque.</param>
    /// <returns></returns>
    vkwType::ckw Opacity(int iOpacity);

    /// <summary>
    /// Sets the label/title of the vector line.
    /// <para></para>
    /// --> The Vector label is displayed in a default font.  
    /// <para></para>
    /// --> By default, the label is centered above the vector line.
    /// <para></para>
    /// --> Use LabelFont() to set a specific font, or SetTextSize() to set small, large,etc. (default is small).
    /// <para></para>
    /// ** note: Label() and Title() are the same keyword functions.
    /// </summary>
    /// <param name="sText"> - Label Text</param>
    /// <returns></returns>
    vkwType::ckw Title(const char * sText);

    /// <summary>
    /// Sets the label/title of the vector line.
    /// <para></para>
    /// --> The Vector label is displayed in a default font.  
    /// <para></para>
    /// --> By default, the label is centered above the vector line.
    /// <para></para>
    /// --> Use LabelFont() to set a specific font, or SetTextSize() to set small, large,etc. (default is small).
    /// <para></para>
    /// ** note: Label() and Title() are the same keyword functions.
    /// </summary>
    /// <param name="sText"> - Label Text</param>
    /// <returns></returns>
    vkwType::ckw Label(const char * sText);

    /// <summary>
    /// Sets the angle (in degrees) for the vector.
    /// <para></para>
    /// This calculates the center of the vector and rotates it by the specified angle.
    /// <para></para>
    /// --> Use AngleRad() to use Radians. 
    /// <para></para>
    /// --> This is useful when using SetCenter() to orient the vector around a specific point.
    /// <para></para>
    /// ** note: AngleDeg() and Angle() are the same function. Angle() defaults to degrees.
    /// </summary>
    /// <param name="Angle"> - Angle (in degrees) to rotate the vector</param>
    /// <returns></returns>
    vkwType::ckw AngleDeg(int iAngle);

    /// <summary>
    /// Sets the angle (in degrees) for the vector.
    /// <para></para>
    /// This calculates the center of the vector and rotates it by the specified angle.
    /// <para></para>
    /// --> Use AngleRad() to use Radians. 
    /// <para></para>
    /// --> This is useful when using SetCenter() to orient the vector around a specific point.
    /// <para></para>
    /// ** note: AngleDeg() and Angle() are the same function. Angle() defaults to degrees.
    /// </summary>
    /// <param name="Angle"> - Angle (in degrees) to rotate the vector</param>
    /// <returns></returns>
    vkwType::ckw AngleDeg(float fAngle) ;

    /// <summary>
    /// Sets the angle (in degrees) for the vector.
    /// <para></para>
    /// This calculates the center of the vector and rotates it by the specified angle.
    /// <para></para>
    /// --> Use AngleRad() to use Radians. 
    /// <para></para>
    /// --> This is useful when using SetCenter() to orient the vector around a specific point.
    /// <para></para>
    /// ** note: AngleDeg() and Angle() are the same function. Angle() defaults to degrees.
    /// </summary>
    /// <param name="Angle"> - Angle (in degrees) to rotate the vector</param>
    /// <returns></returns>
    vkwType::ckw AngleDeg(double fAngle);


    /// <summary>
    /// Sets the angle (in degrees) for the vector.
    /// <para></para>
    /// This calculates the center of the vector and rotates it by the specified angle.
    /// <para></para>
    /// --> Use AngleRad() to use Radians. 
    /// <para></para>
    /// --> This is useful when using SetCenter() to orient the vector around a specific point.
    /// <para></para>
    /// ** note: AngleDeg() and Angle() are the same function. Angle() defaults to degrees.
    /// </summary>
    /// <param name="Angle"> - Angle (in degrees) to rotate the vector</param>
    /// <returns></returns>
    vkwType::ckw Angle(int iAngle);

    /// <summary>
    /// Sets the angle (in degrees) for the vector.
    /// <para></para>
    /// This calculates the center of the vector and rotates it by the specified angle.
    /// <para></para>
    /// --> Use AngleRad() to use Radians. 
    /// <para></para>
    /// --> This is useful when using SetCenter() to orient the vector around a specific point.
    /// <para></para>
    /// ** note: AngleDeg() and Angle() are the same function. Angle() defaults to degrees.
    /// </summary>
    /// <param name="Angle"> - Angle (in degrees) to rotate the vector</param>
    /// <returns></returns>
    vkwType::ckw Angle(float fAngle) ;

    /// <summary>
    /// Sets the angle (in degrees) for the vector.
    /// <para></para>
    /// This calculates the center of the vector and rotates it by the specified angle.
    /// <para></para>
    /// --> Use AngleRad() to use Radians. 
    /// <para></para>
    /// --> This is useful when using SetCenter() to orient the vector around a specific point.
    /// <para></para>
    /// ** note: AngleDeg() and Angle() are the same function. Angle() defaults to degrees.
    /// </summary>
    /// <param name="Angle"> - Angle (in degrees) to rotate the vector</param>
    /// <returns></returns>
    vkwType::ckw Angle(double fAngle);

    /// <summary>
    /// Sets the angle (in radians) for the vector.
    /// <para></para>
    /// This calculates the center of the vector and rotates it by the specified angle.
    /// <para></para>
    /// --> Use Angle() or AngleDeg() to use Degrees. 
    /// <para></para>
    /// --> This is useful when using SetCenter() to orient the vector around a specific point.
    /// </summary>
    /// <param name="Angle"> - Angle (in radians) to rotate the vector</param>
    /// <returns></returns>
    vkwType::ckw AngleRad(float fAngle) ;

    /// <summary>
    /// Sets the angle (in radians) for the vector.
    /// <para></para>
    /// This calculates the center of the vector and rotates it by the specified angle.
    /// <para></para>
    /// --> Use Angle() or AngleDeg() to use Degrees. 
    /// <para></para>
    /// --> This is useful when using SetCenter() to orient the vector around a specific point.
    /// </summary>
    /// <param name="Angle"> - Angle (in radians) to rotate the vector</param>
    /// <returns></returns>
    vkwType::ckw AngleRad(double fAngle);

    /// <summary>
    /// Sets the vector label/title font to a specific font, either text,integer, or HGont as input.
    /// <para></para>
    /// --> Example:  LabelFont(50), LabelFont("Arial,100"), LabelFont("MyFont"), LabelFont(hMyFont)
    /// <para></para>
    /// --> See: SetTextSize() to set small, xsmall, large, etc. (default is 'small') using the default font.
    /// </summary>
    /// <param name="Font"> - Text, Integer, or Hfont font.</param>
    /// <returns></returns>
    vkwType::ckw LabelFont(HFONT hFont);

    /// <summary>
    /// Sets the vector label/title font to a specific font, either text,integer, or HGont as input.
    /// <para></para>
    /// --> Example:  LabelFont(50), LabelFont("Arial,100"), LabelFont("MyFont"), LabelFont(hMyFont)
    /// <para></para>
    /// --> See: SetTextSize() to set small, xsmall, large, etc. (default is 'small') using the default font.
    /// </summary>
    /// <param name="Font"> - Text, Integer, or Hfont font.</param>
    /// <returns></returns>
    vkwType::ckw LabelFont(int iFontSize);

    /// <summary>
    /// Sets the vector label/title font to a specific font, either text,integer, or HGont as input.
    /// <para></para>
    /// --> Example:  LabelFont(50), LabelFont("Arial,100"), LabelFont("MyFont"), LabelFont(hMyFont)
    /// <para></para>
    /// --> See: SetTextSize() to set small, xsmall, large, etc. (default is 'small') using the default font.
    /// </summary>
    /// <param name="Font"> - Text, Integer, or Hfont font.</param>
    /// <returns></returns>
    vkwType::ckw LabelFont(const char * sFont);

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
    vkwType::ckw LabelJust(vkwType::LabelJustType labelJust);

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
    vkwType::ckw LabelJust(const char * sLabelJust);

    /// <summary>
    /// Adjusts the label text of the vector left or right by a number of pixels.
    /// <para></para>
    /// --> Negative values move the label to the left, positive values moving the label to the right.
    /// <para></para>
    /// --> Movement is relative to the line, with left the first vector coordinate and right being the second.
    /// <para></para>
    /// --> See: LabelPadY() to move the label up or down (relative to the vector's orientation).
    /// <para></para>
    /// ** note: LabelPad() and LabelPadX() are the same function.
    /// </summary>
    /// <param name="iPadX"></param>
    /// <returns></returns>
    vkwType::ckw LabelPad(int iPadX);

    /// <summary>
    /// Adjusts the label text of the vector left or right by a number of pixels.
    /// <para></para>
    /// --> Negative values move the label to the left, positive values moving the label to the right.
    /// <para></para>
    /// --> Movement is relative to the line, with left the first vector coordinate and right being the second.
    /// <para></para>
    /// --> See: LabelPadY() to move the label up or down (relative to the vector's orientation).
    /// <para></para>
    /// ** note: LabelPad(), PadX(), and LabelPadX() are the same function.
    /// </summary>
    /// <param name="iPadX"></param>
    /// <returns></returns>
    vkwType::ckw LabelPadX(int iPadX);

    /// <summary>
    /// Adjusts the label text of the vector left or right by a number of pixels.
    /// <para></para>
    /// --> Negative values move the label to the left, positive values moving the label to the right.
    /// <para></para>
    /// --> Movement is relative to the line, with left the first vector coordinate and right being the second.
    /// <para></para>
    /// --> See: LabelPadY() to move the label up or down (relative to the vector's orientation).
    /// <para></para>
    /// ** note: LabelPad(), PadX(), and LabelPadX() are the same function.
    /// </summary>
    /// <param name="iPadX"></param>
    /// <returns></returns>
    vkwType::ckw PadX(int iPadX);

    /// <summary>
    /// Adjusts the label text of the vector up or down by a number of pixels.
    /// <para></para>
    /// --> Negative values move the label up, positive values moving the label down.
    /// <para></para>
    /// --> Movement is relative to the line, with up relative to the center moving away from the line, and down moving towards the line.
    /// <para></para>
    /// --> See: LabelPadX() to move the label left or right (relative to the vector's orientation).
    /// <para></para>
    /// ** note: LabelPad()Y and Pady() are the same function.
    /// </summary>
    /// <param name="iPadX"></param>
    /// <returns></returns>
    vkwType::ckw LabelPadY(int iPadY);

    /// <summary>
    /// Adjusts the label text of the vector up or down by a number of pixels.
    /// <para></para>
    /// --> Negative values move the label up, positive values moving the label down.
    /// <para></para>
    /// --> Movement is relative to the line, with up relative to the center moving away from the line, and down moving towards the line.
    /// <para></para>
    /// --> See: LabelPadX() to move the label left or right (relative to the vector's orientation).
    /// <para></para>
    /// ** note: LabelPad()Y and Pady() are the same function.
    /// </summary>
    /// <param name="iPadX"></param>
    /// <returns></returns>
    vkwType::ckw PadY(int iPadY);

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
    vkwType::ckw TextSize(vkwType::TextSizeType textSize);

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
    vkwType::ckw TextSize(const char * sTextSize);

    /// <summary>
    /// Sets the opacity of the label. 
    /// The label will inherit the opacity of the line by default.  If the line's opacity is changed, the label also changes
    /// to the same opacity level unless LabelOpacity is used.
    /// <para></para>
    /// --> LabelOpacity() provides a way to have a different opacity for the label text than the lines opacity.
    /// <para></para>
    /// ** note: LabelOpacity() and TextOpacity() are the same function
    /// </summary>
    /// <param name="iOpacity"> - Opacity level 0-255. 0 is completely tranparent, 255 is completely opaque (default is 255)</param>
    vkwType::ckw LabelOpacity(int iOpacity);

    /// <summary>
    /// Sets the opacity of the label. 
    /// The label will inherit the opacity of the line by default.  If the line's opacity is changed, the label also changes
    /// to the same opacity level unless LabelOpacity is used.
    /// <para></para>
    /// --> LabelOpacity() provides a way to have a different opacity for the label text than the lines opacity.
    /// <para></para>
    /// ** note: LabelOpacity() and TextOpacity() are the same function
    /// </summary>
    /// <param name="iOpacity"> - Opacity level 0-255. 0 is completely tranparent, 255 is completely opaque (default is 255)</param>
    vkwType::ckw TextOpacity(int iOpacity);

    /// <summary>
    /// Sets the color of the label text.  
    /// By default, the label color is white. 
    /// <para></para>
    /// Using LabelColor() changes the color of the text of the label.
    /// <para></para>
    /// --> Rgb Colors may be used, e.g. LabelColor(PanColor::Red), LabelColor(MyColor), LabelColor(RgbColor{0,0,128}), etc.
    /// <para></para>
    /// --> Strings may be used as well, e.g. LabelColor("red")
    /// </summary>
    /// <param name="rgbColor"> - Color to set the label text.</param>
    /// <returns></returns>
    vkwType::ckw LabelColor(RgbColor rgbColor);

    /// <summary>
    /// Sets the color of the label text.  
    /// By default, the label color is white. 
    /// <para></para>
    /// Using LabelColor() changes the color of the text of the label.
    /// <para></para>
    /// --> Rgb Colors may be used, e.g. LabelColor(PanColor::Red), LabelColor(MyColor), LabelColor(RgbColor{0,0,128}), etc.
    /// <para></para>
    /// --> Strings may be used as well, e.g. LabelColor("red")
    /// </summary>
    /// <param name="rgbColor"> - Color to set the label text.</param>
    /// <returns></returns>
    vkwType::ckw LabelColor(const char * sColor);

    /// <summary>
    /// Sets a label orientation for the the label text:
    /// <para></para>
    /// --> Horizontal.  Keeps the label horizontal (i.e. 0 degress relative to the line).  This is the default
    /// <para></para>
    /// --> Vertical.  Keeps the label -90 degrees, i.e. sideways on the line
    /// <para></para>
    /// --> Vertical180.  Keeps the label 90 degrees, i.e. sideways on the line,but rotated 180 degrees.
    /// <para></para>
    /// The orientation will track with the line, unless bStaticAngle = true.
    /// <para></para>
    /// When bStaticAngle is true, the angle will become static, with the text keeping to the same angle to the screen regardless of line orientation.
    /// <para></para>
    /// This allows the ability to always have a horizontal or vertical line with respect to the screen.
    /// <para></para>
    /// --> Types can be LabelAngleType, e.g. LabelAngleType(vkwType::LabelAngleType::Vertical180)
    /// <para></para>
    /// --> Types can be a string, e.g. LabelAngleType("Vertical180,static");
    /// </summary>
    /// <param name="angleType"> - Label angle orientation type, i.e. horizontal, vertical, vertical180</param>
    /// <param name="bStaticAngle"> - When false (default), angle is relative to the line angle.  When true, Angle is relative to the screen.</param>
    /// <returns></returns>
    vkwType::ckw LabelAngle(vkwType::LabelAngleType angleType,bool bStaticAngle = false);

    /// <summary>
    /// Sets a label orientation for the the label text:
    /// <para></para>
    /// --> Horizontal.  Keeps the label horizontal (i.e. 0 degress relative to the line).  This is the default
    /// <para></para>
    /// --> Vertical.  Keeps the label -90 degrees, i.e. sideways on the line
    /// <para></para>
    /// --> Vertical180.  Keeps the label 90 degrees, i.e. sideways on the line,but rotated 180 degrees.
    /// <para></para>
    /// The orientation will track with the line, unless bStaticAngle = true.
    /// <para></para>
    /// When bStaticAngle is true, the angle will become static, with the text keeping to the same angle to the screen regardless of line orientation.
    /// <para></para>
    /// This allows the ability to always have a horizontal or vertical line with respect to the screen.
    /// <para></para>
    /// --> Types can be LabelAngleType, e.g. LabelAngleType(vkwType::LabelAngleType::Vertical180)
    /// <para></para>
    /// --> Types can be a string, e.g. LabelAngleType("Vertical180,static");
    /// </summary>
    /// <param name="angleType"> - Label angle orientation type, i.e. horizontal, vertical, vertical180</param>
    /// <param name="bStaticAngle"> - When false (default), angle is relative to the line angle.  When true, Angle is relative to the screen.</param>
    /// <returns></returns>
    vkwType::ckw LabelAngle(const char * sAngleType);
    
    // Keeps the label right-side up when it would otherwise be upside-down on a line that has been rotated more than 90 degrees in either direction.
    //
    vkwType::ckw LabelUp(bool bLabelUp = true);

    /// <summary>
    /// Sets the center of the vector.  This allows vectors of a length and orientation to be specified, without a specific point of origin.
    /// <para></para>
    /// SetCenter() sets the center of the vector.  The original angle between the two input points and any other angle changes is kept and
    /// the new start and end points are calculated based on the center input.
    /// <para></para>
    /// This allows non-descript and generic vectors to be defined and re-defined, such as DrawVector({0,0},{200,0},kw::SetCenter(some_screen_value) + kw::Angle(45)) 
    /// </summary>
    /// <param name="pCenter"> - Center of vector in screen coordinates.</param>
    vkwType::ckw SetCenter(POINT pCenter);

    /// <summary>
    /// Sets the center of the vector.  This allows vectors of a length and orientation to be specified, without a specific point of origin.
    /// <para></para>
    /// SetCenter() sets the center of the vector.  The original angle between the two input points and any other angle changes is kept and
    /// the new start and end points are calculated based on the center input.
    /// <para></para>
    /// This allows non-descript and generic vectors to be defined and re-defined, such as DrawVector({0,0},{200,0},kw::SetCenter(some_screen_value) + kw::Angle(45)) 
    /// </summary>
    /// <param name="pCenter"> - Center of vector in screen coordinates.</param>
    vkwType::ckw SetCenter(CfPoint pCenter);

    /// <summary>
    /// Sets the center of the vector.  This allows vectors of a length and orientation to be specified, without a specific point of origin.
    /// <para></para>
    /// SetCenter() sets the center of the vector.  The original angle between the two input points and any other angle changes is kept and
    /// the new start and end points are calculated based on the center input.
    /// <para></para>
    /// This allows non-descript and generic vectors to be defined and re-defined, such as DrawVector({0,0},{200,0},kw::SetCenter(some_screen_value) + kw::Angle(45)) 
    /// </summary>
    /// <param name="pCenter"> - Center of vector in screen coordinates.</param>
    vkwType::ckw SetCenter(int iCenterX,int iCenterY);

    /// <summary>
    /// Sets the center of the vector.  This allows vectors of a length and orientation to be specified, without a specific point of origin.
    /// <para></para>
    /// SetCenter() sets the center of the vector.  The original angle between the two input points and any other angle changes is kept and
    /// the new start and end points are calculated based on the center input.
    /// <para></para>
    /// This allows non-descript and generic vectors to be defined and re-defined, such as DrawVector({0,0},{200,0},kw::SetCenter(some_screen_value) + kw::Angle(45)) 
    /// </summary>
    /// <param name="pCenter"> - Center of vector in screen coordinates.</param>
    vkwType::ckw SetCenter(double fCenterX,double fCenterY);

    /// <summary>
    /// Sets the center of the vector.  This allows vectors of a length and orientation to be specified, without a specific point of origin.
    /// <para></para>
    /// SetCenter() sets the center of the vector.  The original angle between the two input points and any other angle changes is kept and
    /// the new start and end points are calculated based on the center input.
    /// <para></para>
    /// This allows non-descript and generic vectors to be defined and re-defined, such as DrawVector({0,0},{200,0},kw::SetCenter(some_screen_value) + kw::Angle(45)) 
    /// </summary>
    /// <param name="pCenter"> - Center of vector in screen coordinates.</param>
    vkwType::ckw SetCenter(float fCenterX,float fCenterY);

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
    vkwType::ckw ShowCenter(const char * sColor);

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
    vkwType::ckw ShowCenter(RgbColor rgbColor = SageColor::Cyan);
    
    /// <summary>
    /// Sets Beginning and End Line Cap Types individually or together. A text string or 1-2 symbolic LineCapType values can be used. Also see BegCap() and EndCap().
    /// <para></para>
    /// Symbolic LineCapUsage:  Using one value sets both line caps to the same type.  Using two values sets each line cap individually.  Use LineCapType::Default for default value.
    /// <para></para>
    /// --> Example:  LineCaps(LineCapType::RoundAnchor)  -- Sets both to Round Anchor type
    /// <para></para>
    /// --> Example:  LineCaps(LineCapType::RoundAnchor,LineCapType::Diamond)  -- Sets Beginning line cap to Round Anchor.  End Cap to Diamond line cap.
    /// <para></para>
    /// String Usage:  Using one text value (e.g. "roundanchor") sets both line caps to the same type.  Use two values separate by a comma, or a comma preceded by or followed by a
    /// blank to use the default value for that position.
    /// <para></para>
    /// --> Examples: LineCaps("round anchor"), LineCaps("round Anchor,Diamond"), LineCaps(",Diamond Anchor"), LineCaps("Arrow Anchor,")
    /// <para></para>
    /// The use of commas in the (",Diamond Anchor") sets the beginning anchor to default.  "Arrow Anchor," Sets the end cap to the default.
    /// </summary>
    /// <param name="eLineCapType"> - Beginning Cap type (or both cap types of no second parameter).  This may also be a string</param>
    /// <param name="eEndCapType"> - [optional] End Cap Type (leave blank for default).</param>
    /// <returns></returns>
    vkwType::ckw LineCaps(LineCapType eLineCapType,LineCapType eEndCapType = LineCapType::Undefined);
    

    /// <summary>
    /// Sets Beginning and End Line Cap Types individually or together. A text string or 1-2 symbolic LineCapType values can be used. Also see BegCap() and EndCap().
    /// <para></para>
    /// Symbolic LineCapUsage:  Using one value sets both line caps to the same type.  Using two values sets each line cap individually.  Use LineCapType::Default for default value.
    /// <para></para>
    /// --> Example:  LineCaps(LineCapType::RoundAnchor)  -- Sets both to Round Anchor type
    /// <para></para>
    /// --> Example:  LineCaps(LineCapType::RoundAnchor,LineCapType::Diamond)  -- Sets Beginning line cap to Round Anchor.  End Cap to Diamond line cap.
    /// <para></para>
    /// String Usage:  Using one text value (e.g. "roundanchor") sets both line caps to the same type.  Use two values separate by a comma, or a comma preceded by or followed by a
    /// blank to use the default value for that position.
    /// <para></para>
    /// --> Examples: LineCaps("round anchor"), LineCaps("round Anchor,Diamond"), LineCaps(",Diamond Anchor"), LineCaps("Arrow Anchor,")
    /// <para></para>
    /// The use of commas in the (",Diamond Anchor") sets the beginning anchor to default.  "Arrow Anchor," Sets the end cap to the default.
    /// </summary>
    /// <param name="eLineCapType"> - Beginning Cap type (or both cap types of no second parameter).  This may also be a string</param>
    /// <param name="eEndCapType"> - [optional] End Cap Type (leave blank for default).</param>
    /// <returns></returns>
    vkwType::ckw LineCaps(const char * sLineCapTypes);



}

namespace _vkw  // rename to whatever fits your program.  Or bring it in as a class/struct into a class.
{

    // Sets Cap Type for start of vector line.  
    // Use Sage::LineCapType::  (i.e. StartCap(LineCapType::Arrow))
    // or use strings, such as BegCap("arrow"); 
    //
    // --> Valid types are Flat, Arrow, Round, Square, Diamond, ArrowAnchor, RoundAnchor, SquareAnchor, and DiamondAnchor
    //
    // Default for start cap is Round. Default for EndCap is ArrowAnchor. 
    //
    // 'Anchor' types are larger.  Non-anchor types are the same size of the line.  To set Cap sizes, use Anchor Types.
    // See SetCapSize(), BegCapSize() and EndCapSize()
    vkwType::ckw _BegCap(const char * sText);

    // Sets Cap Type for start of vector line.  
    // Use Sage::LineCapType::  (i.e. StartCap(LineCapType::Arrow))
    // or use strings, such as BegCap("arrow"); 
    //
    // --> Valid types are Flat, Arrow, Round, Square, Diamond, ArrowAnchor, RoundAnchor, SquareAnchor, and DiamondAnchor
    //
    // Default for start cap is Round. Default for EndCap is ArrowAnchor. 
    //
    // 'Anchor' types are larger.  Non-anchor types are the same size of the line.  To set Cap sizes, use Anchor Types.
    // See SetCapSize(), BegCapSize() and EndCapSize()
    vkwType::ckw _BegCap(LineCapType eCapType);

    // Sets Cap Type for end of vector line.  
    // Use Sage::LineCapType::  (i.e. StartCap(LineCapType::Arrow))
    // or use strings, such as StartCap("arrow"); 
    //
    // --> Valid types are Flat, Arrow, Round, Square, Diamond, ArrowAnchor, RoundAnchor, SquareAnchor, and DiamondAnchor
    //
    // Default for start cap is Round. Default for EndCap is ArrowAnchor. 
    //
    // 'Anchor' types are larger.  Non-anchor types are the same size of the line.  To set Cap sizes, use Anchor Types.
    // See SetCapSize(), BegCapSize() and EndCapSize()
    vkwType::ckw _EndCap(const char * sText);

    // Sets Cap Type for end of vector line.  
    // Use Sage::LineCapType::  (i.e. StartCap(LineCapType::Arrow))
    // or use strings, such as StartCap("arrow"); 
    //
    // --> Valid types are Flat, Arrow, Round, Square, Diamond, ArrowAnchor, RoundAnchor, SquareAnchor, and DiamondAnchor
    //
    // Default for start cap is Round. Default for EndCap is ArrowAnchor. 
    //
    // 'Anchor' types are larger.  Non-anchor types are the same size of the line.  To set Cap sizes, use Anchor Types.
    // See SetCapSize(), BegCapSize() and EndCapSize()
    vkwType::ckw _EndCap(LineCapType eCapType)      ;

    /// <summary>
    /// Sets the color of the Start Cap of the line.  When not specified, the color is the same color as the vector line itself.
    /// <para></para>
    /// --> Rgb Colors may be used, e.g. BegCapColor(PanColor::Red), BegCapColor(MyColor), BegCapColor(RgbColor{0,0,128}), etc.
    /// <para></para>
    /// --> Strings may be used as well, e.g. BegCapColor("red")
    /// </summary>
    /// <returns></returns>
    vkwType::ckw _BegCapColor(const Sage::RgbColor rgbColor);

    /// <summary>
    /// Sets the color of the Start Cap of the line.  When not specified, the color is the same color as the vector line itself.
    /// <para></para>
    /// --> Rgb Colors may be used, e.g. BegCapColor(PanColor::Red), BegCapColor(MyColor), BegCapColor(RgbColor{0,0,128}), etc.
    /// <para></para>
    /// --> Strings may be used as well, e.g. BegCapColor("red")
    /// </summary>
    /// <returns></returns>
    vkwType::ckw _BegCapColor(const char * sCapColor);
 
    /// <summary>
    /// Sets the color of the End Cap of the line.  When not specified, the color is the same color as the vector line itself.
    /// <para></para>
    /// --> Rgb Colors may be used, e.g. EndCapColor(PanColor::Red), EndCapColor(MyColor), EndCapColor(RgbColor{0,0,128}), etc.
    /// <para></para>
    /// --> Strings may be used as well, e.g. EndCapColor("red")
    /// </summary>
    /// <returns></returns>
    vkwType::ckw _EndCapColor(const Sage::RgbColor rgbColor);

    /// <summary>
    /// Sets the color of the End Cap of the line.  When not specified, the color is the same color as the vector line itself.
    /// <para></para>
    /// --> Rgb Colors may be used, e.g. EndCapColor(PanColor::Red), EndCapColor(MyColor), EndCapColor(RgbColor{0,0,128}), etc.
    /// <para></para>
    /// --> Strings may be used as well, e.g. EndCapColor("red")
    /// </summary>
    /// <returns></returns>
    vkwType::ckw _EndCapColor(const char * sCapColor)      ;

    /// <summary>
    /// Sets the color of the vector line and end caps (if any).
    /// <para></para>
    /// --> Start and End Caps may be set with BegCap() and EndCap() keywords.  Defaults are round and ArrowAnchor, respectively.
    /// <para></para>
    /// --> Rgb Colors may be used, e.g. Color(PanColor::Red), Color(MyColor), Color(RgbColor{0,0,128}), etc.
    /// <para></para>
    /// --> Strings may be used as well, e.g. Color("red")
    /// <para></para>
    /// ** Note: Color and LineColor() are the same function.
    /// </summary>
    /// <param name="sColor"></param>
    /// <returns></returns>
    vkwType::ckw _Color(const char * sColor);

    /// <summary>
    /// Sets the color of the vector line and end caps (if any).
    /// <para></para>
    /// --> Start and End Caps may be set with BegCap() and EndCap() keywords.  Defaults are round and ArrowAnchor, respectively.
    /// <para></para>
    /// --> Rgb Colors may be used, e.g. Color(PanColor::Red), Color(MyColor), Color(RgbColor{0,0,128}), etc.
    /// <para></para>
    /// --> Strings may be used as well, e.g. Color("red")
    /// <para></para>
    /// ** Note: Color and LineColor() are the same function.
    /// </summary>
    /// <param name="sColor"></param>
    /// <returns></returns>
    vkwType::ckw _Color(const Sage::RgbColor rgbColor);

    /// <summary>
    /// Sets the color of the vector line and end caps (if any).
    /// <para></para>
    /// --> Start and End Caps may be set with BegCap() and EndCap() keywords.  Defaults are round and ArrowAnchor, respectively.
    /// <para></para>
    /// --> Rgb Colors may be used, e.g. LineColor(PanColor::Red), LineColor(MyColor), LineColor(RgbColor{0,0,128}), etc.
    /// <para></para>
    /// --> Strings may be used as well, e.g. LineColor("red")
    /// <para></para>
    /// ** Note: Color and LineColor() are the same function.
    /// </summary>
    /// <param name="sColor"></param>
    /// <returns></returns>
    vkwType::ckw _LineColor(const char * sColor);

    /// <summary>
    /// Sets the color of the vector line and end caps (if any).
    /// <para></para>
    /// --> Start and End Caps may be set with BegCap() and EndCap() keywords.  Defaults are round and ArrowAnchor, respectively.
    /// <para></para>
    /// --> Rgb Colors may be used, e.g. LineColor(PanColor::Red), LineColor(MyColor), LineColor(RgbColor{0,0,128}), etc.
    /// <para></para>
    /// --> Strings may be used as well, e.g. LineColor("red")
    /// <para></para>
    /// ** Note: Color and LineColor() are the same function.
    /// </summary>
    /// <param name="sColor"></param>
    /// <returns></returns>
    vkwType::ckw _LineColor(const Sage::RgbColor rgbColor);

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
    vkwType::ckw _BegCapSize(int iCapSize);

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
    vkwType::ckw _BegCapSize(float fCapSize);

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
    vkwType::ckw _BegCapSize(double fCapSize);

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
    vkwType::ckw _EndCapSize(int iCapSize);

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
    vkwType::ckw _EndCapSize(float fCapSize);

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
    vkwType::ckw _EndCapSize(double fCapSize);

    /// <summary>
    /// Sets the size of the stard and end cap of the line. This function sets a multplier based on the line size.
    /// Default value is 1.0, with a minimum of .5 for the multiplier.
    /// <para></para>
    /// As the line width grows, the anchor size will also grow, applying this multiplier, or using the default value of 1.0.
    /// <para></para>
    /// --> This only works with Anchor types, e.g. ArrowAnchor, CircleAnchor, etc.
    /// <para></para>
    /// Non-anchor types are static, adjusting to the line size.
    /// <para></para>
    /// ** note: See BegCapSize() and EndCapSize() to set cap sizes for Start and End Caps individually.
    /// </summary>
    vkwType::ckw _CapSize(int iCapSize);

    /// <summary>
    /// Sets the size of the stard and end cap of the line. This function sets a multplier based on the line size.
    /// Default value is 1.0, with a minimum of .5 for the multiplier.
    /// <para></para>
    /// As the line width grows, the anchor size will also grow, applying this multiplier, or using the default value of 1.0.
    /// <para></para>
    /// --> This only works with Anchor types, e.g. ArrowAnchor, CircleAnchor, etc.
    /// <para></para>
    /// Non-anchor types are static, adjusting to the line size.
    /// <para></para>
    /// ** note: See BegCapSize() and EndCapSize() to set cap sizes for Start and End Caps individually.
    /// </summary>
    vkwType::ckw _CapSize(float fCapSize);

    /// <summary>
    /// Sets the size of the stard and end cap of the line. This function sets a multplier based on the line size.
    /// Default value is 1.0, with a minimum of .5 for the multiplier.
    /// <para></para>
    /// As the line width grows, the anchor size will also grow, applying this multiplier, or using the default value of 1.0.
    /// <para></para>
    /// --> This only works with Anchor types, e.g. ArrowAnchor, CircleAnchor, etc.
    /// <para></para>
    /// Non-anchor types are static, adjusting to the line size.
    /// <para></para>
    /// ** note: See BegCapSize() and EndCapSize() to set cap sizes for Start and End Caps individually.
    /// </summary>
    vkwType::ckw _CapSize(double fCapSize)   ;

    /// <summary>
    /// Sets the opacity of the vector line, 0-255.  This includes start and end caps if used.
    /// <para></para>
    /// --> The label is also set to this opacity unless SetLabelOpacity() is used to individually set the label opacity.
    /// </summary>
    /// <param name="iOpacity"> - Opacity value of the line (and label). 0 = completely transparent.  255 = compeltely opaque.</param>
    /// <returns></returns>
    vkwType::ckw _Opacity(int iOpacity);

    /// <summary>
    /// Sets the label/title of the vector line.
    /// <para></para>
    /// --> The Vector label is displayed in a default font.  
    /// <para></para>
    /// --> By default, the label is centered above the vector line.
    /// <para></para>
    /// --> Use LabelFont() to set a specific font, or SetTextSize() to set small, large,etc. (default is small).
    /// <para></para>
    /// ** note: Label() and Title() are the same keyword functions.
    /// </summary>
    /// <param name="sText"> - Label Text</param>
    /// <returns></returns>
    vkwType::ckw _Title(const char * sText);

    /// <summary>
    /// Sets the label/title of the vector line.
    /// <para></para>
    /// --> The Vector label is displayed in a default font.  
    /// <para></para>
    /// --> By default, the label is centered above the vector line.
    /// <para></para>
    /// --> Use LabelFont() to set a specific font, or SetTextSize() to set small, large,etc. (default is small).
    /// <para></para>
    /// ** note: Label() and Title() are the same keyword functions.
    /// </summary>
    /// <param name="sText"> - Label Text</param>
    /// <returns></returns>
    vkwType::ckw _Label(const char * sText);

    /// <summary>
    /// Sets the angle (in degrees) for the vector.
    /// <para></para>
    /// This calculates the center of the vector and rotates it by the specified angle.
    /// <para></para>
    /// --> Use AngleRad() to use Radians. 
    /// <para></para>
    /// --> This is useful when using SetCenter() to orient the vector around a specific point.
    /// <para></para>
    /// ** note: AngleDeg() and Angle() are the same function. Angle() defaults to degrees.
    /// </summary>
    /// <param name="Angle"> - Angle (in degrees) to rotate the vector</param>
    /// <returns></returns>
    vkwType::ckw _AngleDeg(int iAngle);

    /// <summary>
    /// Sets the angle (in degrees) for the vector.
    /// <para></para>
    /// This calculates the center of the vector and rotates it by the specified angle.
    /// <para></para>
    /// --> Use AngleRad() to use Radians. 
    /// <para></para>
    /// --> This is useful when using SetCenter() to orient the vector around a specific point.
    /// <para></para>
    /// ** note: AngleDeg() and Angle() are the same function. Angle() defaults to degrees.
    /// </summary>
    /// <param name="Angle"> - Angle (in degrees) to rotate the vector</param>
    /// <returns></returns>
    vkwType::ckw _AngleDeg(float fAngle) ;

    /// <summary>
    /// Sets the angle (in degrees) for the vector.
    /// <para></para>
    /// This calculates the center of the vector and rotates it by the specified angle.
    /// <para></para>
    /// --> Use AngleRad() to use Radians. 
    /// <para></para>
    /// --> This is useful when using SetCenter() to orient the vector around a specific point.
    /// <para></para>
    /// ** note: AngleDeg() and Angle() are the same function. Angle() defaults to degrees.
    /// </summary>
    /// <param name="Angle"> - Angle (in degrees) to rotate the vector</param>
    /// <returns></returns>
    vkwType::ckw _AngleDeg(double fAngle);


    /// <summary>
    /// Sets the angle (in degrees) for the vector.
    /// <para></para>
    /// This calculates the center of the vector and rotates it by the specified angle.
    /// <para></para>
    /// --> Use AngleRad() to use Radians. 
    /// <para></para>
    /// --> This is useful when using SetCenter() to orient the vector around a specific point.
    /// <para></para>
    /// ** note: AngleDeg() and Angle() are the same function. Angle() defaults to degrees.
    /// </summary>
    /// <param name="Angle"> - Angle (in degrees) to rotate the vector</param>
    /// <returns></returns>
    vkwType::ckw _Angle(int iAngle);

    /// <summary>
    /// Sets the angle (in degrees) for the vector.
    /// <para></para>
    /// This calculates the center of the vector and rotates it by the specified angle.
    /// <para></para>
    /// --> Use AngleRad() to use Radians. 
    /// <para></para>
    /// --> This is useful when using SetCenter() to orient the vector around a specific point.
    /// <para></para>
    /// ** note: AngleDeg() and Angle() are the same function. Angle() defaults to degrees.
    /// </summary>
    /// <param name="Angle"> - Angle (in degrees) to rotate the vector</param>
    /// <returns></returns>
    vkwType::ckw _Angle(float fAngle) ;

    /// <summary>
    /// Sets the angle (in degrees) for the vector.
    /// <para></para>
    /// This calculates the center of the vector and rotates it by the specified angle.
    /// <para></para>
    /// --> Use AngleRad() to use Radians. 
    /// <para></para>
    /// --> This is useful when using SetCenter() to orient the vector around a specific point.
    /// <para></para>
    /// ** note: AngleDeg() and Angle() are the same function. Angle() defaults to degrees.
    /// </summary>
    /// <param name="Angle"> - Angle (in degrees) to rotate the vector</param>
    /// <returns></returns>
    vkwType::ckw _Angle(double fAngle);

    /// <summary>
    /// Sets the angle (in radians) for the vector.
    /// <para></para>
    /// This calculates the center of the vector and rotates it by the specified angle.
    /// <para></para>
    /// --> Use Angle() or AngleDeg() to use Degrees. 
    /// <para></para>
    /// --> This is useful when using SetCenter() to orient the vector around a specific point.
    /// </summary>
    /// <param name="Angle"> - Angle (in radians) to rotate the vector</param>
    /// <returns></returns>
    vkwType::ckw _AngleRad(float fAngle) ;

    /// <summary>
    /// Sets the angle (in radians) for the vector.
    /// <para></para>
    /// This calculates the center of the vector and rotates it by the specified angle.
    /// <para></para>
    /// --> Use Angle() or AngleDeg() to use Degrees. 
    /// <para></para>
    /// --> This is useful when using SetCenter() to orient the vector around a specific point.
    /// </summary>
    /// <param name="Angle"> - Angle (in radians) to rotate the vector</param>
    /// <returns></returns>
    vkwType::ckw _AngleRad(double fAngle);

    /// <summary>
    /// Sets the vector label/title font to a specific font, either text,integer, or HGont as input.
    /// <para></para>
    /// --> Example:  LabelFont(50), LabelFont("Arial,100"), LabelFont("MyFont"), LabelFont(hMyFont)
    /// <para></para>
    /// --> See: SetTextSize() to set small, xsmall, large, etc. (default is 'small') using the default font.
    /// </summary>
    /// <param name="Font"> - Text, Integer, or Hfont font.</param>
    /// <returns></returns>
    vkwType::ckw _LabelFont(HFONT hFont);

    /// <summary>
    /// Sets the vector label/title font to a specific font, either text,integer, or HGont as input.
    /// <para></para>
    /// --> Example:  LabelFont(50), LabelFont("Arial,100"), LabelFont("MyFont"), LabelFont(hMyFont)
    /// <para></para>
    /// --> See: SetTextSize() to set small, xsmall, large, etc. (default is 'small') using the default font.
    /// </summary>
    /// <param name="Font"> - Text, Integer, or Hfont font.</param>
    /// <returns></returns>
    vkwType::ckw _LabelFont(int iFontSize);

    /// <summary>
    /// Sets the vector label/title font to a specific font, either text,integer, or HGont as input.
    /// <para></para>
    /// --> Example:  LabelFont(50), LabelFont("Arial,100"), LabelFont("MyFont"), LabelFont(hMyFont)
    /// <para></para>
    /// --> See: SetTextSize() to set small, xsmall, large, etc. (default is 'small') using the default font.
    /// </summary>
    /// <param name="Font"> - Text, Integer, or Hfont font.</param>
    /// <returns></returns>
    vkwType::ckw _LabelFont(const char * sFont);

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
    vkwType::ckw _LabelJust(vkwType::LabelJustType labelJust);

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
    vkwType::ckw _LabelJust(const char * sLabelJust);

    /// <summary>
    /// Adjusts the label text of the vector left or right by a number of pixels.
    /// <para></para>
    /// --> Negative values move the label to the left, positive values moving the label to the right.
    /// <para></para>
    /// --> Movement is relative to the line, with left the first vector coordinate and right being the second.
    /// <para></para>
    /// --> See: LabelPadY() to move the label up or down (relative to the vector's orientation).
    /// <para></para>
    /// ** note: LabelPad() and LabelPadX() are the same function.
    /// </summary>
    /// <param name="iPadX"></param>
    /// <returns></returns>
    vkwType::ckw _LabelPad(int iPadX);

    /// <summary>
    /// Adjusts the label text of the vector left or right by a number of pixels.
    /// <para></para>
    /// --> Negative values move the label to the left, positive values moving the label to the right.
    /// <para></para>
    /// --> Movement is relative to the line, with left the first vector coordinate and right being the second.
    /// <para></para>
    /// --> See: LabelPadY() to move the label up or down (relative to the vector's orientation).
    /// <para></para>
    /// ** note: LabelPad(), PadX() and LabelPadX() are the same function.
    /// </summary>
    /// <param name="iPadX"></param>
    /// <returns></returns>
    vkwType::ckw _LabelPadX(int iPadX);

    /// <summary>
    /// Adjusts the label text of the vector left or right by a number of pixels.
    /// <para></para>
    /// --> Negative values move the label to the left, positive values moving the label to the right.
    /// <para></para>
    /// --> Movement is relative to the line, with left the first vector coordinate and right being the second.
    /// <para></para>
    /// --> See: LabelPadY() to move the label up or down (relative to the vector's orientation).
    /// <para></para>
    /// ** note: LabelPad(), PadX() and LabelPadX() are the same function.
    /// </summary>
    /// <param name="iPadX"></param>
    /// <returns></returns>
    vkwType::ckw _PadX(int iPadX);

    /// <summary>
    /// Adjusts the label text of the vector up or down by a number of pixels.
    /// <para></para>
    /// --> Negative values move the label up, positive values moving the label down.
    /// <para></para>
    /// --> Movement is relative to the line, with up relative to the center moving away from the line, and down moving towards the line.
    /// <para></para>
    /// --> See: LabelPadX() to move the label left or right (relative to the vector's orientation).
    /// <para></para>
    /// ** note: LabelPadY and PadY() are the same function.
    /// </summary>
    /// <param name="iPadX"></param>
    /// <returns></returns>
    vkwType::ckw _LabelPadY(int iPadY);

    /// <summary>
    /// Adjusts the label text of the vector up or down by a number of pixels.
    /// <para></para>
    /// --> Negative values move the label up, positive values moving the label down.
    /// <para></para>
    /// --> Movement is relative to the line, with up relative to the center moving away from the line, and down moving towards the line.
    /// <para></para>
    /// --> See: LabelPadX() to move the label left or right (relative to the vector's orientation).
    /// <para></para>
    /// ** note: LabelPadY and PadY() are the same function.
    /// </summary>
    /// <param name="iPadX"></param>
    /// <returns></returns>
    vkwType::ckw _PadY(int iPadY);

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
    vkwType::ckw _TextSize(vkwType::TextSizeType textSize);

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
    vkwType::ckw _TextSize(const char * sTextSize);

    /// <summary>
    /// Sets the opacity of the label. 
    /// The label will inherit the opacity of the line by default.  If the line's opacity is changed, the label also changes
    /// to the same opacity level unless LabelOpacity is used.
    /// <para></para>
    /// --> LabelOpacity() provides a way to have a different opacity for the label text than the lines opacity.
    /// <para></para>
    /// ** note: LabelOpacity() and TextOpacity() are the same function
    /// </summary>
    /// <param name="iOpacity"> - Opacity level 0-255. 0 is completely tranparent, 255 is completely opaque (default is 255)</param>
    vkwType::ckw _LabelOpacity(int iOpacity);

    /// <summary>
    /// Sets the opacity of the label. 
    /// The label will inherit the opacity of the line by default.  If the line's opacity is changed, the label also changes
    /// to the same opacity level unless LabelOpacity is used.
    /// <para></para>
    /// --> LabelOpacity() provides a way to have a different opacity for the label text than the lines opacity.
    /// <para></para>
    /// ** note: LabelOpacity() and TextOpacity() are the same function
    /// </summary>
    /// <param name="iOpacity"> - Opacity level 0-255. 0 is completely tranparent, 255 is completely opaque (default is 255)</param>
    vkwType::ckw _TextOpacity(int iOpacity);

    /// <summary>
    /// Sets the color of the label text.  
    /// By default, the label color is white. 
    /// <para></para>
    /// Using LabelColor() changes the color of the text of the label.
    /// <para></para>
    /// --> Rgb Colors may be used, e.g. LabelColor(PanColor::Red), LabelColor(MyColor), LabelColor(RgbColor{0,0,128}), etc.
    /// <para></para>
    /// --> Strings may be used as well, e.g. LabelColor("red")
    /// </summary>
    /// <param name="rgbColor"> - Color to set the label text.</param>
    /// <returns></returns>
    vkwType::ckw _LabelColor(RgbColor rgbColor);

    /// <summary>
    /// Sets the color of the label text.  
    /// By default, the label color is white. 
    /// <para></para>
    /// Using LabelColor() changes the color of the text of the label.
    /// <para></para>
    /// --> Rgb Colors may be used, e.g. LabelColor(PanColor::Red), LabelColor(MyColor), LabelColor(RgbColor{0,0,128}), etc.
    /// <para></para>
    /// --> Strings may be used as well, e.g. LabelColor("red")
    /// </summary>
    /// <param name="rgbColor"> - Color to set the label text.</param>
    /// <returns></returns>
    vkwType::ckw _LabelColor(const char * sColor);

    /// <summary>
    /// Sets a label orientation for the the label text:
    /// <para></para>
    /// --> Horizontal.  Keeps the label horizontal (i.e. 0 degress relative to the line).  This is the default
    /// <para></para>
    /// --> Vertical.  Keeps the label -90 degrees, i.e. sideways on the line
    /// <para></para>
    /// --> Vertical180.  Keeps the label 90 degrees, i.e. sideways on the line,but rotated 180 degrees.
    /// <para></para>
    /// The orientation will track with the line, unless bStaticAngle = true.
    /// <para></para>
    /// When bStaticAngle is true, the angle will become static, with the text keeping to the same angle to the screen regardless of line orientation.
    /// <para></para>
    /// This allows the ability to always have a horizontal or vertical line with respect to the screen.
    /// <para></para>
    /// --> Types can be LabelAngleType, e.g. LabelAngleType(vkwType::LabelAngleType::Vertical180)
    /// <para></para>
    /// --> Types can be a string, e.g. LabelAngleType("Vertical180,static");
    /// </summary>
    /// <param name="angleType"> - Label angle orientation type, i.e. horizontal, vertical, vertical180</param>
    /// <param name="bStaticAngle"> - When false (default), angle is relative to the line angle.  When true, Angle is relative to the screen.</param>
    /// <returns></returns>
    vkwType::ckw _LabelAngle(vkwType::LabelAngleType angleType,bool bStaticAngle = false);

    /// <summary>
    /// Sets a label orientation for the the label text:
    /// <para></para>
    /// --> Horizontal.  Keeps the label horizontal (i.e. 0 degress relative to the line).  This is the default
    /// <para></para>
    /// --> Vertical.  Keeps the label -90 degrees, i.e. sideways on the line
    /// <para></para>
    /// --> Vertical180.  Keeps the label 90 degrees, i.e. sideways on the line,but rotated 180 degrees.
    /// <para></para>
    /// The orientation will track with the line, unless bStaticAngle = true.
    /// <para></para>
    /// When bStaticAngle is true, the angle will become static, with the text keeping to the same angle to the screen regardless of line orientation.
    /// <para></para>
    /// This allows the ability to always have a horizontal or vertical line with respect to the screen.
    /// <para></para>
    /// --> Types can be LabelAngleType, e.g. LabelAngleType(vkwType::LabelAngleType::Vertical180)
    /// <para></para>
    /// --> Types can be a string, e.g. LabelAngleType("Vertical180,static");
    /// </summary>
    /// <param name="angleType"> - Label angle orientation type, i.e. horizontal, vertical, vertical180</param>
    /// <param name="bStaticAngle"> - When false (default), angle is relative to the line angle.  When true, Angle is relative to the screen.</param>
    /// <returns></returns>
    vkwType::ckw _LabelAngle(const char * sAngleType);
    
    // Keeps the label right-side up when it would otherwise be upside-down on a line that has been rotated more than 90 degrees in either direction.
    //
    vkwType::ckw _LabelUp(bool bLabelUp = true);

    /// <summary>
    /// Sets the center of the vector.  This allows vectors of a length and orientation to be specified, without a specific point of origin.
    /// <para></para>
    /// SetCenter() sets the center of the vector.  The original angle between the two input points and any other angle changes is kept and
    /// the new start and end points are calculated based on the center input.
    /// <para></para>
    /// This allows non-descript and generic vectors to be defined and re-defined, such as DrawVector({0,0},{200,0},kw::SetCenter(some_screen_value) + kw::Angle(45)) 
    /// </summary>
    /// <param name="pCenter"> - Center of vector in screen coordinates.</param>
    vkwType::ckw _SetCenter(POINT pCenter);

    /// <summary>
    /// Sets the center of the vector.  This allows vectors of a length and orientation to be specified, without a specific point of origin.
    /// <para></para>
    /// SetCenter() sets the center of the vector.  The original angle between the two input points and any other angle changes is kept and
    /// the new start and end points are calculated based on the center input.
    /// <para></para>
    /// This allows non-descript and generic vectors to be defined and re-defined, such as DrawVector({0,0},{200,0},kw::SetCenter(some_screen_value) + kw::Angle(45)) 
    /// </summary>
    /// <param name="pCenter"> - Center of vector in screen coordinates.</param>
    vkwType::ckw _SetCenter(CfPoint pCenter);

    /// <summary>
    /// Sets the center of the vector.  This allows vectors of a length and orientation to be specified, without a specific point of origin.
    /// <para></para>
    /// SetCenter() sets the center of the vector.  The original angle between the two input points and any other angle changes is kept and
    /// the new start and end points are calculated based on the center input.
    /// <para></para>
    /// This allows non-descript and generic vectors to be defined and re-defined, such as DrawVector({0,0},{200,0},kw::SetCenter(some_screen_value) + kw::Angle(45)) 
    /// </summary>
    /// <param name="pCenter"> - Center of vector in screen coordinates.</param>
    vkwType::ckw _SetCenter(int iCenterX,int iCenterY);

    /// <summary>
    /// Sets the center of the vector.  This allows vectors of a length and orientation to be specified, without a specific point of origin.
    /// <para></para>
    /// SetCenter() sets the center of the vector.  The original angle between the two input points and any other angle changes is kept and
    /// the new start and end points are calculated based on the center input.
    /// <para></para>
    /// This allows non-descript and generic vectors to be defined and re-defined, such as DrawVector({0,0},{200,0},kw::SetCenter(some_screen_value) + kw::Angle(45)) 
    /// </summary>
    /// <param name="pCenter"> - Center of vector in screen coordinates.</param>
    vkwType::ckw _SetCenter(double fCenterX,double fCenterY);

    /// <summary>
    /// Sets the center of the vector.  This allows vectors of a length and orientation to be specified, without a specific point of origin.
    /// <para></para>
    /// SetCenter() sets the center of the vector.  The original angle between the two input points and any other angle changes is kept and
    /// the new start and end points are calculated based on the center input.
    /// <para></para>
    /// This allows non-descript and generic vectors to be defined and re-defined, such as DrawVector({0,0},{200,0},kw::SetCenter(some_screen_value) + kw::Angle(45)) 
    /// </summary>
    /// <param name="pCenter"> - Center of vector in screen coordinates.</param>
    vkwType::ckw _SetCenter(float fCenterX,float fCenterY);

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
    vkwType::ckw _ShowCenter(const char * sColor);

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
    vkwType::ckw _ShowCenter(RgbColor rgbColor = SageColor::Cyan);

    /// <summary>
    /// Sets Beginning and End Line Cap Types individually or together. A text string or 1-2 symbolic LineCapType values can be used. Also see BegCap() and EndCap().
    /// <para></para>
    /// Symbolic LineCapUsage:  Using one value sets both line caps to the same type.  Using two values sets each line cap individually.  Use LineCapType::Default for default value.
    /// <para></para>
    /// --> Example:  LineCaps(LineCapType::RoundAnchor)  -- Sets both to Round Anchor type
    /// <para></para>
    /// --> Example:  LineCaps(LineCapType::RoundAnchor,LineCapType::Diamond)  -- Sets Beginning line cap to Round Anchor.  End Cap to Diamond line cap.
    /// <para></para>
    /// String Usage:  Using one text value (e.g. "roundanchor") sets both line caps to the same type.  Use two values separate by a comma, or a comma preceded by or followed by a
    /// blank to use the default value for that position.
    /// <para></para>
    /// --> Examples: LineCaps("round anchor"), LineCaps("round Anchor,Diamond"), LineCaps(",Diamond Anchor"), LineCaps("Arrow Anchor,")
    /// <para></para>
    /// The use of commas in the (",Diamond Anchor") sets the beginning anchor to default.  "Arrow Anchor," Sets the end cap to the default.
    /// </summary>
    /// <param name="eLineCapType"> - Beginning Cap type (or both cap types of no second parameter).  This may also be a string</param>
    /// <param name="eEndCapType"> - [optional] End Cap Type (leave blank for default).</param>
    /// <returns></returns>
    vkwType::ckw LineCaps(LineCapType eLineCapType,LineCapType eEndCapType = LineCapType::Undefined);
    
    /// <summary>
    /// Sets Beginning and End Line Cap Types individually or together. A text string or 1-2 symbolic LineCapType values can be used. Also see BegCap() and EndCap().
    /// <para></para>
    /// Symbolic LineCapUsage:  Using one value sets both line caps to the same type.  Using two values sets each line cap individually.  Use LineCapType::Default for default value.
    /// <para></para>
    /// --> Example:  LineCaps(LineCapType::RoundAnchor)  -- Sets both to Round Anchor type
    /// <para></para>
    /// --> Example:  LineCaps(LineCapType::RoundAnchor,LineCapType::Diamond)  -- Sets Beginning line cap to Round Anchor.  End Cap to Diamond line cap.
    /// <para></para>
    /// String Usage:  Using one text value (e.g. "roundanchor") sets both line caps to the same type.  Use two values separate by a comma, or a comma preceded by or followed by a
    /// blank to use the default value for that position.
    /// <para></para>
    /// --> Examples: LineCaps("round anchor"), LineCaps("round Anchor,Diamond"), LineCaps(",Diamond Anchor"), LineCaps("Arrow Anchor,")
    /// <para></para>
    /// The use of commas in the (",Diamond Anchor") sets the beginning anchor to default.  "Arrow Anchor," Sets the end cap to the default.
    /// </summary>
    /// <param name="eLineCapType"> - Beginning Cap type (or both cap types of no second parameter).  This may also be a string</param>
    /// <param name="eEndCapType"> - [optional] End Cap Type (leave blank for default).</param>
    /// <returns></returns>
    vkwType::ckw LineCaps(const char * sLineCapTypes);

}


} // namespace Sage