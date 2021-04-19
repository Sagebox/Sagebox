#pragma once

#if !defined(_SortAlgorithms_h_)
#define _SortAlgorithms_h_

// note: CMemClass is included here due to the Mem<> template uses.  These are used a simple way to effectively allocate managed 
// memory that will be deleted automatically when the class is deleted.  Mem also has other management functions (such as ResizeMax())
// which is used in the Merge function.
//
// Only the Merge sort uses it, and if Mem is replaced with standard C/C++ memory allocation, then CMemClas.H can be removed and this
// set of functions is completely transportable without any other sources other than this .H file and the .cpp file.

#include "CMemClass.h"        

// UpdateFunc() 
//
// This structure allows an external call to be made to an update routine without the
// sort algorithm neededing any details.
//
// If the udpate algorithm is not provided (or is nullptr), the sort function acts as normal with
// no real consequence to efficiency to the algorithm -- that is, all of the sort algorithms can be
// used as general sort algorithms, as there is no time penalty when not providing an Update function.
//
struct UpdateFunc
{
    void * pData;
    void (*fUpdate)(void * pObj,int a,int b);
    __forceinline void func(int a,int b) { if (fUpdate) fUpdate(pData,a,b); };
};

// --------------------------------
// About the Various Sort functions
// --------------------------------
//
// All of these functions were copied from CodeProject and are as-is (including original comments)
// In the code. 
// 
// The classes used for two purposes:
//
//    1. Keep a clean interface
//    2. Allow the functions to be used with their original, copied source but with an 
//       option Update() function that allows the CVisualSort function (or anything else) to 
//       display the results realtime.
//
//     Without the Update() function, these routines can be used as normal sort functions. 
//
// --------------------------------------
// Copying Code and Using it with SageBox
// --------------------------------------
//
// As mentioned, the code for all sorting algorithms are in the exact same form and copied directly from 
// Code Project.  This is to show how code can be used with Sagebox as-is, with very little needed to 
// get the code working in a GUI format. 
//
// In this case, little insertions for the Update() function in the right points for an update
// display function is all that was needed, without touching any of the original code.
//
// There are two places where the code differs from the original:
//
//    1. In MergeSort(), a memory object is used for temporary arrays.  This is because MSVC doesn't support
//       variable-sized arrays on the stack, but also -- and possibly more importantly -- putting these arrays on the 
//       stack in merge() can possily cause a crash if they exceed the size of the stack.
//
//     In this case, the memory objects are re-allocated only if the size needed is bigger.
//       This does not happen with CVisualSort, as they are pre-allocated to a size that is larger than
//     the size needed.  Therefore, the memory arrays are only allocated once.
//
//    2. Swap() -- there are many sort functions that call swap(), which should be a separate function
//       local to each class.  However, since MSVC doesn't support inline lambdas, this was changed to a #define
//       for the sake of efficiency.  Read the notes above the swap() #define.
//

// ----------------
// Merge Sort Class
// ----------------
//
class CMergeSort
{
    UpdateFunc Update = {};
    void merge(int arr[], int l, int m, int r); 
     Sage::Mem<int> Lm;    // Memory for merge() function. Changed due to variable-sized stack array in original version.
    Sage::Mem<int> Rm;    // Memory for merge() function

public:
    CMergeSort(UpdateFunc stUpdate) : Update(stUpdate) { Lm = 2000; Rm = 2000; };
    CMergeSort() { Lm = 2000; Rm = 2000; };        // Provide a version that does not require an Update function
    void MergeSort(int arr[], int l, int r);
};

// ----------------
// Quick Sort Class
// ----------------
//
class CQuickSort
{
    UpdateFunc Update = {};
    int partition (int arr[], int low, int high);
public:
    CQuickSort(UpdateFunc stUpdate) : Update(stUpdate) {};
    CQuickSort() { };    // Provide a version that does not require an Update function
    void QuickSort(int arr[], int l, int r);
};

// ---------------
// Heap Sort Class
// ---------------
//
class CHeapSort
{
    UpdateFunc Update = {};
    void heapify(int arr[], int n, int i);
public:
    CHeapSort(UpdateFunc stUpdate) : Update(stUpdate) {};
    CHeapSort() { };    // Provide a version that does not require an Update function
    void HeapSort(int arr[], int n);
};

// ----------------
// Shell Sort Class
// ----------------
//
class CShellSort
{
    UpdateFunc Update = {};
public:
    CShellSort(UpdateFunc stUpdate) : Update(stUpdate) {};
    CShellSort() { };    // Provide a version that does not require an Update function
    int ShellSort(int arr[], int n);
};

// --------------------
// Selection Sort Class
// --------------------
//
class CSelectionSort
{
    UpdateFunc Update = {};
public:
    CSelectionSort(UpdateFunc stUpdate) : Update(stUpdate) {};
    CSelectionSort() { };    // Provide a version that does not require an Update function
    void SelectionSort(int arr[], int n);
};

#endif // _SortAlgorithms_h_