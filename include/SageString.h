
//#pragma once
#if !defined(_SageString_H_)
#define _SageString_H_
namespace Sage
{
struct SageStringW;

struct SageString
{
	int iStringSize;
	int iMemSize;
	char * spData;
public:
		void Init() { spData = nullptr;  iStringSize = iMemSize = 0; }
	void Delete()
	{
		if (spData) free(spData);
		spData = nullptr;
		iStringSize = iMemSize = 0;
	}
	void Clear()
	{
		if (spData) spData[0] = 0;
		iStringSize = 0;
	}

	// We've just been coped, but need to act as a copy constructor by reallocating new memory
	void CloneSelf()
	{
		if (spData)
		{
			char * spOld = spData;
			spData = (char *) malloc(iMemSize);
			strcpy(spData,spOld);
		}
	}

	// return Nullptr or string reference, but not a ""
	//
	char * GetPureStr()
	{
		return (!spData || !*spData) ? nullptr : spData;
	}
	char * GetMem(int iSize);
	char * SetString(char * sString)
	{
		// if it's a nullptr, just return what we have.  CLear string if we have memory.
		if (!sString)
		{
		//	if (spData) FailBox("SageString::SetString()","Setting !String with spData");
			if (spData) spData[0] = 0;
			return spData;		// return either nullptr or pointer to initialized string
		}

		// If it's an empty string, allocate memory anyway.

		int iLength = (int) strlen(sString);
		GetMem(iLength+1);
		if (spData) 
		{
			strcpy(spData,sString);
			iStringSize = iLength;
		}
		else iStringSize = 0;
		return spData;
	}
	char * SetString(wchar_t * sString)
	{
		// if it's a nullptr, just return what we have.  CLear string if we have memory.
		if (!sString)
		{
		//	if (spData) FailBox("SageString::SetString()","Setting !String with spData");
			if (spData) spData[0] = 0;
			return spData;		// return either nullptr or pointer to initialized string
		}

		// If it's an empty string, allocate memory anyway.

		int iLength = (int) wcslen(sString);
		GetMem(iLength+1);
		if (spData) 
		{
			wcstombs(spData,sString,iLength+1);
//			strcpy(spData,sString);
			iStringSize = iLength;
		}
		else iStringSize = 0;
		return spData;
	}
	char * AddString(char * sString)
	{
		if (!sString || !*sString) return spData;
		int iLength = (int) strlen(sString);
		GetMem(iMemSize + iLength);
		if (spData) strcat(spData,sString);
		iStringSize += iLength;
	
		return spData;
	}

	char * operator * () { return spData;  }
	 operator char * () { return spData;  }
	 operator const char * () { return spData;  }
	SageString & operator = (SageStringW & sString);
	SageString & operator = (char * sString) { SetString(sString); return *this; }
	SageString & operator = (const char * sString) { SetString((char *) sString); return *this; }
	//SageString & operator = (const char * sString) { SetString((char *) sString); return *this; }
	SageString & operator += (char * sString) { AddString(sString); return *this; }
	SageString & operator += (SageString & stString) { AddString(*stString); return *this; }

};


struct SageStringW
{
	int iStringSize;
	int iMemSize;
	wchar_t * spData;
public:
		void Init() { spData = nullptr;  iStringSize = iMemSize = 0; }
	void Delete()
	{
		if (spData) free(spData);
		spData = nullptr;
		iStringSize = iMemSize = 0;
	}
	void Clear()
	{
		if (spData) spData[0] = 0;
		iStringSize = 0;
	}

	// We've just been coped, but need to act as a copy constructor by reallocating new memory
	void CloneSelf()
	{
		if (spData)
		{
			wchar_t * spOld = spData;
			spData = (wchar_t *) malloc(iMemSize*sizeof(wchar_t));
			wcscpy(spData,spOld);
		}
	}

	// return Nullptr or string reference, but not a ""
	//
	wchar_t * GetPureStr()
	{
		return (!spData || !*spData) ? nullptr : spData;
	}
	wchar_t * GetMem(int iSize)
	{
	//	if (!iSize) spData;
		// If it's the initial allocation, allocate more memory than needed to avoid
		// continuous reallocations
		//if (!spData) iSize = 1024 + iSize;		
		if (iSize > iMemSize)
		{
			spData = (wchar_t *) realloc(spData,(iSize+1024)*sizeof(wchar_t));
			iMemSize = iSize+1024;
		}
		return spData;
	}
	wchar_t * SetString(char * sString)
	{
		if (!sString)
		{
		//	if (spData) FailBox("SageString::SetString()","Setting !String with spData");
			if (spData) spData[0] = 0;
			return spData;		// return either nullptr or pointer to initialized string
		}

		// If it's an empty string, allocate memory anyway.

		int iLength = (int) strlen(sString);
		GetMem(iLength+1);
		if (spData) 
		{
			mbstowcs(spData,sString,iLength+1);
		//	wcscpy(spData,sString);
			iStringSize = iLength;
		}
		else iStringSize = 0;
		return spData;	
	}

	wchar_t * SetString(wchar_t * sString)
	{
		// if it's a nullptr, just return what we have.  CLear string if we have memory.
		if (!sString)
		{
		//	if (spData) FailBox("SageString::SetString()","Setting !String with spData");
			if (spData) spData[0] = 0;
			return spData;		// return either nullptr or pointer to initialized string
		}

		// If it's an empty string, allocate memory anyway.

		int iLength = (int) wcslen(sString);
		GetMem(iLength+1);
		if (spData) 
		{
			wcscpy(spData,sString);
			iStringSize = iLength;
		}
		else iStringSize = 0;
		return spData;
	}

	wchar_t * AddString(char * sString)
	{
		if (!sString || !*sString) return spData;
		int iLength = (int) strlen(sString);
		GetMem(iMemSize + iLength);
		if (spData)
		{
			mbstowcs(spData + (int) wcslen(spData),sString,iLength+1);
//			wcscat(spData,sString);
		}
		iStringSize += iLength;
	
		return spData;
	}
	wchar_t * AddString(wchar_t * sString)
	{
		if (!sString || !*sString) return spData;
		int iLength = (int) wcslen(sString);
		GetMem(iMemSize + iLength);
		if (spData) wcscat(spData,sString);
		iStringSize += iLength;
	
		return spData;
	}

	wchar_t * operator * () { return spData;  }
	 operator wchar_t * () { return spData;  }
	 operator const wchar_t * () { return spData;  }
	SageStringW & operator = (char * sString) { SetString(sString); return *this; }
	SageStringW & operator = (const char * sString) { SetString((char *) sString); return *this; }
	SageStringW & operator += (char * sString) { AddString(sString); return *this; }
	SageStringW & operator += (SageString & stString) { AddString(*stString); return *this; }

	SageStringW & operator = (wchar_t * sString) { SetString(sString); return *this; }
	SageStringW & operator = (const wchar_t * sString) { SetString((wchar_t *) sString); return *this; }
	SageStringW & operator += (wchar_t * sString) { AddString(sString); return *this; }
	SageStringW & operator += (SageStringW & stString) { AddString(*stString); return *this; }


};


}; // namepace Sage

#endif // _SageString_H_