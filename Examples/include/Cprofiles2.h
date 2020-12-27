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

#if !defined(__CProfiles2_H__)
#define __CProfiles2_H__
#include "CSageBox.h"



typedef struct
{
	char * sWindowTitle;
	char * sFileExtension;
	char * sFileVerify;
} stUserProfileTable_t;

typedef struct
{
	int iNumPoints;
	POINT pPoints[1024];
} stProfileIntegerPairs_t;

typedef struct
{
	int iFileSize;
	char sFilename[1025];
	unsigned char * sFileData;
	unsigned int uiCheckCRC;
} stProfileFileData_t;

#define kMaxProfileFiles		50
#define kMaxProfileFileSize		(50*1024*1024)	// Needs to be large to support embeded masks, etc. 
typedef enum
{
	eEventNone,
	eEventCancel,
	eEventLoad,
	eEventSave,
} eUserProfileInput_t;

typedef struct
{
	int iWidth;
	int iHeight;
	int bBmp;					// Designates that the entire BMP file is in the memory location and that the data starts at +54
	char sFilename[200];
	unsigned char * sFileData;

} stProfileHtmlImages_t;

typedef struct
{
	int						  m_iNumHtmlImages		;
	unsigned char			* m_sSampleJpeg			;
	unsigned char			* m_sHtmlFile			;
	int						  m_iSampleJpegWidth	;
	int						  m_iSampleJpegHeight	;
	int						  m_iHtmlFileLength		;
	stProfileHtmlImages_t	  m_stHtmlImages[100]	;
	int						  m_iHelpImageStart		;
	int						  m_iHtmlType			;
} stProfileTransferPackage_t;
class CProfiles
{
public:
	enum class Status
	{
		Ok			= 0			,
		FileNotFound			,
		FileCorrupted			,
		MemoryError				,
		FileTooLarge			,
		FormatError				,
		TagNotFound				,
		FileTypeNotFound		,
		OutputNotInitialized	,
		CouldntOpenOutputFile	,
		NoFilename				,
		NoObject				,

	};
public:
	typedef struct
	{
		char * sData;
		int				iLength;
	} stControlRawData_t;

private:
	char					* m_sOutputFileData	= nullptr;
	int						  m_iOutputFileSize = 0;
	char					* m_sOutputFilename		;
	int						  m_bOutputFile			;
	unsigned char			* m_sFileMemory			;
	int						  m_iFileExpectedLength	;
	int						  m_iFileLength			;
	unsigned int			  m_uiFileCRC			;

	int						  m_bReturnCancel		;
	int						  m_iHtmlType			;
	int						  m_bTransferredPackage ;
	int						  m_iNumHtmlImages		;

	int						  m_iSampleJpegWidth	;
	int						  m_iSampleJpegHeight	;
	int						  m_iHtmlFileLength		;
	int						  m_bForceSaveEvent;
	int						  m_bFileExists;
	char					  m_sTempString[10001];
//	char					  m_sOutputFile[MAX_PATH+2049];
	int						  m_iOutputBuffer;
	char					* m_sOutputBuffers[2];
//	char					* m_sOutTemplate;

	char					  m_sDirFilename[MAX_PATH+2048];
	int						  m_iNumFiles;
	stProfileFileData_t		  m_stFileData[kMaxProfileFiles];
	char					  m_sFileType[1025];
	char					  m_sTag[10001];
	char					  m_sErrorMessage[1024];
	char					  m_sFile[MAX_PATH+2048];
	unsigned char			* m_sProfileMemory;
	int						  m_iProfileLength;
	Status			  SkipWhiteSpace(unsigned char * & sSource);
	Status			  InsertString(unsigned char * & sSource,unsigned char * & sDest);
	Status			  SkipOneLineComment(unsigned char * & sSource);
	Status			  SkipCommentBody(unsigned char * & sSource);

	Status			  CleanImage(unsigned char * sFileMemory,int iCurrentSize, int & iNewSize);
	Status			  ParseLines(unsigned char * sFileMemory,int iCurrentSize, int & iNewSize);

	eUserProfileInput_t		  HandleLoadProfile(int bReturnCancel = 0);
	eUserProfileInput_t		  HandleSaveProfile(int bReturnCancel = 0);
	int						  DeleteUserProfileWindow();
	bool						  CreateOutputFileMemory(unsigned char * sInMemory,int iInMemorySize);
	int GetTagString(char * sTag,char * & sTagReturn);
	int GetTagBoolean(char * sTag,int bDefault);
	int GetTagInteger(char * sTag,int bDefault);
	double GetTagFloat(char * sTag,double fDefault);
	int						  PutTagString(char * sTag,char * sString);
	int						  PutTag(char * sTag,char * sString);
	int						  PutTagFloat(char * sTag,double fValue);
	int						  PutTagInteger(char * sTag,int iValue);
	int						  PutTagBoolean(char * sTag,int bValue);

public:

	char 			  m_sAuthor[1025]				;
	char 			  m_sDate[1025]					;
	char 			  m_sFileVersion[1025]			;
	char 			  m_sWebsite[1025]				;
	char 			  m_sEmail[1025]				;
	char 			  m_sTitle[10001]				;
	char 			  m_sDescription[10001]			;

	CProfiles();
	~CProfiles();
	Status ReadFile(char * sFile = NULL,int iMemLength = 0);
	Status ReadMemoryFile(char * sFile,int iMemLength);
	CString GetString(const char * sTag,const char * sDefault = nullptr);
	CfPoint GetCfPoint(const char * sTag,CfPoint & cfDefault = CfPoint{});

	int GetBool(const char * sTag,bool bDefault = false);
	int GetInteger(const char * sTag,int iDefault = 0);
	double GetFloat(const char * sTag,double fDefault = 0);
//	int	TransferExtraFileData(stProfileTransferPackage_t & stFileData);
	int GetTagPointer(char * sTag,char * & sTagReturn,char * sStartLocation = NULL);
	int GetDirPointer(char * sTag,char * & sTagReturn,char * sStartLocation = NULL);
	int CopyTag(char * sTag,char * sTagReturn,int iMaxLen = 400);
	int CopyTagString(char * sTag,char * sTagReturn,int iMaxLen = 10000);
	int CopyTagIntegerPairs(char * sTag,stProfileIntegerPairs_t & stTagReturn,int iMaxPairs = 1024);
	int GetTagTriState(char * sTag,char * sState1,char * sState2,char * sState3,int iDefault);
	int GetTagQuadState(char * sTag,char * sState1,char * sState2,char * sState3,char * sState4,int iDefault);
	int GetTagBooleanString(char * sTag,char * sState1,char * sState2,int iDefault);

	bool	  PutString(const char * sTag,char * sString);
	bool	  PutText(const char * sTag,char * sText);
	bool	  PutFloat(const char * sTag,double fValue);
	bool	  PutInteger(const char * sTag,int iValue);
	bool	  PutBool(const char * sTag,bool bValue);
	bool	  PutCfPoint(const char * sTag,CfPoint cPoint);


	char * GetVerifyName(int iVerify);
	eUserProfileInput_t HandleMenuInput();
	char			* GetErrorCodeMessage(Status eErrorCode);
	Status VerifyFileType(char * sFileType);
	int						  ForceSaveEvent();
	int						  GetProfileFilename();
	int						  GetProfileSaveFilename();
	Status InitOutput(const char * sTemplate,int iTemplateLength = 0);
	Status WriteProfile(const char * sFile);
	int						  PutTagTriState(char * sTag,char * sString1,char * sString2,char * sString3,int iValue);
	int						  PutTagQuadState(char * sTag,char * sString1,char * sString2,char * sString3,char * sString4,int iValue);
	int						  PutTagBoolString(char * sTag,char * sString1,char * sString2,int iValue);
	int						  PutTagIntegerPairs(char * sTag,stProfileIntegerPairs_t & stIntegerPair);
	int						  FileExists();
	static int						  DeleteExtraFileData(stProfileTransferPackage_t & stFileData);
	int						  SetProfileName(char * sFile);
	void					  SetProfileMemory(unsigned char * sProfileMemory);
	bool					  AddDataFile(const char * sFilename, void * sFileMemory,int iFileLength);
	[[nodiscard]] Mem<char>				  GetFileData(const char * sFilename,bool * bSuccess = nullptr);


};

#endif