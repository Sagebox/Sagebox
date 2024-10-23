#include "Pendulum.h"

// DoublePendulum Class using Sagebox
//
// Most of this code was copied and adapted from various internet sources.
//
// --------------------------
// Sagebox Graphics Functions
// --------------------------
//
// Sagebox graphics functions comprise 5-6 lines of code to draw the rods, bobs, and pendulum trail.
//
// Otherwise, the DoublePendulum class simply calculates and maintains the pendulum.
//
// See the Render Function for Sagebox code.
//
// ---------------------
// Floating-Point Errors
// ---------------------
//
// With a high mass ratio (i.e. the bottom bob is much heavier than the top bob) and/or low dampening settings (i.e. close to 1.0 -- very little friction), math errors can occur
// where the pendulum speeds up out of control.  This is due to a low sampling rate and some math than needs to be added to modulate the energy being generated in the pendulum when 
// it exceeds is bounds.
//
// These errors can also happen when the top bob is very close to the peg and the second bob is closed to the first one.
// 
// One answer may be to decrease the dt time-slice and call Update() 5-6 times in a row for a better resolution.  
//
// In the meantime, I added some code to compare the Max Potential Energy with the current Kinetic Energy.  If the Ke > Pe then the algorithm starts applying dampening.
// I think the real solution is to determine when the math has caused the pendulum to have more Pe than it should have at any given state. 
//
// But, I just copied this code for a Sagebox/Pybox demo and haven't work with it much, though it is compelling!
//

DoublePendulum::DoublePendulum(CWindow & cWin,ftype length1, ftype length2, ftype mass1, ftype mass2, ftype angle1, ftype angle2, ftype dampen, ftype PegY) : m_cWin(cWin)
{
    m_fLength[0]    = length1;
    m_fLength[1]    = length2;
    m_fMass[0]      = mass1;
    m_fMass[1]      = mass2;
    m_fAngle[0]     = angle1*(ftype) Sage::Math::PI/(ftype) 180.0;
    m_fAngle[1]     = angle2*(ftype) Sage::Math::PI/(ftype) 180.0;
    m_fDamp1        = dampen;
    m_fDamp2        = dampen;

    m_RodVertex[0] = (pointType) cWin.GetWindowSize()/2 * pointType(1,PegY*2); 

}

// Draw the pendulum out to the window. 
//
void DoublePendulum::Render()
{
   m_RodVertex[1] = m_fPos1*m_fZoom + m_RodVertex[0];
   m_RodVertex[2] = m_fPos2*m_fZoom + m_RodVertex[0];

    //Trails

    int iTrailSize = (int) m_vTrails.size();
    if (!m_bPause)
    {
        if (iTrailSize < m_iMaxTrailSize) m_vTrails.push_back({m_fPos2, kLineColor*iTrailSize/m_iMaxTrailSize}); 
        else
        {
            for (int i=0; i<iTrailSize-1; i++) m_vTrails[i].position = m_vTrails[i+1].position;
            m_vTrails[iTrailSize-1].position = m_fPos2;
        }
    }
 
    // Draw the lines.  The DrawLineToEx() call below does a Draw To, except for the first call where it just sets the point. 
    //                  This allows just the one call so we can use the STL loop format (otherwise we'd have to get the size and
    //                  independently address each member with size()-1) loop.
  
    int iCount = 0;

    if (!m_bSinglePend && m_bShowTrail)
    for (auto & t : m_vTrails) m_cWin.DrawLineToEx(!(bool) (iCount++),t.position*m_fZoom+m_RodVertex[0],t.color,kw::PenSize(fTrailThickness)); 

    // Draw Rods, Peg, and both pendulums at various zoom/thickness/size factors. 

    m_cWin.DrawLine(m_RodVertex[0],m_RodVertex[1],SageColor::White,kw::PenSize(m_fThickMul*fLineThickness*m_fZoom)); 
    if (!m_bSinglePend) m_cWin.DrawLine(m_RodVertex[1],m_RodVertex[2],SageColor::Cyan,kw::PenSize(m_fThickMul*fLineThickness*m_fZoom)); 
    m_cWin.FillCircle_f(m_RodVertex[0],m_fThickMul*fPegRadius*m_fZoom,SageColor::White);
    m_cWin.FillCircle_f(m_RodVertex[1],fTopRadius*m_fZoom*m_fCircleMult,SageColor::Red,kw::PenSize(2.5));
    if (!m_bSinglePend) m_cWin.FillCircle_f(m_RodVertex[2],fBotRadius*m_fZoom*m_fCircleMult,SageColor::Green,kw::PenSize(2.5));
}

// Update the pendulum angular velocity and angles. 
//
// This routine was largely copied from Internet sources, except for some observational changes to make it work
// with the 60fps loop better, as well as the overflow checking below. 
//
void DoublePendulum::Update()
{
    if (m_bPause) return;

    auto fLength1 = m_fLength[0]/(m_fWeightMul*m_fLineMul);;
    auto fLength2 = m_fLength[1]/(m_fWeightMul*m_fLineMul);;

    auto fMass1 = m_fMass[0]/10.0;
    auto fMass2 = m_fMass[1]/10.0;

    if (m_bSinglePend) fMass2 = 0;

    auto fAngle1 = m_fAngle[0];
    auto fAngle2 = m_fAngle[1];

    // Code pretty much copied from the internet. 

    ftype dt = this->m_fdt*this->m_fdt*m_fWeightMul;
    ftype n11 = -gravity*(2.0f*fMass1+fMass2)*std::sin(fAngle1);
    ftype n12 = -fMass2*gravity*std::sin(fAngle1-2.0f*fAngle2);
    ftype n13 = -2.0f*std::sin(fAngle1-fAngle2) * fMass2;
    ftype n14 = (m_fAngVel2*m_fAngVel2*fLength2 + m_fAngVel1*m_fAngVel1*fLength1*std::cos(fAngle1-fAngle2));
    ftype den = 2.0f*fMass1+fMass2-fMass2*std::cos(2.0f*fAngle1-2.0f*fAngle2);
    ftype n21 = 2.0f*std::sin(fAngle1-fAngle2);
    ftype n22 = m_fAngVel1*m_fAngVel1*fLength1*(fMass1+fMass2);
    ftype n23 = gravity*(fMass1+fMass2)*std::cos(fAngle1);
    ftype n24 = m_fAngVel2*m_fAngVel2*fLength2*fMass2*std::cos(fAngle1-fAngle2);

    m_fAngAccel1 = (n11+n12+n13*n14)/(fLength1*den*dt);
    m_fAngAccel2 = (n21*(n22+n23+n24))/(fLength2*den*dt);

    m_fAngVel1 += m_fAngAccel1;
    m_fAngVel2 += m_fAngAccel2;

    m_fAngle[0] += m_fAngVel1;
    m_fAngle[1] += m_fAngVel2;

    // Keep the angle within +/- 360 degrees to keep the floating-point resolution as high as possible.

    if (m_fAngle[0] > PI*2)  m_fAngle[0] -= PI*2;
    if (m_fAngle[1] > PI*2)  m_fAngle[1] -= PI*2;
    if (m_fAngle[0] < -PI*2) m_fAngle[0] += PI*2;
    if (m_fAngle[1] < -PI*2) m_fAngle[1] += PI*2;
    
    UpdatePos();

    m_fAngVel1 *= m_fDamp1*m_fOverflowMul;        // Apply dampening (i.e. friction)
    m_fAngVel2 *= m_fDamp2*m_fOverflowMul;

    double v1 = m_fAngVel1/dt;
    double v2 = m_fAngVel2/dt;

    // Check Max Potential Energy vs. Kinetic energy.  Look for overflows when Ke > Pe
    // 
    // Overflows occur because of inaccurate calculations due to time-slice resolution which can 
    // cause the pendulum to move too fast when there is little or no dampening. 
    // This routine works for a 60fps reference with 1 sample per-frame, which is a pretty big dt. 
    // The proper method is to probably call Update() 5-10 times in succession and then render it, rather than
    // just the one time.

    // Note: This equation works, but I am not completely sure why because the v1/v2 calculations are not 
    //       really correct -- the dt division is not right, so it's probably a happy accident.

    double Pe2 = -(fMass1+fMass2)*gravity*fLength1-fMass2*gravity*fLength2; 
    double Ke2 = .5*fMass1*v1*v1*fLength1*fLength1 + .5*fMass2*(v1*v1*fLength1*fLength1 + 
                            v2*v2*fLength2*fLength2 + 2*v1*fLength1*v2*fLength2*cos(m_fAngle[0]-m_fAngle[1])); 

    // If the Kinetic energy went over the max potential energy then slow the pendulum down.
    // If we have seen more than two overflows in this session than add some dampening to 
    // keep slowing it down. 

    if (fabs(Ke2) > fabs(Pe2))
    {
        double fRatio = fabs(Pe2/Ke2); 
        m_iOverflowCount++;
        m_fAngVel1 *= fRatio*.65; 
        m_fAngVel2 *= fRatio*.65; 

        // Start slowing it down if we've seen more than 2 hits.

        if (m_iOverflowCount > 2) 
        {
            m_fOverflowMul *= m_fOverflowMul; 
            if (m_fOverflowMul == 1) m_fOverflowMul = .9999; 
        }
    }
}

// Update the pendulum bob positions.
//
void DoublePendulum::UpdatePos()
{
    m_fPos1 = pointType{ std::sin(m_fAngle[0]), std::cos(m_fAngle[0]) } * m_fLength[0];
    m_fPos2 = m_fPos1 + pointType{ std::sin(m_fAngle[1]), std::cos(m_fAngle[1]) } * m_fLength[1];
}