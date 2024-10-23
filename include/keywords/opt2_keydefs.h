#pragma once

#include "CPoint.h"
// ---------------------------
// Example keyword definitions
// ---------------------------

// Fill in the macros below for keyword name and keyword type.
// This sample file has 4 entries as examples.

// --> Using Multiple Keywords Sets in the Same Program
// 
//     Some #defines are used, which are necessarily included in the keyword interface, which means
//     they become global macros, hence the "_ckwargs_" prefix to make them unique. 
//
//     For programs that want to use multiple keyword sets, 
//     the first 4 #defines will need to be unique, such as basing the prefix on the namespace for each keyword set.
//
//     Another option is to remove the #defines (nothing else exposed but the first 4 #defines below)
//     and add each keyword name for each of the 4 sections where they are used.
// 
//     The using statements and CheckItem macros are not exposed pubilicy and don't need to be changed for 
//     other keywords sets.
//

// For a program to use functions using cwargs, it only needs to include the single my_keywords.h/my_keyfuncs.h, depending on the
// form being used.  The file included will set up all keywords for the program. 

namespace Sage
{
namespace kwType
{
    struct stRegionDef_t
    {
        SizeRect szRegion;
        bool bClipped;
    };
    struct stGradientPoints_t
    {
        POINT p1;
        POINT p2;
        float fAngleDeg;
    };

    struct stLineCaps_t
    {
        LineCapType eBegCap; 
        LineCapType eEndCap;
    };

    struct stSageIcon_t
    {
        Sage::SageIconType Icon;
        TextJust just;
    };
    struct stSetIcon_t
    {
        long long lDataStoreID; 
        TextJust just;
    };
   struct stDualColorStruct_t
    {
        RgbColor rgbColor1;
        RgbColor rgbColor2;
    };

   struct stPenColorAndSize_t
   {
       CRgbColor     rgbColor; 
       const char * sRgbColor;
       int          iPenSize;
       float        fPenSize; 
   };

   struct stColorGradientStr_t
   {
       const char * sColor1;
       const char * sColor2;
   };

   struct DualFloatInt
   {
       struct stOpValInt
       {
           int iValue;
           bool bValueSet;
       };

       struct stOpValFlt
       {
           float fValue;
           bool bValueSet;
       };

       stOpValInt vInt[2];
       stOpValFlt vFlt[2];

       int IntX_or(int iRetValue)
       {
           if (vInt[0].bValueSet) return vInt[0].iValue;
           if (vFlt[0].bValueSet) return (int) vFlt[0].fValue;
           return iRetValue;
       };
       int IntY_or(int iRetValue)
       {
           if (vInt[1].bValueSet) return vInt[1].iValue;
           if (vFlt[1].bValueSet) return (int) vFlt[1].fValue;
           return iRetValue;
       };
       std::optional<int> IntX()
       {
           if (vInt[0].bValueSet) return vInt[0].iValue;
           if (vFlt[0].bValueSet) return (int) vFlt[0].fValue;
           return std::nullopt;
       };
       std::optional<int> IntY()
       {
           if (vInt[1].bValueSet) return vInt[1].iValue;
           if (vFlt[1].bValueSet) return (int) vFlt[1].fValue;
           return std::nullopt;
       };
   };
// define the keyword names used.  These are not the keywords used by the user (these come from my_keywordsh and/or my_keyfuncs.h).
// The keys below are used by the functions using the keywords as symblic names such as key.Range, key.Text, etc.

#define _sageopt2_ckwargs_key1 _StartCapStr         // Example Range keyword or function, i.e. Range = {5,10} or Range(5,10)
#define _sageopt2_ckwargs_key2 _EndCapStr           // i.e. Text = "Hello World" or Text("Hello World")
#define _sageopt2_ckwargs_key3 _PenSize             // i.e. BorderSize = 10, or BorderSize(10)
#define _sageopt2_ckwargs_key4 _PenSizef            // i.e. AddBorder = true, AddBorder = false, or AddBorder() or AddBorder(true) or AddBorder(false)
#define _sageopt2_ckwargs_key5 _AngleRad    
#define _sageopt2_ckwargs_key6 _AngleRadf      
#define _sageopt2_ckwargs_key7 _AngleDeg
#define _sageopt2_ckwargs_key8 _AngleDegf      
#define _sageopt2_ckwargs_key9 _PenColorCRgb
#define _sageopt2_ckwargs_key10 _PenColorStr 
#define _sageopt2_ckwargs_key11 _Truncate 
#define _sageopt2_ckwargs_key12 _SetCenter 
#define _sageopt2_ckwargs_key13 _SetCenterf
#define _sageopt2_ckwargs_key14 _Opacity 
#define _sageopt2_ckwargs_key15 _DebugBool1
#define _sageopt2_ckwargs_key16 _DebugBool2
#define _sageopt2_ckwargs_key17 _DebugBool3
#define _sageopt2_ckwargs_key18 _DebugBool4
#define _sageopt2_ckwargs_key19 _AutoMouseCapture    
#define _sageopt2_ckwargs_key20 _NoPromote              // bool
#define _sageopt2_ckwargs_key21 _GradientAngleDeg
#define _sageopt2_ckwargs_key22 _GradientAngleDegf
#define _sageopt2_ckwargs_key23 _GradientAngleRad
#define _sageopt2_ckwargs_key24 _PenGradient_free       // free!!
#define _sageopt2_ckwargs_key25 _PenGradientStr_free    // free!!
#define _sageopt2_ckwargs_key26 _PenGradientAngleDeg
#define _sageopt2_ckwargs_key27 _PenGradientAngleDegf
#define _sageopt2_ckwargs_key28 _PenGradientAngleRad
#define _sageopt2_ckwargs_key29 _SelectionAutoFocus
#define _sageopt2_ckwargs_key30 _BoundingRect2_free     // free -- maybe
#define _sageopt2_ckwargs_key31 _BoundingSizeRect2


// Additions for NewWindow()

#define _sageopt2_ckwargs_key32 _Hidden             // bool
#define _sageopt2_ckwargs_key33 _ResizeOk           // bool
#define _sageopt2_ckwargs_key34 _DirectDraw         // bool
#define _sageopt2_ckwargs_key35 _Realtime           // bool
#define _sageopt2_ckwargs_key36 __priv_QuickCPP     // bool
#define _sageopt2_ckwargs_key37 __priv_DebugMode    // bool
#define _sageopt2_ckwargs_key38 __priv_NonPrimary   // bool
#define _sageopt2_ckwargs_key39 _Title              // String (const char *)
#define _sageopt2_ckwargs_key40 _Name               // String (const char *)
#define _sageopt2_ckwargs_key41 _ID                 // Int
#define _sageopt2_ckwargs_key42 _Font               // Int
#define _sageopt2_ckwargs_key43 _Fontf              // Float
#define _sageopt2_ckwargs_key44 _FontStr            // String (const char *)
#define _sageopt2_ckwargs_key45 _bgColor            // Rgb Gradient
#define _sageopt2_ckwargs_key46 _bgColorStr         // const char *
#define _sageopt2_ckwargs_key47 _fgColor            // Rgb Gradient
#define _sageopt2_ckwargs_key48 _fgColorStr         // String (const char *)


#define _sageopt2_ckwargs_key49 _NoBorder
#define _sageopt2_ckwargs_key50 _Child
#define _sageopt2_ckwargs_key51 _Border
#define _sageopt2_ckwargs_key52 _Raised
#define _sageopt2_ckwargs_key53 _Recessed
#define _sageopt2_ckwargs_key54 _Depressed
#define _sageopt2_ckwargs_key55 _Sunken
#define _sageopt2_ckwargs_key56 _Transparent
#define _sageopt2_ckwargs_key57 _NoBorder2
#define _sageopt2_ckwargs_key58 _SetLocation 
#define _sageopt2_ckwargs_key59 _Str
#define _sageopt2_ckwargs_key60 _AsFloat
#define _sageopt2_ckwargs_key61 _DefaultColors
#define _sageopt2_ckwargs_key62 _NoSysMenu
#define _sageopt2_ckwargs_key63 _NoCaption
#define _sageopt2_ckwargs_key64 _Toolwindow
#define _sageopt2_ckwargs_key65 _DoubleClick
#define _sageopt2_ckwargs_key66 _NoTaskbar
#define _sageopt2_ckwargs_key67 _NoOwner
#define _sageopt2_ckwargs_key68 _NoCursor
#define _sageopt2_ckwargs_key69 _Resizeable_free // !! free (??)
#define _sageopt2_ckwargs_key70 _NoSizing
#define _sageopt2_ckwargs_key71 _NoClose

#define _sageopt2_ckwargs_key72 ___priv_xransparent
#define _sageopt2_ckwargs_key73 _DropShadow
#define _sageopt2_ckwargs_key74 _NoAutoUpdate
#define _sageopt2_ckwargs_key75 _DefaultStr
#define _sageopt2_ckwargs_key76 _NoTitle
#define _sageopt2_ckwargs_key77 _AutoUpdate
#define _sageopt2_ckwargs_key78 _NotResizable
#define _sageopt2_ckwargs_key79 ___priv_LocalWindow
#define _sageopt2_ckwargs_key80 _NoScrollbar
#define _sageopt2_ckwargs_key81 _TextOnly
#define _sageopt2_ckwargs_key82 _AsDialog


// Write functions

#define _sageopt2_ckwargs_key83 _TabPos
#define _sageopt2_ckwargs_key84 _Tab
#define _sageopt2_ckwargs_key85 _XPos
#define _sageopt2_ckwargs_key86 _PadX
#define _sageopt2_ckwargs_key87 _PadY
#define _sageopt2_ckwargs_key88 __free_JustBottom      
#define _sageopt2_ckwargs_key89 __free_JustTop        
#define _sageopt2_ckwargs_key90 __free_JustRight       
#define _sageopt2_ckwargs_key91 __free_JustCenter      
#define _sageopt2_ckwargs_key92 __free_JustLeft         
#define _sageopt2_ckwargs_key93 __free_JustTopLeft     
#define _sageopt2_ckwargs_key94 __free_JustTopRight    
#define _sageopt2_ckwargs_key95 __free_JustTopCenter   
#define _sageopt2_ckwargs_key96 __free_JustBottomLeft  
#define _sageopt2_ckwargs_key97 __free_JustBottomRight 
#define _sageopt2_ckwargs_key98 __free_JustBottomCenter

#define _sageopt2_ckwargs_key99  __free_TextCenter      
#define _sageopt2_ckwargs_key100 __free_Center
#define _sageopt2_ckwargs_key101 __free_TextCenterX
#define _sageopt2_ckwargs_key102 __free_TextCenterY
#define _sageopt2_ckwargs_key103 _CenterXY
#define _sageopt2_ckwargs_key104 __free_JustCenterX
#define _sageopt2_ckwargs_key105 __free_JustCenterY

// Sliders

#define _sageopt2_ckwargs_key106 _WinColors                 // bool 
#define _sageopt2_ckwargs_key107 _ValueColor                // RgbColor 
#define _sageopt2_ckwargs_key108 _ValueColorStr             // const char * 
#define _sageopt2_ckwargs_key109 _MinValue                  // int 
#define _sageopt2_ckwargs_key110 _MinValuef                 // float 
#define _sageopt2_ckwargs_key111 _MaxValue                  // int 
#define _sageopt2_ckwargs_key112 _MaxValuef                 // float 
#define _sageopt2_ckwargs_key113 _Default                   // int 
#define _sageopt2_ckwargs_key114 _Defaultf                  // float 
#define _sageopt2_ckwargs_key115 _Range                     // POINT 
#define _sageopt2_ckwargs_key116 _Rangef                    // CfPointf 
#define _sageopt2_ckwargs_key117 _Group                     // const char * 
#define _sageopt2_ckwargs_key118 _GroupID                   // int 
#define _sageopt2_ckwargs_key119 _Horizontal                // bool 
#define _sageopt2_ckwargs_key120 _Vertical                  // bool 
#define _sageopt2_ckwargs_key121 _ShowValue                 // bool 
#define _sageopt2_ckwargs_key122 _WinToolTip                // bool             --> Private
#define _sageopt2_ckwargs_key123 _EnableFocusBox            // bool             --> Private
#define _sageopt2_ckwargs_key124 _Style                     // const char * 
#define _sageopt2_ckwargs_key125 _Bottom                    // bool             --> Private
#define _sageopt2_ckwargs_key126 _Right                     // bool             --> Private
#define _sageopt2_ckwargs_key127 _Disabled                  // bool 
#define _sageopt2_ckwargs_key128 _Label                     // const char * 
#define _sageopt2_ckwargs_key129 _ValueFont                 // int 
#define _sageopt2_ckwargs_key130 _ValueFontf                // float 
#define _sageopt2_ckwargs_key131 _ValueFontStr              // const char * 
#define _sageopt2_ckwargs_key132 _AllowRollover             // bool 
#define _sageopt2_ckwargs_key133 _Width                     // int 
#define _sageopt2_ckwargs_key134 _Widthf                    // float 
#define _sageopt2_ckwargs_key135 _Height                    // int 
#define _sageopt2_ckwargs_key136 _Heightf                   // float 
#define _sageopt2_ckwargs_key137 _GroupInt                  // int 
#define _sageopt2_ckwargs_key138 _LabelFontStr              // const char *
#define _sageopt2_ckwargs_key139 _LabelFont                 // int
#define _sageopt2_ckwargs_key140 _LabelFontf                // float
#define _sageopt2_ckwargs_key141 _LabelColorStr             // const char *
#define _sageopt2_ckwargs_key142 _LabelColor                // RgbColor
#define _sageopt2_ckwargs_key143 _LabelSetXPos              // int
#define _sageopt2_ckwargs_key144 _LabelX                    // int
#define _sageopt2_ckwargs_key145 _AddArrowBox               // bool
#define _sageopt2_ckwargs_key146 _JustLabelLeft             // bool
#define _sageopt2_ckwargs_key147 _JustLabelRight            // bool
#define _sageopt2_ckwargs_key148 _JustLabelTop              // bool
#define _sageopt2_ckwargs_key149 _JustLabelBottom           // bool

// Edit Boxes (Input Boxes)

#define _sageopt2_ckwargs_key150 ___NoExdent                // bool  (priv)
#define _sageopt2_ckwargs_key151 ___TempEmptyOk             // bool  (priv)
#define _sageopt2_ckwargs_key152 ___EmptyBlankErr           // bool  (priv)
#define _sageopt2_ckwargs_key153 _MultiLine                 // bool
#define _sageopt2_ckwargs_key154 _Password                  // bool
#define _sageopt2_ckwargs_key155 _VScroll                   // bool    
#define _sageopt2_ckwargs_key156 _HScroll                   // bool    
#define _sageopt2_ckwargs_key157 _ThickBorder               // bool
#define _sageopt2_ckwargs_key158 _Columns                   // int
#define _sageopt2_ckwargs_key159 __free_JustTextLeft              // bool    (Changed to TextLeft in opt::)
#define _sageopt2_ckwargs_key160 __free_JustTextRight             // bool    (Changed to TextRight in opt::)
#define _sageopt2_ckwargs_key161 _ReadOnly                  // bool
#define _sageopt2_ckwargs_key162 _FloatsOnly                // bool
#define _sageopt2_ckwargs_key163 _NumbersOnly               // bool
#define _sageopt2_ckwargs_key164 _StartGroup                // bool 
#define _sageopt2_ckwargs_key165 _EndGroup                  // bool
#define _sageopt2_ckwargs_key166 _TabStop                   // bool
#define _sageopt2_ckwargs_key167 _WantReturn                // bool
#define _sageopt2_ckwargs_key168 _AllowScroll               // bool 
#define _sageopt2_ckwargs_key169 _AllowHScroll              // bool 
#define _sageopt2_ckwargs_key170 _AttachButton              // int (button id)      // Puts out ButtonName = in opt:: 
#define _sageopt2_ckwargs_key171 _AttachButtonStr           // const char * 
#define _sageopt2_ckwargs_key172 _AllowVScroll              // bool 

// Buttons

#define _sageopt2_ckwargs_key173 _Checked                    // bool
#define _sageopt2_ckwargs_key174 _FontHigh                   // int
#define _sageopt2_ckwargs_key175 _FontHighStr                // const char *
#define _sageopt2_ckwargs_key176 _FontChecked                // int
#define _sageopt2_ckwargs_key177 _FontCheckedStr             // const char *
#define _sageopt2_ckwargs_key178 _fgHigh                     // RgbColor
#define _sageopt2_ckwargs_key179 _fgHighStr                  // const char *
#define _sageopt2_ckwargs_key180 _bgHigh                     // RgbColor
#define _sageopt2_ckwargs_key181 _bgHighStr                  // const char *
#define _sageopt2_ckwargs_key182 _bgChecked                  // RgbColor
#define _sageopt2_ckwargs_key183 _bgCheckedStr               // const char *
#define _sageopt2_ckwargs_key184 _fgChecked                  // RgbColor
#define _sageopt2_ckwargs_key185 _fgCheckedStr               // const char *
#define _sageopt2_ckwargs_key186 _cbTitleCell                // conat char *
#define _sageopt2_ckwargs_key187 _Checkbox                   // bool
#define _sageopt2_ckwargs_key188 _RadioButton                // bool
#define _sageopt2_ckwargs_key189 _ValidateGroup              // const char *
#define _sageopt2_ckwargs_key190 _ValidateGroupNum           // int
#define _sageopt2_ckwargs_key191 _RadioGroup                 // const char *
#define _sageopt2_ckwargs_key192 _RadioGroupID               // int

// FuncFindOptions

#define _sageopt2_ckwargs_key193 _Rows                      // int
#define _sageopt2_ckwargs_key194 _MaxCharWidth              // int
#define _sageopt2_ckwargs_key195 _Size                      // int
#define _sageopt2_ckwargs_key196 _Sizef                     // float
#define _sageopt2_ckwargs_key197 _AddNewLine                // int
#define _sageopt2_ckwargs_key198 _UseLast                   // bool - probably not used...  make private
#define _sageopt2_ckwargs_key199 _AllowEsc                  // bool
#define _sageopt2_ckwargs_key200 _AllowBlanks               // bool
#define _sageopt2_ckwargs_key201 _NoBlanks                  // bool

// Others

#define _sageopt2_ckwargs_key202 _AllowClose                // bool
#define _sageopt2_ckwargs_key203 _NoAutoClose               // bool
#define _sageopt2_ckwargs_key204 _InnerSize                 // bool

// CWindows Windows

#define _sageopt2_ckwargs_key205 ___priv_Embed              // bool
#define _sageopt2_ckwargs_key206 _AllowDrag                 // bool
#define _sageopt2_ckwargs_key207 _Popup                     // bool
#define _sageopt2_ckwargs_key208 _Modal                     // bool
#define _sageopt2_ckwargs_key209 _UsingSysMenu              // bool
#define _sageopt2_ckwargs_key210 ___priv_AddShadow          // bool
#define _sageopt2_ckwargs_key211 _CancelOk                  // bool
#define _sageopt2_ckwargs_key212 _HideCancel                // bool
#define _sageopt2_ckwargs_key213 _ProgressBar               // bool
#define _sageopt2_ckwargs_key214 _CenterWin                 // bool

// Quick Dialog

#define _sageopt2_ckwargs_key215 _NoCancel                  // bool
#define _sageopt2_ckwargs_key216 _NoAutoHide                // bool
#define _sageopt2_ckwargs_key217 ___priv_CenterDesktop      // bool

// Text Widget

#define _sageopt2_ckwargs_key218 _WriteShadow               // bool !! free
#define _sageopt2_ckwargs_key219 __free_JustTextTop               // bool
#define _sageopt2_ckwargs_key220 __freeJustTextBottom            // bool

// Others / Stragglers

#define _sageopt2_ckwargs_key221 _SetTopmost                // bool
#define _sageopt2_ckwargs_key222 _SetSize                   // DualFloatInt
#define _sageopt2_ckwargs_key223 _MaxSize                   // DualFloatInt
#define _sageopt2_ckwargs_key224 _SageIcon                  // stSageIcon_t
#define _sageopt2_ckwargs_key225 _SetIcon                   // stSetIcon_t
#define _sageopt2_ckwargs_key226 _YesNoCancel               // bool
#define _sageopt2_ckwargs_key227 _NoPadding                 // bool
#define _sageopt2_ckwargs_key228 ___priv_DefaultTitle       // const char *
#define _sageopt2_ckwargs_key229 _IconWarning               // bool
#define _sageopt2_ckwargs_key230 _IconStop	                // bool
#define _sageopt2_ckwargs_key231 _IconInfo	                // bool
#define _sageopt2_ckwargs_key232 _IconNone	                // bool
#define _sageopt2_ckwargs_key233 _UseEventThread            // bool (private)
#define _sageopt2_ckwargs_key234 _Maximize                  // bool  
#define _sageopt2_ckwargs_key235 _Minimize                  // bool
#define _sageopt2_ckwargs_key236 _NoZoom                    // bool
#define _sageopt2_ckwargs_key237 _FillZoom                  // bool
#define _sageopt2_ckwargs_key238 _Percent                   // int
#define _sageopt2_ckwargs_key239 _UpdateNow                 // bool
#define _sageopt2_ckwargs_key240 _ZoomBox                   // bool
#define _sageopt2_ckwargs_key241 _Reversed                  // bool
#define _sageopt2_ckwargs_key242 _WaitforClose              // bool
#define _sageopt2_ckwargs_key243 ___priv_DefaultWinTitle    // const char *
#define _sageopt2_ckwargs_key244 _BeforeTitle               // const char *
#define _sageopt2_ckwargs_key245 _AfterTitle                // const char *
#define _sageopt2_ckwargs_key246 _ShowWatermark             // bool
#define _sageopt2_ckwargs_key247 _BorderColorFree           // RgbColor            // Free !!
#define _sageopt2_ckwargs_key248 _BorderColorStrFree        // const char *        // Free !!
#define _sageopt2_ckwargs_key249 ___priv_Defer              // bool
#define _sageopt2_ckwargs_key250 _AutoGradientInt           // int
#define _sageopt2_ckwargs_key251 _AutoGradientBool          // bool

#define _sageopt2_ckwargs_key252 _SetCenterType             // Sage::CenterType
#define _sageopt2_ckwargs_key253 _FontH                     // HFONT
#define _sageopt2_ckwargs_key254 _SetCenterTypeStr          // const char *
#define _sageopt2_ckwargs_key255 _SetJustType               // Sage::JustType
#define _sageopt2_ckwargs_key256 _SetJustTypeStr            // const char *
#define _sageopt2_ckwargs_key257 _SetTextJustType           // Sage::JustType
#define _sageopt2_ckwargs_key258 _SetTextJustTypeStr        // const char *
#define _sageopt2_ckwargs_key259 _ValueFontH                // HFONT
#define _sageopt2_ckwargs_key260 _LabelFontH                // HFONT
#define _sageopt2_ckwargs_key261 _Filled                    // bool
#define _sageopt2_ckwargs_key262 _Fullscreen                // bool
#define _sageopt2_ckwargs_key263 _SetCapsType               // stLineCaps_t
#define _sageopt2_ckwargs_key264 _SetCapsStr                // const char *
#define _sageopt2_ckwargs_key265 _PenOpacity                // int
#define _sageopt2_ckwargs_key266 _Tension                   // float
#define _sageopt2_ckwargs_key267 _LineJoinTypeType          // Sage::LineJoinType
#define _sageopt2_ckwargs_key268 _LineJoinTypeStr           // const char *
#define _sageopt2_ckwargs_key269 _GradientColorC            // const CRgbColor *
#define _sageopt2_ckwargs_key270 _PenGradientColorC         // const CRgbColor *
#define _sageopt2_ckwargs_key271 _BorderColorC              // const CRgbColor *
#define _sageopt2_ckwargs_key272 _SetGradientPoints         // stGradientPoints_t
#define _sageopt2_ckwargs_key273 _SetPenGradientPoints      // stGradientPoints_t
#define _sageopt2_ckwargs_key274 _SetRegion                 // stRegionDef_t

#define _mouse_region_key_mrAutoDraw _LabelFontStr          // Reuse LabelFontStr space        
#define _mouse_region_key_mrBindtoWindow _Password          // Reuse _Password space   
// $$ Todo: Alias TextFont = Font

// Set the type for each keyword. Note that for this example, <array> is included in ckwargs.h to define array for use here.

using  _sageopt2_ckwargs_type1 = const char *        ;   // i.e. std::array<int,2> Range, etc.
using  _sageopt2_ckwargs_type2 = const char *        ;   // i.e. const char * Text
using  _sageopt2_ckwargs_type3 = int                 ;   // i.e. int BoderSize
using  _sageopt2_ckwargs_type4 = float               ;   // i.e. bool AddBorder
using  _sageopt2_ckwargs_type5 = int                 ;   // i.e. bool AddBorder
using  _sageopt2_ckwargs_type6 = float               ;   // i.e. bool AddBorder
using  _sageopt2_ckwargs_type7 = int                 ;   // i.e. bool AddBorder
using  _sageopt2_ckwargs_type8 = float               ;   // i.e. bool AddBorder
using  _sageopt2_ckwargs_type9 = stPenColorAndSize_t      ;   // i.e. bool AddBorder
using  _sageopt2_ckwargs_type10 = const char *       ;   // i.e. bool AddBorder // Free at the moment (old PenColor)
using  _sageopt2_ckwargs_type11 = bool               ;   // i.e. bool AddBorder
using  _sageopt2_ckwargs_type12 = POINT               ;   // i.e. bool AddBorder
using  _sageopt2_ckwargs_type13 = CfPointf             ;   // i.e. bool AddBorder
using  _sageopt2_ckwargs_type14 = int               ;   // i.e. bool AddBorder
using  _sageopt2_ckwargs_type15 = bool              ;   // i.e. bool AddBorder
using  _sageopt2_ckwargs_type16 = bool              ;   // i.e. bool AddBorder
using  _sageopt2_ckwargs_type17 = bool              ;   // i.e. bool AddBorder
using  _sageopt2_ckwargs_type18 = bool              ;   // i.e. bool AddBorder
using  _sageopt2_ckwargs_type19 = bool              ;   // _AutoMouseCapture
using  _sageopt2_ckwargs_type20 = bool              ;   // i.e. bool NoPromote
using  _sageopt2_ckwargs_type21 = int                   ;   // i.e. int BoderSize
using  _sageopt2_ckwargs_type22 = float                 ;   // i.e. bool AddBorder
using  _sageopt2_ckwargs_type23 = float                 ;   // i.e. bool AddBorder
using  _sageopt2_ckwargs_type24 = Sage::RgbColor        ;   // i.e. bool AddBorder
using  _sageopt2_ckwargs_type25 = const char *          ;   // i.e. bool AddBorder
using  _sageopt2_ckwargs_type26 = int                   ;   // i.e. int BoderSize
using  _sageopt2_ckwargs_type27 = float                 ;   // i.e. bool AddBorder
using  _sageopt2_ckwargs_type28 = float                 ;   // i.e. bool AddBorder
using  _sageopt2_ckwargs_type29 = bool                  ;   // i.e. bool AddBorder
using  _sageopt2_ckwargs_type30 = RECT                  ;   // i.e. bool AddBorder
using  _sageopt2_ckwargs_type31 = SizeRect              ;   // i.e. bool AddBorder
using  _sageopt2_ckwargs_type32 = bool          ;   // i.e. bool AddBorder
using  _sageopt2_ckwargs_type33 = bool          ;   // i.e. bool AddBorder
using  _sageopt2_ckwargs_type34 = bool          ;   // i.e. bool AddBorder
using  _sageopt2_ckwargs_type35 = bool          ;   // i.e. bool AddBorder
using  _sageopt2_ckwargs_type36 = bool          ;   // i.e. bool AddBorder
using  _sageopt2_ckwargs_type37 = bool          ;   // i.e. bool AddBorder
using  _sageopt2_ckwargs_type38 = bool          ;   // i.e. bool AddBorder
using  _sageopt2_ckwargs_type39 = const char *  ;   // i.e. bool AddBorder
using  _sageopt2_ckwargs_type40 = const char *  ;             // String (const char *)
using  _sageopt2_ckwargs_type41 = int           ;                 // Int
using  _sageopt2_ckwargs_type42 = int           ;   
using  _sageopt2_ckwargs_type43 = float         ;   
using  _sageopt2_ckwargs_type44 = const char *  ;   
using  _sageopt2_ckwargs_type45 = RgbGradient   ;   
using  _sageopt2_ckwargs_type46 = stColorGradientStr_t  ;   
using  _sageopt2_ckwargs_type47 = RgbColor   ;   
using  _sageopt2_ckwargs_type48 = const char *  ;   
using  _sageopt2_ckwargs_type49 = bool;
using  _sageopt2_ckwargs_type50 = bool;
using  _sageopt2_ckwargs_type51 = bool;
using  _sageopt2_ckwargs_type52 = bool;
using  _sageopt2_ckwargs_type53 = bool;
using  _sageopt2_ckwargs_type54 = bool;
using  _sageopt2_ckwargs_type55 = bool;
using  _sageopt2_ckwargs_type56 = bool;
using  _sageopt2_ckwargs_type57 = bool;
using  _sageopt2_ckwargs_type58 = POINT;
using  _sageopt2_ckwargs_type59 = const char *;
using  _sageopt2_ckwargs_type60 = bool;
using  _sageopt2_ckwargs_type61 = bool;
using  _sageopt2_ckwargs_type62 = bool;
using  _sageopt2_ckwargs_type63 = bool;
using  _sageopt2_ckwargs_type64 = bool;
using  _sageopt2_ckwargs_type65 = bool;
using  _sageopt2_ckwargs_type66 = bool;
using  _sageopt2_ckwargs_type67 = bool;
using  _sageopt2_ckwargs_type68 = bool;
using  _sageopt2_ckwargs_type69 = bool;
using  _sageopt2_ckwargs_type70 = bool;
using  _sageopt2_ckwargs_type71 = bool;
using  _sageopt2_ckwargs_type72 = bool;
using  _sageopt2_ckwargs_type73 = bool;
using  _sageopt2_ckwargs_type74 = bool;
using  _sageopt2_ckwargs_type75 = const char *;
using  _sageopt2_ckwargs_type76 = bool;
using  _sageopt2_ckwargs_type77 = AutoUpdateType;
using  _sageopt2_ckwargs_type78 = bool;
using  _sageopt2_ckwargs_type79 = bool;
using  _sageopt2_ckwargs_type80 = bool;
using  _sageopt2_ckwargs_type81 = bool;
using  _sageopt2_ckwargs_type82 = bool;
using  _sageopt2_ckwargs_type83 = int;
using  _sageopt2_ckwargs_type84 = int;
using  _sageopt2_ckwargs_type85 = int;
using  _sageopt2_ckwargs_type86 = int;
using  _sageopt2_ckwargs_type87 = int;
using  _sageopt2_ckwargs_type88 = bool;
using  _sageopt2_ckwargs_type89 = bool;
using  _sageopt2_ckwargs_type90 = bool;
using  _sageopt2_ckwargs_type91 = bool;
using  _sageopt2_ckwargs_type92 = bool;
using  _sageopt2_ckwargs_type93 = bool;
using  _sageopt2_ckwargs_type94 = bool;
using  _sageopt2_ckwargs_type95 = bool;
using  _sageopt2_ckwargs_type96 = bool;
using  _sageopt2_ckwargs_type97 = bool;
using  _sageopt2_ckwargs_type98 = bool;
using  _sageopt2_ckwargs_type99  = bool;
using  _sageopt2_ckwargs_type100 = bool;
using  _sageopt2_ckwargs_type101 = bool;
using  _sageopt2_ckwargs_type102 = bool;
using  _sageopt2_ckwargs_type103 = bool;
using  _sageopt2_ckwargs_type104 = bool;
using  _sageopt2_ckwargs_type105 = bool;
using  _sageopt2_ckwargs_type106 = bool         ;
using  _sageopt2_ckwargs_type107 = RgbColor     ;
using  _sageopt2_ckwargs_type108 = const char * ;
using  _sageopt2_ckwargs_type109 = int          ;
using  _sageopt2_ckwargs_type110 = float        ;
using  _sageopt2_ckwargs_type111 = int          ;
using  _sageopt2_ckwargs_type112 = float        ;
using  _sageopt2_ckwargs_type113 = int          ;
using  _sageopt2_ckwargs_type114 = float        ;
using  _sageopt2_ckwargs_type115 = POINT        ;
using  _sageopt2_ckwargs_type116 = CfPointf     ;
using  _sageopt2_ckwargs_type117 = const char * ;
using  _sageopt2_ckwargs_type118 = int          ;
using  _sageopt2_ckwargs_type119 = bool         ;
using  _sageopt2_ckwargs_type120 = bool         ;
using  _sageopt2_ckwargs_type121 = bool         ;
using  _sageopt2_ckwargs_type122 = bool         ;
using  _sageopt2_ckwargs_type123 = bool         ;
using  _sageopt2_ckwargs_type124 = const char * ;
using  _sageopt2_ckwargs_type125 = bool         ;
using  _sageopt2_ckwargs_type126 = bool         ;
using  _sageopt2_ckwargs_type127 = bool         ;
using  _sageopt2_ckwargs_type128 = const char * ;
using  _sageopt2_ckwargs_type129 = int          ;
using  _sageopt2_ckwargs_type130 = float        ;
using  _sageopt2_ckwargs_type131 = const char * ;
using  _sageopt2_ckwargs_type132 = bool         ;
using  _sageopt2_ckwargs_type133 = int          ;
using  _sageopt2_ckwargs_type134 = float        ;
using  _sageopt2_ckwargs_type135 = int          ;
using  _sageopt2_ckwargs_type136 = float        ;
using  _sageopt2_ckwargs_type137 = int          ;
using  _sageopt2_ckwargs_type138 = const char * ;
using  _sageopt2_ckwargs_type139 = int          ;
using  _sageopt2_ckwargs_type140 = float        ;
using  _sageopt2_ckwargs_type141 = const char * ;
using  _sageopt2_ckwargs_type142 = RgbColor     ;
using  _sageopt2_ckwargs_type143 = int          ;
using  _sageopt2_ckwargs_type144 = int          ;
using  _sageopt2_ckwargs_type145 = bool         ;
using  _sageopt2_ckwargs_type146 = bool         ;
using  _sageopt2_ckwargs_type147 = bool         ;
using  _sageopt2_ckwargs_type148 = bool         ;
using  _sageopt2_ckwargs_type149 = bool         ;
using  _sageopt2_ckwargs_type150 = bool         ;
using  _sageopt2_ckwargs_type151 = bool         ;
using  _sageopt2_ckwargs_type152 = bool         ;
using  _sageopt2_ckwargs_type153 = bool         ;
using  _sageopt2_ckwargs_type154 = bool         ;
using  _sageopt2_ckwargs_type155 = bool         ;
using  _sageopt2_ckwargs_type156 = bool         ;
using  _sageopt2_ckwargs_type157 = bool         ;
using  _sageopt2_ckwargs_type158 = int          ;
using  _sageopt2_ckwargs_type159 = bool         ;
using  _sageopt2_ckwargs_type160 = bool         ;
using  _sageopt2_ckwargs_type161 = bool         ;
using  _sageopt2_ckwargs_type162 = bool         ;
using  _sageopt2_ckwargs_type163 = bool         ;
using  _sageopt2_ckwargs_type164 = bool         ;
using  _sageopt2_ckwargs_type165 = bool         ;
using  _sageopt2_ckwargs_type166 = bool         ;
using  _sageopt2_ckwargs_type167 = bool         ;
using  _sageopt2_ckwargs_type168 = bool         ;
using  _sageopt2_ckwargs_type169 = bool         ;
using  _sageopt2_ckwargs_type170 = int          ;
using  _sageopt2_ckwargs_type171 = const char * ;
using  _sageopt2_ckwargs_type172 = bool         ;
using  _sageopt2_ckwargs_type173 = bool         ;
using  _sageopt2_ckwargs_type174 = int          ;
using  _sageopt2_ckwargs_type175 = const char * ;
using  _sageopt2_ckwargs_type176 = int          ;
using  _sageopt2_ckwargs_type177 = const char * ;
using  _sageopt2_ckwargs_type178 = RgbColor     ;
using  _sageopt2_ckwargs_type179 = const char * ;
using  _sageopt2_ckwargs_type180 = RgbColor          ;
using  _sageopt2_ckwargs_type181 = const char * ;
using  _sageopt2_ckwargs_type182 = RgbColor          ;
using  _sageopt2_ckwargs_type183 = const char * ;
using  _sageopt2_ckwargs_type184 = RgbColor          ;
using  _sageopt2_ckwargs_type185 = const char * ;
using  _sageopt2_ckwargs_type186 = const char * ;
using  _sageopt2_ckwargs_type187 = bool         ;
using  _sageopt2_ckwargs_type188 = bool         ;
using  _sageopt2_ckwargs_type189 = const char * ;
using  _sageopt2_ckwargs_type190 = int          ;
using  _sageopt2_ckwargs_type191 = const char * ;
using  _sageopt2_ckwargs_type192 = int          ;
using  _sageopt2_ckwargs_type193 = int          ;
using  _sageopt2_ckwargs_type194 = int          ;
using  _sageopt2_ckwargs_type195 = int          ;
using  _sageopt2_ckwargs_type196 = float        ;
using  _sageopt2_ckwargs_type197 = int          ;
using  _sageopt2_ckwargs_type198 = bool         ;
using  _sageopt2_ckwargs_type199 = bool         ;
using  _sageopt2_ckwargs_type200 = bool         ;
using  _sageopt2_ckwargs_type201 = bool         ;
using  _sageopt2_ckwargs_type202 = bool         ;
using  _sageopt2_ckwargs_type203 = bool         ;
using  _sageopt2_ckwargs_type204 = bool         ;
using  _sageopt2_ckwargs_type205 = bool         ;
using  _sageopt2_ckwargs_type206 = bool         ;
using  _sageopt2_ckwargs_type207 = bool         ;
using  _sageopt2_ckwargs_type208 = bool         ;
using  _sageopt2_ckwargs_type209 = bool         ;
using  _sageopt2_ckwargs_type210 = bool         ;
using  _sageopt2_ckwargs_type211 = bool         ;
using  _sageopt2_ckwargs_type212 = bool         ;
using  _sageopt2_ckwargs_type213 = bool         ;
using  _sageopt2_ckwargs_type214 = bool         ;
using  _sageopt2_ckwargs_type215 = bool         ;
using  _sageopt2_ckwargs_type216 = bool         ;
using  _sageopt2_ckwargs_type217 = bool         ;
using  _sageopt2_ckwargs_type218 = bool         ;
using  _sageopt2_ckwargs_type219 = bool         ;
using  _sageopt2_ckwargs_type220 = bool         ;
using  _sageopt2_ckwargs_type221 = bool         ;
using  _sageopt2_ckwargs_type222 = DualFloatInt ;
using  _sageopt2_ckwargs_type223 = DualFloatInt ;
using  _sageopt2_ckwargs_type224 = stSageIcon_t ;
using  _sageopt2_ckwargs_type225 = stSetIcon_t  ;
using  _sageopt2_ckwargs_type226 = bool         ;
using  _sageopt2_ckwargs_type227 = bool         ;
using  _sageopt2_ckwargs_type228 = const char * ;
using  _sageopt2_ckwargs_type229 = bool         ;
using  _sageopt2_ckwargs_type230 = bool         ;
using  _sageopt2_ckwargs_type231 = bool         ;
using  _sageopt2_ckwargs_type232 = bool         ;
using  _sageopt2_ckwargs_type233 = bool         ;
using  _sageopt2_ckwargs_type234 = bool         ;
using  _sageopt2_ckwargs_type235 = bool         ;
using  _sageopt2_ckwargs_type236 = bool         ;
using  _sageopt2_ckwargs_type237 = bool         ;
using  _sageopt2_ckwargs_type238 = int          ;
using  _sageopt2_ckwargs_type239 = bool         ; // UpdateNow
using  _sageopt2_ckwargs_type240 = bool         ;
using  _sageopt2_ckwargs_type241 = bool         ;
using  _sageopt2_ckwargs_type242 = bool         ;
using  _sageopt2_ckwargs_type243 = const char * ;
using  _sageopt2_ckwargs_type244 = const char * ;
using  _sageopt2_ckwargs_type245 = const char * ;
using  _sageopt2_ckwargs_type246 = bool         ;
using  _sageopt2_ckwargs_type247 = RgbColor     ;
using  _sageopt2_ckwargs_type248 = const char * ;
using  _sageopt2_ckwargs_type249 = bool         ;
using  _sageopt2_ckwargs_type250 = int          ;
using  _sageopt2_ckwargs_type251 = bool         ;
using  _sageopt2_ckwargs_type252 = CenterType   ;
using  _sageopt2_ckwargs_type253 = HFONT        ;
using  _sageopt2_ckwargs_type254 = const char * ;
using  _sageopt2_ckwargs_type255 = JustType     ;
using  _sageopt2_ckwargs_type256 = const char * ;
using  _sageopt2_ckwargs_type257 = JustType     ;
using  _sageopt2_ckwargs_type258 = const char * ;
using  _sageopt2_ckwargs_type258 = const char * ;
using  _sageopt2_ckwargs_type259 = HFONT        ;
using  _sageopt2_ckwargs_type260 = HFONT        ;
using  _sageopt2_ckwargs_type261 = bool         ;
using  _sageopt2_ckwargs_type262 = bool         ;
using  _sageopt2_ckwargs_type263 = stLineCaps_t ;
using  _sageopt2_ckwargs_type264 = const char * ;
using  _sageopt2_ckwargs_type265 = int          ;   // _PenOpacity             
using  _sageopt2_ckwargs_type266 = float        ;   // _Tension                
using  _sageopt2_ckwargs_type267 = LineJoinType ;   // _LineJoinTypeType       
using  _sageopt2_ckwargs_type268 = const char * ;   // _LineJoinTypeStr        
using  _sageopt2_ckwargs_type269 = const CRgbColor *    ;   // _GradientColorC            
using  _sageopt2_ckwargs_type270 = const CRgbColor *    ;   // _PenGradientColorFree      
using  _sageopt2_ckwargs_type271 = const CRgbColor *    ;   // _BorderColor      
using  _sageopt2_ckwargs_type272 = stGradientPoints_t   ;   // _SetGradientPoints 
using  _sageopt2_ckwargs_type273 = stGradientPoints_t   ;   // _SetPenGradientPoints 
using  _sageopt2_ckwargs_type274 = stRegionDef_t        ;   // _SetRegion 

// These sections don't need to be changed for keywords, but need to have the same number of entries as keywords defined above. 

    enum class Keywords 
    {   
        _sageopt2_ckwargs_key1, 
        _sageopt2_ckwargs_key2, 
        _sageopt2_ckwargs_key3, 
        _sageopt2_ckwargs_key4, 
        _sageopt2_ckwargs_key5, 
        _sageopt2_ckwargs_key6, 
        _sageopt2_ckwargs_key7, 
        _sageopt2_ckwargs_key8, 
        _sageopt2_ckwargs_key9, 
        _sageopt2_ckwargs_key10, 
        _sageopt2_ckwargs_key11, 
        _sageopt2_ckwargs_key12, 
        _sageopt2_ckwargs_key13, 
        _sageopt2_ckwargs_key14, 
        _sageopt2_ckwargs_key15, 
        _sageopt2_ckwargs_key16, 
        _sageopt2_ckwargs_key17, 
        _sageopt2_ckwargs_key18, 
        _sageopt2_ckwargs_key19, 
        _sageopt2_ckwargs_key20, 
        _sageopt2_ckwargs_key21, 
        _sageopt2_ckwargs_key22, 
        _sageopt2_ckwargs_key23, 
        _sageopt2_ckwargs_key24, 
        _sageopt2_ckwargs_key25, 
        _sageopt2_ckwargs_key26, 
        _sageopt2_ckwargs_key27, 
        _sageopt2_ckwargs_key28, 
        _sageopt2_ckwargs_key29, 
        _sageopt2_ckwargs_key30, 
        _sageopt2_ckwargs_key31, 
        _sageopt2_ckwargs_key32, 
        _sageopt2_ckwargs_key33, 
        _sageopt2_ckwargs_key34, 
        _sageopt2_ckwargs_key35, 
        _sageopt2_ckwargs_key36, 
        _sageopt2_ckwargs_key37, 
        _sageopt2_ckwargs_key38, 
        _sageopt2_ckwargs_key39, 
        _sageopt2_ckwargs_key40,
        _sageopt2_ckwargs_key41,
        _sageopt2_ckwargs_key42,
        _sageopt2_ckwargs_key43,
        _sageopt2_ckwargs_key44,
        _sageopt2_ckwargs_key45,
        _sageopt2_ckwargs_key46,
        _sageopt2_ckwargs_key47,
        _sageopt2_ckwargs_key48,
        _sageopt2_ckwargs_key49, 
        _sageopt2_ckwargs_key50, 
        _sageopt2_ckwargs_key51, 
        _sageopt2_ckwargs_key52, 
        _sageopt2_ckwargs_key53, 
        _sageopt2_ckwargs_key54, 
        _sageopt2_ckwargs_key55, 
        _sageopt2_ckwargs_key56, 
        _sageopt2_ckwargs_key57, 
        _sageopt2_ckwargs_key58, 
        _sageopt2_ckwargs_key59, 
        _sageopt2_ckwargs_key60, 
        _sageopt2_ckwargs_key61, 
        _sageopt2_ckwargs_key62, 
        _sageopt2_ckwargs_key63, 
        _sageopt2_ckwargs_key64, 
        _sageopt2_ckwargs_key65, 
        _sageopt2_ckwargs_key66, 
        _sageopt2_ckwargs_key67, 
        _sageopt2_ckwargs_key68, 
        _sageopt2_ckwargs_key69, 
        _sageopt2_ckwargs_key70, 
        _sageopt2_ckwargs_key71, 
        _sageopt2_ckwargs_key72,
        _sageopt2_ckwargs_key73,
        _sageopt2_ckwargs_key74,
        _sageopt2_ckwargs_key75,
        _sageopt2_ckwargs_key76,
        _sageopt2_ckwargs_key77,
        _sageopt2_ckwargs_key78,
        _sageopt2_ckwargs_key79,
        _sageopt2_ckwargs_key80,
        _sageopt2_ckwargs_key81,
        _sageopt2_ckwargs_key82,
        _sageopt2_ckwargs_key83,
        _sageopt2_ckwargs_key84,
        _sageopt2_ckwargs_key85,
        _sageopt2_ckwargs_key86,
        _sageopt2_ckwargs_key87,
        _sageopt2_ckwargs_key88,
        _sageopt2_ckwargs_key89,
        _sageopt2_ckwargs_key90,
        _sageopt2_ckwargs_key91,
        _sageopt2_ckwargs_key92,
        _sageopt2_ckwargs_key93,
        _sageopt2_ckwargs_key94,
        _sageopt2_ckwargs_key95,
        _sageopt2_ckwargs_key96,
        _sageopt2_ckwargs_key97,
        _sageopt2_ckwargs_key98,
        _sageopt2_ckwargs_key99, 
        _sageopt2_ckwargs_key100,
        _sageopt2_ckwargs_key101,
        _sageopt2_ckwargs_key102,
        _sageopt2_ckwargs_key103,
        _sageopt2_ckwargs_key104,
        _sageopt2_ckwargs_key105,
        _sageopt2_ckwargs_key106,
        _sageopt2_ckwargs_key107,
        _sageopt2_ckwargs_key108,
        _sageopt2_ckwargs_key109,
        _sageopt2_ckwargs_key110,
        _sageopt2_ckwargs_key111,
        _sageopt2_ckwargs_key112,
        _sageopt2_ckwargs_key113,
        _sageopt2_ckwargs_key114,
        _sageopt2_ckwargs_key115,
        _sageopt2_ckwargs_key116,
        _sageopt2_ckwargs_key117,
        _sageopt2_ckwargs_key118,
        _sageopt2_ckwargs_key119,
        _sageopt2_ckwargs_key120,
        _sageopt2_ckwargs_key121,
        _sageopt2_ckwargs_key122,
        _sageopt2_ckwargs_key123,
        _sageopt2_ckwargs_key124,
        _sageopt2_ckwargs_key125,
        _sageopt2_ckwargs_key126,
        _sageopt2_ckwargs_key127,
        _sageopt2_ckwargs_key128,
        _sageopt2_ckwargs_key129,
        _sageopt2_ckwargs_key130,
        _sageopt2_ckwargs_key131,
        _sageopt2_ckwargs_key132,
        _sageopt2_ckwargs_key133,
        _sageopt2_ckwargs_key134,
        _sageopt2_ckwargs_key135,
        _sageopt2_ckwargs_key136,
        _sageopt2_ckwargs_key137,
        _sageopt2_ckwargs_key138,
        _sageopt2_ckwargs_key139,
        _sageopt2_ckwargs_key140,
        _sageopt2_ckwargs_key141,
        _sageopt2_ckwargs_key142,
        _sageopt2_ckwargs_key143,
        _sageopt2_ckwargs_key144,
        _sageopt2_ckwargs_key145,
        _sageopt2_ckwargs_key146,
        _sageopt2_ckwargs_key147,
        _sageopt2_ckwargs_key148,
        _sageopt2_ckwargs_key149,
        _sageopt2_ckwargs_key150,
        _sageopt2_ckwargs_key151,
        _sageopt2_ckwargs_key152,
        _sageopt2_ckwargs_key153,
        _sageopt2_ckwargs_key154,
        _sageopt2_ckwargs_key155,
        _sageopt2_ckwargs_key156,
        _sageopt2_ckwargs_key157,
        _sageopt2_ckwargs_key158,
        _sageopt2_ckwargs_key159,
        _sageopt2_ckwargs_key160,
        _sageopt2_ckwargs_key161,
        _sageopt2_ckwargs_key162,
        _sageopt2_ckwargs_key163,
        _sageopt2_ckwargs_key164,
        _sageopt2_ckwargs_key165,
        _sageopt2_ckwargs_key166,
        _sageopt2_ckwargs_key167,
        _sageopt2_ckwargs_key168,
        _sageopt2_ckwargs_key169,
        _sageopt2_ckwargs_key170,
        _sageopt2_ckwargs_key171,
        _sageopt2_ckwargs_key172,
        _sageopt2_ckwargs_key173,
        _sageopt2_ckwargs_key174,
        _sageopt2_ckwargs_key175,
        _sageopt2_ckwargs_key176,
        _sageopt2_ckwargs_key177,
        _sageopt2_ckwargs_key178,
        _sageopt2_ckwargs_key179,
        _sageopt2_ckwargs_key180,
        _sageopt2_ckwargs_key181,
        _sageopt2_ckwargs_key182,
        _sageopt2_ckwargs_key183,
        _sageopt2_ckwargs_key184,
        _sageopt2_ckwargs_key185,
        _sageopt2_ckwargs_key186,
        _sageopt2_ckwargs_key187,
        _sageopt2_ckwargs_key188,
        _sageopt2_ckwargs_key189,
        _sageopt2_ckwargs_key190,
        _sageopt2_ckwargs_key191,
        _sageopt2_ckwargs_key192,
        _sageopt2_ckwargs_key193,
        _sageopt2_ckwargs_key194,
        _sageopt2_ckwargs_key195,
        _sageopt2_ckwargs_key196,
        _sageopt2_ckwargs_key197,
        _sageopt2_ckwargs_key198,
        _sageopt2_ckwargs_key199,
        _sageopt2_ckwargs_key200,
        _sageopt2_ckwargs_key201,
        _sageopt2_ckwargs_key202,
        _sageopt2_ckwargs_key203,
        _sageopt2_ckwargs_key204,
        _sageopt2_ckwargs_key205,
        _sageopt2_ckwargs_key206,
        _sageopt2_ckwargs_key207,
        _sageopt2_ckwargs_key208,
        _sageopt2_ckwargs_key209,
        _sageopt2_ckwargs_key210,
        _sageopt2_ckwargs_key211,
        _sageopt2_ckwargs_key212,
        _sageopt2_ckwargs_key213,
        _sageopt2_ckwargs_key214,
        _sageopt2_ckwargs_key215,
        _sageopt2_ckwargs_key216,
        _sageopt2_ckwargs_key217,
        _sageopt2_ckwargs_key218,
        _sageopt2_ckwargs_key219,
        _sageopt2_ckwargs_key220,
        _sageopt2_ckwargs_key221,
        _sageopt2_ckwargs_key222,
        _sageopt2_ckwargs_key223,
        _sageopt2_ckwargs_key224,
        _sageopt2_ckwargs_key225,
        _sageopt2_ckwargs_key226,
        _sageopt2_ckwargs_key227,
        _sageopt2_ckwargs_key228,
        _sageopt2_ckwargs_key229,
        _sageopt2_ckwargs_key230,
        _sageopt2_ckwargs_key231,
        _sageopt2_ckwargs_key232,
        _sageopt2_ckwargs_key233,
        _sageopt2_ckwargs_key234,
        _sageopt2_ckwargs_key235,
        _sageopt2_ckwargs_key236,
        _sageopt2_ckwargs_key237,
        _sageopt2_ckwargs_key238,
        _sageopt2_ckwargs_key239,
        _sageopt2_ckwargs_key240,
        _sageopt2_ckwargs_key241,
        _sageopt2_ckwargs_key242,
        _sageopt2_ckwargs_key243,
        _sageopt2_ckwargs_key244,
        _sageopt2_ckwargs_key245,
        _sageopt2_ckwargs_key246,
        _sageopt2_ckwargs_key247,
        _sageopt2_ckwargs_key248,
        _sageopt2_ckwargs_key249,
        _sageopt2_ckwargs_key250,
        _sageopt2_ckwargs_key251,
        _sageopt2_ckwargs_key252,
        _sageopt2_ckwargs_key253,
        _sageopt2_ckwargs_key254,
        _sageopt2_ckwargs_key255,
        _sageopt2_ckwargs_key256,
        _sageopt2_ckwargs_key257,
        _sageopt2_ckwargs_key258,
        _sageopt2_ckwargs_key259,
        _sageopt2_ckwargs_key260,
        _sageopt2_ckwargs_key261,
        _sageopt2_ckwargs_key262,
        _sageopt2_ckwargs_key263,
        _sageopt2_ckwargs_key264,
        _sageopt2_ckwargs_key265,
        _sageopt2_ckwargs_key266,
        _sageopt2_ckwargs_key267,
        _sageopt2_ckwargs_key268,
        _sageopt2_ckwargs_key269,
        _sageopt2_ckwargs_key270,
        _sageopt2_ckwargs_key271,
        _sageopt2_ckwargs_key272,
        _sageopt2_ckwargs_key273,
        _sageopt2_ckwargs_key274,


};

#ifdef _sageopt2_ckwargs_inc_check_items    // Limit exposure to _kw_CheckItems macro so we can re-use it with multiple ckwargs modules
                                            // (i.e. this #define never needs to be changed for multiple ckwargs uses in the same module)

    #define _sageopt2_ckwargs_CheckItems    CheckItem(_sageopt2_ckwargs_key1    );      \
                                            CheckItem(_sageopt2_ckwargs_key2    );      \
                                            CheckItem(_sageopt2_ckwargs_key3    );      \
                                            CheckItem(_sageopt2_ckwargs_key4    );      \
                                            CheckItem(_sageopt2_ckwargs_key5    );      \
                                            CheckItem(_sageopt2_ckwargs_key6    );      \
                                            CheckItem(_sageopt2_ckwargs_key7    );      \
                                            CheckItem(_sageopt2_ckwargs_key8    );      \
                                            CheckItem(_sageopt2_ckwargs_key9    );      \
                                            CheckItem(_sageopt2_ckwargs_key10    );     \
                                            CheckItem(_sageopt2_ckwargs_key11    );     \
                                            CheckItem(_sageopt2_ckwargs_key12    );     \
                                            CheckItem(_sageopt2_ckwargs_key13    );     \
                                            CheckItem(_sageopt2_ckwargs_key14    );     \
                                            CheckItem(_sageopt2_ckwargs_key15    );     \
                                            CheckItem(_sageopt2_ckwargs_key16    );     \
                                            CheckItem(_sageopt2_ckwargs_key17    );     \
                                            CheckItem(_sageopt2_ckwargs_key18    );     \
                                            CheckItem(_sageopt2_ckwargs_key19    );     \
                                            CheckItem(_sageopt2_ckwargs_key20    );     \
                                            CheckItem(_sageopt2_ckwargs_key21    );     \
                                            CheckItem(_sageopt2_ckwargs_key22    );     \
                                            CheckItem(_sageopt2_ckwargs_key23    );     \
                                            CheckItem(_sageopt2_ckwargs_key24    );     \
                                            CheckItem(_sageopt2_ckwargs_key25    );     \
                                            CheckItem(_sageopt2_ckwargs_key26    );     \
                                            CheckItem(_sageopt2_ckwargs_key27    );     \
                                            CheckItem(_sageopt2_ckwargs_key28    );     \
                                            CheckItem(_sageopt2_ckwargs_key29    );     \
                                            CheckItem(_sageopt2_ckwargs_key30    );     \
                                            CheckItem(_sageopt2_ckwargs_key31    );     \
                                            CheckItem(_sageopt2_ckwargs_key32    );     \
                                            CheckItem(_sageopt2_ckwargs_key33    );     \
                                            CheckItem(_sageopt2_ckwargs_key34    );     \
                                            CheckItem(_sageopt2_ckwargs_key35    );     \
                                            CheckItem(_sageopt2_ckwargs_key36    );     \
                                            CheckItem(_sageopt2_ckwargs_key37    );     \
                                            CheckItem(_sageopt2_ckwargs_key38    );     \
                                            CheckItem(_sageopt2_ckwargs_key39    );     \
                                            CheckItem(_sageopt2_ckwargs_key40    );     \
                                            CheckItem(_sageopt2_ckwargs_key41    );     \
                                            CheckItem(_sageopt2_ckwargs_key42    );     \
                                            CheckItem(_sageopt2_ckwargs_key43    );     \
                                            CheckItem(_sageopt2_ckwargs_key44    );     \
                                            CheckItem(_sageopt2_ckwargs_key45    );     \
                                            CheckItem(_sageopt2_ckwargs_key46    );     \
                                            CheckItem(_sageopt2_ckwargs_key47    );     \
                                            CheckItem(_sageopt2_ckwargs_key48    );     \
                                            CheckItem(_sageopt2_ckwargs_key49    );     \
                                            CheckItem(_sageopt2_ckwargs_key50    );     \
                                            CheckItem(_sageopt2_ckwargs_key51    );     \
                                            CheckItem(_sageopt2_ckwargs_key52    );     \
                                            CheckItem(_sageopt2_ckwargs_key53    );     \
                                            CheckItem(_sageopt2_ckwargs_key54    );     \
                                            CheckItem(_sageopt2_ckwargs_key55    );     \
                                            CheckItem(_sageopt2_ckwargs_key56    );     \
                                            CheckItem(_sageopt2_ckwargs_key57    );     \
                                            CheckItem(_sageopt2_ckwargs_key58    );     \
                                            CheckItem(_sageopt2_ckwargs_key59    );     \
                                            CheckItem(_sageopt2_ckwargs_key60    );     \
                                            CheckItem(_sageopt2_ckwargs_key61    );     \
                                            CheckItem(_sageopt2_ckwargs_key62    );     \
                                            CheckItem(_sageopt2_ckwargs_key63    );     \
                                            CheckItem(_sageopt2_ckwargs_key64    );     \
                                            CheckItem(_sageopt2_ckwargs_key65    );     \
                                            CheckItem(_sageopt2_ckwargs_key66    );     \
                                            CheckItem(_sageopt2_ckwargs_key67    );     \
                                            CheckItem(_sageopt2_ckwargs_key68    );     \
                                            CheckItem(_sageopt2_ckwargs_key69    );     \
                                            CheckItem(_sageopt2_ckwargs_key70    );     \
                                            CheckItem(_sageopt2_ckwargs_key71    );     \
                                            CheckItem(_sageopt2_ckwargs_key72   );      \
                                            CheckItem(_sageopt2_ckwargs_key73   );      \
                                            CheckItem(_sageopt2_ckwargs_key74   );      \
                                            CheckItem(_sageopt2_ckwargs_key75   );      \
                                            CheckItem(_sageopt2_ckwargs_key76   );      \
                                            CheckItem(_sageopt2_ckwargs_key77   );      \
                                            CheckItem(_sageopt2_ckwargs_key78   );      \
                                            CheckItem(_sageopt2_ckwargs_key79   );      \
                                            CheckItem(_sageopt2_ckwargs_key80   );      \
                                            CheckItem(_sageopt2_ckwargs_key81   );      \
                                            CheckItem(_sageopt2_ckwargs_key82   );      \
                                            CheckItem(_sageopt2_ckwargs_key83   );      \
                                            CheckItem(_sageopt2_ckwargs_key84   );      \
                                            CheckItem(_sageopt2_ckwargs_key85   );      \
                                            CheckItem(_sageopt2_ckwargs_key86   );      \
                                            CheckItem(_sageopt2_ckwargs_key87   );      \
                                            CheckItem(_sageopt2_ckwargs_key88   );      \
                                            CheckItem(_sageopt2_ckwargs_key89   );      \
                                            CheckItem(_sageopt2_ckwargs_key90   );      \
                                            CheckItem(_sageopt2_ckwargs_key91   );      \
                                            CheckItem(_sageopt2_ckwargs_key92   );      \
                                            CheckItem(_sageopt2_ckwargs_key93   );      \
                                            CheckItem(_sageopt2_ckwargs_key94   );      \
                                            CheckItem(_sageopt2_ckwargs_key95   );      \
                                            CheckItem(_sageopt2_ckwargs_key96   );      \
                                            CheckItem(_sageopt2_ckwargs_key97   );      \
                                            CheckItem(_sageopt2_ckwargs_key98   );      \
                                            CheckItem(_sageopt2_ckwargs_key99   );      \
                                            CheckItem(_sageopt2_ckwargs_key100  );      \
                                            CheckItem(_sageopt2_ckwargs_key101  );      \
                                            CheckItem(_sageopt2_ckwargs_key102  );      \
                                            CheckItem(_sageopt2_ckwargs_key103  );      \
                                            CheckItem(_sageopt2_ckwargs_key104  );      \
                                            CheckItem(_sageopt2_ckwargs_key105  );      \
                                            CheckItem(_sageopt2_ckwargs_key106  );      \
                                            CheckItem(_sageopt2_ckwargs_key107  );      \
                                            CheckItem(_sageopt2_ckwargs_key108  );      \
                                            CheckItem(_sageopt2_ckwargs_key109  );      \
                                            CheckItem(_sageopt2_ckwargs_key110  );      \
                                            CheckItem(_sageopt2_ckwargs_key111  );      \
                                            CheckItem(_sageopt2_ckwargs_key112  );      \
                                            CheckItem(_sageopt2_ckwargs_key113  );      \
                                            CheckItem(_sageopt2_ckwargs_key114  );      \
                                            CheckItem(_sageopt2_ckwargs_key115  );      \
                                            CheckItem(_sageopt2_ckwargs_key116  );      \
                                            CheckItem(_sageopt2_ckwargs_key117  );      \
                                            CheckItem(_sageopt2_ckwargs_key118  );      \
                                            CheckItem(_sageopt2_ckwargs_key119  );      \
                                            CheckItem(_sageopt2_ckwargs_key120  );      \
                                            CheckItem(_sageopt2_ckwargs_key121  );      \
                                            CheckItem(_sageopt2_ckwargs_key122  );      \
                                            CheckItem(_sageopt2_ckwargs_key123  );      \
                                            CheckItem(_sageopt2_ckwargs_key124  );      \
                                            CheckItem(_sageopt2_ckwargs_key125  );      \
                                            CheckItem(_sageopt2_ckwargs_key126  );      \
                                            CheckItem(_sageopt2_ckwargs_key127  );      \
                                            CheckItem(_sageopt2_ckwargs_key128  );      \
                                            CheckItem(_sageopt2_ckwargs_key129  );      \
                                            CheckItem(_sageopt2_ckwargs_key130  );      \
                                            CheckItem(_sageopt2_ckwargs_key131  );      \
                                            CheckItem(_sageopt2_ckwargs_key132  );      \
                                            CheckItem(_sageopt2_ckwargs_key133  );      \
                                            CheckItem(_sageopt2_ckwargs_key134  );      \
                                            CheckItem(_sageopt2_ckwargs_key135  );      \
                                            CheckItem(_sageopt2_ckwargs_key136  );      \
                                            CheckItem(_sageopt2_ckwargs_key137  );      \
                                            CheckItem(_sageopt2_ckwargs_key138  );      \
                                            CheckItem(_sageopt2_ckwargs_key139  );      \
                                            CheckItem(_sageopt2_ckwargs_key140  );      \
                                            CheckItem(_sageopt2_ckwargs_key141  );      \
                                            CheckItem(_sageopt2_ckwargs_key142  );      \
                                            CheckItem(_sageopt2_ckwargs_key143  );      \
                                            CheckItem(_sageopt2_ckwargs_key144  );      \
                                            CheckItem(_sageopt2_ckwargs_key145  );      \
                                            CheckItem(_sageopt2_ckwargs_key146  );      \
                                            CheckItem(_sageopt2_ckwargs_key147  );      \
                                            CheckItem(_sageopt2_ckwargs_key148  );      \
                                            CheckItem(_sageopt2_ckwargs_key149  );      \
                                            CheckItem(_sageopt2_ckwargs_key150  );      \
                                            CheckItem(_sageopt2_ckwargs_key151  );      \
                                            CheckItem(_sageopt2_ckwargs_key152  );      \
                                            CheckItem(_sageopt2_ckwargs_key153  );      \
                                            CheckItem(_sageopt2_ckwargs_key154  );      \
                                            CheckItem(_sageopt2_ckwargs_key155  );      \
                                            CheckItem(_sageopt2_ckwargs_key156  );      \
                                            CheckItem(_sageopt2_ckwargs_key157  );      \
                                            CheckItem(_sageopt2_ckwargs_key158  );      \
                                            CheckItem(_sageopt2_ckwargs_key159  );      \
                                            CheckItem(_sageopt2_ckwargs_key160  );      \
                                            CheckItem(_sageopt2_ckwargs_key161  );      \
                                            CheckItem(_sageopt2_ckwargs_key162  );      \
                                            CheckItem(_sageopt2_ckwargs_key163  );      \
                                            CheckItem(_sageopt2_ckwargs_key164  );      \
                                            CheckItem(_sageopt2_ckwargs_key165  );      \
                                            CheckItem(_sageopt2_ckwargs_key166  );      \
                                            CheckItem(_sageopt2_ckwargs_key167  );      \
                                            CheckItem(_sageopt2_ckwargs_key168  );      \
                                            CheckItem(_sageopt2_ckwargs_key169  );      \
                                            CheckItem(_sageopt2_ckwargs_key170  );      \
                                            CheckItem(_sageopt2_ckwargs_key171  );      \
                                            CheckItem(_sageopt2_ckwargs_key172  );      \
                                            CheckItem(_sageopt2_ckwargs_key173  );      \
                                            CheckItem(_sageopt2_ckwargs_key174  );      \
                                            CheckItem(_sageopt2_ckwargs_key175  );      \
                                            CheckItem(_sageopt2_ckwargs_key176  );      \
                                            CheckItem(_sageopt2_ckwargs_key177  );      \
                                            CheckItem(_sageopt2_ckwargs_key178  );      \
                                            CheckItem(_sageopt2_ckwargs_key179  );      \
                                            CheckItem(_sageopt2_ckwargs_key180  );      \
                                            CheckItem(_sageopt2_ckwargs_key181  );      \
                                            CheckItem(_sageopt2_ckwargs_key182  );      \
                                            CheckItem(_sageopt2_ckwargs_key183  );      \
                                            CheckItem(_sageopt2_ckwargs_key184  );      \
                                            CheckItem(_sageopt2_ckwargs_key185  );      \
                                            CheckItem(_sageopt2_ckwargs_key186  );      \
                                            CheckItem(_sageopt2_ckwargs_key187  );      \
                                            CheckItem(_sageopt2_ckwargs_key188  );      \
                                            CheckItem(_sageopt2_ckwargs_key189  );      \
                                            CheckItem(_sageopt2_ckwargs_key190  );      \
                                            CheckItem(_sageopt2_ckwargs_key191  );      \
                                            CheckItem(_sageopt2_ckwargs_key192  );      \
                                            CheckItem(_sageopt2_ckwargs_key193  );      \
                                            CheckItem(_sageopt2_ckwargs_key194  );      \
                                            CheckItem(_sageopt2_ckwargs_key195  );      \
                                            CheckItem(_sageopt2_ckwargs_key196  );      \
                                            CheckItem(_sageopt2_ckwargs_key197  );      \
                                            CheckItem(_sageopt2_ckwargs_key198  );      \
                                            CheckItem(_sageopt2_ckwargs_key199  );      \
                                            CheckItem(_sageopt2_ckwargs_key200  );      \
                                            CheckItem(_sageopt2_ckwargs_key201  );      \
                                            CheckItem(_sageopt2_ckwargs_key202  );      \
                                            CheckItem(_sageopt2_ckwargs_key203  );      \
                                            CheckItem(_sageopt2_ckwargs_key204  );      \
                                            CheckItem(_sageopt2_ckwargs_key205  );      \
                                            CheckItem(_sageopt2_ckwargs_key206  );      \
                                            CheckItem(_sageopt2_ckwargs_key207  );      \
                                            CheckItem(_sageopt2_ckwargs_key208  );      \
                                            CheckItem(_sageopt2_ckwargs_key209  );      \
                                            CheckItem(_sageopt2_ckwargs_key210  );      \
                                            CheckItem(_sageopt2_ckwargs_key211  );      \
                                            CheckItem(_sageopt2_ckwargs_key212  );      \
                                            CheckItem(_sageopt2_ckwargs_key213  );      \
                                            CheckItem(_sageopt2_ckwargs_key214  );      \
                                            CheckItem(_sageopt2_ckwargs_key215  );      \
                                            CheckItem(_sageopt2_ckwargs_key216  );      \
                                            CheckItem(_sageopt2_ckwargs_key217  );      \
                                            CheckItem(_sageopt2_ckwargs_key218  );      \
                                            CheckItem(_sageopt2_ckwargs_key219  );      \
                                            CheckItem(_sageopt2_ckwargs_key221  );      \
                                            CheckItem(_sageopt2_ckwargs_key220  );      \
                                            CheckItem(_sageopt2_ckwargs_key222  );      \
                                            CheckItem(_sageopt2_ckwargs_key223  );      \
                                            CheckItem(_sageopt2_ckwargs_key224  );      \
                                            CheckItem(_sageopt2_ckwargs_key225  );      \
                                            CheckItem(_sageopt2_ckwargs_key226  );      \
                                            CheckItem(_sageopt2_ckwargs_key227  );      \
                                            CheckItem(_sageopt2_ckwargs_key228  );      \
                                            CheckItem(_sageopt2_ckwargs_key229  );      \
                                            CheckItem(_sageopt2_ckwargs_key230  );      \
                                            CheckItem(_sageopt2_ckwargs_key231  );      \
                                            CheckItem(_sageopt2_ckwargs_key232  );      \
                                            CheckItem(_sageopt2_ckwargs_key233  );      \
                                            CheckItem(_sageopt2_ckwargs_key234  );      \
                                            CheckItem(_sageopt2_ckwargs_key235  );      \
                                            CheckItem(_sageopt2_ckwargs_key236  );      \
                                            CheckItem(_sageopt2_ckwargs_key237  );      \
                                            CheckItem(_sageopt2_ckwargs_key238  );      \
                                            CheckItem(_sageopt2_ckwargs_key239  );      \
                                            CheckItem(_sageopt2_ckwargs_key240  );      \
                                            CheckItem(_sageopt2_ckwargs_key241  );      \
                                            CheckItem(_sageopt2_ckwargs_key242  );      \
                                            CheckItem(_sageopt2_ckwargs_key243  );      \
                                            CheckItem(_sageopt2_ckwargs_key244  );      \
                                            CheckItem(_sageopt2_ckwargs_key245  );      \
                                            CheckItem(_sageopt2_ckwargs_key246  );      \
                                            CheckItem(_sageopt2_ckwargs_key247  );      \
                                            CheckItem(_sageopt2_ckwargs_key248  );      \
                                            CheckItem(_sageopt2_ckwargs_key249  );      \
                                            CheckItem(_sageopt2_ckwargs_key250  );      \
                                            CheckItem(_sageopt2_ckwargs_key251  );      \
                                            CheckItem(_sageopt2_ckwargs_key252  );      \
                                            CheckItem(_sageopt2_ckwargs_key253  );      \
                                            CheckItem(_sageopt2_ckwargs_key254  );      \
                                            CheckItem(_sageopt2_ckwargs_key255  );      \
                                            CheckItem(_sageopt2_ckwargs_key256  );      \
                                            CheckItem(_sageopt2_ckwargs_key257  );      \
                                            CheckItem(_sageopt2_ckwargs_key258  );      \
                                            CheckItem(_sageopt2_ckwargs_key259  );      \
                                            CheckItem(_sageopt2_ckwargs_key260  );      \
                                            CheckItem(_sageopt2_ckwargs_key261  );      \
                                            CheckItem(_sageopt2_ckwargs_key262  );      \
                                            CheckItem(_sageopt2_ckwargs_key263  );      \
                                            CheckItem(_sageopt2_ckwargs_key264  );      \
                                            CheckItem(_sageopt2_ckwargs_key265  );      \
                                            CheckItem(_sageopt2_ckwargs_key266  );      \
                                            CheckItem(_sageopt2_ckwargs_key267  );      \
                                            CheckItem(_sageopt2_ckwargs_key268  );      \
                                            CheckItem(_sageopt2_ckwargs_key269  );      \
                                            CheckItem(_sageopt2_ckwargs_key270  );      \
                                            CheckItem(_sageopt2_ckwargs_key271  );      \
                                            CheckItem(_sageopt2_ckwargs_key272  );      \
                                            CheckItem(_sageopt2_ckwargs_key273  );      \
                                            CheckItem(_sageopt2_ckwargs_key274  );
#endif
    // -----------------
    // Key Value storage
    // -----------------
    //  
    // This is currently set as a union.  It can be a struct to allow container items with constructors/destructors/etc., but
    // is not recommended.  It's recommended to use references as the input for the keywords for class objects. 
    
    union KeyValues     // Optionally a struct for more flexibility (and headaches)
    {
        _sageopt2_ckwargs_type1         _sageopt2_ckwargs_key1 ;
        _sageopt2_ckwargs_type2         _sageopt2_ckwargs_key2 ;    
        _sageopt2_ckwargs_type3         _sageopt2_ckwargs_key3 ;
        _sageopt2_ckwargs_type4         _sageopt2_ckwargs_key4 ;
        _sageopt2_ckwargs_type5         _sageopt2_ckwargs_key5 ;
        _sageopt2_ckwargs_type6         _sageopt2_ckwargs_key6 ;
        _sageopt2_ckwargs_type7         _sageopt2_ckwargs_key7 ;
        _sageopt2_ckwargs_type8         _sageopt2_ckwargs_key8 ;
        _sageopt2_ckwargs_type9         _sageopt2_ckwargs_key9 ;
        _sageopt2_ckwargs_type10        _sageopt2_ckwargs_key10 ;
        _sageopt2_ckwargs_type11        _sageopt2_ckwargs_key11 ;
        _sageopt2_ckwargs_type12        _sageopt2_ckwargs_key12 ;
        _sageopt2_ckwargs_type13        _sageopt2_ckwargs_key13 ;
        _sageopt2_ckwargs_type14        _sageopt2_ckwargs_key14 ;
        _sageopt2_ckwargs_type15        _sageopt2_ckwargs_key15 ;
        _sageopt2_ckwargs_type16        _sageopt2_ckwargs_key16 ;
        _sageopt2_ckwargs_type17        _sageopt2_ckwargs_key17 ;
        _sageopt2_ckwargs_type18        _sageopt2_ckwargs_key18 ;
        _sageopt2_ckwargs_type19        _sageopt2_ckwargs_key19 ;
        _sageopt2_ckwargs_type20        _sageopt2_ckwargs_key20 ;
        _sageopt2_ckwargs_type21        _sageopt2_ckwargs_key21 ;
        _sageopt2_ckwargs_type22        _sageopt2_ckwargs_key22 ;
        _sageopt2_ckwargs_type23        _sageopt2_ckwargs_key23 ;
        _sageopt2_ckwargs_type24        _sageopt2_ckwargs_key24 ;
        _sageopt2_ckwargs_type25        _sageopt2_ckwargs_key25 ;
        _sageopt2_ckwargs_type26        _sageopt2_ckwargs_key26 ;
        _sageopt2_ckwargs_type27        _sageopt2_ckwargs_key27 ;
        _sageopt2_ckwargs_type28        _sageopt2_ckwargs_key28 ;
        _sageopt2_ckwargs_type29        _sageopt2_ckwargs_key29 ;
        _sageopt2_ckwargs_type30        _sageopt2_ckwargs_key30 ;
        _sageopt2_ckwargs_type31        _sageopt2_ckwargs_key31 ;
        _sageopt2_ckwargs_type32        _sageopt2_ckwargs_key32 ;
        _sageopt2_ckwargs_type33        _sageopt2_ckwargs_key33 ;
        _sageopt2_ckwargs_type34        _sageopt2_ckwargs_key34 ;
        _sageopt2_ckwargs_type35        _sageopt2_ckwargs_key35 ;
        _sageopt2_ckwargs_type36        _sageopt2_ckwargs_key36 ;
        _sageopt2_ckwargs_type37        _sageopt2_ckwargs_key37 ;
        _sageopt2_ckwargs_type38        _sageopt2_ckwargs_key38 ;
        _sageopt2_ckwargs_type39        _sageopt2_ckwargs_key39 ;
        _sageopt2_ckwargs_type40        _sageopt2_ckwargs_key40 ;
        _sageopt2_ckwargs_type41        _sageopt2_ckwargs_key41 ;
        _sageopt2_ckwargs_type42        _sageopt2_ckwargs_key42 ;
        _sageopt2_ckwargs_type43        _sageopt2_ckwargs_key43 ;
        _sageopt2_ckwargs_type44        _sageopt2_ckwargs_key44 ;
        _sageopt2_ckwargs_type45        _sageopt2_ckwargs_key45 ;
        _sageopt2_ckwargs_type46        _sageopt2_ckwargs_key46 ;
        _sageopt2_ckwargs_type47        _sageopt2_ckwargs_key47 ;
        _sageopt2_ckwargs_type48        _sageopt2_ckwargs_key48 ;
        _sageopt2_ckwargs_type49        _sageopt2_ckwargs_key49 ;
        _sageopt2_ckwargs_type50        _sageopt2_ckwargs_key50 ;
        _sageopt2_ckwargs_type51        _sageopt2_ckwargs_key51 ;
        _sageopt2_ckwargs_type52        _sageopt2_ckwargs_key52 ;
        _sageopt2_ckwargs_type53        _sageopt2_ckwargs_key53 ;
        _sageopt2_ckwargs_type54        _sageopt2_ckwargs_key54 ;
        _sageopt2_ckwargs_type55        _sageopt2_ckwargs_key55 ;
        _sageopt2_ckwargs_type56        _sageopt2_ckwargs_key56 ;
        _sageopt2_ckwargs_type57        _sageopt2_ckwargs_key57 ;
        _sageopt2_ckwargs_type58        _sageopt2_ckwargs_key58 ;
        _sageopt2_ckwargs_type59        _sageopt2_ckwargs_key59 ;
        _sageopt2_ckwargs_type60        _sageopt2_ckwargs_key60 ;
        _sageopt2_ckwargs_type61        _sageopt2_ckwargs_key61 ;
        _sageopt2_ckwargs_type62        _sageopt2_ckwargs_key62 ;
        _sageopt2_ckwargs_type63        _sageopt2_ckwargs_key63 ;
        _sageopt2_ckwargs_type64        _sageopt2_ckwargs_key64 ;
        _sageopt2_ckwargs_type65        _sageopt2_ckwargs_key65 ;
        _sageopt2_ckwargs_type66        _sageopt2_ckwargs_key66 ;
        _sageopt2_ckwargs_type67        _sageopt2_ckwargs_key67 ;
        _sageopt2_ckwargs_type68        _sageopt2_ckwargs_key68 ;
        _sageopt2_ckwargs_type69        _sageopt2_ckwargs_key69 ;
        _sageopt2_ckwargs_type70        _sageopt2_ckwargs_key70 ;
        _sageopt2_ckwargs_type71        _sageopt2_ckwargs_key71 ;
        _sageopt2_ckwargs_type72        _sageopt2_ckwargs_key72 ;
        _sageopt2_ckwargs_type73        _sageopt2_ckwargs_key73 ;
        _sageopt2_ckwargs_type74        _sageopt2_ckwargs_key74 ;
        _sageopt2_ckwargs_type75        _sageopt2_ckwargs_key75 ;
        _sageopt2_ckwargs_type76        _sageopt2_ckwargs_key76 ;
        _sageopt2_ckwargs_type77        _sageopt2_ckwargs_key77 ;
        _sageopt2_ckwargs_type78        _sageopt2_ckwargs_key78 ;
        _sageopt2_ckwargs_type79        _sageopt2_ckwargs_key79 ;
        _sageopt2_ckwargs_type80        _sageopt2_ckwargs_key80 ;
        _sageopt2_ckwargs_type81        _sageopt2_ckwargs_key81 ;
        _sageopt2_ckwargs_type82        _sageopt2_ckwargs_key82 ;
        _sageopt2_ckwargs_type83        _sageopt2_ckwargs_key83 ;
        _sageopt2_ckwargs_type84        _sageopt2_ckwargs_key84 ;
        _sageopt2_ckwargs_type85        _sageopt2_ckwargs_key85 ;
        _sageopt2_ckwargs_type86        _sageopt2_ckwargs_key86 ;
        _sageopt2_ckwargs_type87        _sageopt2_ckwargs_key87 ;
        _sageopt2_ckwargs_type88        _sageopt2_ckwargs_key88 ;
        _sageopt2_ckwargs_type89        _sageopt2_ckwargs_key89 ;
        _sageopt2_ckwargs_type90        _sageopt2_ckwargs_key90 ;
        _sageopt2_ckwargs_type91        _sageopt2_ckwargs_key91 ;
        _sageopt2_ckwargs_type92        _sageopt2_ckwargs_key92 ;
        _sageopt2_ckwargs_type93        _sageopt2_ckwargs_key93 ;
        _sageopt2_ckwargs_type94        _sageopt2_ckwargs_key94 ;
        _sageopt2_ckwargs_type95        _sageopt2_ckwargs_key95 ;
        _sageopt2_ckwargs_type96        _sageopt2_ckwargs_key96 ;
        _sageopt2_ckwargs_type97        _sageopt2_ckwargs_key97 ;
        _sageopt2_ckwargs_type98        _sageopt2_ckwargs_key98 ;
        _sageopt2_ckwargs_type99        _sageopt2_ckwargs_key99 ;
        _sageopt2_ckwargs_type100       _sageopt2_ckwargs_key100 ;
        _sageopt2_ckwargs_type101       _sageopt2_ckwargs_key101 ;
        _sageopt2_ckwargs_type102       _sageopt2_ckwargs_key102 ;
        _sageopt2_ckwargs_type103       _sageopt2_ckwargs_key103 ;
        _sageopt2_ckwargs_type104       _sageopt2_ckwargs_key104 ;
        _sageopt2_ckwargs_type105       _sageopt2_ckwargs_key105 ;
        _sageopt2_ckwargs_type106       _sageopt2_ckwargs_key106 ;
        _sageopt2_ckwargs_type107       _sageopt2_ckwargs_key107 ;
        _sageopt2_ckwargs_type108       _sageopt2_ckwargs_key108 ;
        _sageopt2_ckwargs_type109       _sageopt2_ckwargs_key109 ;
        _sageopt2_ckwargs_type110       _sageopt2_ckwargs_key110 ;
        _sageopt2_ckwargs_type111       _sageopt2_ckwargs_key111 ;
        _sageopt2_ckwargs_type112       _sageopt2_ckwargs_key112 ;
        _sageopt2_ckwargs_type113       _sageopt2_ckwargs_key113 ;
        _sageopt2_ckwargs_type114       _sageopt2_ckwargs_key114 ;
        _sageopt2_ckwargs_type115       _sageopt2_ckwargs_key115 ;
        _sageopt2_ckwargs_type116       _sageopt2_ckwargs_key116 ;
        _sageopt2_ckwargs_type117       _sageopt2_ckwargs_key117 ;
        _sageopt2_ckwargs_type118       _sageopt2_ckwargs_key118 ;
        _sageopt2_ckwargs_type119       _sageopt2_ckwargs_key119 ;
        _sageopt2_ckwargs_type120       _sageopt2_ckwargs_key120 ;
        _sageopt2_ckwargs_type121       _sageopt2_ckwargs_key121 ;
        _sageopt2_ckwargs_type122       _sageopt2_ckwargs_key122 ;
        _sageopt2_ckwargs_type123       _sageopt2_ckwargs_key123 ;
        _sageopt2_ckwargs_type124       _sageopt2_ckwargs_key124 ;
        _sageopt2_ckwargs_type125       _sageopt2_ckwargs_key125 ;
        _sageopt2_ckwargs_type126       _sageopt2_ckwargs_key126 ;
        _sageopt2_ckwargs_type127       _sageopt2_ckwargs_key127 ;
        _sageopt2_ckwargs_type128       _sageopt2_ckwargs_key128 ;
        _sageopt2_ckwargs_type129       _sageopt2_ckwargs_key129 ;
        _sageopt2_ckwargs_type130       _sageopt2_ckwargs_key130 ;
        _sageopt2_ckwargs_type131       _sageopt2_ckwargs_key131 ;
        _sageopt2_ckwargs_type132       _sageopt2_ckwargs_key132 ;
        _sageopt2_ckwargs_type133       _sageopt2_ckwargs_key133 ;
        _sageopt2_ckwargs_type134       _sageopt2_ckwargs_key134 ;
        _sageopt2_ckwargs_type135       _sageopt2_ckwargs_key135 ;
        _sageopt2_ckwargs_type136       _sageopt2_ckwargs_key136 ;
        _sageopt2_ckwargs_type137       _sageopt2_ckwargs_key137 ;
        _sageopt2_ckwargs_type138       _sageopt2_ckwargs_key138 ;
        _sageopt2_ckwargs_type139       _sageopt2_ckwargs_key139 ;
        _sageopt2_ckwargs_type140       _sageopt2_ckwargs_key140 ;
        _sageopt2_ckwargs_type141       _sageopt2_ckwargs_key141 ;
        _sageopt2_ckwargs_type142       _sageopt2_ckwargs_key142 ;
        _sageopt2_ckwargs_type143       _sageopt2_ckwargs_key143 ;
        _sageopt2_ckwargs_type144       _sageopt2_ckwargs_key144 ;
        _sageopt2_ckwargs_type145       _sageopt2_ckwargs_key145 ;
        _sageopt2_ckwargs_type146       _sageopt2_ckwargs_key146 ;
        _sageopt2_ckwargs_type147       _sageopt2_ckwargs_key147 ;
        _sageopt2_ckwargs_type148       _sageopt2_ckwargs_key148 ;
        _sageopt2_ckwargs_type149       _sageopt2_ckwargs_key149 ;
        _sageopt2_ckwargs_type150       _sageopt2_ckwargs_key150 ;
        _sageopt2_ckwargs_type151       _sageopt2_ckwargs_key151 ;
        _sageopt2_ckwargs_type152       _sageopt2_ckwargs_key152 ;
        _sageopt2_ckwargs_type153       _sageopt2_ckwargs_key153 ;
        _sageopt2_ckwargs_type154       _sageopt2_ckwargs_key154 ;
        _sageopt2_ckwargs_type155       _sageopt2_ckwargs_key155 ;
        _sageopt2_ckwargs_type156       _sageopt2_ckwargs_key156 ;
        _sageopt2_ckwargs_type157       _sageopt2_ckwargs_key157 ;
        _sageopt2_ckwargs_type158       _sageopt2_ckwargs_key158 ;
        _sageopt2_ckwargs_type159       _sageopt2_ckwargs_key159 ;
        _sageopt2_ckwargs_type160       _sageopt2_ckwargs_key160 ;
        _sageopt2_ckwargs_type161       _sageopt2_ckwargs_key161 ;
        _sageopt2_ckwargs_type162       _sageopt2_ckwargs_key162 ;
        _sageopt2_ckwargs_type163       _sageopt2_ckwargs_key163 ;
        _sageopt2_ckwargs_type164       _sageopt2_ckwargs_key164 ;
        _sageopt2_ckwargs_type165       _sageopt2_ckwargs_key165 ;
        _sageopt2_ckwargs_type166       _sageopt2_ckwargs_key166 ;
        _sageopt2_ckwargs_type167       _sageopt2_ckwargs_key167 ;
        _sageopt2_ckwargs_type168       _sageopt2_ckwargs_key168 ;
        _sageopt2_ckwargs_type169       _sageopt2_ckwargs_key169 ;
        _sageopt2_ckwargs_type170       _sageopt2_ckwargs_key170 ;
        _sageopt2_ckwargs_type171       _sageopt2_ckwargs_key171 ;
        _sageopt2_ckwargs_type172       _sageopt2_ckwargs_key172 ;
        _sageopt2_ckwargs_type173       _sageopt2_ckwargs_key173 ;
        _sageopt2_ckwargs_type174       _sageopt2_ckwargs_key174 ;
        _sageopt2_ckwargs_type175       _sageopt2_ckwargs_key175 ;
        _sageopt2_ckwargs_type176       _sageopt2_ckwargs_key176 ;
        _sageopt2_ckwargs_type177       _sageopt2_ckwargs_key177 ;
        _sageopt2_ckwargs_type178       _sageopt2_ckwargs_key178 ;
        _sageopt2_ckwargs_type179       _sageopt2_ckwargs_key179 ;
        _sageopt2_ckwargs_type180       _sageopt2_ckwargs_key180 ;
        _sageopt2_ckwargs_type181       _sageopt2_ckwargs_key181 ;
        _sageopt2_ckwargs_type182       _sageopt2_ckwargs_key182 ;
        _sageopt2_ckwargs_type183       _sageopt2_ckwargs_key183 ;
        _sageopt2_ckwargs_type184       _sageopt2_ckwargs_key184 ;
        _sageopt2_ckwargs_type185       _sageopt2_ckwargs_key185 ;
        _sageopt2_ckwargs_type186       _sageopt2_ckwargs_key186 ;
        _sageopt2_ckwargs_type187       _sageopt2_ckwargs_key187 ;
        _sageopt2_ckwargs_type188       _sageopt2_ckwargs_key188 ;
        _sageopt2_ckwargs_type189       _sageopt2_ckwargs_key189 ;
        _sageopt2_ckwargs_type190       _sageopt2_ckwargs_key190 ;
        _sageopt2_ckwargs_type191       _sageopt2_ckwargs_key191 ;
        _sageopt2_ckwargs_type192       _sageopt2_ckwargs_key192 ;
        _sageopt2_ckwargs_type193       _sageopt2_ckwargs_key193 ;
        _sageopt2_ckwargs_type194       _sageopt2_ckwargs_key194 ;
        _sageopt2_ckwargs_type195       _sageopt2_ckwargs_key195 ;
        _sageopt2_ckwargs_type196       _sageopt2_ckwargs_key196 ;
        _sageopt2_ckwargs_type197       _sageopt2_ckwargs_key197 ;
        _sageopt2_ckwargs_type198       _sageopt2_ckwargs_key198 ;
        _sageopt2_ckwargs_type199       _sageopt2_ckwargs_key199 ;
        _sageopt2_ckwargs_type200       _sageopt2_ckwargs_key200 ;
        _sageopt2_ckwargs_type201       _sageopt2_ckwargs_key201 ;
        _sageopt2_ckwargs_type202       _sageopt2_ckwargs_key202 ;
        _sageopt2_ckwargs_type203       _sageopt2_ckwargs_key203 ;
        _sageopt2_ckwargs_type204       _sageopt2_ckwargs_key204 ;
        _sageopt2_ckwargs_type205       _sageopt2_ckwargs_key205 ;
        _sageopt2_ckwargs_type206       _sageopt2_ckwargs_key206 ;
        _sageopt2_ckwargs_type207       _sageopt2_ckwargs_key207 ;
        _sageopt2_ckwargs_type208       _sageopt2_ckwargs_key208 ;
        _sageopt2_ckwargs_type209       _sageopt2_ckwargs_key209 ;
        _sageopt2_ckwargs_type210       _sageopt2_ckwargs_key210 ;
        _sageopt2_ckwargs_type211       _sageopt2_ckwargs_key211 ;
        _sageopt2_ckwargs_type212       _sageopt2_ckwargs_key212 ;
        _sageopt2_ckwargs_type213       _sageopt2_ckwargs_key213 ;
        _sageopt2_ckwargs_type214       _sageopt2_ckwargs_key214 ;
        _sageopt2_ckwargs_type215       _sageopt2_ckwargs_key215 ;
        _sageopt2_ckwargs_type216       _sageopt2_ckwargs_key216 ;
        _sageopt2_ckwargs_type217       _sageopt2_ckwargs_key217 ;
        _sageopt2_ckwargs_type218       _sageopt2_ckwargs_key218 ;
        _sageopt2_ckwargs_type219       _sageopt2_ckwargs_key219 ;
        _sageopt2_ckwargs_type220       _sageopt2_ckwargs_key220 ;
        _sageopt2_ckwargs_type221       _sageopt2_ckwargs_key221 ;
        _sageopt2_ckwargs_type222       _sageopt2_ckwargs_key222 ;
        _sageopt2_ckwargs_type223       _sageopt2_ckwargs_key223 ;
        _sageopt2_ckwargs_type224       _sageopt2_ckwargs_key224 ;
        _sageopt2_ckwargs_type225       _sageopt2_ckwargs_key225 ;
        _sageopt2_ckwargs_type226       _sageopt2_ckwargs_key226 ;
        _sageopt2_ckwargs_type227       _sageopt2_ckwargs_key227 ;
        _sageopt2_ckwargs_type228       _sageopt2_ckwargs_key228 ;
        _sageopt2_ckwargs_type229       _sageopt2_ckwargs_key229 ;
        _sageopt2_ckwargs_type230       _sageopt2_ckwargs_key230 ;
        _sageopt2_ckwargs_type231       _sageopt2_ckwargs_key231 ;
        _sageopt2_ckwargs_type232       _sageopt2_ckwargs_key232 ;
        _sageopt2_ckwargs_type233       _sageopt2_ckwargs_key233 ;
        _sageopt2_ckwargs_type234       _sageopt2_ckwargs_key234 ;
        _sageopt2_ckwargs_type235       _sageopt2_ckwargs_key235 ;
        _sageopt2_ckwargs_type236       _sageopt2_ckwargs_key236 ;
        _sageopt2_ckwargs_type237       _sageopt2_ckwargs_key237 ;
        _sageopt2_ckwargs_type238       _sageopt2_ckwargs_key238 ;
        _sageopt2_ckwargs_type239       _sageopt2_ckwargs_key239 ;
        _sageopt2_ckwargs_type240       _sageopt2_ckwargs_key240 ;
        _sageopt2_ckwargs_type241       _sageopt2_ckwargs_key241 ;
        _sageopt2_ckwargs_type242       _sageopt2_ckwargs_key242 ;
        _sageopt2_ckwargs_type243       _sageopt2_ckwargs_key243 ;
        _sageopt2_ckwargs_type244       _sageopt2_ckwargs_key244 ;
        _sageopt2_ckwargs_type245       _sageopt2_ckwargs_key245 ;
        _sageopt2_ckwargs_type246       _sageopt2_ckwargs_key246 ;
        _sageopt2_ckwargs_type247       _sageopt2_ckwargs_key247 ;
        _sageopt2_ckwargs_type248       _sageopt2_ckwargs_key248 ;
        _sageopt2_ckwargs_type249       _sageopt2_ckwargs_key249 ;
        _sageopt2_ckwargs_type250       _sageopt2_ckwargs_key250 ;
        _sageopt2_ckwargs_type251       _sageopt2_ckwargs_key251 ;
        _sageopt2_ckwargs_type252       _sageopt2_ckwargs_key252 ;
        _sageopt2_ckwargs_type253       _sageopt2_ckwargs_key253 ;
        _sageopt2_ckwargs_type254       _sageopt2_ckwargs_key254 ;
        _sageopt2_ckwargs_type255       _sageopt2_ckwargs_key255 ;
        _sageopt2_ckwargs_type256       _sageopt2_ckwargs_key256 ;
        _sageopt2_ckwargs_type257       _sageopt2_ckwargs_key257 ;
        _sageopt2_ckwargs_type258       _sageopt2_ckwargs_key258 ;
        _sageopt2_ckwargs_type259       _sageopt2_ckwargs_key259 ;
        _sageopt2_ckwargs_type260       _sageopt2_ckwargs_key260 ;
        _sageopt2_ckwargs_type261       _sageopt2_ckwargs_key261 ;
        _sageopt2_ckwargs_type262       _sageopt2_ckwargs_key262 ;
        _sageopt2_ckwargs_type263       _sageopt2_ckwargs_key263 ;
        _sageopt2_ckwargs_type264       _sageopt2_ckwargs_key264 ;
        _sageopt2_ckwargs_type265       _sageopt2_ckwargs_key265 ;
        _sageopt2_ckwargs_type266       _sageopt2_ckwargs_key266 ;
        _sageopt2_ckwargs_type267       _sageopt2_ckwargs_key267 ;
        _sageopt2_ckwargs_type268       _sageopt2_ckwargs_key268 ;
        _sageopt2_ckwargs_type269       _sageopt2_ckwargs_key269 ;
        _sageopt2_ckwargs_type270       _sageopt2_ckwargs_key270 ;
        _sageopt2_ckwargs_type271       _sageopt2_ckwargs_key271 ;
        _sageopt2_ckwargs_type272       _sageopt2_ckwargs_key272 ;
        _sageopt2_ckwargs_type273       _sageopt2_ckwargs_key273 ;
        _sageopt2_ckwargs_type274       _sageopt2_ckwargs_key274 ;
        
        KeyValues& operator=(const KeyValues& other)  noexcept
        {
            memcpy(this, &other, sizeof(KeyValues)); 
            return *this;
        }
        KeyValues & operator=(const KeyValues&& other) noexcept
        {
           memcpy(this, &other, sizeof(KeyValues)); 
           return *this;
        }
    };

    // ------------
    // KeyValuesPtr
    // ------------
    // 
    // Used when calling KillKeyValues() and not part of the keyword class (i.e. it's put on the stack of the function using
    // the keywords)
    //  
    // This is the exact format as KeyValues, but as pointers.  
    //
     // See the Get() functions for shortcuts on retrieving values.
    //
    struct KeyValuesPtr
    {
        _sageopt2_ckwargs_type1  * _sageopt2_ckwargs_key1 ;
        _sageopt2_ckwargs_type2  * _sageopt2_ckwargs_key2 ;
        _sageopt2_ckwargs_type3  * _sageopt2_ckwargs_key3 ;
        _sageopt2_ckwargs_type4  * _sageopt2_ckwargs_key4 ;
        _sageopt2_ckwargs_type5  * _sageopt2_ckwargs_key5 ;
        _sageopt2_ckwargs_type6  * _sageopt2_ckwargs_key6 ;
        _sageopt2_ckwargs_type7  * _sageopt2_ckwargs_key7 ;
        _sageopt2_ckwargs_type8  * _sageopt2_ckwargs_key8 ;
        _sageopt2_ckwargs_type9  * _sageopt2_ckwargs_key9 ;
        _sageopt2_ckwargs_type10 * _sageopt2_ckwargs_key10 ;
        _sageopt2_ckwargs_type11 * _sageopt2_ckwargs_key11 ;
        _sageopt2_ckwargs_type12 * _sageopt2_ckwargs_key12 ;
        _sageopt2_ckwargs_type13 * _sageopt2_ckwargs_key13 ;
        _sageopt2_ckwargs_type14 * _sageopt2_ckwargs_key14 ;
        _sageopt2_ckwargs_type15 * _sageopt2_ckwargs_key15 ;
        _sageopt2_ckwargs_type16 * _sageopt2_ckwargs_key16 ;
        _sageopt2_ckwargs_type17 * _sageopt2_ckwargs_key17 ;
        _sageopt2_ckwargs_type18 * _sageopt2_ckwargs_key18 ;
        _sageopt2_ckwargs_type19 * _sageopt2_ckwargs_key19 ;
        _sageopt2_ckwargs_type20 * _sageopt2_ckwargs_key20 ;
        _sageopt2_ckwargs_type21 * _sageopt2_ckwargs_key21 ;
        _sageopt2_ckwargs_type22 * _sageopt2_ckwargs_key22 ;
        _sageopt2_ckwargs_type23 * _sageopt2_ckwargs_key23 ;
        _sageopt2_ckwargs_type24 * _sageopt2_ckwargs_key24 ;
        _sageopt2_ckwargs_type25 * _sageopt2_ckwargs_key25 ;
        _sageopt2_ckwargs_type26 * _sageopt2_ckwargs_key26 ;
        _sageopt2_ckwargs_type27 * _sageopt2_ckwargs_key27 ;
        _sageopt2_ckwargs_type28 * _sageopt2_ckwargs_key28 ;
        _sageopt2_ckwargs_type29 * _sageopt2_ckwargs_key29 ;
        _sageopt2_ckwargs_type30 * _sageopt2_ckwargs_key30 ;
        _sageopt2_ckwargs_type31 * _sageopt2_ckwargs_key31 ;
        _sageopt2_ckwargs_type32 * _sageopt2_ckwargs_key32 ;
        _sageopt2_ckwargs_type33 * _sageopt2_ckwargs_key33 ;
        _sageopt2_ckwargs_type34 * _sageopt2_ckwargs_key34 ;
        _sageopt2_ckwargs_type35 * _sageopt2_ckwargs_key35 ;
        _sageopt2_ckwargs_type36 * _sageopt2_ckwargs_key36 ;
        _sageopt2_ckwargs_type37 * _sageopt2_ckwargs_key37 ;
        _sageopt2_ckwargs_type38 * _sageopt2_ckwargs_key38 ;
        _sageopt2_ckwargs_type39 * _sageopt2_ckwargs_key39 ;
        _sageopt2_ckwargs_type40 * _sageopt2_ckwargs_key40 ;
        _sageopt2_ckwargs_type41 * _sageopt2_ckwargs_key41 ;
        _sageopt2_ckwargs_type42 * _sageopt2_ckwargs_key42 ;
        _sageopt2_ckwargs_type43 * _sageopt2_ckwargs_key43 ;
        _sageopt2_ckwargs_type44 * _sageopt2_ckwargs_key44 ;
        _sageopt2_ckwargs_type45 * _sageopt2_ckwargs_key45 ;
        _sageopt2_ckwargs_type46 * _sageopt2_ckwargs_key46 ;
        _sageopt2_ckwargs_type47 * _sageopt2_ckwargs_key47 ;
        _sageopt2_ckwargs_type48 * _sageopt2_ckwargs_key48 ;
        _sageopt2_ckwargs_type49 * _sageopt2_ckwargs_key49 ;
        _sageopt2_ckwargs_type50 * _sageopt2_ckwargs_key50 ;
        _sageopt2_ckwargs_type51 * _sageopt2_ckwargs_key51 ;
        _sageopt2_ckwargs_type52 * _sageopt2_ckwargs_key52 ;
        _sageopt2_ckwargs_type53 * _sageopt2_ckwargs_key53 ;
        _sageopt2_ckwargs_type54 * _sageopt2_ckwargs_key54 ;
        _sageopt2_ckwargs_type55 * _sageopt2_ckwargs_key55 ;
        _sageopt2_ckwargs_type56 * _sageopt2_ckwargs_key56 ;
        _sageopt2_ckwargs_type57 * _sageopt2_ckwargs_key57 ;
        _sageopt2_ckwargs_type58 * _sageopt2_ckwargs_key58 ;
        _sageopt2_ckwargs_type59 * _sageopt2_ckwargs_key59 ;
        _sageopt2_ckwargs_type60 * _sageopt2_ckwargs_key60 ;
        _sageopt2_ckwargs_type61 * _sageopt2_ckwargs_key61 ;
        _sageopt2_ckwargs_type62 * _sageopt2_ckwargs_key62 ;
        _sageopt2_ckwargs_type63 * _sageopt2_ckwargs_key63 ;
        _sageopt2_ckwargs_type64 * _sageopt2_ckwargs_key64 ;
        _sageopt2_ckwargs_type65 * _sageopt2_ckwargs_key65 ;
        _sageopt2_ckwargs_type66 * _sageopt2_ckwargs_key66 ;
        _sageopt2_ckwargs_type67 * _sageopt2_ckwargs_key67 ;
        _sageopt2_ckwargs_type68 * _sageopt2_ckwargs_key68 ;
        _sageopt2_ckwargs_type69 * _sageopt2_ckwargs_key69 ;
        _sageopt2_ckwargs_type70 * _sageopt2_ckwargs_key70 ;
        _sageopt2_ckwargs_type71 * _sageopt2_ckwargs_key71 ;
        _sageopt2_ckwargs_type72 * _sageopt2_ckwargs_key72 ;
        _sageopt2_ckwargs_type73 * _sageopt2_ckwargs_key73 ;
        _sageopt2_ckwargs_type74 * _sageopt2_ckwargs_key74 ;
        _sageopt2_ckwargs_type75 * _sageopt2_ckwargs_key75 ;
        _sageopt2_ckwargs_type76 * _sageopt2_ckwargs_key76 ;
        _sageopt2_ckwargs_type77 * _sageopt2_ckwargs_key77 ;
        _sageopt2_ckwargs_type78 * _sageopt2_ckwargs_key78 ;
        _sageopt2_ckwargs_type79 * _sageopt2_ckwargs_key79 ;
        _sageopt2_ckwargs_type80 * _sageopt2_ckwargs_key80 ;
        _sageopt2_ckwargs_type81 * _sageopt2_ckwargs_key81 ;
        _sageopt2_ckwargs_type82 * _sageopt2_ckwargs_key82 ;
        _sageopt2_ckwargs_type83 * _sageopt2_ckwargs_key83 ;
        _sageopt2_ckwargs_type84 * _sageopt2_ckwargs_key84 ;
        _sageopt2_ckwargs_type85 * _sageopt2_ckwargs_key85 ;
        _sageopt2_ckwargs_type86 * _sageopt2_ckwargs_key86 ;
        _sageopt2_ckwargs_type87 * _sageopt2_ckwargs_key87 ;
        _sageopt2_ckwargs_type88 * _sageopt2_ckwargs_key88 ;
        _sageopt2_ckwargs_type89 * _sageopt2_ckwargs_key89 ;
        _sageopt2_ckwargs_type90 * _sageopt2_ckwargs_key90 ;
        _sageopt2_ckwargs_type91 * _sageopt2_ckwargs_key91 ;
        _sageopt2_ckwargs_type92 * _sageopt2_ckwargs_key92 ;
        _sageopt2_ckwargs_type93 * _sageopt2_ckwargs_key93 ;
        _sageopt2_ckwargs_type94 * _sageopt2_ckwargs_key94 ;
        _sageopt2_ckwargs_type95 * _sageopt2_ckwargs_key95 ;
        _sageopt2_ckwargs_type96 * _sageopt2_ckwargs_key96 ;
        _sageopt2_ckwargs_type97 * _sageopt2_ckwargs_key97 ;
        _sageopt2_ckwargs_type98 * _sageopt2_ckwargs_key98 ;
        _sageopt2_ckwargs_type99  * _sageopt2_ckwargs_key99 ;
        _sageopt2_ckwargs_type100 * _sageopt2_ckwargs_key100 ;
        _sageopt2_ckwargs_type101 * _sageopt2_ckwargs_key101 ;
        _sageopt2_ckwargs_type102 * _sageopt2_ckwargs_key102 ;
        _sageopt2_ckwargs_type103 * _sageopt2_ckwargs_key103 ;
        _sageopt2_ckwargs_type104 * _sageopt2_ckwargs_key104 ;
        _sageopt2_ckwargs_type105 * _sageopt2_ckwargs_key105 ;
        _sageopt2_ckwargs_type106 * _sageopt2_ckwargs_key106 ;
        _sageopt2_ckwargs_type107 * _sageopt2_ckwargs_key107 ;
        _sageopt2_ckwargs_type108 * _sageopt2_ckwargs_key108 ;
        _sageopt2_ckwargs_type109 * _sageopt2_ckwargs_key109 ;
        _sageopt2_ckwargs_type110 * _sageopt2_ckwargs_key110 ;
        _sageopt2_ckwargs_type111 * _sageopt2_ckwargs_key111 ;
        _sageopt2_ckwargs_type112 * _sageopt2_ckwargs_key112 ;
        _sageopt2_ckwargs_type113 * _sageopt2_ckwargs_key113 ;
        _sageopt2_ckwargs_type114 * _sageopt2_ckwargs_key114 ;
        _sageopt2_ckwargs_type115 * _sageopt2_ckwargs_key115 ;
        _sageopt2_ckwargs_type116 * _sageopt2_ckwargs_key116 ;
        _sageopt2_ckwargs_type117 * _sageopt2_ckwargs_key117 ;
        _sageopt2_ckwargs_type118 * _sageopt2_ckwargs_key118 ;
        _sageopt2_ckwargs_type119 * _sageopt2_ckwargs_key119 ;
        _sageopt2_ckwargs_type120 * _sageopt2_ckwargs_key120 ;
        _sageopt2_ckwargs_type121 * _sageopt2_ckwargs_key121 ;
        _sageopt2_ckwargs_type122 * _sageopt2_ckwargs_key122 ;
        _sageopt2_ckwargs_type123 * _sageopt2_ckwargs_key123 ;
        _sageopt2_ckwargs_type124 * _sageopt2_ckwargs_key124 ;
        _sageopt2_ckwargs_type125 * _sageopt2_ckwargs_key125 ;
        _sageopt2_ckwargs_type126 * _sageopt2_ckwargs_key126 ;
        _sageopt2_ckwargs_type127 * _sageopt2_ckwargs_key127 ;
        _sageopt2_ckwargs_type128 * _sageopt2_ckwargs_key128 ;
        _sageopt2_ckwargs_type129 * _sageopt2_ckwargs_key129 ;
        _sageopt2_ckwargs_type130 * _sageopt2_ckwargs_key130 ;
        _sageopt2_ckwargs_type131 * _sageopt2_ckwargs_key131 ;
        _sageopt2_ckwargs_type132 * _sageopt2_ckwargs_key132 ;
        _sageopt2_ckwargs_type133 * _sageopt2_ckwargs_key133 ;
        _sageopt2_ckwargs_type134 * _sageopt2_ckwargs_key134 ;
        _sageopt2_ckwargs_type135 * _sageopt2_ckwargs_key135 ;
        _sageopt2_ckwargs_type136 * _sageopt2_ckwargs_key136 ;
        _sageopt2_ckwargs_type137 * _sageopt2_ckwargs_key137 ;
        _sageopt2_ckwargs_type138 * _sageopt2_ckwargs_key138 ;
        _sageopt2_ckwargs_type139 * _sageopt2_ckwargs_key139 ;
        _sageopt2_ckwargs_type140 * _sageopt2_ckwargs_key140 ;
        _sageopt2_ckwargs_type141 * _sageopt2_ckwargs_key141 ;
        _sageopt2_ckwargs_type142 * _sageopt2_ckwargs_key142 ;
        _sageopt2_ckwargs_type143 * _sageopt2_ckwargs_key143 ;
        _sageopt2_ckwargs_type144 * _sageopt2_ckwargs_key144 ;
        _sageopt2_ckwargs_type145 * _sageopt2_ckwargs_key145 ;
        _sageopt2_ckwargs_type146 * _sageopt2_ckwargs_key146 ;
        _sageopt2_ckwargs_type147 * _sageopt2_ckwargs_key147 ;
        _sageopt2_ckwargs_type148 * _sageopt2_ckwargs_key148 ;
        _sageopt2_ckwargs_type149 * _sageopt2_ckwargs_key149 ;
        _sageopt2_ckwargs_type150 * _sageopt2_ckwargs_key150 ;
        _sageopt2_ckwargs_type151 * _sageopt2_ckwargs_key151 ;
        _sageopt2_ckwargs_type152 * _sageopt2_ckwargs_key152 ;
        _sageopt2_ckwargs_type153 * _sageopt2_ckwargs_key153 ;
        _sageopt2_ckwargs_type154 * _sageopt2_ckwargs_key154 ;
        _sageopt2_ckwargs_type155 * _sageopt2_ckwargs_key155 ;
        _sageopt2_ckwargs_type156 * _sageopt2_ckwargs_key156 ;
        _sageopt2_ckwargs_type157 * _sageopt2_ckwargs_key157 ;
        _sageopt2_ckwargs_type158 * _sageopt2_ckwargs_key158 ;
        _sageopt2_ckwargs_type159 * _sageopt2_ckwargs_key159 ;
        _sageopt2_ckwargs_type160 * _sageopt2_ckwargs_key160 ;
        _sageopt2_ckwargs_type161 * _sageopt2_ckwargs_key161 ;
        _sageopt2_ckwargs_type162 * _sageopt2_ckwargs_key162 ;
        _sageopt2_ckwargs_type163 * _sageopt2_ckwargs_key163 ;
        _sageopt2_ckwargs_type164 * _sageopt2_ckwargs_key164 ;
        _sageopt2_ckwargs_type165 * _sageopt2_ckwargs_key165 ;
        _sageopt2_ckwargs_type166 * _sageopt2_ckwargs_key166 ;
        _sageopt2_ckwargs_type167 * _sageopt2_ckwargs_key167 ;
        _sageopt2_ckwargs_type168 * _sageopt2_ckwargs_key168 ;
        _sageopt2_ckwargs_type169 * _sageopt2_ckwargs_key169 ;
        _sageopt2_ckwargs_type170 * _sageopt2_ckwargs_key170 ;
        _sageopt2_ckwargs_type171 * _sageopt2_ckwargs_key171 ;
        _sageopt2_ckwargs_type172 * _sageopt2_ckwargs_key172 ;
        _sageopt2_ckwargs_type173 * _sageopt2_ckwargs_key173 ;
        _sageopt2_ckwargs_type174 * _sageopt2_ckwargs_key174 ;
        _sageopt2_ckwargs_type175 * _sageopt2_ckwargs_key175 ;
        _sageopt2_ckwargs_type176 * _sageopt2_ckwargs_key176 ;
        _sageopt2_ckwargs_type177 * _sageopt2_ckwargs_key177 ;
        _sageopt2_ckwargs_type178 * _sageopt2_ckwargs_key178 ;
        _sageopt2_ckwargs_type179 * _sageopt2_ckwargs_key179 ;
        _sageopt2_ckwargs_type180 * _sageopt2_ckwargs_key180 ;
        _sageopt2_ckwargs_type181 * _sageopt2_ckwargs_key181 ;
        _sageopt2_ckwargs_type182 * _sageopt2_ckwargs_key182 ;
        _sageopt2_ckwargs_type183 * _sageopt2_ckwargs_key183 ;
        _sageopt2_ckwargs_type184 * _sageopt2_ckwargs_key184 ;
        _sageopt2_ckwargs_type185 * _sageopt2_ckwargs_key185 ;
        _sageopt2_ckwargs_type186 * _sageopt2_ckwargs_key186 ;
        _sageopt2_ckwargs_type187 * _sageopt2_ckwargs_key187 ;
        _sageopt2_ckwargs_type188 * _sageopt2_ckwargs_key188 ;
        _sageopt2_ckwargs_type189 * _sageopt2_ckwargs_key189 ;
        _sageopt2_ckwargs_type190 * _sageopt2_ckwargs_key190 ;
        _sageopt2_ckwargs_type191 * _sageopt2_ckwargs_key191 ;
        _sageopt2_ckwargs_type192 * _sageopt2_ckwargs_key192 ;
        _sageopt2_ckwargs_type193 * _sageopt2_ckwargs_key193 ;
        _sageopt2_ckwargs_type194 * _sageopt2_ckwargs_key194 ;
        _sageopt2_ckwargs_type195 * _sageopt2_ckwargs_key195 ;
        _sageopt2_ckwargs_type196 * _sageopt2_ckwargs_key196 ;
        _sageopt2_ckwargs_type197 * _sageopt2_ckwargs_key197 ;
        _sageopt2_ckwargs_type198 * _sageopt2_ckwargs_key198 ;
        _sageopt2_ckwargs_type199 * _sageopt2_ckwargs_key199 ;
        _sageopt2_ckwargs_type200 * _sageopt2_ckwargs_key200 ;
        _sageopt2_ckwargs_type201 * _sageopt2_ckwargs_key201 ;
        _sageopt2_ckwargs_type202 * _sageopt2_ckwargs_key202 ;
        _sageopt2_ckwargs_type203 * _sageopt2_ckwargs_key203 ;
        _sageopt2_ckwargs_type204 * _sageopt2_ckwargs_key204 ;
        _sageopt2_ckwargs_type205 * _sageopt2_ckwargs_key205 ;
        _sageopt2_ckwargs_type206 * _sageopt2_ckwargs_key206 ;
        _sageopt2_ckwargs_type207 * _sageopt2_ckwargs_key207 ;
        _sageopt2_ckwargs_type208 * _sageopt2_ckwargs_key208 ;
        _sageopt2_ckwargs_type209 * _sageopt2_ckwargs_key209 ;
        _sageopt2_ckwargs_type210 * _sageopt2_ckwargs_key210 ;
        _sageopt2_ckwargs_type211 * _sageopt2_ckwargs_key211 ;
        _sageopt2_ckwargs_type212 * _sageopt2_ckwargs_key212 ;
        _sageopt2_ckwargs_type213 * _sageopt2_ckwargs_key213 ;
        _sageopt2_ckwargs_type214 * _sageopt2_ckwargs_key214 ;
        _sageopt2_ckwargs_type215 * _sageopt2_ckwargs_key215 ;
        _sageopt2_ckwargs_type216 * _sageopt2_ckwargs_key216 ;
        _sageopt2_ckwargs_type217 * _sageopt2_ckwargs_key217 ;
        _sageopt2_ckwargs_type218 * _sageopt2_ckwargs_key218 ;
        _sageopt2_ckwargs_type219 * _sageopt2_ckwargs_key219 ;
        _sageopt2_ckwargs_type220 * _sageopt2_ckwargs_key220 ;
        _sageopt2_ckwargs_type221 * _sageopt2_ckwargs_key221 ;
        _sageopt2_ckwargs_type222 * _sageopt2_ckwargs_key222 ;
        _sageopt2_ckwargs_type223 * _sageopt2_ckwargs_key223 ;
        _sageopt2_ckwargs_type224 * _sageopt2_ckwargs_key224 ;
        _sageopt2_ckwargs_type225 * _sageopt2_ckwargs_key225 ;
        _sageopt2_ckwargs_type226 * _sageopt2_ckwargs_key226 ;
        _sageopt2_ckwargs_type227 * _sageopt2_ckwargs_key227 ;
        _sageopt2_ckwargs_type228 * _sageopt2_ckwargs_key228 ;
        _sageopt2_ckwargs_type229 * _sageopt2_ckwargs_key229 ;
        _sageopt2_ckwargs_type230 * _sageopt2_ckwargs_key230 ;
        _sageopt2_ckwargs_type231 * _sageopt2_ckwargs_key231 ;
        _sageopt2_ckwargs_type232 * _sageopt2_ckwargs_key232 ;
        _sageopt2_ckwargs_type233 * _sageopt2_ckwargs_key233 ;
        _sageopt2_ckwargs_type234 * _sageopt2_ckwargs_key234 ;
        _sageopt2_ckwargs_type235 * _sageopt2_ckwargs_key235 ;
        _sageopt2_ckwargs_type236 * _sageopt2_ckwargs_key236 ;
        _sageopt2_ckwargs_type237 * _sageopt2_ckwargs_key237 ;
        _sageopt2_ckwargs_type238 * _sageopt2_ckwargs_key238 ;
        _sageopt2_ckwargs_type239 * _sageopt2_ckwargs_key239 ;
        _sageopt2_ckwargs_type240 * _sageopt2_ckwargs_key240 ;
        _sageopt2_ckwargs_type241 * _sageopt2_ckwargs_key241 ;
        _sageopt2_ckwargs_type242 * _sageopt2_ckwargs_key242 ;
        _sageopt2_ckwargs_type243 * _sageopt2_ckwargs_key243 ;
        _sageopt2_ckwargs_type244 * _sageopt2_ckwargs_key244 ;
        _sageopt2_ckwargs_type245 * _sageopt2_ckwargs_key245 ;
        _sageopt2_ckwargs_type246 * _sageopt2_ckwargs_key246 ;
        _sageopt2_ckwargs_type247 * _sageopt2_ckwargs_key247 ;
        _sageopt2_ckwargs_type248 * _sageopt2_ckwargs_key248 ;
        _sageopt2_ckwargs_type249 * _sageopt2_ckwargs_key249 ;
        _sageopt2_ckwargs_type250 * _sageopt2_ckwargs_key250 ;
        _sageopt2_ckwargs_type251 * _sageopt2_ckwargs_key251 ;
        _sageopt2_ckwargs_type252 * _sageopt2_ckwargs_key252 ;
        _sageopt2_ckwargs_type253 * _sageopt2_ckwargs_key253 ;
        _sageopt2_ckwargs_type254 * _sageopt2_ckwargs_key254 ;
        _sageopt2_ckwargs_type255 * _sageopt2_ckwargs_key255 ;
        _sageopt2_ckwargs_type256 * _sageopt2_ckwargs_key256 ;
        _sageopt2_ckwargs_type257 * _sageopt2_ckwargs_key257 ;
        _sageopt2_ckwargs_type258 * _sageopt2_ckwargs_key258 ;
        _sageopt2_ckwargs_type259 * _sageopt2_ckwargs_key259 ;
        _sageopt2_ckwargs_type260 * _sageopt2_ckwargs_key260 ;
        _sageopt2_ckwargs_type261 * _sageopt2_ckwargs_key261 ;
        _sageopt2_ckwargs_type262 * _sageopt2_ckwargs_key262 ;
        _sageopt2_ckwargs_type263 * _sageopt2_ckwargs_key263 ;
        _sageopt2_ckwargs_type264 * _sageopt2_ckwargs_key264 ;
        _sageopt2_ckwargs_type265 * _sageopt2_ckwargs_key265 ;
        _sageopt2_ckwargs_type266 * _sageopt2_ckwargs_key266 ;
        _sageopt2_ckwargs_type267 * _sageopt2_ckwargs_key267 ;
        _sageopt2_ckwargs_type268 * _sageopt2_ckwargs_key268 ;
        _sageopt2_ckwargs_type269 * _sageopt2_ckwargs_key269 ;
        _sageopt2_ckwargs_type270 * _sageopt2_ckwargs_key270 ;
        _sageopt2_ckwargs_type271 * _sageopt2_ckwargs_key271 ;
        _sageopt2_ckwargs_type272 * _sageopt2_ckwargs_key272 ;
        _sageopt2_ckwargs_type273 * _sageopt2_ckwargs_key273 ;
        _sageopt2_ckwargs_type274 * _sageopt2_ckwargs_key274 ;

    };
} }// namespace ckwargs