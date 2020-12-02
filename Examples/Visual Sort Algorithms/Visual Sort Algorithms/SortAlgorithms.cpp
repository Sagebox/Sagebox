
#include "SortAlgorithms.h"

// Swap -- this is used by many sorting functions, and is a #define for a few reasons. 
//
// 1. The best approach is as an inline lamdba function local to each function that uses it
// 2. However, Microsoft does not yet support inline lambdas, which means that each time it is used it is a call
//    that uses up processor time and dumps the processor cache.
// 3. We want this to be as fast as possible and don't want it to be a call, since this is one of the main
//    elements used in many sort functions.  Making it a call would slow down the sort function.
// 4. An alternative is to make this an __forceinline function in the class structure for each sort function
//    I don't like this (over the inline lambda not supported in MSVC) because then I have to add more structure to each
//    sorting class when the classes themselves are only meant to be a container for the pure sorting function, which are only
//    in a class to support the Update() function for monitoring and GUI functions (i.e. the bar-graph update)
//
//    I don't want to make the classes complex and just want the sort routines themselves and the Update() function.
//
//  5. So, we're left with a #define because MSVC does not yet support inline lambdas 
//

// A utility function to swap two elements 
//
#define swap(_x,_y)                         \
{                                           \
    int t = arr[_x];                        \
    arr[_x] = arr[_y];                      \
    arr[_y] = t;                            \
    Update.func(_x,_y);                     \
}  

// --------------------------------------------------
// All Functions copied directly from original source
// --------------------------------------------------

 int CShellSort::ShellSort(int arr[], int n) 
{ 
    // Start with a big gap, then reduce the gap 

    for (int gap = n/2; gap > 0; gap /= 2) 
    { 
        // Do a gapped insertion sort for this gap size. 
        // The first gap elements a[0..gap-1] are already in gapped order 
        // keep adding one more element until the entire array is 
        // gap sorted  
        for (int i = gap; i < n; i += 1) 
        { 
            // add a[i] to the elements that have been gap sorted 
            // save a[i] in temp and make a hole at position i 
            int temp = arr[i]; 
  
            // shift earlier gap-sorted elements up until the correct  
            // location for a[i] is found 
            int j;             
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) 
            {
                arr[j] = arr[j - gap]; 
            }
              
            //  put temp (the original a[i]) in its correct location 
            arr[j] = temp; 
            Update.func(i,j);
        } 
    } 
    return 0; 
} 

void CHeapSort::heapify(int arr[], int n, int i) 
{ 
    int largest = i; // Initialize largest as root 
    int l = 2*i + 1; // left = 2*i + 1 
    int r = 2*i + 2; // right = 2*i + 2 
  
    // If left child is larger than root 
    if (l < n && arr[l] > arr[largest])  
        largest = l; 
  
    // If right child is larger than largest so far 
    if (r < n && arr[r] > arr[largest]) 
        largest = r; 
  
    // If largest is not root 
    if (largest != i) 
    { 
        swap(i, largest); 
  
        // Recursively heapify the affected sub-tree 
        heapify(arr, n, largest); 
    } 
} 
  
// main function to do heap sort 
//
void CHeapSort::HeapSort(int arr[], int n) 
{ 
    // Build heap (rearrange array) 
    for (int i = n / 2 - 1; i >= 0; i--) 
        heapify(arr, n, i); 
  
    // One by one extract an element from heap 
    for (int i=n-1; i>0; i--) 
    { 
        // Move current root to end 
        swap(0, i); 
  
        // call max heapify on the reduced heap 
        heapify(arr, i, 0); 
    } 
} 


void CSelectionSort::SelectionSort(int arr[], int n) 
{ 
    int i, j, min_idx; 
  
    // One by one move boundary of unsorted subarray 
    for (i = 0; i < n-1; i++) 
    { 
        // Find the minimum element in unsorted array 
        min_idx = i; 
        for (j = i+1; j < n; j++) 
          if (arr[j] < arr[min_idx]) 
          {
              min_idx = j;
              Update.func(min_idx,i);
          }
  
        // Swap the found minimum element with the first element 
        swap(min_idx, i); 
    } 
} 


void CMergeSort::merge(int arr[], int l, int m, int r) 
{ 
    int i, j, k; 
    int n1 = m - l + 1; 
    int n2 = r - m; 
  
    /* create temp arrays */

    // Change the original because Microsoft (and, really, canonical C++, I think...) doesn't support variable sized arrays on the stack, although it really should. 
    // Obviously this was originally written with gcc or some other compiler that supports it. 
    //
    // I am using the SageBox Mem<> template to make it easy, though I'd really rather use a canonical C/C++ function.
    // It's ok, though, because it isn't really a good idea for the stack anyway, since larger arrays could crash the
    // program -- for our purposes it is fine, but not for a robust function).
    
    // To make life easy, I just re-adjust the size of the arrays with the ResizeMax() function.  This allows
    // the merge function to be used more effectively since it will only reallocate the memory if the memory requirement is larger. 
    // So, in our case, the allocation will only happen once since we don't ever give it anything larger to work with. 

    // note: to change the code to canonical C++ would mean to either just reallocate the memory
    // or, in the case of what we do with Mem<>, reallocate only if the current size is less than the new size needed.
    //
    // also note: it is better to have these allocated beforehand for the greatest possible size, as reallocating each time
    // is very inefficient, and the original code is going to crash the program with larger arrays.

 //   int L[n1], R[n2];            --> The original code not supported by MSVC/C++
  
    Lm.ResizeMax(n1);            // i.e. int L[n1], except it only reallocates if necessary (i.e. n1 > current size)
    Rm.ResizeMax(n2);            // i.e. int R[n2]

    int * L = Lm;    // Get the addresses of the Mem<> arrays -- we don't want to use them directly because
    int * R = Rm;    // each access would be through a pointer (same with std::Vector, for example)

    // --- Original function follows (except for insertio of Update.func() ---
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++) 
        L[i] = arr[l + i]; 
    for (j = 0; j < n2; j++) 
        R[j] = arr[m + 1 + j]; 
  
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray 
    j = 0; // Initial index of second subarray 
    k = l; // Initial index of merged subarray 
    while (i < n1 && j < n2) { 
        if (L[i] <= R[j]) { 
            arr[k] = L[i]; 
            i++; 
        } 
        else { 
            arr[k] = R[j]; 
            j++; 
        } 
        k++; 
      Update.func(l,r);
  } 
  
    /* Copy the remaining elements of L[], if there are any */
    while (i < n1) { 
        arr[k] = L[i]; 
        i++; 
        k++; 
      Update.func(l,r);
    } 
  
    /* Copy the remaining elements of R[], if there are any */
    while (j < n2) { 
        arr[k] = R[j]; 
        j++; 
        k++; 
      Update.func(l,r);
   } 
} 

/* l is for left index and r is right index of the 
   sub-array of arr to be sorted */
void CMergeSort::MergeSort(int arr[], int l, int r) 
{ 
    if (l < r) { 
        // Same as (l+r)/2, but avoids overflow for 
        // large l and h 
        int m = l + (r - l) / 2; 
  
        // Sort first and second halves 
        MergeSort(arr, l, m); 
        MergeSort(arr, m + 1, r); 
  
        merge(arr, l, m, r); 
    } 
} 

  
/* This function takes last element as pivot, places  
the pivot element at its correct position in sorted  
array, and places all smaller (smaller than pivot)  
to left of pivot and all greater elements to right   
of pivot */
int CQuickSort::partition (int arr[], int low, int high)  
{  
    int pivot = arr[high]; // pivot  
    int i = (low - 1); // Index of smaller element  
  
    for (int j = low; j <= high - 1; j++)  
    {  
        // If current element is smaller than the pivot  
        if (arr[j] < pivot)  
        {  
            i++; // increment index of smaller element  
            swap(i,j);  
      }  
    }  
    swap(i + 1, high);  
     return (i + 1);  
}  
  
/* The main function that implements QuickSort  
arr[] --> Array to be sorted,  
low --> Starting index,  
high --> Ending index */
void CQuickSort::QuickSort(int arr[], int low, int high)  
{  
    if (low < high)  
    {  
        /* pi is partitioning index, arr[p] is now  
        at right place */
        int pi = partition(arr, low, high);  
  
        // Separately sort elements before  
        // partition and after partition  
        QuickSort(arr, low, pi - 1);  
        QuickSort(arr, pi + 1, high);  
    }  
}  