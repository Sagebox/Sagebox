// This file copyright(c) 2021 Rob Nelson, All Rights Reserved.    E-mail rob@projectsagebox.com for more information.
//

// Mem and MemA classes work as a low-level std::vector aimed at more efficient CPU-usage 
// and flexibility. 
//
// MemA is used to align memory on a 128-byte boundary for using SSE, and other instructions that require alignment.
// MemA alignment is 128 bytes regardless of the data type, allowing arrays of unsigned char, int, etc to be alligned
// on a 128-byte boundary more easily. 

// While exceptions and boundary safety are provided with [] usage (i.e. MyMemory[iIndex]), the Mem and MemA classes
// are meant to be used as containers, with the main usage as pointers directly to memory.
//
// for example,
//
//      Mem<int> MyMemObj(iLengthNeeded);
//      int * MyMemPtr = MyMemObj;  // also expressed as auto MyMemPtr = &Mem[0];
//
// This code creates the memory container in MyMemObj, but then assigns the memory to an int * pointer for much more effcient use than
// using Mem or std::vector can natively provide. 
//
// As with std::vector any memory allocated is de-allocated on exit. 
//
// Mem and MemA are only for data types, not classes or structures with destructors. 
// You can use Sage::Obj class for this (which constructs one object only, but deletes it automatically for you), or vector to use an array of class objects. 
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
#include <cstdlib>
#include <cstdio>
#include <memory>
#include <malloc.h>
#include "Sage.h"
#include "ErrCtl.h"
#include "CString.h"
#include <stdexcept>

//#pragma once
#if !defined(_CMemClass_H_)
#define _CMemClass_H_

namespace Sage
{
    class MemTools
    {
    public:
        static CString ShowExceptMsg(const char * sTitle, const char * sMsg,const char * sFile,unsigned int sLine);
    };
template <class _t>
	class Mem
	{

	private:
	Mem & operator = (const Mem & p2)
	{
        if (this != &p2)
        {
		    if (pMem) free(pMem);
		    memcpy(this,&p2,sizeof(*this));
		    Mem * pMem = (Mem *) &p2;
		    pMem->pMem = nullptr;
		    pMem->iSize = 0;
        }
		return *this;
	}
		Mem(const Mem &p2)
		{
			if (&p2 == this)
			{
				int gg = 1;
			}
			memcpy(this,&p2,sizeof(*this));
			Mem * pMem = (Mem *) &p2;
			pMem->pMem = nullptr;
			pMem->iSize = 0;
		}
	public:

		int iSize = 0;
		_t * pMem = nullptr;
        _t * GetMem() { return pMem; }

    Mem(Mem && p2) noexcept
    {
        iSize = p2.iSize;
        pMem = p2.pMem;

        p2.iSize = 0;
        p2.pMem = nullptr;
    }
    Mem & operator=(Mem && p2) noexcept
    {
        if (this != &p2)
        {
            iSize = p2.iSize;
            pMem = p2.pMem ;

            p2.iSize = 0;
            p2.pMem = nullptr;
        }
        return *this;
    }

		bool SetData(_t * pInMem,int iFileSize)
		{
			if (!pInMem || iFileSize <= 0) return false;			
			if (pMem) free(pMem);
			pMem = pInMem;
			iSize = iFileSize;
			return true;
		}
		void DeleteData()
		{
			if (pMem) free(pMem);
			pMem = nullptr;
			iSize = 0;
		}
		Mem(_t * pMem,int iFileSize)
		{
			SetData(pMem,iFileSize);
		}
		
		Mem & moveFrom(const Mem & p2)
		{
			*this = p2;
			return *this;
		}
		Mem(int iSize)
		{
			this->iSize = iSize;
			if (iSize) pMem = (_t *) std::malloc(iSize*sizeof(_t));;
		}
		Mem()
		{
			iSize = 0;
			pMem = nullptr;
		}
		~Mem()
		{
			if (pMem) free(pMem);
			pMem = nullptr;
			iSize = 0;
		}
		// GetNumItems() -- Returns the number of ellements allocated.  Use GetMemSize() for total memory size
		//
		__forceinline int GetNumitems() { return  pMem ? iSize : 0; }

		// GetMemSize() -- Returns the total memory size allocated for this memory object (i.e. number of elements X sizeof(element_type)_
		// Use GetNumItems() for number of elements, or use variable "iSize" directly.
		//
		__forceinline long GetMemSize() { return pMem ? iSize*sizeof(_t) : 0; }

		// Clear all allocate memory of array.  If a value is specified, the memory is filled with this value (unsigned char of 0-255)
		//
		void ClearMem()
		{
			if (pMem) std::memset(pMem,0,iSize*sizeof(_t));
		}

		// Clear all allocate memory of array.  If a value is specified, the memory is filled with this value (unsigned char of 0-255)
		//
		void ClearMem(unsigned char ucValue)
		{
			if (pMem) std::memset(pMem,ucValue,iSize*sizeof(_t));
		}
        /// <summary>
        /// Fill memory with a specified value corresponding to the Mem type (i.e. int, double, etc. 
        /// This differs from ClearMem() which fills the memory explictitly with zeros or the unsigned char value specified
        /// (which is faster than using Fill).
        /// <para></para>&#160;&#160;&#160;
        /// Fill() fills the entire array with the same value (or copy of object if a class type)
        /// <para></para>&#160;&#160;&#160;
        /// Fill can use any type, including classes and will copy the value of the parameter.  In the case of classes, this 
        /// may invoke a copy-constructor.
        /// </summary>
        /// <param name="t"> = Value of Mem-type to fill</param>
        void Fill(const _t & t)
        {
            if (pMem)
            {
                _t * pTemp = pMem;
                for (int i=0;i<iSize;i++) *pTemp++ = t;
            }
        }

        // SetMaxBlock() -- Keep Memory sized to a certain block size past the current size. 
        //
        // **** Use this function BEFORE accessing the next pointer to ensure memory is available ****
        //
        // --> This sets the number of elements of the type in Mem<>, not the actual memory size.
        // --> Memory size is <size of type>*number of elements requested
        // 
        // Input:  iEval        --> Current place to evaluate (i.e. an index into the array)
        //         iThreshold   --> Threshold within the current block that should cause a reallocation.
        //                          if iThreshold is omitted, it iBlockSize is used in its place.
        //         iBlockSize   --> Memory Block Size
        //
        // Return: Pointer to current memory
        //        
        // Important Notes:     1. When calling this function, only use the returned pointer. 
        //                         Once SetMaxBlock() is called, the memory pointer may be changed. 
        //
        //                      2. This function is only evaluated once and is not continuous. 
        //                         It must be used as necessary as memory grows, and does not
        //                         grow memory automatically.
        //
        // Example:  SetMaxBlock(1234,700,1000)
        //           This declares a block size of 1000, and that the next block should be allocated
        //           if the current index (iEval) is 700.
        //
        //           Since the iEvel is 1234 and the block size is 1000, this means the place in the current
        //           block is iEval % iBlockSize, or 1234 % 1000 or 234.  Since 234 is < 700, the new block won't be added
        //           
        //           SetMaxBlock(1812,700,1000) will allocated new memory since the current position would
        //           be evaluate as 1812 % 1000, or 812, which is >= 700.
        //
        //           If the current memory exceeds the current place + the memory needed, no memory will be allocated.
        //
        _t * SetMaxBlock(int iEval,int iThreshold,int iBlockSize,bool bClearNewBlock = false)
        {
            int iEvalMod = iEval % iBlockSize; 
            
            // Correct for when iEval is a multiple of the block size. 

            if (!iEvalMod && iEval && iThreshold == iBlockSize) iEvalMod = iBlockSize; // We're at a multiple of blocksize.. 

            if ((iEvalMod >= iThreshold || iSize < iBlockSize || iSize < iEval) && iSize-iEval < iBlockSize)
            {
                // If the iEval is preemptive (i.e. bigger than current memory, allocate to the iEval value + Blocksize margin)

                if (iEval > iSize) iBlockSize = iBlockSize*((iEval + iBlockSize-1)/iBlockSize) - iSize + iBlockSize;
            //    _sbDebug(printf("%s: Allocated Memory: OrgEval = %d, iEvalMod = %d, iBlockSize = %d, iMem = %d, iNewMem = %d\n",    \
            //                                                        __func__,iEval,iEvalMod,iBlockSize,iSize,iSize+iBlockSize));
                int iCurSize = iSize;
                ResizeMax(iSize+iBlockSize);
                if (bClearNewBlock && pMem && iSize == iCurSize+iBlockSize)
                {
                    debugMsg(CString() << __func__ << ": Cleared New Memory (" << iSize-iCurSize << ") bytes.\n"); 
                    memset(pMem + iCurSize,0,(iSize-iCurSize)*sizeof(_t));
                }
            }
            return pMem;
        }

        // SetMaxBlockFast() -- Inlined version of SetMaxBlock() for when speed is an issue
        //
        // This allows the query to be fast so that SetMaxBlockSizeFast() can be called in a tight loop without
        // calling out to a function.  Note that ResizeMax is also inlined. 
        //
        //
        // Input:  iEval        --> Current place to evaluate (i.e. an index into the array)
        //         iThreshold   --> Threshold within the current block that should cause a reallocation.
        //                          if iThreshold is omitted, it iBlockSize is used in its place.
        //         iBlockSize   --> Memory Block Size
        //
        // Return: Pointer to current memory
        //        
        // Important Notes:     1. When calling this function, only use the returned pointer. 
        //                         Once SetMaxBlockFast() is called, the memory pointer may be changed. 
        //
        //                      2. This function is only evaluated once and is not continuous. 
        //                         It must be used as necessary as memory grows, and does not
        //                         grow memory automatically.
        //
        // Example:  SetMaxBlockFast(1234,700,1000)
        //           This declares a block size of 1000, and that the next block should be allocated
        //           if the current index (iEval) is 700.
        //
        //           Since the iEvel is 1234 and the block size is 1000, this means the place in the current
        //           block is iEval % iBlockSize, or 1234 % 1000 or 234.  Since 234 is < 700, the new block won't be added
        //           
        //           SetMaxBlockFast(1812,700,1000) will allocated new memory since the current position would
        //           be evaluate as 1812 % 1000, or 812, which is >= 700.
        //
        //           If the current memory exceeds the current place + the memory needed, no memory will be allocated.
        //
        // --> This sets the number of elements of the type in Mem<>, not the actual memory size.
        // --> Memory size is <size of type>*number of elements requested
        // 
        __forceinline _t * SetMaxBlock(int iEval,int iBlockSize,bool bClearNewBlock = false) { return SetMaxBlock(iEval,iBlockSize,iBlockSize,bClearNewBlock); }
        // --> This sets the number of elements of the type in Mem<>, not the actual memory size.
        // --> Memory size is <size of type>*number of elements requested
        // 
        __forceinline _t * SetMaxBlock(int iBlockSize,bool bClearNewBlock = false) { return SetMaxBlock(GetNumitems(),iBlockSize,iBlockSize,bClearNewBlock); }

        // SetMaxBlockFast() -- Inlined version of SetMaxBlock() for when speed is an issue
        //
        // This allows the query to be fast so that SetMaxBlockSizeFast() can be called in a tight loop without
        // calling out to a function.  Note that ResizeMax is also inlined. 
        //
        //
        // Input:  iEval        --> Current place to evaluate (i.e. an index into the array)
        //         iThreshold   --> Threshold within the current block that should cause a reallocation.
        //                          if iThreshold is omitted, it iBlockSize is used in its place.
        //         iBlockSize   --> Memory Block Size
        //
        // Return: Pointer to current memory
        //        
        // Important Notes:     1. When calling this function, only use the returned pointer. 
        //                         Once SetMaxBlockFast() is called, the memory pointer may be changed. 
        //
        //                      2. This function is only evaluated once and is not continuous. 
        //                         It must be used as necessary as memory grows, and does not
        //                         grow memory automatically.
        //
        // Example:  SetMaxBlockFast(1234,700,1000)
        //           This declares a block size of 1000, and that the next block should be allocated
        //           if the current index (iEval) is 700.
        //
        //           Since the iEvel is 1234 and the block size is 1000, this means the place in the current
        //           block is iEval % iBlockSize, or 1234 % 1000 or 234.  Since 234 is < 700, the new block won't be added
        //           
        //           SetMaxBlockFast(1812,700,1000) will allocated new memory since the current position would
        //           be evaluate as 1812 % 1000, or 812, which is >= 700.
        //
        //           If the current memory exceeds the current place + the memory needed, no memory will be allocated.
        //
        // --> This sets the number of elements of the type in Mem<>, not the actual memory size.
        // --> Memory size is <size of type>*number of elements requested
        // 
      __forceinline _t * SetMaxBlockFast(int iEval,int iThreshold,int iBlockSize,bool bClearNewBlock = false)
        {
           int iEvalMod = iEval % iBlockSize; 
            
            // Correct for when iEval is a multiple of the block size. 

            if (!iEvalMod && iEval && iThreshold == iBlockSize) iEvalMod = iBlockSize; // We're at a multiple of blocksize.. 

            if ((iEvalMod >= iThreshold || iSize < iBlockSize || iSize < iEval) && iSize-iEval < iBlockSize)
            {
                // If the iEval is preemptive (i.e. bigger than current memory, allocate to the iEval value + Blocksize margin)

                if (iEval > iSize) iBlockSize = iBlockSize*((iEval + iBlockSize-1)/iBlockSize) - iSize + iBlockSize;
                _sbDebug(printf("%s: Allocated Memory: OrgEval = %d, iEvalMod = %d, iBlockSize = %d, iMem = %d, iNewMem = %d\n",    \
                                                                    __func__,iEval,iEvalMod,iBlockSize,iSize,iSize+iBlockSize));
                int iCurSize = iSize;
                ResizeMax(iSize+iBlockSize);
                if (bClearNewBlock && pMem && iSize == iCurSize+iBlockSize)
                {
                    debugMsg(CString() << __func__ << ": Cleared New Memory (" << iSize-iCurSize << ") bytes.\n"); 
                    memset(pMem + iCurSize,0,(iSize-iCurSize)*sizeof(_t));
                }
            }
            return pMem;
        }

        // SetMaxBlockFast() -- Inlined version of SetMaxBlock() for when speed is an issue
        //
        // This allows the query to be fast so that SetMaxBlockSizeFast() can be called in a tight loop without
        // calling out to a function.  Note that ResizeMax is also inlined. 
        //
        //
        // Input:  iEval        --> Current place to evaluate (i.e. an index into the array)
        //         iThreshold   --> Threshold within the current block that should cause a reallocation.
        //                          if iThreshold is omitted, it iBlockSize is used in its place.
        //         iBlockSize   --> Memory Block Size
        //
        // Return: Pointer to current memory
        //        
        // Important Notes:     1. When calling this function, only use the returned pointer. 
        //                         Once SetMaxBlockFast() is called, the memory pointer may be changed. 
        //
        //                      2. This function is only evaluated once and is not continuous. 
        //                         It must be used as necessary as memory grows, and does not
        //                         grow memory automatically.
        //
        // Example:  SetMaxBlockFast(1234,700,1000)
        //           This declares a block size of 1000, and that the next block should be allocated
        //           if the current index (iEval) is 700.
        //
        //           Since the iEvel is 1234 and the block size is 1000, this means the place in the current
        //           block is iEval % iBlockSize, or 1234 % 1000 or 234.  Since 234 is < 700, the new block won't be added
        //           
        //           SetMaxBlockFast(1812,700,1000) will allocated new memory since the current position would
        //           be evaluate as 1812 % 1000, or 812, which is >= 700.
        //
        //           If the current memory exceeds the current place + the memory needed, no memory will be allocated.
        //
        __forceinline _t * SetMaxBlockFast(int iEval,int iBlockSize,bool bClearNewBlock = false) { return SetMaxBlockFast(iEval,iBlockSize,iBlockSize,bClearNewBlock); }

		// ResizeMax() -- Resize the memory to the new value or keep the current size, whichever is greater
		// if the parameter iMemSize is larger than the current memory size, the memory is reallocated.
		// Otherwise, the memory is not changed.
		//
		// This function is inlined for speed, so it may be used in tight loops to ensure memory before using it without
		// dumping the processor cache on a call.
		//
		// ResizeMax() returns the current pointer to the allocated memory, or nullptr if the allocation failed.
		//
        // --> This sets the number of elements of the type in Mem<>, not the actual memory size.
        // --> Memory size is <size of type>*number of elements requested
        // 
        // Note:  if the re-allocation fails, all memory is invalid and a nullptr is returned.  Check the return value for success or failure.
        //
		__forceinline _t * ResizeMax(int iNumElements)
		{
			if (iNumElements > iSize) 
			{
				auto pMem2 = (_t *) realloc(pMem,iNumElements*sizeof(_t)); 
                if (pMem2)
                {
                    pMem = pMem2; 
                    iSize = iNumElements;
                }
                else if (pMem)
                {
                    free(pMem); 
                    pMem = nullptr;
                }
			}
			if (!pMem) iSize = 0;
			return pMem;
		}

        // This will perform a new allocation and will not re-allocate data.
        // Any current data is lost when using this function.  Use ResizeMax() to reallocate memory and keep the same memory. 
        // 
        // This function will set the memory required, reducing or growing stored memory.  ResizeMax() will only size upwards and will not reduce memory.
        //
        // --> This sets the number of elements of the type in Mem<>, not the actual memory size.
        // --> Memory size is <size of type>*number of elements requested
        // 
		__forceinline _t * SetMemSize(int iNumElements)
		{
             if (iNumElements <= 0) return pMem;
             if (iSize == iNumElements) return pMem;
            if (pMem) free(pMem);
			pMem = (_t *) malloc(iNumElements*sizeof(_t)); 
            iSize = pMem ? iNumElements : 0;
			return pMem;
		}

       // This will perform a new allocation and will not re-allocate data, but will only grow memory, and will not reduce the size of the current memory.
       // This is useful in establishing a maximum buffer size that can vary, without reducing the maximum value seen.
        // Any current data is lost when using this function.  Use ResizeMax() to reallocate memory and keep the same memory. 
        // 
        // --> This sets the number of elements of the type in Mem<>, not the actual memory size.
        // --> Memory size is <size of type>*number of elements requested
        // 
        // Use SetMemSize() to set a specific memory size, which will grow or shrink memory used. 
        //
		__forceinline _t * SetMemSizeMax(int iNumElements)
		{
			return iNumElements > iSize ? SetMemSize(iNumElements) : pMem;
		}
	
		_t MemNull{};
		bool isValid() { return pMem != nullptr; };
		bool isEmpty() { return pMem == nullptr; };
	//	_t * operator ->() const { return &pMem; };
		operator _t * () const { return (_t *) pMem; };
		_t * operator = (int iSize)
		{
			if (pMem) free(pMem);
			this->iSize = iSize;
			if (iSize) pMem = (_t *) std::malloc(iSize*sizeof(_t));
			return (_t *) pMem;
		}
		///__forceinline _t & operator [](int i) { return pMem ? *(pMem + i) : MemNull;  }
#if defined(NOBOUNDS)
            __forceinline _t & operator [](int i) { return pMem[i]; }
#elif defined(NOMEMEXCEPT)

		 __forceinline _t & operator [](int i) 
         { 
             if (!pMem) return MemNull;
             if ((i < 0) | (i >= iSize)) i = 0;
             return pMem[i];  
         }
#else
		 __forceinline _t & operator [](int i) 
         { 
             if ((!pMem) | (i < 0) | (i >= iSize))
             {
                 auto cs = MemTools::ShowExceptMsg("Mem Object Error",
                        CString() << "Mem::[] Value out of range.\n\nValue = " << i << "\nMax Size = " << iSize,__FILE__,__LINE__);
                throw std::exception(cs.str()); 
             }
             return pMem[i];  
         }
#endif


		 __forceinline _t & operator ()(int i) { return pMem[i];  }
		bool copyFrom(Mem & p2)
		{
			if (pMem) free(pMem); 
			iSize = 0;
			if (p2.isEmpty()) return true;
			pMem = (_t *) std::malloc(p2.iSize*sizeof(_t)); 
			if (!pMem) return false;
			memcpy(pMem,p2.pMem,p2.iSize*sizeof(_t));
			iSize = p2.iSize;
			return true;
		}

	};


// Aligned Memory version -- useful for SSE operations, and is just in-general faster for int, double, long arrays, etc.
//
template <class _t>
	class MemA
	{

	private:
	MemA & operator = (const MemA &p2)
		{
			if (pMem) _aligned_free(pMem);
			memcpy(this,&p2,sizeof(*this));
			MemA * pMem = (MemA *) &p2;
			pMem->pMem = nullptr;
			pMem->iSize = 0;

			return *this;
		}

	public:
		int iSize = 0;
		_t * pMem = nullptr;
        _t * GetMem() { return pMem; }

   MemA(MemA && p2) noexcept
    {
        iSize = p2.iSize;
        pMem = p2.pMem; 

        p2.iSize = 0;
        p2.pMem = nullptr;
    }
    MemA & operator=(MemA && p2) noexcept
    {
        if (this != &p2)
        {
            iSize = p2.iSize;
            pMem = p2.pMem ;

            p2.iSize = 0;
            p2.pMem = nullptr;
        }
        return *this;
    }		bool SetData(_t * pInMem,int iFileSize)
		{
			if (!pInMem || iFileSize <= 0) return false;			
			if (pMem) _aligned_free(pMem);
			pMem = pInMem;
			iSize = iFileSize;
			return true;
		}
		void DeleteData()
		{
			if (pMem) _aligned_free(pMem);
			pMem = nullptr;
			iSize = 0;
		}
		MemA(_t * pMem,int iFileSize)
		{
			SetData(pMem,iFileSize);
		}
		
		MemA & moveFrom(const MemA & p2)
		{
			*this = p2;
			return *this;
		}
		MemA(int iSize)
		{
			this->iSize = iSize;
			if (iSize) pMem = (_t *) _aligned_malloc(iSize*sizeof(_t),128);;
		}
		MemA()
		{
			iSize = 0;
			pMem = nullptr;
		}
		~MemA()
		{
			if (pMem) _aligned_free(pMem);
			pMem = nullptr;
			iSize = 0;
		}
		// GetNumItems() -- Returns the number of ellements allocated.  Use GetMemSize() for total memory size
		//
		__forceinline int GetNumitems()  { return pMem ? iSize : 0; }

		// GetMemSize() -- Returns the total memory size allocated for this memory object (i.e. number of elements X sizeof(element_type)_
		// Use GetNumItems() for number of elements, or use variable "iSize" directly.
		//
		__forceinline long GetMemSize() { return pMem ? iSize*sizeof(_t) : 0; }

		// Clear all allocate memory of array.  If a value is specified, the memory is filled with this value (unsigned char of 0-255)
		//
		void ClearMem()
		{
			if (pMem) std::memset(pMem,0,iSize*sizeof(_t));
		}

		// Clear all allocate memory of array.  If a value is specified, the memory is filled with this value (unsigned char of 0-255)
		//
		void ClearMem(unsigned char ucValue)
		{
			if (pMem) std::memset(pMem,ucValue,iSize*sizeof(_t));
		}

        /// <summary>
        /// Fill memory with a specified value corresponding to the Mem type (i.e. int, double, etc. 
        /// This differs from ClearMem() which fills the memory explictitly with zeros or the unsigned char value specified
        /// (which is faster than using Fill).
        /// <para></para>&#160;&#160;&#160;
        /// Fill() fills the entire array with the same value (or copy of object if a class type)
        /// <para></para>&#160;&#160;&#160;
        /// Fill can use any type, including classes and will copy the value of the parameter.  In the case of classes, this 
        /// may invoke a copy-constructor.
        /// </summary>
        /// <param name="t"> = Value of Mem-type to fill</param>
        void Fill(const _t & t)
        {
            if (pMem)
            {
                _t * pTemp = pMem;
                for (int i=0;i<iSize;i++) *pTemp++ = t;
            }
        }
		MemA(const MemA &p2)
		{
			if (&p2 == this)
			{
				int gg = 1;
			}
			memcpy(this,&p2,sizeof(*this));
			MemA * pMem = (MemA *) &p2;
			pMem->pMem = nullptr;
			pMem->iSize = 0;
		}

		// ResizeMax() -- Resize the memory to the new value or keep the current size (number of elements), whichever is greater
		// if the parameter iMemSize is larger than the current memory size, the memory is reallocated.
		// Otherwise, the memory is not changed.
		//
		// This function is inlined for speed, so it may be used in tight loops to ensure memory before using it without
		// dumping the processor cache on a call.
		//
		// ResizeMax() returns the current pointer to the allocated memory, or nullptr if the allocation failed.
		//
		__forceinline _t * ResizeMax(int iNumElements)
		{
            _t * pNewMem = nullptr;
			if (iNumElements > iSize) 
			{
				if (pNewMem = (_t *) _aligned_realloc(pMem,iNumElements*sizeof(_t),128))
                {
				    iSize = iNumElements;
                    pMem = pNewMem;
                } 

			    if (!pNewMem) 
                {
                    iSize = 0;
                    if (pMem) _aligned_free(pMem);
                    pMem = nullptr;
                }
            }
			return pMem;     // Return nullptr if we failed, even if old memory is still intact. 
		}

        // This will perform a new allocation and will not re-allocate data.
        // Any current data is lost when using this function.  Use ResizeMax() to reallocate memory and keep the same memory. 
        // 
        // This function will set the memory required, reducing or growing stored memory.  ResizeMax() will only size upwards and will not reduce memory.
        //
		__forceinline _t * SetMemSize(int iNumElements)
		{
            if (iNumElements <= 0) return pMem;
            if (pMem) _aligned_free(pMem);
			pMem = (_t *) _aligned_malloc(iNumElements*sizeof(_t),128); 
            iSize = pMem ? iNumElements : 0;
			return pMem;
		}

       // This will perform a new allocation and will not re-allocate data, but will only grow memory, and will not reduce the size of the current memory.
       // This is useful in establishing a maximum buffer size that can vary, without reducing the maximum value seen.
        // Any current data is lost when using this function.  Use ResizeMax() to reallocate memory and keep the same memory. 
        // 
        // Use SetMemSize() to set a specific memory size, which will grow or shrink memory used. 
        //
		__forceinline _t * SetMemSizeMax(int iNumElements)
		{
			return iNumElements > iSize ? SetMemSize(iNumElements) : pMem;
		}		_t MemNull{};

		bool isValid() { return pMem != nullptr; };
		bool isEmpty() { return pMem == nullptr; };
	//	_t * operator ->() const { return &pMem; };
		operator _t * () const { return (_t *) pMem; };
		_t * operator = (int iSize)
		{
			if (pMem) _aligned_free(pMem);
			this->iSize = iSize;
			if (iSize) pMem = (_t *) _aligned_malloc(iSize*sizeof(_t),128);
			return (_t *) pMem;
		}
//		__forceinline_t & operator [](int i) { return pMem ? pMem[i] : MemNull; }
#if defined(NOBOUNDS)
            __forceinline _t & operator [](int i) { return pMem[i]; }
#elif defined(NOMEMEXCEPT)

		 __forceinline _t & operator [](int i) 
         { 
             if (!pMem) return MemNull;
             if ((i < 0) | (i >= iSize)) i = 0;
             return pMem[i];  
         }
#else
		 __forceinline _t & operator [](int i) 
         { 
             if ((!pMem) | (i < 0) | (i >= iSize))
             {
                 auto cs = MemTools::ShowExceptMsg("MemA Object Error",
                        CString() << "MemA::[] Value out of range.\n\nValue = " << i << "\nMax Size = " << iSize,__FILE__,__LINE__);
                throw std::exception(cs.str()); 
             }
             return pMem[i];  
         }
#endif
		bool copyFrom(MemA & p2)
		{
			if (pMem) _aligned_free(pMem); 
			iSize = 0;
			if (p2.isEmpty()) return true;
			pMem = (_t *) _aligned_malloc(p2.iSize*sizeof(_t),128); 
			if (!pMem) return false;
			memcpy(pMem,p2.pMem,p2.iSize*sizeof(_t));
			iSize = p2.iSize;
			return true;
		}

	};

template <class _t>
	class Obj
	{

	private:
		_t * pObj = nullptr;

	public:
		Obj(_t * p)
		{
			pObj = p; 
		}
		~Obj()
		{
			if (pObj) delete pObj;
			pObj = nullptr;
		}
		Obj(const Obj &p2)
		{
			memcpy(this,&p2,sizeof(*this));
			Obj * pMem = (Mem *) &p2;
			pMem->pObj = nullptr;
		}

		bool isValid() { return Obj != nullptr; };
		bool isEmpty() { return Obj == nullptr; };
		_t * operator ->() const { return pObj; };
	};
}; // namespace Sage
#endif // _CMemClass_H_