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

//#pragma once
#if !defined(_CMemClass_H_)
#define _CMemClass_H_
#include <cstdlib>
#include <cstdio>
#include <memory>
#include <malloc.h>

namespace Sage
{

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

		// ResizeMax() -- Resize the memory to the new value or keep the current size, whichever is greater
		// if the parameter iMemSize is larger than the current memory size, the memory is reallocated.
		// Otherwise, the memory is not changed.
		//
		// This function is inlined for speed, so it may be used in tight loops to ensure memory before using it without
		// dumping the processor cache on a call.
		//
		// ResizeMax() returns the current pointer to the allocated memory, or nullptr if the allocation failed.
		//
		__forceinline _t * ResizeMax(int iMemSize)
		{
			if (iMemSize > iSize) 
			{
				pMem = (_t *) realloc(pMem,iMemSize*sizeof(_t)); 
				iSize = iMemSize;
			}
			if (!pMem) iSize = 0;
			return pMem;
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
		_t & operator [](int i) { return pMem ? pMem[i] : MemNull; }
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
			if (iSize) pMem = (_t *) _aligned_malloc(iSize*sizeof(_t),16);;
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

		// ResizeMax() -- Resize the memory to the new value or keep the current size, whichever is greater
		// if the parameter iMemSize is larger than the current memory size, the memory is reallocated.
		// Otherwise, the memory is not changed.
		//
		// This function is inlined for speed, so it may be used in tight loops to ensure memory before using it without
		// dumping the processor cache on a call.
		//
		// ResizeMax() returns the current pointer to the allocated memory, or nullptr if the allocation failed.
		//
		__forceinline _t * ResizeMax(int iMemSize)
		{
			if (iMemSize > iSize) 
			{
				pMem = (_t *) _aligned_realloc(pMem,iMemSize*sizeof(_t),16); 
				iSize = iMemSize;
			}
			if (!pMem) iSize = 0;
			return pMem;
		}
	
		_t MemNull{};
		bool isValid() { return pMem != nullptr; };
		bool isEmpty() { return pMem == nullptr; };
	//	_t * operator ->() const { return &pMem; };
		operator _t * () const { return (_t *) pMem; };
		_t * operator = (int iSize)
		{
			if (pMem) _aligned_free(pMem);
			this->iSize = iSize;
			if (iSize) pMem = (_t *) _aligned_malloc(iSize*sizeof(_t),16);
			return (_t *) pMem;
		}
		_t & operator [](int i) { return pMem ? pMem[i] : MemNull; }
		bool copyFrom(MemA & p2)
		{
			if (pMem) _aligned_free(pMem); 
			iSize = 0;
			if (p2.isEmpty()) return true;
			pMem = (_t *) _aligned_malloc(p2.iSize*sizeof(_t),16); 
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