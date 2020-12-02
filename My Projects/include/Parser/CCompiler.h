//#pragma once
#if !defined(CCOMPILER_H)
#define CCOMPILER_H

#include <process.h>

#include "Parser1.h"
//#include "Code ExplorerDoc.h"
//#include "Code ExplorerView.h"
//#include "Webdriver.h"
//#include "StatusWindow.h"
//#include "CPasWindow.h"
//#include "CCompLink.h"
#define davCOLOR_MENU		1
namespace CSageScript{

class CCompLink;
class CCompiler
{
public:
	CCompLink * m_cCompLink = nullptr;
	char	m_sFilename[300];
	char *	m_sMainFilename;			
	int		m_bLab;						// True signifies in Lab mode
	CCodeBlock * cCodeBlock;			// Main CodeBlock
	CCodeGen * cCodeGen;				// Code Generation routines
	unsigned char * sStack = nullptr ;				// Pointer to main stack
	CCPUBlock * cCpuBlock;				// Cpu Descriptor
	unsigned short * sRegisterStack = nullptr;	// Stack for register stores during calls (I think)
	CError * cMainError;				// The Main CError between calls, etc. (### Should be removed)
	CWebDriver * cWebDriver;			// Realistically, the Relaxed-Pascal-specific definitions
//	CStatusWindow * m_cStatusWindow;	// Status Window for Status Messages. Can be NULL.
//	CPasWindow * m_cUserWindow;			// User's Window -- The main display window
	int	m_iLabCodeStartAddr;			// Start address to set for all LAB window calls
//	CCodeGen	m_cCodeGen;

//	int Init(CStatusWindow * cStatus, CPasWindow * cUserWindow,int bLab);
	int Init(int bLab);
	void SetCompileFilename(char * sFilename);
	int Compile(int iRun = 1,const char * sInput = nullptr);
	void ShutDown();
	void WriteStatusMessage(char * sMessage,int iType = -1);
	void ReportErrorMsg(int iLineNo, int iColumn, char * sMessage);
	char * sSourceLine;
	char sSourceLine2[2000];
	int Run();
	char sRunReturnMsg[200];
	CStringOverload * cStringOverload;
	CCharOverloads * cCharOverloads;
	CIntOverloads * cIntOverloads;
	CFloatOverloads * cFloatOverloads;
	void InitOverloads(CCodeBlock * cCodeBase);
	int RunLab(const char * sRun);
	void SetCompLink(CCompLink * cCompLink)
	{
		m_cCompLink = cCompLink;
	}

	~CCompiler();

};



class CVars
{
	stVARSTRUCT * stSave;
	int iNumAutoGlobals; 
public:
	void FindTail();
	stVARDATASPACE DataSpace;
	CVars * stPreviousCvar;
	CVars * cGlobals;
	CVarTypeBase * cVarTypeBase; 
	stVARSTRUCT * stOrg;
	int iNumVars; 
	stVARSTRUCT * sMainVar; 
	enum eVARTYPES ConvertType(enum TokenType eToken,int iPointerDepth,int bUnsigned); 
	stVARSTRUCT * stCurrent;
	CVars(CVarTypeBase * cVarTypeBase,stVARDATASPACE stDataSpace,CVars * stPrevious = NULL);
	~CVars();

	stVARSTRUCT * FindVar(char * sVarName); 

	stVARSTRUCT * AddVar(char * sName,char * sOrgName,int iTypeIndex=0,int iArraySize = 0,int iPointerDepth=NULL,
											int iDecLineNo=NULL,stCODEBLOCK * stCodeBlock=NULL);
	
	
	void FillVarData(int iTypeIndex,int iArraySize,int iPointerDepth,int iDecLineNo,
																		stCODEBLOCK * stCodeBlock=NULL);

	static stVARSTRUCT * DeclareGlobalString(CVars * cVars,char * sString); 

	void SaveCurrent() { stSave = stCurrent; } ;
	void RestoreCurrent() { stCurrent = stSave; };
	void DeleteVars(int iNumtoDelete);
	int GetVarSize(enum eVARTYPES eVarType,int iStackBased);

};

}; // CSageScript
#endif