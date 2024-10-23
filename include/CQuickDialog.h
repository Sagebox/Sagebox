
// CQuickDialog.H -- SageProx Project.   Copyright (c) 2020 Rob Nelson, all Rights Reserved.  RobNelsonxx2@gmail.com


// ## Note: This file is in-progress, with more functions to be added, and some cleanup. 

// ---------------------------------
// CQuickDialog - Quick Dialog Class
// ---------------------------------
//
// Preliminary, first-pass version of a QuickDialog class and set of functions that allows creating a dialog quickly, in terms faster than 
// using the UI designer or creating a window and then adding controls in specific positions
//
// Quick Dialog supports a limited set of controls, meant for quick user-input interactions with controls that can be specified. 
// More powerful than the specific dialog functions such as GetInput(), GetFloat(), InfoDialog(), etc., the Quick Dialog
// functions give control with an easy interface that is automatically launched, close, and managed by the parent window.
//
// The Quick Dialog functions allow a way to create a dialog window quickly with as little as two lines of code.
// The calling process can wait for the user to finish or manage the inputs-as-they-happen through the GetEvent() loop
// or by handing a SageEvent() in a message callback routine (or CWindow overload)
//
// Example:
//      auto& cDialog = cWin.QuickDialog("Pick a size"); 
//      auto& cRadioGroup = cDialog.AddRadioButtons("Small\nMedium\nLarge",Default(1)); // Set Medium as the default 
//
//      cDialog.WaitforClose(); 
//      int iSelection = cRadioGroup.GetChecked(); 
//
// This is an example of using the dialog group in 4 lines.  It is automatically closed and deleted. 
// cDialog.Show() can be used to show the dialog, and then the dialog box can be managed by checking cDialog.StatusChanged() as events come in.
//
// note that the status returned by WaitforClose() was not evaluated.  In this case, we take the selection or, in the case of a cancel, the default value.
//
// In the case of other items, such as an input box, or checkboxes, if a validation fails, the box can be re-used by re-calling cDialog.WaitforClose(), or 
// cDialog.ResetStatus() if cDialog.Show() was used to manage the events in the GetEvent() loop or Event handler. 
//

#pragma once
#include "CSagebox.h"
#if !defined(_CQuickDialog_h_)
#define _CQuickDialog_h_

namespace Sage
{
class CQuickDialog
{
public:

    // Returned status from WaitforClose() or GetStatus()
    // Remains "Unknown" until user presses a button or closes the window.
    //
    enum class Status
    {
        Ok,         // Ok Button pressed
        Canceled,   // Cancel button pressed
        Unknown,    // Nothing pressed yet
    };


    // Make deleter, copy constructor and others private so they can't be used (this object is managed by CWindow)

private:

    // ListBox Handler class.
    //
    class DialogListBoxHandler : public CListBoxHandler
    {
        CQuickDialog * cDialog = nullptr;
    public:
	    void Init(void * pClassInfo) override;
    	virtual MsgStatus OnDoubleClick(int iSelection) override;

    };

    // Button Handler Class
    //
    class DialogButtonHandler : public CButtonHandler
    {
        CQuickDialog * cDialog = nullptr;
    public:
	    void Init(void * pClassInfo) override;
	    MsgStatus OnPressed() override;
	    MsgStatus OnUnPressed() override;
    };

    // keep Delete, copy constructor, and move constructor private. The CQuickDialog object is a managed object, and is deleted by the main window class. 

    void operator delete(void * ptr)
    { 
        ::operator delete(ptr); 
    };
    // Copy Constructor
    //
    CQuickDialog(const CQuickDialog &p2) 
    {
        FailBox("CQuickDialog::CopyConstructor","In Copy Constructor")   // In debug mode, lets us know if it is ever used
    };

    // Move constructor
    //
    CQuickDialog(CQuickDialog && p2) noexcept
    {
        FailBox("CQuickDialog::CopyConstructor","In Move Constructor")   // In debug mode, lets us know if it is ever used
    };

    DialogButtonHandler     ButtonHandler;      // Button handler for OK & Cancel button to close window and set status changed flag and event
    DialogListBoxHandler    ListboxHandler;     // Listbox handler to close window and set event & status on double-click when its the only control


    // Private Data will be removed in some future release.
    //
private:
    enum class OutType
    {
        None = 0,
        Float,
        Double,
        Bool,
        Int,
        StdString,
        CString,
    };

    // Internal Control Types

    enum class Control
    {
        Header,
        H1Header,
        H2Header,
        H3Header,
        H4Header,
        Button,
        InputBox,
        EditBox,
        Slider,
        Checkbox,
        Listbox,
        Combobox,
        CheckboxGroup,
        RadioButton,
        Undefined,
    };

    // Deprecated
    //
    enum class HeaderType
    {
        H1  = 0 ,
        H2      ,
        H3      ,
        H4      ,
        Blank   ,
    };

    // Individual elements as they are added. 
    //
    struct stElement_t
    {

        Control eType;
        const cwfOpt * cwOpt;
        const kwOpt * keywords;
        const char * sName;
        const char * sDesc;
        void * vOutValue;
        OutType eOutType;
 
        SIZE szSize;
    };

private:
    // Various defaults 

    static constexpr const char * m_sGroupPrefix = "__quickdialog_groupid__";       // Group prefix names for checkboxes & radiobutton groups
    static constexpr int          m_iMaxCheckboxes      = 50;
    static constexpr int          m_iMaxHeaderLines     = 5;
    static constexpr int          m_iIndentX            = 20;
    static constexpr int          m_iIndentY            = 10;
    static constexpr int          m_iIndentYBot         = 5; //10;
    static constexpr int          m_iMaxWidth           = 1200;
    static constexpr RgbColor     m_rgbDefaultBg         = {32, 73, 137}; //{ 0, 55, 155 }; //{ 0, 60, 170 };
    static constexpr RgbColor     m_rgbDefaultBg1        = { 16, 65, 175 }; //{ 35, 107, 197 }; //, { 35, 76, 172 }; //{ 0, 55, 155 }; //{ 0, 60, 170 };
    static constexpr RgbColor     m_rgbDefaultBg2        = { 26,62,115  }; //{ 37, 102, 206 }; //{ 11, 43, 120 }; //{ 0, 55, 155 }; //{ 0, 60, 170 };
    static constexpr int          m_iHeaderFontSizes[m_iMaxHeaderLines+1] = { 17,14,13,12,12,12 };  // This follows H1 (+blank), not number of lines. 
    static constexpr const char * m_sListboxTitleFont   = "Arial,13";
    static constexpr const char * m_sListboxItemFont    = "Arial,13";
    static constexpr RgbColor     m_rgbListBox          = {0,35,105};
    static constexpr const SIZE   m_szListboxPadding    = { 0,10 };
    static constexpr const char * m_sComboboxItemFont    = "Arial,13";
    static constexpr const SIZE   m_szComboboxPadding    = { 0,10 };

    RgbColor                      m_bgColorPlain         = m_rgbDefaultBg;         // $$ Temporary until Radio Button Transparency is fixed.
    RgbColor                      m_bgColor1             = m_rgbDefaultBg1;
    RgbColor                      m_bgColor2             = m_rgbDefaultBg2;
    // Justification for text items
    //
    enum class Just 
    {
        Top         ,
        Bottom      ,
        Left        ,
        Right       ,
        Center      ,
        Variable    ,
    };

    // Various structure for individual control items
    struct stCheckboxData_t
    {
        CString       cText; 
        CButton     * cCheckbox;
        bool          bDefault;
    };

    struct stSlider_t
    {
        CSlider     * cSlider;
        SIZE          szWinSize; 
    };

    struct stListbox_t
    {
        CString       csTitle;
        CWindow     * cWin;
        CString       csTitleFont; 
        CString       csItemFont; 
        CListbox    * cListbox; 
        SIZE          szWinSize;
        SIZE          szListboxSize;   
        int           iTextHeaderItem;
    };

    struct stCombobox_t
    {
        CString       csTitle;
        CWindow     * cWin;
        CString       csTitleFont; 
        CString       csItemFont; 
        CCombobox   * cCombobox; 
        SIZE          szWinSize;
        SIZE          szComboboxSize;   
        int           iTextHeaderItem;
    };


    struct stEditBox_t
    {
        CWindow     * cWin;
        CEditBox    * cEditBox; 
        SIZE          szWinSize;        
        POINT         pBoxStart;        // Edge of edit box (vs label)
        int           iBoxBoxX;        // Edge of edit box (vs label)

    };
    struct stCheckbox_t
    {
        CWindow     * cWin;
        CButton     * cButton;
        SIZE          szWinSize;
        int           iNumCheckboxes; 
        bool          bGroup;
        int           iGroupNumber; 
        int           iDefault;       // For radio buttons ($$ probably not used)
        bool          bJustLeft;

        std::vector<stCheckboxData_t> vCheckboxData;

    };
    struct stHeaderData_t
    {
        HeaderType  eType;
        CString     csText; 
        SIZE        szText;
    };
    struct stHeader_t
    {
        enum class Defer
        {
            Defer,
            Output,
            None,
        };
        stHeaderData_t  stData[m_iMaxHeaderLines];
        bool            bHaveWindowTitle; 
        CString         csText; 
        CString         csFont;
        SIZE            szTextSize;
        POINT           pJustLoc;       // when specific X position is specified (Y is not used, or may be used as an offset)
        Defer           eDefer;         // Some other process will specifically call to have it printed. 
        int             iDeferY;
        int             iNumLines; 
        Just            eJust;
        RgbColor        rgbText;
    };

    struct stItem_t
    {
        POINT   pLoc;
        SIZE    szSize;

        SIZE    szSizeRequired;    // Used to calculate min/max width/height of window

        Control eControl;

        OutType eOutType; 
        void * vOutValue;
        struct stControlData_t
        {
            stCheckbox_t    stCheckbox; 
            stHeader_t      stHeader;
            stEditBox_t     stEditBox;
            stListbox_t     stListbox;
            stCombobox_t    stCombobox;
            stSlider_t      stSlider;
        };
        stControlData_t stControl;
    };
    struct stCheckboxDefaults_t
    {
        const char * sDefault;
        bool bDefault;
    };
    bool Init(const kwOpt & keywords);

private:

    // More defaults

    static constexpr char m_cDefaultOpen = '|';
    static constexpr char m_cDefaultClose = '|'; 
    static constexpr const char * m_sValidateGroupStub = "__Qdialog_validate_"; 

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

    static int        m_iLastValidateGroup;
    int               m_iMaxEditBoxWidth = 0;
    int               m_iEditBoxBoxPos = 0;
    static int        m_iLastGroup;
    CButton         * m_cSysButtons[10]{};        // 10 = logical maximum
    CButton         * m_cButtonOk       = nullptr;
    CButton         * m_cButtonCancel   = nullptr;
    bool              m_bCenterWindow   = true;     // Center in parent window 
    bool              m_bCenterDesktop  = true;     // Center on desktop
    POINT             m_pWinLoc{};                  // user-based location when specified
    std::vector<stItem_t>     m_vcItems;
    int               m_iNumItems = 0;
    CString           m_csErrorMessage;
    std::vector<stElement_t> * m_stElements = nullptr;
    int               m_iNumElements =0 ; 
    SIZE              m_szSysButton{};

    CWindow     * m_cParent = nullptr;
    CWindow     * m_cWin = nullptr;
    HWND          m_hWnd = nullptr;             // Used for CPasWindow::Validator
    HWND          m_hWndParent = nullptr;      
    int           m_iCurrentY           = m_iIndentY;
    int           m_iWidth              = 0;
    int           m_iHeight             = 0;
    int           m_iNumSysButtons      = 2;
    int           m_iSysButtonWidth     = 125;
    SIZE          m_szSysButtonIndent   = { 20, 15 };
    SIZE          m_szSysButtonSpacing  = { 20, 0 };
    bool          m_bGradientInvalid    = false;        // Bug fix for Radio buttons that current do not support transparency, so it 
                                                        // doesn't use a gradient for the window when a radiobutton has been added.
    bool          m_bSetValidateButton  = false;        // When true, OK Button validates Edit Boxes. 

    bool m_bInvalid = true;             // When true, the Dialog Window is invalid (in the case of Dialog box close, invalid window, etc.)

    int     m_iNumControls  = 0;        // Number of actual controls vs. display items (headers, etc.)

    static cwfOpt      cwNoOpt;                 // Empty option usable for defaults

    bool m_bOkButtonPressed = false;
    bool m_bStatusChanged = false;
    bool m_bCanceled = false;
    void CheckCheckboxDefault(stCheckboxData_t & stData);
    
    bool m_bWinNeedsCalc      = true; 
    bool m_bNoClose           = false;  // When true, user cannot close window.  This must be done by calling process.
    bool m_bNoCancel          = false;  // When true, there is no cancel widow and dialog window cannot close (also sets m_bNoClose = true);
    bool m_bNoAutoHide        = false;  // Will not auto-hide the dialog window when user presses Cancel after calling WaitforClose() -- avoids flash
    bool m_bModal             = false;
    bool HandleCheckbox(stElement_t & stElement,const kwOpt & keywords);
    bool HandleHeader(stElement_t & stElement,const kwOpt & keywords);
    bool HandleSlider(stElement_t & stElement);
   // bool HandleEditBox(stElement_t & stElement); 
    bool HandleEditBox(stElement_t & stElement,const kwOpt & keywords); 
    bool HandleListbox(stElement_t & stElement, const kwOpt& keywords);
    bool HandleCombobox(stElement_t & stElement,const kwOpt & keywords);
    bool AddItem(stItem_t & stItem);
    bool AddSysButtons();
    bool CalcWindow();
    bool CalcHeader(stItem_t & stItem);
    bool CalcCheckbox(stItem_t & stItem);
    bool CalcEditBox(stItem_t & stItem);
    bool CalcSlider(stItem_t & stItem);
    bool CalcListbox(stItem_t & stItem);
    bool CalcCombobox(stItem_t & stItem);
    bool _SetModal(bool bModal,bool bOverride = false);                       // Local function to set Windows Modal status
    CString GetGroupName(int iGroup);
    CString CreateNewGroupName();
    stItem_t * GetLastItem();
    bool AddOutValue(OutType eOutType,void * vValue);
    bool CalcOutboundValues();

kSagePrivPublic:
    CQuickDialog(CWindow * cWin,const kwOpt & keywords = kw::none);


    // --------------------------
    // Public Interface Functions
    // --------------------------
public:
    
    /// <summary>
    /// Internal Use Only. Do not use.  The CQuick Dialog object is managed by its parent window and is deleted when the 
    /// parent window is deleted.
    /// <para></para>&#160;&#160;&#160;
    /// Use Close() to close the dialog box and remove it from the window.
    /// </summary>
    static void _Deleter(void * pObj);

    /// <summary>
    /// Closes and invalidates the dialog box.  Similar to Show(false) (which hides the dialog window), but closes the dialog
    /// box permanently.
    /// </summary>
    void Close();

    /// <summary>
    ///  Deprecated. Do not use.
    /// </summary>
    /// <returns></returns>
    bool StartDialog(std::vector<stElement_t> & stElements);


    /// <summary>
    /// Adds a text-string header to the Quick Dialog Window. Also see AddHeaderH3().
    /// <para></para>
    /// --> Multiple lines may be used. The next lines (2 and 3) are smaller. 
    /// <para></para>
    /// --> Use opt::JustLeft() to place the text on the left. Otherwise, it is centered by default.
    /// <para></para>
    /// --> Use color tags in the text to set the color, i.e. "{yellow}Enter a Number\nEnter a number from 1 to 10"
    /// displays the first line in yellow ("{y}" also works) and the second in the default color.
    /// <para></para>
    /// --> Color Tags can be used partially, such as "Enter a number from {g}1{/} to {g}10{/}" displays "1" and "10" in green.
    /// </summary>
    /// <param name="sHeader">- Text of header string.  This can contain multiple lines of text.</param>
    /// <param name="cwOpt"> - (optional) any options, such as opt::JustLeft()</param>
    /// <returns></returns>
    bool AddHeader(const char * sHeader,const kwOpt & keywords = kw::none); 

   /// <summary>
    /// Adds a text-string header to the Quick Dialog Window, with a text smaller than AddHeader(). Also see AddHeader().
    /// <para></para>
    /// --> Multiple lines may be used. The next lines (2 and 3) are smaller. 
    /// <para></para>
    /// --> Use opt::JustLeft() to place the text on the left. Otherwise, it is centered by default.
    /// <para></para>
    /// --> Use color tags in the text to set the color, i.e. "{yellow}Enter a Number\nEnter a number from 1 to 10"
    /// displays the first line in yellow ("{y}" also works) and the second in the default color.
    /// <para></para>
    /// --> Color Tags can be used partially, such as "Enter a number from {g}1{/} to {g}10{/}" displays "1" and "10" in green.
    /// </summary>
    /// <param name="sHeader">- Text of header string.  This can contain multiple lines of text.</param>
    /// <param name="cwOpt"> - (optional) any options, such as opt::JustLeft()</param>
    /// <returns></returns>
    bool AddHeaderH3(const char * sHeader,const kwOpt & keywords = kw::none); 

    /// <summary>
    /// Adds a checkbox to the Quick Dialog window. You can use the opt::Default(true) to set the check ON as default.
    /// <para></para>
    /// --> You can also append "|c|" to the description to check the box, i.e. AddCheckbox("This is a checkbox|c|"); 
    /// <para></para>
    /// --> You can provide bool value automatically fill the checked status when the Quick Dialog Window is closed with the OK button, i.e. AddCheckbox(bChecked,"This is a Checkbox");
    /// <para></para>
    /// --> See AddCheckboxes() to add multiple checkboxes at one time.
    /// </summary>
    /// <param name="bool bChecked"> - (optional) Value to be filled with checked status when Quick Dialog OK button is pressed.</param>
    /// <param name="sCheckboxText"> - Checkbox label</param>
    /// <param name="cwOpt"> - Any options (optional), such as Default() or other checkbox options</param>
    /// <returns>CButton object to access the Checkbox created by the Quick Dialog. Not needed of bChecked reference was used.</returns>
    CButton & AddCheckbox(const char * sCheckboxText,const kwOpt & keywords = kw::none);

    /// <summary>
    /// Adds a checkbox to the Quick Dialog window. You can use the opt::Default(true) to set the check ON as default.
    /// <para></para>
    /// --> You can also append "|c|" to the description to check the box, i.e. AddCheckbox("This is a checkbox|c|"); 
    /// <para></para>
    /// --> You can provide bool value automatically fill the checked status when the Quick Dialog Window is closed with the OK button, i.e. AddCheckbox(bChecked,"This is a Checkbox");
    /// <para></para>
    /// --> See AddCheckboxes() to add multiple checkboxes at one time.
    /// </summary>
    /// <param name="bool bChecked"> - (optional) Value to be filled with checked status when Quick Dialog OK button is pressed.</param>
    /// <param name="sCheckboxText"> - Checkbox label</param>
    /// <param name="cwOpt"> - Any options (optional), such as Default() or other checkbox options</param>
    /// <returns>CButton object to access the Checkbox created by the Quick Dialog. Not needed of bChecked reference was used.</returns>
    CButton & AddCheckbox(const char * sCheckboxText,bool & bError,const kwOpt & keywords = kw::none);
 
    /// <summary>
    /// Adds a checkbox to the Quick Dialog window. You can use the opt::Default(true) to set the check ON as default.
    /// <para></para>
    /// --> You can also append "|c|" to the description to check the box, i.e. AddCheckbox("This is a checkbox|c|"); 
    /// <para></para>
    /// --> You can provide bool value automatically fill the checked status when the Quick Dialog Window is closed with the OK button, i.e. AddCheckbox(bChecked,"This is a Checkbox");
    /// <para></para>
    /// --> See AddCheckboxes() to add multiple checkboxes at one time.
    /// </summary>
    /// <param name="bool bChecked"> - (optional) Value to be filled with checked status when Quick Dialog OK button is pressed.</param>
    /// <param name="sCheckboxText"> - Checkbox label</param>
    /// <param name="cwOpt"> - Any options (optional), such as Default() or other checkbox options</param>
    /// <returns>CButton object to access the Checkbox created by the Quick Dialog. Not needed of bChecked reference was used.</returns>
    CButton & AddCheckbox(bool & bChecked,const char * sCheckboxText,const kwOpt & keywords = kw::none);

    /// <summary>
    /// Adds a checkbox to the Quick Dialog window. You can use the opt::Default(true) to set the check ON as default.
    /// <para></para>
    /// --> You can also append "|c|" to the description to check the box, i.e. AddCheckbox("This is a checkbox|c|"); 
    /// <para></para>
    /// --> You can provide bool value automatically fill the checked status when the Quick Dialog Window is closed with the OK button, i.e. AddCheckbox(bChecked,"This is a Checkbox");
    /// <para></para>
    /// --> See AddCheckboxes() to add multiple checkboxes at one time.
    /// </summary>
    /// <param name="bool bChecked"> - (optional) Value to be filled with checked status when Quick Dialog OK button is pressed.</param>
    /// <param name="sCheckboxText"> - Checkbox label</param>
    /// <param name="cwOpt"> - Any options (optional), such as Default() or other checkbox options</param>
    /// <returns>CButton object to access the Checkbox created by the Quick Dialog. Not needed of bChecked reference was used.</returns>
    CButton & AddCheckbox(bool & bChecked,const char * sCheckboxText,bool & bError,const kwOpt & keywords = kw::none);
 
    /// <summary>
    /// Adds an EditBox (or InputBox) to the Quick Dialog, which can be used to get a string, integer value, or double value from the user. Note: AddEditBox() and AddInputBox() functions are the same.
    /// <para></para>
    /// --> Use the returned CEditbox object to get the string or number from the edit box.  Use opt::Range(), opt::Min, or opt::Max(), to set a range or seperate min/max values.
    /// <para></para>
    /// --> You can provide an integer, double, CString, or std::string value to be filled with the currently displayed value when the OK button is pressed in the Quick Dialog Window.
    /// <para></para>
    /// --> Examples
    /// <para></para>
    /// auto cEditbox = cDialog.AddEditbox(); -- returns cEditBox so that it can be accessed directly as a regular editbox
    /// <para></para>
    /// cDialog.AddEditbox(IntValue,Range(0,100));  -- fills IntValue with integer value. Validates Range of 0-100
    /// </summary>
    /// <param name="Return Variable Name"> - (optional) Name of int, double, std::string, or CString value to fill when OK button is pressed.</param>
    /// <param name="sLabel"> - (optional) Label for edit box (shows above the edit box)</param>
    /// <param name="cwOpt"> - (optional) Options such as opt::Range(), opt::Min(), opt::Max(), etc.</param>
    /// <returns>CEdibox object to access EditBox/Input box created by the Quick Dialog.  Not needed of a variable reference to fill was given instead.</returns>
    CEditBox & AddInputBox(const char * sLabel,const kwOpt & keywords = kw::none);
 

    /// <summary>
    /// Adds an EditBox (or InputBox) to the Quick Dialog, which can be used to get a string, integer value, or double value from the user. Note: AddEditBox() and AddInputBox() functions are the same.
    /// <para></para>
    /// --> Use the returned CEditbox object to get the string or number from the edit box.  Use opt::Range(), opt::Min, or opt::Max(), to set a range or seperate min/max values.
    /// <para></para>
    /// --> You can provide an integer, double, CString, or std::string value to be filled with the currently displayed value when the OK button is pressed in the Quick Dialog Window.
    /// <para></para>
    /// --> Examples
    /// <para></para>
    /// auto cEditbox = cDialog.AddEditbox(); -- returns cEditBox so that it can be accessed directly as a regular editbox
    /// <para></para>
    /// cDialog.AddEditbox(IntValue,Range(0,100));  -- fills IntValue with integer value. Validates Range of 0-100
    /// </summary>
    /// <param name="Return Variable Name"> - (optional) Name of int, double, std::string, or CString value to fill when OK button is pressed.</param>
    /// <param name="sLabel"> - (optional) Label for edit box (shows above the edit box)</param>
    /// <param name="cwOpt"> - (optional) Options such as opt::Range(), opt::Min(), opt::Max(), etc.</param>
    /// <returns>CEdibox object to access EditBox/Input box created by the Quick Dialog.  Not needed of a variable reference to fill was given instead.</returns>
    CEditBox & AddInputBox(const kwOpt & keywords = kw::none);

    /// <summary>
    /// Adds an EditBox (or InputBox) to the Quick Dialog, which can be used to get a string, integer value, or double value from the user. Note: AddEditBox() and AddInputBox() functions are the same.
    /// <para></para>
    /// --> Use the returned CEditbox object to get the string or number from the edit box.  Use opt::Range(), opt::Min, or opt::Max(), to set a range or seperate min/max values.
    /// <para></para>
    /// --> You can provide an integer, double, CString, or std::string value to be filled with the currently displayed value when the OK button is pressed in the Quick Dialog Window.
    /// <para></para>
    /// --> Examples
    /// <para></para>
    /// auto cEditbox = cDialog.AddEditbox(); -- returns cEditBox so that it can be accessed directly as a regular editbox
    /// <para></para>
    /// cDialog.AddEditbox(IntValue,Range(0,100));  -- fills IntValue with integer value. Validates Range of 0-100
    /// </summary>
    /// <param name="Return Variable Name"> - (optional) Name of int, double, std::string, or CString value to fill when OK button is pressed.</param>
    /// <param name="sLabel"> - (optional) Label for edit box (shows above the edit box)</param>
    /// <param name="cwOpt"> - (optional) Options such as opt::Range(), opt::Min(), opt::Max(), etc.</param>
    /// <returns>CEdibox object to access EditBox/Input box created by the Quick Dialog.  Not needed of a variable reference to fill was given instead.</returns>
    CEditBox & AddInputBox(int & iValue,const char * sLabel,const kwOpt & keywords = kw::none);

    /// <summary>
    /// Adds an InputBox (or EditBox) to the Quick Dialog, which can be used to get a string, integer value, or double value from the user. Note: AddEditBox() and AddInputBox() functions are the same.
    /// <para></para>
    /// --> Use the returned CEditbox object to get the string or number from the edit box.  Use opt::Range(), opt::Min, or opt::Max(), to set a range or seperate min/max values.
    /// <para></para>
    /// --> You can provide an integer, double, CString, or std::string value to be filled with the currently displayed value when the OK button is pressed in the Quick Dialog Window.
    /// <para></para>
    /// --> Examples
    /// <para></para>
    /// auto cEditbox = cDialog.AddEditbox(); -- returns cEditBox so that it can be accessed directly as a regular editbox
    /// <para></para>
    /// cDialog.AddEditbox(IntValue,Range(0,100));  -- fills IntValue with integer value. Validates Range of 0-100
    /// </summary>
    /// <param name="Return Variable Name"> - (optional) Name of int, double, std::string, or CString value to fill when OK button is pressed.</param>
    /// <param name="sLabel"> - (optional) Label for edit box (shows above the edit box)</param>
    /// <param name="cwOpt"> - (optional) Options such as opt::Range(), opt::Min(), opt::Max(), etc.</param>
    /// <returns>CEdibox object to access EditBox/Input box created by the Quick Dialog.  Not needed of a variable reference to fill was given instead.</returns>
    CEditBox & AddInputBox(int & iValue,const kwOpt & keywords = kw::none);

    /// <summary>
    /// Adds an EditBox (or InputBox) to the Quick Dialog, which can be used to get a string, integer value, or double value from the user. Note: AddEditBox() and AddInputBox() functions are the same.
    /// <para></para>
    /// --> Use the returned CEditbox object to get the string or number from the edit box.  Use opt::Range(), opt::Min, or opt::Max(), to set a range or seperate min/max values.
    /// <para></para>
    /// --> You can provide an integer, double, CString, or std::string value to be filled with the currently displayed value when the OK button is pressed in the Quick Dialog Window.
    /// <para></para>
    /// --> Examples
    /// <para></para>
    /// auto cEditbox = cDialog.AddEditbox(); -- returns cEditBox so that it can be accessed directly as a regular editbox
    /// <para></para>
    /// cDialog.AddEditbox(IntValue,Range(0,100));  -- fills IntValue with integer value. Validates Range of 0-100
    /// </summary>
    /// <param name="Return Variable Name"> - (optional) Name of int, double, std::string, or CString value to fill when OK button is pressed.</param>
    /// <param name="sLabel"> - (optional) Label for edit box (shows above the edit box)</param>
    /// <param name="cwOpt"> - (optional) Options such as opt::Range(), opt::Min(), opt::Max(), etc.</param>
    /// <returns>CEdibox object to access EditBox/Input box created by the Quick Dialog.  Not needed of a variable reference to fill was given instead.</returns>
    CEditBox & AddInputBox(float & fValue,const char * sLabel,const kwOpt & keywords = kw::none);

    /// <summary>
    /// Adds an InputBox (or EditBox) to the Quick Dialog, which can be used to get a string, integer value, or double value from the user. Note: AddEditBox() and AddInputBox() functions are the same.
    /// <para></para>
    /// --> Use the returned CEditbox object to get the string or number from the edit box.  Use opt::Range(), opt::Min, or opt::Max(), to set a range or seperate min/max values.
    /// <para></para>
    /// --> You can provide an integer, double, CString, or std::string value to be filled with the currently displayed value when the OK button is pressed in the Quick Dialog Window.
    /// <para></para>
    /// --> Examples
    /// <para></para>
    /// auto cEditbox = cDialog.AddEditbox(); -- returns cEditBox so that it can be accessed directly as a regular editbox
    /// <para></para>
    /// cDialog.AddEditbox(IntValue,Range(0,100));  -- fills IntValue with integer value. Validates Range of 0-100
    /// </summary>
    /// <param name="Return Variable Name"> - (optional) Name of int, double, std::string, or CString value to fill when OK button is pressed.</param>
    /// <param name="sLabel"> - (optional) Label for edit box (shows above the edit box)</param>
    /// <param name="cwOpt"> - (optional) Options such as opt::Range(), opt::Min(), opt::Max(), etc.</param>
    /// <returns>CEdibox object to access EditBox/Input box created by the Quick Dialog.  Not needed of a variable reference to fill was given instead.</returns>
    CEditBox & AddInputBox(float & fValue,const kwOpt & keywords = kw::none);

    /// <summary>
    /// Adds an EditBox (or InputBox) to the Quick Dialog, which can be used to get a string, integer value, or double value from the user. Note: AddEditBox() and AddInputBox() functions are the same.
    /// <para></para>
    /// --> Use the returned CEditbox object to get the string or number from the edit box.  Use opt::Range(), opt::Min, or opt::Max(), to set a range or seperate min/max values.
    /// <para></para>
    /// --> You can provide an integer, double, CString, or std::string value to be filled with the currently displayed value when the OK button is pressed in the Quick Dialog Window.
    /// <para></para>
    /// --> Examples
    /// <para></para>
    /// auto cEditbox = cDialog.AddEditbox(); -- returns cEditBox so that it can be accessed directly as a regular editbox
    /// <para></para>
    /// cDialog.AddEditbox(IntValue,Range(0,100));  -- fills IntValue with integer value. Validates Range of 0-100
    /// </summary>
    /// <param name="Return Variable Name"> - (optional) Name of int, double, std::string, or CString value to fill when OK button is pressed.</param>
    /// <param name="sLabel"> - (optional) Label for edit box (shows above the edit box)</param>
    /// <param name="cwOpt"> - (optional) Options such as opt::Range(), opt::Min(), opt::Max(), etc.</param>
    /// <returns>CEdibox object to access EditBox/Input box created by the Quick Dialog.  Not needed of a variable reference to fill was given instead.</returns>
    CEditBox & AddInputBox(double & fValue,const char * sLabel,const kwOpt & keywords = kw::none);

    /// <summary>
    /// Adds an InputBox (or EditBox) to the Quick Dialog, which can be used to get a string, integer value, or double value from the user. Note: AddEditBox() and AddInputBox() functions are the same.
    /// <para></para>
    /// --> Use the returned CEditbox object to get the string or number from the edit box.  Use opt::Range(), opt::Min, or opt::Max(), to set a range or seperate min/max values.
    /// <para></para>
    /// --> You can provide an integer, double, CString, or std::string value to be filled with the currently displayed value when the OK button is pressed in the Quick Dialog Window.
    /// <para></para>
    /// --> Examples
    /// <para></para>
    /// auto cEditbox = cDialog.AddEditbox(); -- returns cEditBox so that it can be accessed directly as a regular editbox
    /// <para></para>
    /// cDialog.AddEditbox(IntValue,Range(0,100));  -- fills IntValue with integer value. Validates Range of 0-100
    /// </summary>
    /// <param name="Return Variable Name"> - (optional) Name of int, double, std::string, or CString value to fill when OK button is pressed.</param>
    /// <param name="sLabel"> - (optional) Label for edit box (shows above the edit box)</param>
    /// <param name="cwOpt"> - (optional) Options such as opt::Range(), opt::Min(), opt::Max(), etc.</param>
    /// <returns>CEdibox object to access EditBox/Input box created by the Quick Dialog.  Not needed of a variable reference to fill was given instead.</returns>
    CEditBox & AddInputBox(double & fValue,const kwOpt & keywords = kw::none);

    /// <summary>
    /// Adds an InputBox (or EditBox) to the Quick Dialog, which can be used to get a string, integer value, or double value from the user. Note: AddEditBox() and AddInputBox() functions are the same.
    /// <para></para>
    /// --> Use the returned CEditbox object to get the string or number from the edit box.  Use opt::Range(), opt::Min, or opt::Max(), to set a range or seperate min/max values.
    /// <para></para>
    /// --> You can provide an integer, double, CString, or std::string value to be filled with the currently displayed value when the OK button is pressed in the Quick Dialog Window.
    /// <para></para>
    /// --> Examples
    /// <para></para>
    /// auto cEditbox = cDialog.AddEditbox(); -- returns cEditBox so that it can be accessed directly as a regular editbox
    /// <para></para>
    /// cDialog.AddEditbox(IntValue,Range(0,100));  -- fills IntValue with integer value. Validates Range of 0-100
    /// </summary>
    /// <param name="Return Variable Name"> - (optional) Name of int, double, std::string, or CString value to fill when OK button is pressed.</param>
    /// <param name="sLabel"> - (optional) Label for edit box (shows above the edit box)</param>
    /// <param name="cwOpt"> - (optional) Options such as opt::Range(), opt::Min(), opt::Max(), etc.</param>
    /// <returns>CEdibox object to access EditBox/Input box created by the Quick Dialog.  Not needed of a variable reference to fill was given instead.</returns>
    CEditBox & AddInputBox(CString & cString,const char * sLabel,const kwOpt & keywords = kw::none);

    /// <summary>
    /// Adds an InputBox (or EditBox) to the Quick Dialog, which can be used to get a string, integer value, or double value from the user. Note: AddEditBox() and AddInputBox() functions are the same.
    /// <para></para>
    /// --> Use the returned CEditbox object to get the string or number from the edit box.  Use opt::Range(), opt::Min, or opt::Max(), to set a range or seperate min/max values.
    /// <para></para>
    /// --> You can provide an integer, double, CString, or std::string value to be filled with the currently displayed value when the OK button is pressed in the Quick Dialog Window.
    /// <para></para>
    /// --> Examples
    /// <para></para>
    /// auto cEditbox = cDialog.AddEditbox(); -- returns cEditBox so that it can be accessed directly as a regular editbox
    /// <para></para>
    /// cDialog.AddEditbox(IntValue,Range(0,100));  -- fills IntValue with integer value. Validates Range of 0-100
    /// </summary>
    /// <param name="Return Variable Name"> - (optional) Name of int, double, std::string, or CString value to fill when OK button is pressed.</param>
    /// <param name="sLabel"> - (optional) Label for edit box (shows above the edit box)</param>
    /// <param name="cwOpt"> - (optional) Options such as opt::Range(), opt::Min(), opt::Max(), etc.</param>
    /// <returns>CEdibox object to access EditBox/Input box created by the Quick Dialog.  Not needed of a variable reference to fill was given instead.</returns>
    CEditBox & AddInputBox(CString & cString,const kwOpt & keywords = kw::none);

    /// <summary>
    /// Adds an InputBox (or EditBox) to the Quick Dialog, which can be used to get a string, integer value, or double value from the user. Note: AddEditBox() and AddInputBox() functions are the same.
    /// <para></para>
    /// --> Use the returned CEditbox object to get the string or number from the edit box.  Use opt::Range(), opt::Min, or opt::Max(), to set a range or seperate min/max values.
    /// <para></para>
    /// --> You can provide an integer, double, CString, or std::string value to be filled with the currently displayed value when the OK button is pressed in the Quick Dialog Window.
    /// <para></para>
    /// --> Examples
    /// <para></para>
    /// auto cEditbox = cDialog.AddEditbox(); -- returns cEditBox so that it can be accessed directly as a regular editbox
    /// <para></para>
    /// cDialog.AddEditbox(IntValue,Range(0,100));  -- fills IntValue with integer value. Validates Range of 0-100
    /// </summary>
    /// <param name="Return Variable Name"> - (optional) Name of int, double, std::string, or CString value to fill when OK button is pressed.</param>
    /// <param name="sLabel"> - (optional) Label for edit box (shows above the edit box)</param>
    /// <param name="cwOpt"> - (optional) Options such as opt::Range(), opt::Min(), opt::Max(), etc.</param>
    /// <returns>CEdibox object to access EditBox/Input box created by the Quick Dialog.  Not needed of a variable reference to fill was given instead.</returns>
    CEditBox & AddInputBox(std::string & cString,const char * sLabel,const kwOpt & keywords = kw::none);

    /// <summary>
    /// Adds an InputBox (or EditBox) to the Quick Dialog, which can be used to get a string, integer value, or double value from the user. Note: AddEditBox() and AddInputBox() functions are the same.
    /// <para></para>
    /// --> Use the returned CEditbox object to get the string or number from the edit box.  Use opt::Range(), opt::Min, or opt::Max(), to set a range or seperate min/max values.
    /// <para></para>
    /// --> You can provide an integer, double, CString, or std::string value to be filled with the currently displayed value when the OK button is pressed in the Quick Dialog Window.
    /// <para></para>
    /// --> Examples
    /// <para></para>
    /// auto cEditbox = cDialog.AddEditbox(); -- returns cEditBox so that it can be accessed directly as a regular editbox
    /// <para></para>
    /// cDialog.AddEditbox(IntValue,Range(0,100));  -- fills IntValue with integer value. Validates Range of 0-100
    /// </summary>
    /// <param name="Return Variable Name"> - (optional) Name of int, double, std::string, or CString value to fill when OK button is pressed.</param>
    /// <param name="sLabel"> - (optional) Label for edit box (shows above the edit box)</param>
    /// <param name="cwOpt"> - (optional) Options such as opt::Range(), opt::Min(), opt::Max(), etc.</param>
    /// <returns>CEdibox object to access EditBox/Input box created by the Quick Dialog.  Not needed of a variable reference to fill was given instead.</returns>
    CEditBox & AddInputBox(std::string & cString,const kwOpt & keywords = kw::none);

    /// <summary>
    /// Adds a Listbox to the Quick Dialog Box. 
    /// <para></para>
    /// --> Use opt::Default() to set the default listbox item (only useful when an Item List is givein in the AddListbox() call.
    /// <para></para>
    /// --> Use opt::Title() to set a title string above the listbox, i.e. AddListbox(Title("This is a listbox")).
    /// <para></para>
    /// --> Omit Width and Height values to use default width/height. Otherwise use 0 for either value for default width or height.
    /// <para></para>
    /// --> An item list string, separated by "\n" for each entry (i.e. "Item 1\nItem 2\n Item3"), can be used to initialize the listbox. 
    /// You can also use a const char * * list that points to each element, with the last entry a nullptr, i.e. const char ** List[4] = {"Item 1","Item 2", "Item 3", nullptr};
    /// </summary>
    /// <param name="iSelection"> - (optional) an integer value to be fulled with the currently highlighed item when OK is pressed in the Quick Dialog.</param>
    /// <param name="iWidth"> - (optional) Width of the Listbox (use 0 for default width)</param>
    /// <param name="iHeight"> - (optional) Height of the Listbox (use 0 for default height)</param>
    /// <param name="sItemList">- (optional) a list of items (either a const char * null-terminated string or const char ** point with a null at the end of the list.</param>
    /// <param name="cwOpt"> - (optional) Any options for the Listbox, such as opt::Title() or opt::Default()</param>
    /// <returns></returns>
    /// <returns>CListbox object to access Listbox created by the Quick Dialog.  Not needed of iSelection is used instead.</returns>
    CListbox & AddListbox(int iWidth,int iHeight,const char * * sItemList, const kwOpt& keywords = kw::none);

    /// <summary>
    /// Adds a Listbox to the Quick Dialog Box. 
    /// <para></para>
    /// --> Use opt::Default() to set the default listbox item (only useful when an Item List is givein in the AddListbox() call.
    /// <para></para>
    /// --> Use opt::Title() to set a title string above the listbox, i.e. AddListbox(Title("This is a listbox")).
    /// <para></para>
    /// --> Omit Width and Height values to use default width/height. Otherwise use 0 for either value for default width or height.
    /// <para></para>
    /// --> An item list string, separated by "\n" for each entry (i.e. "Item 1\nItem 2\n Item3"), can be used to initialize the listbox. 
    /// You can also use a const char * * list that points to each element, with the last entry a nullptr, i.e. const char ** List[4] = {"Item 1","Item 2", "Item 3", nullptr};
    /// </summary>
    /// <param name="iSelection"> - (optional) an integer value to be fulled with the currently highlighed item when OK is pressed in the Quick Dialog.</param>
    /// <param name="iWidth"> - (optional) Width of the Listbox (use 0 for default width)</param>
    /// <param name="iHeight"> - (optional) Height of the Listbox (use 0 for default height)</param>
    /// <param name="sItemList">- (optional) a list of items (either a const char * null-terminated string or const char ** point with a null at the end of the list.</param>
    /// <param name="cwOpt"> - (optional) Any options for the Listbox, such as opt::Title() or opt::Default()</param>
    /// <returns></returns>
    /// <returns>CListbox object to access Listbox created by the Quick Dialog.  Not needed of iSelection is used instead.</returns>
    CListbox& AddListbox(int iWidth, int iHeight, const char* sItemList, const kwOpt & keywords = kw::none);

    /// <summary>
    /// Adds a Listbox to the Quick Dialog Box. 
    /// <para></para>
    /// --> Use opt::Default() to set the default listbox item (only useful when an Item List is givein in the AddListbox() call.
    /// <para></para>
    /// --> Use opt::Title() to set a title string above the listbox, i.e. AddListbox(Title("This is a listbox")).
    /// <para></para>
    /// --> Omit Width and Height values to use default width/height. Otherwise use 0 for either value for default width or height.
    /// <para></para>
    /// --> An item list string, separated by "\n" for each entry (i.e. "Item 1\nItem 2\n Item3"), can be used to initialize the listbox. 
    /// You can also use a const char * * list that points to each element, with the last entry a nullptr, i.e. const char ** List[4] = {"Item 1","Item 2", "Item 3", nullptr};
    /// </summary>
    /// <param name="iSelection"> - (optional) an integer value to be fulled with the currently highlighed item when OK is pressed in the Quick Dialog.</param>
    /// <param name="iWidth"> - (optional) Width of the Listbox (use 0 for default width)</param>
    /// <param name="iHeight"> - (optional) Height of the Listbox (use 0 for default height)</param>
    /// <param name="sItemList">- (optional) a list of items (either a const char * null-terminated string or const char ** point with a null at the end of the list.</param>
    /// <param name="cwOpt"> - (optional) Any options for the Listbox, such as opt::Title() or opt::Default()</param>
    /// <returns></returns>
    /// <returns>CListbox object to access Listbox created by the Quick Dialog.  Not needed of iSelection is used instead.</returns>
    CListbox & AddListbox(int iWidth,int iHeight, const kwOpt& keywords = kw::none);

    /// <summary>
    /// Adds a Listbox to the Quick Dialog Box. 
    /// <para></para>
    /// --> Use opt::Default() to set the default listbox item (only useful when an Item List is givein in the AddListbox() call.
    /// <para></para>
    /// --> Use opt::Title() to set a title string above the listbox, i.e. AddListbox(Title("This is a listbox")).
    /// <para></para>
    /// --> Omit Width and Height values to use default width/height. Otherwise use 0 for either value for default width or height.
    /// <para></para>
    /// --> An item list string, separated by "\n" for each entry (i.e. "Item 1\nItem 2\n Item3"), can be used to initialize the listbox. 
    /// You can also use a const char * * list that points to each element, with the last entry a nullptr, i.e. const char ** List[4] = {"Item 1","Item 2", "Item 3", nullptr};
    /// </summary>
    /// <param name="iSelection"> - (optional) an integer value to be fulled with the currently highlighed item when OK is pressed in the Quick Dialog.</param>
    /// <param name="iWidth"> - (optional) Width of the Listbox (use 0 for default width)</param>
    /// <param name="iHeight"> - (optional) Height of the Listbox (use 0 for default height)</param>
    /// <param name="sItemList">- (optional) a list of items (either a const char * null-terminated string or const char ** point with a null at the end of the list.</param>
    /// <param name="cwOpt"> - (optional) Any options for the Listbox, such as opt::Title() or opt::Default()</param>
    /// <returns></returns>
    /// <returns>CListbox object to access Listbox created by the Quick Dialog.  Not needed of iSelection is used instead.</returns>
    CListbox & AddListbox(const char * * sItemList, const kwOpt& keywords = kw::none);

    /// <summary>
    /// Adds a Listbox to the Quick Dialog Box. 
    /// <para></para>
    /// --> Use opt::Default() to set the default listbox item (only useful when an Item List is givein in the AddListbox() call.
    /// <para></para>
    /// --> Use opt::Title() to set a title string above the listbox, i.e. AddListbox(Title("This is a listbox")).
    /// <para></para>
    /// --> Omit Width and Height values to use default width/height. Otherwise use 0 for either value for default width or height.
    /// <para></para>
    /// --> An item list string, separated by "\n" for each entry (i.e. "Item 1\nItem 2\n Item3"), can be used to initialize the listbox. 
    /// You can also use a const char * * list that points to each element, with the last entry a nullptr, i.e. const char ** List[4] = {"Item 1","Item 2", "Item 3", nullptr};
    /// </summary>
    /// <param name="iSelection"> - (optional) an integer value to be fulled with the currently highlighed item when OK is pressed in the Quick Dialog.</param>
    /// <param name="iWidth"> - (optional) Width of the Listbox (use 0 for default width)</param>
    /// <param name="iHeight"> - (optional) Height of the Listbox (use 0 for default height)</param>
    /// <param name="sItemList">- (optional) a list of items (either a const char * null-terminated string or const char ** point with a null at the end of the list.</param>
    /// <param name="cwOpt"> - (optional) Any options for the Listbox, such as opt::Title() or opt::Default()</param>
    /// <returns></returns>
    /// <returns>CListbox object to access Listbox created by the Quick Dialog.  Not needed of iSelection is used instead.</returns>
    CListbox & AddListbox(const char * sItemList, const kwOpt& keywords = kw::none);

    /// <summary>
    /// Adds a Listbox to the Quick Dialog Box. 
    /// <para></para>
    /// --> Use opt::Default() to set the default listbox item (only useful when an Item List is givein in the AddListbox() call.
    /// <para></para>
    /// --> Use opt::Title() to set a title string above the listbox, i.e. AddListbox(Title("This is a listbox")).
    /// <para></para>
    /// --> Omit Width and Height values to use default width/height. Otherwise use 0 for either value for default width or height.
    /// <para></para>
    /// --> An item list string, separated by "\n" for each entry (i.e. "Item 1\nItem 2\n Item3"), can be used to initialize the listbox. 
    /// You can also use a const char * * list that points to each element, with the last entry a nullptr, i.e. const char ** List[4] = {"Item 1","Item 2", "Item 3", nullptr};
    /// </summary>
    /// <param name="iSelection"> - (optional) an integer value to be fulled with the currently highlighed item when OK is pressed in the Quick Dialog.</param>
    /// <param name="iWidth"> - (optional) Width of the Listbox (use 0 for default width)</param>
    /// <param name="iHeight"> - (optional) Height of the Listbox (use 0 for default height)</param>
    /// <param name="sItemList">- (optional) a list of items (either a const char * null-terminated string or const char ** point with a null at the end of the list.</param>
    /// <param name="cwOpt"> - (optional) Any options for the Listbox, such as opt::Title() or opt::Default()</param>
    /// <returns></returns>
    /// <returns>CListbox object to access Listbox created by the Quick Dialog.  Not needed of iSelection is used instead.</returns>
    CListbox & AddListbox(const kwOpt& keywords = kw::none);

    /// <summary>
    /// Adds a Listbox to the Quick Dialog Box. 
    /// <para></para>
    /// --> Use opt::Default() to set the default listbox item (only useful when an Item List is givein in the AddListbox() call.
    /// <para></para>
    /// --> Use opt::Title() to set a title string above the listbox, i.e. AddListbox(Title("This is a listbox")).
    /// <para></para>
    /// --> Omit Width and Height values to use default width/height. Otherwise use 0 for either value for default width or height.
    /// <para></para>
    /// --> An item list string, separated by "\n" for each entry (i.e. "Item 1\nItem 2\n Item3"), can be used to initialize the listbox. 
    /// You can also use a const char * * list that points to each element, with the last entry a nullptr, i.e. const char ** List[4] = {"Item 1","Item 2", "Item 3", nullptr};
    /// </summary>
    /// <param name="iSelection"> - (optional) an integer value to be fulled with the currently highlighed item when OK is pressed in the Quick Dialog.</param>
    /// <param name="iWidth"> - (optional) Width of the Listbox (use 0 for default width)</param>
    /// <param name="iHeight"> - (optional) Height of the Listbox (use 0 for default height)</param>
    /// <param name="sItemList">- (optional) a list of items (either a const char * null-terminated string or const char ** point with a null at the end of the list.</param>
    /// <param name="cwOpt"> - (optional) Any options for the Listbox, such as opt::Title() or opt::Default()</param>
    /// <returns></returns>
    /// <returns>CListbox object to access Listbox created by the Quick Dialog.  Not needed of iSelection is used instead.</returns>
    CListbox & AddListbox(int & iSelection,int iWidth,int iHeight,const char * * sItemList, const kwOpt& keywords = kw::none);

    /// <summary>
    /// Adds a Listbox to the Quick Dialog Box. 
    /// <para></para>
    /// --> Use opt::Default() to set the default listbox item (only useful when an Item List is givein in the AddListbox() call.
    /// <para></para>
    /// --> Use opt::Title() to set a title string above the listbox, i.e. AddListbox(Title("This is a listbox")).
    /// <para></para>
    /// --> Omit Width and Height values to use default width/height. Otherwise use 0 for either value for default width or height.
    /// <para></para>
    /// --> An item list string, separated by "\n" for each entry (i.e. "Item 1\nItem 2\n Item3"), can be used to initialize the listbox. 
    /// You can also use a const char * * list that points to each element, with the last entry a nullptr, i.e. const char ** List[4] = {"Item 1","Item 2", "Item 3", nullptr};
    /// </summary>
    /// <param name="iSelection"> - (optional) an integer value to be fulled with the currently highlighed item when OK is pressed in the Quick Dialog.</param>
    /// <param name="iWidth"> - (optional) Width of the Listbox (use 0 for default width)</param>
    /// <param name="iHeight"> - (optional) Height of the Listbox (use 0 for default height)</param>
    /// <param name="sItemList">- (optional) a list of items (either a const char * null-terminated string or const char ** point with a null at the end of the list.</param>
    /// <param name="cwOpt"> - (optional) Any options for the Listbox, such as opt::Title() or opt::Default()</param>
    /// <returns></returns>
    /// <returns>CListbox object to access Listbox created by the Quick Dialog.  Not needed of iSelection is used instead.</returns>
    CListbox& AddListbox(int& iSelection, int iWidth, int iHeight, const char* sItemList, const kwOpt& keywords = kw::none);

    /// <summary>
    /// Adds a Listbox to the Quick Dialog Box. 
    /// <para></para>
    /// --> Use opt::Default() to set the default listbox item (only useful when an Item List is givein in the AddListbox() call.
    /// <para></para>
    /// --> Use opt::Title() to set a title string above the listbox, i.e. AddListbox(Title("This is a listbox")).
    /// <para></para>
    /// --> Omit Width and Height values to use default width/height. Otherwise use 0 for either value for default width or height.
    /// <para></para>
    /// --> An item list string, separated by "\n" for each entry (i.e. "Item 1\nItem 2\n Item3"), can be used to initialize the listbox. 
    /// You can also use a const char * * list that points to each element, with the last entry a nullptr, i.e. const char ** List[4] = {"Item 1","Item 2", "Item 3", nullptr};
    /// </summary>
    /// <param name="iSelection"> - (optional) an integer value to be fulled with the currently highlighed item when OK is pressed in the Quick Dialog.</param>
    /// <param name="iWidth"> - (optional) Width of the Listbox (use 0 for default width)</param>
    /// <param name="iHeight"> - (optional) Height of the Listbox (use 0 for default height)</param>
    /// <param name="sItemList">- (optional) a list of items (either a const char * null-terminated string or const char ** point with a null at the end of the list.</param>
    /// <param name="cwOpt"> - (optional) Any options for the Listbox, such as opt::Title() or opt::Default()</param>
    /// <returns></returns>
    /// <returns>CListbox object to access Listbox created by the Quick Dialog.  Not needed of iSelection is used instead.</returns>
    CListbox & AddListbox(int & iSelection,int iWidth,int iHeight, const kwOpt& keywords = kw::none);

    /// <summary>
    /// Adds a Listbox to the Quick Dialog Box. 
    /// <para></para>
    /// --> Use opt::Default() to set the default listbox item (only useful when an Item List is givein in the AddListbox() call.
    /// <para></para>
    /// --> Use opt::Title() to set a title string above the listbox, i.e. AddListbox(Title("This is a listbox")).
    /// <para></para>
    /// --> Omit Width and Height values to use default width/height. Otherwise use 0 for either value for default width or height.
    /// <para></para>
    /// --> An item list string, separated by "\n" for each entry (i.e. "Item 1\nItem 2\n Item3"), can be used to initialize the listbox. 
    /// You can also use a const char * * list that points to each element, with the last entry a nullptr, i.e. const char ** List[4] = {"Item 1","Item 2", "Item 3", nullptr};
    /// </summary>
    /// <param name="iSelection"> - (optional) an integer value to be fulled with the currently highlighed item when OK is pressed in the Quick Dialog.</param>
    /// <param name="iWidth"> - (optional) Width of the Listbox (use 0 for default width)</param>
    /// <param name="iHeight"> - (optional) Height of the Listbox (use 0 for default height)</param>
    /// <param name="sItemList">- (optional) a list of items (either a const char * null-terminated string or const char ** point with a null at the end of the list.</param>
    /// <param name="cwOpt"> - (optional) Any options for the Listbox, such as opt::Title() or opt::Default()</param>
    /// <returns></returns>
    /// <returns>CListbox object to access Listbox created by the Quick Dialog.  Not needed of iSelection is used instead.</returns>
    CListbox & AddListbox(int & iSelection,const char * * sItemList, const kwOpt& keywords = kw::none);

    /// <summary>
    /// Adds a Listbox to the Quick Dialog Box. 
    /// <para></para>
    /// --> Use opt::Default() to set the default listbox item (only useful when an Item List is givein in the AddListbox() call.
    /// <para></para>
    /// --> Use opt::Title() to set a title string above the listbox, i.e. AddListbox(Title("This is a listbox")).
    /// <para></para>
    /// --> Omit Width and Height values to use default width/height. Otherwise use 0 for either value for default width or height.
    /// <para></para>
    /// --> An item list string, separated by "\n" for each entry (i.e. "Item 1\nItem 2\n Item3"), can be used to initialize the listbox. 
    /// You can also use a const char * * list that points to each element, with the last entry a nullptr, i.e. const char ** List[4] = {"Item 1","Item 2", "Item 3", nullptr};
    /// </summary>
    /// <param name="iSelection"> - (optional) an integer value to be fulled with the currently highlighed item when OK is pressed in the Quick Dialog.</param>
    /// <param name="iWidth"> - (optional) Width of the Listbox (use 0 for default width)</param>
    /// <param name="iHeight"> - (optional) Height of the Listbox (use 0 for default height)</param>
    /// <param name="sItemList">- (optional) a list of items (either a const char * null-terminated string or const char ** point with a null at the end of the list.</param>
    /// <param name="cwOpt"> - (optional) Any options for the Listbox, such as opt::Title() or opt::Default()</param>
    /// <returns></returns>
    /// <returns>CListbox object to access Listbox created by the Quick Dialog.  Not needed of iSelection is used instead.</returns>
    CListbox & AddListbox(int & iSelection,const char * sItemList, const kwOpt& keywords = kw::none);

    /// <summary>
    /// Adds a Listbox to the Quick Dialog Box. 
    /// <para></para>
    /// --> Use opt::Default() to set the default listbox item (only useful when an Item List is givein in the AddListbox() call.
    /// <para></para>
    /// --> Use opt::Title() to set a title string above the listbox, i.e. AddListbox(Title("This is a listbox")).
    /// <para></para>
    /// --> Omit Width and Height values to use default width/height. Otherwise use 0 for either value for default width or height.
    /// <para></para>
    /// --> An item list string, separated by "\n" for each entry (i.e. "Item 1\nItem 2\n Item3"), can be used to initialize the listbox. 
    /// You can also use a const char * * list that points to each element, with the last entry a nullptr, i.e. const char ** List[4] = {"Item 1","Item 2", "Item 3", nullptr};
    /// </summary>
    /// <param name="iSelection"> - (optional) an integer value to be fulled with the currently highlighed item when OK is pressed in the Quick Dialog.</param>
    /// <param name="iWidth"> - (optional) Width of the Listbox (use 0 for default width)</param>
    /// <param name="iHeight"> - (optional) Height of the Listbox (use 0 for default height)</param>
    /// <param name="sItemList">- (optional) a list of items (either a const char * null-terminated string or const char ** point with a null at the end of the list.</param>
    /// <param name="cwOpt"> - (optional) Any options for the Listbox, such as opt::Title() or opt::Default()</param>
    /// <returns>CListbox object to access Listbox created by the Quick Dialog.  Not needed of iSelection is used instead.</returns>
    CListbox & AddListbox(int & iSelection, const kwOpt& keywords = kw::none);

    /// <summary>
    /// Adds a Combobox to the Quick Dialog Box. 
    /// <para></para>
    /// --> Use opt::Default() to set the default combobox item (only useful when an Item List is givein in the AddCombobox() call.
    /// <para></para>
    /// --> Use opt::Title() to set a title string above the combobox, i.e. AddCombobox(Title("This is a combobox")).
    /// <para></para>
    /// --> An item list string, separated by "\n" for each entry (i.e. "Item 1\nItem 2\n Item3"), can be used to initialize the combobox. 
    /// You can also use a const char * * list that points to each element, with the last entry a nullptr, i.e. const char ** List[4] = {"Item 1","Item 2", "Item 3", nullptr};
    /// </summary>
    /// <param name="iSelection"> - (optional) an integer value to be fulled with the currently highlighed item when OK is pressed in the Quick Dialog.</param>
    /// <param name="iWidth"> - (optional) Width of the Combobox (use 0 for default width)</param>
    /// <param name="sItemList">- (optional) a list of items (either a const char * null-terminated string or const char ** point with a null at the end of the list.</param>
    /// <param name="cwOpt"> - (optional) Any options for the Combobox, such as opt::Title() or opt::Default()</param>
    /// <returns>Combobox object created for the Quick Dialog (not needed if iSelection is used instead)</returns>
    CCombobox & AddCombobox(int iWidth,const char * * sItemList,const kwOpt & keywords = kw::none); 

    /// <summary>
    /// Adds a Combobox to the Quick Dialog Box. 
    /// <para></para>
    /// --> Use opt::Default() to set the default combobox item (only useful when an Item List is givein in the AddCombobox() call.
    /// <para></para>
    /// --> Use opt::Title() to set a title string above the combobox, i.e. AddCombobox(Title("This is a combobox")).
    /// <para></para>
    /// --> An item list string, separated by "\n" for each entry (i.e. "Item 1\nItem 2\n Item3"), can be used to initialize the combobox. 
    /// You can also use a const char * * list that points to each element, with the last entry a nullptr, i.e. const char ** List[4] = {"Item 1","Item 2", "Item 3", nullptr};
    /// </summary>
    /// <param name="iSelection"> - (optional) an integer value to be fulled with the currently highlighed item when OK is pressed in the Quick Dialog.</param>
    /// <param name="iWidth"> - (optional) Width of the Combobox (use 0 for default width)</param>
    /// <param name="sItemList">- (optional) a list of items (either a const char * null-terminated string or const char ** point with a null at the end of the list.</param>
    /// <param name="cwOpt"> - (optional) Any options for the Combobox, such as opt::Title() or opt::Default()</param>
    /// <returns>Combobox object created for the Quick Dialog (not needed if iSelection is used instead)</returns>
    CCombobox & AddCombobox(int iWidth,const char * sItemList,const kwOpt & keywords = kw::none); 

    /// <summary>
    /// Adds a Combobox to the Quick Dialog Box. 
    /// <para></para>
    /// --> Use opt::Default() to set the default combobox item (only useful when an Item List is givein in the AddCombobox() call.
    /// <para></para>
    /// --> Use opt::Title() to set a title string above the combobox, i.e. AddCombobox(Title("This is a combobox")).
    /// <para></para>
    /// --> An item list string, separated by "\n" for each entry (i.e. "Item 1\nItem 2\n Item3"), can be used to initialize the combobox. 
    /// You can also use a const char * * list that points to each element, with the last entry a nullptr, i.e. const char ** List[4] = {"Item 1","Item 2", "Item 3", nullptr};
    /// </summary>
    /// <param name="iSelection"> - (optional) an integer value to be fulled with the currently highlighed item when OK is pressed in the Quick Dialog.</param>
    /// <param name="iWidth"> - (optional) Width of the Combobox (use 0 for default width)</param>
    /// <param name="sItemList">- (optional) a list of items (either a const char * null-terminated string or const char ** point with a null at the end of the list.</param>
    /// <param name="cwOpt"> - (optional) Any options for the Combobox, such as opt::Title() or opt::Default()</param>
    /// <returns>Combobox object created for the Quick Dialog (not needed if iSelection is used instead)</returns>
    CCombobox & AddCombobox(int iWidth,const kwOpt & keywords = kw::none);  

    /// <summary>
    /// Adds a Combobox to the Quick Dialog Box. 
    /// <para></para>
    /// --> Use opt::Default() to set the default combobox item (only useful when an Item List is givein in the AddCombobox() call.
    /// <para></para>
    /// --> Use opt::Title() to set a title string above the combobox, i.e. AddCombobox(Title("This is a combobox")).
    /// <para></para>
    /// --> An item list string, separated by "\n" for each entry (i.e. "Item 1\nItem 2\n Item3"), can be used to initialize the combobox. 
    /// You can also use a const char * * list that points to each element, with the last entry a nullptr, i.e. const char ** List[4] = {"Item 1","Item 2", "Item 3", nullptr};
    /// </summary>
    /// <param name="iSelection"> - (optional) an integer value to be fulled with the currently highlighed item when OK is pressed in the Quick Dialog.</param>
    /// <param name="iWidth"> - (optional) Width of the Combobox (use 0 for default width)</param>
    /// <param name="sItemList">- (optional) a list of items (either a const char * null-terminated string or const char ** point with a null at the end of the list.</param>
    /// <param name="cwOpt"> - (optional) Any options for the Combobox, such as opt::Title() or opt::Default()</param>
    /// <returns>Combobox object created for the Quick Dialog (not needed if iSelection is used instead)</returns>
    CCombobox & AddCombobox(const char * sItemList,const kwOpt & keywords = kw::none); 

    /// <summary>
    /// Adds a Combobox to the Quick Dialog Box. 
    /// <para></para>
    /// --> Use opt::Default() to set the default combobox item (only useful when an Item List is givein in the AddCombobox() call.
    /// <para></para>
    /// --> Use opt::Title() to set a title string above the combobox, i.e. AddCombobox(Title("This is a combobox")).
    /// <para></para>
    /// --> An item list string, separated by "\n" for each entry (i.e. "Item 1\nItem 2\n Item3"), can be used to initialize the combobox. 
    /// You can also use a const char * * list that points to each element, with the last entry a nullptr, i.e. const char ** List[4] = {"Item 1","Item 2", "Item 3", nullptr};
    /// </summary>
    /// <param name="iSelection"> - (optional) an integer value to be fulled with the currently highlighed item when OK is pressed in the Quick Dialog.</param>
    /// <param name="iWidth"> - (optional) Width of the Combobox (use 0 for default width)</param>
    /// <param name="sItemList">- (optional) a list of items (either a const char * null-terminated string or const char ** point with a null at the end of the list.</param>
    /// <param name="cwOpt"> - (optional) Any options for the Combobox, such as opt::Title() or opt::Default()</param>
    /// <returns>Combobox object created for the Quick Dialog (not needed if iSelection is used instead)</returns>
    CCombobox & AddCombobox(const char * * sItemList,const kwOpt & keywords = kw::none); 

    /// <summary>
    /// Adds a Combobox to the Quick Dialog Box. 
    /// <para></para>
    /// --> Use opt::Default() to set the default combobox item (only useful when an Item List is givein in the AddCombobox() call.
    /// <para></para>
    /// --> Use opt::Title() to set a title string above the combobox, i.e. AddCombobox(Title("This is a combobox")).
    /// <para></para>
    /// --> An item list string, separated by "\n" for each entry (i.e. "Item 1\nItem 2\n Item3"), can be used to initialize the combobox. 
    /// You can also use a const char * * list that points to each element, with the last entry a nullptr, i.e. const char ** List[4] = {"Item 1","Item 2", "Item 3", nullptr};
    /// </summary>
    /// <param name="iSelection"> - (optional) an integer value to be fulled with the currently highlighed item when OK is pressed in the Quick Dialog.</param>
    /// <param name="iWidth"> - (optional) Width of the Combobox (use 0 for default width)</param>
    /// <param name="sItemList">- (optional) a list of items (either a const char * null-terminated string or const char ** point with a null at the end of the list.</param>
    /// <param name="cwOpt"> - (optional) Any options for the Combobox, such as opt::Title() or opt::Default()</param>
    /// <returns>Combobox object created for the Quick Dialog (not needed if iSelection is used instead)</returns>
    CCombobox & AddCombobox(const kwOpt & keywords = kw::none); 

    /// <summary>
    /// Adds a Combobox to the Quick Dialog Box. 
    /// <para></para>
    /// --> Use opt::Default() to set the default combobox item (only useful when an Item List is givein in the AddCombobox() call.
    /// <para></para>
    /// --> Use opt::Title() to set a title string above the combobox, i.e. AddCombobox(Title("This is a combobox")).
    /// <para></para>
    /// --> An item list string, separated by "\n" for each entry (i.e. "Item 1\nItem 2\n Item3"), can be used to initialize the combobox. 
    /// You can also use a const char * * list that points to each element, with the last entry a nullptr, i.e. const char ** List[4] = {"Item 1","Item 2", "Item 3", nullptr};
    /// </summary>
    /// <param name="iSelection"> - (optional) an integer value to be fulled with the currently highlighed item when OK is pressed in the Quick Dialog.</param>
    /// <param name="iWidth"> - (optional) Width of the Combobox (use 0 for default width)</param>
    /// <param name="sItemList">- (optional) a list of items (either a const char * null-terminated string or const char ** point with a null at the end of the list.</param>
    /// <param name="cwOpt"> - (optional) Any options for the Combobox, such as opt::Title() or opt::Default()</param>
    /// <returns>Combobox object created for the Quick Dialog (not needed if iSelection is used instead)</returns>
    CCombobox & AddCombobox_Set(int & iSelection,int iWidth,const char * * sItemList,const kwOpt & keywords = kw::none);  

    /// <summary>
    /// Adds a Combobox to the Quick Dialog Box. 
    /// <para></para>
    /// --> Use opt::Default() to set the default combobox item (only useful when an Item List is givein in the AddCombobox() call.
    /// <para></para>
    /// --> Use opt::Title() to set a title string above the combobox, i.e. AddCombobox(Title("This is a combobox")).
    /// <para></para>
    /// --> An item list string, separated by "\n" for each entry (i.e. "Item 1\nItem 2\n Item3"), can be used to initialize the combobox. 
    /// You can also use a const char * * list that points to each element, with the last entry a nullptr, i.e. const char ** List[4] = {"Item 1","Item 2", "Item 3", nullptr};
    /// </summary>
    /// <param name="iSelection"> - (optional) an integer value to be fulled with the currently highlighed item when OK is pressed in the Quick Dialog.</param>
    /// <param name="iWidth"> - (optional) Width of the Combobox (use 0 for default width)</param>
    /// <param name="sItemList">- (optional) a list of items (either a const char * null-terminated string or const char ** point with a null at the end of the list.</param>
    /// <param name="cwOpt"> - (optional) Any options for the Combobox, such as opt::Title() or opt::Default()</param>
    /// <returns>Combobox object created for the Quick Dialog (not needed if iSelection is used instead)</returns>
    CCombobox & AddCombobox_Set(int & iSelection,int iWidth,const char * sItemList,const kwOpt & keywords = kw::none); 

    /// <summary>
    /// Adds a Combobox to the Quick Dialog Box. 
    /// <para></para>
    /// --> Use opt::Default() to set the default combobox item (only useful when an Item List is givein in the AddCombobox() call.
    /// <para></para>
    /// --> Use opt::Title() to set a title string above the combobox, i.e. AddCombobox(Title("This is a combobox")).
    /// <para></para>
    /// --> An item list string, separated by "\n" for each entry (i.e. "Item 1\nItem 2\n Item3"), can be used to initialize the combobox. 
    /// You can also use a const char * * list that points to each element, with the last entry a nullptr, i.e. const char ** List[4] = {"Item 1","Item 2", "Item 3", nullptr};
    /// </summary>
    /// <param name="iSelection"> - (optional) an integer value to be fulled with the currently highlighed item when OK is pressed in the Quick Dialog.</param>
    /// <param name="iWidth"> - (optional) Width of the Combobox (use 0 for default width)</param>
    /// <param name="sItemList">- (optional) a list of items (either a const char * null-terminated string or const char ** point with a null at the end of the list.</param>
    /// <param name="cwOpt"> - (optional) Any options for the Combobox, such as opt::Title() or opt::Default()</param>
    /// <returns>Combobox object created for the Quick Dialog (not needed if iSelection is used instead)</returns>
    CCombobox & AddCombobox_Set(int & iSelection,int iWidth,const kwOpt & keywords = kw::none);  

    /// <summary>
    /// Adds a Combobox to the Quick Dialog Box. 
    /// <para></para>
    /// --> Use opt::Default() to set the default combobox item (only useful when an Item List is givein in the AddCombobox() call.
    /// <para></para>
    /// --> Use opt::Title() to set a title string above the combobox, i.e. AddCombobox(Title("This is a combobox")).
    /// <para></para>
    /// --> An item list string, separated by "\n" for each entry (i.e. "Item 1\nItem 2\n Item3"), can be used to initialize the combobox. 
    /// You can also use a const char * * list that points to each element, with the last entry a nullptr, i.e. const char ** List[4] = {"Item 1","Item 2", "Item 3", nullptr};
    /// </summary>
    /// <param name="iSelection"> - (optional) an integer value to be fulled with the currently highlighed item when OK is pressed in the Quick Dialog.</param>
    /// <param name="iWidth"> - (optional) Width of the Combobox (use 0 for default width)</param>
    /// <param name="sItemList">- (optional) a list of items (either a const char * null-terminated string or const char ** point with a null at the end of the list.</param>
    /// <param name="cwOpt"> - (optional) Any options for the Combobox, such as opt::Title() or opt::Default()</param>
    /// <returns>Combobox object created for the Quick Dialog (not needed if iSelection is used instead)</returns>
    CCombobox & AddCombobox_Set(int & iSelection,const char * sItemList,const kwOpt & keywords = kw::none); 

    /// <summary>
    /// Adds a Combobox to the Quick Dialog Box. 
    /// <para></para>
    /// --> Use opt::Default() to set the default combobox item (only useful when an Item List is givein in the AddCombobox() call.
    /// <para></para>
    /// --> Use opt::Title() to set a title string above the combobox, i.e. AddCombobox(Title("This is a combobox")).
    /// <para></para>
    /// --> An item list string, separated by "\n" for each entry (i.e. "Item 1\nItem 2\n Item3"), can be used to initialize the combobox. 
    /// You can also use a const char * * list that points to each element, with the last entry a nullptr, i.e. const char ** List[4] = {"Item 1","Item 2", "Item 3", nullptr};
    /// </summary>
    /// <param name="iSelection"> - (optional) an integer value to be fulled with the currently highlighed item when OK is pressed in the Quick Dialog.</param>
    /// <param name="iWidth"> - (optional) Width of the Combobox (use 0 for default width)</param>
    /// <param name="sItemList">- (optional) a list of items (either a const char * null-terminated string or const char ** point with a null at the end of the list.</param>
    /// <param name="cwOpt"> - (optional) Any options for the Combobox, such as opt::Title() or opt::Default()</param>
    /// <returns>Combobox object created for the Quick Dialog (not needed if iSelection is used instead)</returns>
    CCombobox & AddCombobox_Set(int & iSelection,const char * * sItemList,const kwOpt & keywords = kw::none); 

    /// <summary>
    /// Adds a Combobox to the Quick Dialog Box. 
    /// <para></para>
    /// --> Use opt::Default() to set the default combobox item (only useful when an Item List is givein in the AddCombobox() call.
    /// <para></para>
    /// --> Use opt::Title() to set a title string above the combobox, i.e. AddCombobox(Title("This is a combobox")).
    /// <para></para>
    /// --> An item list string, separated by "\n" for each entry (i.e. "Item 1\nItem 2\n Item3"), can be used to initialize the combobox. 
    /// You can also use a const char * * list that points to each element, with the last entry a nullptr, i.e. const char ** List[4] = {"Item 1","Item 2", "Item 3", nullptr};
    /// </summary>
    /// <param name="iSelection"> - (optional) an integer value to be fulled with the currently highlighed item when OK is pressed in the Quick Dialog.</param>
    /// <param name="iWidth"> - (optional) Width of the Combobox (use 0 for default width)</param>
    /// <param name="sItemList">- (optional) a list of items (either a const char * null-terminated string or const char ** point with a null at the end of the list.</param>
    /// <param name="cwOpt"> - (optional) Any options for the Combobox, such as opt::Title() or opt::Default()</param>
    /// <returns>Combobox object created for the Quick Dialog (not needed if iSelection is used instead)</returns>
    CCombobox & AddCombobox_Set(int & iSelection,const kwOpt & keywords = kw::none); 

    /// <summary>
    /// Add multiple checkboxes (horizontally or vertically) to the Quick Dialog Window.
    /// <para></para>
    /// --> Use opt::Horz() to specify horizontally-aligned checkboxes (default is one each row)
    /// <para></para>
    /// --> Use the sItemList to set the checkbox names, delimited by \n for each name: i.e. "Checkbox 1\nCheckbox 2\nCheckbox 3".
    /// <para></para>
    /// --> Use "|c|" after each label to specify the checkbox should be pre-checked, i.e. "Checkbox 1\nCheckbox 2|c|\nCheckbox 3"
    /// </summary>
    /// <param name="sCheckboxes"> - name of checkboxes, separate by "\n", i.e. "Checkbox 1\nCheckbox 2|c|\nCheckbox 3", where "|c|" specified pre-checked.</param>
    /// <param name="bError"> - (optional) Not used [deprecated]</param>
    /// <param name="cwOpt"> - (optional) Checkbox options, such as opt::Horz().  opt::Default(true) only checks the first checkbox</param>
    /// <returns>Control Group object to access checkboxes (by order of placement, i.e. 0-N)</returns>
    ControlGroup AddCheckboxes(const char * sCheckboxes,const kwOpt & keywords = kw::none);

    /// <summary>
    /// Add multiple checkboxes (horizontally or vertically) to the Quick Dialog Window.
    /// <para></para>
    /// --> Use opt::Horz() to specify horizontally-aligned checkboxes (default is one each row)
    /// <para></para>
    /// --> Use the sCheckboxes parameter to set the checkbox names, delimited by \n for each name: i.e. "Checkbox 1\nCheckbox 2\nCheckbox 3".
    /// <para></para>
    /// --> Use "|c|" after each label to specify the checkbox should be pre-checked, i.e. "Checkbox 1\nCheckbox 2|c|\nCheckbox 3"
    /// </summary>
    /// <param name="sCheckboxes"> - name of checkboxes, separate by "\n", i.e. "Checkbox 1\nCheckbox 2|c|\nCheckbox 3", where "|c|" specified pre-checked.</param>
    /// <param name="bError"> - (optional) Not used [deprecated]</param>
    /// <param name="cwOpt"> - (optional) Checkbox options, such as opt::Horz().  opt::Default(true) only checks the first checkbox</param>
    /// <returns>Control Group object to access checkboxes (by order of placement, i.e. 0-N)</returns>
    ControlGroup AddCheckboxes(const char * sCheckboxes,bool & bError,const kwOpt & keywords = kw::none);
    
    /// <summary>
    /// Adds a set of radio buttons to the Quick Dialog window, either vertically (default) or horizontally.
    /// <para></para>
    /// --> Use opt::Horz() to specify horizontally-aligned checkboxes (default is one each row)
    /// <para></para>
    /// --> Use opt::Default() to set the default filled/checked radio box (i.e. Default(1) for the second radio box)
    /// <para></para>
    /// --> Use the sRadioButtons parameter to set the radiobox names, delimited by \n for each name: i.e. "Radio box 1\Radio box 2\Radio box 3".
    /// </summary>
    /// <param name="sRadioButtons"></param>
    /// <param name="iSelection"> - (optional) Integer variable name to fill the index (0-based) of the currently filled radio button when the OK button is pressed in the Quick Dialog Window.</param>
    /// <param name="bError"> - (optional) Not used [deprecated]</param>
    /// <param name="cwOpt"> - (optional) Checkbox options, such as opt::Horz().  opt::Default(true) only checks the first checkbox</param>
    /// <returns>Control Group object to access the radio buttons.  Not needed of the iSelection value was used to fill the returned value.</returns>
    ControlGroup AddRadioButtons(const char * sRadioButtons,const kwOpt & keywords = kw::none);

    /// <summary>
    /// Adds a set of radio buttons to the Quick Dialog window, either vertically (default) or horizontally.
    /// <para></para>
    /// --> Use opt::Horz() to specify horizontally-aligned checkboxes (default is one each row)
    /// <para></para>
    /// --> Use opt::Default() to set the default filled/checked radio box (i.e. Default(1) for the second radio box)
    /// <para></para>
    /// --> Use the sRadioButtons parameter to set the radiobox names, delimited by \n for each name: i.e. "Radio box 1\Radio box 2\Radio box 3".
    /// </summary>
    /// <param name="sRadioButtons"></param>
    /// <param name="iSelection"> - (optional) Integer variable name to fill the index (0-based) of the currently filled radio button when the OK button is pressed in the Quick Dialog Window.</param>
    /// <param name="bError"> - (optional) Not used [deprecated]</param>
    /// <param name="cwOpt"> - (optional) Checkbox options, such as opt::Horz().  opt::Default(true) only checks the first checkbox</param>
    /// <returns>Control Group object to access the radio buttons.  Not needed of the iSelection value was used to fill the returned value.</returns>
    ControlGroup AddRadioButtons(const char * sRadioButtons,bool & bError,const kwOpt & keywords = kw::none);
    
    /// <summary>
    /// Adds a set of radio buttons to the Quick Dialog window, either vertically (default) or horizontally.
    /// <para></para>
    /// --> Use opt::Horz() to specify horizontally-aligned checkboxes (default is one each row)
    /// <para></para>
    /// --> Use opt::Default() to set the default filled/checked radio box (i.e. Default(1) for the second radio box)
    /// <para></para>
    /// --> Use the sRadioButtons parameter to set the radiobox names, delimited by \n for each name: i.e. "Radio box 1\Radio box 2\Radio box 3".
    /// </summary>
    /// <param name="sRadioButtons"></param>
    /// <param name="iSelection"> - (optional) Integer variable name to fill the index (0-based) of the currently filled radio button when the OK button is pressed in the Quick Dialog Window.</param>
    /// <param name="bError"> - (optional) Not used [deprecated]</param>
    /// <param name="cwOpt"> - (optional) Checkbox options, such as opt::Horz().  opt::Default(true) only checks the first checkbox</param>
    /// <returns>Control Group object to access the radio buttons.  Not needed of the iSelection value was used to fill the returned value.</returns>
    ControlGroup AddRadioButtons(int & iSelection,const char * sRadioButtons,const kwOpt & keywords = kw::none);

    /// <summary>
    /// Adds a set of radio buttons to the Quick Dialog window, either vertically (default) or horizontally.
    /// <para></para>
    /// --> Use opt::Horz() to specify horizontally-aligned checkboxes (default is one each row)
    /// <para></para>
    /// --> Use opt::Default() to set the default filled/checked radio box (i.e. Default(1) for the second radio box)
    /// <para></para>
    /// --> Use the sRadioButtons parameter to set the radiobox names, delimited by \n for each name: i.e. "Radio box 1\Radio box 2\Radio box 3".
    /// </summary>
    /// <param name="sRadioButtons"></param>
    /// <param name="iSelection"> - (optional) Integer variable name to fill the index (0-based) of the currently filled radio button when the OK button is pressed in the Quick Dialog Window.</param>
    /// <param name="bError"> - (optional) Not used [deprecated]</param>
    /// <param name="cwOpt"> - (optional) Checkbox options, such as opt::Horz().  opt::Default(true) only checks the first checkbox</param>
    /// <returns>Control Group object to access the radio buttons.  Not needed of the iSelection value was used to fill the returned value.</returns>
    ControlGroup AddRadioButtons(int & iSelection,const char * sRadioButtons,bool & bError,const kwOpt & keywords = kw::none);

    static CQuickDialog m_cEmptyDialog;     // This is used to return a reference for an empty/non-functional Quick Dialog box when the state of the system is unknonw,

    /// <summary>
    /// Waits for the dialog window to close. Pressing the OK or Cancel button will close the window, as will closing the
    /// window with the 'X' (top-right) button, unless the close has been disabled.  
    /// <para></para>
    /// This hides the Dialog Window but does not 
    /// delete the Dialog or interfere with retrieving values from the dialog controls.
    /// <para></para>&#160;&#160;&#160;
    /// After the window is closed, the status (i.e. OK or Cancel button pressed) may be queried with GetStatus(). 
    /// <para></para>
    /// Once closed, all controls are still accessible so that their values may be retreived. 
    /// <para></para>
    /// The Dialog Window may then be closed with QuickDialog::Close(), or re-activated with QuickDialog::Show()
    /// <para></para>&#160;&#160;&#160;
    /// Note: You do not need to use CDialog::Show() before using WaitforClose() (it shows the Dialog Window automatically)
    /// </summary>
    /// <returns></returns>
    Status WaitforClose();

    /// <summary>
    /// Shows or hides the Dialog Window.  Use Show() to intially show the window, after which you can look at individual control messages or StatusChanged() to 
    /// handle events, either in the GetEvent() loop or Window Message Handler.
    /// <para></para>&#160;&#160;&#160;
    /// You can use WaitforClose() to wait for the user to close the window, then retrieve control values, without entering the event loop (i.e. GetEvent() or handling control events.
    /// <para></para>&#160;&#160;&#160;
    /// Using this method allows events to be handled as controls are used, where WaitforClose() allows the dialog box to be used as a modal dialog box that will wait for closure.
    /// </summary>
    /// <param name="bShow"></param>
    /// <returns></returns>
    bool Show(bool bShow = true); 

    /// <summary>
    /// Returns the CWindow object for the dialog window. 
    /// </summary>
    CWindow & GetWindow();
   
    /// <summary>
    /// Returns current status, which may be one of the following values:
    /// <para></para>&#160;&#160;&#160;
    /// Status::Ok -- The Ok Button was pressed.
    /// <para></para>&#160;&#160;&#160;
    /// Status::Canceled -- The Cancel Button was pressed.
    /// <para></para>&#160;&#160;&#160;
    /// Status::Unknown -- Ok Button or Cancel button has not been pressed yet.
    /// <para></para>
    /// --> Use ResetStatus() to reset the status to Status::Unknown
    /// <para></para>
    /// --> Use StatusChanged() to determine if a button has been pressed (an event is signaled to the window when this happens)
    /// </summary>
    /// <returns></returns>
    Status GetStatus(); 

    /// <summary>
    /// Returns an empty dialog.  Used for internal use, which can return an empty dialog if an error occurs, so dialog functions can be used passively in an error condition
    /// <para></para>
    /// --> Use CQuickDialog::isEmpty() to determine if the Dialog object is valid.
    /// </summary>
    /// <returns>TRUE when the dialog object is an empty/invalid object.</returns>
    static CQuickDialog * GetEmptyDialog();

    /// <summary>
    /// Used Internally.  Do not use.  Use CWindow::QuickDialog() or Sagebox::QuickDialog() to create a Quick Dialog Window
    /// </summary>
    static CQuickDialog * CreateNew(CWindow * cWin,const char * sHeader,const kwOpt & keywords);

    /// <summary>
    /// Returns true of the OK or CANCEL buttons has been pressed.  This is an event: GetEvent() returns when the status changed, and a 
    /// SageEvent is sent to the Window Message Handler of the Dialog's parent window.
    /// <para></para>
    /// --> You can provide a status variable to fill the Status that can be checked when StatusChanged() returns true.  Otherwise, use GetStatus() to retrieve the current status signaled
    /// by the StatusChanged flag when StatusChanged() returns true.
    /// </summary>
    /// <param name="(optional) eStatus"> = Status variable to fill with the Status</param>
    /// <param name="(optional) peek"> = Peek:Yes to return the StatusChanged flag without changing it.  Default is to clear the StatusChanged flag (but not the status itself).</param>
    /// <returns></returns>
    bool StatusChanged(Peek peek = Peek::No);

    /// <summary>
    /// Returns true of the OK or CANCEL buttons has been pressed.  This is an event: GetEvent() returns when the status changed, and a 
    /// SageEvent is sent to the Window Message Handler of the Dialog's parent window.
    /// <para></para>
    /// --> You can provide a status variable to fill the Status that can be checked when StatusChanged() returns true.  Otherwise, use GetStatus() to retrieve the current status signaled
    /// by the StatusChanged flag when StatusChanged() returns true.
    /// </summary>
    /// <param name="(optional) eStatus"> = Status variable to fill with the Status</param>
    /// <param name="(optional) peek"> = Peek:Yes to return the StatusChanged flag without changing it.  Default is to clear the StatusChanged flag (but not the status itself).</param>
    /// <returns></returns>
    bool StatusChanged(Status & eStatus,Peek peek = Peek::No);

    /// <summary>
    /// Returns TRUE is the dialog box is empty/invalid. 
    /// </summary>
    /// <returns></returns>
    bool isEmpty();

    /// <summary>
    /// Disable (or Enable) the automatic closing of the window on Ok, Cancel, or X button.
    /// <para></para>
    /// "NoClose()" can be specified on the initial call, such as cWindow->QuickDialog("+Title\Header",NoClose()); 
    /// </summary>
    /// <param name="bDisableClose"></param>
    /// <returns></returns>
    bool DisableClose(bool bDisableClose = true);

    /// <summary>
    /// Resets Status to Unknown. This can be used when an Ok or Cancel button is pressed (or window is closed by the user) and subsequently rejected. This allows the window
    /// to remain open while resetting the status so the user may try again.  The next Status change event will have a fresh/new status reflecting the retry with the dialog window.
    /// <para></para>&#160;&#160;&#160;
    /// Subsequent GetStatus() calls will return Status::Unknown until the user pressed Ok or Cancel, or the window is closed (which returns Status::Canceled)
    /// </summary>
    /// <returns></returns>
    bool ResetStatus();

    /// <summary>
    /// Sets the dialog box as Modal, disabling and freezing any events (controls, window close, etc.) until the dialog box is closed.
    /// <para></para>
    /// This can be used to prevent user interaction on the parent window until entry on the dialog box is validated and accepted.
    /// <para></para>
    /// --> Modal() can be placed on the dialog initialization, i.e. MyDialog = NewDialog("Header",Modal()); 
    /// </summary>
    /// <param name="bModal"> = 'true' when the dialog box should disable parent window; 'false' when parent window can be used when the dialog is displaying.</param>
    /// <returns></returns>
    bool SetModal(bool bModal = true);

    /// <summary>
    /// Sets the title of the dialog window in the title bar.   Otherwise a default title is used.
    /// <para></para>The title can also be set in the following ways
    /// <para></para>--> Use the Title() option on the initial QuickDialog() call
    /// <para></para>--> Set the first string in the Header as "+", (i.e. "+This is the title");
    /// <para></para>--> Use AddHeader() with "+" as the beginning of the first string.
    /// </summary>
    /// <param name="sTitle">Title of window</param>
    /// <returns></returns>
    bool SetWindowTitle(const char * sTitle);


    /// <summary>
    /// Sets Window location to place window when showing it for the first time.
    /// <para></para>
    /// By default, the window is centered in the parent window's client area, or centered on the desktop
    /// when opt::Center() is specified. 
    /// <para></para>
    /// --> You can specify a POINT or X,Y values, i.e. SetWinLocation(x,y) or SetWinLocation(MyPoint)
    /// </summary>
    /// <param name="pLoc"></param>
    bool SetWinLocation(POINT pLoc);
    
    /// <summary>
    /// Sets Window location to place window when showing it for the first time.
    /// <para></para>
    /// By default, the window is centered in the parent window's client area, or centered on the desktop
    /// when opt::Center() is specified. 
    /// <para></para>
    /// --> You can specify a POINT or X,Y values, i.e. SetWinLocation(x,y) or SetWinLocation(MyPoint)
    /// </summary>
    /// <param name="iX">X Position of Dialog Window</param>
    /// <param name="iY">X Position of Dialog Window</param>
    bool SetWinLocation(int iX,int iY);

    /// <summary>
    /// Returns true if the dialog box was cancelled by the user.  This is the equivalent of checking whether GetStatus() returns a cancelled status.
    /// </summary>
    /// <returns>TRUE if the user cancelled the dialog, false if the OK button was pressed.</returns>
    bool WasCanceled();
    ~CQuickDialog();

};

using QuickDialog = CQuickDialog;

}; // namespace Sage

#endif // _CQuickDialog_h_