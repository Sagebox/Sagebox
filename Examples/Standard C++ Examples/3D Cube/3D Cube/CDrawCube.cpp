// CDrawCube.cpp -- Copyright(c) 2020, 2021 Rob Nelson.  robnelsonxx2@gmail.com -- all rights reserved.
// This file, information, and process within are for personal use only and may not be distributed without permission.
// Please modify, copy, do whatever you want for personal uses.  For professional, distribution or commercial uses, 
// contact the e-mail address above

// *****************************************
// SageBox 3D Interactive Cube Demonstration
// *****************************************
//
// This program draws a 3-D cube that can be rotated with the mouse and sized up/down with the mousehweel.
//
// The program also puts specular highlights on the cube as it is rotated. 
//
// This shows how SageBox can be used for graphics programs and more complex math.
// The program is fairly simple, the only extended set of code being the code that calculates the diffusion.
//
// ---------------------------------------
// SageBox Features Shown in this Program
// ---------------------------------------
//
// This program is very short, so just look around and see what SageBox functions are used.  They are usually
// self-explanatory and self-documenting.  In most cases, you can hold the mouse over the function and the 
// documentation for that function will appear on the screen.
//
// Transparent Controls
//
//  One thing to note is the top text and the checkbox -- when the Cube is enlarged, the backgrounds of these controls are updated
//  to blend with the cube that is underneath them.
//

#include "CDrawCube.h"
#include "3DCubeTexture.pgr2.h"

using namespace Sage::kw; 

void DrawCube::Main()
{
    if (!m_cWin) return;            // It's always good to do an initial check on whether or not we have 
                                    // the a window like we think we should.

    m_cWin->SetFont("Arial,12");    // Set a smaller font than the default for the output
                                    // since we're putting it out directly to the window rather than a text widget or 
                                    // separate window.

    CreateCubePoints();             // Initialize the cube vector points in 3D space (8 points for a cube)

    // The main set of functions to rotate, transform, and output the cube into the window.

    bool bRotating = false;         // true when mouse is down and we're rotating the cube with the mouse

    // Start positions and values when mouse button is first pressed

    int iStartMouseX,iStartMouseY;
    double fStartAngleY;
    double fStartAngleZ;
    double fStartAngleX;

    int iUpdateCount = 0;                                           // Updated every time we print a cube, just for general info

    m_cWin->SetWindowSize(850,650,true);                          // Whatever the window size we were given, resize it so 850,650
    m_cWin->Cls(PanColor::DarkBlue,PanColor::Black);            // Clear screen from a dark blue to black gradient. 

    // We don't need to clear the screen (above) since we're loading a texture.
    // The Cls() above was kept in so that the cTexture can be removed to show a non-textured version, with 
    // a dark-blue-to-black gradient instead.   
    //
    // To do this, remove the next line and the line below it.

    auto cTexture = m_cWin->ReadPgrBitmap("texture",sTexture);        // Get texture contained in text-file based .PGR (Portable Graphics Resource) file
    m_cWin->SetClsBitmap(cTexture);

    // Put a textWidget -- the object is kept so we can update the background if the cube is underneath it.

    auto& cText              = m_cWin->TextWidget(0,10,"Use the mouse to rotate the cube and Mouse Wheel to Zoom In and Out",Font("Arial,20") | opt::CenterX() | Transparent() | fgColor("red"));
    auto& cCheckboxWire      = m_cWin->NewCheckbox(5,200,"Wireframe",Transparent()); 
    auto& cCheckboxSemiTrans = m_cWin->NewCheckbox(5,225,"Semi-Transparent",Transparent()); 
    cCheckboxWire.SetHoverMsg("Click to show Wire Frame version of Box");
    cCheckboxSemiTrans.SetHoverMsg("Makes the box semi-transparent (when not in Wireframe mode)");
    bool bWireframe = false;

    CString cs;
    
    // Create out update routine as a lambda, to decouple it from the GetEvent() loop (since the display code
    // is a few lines of code itself), but not force it out to a class memmber or to pass a number of parameters
    // I may or may not use later on down the reoad.

    // note: After a time this would become a separate function, i.e. DrawCube::Update(). 
    //
    // In part, It is left here to help document the structure of the code, plus then I'd have to put a number of local variables 
    // in to the class structure when I can keep them locally.  
    //
    // This would no-doubt be put into a separate function (as part of the existing class structure) on the next
    // development interation, as it has grown enough and mature enough to do so with confidence it's 
    // in its overall basic form now.

    auto Update = [&]()
    {
        RotatePoints();     // Rotate based on current rotation angles for each acess
        Create2DPoints();   // Tranform 3D points to 2D perspactive (i.e. (x,y,z) -> (x,y)

        m_cWin->Cls();      // Clear the screen to the original backrgound before putting out the display
                            // This removes the previous cube that was drawn, clearing it to the bitmap
                            // or gradient if the bitmap was not loaded.

        if (bWireframe) DrawWireframe();    // Draw the wireframe (if m_bDrawWireframe = true);
        else DrawSurfaces();                // Draw opqaue/polygonal cube (i.e. surfaces) when m_bDrawSurfaces == true
        
        m_cWin->SetWritePos(0,100);         // Set a position to write out to the data to the screen.

        // Write out the data.  {g} sets the remainder of the line to a green color.  It's short for "{green}"

        m_cWin->Writeln(cs >> "X-Axis Angle = {g}" << (int) (m_fRotX*180/3.14159) % 360 << "°");
        m_cWin->Writeln(cs >> "Y-Axis Angle = {g}" << (int) (m_fRotY*180/3.14159) % 360 << "°");
        m_cWin->Writeln(cs >> "Z-Axis Angle = {g}" << (int) (m_fRotZ*180/3.14159) % 360 << "°");
        m_cWin->Write  (cs >> "Update Count = {g}" << ++iUpdateCount);

        // Update the backgrounds of the two transparent controls so the cube can be behind them/
        // If the cube isn't large enough, then we don't need to update them.  Since the cube can grow
        // larger than the screen, we want the controls to update their backgrounds. 
        //
        // There is a way to have Update() do this for us automatically, but we might as well do it here.

        cText.UpdateBg();                    // Update transparent background of the text Widget 
        cCheckboxWire.UpdateBg();            // Update the transparent background of the Checkboxes
        cCheckboxSemiTrans.UpdateBg();           
        m_cWin->Update();                    // Update the window with the new cube.    };
    };


    bool bUpdate = true;         // Set true so we will get an initial update (before any events)
                                // The first call to getEvent() returns true immediately just for this type of purpose
                                // In this case, I moved the Display out to separate (i.e. decouple) the GetEvent loop with      
                                // putting out the display (but left it in as a lambda), but in most cases, the display
                                // starts off in the GetEvent() loop for initial prtotypying, then moves out later.

    while(m_cWin->GetEvent())
    {
        if (bUpdate) Update();        // Update if something changed or we're on our initial display
        bUpdate |= cCheckboxWire.Pressed(bWireframe);                   // Check the Wire Checkbox and set values accordingly.
        bUpdate |= cCheckboxSemiTrans.Pressed(m_bSemiTransparent);        // Check the Wire Checkbox and set values accordingly.

        // Check if the mousewheel on the window has moved.  If so, set the zoom factor.

        int iMouseWheel;
        if (m_cWin->MouseWheelMoved(iMouseWheel))
        {
            m_fMag *= (iMouseWheel < 0) ? -.9*(double) iMouseWheel : 1/.9*(double) iMouseWheel;
            m_fMag = max(5,min(400,m_fMag));        // Keep to a reasonable range.
            bUpdate = true;
        }

        // Check if the mouse was moved.  if it was moved and this is the first time, then set the 'brotating' flag so we can keep 
        // track of our original movement. 
        //
        // Otherwise, rotate of from the begining point we started with.
        //
        // If the mouse was moved and the button isn't down, then it just resets bRotating. 
    
        if (m_cWin->MouseMoved())
        {
            bool bMouseDown = m_cWin->MouseButtonDown();

            bRotating &= bMouseDown;                // If the mouse isn't down, then reset any current status
                                                    // Mouse moved but with the button not pressed, so reset the status
            if (bMouseDown)
            {
                // If the mouse was down before (i.e. we're rotating the cube), then calculate the new values

                if (bRotating)
                {
                    POINT pMousePos = m_cWin->GetMousePos();
                    double fDistX = (double) ((int) pMousePos.x-iStartMouseX);
                    double fDistY = (double) ((int) pMousePos.y-iStartMouseY);
                    m_fRotY = fStartAngleY - fDistX/150.0;
                    m_fRotX = fStartAngleX + fDistY/150.0;
                }
                else // This is the first mouse click, so set the starting angles and save the mouse position.
                {
                    m_cWin->GetMouseClickPos(iStartMouseX,iStartMouseY); 
                    fStartAngleY = m_fRotY;
                    fStartAngleX = m_fRotX;
                    fStartAngleZ = m_fRotZ;
                }

                bRotating = bUpdate = true;        // We want to make sure we update (and we're also Rotating)
            }
        }
    }
}

// SortPolygons() -- Sort the 6 polygons that make up the cube in ascending z-order.
//
// This places the rearmost polygons in the back, in order, so we can display
// the polygons in order of Z-depth so that the foreground polygons are printed last,
// assuring polygons overlap in the right order.
//
void DrawCube::SortPolygons()
{
    // Just a simple sort, since we only have 6 polygons. 

    bool bReplaced = true;
    
    while(bReplaced)
    {
        bReplaced = false;
        for (int i=0;i<5;i++)
        {
            if (m_stAllPolys[i].fMax < m_stAllPolys[i+1].fMax)
            {
                stPoly_t stTemp = m_stAllPolys[i];
                m_stAllPolys[i] = m_stAllPolys[i+1];
                m_stAllPolys[i+1] = stTemp;
                bReplaced = true;
            }
        }

    };
}

// CreateCubePoints() -- Initialize the Cube to it's starting point.
//
void DrawCube::CreateCubePoints()
{
        m_fCube[0] = { -1,    1,    -1    };
        m_fCube[1] = {  1,    1,    -1    };
        m_fCube[2] = {  1,    -1,    -1    };
        m_fCube[3] = { -1,    -1,    -1    };
        m_fCube[4] = { -1,    1,    1    };
        m_fCube[5] = {  1,    1,    1    };
        m_fCube[6] = {  1,    -1,    1    };
        m_fCube[7] = { -1,    -1,    1    };
}

// Add specular reflections to incoming color/light, beased on plane formed by (i1,i2) and (i2,i3). 
// The lighting angle and viewpoint are canned in the routine,
//
void DrawCube::AddReflections(int i1,int i2,int i3,Point3D_t & pColor)
{
    Point3D_t & p1 = m_fRot[i1];    // Form a plane from three connected points
    Point3D_t & p2 = m_fRot[i3];    // They're just copied here to make the code cleaner and more 
    Point3D_t & p3 = m_fRot[i2];    // self-documenting, when we could just use m_fRot[i1], etc. directly.
    
    Point3D_t     m_pView   = { 0,0, m_fEyeDistance };          // Get our viewpoint
    Point3D_t     m_pLight  = { -0,80000,m_fEyeDistance };      // Set an angle a little off 45-degrees so we see reflections when we spin it in 
                                                                // any direction. 

    Point3D_t pNormal   = ((p2 - p1)^(p3 - p1)).Normalize();    // Get the normal vector (of the plain the polygon forms)
    Point3D_t pView     = (m_pView-p1).Normalize();             // Get the vector to the viewpoint (it should be the middle of the polygon, but this is easier)
    Point3D_t pLight    = (m_pLight-p1).Normalize();            // Get the vector to the light from the polygon.
    
    double fDiffusion = max(0,pLight | pNormal);                // Dot product of Light Vector and Normal to polygonal plane 

    // Get the reflection, angle...   defined by R = 2NCos(theta)-L, where N = pNormal and L = pLight

    Point3D_t pReflect = pNormal*(2*fDiffusion)-pLight;

    // Now get the dot product between fiew viewer vector and this vector

    double fRefCos = max(0,pReflect | pView);           // This is the basic amount of reflection coming our way as the viewer.
                
    double fReflection = fRefCos*fRefCos*fRefCos*fRefCos*fRefCos*fRefCos*fRefCos*fRefCos;   // Keep it >= 0 and modify it for how shiny we want the surface (kReflectAug)
                                                                                            // Without this, we get too much reflection, when we want it to appear more glossy
        
    // Put the ambient color and diffusion in 3DPoint form as a shortcut to work on all colors simultaneously vs. each color separately.
        
    pColor /= 255.0;                            // Normalize the color to 1
    fReflection *= .85;                         // Dull down the reflection a little, otherwise it can look artificial
    Point3D_t pDiffuse = pColor*fDiffusion;     // Get the diffusion light strength
    Point3D_t pAmbient = pColor;
                
    // This should may be better as sqrt(pAmbient+pDiffuse)^2 + fReflection^2), but it doesn't matter here since its a 
    // flat cube.
                
    // Use the 3D point as a color to get the lighting across all colors with one line of code -- it's less efficient to do it this way,
    // but it keeps the code simple and we don't really need the speed here since we're just doing a cube. 
    //
    pColor = min3d(sqrt(pAmbient*pAmbient + pDiffuse*pDiffuse + fReflection*fReflection),1.0)*255.0; 
              
}

// FillPoly() -- Fill the output 2-D polygon data, preparing it for output
//
void DrawCube::FillPoly(stPoly_t & stPoly,int i1,int i2,int i3,int i4,Point3D_t pColor)
{
    // Fill the polygon with the vertices based on the current output (rotated) point 
    // that has been converted to a 2D point.

    stPoly = {  { m_iMidX + (int) m_fOut[i1].fX, m_iMidY + (int) m_fOut[i1].fY },
                { m_iMidX + (int) m_fOut[i2].fX, m_iMidY + (int) m_fOut[i2].fY },
                { m_iMidX + (int) m_fOut[i3].fX, m_iMidY + (int) m_fOut[i3].fY },
                { m_iMidX + (int) m_fOut[i4].fX, m_iMidY + (int) m_fOut[i4].fY }, };

    AddReflections(i1,i2,i3,pColor);        // Add specular reflections.  Not necessary, but nice.

    // Set the maximum Z depth so we can sort the polygons by it later. 

    stPoly.fMax            = max(m_fOut[i1].fZ,max(m_fOut[i2].fZ,max(m_fOut[i3].fZ,m_fOut[i4].fZ)));
    stPoly.rgbColor        = pColor.toRGB();    // Convert the 3D-Point color to an RGB color
}

// Create2DPoints() -- Convert the 3D points to 2D points on a flat surface
//
// This uses standard equations to put 3D points on a 2D plane, much like
// taking a picture with things in the foreground and background. 
//
void DrawCube::Create2DPoints()
{
    for (int i=0;i<8;i++)
    {
        double fX = m_fRot[i].fX*m_fMag;
        double fY = m_fRot[i].fY*m_fMag;
        double fZ = m_fRot[i].fZ*m_fMag;

        double fOutX =(m_fEyeDistance)*fX/(m_fEyeDistance+fZ);
        double fOutY =(m_fEyeDistance)*fY/(m_fEyeDistance+fZ);

        m_fOut[i].fX = fOutX;
        m_fOut[i].fY = fOutY;
        m_fOut[i].fZ = fZ;
    }
}

// RotatePoints() -- Rotate the points of the cube and put them into another buffer/
//
// Note: there is no RotateZ() because there m_fRotZ currently is never non-zero
//
void DrawCube::RotatePoints()
{
    for (int i=0;i<8;i++)
        m_fRot[i] = m_fCube[i].RotateY(m_fRotY).RotateX(m_fRotX);   // Rotate around Y axis, then X-axis
}

// DrawWireframe() -- Draw the wire-frame version of the cube. 
//
void DrawCube::DrawWireframe()
{
    auto rgbColor = SageColor::White;

    // Draw lines from each corner.

    for (int i=0;i<4;i++)
    {
        m_cWin->DrawLine(m_iMidX + (int) m_fOut[i].fX,m_iMidY   + (int) m_fOut[i].fY,m_iMidX + (int) m_fOut[(i+1) % 4].fX,m_iMidY + (int) m_fOut[(i+1) % 4].fY,rgbColor);
        m_cWin->DrawLine(m_iMidX + (int) m_fOut[i+4].fX,m_iMidY + (int) m_fOut[i+4].fY,m_iMidX + (int) m_fOut[((i+1) % 4)+4].fX,m_iMidY + (int) m_fOut[((i+1) % 4)+4].fY,rgbColor);
        m_cWin->DrawLine(m_iMidX + (int) m_fOut[i].fX,m_iMidY   + (int) m_fOut[i].fY,m_iMidX + (int) m_fOut[i+4].fX,m_iMidY + (int) m_fOut[i+4].fY,rgbColor);
    }
} 

// DrawSurfaces() -- Draw the cube using polygons, creating a solid cube.
//
void DrawCube::DrawSurfaces()
{
    // Fill each polygon with the colors -- random colors, really, so they are just hard-coded for now.
    // The numbers are the vertices based on the polygon array in memory. 

    FillPoly(m_stAllPolys[0],0,4,7,3,{255,255,0});
    FillPoly(m_stAllPolys[1],1,5,6,2,{255,255,0*128});
    FillPoly(m_stAllPolys[2],0,4,5,1,{0*128,0*128,255});
    FillPoly(m_stAllPolys[3],3,7,6,2,{0*64,0*64,255});
    FillPoly(m_stAllPolys[4],4,5,6,7,{255,0*128,255});
    FillPoly(m_stAllPolys[5],0,1,2,3,{255,0,255});

    SortPolygons();        // Sort in z-order so we can just display them sequentially.

    m_cWin->SetDrawOpacity(m_bSemiTransparent ? 75 : 100);
    for (int i=0;i<6;i++) m_cWin->FillPolygon(m_stAllPolys[i],4,m_stAllPolys[i].rgbColor);
}

// Constructor() -- just store the window we're given so we can use it. 
//
DrawCube::DrawCube(CWindow * cWindow)
{
    m_cWin = cWindow;
}
