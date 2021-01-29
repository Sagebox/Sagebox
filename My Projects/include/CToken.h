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
#if !defined(_CToken_h_)
#define _CToken_h_

#include <cstdio>
#include <stdlib.h>
#include <memory>
//#pragma warning( disable : 4996) 

namespace Sage
{
class CCompiler;
class CTree;
class CFunc;
extern int gMessagesOff;

#define FALSE   0
#define TRUE    1
const int TokenInsert_File   = 1;
const int TokenInsert_Define = 2;

struct stTOKENOPERATORLOOKUP
{ 
	char * sToken;
	enum	TokenType eToken;
}; 
struct TokenInsert_t
{
	TokenInsert_t * stNext;
	TokenInsert_t * stPrevious;

	int iOldLineNumber;
	int iOldLinePosition;
	char * sAlloc;
	int		iType;		// 1 = file, 2 = macro/define
	FILE * sFile;		// InsertFilename
	char * sOldCurrentPlace;


};
// First char represent case-sensitive code:
// ! = not-case sensitive		-- Example = "begin" or "BEGIN" for pascal
// * = case-sensitive			-- Example = "AND", "OR", "NOT" for pascal


struct MultiWordTokens_t
{
	enum TokenType eSourceToken; 
	enum TokenType eDestToken; 
	char * sString;
};


// Table for fast operator determination.  i.e. any non-alphanumeric,non-whitespace

static char sOperatorTable[128] = 
{ 
	00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
	00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
	00, 00,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
	00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 26, 15, 16, 17, 18, 00,
	01, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
	00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 19, 00, 20, 21, 00,
	00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
	00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 22, 23, 24, 25, 00 };

// defines for character type determination
static char sAlphaNumericTable[128] = 
{ 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
	0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1,
	0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 };
#define isAlphaNumeric(_x) sAlphaNumericTable[_x]
#define isOperator(_x) sOperatorTable[_x]

// All node types in the binary tree

enum eRAWNUMTYPE
{
	__numtypSIGNED      = 0x0000,
    __numtypUNSIGNED    = 0x1000,
	numtypInt			= 0x0000       + __numtypSIGNED  ,
	numtypLong			= 0x0001       + __numtypSIGNED  ,
	numtypShort			= 0x0002       + __numtypSIGNED  ,
	numtypUInt			= numtypInt    + __numtypUNSIGNED,
	numtypULong			= numtypLong   + __numtypUNSIGNED,
	numtypeUShort		= numtypShort  + __numtypUNSIGNED,
	numtypBad			= -1
};



enum eVARTYPES
{
	vtNULL = 0,
	vt8bit=0,				// Char, Byte
	vt16bit,				// Short, Word 
	vt32bit,				// Int, long, etc.
	vt64bit,				// Currently not supported
	vttSigned	 = 0x0000,	// i.e. int, char, long (default)
	vttUnsigned  = 0x0100,	// i.e. unsigned int, unsigned char, unsigned short, etc. 
	vttPointer   = 0x1000,	// i.e. int *, long *, char * (redundant here, in a way)
	vttReference = 0x2000,	// i.e. int & (not supported yet)
	vttAny       = 0x4000,  // Used for undefined vars to satisfy warnings. (may not be used). Might
							// just rely on bPhantom instead. 
};
enum eNODETYPE
{
	nNULL=0,
	nNum,
	nFloat,
	nVar,
	nFunc,
	nAdd,
	nSub,
	nMul,
	nDiv,
	nAnd,
	nOr,
	nlogAnd,
	nlogOr,
	nxor,
	nMod,
	nneg,
	nAssign,
	nPtr,
	nTakeAddress,
	nNot,
	nEqualto,
	nGreaterThan,
	nLessThan,
	nPlusEqual,
	nNotEqualto,
	nlparen,
	nrparen,

	// Float subsitutions... 

	nFadd,
	nFsub,
	nFmul,
	nFdiv,
	nFneg,
	nFGreaterThan,

	nClass,

	nSubExp,	// This node was a sub-expression as was already calculated (see stNodeData->eOldNodeType)
	nUsed,		// Already Used, i.e. modified (see stNodeData->eOldNodeType)
	nBad,
	nUndef,
};


struct stNUMBER
{
	unsigned long	LSW;
	unsigned long	MSW;
	enum eRAWNUMTYPE eRawNumType;
	float fFloat;

};
struct stTOKENDATA_NUMBER
{
	stNUMBER stNumber;
	eRAWNUMTYPE eRawNumType;
};
struct stCODEBLOCK
{
	unsigned char * sBlock;
};
struct stARRAYCONFIG
{
	int	iDepth; 
	int iStart;			// For Pascal.  For C, this is always 0. 
						// But, for pascal, we can have Array 25..30 of Array 50..60, etc, 
						// For string, this is 256,0. 
	stARRAYCONFIG * stPrevious;
	stARRAYCONFIG * stNext; 
};
struct stVARSTRUCT
{
	stVARSTRUCT * stNextVar;
	stVARSTRUCT * stPreviousVar;

	char sName[100];	// Will malloc this from a pool later
	char sOrgName[100];	// Same Here
	char * spName;		// Pointer to the processed name for later compatibility with changes
						// i.e. int iMyVar potentially gets converted to $$global_iMyVar; ## not sure yet
	char * spOrgName;	// The original name, i.e. case-sensitive, for error-/warning-reporting purposes
	enum eVARTYPES eBaseType;
	int iPointerDepth;	// if it's a pointer, what depth? i.e. int * * iMyPointer (depth = 2)
						// 0 is default for no point, making vttPointer somewhat redundant. 
	int bPhantom;		// Have we assigned it even though it doesn't exist? (i.e. to shut off warnings)
	int iProperType;	// Pointer to type tables, such as int, word, or user-defined typedef. 
	int iDecLineNo;		// Line it was declared on (for warning/error purposes)
	stCODEBLOCK * stCodeBlock;	// If non-zero, this is initial codeblock to set the variable. 
	int iStackBased;
	int iAddress;
	int iTypeIndex;
	int iTotalSize;		// TotalSize, i.e. typedef int myInt_t[500]; and then a myInt_t myInt[20]
						// would be sizeof(int)*500*20; 
	int bReference;		// Is this a reference... 

	stARRAYCONFIG *	stArrayConfig;			// Totalsize of var. 
};
struct TempClass_t
{
	TempClass_t * stPrevious; 
	TempClass_t * stNext; 
	char sClassName[100];
	char sOrgClassName[100];
	char * spOrgName; 
	char * spName;

};

class CTempClass
{
public:
	TempClass_t * stCurrent;
	TempClass_t * stOrg; 
	int iNumClasses;

	CTempClass() { stOrg = stCurrent = NULL; iNumClasses = 0;};
    void AddClass(char * sClassName); 
	TempClass_t * FindClass(char * sClasName);
};
union suTOKENDATA
{
	stTOKENDATA_NUMBER stNumber;
//	stTOKENDATA_VAR stVar;
	stVARSTRUCT * stVar;
	CFunc * cFunc;
	TempClass_t * stClass;
//	CTree * cAnyTree;
};


enum TokenType
{
	tNULL=0, 
	tAdd, 
	tMul, 
	tSub, 
 	tDiv, 
	tAnd, 
	tOr, 
	tVar,
	tFunc,
	tNum, 
	tFloat,
	tlogAnd, 
	tlogOr, 
	txor,
	tMod,
	tneg,
	tlparen, 
	trparen, 
	tLineDelimit,
	tVarDelim,	// Only used with Pascal, I think, i.e. var : Integer;
	tDelimExpr,
	tAssign,
	tPtr,
	tTakeAddress,
// Comparison operator tokens

	tNot,
	tEqualto,
	tGreaterThan,
	tLessThan,
	tPlusEqual,
	tNotEqualto,
	tLessThanEqual,
	tGreaterThanEqual,
	tAddr,
	tComma,
	tColon,
	tClass,
	tEOF,
	tBad,

	tInt,
	tWord,
	tLong,
	tChar,
	tUnsigned,
	
// Web-driver tokens

	tVarDecl,
	tProcedureDecl,
	tFunctionDecl,
	tOverload,
	tThreadDecl,
	tBegin,
	tReturn,
	tIf,
	tThen,
	tElse,
	tFor,
	tRepeat,
	tUntil,
	tTo,
	tDo,
	tStep,
	tStopCompile,
	tEnd,

	tInclude,		// Differs from Relaxed Pascal to Pascal or C.. i.e. C= #include (not sure what
					// Pascal Does), RPascal is just "include". 


	tBreakCompile,
	tLiteral,
	tStopCalc,			// Used to stop the parser cold. 

	tTypeArithOps = 0x1000,
	tTypeLogicalOps,
	tTypeValueTokens,
	tTypeUnaryPrefixOps,
};
const struct MultiWordTokens_t stMultiWordTokens[] = 
{
	tOr,tlogOr,"IF",
	tAnd,tlogAnd,"IF",
	tNULL,tNULL,NULL
};
const struct stTOKENOPERATORLOOKUP stTokenAlphaLookup[] = 
{
	"!VAR",tVarDecl,
	"!VARS",tVarDecl,
	"!PROCEDURE",tFunctionDecl,
	"!OVERLOAD",tOverload,
	"!THREAD",tThreadDecl,
	"!FUNCTION",tFunctionDecl,
	"!STOPCOMPILE",tStopCompile,
	"!BREAKCOMPILE",tBreakCompile,
	"!BEGIN",tBegin,
	"!RETURN",tReturn,
	"!DIV",tDiv,
	"!FOR",tFor,
	"!REPEAT",tRepeat,
	"!UNTIL",tUntil,
	"!TO",tTo,
	"!DO",tDo,
	"!IF",tIf,
	"!THEN",tThen,
	"!ELSE",tElse,
	"!STEP",tStep,
	"!END",tEnd,
	"!STOPCALC",tStopCalc,
	"!INTEGER",tInt,
	"!INT",tInt,
	"!LONG",tLong,
	"!UNSIGNED",tUnsigned,
	"!CHAR",tChar,
	"!BEGIN",tBegin,
	"!MOD",tMod,
	"!NOT",tNot,
	"!END",tEnd,
	"!INCLUDE",tInclude,
	"!OR",tOr,
	"!AND",tAnd,
	"!ORIF",tlogOr,
	"!ANDIF",tlogAnd,
	NULL,tNULL
};

const struct stTOKENOPERATORLOOKUP stTokenOperatorLookup[] = {
	"-",tSub,
	"+",tAdd,
	"*",tMul,
 	"/",tDiv,
	"&",tAnd,
	"&&",tlogAnd,
	"||",tlogOr,
	"^",txor,
	"%",tMod,
	"(",tlparen,
	")",trparen,
	"-",tneg,
	"{",tBegin,
	"}",tEnd,
	":=",tAssign,
	":",tVarDelim,
	";",tDelimExpr,
//	"=",tAssign,
	"==",tEqualto,
	"!=",tNotEqualto,
	"!",tNot,
	"<>",tNotEqualto,
	"<",tLessThan,
	">",tGreaterThan ,
	"<=",tLessThanEqual,
	">=",tGreaterThanEqual,
	"@",tTakeAddress,

	",",tComma
};
struct stNODEDATA
{
	eNODETYPE eNodeType;
	enum TokenType	eToken;		// Finally, the literal token, i.e. <add><sub><pointer>, etc. 
	union suTOKENDATA suTokenData;			//
	int iNodePriority;
	int bUnaryType;
	eNODETYPE ePreviousNodeType;	// For use when the tree changes. 
	int iLineNumber;
	int iLinePosition; 
	int iLValue;
	int iType;		// User-type, if any 
	eVARTYPES eBaseType;		// Base type, i.e. char, int, etc. 
	int iPointerDepth; 
	int bReference;
	CTree * cAnyTree;
	char * sLiteral;
};
struct stNODE
{
	stNODE * tLeft;
	stNODE * tRight;
	stNODE * tParent;
	stNODE * tKey;		// lower-most l-value
	stNODEDATA * stNodeData;
	int iValueType;			// i.e. int, unsigned, etc.
	int iData;
	stNODE * stPreviousNode;
	stNODE * stNextNode;
	int bEndExpr;		// Used to recognize top-nodes of sub-expressions
	//int iPriority;
};

enum t_VarDisplacement
{
	vdGlobal,			// Exact Address
	vdFuncInit,			// bp+Address
	vdAuto,				// bp-Address-varsize
};

struct stVARDATASPACE
{
	int iMaxSize;
	int	iCurrentAddr;
	enum t_VarDisplacement eVarDisplacement;
	unsigned char * sDataSpace;
};
struct stVARNAMELIST
{
	char sName[50];
	char * spName;
	stVARNAMELIST * stPrevious;
	stVARNAMELIST * stNext;

};
struct stARRAYDATA
{
	int	iArrayDepth; 
	stARRAYCONFIG stArrayConfig; 
};

struct stVARTYPEDATA
{
	int		iTypeNo;			/* This is the number of the type.  This can mean user-defined types, but 
	                               the strategy here is to add the basic types in the same list as a      
	                               sort-of cross-reference, with an extended name-list, i.e. "int",       
							       "Integer", etc. for specific type-casting, i.e. ip1 = (int *) 5        
	                               will check the list for a specific match, rather than a base-type     */
	bool	iBase;				/* Yes if this is a basic base-type that converts to basic int regardless 
							       this can be true even if it user-defined, i.e. typedef int myint;     */
	int		iWidth;				// Width is width in bits
	bool	bSigned;			// true signifies this is a signed type
	int		iPointerDepth;		// 0 signifies this is not a pointer
	int		iTotalSize;			// Total Size of type (including array depths added)
	int		bStrict;			// Strict usage?  i.e. good for consts and enums and such that have
								// generic names. 
	stARRAYDATA stArrayData;	// NULL signifies it's not an array.
};
class CCodeBlock;
class COverload
{
public:
	CCodeBlock * cCodeBlock;
	int	iOverloadIndex;
	stNODE * stNode;
	virtual stNODE * OverloadFunction(stNODE * stNode,int iOverloadIndex) = 0;
	stNODE * OverloadNode(stNODE * stNode,char * sExpression,bool bImplicit); 
	COverload(CCodeBlock * cCodeBlock) { this->cCodeBlock = cCodeBlock; }
};

struct stOVERLOADS
{
	int iSourcePointerDepth;	// Tricky; this is the pointer depth for the source. For instance, if
								// we're overloading int + float, depth = 0; int * + float = 1
	int	iOverloadType;			// The incoming type (i.e. right node)
	int	iOverloadPointerDepth;	
	int iReturnType;			// The type that gets returned. 
	int iReturnPointerDepth;
	eNODETYPE eOpNodeType;		// Operation type that is overloaded
	stOVERLOADS * stNext;
	stOVERLOADS * stPrevious; 
	COverload * * cOverload;
	int iNumOverload;
};


struct stVARTYPE
{
	int iNames;
	stVARNAMELIST stVarNameList;	// Usually just one, so no reason to malloc the first. 
	stVARTYPEDATA stVarTypeData;
	stOVERLOADS * stOverloads;
	stOVERLOADS * stCurOverload;
	int iNumOverloads; 
	stVARTYPE   * stPrevious; 
	stVARTYPE   * stNext; 
};
class CVarTypeBase
{

	bool bCaseSensitive;			// false for pascal, true for c
	stVARTYPE * stMainBase;			// Start of the vartype list
	int iNumVarTypes;				// Number of types in the list
	stVARTYPE * stCurrent;
public:
	CVarTypeBase(bool bCaseSensitive);
	int AddVarType(char * sVarName,int iWidth,bool bSigned,int iPointerDepth,stARRAYDATA * stArrayData,
		int bStrict=0);
	stVARTYPE * GetVarType(int iIndex);
	int isVarType(char * sVarString); 
	int AddOverload(int iSourceType,int iSourcePointerDepth,eNODETYPE eNodeType, char * sType,char * sReturnType,COverload * * cOverload);

	int CheckTypes(int iFuncType,int iFuncPointerDepth, int iFuncReference, CTree * cFuncTree);
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

	static stVARSTRUCT * FindVar(CVars * cThis,char * sVarName); 

	void AddVar(char * sName,char * sOrgName,int iTypeIndex=0,int iArraySize = 0,int iPointerDepth=NULL,
											int iDecLineNo=NULL,stCODEBLOCK * stCodeBlock=NULL);
	
	
	void FillVarData(int iTypeIndex,int iArraySize,int iPointerDepth,int iDecLineNo,
																		stCODEBLOCK * stCodeBlock=NULL);

	static stVARSTRUCT * DeclareGlobalString(CVars * cThis,char * sString); 

	void SaveCurrent() { stSave = stCurrent; } ;
	void RestoreCurrent() { stCurrent = stSave; };
	void DeleteVars(int iNumtoDelete);
	int GetVarSize(enum eVARTYPES eVarType,int iStackBased);

};
class CFunc
{
public:
	stVARSTRUCT * stBaseVar;
	CFunc * cPrevious;
	CFunc * cNext;
	char sOrgName[100];	// a little kludgy, for now
	char sName[100];	// ditto
	char * spOrgName;
	char * spName;
	CVars * cVar;		// Vars in this function.  Should link back to global as well as into blocks. 
// CBlocks * CBlocks;	// Something with blocks... don't really know yet
	int	iNumParms;		// Number of parameter vars. 
	int	iStartAddress;	// Start address of this function in the main codeblock
	int ibpAddr;	// Amount to add to bp
	int ispAddr;	// Amount to subtract off of sp
	int iReturnType;
	int iReturnDepth;
	int bIsThread;
	stNODE * stInitNodes;		// Initilization trees for values that aren't entered. 
	int bVarAddressable;
	CFunc(CFunc * stPreviousFunc=NULL);
	CFunc * FindFunc(char * sFunc,CFunc * cFuncBase=NULL);
	CFunc * FindNextFunc();
};

class CTree
{
public:
	int bNodesActive;
	int iTreePriority;		// The priority of the operators of this current tree
	int bPrioritySearch;
	CTree * cPreviousTree;
	CTree * cNextTree;
	CTree(stNODEDATA * stNodeDATA,CTree * cPreviousTree);
	CTree(stNODE * stNode,CTree * cPreviousTree);
//	CTree() { bNodesActive = 0}
	CTree(CTree * cPreviousTree2=NULL);
	void AddNode(int iSide,stNODEDATA * stNodeData);
	void AddParentNode(stNODEDATA * stNodeData); 
	void AddLeftNode(stNODEDATA * stNodeData);
	void AddNodeLeft(stNODEDATA * stNodeData);
	void AddNodeRight(stNODEDATA * stNodeData);
	void AddNodeUnaryLeft(stNODEDATA * stNodeData);
	void AddParentNodeRight(stNODEDATA * stNodeData);
	void AddParentNodeLeft(stNODEDATA * stNodeData);
	void Init(stNODEDATA * stNodeData);
	void GraftLeftNode(CTree * cTree);
	void GraftLeftTree(CTree * cTree);
	void GraftRightNode(CTree * cTree);
	CTree *  GraftLeftoverTrees();
	void PrintTree();

	stNODE * CreateNode();

	stNODE * stMaster;
	stNODE * stCurrent;
	stNODE * stKey;			// Original tree
	void SetNodeData(stNODEDATA * stNodeData) { stCurrent->stNodeData = stNodeData;
	}

private:	
	void SetNodeData(stNODE * stNode,stNODEDATA * stNodeData) 
						{ stNode->stNodeData = stNodeData; 
	stCurrent->tKey = (stCurrent->tLeft) ? stCurrent->tLeft->tKey : stKey; 
	};
};


		
class CToken
{
#define MAX_TOKEN_STRING_SIZE	200
	stNODEDATA sLocalNodeData,sLastNodeData;
	int bReverse;
	stTOKENOPERATORLOOKUP * stAltOperatorLookup,* stAltAlphaLookup;
	int bJustReturnString; 
	char * sOldCurrentPlace;
	int SkipWhiteSpace();
	int GetString();
	int iAltOpSize;
	void PassCommentBlock();
	void PassEOL();
	char * sParseString;
	char sOrgTokenString[MAX_TOKEN_STRING_SIZE];
	int GetNumber(stNUMBER & stNumber);
	int iTempLength;
	int cmpTokenString(char * sToken,char * sSearch);
	CVars * cVar;
	CCodeBlock * cCodeBlock;
	TokenInsert_t * stTokenInsert;
//	char sTokenString[200];
//	stNODEDATA * stNodeData;
public:
	char sTokenString[MAX_TOKEN_STRING_SIZE];		// Anything past this, we have trouble
	void SetAltTokens(stTOKENOPERATORLOOKUP * stAltAlpha,stTOKENOPERATORLOOKUP * stAltOperators,int iAltOpSize); 
	char * sCurrentPlace;
	char * sArray;
	//stNODEDATA * getNodeData() { return stNodeData; } 
	stNODEDATA *  GetLiteralString();
	CToken(char * sString,CVars * cMainVar=NULL);
	int CToken::CheckforEmptyParens(void);
	void Reverse() { bReverse = 1; }
	void SetVars(CVars * cMainVar) { cVar = cMainVar; }
	void SetCodeBlock(CCodeBlock * cCodeBlock) { this->cCodeBlock = cCodeBlock; }
	stNODEDATA * GetToken(int bAllocate=TRUE);
	char * ReturnFoundString() { return sOrgTokenString; };
	char * ReturnFoundUppperString() { return sTokenString; };
	int InsertFile(char * sIncludeFile);
	int iInsertDepth;
	
	//	~CToken() { delete stNodeData; }
};


enum ErrType
{
	ERR_MSG = -2,					// Just want to print a message
	ERR_TEMP = -3,
	ERR_CLEAN_EXIT = - 4,			// Just want to get out of the program cleanly. 
	ERR_IGNORE =-5,					// Already handled.  Still throws down the chain, but gets ignored
	ERR_HANDLED = -6,				// Error has been handled, throws locally but doesn't get transmitted
	ERR_NULL			= 0,
	ERR_BASE			= 0,		// Basic Errors
	ERR_CREATE_NODE,				// Couldn't malloc data for node creation
	ERR_PARENT_EXISTS,				// Tried to create parent node, but parent node exists
	ERR_TOKEN_TOO_LARGE,			// Token field was too long (will probably remove later)

	ERR_TOKEN_BASE      = 0x01000,	// Token Errors
	ERR_TOKEN_EOL,					// Found EOL (Not an Error)
	ERR_TOKEN_EOL_INT,				// Found premature EOL in token search -- definitely an error
	ERR_TOKEN_STRING_EOF,			// Found EOF while searching for a string
	ERR_TOKEN_SQUOTE_UNMATCH,		// Didn't find matching single-quote
	ERR_TOKEN_STRING_EOL,			// Found EOL while scanning string for string
	ERR_TOKEN_UNKNOWN_CHAR,			// Unknown character in token string
	ERR_TOKEN_NOT_NUMBER,			// Non-Numbers in number stream.
	ERR_TOKEN_NOT_BINARY_NUMBER,	// Number declared as binary has bad chars
	ERR_TOKEN_NOT_HEX_NUMBER,		// Number declared as hex has bad chars
	ERR_TOKEN_NUMBER_TOO_LARGE,  	// Entered a number that is too large (need types, L/S/etc)
	ERR_TOKEN_COULDNT_ALLOCATE,		// Memory error
	ERR_TOKEN_BAD_TOKEN,			// Couldn't find a token -- garbage, misspelling,etc.
	ERR_TOKEN_BAD_NODE,				// Couldn't identify Node from token
	ERR_NODE_BAD_NODE,				// Couldn't resolve node type (with valid token)
	ERR_NODE_MISMATCH,				// Some internal problem 
	ERR_VAR_ALLOC,					// Couldn't allocate space for vars
	ERR_WB_INITIAL_TOKEN,			// We looking for Initial token such a Var, Begin, Procedure, etc. 
	ERR_VAR_IN_VAR,					// Found Var statement in a var block
	ERR_VAR_TOKEN_UNREC,			// Unrecognized (valid) token in var block
	ERR_VAR_BAD_VARSTRING,			// Var string isn't valid -- i.e. int MyVar@2, ec. 
	ERR_VAR_NUM_START,				// Variable started with a number
	ERR_VAR_HIDDEN_CHAR,			// Variable has hidden chars in it
	ERR_VAR_OP_IN_VARNAME,			// Found disallowed character in varname
	ERR_VAR_EXPECTED_VDELIM,		// Expected variable delimiter (this is pascal only, so it is a ':');
	ERR_VAR_EXPECTED_TYPE,			// Pascal Specific (?): Expected variable type, i.e. var : Integer
	ERR_EXPECTED_SEMICOLON,			// Expected Semicolon -- generic error
	ERR_CVAR_MALLOC,				// Couldn't allocate variable
	ERR_PROG_PASSED_DELETE,			// Programmer control -- passed a cError->Delete unexpectedly
	ERR_PREMATURE_EOF,				// EOF
	ERR_BAD_TREE,					// ParseExpression returned a bad tree
	ERR_EMPTY_PARENS,				// Empty expression, i.e. "i = ()"; 
};



class CError
{
public:			// Temporary kludge
	void SpotCheck();
	int bOutstandingError;
	static char * sStaticErrMsg;
	int bThrowsOn;
//	int eLastError;
	int bFatal;
	ErrType eLastError,eCurrentError;
	CError * cPreviousError;
	int bRevert;		// 1 = revert to previous state (i.e. flip throw status) on previous in destructor
	ErrType eMacroError;
	int bKillMacro;
	char sFuncName[100];		// Functionname initialized with 
public:
	static CError * cError;
	int ReportError(ErrType eError, int bOverridden=0);
	void ThrowsOn() { bThrowsOn = 1; }
	void ThrowsOff() { if (this) bThrowsOn = 0; }
	void ThrowsOffUntilNext() { bThrowsOn = 0; bRevert = 1; }
	void ThrowsOnUntilNext() { bThrowsOn = 0; bRevert = 1; }
	void KillMacroErrors() { bRevert = 0; eMacroError = ERR_NULL; } 
	CError(char * sFuncName);
	~CError();
	ErrType GetLastError();
	void SetMacroError(ErrType eMacro);
	void SetMacroErrorNext(ErrType eMacro);
	void Delete();
	int AssertMacroError(int iExpr);
	int AssertError(bool iExpr,ErrType eError);
	int AssertErrorMsg(bool iExpr,char * sMessage) { sStaticErrMsg = sMessage; return(AssertError(iExpr,ERR_MSG)); }
	void ReportErrorMsg(char * sMessage,int bDontThrow=0) {
		if (!gMessagesOff || (bThrowsOn && !bDontThrow)){ if (bDontThrow) ThrowsOffUntilNext();  sStaticErrMsg = sMessage; ReportError(ERR_MSG);} }
	static void PassedDelete();
};

}; // namespace Sage
#endif // _CToken_h_