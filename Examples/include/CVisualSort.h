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
#if !defined(_CVisualSort_h_)
#define _CVisualSort_h_

#ifdef _LOCALTEST

#include "CSageBox.h"

class CVisualSort;


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
//		1. Keep a clean interface
//		2. Allow the functions to be used with their original, copied source but with an 
//		   option Update() function that allows the CVisualSort function (or anything else) to 
//		   display the results realtime.
//
//         Without the Update() function, these routines can be used as normal sort functions. 
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
//		1. In MergeSort(), a memory object is used for temporary arrays.  This is because MSVC doesn't support
//		   variable-sized arrays on the stack, but also -- and possibly more importantly -- putting these arrays on the 
//		   stack in merge() can possily cause a crash if they exceed the size of the stack.
//
//         In this case, the memory objects are re-allocated only if the size needed is bigger.
//		   This does not happen with CVisualSort, as they are pre-allocated to a size that is larger than
//         the size needed.  Therefore, the memory arrays are only allocated once.
//
//		2. Swap() -- there are many sort functions that call swap(), which should be a separate function
//		   local to each class.  However, since MSVC doesn't support inline lambdas, this was changed to a #define
//		   for the sake of efficiency.  Read the notes above the swap() #define.
//

// ----------------
// Merge Sort Class
// ----------------
//
class CMergeSort
{
	UpdateFunc Update = {};
	void merge(int arr[], int l, int m, int r); 
 	Mem<int> Lm;	// Memory for merge() function. Changed due to variable-sized stack array in original version.
	Mem<int> Rm;	// Memory for merge() function

public:
	CMergeSort(UpdateFunc stUpdate) : Update(stUpdate) { Lm = 2000; Rm = 2000; };
	CMergeSort() { Lm = 2000; Rm = 2000; };		// Provide a version that does not require an Update function
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
	CQuickSort() { };	// Provide a version that does not require an Update function
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
	CHeapSort() { };	// Provide a version that does not require an Update function
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
	CShellSort() { };	// Provide a version that does not require an Update function
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
	CSelectionSort() { };	// Provide a version that does not require an Update function
	void SelectionSort(int arr[], int n);
};



// ------------------
// CVisual Sort Class
// ------------------
//
class CVisualSort
{
	// A structure so we can easily add more sort routines later, if we want.

	struct SortType
	{
		const char * sTitle;					// Name of the sort type
		void (CVisualSort::*fSortFunc)();		// Sort Function
	};

	static constexpr int kSortSize = 1000;		// The number of elements to sort.  This used to control the size of the window, as it auto-fits
												// to the size needed.  ~1800 is getting near full-sized on a 1920x1080 screen.
												// Smaller numbers create a smaller window.

	CWindow			* m_cWin				= nullptr;		// The main Window
	CWindow			* m_cGraphWin			= nullptr;		// The Graph Window embedded in the Main Window
	CTextWidget		* m_cText				= nullptr;		// This is where the title of the sort is displayed (on the bottom of the screen)
	CSlider			* m_cSliderThreshold	= nullptr;		// Slider to change the display threshold (i.e. how often it updates the graph window).
															// Higher numbers increase the speed of the results since the display updates less.
															// Lower numbers slow down the sort, which shows the sort working more closely.
 
	Mem<int>		  m_ipArray;							// Randomized data memory (0-(kSortSize-1)), randomized
 	Mem<bool>		  m_ipFilled;							// Memory to help randomize the data

	// Sort function objects

	CMergeSort			m_MergeSort; 
	CQuickSort			m_QuickSort; 
	CSelectionSort		m_SelectionSort;
	CHeapSort			m_HeapSort;
	CShellSort			m_ShellSort;

	int					m_iCounter;						// Counter for the threshold (i.e. how many Update() calls before we print a graph)
	int					m_iThreshold = 35;				// How many Update()/DrawGraph() calls before we draw a graph and update the window.

	static constexpr int kTriangleSize = 8;				// Size of the bottom triangle that shows LEFT and RIGHT indexes in the sort in real-time

	CPoint m_pTri[3] = { { 0, 0 }, { -kTriangleSize, kTriangleSize,}, { kTriangleSize, kTriangleSize} };		// Vertices of the triangle for drawing
	
	// SortUpdate() -- Update() call back for the sort functions -- this passes in our 'this' pointer so we can call DrawGraph();
	//
	static void SortUpdate(void * pData,int a,int b)
	{
		if (pData) ((CVisualSort *) pData)->DrawGraph(a,b);
	}
	// Local Sort functions that call out to the various sort functions, allowing us to put them into an array. 
	// see Run() function

	void MergeSort()		{ m_MergeSort.MergeSort(m_ipArray,0,kSortSize-1); }
	void QuickSort()		{ m_QuickSort.QuickSort(m_ipArray,0,kSortSize-1); }
	void SelectionSort()	{ m_SelectionSort.SelectionSort(m_ipArray,kSortSize); }
	void HeapSort()			{ m_HeapSort.HeapSort(m_ipArray,kSortSize); }
	void ShellSort()		{ m_ShellSort.ShellSort(m_ipArray,kSortSize); }

private:
	void InitDataArray();					// Initialize the array with random data
	void DrawGraph(int a =0 ,int b = 0);	// DrawGraph with Left and Right for the triangle on the bottom
	void Go();								// Run the sort program until someone closes the window
	bool RunSort(const SortType & stSort);	// Run an individual sort. 
	bool InitWindow(CSageBox & cSageBox);	// Initialize thw Window, Controls, etc.
	bool InitMem();							// Allocate the initial memory.
public:
	bool main();							// same as C++ main(), just here in our class.

	// Instantiate and allocate all Sorting classes with the Update Pointer and passed object pointer (i.e. this)
	//
	CVisualSort()	: m_MergeSort(		CMergeSort(		{this,SortUpdate})),
					  m_QuickSort(		CQuickSort(		{this,SortUpdate})),
					  m_HeapSort(		CHeapSort(		{this,SortUpdate})),
					  m_ShellSort(		CShellSort(		{this,SortUpdate})),
					  m_SelectionSort(	CSelectionSort(	{this,SortUpdate}))
	{
	}
	
};

#endif	// _CVisualSort_h_
#endif //  _LOCALTEST