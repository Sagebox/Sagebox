// CVisualSort.Cpp -- Copyright(c) 2020, 2021 Rob Nelson.  robnelsonxx2@gmail.com -- all rights reserved.
// This file, information, and process within are for personal use only and may not be distributed without permission.
// Please modify, copy, do whatever you want for personal uses.  For professional, distribution or commercial uses, 
// contact the e-mail address above

// *******************************************************
// CVisualSort.h -- SageBox Neural Network 7-Bit Counter
// *******************************************************
//
// See notes in CVisualSort.cpp for more information

#pragma once
#if !defined(_CVisualSort_h_)
#define _CVisualSort_h_

#include "CSageBox.h"
#include "SortAlgorithms.h"

class CVisualSort;

// ------------------
// CVisual Sort Class
// ------------------
//
class CVisualSort
{
    // A structure so we can easily add more sort routines later, if we want.

    struct SortType
    {
        const char * sTitle;                    // Name of the sort type
        void (CVisualSort::*fSortFunc)();       // Sort Function
    };

    static constexpr int kSortSize = 1000;      // The number of elements to sort.  This used to control the size of the window, as it auto-fits
                                                // to the size needed.  ~1800 is getting near full-sized on a 1920x1080 screen.
                                                // Smaller numbers create a smaller window.

    CWindow            * m_cWin             = nullptr;  // The main Window
    CWindow            * m_cGraphWin        = nullptr;  // The Graph Window embedded in the Main Window
    CTextWidget        * m_cText            = nullptr;  // This is where the title of the sort is displayed (on the bottom of the screen)
    CSlider            * m_cSliderThreshold = nullptr;  // Slider to change the display threshold (i.e. how often it updates the graph window).
                                                        // Higher numbers increase the speed of the results since the display updates less.
                                                        // Lower numbers slow down the sort, which shows the sort working more closely.
 
    Mem<int>           m_ipArray;                       // Randomized data memory (0-(kSortSize-1)), randomized
    Mem<bool>          m_ipFilled;                      // Memory to help randomize the data

    bool              m_bConsoleApp;                    // If we're a console or Windows app, we print out the wait message differently in 
                                                        // DrawGraph().
    // Sort function objects

    CMergeSort            m_MergeSort; 
    CQuickSort            m_QuickSort; 
    CSelectionSort        m_SelectionSort;
    CHeapSort             m_HeapSort;
    CShellSort            m_ShellSort;

    int                    m_iCounter;                  // Counter for the threshold (i.e. how many Update() calls before we print a graph)
    int                    m_iThreshold = 35;           // How many Update()/DrawGraph() calls before we draw a graph and update the window.

    static constexpr int kTriangleSize = 8;             // Size of the bottom triangle that shows LEFT and RIGHT indexes in the sort in real-time

    CPoint m_pTri[3] = { { 0, 0 }, { -kTriangleSize, kTriangleSize,}, { kTriangleSize, kTriangleSize} };        // Vertices of the triangle for drawing
    
    // SortUpdate() -- Update() call back for the sort functions -- this passes in our 'this' pointer so we can call DrawGraph();
    //
    static void SortUpdate(void * pData,int a,int b)
    {
        if (pData) ((CVisualSort *) pData)->DrawGraph(a,b);
    }

    // Menu Items

    enum class MenuItems
    {
        Exit        = 100,
        About
    };

    // Local Sort functions that call out to the various sort functions, allowing us to put them into an array. 
    // see Run() function

    void MergeSort()        { m_MergeSort.MergeSort(m_ipArray,0,kSortSize-1); }
    void QuickSort()        { m_QuickSort.QuickSort(m_ipArray,0,kSortSize-1); }
    void SelectionSort()    { m_SelectionSort.SelectionSort(m_ipArray,kSortSize); }
    void HeapSort()         { m_HeapSort.HeapSort(m_ipArray,kSortSize); }
    void ShellSort()        { m_ShellSort.ShellSort(m_ipArray,kSortSize); }

private:
    void InitDataArray();                    // Initialize the array with random data
    void DrawGraph(int a =0 ,int b = 0);    // DrawGraph with Left and Right for the triangle on the bottom
    void Go();                                // Run the sort program until someone closes the window
    bool RunSort(const SortType & stSort);    // Run an individual sort. 
    bool InitWindow(CSageBox & cSageBox);    // Initialize thw Window, Controls, etc.
    bool InitMem();                            // Allocate the initial memory.
public:
    bool main(bool bConsoleApp);                            // same as C++ main(), just here in our class.

    // Instantiate and allocate all Sorting classes with the Update Pointer and passed object pointer (i.e. this)
    //
    CVisualSort()    : m_MergeSort(     CMergeSort(     {this,SortUpdate})),
                      m_QuickSort(      CQuickSort(     {this,SortUpdate})),
                      m_HeapSort(       CHeapSort(      {this,SortUpdate})),
                      m_ShellSort(      CShellSort(     {this,SortUpdate})),
                      m_SelectionSort(  CSelectionSort( {this,SortUpdate}))
    {
    }
};

#endif    // _CVisualSort_h_
