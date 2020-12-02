
//#pragma once


#ifndef __CPGR_H__
#define __CPGR_H__

#include <Windows.h>
#include "Sage.h"
#include "CString.h"
namespace Sage
{

struct stAuthNames_t
{
    char sAuthName[100];
    char sAuthKey[200];
    int iExpires; 
};
void InitializeEncrypt(int iSeed);

void EncryptPGRData(unsigned char * sInString,int iLength,unsigned int uiDValue);
void DecryptPGRData(unsigned char * sInputString,int iLength,unsigned int uiDValue);
void DecryptEntireFile(unsigned char * sSource,int iSize,int iLocation);
void InitializeEncrypt(int iSeed);
int DecryptString(char * sInputString,char * sOutString,int iLength,unsigned int uiDValue);
int EncryptString(char * sInString,char * sOutString,int iLength,unsigned int uiDValue);




#define kPGRFileDataID              0x53E918C4
#define kPGRFileHeaderID            0xB259E812
#define kPGRKeyTableDataID          0x7E9B235A
#define kPGRStringTableDataID       0xE38A13A4
#define kPGRFilesID                 0x4AB72779

#define kRandomEncryptXorValue     (((unsigned int) (EncRand() & 0x7FFF)) << 17 | (((unsigned int) (EncRand() & 0x7FFF)) << 2) | (EncRand() & 3))

// Encryption Keys for Encrypted PGR Keys

#define kPGR_DataKey1       0x34,0x87,0xF3,0xE9,0xDA,0x9E,0xF7,0x7D,0x68,0xBE,0xE4              // 8-bit data xor'd with LENGTH of DATA
#define kPGR_DataKey2       0x57,0xE4,0x49,0x9A,0x3B,0x79,0xF3,0xE3,0x6D,0xC1,0x92              // Rotation Data for 16-bit value (xor'd with LENGTH of DATA)
#define kPGR_DataKey3       0x1134,0x9e4D,0xFe45,0x1D39,0xD9ED,0x3E98,0xF1D3,0x4F3A,0x1E39      // 16-Bit Data.  Data is exchanged and xored with this value (xor'd with LENGTH of DATA)
#define kPGR_DataKey4       1,12,3,9,13,9,10,11,8,3,10,8,3,4,8,5,12,13                          // Rotation Data for 16-bit Value AFTER 16-bit XOR(xor'd with LENGTH of DATA)

// Encryption Keys for Encrypted PGR Data (Keys are doubly encrypted with above keys) 

#define kPGR_GDataKey1      0x83,0xe9,0x2C,0xF4,0x9B,0xE3,0x6A,0x32,0x9F,0x5F,0xB8              // 8-bit data xor'd with LENGTH of DATA
#define kPGR_GDataKey2      0xF9,0xB3,0x87,0x2E,0x17,0x75,0xBB,0x98,0xBF,0x3E,0x71              // Rotation Data for 16-bit value (xor'd with LENGTH of DATA)
#define kPGR_GDataKey3      0xBF34,0x19e3,0x8531,0xC4E1,0x3132,0xE5de,0x8A95,0x0843,0xB143      // 16-Bit Data.  Data is exchanged and xored with this value (xor'd with LENGTH of DATA)
#define kPGR_GDataKey4      5,8,11,6,12,9,11,13,4,8,3,10,7,12,4,8,9,3                           // Rotation Data for 16-bit Value AFTER 16-bit XOR(xor'd with LENGTH of DATA)

#define kPGR_EncryptSeed    0x238402d3

class CReadPGR;
struct stImageDataStruct_t
{
    int iLocation;
    int iWidth;
    int iHeight;
    CReadPGR    * cPGR;
};
struct stMultiKeyData_t
{
    stImageDataStruct_t     stGeneric;
    stImageDataStruct_t     stLarge;
    stImageDataStruct_t     stMedium;
    stImageDataStruct_t     stSmall;
    stImageDataStruct_t     stWork;
};


typedef struct
{
    unsigned int    ulCRC;
    unsigned int    ulVersion;
    unsigned int    ulKeyLocation;      // Location of Key Data 
    unsigned int    ulStringTableLocation;
    unsigned int    ulFileDataLocation;
    unsigned int    ulNumKeys;
    unsigned int    ulNumFiles;
    unsigned int    ulKeyLength;
    unsigned int    ulStringTableLength;
    unsigned int    ulFileDataLength;
    unsigned int    ulDirectoryLocation;
	unsigned int	ulHeaderCRC;

    
} stPGRHeader_t;

typedef struct
{
    unsigned int    ulFileNamePointer;
    unsigned int    ulFileSize;
    unsigned int    ulFilePointer;

} stPGRFileDirectory_t;

typedef struct
{
    unsigned int    ulKeynamePointer;
    unsigned int    ulKeyValuePointer;

} stPGRKeyTable_t;

typedef enum
{
    ePGR_OK,
	ePGR_NotFound,
	ePGR_NullObject,
    ePGR_MemoryAllocation,
    ePGR_FileNotFound,
    ePGR_OpenError,
    ePGR_FileNotOpen,
    ePGR_ReadError,
    ePGR_BadCRC,
    ePGR_Bad,            // Useful for pre-setting conditions 


} ePGR_t;
class CReadPGR
{
   // Version 2 header, containing memory length and Checksum
   struct stPGR2Header
    {
        char   sHeader[4];      // PGRX
        DWORD  dwVersion;
        int    iSize;
        int    iCheckSum; 
    };
public:
    int                       m_iLastJPEGWidth		;
    int                       m_iLastJPEGHeight		;

    stPGRHeader_t             m_stPGRHeader;
    stPGRFileDirectory_t    * m_stPGRFileDirectory;
    stPGRKeyTable_t         * m_stPGRKeyTable;
    char                    * m_sStringTable;
    FILE                    * m_fFile;
    char                    * m_sFile;
    int                       m_iLastKeyUsed;
    char                    * FindKey(const char * sKey,const char * sSubKey = NULL);
    char                    * FindKeyCont(char * sKey,char * sSubKey = NULL);
    int                       FindPartialKey(char * sKey);
    int                       GetFileLocation(char * sFile,int * iFileSize = NULL);
    int                       ReadPGRJpeg(int & iWidth,int & iHeight,int iFileLocation,unsigned char * * sReturnMemory);

    int                       GetJpegInfo(int iFileLocation,int & iWidth,int & iHeight);
    int                       GetFileSizeByLocation(int iFileLocation);
    int                       m_bFileOpen;
    int                       m_iFileStart = 0;

     // Storage used for outside processes, but not strictly part of a PGR, when it is expanded into other areas.

    char                    * m_sAuthNames;     // Used to save AuthNames for later comparison  $$ Remove (I think)
    int                       m_bIsParent; 
    int                       m_iNumImages;     // Number of images or panoramas found in the PGR
    CReadPGR                * m_cParent;        // Parent PGR, if there is one

  


    CReadPGR(char * sFile);
    virtual ~CReadPGR(void);
    ePGR_t ReadFile(void);
    void    Main(void);
    void CloseFile(void);
    ePGR_t Read(unsigned char * sDest,int iLength);
    ePGR_t ReadandClose(unsigned char * sDest,int iLength) { ePGR_t eRetValue = Read(sDest,iLength); CloseFile(); return(eRetValue); }
    ePGR_t Seek(int iFileLocation);
    virtual size_t fread(void * pSource,size_t iWidth,size_t iLength,FILE * fFile);
    virtual FILE * fopen(const char * sFile,const char * sControl);
    virtual int fclose(FILE * fFile);
    virtual int fseek(FILE * fFile,long iLocation,int iStart);
    virtual long filelength(int iFile);


	void GetLastJpegDimensions(int & iWidth,int & iHeight) { iWidth = m_iLastJPEGWidth; iHeight = m_iLastJPEGHeight; }
};

class CMemPGR : public CReadPGR
{
public:
    unsigned char * m_sPGRLocation;
    int             m_iCurrentPointer;
    unsigned int    m_iFileSize;

    CMemPGR(unsigned char * sLocation,int iLength);
	~CMemPGR() {  m_fFile = nullptr; }
    size_t fread(void * pSource,size_t iWidth,size_t iLength,FILE * fFile);
    FILE * fopen(const char * sFile,const char * sControl);
    int fclose(FILE * fFile);
    int fseek(FILE * fFile,long iLocation,int iStart);
    int ReadJpegData(void * sBuffer,unsigned int iWidth,unsigned int iBufferSize);
    long filelength(int iFile);


};
class CSagePGR
{
private:

    // Version 2 header, containing memory length and Checksum
   struct stPGR2Header
    {
        char   sHeader[4];      // PGRX
        DWORD  dwVersion;
        int    iSize;
        int    iCheckSum; 
    };

   bool m_bVersion2 = false;        // Uses new header when true, which must be handled as the memory address is wrong due to 
                                    // adjustment in code

	CReadPGR	* m_cPGR = nullptr;

	void DeletePGR();
    bool m_bInvalid = true;
public:
	CSagePGR() { m_bInvalid = false;};
    CSagePGR(const char * sPath);
    CSagePGR(const unsigned char * sPgrData);

    bool isValid() { return !m_bInvalid; };
	ePGR_t ReadPGR(const char * sFile);
	ePGR_t ReadMemPGR(const unsigned char * sLocation,int iLength = 0);
	const char * FindKey(const char * sKey,const char * sSubKey = nullptr);
	const char * ReadText(const char * sKey,const char * sSubKey = nullptr);
	bool ReadText(CString & cString,const char * sKey,const char * sSubKey = nullptr);
	ePGR_t ReadSize(SIZE & szSize,const char * sKey,const char * sSubKey = nullptr);
	SIZE ReadSize(const char * sKey,bool * bSuccess = nullptr);
	ePGR_t ReadPoint(POINT & szPoint,const char * sKey,const char * sSubKey = nullptr);
	POINT ReadPoint(const char * sKey,bool * bSuccess = nullptr);
	Sage::RGBColor_t ReadRGB(const char * sKey,bool * bSuccess = nullptr);
	ePGR_t ReadRGB(Sage::RGBColor_t & rgbColor,const char * sKey,const char * sSubKey = nullptr);
	ePGR_t ReadRGB(Sage::RGBColor_t & rgbColor,Sage::RGBColor_t  rgbDefault,const char * sKey,const char * sSubKey = nullptr);
	unsigned char * ReadRawFile(const char * sFile,int & iFilesize);
	unsigned char * ReadFile(const char * sFile,int & iFilesize);
	unsigned char * ReadFile(const char * sTopKey,const char * sFile,int & iFilesize,bool bRawFile = false);
	[[nodiscard]] Sage::RawBitmap_t ReadRawBitmap(const char * sFile);
	[[nodiscard]] Sage::RawBitmap_t ReadRawBitmap(const char * sTopKey,const char * sFile);
	CSageBitmap ReadBitmap(const char * sFile);
	CSageBitmap ReadBitmap(const char * sTopKey,const char * sFile);
	bool FileExists(const char * sTopKey,const char * sFile);
	bool FileExists(const char * sFile);
	ePGR_t ReadInt(int * iValue,const char * sKey,const char * sSubKey = nullptr); 
//	ePGR_t ReadInt(int & iValue,int iDefault,char * sKey,char * sSubKey = nullptr); 
	int ReadInt(int iDefault,const char * sKey,const char * sSubKey = nullptr); 
	int ReadInt(const char * sKey,const char * sSubKey = nullptr); 
	ePGR_t ReadBool(bool * bValue,const char * sKey,const char * sSubKey = nullptr); 
	bool ReadBool(bool bDefault,const char * sKey,const char * sSubKey = nullptr);
	ePGR_t ReadFloat(double * fValue,const char * sKey,const char * sSubKey = nullptr); 
	double ReadFloat(double fDefault,const char * sKey,const char * sSubKey = nullptr);
	static void FreeFileMem(unsigned char * & sFile);
	~CSagePGR();
};

}; // namespace Sage
#endif