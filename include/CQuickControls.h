// This file copyright(c) 2021 Rob Nelson, All Rights Reserved.    E-mail rob@projectsagebox.com for more information.
//

// --------------------------------------------------------------------------------------------------------------
// PRELIMINARY VERSION.  This file has been brought into the Sagebox project from the original sources and has 
// not yet been commented for Sagebox, or properly formatted (I used tabs and am converting to spaces).
// --------------------------------------------------------------------------------------------------------------
//
// ****** UNDER CONSTRUCTION ******
//
// This file is still under construction and may not yet include specifics, awaiting proper testing and integration into Sagebox.
//

#pragma once
#include "CSageBox.h"

//****************************************************************************************************************
// CDevControls -- Fast, Windows Based controls for console mode and windows program prototyping and development
//****************************************************************************************************************
//
// CDevControls is a set of controls such as sliders, buttons, and so-forth that can be brought up very quickly without 
// the need to place the controls or created a window to store the controls. 
//
// The main purpose of CDevControls is to allow for protypying and development in places where the controls are useful, but you don't
// want to put them in your Windows interface or populate your console-mode program or library function with UI functions. 
//
// For example, if you want an abort button, or a simple slider to control program flow, both of these can be brought up with one line and controlled
// in different ways -- these can also be used without using any windows controls by specifying where to place the event and data from the control as it is 
// used. 
//
// This allows test elements to be put as an aside to the program that can later be removed or integrated into the program if they prove useful.
//
// ----------------------
// One-line instantiation
// ----------------------
//
// CDevControls works by keeping track of the controls and automatically placing them.  When first instantiated, nothing happens until a control is defined, after which a 
// window comes up with the control.
//
// As controls are added, the window grows to fit the controls.
//
// If there are too many controls to fit in the window, you can easily open another CDevControls window for more controls.
//
// For example, in console mode you can specify:
//
//   cSageBox.DevButton("Press to Stop");  to bring up a button.  or
//   cSageBox.DevEditbox("This is an Edit Box"); to bring up an edit box.
//
// ----------------------
// Accessing the Controls
// ----------------------
//
// Each control returns a pointer to the control, but this does not need to be used to access the control.
//
// For example,
//
// auto& myButton = cSageBox.DevButton("Stop"); 
//
// After which myButton.Pressed() can be used to determine when the button is pressed.
//
// However, sometimes you don't want to use the EventLoop() or access the button as a UI element in your code.
// In this case, you can set a Signal, such as:
//
// bool bSignal;
// myButton.SetSignal(&bSignal);
//
// Which will fill bSignal when the button is pressed (note that bSignal should be set to false to free it for the next press).
// --> You can also use GetSignal(bSignal), which is a macro that will return the Signal status and reset it at the same time.
//     ButtonSignal (which is a simple structure containing the bSignal and another boolean used in Checkboxes)
//     can also be used, such as ButtonSignal stSignal;, where stSignal.GetSignal() can then be used.
//
// As a oneliner, the following can be used without saving the button object:
//
// cSageBox.DevButton("Stop").SetSignal(&bSignal); 
//
// note:  While using signals isn't the safest way to go about checking the status, as the scope must be maintained for the signal (which usually is not a problem),
//        using the signal is a powerful way to use the button press, as the boolean value can be passed to any function that does not need to know or care that it is 
//        filled be a GUI control.
//
// See the notes on functions below for more information.
//
// -------------------------------------------------------------------------------------------
// cSageBox.DevButton() vs. cDevControls.AddButton(), etc. : Console Mode vs. using Windows
// -------------------------------------------------------------------------------------------
//
// When using a Window that has been created, or when creating another CDevControls object, the main functions are AddButton(), AddEditBox(), etc. 
// CSageBox has a pre-created CDevControls object (CDevControls) that can be access through various functions without the need to create a window or a 
// CDevControls object.
//
// This is also true when using a Window -- you can call cSageBox.QuickButton(), for example, in console or Window mode, with our without an active window.
// With an active Window, you can also call CWindow::DevControlsWindow() to create a CDevControls Window. 
//
// For very quick action, CSageBox features the ability to simply state things like:
//
// cSageBox.DevButton("Stop");
// cSageBox.DevEditBox();
// cSageBox.DevSlider("Some Slider")  
//
// with no preamble such as setting up the CQuickControls object.
//
// The equivalent, when setting one up yourself, is as follows:
//
// CDevControls cDevControls = MyWin.DevControlWindow() or (or cSageBox.DevControlWindow())
//
// cQuickControls->DevButton("Stop");
// cQuickControls->DevEditBox();
// cQuickControls->DevSlider("Some Slider")  
//
// --------------------------------
// Deleting a CDevControls Object
// --------------------------------
//
// The automatically created CDevControls object in CSagebox is automatically deleted, however:
//
// *** Important Note *** --> When using CDevControls() to return a Quick Control object THIS IS RETURNED AS A POINTER THAT MUST BE DELETED.
//                            In some examples, you may see Obj<CDevControls> cQuick = MyWin.DevControlsWindow() to turn the pointer into an object scoped
//                            to the current function to ensure it is deleted when the function is finished -- or, as a Obj<CDevControls> in a class to 
//                            ensure automatic deletion when the class object is destroyed.
//
// ------------
// SetTopmost()
// ------------
//
// This function is worth noting, as the CDevControls window can be overlapped by other windows.  Use SetTopmost() to set the window
// so that other windows will no overlap, or use opt::SetTopmost() as a parameter when creating the CDevControls window.
//
// For the automatically-created CDevControls window in CSagebox, use DevControlsTopmost() as a quick way to set it.
//
#if !defined(_CQuickControls_H_)
#define _CQuickControls_H_
namespace Sage
{
class CDevControls
{
	// Note: This is still in-progress, and items will be added as needed.

private:

    class CDevWinHandler : public CWindowHandler
    {
    public:
        MsgStatus OnNCLButtonDown(int iMouseX,int iMouseY) override;
        MsgStatus OnSageEvent() override;
    };
    static constexpr char m_cDefaultOpen = '|';
    static constexpr char m_cDefaultClose = '|'; 
    // Various structure for individual control items
    struct stCheckboxData_t
    {
        CString       cText; 
        CButton     * cCheckbox;
        bool          bDefault;
    };
    struct stCheckboxDefaults_t
    {
        const char * sDefault;
        bool bDefault;
    };
    // How to specify checkbox defaults in text, i.e. "Checkbox 1|ch|" = checked; default is not checked.
    //
    static constexpr stCheckboxDefaults_t m_stCheckboxDefaults[] = 
    {
        { "0", false },
        { "1", true },
        { "u", false },
        { "c", true },
        { "uc", false },
        { "ch", true },
        { "f", false },
        { "t", true },
        { "unchecked", false },
        { "checked", true },
        { nullptr, true },
    };

	// Various control types used in CQuickControls
	//
	enum class ControlType
	{
		Button,
		Checkbox,
        RadioButtonGroup,
		Slider,
		EditBox,
		TextWidget,
		ComboBox,
		Window,
		Header,
		Divider,
		Unknown,
	};

	// Control structure saved for each entry
	//
	struct stControl_t
	{
		POINT pLoc;
		SIZE szSize;
		ControlType controlType;
		CString cSectionName;	// unneeded memory for most items, but we don't care about memory usage here. 
		union Un
		{
			CButton * cButton;
			CSlider * cSlider;
			CEditBox * cEditBox;
		};
		Un un;

        // *** Makeshift data settings -- the union above should be structures for each type, rather than just pointers to controls. 
        //    $$ This needs to be fixed at some point 

        bool bSlider_SmallStyle;         // For now, we'll just put these here
	};

	std::vector<stControl_t> m_vControls;		// Controls storage

	int m_iVecEntries = 0;			// i.e. entries in vControls
	int m_iNumControls = 0;			// i.e. actual controls

	static constexpr SIZE kMaxWinSize	= { 1920,1080 };	// Max Window Size
	static constexpr SIZE kMaxSize		= { 800, 700 };		// Max outlay size (in Window)
	static constexpr int kSectionWidth	= 400;				// TBD when multi-column support is added.
    static constexpr SIZE szIndent      = { 10,10 };        // Indent for X and Y (left and top)

    bool m_bWindowClosed    = false;                        // True when close controls are active (either 'X' button and optional Close Button)
    bool m_bCloseX          = false;                        // true when there is an 'x' to close on the window.
    bool m_bAutoHide        = false;                        // Auto-hide the window when the 'x' or close button is pressed.
    SizeRect m_srClose{};                                   // bounds for close rect, when active.


	CWindow * m_cParentWin	= nullptr;
	CWindow * m_cWin		= nullptr;
	bool m_bInitialHide		= true;		// Hide the window initially, until we see our first control.
	bool m_bShowing			= true;		
	bool m_bTopmost			= false;	// When true, window is forced as top window so other windows won't overlap. 
    bool m_bWindowCloseEvent = false;   // Set when the 'X' or close button is pressed.  Resets when window is shown.

    CButton * m_cButtonClose    = nullptr;  // Added close button for automatic close controls
	int m_iCurrentX	= 0;
	int m_iCurrentY	= 0;

	int m_iUnNamedComoboxes		= 0;
	int m_iUnNamedSliders		= 0;
	int m_iUnNamedButtons		= 0;
	int m_iUnnamedSections		= 0;
	int m_iUnnamedCheckboxes	= 0;
	int m_iUnnamedEditBoxes		= 0;
	int m_iUnnamedWindows		= 0;
	int m_iUnnamedRadioGroups   = 0;

    int m_iGroupCount           = 0;

    CDevWinHandler  m_cWindowHandler;
	
    static constexpr const char * m_sGroupPrefix = "__Sage_DevControls__";
    void InitialShow();
	void SetWindow();
	bool AddControl(void * cControl,ControlType type,SIZE szSize);
	void SetTopWindow();
    bool isLastCheckbox(stControl_t * & stCheckbox);
    stControl_t * GetLastControl();
    bool DrawCloseX(bool bUpdate); 
    bool HaveCloseX();
    void CheckCheckboxDefault(stCheckboxData_t & stData);
    CString CleanLine(const char * sLine);
    CString GetNewGroupName();
public:
	// ----------------
	// Public Interface
	// ----------------

	// QuickControls() -- Start a Quick Controls window.  This window will remain invisible until a control as added.  It sizes itself 
	// dymamically to the number of controls added.
	//
	// Don't use this to create the Quick Control Window! -- Instead,
	// Use CSageBox::QuickControlWindow() or CWindow::QuickControlWindow() to create a QuickControl Window -- this 
	// allows you to set a title, background color, and position for the window.
	//
    // ** Important note ** the object pointer returned from CSageBox and Cwindows QuickControlWindow() functions
	// MUST BE DELETED, as it is not a managed object. 
	//
    // Example code tends to use Obj<CQuickControls> cQuickControl = QuickControlWindow() to treat it as a stack object
    // that is automatically deleted when the current function (or class) goes out of scope.
    // (However, since this is usually only used for develpment, leaving it allocated prior to program end causes no problems)
    //
	CDevControls(CWindow * cWin,const char * sTitle = nullptr,const cwfOpt & cwOpt = cwfOpt()); 

    // GetWindow() -- Returns the Window object pointer (i.e. CWindow object) of the Dev Controls Window.
    // This can be used for various operations with the window.
    //
    // note: The Dev Controls Window is self-managed.  Use the Window functions with care.
    //
    CWindow * GetWindow(); 

    // group() -- returns the group (i.e. WinGroup) substructure for the Dev Controls Window.  This can be used to perform group functions.
    // For example, when using multiple DevSlider() calls with a declared common group (i.e. DevSlider("MySlider",opt::Group(100,1)), group.SliderMoved(100)
    // can be used to determine if any slider in the group was moved, the slider ID and the slider position.
    //
    // Groups created in DevControls must be used through that Dev Controls Window's group function, which is why group() is provided.
    //
    // For example, using cMyDevWindow->group()->SliderMoved(MyGroup,..) will check any sliders within the given group within the DevWindow.    
    //
    CWindow::WinGroup * group(); 

    /// <summary>
    /// Adds 'X' on top-right and optional "Close" button.
    /// <para></para>&#160;&#160;&#160;
    /// Dev Windows are not typically closed and do not have close controls by default.  AllowClose() adds the ability to close the window,
    /// which can be used as a way to terminate a console-mode or other program that does not have a visible window. 
    /// <para></para>
    /// Use WindowClosed() to determine if the window has been closed (or the added Close Button has been pressed.
    /// <para></para>
    /// When the 'X' button is pressed, a Sage Event is sent to the window where it can be checked. The Close button also sends a Sage Event.
    /// <para></para>&#160;&#160;&#160;
    /// Note: When the user presses the 'X' button or "Close" button, the window is not closed or hidden.  These controls are only used
    /// to provide an indication (through WindowClosed()) that the user has pressed a control as a message to terminate the application.
    /// <para></para>&#160;&#160;&#160;
    /// ---> This function is not used to close the Dev Window but as an easy way for the user to signal to close the application.
    /// </summary>
    /// <param name="bAddCloseButton">when TRUE adds a "Close" button.  Otherwise only the 'X' is placed on the right-top of the window for closure.</param>
    /// <returns></returns>
    bool AllowClose(bool bAddCloseButton = false); 

    // Returns true if a close button or 'x' has been placed.  Used internally to decide when to attach a close button
    // or just stay with a previous configuration.
    //
    bool Closeable(); 

    /// <summary>
    /// Auto-hides the DevWindow when the user presses the 'x' button or close button.  This does not destroy the devwindow, and only hides it.
    /// <para></para>
    /// Once Hidden, the DevWindow can be shown again with a call to Show()
    /// </summary>
    /// <param name="bAutoHide"> - True to auto-hide window; false to turn auti-hide off</param>
    /// <param name="bAddCloseButton"> -- True to create a close button.  Otherwise only the 'x' appears in the upper-right corner.</param>
    /// <returns></returns>
    bool AutoHide(bool bAutoHide = true,bool bAddCloseButton = false);

    /// <summary>
    /// Returns TRUE if the "Close" Button or 'X' has been pressed (both are added by AllowClose()).
    /// <para></para>
    /// This can be used as a quick way to close an application that only has a Dev Window and no other window except the Console Window.
    /// </summary>
    /// <param name="bAddCloseButton"> -- True to create a close button.  Otherwise only the 'x' appears in the upper-right corner.</param>
    /// <returns>true if the user has attempted to close the window.</returns>
    bool WindowClosed(bool bAddCloseButton = false); 

    /// <summary>
    /// Returns true if the DevWindow Close Button was pressed, but only once -- it returns false after this point until the DevWindow
    /// is again made visible by a call to Show()
    /// <para></para>
    /// --> This difference from WindowsClose() which will continuously give a Windows-Close status of TRUE if the window is closed
    /// <para></para>
    /// WindowCloseEvent(), however, is an event status and only returns TRUE once after the close (or X) button is pressed.
    /// </summary>
    /// <param name="bAddCloseButton"> -- True to create a close button.  Otherwise only the 'x' appears in the upper-right corner.</param>
    /// <returns></returns>
    bool WindowCloseEvent(bool bAddCloseButton = false);

	// AddButton() -- Add a button to the Quick Control Window.  This accepts all options as normal buttons, but 
	// the default will add a regular button. 
	//
	// The Name used as a title for the button, but is optional. 
	//
	CButton & AddButton(const char * sButtonText = nullptr,const cwfOpt & cwOpt = cwfOpt()); 

	// AddCheckbox() -- Add a checkbox to the Quick Control Window. This accepts all options as normal buttons, but 
	// the default will add a regular button. 
	//
	// The Name used as a title for the button, but is optional. 
	//
	CButton & AddCheckbox(const char * sButtonText = nullptr,const cwfOpt & cwOpt = cwfOpt()); 

    /// <summary>
    /// Adds a RadioGroup to the DevWindow.  See different prototypes for different methods (const char *, const char **, vector&lt;char *&gt;
    /// <para></para>
    /// In the case of using a simple string, separate button names with '\n', such as: 
    /// <para></para>
    /// --> AddRadioButtonGroup("Button1\nButton2\nButton3"); 
    /// <para></para>
    /// Use Title() for the section title, and Default to set a default (if Default() is not supplied, the first button is the default)
    /// <para></para>
    /// --> Example: AddRadioButtonGroup("Button1\nButton2\nButton3",Title("Select Button") | Default(1))
    /// <para></para>
    /// This sets the title to "Select Button" and sets the default button to the second button.
    /// <para></para>
    /// You can also use char * *, or vector&lt;char *&gt; to give a list of pointers to each button. 
    /// <para></para>
    /// --> Important Note: In the case of using a "char * *" array, the last entry MUST be a nullptr.  You can precede the char ** array with the number of items to avoid using a nullptr.
    /// </summary>
    /// <param name="iNumButtons">- (optional) When using "const char * *" for a list of names, you can give it the number of items (the end does not need to be a nullptr in this case)</param>
    /// <param name="sButtonNames">String of button names, or char * * list, or std::vector&lt;const char *&gt; of names.</param>
    /// <param name="cwOpt">Options such as Default() and Title()</param>
    /// <returns></returns>
    ButtonGroup AddRadioButtonGroup(const char * sButtonNames,const cwfOpt & cwOpt = cwfOpt());

    /// <summary>
    /// Adds a RadioGroup to the DevWindow.  See different prototypes for different methods (const char *, const char **, vector&lt;char *&gt;
    /// <para></para>
    /// In the case of using a simple string, separate button names with '\n', such as: 
    /// <para></para>
    /// --> AddRadioButtonGroup("Button1\nButton2\nButton3"); 
    /// <para></para>
    /// Use Title() for the section title, and Default to set a default (if Default() is not supplied, the first button is the default)
    /// <para></para>
    /// --> Example: AddRadioButtonGroup("Button1\nButton2\nButton3",Title("Select Button") | Default(1))
    /// <para></para>
    /// This sets the title to "Select Button" and sets the default button to the second button.
    /// <para></para>
    /// You can also use char * *, or vector&lt;char *&gt; to give a list of pointers to each button. 
    /// <para></para>
    /// --> Important Note: In the case of using a "char * *" array, the last entry MUST be a nullptr.  You can precede the char ** array with the number of items to avoid using a nullptr.
    /// </summary>
    /// <param name="iNumButtons">- (optional) When using "const char * *" for a list of names, you can give it the number of items (the end does not need to be a nullptr in this case)</param>
    /// <param name="sButtonNames">String of button names, or char * * list, or std::vector&lt;const char *&gt; of names.</param>
    /// <param name="cwOpt">Options such as Default() and Title()</param>
    /// <returns></returns>
    ButtonGroup AddRadioButtonGroup(const char * * sButtonNames,const cwfOpt & cwOpt = cwfOpt());

    /// <summary>
    /// Adds a RadioGroup to the DevWindow.  See different prototypes for different methods (const char *, const char **, vector&lt;char *&gt;
    /// <para></para>
    /// In the case of using a simple string, separate button names with '\n', such as: 
    /// <para></para>
    /// --> AddRadioButtonGroup("Button1\nButton2\nButton3"); 
    /// <para></para>
    /// Use Title() for the section title, and Default to set a default (if Default() is not supplied, the first button is the default)
    /// <para></para>
    /// --> Example: AddRadioButtonGroup("Button1\nButton2\nButton3",Title("Select Button") | Default(1))
    /// <para></para>
    /// This sets the title to "Select Button" and sets the default button to the second button.
    /// <para></para>
    /// You can also use char * *, or vector&lt;char *&gt; to give a list of pointers to each button. 
    /// <para></para>
    /// --> Important Note: In the case of using a "char * *" array, the last entry MUST be a nullptr.  You can precede the char ** array with the number of items to avoid using a nullptr.
    /// </summary>
    /// <param name="iNumButtons">- (optional) When using "const char * *" for a list of names, you can give it the number of items (the end does not need to be a nullptr in this case)</param>
    /// <param name="sButtonNames">String of button names, or char * * list, or std::vector&lt;const char *&gt; of names.</param>
    /// <param name="cwOpt">Options such as Default() and Title()</param>
    /// <returns></returns>
    ButtonGroup AddRadioButtonGroup(int iNumButtons,const char * * sButtonNames,const cwfOpt & cwOpt = cwfOpt());

     /// <summary>
    /// Adds a RadioGroup to the DevWindow.  See different prototypes for different methods (const char *, const char **, vector&lt;char *&gt;
    /// <para></para>
    /// In the case of using a simple string, separate button names with '\n', such as: 
    /// <para></para>
    /// --> AddRadioButtonGroup("Button1\nButton2\nButton3"); 
    /// <para></para>
    /// Use Title() for the section title, and Default to set a default (if Default() is not supplied, the first button is the default)
    /// <para></para>
    /// --> Example: AddRadioButtonGroup("Button1\nButton2\nButton3",Title("Select Button") | Default(1))
    /// <para></para>
    /// This sets the title to "Select Button" and sets the default button to the second button.
    /// <para></para>
    /// You can also use char * *, or vector&lt;char *&gt; to give a list of pointers to each button. 
    /// <para></para>
    /// --> Important Note: In the case of using a "char * *" array, the last entry MUST be a nullptr.  You can precede the char ** array with the number of items to avoid using a nullptr.
    /// </summary>
    /// <param name="iNumButtons">- (optional) When using "const char * *" for a list of names, you can give it the number of items (the end does not need to be a nullptr in this case)</param>
    /// <param name="sButtonNames">String of button names, or char * * list, or std::vector&lt;const char *&gt; of names.</param>
    /// <param name="cwOpt">Options such as Default() and Title()</param>
    /// <returns></returns>
    ButtonGroup AddRadioButtonGroup(std::vector<char *>  vButtonNames,const cwfOpt & cwOpt = cwfOpt());

	// AddEditBox() -- Add an EditBox to the quick control Window.  The sEditBoxTitle, while optional, will provide a
	// label to the left of the edit box.  The default width is 150 pixels or so, but can be changed with normal EditBox options
	//
    // Note: InputBox and EditBox are the same.  EditBox is kept to remain consistent with Windows terminology
    //
	CEditBox & AddEditBox(const char * sEditBoxTitle = nullptr,const cwfOpt & cwOpt = cwfOpt()); 

	// AddInputBox() -- Add an Input Box to the quick control Window.  The sEditBoxTitle, while optional, will provide a
	// label to the left of the edit box.  The default width is 150 pixels or so, but can be changed with normal InputBox options
	//
    // Note: InputBox and EditBox are the same.  EditBox is kept to remain consistent with Windows terminology
    //
	CEditBox & AddInputBox(const char * sInputBoxTitle = nullptr,const cwfOpt & cwOpt = cwfOpt()); 

	CTextWidget & AddText(const char * sText,const cwfOpt & cwOpt = cwfOpt());
	CTextWidget & AddText(const char * sText,int iHeight,const cwfOpt & cwOpt = cwfOpt());
	CTextWidget & AddText(int iHeight,const cwfOpt & cwOpt = cwfOpt());
	CTextWidget & AddText(const cwfOpt & cwOpt = cwfOpt());

	// AddSlider() -- Add a slider to the Quick Controls Window.  The default width is 200 with a 0-100 range.  The Range can be 
	// changed with default Slider options, i.e. opt::Range(0,200), for example, to set a range of 0-200.
	// -->
	// The title is displayed beneath the slider, as well as the value. 
	//
	CSlider & AddSlider(const char * sSliderName = nullptr,const cwfOpt & cwOpt = cwfOpt());

	CComboBox & AddComboBox(const char * sComboBoxName,const cwfOpt & cwOpt = cwfOpt());
	CWindow & AddWindow(const char * sTitle,int iNumlines,const cwfOpt & cwOpt = cwfOpt());
	// AddSection() -- Adds a text section to the window, to separate types of controls.
	// You can use opt::fgColor() to set the text color of the section name.
	//
	bool AddSection(const char * sSectionName = nullptr,const cwfOpt & cwOpt = cwfOpt());

	// SetLocation() -- Change the location of the QuickControls Window
	//
	bool SetLocation(int iX,int iY);
	bool SetLocation(POINT pLoc);

	POINT GetLocation();
	SIZE GetWindowSize();

	// SetTopMost() -- Set the window as a "topmost" window, which will keep the window on top of all other windows, 
	// disallowing other windows from overlapping the window.  This can be useful when using the controls, as other windows, when they
	// gain focus, can overlap the Quick Controls Window
	//
    bool SetTopMost(bool bTopmost = true);

	// Show() -- Show or hide the Quick Controls Window
	//
	bool Show(bool bShow = true);

	// Hide() -- Show or hide the Quick Controls Window
	//
	bool Hide(bool bHide = true);
};
}; // namespace Sage
#endif // _CQuickControls_H_y
