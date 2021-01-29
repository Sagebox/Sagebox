// File copyright(c) 2021, Rob Nelson, All Rights reserved.  Sagebox is free for personal use. 
// e-mail: rob@projectsagebox.com

// CAviFile.h -- Simple Avi Functions for Sagebox 




#if !defined(__CLOCALAVI_H__)
#define __CLOCALAVI_H__

#include "CSageBox.h"
#include <vfw.h>

namespace Sage
{
class CAviFile
{
    static constexpr int kMaxErrMsgLength = 200;

public:
    // Status return for most functions. 

    enum class Status
    {
        Ok                      ,
        DllNotFound         	,
        FileNotFound			,
        VideoStreamNotFound	    ,
        FrameNotFound			,
        FrameBufferDataNotFound	,
        AviNotInitialized       ,
        MemoryAllocationError   ,
        CouldNotWriteToAviFile  ,
        NoOutputFile            ,
        BitmapSizeError         ,
        AviAlreadyOpen          ,
        Unknown                 ,
    };

private:

    // Status messages for each status indicator

    static constexpr const char * m_sErrMessages[13] = {
        "Ok.",
        "avifil32.dll DLL not found.",
        "File not found.",
        "Video stream not found.",
        "AVI frame not found.",
        "AVI frame buffer not found.",
        "AVI not initialized.",
        "Memory could not be allocated.",
        "Could not write to AVI file.",
        "No Output file exists.",
        "Bitmap Size Error (Read or Write operation was given bitmap of incorrect size).",
        "An Avi file has already been opened or created.  ResetAvi() must be used to re-initialize.",
        "Unknown Error."
    };

    // DLL Function prototypes and pointers

    char                    m_sLastError[kMaxErrMsgLength+10];  
	int					    m_iOutFrameCount;
	AVISTREAMINFOA			m_stStreamHeader;
	HRESULT		(WINAPI *   m_fAVIFileOpen			    )(PAVIFILE * ppfile,LPCSTR szFile,UINT uMode,LPCLSID lpHandler);
	HRESULT		(WINAPI *   m_fAVIFileInfo			    )(PAVIFILE pFile,LPAVIFILEINFOA pfi,LONG lSize);
	HRESULT		(WINAPI *   m_fAVIFileGetStream		    )(PAVIFILE pFile,PAVISTREAM * ppavi,DWORD fccType,LONG lParam);
	PGETFRAME	(WINAPI *   m_fAVIStreamGetFrameOpen	)(PAVISTREAM pavi,LPBITMAPINFOHEADER lpbiWanted);
	void		(WINAPI *   m_fAVIFileInit			    )(void);
    ULONG       (WINAPI *   m_fAVIStreamRelease         )(PAVISTREAM pavi);
    void        (WINAPI *   m_fAVIFileExit              )(void);	
    ULONG       (WINAPI *   m_fAVIFileRelease           )(PAVIFILE pfile);
    HRESULT     (WINAPI *   m_fAVIFileCreateStream      )(PAVIFILE pfile, PAVISTREAM FAR *ppavi, AVISTREAMINFOA FAR * psi);
    HRESULT     (WINAPI *   m_fAVIStreamInfo            )(PAVISTREAM pavi,LPAVISTREAMINFOA psi, LONG lSize);
    HRESULT     (WINAPI *   m_fAVIStreamSetFormat       )(PAVISTREAM pavi,LONG lPos,LPVOID lpFormat,LONG cbFormat);
    HRESULT     (WINAPI *   m_fAVIStreamWrite           )(PAVISTREAM pavi,
			                                              LONG lStart, LONG lSamples,
			                                              LPVOID lpBuffer, LONG cbBuffer, DWORD dwFlags,
			                                              LONG FAR *plSampWritten,
			                                              LONG FAR *plBytesWritten);
    LPVOID      (WINAPI * m_fAVIStreamGetFrame          )(PGETFRAME pg,LONG lPos);
    LONG        (WINAPI * m_fAVIStreamLength            )(PAVISTREAM pavi);

    FARPROC m_fAVIFileOpen1;			
	FARPROC m_fAVIFileInfo1;		
	FARPROC m_fAVIFileGetStream1;	
	FARPROC m_fAVIStreamGetFrameOpen1;
	FARPROC m_fAVIFileInit1;
    FARPROC m_fAVIStreamRelease1;		
    FARPROC m_fAVIFileExit1;		
    FARPROC m_fAVIFileRelease1;
    FARPROC m_fAVIFileCreateStream1;
    FARPROC m_fAVIStreamInfo1;
    FARPROC m_fAVIStreamSetFormat1;  
    FARPROC m_fAVIStreamWrite1;   
    FARPROC m_fAVIStreamGetFrame1;
    FARPROC m_fAVIStreamLength1;

	char          sFilename[MAX_PATH+1024];
	char          sOutFilename[MAX_PATH+1024];
	char        * m_sFile;
	char        * m_sOutFile;
	
	AVISTREAMINFOA		m_stAviStreamInfo;
	BITMAPINFOHEADER	m_stBitmapInfo;
	PAVIFILE			m_pAviFile;
	PAVIFILE			m_pOutAviFile;
	PAVISTREAM			m_pAviStream;
	PAVISTREAM			m_pOutAviStream;
	HMODULE				m_hmAviModule;

	bool				m_bDLLLoaded;
	CDevString			m_cs;
	bool InitializeDLL();
	void SetErrorMsg(char * sMsg) { strncpy(m_sLastError,sMsg,kMaxErrMsgLength-1); m_sLastError[kMaxErrMsgLength-1] = 0; }
	
    Status  CloseOutFile();
	Status  CloseInFile();
    void    InitData();
public:
    // Members left public so they can be addressed specifically 

	AVIFILEINFOA			m_stAviFileInfo;
	PGETFRAME			    m_pGetFrame;

	CAviFile();
    ~CAviFile();        // Destructor closes open files.  CloseFile() is included, but not necessary to use. 

    // Public functions

    // OpenAviFile() -- Open an existing AVI File
    //
    // if iFrameCount is given, it is filled with the framecount. 
    //
    // Only one Avi File can be opened at a time.  The Status AviFileAlreadyOpen will be returned if 
    // an Avi has been opened or created without ResetAvi() called first.
    //
	Status  OpenAviFile(char * sFilename);  

    // CreateAviFile -- Create a new AVI file
    //
    // Only one Avi File can be opened at a time.  The Status AviFileAlreadyOpen will be returned if 
    // an Avi has been opened or created without ResetAvi() called first.
    //
	Status  CreateAviFile(const char * sOutputAviFile,int iWidth,int iHeight,int iFrameRate = 30);

    // CreateAviFile -- Create a new AVI file
    //
	Status  CreateAviFile(const char * sOutputAviFile,SIZE szSize,int iFrameRate = 30);

    // CloseFile() -- Close open file (either existing AVI file or newly created AVI file)
    //
    Status  CloseFile();

    // WriteFrame() -- Write a frame of Bitmap data.  Bitmap data is 24-bit RGB Data
    //                 (i.e. unsigned char * of Red,Green,Blue in succession).
    // 
    // sFrameSource     -- the bitmap data
    // bAligned         -- When true, each bitmap line is divisible by 4 (i.e. aligned)
    //                     When false, the bitmap data is sequential with no breaks.
    // 
    // WriteFrame may also be used by specifying a CBitmap, i.e. Writeframe(MyBitmap); 
    //
    // note: The bitmap must be in the exact dimensions as the width & height created, otherwise it is ignored.
    //
    Status  WriteFrame(unsigned char * sFrameSource,bool bAligned = true);

    // WriteFrame() -- Write a frame of Bitmap data.  Bitmap data is 24-bit RGB Data
    //                 (i.e. unsigned char * of Red,Green,Blue in succession).
    // 
    // sFrameSource     -- the bitmap data
    // bAligned         -- When true, each bitmap line is divisible by 4 (i.e. aligned)
    //                     When false, the bitmap data is sequential with no breaks.
    // 
    // WriteFrame may also be used by specifying a CBitmap, i.e. Writeframe(MyBitmap); 
    //
    // note: The bitmap must be in the exact dimensions as the width & height created, otherwise it is ignored.
    //
    Status  WriteFrame(CBitmap & cBitmap);

    // ReadFrame() -- Read a frame of the avi into a bitmap buffer
    //
    // iFrameID         -- Which frame to read
    // sFrameBuffer     -- the bitmap data in which to read the data
    // bAligned         -- When true, each bitmap line is divisible by 4 (i.e. aligned)
    //                     When false, the bitmap data is sequential with no breaks.
    // 
    // ReadFrame may also be used by specifying a CBitmap, i.e. ReadFrame(iFrameID,MyBitmap); 
    //
    // note: The bitmap must be in the exact dimensions as the width & height created, otherwise it is ignored.
    //
	Status  ReadFrame(int iFrameID,unsigned char * sFrameBuffer,bool bAligned = true);

    // ReadFrame() -- Read a frame of the avi into a bitmap buffer
    //
    // iFrameID         -- Which frame to read
    // sFrameBuffer     -- the bitmap data in which to read the data
    // bAligned         -- When true, each bitmap line is divisible by 4 (i.e. aligned)
    //                     When false, the bitmap data is sequential with no breaks.
    // 
    // ReadFrame may also be used by specifying a CBitmap, i.e. ReadFrame(iFrameID,MyBitmap); 
    //
    // note: The bitmap must be in the exact dimensions as the width & height created, otherwise it is ignored.
    //
	Status  ReadFrame(int iFrameID,CBitmap & cBitmap);

    // isOperational() -- returns true of the DLL is loaded, false if not. 
    //
	bool    isOperational() { return m_bDLLLoaded; }

    // ResetAvi() -- Reset loaded AVI this closes any files open and resets the AVI object for re-used.
    //
	Status  ResetAvi();

    // getOutputFrameCount() -- Return number of frames written to AVI file
    //
    int getOutputFrameCount() { return m_iOutFrameCount; }

    // GetStatusMsg() -- return a text string that explains the given status.
    //
    const char * GetStatusMsg(Status aviStatus);

    // Get FrameCount() -- Get number of frames of currently opened AVI file
    //
    // returns 0 on error (or if there are 0 frames) -- any error means there is no active input or output stream.
    //
    long GetFrameCount();

    // Get FrameSize() -- Get the frame size of the currently opened AVI file.  This
    // represents the output or input bitmap size. 
    //
    // return {0,0} on error -- any error means there is no active input or output stream.
    //
    SIZE GetFrameSize();
}; // __CLOCALAVI_H__
}; // namespace Sage
#endif