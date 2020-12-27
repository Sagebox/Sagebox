#pragma once

#if !defined(_CColorSelector_h2_)
#define _CColorSelector_h2_
#include "CSageBox.h"
#include "CColorWheelWidget.h"

class CColorSelector : private CWindowHandler
{
private:

    // Messages we send to ourself.  This is done to avoid multi-threading.  For some functions, such as setting the location,
    // a forced update or background update by the user, we don't want to start a drawing operation while we might already be in
    // one in the main thread. 
    //
    // Because many events come through the message loop, we might be in a drawing routine at any time. 
    // We can use LockProcess() to deal with this, or we can just send ourselves messages to ensure everything is one asynchronously
    // through the message thread. 
    //
    // This makes user the Windows display doesn't collide between the user thread and message thread (which is not dangerous, but can cause
    // a display element to fail), and that there is no chance of altering the same memory.
    // An example of this would be updating the background from a user-call while we're already updating it for another reason (this doesn't take 
    // place here, but you never know if it will change).
    //
    // The other way to do it is to call LockProcess() and UnlockProcess() around any activity that can multi-thread.  In this case, 
    // sending ourself a message seemed like a better idea, though some widgets us LockProcess() instead.

    enum class LocalMessage
    {
        UpdateBg,        // A user call to UpdateBg() for when the parent's background may have changed and we are transparent
        Update,         // Update -- a user call to udpate the widget's display -- this is usually done by SageBox for various reasons, but does
                        // not apply here because we don't register the Widget yet -- but, when we do, Update() can be called under certain circumstances,
                        // though SageBox would probably do it from the thread anyway.
        SetLocation,    // A User call to SetLocation() to change the location of the window.  If the Window is a popup or not transparent, it doesn't
                        // matter where this occurs. Howeverm, for embedded, transparent windows, the parent's background needs to be copied, hence
                        // doing it here to avoid memory conflict issues with the threads and eo ensure not graphic output conflicts.
        Cls             // Set backgrond color or gradient
    };
    
    CColorSelector(const CColorSelector &p2) {}        // Keep copy constructor private so no one copies the object

    // Message Handler overrides (since we subclassed it rather than making our own)

    MsgStatus OnWidgetMessage(void * cWidget,int iMessage)    override;     // When we receive a message from the ColorWheelWidget or ourself
    MsgStatus OnSageEvent()    override;                                    // When We receive an event caused by our controls
                                                                            // This is the same as GetEvent() but without a while loop.
    void Init(void * pClassInfo)    override;                               // Initializes the Message Handler with our object so we know how to call

    HWND        m_hWnd              = nullptr;                                  // Used to verify Windows Window is still active or not
    CSageBox  * m_cSageBox          = nullptr;          // Main Sagebox object -- used to make sure things are still there on destruction
    CWindow   * m_cWin              = nullptr;          // Our main window
    CWindow   * m_cParentWin        = nullptr;          // The parent's window
    CEditBox  * m_cEditBoxRed       = nullptr;          // Edit boxes for colors
    CEditBox  * m_cEditBoxGreen     = nullptr;
    CEditBox  * m_cEditBoxBlue      = nullptr;
    CButton   * m_cButtonOk         = nullptr;          // Ok button when Color Selector is popup, otherwise it does not appear
    CButton   * m_cButtonCancel     = nullptr;          // Cancel button when Color Selector is popup, otherwise it does not appear
    CColorWheelWidget * m_cWidget   = nullptr;          // ColorWheel Widget

    CDavinci      * m_cDavinci        = nullptr;        // Lowest level core object to determine core window status on deletion (deprecated)

    EditBoxSignal   m_stSignal;                         // A signal user for ease-of-use, but not really necessary
                                                        // It's used as a catch-all to determine if any edit box was changed.
    CPoint          m_pLoc;                             // Location of the box we draw on an update

    CPoint          m_pSetLocation;                     // Point for Widget messages to re-locate the window (SelfSetLocation())
    Gradient        m_gClsColor;                        // Color used when setting the window's background color

    bool m_bNoClose         = false;                    // When TRUE, the window will not automatically close when pressed, though a 
                                                        // close message is sent so the parent can decide what to do
    bool m_bPopup           = false;                    // When TRUE, the window is its own floating window.  When false, the window is embedded in
                                                         // the parent's window (opt::Popup() option controls this)

    bool m_bEventActive     = false;                    // TRUE when there is an event to look at.  This can be used in an event loop to check events only
                                                        // when necessary, though it is not required.
    bool m_bValueChanged    = false;                    // TRUE when we have a value-changed event
    bool m_bWindowClosed    = false;                    // TRUE when the close button was pressed.
    bool m_bTransparent     = false;                    // TRUE when window is embedded and transparent (i.e. copies parent's background)
    bool m_bInvalid         = true;
    int  m_iWindowX;
    int  m_iWindowY;

    void HandleEvent();                                 // Check any events.
    void InitWindow(CWindow * cWin,int iX,int iY,const char * sOptions);
    void UpdateWindow();
    void SendLocalMessage(LocalMessage message);        // Send ourselves a message to do something. 
    static void Deleter(void * pThis);                  // Deleter so the user doesn't have to delete us even if it was allocated with new()
    void SendParentEvent();                             // Inform the parent window we have an event for it to look at.

    void UpdateWidgetBg();                              // Update the Color Wheel Widget background if we change our background

    // Self-Sent Message Functions

    void LocalUpdateBg();
    void LocalSetLocation(POINT pLoc);
    void LocalCls(Gradient gColor);
public:


    // This duplicates all event-related functions.  These can be accessed through the main class.
    // The event structure is here for clarity of code, so that "event." can be pressed and a list of all available functions
    // appears
    //
    struct Event
    {
    private:
        friend CColorSelector;
        CColorSelector * cColor;
    public:
        bool WindowClosed(Peek peek = Peek::No);
        bool ValueChanged(Peek peek = Peek::No);
        bool ValueChanged(RGBColor_t & rgbColor,Peek peek = Peek::No);
        bool DisableClose(bool bDisable = true);
        bool OkButtonPressed(Peek peek = Peek::No);
        bool CancelButtonPressed(Peek peek = Peek::No);

        __forceinline bool EventActive() { bool bReturn = cColor->m_bEventActive; if (bReturn) cColor->m_bEventActive = false; return bReturn; }
    };

    Event event;

    CColorSelector(CWindow * cWin,int iX,int iY,const cwfOpt & cwOpt = cwfOpt()); 
    ~CColorSelector();

    // Close the window.  This is the same as Hide().
    // This can be used when the Ok Button or Cancel button was pressed, or the window was closed 
    // by the user. 
    //
    // This keeps the object alive, and Show() may be used to re-show the window rather than creating a new ColorSelector object
    //
    void CloseWindow(); 

    // WindowClosed() -- Returns true of the window was closed.  This also sets the Cancel Button as pressed, so
    // CancelButtonPressed() may be used to determine if the Cancel Button was pressed or the window closed, then
    // WindowClosed() only returns true of the window close button was pressed
    //
    bool WindowClosed(Peek peek = Peek::No);

    // Returns true as an event if the value as changed by changing an edit box or moving the rings in the color wheel.
    // rgbColor may be specified so that the color filled with the current value (only if the color was changed; if not, rgbcolor is
    // not changed
    //
    bool ValueChanged(Peek peek = Peek::No);

    // Returns true as an event if the value as changed by changing an edit box or moving the rings in the color wheel.
    // rgbColor may be specified so that the color filled with the current value (only if the color was changed; if not, rgbcolor is
    // not changed
    //
    bool ValueChanged(Sage::RGBColor_t & rgbColor,Peek peek = Peek::No);

    // Disables the window from closing -- the Window Close is reported as an event, but the window does not close. 
    // (use the opt::NoClose() option when creating the control
    //
    // Default behavior is to close the window when the close button is pressed. Setting this to true keeps the window open
    //
    bool DisableClose(bool bDisable = true);

    // Returns true (event) if the OK button was pressed
    //
    bool OkButtonPressed(Peek peek = Peek::No);

    // Returns true (event) if the Cancel button was pressed.
    //
    bool CancelButtonPressed(Peek peek = Peek::No);


    // Quickly determine if there is an Active event so we can choose to only check events if there is one to find.  
    // This is not required and is provided to keep code faster, where desirable.  You can just check events in the
    // main parent loop whether or not there is an active event.
    //
    // This is inlined to make it a very quick action. 
    // The active event is set back to false so that it returns false on future calls until there is another
    // event active.
    //
    __forceinline bool EventActive() { bool bReturn = m_bEventActive; if (bReturn) m_bEventActive = false; return bReturn; }


    // Public Functions from CColorWheelWidget passed through to CColorSelector

    // Returns the RGB Values of the current selection
    //
    RGBColor_t GetRGBValues();

    // Returns the HSL values of the current selection
    //
    HSLColor_t GetHSLValues();

    // Sets the window location of the Color Selector Window, relative to the parent window
    //
    void SetWinLocation(int iX,int iY);

    // Sets the window location of the Color Selector Window, relative to the parent window
    //
    void SetWinLocation(POINT pLocation);
    
    // Shows (or hides) the main Color Selector Window
    //
    void Show(bool bShow = true);

    // Hides (or shows) the main Color Selector Window
    //
    void Hide(bool bHide = true);

    // Updates the background for transparent windows.  This copies the background of the parent and updates the window and colorwheel.
    // This can be used when the parent background has changed to update the blending of the transparent parts of the Color Selector Window
    //
    void UpdateBg(bool bRedraw = true);

    // Redraw the Color Selector Window.  Usuallt this not needed, but can be useful if Windows System calls are used.
    //
    void Update();

    // Returns whether or not the Color Selector is valid.  This can be used if the window does not appear to be responding.
    // If this returns FALSE, then something is wrong, such as the parent window not existing, or some memory error.
    //
    bool isValid();

    // Set the current RGB value showing in the Color Selector Window.  This updates the colors and edit boxes.
    //
    bool SetRGBValue(RGBColor_t rgbColor);

    // Get the size of the Color Selector Window
    //
    SIZE GetWindowSize();

    // Returns the window location of Color Selector Window relative to the parent window
    //
    POINT GetWinLocation();

    // Cls() -- Sets the baackground color of the window.  Providing two colors
    // gives the background a gradient.  For example, Cls({0,0,0},{0,0,255}) sets a
    // gradient from black to blue. 
    //
    // Also, strings may be used, such as:
    //
    // Cls("black","blue")
    // Cls("blue");
    // Cls({0,0,0});        // Same as cls("Black");
    // Cls(rgbMyColor);    
    //
    bool SetBgColor(const RGBColor_t rgbColor,const RGBColor_t rgbColor2);

    // Cls() -- Sets the baackground color of the window.  Providing two colors
    // gives the background a gradient.  For example, Cls({0,0,0},{0,0,255}) sets a
    // gradient from black to blue. 
    //
    // Also, strings may be used, such as:
    //
    // Cls("black","blue")
    // Cls("blue");
    // Cls({0,0,0});        // Same as cls("Black");
    // Cls(rgbMyColor);    
    //
    bool SetBgColor(const RGBColor_t rgbColor);

        // Cls() -- Sets the baackground color of the window.  Providing two colors
    // gives the background a gradient.  For example, Cls({0,0,0},{0,0,255}) sets a
    // gradient from black to blue. 
    //
    // Also, strings may be used, such as:
    //
    // Cls("black","blue")
    // Cls("blue");
    // Cls({0,0,0});        // Same as cls("Black");
    // Cls(rgbMyColor);    
    //
    bool SetBgColor(const char * sColor1,const char * sColor2 = nullptr);
};

#endif // _CColorSelector_h2_


