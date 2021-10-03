// Calculator.Cpp -- Copyright(c) 2020, 2021 Rob Nelson.  robnelsonxx2@gmail.com -- all rights reserved.
// This file, information, and process within are for personal use only and may not be distributed without permission.
// Please modify, copy, do whatever you want for personal uses.  For professional, distribution or commercial uses, 
// contact the e-mail address above

// *******************************************************
// Calculator.Cpp -- SageBox Simple Calculator Application 
// *******************************************************
//
// Calculator is a simple SageBox program consisting of roughly 100 executable lines of C++ code.
// It provides simple calculator functions such as +,-,/,*, as well as more advanced ones such as 
// Sin, Cos, Log, etc.  It also features memory store, recall, and clear.
//
// ----------------------------
// Outlined Features of SageBox
// ----------------------------
//
// 1. Button Groups and Windows-Style buttons
//
//    Button groups can be used with regular buttons of any button style, as well as checkboxes and radio buttons.
//
//    In this program button groups are demonstrated with Windows-Style buttons (i.e. rectangular) to fit a calculator look,
//    although any button style can be used, including user-defined buttons.
//
//    Button groups are a great way to specify an NxM row/column of buttons in programs that will use a number of 
//    buttons that are generic.
//
//    As with many controls in SageBox, Button groups manage themselves, and you do not need to remember them -- 
//    Sagebox destroys them with the window.
//
//    Only a user-supplied Group-ID is needed to access any button in the group.
//
//    note: notice that one button group as a nullptr in the definition -- this allows a button space to be created without
//          a button creation so that button groups can overlap, as they do in this example.
//
// 2. Using Controls without saving Object Pointers or other information
//
//    As mentioned above, Button groups are defined as a group by a Group ID.  When used, only that number needs to be kept by the program to access button functionality.
//    The actual button object can be retrieved for specific operations. 
//
//    In this program, only the order-of-creation number is needed when asking SageBox is a member of the group has been pressed.
//
// 3. Edit Box functionality
//
//    There is an edit box used in the calculator, and many of the functions show how it is used easily with floating point values.
//
// 4. Background Textures and ReadPGrBitmap()
//
//    This reads a bitmap (either .BMP, .JPEG, or other format) embedded in a PGR (Portable Graphics Resource) file.  This is a quick
//    way to get an ensure bitmap (i.e. there can be no error once it has been verified in testing once) is retreived. 
//
//    There are many ways to retreive a bitmap or JPEG image in SageBox, and this is a nice way to store a collection and
//    other information about a specific program.
//
//    .PGR files may be used from the hard-drive or in-memory as in this program (i.e. for portability).
//

#include "Calculator.h"
#include "calctexture.pgr2.h"       // PGR (Graphics Resource) file containing the background texture.

#pragma warning(disable: 4996)      // Stop MSVC complaining about my use of strncat 

// PrintValue() -- Print the current value in the edit box
//
void Calculator::PrintValue()
{
    CString csTemp;
    const char * sOut = m_bValueError ? "Invalid Input" : "0";
    if (!m_bPureString && !m_bValueError) 
    {
        bool bZero = !atof(m_csValue);
        if (m_bNegate && !bZero) csTemp << "-";
        sOut = (csTemp << *m_csValue);
    }
    m_cEditBox->SetText(sOut);
};

// AddDigit() -- Add a digit to the current value in the edit box
//
void Calculator::AddDigit(int iValue)
{
    if (m_bPureString) **m_csValue = 0;

    m_bPureString = false;        // not an empty "0" string any longer
    strncat(m_csValue.c_str(),(char *) &iValue,1); 
    PrintValue();
};

// Add a floating-point . to the number
//
void Calculator::AddDot()
{
    if (!m_bDotExists) AddDigit('.');    
    m_bDotExists = true;            // Make sure only one is allowed
};
    
// Clear the value and all status values with it. 
//
void Calculator::ClearValue(bool bClear)
{
    m_bDotExists    = false;
    m_bPureString   = true; 
    m_bNegate       = false;
    m_bValueError   = false;
    m_bOperand      = false;

    m_csValue.ClearString();
    if (bClear) PrintValue();
};

// Return the value from the edit box. 
// 
// note: normally, this would be an inline function in the class header.
//       It here to keep the full source in the main source-code file, for 
//       context of using SageBox.
//
double Calculator::GetValue()
{ 
    return m_cEditBox->GetFloat(); 
};
    
// NewValue() -- Set a new value, usually the result of an operation, such as sqrt(), sin(), etc.
//
void Calculator::NewValue(double fValue,bool bError)
{
    m_csValue >> fabs(fValue);      // ">>" direction re-initializes, where "<<" adds to the string.
                                    // fabs() is used since we keep track of the '-' status in bNegate
    m_bNegate       = fValue < 0;
    m_bValueError   = bError;
    m_bPureString   = !fValue;
    PrintValue();
};

// SetOperand() -- Set the current operation in-progress in two-value operations, 
// such as X * Y, X/Y, x^y, etc. 
//
void Calculator::SetOperand(Ops op)
{
    m_bOperand = true; 
    m_csOperand >> GetValue();
    m_opOperand = op;
    ClearValue(false);        // false = don't reset the display to "0"
};
    
//InitWindow() -- Initialize controls used in the calculator
//
// There are only two types of controls: 
//
//    Buttons:  Used as imput for numbers and all math and memory functions. 
//    Edit Box: Used to display the value of the calculator.  It can also 
//              be set with entering the keyboard, but this can be shut off with ReadOnly status.
//
// note: There are many items that would usually be refactored out as constexpr or other functions.
// For example, the fonts "Arial,23", "Arial,20", the Style("Windows"), the X,Y locations, etc. 
// would all be constexpr values in the main header to avoid Magic Numbers.
//
// These are left in the source to show more in context how SageBox works. 
//
bool Calculator::InitWindow()
{
    // Get our backdrop.  We could also just use a cls() to clear the screen to a preferred color or backdrop, but
    // a textured background is nicer and just as easy to put in the window.

    auto cBitmap = m_cSageBox->ReadPgrBitmap("Texture",sCalcTexture);       // Grab the .JPG file from the .PGR file in memory.
                                                                            // We can also use file paths, as well as .JPG or .BMP
                                                                            // files or memory.  In this case, we can use the 
                                                                            // .PGR file as a container, perhaps using it in the future
                                                                            // to contain other files or declarations.
    
    // Note that since we grabbed the texture from a file in memory, we're guaranteed of its success, so we don't need to do any error-checking 
    // on whether or not it was loaded.  

    // However, it doesn't matter too much -- if it doesn't get loaded, the window size is 0 but there are no other errors.
    // Try removing the "ReadPgrBitmap" line -- it will run the program but not have a client area because the Width & Height
    // if the bitmap are both now 0. 

    m_cWin = &m_cSageBox->NewWindow(50,50,cBitmap.GetWidth(),cBitmap.GetHeight(),InnerSize() | Hidden());

    m_cWin->DisplayBitmap(cBitmap);                         // Now display the bitmap before we create the controls, so any 
                                                            // controls with transparencies can copy the background.

    // Initialize the Edit Box.  It is put just above the button grid, with the same width (i.e. 5*the button size)
    
    m_cEditBox = &m_cWin->NewEditBox(kWinLoc.x,kWinLoc.y-35,kButtonSpacing.x*5,33,Font("Arial,23") | bgColor(SageColor::White)
                                                                                                   | fgColor(SageColor::Black) | opt::ThickBorder() | opt::FloatsOnly());

    // Set generic options for all buttons. This sets a highlighted background color for each button (i.e. bgHigh()).  This is the same for 
    // all buttons to make things easy, but can be set to have a different background color for each button section with different colors, for 
    // better blending. 
    //
    // Style("Windows") sets panel-style buttons vs. default buttons
    // TextColor() sets the text color on the buttons, which usually defaults to black with Windows Style buttons

    cwfOpt cwOpt = opt::Style("Windows") | TextColor(SageColor::White) | bgHigh({40,40,255}) | Font("Arial,20");

    // Some lambda values to make life easer. 

    auto ButLoc = [&](int iX,int iY)    { return POINT{kWinLoc.x + iX*kButtonSpacing.x, kWinLoc.y + iY*kButtonSpacing.y}; };
    auto getOpt = [&](RGBColor_t color) { return cwfOpt() << cwOpt | bgColor(color); };

    // Create button groups with group IDs -- Numbers, Ops (mul, div, etc.), Ops2 (two rows of functions), and memory, clear, etc.
    // This creates 4 button grid areas, two of which overlap (one has a nullptr in the place of a button to keep the 
    // area open for another grid that uses it). 

    m_cWin->CreateButtonGroup((int) ButtonGroups::Numbers, 12, sButtonNumbers,  ButLoc(1,2), kButtonSize,3,kSpace,getOpt(kColorNum));
    m_cWin->CreateButtonGroup((int) ButtonGroups::Ops,     4,  sButtonOps,      ButLoc(0,2), kButtonSize,1,kSpace,getOpt(kColorOp));
    m_cWin->CreateButtonGroup((int) ButtonGroups::Mem,     5,  sButtonMem,      ButLoc(4,1), kButtonSize,1,kSpace,getOpt(kColorMem));
    m_cWin->CreateButtonGroup((int) ButtonGroups::Ops2,    10, sButtonOps2,     ButLoc(0,0), kButtonSize,5,kSpace,getOpt(kColorOp));

    m_cWin->Show();        // Show the window now that we've created all of the controls and initialized any display.
    return (m_cWin != nullptr);        // Just in case we didn't get our window created, return an error it is nullptr
}

// Execute() -- Execute a function in a 2-value operation, such as multiply, divide, etc.
//
void Calculator::Execute()
{
    double fValue1 = GetValue();
    double fValue2 = atof(m_csOperand);
    switch(m_opOperand)
    {
        case Ops::Add   :    NewValue(fValue2+fValue1);                             break;
        case Ops::Sub   :    NewValue(fValue2-fValue1);                             break;
        case Ops::Mul   :    NewValue(fValue1*fValue2);                             break;
        case Ops::Div   :    NewValue(fValue1 ? fValue2/fValue1 : 0, !fValue1);     break;
        case Ops::XpowY :    NewValue(pow(fValue2,fValue1));                        break;
    }
};

// Run() -- Run the main calculator
//
// note:  Some of the code here (such as the operation arrays and lambda functions) would normally be part of the 
// class structure or as separate functions -- for example the lambdas would probably just be one-liners in the 
// class header, or at least defined as separate functions.
//
// These are left in the main body of code to give more context to how the code works and how it works with 
// SageBox. 
//
void Calculator::Run()
{
    if (!InitWindow()) return;    // Create controls, etc.

    // Arrays defining operation enums corresponding to the buttons created (i.e. compare these to the button names sButtonNumbers,
    // sButtonOps, etc. in the class declaration) -- this allows us to get the operation for the button by just pulling from 
    // the array with the button ID we get from the query in the event loop.

    Ops stNumbers[] = { Ops::n7,  Ops::n8,  Ops::n9,  Ops::n4, Ops::n5, Ops::n6, Ops::n1, Ops::n2, Ops::n3, Ops::Neg, Ops::n0, Ops::Dot };
    Ops stOps[]     = { Ops::Div, Ops::Mul, Ops::Sub, Ops::Add };
    Ops stOps2[]    = { Ops::Sin, Ops::Cos, Ops::Tan, Ops::XpowY,  Ops::PI,    Ops::Inverse, Ops::SquareX, Ops::SqrtX, Ops::Log };
    Ops stMem[]     = { Ops::MS,  Ops::MR,  Ops::MC,  Ops::C,      Ops::Equal, Ops::NoOp };

    // Some Lambdas to clean up code in the switch() statement in the main event loop 

    auto Negate     = [&](double fValue) { NewValue(-fValue); };
    auto Sqrt       = [&](double fValue) { NewValue(fValue < 0 ? 0 : sqrt(fValue),fValue < 0); };
    auto Inverse    = [&](double fValue) { NewValue(fValue ? 1.0/fValue : 0, !fValue); };
    auto Sin        = [&](double fValue) { NewValue(sin(fValue)); };
    auto Cos        = [&](double fValue) { NewValue(cos(fValue)); };
    auto Tan        = [&](double fValue) { NewValue(tan(fValue)); };
    auto Log        = [&](double fValue) { NewValue(fValue <= 0 ? 0 : log(fValue),fValue <= 0); };
    auto Sq         = [&](double fValue) { NewValue(fValue*fValue); };

    PrintValue();    // Start with the initial "0" rather than a blank edit box.

    // The Main Loop() -- Loop while we're getting events.  GetEvent() returns false if the window is closed or destroyed, which can be 
    // checked with WindowClosing()
    //
    while (m_cWin->GetEvent())
    {
        int iButton;
        Ops eOp = Ops::NoOp; 

        // A Lambda to make getting button presses from the groups easier.
        // m_cWin->group.isPressed() returns true of a button was pressed in the group given, filling iButton with the 
        // ID of the button -- the same ID which can be checked in the array given as a parameter. 

        auto GetOp = [&](ButtonGroups group, Ops * sOp) { if (m_cWin->group.isPressed((int) group,iButton)) eOp = sOp[iButton]; };

        GetOp(ButtonGroups::Numbers,stNumbers);
        GetOp(ButtonGroups::Ops,stOps);
        GetOp(ButtonGroups::Ops2,stOps2);
        GetOp(ButtonGroups::Mem,stMem);

        // If the Operation is not Ops::NoOp, a button was pressed and we know which one.

        if (eOp != Ops::NoOp)
        {
            double fValue = GetValue();
            if ((int) eOp < 10) AddDigit((int) eOp + '0'); 
            else

            // Do a switch() for now, since it's a small program.
            //
            // If it were to grow larger, then we could look at making it an overload or array-based 
            // function call to reduce its footprint int the main event loop. 
            //
            // Or, we could set overloads on all the buttons to do it automatically, but, while its a canonical
            // approach, that it a lot of work, especially in a protoype/early-to-mid development stage.
            
            switch(eOp)
            {
                case Ops::C         : ClearValue();             break;
                case Ops::Dot       : AddDot();                 break;
                case Ops::Neg       : Negate(fValue);           break;
                case Ops::SqrtX     : Sqrt(fValue);             break;
                case Ops::Inverse   : Inverse(fValue);          break;
                case Ops::Sin       : Sin(fValue);              break;
                case Ops::Cos       : Cos(fValue);              break;
                case Ops::Tan       : Tan(fValue);              break;
                case Ops::Log       : Log(fValue);              break;
                case Ops::SquareX   : Sq(fValue);               break;
             
                case Ops::Equal     : Execute();                        break;
                case Ops::PI        : NewValue(3.1415926535897932);     break;
                case Ops::MR        : NewValue(m_fMemory);              break;
                case Ops::MS        : m_fMemory = GetValue();           break;
                case Ops::MC        : NewValue(m_fMemory = 0);          break;
                default:
                    SetOperand(eOp); break;     // Handle two-value functions (i.e. pow, Mul, Div, Add, Sin, etc.)
                                                // i.e. that's all that is left.
            }
        }
    }
}
