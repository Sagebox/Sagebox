#pragma once
#include "CPoint.h"

// ----------------------------------
// *** Vector keyword definitions ***
// ----------------------------------

// ** Most documentation below is boilerplate comments from Ckwargs project **

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
namespace vkwType
{
   struct stDualColorStruct_t
    {
        RgbColor rgbColor1;
        RgbColor rgbColor2;
    };


   struct stLineCaps_t
   {
       LineCapType eBegCap;
       LineCapType eEndCap;
   };

    enum class TextSizeType
    {
        XXSmall      = 0,
        XSmall      = 1,
        Default     = 2,
        Small       = 2,
        Medium      = 3, 
        Large       = 4,
        XLarge      = 5,
        XXLarge     = 6,
    };

   enum class LabelJustType
   {
       Default              = 0,
       TopCenter            = 0,
       TopLeft              = 1,
       TopCenterLeft        = 2,
       TopRight             = 3,
       TopCenterRight       = 4,
       BottomCenter         = 5,
       BottomLeft           = 6,
       BottomCenterLeft     = 7,
       BottomRight          = 8,
       BottomCenterRight    = 9,
       MiddleCenter         = 10,
       MiddleLeft           = 11,
       MiddleCenterLeft     = 12,
       MiddleRight          = 13,
       MiddleCenterRight    = 14,

   };

   enum class LabelAngleType
   {
       Default      = 0,
       Horizontal   = 0,
       Vertical     = 1,
       Vertical180  = 2,
   };

    struct stAngleTypeStruct_t
   {
       LabelAngleType eLabelAngleType;
       bool bStatic;
   };

// define the keyword names used.  These are not the keywords used by the user (these come from my_keywordsh and/or my_keyfuncs.h).
// The keys below are used by the functions using the keywords as symblic names such as key.Range, key.Text, etc.

#define _vkw_ckwargs_key1 _StartCapStr      // Example Range keyword or function, i.e. Range = {5,10} or Range(5,10)
#define _vkw_ckwargs_key2 _EndCapStr        // i.e. Text = "Hello World" or Text("Hello World")
#define _vkw_ckwargs_key3 _StartCap         // i.e. BorderSize = 10, or BorderSize(10)
#define _vkw_ckwargs_key4 _EndCap           // i.e. AddBorder = true, AddBorder = false, or AddBorder() or AddBorder(true) or AddBorder(false)
#define _vkw_ckwargs_key5 _StartCapColor    
#define _vkw_ckwargs_key6 _EndCapColor      
#define _vkw_ckwargs_key7 _StartCapColorStr    
#define _vkw_ckwargs_key8 _EndCapColorStr      
#define _vkw_ckwargs_key9 _Color
#define _vkw_ckwargs_key10 _ColorStr 
#define _vkw_ckwargs_key11 _StartCapSize 
#define _vkw_ckwargs_key12 _EndCapSize 
#define _vkw_ckwargs_key13 _CapSize 
#define _vkw_ckwargs_key14 _Opacity 
#define _vkw_ckwargs_key15 _Title 
#define _vkw_ckwargs_key16 _AngleDeg        // int
#define _vkw_ckwargs_key17 _AngleDegf       // float
#define _vkw_ckwargs_key18 _AngleRad        // float
#define _vkw_ckwargs_key19 _FontH           // HFont
#define _vkw_ckwargs_key20 _FontStr         // const char *
#define _vkw_ckwargs_key21 _LabelJust       // LabelJustType
#define _vkw_ckwargs_key22 _LabelJustStr    // const char *
#define _vkw_ckwargs_key23 _LabelPadX       // int
#define _vkw_ckwargs_key24 _LabelPadY       // int
#define _vkw_ckwargs_key25 _TextSize        // TextSize      
#define _vkw_ckwargs_key26 _TextSizeStr     // const char *  
#define _vkw_ckwargs_key27 _FontMult        // int 
#define _vkw_ckwargs_key28 _FontMultf       // float 
#define _vkw_ckwargs_key29 _LabelOpacity    // int 
#define _vkw_ckwargs_key30 _LabelColor      // RgbColor 
#define _vkw_ckwargs_key31 _LabelColorStr   // const char * 
#define _vkw_ckwargs_key32 _LabelAngle      // stAngleTypeStruct_t
#define _vkw_ckwargs_key33 _LabelAngleStr   // const char * 
#define _vkw_ckwargs_key34 _LabelAlwaysUp   // bool 
#define _vkw_ckwargs_key35 _SetCenter       // CfPoint 
#define _vkw_ckwargs_key36 _ShowColorRgb    // RgbColor 
#define _vkw_ckwargs_key37 _ShowColorStr    // const char * 
#define _vkw_ckwargs_key38 _FontInt         // int 
#define _vkw_ckwargs_key39 _StaticAngle     // bool     --> Note: Only required for packed-parameter-type keywords because we can't use it in LabelAngle() with packed-parameters 
#define _vkw_ckwargs_key40 _SetCapsType     // stLineCaps_t
#define _vkw_ckwargs_key41 _SetCapsStr      // const char *

// Set the type for each keyword. Note that for this example, <array> is included in ckwargs.h to define array for use here.

using  _vkw_ckwargs_type1 = const char *        ;   // i.e. std::array<int,2> Range, etc.
using  _vkw_ckwargs_type2 = const char *        ;   // i.e. const char * Text
using  _vkw_ckwargs_type3 = LineCapType    ;   // i.e. int BoderSize
using  _vkw_ckwargs_type4 = LineCapType    ;   // i.e. bool AddBorder
using  _vkw_ckwargs_type5 = stDualColorStruct_t ;   // i.e. bool AddBorder
using  _vkw_ckwargs_type6 = stDualColorStruct_t ;   // i.e. bool AddBorder
using  _vkw_ckwargs_type7 = const char *        ;   // i.e. bool AddBorder
using  _vkw_ckwargs_type8 = const char *        ;   // i.e. bool AddBorder
using  _vkw_ckwargs_type9 = Sage::RgbColor            ;   // i.e. bool AddBorder
using  _vkw_ckwargs_type10 = const char *        ;   // i.e. bool AddBorder
using  _vkw_ckwargs_type11 = double         ;   // i.e. bool AddBorder
using  _vkw_ckwargs_type12 = double         ;   // i.e. bool AddBorder
using  _vkw_ckwargs_type13 = double         ;   // i.e. bool AddBorder
using  _vkw_ckwargs_type14 = int            ;   // i.e. bool AddBorder
using  _vkw_ckwargs_type15 = const char *   ;   // i.e. bool AddBorder
using  _vkw_ckwargs_type16 = int            ;
using  _vkw_ckwargs_type17 = float          ;
using  _vkw_ckwargs_type18 = float          ;
using  _vkw_ckwargs_type19 = HFONT          ;
using  _vkw_ckwargs_type20 = const char *   ;
using  _vkw_ckwargs_type21 = LabelJustType  ;
using  _vkw_ckwargs_type22 = const char *   ;
using  _vkw_ckwargs_type23 = int            ;
using  _vkw_ckwargs_type24 = int            ;
using  _vkw_ckwargs_type25 = TextSizeType   ;
using  _vkw_ckwargs_type26 = const char *   ;
using  _vkw_ckwargs_type27 = int            ;
using  _vkw_ckwargs_type28 = float          ;
using  _vkw_ckwargs_type29 = int            ;
using  _vkw_ckwargs_type30 = RgbColor       ;
using  _vkw_ckwargs_type31 = const char *   ;
using  _vkw_ckwargs_type32 = stAngleTypeStruct_t ;
using  _vkw_ckwargs_type33 = const char *   ;
using  _vkw_ckwargs_type34 = bool           ;
using  _vkw_ckwargs_type35 = CfPoint        ;
using  _vkw_ckwargs_type36 = RgbColor           ;
using  _vkw_ckwargs_type37 = const char     *   ;
using  _vkw_ckwargs_type38 = int    ;
using  _vkw_ckwargs_type39 = bool   ;   // --> Note: Only required for packed-parameter-type keywords because we can't use it in LabelAngle() with packed-parameters 
using  _vkw_ckwargs_type40 = stLineCaps_t   ;   
using  _vkw_ckwargs_type41 = const char *   ;   
    
// These sections don't need to be changed for keywords, but need to have the same number of entries as keywords defined above. 

    enum class Keywords 
    {   
        _vkw_ckwargs_key1, 
        _vkw_ckwargs_key2, 
        _vkw_ckwargs_key3, 
        _vkw_ckwargs_key4, 
        _vkw_ckwargs_key5, 
        _vkw_ckwargs_key6, 
        _vkw_ckwargs_key7, 
        _vkw_ckwargs_key8, 
        _vkw_ckwargs_key9, 
        _vkw_ckwargs_key10, 
        _vkw_ckwargs_key11, 
        _vkw_ckwargs_key12, 
        _vkw_ckwargs_key13, 
        _vkw_ckwargs_key14, 
        _vkw_ckwargs_key15, 
        _vkw_ckwargs_key16, 
        _vkw_ckwargs_key17, 
        _vkw_ckwargs_key18, 
        _vkw_ckwargs_key19, 
        _vkw_ckwargs_key20, 
        _vkw_ckwargs_key21, 
        _vkw_ckwargs_key22, 
        _vkw_ckwargs_key23, 
        _vkw_ckwargs_key24, 
        _vkw_ckwargs_key25, 
        _vkw_ckwargs_key26, 
        _vkw_ckwargs_key27, 
        _vkw_ckwargs_key28, 
        _vkw_ckwargs_key29, 
        _vkw_ckwargs_key30, 
        _vkw_ckwargs_key31, 
        _vkw_ckwargs_key32, 
        _vkw_ckwargs_key33, 
        _vkw_ckwargs_key34, 
        _vkw_ckwargs_key35, 
        _vkw_ckwargs_key36, 
        _vkw_ckwargs_key37, 
        _vkw_ckwargs_key38, 
        _vkw_ckwargs_key39,     // --> Note: Only required for packed-parameter-type keywords because we can't use it in LabelAngle() with packed-parameters  
        _vkw_ckwargs_key40, 
        _vkw_ckwargs_key41,     
    };

#ifdef _ckwargs_inc_check_items // Limit exposure to _kw_CheckItems macro so we can re-use it with multiple ckwargs modules
                                // (i.e. this #define never needs to be changed for multiple ckwargs uses in the same module)

    #define _ckwargs_CheckItems     CheckItem(_vkw_ckwargs_key1    );   \
                                    CheckItem(_vkw_ckwargs_key2    );   \
                                    CheckItem(_vkw_ckwargs_key3    );   \
                                    CheckItem(_vkw_ckwargs_key4    );   \
                                    CheckItem(_vkw_ckwargs_key5    );   \
                                    CheckItem(_vkw_ckwargs_key6    );   \
                                    CheckItem(_vkw_ckwargs_key7    );   \
                                    CheckItem(_vkw_ckwargs_key8    );   \
                                    CheckItem(_vkw_ckwargs_key9    );   \
                                    CheckItem(_vkw_ckwargs_key10    );  \
                                    CheckItem(_vkw_ckwargs_key11    );  \
                                    CheckItem(_vkw_ckwargs_key12    );  \
                                    CheckItem(_vkw_ckwargs_key13    );  \
                                    CheckItem(_vkw_ckwargs_key14    );  \
                                    CheckItem(_vkw_ckwargs_key15    );  \
                                    CheckItem(_vkw_ckwargs_key16    );  \
                                    CheckItem(_vkw_ckwargs_key17    );  \
                                    CheckItem(_vkw_ckwargs_key18    );  \
                                    CheckItem(_vkw_ckwargs_key19    );  \
                                    CheckItem(_vkw_ckwargs_key20    );  \
                                    CheckItem(_vkw_ckwargs_key21    );  \
                                    CheckItem(_vkw_ckwargs_key22    );  \
                                    CheckItem(_vkw_ckwargs_key23    );  \
                                    CheckItem(_vkw_ckwargs_key24    );  \
                                    CheckItem(_vkw_ckwargs_key25    );  \
                                    CheckItem(_vkw_ckwargs_key26    );  \
                                    CheckItem(_vkw_ckwargs_key27    );  \
                                    CheckItem(_vkw_ckwargs_key28    );  \
                                    CheckItem(_vkw_ckwargs_key29    );  \
                                    CheckItem(_vkw_ckwargs_key30    );  \
                                    CheckItem(_vkw_ckwargs_key31    );  \
                                    CheckItem(_vkw_ckwargs_key32    );  \
                                    CheckItem(_vkw_ckwargs_key33    );  \
                                    CheckItem(_vkw_ckwargs_key34    );  \
                                    CheckItem(_vkw_ckwargs_key35    );  \
                                    CheckItem(_vkw_ckwargs_key36    );  \
                                    CheckItem(_vkw_ckwargs_key37    );  \
                                    CheckItem(_vkw_ckwargs_key38    );  \
                                    CheckItem(_vkw_ckwargs_key39    );  \
                                    CheckItem(_vkw_ckwargs_key40    );  \
                                    CheckItem(_vkw_ckwargs_key41    );  

#endif
    // -----------------
    // Key Value storage
    // -----------------
    //  
    // This is currently set as a union.  It can be a struct to allow container items with constructors/destructors/etc., but
    // is not recommended.  It's recommended to use references as the input for the keywords for class objects. 
    
    union KeyValues     // Optionally a struct for more flexibility (and headaches)
    {
        _vkw_ckwargs_type1         _vkw_ckwargs_key1 ;
        _vkw_ckwargs_type2         _vkw_ckwargs_key2 ;    
        _vkw_ckwargs_type3         _vkw_ckwargs_key3 ;
        _vkw_ckwargs_type4         _vkw_ckwargs_key4 ;
        _vkw_ckwargs_type5         _vkw_ckwargs_key5 ;
        _vkw_ckwargs_type6         _vkw_ckwargs_key6 ;
        _vkw_ckwargs_type7         _vkw_ckwargs_key7 ;
        _vkw_ckwargs_type8         _vkw_ckwargs_key8 ;
        _vkw_ckwargs_type9         _vkw_ckwargs_key9 ;
        _vkw_ckwargs_type10        _vkw_ckwargs_key10 ;
        _vkw_ckwargs_type11        _vkw_ckwargs_key11 ;
        _vkw_ckwargs_type12        _vkw_ckwargs_key12 ;
        _vkw_ckwargs_type13        _vkw_ckwargs_key13 ;
        _vkw_ckwargs_type14        _vkw_ckwargs_key14 ;
        _vkw_ckwargs_type15        _vkw_ckwargs_key15 ;
        _vkw_ckwargs_type16        _vkw_ckwargs_key16 ;
        _vkw_ckwargs_type17        _vkw_ckwargs_key17 ;
        _vkw_ckwargs_type18        _vkw_ckwargs_key18 ;
        _vkw_ckwargs_type19        _vkw_ckwargs_key19 ;
        _vkw_ckwargs_type20        _vkw_ckwargs_key20 ;
        _vkw_ckwargs_type21        _vkw_ckwargs_key21 ;
        _vkw_ckwargs_type22        _vkw_ckwargs_key22 ;
        _vkw_ckwargs_type23        _vkw_ckwargs_key23 ;
        _vkw_ckwargs_type24        _vkw_ckwargs_key24 ;
        _vkw_ckwargs_type25        _vkw_ckwargs_key25 ;
        _vkw_ckwargs_type26        _vkw_ckwargs_key26 ;
        _vkw_ckwargs_type27        _vkw_ckwargs_key27 ;
        _vkw_ckwargs_type28        _vkw_ckwargs_key28 ;
        _vkw_ckwargs_type29        _vkw_ckwargs_key29 ;
        _vkw_ckwargs_type30        _vkw_ckwargs_key30 ;
        _vkw_ckwargs_type31        _vkw_ckwargs_key31 ;
        _vkw_ckwargs_type32        _vkw_ckwargs_key32 ;
        _vkw_ckwargs_type33        _vkw_ckwargs_key33 ;
        _vkw_ckwargs_type34        _vkw_ckwargs_key34 ;
        _vkw_ckwargs_type35        _vkw_ckwargs_key35 ;
        _vkw_ckwargs_type36        _vkw_ckwargs_key36 ;
        _vkw_ckwargs_type37        _vkw_ckwargs_key37 ;
        _vkw_ckwargs_type38        _vkw_ckwargs_key38 ;
        _vkw_ckwargs_type39        _vkw_ckwargs_key39 ; // --> Note: Only required for packed-parameter-type keywords because we can't use it in LabelAngle() with packed-parameters  
        _vkw_ckwargs_type40        _vkw_ckwargs_key40 ;
        _vkw_ckwargs_type41        _vkw_ckwargs_key41 ;

       
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
        _vkw_ckwargs_type1  * _vkw_ckwargs_key1 ;
        _vkw_ckwargs_type2  * _vkw_ckwargs_key2 ;
        _vkw_ckwargs_type3  * _vkw_ckwargs_key3 ;
        _vkw_ckwargs_type4  * _vkw_ckwargs_key4 ;
        _vkw_ckwargs_type5  * _vkw_ckwargs_key5 ;
        _vkw_ckwargs_type6  * _vkw_ckwargs_key6 ;
        _vkw_ckwargs_type7  * _vkw_ckwargs_key7 ;
        _vkw_ckwargs_type8  * _vkw_ckwargs_key8 ;
        _vkw_ckwargs_type9  * _vkw_ckwargs_key9 ;
        _vkw_ckwargs_type10 * _vkw_ckwargs_key10 ;
        _vkw_ckwargs_type11 * _vkw_ckwargs_key11 ;
        _vkw_ckwargs_type12 * _vkw_ckwargs_key12 ;
        _vkw_ckwargs_type13 * _vkw_ckwargs_key13 ;
        _vkw_ckwargs_type14 * _vkw_ckwargs_key14 ;
        _vkw_ckwargs_type15 * _vkw_ckwargs_key15 ;
        _vkw_ckwargs_type16 * _vkw_ckwargs_key16 ;
        _vkw_ckwargs_type17 * _vkw_ckwargs_key17 ;
        _vkw_ckwargs_type18 * _vkw_ckwargs_key18 ;
        _vkw_ckwargs_type19 * _vkw_ckwargs_key19 ;
        _vkw_ckwargs_type20 * _vkw_ckwargs_key20 ;
        _vkw_ckwargs_type21 * _vkw_ckwargs_key21 ;
        _vkw_ckwargs_type22 * _vkw_ckwargs_key22 ;
        _vkw_ckwargs_type23 * _vkw_ckwargs_key23 ;
        _vkw_ckwargs_type24 * _vkw_ckwargs_key24 ;
        _vkw_ckwargs_type25 * _vkw_ckwargs_key25 ;
        _vkw_ckwargs_type26 * _vkw_ckwargs_key26 ;
        _vkw_ckwargs_type27 * _vkw_ckwargs_key27 ;
        _vkw_ckwargs_type28 * _vkw_ckwargs_key28 ;
        _vkw_ckwargs_type29 * _vkw_ckwargs_key29 ;
        _vkw_ckwargs_type30 * _vkw_ckwargs_key30 ;
        _vkw_ckwargs_type31 * _vkw_ckwargs_key31 ;
        _vkw_ckwargs_type32 * _vkw_ckwargs_key32 ;
        _vkw_ckwargs_type33 * _vkw_ckwargs_key33 ;
        _vkw_ckwargs_type34 * _vkw_ckwargs_key34 ;
        _vkw_ckwargs_type35 * _vkw_ckwargs_key35 ;
        _vkw_ckwargs_type36 * _vkw_ckwargs_key36 ;
        _vkw_ckwargs_type37 * _vkw_ckwargs_key37 ;
        _vkw_ckwargs_type38 * _vkw_ckwargs_key38 ;
        _vkw_ckwargs_type39 * _vkw_ckwargs_key39 ;  // --> Note: Only required for packed-parameter-type keywords because we can't use it in LabelAngle() with packed-parameters  
        _vkw_ckwargs_type40 * _vkw_ckwargs_key40 ;
        _vkw_ckwargs_type41 * _vkw_ckwargs_key41 ; 
    };
} }// namespace ckwargs