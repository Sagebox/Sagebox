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

#include "Sage.h"

#if !defined(_CJPEG_H_)
#define _CJPEG_H_
namespace Sage
{

class CJpeg
{
public:
	enum class Status
	{
		Ok,
		EmptyFilePath,
		FileNotFound,
		FileLengthZero,
		Error,
	};
private:
	static bool m_bBusy;
	Status m_eStatus = Status::Ok;
	[[nodiscard]] RawBitmap_t ReadJpegMem(const unsigned char * sData,int iDataLength,bool * bSuccess = nullptr);
	void SetBusy(bool bBusy);	// Stop-gap until multi-threading is more properly buit in
	void SleepBusy();
public:
	[[nodiscard]] RawBitmap_t  ReadJpegFile(const char * sPath,bool * bSuccess = nullptr);
	[[nodiscard]] RawBitmap_t  ReadJpeg(const unsigned char * sData,int iDataLength,bool * bSuccess = nullptr);
	Status GetStatus() { return m_eStatus; }		// Status of last opersation/request
};
}; // namespace Sage
#endif // _CJPEG_H_
