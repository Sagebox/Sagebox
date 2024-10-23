#pragma once


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

#include "opt2_ckwargs.h"   

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
    using kwOpt = kwType::ckw;
    
namespace kw  // rename to whatever fits your program.  Or bring it in as a class/struct into a class.
{
 
    extern const kwType::ckw none;
    // Sets Cap Type for start of vector line.  
    // Use Sage::LineCapType::  (i.e. StartCap(LineCapType::Arrow))
    // or use strings, such as StartCap("arrow"); 
    //
    // --> Valid types are Flat, Arrow, Round, Square, Diamond, ArrowAnchor, RoundAnchor, SquareAnchor, and DiamondAnchor
    //
    // Default for start cap is Flat. 
    //

    // 
	// Sets the Pen Size for a shape such as a circle, square, triangle, etc.  This is the thickness of the border around the shape.
    // When not used, the current default setting is used (it defaults to 1, but can be changed).
    // 
    // For example,
	// 
    // CWin.draw.FillCircle(400,400,200,SageColor::Red,kw:PenColor("Green") | kw::PenSize(20))
    // 
    // Draw a red filled circle with an outside border of Green with a thickness of 20 pixels.
    //

    /// <summary>
    /// Sets the Pen Size for a shape such as a circle, square, triangle, etc.  This is the thickness of the border around the shape.
    /// When not used, the current default setting is used (it defaults to 1, but can be changed).
    /// </summary>
    /// <param name="PenSize"> - Integer, Float, or Double PenSize value</param>
    /// <returns></returns>
    kwType::ckw PenSize(int iPenSize)      ;

    /// <summary>
    /// Sets the Pen Size for a shape such as a circle, square, triangle, etc.  This is the thickness of the border around the shape.
    /// When not used, the current default setting is used (it defaults to 1, but can be changed).
    /// </summary>
    /// <param name="PenSize"> - Integer, Float, or Double PenSize value</param>
    /// <returns></returns>
    kwType::ckw PenSize(float fPenSize)      ;

    /// <summary>
    /// Sets the Pen Size for a shape such as a circle, square, triangle, etc.  This is the thickness of the border around the shape.
    /// When not used, the current default setting is used (it defaults to 1, but can be changed).
    /// </summary>
    /// <param name="PenSize"> - Integer, Float, or Double PenSize value</param>
    /// <returns></returns>
    kwType::ckw PenSize(double fPenSize)      ;
  
    /// <summary>
    /// Sets the Color of the Pen Used to draw an object. 
    /// <para></para>--> Color can be a defined color such as SageColor::Yellow or PanColor::ForestGreen, or a string, such as "Yellow" or "ForestGreen"
    /// <para></para>--> Note that strings take much longer to process.  Using defined colors (i.e. SageColor::Yellow) is much faster in loops with large amounts of drawing.
    /// <para></para>--> An integer pen size may optionally be specified.  For setting PenSize directly or floating-point pen sizes, use kw::PenSize() instead.
    /// <para></para>--> When PenSize() is used, the iPenSize value in this function is ignored.
    /// </summary>
    /// <param name="rgbColor">Defined color (i.e. SageColor::yellow) when RgbColor form is used</param>
    /// <param name="Scolor">String-based color, such as "Yellow", when sColor form is used</param>
    /// <param name="iPenSize">Set to a 0 or positive value.  Default is -1 to use the default pen size.</param>
    /// <returns></returns>
    kwType::ckw PenColor(CRgbColor rgbColor,int iPenSize = -1)        ;
 #if 0   
    /// <summary>
    /// Sets the Color of the Pen Used to draw an object. 
    /// <para></para>--> Color can be a defined color such as SageColor::Yellow or PanColor::ForestGreen, or a string, such as "Yellow" or "ForestGreen"
    /// <para></para>--> Note that strings take much longer to process.  Using defined colors (i.e. SageColor::Yellow) is much faster in loops with large amounts of drawing.
    /// <para></para>--> An integer pen size may optionally be specified.  For setting PenSize directly or floating-point pen sizes, use kw::PenSize() instead.
    /// <para></para>--> When PenSize() is used, the iPenSize value in this function is ignored.
    /// </summary>
    /// <param name="rgbColor">Defined color (i.e. SageColor::yellow) when RgbColor form is used</param>
    /// <param name="Scolor">String-based color, such as "Yellow", when sColor form is used</param>
    /// <param name="iPenSize">Set to a 0 or positive value.  Default is -1 to use the default pen size.</param>
    /// <returns></returns>
    kwType::ckw PenColor(const char * sColor,int iPenSize = -1)        ;
#endif

    /// <summary>
    /// Sets the angle of an object such as a circle, square, ellipse, bitmap, etc. in degrees.
    /// <para></para>
    /// --> Angle is in Degrees (i.e. 0-360).  Use Angle() to set angle in radians
    /// <para></para>
    /// ** note: Angle() and AngleDeg() are the same function
    /// </summary>
    /// <param name="Angle"> - Integer, float or double angle value in radians (see Angle() to use radians instead of degrees)</param>
    /// <returns></returns>
    kwType::ckw Angle(int iAngle)      ;

    /// <summary>
    /// Sets the angle of an object such as a circle, square, ellipse, bitmap, etc. in degrees.
    /// <para></para>
    /// --> Angle is in Degrees (i.e. 0-360).  Use Angle() to set angle in radians
    /// <para></para>
    /// ** note: Angle() and AngleDeg() are the same function
    /// </summary>
    /// <param name="Angle"> - Integer, float or double angle value in radians (see Angle() to use radians instead of degrees)</param>
    /// <returns></returns>
    kwType::ckw Angle(float fAngle)      ;

    /// <summary>
    /// Sets the angle of an object such as a circle, square, ellipse, bitmap, etc. in degrees.
    /// <para></para>
    /// --> Angle is in Degrees (i.e. 0-360).  Use Angle() to set angle in radians
     /// <para></para>
    /// ** note: Angle() and AngleDeg() are the same function
   /// </summary>
    /// <param name="Angle"> - Integer, float or double angle value in radians (see Angle() to use radians instead of degrees)</param>
    /// <returns></returns>
    kwType::ckw Angle(double fAngle)      ;

    /// <summary>
    /// Sets the angle of an object such as a circle, square, ellipse, bitmap, etc. in radians.
    /// <para></para>
    /// --> Angle is in Radians.  Use AngleDeg() to set angle in degrees (i.e. 0-360)
    /// </summary>
    /// <param name="fAngle"> - float or double angle value in radians (see AngleDeg() to use degrees instead of radians)</param>
    /// <returns></returns>
    kwType::ckw AngleRad(float fAngle)      ;

    /// <summary>
    /// Sets the angle of an object such as a circle, square, ellipse, bitmap, etc. in radians.
    /// <para></para>
    /// --> Angle is in Radians.  Use AngleDeg() to set angle in degrees (i.e. 0-360)
    /// </summary>
    /// <param name="fAngle"> - float or double angle value in radians (see AngleDeg() to use degrees instead of radians)</param>
    /// <returns></returns>
    kwType::ckw AngleRad(double fAngle)      ;

    /// <summary>
    /// Sets the angle of an object such as a circle, square, ellipse, bitmap, etc. in degrees.
    /// <para></para>
    /// --> Angle is in Degrees (i.e. 0-360).  Use Angle() to set angle in radians
    /// <para></para>
    /// ** note: Angle() and AngleDeg() are the same function
    /// </summary>
    /// <param name="Angle"> - Integer, float or double angle value in radians (see Angle() to use radians instead of degrees)</param>
    /// <returns></returns>
    kwType::ckw AngleDeg(int iAngle)      ;

    /// <summary>
    /// Sets the angle of an object such as a circle, square, ellipse, bitmap, etc. in degrees.
    /// <para></para>
    /// --> Angle is in Degrees (i.e. 0-360).  Use Angle() to set angle in radians
    /// <para></para>
    /// ** note: Angle() and AngleDeg() are the same function
    /// </summary>
    /// <param name="Angle"> - Integer, float or double angle value in radians (see Angle() to use radians instead of degrees)</param>
    /// <returns></returns>
    kwType::ckw AngleDeg(float fAngle)      ;

    /// <summary>
    /// Sets the angle of an object such as a circle, square, ellipse, bitmap, etc. in degrees.
    /// <para></para>
    /// --> Angle is in Degrees (i.e. 0-360).  Use Angle() to set angle in radians
     /// <para></para>
    /// ** note: Angle() and AngleDeg() are the same function
   /// </summary>
    /// <param name="Angle"> - Integer, float or double angle value in radians (see Angle() to use radians instead of degrees)</param>
    /// <returns></returns>
    kwType::ckw AngleDeg(double fAngle)      ;



    /// <summary>
    /// Sets the opacity of an object or control, from 0 (invisible) - 100 (completely opaque).
    /// </summary>
    /// <param name="iOpacity"> - Opacity level.  0 = completely transparent, 100 = completely opaque. </param>
    /// <returns></returns>
    kwType::ckw Opacity(int iOpacity)           ;
    
    /// <summary>
    /// Sets the center of an object.  This is used for drawn objects at various angles, to set the center of the object for rotation.
    /// <para></para>
    /// --> By default, the center is mathematical center.  SetCenter() changes the center of rotation.
    /// --> See prototypes for various types of inputs.  Floating-point values can be used for higher precision.
    /// </summary>
    kwType::ckw SetCenter(int iX,int iY)        ;

    /// <summary>
    /// Sets the center of an object.  This is used for drawn objects at various angles, to set the center of the object for rotation.
    /// <para></para>
    /// --> By default, the center is mathematical center.  SetCenter() changes the center of rotation.
    /// --> See prototypes for various types of inputs.  Floating-point values can be used for higher precision.
    /// </summary>
    kwType::ckw SetCenter(POINT pCenter)        ;

    /// <summary>
    /// Sets the center of an object.  This is used for drawn objects at various angles, to set the center of the object for rotation.
    /// <para></para>
    /// --> By default, the center is mathematical center.  SetCenter() changes the center of rotation.
    /// --> See prototypes for various types of inputs.  Floating-point values can be used for higher precision.
    /// </summary>
    kwType::ckw SetCenter(CPoint pCenter)       ;

    /// <summary>
    /// Sets the center of an object.  This is used for drawn objects at various angles, to set the center of the object for rotation.
    /// <para></para>
    /// --> By default, the center is mathematical center.  SetCenter() changes the center of rotation.
    /// --> See prototypes for various types of inputs.  Floating-point values can be used for higher precision.
    /// </summary>
    kwType::ckw SetCenter(float fx,float fy)    ;

    /// <summary>
    /// Sets the center of an object.  This is used for drawn objects at various angles, to set the center of the object for rotation.
    /// <para></para>
    /// --> By default, the center is mathematical center.  SetCenter() changes the center of rotation.
    /// --> See prototypes for various types of inputs.  Floating-point values can be used for higher precision.
    /// </summary>
    kwType::ckw SetCenter(double fx,double fy)  ;

    /// <summary>
    /// Sets the center of an object.  This is used for drawn objects at various angles, to set the center of the object for rotation.
    /// <para></para>
    /// --> By default, the center is mathematical center.  SetCenter() changes the center of rotation.
    /// --> See prototypes for various types of inputs.  Floating-point values can be used for higher precision.
    /// </summary>
    kwType::ckw SetCenter(CfPoint cfCenter)      ;

    /// <summary>
    /// Sets the center of an object.  This is used for drawn objects at various angles, to set the center of the object for rotation.
    /// <para></para>
    /// --> By default, the center is mathematical center.  SetCenter() changes the center of rotation.
    /// --> See prototypes for various types of inputs.  Floating-point values can be used for higher precision.
    /// </summary>
    kwType::ckw SetCenter(CfPointf cfCenter)     ;

    /// <summary>
    /// Specifies the center of an object -- such as a text string, circle, etc. -- is the calculated center.
    /// <para></para>
    /// This is typically the default.  For example, when specifying an angle for writing out text or drawing a graphics object such as a circle or rectangle, the default is the center of the text.
    /// <para></para>
    /// --> This is the equivalent of SetCenter(CenterType::Center) or SetCenter("center");  
    /// <para></para>
    /// *** note: This keyword function is possibly deprecated.  Using SetCenter() using a CenterType or string is safer, such as SetCenter(CenterType::LeftCenter) or SetCenter("LeftCenter"), etc.
    /// </summary>
    /// <returns></returns>
    kwType::ckw SetCenter();

    /// <summary>
    /// Specifies the center of an object based on the input type. Objects such as output text using Write() or WriteGdiText(), or drawing a circle, square, etc.
    /// <para></para>
    /// In most cases, the center of an object defaults to the calculated center (same as using SetCenter(CenterType::Center)). 
    /// <para></para>
    /// Setting the center is usually used for rotating the object.
    /// <para></para>
    /// The calculated center can be changed to other area, such as left, right, topright, bottomcenter, etc.
    /// <para></para>
    /// Example: SetCenter(CenterType::BottomRight) --> Sets the calculated center as the Bottom Right of the object.
    /// <para></para>
    /// --> Also see SetCenter(x,y) or SetCenter(POINT) where a specific location for the center of the object can be specifically set, which can be inside or outside the object itself.
    /// <para></para>
    /// ** note:  SetCenter sets the calculated center of the object.  See SetJust() and SetTextJust() for setting the location of the object and/or text associated with it.
    /// </summary>
    /// <param name="centerType">CenterType enum value, such as CenterType::Center, CenterType::Left, etc.</param>
    /// <returns></returns>
    kwType::ckw SetCenter(CenterType centerType);

    /// <summary>
    /// Specifies the center of an object based on the input type, using an input string. Objects such as output text using Write() or WriteGdiText(), or drawing a circle, square, etc.
    /// <para></para>
    /// ** note:  With programs with real-time concerns, SetCenter with a string, such as SetCenter("bottomright") is slower than using an enum, such as SetCenter(CenterType::BottomRight)
    /// <para></para>
    /// In most cases, the center of an object defaults to the calculated center (same as using SetCenter("center")). 
    /// <para></para>
    /// Setting the center is usually used for rotating the object.
    /// <para></para>
    /// The calculated center can be changed to other area, such as left, right, topright, bottomcenter, etc.
    /// <para></para>
    /// Example: SetCenter("bottomright") --> Sets the calculated center as the Bottom Right of the object.
    /// <para></para>
    /// --> Also see SetCenter(x,y) or SetCenter(POINT) where a specific location for the center of the object can be specifically set, which can be inside or outside the object itself.
    /// <para></para>
    /// ** note:  SetCenter sets the calculated center of the object.  See SetJust() and SetTextJust() for setting the location of the object and/or text associated with it.
    /// </summary>
    /// <param name="centerType">CenterType enum value, such as CenterType::Center, CenterType::Left, etc.</param>
    /// <returns></returns>
    kwType::ckw SetCenter(const char * sCenterType);

    /// <summary>
    /// Sets the center of an object to the calculated center of the object.
    /// <para></para>
    /// *** note: This function may be deprecated in the future.  See: SetCenter(CenterType) instead, such as SetCenter(CenterJust::Center);
    /// <para></para>
    /// --> See SetCenter() for more information.
    /// </summary>
    /// <returns></returns>
    kwType::ckw SetCenterMiddle(); 

    /// <summary>
    /// Sets the center of an object to the calculated center of the object.
    /// <para></para>
    /// *** note: This function may be deprecated in the future.  See: SetCenter(CenterType) instead, such as SetCenter(CenterJust::Center);
    /// <para></para>
    /// --> See SetCenter() for more information.
    /// </summary>
    /// <returns></returns>
    kwType::ckw SetCenterTopLeft();

    /// <summary>
    /// Sets the center of an object to the calculated center of the object.
    /// <para></para>
    /// *** note: This function may be deprecated in the future.  See: SetCenter(CenterType) instead, such as SetCenter(CenterJust::Center);
    /// <para></para>
    /// --> See SetCenter() for more information.
    /// </summary>
    /// <returns></returns>
    kwType::ckw SetCenterTop();

    /// <summary>
    /// Sets the center of an object to the calculated center of the object.
    /// <para></para>
    /// *** note: This function may be deprecated in the future.  See: SetCenter(CenterType) instead, such as SetCenter(CenterJust::Center);
    /// <para></para>
    /// --> See SetCenter() for more information.
    /// </summary>
    /// <returns></returns>
    kwType::ckw SetCenterTopRight();

    /// <summary>
    /// Sets the center of an object to the calculated right-center of the object.
    /// <para></para>
    /// *** note: This function may be deprecated in the future.  See: SetCenter(CenterType) instead, such as SetCenter(CenterJust::Center);
    /// <para></para>
    /// --> See SetCenter() for more information.
    /// </summary>
    /// <returns></returns>
    kwType::ckw SetCenterRight(); 

    /// <summary>
    /// Sets the center of an object to the calculated bottom-right of the object.
    /// <para></para>
    /// *** note: This function may be deprecated in the future.  See: SetCenter(CenterType) instead, such as SetCenter(CenterJust::Center);
    /// <para></para>
    /// --> See SetCenter() for more information.
    /// </summary>
    /// <returns></returns>
    kwType::ckw SetCenterBottomRight();

    /// <summary>
    /// Sets the center of an object to the calculated bottom-center of the object.
    /// <para></para>
    /// *** note: This function may be deprecated in the future.  See: SetCenter(CenterType) instead, such as SetCenter(CenterJust::Center);
    /// <para></para>
    /// --> See SetCenter() for more information.
    /// </summary>
    /// <returns></returns>
    kwType::ckw SetCenterBottom();

    /// <summary>
    /// Sets the center of an object to the calculated bottom-left of the object.
    /// <para></para>
    /// *** note: This function may be deprecated in the future.  See: SetCenter(CenterType) instead, such as SetCenter(CenterJust::Center);
    /// <para></para>
    /// --> See SetCenter() for more information.
    /// </summary>
    /// <returns></returns>
    kwType::ckw SetCenterBottomLeft();

    /// <summary>
    /// Sets the center of an object to the calculated left-center of the object.
    /// <para></para>
    /// *** note: This function may be deprecated in the future.  See: SetCenter(CenterType) instead, such as SetCenter(CenterJust::Center);
    /// <para></para>
    /// --> See SetCenter() for more information.
    /// </summary>
    /// <returns></returns>
    kwType::ckw SetCenterLeft();

    /// <summary>
    /// Sets the justification of an object relative to the window client area, such as text output, a text widget, or other controls and some shapes.
    /// <para></para>
    /// Typically, an object is placed at a specific X,Y position.  However, setting the justification of an object can place the object in anchored points in the window.
    /// <para></para>
    /// For example, specifying kw:SetJust(JustType::Center) will center a text widget in the window, where kw::SetJust(Just::BottomRight) will place the text widget in the bottom-right area of the window.
    /// <para></para>
    /// This is also useful for TextWidgets and other controls.
    /// <para></para>
    /// ** note:  SetJust() and Just() are the same function.
    /// <para></para>
    /// --> Also See: SetJustText() or JustText() for setting the justification of text objects. vs. controls or shapes.
    /// </summary>
    /// <param name="justType">Type of justification, i.e. JustType::Center, JustType::TopRight, etc.</param>
    /// <returns></returns>
    kwType::ckw SetJust(JustType justType);

    /// <summary>
    /// Sets the justification of an object relative to the window client area, such as text output, a text widget, or other controls and some shapes.
    /// <para></para>
    /// Typically, an object is placed at a specific X,Y position.  However, setting the justification of an object can place the object in anchored points in the window. 
    /// For example, specifying kw:SetJust(JustType::Center) will center a text widget in the window, where kw::SetJust(Just::BottomRight) will place the text widget in the bottom-right area of the window.
    /// This is also useful for TextWidgets and other controls.
    /// <para></para>
    /// ** note:  SetJust() and Just() are the same function.
    /// <para></para>
    /// --> Also See: SetJustText() or JustText() for setting the justification of text objects. vs. controls or shapes.
    /// --> Also See: SetJust(JustType) to use enum values instead, which are faster, i.e. SetJust(JustType::Center) vs. SetJust("center")
    /// </summary>
    /// <param name="justType">Type of justification, i.e. JustType::Center, JustType::TopRight, etc.</param>
    /// <returns></returns>
    kwType::ckw SetJust(const char * sJustType);

    /// <summary>
    /// Sets the justification of an object relative to the window client area, such as text output, a text widget, or other controls and some shapes.
    /// <para></para>
    /// Typically, an object is placed at a specific X,Y position.  However, setting the justification of an object can place the object in anchored points in the window.
    /// <para></para>
    /// For example, specifying kw:SetJust(JustType::Center) will center a text widget in the window, where kw::SetJust(Just::BottomRight) will place the text widget in the bottom-right area of the window.
    /// <para></para>
    /// This is also useful for TextWidgets and other controls.
    /// <para></para>
    /// ** note:  SetJust() and Just() are the same function.
    /// <para></para>
    /// --> Also See: SetJustText() or JustText() for setting the justification of text objects. vs. controls or shapes.
    /// </summary>
    /// <param name="justType">Type of justification, i.e. JustType::Center, JustType::TopRight, etc.</param>
    /// <returns></returns>
     kwType::ckw Just(JustType justType);

    /// <summary>
    /// Sets the justification of an object relative to the window client area, such as text output, a text widget, or other controls and some shapes.
    /// <para></para>
    /// Typically, an object is placed at a specific X,Y position.  However, setting the justification of an object can place the object in anchored points in the window. 
    /// For example, specifying kw:SetJust(JustType::Center) will center a text widget in the window, where kw::SetJust(Just::BottomRight) will place the text widget in the bottom-right area of the window.
    /// This is also useful for TextWidgets and other controls.
    /// <para></para>
    /// ** note:  SetJust() and Just() are the same function.
    /// <para></para>
    /// --> Also See: SetJustText() or JustText() for setting the justification of text objects. vs. controls or shapes.
    /// --> Also See: SetJust(JustType) to use enum values instead, which are faster, i.e. SetJust(JustType::Center) vs. SetJust("center")
    /// </summary>
    /// <param name="justType">Type of justification, i.e. JustType::Center, JustType::TopRight, etc.</param>
    /// <returns></returns>
    kwType::ckw Just(const char * sJustType);


    /// <summary>
    /// Sets the justification of text relative to the window client area or control area, such as writing text or in a text widget, etc.
    /// <para></para>
    /// Typically, text is placed in a pre-defined or specified place.  Using a Text Justification can set the text to specific anchor points within a control's area (e.g. TextWidget)
    /// or the window's entire client area in the case of writing text via the CWindow::Write() function (or WriteGdiText)
    /// <para></para>
    /// For example, specifying kw:SetJustText(JustType::Center) will center text in the window, where kw::SetJustText(Just::BottomRight) will place the text in the bottom-right area of the window.
    /// <para></para>
    /// ** note:  SetJustText() and JustText() are the same function.
    /// <para></para>
    /// --> Also See: SetJust() or Just() for setting the justification of text objects. vs. controls or shapes.
    /// </summary>
    /// <param name="justType">Type of justification, i.e. JustType::Center, JustType::TopRight, etc.</param>
    /// <returns></returns>
    kwType::ckw SetJustText(JustType justType);


    /// <summary>
    /// Sets the justification of text relative to the window client area or control area, such as writing text or in a text widget, etc, using a text string instead of a JustType, 
    /// e.g. TextJust("Center") instead of TextJust(JustType::Center)
    /// <para></para>
    /// Typically, text is placed in a pre-defined or specified place.  Using a Text Justification can set the text to specific anchor points within a control's area (e.g. TextWidget)
    /// or the window's entire client area in the case of writing text via the CWindow::Write() function (or WriteGdiText)
    /// <para></para>
    /// For example, specifying kw:SetJustText("center") will center text in the window, where kw::SetJustText("Just::BottomRight"bottomright") will place the text in the bottom-right area of the window.
    /// <para></para>
    /// ** note:  SetJustText() and JustText() are the same function.
    /// <para></para>
    /// ** note:  Using text strings is provided for convenience, but can be slower in real-time situations.  Using TextJust(JustType::Center) is faster than using TextJust("center").
    /// <para></para>
    /// --> Also See: SetJust() or Just() for setting the justification of text objects. vs. controls or shapes.
    /// </summary>
    /// <param name="justType">Type of justification, i.e. JustType::Center, JustType::TopRight, etc.</param>
    /// <returns></returns>
    kwType::ckw SetJustText(const char * sJustType);

    /// <summary>
    /// Sets the justification of text relative to the window client area or control area, such as writing text or in a text widget, etc.
    /// <para></para>
    /// Typically, text is placed in a pre-defined or specified place.  Using a Text Justification can set the text to specific anchor points within a control's area (e.g. TextWidget)
    /// or the window's entire client area in the case of writing text via the CWindow::Write() function (or WriteGdiText)
    /// <para></para>
    /// For example, specifying kw:SetJustText(JustType::Center) will center text in the window, where kw::SetJustText(Just::BottomRight) will place the text in the bottom-right area of the window.
    /// <para></para>
    /// ** note:  SetJustText() and JustText() are the same function.
    /// <para></para>
    /// --> Also See: SetJust() or Just() for setting the justification of text objects. vs. controls or shapes.
    /// </summary>
    /// <param name="justType">Type of justification, i.e. JustType::Center, JustType::TopRight, etc.</param>
    /// <returns></returns>
    kwType::ckw JustText(JustType justType);

    /// <summary>
    /// Sets the justification of text relative to the window client area or control area, such as writing text or in a text widget, etc, using a text string instead of a JustType, 
    /// e.g. TextJust("Center") instead of TextJust(JustType::Center)
    /// <para></para>
    /// Typically, text is placed in a pre-defined or specified place.  Using a Text Justification can set the text to specific anchor points within a control's area (e.g. TextWidget)
    /// or the window's entire client area in the case of writing text via the CWindow::Write() function (or WriteGdiText)
    /// <para></para>
    /// For example, specifying kw:SetJustText("center") will center text in the window, where kw::SetJustText("Just::BottomRight"bottomright") will place the text in the bottom-right area of the window.
    /// <para></para>
    /// ** note:  SetJustText() and JustText() are the same function.
    /// <para></para>
    /// ** note:  Using text strings is provided for convenience, but can be slower in real-time situations.  Using TextJust(JustType::Center) is faster than using TextJust("center").
    /// <para></para>
    /// --> Also See: SetJust() or Just() for setting the justification of text objects. vs. controls or shapes.
    /// </summary>
    /// <param name="justType">Type of justification, i.e. JustType::Center, JustType::TopRight, etc.</param>
    /// <returns></returns>
    kwType::ckw JustText(JustType justType);

    /// <summary>
    /// Sets the justification of text relative to the window client area or control area, such as writing text or in a text widget, etc, using a text string instead of a JustType, 
    /// e.g. TextJust("Center") instead of TextJust(JustType::Center)
    /// <para></para>
    /// Typically, text is placed in a pre-defined or specified place.  Using a Text Justification can set the text to specific anchor points within a control's area (e.g. TextWidget)
    /// or the window's entire client area in the case of writing text via the CWindow::Write() function (or WriteGdiText)
    /// <para></para>
    /// For example, specifying kw:SetJustText("center") will center text in the window, where kw::SetJustText("Just::BottomRight"bottomright") will place the text in the bottom-right area of the window.
    /// <para></para>
    /// ** note:  SetJustText() and JustText() are the same function.
    /// <para></para>
    /// ** note:  Using text strings is provided for convenience, but can be slower in real-time situations.  Using TextJust(JustType::Center) is faster than using TextJust("center").
    /// <para></para>
    /// --> Also See: SetJust() or Just() for setting the justification of text objects. vs. controls or shapes.
    /// </summary>
    /// <param name="justType">Type of justification, i.e. JustType::Center, JustType::TopRight, etc.</param>
    /// <returns></returns>
    kwType::ckw JustText(const char * sJustType);


    /// <summary>
    /// Truncates float-point values for output.  This is useful for drawing functions to use vertices as integer values
    /// rather than floating-point values that can cause half-pixel drawing causing antialiasing edges that may not be desired.
    /// </summary>
    /// <param name="bTruncate">  - When true (default) Truncate is set as an option.</param>
    /// <returns></returns>
    kwType::ckw Truncate(bool bTruncate = true)      ;

    /// <summary>
    /// DebugBool1-DebugBool4 can be used for debugging and testing purposes.  This allows passing debug-level keywords for testing and development that are not
    /// associated with other keywords.
    /// </summary>
    /// <returns></returns>
    kwType::ckw DebugBool1(bool bDebugValue = true); 

    /// <summary>
    /// DebugBool1-DebugBool4 can be used for debugging and testing purposes.  This allows passing debug-level keywords for testing and development that are not
    /// associated with other keywords.
    /// </summary>
    /// <returns></returns>
    kwType::ckw DebugBool2(bool bDebugValue = true); 

    /// <summary>
    /// DebugBool1-DebugBool4 can be used for debugging and testing purposes.  This allows passing debug-level keywords for testing and development that are not
    /// associated with other keywords.
    /// </summary>
    /// <returns></returns>
    kwType::ckw DebugBool3(bool bDebugValue = true); 

    /// <summary>
    /// DebugBool1-DebugBool4 can be used for debugging and testing purposes.  This allows passing debug-level keywords for testing and development that are not
    /// associated with other keywords.
    /// </summary>
    /// <returns></returns>
    kwType::ckw DebugBool4(bool bDebugValue = true); 

    /// <summary>
    /// ** note:  This function is mostly deprecated.  Use dual colors in the color specification to use gradient, e.g. "red,blue" or { PanColor::Red, PanColor::Blue }
    /// <para></para>
    /// Sets a gradient for various drawn objects, such as rectangles, ellipses, polygons etc.
    /// <para></para>
    /// --> Rgb Colors (i.e. PanColor::ForestGreen, SageColor::Red, etc.) can be used
    /// <para></para>
    /// --> Strings can be used, such as "ForestGreen", "Red", etc. 
    /// <para></para>
    /// *** Note that using strings takes much longer to process, and can affect tight, real-time loops.
    /// <para></para>
    /// --> The Gradient color is the secondary gradient color (i.e. bottom color) - the top color is the first color specified in the function call.
    /// <para></para>
    /// </summary>
    /// <returns></returns>
    kwType::ckw GradientColor(const CRgbColor & rgbColor);


    kwType::ckw AutoGradient(bool bGradient = true);
    kwType::ckw AutoGradient(int iGradient);

    /// <summary>
    /// Sets the angle of a gradient (in radians) specified for drawn objects (i.e. rectangles, ellipses, etc.). 
    /// <para></para>
    /// --> Angle is specified in radians.  See GradientAngle() to use degrees.
    /// <para></para>
    /// --> Gradients are top-to-bottom by default (i.e. 0 degrees).  For example, 90 degrees is a gradient from left-to-right vs. top-bottom.
    /// </summary>
    /// <param name="fAngleRad"> - Angle of Gradient, in radians.</param>
    /// <returns></returns>
    /// 
    kwType::ckw GradientAngleRad(float fAngleRad);

    /// <summary>
    /// Sets the angle of a gradient (in radians) specified for drawn objects (i.e. rectangles, ellipses, etc.). 
    /// <para></para>
    /// --> Angle is specified in radians.  See GradientAngle() to use degrees.
    /// <para></para>
    /// --> Gradients are top-to-bottom by default (i.e. 0 degrees).  For example, 90 degrees is a gradient from left-to-right vs. top-bottom.
    /// </summary>
    /// <param name="fAngleRad"> - Angle of Gradient, in radians.</param>
    /// <returns></returns>
    kwType::ckw GradientAngleRad(double fAngleRad);

    /// <summary>
    /// Sets the angle of a gradient (in degrees) specified for drawn objects (i.e. rectangles, ellipses, etc.). 
    /// <para></para>
    /// --> Angle is specified in degrees.  See GradientAngleRad() to use radians.
    /// <para></para>
    /// --> Gradients are top-to-bottom by default (i.e. 0 degrees).  For example, 90 degrees is a gradient from left-to-right vs. top-bottom.
    /// </summary>
    /// <param name="AngleDeg"> - Angle of Gradient, in degrees.</param>
    /// <returns></returns>
    kwType::ckw GradientAngle(int iAngleDeg)         ;

    /// <summary>
    /// Sets the angle of a gradient (in degrees) specified for drawn objects (i.e. rectangles, ellipses, etc.). 
    /// <para></para>
    /// --> Angle is specified in degrees.  See GradientAngleRad() to use radians.
    /// <para></para>
    /// --> Gradients are top-to-bottom by default (i.e. 0 degrees).  For example, 90 degrees is a gradient from left-to-right vs. top-bottom.
    /// </summary>
    /// <param name="AngleDeg"> - Angle of Gradient, in degrees.</param>
    /// <returns></returns>
    kwType::ckw GradientAngle(float fAngleDeg)       ;

    /// <summary>
    /// Sets the angle of a gradient (in degrees) specified for drawn objects (i.e. rectangles, ellipses, etc.). 
    /// <para></para>
    /// --> Angle is specified in degrees.  See GradientAngleRad() to use radians.
    /// <para></para>
    /// --> Gradients are top-to-bottom by default (i.e. 0 degrees).  For example, 90 degrees is a gradient from left-to-right vs. top-bottom.
    /// </summary>
    /// <param name="AngleDeg">Angle of Gradient, in degrees.</param>
    /// <returns></returns>
    kwType::ckw GradientAngle(double fAngleDeg)      ;

    /// <summary>
    /// ** note:  This function is mostly deprecated.  Use dual colors in the color specification to use gradient, e.g. "red,blue" or { PanColor::Red, PanColor::Blue }
    /// <para></para>
    /// Sets a gradient for the border (i.e. pen area) of a drawn object, such as a rectangle, ellipse, etc.
    /// <para></para>
    /// --> Rgb Colors (i.e. PanColor::ForestGreen, SageColor::Red, etc.) can be used
    /// <para></para>
    /// --> Strings can be used, such as "ForestGreen", "Red", etc. 
    /// <para></para>
    /// *** Note that using strings takes much longer to process, and can affect tight, real-time loops.
    /// <para></para>
    /// --> The Gradient color is the secondary gradient color (i.e. bottom color) - the top color is the first color specified in the function call.
    /// --> The the object has no interior (i.e. only an open border or pen), Gradient() and PenGradient() are the same functions.
    /// </summary>
    /// <param name="rgbGradient"></param>
    /// <returns></returns>
    kwType::ckw PenGradientColor(const CRgbColor & rgbColor);

    /// <summary>
    /// Sets the angle of the border -- or pen area -- (in radians) of a drawn object, such as a rectangle, ellipse, etc. 
    /// <para></para>
    /// --> Use PenGradientAngle() to set the angle in degrees instead of radians.
    /// --> Use GradientAngle() to set the angle of the interior gradient of a filled object.  This allows two different gradient angles between
    /// the interior of an object and the border itself.
    /// <para></para>
    /// --> When the drawn object has no interior (i.e. has only a pen or border), PenGradientAnfle() and Gradient() angle are the same function.
    /// 
    /// </summary>
    /// <param name="fAngleRad"> - Angle of pen gradient in radians.</param>
    /// <returns></returns>
    kwType::ckw PenGradientAngleRad(float fAngleRad)          ;

    /// <summary>
    /// Sets the angle of the border -- or pen area -- (in radians) of a drawn object, such as a rectangle, ellipse, etc. 
    /// <para></para>
    /// --> Use PenGradientAngle() to set the angle in degrees instead of radians.
    /// --> Use GradientAngle() to set the angle of the interior gradient of a filled object.  This allows two different gradient angles between
    /// the interior of an object and the border itself.
    /// <para></para>
    /// --> When the drawn object has no interior (i.e. has only a pen or border), PenGradientAnfle() and Gradient() angle are the same function.
    /// 
    /// </summary>
    /// <param name="fAngleRad"> - Angle of pen gradient in radians.</param>
    /// <returns></returns>
    kwType::ckw PenGradientAngleRad(double fAngleRad)         ;

    /// <summary>
    /// Sets the angle of the border -- or pen area -- (in degrees) of a drawn object, such as a rectangle, ellipse, etc. 
    /// <para></para>
    /// --> Use PenGradientAngleRad() to set the angle in radians instead of degrees.
    /// --> Use GradientAngle() to set the angle of the interior gradient of a filled object.  This allows two different gradient angles between
    /// the interior of an object and the border itself.
    /// <para></para>
    /// --> When the drawn object has no interior (i.e. has only a pen or border), PenGradientAnfle() and Gradient() angle are the same function.
    /// 
    /// </summary>
    /// <param name="AngleDeg"> - Angle of pen gradient in degrees.</param>
    /// <returns></returns>
    kwType::ckw PenGradientAngle(int iAngleDeg)         ;
 
    /// <summary>
    /// Sets the angle of the border -- or pen area -- (in degrees) of a drawn object, such as a rectangle, ellipse, etc. 
    /// <para></para>
    /// --> Use PenGradientAngleRad() to set the angle in radians instead of degrees.
    /// --> Use GradientAngle() to set the angle of the interior gradient of a filled object.  This allows two different gradient angles between
    /// the interior of an object and the border itself.
    /// <para></para>
    /// --> When the drawn object has no interior (i.e. has only a pen or border), PenGradientAnfle() and Gradient() angle are the same function.
    /// 
    /// </summary>
    /// <param name="AngleDeg"> - Angle of pen gradient in degrees.</param>
    /// <returns></returns>
    kwType::ckw PenGradientAngle(float fAngleDeg)       ;

    /// <summary>
    /// Sets the angle of the border -- or pen area -- (in degrees) of a drawn object, such as a rectangle, ellipse, etc. 
    /// <para></para>
    /// --> Use PenGradientAngleRad() to set the angle in radians instead of degrees.
    /// --> Use GradientAngle() to set the angle of the interior gradient of a filled object.  This allows two different gradient angles between
    /// the interior of an object and the border itself.
    /// <para></para>
    /// --> When the drawn object has no interior (i.e. has only a pen or border), PenGradientAnfle() and Gradient() angle are the same function.
    /// 
    /// </summary>
    /// <param name="AngleDeg"> - Angle of pen gradient in degrees.</param>
    /// <returns></returns>
    kwType::ckw PenGradientAngle(double fAngleDeg)      ;


 
    // $$ Mouse Region Experimental
    //  
    kwType::ckw SelectionAutoFocus(bool bAutoFocus = true)      ;

    /// <summary>
    /// For Mouse Regions and some other functions, kw::BoundBox() sets a bounding box for the region. 
    /// <para></para>
    /// --> For example, with a Mouse Region, this will limit the mouse region to the boundaries of the bounding box, not allowing
    /// the mouse to move the region/point outside of this area.
    /// <para></para>
    /// ** note:  For write() and graphics display functions, use SetRegion() or ClipRegion().
    /// </summary>
    /// <param name="srRect"></param>
    /// <returns></returns>
    kwType::ckw BoundBox(const SizeRect & srRect);

    /// <summary>
	/// Keeps the control, widget, or window hidden as it is created.
    /// <para></para>
	/// By default, all Windows, Widgets, and Controls appear upon creation. 
    /// <para></para>
	/// Hidden() will keep the Window or Control hidden, allowing for changes to appearance and behavior before they
	/// show in the screen.
    /// <para></para>
	/// --> Use &lt;control&gt;.Show() (i.e. myWindow.Show()) to show the Window, Control or Widget that is hidden.
    /// <para></para>
	/// ** note: Hide() and Hidden() are the same function
    /// </summary>
    /// <returns></returns>
    kwType::ckw Hidden(bool bHidden = true);

    /// <summary>
	/// Keeps the control, widget, or window hidden as it is created.
    /// <para></para>
	/// By default, all Windows, Widgets, and Controls appear upon creation. 
    /// <para></para>
	/// Hidden() will keep the Window or Control hidden, allowing for changes to appearance and behavior before they
	/// show in the screen.
    /// <para></para>
	/// --> Use &lt;control&gt;.Show() (i.e. myWindow.Show()) to show the Window, Control or Widget that is hidden.
    /// <para></para>
	/// ** note: Hide() and Hidden() are the same function
    /// </summary>
    /// <returns></returns>
    kwType::ckw Hide(bool bHide = true);

    /// <summary>
	/// Allows the control or window from to be resized by the user by grabbing the edges of the window/control.
	/// Windows and some controls are not resizeable by default.   
    /// <para></para>
    /// --> Using ResizeOk() allows this when using the mouse. 
    /// <para></para>
    /// --> The Window or Control can still be resized programmatically via the SetWindowSize() function for the specific window/control.
    /// </summary>
    kwType::ckw ResizeOk(bool bResizeOk = true);     

	/// <summary>
	/// Sets the window for direct output to the display bitmap rather than buffer the output to a separate bitmap,
    /// which then displays when an update occurs -- this will paint direfctly to the window, making the program responsible for PAINT messages from the OS
    /// <para></para> 
    /// This also Signals that a window will be used for Real Time drawing, typically based on the vertical retrace. 
    /// <para></para> 
    /// --> Sets AutoUpdate off for the Window (Window must be automatically updated) <para></para>
    /// --> Creates memory and sets other environmental factors more suitable to real-time graphics display <para></para>
    /// --> Also see RealTime() option to set up a RealTime display status without setting a direct output to the window.<para></para>
    /// Note: This function is still TBD and is meant to work with real-time graphics as well as the GPU. 
    /// When this option is used, Auto Updating of the window is also turned off. 
	/// </summary>
	/// <returns></returns>	
    kwType::ckw DirectDraw(bool bDirectDraw = true);

	/// <summary>
	/// Signals that a window will be used for Real Time drawing, typically based on the vertical retrace. 
    /// <para></para> 
    /// --> sets AutoUpdate off for the Window (Window must be automatically updated) <para></para>
    /// <para></para> 
    /// --> Creates memory and sets other environmental factors more suitable to real-time graphics display <para></para>
    /// <para></para> 
    /// --> Also see DirectDraw() option<para></para>
    /// Note: This function is still TBD and is meant to work with real-time graphics as well as the GPU. 
    /// <para></para> 
    /// When this option is used, Auto Updating of the window is also turned off. 
	/// </summary>
	/// <returns></returns>
    kwType::ckw Realtime(bool bRealtime = true);

    /// <summary>
	/// Adds a title to a control, such as a slider, input dialog title, and other controls, windows, etc.
    /// <para></para> 
	/// Without a title, many controls/dialog show the ProgramName() in the title bar of the program name is set.
	/// Otherwise, this value is left blank or set to a default that is descriptive of the control.
    /// </summary>
    /// <returns></returns>
    kwType::ckw Title(const char * sTitle)               ;
    
    /// <summary>
	/// Set the name of the control.  This can be used to find the control later, without saving the 
	/// returned object pointer.
    /// <para></para>
	/// --> ID() with an integer value can also be used for the same result and is more efficient, though name() can be easier to remember,
	/// such as "MyButton" or "RangeSlider"
    /// <para></para>
	/// Example: (create a button) -- >CreateButton(10,10,"Ok",Name("MyButton");
    /// <para></para>
	/// Example: (get a button by name) --> auto&amp; MyButton = cWin.button("MyButton");
    /// <para></para>
	/// Example: (get button status by name) --> button("MyButton").Pressed() (i.e. any function) can be used for quick access, though it is inefficient.
    /// </summary>
    /// <param name="sName"></param>
    /// <returns></returns>
    kwType::ckw Name(const char * sName);

    /// <summary>
	/// Sets the ID of the control, window or Widget.
    /// <para></para>
	/// This gives the control an identifier than can be used to find it or control it later without needing the control object pointer.
	/// This can be used for Groups or for general ID.
    /// <para></para>
	/// --> note: Avoid using 0 for an ID, as this conflicts with some functions mixing a boolean return with 
	/// a control value. 
    /// <para></para>
	/// --> For example, to find a button with a control: CreateButton(10,20,"OK",ID(10));
    /// <para></para>
	/// --> Later on, auto&amp; MyButton = button(10) can be used to get the button pointer, or button(10).Pressed()  (i.e. any function) can be used to quickly access
	/// the button without knowing the control pointer value.
    /// <para></para>
    /// *** Note:  ID() and ControlID() are the same functions.
    /// </summary>
    /// <param name="iID"> - Integer ID value of control.</param>
    /// <returns></returns>
    kwType::ckw ID(int iID);

    /// <summary>
	/// Sets the ID of the control, window or Widget.
    /// <para></para>
	/// This gives the control an identifier than can be used to find it or control it later without needing the control object pointer.
	/// This can be used for Groups or for general ID.
    /// <para></para>
	/// --> note: Avoid using 0 for an ID, as this conflicts with some functions mixing a boolean return with 
	/// a control value. 
    /// <para></para>
	/// --> For example, to find a button with a control: CreateButton(10,20,"OK",ID(10));
    /// <para></para>
	/// --> Later on, auto&amp; MyButton = button(10) can be used to get the button pointer, or button(10).Pressed()  (i.e. any function) can be used to quickly access
	/// the button without knowing the control pointer value.
    /// <para></para>
    /// *** Note:  ID() and ControlID() are the same functions.
    /// </summary>
    /// <param name="iID"> - Integer ID value of control.</param>
    /// <returns></returns>
    kwType::ckw ControlID(int iID);

    /// <summary>
    // Set the Text Font for the current Control, Widget, or Window.
    /// <para></para>
	// --> When a plain integer is used, i.e. Font(15), it is set to the default font with a size of the value, i.e. "Arial,15"
    /// <para></para>
	// Examples:
    /// <para></para>
	// -->    Font(20)  -- Set the font to "Arial,20".
    /// <para></para>
	// -->    Font("Arial,20")  -- Set the font to "Arial,20".  You can also add italic an bold, i.e. Font("Arial,20,bold,italic");
    /// <para></para>
	// -->    Font("MyFont")    -- Set the font by a previously created font named "MyFont"
    /// <para></para>
	// *** note: Font() and TextFont() are the same function.
    /// </summary>
    /// <returns></returns>
    kwType::ckw Font(const char * sFont);

    /// <summary>
    // Set the Text Font for the current Control, Widget, or Window.
    /// <para></para>
	// --> When a plain integer is used, i.e. Font(15), it is set to the default font with a size of the value, i.e. "Arial,15"
    /// <para></para>
	// Examples:
    /// <para></para>
	// -->    Font(20)  -- Set the font to "Arial,20".
    /// <para></para>
	// -->    Font("Arial,20")  -- Set the font to "Arial,20".  You can also add italic an bold, i.e. Font("Arial,20,bold,italic");
    /// <para></para>
	// -->    Font("MyFont")    -- Set the font by a previously created font named "MyFont"
    /// <para></para>
	// *** note: Font() and TextFont() are the same function.
    /// </summary>
    /// <returns></returns>
    kwType::ckw Font(int iFontSize);

    /// <summary>
    // Set the Text Font for the current Control, Widget, or Window.
    /// <para></para>
	// --> When a plain integer is used, i.e. Font(15), it is set to the default font with a size of the value, i.e. "Arial,15"
    /// <para></para>
	// Examples:
    /// <para></para>
	/// -->    Font(20)  -- Set the font to "Arial,20".
    /// <para></para>
	/// -->    Font("Arial,20")  -- Set the font to "Arial,20".  You can also add italic an bold, i.e. Font("Arial,20,bold,italic");
    /// <para></para>
	/// -->    Font("MyFont")    -- Set the font by a previously created font named "MyFont"
    /// <para></para>
	/// *** note: Font() and TextFont() are the same function.
    /// </summary>
    /// <returns></returns>
    kwType::ckw Font(float fFontSize);

    /// <summary>
    /// Set the Text Font for the current Control, Widget, or Window with a Windows HFONT value as input..
    /// <para></para>
	/// --> An HFONT value is returned when a font is created.  Also, CWindow::GetFont(&lt;FontName&gt;) can also be used to retrieve the HFONT value for storage and later usage
    ///(e.g. HFONT hMyFont = myWindow.GetFont("MyFont").
    /// <para></para>
	/// Examples:
    /// <para></para>
	/// -->    Font(hMyFont)  -- Set the font to the font specified by the value in hMyFont.
    /// <para></para>
	/// *** note: Font() and TextFont() are the same function.
    /// </summary>
    /// <returns></returns>
    kwType::ckw Font(HFONT hfont);

    /// <summary>
    // Set the Text Font for the current Control, Widget, or Window.
    /// <para></para>
	// --> When a plain integer is used, i.e. Font(15), it is set to the default font with a size of the value, i.e. "Arial,15"
    /// <para></para>
	// Examples:
    /// <para></para>
	// -->    Font(20)  -- Set the font to "Arial,20".
    /// <para></para>
	// -->    Font("Arial,20")  -- Set the font to "Arial,20".  You can also add italic an bold, i.e. Font("Arial,20,bold,italic");
    /// <para></para>
	// -->    Font("MyFont")    -- Set the font by a previously created font named "MyFont"
    /// <para></para>
	// *** note: Font() and TextFont() are the same function.
    /// </summary>
    /// <returns></returns>
    kwType::ckw Font(double fFontSize);

    /// <summary>
    // Set the Text Font for the current Control, Widget, or Window.
    /// <para></para>
	// --> When a plain integer is used, i.e. Font(15), it is set to the default font with a size of the value, i.e. "Arial,15"
    /// <para></para>
	// Examples:
    /// <para></para>
	// -->    TextFont(20)  -- Set the font to "Arial,20".
    /// <para></para>
	// -->    TextFont("Arial,20")  -- Set the font to "Arial,20".  You can also add italic an bold, i.e. Font("Arial,20,bold,italic");
    /// <para></para>
	// -->    TextFont("MyFont")    -- Set the font by a previously created font named "MyFont"
    /// <para></para>
	// *** note: TextFont() and TextFont() are the same function.
    /// </summary>
    /// <returns></returns>
    kwType::ckw TextFont(const char * sFont);

    /// <summary>
    // Set the Text Font for the current Control, Widget, or Window.
    /// <para></para>
	// --> When a plain integer is used, i.e. Font(15), it is set to the default font with a size of the value, i.e. "Arial,15"
    /// <para></para>
	// Examples:
    /// <para></para>
	// -->    TextFont(20)  -- Set the font to "Arial,20".
    /// <para></para>
	// -->    TextFont("Arial,20")  -- Set the font to "Arial,20".  You can also add italic an bold, i.e. Font("Arial,20,bold,italic");
    /// <para></para>
	// -->    TextFont("MyFont")    -- Set the font by a previously created font named "MyFont"
    /// <para></para>
	// *** note: Font() and TextFont() are the same function.
    /// </summary>
    /// <returns></returns>
    kwType::ckw TextFont(int iFontSize);

    /// <summary>
    /// Set the Text Font for the current Control, Widget, or Window with a Windows HFONT value as input..
    /// <para></para>
	/// --> An HFONT value is returned when a font is created.  Also, CWindow::GetFont(&lt;FontName&gt;) can also be used to retrieve the HFONT value for storage and later usage
    ///(e.g. HFONT hMyFont = myWindow.GetFont("MyFont").
    /// <para></para>
	/// Examples:
    /// <para></para>
	/// -->    Font(hMyFont)  -- Set the font to the font specified by the value in hMyFont.
    /// <para></para>
	/// *** note: Font() and TextFont() are the same function.
    /// </summary>
    /// <returns></returns>
    kwType::ckw TextFont(HFONT hfont);


    /// <summary>
    // Set the Text Font for the current Control, Widget, or Window.
    /// <para></para>
	// --> When a plain integer is used, i.e. Font(15), it is set to the default font with a size of the value, i.e. "Arial,15"
    /// <para></para>
	// Examples:
    /// <para></para>
	// -->    TextFont(20)  -- Set the font to "Arial,20".
    /// <para></para>
	// -->    TextFont("Arial,20")  -- Set the font to "Arial,20".  You can also add italic an bold, i.e. Font("Arial,20,bold,italic");
    /// <para></para>
	// -->    TextFont("MyFont")    -- Set the font by a previously created font named "MyFont"
    /// <para></para>
	// *** note: Font() and TextFont() are the same function.
    /// </summary>
    /// <returns></returns>
    kwType::ckw TextFont(float fFontSize);

    /// <summary>
    // Set the Text Font for the current Control, Widget, or Window.
    /// <para></para>
	// --> When a plain integer is used, i.e. Font(15), it is set to the default font with a size of the value, i.e. "Arial,15"
    /// <para></para>
	// Examples:
    /// <para></para>
	// -->    TextFont(20)  -- Set the font to "Arial,20".
    /// <para></para>
	// -->    TextFont("Arial,20")  -- Set the font to "Arial,20".  You can also add italic an bold, i.e. Font("Arial,20,bold,italic");
    /// <para></para>
	// -->    TextFont("MyFont")    -- Set the font by a previously created font named "MyFont"
    /// <para></para>
	// *** note: Font() and TextFont() are the same function.
    /// </summary>
    /// <returns></returns>
    kwType::ckw TextFont(double fFontSize);

    /// <summary>
    /// Sets the background color of a window, text widget, control, or other object. 
    /// <para></para>
    /// --> Rgb Colors such as PanColor::ForestGreen and SageColor::Red can be used.
    /// <para></para>
    /// --> String based colors can be used, also, such as "ForestGreen" or "Red", although processing is slower in real-time situations.
    /// <para></para>
    /// --> For windows and other controls and objects that can use gradients, two colors can used such as bgColor(PanColor::Blue,PanColor::White) or 
    /// bgColor("blue,white") to express a gradient.
    /// <para></para>
    /// --> Example: auto&amp; MyWindow = Sagebox::NewWindow(bgColor(SageColor::Blue)) - sets a blue background using rgb colors
    /// <para></para>
    /// --> Example: auto&amp; MyWindow = Sagebox::NewWindow(bgColor("black","blue")) - sets a blue-to-black gradient background using string-based colors.
    /// </summary>
    /// <param name="bgColor"> - Rgb or String color</param>
    /// <param name="bgColor2"> - Secondary Rgb or String color to form a gradient (when applicable)</param>
    /// <returns></returns>
    kwType::ckw bgColor(RgbColor bgColor);  

    /// <summary>
    /// Sets the background color of a window, text widget, control, or other object. 
    /// <para></para>
    /// --> Rgb Colors such as PanColor::ForestGreen and SageColor::Red can be used.
    /// <para></para>
    /// --> String based colors can be used, also, such as "ForestGreen" or "Red", although processing is slower in real-time situations.
    /// <para></para>
    /// --> For windows and other controls and objects that can use gradients, two colors can used such as bgColor(PanColor::Blue,PanColor::White) or 
    /// bgColor("blue,white") to express a gradient.
    /// <para></para>
    /// --> Example: auto&amp; MyWindow = Sagebox::NewWindow(bgColor(SageColor::Blue)) - sets a blue background using rgb colors
    /// <para></para>
    /// --> Example: auto&amp; MyWindow = Sagebox::NewWindow(bgColor("black","blue")) - sets a blue-to-black gradient background using string-based colors.
    /// </summary>
    /// <param name="bgColor"> - Rgb or String color</param>
    /// <param name="bgColor2"> - Secondary Rgb or String color to form a gradient (when applicable)</param>
    /// <returns></returns>
    kwType::ckw bgColor(RgbColor bgColor,RgbColor bgColor2);

    /// <summary>
    /// Sets the background color of a window, text widget, control, or other object. 
    /// <para></para>
    /// --> Rgb Colors such as PanColor::ForestGreen and SageColor::Red can be used.
    /// <para></para>
    /// --> String based colors can be used, also, such as "ForestGreen" or "Red", although processing is slower in real-time situations.
    /// <para></para>
    /// --> For windows and other controls and objects that can use gradients, two colors can used such as bgColor(PanColor::Blue,PanColor::White) or 
    /// bgColor("blue,white") to express a gradient.
    /// <para></para>
    /// --> Example: auto&amp; MyWindow = Sagebox::NewWindow(bgColor(SageColor::Blue)) - sets a blue background using rgb colors
    /// <para></para>
    /// --> Example: auto&amp; MyWindow = Sagebox::NewWindow(bgColor("black","blue")) - sets a blue-to-black gradient background using string-based colors.
    /// </summary>
    /// <param name="bgColor"> - Rgb or String color</param>
    /// <param name="bgColor2"> - Secondary Rgb or String color to form a gradient (when applicable)</param>
    /// <returns></returns>
    kwType::ckw bgColor(const char * sbgColor);;

    /// <summary>
    /// Sets the background color of a window, text widget, control, or other object. 
    /// <para></para>** note: bgColor() and Color() are the same function.
    /// <para></para>
    /// --> Rgb Colors such as PanColor::ForestGreen and SageColor::Red can be used.
    /// <para></para>
    /// --> String based colors can be used, also, such as "ForestGreen" or "Red", although processing is slower in real-time situations.
    /// <para></para>
    /// --> For windows and other controls and objects that can use gradients, two colors can used such as bgColor(PanColor::Blue,PanColor::White) or 
    /// bgColor("blue,white") to express a gradient.
    /// <para></para>
    /// --> Example: auto&amp; MyWindow = Sagebox::NewWindow(bgColor(SageColor::Blue)) - sets a blue background using rgb colors
    /// <para></para>
    /// --> Example: auto&amp; MyWindow = Sagebox::NewWindow(bgColor("black","blue")) - sets a blue-to-black gradient background using string-based colors.
    /// </summary>
    /// <param name="bgColor"> - Rgb or String color</param>
    /// <param name="bgColor2"> - Secondary Rgb or String color to form a gradient (when applicable)</param>
    /// <returns></returns>
    kwType::ckw bgColor(const char * sbgColor,const char * sbgColor2);

    /// <summary>
    /// Sets the background color of a window, text widget, control, or other object. 
    /// <para></para>** note: bgColor() and Color() are the same function.
    /// <para></para>
    /// --> Rgb Colors such as PanColor::ForestGreen and SageColor::Red can be used.
    /// <para></para>
    /// --> String based colors can be used, also, such as "ForestGreen" or "Red", although processing is slower in real-time situations.
    /// <para></para>
    /// --> For windows and other controls and objects that can use gradients, two colors can used such as bgColor(PanColor::Blue,PanColor::White) or 
    /// bgColor("blue,white") to express a gradient.
    /// <para></para>
    /// --> Example: auto&amp; MyWindow = Sagebox::NewWindow(Color(SageColor::Blue)) - sets a blue background using rgb colors
    /// <para></para>
    /// --> Example: auto&amp; MyWindow = Sagebox::NewWindow(Color("black","blue")) - sets a blue-to-black gradient background using string-based colors.
    /// </summary>
    /// <param name="bgColor"> - Rgb or String color</param>
    /// <param name="bgColor2"> - Secondary Rgb or String color to form a gradient (when applicable)</param>
    /// <returns></returns>
    kwType::ckw Color(RgbColor bgColor);

    /// <summary>
    /// Sets the background color of a window, text widget, control, or other object. 
    /// <para></para>** note: bgColor() and Color() are the same function.
    /// <para></para>
    /// --> Rgb Colors such as PanColor::ForestGreen and SageColor::Red can be used.
    /// <para></para>
    /// --> String based colors can be used, also, such as "ForestGreen" or "Red", although processing is slower in real-time situations.
    /// <para></para>
    /// --> For windows and other controls and objects that can use gradients, two colors can used such as bgColor(PanColor::Blue,PanColor::White) or 
    /// bgColor("blue,white") to express a gradient.
    /// <para></para>
    /// --> Example: auto&amp; MyWindow = Sagebox::NewWindow(Color(SageColor::Blue)) - sets a blue background using rgb colors
    /// <para></para>
    /// --> Example: auto&amp; MyWindow = Sagebox::NewWindow(Color("black","blue")) - sets a blue-to-black gradient background using string-based colors.
    /// </summary>
    /// <param name="bgColor"> - Rgb or String color</param>
    /// <param name="bgColor2"> - Secondary Rgb or String color to form a gradient (when applicable)</param>
    /// <returns></returns>
    kwType::ckw Color(RgbColor bgColor,RgbColor bgColor2);

    /// <summary>
    /// Sets the background color of a window, text widget, control, or other object. 
    /// <para></para>** note: bgColor() and Color() are the same function.
    /// <para></para>
    /// --> Rgb Colors such as PanColor::ForestGreen and SageColor::Red can be used.
    /// <para></para>
    /// --> String based colors can be used, also, such as "ForestGreen" or "Red", although processing is slower in real-time situations.
    /// <para></para>
    /// --> For windows and other controls and objects that can use gradients, two colors can used such as bgColor(PanColor::Blue,PanColor::White) or 
    /// bgColor("blue,white") to express a gradient.
    /// <para></para>
    /// --> Example: auto&amp; MyWindow = Sagebox::NewWindow(Color(SageColor::Blue)) - sets a blue background using rgb colors
    /// <para></para>
    /// --> Example: auto&amp; MyWindow = Sagebox::NewWindow(Color("black","blue")) - sets a blue-to-black gradient background using string-based colors.
    /// </summary>
    /// <param name="bgColor"> - Rgb or String color</param>
    /// <param name="bgColor2"> - Secondary Rgb or String color to form a gradient (when applicable)</param>
    /// <returns></returns>
    kwType::ckw Color(const char * sbgColor);;

    /// <summary>
    /// Sets the background color of a window, text widget, control, or other object. 
    /// <para></para>** note: bgColor() and Color() are the same function.
    /// <para></para>
    /// --> Rgb Colors such as PanColor::ForestGreen and SageColor::Red can be used.
    /// <para></para>
    /// --> String based colors can be used, also, such as "ForestGreen" or "Red", although processing is slower in real-time situations.
    /// <para></para>
    /// --> For windows and other controls and objects that can use gradients, two colors can used such as bgColor(PanColor::Blue,PanColor::White) or 
    /// bgColor("blue,white") to express a gradient.
    /// <para></para>
    /// --> Example: auto&amp; MyWindow = Sagebox::NewWindow(Color(SageColor::Blue)) - sets a blue background using rgb colors
    /// <para></para>
    /// --> Example: auto&amp; MyWindow = Sagebox::NewWindow(Color("black","blue")) - sets a blue-to-black gradient background using string-based colors.
    /// </summary>
    /// <param name="bgColor"> - Rgb or String color</param>
    /// <param name="bgColor2"> - Secondary Rgb or String color to form a gradient (when applicable)</param>
    /// <returns></returns>
    kwType::ckw Color(const char * sbgColor,const char * sbgColor2);

    /// <summary>
    /// Sets the foreground (i.e. text color) color of a window, text widget, control, or other object. 
    /// <para></para>** note: fgColor() and TextColor() are the same function.
    /// <para></para>
    /// --> Rgb Colors such as PanColor::ForestGreen and SageColor::Red can be used.
    /// <para></para>
    /// --> String based colors can be used, also, such as "ForestGreen" or "Red", although processing is slower in real-time situations.
    /// <para></para>
    /// --> Example: auto&amp; MyWindow = Sagebox::NewWindow(fgColor(SageColor::Yellow)) - Sets the text folor to Yellow
    /// <para></para>
    /// --> Example: auto&amp; MyWindow = Sagebox::NewWindow(fgColor("Yellow")) - sets the text color to yellow using string-based colors.
    /// </summary>
    /// <param name="bgColor"> - Rgb or String color</param>
    /// <returns></returns>
    kwType::ckw fgColor(RgbColor fgColor);

    /// <summary>
    /// Sets the foreground (i.e. text color) color of a window, text widget, control, or other object. 
    /// <para></para>** note: fgColor() and TextColor() are the same function.
    /// <para></para>
    /// --> Rgb Colors such as PanColor::ForestGreen and SageColor::Red can be used.
    /// <para></para>
    /// --> String based colors can be used, also, such as "ForestGreen" or "Red", although processing is slower in real-time situations.
    /// <para></para>
    /// --> Example: auto&amp; MyWindow = Sagebox::NewWindow(fgColor(SageColor::Yellow)) - Sets the text folor to Yellow
    /// <para></para>
    /// --> Example: auto&amp; MyWindow = Sagebox::NewWindow(fgColor("Yellow")) - sets the text color to yellow using string-based colors.
    /// </summary>
    /// <param name="bgColor"> - Rgb or String color</param>
    /// <returns></returns>
    kwType::ckw fgColor(const char * sfgColor);

    /// <summary>
    /// Sets the foreground (i.e. text color) color of a window, text widget, control, or other object. 
    /// <para></para>** note: fgColor() and TextColor() are the same function.
    /// <para></para>
    /// --> Rgb Colors such as PanColor::ForestGreen and SageColor::Red can be used.
    /// <para></para>
    /// --> String based colors can be used, also, such as "ForestGreen" or "Red", although processing is slower in real-time situations.
    /// <para></para>
    /// --> Example: auto&amp; MyWindow = Sagebox::NewWindow(TextColor(SageColor::Yellow)) - Sets the text folor to Yellow
    /// <para></para>
    /// --> Example: auto&amp; MyWindow = Sagebox::NewWindow(TextColor("Yellow")) - sets the text color to yellow using string-based colors.
    /// </summary>
    /// <param name="TextColor"> - Rgb or String color</param>
    /// <returns></returns>
    kwType::ckw TextColor(RgbColor rgbTextColor);

    /// <summary>
    /// Sets the foreground (i.e. text color) color of a window, text widget, control, or other object. 
    /// <para></para>** note: fgColor() and TextColor() are the same function.
    /// <para></para>
    /// --> Rgb Colors such as PanColor::ForestGreen and SageColor::Red can be used.
    /// <para></para>
    /// --> String based colors can be used, also, such as "ForestGreen" or "Red", although processing is slower in real-time situations.
    /// <para></para>
    /// --> Example: auto&amp; MyWindow = Sagebox::NewWindow(TextColor(SageColor::Yellow)) - Sets the text folor to Yellow
    /// <para></para>
    /// --> Example: auto&amp; MyWindow = Sagebox::NewWindow(TextColor("Yellow")) - sets the text color to yellow using string-based colors.
    /// </summary>
    /// <param name="TextColor"> - Rgb or String color</param>
    /// <returns></returns>
    kwType::ckw TextColor(const char * sTextColor);

    /// <summary>
    /// Sets the border color of a control or other object that has an outline or border. 
    /// <para></para>
    /// --> Rgb Colors such as PanColor::ForestGreen and SageColor::Red can be used.
    /// <para></para>
    /// --> String based colors can be used, also, such as "ForestGreen" or "Red", although processing is slower in real-time situations.
    /// </summary>
    /// <param name="BorderColor"> - Rgb or String color</param>
    /// <returns></returns>
    kwType::ckw BorderColor(const CRgbColor & rgbBorderColor);
 

    /// <summary>
    // Tells the Window, Control, input box, or Widget to no use a border when it may otherwise default to using a border.
    /// </summary>
    /// <returns></returns>
    kwType::ckw NoBorder(bool bNoBorder = true);

    /// <summary>
    /// This keyword is still in progress. TBD
    /// </summary>
    /// <param name="bChild"></param>
    /// <returns></returns>
    kwType::ckw Child(bool bChild = true);

    /// <summary>
	/// Adds a border to the control, such as a Window or Input Control (i.e. string, integer, etc.). 
    /// <para></para>
    /// <para>--> note: AddBorder() and Border() are the same function.</para>
    /// </summary>
    /// <returns></returns>
    kwType::ckw AddBorder(bool bBorder = true);

    /// <summary>
	/// Adds a border to the control, such as a Window or Input Control (i.e. string, integer, etc.). 
    /// <para></para>
	/// This is deprecated and will be changed to AddBorder()
    /// <para>--> note: AddBorder() and Border() are the same function.</para>
    /// </summary>
    /// <returns></returns>
    kwType::ckw Border(bool bBorder = true);

    /// <summary>
	/// Adds a recessed border to the control, such as a Window or Input Control (i.e. string, integer, etc.)
    /// <para></para>
    /// Note: Recessed() and Border() amount to the same function.
    /// </summary>
    /// <param name="bRecessed"></param>
    /// <returns></returns>
    kwType::ckw Recessed(bool bRecessed = true);

 	/// <summary>
	/// Removes Minimize and Maximize buttons on a new window (i.e. NewWindow(), ChildWindow(), etc).  By default, the user may minimize and maximize a window. 
    /// This removes the buttons to allow minimizing and maximizing. 
    /// <para></para>
    /// ---> Use this option with NoSizing() to also remove the ability for the user to dynamically size the window by grabbing its edges.
	/// </summary>
	/// <returns></returns>
    kwType::ckw NoSysMenu(bool bNoSysMenu = true);

    /// <summary>
	/// Allows the window to be resized by the user by dragging the edge of the window and maximizing the window.
    /// <para></para>
	/// By default, once the window is created, the size cannot be changed by the user. 
    /// <para></para>
	/// When bResizeable = true, make sure the window has the canvas size to support larger sizes.  The window will not
    /// <para></para>
	/// be allowed to size greater than the original size of greater canvas set set in the program
	///
    /// </summary>
    /// <returns></returns>
    kwType::ckw Resizeable(bool bResizeable = true);

	/// <summary>
	/// Disallows sizing of a window (i.e. through NewWindow(), ChildWindow(), etc.).  By default, the user may resize the window up to the canvas size by dragging the window's edges. 
    /// <para></para>
    /// ---> When NoSizing() is used as an option, the window cannot be resized by the user (but can be resized by the program).  This does not include Minimize
    /// and Maximize controls.  Use NoSysMenu() to eliminate these options.
	/// </summary>
	/// <returns></returns>
    kwType::ckw NoSizing(bool bNoSizing = true);

    /// <summary>
	/// Disable automatic closing for dialogs and windows
    /// <para></para>
	/// When a dialog or other window is created, by default, pressing the 'X' close button in the top-right of the window
	/// will close the dialog.
    /// <para></para>
	/// NoClose() will keep the window open and set a ClosePending() flag.
	/// The OnClose() can also be intercepted in the Window Handler.
    /// </summary>
    /// <returns></returns>
    kwType::ckw NoClose(bool bNoClose = true);

    /// <summary>
	/// prevents a window from auto updating its contents (used in creating a Window).  When bNoAutoUpdate = true (and no auto updating is used), you must
	/// update the window manually with CWindow::Update() when appropriate.  
    /// <para></para>
	/// Default behavior for new windows is AutoUpdate = On, which automatically updates the window every 10ms or so as new items are drawn in the window.
    /// <para></para>
    /// --> Note that this occurs only as items are placed in the window and it is time to update. An Update() call can be used at the end of code segment
    /// that draws items to the window to ensure it is updated immediately when finished.
    /// <para></para>
    /// </summary>
    /// <returns></returns>
    kwType::ckw NoAutoUpdate(bool bNoAutoUpdate = true);

    /// <summary>
    /// Sets the AutoUpdate Type to the type specified, such as No Update, On, Immediate, OnTime, etc.
    /// </summary>
    /// <param name="eUpdateType"> = Auto Update Type</param>
    /// <returns></returns>
    kwType::ckw AutoUpdate(AutoUpdateType eUpdateType = AutoUpdateType::On);

    /// <summary>
    /// Sets the AutoUpdate Type to the type specified, such as No Update, On, Immediate, OnTime, etc.
    /// </summary>
    /// <param name="eUpdateType"> = Auto Update Type</param>
    /// <returns></returns>
    kwType::ckw AutoUpdate(const char * sAutoUpdateType);

    /// <summary>
	/// Disable the control or window from being resized by the user by grabbing the edges of the window/control.
    /// <para></para>
	/// Windows and some controls are resizeable by default.   Using NoResize() disables this when using the mouse. 
    /// The Window or Control can still be resized programmatically via the SetWindowSize() function for the specific window/control.
    /// <para></para>
    /// ** Note: NotResizeable() and NoResize() are the same function.
    /// </summary>
    /// <param name="bNotResizable"></param>
    /// <returns></returns>
    kwType::ckw NotResizable(bool bNotResizable = true);

    /// <summary>
	/// Disable the control or window from being resized by the user by grabbing the edges of the window/control.
    /// <para></para>
	/// Windows and some controls are resizeable by default.   Using NoResize() disables this when using the mouse. 
    /// The Window or Control can still be resized programmatically via the SetWindowSize() function for the specific window/control.
    /// <para></para>
    /// ** Note: NotResizeable() and NoResize() are the same function.
    /// </summary>
    /// <param name="bNotResizable"></param>
    /// <returns></returns>
    kwType::ckw NoResize(bool bNotResizable = true);

    /// <summary>
	// Doesn't add a scrollbar on a control where it may be a default (such as a Listbox).
    /// </summary>
    /// <returns></returns>
    kwType::ckw NoScrollbar(bool bNoScrollbar = true);

    /// <summary>
	/// Causes Text, Control, or Widget to have a transparent background.
    /// <para></para>
    /// ** note: This is the default on many controls and will probably become the default for all created controls.
    /// <para></para>
	/// --> This will cause the background of the parent window to show through, allowing the Text, control or widget to blend in.
    /// <para></para>
	/// --> When this option is not active, the text, control or widget will have a solid background.  For most windows, which have a solid background, this is fine.
    /// <para></para>
	/// --> However, with textures, bitmaps, and gradients, this will allow blending in a seamless manner.
    /// <para></para>
    /// ** note: For some controls (such as a TextWidget) Opacity() can be used to set the transparency level from 0-100.  When Opacity is used() Transparent()
    /// does not need to be specified.
    /// </summary>
    /// <returns></returns>
    kwType::ckw Transparent(bool bTransparent = true);

    /// <summary>
	/// Sets the (X,Y) position for functions like Write() prior to writing.
    /// <para></para>
	/// Example, cWin &lt;&lt; SetPos(100,200) &lt;&lt; "Hello World" -- Writes "Hello World" at position (100,200);
    /// <para></para>
	/// Write("Hello World",SetPos(100,200)) does the same thing
    /// <para></para>
    /// ** note: SetPos(), Pos() and GotoXY() are the same function.
    /// </summary>
    /// <param name="pLocation"></param>
    /// <returns></returns>
    kwType::ckw SetPos(POINT pLocation);

    /// <summary>
	/// Sets the (X,Y) position for functions like Write() prior to writing.
    /// <para></para>
	/// Example, cWin &lt;&lt; SetPos(100,200) &lt;&lt; "Hello World" -- Writes "Hello World" at position (100,200);
    /// <para></para>
	/// Write("Hello World",SetPos(100,200)) does the same thing
    /// <para></para>
    /// ** note: SetPos(), Pos(), and GotoXY() are the same function.
    /// </summary>
    /// <param name="pLocation"></param>
    /// <returns></returns>
    kwType::ckw SetPos(int iX,int iY);

    /// <summary>
	/// Sets the (X,Y) position for functions like Write() prior to writing.
    /// <para></para>
	/// Example, cWin &lt;&lt; GotoXY(100,200) &lt;&lt; "Hello World" -- Writes "Hello World" at position (100,200);
    /// <para></para>
	/// Write("Hello World",GotoXY(100,200)) does the same thing
    /// <para></para>
    /// ** note: SetPos() and GotoXY() are the same function.
    /// </summary>
    /// <param name="pLocation"></param>
    /// <returns></returns>
    kwType::ckw GotoXY(POINT pLocation);

    /// <summary>
	/// Sets the (X,Y) position for functions like Write() prior to writing.
    /// <para></para>
	/// Example, cWin &lt;&lt; SetPos(100,200) &lt;&lt; "Hello World" -- Writes "Hello World" at position (100,200);
    /// <para></para>
	/// Write("Hello World",SetPos(100,200)) does the same thing
    /// <para></para>
    /// ** note: SetPos(), Pos(), and GotoXY() are the same function.
    /// </summary>
    /// <param name="pLocation"></param>
    /// <returns></returns>
    kwType::ckw Pos(POINT pLocation);

    /// <summary>
	/// Sets the (X,Y) position for functions like Write() prior to writing.
    /// <para></para>
	/// Example, cWin &lt;&lt; GotoXY(100,200) &lt;&lt; "Hello World" -- Writes "Hello World" at position (100,200);
    /// <para></para>
	/// Write("Hello World",GotoXY(100,200)) does the same thing
    /// <para></para>
    /// ** note: SetPos() and GotoXY() are the same function.
    /// </summary>
    /// <param name="pLocation"></param>
    /// <returns></returns>
    kwType::ckw Pos(int iX,int iY);

    /// <summary>
	/// Sets the (X,Y) position for functions like Write() prior to writing.
    /// <para></para>
	/// Example, cWin &lt;&lt; GotoXY(100,200) &lt;&lt; "Hello World" -- Writes "Hello World" at position (100,200);
    /// <para></para>
	/// Write("Hello World",GotoXY(100,200)) does the same thing
    /// <para></para>
    /// ** note: SetPos() and GotoXY() are the same function.
    /// </summary>
    /// <param name="pLocation"></param>
    /// <returns></returns>
    kwType::ckw GotoXY(int iX,int iY);


    /// <summary>
	/// Sends a literal string to the Window, Control, or Widget to set options that may not exist int the keyword (kw) namespace
    /// <para></para>
	/// For example, MyWiget(100,200,kw::Transparent() | kw::Str("WidgetOption=Value") can tell the widget to perform an action
	/// that cannot be done through kw() functions.
    /// <para></para>
    /// The string can be later retreived with ckw::Get(keys._Str); 
    /// </summary>
    /// <returns></returns>
    kwType::ckw Str(const char * sString);

    /// <summary>
	/// TabPos() Can be used in a console input function (i.e. console.GetInput(), console.getFloat(), etc.) to set the position of the 
	/// input box or text start.  
    /// <para></para>
    /// TabPos sets (average character width of font)*size entered in the TabPos.  
    /// <para></para>
    /// --> For example, TabPos(50) sets the X position at X = 50 aerage characters for the font.
    /// <para></para>
	/// This allows the boxes to be easily lined up with text to the left and aligned boxes to the right.
    /// <para></para>
    /// ** note:  In write functions putting {x=number} (i.e. {x=40} sets the X position for the text}
    /// </summary>
    /// <returns></returns>
    kwType::ckw TabPos(int iTabPos); 

    /// <summary>
    /// Tab() is currently being developed.  TBD
    /// </summary>
    /// <param name="iTab"></param>
    /// <returns></returns>
    kwType::ckw Tab(int iTab); 

    /// <summary>
	/// XPos() Can be used in a console input function (i.e. console.GetInput(), console.getFloat(), etc.) to set the position of the 
	/// input box.  
    /// <para></para>
    /// This allows the boxes to be easily lined up with text to the left and aligned boxes to the right.
    /// <para>** note: XPos and SetXPos() are the same function</para>
    /// </summary>
    /// <returns></returns>
    kwType::ckw XPos(int iXPos); 

    /// <summary>
	// Add X pixels to the size or placement of a control or other object.
    /// <para></para>
	/// Example: CreateButton(10,50,"OK",PadX(20)) will add 20 pixels to the size of the button to make the button wider with "OK" in the center.
    /// <para></para>
    /// ** note: PadX() and XOffset() are the same function
    /// </summary>
    /// <returns></returns>
    kwType::ckw PadX(int iPadX); 

    /// <summary>
	/// Add Y pixels to the size or placement of a control or other object.
    /// <para></para>
	/// Example: CreateButton(10,50,"OK",PadY(20)) will add 20 pixels to the vertical size of the button to make the button taller with "OK" in the center.
    /// <para></para>
	/// PadY() only works with certain controls.  For example, standard buttons work with PadX() but not PadY(), where
	/// Panel Buttons and Windows Buttons (i.e. Style("Panel") and Style("Windows") respond do the PadY(), though 
	/// specifiying an explicit width and height is a better option.
    /// <para></para>
    /// ** note: PadY() and YOffset() are the same function
    /// </summary>
    /// <returns></returns>
    kwType::ckw PadY(int iPadY); 

    /// <summary>
	/// Sets the Control, Window, Widget or other object to the bottom of the window.
    /// <para></para>
	/// --> Use PadY() to set a vertical offset from this position.
    /// <para></para>
	/// --> Use PadX() to set a horizontal offset from this position.
    ///  </summary>
    /// <returns></returns>
    kwType::ckw JustBottom(bool bJustBottom = true);

    /// <summary>
	/// Sets the Control, Window, Widget or other object to the top of the window.
    /// <para></para>
	/// --> Use PadY() to set a vertical offset from this position.
    /// <para></para>
	/// --> Use PadX() to set a horizontal offset from this position.
    /// </summary>
    /// <returns></returns>
    kwType::ckw JustTop(bool bJustTop = true);

    /// <summary>
	/// Sets the Control, Window, Widget or other object to the bottom righmost part of the window.
    /// <para></para>
	/// --> Use PadY() to set a vertical offset from this position.
    /// <para></para>
	/// --> Use PadX() to set a horizontal offset from this position.
    /// </summary>
    /// <returns></returns>
    kwType::ckw JustRight(bool bJustRight = true);

    ///  <summary>
	/// Sets the Control, Window, Widget or other object to the horizontal center of the window.
    /// <para></para>
	/// --> Add PadY() to set a vertical offset from this position.
    /// <para></para>
	/// --> Add PadX() to set a horizontal offset from this position.
    /// </summary>
    /// <returns></returns>
    kwType::ckw JustCenter(bool bJustCenter = true);

    /// <summary>
	/// Sets the Control, Window, Widget or other object to the left in the window.
    /// <para></para>
	/// --> Use PadY() to set a vertical offset from this position.
    /// <para></para>
	/// --> Use PadX() to set a horizontal offset from this position.
    /// </summary>
    /// <returns></returns>
    kwType::ckw JustLeft(bool bJustLeft = true);

    ///  <summary>
	/// Sets the Control, Window, Widget or other object to the top-left of the window.
    /// <para></para>
	/// --> Use PadY() to set a vertical offset from this position.
    /// <para></para>
	/// --> Use PadX() to set a horizontal offset from this position.
    ///  </summary>
    ///  <returns></returns>
    kwType::ckw JustTopLeft(bool bJustTopLeft = true);

    /// <summary>
	/// Sets the Control, Window, Widget or other object to the top-right of the window.
    /// <para></para>
	/// --> Use PadY() to set a vertical offset from this position.
    /// <para></para>
	/// --> Use PadX() to set a horizontal offset from this position.
    /// </summary>
    /// <returns></returns>
    kwType::ckw JustTopRight(bool bJustTopRight = true);

    /// <summary>
	/// Sets the Control, Window, Widget or other object to the top-center of the window.
    /// <para></para>
	/// --> Use PadY() to set a vertical offset from this position.
    /// <para></para>
	/// --> Use PadX() to set a horizontal offset from this position.
    /// </summary>
    /// <returns></returns>
    kwType::ckw JustTopCenter(bool bJustTopCenter = true);

    /// <summary>
	/// Sets the Control, Window, Widget or other object to the bottom-left of the window.
    /// <para></para>
	/// --> Use PadY() to set a vertical offset from this position.
    /// <para></para>
	/// --> Use PadX() to set a horizontal offset from this position.
    /// </summary>
    /// <returns></returns>
    kwType::ckw JustBottomLeft(bool bJustBottomLeft = true);

    /// <summary>
	/// Sets the Control, Window, Widget or other object to the bottom-right of the window.
    /// <para></para>
	/// --> Use PadY() to set a vertical offset from this position.
    /// <para></para>
	/// --> Use PadX() to set a horizontal offset from this position.
    /// </summary>
    /// <returns></returns>
    kwType::ckw JustBottomRight(bool bJustBottomRight = true);

    /// <summary>
	/// Sets the Control, Window, Widget or other object to the bottom-center of the window.
    /// <para></para>
	/// --> Use PadY() to set a vertical offset from this position.
    /// <para></para>
	/// --> Use PadX() to set a horizontal offset from this position.
    /// </summary>
    /// <returns></returns>
    kwType::ckw JustBottomCenter(bool bJustBottomCenter = true);


    /// <summary>
	/// Sets the Control, Window, Widget or other object to the horizontal center of the window.
    /// <para></para>
	/// --> Use PadY() to set a vertical offset from this position.
    /// <para></para>
	/// --> Use PadX() to set a horizontal offset from this position.
    /// <para></para>
    /// **note : CenterX(), Center(), JustCenterX() and JustCenter() the same function
    /// </summary>
    /// <returns></returns>
    kwType::ckw Center(bool bCenter = true);

    /// <summary>
	/// Sets the Control, Window, Widget or other object to the horizontal center of the window.
    /// <para></para>
	/// --> Use PadY() to set a vertical offset from this position.
    /// <para></para>
	/// --> Use PadX() to set a horizontal offset from this position.
    /// <para></para>
    /// **note : CenterX(), Center(), JustCenterX() and JustCenter() the same function
    /// </summary>
    /// <returns></returns>
    kwType::ckw CenterX(bool bCenter = true);

    /// <summary>
	/// Sets the Control, Window, Widget or other object to the vertical center of the window.
    /// <para></para>
	/// --> Use PadY() to set a vertical offset from this position.
    /// <para></para>
	/// --> Use PadX() to set a horizontal offset from this position.
    /// <para></para>
    /// **note : CenterY() and JustCenterY() are the same function
    /// </summary>
    /// <returns></returns>
    kwType::ckw CenterY(bool bCenter = true);


    /// <summary>
	/// Sets the text in Control, Window, Widget or other object to the horizontal center of the object (i.e. window, dialog, etc.).
    /// <para></para>
	/// --> Use PadY() to set a vertical offset from this position.
    /// <para></para>
	/// --> Use PadX() to set a horizontal offset from this position.
    /// </summary>
    /// <para></para>
    /// **note : TextCenter() and TextCenterX() are the same function
    /// <returns></returns>
    kwType::ckw TextCenter(bool bTextCenter = true);

    /// <summary>
	/// Sets the text in Control, Window, Widget or other object to the horizontal center of the object (i.e. window, dialog, etc.).
    /// <para></para>
	/// --> Use PadY() to set a vertical offset from this position.
    /// <para></para>
	/// --> Use PadX() to set a horizontal offset from this position.
    /// <para></para>
    /// **note : TextCenter() and TextCenterX() are the same function
    /// </summary>
    /// <returns></returns>
    kwType::ckw TextCenterX(bool bTextCenterX = true);


    /// <summary>
	/// Sets the text in Control, Window, Widget or other object to the vertucal center of the object (i.e. window, dialog, etc.).
    /// <para></para>
	/// --> Use PadY() to set a vertical offset from this position.
    /// <para></para>
	/// --> Use PadX() to set a horizontal offset from this position.
    /// </summary>
    /// <returns></returns>
    kwType::ckw TextCenterY(bool bTextCenterY = true);

    /// <summary>
	/// Sets the text in Control, Window, Widget or other object to the vertucal center of the object (i.e. window, dialog, etc.).
    /// <para></para>
	/// --> Use PadY() to set a vertical offset from this position.
    /// <para></para>
	/// --> Use PadX() to set a horizontal offset from this position.
    /// </summary>
    /// <returns></returns>
    kwType::ckw CenterXY(bool bCenterXY = true);

    /// <summary>
	/// Sets the Control, Window, Widget or other object to the horizontal center of the window.
    /// <para></para>
	/// --> Use PadY() to set a vertical offset from this position.
    /// <para></para>
	/// --> Use PadX() to set a horizontal offset from this position.
    /// <para></para>
    /// **note : CenterX(), Center(), JustCenterX() and JustCenter() the same function
    /// </summary>
    /// <returns></returns>
    kwType::ckw JustCenterX(bool bJustCenterX = true);

    /// <summary>
	/// Sets the Control, Window, Widget or other object to the vertical center of the window.
    /// <para></para>
	/// --> Use PadY() to set a vertical offset from this position.
    /// <para></para>
	/// --> Use PadX() to set a horizontal offset from this position.
    /// <para></para>
    /// **note : CenterY() and JustCenterY() are the same function
    /// </summary>
    /// <returns></returns>
    kwType::ckw JustCenterY(bool bJustCenterY = true);

    /// <summary>
	/// Sets  Windows OS colors for the control, window, etc., overriding default colors.
    /// <para></para>
	/// ** note:  This conrol is a little TBD. 
    /// <para></para>
    /// --> This typically sets 'Windows OS colors' for a control overriding the default colors which are based on
    /// <para></para>
    /// the current background and foreground colors in the parent window.
    /// <para></para>
    /// --> This usually means that it will set the bacground to white and foreground to black, overriding current colors.
    /// <para></para>
    /// --> This is useful for items with backgrounds, such as ListBoxes, Edit Boxes, Comboboxes, etc.
    /// </summary>
    /// <returns></returns>
    kwType::ckw WinColors(bool bWinColors = true);      

    /// <summary>
	/// Set the color for the Value display in a slider.
    /// <para></para>
    /// --> By default, the value color is the foreground/text color of the current window.
    /// <para></para>
    /// --> Using ValueColor() sets the value display color specifically.
    /// <para></para>
    /// --> Rgb Colors (i.e. PanColor::ForestGreen, SageColor::Red, etc.) can be used
    /// <para></para>
    /// --> Strings can be used, such as "ForestGreen", "Red", etc. 
    /// </summary>
    /// <returns></returns>
    kwType::ckw ValueColor(RgbColor rgbColor);

    /// <summary>
	/// Set the color for the Value display in a slider.
    /// <para></para>
    /// --> By default, the value color is the foreground/text color of the current window.
    /// <para></para>
    /// --> Using ValueColor() sets the value display color specifically.
    /// <para></para>
    /// --> Rgb Colors (i.e. PanColor::ForestGreen, SageColor::Red, etc.) can be used
    /// <para></para>
    /// --> Strings can be used, such as "ForestGreen", "Red", etc. 
    /// </summary>
    /// <returns></returns>
    kwType::ckw ValueColor(const char * sValueColor);

    /// <summary>
    /// Set a Minimum value allowed (for integer or floating-point).  
    /// <para></para>
    /// When this value is exceeded (i.e. too low), a dialog box appears with min/max info if a value outisde of the range is entered, preventing the user from
    /// continuing until a proper value is entered.
    /// <para></para>
	/// --> Example:  GetFloat(fDefault,MinValue(0)); 
    /// <para></para>
    /// --> Also see: MaxValue() and Range() functions.  Range() allows entering a minimum and maximum value together.
    /// <para></para>
    /// --> MinValue() is same value as the first value entered using Range().
    /// </summary>
    /// <param name="sValueColor"></param>
    /// <returns></returns>    
    kwType::ckw MinValue(int iMinValue);
    
    /// <summary>
    /// Set a Minimum value allowed (for integer or floating-point).  
    /// <para></para>
    /// When this value is exceeded (i.e. too low), a dialog box appears with min/max info if a value outisde of the range is entered, preventing the user from
    /// continuing until a proper value is entered.
    /// <para></para>
	/// --> Example:  GetFloat(fDefault,MinValue(0)); 
    /// <para></para>
    /// --> Also see: MaxValue() and Range() functions.  Range() allows entering a minimum and maximum value together.
    /// <para></para>
    /// --> MinValue() is same value as the first value entered using Range().
    /// </summary>
    /// <param name="sValueColor"></param>
    /// <returns></returns>    
    kwType::ckw MinValue(float fMinValue);

    /// <summary>
    /// Set a Minimum value allowed (for integer or floating-point).  
    /// <para></para>
    /// When this value is exceeded (i.e. too low), a dialog box appears with min/max info if a value outisde of the range is entered, preventing the user from
    /// continuing until a proper value is entered.
    /// <para></para>
	/// --> Example:  GetFloat(fDefault,MinValue(0)); 
    /// <para></para>
    /// --> Also see: MaxValue() and Range() functions.  Range() allows entering a minimum and maximum value together.
    /// <para></para>
    /// --> MinValue() is same value as the first value entered using Range().
    /// </summary>
    /// <param name="sValueColor"></param>
    /// <returns></returns>
    kwType::ckw MinValue(double fMinValue);

    /// <summary>
    /// Set a Maximum value allowed (for integer or floating-point).  
    /// <para></para>
    /// When this value is exceeded (i.e. too high), a dialog box appears with min/max info if a value outisde of the range is entered, preventing the user from
    /// continuing until a proper value is entered.
    /// <para></para>
	/// --> Example:  GetFloat(fDefault,MaxValue(1000)); 
    /// <para></para>
    /// --> Also see: MaxValue() and Range() functions.  Range() allows entering a minimum and maximum value together.
    /// <para></para>
    /// --> MaxValue() is same value as the second value entered using Range().
    /// </summary>
    /// <param name="sValueColor"></param>
    /// <returns></returns>
    kwType::ckw MaxValue(int iMaxValue);

    /// <summary>
    /// Set a Maximum value allowed (for integer or floating-point).  
    /// <para></para>
    /// When this value is exceeded (i.e. too high), a dialog box appears with min/max info if a value outisde of the range is entered, preventing the user from
    /// continuing until a proper value is entered.
    /// <para></para>
	/// --> Example:  GetFloat(fDefault,MaxValue(1000)); 
    /// <para></para>
    /// --> Also see: MaxValue() and Range() functions.  Range() allows entering a minimum and maximum value together.
    /// <para></para>
    /// --> MaxValue() is same value as the second value entered using Range().
    /// </summary>
    /// <param name="sValueColor"></param>
    /// <returns></returns>
    kwType::ckw MaxValue(float fMaxValue);

    /// <summary>
    /// Set a Maximum value allowed (for integer or floating-point).  
    /// <para></para>
    /// When this value is exceeded (i.e. too high), a dialog box appears with min/max info if a value outisde of the range is entered, preventing the user from
    /// continuing until a proper value is entered.
    /// <para></para>
	/// --> Example:  GetFloat(fDefault,MaxValue(1000)); 
    /// <para></para>
    /// --> Also see: MaxValue() and Range() functions.  Range() allows entering a minimum and maximum value together.
    /// <para></para>
    /// --> MaxValue() is same value as the second value entered using Range().
    /// </summary>
    /// <param name="sValueColor"></param>
    /// <returns></returns>
    kwType::ckw MaxValue(double fMaxValue);

    /// <summary>
    /// Sets the default display and return value of a control such as an edit box, or any other object that has an input for a string or number.
    /// <para></para>
    /// --> Example: GetNumber("Enter a number",Default(1234)); 
    /// <para></para>
    /// --> Example: DevInputBox("Enter a string",Default("1234"This is the default string")); 
    /// <para></para>
    /// Default values can be integers, floating point, or strings.
    /// </summary>
    /// <returns></returns>
    kwType::ckw Default(int iDefault);

    /// <summary>
    /// Sets the default display and return value of a control such as an edit box, or any other object that has an input for a string or number.
    /// <para></para>
    /// --> Example: GetNumber("Enter a number",Default(1234)); 
    /// <para></para>
    /// --> Example: DevInputBox("Enter a string",Default("1234"This is the default string")); 
    /// <para></para>
    /// Default values can be integers, floating point, or strings.
    /// </summary>
    /// <returns></returns>
    kwType::ckw Default(float fDefault);

    /// <summary>
    /// Sets the default display and return value of a control such as an edit box, or any other object that has an input for a string or number.
    /// <para></para>
    /// --> Example: GetNumber("Enter a number",Default(1234)); 
    /// <para></para>
    /// --> Example: DevInputBox("Enter a string",Default("1234"This is the default string")); 
    /// <para></para>
    /// Default values can be integers, floating point, or strings.
    /// </summary>
    /// <returns></returns>
    kwType::ckw Default(double fDefault);

    /// <summary>
    /// Sets the default display and return value of a control such as an edit box, or any other object that has an input for a string or number.
    /// <para></para>
    /// --> Example: GetNumber("Enter a number",Default(1234)); 
    /// <para></para>
    /// --> Example: DevInputBox("Enter a string",Default("1234"This is the default string")); 
    /// <para></para>
    /// Default values can be integers, floating point, or strings.
    /// </summary>
    /// <returns></returns>
    kwType::ckw Default(const char * sDefault);

    /// <summary>
    /// Sets the range of a control or object that can use both a minimum and maximum value.
    /// <para></para>
    /// --> Range sets the minimum and maximum value for the control.
    /// <para></para>
    /// --> Values can be floating-point or integer.
    /// <para></para>
    /// --> Entered values can be X,Y format.
    /// <para></para>
    /// --> Entered values can be POINT, CPoint, CfPoint (double) or CPointf (float) paired values.
    /// <para></para>
    /// --> The range values are the same as using MinValue() and MaxValue() functions separately.
    /// <para></para>
    /// Example: GetFloat("Enter a value",Range(-100,100)); 
    /// </summary>
    /// <returns></returns>
    kwType::ckw Range(int iMin,int iMax);   

    /// <summary>
    /// Sets the range of a control or object that can use both a minimum and maximum value.
    /// <para></para>
    /// --> Range sets the minimum and maximum value for the control.
    /// <para></para>
    /// --> Values can be floating-point or integer.
    /// <para></para>
    /// --> Entered values can be X,Y format.
    /// <para></para>
    /// --> Entered values can be POINT, CPoint, CfPoint (double) or CPointf (float) paired values.
    /// <para></para>
    /// --> The range values are the same as using MinValue() and MaxValue() functions separately.
    /// <para></para>
    /// Example: GetFloat("Enter a value",Range(-100,100)); 
    /// </summary>
    /// <returns></returns>
    kwType::ckw Range(float fMin,float fMax);     

    /// <summary>
    /// Sets the range of a control or object that can use both a minimum and maximum value.
    /// <para></para>
    /// --> Range sets the minimum and maximum value for the control.
    /// <para></para>
    /// --> Values can be floating-point or integer.
    /// <para></para>
    /// --> Entered values can be X,Y format.
    /// <para></para>
    /// --> Entered values can be POINT, CPoint, CfPoint (double) or CPointf (float) paired values.
    /// <para></para>
    /// --> The range values are the same as using MinValue() and MaxValue() functions separately.
    /// <para></para>
    /// Example: GetFloat("Enter a value",Range(-100,100)); 
    /// <para></para>
    /// ** note: See Rangef() to explicitly set a floating-point range.
    /// </summary>
    /// <returns></returns>
    kwType::ckw Range(double fMin,double fMax);     

    /// <summary>
    /// Sets the range of a control or object that can use both a minimum and maximum value.
    /// <para></para>
    /// --> Range sets the minimum and maximum value for the control.
    /// <para></para>
    /// --> Values can be floating-point or integer.
    /// <para></para>
    /// --> Entered values can be X,Y format.
    /// <para></para>
    /// --> Entered values can be POINT, CPoint, CfPoint (double) or CPointf (float) paired values.
    /// <para></para>
    /// --> The range values are the same as using MinValue() and MaxValue() functions separately.
    /// <para></para>
    /// Example: GetFloat("Enter a value",Range(-100,100)); 
    /// <para></para>
    /// ** note: See Rangef() to explicitly set a floating-point range.
    /// </summary>
    /// <returns></returns>
    kwType::ckw Range(POINT pRange);               

    /// <summary>
    /// Sets the range of a control or object that can use both a minimum and maximum value.
    /// <para></para>
    /// --> Range sets the minimum and maximum value for the control.
    /// <para></para>
    /// --> Values can be floating-point or integer.
    /// <para></para>
    /// --> Entered values can be X,Y format.
    /// <para></para>
    /// --> Entered values can be POINT, CPoint, CfPoint (double) or CPointf (float) paired values.
    /// <para></para>
    /// --> The range values are the same as using MinValue() and MaxValue() functions separately.
    /// <para></para>
    /// Example: GetFloat("Enter a value",Range(-100,100)); 
    /// <para></para>
    /// ** note: See Rangef() to explicitly set a floating-point range.
    /// </summary>
    /// <returns></returns>
    kwType::ckw Range(CPoint pRange);               

    /// <summary>
    /// Sets the range of a control or object that can use both a minimum and maximum value.
    /// <para></para>
    /// --> Range sets the minimum and maximum value for the control.
    /// <para></para>
    /// --> Values can be floating-point or integer.
    /// <para></para>
    /// --> Entered values can be X,Y format.
    /// <para></para>
    /// --> Entered values can be POINT, CPoint, CfPoint (double) or CPointf (float) paired values.
    /// <para></para>
    /// --> The range values are the same as using MinValue() and MaxValue() functions separately.
    /// <para></para>
    /// Example: GetFloat("Enter a value",Range(-100,100)); 
    /// <para></para>
    /// ** note: See Rangef() to explicitly set a floating-point range.
    /// </summary>
    /// <returns></returns>
    kwType::ckw Range(CfPointf pRange);              

    /// <summary>
    /// Sets the range of a control or object that can use both a minimum and maximum value.
    /// <para></para>
    /// --> Range sets the minimum and maximum value for the control.
    /// <para></para>
    /// --> Values can be floating-point or integer.
    /// <para></para>
    /// --> Entered values can be X,Y format.
    /// <para></para>
    /// --> Entered values can be POINT, CPoint, CfPoint (double) or CPointf (float) paired values.
    /// <para></para>
    /// --> The range values are the same as using MinValue() and MaxValue() functions separately.
    /// <para></para>
    /// Example: GetFloat("Enter a value",Range(-100,100)); 
    /// <para></para>
    /// ** note: See Rangef() to explicitly set a floating-point range.
    /// </summary>
    /// <returns></returns>
    kwType::ckw Range(CfPoint pRange);               

    /// <summary>
    /// Sets the range of a control or object that can use both a minimum and maximum value, explicitly setting floating-point values.
    /// <para></para>
    /// --> Range sets the minimum and maximum value for the control.
    /// <para></para>
    /// --> Values can be floating-point or integer.
    /// <para></para>
    /// --> Entered values can be float (x,y), double (x,y), CfPoint (double) or CPointf (float) paired values.
    /// <para></para>
    /// --> The range values are the same as using MinValue() and MaxValue() functions separately.
    /// <para></para>
    /// Example: GetFloat("Enter a value",Rangef(-100.0,100.0)); 
    /// <para></para>
    /// ** note: See Range() to set both integer and float values for a range.
    /// </summary>
    /// <returns></returns>
    kwType::ckw Rangef(float fMin,float fMax);     

    /// <summary>
    /// Sets the range of a control or object that can use both a minimum and maximum value, explicitly setting floating-point values.
    /// <para></para>
    /// --> Range sets the minimum and maximum value for the control.
    /// <para></para>
    /// --> Values can be floating-point or integer.
    /// <para></para>
    /// --> Entered values can be float (x,y), double (x,y), CfPoint (double) or CPointf (float) paired values.
    /// <para></para>
    /// --> The range values are the same as using MinValue() and MaxValue() functions separately.
    /// <para></para>
    /// Example: GetFloat("Enter a value",Rangef(-100.0,100.0)); 
    /// <para></para>
    /// ** note: See Range() to set both integer and float values for a range.
    /// </summary>
    /// <returns></returns>
    kwType::ckw Rangef(double fMin,double fMax);     

    /// <summary>
    /// Sets the range of a control or object that can use both a minimum and maximum value, explicitly setting floating-point values.
    /// <para></para>
    /// --> Range sets the minimum and maximum value for the control.
    /// <para></para>
    /// --> Values can be floating-point or integer.
    /// <para></para>
    /// --> Entered values can be float (x,y), double (x,y), CfPoint (double) or CPointf (float) paired values.
    /// <para></para>
    /// --> The range values are the same as using MinValue() and MaxValue() functions separately.
    /// <para></para>
    /// Example: GetFloat("Enter a value",Rangef(-100.0,100.0)); 
    /// <para></para>
    /// ** note: See Range() to set both integer and float values for a range.
    /// </summary>
    /// <returns></returns>
    kwType::ckw Rangef(CfPointf pRange);              

    /// <summary>
    /// Sets the range of a control or object that can use both a minimum and maximum value, explicitly setting floating-point values.
    /// <para></para>
    /// --> Range sets the minimum and maximum value for the control.
    /// <para></para>
    /// --> Values can be floating-point or integer.
    /// <para></para>
    /// --> Entered values can be float (x,y), double (x,y), CfPoint (double) or CPointf (float) paired values.
    /// <para></para>
    /// --> The range values are the same as using MinValue() and MaxValue() functions separately.
    /// <para></para>
    /// Example: GetFloat("Enter a value",Rangef(-100.0,100.0)); 
    /// <para></para>
    /// ** note: See Range() to set both integer and float values for a range.
    /// </summary>
    /// <returns></returns>
    kwType::ckw Rangef(CfPoint pRange);               

    /// <summary>
    /// Sets a Group Name or ID (integer or string) for a control, such as a button group that can be used later to refer to all controls in the group.
    /// <para></para>
    /// Example:  CreateRadioButton(10,50,"MyButton",Group("MyButtonGroup))
    /// <para></para>
    /// Example:  CreateRadioButton(10,50,"MyButton",Group(1234))
    /// <para></para>
    /// ** note: using GroupID() and Group(&lt;integer value&gt;) are the same function.
    /// </summary>
    /// <returns></returns>
    kwType::ckw Group(const char * sGroup);   

    /// <summary>
    /// Sets a Group Name or ID (integer or string) for a control, such as a button group that can be used later to refer to all controls in the group.
    /// <para></para>
    /// Example:  CreateRadioButton(10,50,"MyButton",Group("MyButtonGroup))
    /// <para></para>
    /// Example:  CreateRadioButton(10,50,"MyButton",Group(1234))
    /// <para></para>
    /// ** note: using GroupID() and Group(&lt;integer value&gt;) are the same function.
    /// </summary>
    /// <returns></returns>
    kwType::ckw Group(int iGroup);   

    /// <summary>
    /// Sets a Group ID (integer value) for a control, such as a button group that can be used later to refer to all controls in the group.
    /// <para></para>
    /// Example:  CreateRadioButton(10,50,"MyButton",Group(1234))
    /// <para></para>
    /// ** note: using GroupID() and Group(&lt;integer value&gt;) are the same function.
    /// </summary>
    /// <returns></returns>
    kwType::ckw GroupID(int iGroupID); 

    /// <summary>
    /// Sets a horizontal orientation for sliders, radio and checkbox groups, and other controls.
    /// <para></para>
    /// --> Sets a horizontal orientation for sliders (default);
    /// <para></para>
    /// --> Sets a horizontal grouping for radio button and checkbox groups (default is vertical)
    /// <para></para>
    /// ** note: Horz() and Horizonal() are the same function.
    /// </summary>
    /// <param name="bHorizontal"></param>
    /// <returns></returns>
    kwType::ckw Horizontal(bool bHorizontal = true);      

    /// <summary>
    /// Sets a horizontal orientation for sliders, radio and checkbox groups, and other controls.
    /// <para></para>
    /// --> Sets a horizontal orientation for sliders (default);
    /// <para></para>
    /// --> Sets a horizontal grouping for radio button and checkbox groups (default is vertical)
    /// <para></para>
    /// ** note: Horz() and Horizonal() are the same function.
    /// </summary>
    /// <param name="bHorizontal"></param>
    /// <returns></returns>
    kwType::ckw Horz(bool bHorizontal = true);   

    /// <summary>
    /// Sets a vertical orientation for sliders, radio and checkbox groups, and other controls.
    /// <para></para>
    /// --> Sets a vertical orientation for sliders (default is horizontal);
    /// <para></para>
    /// --> Sets a vertical grouping for radio button and checkbox groups (default)
    /// <para></para>
    /// ** note: Vert() and Vertical() are the same function.
    /// </summary>
    /// <param name="bHorizontal"></param>
    /// <returns></returns>
    kwType::ckw Vertical(bool bVertical = true);      

    /// <summary>
    /// Sets a vertical orientation for sliders, radio and checkbox groups, and other controls.
    /// <para></para>
    /// --> Sets a vertical orientation for sliders (default is horizontal);
    /// <para></para>
    /// --> Sets a vertical grouping for radio button and checkbox groups (default)
    /// <para></para>
    /// ** note: Vert() and Vertical() are the same function.
    /// </summary>
    /// <param name="bHorizontal"></param>
    /// <returns></returns>
    kwType::ckw Vert(bool bVertical = true);      

    /// <summary>
    /// Shows or Hides a value in a slider.  Default is to show the value.
    /// <para></para>
    /// When bShowValue = false, the value of the slider is not shown, allowing for a more figurative slider rather than displaying explicit values.
    /// <para></para>
    /// --> Example:  DevSlider("This is a slider",ShowValue(false)); 
    /// </summary>
    /// <param name="bShowValue"></param>
    /// <returns></returns>
    kwType::ckw ShowValue(bool bShowValue = true);  

    /// <summary>
    /// Style() sets the style for buttons, windows, and other controls.  This function is control-dependent, and still in development.
    /// <para></para>
	/// Styles can be pre-defined styles or ones created through loading graphics and setting the style with Sagebox, such as SetButtonStyle()
    /// <para></para>
	/// Various Examples:
    /// <para></para>
	/// CreateButton(50,50,"Ok",Style("medium")  -- sets a larger button
    /// <para></para>
	/// CreateButton(50,50,"Ok",Style("medium:red")  -- sets a larger button in red (where gray, blue, red, light gray, dark, are pre-defined styles)
    /// <para></para>
	/// CreateButton(50,50,"Ok",Style("Panel")  -- sets a Panel button
    /// <para></para>
	/// CreateButton(50,50,"Ok",Style("Windows")  -- sets a Windows-style button
    /// <para></para>
    /// </summary>
    /// <returns></returns>
    kwType::ckw Style(const char * sStyle);   

    /// <summary>
    /// Disables a control when initially created.  When disabled, a control is greyed-out and will not accept input.
    /// <para></para>
    /// --> Use the control's Enable() (or Disable(false)) function to enable/disable manually. (i.e. mySlider.Enable())
    /// <para></para>
    /// --> Example: DevSlider("This is a slider",Range(-100,100) &lt;&lt; Disabled()) -- set slider as initially disabled.
    /// </summary>
    /// <returns></returns>
    kwType::ckw Disabled(bool bDisabled = true);  

	// Adds a title to a control, such as a slider, input dialog title bard, and other controls.
	//
	// Without a title, many controls/dialog show the ProgramName() in the title bar of the program name is set.
	// Otherwise, this value is left blank or set to a default that is descriptive of the control.
	//
	// Title() and Label() are the same.
	//

    /// <summary>
	/// Adds a title to a control, such as a slider, and other controls.
    /// <para></para>
    /// --> For most functions, Label() and Title() are the same function.
    /// <para></para>
    /// --> For sliders, Label() adds a label underneath the slider.
    /// <para></para>
	/// Without a title, many controls/dialog show a default in the title bar of the program name is set.
    /// <para></para>
	/// Otherwise, this value is left blank or set to a default that is descriptive of the control.
    /// <para></para>
	/// **note: Also See: Title().
    /// </summary>
    /// <param name="sLabel"></param>
    /// <returns></returns>
    kwType::ckw Label(const char * sLabel);

    /// <summary>
    ///  Value Font is deprecated.  It was used to set the font for a value display in the slider.  
    ///  This has been deprecated, and the label font is now used.
    /// </summary>
    /// <param name="iValueFont"></param>
    /// <returns></returns>
    kwType::ckw ValueFont(int iValueFont);

    /// <summary>
    ///  Value Font is deprecated.  It was used to set the font for a value display in the slider.  
    ///  This has been deprecated, and the label font is now used.
    /// </summary>
    /// <returns></returns>
    kwType::ckw ValueFont(HFONT hfont);


    /// <summary>
    ///  Value Font is deprecated.  It was used to set the font for a value display in the slider.  
    ///  This has been deprecated, and the label font is now used.
    /// </summary>
    /// <param name="iValueFont"></param>
    /// <returns></returns>
    kwType::ckw ValueFont(float fValueFont);

    /// <summary>
    ///  Value Font is deprecated.  It was used to set the font for a value display in the slider.  
    ///  This has been deprecated, and the label font is now used.
    /// </summary>
    /// <param name="iValueFont"></param>
    /// <returns></returns>
    kwType::ckw ValueFont(double fValueFont);

    /// <summary>
    ///  Value Font is deprecated.  It was used to set the font for a value display in the slider.  
    ///  This has been deprecated, and the label font is now used.
    /// </summary>
    /// <param name="iValueFont"></param>
    /// <returns></returns>
    kwType::ckw ValueFont(const char * sValueFont);

	/// <summary>
	/// Allows rollover for slider values.
    /// <para></para>
    /// By default, if SetPos() is called or the Mousewheel is moved, and the value exceeds the minimum or maximum range value, the 
    /// value will rollover to the minimum and maxmimum values, respectively, keeping the value within the slider's range.
    /// <para></para>
    /// --> When AllowRollover() is used, and SetPos() is called with a value below the minimum or above the maximum range value,
    /// the value will rollover in the slider.
    /// <para></para>
    /// --> Rollover only works with SetPos() and using the MouseWheel to adjust the slider position.  Manually moving the slider or pressing ctrl+Mousewheel for fine 
    /// control does not allow for rollover.
    /// <para></para>
    /// --> note: This currently only works with sliders that have integer ranges (default slider behavior, i.e. non-floating-point ranges), and where the minimum range value is less
    /// then the maxmimum range value (i.e. ranges of (-50,50) or (0,100) will allow rollover, where reversed ranges such as (100,-100) will not rollover).
	/// </summary>
	/// <param name="bAllowRollover"> - When set to true (default value for this function call), rollover behavior is enabled.  When false (default slider behavior when created), rollover is not allowed.</param>
	/// <returns></returns>
    kwType::ckw AllowRollover(bool bAllowRollover = true);     


	/// <summary>
	/// For Sliders, AsFloat() declares the Slider as a floating-point slider with a default range of 0.0-1.0. <para></para>
    /// <para></para>
    /// --> Use the slider funtions GetPosf(), SetPosf(), SetRangef() for floating-point functions. <para></para>
    /// <para></para>
    /// --> Use Range() or Rangef() in slider creation to set a range. <para></para>
    /// <para></para>
    /// --> Use slider.SetRangef() to set a specific floating-point range after the slider is created. <para></para>
    /// <para></para>
    /// ** note: NewSliderf() can be used instead of specifying AsFloat.   Use opt::SetRangef() to set floating point range.
	/// </summary>
	/// <returns></returns>
    kwType::ckw AsFloat(bool bAsFloat = true);    

    /// <summary>
	// Set the width (in pixels) of a window, control or input box.
    /// <para></para>
	// This sets the width of the input box of a control such as GetString(). 
    /// </summary>
    /// <returns></returns>
    kwType::ckw Width(int iWidth);       

    /// <summary>
	// Set the width (in pixels) of a window, control or input box.
    /// <para></para>
	// This sets the width of the input box of a control such as GetString(). 
    /// </summary>
    /// <returns></returns>
    kwType::ckw Width(float fWidth);       

    /// <summary>
	// Set the width (in pixels) of a window, control or input box.
    /// <para></para>
	// This sets the width of the input box of a control such as GetString(). 
    /// </summary>
    /// <returns></returns>
    kwType::ckw Width(double fWidth);       

    /// <summary>
	// Set the height (in pixels) of a window, control or input box.
    /// <para></para>
	// This sets the width of the input box of a control such as GetString(). 
    /// </summary>
    /// <returns></returns>
    kwType::ckw Height(int iHeight);       

    /// <summary>
	// Set the height (in pixels) of a window, control or input box.
    /// <para></para>
	// This sets the width of the input box of a control such as GetString(). 
    /// </summary>
    /// <returns></returns>
    kwType::ckw Height(float fHeight);       

    /// <summary>
	// Set the height (in pixels) of a window, control or input box.
    /// <para></para>
	// This sets the width of the input box of a control such as GetString(). 
    /// </summary>
    /// <returns></returns>
    kwType::ckw Height(double fHeight);     

    /// <summary>
    /// Set the Font for a label in a control, input box, or other control/widget with a label.
    /// <para></para>
	/// --> When a plain integer is used, i.e. LabelFont(15), it is set to the default font with a size of the value, i.e. "Arial,15"
    /// <para></para>
	/// Examples:
    /// <para></para>
	/// --> LabelFont(20)  -- Set the font to "Arial,20".
    /// <para></para>
	/// --> LabelFont("Arial,20")  -- Set the font to "Arial,20".  You can also add italic an bold, i.e. Font("Arial,20,bold,italic");
    /// <para></para>
	/// --> LabelFont("MyFont")    -- Set the font by a previously created font named "MyFont"
    /// </summary>
    /// <returns></returns>   
    kwType::ckw LabelFont(const char * sLabelFont);

    /// <summary>
    /// Set the Font for a label in a control, input box, or other control/widget with a label, using an HFONT value as input.
    /// <para></para>
	/// --> An HFONT value is returned when a font is created.  Also, CWindow::GetFont(&lt;FontName&gt;) can also be used to retrieve the HFONT value for storage and later usage
    ///(e.g. HFONT hMyFont = myWindow.GetFont("MyFont").
    /// <para></para>
	/// Examples:
    /// <para></para>
	/// -->    LabelFont(hMyFont)  -- Set the font to the font specified by the value in hMyFont.
    /// <para></para>
    /// </summary>
    /// <returns></returns>
    kwType::ckw LabelFont(HFONT hfont);

    /// <summary>
    // Set the Font for a label in a control, input box, or other control/widget with a label.
    /// <para></para>
	// --> When a plain integer is used, i.e. LabelFont(15), it is set to the default font with a size of the value, i.e. "Arial,15"
    /// <para></para>
	// Examples:
    /// <para></para>
	// --> LabelFont(20)  -- Set the font to "Arial,20".
    /// <para></para>
	// --> LabelFont("Arial,20")  -- Set the font to "Arial,20".  You can also add italic an bold, i.e. Font("Arial,20,bold,italic");
    /// <para></para>
	// --> LabelFont("MyFont")    -- Set the font by a previously created font named "MyFont"
    /// </summary>
    /// <returns></returns>   
    kwType::ckw LabelFont(int iLabelFontSize);

    /// <summary>
    // Set the Font for a label in a control, input box, or other control/widget with a label.
    /// <para></para>
	// --> When a plain integer is used, i.e. LabelFont(15), it is set to the default font with a size of the value, i.e. "Arial,15"
    /// <para></para>
	// Examples:
    /// <para></para>
	// --> LabelFont(20)  -- Set the font to "Arial,20".
    /// <para></para>
	// --> LabelFont("Arial,20")  -- Set the font to "Arial,20".  You can also add italic an bold, i.e. Font("Arial,20,bold,italic");
    /// <para></para>
	// --> LabelFont("MyFont")    -- Set the font by a previously created font named "MyFont"
    /// </summary>
    /// <returns></returns>   
    kwType::ckw LabelFont(float fLabelFontSize);

    /// <summary>
    // Set the Font for a label in a control, input box, or other control/widget with a label.
    /// <para></para>
	// --> When a plain integer is used, i.e. LabelFont(15), it is set to the default font with a size of the value, i.e. "Arial,15"
    /// <para></para>
	// Examples:
    /// <para></para>
	// --> LabelFont(20)  -- Set the font to "Arial,20".
    /// <para></para>
	// --> LabelFont("Arial,20")  -- Set the font to "Arial,20".  You can also add italic an bold, i.e. Font("Arial,20,bold,italic");
    /// <para></para>
	// --> LabelFont("MyFont")    -- Set the font by a previously created font named "MyFont"
    /// </summary>
    /// <returns></returns>   
    kwType::ckw LabelFont(double fLabelFontSize);

    /// <summary>
	/// Set the color for the color of a label for controls, widgets, or other objects that have a label.
    /// <para></para>
    /// --> By default, the label color is the foreground/text color of the current window or a specifically-set default color.
    /// <para></para>
    /// --> Using LabelColor() sets the label display color specifically.
    /// <para></para>
    /// --> Rgb Colors (i.e. PanColor::ForestGreen, SageColor::Red, etc.) can be used
    /// <para></para>
    /// --> Strings can be used, such as "ForestGreen", "Red", etc. 
    /// </summary>
    /// <returns></returns>
    kwType::ckw LabelColor(const char * sLabelColor);

    /// <summary>
	/// Set the color for the color of a label for controls, widgets, or other objects that have a label.
    /// <para></para>
    /// --> By default, the label color is the foreground/text color of the current window or a specifically-set default color.
    /// <para></para>
    /// --> Using LabelColor() sets the label display color specifically.
    /// <para></para>
    /// --> Rgb Colors (i.e. PanColor::ForestGreen, SageColor::Red, etc.) can be used
    /// <para></para>
    /// --> Strings can be used, such as "ForestGreen", "Red", etc. 
    /// </summary>
    /// <returns></returns>
    kwType::ckw LabelColor(RgbColor rgbLabelColor); 

    /// <summary>
    /// Currently undocumented -- To be more defined. 
    /// </summary>
    /// <param name="iXPos"></param>
    /// <returns></returns>
    kwType::ckw LabelSetXPos(int iXPos);

    /// <summary>
    /// Currently undocumented -- To be more defined. 
    /// </summary>
    /// <param name="iXPos"></param>
    /// <returns></returns>
    kwType::ckw LabelX(int iLabelX);

    /// <summary>
    /// Adds a set of arrows with a value to the right of an input box, allowing the user to use the arrows to change the value of the input box when
    /// numbers are used as entry.
    /// <para></para>
    /// --> Example: auto&amp; myInputBox = Sage::InputBox(AddArrowBox()); 
    /// <para></para>
    /// --> Up and Down presses are sent as mousewheel events in the input box. 
    /// </summary>
    /// <param name="bAddArrowBox"></param>
    /// <returns></returns>
    kwType::ckw AddArrowBox(bool bAddArrowBox = true);


    /// <summary>
	/// Sets the label of a  Control, Window, Widget or other object to the left of the control or shape.
    /// <para></para>
    /// This is used for labels around input boxes, checkbox/radiobutton groups, etc.
    /// <para></para>
	/// --> Use PadY() to set a vertical offset from this position.
    /// <para></para>
	/// --> Use PadX() to set a horizontal offset from this position.
    /// </summary>
    /// <returns></returns>
    kwType::ckw JustLabelLeft(bool bJustLabelLeft = true); 

    /// <summary>
	/// Sets the label of a  Control, Window, Widget or other object to the right of the control or shape.
    /// <para></para>
    /// This is used for labels around input boxes, checkbox/radiobutton groups, etc.
    /// <para></para>
	/// --> Use PadY() to set a vertical offset from this position.
    /// <para></para>
	/// --> Use PadX() to set a horizontal offset from this position.
    /// </summary>
    /// <returns></returns>
    kwType::ckw JustLabelRight(bool bJustLabelRight = true); 

    /// <summary>
	/// Sets the label of a  Control, Window, Widget or other object to the top of the control or shape.
    /// <para></para>
    /// This is used for labels around input boxes, checkbox/radiobutton groups, etc.
    /// <para></para>
	/// --> Use PadY() to set a vertical offset from this position.
    /// <para></para>
	/// --> Use PadX() to set a horizontal offset from this position.
    /// </summary>
    /// <returns></returns>
    kwType::ckw JustLabelTop(bool bJustLabelTop = true); 

    /// <summary>
	/// Sets the label of a  Control, Window, Widget or other object to the bottom of the control or shape.
    /// <para></para>
    /// This is used for labels around input boxes, checkbox/radiobutton groups, etc.
    /// <para></para>
	/// --> Use PadY() to set a vertical offset from this position.
    /// <para></para>
	/// --> Use PadX() to set a horizontal offset from this position.
    /// </summary>
    /// <returns></returns>
    kwType::ckw JustLabelBottom(bool bJustLabelBottom = true); 

	/// <summary>
	/// Allows an Input Box to accept multiple lines (the default is one line).
    /// <para></para>
    /// When multiple lines are used, a button or some other method can be used to determine when the user has signaled sending (or finishing) with the edit box.
	/// </summary>
	/// <returns></returns>
    kwType::ckw MultiLine(bool bMultiLine = true); 

    /// <summary>
    /// Prints only '*' in an input box instead of the characters type.  This allows usage as a password box.
    /// </summary>
    /// <returns></returns>
    kwType::ckw Password(bool bPassword = true); 

	/// <summary>
	/// Sets a vertical scroll bar on an applicable control (i.e. an edit box)
	/// </summary>
    kwType::ckw VScroll(bool bVScroll = true); 

	/// <summary>
	/// Sets a horizontal scroll bar on an applicable control (i.e. an edit box)
	/// </summary>
    kwType::ckw HScroll(bool bHScroll = true); 
    
    /// <summary>
	// This function is deprecated.  Use AddBorder() to add a border to an object.
    /// </summary>
    /// <param name="bThickBorder"></param>
    /// <returns></returns>
    kwType::ckw ThickBorder(bool bThickBorder = true); 

    /// <summary>
	/// Sets the text in Control, Window, Widget or other object to the horizontal center of the object (i.e. window, dialog, etc.).
    /// <para></para>
	/// --> Use PadY() to set a vertical offset from this position.
    /// <para></para>
	/// --> Use PadX() to set a horizontal offset from this position.
    /// </summary>
    /// <para></para>
    /// **note : JustTextCenter(), TextCenter() and TextCenterX() are the same function
    /// <returns></returns>
    kwType::ckw JustTextCenter(bool bJustTextCenter = true); 

   /// <summary>
	/// Sets the text in Control, Window, Widget or other object to the top of the object (i.e. window, dialog, etc.).
    /// <para></para>
	/// --> Use PadY() to set a vertical offset from this position.
    /// <para></para>
	/// --> Use PadX() to set a horizontal offset from this position.
    /// </summary>
    /// <para></para>
    /// <returns></returns>
    kwType::ckw JustTextTop(bool bTextJustTop = true);

    /// <summary>
	/// Sets the text in Control, Window, Widget or other object to the bottom of the object (i.e. window, dialog, etc.).
    /// <para></para>
	/// --> Use PadY() to set a vertical offset from this position.
    /// <para></para>
	/// --> Use PadX() to set a horizontal offset from this position.
    /// </summary>
    /// <para></para>
    /// <returns></returns>
    kwType::ckw JustTextBottom(bool bTextJustBottom = true);

    /// <summary>
	/// Sets the text in Control, Window, Widget or other object to the horizontal center of the object (i.e. window, dialog, etc.).
    /// <para></para>
	/// --> Use PadY() to set a vertical offset from this position.
    /// <para></para>
	/// --> Use PadX() to set a horizontal offset from this position.
    /// </summary>
    /// <para></para>
    /// **note : JustTextCenterX(), TextCenterX() and TextCenterX() are the same function
    /// <returns></returns>
    kwType::ckw JustTextCenterX(bool bJustTextCenter = true); 

    /// <summary>
	/// Sets the text in a Control, Window, Widget or other object to the left in the object.
    /// <para></para>
	/// --> Use PadY() to set a vertical offset from this position.
    /// <para></para>
	/// --> Use PadX() to set a horizontal offset from this position.
    /// </summary>
    /// <returns></returns>
    kwType::ckw JustTextLeft(bool bJustTextLeft = true); 

    /// <summary>
	/// Sets the text in a Control, Window, Widget or other object to the right in the object.
    /// <para></para>
	/// --> Use PadY() to set a vertical offset from this position.
    /// <para></para>
	/// --> Use PadX() to set a horizontal offset from this position.
    /// </summary>
    /// <returns></returns>
    kwType::ckw JustTextRight(bool bJustTextRight = true); 

    /// <summary>
	/// In an EditBox (or other text-based control) ReadOnly makes the text non-editable and only for display. 
    /// </summary>
    /// <returns></returns>
    kwType::ckw ReadOnly(bool bReadOnly = true); 

    /// <summary>
	/// In an Edit Box, FloatsOnly() limits the text to floating point numbersm allowing a '.',numbers and the '-' sign.  All other text is ignored.
	// See NumbersOnly() to limit text to integer numbers (i.e. without the '.' needed for floating-point numbers.
    /// <para></para>
	/// ** note: This also automatically sets the test justification for an EditBox to TextRight(). This can be changed by explicitly setting another for, i.e. TextCenter() or TextLeft();
    /// </summary>
    /// <returns></returns>
    kwType::ckw FloatsOnly(bool bFloatsOnly = true); 

 
    /// <summary>
	/// In an EditBox, NumbersOnly() limits the text input to only integer numbers. All other text is ignored.  See FloatsOnly() for floating-point numbers.
    /// <para></para>
	/// ** note: This also automatically sets the test justification for an EditBox to TextRight(). This can be changed by explicitly setting another for, i.e. TextCenter() or TextLeft();
    /// </summary>
    /// <returns></returns>
    kwType::ckw NumbersOnly(bool bNumbersOnly = true); 

    /// <summary>
    /// TBD
    /// </summary>
    /// <param name="bStartGroup"></param>
    /// <returns></returns>
    kwType::ckw StartGroup(bool bStartGroup = true); 
   
    /// <summary>
    /// TBD
    /// </summary>
    /// <param name="bEndGroup"></param>
    /// <returns></returns>
    kwType::ckw EndGroup(bool bEndGroup = true); 

    /// <summary>
    /// Allows a control in a dialog window to be tabbed to with the TAB key when using keys to move from one control to another.
    /// <para></para>
    /// note: ** This keyword and its applications are still under development
    /// </summary>
    /// <returns></returns>
    kwType::ckw TabStop(bool bTabStop = true); 

	/// <summary>
	/// In a multi-line edit box (where the "MultiLine" option has been used), this tells the edit box to send a PressReturned() message when the 
    /// Return Key is pressed.  
    /// <para></para>
    /// Otherwise, the multi-line edit box will accept the return key as input and start a new line in the edit box.
	/// </summary>
	/// <returns></returns>
    kwType::ckw WantReturn(bool bWantReturn = true); 

    /// <summary>
    /// ** This keyword and its applications is still under development.
    /// </summary>
    /// <returns></returns>
    kwType::ckw AttachButton(int iButtonID); 

    /// <summary>
    /// ** This keyword and its applications is still under development.
    /// </summary>
    /// <returns></returns>
    kwType::ckw AttachButton(const char * sButtonName); 


    /// <summary>
	// Allows horizontal scrolling in an input box.  This allows the text to roll in the box when the box is smaller than the input
	/// text.  
    /// <para></para>
    /// Otherwise, the text is lmited to the size of the input box.
    /// <para></para>
    /// ** note AllowScroll() and AllowHScroll() are the same function.
    /// </summary>
    /// <returns></returns>
    kwType::ckw AllowScroll     (bool bAllowScroll  = true); 

    /// <summary>
	// Allows horizontal scrolling in an input box.  This allows the text to roll in the box when the box is smaller than the input
	/// text.  
    /// <para></para>
    /// Otherwise, the text is lmited to the size of the input box.
    /// <para></para>
    /// ** note AllowScroll() and AllowHScroll() are the same function.
    /// </summary>
    /// <returns></returns>
    kwType::ckw AllowHScroll    (bool bAllowHScroll = true); 

    /// <summary>
	// Allows vertical scrolling in an input box.  This allows the text to roll in the box when the box is smaller than the input
	/// text.  
    /// <para></para>
    /// Otherwise, the text is lmited to the size of the input box.
    /// <para></para>
    /// ** note AllowScroll() and AllowHScroll() are the same function.
    /// </summary>
    /// <returns></returns>
    kwType::ckw AllowVScroll    (bool bAllowVScroll = true); 

    /// <summary>
    /// When creating a checkbox, Checked() (or Checked(true)) will set the checkbox as initially checked. 
    ///<para></para>
    /// Checked(false) sets the checkbox as unchecked -- howver, this is not needed since the checkbox defaults to unchecked.
    /// </summary>
    /// <returns></returns>
    kwType::ckw Checked(bool bChecked = true); 


    /// <summary>
	/// Sets the Font for the highlighted button, checkbox or radiobox -- used for Panel Type buttons.
    ///<para></para>
	/// See Font() for more information how to set the Font.  Example: FontHigh("Arial,40") or FontHigh(MyFont)
    /// </summary>
    /// <returns></returns>
    kwType::ckw FontHigh(int iFontSize); 

    /// <summary>
	/// Sets the Font for the highlighted button, checkbox or radiobox -- used for Panel Type buttons.
    ///<para></para>
	/// See Font() for more information how to set the Font.  Example: FontHigh("Arial,40") or FontHigh(MyFont)
    /// </summary>
    /// <returns></returns>
    kwType::ckw FontHigh(const char * sFont); 

    /// <summary>
	/// Sets the Font for the checked button, checkbox or radiobox -- used for Panel Type buttons.
    ///<para></para>
	/// See Font() for more information how to set the Font.  Example: FontChecked("Arial,40") or FontChecked(MyFont)
    /// </summary>
    /// <param name="iFontSize"></param>
    /// <returns></returns>
    
    kwType::ckw FontChecked(int iFontSize); 
    /// <summary>
	/// Sets the Font for the checked button, checkbox or radiobox -- used for Panel Type buttons.
    ///<para></para>
	/// See Font() for more information how to set the Font.  Example: FontChecked("Arial,40") or FontChecked(MyFont)
    /// </summary>
    /// <param name="iFontSize"></param>
    /// <returns></returns>
    kwType::ckw FontChecked(const char * sFont); 

    /// <summary>
	/// Sets the highlighted foreground (Text) color for the control when the mouse is over the control (i.e. a checkbox).
    ///<para></para>
    /// This is usually used with Panel Type checkboxes and radiobuttons, But can be used for any control that accepts it.
    /// </summary>
    /// <returns></returns>
    kwType::ckw fgHigh(RgbColor rgbColor);

    /// <summary>
	/// Sets the highlighted foreground (Text) color for the control when the mouse is over the control (i.e. a checkbox).
    ///<para></para>
    /// This is usually used with Panel Type checkboxes and radiobuttons, But can be used for any control that accepts it.
    /// </summary>
    /// <returns></returns>
    kwType::ckw fgHigh(const char * sFont); 

    /// <summary>
	/// Sets the highlighted background color for the control when the mouse is over the control (i.e. a checkbox).
    ///<para></para>
    /// This is usually used with Panel Type checkboxes and radiobuttons, But can be used for any control that accepts it.
    /// </summary>
    /// <returns></returns>
    kwType::ckw bgHigh(RgbColor rgbColor); 

    /// <summary>
	/// Sets the highlighted background color for the control when the mouse is over the control (i.e. a checkbox).
    ///<para></para>
    /// This is usually used with Panel Type checkboxes and radiobuttons, But can be used for any control that accepts it.
    /// </summary>
    /// <returns></returns>
    kwType::ckw bgHigh(const char * sColor); 

    /// <summary>
	/// Sets the highlighted background color for the control that is checked or filled when the mouse is over the control (i.e. a checkbox).
    ///<para></para>
    /// This is usually used with Panel Type checkboxes and radiobuttons, But can be used for any control that accepts it.
    /// </summary>
    /// <returns></returns>
    kwType::ckw bgChecked(RgbColor rgbColor); 

    /// <summary>
	/// Sets the highlighted background color for the control that is checked or filled when the mouse is over the control (i.e. a checkbox).
    ///<para></para>
    /// This is usually used with Panel Type checkboxes and radiobuttons, But can be used for any control that accepts it.
    /// </summary>
    /// <returns></returns>
    kwType::ckw bgChecked(const char * sColor); 

    /// <summary>
	/// Sets the highlighted foreground (text) color for the control that is checked or filled when the mouse is over the control (i.e. a checkbox).
    ///<para></para>
    /// This is usually used with Panel Type checkboxes and radiobuttons, But can be used for any control that accepts it.
    /// </summary>
    /// <returns></returns>
    kwType::ckw fgChecked(RgbColor rgbColor); 

    /// <summary>
	/// Sets the highlighted foreground (text) color for the control that is checked or filled when the mouse is over the control (i.e. a checkbox).
    ///<para></para>
    /// This is usually used with Panel Type checkboxes and radiobuttons, But can be used for any control that accepts it.
    /// </summary>
    /// <returns></returns>
    kwType::ckw fgChecked(const char * sColor); 

    /// <summary>
	/// XPos() Can be used in a console input function (i.e. console.GetInput(), console.getFloat(), etc.) to set the position of the 
	/// input box.  
    /// <para></para>
    /// This allows the boxes to be easily lined up with text to the left and aligned boxes to the right.
    /// <para>** note: XPos and SetXPos() are the same function</para>
    /// </summary>
    /// <returns></returns>
    kwType::ckw SetXPos(int iPos);

    /// <summary>
	/// Set the width of an input control in terms of average character width (i.e. CharWidth)
    /// <para></para>
	/// In an input, this can set the range of the input.  For example., CharWidth(50) sets the size of the input box
	/// to the size of 50 average characters.  
    /// <para></para>
	/// --> Since fonts are proportional, this is only an estimate, unless using a terminal font, such as Courier, Courier New, etc.
    /// <para></para>
	/// --> Width() can also be used to set a specific width.
    /// <para></para>
	/// ** note: MaxCharWidth() and CharWidth() are the same function. 
    /// </summary>
    /// <param name="iWidth"></param>
    /// <returns></returns>
    kwType::ckw MaxCharWidth(int iWidth);

    /// <summary>
	/// Set the width of an input control in terms of average character width (i.e. CharWidth)
    /// <para></para>
	/// In an input, this can set the range of the input.  For example., CharWidth(50) sets the size of the input box
	/// to the size of 50 average characters.  
    /// <para></para>
	/// --> Since fonts are proportional, this is only an estimate, unless using a terminal font, such as Courier, Courier New, etc.
    /// <para></para>
	/// --> Width() can also be used to set a specific width.
    /// <para></para>
	/// ** note: MaxCharWidth() and CharWidth() are the same function. 
    /// </summary>
    /// <param name="iWidth"></param>
    /// <returns></returns>
    kwType::ckw CharWidth(int iWidth);

    /// <summary>
    /// ** This keyword is still being defined.  
    /// </summary>
    /// <param name="bAddNewLine"></param>
    /// <returns></returns>
    kwType::ckw AddNewLine(bool bAddNewLine);

    /// <summary>
    /// ** This keyword is still being defined.  
    /// </summary>
    /// <param name="bAddNewLine"></param>
    /// <returns></returns>
    kwType::ckw AddLf(bool bAddNewLine);

    /// <summary>
    /// ** This keyword is still being defined.  
    /// </summary>
    /// <param name="bAddNewLine"></param>
    /// <returns></returns>
    kwType::ckw AddNewLines(int iNumLines);

    /// <summary>
    /// ** This keyword is still being defined.  
    /// </summary>
    /// <param name="bAddNewLine"></param>
    /// <returns></returns>
    kwType::ckw AddLfs(int iNumLines);


	// Use the last known options.  This is deprecated.
	// Instead, set the options in a cwfOpt2 value, such as:
	//
	// cwfOpt2 cwOpt = fgColor("Red")  | Transparent() | CenterXY(), etc. 
	// and then use cwOpt as the input. 
	//

    /// <summary>
    /// ** This keywords is deprecated.
    /// </summary>
    /// <param name="bUseLast"></param>
    /// <returns></returns>
    kwType::ckw UseLast(bool bUseLast = true);


    /// <summary>
    /// Allows empty returns on input boxes (e.g. pressing return in an empty input box)
    /// <para></para>
    /// </summary>
    /// <returns></returns>
    kwType::ckw AllowBlanks(bool bAllowBlanks = true);

    /// <summary>
    /// Disallows empty returns on input boxes (e.g. pressing return in an empty input box) when otherwise an empty line would return an empty string.
    /// <para></para>
    /// This has the affect of making the user enter something in the input box before continuing, and will bring up a message box informing the user something
    /// must be entered.
    /// </summary>
    /// <returns></returns>
    kwType::ckw NoBlanks(bool bNoBlanks = true);

    /// <summary>
    /// Allows the Escape Key to cancel dialog boxes or other input boxes.
    /// <para></para>
    /// --> Typically, a button must be pressed or a Return Key entered to dismiss a dialog box. 
    /// <para></para>
    /// --> Using AllowEsc() enables using the ESC to cancel the dialog box, which will then remove the dialog box from the screen and
    /// return with a cancelled status.
    /// </summary>
    /// <returns></returns>
    kwType::ckw AllowEsc(bool bAllowEsc = true);

    /// <summary>
    /// Sets the number of columns in a button group, such as with multiple checkboxes, radio buttons, or panel buttons.
    /// </summary>
    /// <returns></returns>
    kwType::ckw Columns(int iColumns);

    /// <summary>
    /// Sets the number of rows in a button group, such as with multiple checkboxes, radio buttons, or panel buttons.
    /// </summary>
    /// <returns></returns>
    kwType::ckw Rows(int iRows);

     /// <summary>
	// Add X pixels to the size or placement of a control or other object.
    /// <para></para>
	/// Example: CreateButton(10,50,"OK",PadX(20)) will add 20 pixels to the size of the button to make the button wider with "OK" in the center.
    /// <para></para>
    /// ** note: PadX() and XOffset() are the same function
    /// </summary>
    /// <returns></returns>
    kwType::ckw XOffset(int iPadX); 

    /// <summary>
	/// Add Y pixels to the size or placement of a control or other object.
    /// <para></para>
	/// Example: CreateButton(10,50,"OK",PadY(20)) will add 20 pixels to the vertical size of the button to make the button taller with "OK" in the center.
    /// <para></para>
	/// PadY() only works with certain controls.  For example, standard buttons work with PadX() but not PadY(), where
	/// Panel Buttons and Windows Buttons (i.e. Style("Panel") and Style("Windows") respond do the PadY(), though 
	/// specifiying an explicit width and height is a better option.
    /// <para></para>
    /// ** note: PadY() and YOffset() are the same function
    /// </summary>
    /// <returns></returns>
    kwType::ckw YOffset(int iPadY); 

    /// <summary>
    /// Sets the title cell text for a combo box.
    /// <para></para>
    /// --> Without a title cell text (default), the first entry in the combo box list is displayted in the main label/display area for the combobox.
    /// <para></para>
    /// --> With a title cell, the "title cell" text is displayed when there is no current selection.
    /// <para></para>
    /// --> This is used only when creating new comboboxes, such as DevComboBox(), etc.
    /// </summary>
    /// <returns></returns>
    kwType::ckw cbTitleCell(const char * sTitleCell);

    /// <summary>
	/// When creating a new window with a frame or border, InnerSize() creates the window with the
	/// canvas size set to the input width/height.  
    /// <para></para>
    /// Otherwise, the Width/Height is the size of the entire window, including the frame or border
    /// </summary>
    /// <returns></returns>
    kwType::ckw InnerSize(bool bInnerSize = true);

    /// <summary>
	/// Allows the user to move the control or Widget (in Controls and Widgets that support it) by placing the mouse anywhere in the window's main client area.
    /// <para></para>
    /// Otherwise, the window can only be moved by placing the mouse in the title bar area.
    /// <para></para>
	/// This allows the user to move the Control or Widget on the screen with the mouse, usually
	/// with the Control-Key pressed, but also sometimes with normal mouse movement if there is no 
	/// other reason to use the mouse on the Control or Widget.
    /// <para></para>
	/// --> Some controls may require the control-key pressed as the window is being moved (TBD)
    /// <para></para>
	/// --> Some controls may also support using the right mouse button (TBD)
    /// </summary>
    /// <returns></returns>
    kwType::ckw AllowDrag(bool bAllowDrag = true);

    /// <summary>
	/// When creating a BitmapWindow or other types of windows, this creates a separate "popup" window.
    /// <para></para>
	/// Otherwise, the window is embedded in the current window at the (X,Y) position specified.
    /// </summary>
    /// <returns></returns>
    kwType::ckw Popup(bool bPopup = true);

	/// <summary>
	/// Can be used in dialog boxes such as GetInteger(), GetFloat,QuickDialog(), etc. to elminate the cancel button.
	/// This will insist the user to enter a value before continuing with no way to cancel the input dialog.
    /// <para></para>&#160;&#160;&#160;
	/// If the user closes the window, the dialog box will return the default value, even when NoCancel() is used.
    /// <para></para>&#160;&#160;&#160;
    /// --> Use kw::Modal() to disable the parent window until the dialog box is closed.
    /// </summary>
	/// <returns></returns>
    kwType::ckw Modal(bool bModal = true);

    /// <summary>
    /// ** This keyword is still in development. TBD
    /// </summary>
    /// <param name="bUsingSysMenu"></param>
    /// <returns></returns>
    kwType::ckw UsingSysMenu(bool bUsingSysMenu = true);

    /// <summary>
	/// Allows the user to Cancel certain controls that otherwise not have a cancel option.
    /// <para></para>
    /// This used when creating a PleaseWaitWindow() to allow the user to cancel, which can then be 
	/// detected by polling the please wait window.
    /// </summary>
    /// <returns></returns>
    kwType::ckw CancelOk(bool bCancelOk = true); 

    /// <summary>
	/// Hides the cancel button on dialog.  This is used for the PleaseWaitWindow() to remove the cancel button
	/// after a certain point -- i.e. after a point of no return. 
    /// </summary>
    /// <param name="bHideCancel"></param>
    /// <returns></returns>
    kwType::ckw HideCancel(bool bHideCancel = true); 

    /// <summary>
	/// Adds a progress bar to the PleaseWaitWindow(),
    /// <para></para>
	/// --> Example: cWin.PleaseWaitWindow(ProgressBar());
    /// </summary>
    /// <returns></returns>
    kwType::ckw ProgressBar(bool bProgressBar = true); 

    /// <summary>
	// When Creating a Dialog (i.e. dialog.Info(), etc.) or Dialog Window, CenterWin()
	// causes the newly created window to be centered in the parent window creating it. 
    /// <para></para>
	// Otherwise, the window is put somewhere appropriate on the screen, or, in the case of dialog windows and
	// dialog functions, centered on the screen rather than the parent window.
    /// </summary>
    /// <param name="bCenterWin"></param>
    /// <returns></returns>
    kwType::ckw CenterWin(bool bCenterWin = true); 

	/// <summary>
	/// Can be used in dialog boxes such as GetInteger(), GetFloat,QuickDialog(), etc. to elminate the cancel button.
	/// This will insist the user to enter a value before continuing with no way to cancel the input dialog.
    /// <para></para>&#160;&#160;&#160;
	/// If the user closes the window, the dialog box will return the default value, even when NoCancel() is used.
    /// <para></para>&#160;&#160;&#160;
    /// --> Use opt::Modal() to disable the parent window until the dialog box is closed.
    /// </summary>
	/// <returns></returns>
    kwType::ckw NoCancel(bool bNoCancel = true);

	/// <summary>
	/// Tell various functions (such as QuickDialog) not to auto hide the window when done.
    /// <para></para>
    /// When this is used, it is the calling program's responsibility to hide the window when finished. 
    /// <para></para>&#160;&#160;&#160;
    /// This is useful for Quick Dialogs (and some other functions, TBD) to validate data after the user pressed OK, to
    /// re-enter the loop without creating a flash when hiding and re-showing the window.
	/// </summary>
	/// <returns></returns>
    kwType::ckw NoAutoHide(bool bNoAutoHide = true);


    /// <summary>
	/// Specifies window as "TopMost", keeping the window at the top of all windows.  This prevents other windows from 
	/// overlapping the window. 
    /// <para></para>
    ///  This is useful for windows that need to be on top at all times so they do not become
	/// obscured by other windows
    /// </summary>
    /// <returns></returns>
    kwType::ckw SetTopmost(bool bSetTopmost = true);

	/// <summary>
	/// Sets the size of the control, widget, or functional item. 
    /// <para>
    /// When available, this can be used when the Size is not parameter in the function call.
    /// <para>
    /// --> Note: Size() and SetSize() are the same function
	/// </summary>
	/// <returns></returns>
    kwType::ckw SetSize(int iSizeX,int iSizeY);

	/// <summary>
	/// Sets the size of the control, widget, or functional item. 
    /// <para>
    /// When available, this can be used when the Size is not parameter in the function call.
    /// <para>
    /// --> Note: Size() and SetSize() are the same function
	/// </summary>
	/// <returns></returns>
    kwType::ckw SetSize(POINT pSize);
 
 	/// <summary>
	/// Sets the size of the control, widget, or functional item. 
    /// <para>
    /// When available, this can be used when the Size is not parameter in the function call.
    /// <para>
    /// --> Note: Size() and SetSize() are the same function
	/// </summary>
	/// <returns></returns>
    kwType::ckw Size(int iSizeX,int iSizeY);
    
	/// <summary>
	/// Sets the size of the control, widget, or functional item. 
    /// <para>
    /// When available, this can be used when the Size is not parameter in the function call.
    /// <para>
    /// --> Note: Size() and SetSize() are the same function
	/// </summary>
	/// <returns></returns>
    kwType::ckw Size(POINT pSize);


	/// <summary>
	/// Sets the maximum size of the control, widget, or functional item. 
    /// When available, this can be used when the Size is not parameter in the function call.
	/// </summary>
	/// <returns></returns>
    kwType::ckw MaxSize(int iSizeX,int iSizeY);

	/// <summary>
	/// Sets the maximum size of the control, widget, or functional item. 
    /// When available, this can be used when the Size is not parameter in the function call.
	/// </summary>
	/// <returns></returns>
    kwType::ckw MaxSize(POINT pSize);

    /// <summary>
    // Sets Sagebox icon rather than a specified bitmap for various demos and displays. 
    /// </summary>
    /// <returns></returns>
    kwType::ckw SageIcon(Sage::SageIconType Icon = Sage::SageIconType::Green32,TextJust just = TextJust::Default); 


    /// <summary>
    /// Sets a bitmapped icon for placement in Form Windows.
    /// <para></para>
    /// See QuickForm() functions.
    /// </summary>
    /// <returns></returns>
    kwType::ckw SetIcon(CBitmap & cBitmap,TextJust just = TextJust::Default);

    /// <summary>
    /// Used when creating QuickFormWindows() to remove space between the DevWindow and Client Window
    /// <para></para>
    /// See QuickForm() functions
    /// </summary>
    /// <param name="bNoPadding"></param>
    /// <returns></returns>
    kwType::ckw NoPadding(bool bNoPadding = true);

    /// <summary>
    /// Used for Sagebox MessageBox() functions to display a Warning Icon in the dialog box.
    /// <para></para>
    /// This is similar to Windows MB_ICONWARN in Win32 Messageboxes.
    /// </summary>
    /// <returns></returns>
    kwType::ckw IconWarning(bool bIconWarning = true);

    /// <summary>
    /// Used for Sagebox MessageBox() functions to display a Stop Icon in the dialog box.
    /// <para></para>
    /// This is similar to Windows MB_ICONSTOP in Win32 Messageboxes.
    /// </summary>
    /// <returns></returns>
    kwType::ckw IconStop(bool bIconStop = true);

    /// <summary>
    /// Used for Sagebox MessageBox() functions to display a Information Icon in the dialog box.
    /// <para></para>
    /// This is similar to Windows MB_ICONINFO in Win32 Messageboxes.
    /// </summary>
    /// <returns></returns>
    kwType::ckw IconInfo(bool bIconInfo = true);

    /// <summary>
    /// Used for Sagebox MessageBox() functions to force display of no icon in the message box.
    /// </summary>
    /// <returns></returns>
    kwType::ckw IconNone(bool bIconNone = true);

    /// <summary>
    /// Adds shadow to text for various controls.  Currently TextWidget() is the main control that uses AddShadow() to add a shadow to text.
    /// </summary>
    /// <returns></returns>
    kwType::ckw AddShadow(bool bAddShadow = true);

    /// <summary>
    /// Used with YesNoWindow() functions to add a third 'cancel' option.
    /// <para></para>
    /// See documentation on YesNoWindow()
    /// </summary>
    /// <returns></returns>
    kwType::ckw YesNoCancel(bool bYesNoCancel = true);

    /// <summary>
    /// Sets the image view to maximized, using the entire desktop to show the image. 
    /// </summary>
    /// <returns></returns>
    kwType::ckw Maximize(bool bMaximize = true);

    /// <summary>
    /// Sets the image view to minimized, using the entire desktop to show the image. 
    /// </summary>
    /// <returns></returns>
    kwType::ckw Minimize(bool bMinimize = true);

    /// <summary>
    /// NoZoom() is a control used with ImageView() and other file display functions.
    /// <para></para>
    /// Documentation is pending.
    /// </summary>
    /// <param name="bNoZoom"></param>
    /// <returns></returns>
    kwType::ckw NoZoom(bool bNoZoom = true);

    /// <summary>
    /// Tells the ImageView() function to fill the image in the window if the image is size is less than the available space in the window.
    /// </summary>
    /// <param name="bFillZoom"></param>
    /// <returns></returns>
    kwType::ckw FillZoom(bool bFillZoom = true);

    /// <summary>
    /// For ImageView Windows, this sets the size of the image viewer on the desktop.  For example, 50% will use half the width and height for the image view window.
    /// <para></para>
    /// For other controls, this sets a percent value from 0-100%
    /// <para></para>
    /// --> This is an optional parameter.
    /// <para></para>
    /// Percent(100) will maximize the window, using the entire desktop.
    /// <para></para>
    /// Percent can be between 25% and 100%. The default is approximately 75% of the desktop space, depending on the function.
    /// <para></para>
    /// --> Example:  Sagebox::ViewImage("My Image.jpg",Percent(50));
    /// </summary>
    /// <param name="iPercent"></param>
    /// <returns></returns>
    kwType::ckw Percent(int Percent);

    /// <summary>
    /// Adds a ZoomBox for the image viewer for images using ViewImage()
    /// <para></para>
    /// The Zoom Box makes it easy to navigate, and zoom in and out of an image.   
    /// <para></para>
    /// The Zoom Box will change to the new window when a different window using the ImageView() function gains the focus.
    /// </summary>
    /// <param name="bZoombox"></param>
    /// <returns></returns>
    kwType::ckw ZoomBox(bool bZoomBox = true);

    kwType::ckw Reversed(bool bReversed = true);
    kwType::ckw WaitforClose(bool bWaitforClose = true);

    /// <summary>
    /// Sets the "before" title for the Before &amp; After Image Viewer.
    /// <para></para>
    /// This is an optional parameter.  If not used, a default label or no label will be chosen. 
    /// </summary>
    /// <returns></returns>   
    kwType::ckw BeforeTitle(const char * sBeforeTitle);         

    /// <summary>
    /// Sets the "after" title for the Before &amp; After Image Viewer.
    /// <para></para>
    /// This is an optional parameter.  If not used, a default label or no label will be chosen. 
    /// </summary>
    /// <param name="sTitle">Title/Label for After (i.e. resultant image) Window</param>
    /// <returns></returns>    cwfOpt2 BeforeTitle(const char * sTitle) ;
    kwType::ckw AfterTitle(const char * sAfterTitle);    

    /// <summary>
    /// ** Documentation is pending.
    /// </summary>
    /// <param name="bNoZoom"></param>
    /// <returns></returns>
    kwType::ckw ShowWatermark(bool bShowWatermark = true);         

    /// <summary>
    /// Allows a window to close that is otherwise disabled.
    /// <para></para>
    /// This is useful for creating Dev Windows which initially do not have the 'X' close button on the upper-right.
    /// <para></para>
    /// Adding kw::AllowClose() or calling the Dev Window's AllowClose() function will give the user the ability to close the window.
    /// <para></para>
    /// ** note:  When using Sagebox::GetEvent() (vs. CWindow::GetEvent()) the Window's 'X' closeure button will be added automatically
    /// When the last primary window is closed, or a Dev Window is opened without a primary window open (i.e. console mode).
    /// <para></para>
    /// In such a case, you can use the Dev Window's AllowAutoClose(false) (i.e. Sagebox::DevAllowAutoClose(false) function to disable the appearance of the automatic 'X' button. 
    /// </summary>
    /// <returns></returns>
    kwType::ckw AllowClose(bool bAllowClose = true);

    /// <summary>
    /// Makes a shaped filled vs. open when calling basic drawing functions such as DrawRectangle(), DrawCircle(), etc.
    /// <para></para>
    /// When drawing shapes, many shapaes can be filled or open, such as CWindow::DrawCircle() (draws an open circle) or CWindow::DrawFilledCircle() which draws a filled circle.
    /// <para></para>
    /// By using Filled(), the open function (e.g. CWindow::DrawCircle()) can be called with kw::Filled() as a keyword to call the DrawFilledCircle() function.
    /// <para></para>
    /// This allows one statement in a routine that may make a decision on filled vs. open by using Filled() when otherwise two function would be called by use if an if-than-else construct.  
    /// <para></para>
    /// Example: MyWindow.DrawRecangle(x,y,width,height,SageColor::Red,kw::Filled(bisFilled), where: bisFilled is a boolean.  Other keywords can be added, such as border color, angle, border size, etc.
    /// </summary>
    kwType::ckw Filled(bool bFilled=true);

    /// <summary>
    /// When creating a window with the NewWindow() function, the fullscreen keyword will:
    /// <para></para>
    /// 1. If no size is specified in NewWindow() call (e.g. NewWindow(kw::Fullscreen()), etc.), a window that is the size of the main monitor
    /// (e.g. 1920x1080) will be created with no window frame to fill the entire screen.
    /// <para></para>
    /// 2. If a size is specified in NewWindow() call (e.g. NewWindow(400,500,kw::Fullscreen()), the window is created with the size specified but with
    /// no window frame. 
    /// <para></para>
    /// --> When no window frame is present, the user cannot close the window by using the upper-right 'X' button (becuase it doesn't exist), and it can be difficult to
    /// determine how to close the window.
    /// <para></para>
    /// --> It is recommended that the user is provided a way to specifically close the window, or looking for the DevWindowClosed() in a dev window to allow the user to close the window easily.
    /// </summary>
    kwType::ckw Fullscreen(bool bFullscreen=true);

    /// <summary>
    /// Sets Beginning and End Line Cap Types individually or together for a Line, Arc or other line segment. A text string or 1-2 symbolic LineCapType values can be used.
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
    kwType::ckw LineCaps(LineCapType eLineCapType,LineCapType eEndCapType = LineCapType::Undefined);

    /// <summary>
    /// Sets Beginning and End Line Cap Types individually or together for a Line, Arc or other line segment. A text string or 1-2 symbolic LineCapType values can be used.
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
    kwType::ckw LineCaps(const char * sLineCapTypes);

    /// <summary>
    /// Sets the Pen opacity for filled draw functions such as FilledCircle(), FillCurves(), etc. 
    /// <para></para>
    /// - In function drawing open shapes (e.g. DrawCircle(), DrawCurves(),etc.) using kw::Opacity() or kw::PenOpacity() is the same.
    /// <para></para>
    /// - By default, in most filled functions, the Pen color is separate from the Opacity used in the function, and the pen opacity is 255 (completely opaque).
    /// <para></para>
    /// - You can also specify an opacity as a pen color in the color itself, such as kw::PenColor("blue(128)"), kw::PenColor(PanColor::BlueA(128)), kw::PenColor(RgbA{0,0,255,128}, etc.
    /// <para></para>
    /// For pen gradients, the secondary color will inherit the first color's gradient unless a specific color is used.  Thus, if PenOpacity() is used, this sets the opacity
    /// for both colors in the gradient.
    /// </summary>
    /// <param name="iOpacity"> - Opacity to set for the pen.  255 = completely opaque, 0 = completely transparent (invisible)</param>
    /// <returns></returns>
    kwType::ckw PenOpacity(int iOpacity);

    /// <summary>
    /// Sets the tension for Spline Curves, such as with DrawCurves(), DrawClosedCurves() and FillClosedCurves().
    /// <para></para>
    /// - By default the tension is .5.  Using smaller numbers (e.g. .1) makes the curves appear more and more like bent straight lines.
    /// <para></para>
    /// - Using numbers > .5, e.g. 1.5, 2.0, etc. make the 'tension' on the curve higher, causing the curves to take longer to bend around to fit the curve.
    /// <para></para>
    /// ** note: Using a negative number for the tension sets the tension to its default.
    /// Example: window.DrawCurve(myPoints,10,"red",kw::Tension(1.7)); 
    /// </summary>
    /// <param name="fTension"> - Tension to apply to the spline curve.  Negative values set tension to its default value (typically .5)</param>
    kwType::ckw Tension(float fTension);

    /// <summary>
    /// Sets the tension for Spline Curves, such as with DrawCurves(), DrawClosedCurves() and FillClosedCurves().
    /// <para></para>
    /// - By default the tension is .5.  Using smaller numbers (e.g. .1) makes the curves appear more and more like bent straight lines.
    /// <para></para>
    /// - Using numbers > .5, e.g. 1.5, 2.0, etc. make the 'tension' on the curve higher, causing the curves to take longer to bend around to fit the curve.
    /// <para></para>
    /// ** note: Using a negative number for the tension sets the tension to its default.
    /// Example: window.DrawCurve(myPoints,10,"red",kw::Tension(1.7)); 
    /// </summary>
    /// <param name="fTension"> - Tension to apply to the spline curve.  Negative values set tension to its default value (typically .5)</param>
    kwType::ckw Tension(double fTension);

    /// <summary>
    /// Sets a "line join" type for drawn shapes via the GDI, such as Triangles, Ellipses, Curves, etc.,
    /// i.e. any shapes that are drawn with multiple lines or curves.
    /// <para></para>
    /// - By default, shapes are drawn with the Join Type that makes the most sense for the shape.  For examples, ellipses are drawn with a "round" join type, as well as 
    /// Curves.  However, DrawPie(), Triangle(), and most other functions use the default "miter" join edge, which can be over-sharp at times. 
    /// <para></para>
    /// - Switching to a different "line join type" can soften the sharper areas of a shape, or make them sharper and more edgy where desired.
    /// <para></para>
    /// Join Types:
    /// <para></para>
    ///  - Miter (also MiterClipped) -- Sharp edges, which can be very sharp when elements are drawn at small angles. When clipped, the join type switches to "bevel" on very sharp edges.
    /// <para></para>
    ///  - Bevel (or Beveled) --  Join edges are beveled, e.g. an ellipse with a sharp angle will have a flat edge.
    /// <para></para>
    ///  - Round -- Join edges are rounded.  This can create a soft look to shapes such as pie charts, arc, etc.
    /// </summary>
    /// <param name="eLineJoinType"> - Line Join Type.  Sage::LineJoinType (e.g. LineJoingType::Round) may be used.  Strings may also be used, such as "miter clipped". 
    /// Types are Miter, MiterClipped, Round, Bevel.></param>
    /// <returns></returns>
    /// 
    
    kwType::ckw JoinType(LineJoinType eLineJoinType);
    /// <summary>
    /// Sets a "line join" type for drawn shapes via the GDI, such as Triangles, Ellipses, Curves, etc.,
    /// i.e. any shapes that are drawn with multiple lines or curves.
    /// <para></para>
    /// - By default, shapes are drawn with the Join Type that makes the most sense for the shape.  For examples, ellipses are drawn with a "round" join type, as well as 
    /// Curves.  However, DrawPie(), Triangle(), and most other functions use the default "miter" join edge, which can be over-sharp at times. 
    /// <para></para>
    /// - Switching to a different "line join type" can soften the sharper areas of a shape, or make them sharper and more edgy where desired.
    /// <para></para>
    /// Join Types:
    /// <para></para>
    ///  - Miter (also MiterClipped) -- Sharp edges, which can be very sharp when elements are drawn at small angles. When clipped, the join type switches to "bevel" on very sharp edges.
    /// <para></para>
    ///  - Bevel (or Beveled) --  Join edges are beveled, e.g. an ellipse with a sharp angle will have a flat edge.
    /// <para></para>
    ///  - Round -- Join edges are rounded.  This can create a soft look to shapes such as pie charts, arc, etc.
    /// </summary>
    /// <param name="eLineJoinType"> - Line Join Type.  Sage::LineJoinType (e.g. LineJoingType::Round) may be used.  Strings may also be used, such as "miter clipped". 
    /// Types are Miter, MiterClipped, Round, Bevel.></param>
    /// <returns></returns>
    kwType::ckw JoinType(const char * sLineJoinType);
    
    /// @WinGradient
    /// <summary>
    /// Sets the gradient for a drawn shape, based on the input rectangle.  If there is no input rectangle, the window size is used as the input rectangle.
    /// <para></para>
    /// The gradient angle may also be specified.  The gradient is rotated around the input rectangle, expanding and contracting to fit at the given angle.
    /// <para></para>
    /// - Specifying a gradient can be useful, as it provides a static gradient for any drawn item.  For drawn items, the default is to calculate the gradient colors around
    /// the space of the object, which can cause color fluctuations as the object may be moved, or similar objects are drawn.
    /// <para></para>
    /// By passing a gradient rectangle to a draw function, the same gradient will be used for any object drawn with this gradient.
    /// </summary>
    /// <param name="point"> - [optional] Start point of the rectangle (this may also be a Gdi::Rect or SizeRect)</param>
    /// <param name="size"> - [optional] size of the rectangle (omitted of Gdi::Rect or SizeRect is used)</param>
    /// <param name="fAngleDeg"> - [optional] Angle of the gradient.  0 degrees is left-to right, 90 degress is top-to-bottom, etc.  Default is 0</param>
    /// <returns></returns>
    kwType::ckw WinGradient(const POINT point,const POINT size,float fAngleDeg = 0.0f);

    /// @SetGradient
    /// <summary>
    /// Sets the gradient for a drawn shape, based on the input rectangle.  If there is no input rectangle, the window size is used as the input rectangle.
    /// <para></para>
    /// The gradient angle may also be specified.  The gradient is rotated around the input rectangle, expanding and contracting to fit at the given angle.
    /// <para></para>
    /// - Specifying a gradient can be useful, as it provides a static gradient for any drawn item.  For drawn items, the default is to calculate the gradient colors around
    /// the space of the object, which can cause color fluctuations as the object may be moved, or similar objects are drawn.
    /// <para></para>
    /// By passing a gradient rectangle to a draw function, the same gradient will be used for any object drawn with this gradient.
    /// </summary>
    /// <param name="point"> - [optional] Start point of the rectangle (this may also be a Gdi::Rect or SizeRect)</param>
    /// <param name="size"> - [optional] size of the rectangle (omitted of Gdi::Rect or SizeRect is used)</param>
    /// <param name="fAngleDeg"> - [optional] Angle of the gradient.  0 degrees is left-to right, 90 degress is top-to-bottom, etc.  Default is 0</param>
    /// <returns></returns>
    kwType::ckw WinGradient(Gdiplus::Rect rect,float fAngleDeg = 0.0f);

    /// @WinGradient
    /// <summary>
    /// Sets the gradient for a drawn shape, based on the input rectangle.  If there is no input rectangle, the window size is used as the input rectangle.
    /// <para></para>
    /// The gradient angle may also be specified.  The gradient is rotated around the input rectangle, expanding and contracting to fit at the given angle.
    /// <para></para>
    /// - Specifying a gradient can be useful, as it provides a static gradient for any drawn item.  For drawn items, the default is to calculate the gradient colors around
    /// the space of the object, which can cause color fluctuations as the object may be moved, or similar objects are drawn.
    /// <para></para>
    /// By passing a gradient rectangle to a draw function, the same gradient will be used for any object drawn with this gradient.
    /// </summary>
    /// <param name="point"> - [optional] Start point of the rectangle (this may also be a Gdi::Rect or SizeRect)</param>
    /// <param name="size"> - [optional] size of the rectangle (omitted of Gdi::Rect or SizeRect is used)</param>
    /// <param name="fAngleDeg"> - [optional] Angle of the gradient.  0 degrees is left-to right, 90 degress is top-to-bottom, etc.  Default is 0</param>
    /// <returns></returns>
    kwType::ckw WinGradient(float fAngleDeg);

    /// @SetPenGradient
    /// <summary>
    /// Sets the gradient for a drawn shape's pen, based on the input rectangle.  If there is no input rectangle, the window size is used as the input rectangle.
    /// <para></para>
    /// The gradient angle may also be specified.  The gradient is rotated around the input rectangle, expanding and contracting to fit at the given angle.
    /// <para></para>
    /// - Specifying a gradient can be useful, as it provides a static gradient for any drawn item.  For drawn items, the default is to calculate the gradient colors around
    /// the space of the object, which can cause color fluctuations as the object may be moved, or similar objects are drawn.
    /// <para></para>
    /// By passing a gradient rectangle to a draw function, the same gradient will be used for any object drawn with this gradient.
    /// <para></para>
    /// ** note:  For shapes that only used a pen (i.e. non-filled shapes), SetGradient() and SetPenGradient() are the same function.
    /// </summary>
    /// <param name="point"> - [optional] Start point of the rectangle (this may also be a Gdi::Rect or SizeRect)</param>
    /// <param name="size"> - [optional] size of the rectangle (omitted of Gdi::Rect or SizeRect is used)</param>
    /// <param name="fAngleDeg"> - [optional] Angle of the gradient.  0 degrees is left-to right, 90 degress is top-to-bottom, etc.  Default is 0</param>
    /// <returns></returns>
    
    kwType::ckw WinPenGradient(const POINT point,const POINT size,float fAngleDeg = 0.0f);
    /// @WinPenGradient
    /// <summary>
    /// Sets the gradient for a drawn shape's pen, based on the input rectangle.  If there is no input rectangle, the window size is used as the input rectangle.
    /// <para></para>
    /// The gradient angle may also be specified.  The gradient is rotated around the input rectangle, expanding and contracting to fit at the given angle.
    /// <para></para>
    /// - Specifying a gradient can be useful, as it provides a static gradient for any drawn item.  For drawn items, the default is to calculate the gradient colors around
    /// the space of the object, which can cause color fluctuations as the object may be moved, or similar objects are drawn.
    /// <para></para>
    /// By passing a gradient rectangle to a draw function, the same gradient will be used for any object drawn with this gradient.
    /// <para></para>
    /// ** note:  For shapes that only used a pen (i.e. non-filled shapes), SetGradient() and SetPenGradient() are the same function.
    /// </summary>
    /// <param name="point"> - [optional] Start point of the rectangle (this may also be a Gdi::Rect or SizeRect)</param>
    /// <param name="size"> - [optional] size of the rectangle (omitted of Gdi::Rect or SizeRect is used)</param>
    /// <param name="fAngleDeg"> - [optional] Angle of the gradient.  0 degrees is left-to right, 90 degress is top-to-bottom, etc.  Default is 0</param>
    /// <returns></returns>
    kwType::ckw WinPenGradient(Gdiplus::Rect rect,float fAngleDeg = 0.0f);

    /// @WinPenGradient
    /// <summary>
    /// Sets the gradient for a drawn shape's pen, based on the input rectangle.  If there is no input rectangle, the window size is used as the input rectangle.
    /// <para></para>
    /// The gradient angle may also be specified.  The gradient is rotated around the input rectangle, expanding and contracting to fit at the given angle.
    /// <para></para>
    /// - Specifying a gradient can be useful, as it provides a static gradient for any drawn item.  For drawn items, the default is to calculate the gradient colors around
    /// the space of the object, which can cause color fluctuations as the object may be moved, or similar objects are drawn.
    /// <para></para>
    /// By passing a gradient rectangle to a draw function, the same gradient will be used for any object drawn with this gradient.
    /// <para></para>
    /// ** note:  For shapes that only used a pen (i.e. non-filled shapes), SetGradient() and SetPenGradient() are the same function.
    /// </summary>
    /// <param name="point"> - [optional] Start point of the rectangle (this may also be a Gdi::Rect or SizeRect)</param>
    /// <param name="size"> - [optional] size of the rectangle (omitted of Gdi::Rect or SizeRect is used)</param>
    /// <param name="fAngleDeg"> - [optional] Angle of the gradient.  0 degrees is left-to right, 90 degress is top-to-bottom, etc.  Default is 0</param>
    /// <returns></returns>
    kwType::ckw WinPenGradient(float fAngleDeg);

    /// <summary>
    /// When creating a window, using kw::AutoCapture() will tell the window to automatically capture the mouse when the mouse button is pressed.
    /// <para></para>
    /// This can be useful when using the mouse in windows, allowing mouse input outside of the window when the mouse button is pressed.  The default is to not capture the mouse, which does not allow mouse input outside of the window.
    /// <para></para>
    /// ** See CWindow::SetAutoMouseCapture() for more information
    /// </summary>
    /// <param name="bAutoMouseCapture"></param>
    /// <returns></returns>
    kwType::ckw AutoCapture(bool bAutoMouseCapture = true);


    /// <summary>
    /// For Mouse Regions, this sets the characteristics for either all mouse region points/regions or individually, depending on use.
    /// <para></para>
    /// This allows the ability to set shapes, colors, outline thickness, and a number of other characteristics.
    /// <para></para>
    /// ** note ** - Documentation for this feature is still in development.  See various example for usage information.
    /// </summary>
    kwType::ckw AutoDraw(const char * sString = nullptr);

    /// <summary>
    /// When using a MouseRegion, BindtoWindow() causes the mouse region/point to be limited to the current window size.
    /// If the window size changes, the limit of the mouse point adjusts to the new window size.
    /// <para></para>
    /// --> See: kw::BoundBox() to set a specific boundary for mouse points.
    /// <para></para>
    /// --> Note: For region-type mouse regions, the entire mouse region is limited to the window size.
    /// <para></para>
    /// However, for point-type regions, only the center point (i.e. the reported mouse point) is limited to the window boundaries. 
    /// <para></para>
    /// With point-type regions, the width/height around the point (i.e. X- and Y- radius, respectively, will overlap the window edges. 
    /// </summary>
    /// <param name="bBindtoWindow"></param>
    /// <returns></returns>
    kwType::ckw BindtoWindow(bool bBindtoWindow = true);

    /// <summary>
    /// For write() function and other output (such as some drawing functions), SetRegion() sets a measurement region, but does not clip the results.
    /// <para></para>
    /// --> For example, with a write() to the window, if a justification, such as kw::CenterXY() is used, the output is measured against the region and
    /// placed accordingly.
    /// <para></para>
    /// --> Example: 
    /// <para></para>
    /// >>>>> SizeRect MyBox = (400,300,400,500); // Region starting at (400,300) with width and height of (400,500)
    /// <para></para>
    /// >>>>> Write("Hello World",kw::CenterXY() + kw::SetRegion(MyBox));
    /// <para></para>
    /// --> To also clip the results so they it will not exceed the region, use kw::ClipRegion()
    /// <para></para>
    /// --> For general widow clipping, see the window function CWindow::ClipWindow();
    /// </summary>
    /// <param name="szRegion"></param>
    /// <returns></returns>
    kwType::ckw SetRegion(const SizeRect & szRegion); 
    
    /// <summary>
    /// For write() function and other output (such as some drawing functions), ClipRegion() sets a measurement region, also cipping the output, limiting
    /// output to the clip region.
    /// <para></para>
    /// --> For example, with a write() to the window, if a justification, such as kw::CenterXY() is used, the output is measured against the region and
    /// placed accordingly.
    /// <para></para>
    /// --> Example: 
    /// <para></para>
    /// >>>>> SizeRect MyBox = (400,300,400,500); // Region starting at (400,300) with width and height of (400,500)
    /// <para></para>
    /// >>>>> Write("Hello World",kw::CenterXY() + kw::ClipRegion(MyBox));
    /// <para></para>
    /// --> To use a region for measurement, without clipping, use kw::SetRegion()
    /// <para></para>
    /// --> For general widow clipping, see the window function CWindow::ClipWindow();
    /// </summary>
    /// <param name="szRegion"></param>
    /// <returns></returns>
    kwType::ckw ClipRegion(const SizeRect & szRegion); 

    /// <summary>
    /// For Mouse Regions, kw::DontPromote() tells the Mouse Region functions not to promote a mouse region or point when it has been clicked upon
    /// and/or moved by the mouse.
    /// <para></para>
    /// --> By default, when a mouse region/point it selected (i.e. clicked on by the mouse), the region/point is promoted in the display order (not the
    /// region order, just the display order). 
    /// <para></para>
    /// --> To prevent the point from being promoted, use kw::DontPromote() when adding the region/point.
    /// </summary>
    /// <param name="bNoPromote"></param>
    /// <returns></returns>
    kwType::ckw DontPromote(bool bNoPromote = true); 

    /// <summary>
    /// Causes an update to the window.  This can be used in write() functions to ensure the window is updated 
    /// before returning.
    /// <para></para>
    /// This is useful when writing text before performing operations that may take some time, used to avoid
    /// the need to call a separate Update() call after the Write().
    /// <para></para>
    /// Example: Write("Hello World\n",kw::UpdateNow());  vs. using an Update() call right afterward.
    /// </summary>
    /// <param name="bUpdateNow"></param>
    /// <returns></returns>
    kwType::ckw UpdateNow(bool bUpdateNow = true); 
} // namespace kw
namespace __kw_priv
{
    kwType::ckw __priv_CenterDesktop (bool bCenterDesktop = true);
    kwType::ckw __priv_Embed(bool bEmbed = true);
    kwType::ckw __priv_AddShadow(bool bAddShadow = true);
    kwType::ckw __priv_xransparent(bool bxransparent = true);
    kwType::ckw __priv_LocalWindow(bool bLocalWindow = true);
    kwType::ckw __priv_QuickCPP(bool __bpriv_QuickCPP = true)     ;
    kwType::ckw __priv_DebugMode(bool __bpriv_DebugMode = true)     ;
    kwType::ckw __priv_NonPrimary(bool __bpriv_NonPrimary = true)    ;
    kwType::ckw Child(bool bChild = true);
    kwType::ckw Raised(bool bRaised = true);
    kwType::ckw Depressed(bool bDepressed = true);
    kwType::ckw Sunken(bool bSunken = true);
    kwType::ckw NoBorder2(bool bNoBorder2 = true);
    kwType::ckw DefaultColors(bool bDefaultcolors = true);
    kwType::ckw NoCaption(bool bNoCaption = true);
    kwType::ckw Toolwindow(bool bToolwindow = true);
    kwType::ckw DoubleClick(bool bDoubleClick = true);
    kwType::ckw NoTaskbar(bool bNoTaskbar = true);
    kwType::ckw NoOwner(bool bNoOwner = true);
    kwType::ckw NoCursor(bool bNoCursor = true);
    kwType::ckw DropShadow(bool bDropShadow = true);
    kwType::ckw NoTitle(bool bNoTitle = true);
    kwType::ckw TextOnly(bool bTextOnly = true);
    kwType::ckw AsDialog(bool bAsDialog = true);
    kwType::ckw Bottom(bool bBottom = true);           
    kwType::ckw Right(bool bRight = true);  
    kwType::ckw WinToolTip(bool bWinToolTip = true);      
    kwType::ckw EnableFocusBox(bool bEnableFocusBox = true);      
    kwType::ckw __NoExdent(bool bNoExdent = true); 
    kwType::ckw __TempEmptyOk(bool bTempEmptyOk = true); 
    kwType::ckw __EmptyBlankErr(bool bEmptyBlankErr = true); 
    kwType::ckw Checkbox(bool bCheckbox = true); 
    kwType::ckw RadioButton(bool bRadioButton = true); 
    kwType::ckw ValidateGroup(const char * sGroup); 
    kwType::ckw ValidateGroupNum(int iGroup); 
    kwType::ckw RadioGroup(const char * sRadioGroup); 
    kwType::ckw RadioGroupID(int iRadioGroup); 
    kwType::ckw DefaultTitle(const char * sTitle); 
    kwType::ckw UseEventThread(bool bUseEventThread = true); 
    kwType::ckw Defer(bool bDefer = true); 


}   // namespace priv
}