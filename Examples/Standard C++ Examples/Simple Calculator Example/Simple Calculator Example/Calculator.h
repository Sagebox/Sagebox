// Calculator.h -- Copyright(c) 2020, 2021 Rob Nelson.  robnelsonxx2@gmail.com -- all rights reserved.
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
// See notes in Calculator.cpp for more information
//

#pragma once
#if !defined(_Calculator_H_)
#define _Calculator_H_

#include "SageBox.h"

// Main Calculator Class
//
class Calculator
{
private:

    // Values for button groups, i.e. numbers, operations, memory, etc.
    //
    enum class ButtonGroups
    {
        Numbers = 100,      // 100 is arbitrary -- as long as it is > 0
        Ops,
        Ops2,
        Mem,
    };

    // Operations for individual buttons
    //
    enum class Ops
    {
        n0,n1,n2,n3,n4,n5,n6,n7,n8,n9,                              // Numbers 0-9
        Neg, Dot, Div, Mul, Sub, Add,                               // Op Group 1
        Sin, Cos, Tan, XpowY, PI, Inverse, SquareX, SqrtX, Log,     // Op Group 2
        MS, MR, MC, C, Equal,                                       // Mem Group
        NoOp,                                                       // Not defined / no op set
    };

    enum class MenuItem
    {
        Exit    = 100,
        About,
    };
private:
    CWindow        * m_cWin     = nullptr;      // Window passed in to place the calculator
    CEditBox    * m_cEditBox    = nullptr;      // Edit Box for number entry

    static constexpr POINT      kWinLoc         = { 20,62 };            // Location of main button grid in window
    static constexpr SIZE       kButtonSize     = { 70, 60 };           // Button Size for each calculator button
    CPoint                      kSpace          = { 1,1 };              // Spacing between buttons
    CPoint                      kButtonSpacing  = kSpace + kButtonSize; // Button Size + Spacing
    static constexpr RGBColor_t kColorNum{0,0,0};                       // Color for number buttons
    static constexpr RGBColor_t kColorOp{60,100,255};                   // Color for operations buttons
    static constexpr RGBColor_t kColorMem{0,0,192};                     // Color for memory buttons
    
    // Labels for buttons in each group.  In the button groups, the buttons are numbered in order horizontally,
    // so a 3-2 matrix would be buttons numbered 0,1,2 on the first row and 3,4,5 on the second, and so-forth
    // note the nullptr -- this leaves the button uncreated, but keeps the space empty, allowing multiple button sets
    // to overlap the same area (i.e. holes in the button grid).

    const char      * sButtonNumbers[12]    = { "7","8","9","4","5","6","1","2","3","±","0","." };                      // Numbers and +/-0 and .
    const char      * sButtonOps[4]         = { "÷","X","-","+" };                                                      // One row of functions
    const wchar_t   * sButtonOps2[10]       = { L"sin",L"cos",L"tan",L"x^y",L"π",L"1/x",L"x²",L"√x",L"log",nullptr };   // Two rows of functions (note: wchar_t due to sqrt)
    const char      * sButtonMem[5]         = { "ms","mr","mc","C", "="};                                               // Memory Store, Recall, Clear, plus Clear Value and "="

    CString m_csValue;                      // Main value
    CString m_csOperand;                    // First operand on two value operations (i.e. mul, sub, x^y, etc.)
    bool    m_bPureString   = true;         // When showing '0' but there has been no input
    bool    m_bDotExists    = false;        // Has the user entered '.' yet
    bool    m_bNegate       = false;        // Is the value negative
    bool    m_bValueError   = false;        // Is there a current error (which displays an error message)
    bool    m_bOperand      = false;        // Is there a two-value operation occurring
    double  m_fMemory       = 0;            // Memory value (i.e. MR, MS, MC)
    Ops     m_opOperand     = Ops::NoOp;    // The operation found when a button was pressed.

    void PrintValue();                      // Print the current value in the edit box
    void AddDigit(int iValue);              // Add a digit to the current value
    void AddDot();                          // Add a floating-point '.' to the value
    
    double GetValue();                                  // Get the current value in floating-point form (i.e. double)
    void ClearValue(bool bClear = true);                // Clear/Reset the current value
    void NewValue(double fValue,bool bError = false);   // Set a new value, replacing the old one
    void SetOperand(Ops op);                            // Set the current operation in-progress
    bool InitWindow();                                  // Set up Window, controls, buttons, etc.
    void Execute();                                     // Execute a function in a two-value operation (i.e. X * Y, etc.)
public:
    Calculator() {}
    void Run();                                         // Run the calculator, returning when the user closes the window.
};


#endif // _Calculator_H_

