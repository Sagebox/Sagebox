#pragma once
#include "Sagebox.h"

class DoublePendulum
{

public:
    // Set point and ftype.  They were originally float, but the resolution of the float32 was causing issues, so it was moved to double
    // and eventually ftype so later on a higher resolution float can be used, such as on a system that supports 96-bit+floats, which MSVC doesn't
    // (and Intel does but down-converts to double anyway)
    
    using pointType     = CfPoint; 
    using ftype         = double;

    // A number of values are just observational for graphics output. 

    ftype       m_fWeightMul      = .05;          // Observational.  Helps with a more realistic wobbling and gravity loss effect.  
                                              // But can be unstable with floating-point issues.
    ftype       m_fLineMul        = 4.0;          // More observational 
    ftype       m_fThickMul       = 1.0;          // Line thickness multiplier for rods (display)

    ftype       m_fLength[2]      = { 0, 0};
    ftype       m_fMass[2]        = { 0, 0};
    ftype       fLineThickness    = 3.0;          // Starting Line thickness
    ftype       fTrailThickness   = 4.0;          // Line thickness for trails
    ftype       fPegRadius        = 5.0;          // Peg Radius
    ftype       fTopRadius        = 23;           // Radius for bobs (changeable with slider)
    ftype       fBotRadius        = 23;
    int         m_iMaxTrailSize   = 300;          // Max trail point size in the display
    ftype       m_fCircleMult     = 1.0;          // Bob size multiplier for changing display size of bobs
    ftype       m_fZoom           = 1.0;          // Zoom factor for display
    int         m_iOverflowCount  = 0;            // When > 0 an overflow was detected and shut down.   (floating-point resolution issue)
    bool        m_bPause          = false;           // When true, the display and update is frozen.
    bool        m_bShowTrail      = true;
    bool        m_bSinglePend     = false;
    pointType   m_RodVertex[3]{};

    ftype m_fAngle[2]       = { 0, 0};
    ftype m_fDamp1          = .9985;         // Dampening, i.e. friction
    ftype m_fDamp2          = .9985;  

    ftype m_fOverflowMul    = 1;

    ftype m_fAngAccel1      = 0;
    ftype m_fAngAccel2      = 0;
    ftype m_fAngVel1        = 0;
    ftype m_fAngVel2        = 0;

    // Vertex type for trail display containing a position and color
    // Used so the ends of the trail can dissipate.

    struct Vertex
    {
       pointType position; 
       RgbColorA color; 
    };

 
    std::vector<Vertex> m_vTrails;      // Vector for trails

private:
    inline static RgbColor kLineColor = { 255, 50, 0 };     // Color to draw the rods

    static constexpr const ftype gravity    = 9.8;
    static constexpr const ftype FPS        = 60.0;
    static constexpr const ftype m_fdt      = 400.0f*(1.0/FPS);     // 400.0 is just an observational fit.
    static constexpr const ftype PI         = 3.141592653589793238f;
private:
    CWindow & m_cWin;                   // Window for drawing the pendulum
    pointType m_fPos1,m_fPos2;

public:
    void Update();
    void UpdatePos();
    void Render();

    // Reset all moving values. 

    void ResetOverflow() { m_iOverflowCount = 0; m_fOverflowMul = 1; };
    void Reset() 
    { 
        m_fAngVel1 = m_fAngVel2 = m_fAngAccel1 = m_fAngAccel2 = 0;
        ResetOverflow();
        UpdatePos();        // Just in case something changes that affects the display. 
        m_vTrails.clear();        
    }

    DoublePendulum(CWindow & cWin,ftype length1, ftype length2,ftype mass1, ftype mass2,ftype angle1, ftype angle2,  ftype dampen, ftype pegY);
};