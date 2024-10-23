#pragma once

#include "Sagebox.h"

namespace Sage
{
class TurtleShell
{
public:
    enum class QualityType
    {
        Default,            // Default = High
        Normal,             // Draws fill areas over pen strokes
        High,               // Redraws pen strokes over fill areas (draws pen strokes if display/sleep mode is active)
    };


private:
    static constexpr int kMaxDrawTrackSize  = 1024*1024;   // up to a million points. 

    int m_iDrawTrackCount = 0;

    enum class DrawType
    {
        Line,
        Ellipse,
    };

    enum class FillGradientType
    {
        None,
        Radial,
        Rectangular,
    };
    struct DrawTrack
    {
        DrawType drawType;
        RgbColorA rgbColor;
        float fPenSize;
        CfPointf p1;
        CfPointf p2;
        int iRadiusX;
        int iRadiusY;
        int iSweepAngle;
        float fStartAngle;
 
    };


    DrawTrack * m_drawTrack{}; 


    CWindow & m_cWin;
    float m_fAngle          = 0.0f;      // Degrees
    float m_fPenSize        = 2.0f; 

    RgbColorA m_rgbColor    = SageColorA::White();      
    RgbColorA m_rgbFill1    = SageColorA::Red(128);     // Main fill color (set by startfill, etc.) 
    RgbColorA m_rgbFill2    = RgbA(RgbVal::Undefined); // $$ need RgbValA::Undefined here. // Secondary color if using a gradient for the fill color

    RgbColorA m_rgbGrad1{};  // Main fill color (set by startfill, etc.) 
    RgbColorA m_rgbGrad2{}; // $$ need RgbValA::Undefined here. // Secondary color if using a gradient for the fill color

    bool        m_bPenDown = true;  // $$ Don't currently know what the default should be
    CfPointf    m_fLoc{};
    CfPointf    m_fCenter{};

    bool        m_bInFill = false;
    int         m_iCurSleepCount = 0;
    int         m_iSleepCount = 0;
    int         m_iSleep = 25; 

    FillGradientType m_eFillGradientType{}; 

    CfPointf m_pGradient{};
    CfPointf m_pGradientSize{};
    Gdiplus::GraphicsPath * m_gdiPath = nullptr;
    Gdiplus::Graphics * m_cGdi = nullptr;
    CBitmap m_cWinBitmap; 

    using real = Gdiplus::REAL;
    using byte = BYTE;

    bool m_bRedrawFillPaths     = true;         // When true, this returns the window to its original contents before the startfill() function,
                                                // draws the fill areas, then redraws the paths over the fill areas.
                                                //
                                                // When false, this just allows the fill to occur over the drawing areas (e.g. lines/pen areas vs. brush/fill areas)
                                                
    __forceinline bool FillOk() { return m_bInFill && m_cGdi && m_gdiPath; }

    void CheckSleep(bool bOverridePenDown = false);
    bool AddDrawTrackLine(const CfPointf & p1,const CfPointf & p2);
    bool AddDrawTrackEllipse(const CfPointf & p,int iRadiusX,int iRadiusY,int iSweepAngle,float fStartAngle);

public:
    TurtleShell(CWindow & cWin);
    ~TurtleShell();

    /// <summary>
    /// Lifts the pen up.  Any actions performed with the pen up will not draw..
    /// <para></para>
    /// When the pen is up, no graphics are drawn the the screen, allowing for positioning without graphic output.
    /// <para></para>
    /// Use pen_down() to put graphic output to the window (lines, circles, etc.) 
    /// </summary>
    void pen_up();

    /// <summary>
    /// Puts the pen down.  This causes any graphic output function (e.g. circle, forward, etc.) to draw grahics to the screen.
    /// When the pen is up (e.g. pen_up() function), no graphics are drawn the the screen, allowing for positioning without graphic output.
    /// </summary>
    void pen_down();

    /// <summary>
    /// Starts a gradient from rectangle corners defined by upper-left point &lt;p1&gt;, of size &lt;pSize&gt; (width and height).
    /// <para></para>
    /// rgbColor sets the color for the gradient, which may be two colors as strings, or two RGB or RGBA colors. 
    /// <para></para>
    /// Example: begin_gradient(p1,Size,"red,green") or begin_gradient("red(128),green(170)") to use colors with transparent qualities (i.e. opacity).
    /// <para></para>
    /// A zero (e.g. 0) opacity value can be used to use one color and fade to compeletely transparent, e.g. "red(128),red(0)", where "red(0)" is basically no color an
    /// is completely transparent.
    /// <para></para>
    /// ** Note: Using gradients is still in-progress and TBD.
    /// </summary>
    /// <param name="p1"> - Upper-left corner of rectangle</param>
    /// <param name="pSize"> - Width and Height of rectangle</param>
    /// <param name="rgbColor"> - Colors of gradient background</param>
    void begin_gradient(CfPointf p1,CfPointf pSize,const CRgbColor rgbColor);

    /// <summary>
    /// Ends a gradient and draws all internal filled gradient areas that were drawn since the gradient was started with begin_gradient().
    /// <para></para>
    /// See: begin_gradient()
    /// <para></para>
    /// ** Note: Using gradients is still in-progress and TBD.
    /// </summary>
    void end_gradient();

    /// <summary>
    /// Starts a filled area enclosed by subsequent drawing functions.
    /// <para></para>
    /// After begin_fill() is called, drawing functions are drawn and remembered.
    /// <para></para>
    /// When end_fill() is called, the area enclosed by the drawing functions is filled with the color.
    /// <para></para>
    /// rgbColor sets the color for the fill. This color may jave an opacity, e.g. "red" (full opacity, the same as "red(255)") or "red(128)" (half-transparent)
    /// </summary>
    /// <param name="rgbColor"> - Color to fill the enclosed area when end_fill() is called, e.g. "green", "green(128)", SageColor::Green, SageColorA::Green(128), etc.</param>
    void begin_fill(CRgbColor rgbColor);

    /// <summary>
    /// When a fill is in-progress after starting it with begin_fill(), calling end_fill() fills the area defined by all drawing calls called after begin_fill().
    /// <para></para>
    /// See begin_fill() for more information.
    /// </summary>
    void end_fill();

    /// <summary>
    /// Sets the color for subsequent drawing operations. 
    /// <para></para>
    /// Example:
    /// <para></para>
    /// t.forward(100); // draw 100 pixels in current heading in current color
    /// <para></para>
    /// t.color("red"); // Set color to "red"
    /// <para></para>
    /// t.forward(100); // draws 100 pixels in current heading in color "red"
    /// <para></para>
    /// ** note: color() and set_color() are the same function
    /// </summary>
    /// <param name="rgbColor"></param>
    void color(CRgbColor rgbColor);

    /// <summary>
    /// Sets the color for subsequent drawing operations. 
    /// <para></para>
    /// Example:
    /// <para></para>
    /// t.forward(100); // draw 100 pixels in current heading in current color
    /// <para></para>
    /// t.color("red"); // Set color to "red"
    /// <para></para>
    /// t.forward(100); // draws 100 pixels in current heading in color "red"
    /// <para></para>
    /// ** note: color() and set_color() are the same function
    /// </summary>
    /// <param name="rgbColor"></param>
    void set_color(CRgbColor rgbColor);

    /// <summary>
    /// Sets the pen radius size for all drawing operations.  The default is 2 pixels.
    /// <para></para>
    /// Example: pen_size(10) sets the thickness for all drawing functions to a radius of 10 pixels.
    /// </summary>
    /// <param name="fPenSize"> - New pen radius</param>
    void pen_size(float fPenSize);

    /// <summary>
    /// Sets the position of the current pointer (or turtle). 
    /// <para></para>
    /// The if the pen is down, a line will be drawn from the current position to the new position. The heading will not change.
    /// <para></para>
    /// Use pen_up() to move to a position without drawing a line. 
    /// <para></para>
    /// This function is the same as "move_to".
    /// </summary>
    /// <param name="x"> - x position of new location</param>
    /// <param name="y"> - y position of new location</param>
    void set_pos(int x,int y);

    /// <summary>
    /// Sets the position of the current pointer (or turtle). 
    /// <para></para>
    /// The if the pen is down, a line will be drawn from the current position to the new position. The heading will not change.
    /// <para></para>
    /// Use pen_up() to move to a position without drawing a line. 
    /// <para></para>
    /// This function is the same as "set_pos".
    /// </summary>
    /// <param name="x"> - x position of new location</param>
    /// <param name="y"> - y position of new location</param>
    void move_to(int x,int y);

    /// <summary>
    /// Rotates the current heading clockwise (i.e. right) the number of degrees in fAngle.
    /// <para></para>
    /// Angle is in degrees.
    /// </summary>
    /// <param name="fAngle"></param>
    void right(float fAngle);

    /// <summary>
    /// Rotates the current heading clockwise (i.e. positive, to the right) the number of degrees in fAngle.
    /// <para></para>
    /// Angle is in degrees.
    /// </summary>
    /// <param name="fAngle"></param>
    void right(int iAngle);

    /// <summary>
    /// Rotates the current heading counter-clockwise (i.e. negative, to the left) the number of degrees in fAngle.
    /// <para></para>
    /// Angle is in degrees.
    /// </summary>
    /// <param name="fAngle"></param>
    void left(float fAngle);

    /// <summary>
    /// Rotates the current heading counter-clockwise (i.e. negative, to the left) the number of degrees in fAngle.
    /// <para></para>
    /// Angle is in degrees.
    /// </summary>
    /// <param name="fAngle"></param>
    void left(int iAngle); 

    /// <summary>
    /// Moves the current location forward at the angle of the current heading, the number of pixels specified.
    /// <para></para>
    /// If the pen is down, a line is drawn from the current point to the next point.
    /// <para></para>
    /// See pen_up() to move forward without using the pen.
    /// <para></para>
    /// Also see: move_to(), which moves directly to a specified point.
    /// </summary>
    /// <param name="iPixels"></param>
    void forward(int iPixels);

    /// <summary>
    /// Draws a circle, arc, polygon or polygonal-arc, depending on input values.
    /// <para></para>
    /// The heading is change from the current heading to the angle of the output of the circle or last polygonal line.
    /// <para></para>
    /// If no Sweep Angle is entered, a full circle is drawn. Otherwise in arc is drawn.
    /// <para></para>
    /// The "Steps" value, draws the circle as a polygon with sides equal to the "Steps" value, drawing lines from point to point, rather than a circle.
    /// </summary>
    /// <param name="iRadius"> - Radius of Circle (or partial circle/arc)</param>
    /// <param name="iSweepAngle"> - Angle to cover in circle (blank is 360, or a full circle)</param>
    /// <param name="iSteps"> - Number of segments to draw for circle or arc (no value or 0 draws a circle or arc without segment lines)</param>
    void circle(int iRadius,int iSweepAngle = 360,int iSteps = 0);

    /// <summary>
    /// Sets the current heading for draw operations (i.e. heading of the turtle).
    /// <para></para>
    /// Functions such as right(), left(), circle(), etc. adjust the heading accordingly.
    /// <para></para>
    /// set_heading() sets a specific heading (value is in degrees)
    /// </summary>
    /// <param name="fAngle"> - Angle (in degrees) of new heading.</param>
    void set_heading(float fAngle);

    /// <summary>
    /// Sets the speed of drawing operations.  Unlike classical Turtle Graphics, Turtle Shell currently draws entire elements at a time, such as a line, circle, etc.
    /// The input value sets the delay between showing drawn values in milliseconds.
    /// <para></para>
    /// For example, setting speed(20) sets 20ms between showing drawn output.
    /// <para></para>
    /// Setting speed(.5) sets half a millisecond.
    /// <para></para>
    /// ** note: speed(0) is a special case and will not show any drawing operations, relying on the program to update the window itself.  
    /// <para></para>
    /// ** This can be used for real-time drawing, to display the entirety of the results all at one time, not using time to display partial results.
    /// <para></para>
    /// ** Use window.set_realtime() (or set kw::Realtime() as a keyword when creating a window) to set Realtime() status for a window, for smoother real-time output. 
    /// </summary>
    /// <param name="fspeed"></param>
    void set_speed(float fspeed);

    /// <summary>
    /// Returns the current heading of the 'turtle'
    /// </summary>
    /// <returns> - Current heading.</returns>
    float get_heading();

    /// <summary>
    /// Returns the current position of the 'turtle' relative to the center of the window.
    /// </summary>
    /// <returns> - Current position.</returns>
    CfPointf get_pos();
 
}; // class TurtleShell

} // namespace Sage