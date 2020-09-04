//#pragma once
#if !defined(_CMemClass_H_)
#define _CMemClass_H_

namespace Sage
{
template <class _t>
	class Mem
	{
	public:
		int iSize = 0;
		_t * pMem = nullptr;
		Mem(int iSize)
		{
			this->iSize = iSize;
			if (iSize) pMem = (_t *) malloc(iSize*sizeof(_t));;
		}
		Mem()
		{
			iSize = 0;
			pMem = nullptr;
		}
		~Mem()
		{
			if (pMem) free(pMem);
		}
		void ClearMem()
		{
			if (pMem) memset(pMem,0,iSize*sizeof(_t));
		}
		operator _t * () const { return (_t *) pMem; };
		_t * operator = (int iSize)
		{
			if (pMem) free(pMem);
			this->iSize = iSize;
			if (iSize) pMem = (_t *) malloc(iSize*sizeof(_t));
			return (_t *) pMem;
		}
		_t & operator [](int i) const    { return pMem[i]; }
	};
}; // namespace Sage
#endif // _CMemClass_H_