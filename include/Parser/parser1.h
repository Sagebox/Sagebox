//#pragma once


#if !defined(AFX_PARSER1_H__7C8D5110_278E_4549_B184_5869BDCB36F8__INCLUDED_)
#define AFX_PARSER1_H__7C8D5110_278E_4549_B184_5869BDCB36F8__INCLUDED_

#if _MSC_VER > 1000
//#pragma once
#endif // _MSC_VER > 1000

//#include "resource.h"
#define STANDALONE

#define TOTALLY_QUIET

#ifdef TOTALLY_QUIET
  #define QUIET_MODE
  #define NO_DISASSEMBLE
#else 
  #define IMPORTANT_MSGS
// #define NO_DISASSEMBLE

#endif

#define MAX_THREADS		20
#define MAX_USER_THREADS 20 
#define THREAD_STACK_SIZE		40*1024		// 40k stack size 


#ifdef QUIET_MODE
const int gMessagesOff = 1;
#else
const int gMessagesOff = 0;
#endif
#include "..\CDevString.h"
/*********************************************************************
 Tables for faster token searching. 
 *********************************************************************/

// Table for fast AlphaNumeric Finds


namespace CSageScript
{



struct stHEADER
{
	int iReturnCode;
	int iContentLength;
	char sContentType[100];
	int iError;
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

// Name the operators for comparison purposes.   

// Later, we'll probably use a struct or class to describe these for fast token usage

/** WARN **/
/* A Weak link here, but MAKE SURE this set of defines (consts) corresponds to sOperatorTable */

const char optQuote			= 1;
const char optPound			= 2;
const char optDollar		= 3;
const char optPercent		= 4;
const char optAmpersand		= 5;
const char optSingleQuote	= 6;
const char optLeftParen		= 7;
const char optRightParent	= 8;
const char optAsterisk		= 9;
const char optPlus			= 10;
const char optComma			= 11;
const char optMinus			= 12;
const char optPeriod		= 13;
const char optSlash			= 14;
const char optSemiColon		= 15;
const char optLessThan		= 16;
const char optEquals		= 17;
const char optGreaterThan	= 18;
const char optLeftBracket	= 19;
const char optRightBracket	= 20;
const char optCaret			= 21;
const char optLeftBrace		= 22;
const char optGate			= 23;
const char optRightBrace	= 24;
const char optTilda			= 25;


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
struct stTOKENDATA_VAR
{
	int i;
};

// Variable Construct 

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

struct stARRAYCONFIG; 
struct stCODEBLOCK
{
	unsigned char * sBlock;
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

class CFunc; 
class CTree;
struct TempClass_t
{
	TempClass_t * stPrevious; 
	TempClass_t * stNext; 
	char sClassName[100];
	char sOrgClassName[100];
	char * spOrgName; 
	char * spName;

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

enum eNODESTATUS
{
	ntNULL,
	ntOp,
	ntValue,
	ntUpOp,		// Unary-prefix type, i.e. -value, ~value, ,&value, etc. maybe $value at some point for script purposes
	ntMod,		// Modified by program, such as a sub-expresion
	ntUnk,
};
enum eEXPRTYPES
{
	ntsNULL,
	ntsLogical,
	ntsMath,
	ntsAssign,
	
};

enum eTYPECHECKTYPE
{
	tctNULL,
	tctStrong, 
	tctWeak,
};


struct stNODEDATALIST
{
	enum eNODETYPE eNodeType;		// Just for reference (for debugging only)
	char * sName;					// It's name (for debugging only)
	enum eNODESTATUS eType;			// i.e. operator, value, etc.
	enum eEXPRTYPES eSubType;		//
	enum eTYPECHECKTYPE eTypeCheckType;
	int iPriority;					// It's priority, if it's an operator
}; 

// Make a dup-list for tokens that have the same characteristics, such as '-' can be tsub or tneg,
// & can be tlogAnd and tAddress, etc. 
// 
// When the parse goes through this, if it can identify one, that is ok because they are
// mutually exclusive, so that will be correct.  When a token is not found (typically from the
// stNodeDataList), this list will then be referred to for duplicates so that the right token will
// be found. 
//
// This is done this way so that this may be automated later by a yacc-like language generator, and
// the rest of the program doesn't have to worry about any translation, except for one loop in the
// token parser. 

struct stTOKENDUP
{
	enum TokenType eOrgToken;		// The token it will find (i.e.  first in list)
	enum TokenType eDupToken;		// The token that it should be translated to
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

// Since this all canned, we can use a list.  So, we really need here something like this:
struct stTOKENOPERATORLOOKUP
{ 
	char * sToken;
	enum	TokenType eToken;
}; 
			
// I suppose if I were more thoughtful about this, this might not be needed. 


// Some expression scenarios: 

// x + y
// x + *y
// x+*y;
// x* *y;
// x^y
// x^*y
// x*(*y+7)

// I guess, as a matter of global syntax, common to ALL languages, we can consider a '(' as a delimter
// on an expression. 
// I suppose we can also consider the pointer char (if there is one that is a non-alphanumeric character, such
// as PTR, for instance) as a single entry, a delimiter by itself.  So, if we have *this or **that or 
// **************that, we should realy be thinking about this in terms of looking at it one at a time. 

// In GetString, then, this is where '(' should be looked at. 




// Web-Driver

// First char represent case-sensitive code:
// ! = not-case sensitive		-- Example = "begin" or "BEGIN" for pascal
// * = case-sensitive			-- Example = "AND", "OR", "NOT" for pascal


struct MultiWordTokens_t
{
	enum TokenType eSourceToken; 
	enum TokenType eDestToken; 
	char * sString;
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




// Delete the last [iNumtoDelete] vars.  Used for error purposes to cleanly exit without
// destroying all vars



/* Basic Var Types:

Int,unsigned int,char,unsigned char,short,unsigned short,long,unsigned long.
Pointer is a unary operator, so this is handled separately

With the pre-defined types, we're finding them by token types.  With user-defined types or
programmatically added types, we'll find them by name.  Especially since we won't have to worry
about 'unsigned', etc., i.e. typdef unsigned int mytype.. never have to worry about this

*/

enum eBASE_VAR_TYPES
{
	_bvtNULL,
	_bvtChar,
	_bvtUChar,
	_bvtShort,
	_bvtUShort,
	_bvtInt,
	_bvtUInt,
	_bvtLong,
	_bvtULong
}; 


struct stBASE_VAR_TYPE
{
	enum TokenType	eToken;
	enum eBASE_VAR_TYPES eBaseVarType;
	int iWidth;
	bool iSigned; 
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


struct stVALIDPREDEFINEDNAMES
{
	enum TokenType	tToken;
	char * sValidName;
};





struct stVARNAMELIST
{
	char sName[50];
	char * spName;
	stVARNAMELIST * stPrevious;
	stVARNAMELIST * stNext;

};
// Overload class.  Pure virtual.  Call when operators are overloaded. 
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

private:
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
	int AssertError(int iExpr,ErrType eError);
	int AssertErrorMsg(int iExpr,char * sMessage) { sStaticErrMsg = sMessage; return(AssertError(iExpr,ERR_MSG)); }
	void ReportErrorMsg(char * sMessage,int bDontThrow=0) {
		if (!gMessagesOff || (bThrowsOn && !bDontThrow)){ if (bDontThrow) ThrowsOffUntilNext();  sStaticErrMsg = sMessage; ReportError(ERR_MSG);} }
	static void PassedDelete();
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
	static CTree *  GraftLeftoverTrees(CTree * cThis);
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

class CCodeBlock;

class CVars;
const int TokenInsert_File   = 1;
const int TokenInsert_Define = 2;
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
	int CheckforEmptyParens(void);
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


class CDebug
{
	int bDebugStatus;
public:
	void Debug(int bDebug=1) { bDebugStatus = bDebug; }
	void OperatorTableCheck();
	CDebug(int iStatus=1) { bDebugStatus = iStatus; } 
	void PrintToken(stNODEDATA * stNODEDATA);

};
struct RegStruct
{
	int idreg;
	int isreg;
	int curbp;
	int cursp;
	int iCurrentAddress;
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
enum e_ThreadCommand
{
	tc_NULL = 0,
	tc_Sleep,
	tc_WEB,
	tc_DownloadFile,
	tc_ENDWEB,
	tc_GetChar,
};
enum wd_Token_t
{
	wd_tNULL =0,
	wd_tNULL2, 
	wd_tNULL3, 
	wd_tNULL4, 
 	wd_tNULL5, 
	wd_tNULL6, 
	wd_tNULL7, 
	wd_tNULL8,
	wd_tNULL9,		// Filled in to make auto-return of wd_tNum = tNum
	wd_tNum, 
	
	wd_tHTTP,
	wd_tFTP,
	wd_tUrlTypeDelim,
	wd_tSlash,
	wd_tFileSize,
	wd_tRedirs,
	wd_tOverwrite,
	wd_tForms,
	wd_tImageSize,
	wd_tFileTypes,
	wd_tYes,
	wd_tNo,
	wd_tGreater,
	wd_tGreaterEqual,
	wd_tLess,
	wd_tLessEqual,
	wd_tEqual,
	wd_tColon,
	wd_tDash,
	wd_tBy,
	wd_tImageWidth,
	wd_tImageHeight,
	wd_tFileDate,
	wd_tNULLx,
	wd_tNULLx2,
wd_tNULLx3,

	wd_tEOF,			// Needed to synchronization with TokenTypes
	wd_tComma,
	wd_tSemicolon,
	wd_tJpeg,
	wd_tGif,
	wd_tAny,
	wd_tTif,
	wd_tPng,
	wd_tNot,
	wd_tlitNull,
	wd_tExplicit
};

struct WebFileParms_t
{
	enum wd_Token_t stToken;
	int iType;
	char * sParmName;

};

class CTempClass;

class CCPUBlock;

class CThread
{
public:
	int	iThread;
	int	iPriority;
	int iDataRegBase;
	int iAddrRegBase;
	int iLastDataRegUsed;
	int iLastAddrRegUsed;
	int iCallDepth; 
	int iActive;
	int	iStackSize;
	int bThreadBlocking;
	unsigned char * sStack; 
	unsigned int  * sDataReg; 
	unsigned int  * sAddrReg; 
	unsigned char * IP;
	unsigned char * sStackAddr;
	unsigned int    R0;
	unsigned int    BP; 
	unsigned int    iAddrRegs[1000];		// Enough to 1000 addr regs.. 
	unsigned int    iDataRegs[1000];
	unsigned int    sDataRegBase[1000];
	unsigned int    sAddrRegBase[1000];
	unsigned int    uiFlags;
//	WebData_t	stWebData; 
//	CThreader cThreader;
};

#if 1
class CCodeBlock
{
public:
	int iCodeBlockSize;
	CThread cThreads[MAX_USER_THREADS+1];
	int iCurrentThread; 
	int iNumThreads;
	int iThreading; 

	unsigned char * sMainCodeBlock;			// Where the code is stored
	int iCurrentAddr;						// Address of current code pointer
	unsigned char * sFixups;				// Any fixups (TBD)
	int iMaxCodeBlockSize;
	unsigned char * sCurrent;
	//CodeBlock();
	RegStruct RegStruct;
	int iNumFunctions;						// # of functions / procedures besides global main()
												// Later, this will have to be more object-oriented
public:
	CVars * cCurrentVar;
	CVars * cVarBase;						// The variable set for this codeblock
	CFunc * cFuncBase;						// Functions in the code base. 
	CFunc * cCurFunc;
	CCodeBlock(unsigned char * sCodeBlock, CVars * cVar, int iCodeBlockSize);
	void AddPartialFunc(char * sFuncName);
	int iMainAddr;
	void AddUserFunc(int iFuncNumber,char * sPrototype);
	int iInFunction; // ### A little kludgy here
	CCPUBlock * CpuBlock;
	CTempClass * cClasses;

};
class CCPUBlock
{
public:
	int	iCurrentAddr;
	int ibp;						// BP
	int isp;						// SP
	int r0;							// Result Register
	unsigned char * sCodeBlock; 
	unsigned char * sStack; 
	int iStackSize; 
	unsigned short * sRegisterStack;
	CCodeBlock * cCodeBlock;
	int iStackRegisterAddress;
public:
	CCPUBlock(CCodeBlock * cCodeBlock,unsigned char * sStack,int iStackSize,
					 unsigned short * sRegisterStack, int iRegisterStackSize); 
};

// Code Block Class
// 
// Used for Begin/End, {} blocks.  Blocks can be indiscriminant. 
// They have distinct types:
//
//		tFor		-- For loop
//		tIf			-- If
//		tWhile
//		tBegin		-- A blank block, i.e. someone did a Begin End; or { } without a conditional
//                     expression such as if, for, etc. 
// 
/*
For Block:
<init exp> <cond exp> <step exp> 

Flow: 

<init exp>
jmp @end (Block Fixup #1) 
@label  (Block Start Address) 
<step exp>

<< BLOCK CODE >> 

@end: (Fixup start address) 
<cond exp>
gc_cmpzero		// i.e. zero = cond-z, else cond-nz
jnz @label (Block Start Address) 


So, a FOR block has TWO labels, a BEGIN and END. 

IF Flow:

<exp>
gc_cmpzero
jz @else  (Block Fixup #1) 

	<< Block >> 

jmp @end (Block Fixup #2) 
@else
 Else << Block>> 

@end



*/



class CBlock 
{
public:
	enum TokenType	eBlockType;					// i.e. tFor, tIf, etc. 
	unsigned int	uiFixup1;				// First Fixup
	unsigned int	uiFixup2;				// Second Fixup, if required
	unsigned int	uiElseAddress;			// Else Address, if required
	unsigned int	uiEndAddress;			// End address ### these last two may not be needed
	CBlock * cNextBlock;
	CBlock * cPreviousBlock; 
	CTree  * cTree1;						// all: <init>
	CTree  * cTree2;						// For: <cond>, else: none
	CTree  * cTree3;						// For: <step>, if any, else none
	unsigned int	uiStartAddress;			// Block StartAddress of block
	CBlock(CBlock * cBlock=NULL); 


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

struct Fixup_t
{
	Fixup_t * stNext; 
	unsigned int iFixupAddress;
};


class CFixup
{
public:
	CFixup * cPrevious;	// i.e. linked-list of fixups
	int iNumFixups; 
	Fixup_t * stFixup; 
	Fixup_t * stOrgFixup; 
	unsigned int uiAugment;
	CFixup(CFixup * cPrev=NULL,unsigned int uiAugment = 0) { this->uiAugment = uiAugment; iNumFixups = 0; stFixup = stOrgFixup = NULL; cPrevious = cPrev; }
	void AddFixup(unsigned int uiAddress);
};


class CCodeGen
{
#define sa(_x) (_x | 0x4000)
public:
	CVars * cVar;
	int GenerateCode(stNODE * stStartNode,CCodeBlock * cCodeBlock=NULL,int iStartPSRs=0,CFixup * cFixup=NULL);
	void GenerateCodePreamble(CCodeBlock * cCodeBlock);
	void GenerateReturnCode(CCodeBlock * cCodeBlock); 
	void GenerateCodePostamble(CCodeBlock * cCodeBlock);
	CCodeGen(CVars * cVar) { this->cVar = cVar; }
	CCodeBlock * cTempCodeBlock;
	void cg_LoadNum(int iReg1,int iReg2);
	void cg_LoadVar(stNODEDATA * stNodeData,int & iAddrReg, int iDataReg);
	void cg_LoadFunc(stNODEDATA * stNodeData,int & iAddrReg, int iDataReg,CFixup * cFixup);
	void cg_Assign(stNODE * stNode,int iAddrReg,int iReg);
	void cg_Pointer(stNODE * stNode,int iDataReg,int iAddrReg); 
	void WriteByte(unsigned char ucChar);
	void WriteWord(unsigned short usWord);
	inline void WriteDword(unsigned int uiDword);
	CCodeGen operator << (enum t_genCodeTypes x) { WriteDword(x); return(*this); }
	inline CCodeGen operator << (int x) { WriteDword(x); return(*this); }
	void Disassemble(int iWordsBack);
};
class CCompiler;
class CWebDriver
{
public:
	CToken * cToken;
	CDebug * cDebug;
	CVars * cGlobalVars; 
	CCompiler * m_cCompiler;

	CWebDriver(CCompiler * cCompiler) { m_cCompiler = cCompiler; }
	CCodeBlock * Compile(int bLab = 0,CCodeBlock * cInCodeBase = NULL,const char * sSource = nullptr);
	CCodeBlock * CompileSafe(int bLab = 0,CCodeBlock * cInCodeBase = NULL,const char * sSource = nullptr);
	int Run(CCodeBlock * cCodeBlock);
	int HandleVarDecl(CVars * cVar,CCodeBlock * cCodeBlock,enum TokenType * stDelimiters,bool bGenCode=true ); 
	int HandleNewFunctionBlock(CCodeBlock * cCodeBlock,bool bInBlock,bool bOneLiner);
	int HandleNewFunctionDecl(CCodeBlock * cCodeBlock); 
	void FixupVarDecTree(CTree * cTree,CVars * cVar,int iNumVars);
	int HandleFor(CCodeBlock * cCodeBlock,CCodeGen * cCodeGen);
	int HandleIF(CCodeBlock * cCodeBlock,CCodeGen * cCodeGen);
	int HandleRepeat(CCodeBlock * cCodeBlock,CCodeGen * cCodeGen);
	CVars * cVar; 

};

#endif


class CStringOverload: public COverload
{
	int		nAddIntOverloadIndex;
	int		nAddStringOverloadIndex;
	int		nAddCharPtrOverloadIndex;
	int		nAssignCharPtrOverloadIndex;
	int		nAssignStringOverloadIndex;
public:
	stNODE *  OverloadFunction(stNODE * stNode,int iOverloadIndex);
	void AddIntOverloadIndex() { };
	void AddStringOverloadIndex() { };
	void AddCharPtrOverloadIndex() { };
	void AssignCharPtrOverloadIndex() { };
	void AssignStringOverloadIndex() { };
	CStringOverload(CCodeBlock * cCodeBlock, int _nAddIntOverloadIndex,
					int	_nAddStringOverloadIndex,
					int	_nAddCharPtrOverloadIndex,
					int	_nAssignCharPtrOverloadIndex,
					int	_nAssignStringOverloadIndex) :  COverload(cCodeBlock) {

	nAddIntOverloadIndex = _nAddIntOverloadIndex;
	nAddStringOverloadIndex = _nAddStringOverloadIndex;
	nAddCharPtrOverloadIndex = _nAddCharPtrOverloadIndex;
	nAssignCharPtrOverloadIndex = _nAssignCharPtrOverloadIndex;
	nAssignStringOverloadIndex = _nAssignStringOverloadIndex;
	}
	
};

class CCharOverloads: public COverload
{
public:
	stNODE *  OverloadFunction(stNODE * stNode,int iOverloadIndex);
	CCharOverloads(CCodeBlock * cCodeBlock) : COverload(cCodeBlock) { };
};

class CFloatOverloads: public COverload
{
public:
	stNODE *  OverloadFunction(stNODE * stNode,int iOverloadIndex);
	CFloatOverloads(CCodeBlock * cCodeBlock) : COverload(cCodeBlock) { };
};


class CIntOverloads: public COverload
{
public:
	stNODE *  OverloadFunction(stNODE * stNode,int iOverloadIndex);
	CIntOverloads(CCodeBlock * cCodeBlock) : COverload(cCodeBlock) { };
};
}; // CSageScript

#endif // !defined(AFX_PARSER1_H__7C8D5110_278E_4549_B184_5869BDCB36F8__INCLUDED_)
 