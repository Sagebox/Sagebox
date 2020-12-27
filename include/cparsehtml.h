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


#if !defined(__CPARSEHTML_H__)
#define __CPARSEHTML_H__

#include "CSageBox.h"
#include "CHtmlImages.h"

namespace Sage
{
typedef struct
{
	int iInitialized[4];		// Initialization for normal, bold, italic, and italic bold.  Initialized on-the-fly
	int	iFontSize;				// Point reference
	int iFontSizes[4][0x60];	// Sizes for each font element
	char sFontName[4][100];
	char sFontDesc[4][100];
	HANDLE	hFont[4];


	int iFontHeights[4];
} stHtmlFontData_t;

#define kHtmlDrawMaxStyles			30			// 30 styles pushed max.
#define kHtmlMaxTableLines			3000			// No more than <n> lines per table
#define kHtmlMaxLineWidthSize		4000
#define	kHtmlWindowSizeLarge		850			// Window width large help items
#define	kHtmlWindowSizeSmall		350			// Window width for small items
#define kHtmlStringLengthMax		(20*1024)	// Maximum amount of text in one paragraph, strung together.
#define kHtmlDefaultLIAdd			50			// Number of pixels to indent on an LI.  Nesting LI's is not supported.
#define kHtmlMaxFontSizes			50			// Number of fonts sizes we can consider for the font sizing. See notes near CHtmlDraw::m_sStaticFontData
#define kHtmlMinFontSize			8			// To save some space, the subtract for the font table; turns into a minimum font size.

#define	kHtmlMaxBufferSize		(100*1024)		// Max buffer size to put control components and text. 
#define kHtmlStartStyleToken	"<style"
#define kHtmlStyleToken			".style"
#define kHtmlAutoStyleToken		".auto-style"
#define kHtmlEndStyleToken		"</style"
#define	kHtmlFontSize			/*"\x09" */"font-size:"			// $$ using tabs may come back to haunt. Should have a white-space indicator here.
#define	kHtmlFontWeight			/*"\x09" */"font-weight:"			// $$ using tabs may come back to haunt. Should have a white-space indicator here.
#define kHtmlFgColorTAB			"\x09" "color:"				//    It is needed to distinguish between "color" and "background-color", at least easily and seamlessly.
#define kHtmlFgColorSPACE		" " "color:"				//    It is needed to distinguish between "color" and "background-color", at least easily and seamlessly.
#define HtmlBgColor				/*"\x09" */"background-color:"
#define kHtmlTextAlign			/*"\x09" */"text-align:"
#define kHtmlVerticalAlign		/*"\x09" */"vertical-align:"
#define kHtmlStartTitle			"<title>"
#define kHtmlEndTitle			"</title>"
#define kHtmlTableStart			"<table"
#define	kHtmlTableEnd			"</table>"
#define kHtmlTableTRStart		"<tr"
#define kHtmlTableTREnd			"</tr>"
#define kHtmlTableTDStart		"<td"
#define kHtmlTableTDEnd			"</td>"
#define	kHtmlFontBold			"bold"
#define	kHtmlFondNormal			"normal"

#define	kHtmlTextLeft			"left"
#define kHtmlTextCenter			"center"
#define kHtmlTextMiddle			"middle"

#define kHelpSetFgColor		1
#define	kHelpSetBgColor		2
#define	kHelpSetTextSize	3
#define kHelpSetBold		5
#define	kHelpSetItalic		6

#define kHelpEndData		0x00
#define kHelpSetStyle		0x10
#define	kHelpStartTable		0x11
#define	kHelpStartTR		0x12
#define kHelpStartTD		0x13
#define	kHelpSetWidth		0x14
#define kHelpSetHeight		0x15
#define kHelpStartText		0x16
#define kHelpEndText		0x17		// $$ Could just use kHelpStartText for this.
#define	kHelpStartImage		0x18
#define kHelpEndTR			0x19
#define kHelpEndTD			0x1A
#define kHelpEndTable		0x1B
#define kHelpTextBR			0x1C
#define kHelpStartSpan		0x80
#define kHelpEndSpan		0x81
#define kHelpStartEM		0x82
#define kHelpEndEM			0x83
#define kHelpStartUL		0x84
#define kHelpEndUL			0x85
#define kHelpStartLI		0x86
#define kHelpEndLI			0x87
#define kHelpStartP			0x88
#define kHelpEndP			0x89
#define	kHelpSetJust		0x8A
#define kHelpStartStrong	0x8B
#define kHelpEndStrong		0x8C
#define kHelpStartTitle		0x8D
#define kHelpEndTitle		0x8E

#define kHtmlDefaultFont		"Arial12"

typedef struct
{

	// The style structure as read from the HTML code

	char		sName[100];
	DWORD		dwFgColor;
	DWORD		dwBgColor;
	int			iFontSize;			// Always in pixels. Only font used is Arial ($$ for now)
	int			iFontWeight;		// 0 = normal, 1 = bold
	int			iTextAlign;			// 0 = left, 1 = center
	int			iVerticalAlign;		// 0 = bottom, 1 = center, 2 = top; relative to table (?)

	// Additional information used by the parsing/draw routines

	char		sFontName[100];
	HFONT		hFont;				// May or may not be used. 
	int			iFontNumber;		// Font ID number in the font list. 
	int			iFontEmphasis;		// TBD, but basically, 0 = normal, 1 = bold... 

	// Addition tracking information

	int			iLastFont;
	int			iLastEmphasis;
	DWORD		dwLastFgColor;
	DWORD		dwLastBgColor;
	int			iLastTextAlign;		// Probably not used... 

	int			iLastStyle;
	int			iStyleNumber;
	int			iFontHeight;
} stHtmlStyle_t;

#define kMaxHelpImages			500			// Should be allocating this
#define kMaxHelpImageNameLength	150

typedef struct 
{
	char					sID[256];			// ID of the bitmap
	unsigned char		*   ucOriginalBitmap;	// When volatile, the original bitmap is placed here.  Typically a .TPC (lossless compression), but may be
												// a .BMP or .JPG. This must be found by looking into the file.  'BM = BMP; else 'JFIF
												// = JPG; else it's a .TPC file
	int						iOriginalPGRLocation;
	int						bDiscard;			// doesn't initially allocate the original file; some other process will get it from the PGR memory. Used for jpegs 
												// $$ This is transitional: Any volatile bitmap should not be committed to memory if it's a mem PGR
	int						iOriginalBitmapSize;
	CBitmap				theBitmap;
	int						bVolatile;		// If true, then the image is uncompressed when needed only, and then deallocated when CHtmlDraw is deleted

	int						bAlternateAddress;
} stHtmlBitmaps_t;



#define kFontNormal		0
#define kFontBold		1
#define kFontItalic		2

#define htmlfind(_x1,_y1) FindStringEx((unsigned char *) _x1,_y1)
#define htmlAssert(_x,_y) if (!((_x))) throw((char *) _y)
#define kHtmlCompare(_x,_y) !strncmp((char *) _y,_x,strlen(_x))

#define kHtmlOutValue16(_ID,_Value) { *m_sOutBuffer ++ = _ID; *m_sOutBuffer++ = (unsigned char) ((_Value) & 0xff); *m_sOutBuffer++ = (unsigned char) ((_Value) >> 8); }
#define kHtmlEndText		if (bInText)  *m_sOutBuffer++ = kHelpEndText;	bInText = 0;
#define kHtmlStartText		if (!bInText) *m_sOutBuffer++ = kHelpStartText; bInText = 1;
#define kHtmlAddChar(_c)	{ kHtmlStartText; *m_sOutBuffer++  = _c; }
#define kHtmlLIBulletStartX			20
class CParseHtml
{
public:
	int					  m_iLITag[50];
	int					  m_iULTag[50];
	int					  m_iEMTag[50];
	int					  m_iStrongTag[50];
	int					  m_iPTag[50];

	int					  m_iLICount;
	int					  m_iULCount;
	int					  m_iEMCount;
	int					  m_iStrongCount;
	int					  m_iPCount;

	int					  m_iSpanCount;
	int					  m_bMarkLI;
	unsigned char		* m_sTREnd;
	int					  m_iNumStyles;
	stHtmlStyle_t		  m_stStyles[30];		// Allow no more than 30 styles... 

	unsigned char		* m_sFirstMark;
	unsigned char		* m_sSecondMark;
	unsigned char		* m_sOrgInData;
	unsigned char		* m_sOutBuffer;
	unsigned char		* m_sOrgOutBuffer;
	CHtmlImages			* m_cLocHtmlImages = nullptr;
	CHtmlImages			* m_cGblHtmlImages = nullptr;

	CParseHtml(unsigned char * sData,unsigned char * sOutputBuffer,CHtmlImages * cLocalHtmlImages, CHtmlImages * cGlobalHtmlImages);
	unsigned char * Mark(unsigned char * sSource,const char * sBegin,const char * sEnd);
	int Parse(); 
	int ParseStyles(unsigned char * sSearch);
	int GetWebColor(unsigned char * sBuffer,DWORD & dwColor);
	int ParseTDHeader(unsigned char * & sSearch);
	unsigned char * SingleMark(const char * sSearch,unsigned char * sData);
	int ParseStyleClass(unsigned char * sSearch);
	int StartTableData(unsigned char * & sSearch);
	unsigned char *  LocateTableData(unsigned char * & sSearch);
	unsigned char * LocateTable(unsigned char * & sSearch);
	int ParseTableData(unsigned char * sSearch);
	int InsertImage(unsigned char * & sSearch);
	int SetHtmlTitle(unsigned char * & sSearch);
	int AddBodiedTagEx(char * sTag,int bEnding,unsigned char ucStartTag,unsigned char ucEndTag,unsigned char * & sSearch, int * ipTag,		int & iTagCount);
	int AddBodiedTag(char * sTag,int bEnding,unsigned char ucStartTag,unsigned char ucEndTag,unsigned char * & sSearch,int bHasBody = 1);
	int SkipBodiedTag(char * sTag,int bEnding,unsigned char ucStartTag,unsigned char ucEndTag,unsigned char * & sSearch,int bHasBody = 1);

	int AddNonBodiedTag(char * sTag,int bEnding,unsigned char ucStartTag,unsigned char ucEndTag,unsigned char * & sSearch,int bHasBody = 0);
	bool AddImage(const char * sImagePath,Mem<unsigned char> & stImageMem);

};

typedef struct
{
	// $$ Will probably have to add FgColor, BgColor and possibly other things, as well. 

	int iWidth;	
	int iHeight;
	int iJust;
	int iStyle;

} sTDInfo_t;

class CHtmlDraw
{
	static constexpr int		  m_iLeftBufferPixels	= 10;
	static constexpr int		  m_iRightBufferPixels	= 20;
public:
	int							  m_bHtmlExifFixup;
	int							  m_bPreserveBitmaps;
	int							  m_iBaseWindowY;
	int							  m_iRequiredY;
	int							  m_bPrintedTexture;
	RawBitmap_t					  m_stTexture;
	int							  m_bTracking;
	int						      m_iCurrentLine;
	int							  m_iTrackLineHeight;						// Largest height we've seen for the current line.  Gets put into m_iTDLineSizes on NextLine()
	SIZE						  m_iTDLineSizes[kHtmlMaxTableLines];		// Tracking for NextLine() (et. al.) on the actual draw.
	int							  m_iLIY;
	int							  m_iLIFontNumber;
	int							  m_iLIFontEmphasis;

	int							  m_bInEm;
	unsigned char				  m_sDocumentTitle[1024];
	DWORD						  m_dwFillTableColor;
	int							  m_iCurrentStyle;
	int							  m_iTDMinHeight;
	int							  m_iWidthLeft;
	int							  m_iCurrentLength;
	int							  m_bDrawMode;
	char						  m_sPartialString[kHtmlStringLengthMax];
	sTDInfo_t					  m_stTDInfo[20];							// Up to columns 
	stHtmlStyle_t				  m_stStyleStack[kHtmlDrawMaxStyles];
	stHtmlStyle_t				  m_stCurrentStyle;
	stHtmlStyle_t				  m_stCurrentTableStyle;
	unsigned char				* m_ucpStartPlace;
	unsigned char				* m_sInMemory;
	unsigned char				* m_sOrgInMemory;
	int							  m_iTDDepth;
	int							  m_iTableStyle;
	int							  m_iNumCharstoPrint;

	int							  m_bDisableGraphicOutput;
	int							  m_iWindowWidth;
	int							  m_iCurrentX;
	int							  m_iCurrentY;
	int							  m_iLineCharWidths[kHtmlMaxLineWidthSize];	// Used to track the output in the first pass of the Draw HTML
	int							  m_iStyleStackID;
	int							  m_iCurrentMinWidth;
	int							  m_iCurrentMaxWidth;
	int							  m_iCurrentMinHeight[kHtmlMaxTableLines];
	int							  m_iCurrentMinHeight2;
	int							  m_iDefaultFontNumber;
	int							  m_iDefaultFontEmphasis;
	int							  m_iCurrentFontNumber;
	int							  m_iCurrentFontEmphasis;
	DWORD						  m_dwDefaultBgColor;
	DWORD						  m_dwDefaultFgColor;

	HANDLE						  m_hCurrentFont;
	int							  m_iCurrentStartX;							// Current Start X of the table column
	int							  m_iCurrentStartY;

	DWORD						  m_dwCurrentFgColor;
	DWORD						  m_dwCurrentBgColor;
	stHtmlFontData_t			* m_stFontData;
	int							  m_iTableDepth;							// only one is allowed currently.
	stHtmlStyle_t				  m_stStyles[30];							// Allow no more than 30 styles... 
	int 						  m_iNumStyles;
	int 						  m_iDefaultFontSize;
	CWindow						* m_cWin = nullptr;
	int							  m_bInLI;
//	int							  m_iCurrentLine;
	static stHtmlFontData_t		  m_stStaticFontData[kHtmlMaxFontSizes];	// See notes by CHtmlDraw::m_sStaticFontData
	static int					  m_bInitialized;
	CHtmlImages					  * m_cLocHtmlImages = nullptr;
	CHtmlImages					  * m_cGblHtmlImages = nullptr;


	CHtmlDraw(CWindow * cWin,unsigned char * sInMemory,int iDefaultFontSize,CHtmlImages * cLocalHtmlImages, CHtmlImages * cGlobalHtmlImages);

	int 	SetDefaultAttributes();
	int     StartTitle();
	int 	StartTable();
	int 	EndTable();
	int 	StartTR();
	int		HandleTR();
	int		HandleTD();
	int 	EndTR();
	int 	StartTD();	
	int 	EndTD();
	int 	Draw(int iYOffset,int iWindowWidth);
	int		Redraw(int iYOffset); 
	int 	Get16BitInt();
	int 	SetStyle(int iStyle);
	int 	CheckInsertStyle();
	int 	DrawTableData();
	int 	CalculateTableWidth();
	int 	InitializeFonts();
	int 	TransferStyles(stHtmlStyle_t * stStyles,int iNumStyles) 
			{ 
				m_iNumStyles = iNumStyles; 
				memcpy(m_stStyles,stStyles,sizeof(m_stStyles)); 
				return(0);
			}
	int 	CalculateImageWidth();
	int 	CalculateTextWidth();
	int 	SetEmphasisStyle();
	int 	SetStrongStyle();
	int 	PrintTextLine();
	int 	DisableGraphicOutput()	{ m_bDisableGraphicOutput = 1; return(0); }
	int 	EnableGraphicOutput(int bEnable = 1)	{ m_bDisableGraphicOutput = (bEnable) ? 0 : 1; return(0); }
	int 	PushStyle();
	int 	SetCurrentStyle(int iStyle);
	int 	PopStyle(int iStyleNumber = -1);
	int		PrintActualLine();
	int		NextLine(int bForce=0);
	void	Initialize(CWindow * cWin,unsigned char * sInMemory,int iDefaultFontSize);
	int		InitializeStyleFont(int iFontNumber,int iFontEmphasis);
	int		StartLI();
	int		PushConditionalStyle();
	int		PopConditionalStyle();
	~CHtmlDraw();
};

}; // namespace Sage

#endif