
// CKwargs -- Sagebox C++ Named Parameter and Named Functions Class 
// Copyright (c) 2022 Rob Nelson, All Rights Reserved. Released under MIT License.  rob@sagebox.org
// 
// -----------------------------------------------------------------------
// CKwargs: C++ Named-Parameters (aka Keywords) and Named-Parameters Class
// -----------------------------------------------------------------------
//
// CKwargs allows true canonical, keyword-style arguments (i.e. named-parameters) as well as function-style
// keywords that can offer more flexibility.
//
// CKwargs is a very small class, but can do a lot with many options for your program, consisting
// of about 40 lines of actual code, in addition to the keyword code written specifically for the
// program using it (which is typically one or two lines of code per keyword)
// 
// See the Readme.md file at github.com/Sagebox/ckwargs for more information.
// 
// ------------------
// How to use CKwargs
// ------------------
//       
//  1. ckwargs.h must be set with your keyword names and keyword storage types.
//  2. A namespace or class must be created with your keyword functions that
//     pass the information to the ckw class when the keyword is invoked.
//  
//     see my_keyword.h as an example -- your file is meant to replace my_keyword.h, and 
//     my_keyword.h is not referred to by CKwargs and is private to the program that uses CKwargs
// 
// See the Readme.md file at github.com/Sagebox/ckwargs for more information.
// 
// -----------------------------------------
// Classes and Namespaces Defined in CKwargs
// -----------------------------------------
// 
// --> ckwargs.h and ckwargs.cpp are the only two files that are the CKwargs project.
//     a keyword class or namespace is defined by the program using CKwargs and is 
//     represented by the example classes (in my_keywords.h) as 'kw' and 'kf'
// 
//     my_keywords.h is not technically part of CKwargs, and is used as an example of
//     how to define keywords in various forms for CKwargs.
// 
//     -----------
//     ckwargs.cpp
//     -----------
// 
//          ckw      -- main keyword class.  This is the only class or namespace that is truly
//                      defined in CKWargs.  Other namespaces and classes can be changed to suit the program
//                      and to use CKwargs in multiple places in the same project.
//          
//          ckwargs  -- main working namespace.  This is where ckw lives, as well as some other functions.
//                      This is designed to be personalized for the program, and can be changed in one place.
//
//      -------------------------------------------
//      my_keywords.h or my_keyfuncs.h (example file)
//      -------------------------------------------
//                          
//          kw       -- (my_keywords.h) Main keywords example.  This creates keyword-style named parameters, such as 
//                      AddBorder=True, Range= {1,10}, BorderSize = 25, etc.
// 
//                      This is defined as a namespace in my_keywords.h, but can also be a class. 
//                      
//          kf      -- (my_keyfuncs.h) Main keyword functions example.  This created keyword-style named-parameter functions, such as
//                     AddBorder(), AddBorder(true), Range(1,10), BorderSize(25), etc.
// 
//                      This is defined as a namespace in my_keywords.h, but can also be a class.
// 
//          Note: kw and kf are defined as example -- in a typical usage, only one would exist, as they reflect
//                the choice between using keywords or keyword functions.
// 
// -------------------------------------
// About my_keywords.h and my_keyfuncs.h
// -------------------------------------
// 
// 1. Only one file is needed to use ckwargs.  Each file shows a different method (keywords vs keyword-functions),which
//    generally aren't used simultaneously.  
// 2. Choose the form you want and use that file as a template.
// 3. This is the file the client-application includes -- rename this file to something that fits your project.
//
// ----------------------
// About ckwargs namespace
// ----------------------
// 
// This is the base namespace for ckwargs.  For multiple function sets or modules that
// use ckwargs, these files can be copied, with and the ckwargs namespace can be changed to a 
// namespace reflective of the project -- just search and replace 'ckwargs' with
// a new namespace name in the copied files.
// 
// The client program uses only the keyword namespace defined in my_keywords.h/my_keyfuncs.h (depending on keyword form used). 
//  
// For each different keyword block for differnet function sets, a unique namespace is required.
// 
// Ckwargs code is about 50 lines in total, so adding function sets doesn't increase program size by more
// than a couple k (if that) for each instance.


// 
// ---------------
// CKwargs Example 
// ---------------
//
//      let's Say we have a function called DrawBox(int x,int y,int Radius), and we have a lot of potential options
//      but don't want to clutter the function prototype.
// 
//      Some optional keywords we might use for DrawBox() are:
//
//          Filled, AddBorder, BorderSize, Color, Transparency, Angle, Skew
//
//      Where defaults are used if the keywords are not specified.
//
//      --> Keep track of Skew, as it is used in different wasy below for more flexibility
//          Skew needs to values, an X value and Y value.  It is used as an example of 
//          using multiple values and forms in the named-parameter functions vs. named-parameter keywords.
//
//      --> Color can also take muliple forms, such as a color value, i.e. Color=RgbColor(255,0,0) or
//          a color string, Color="Red"
// 
// -----------------------------
// CKwargs Examples of DrawBox()
// -----------------------------
// 
// Some examples of using CKwargs:
// 
//      DrawBox(x,y,radius);        // Uses no keywords
//      
//      DrawBox(x,y,radius, Filled=true, Color="red", Skew={10,-10})
//      DrawBox(x,y,radius, Filled=true, Color=Rgb(255,0,0), Skew={10,-10})
//      DrawBox(x,y,radius, Filled=true, Color=Rgb(255,0,0), Skew=varSkew)
//      
//      A functionalized format can be used, too, for greater flexibility:
//      
//      --> Say varSkew is a variable of type SIZE, i.e. SIZE varSkew = (10,-10)
//      
//      DrawBox(x,y,radius, Filled(true), Color("red"), Skew(10,-10))
//      DrawBox(x,y,radius, Filled(true), Color("red"), Skew(varSkew)
//      DrawBox(x,y,radius, Filled(), Color("red"), Skew={10,-10})
//
//      --> Note that in the named-parameter function style, Skew() was able to take one or two arguments of different types.
//          Using two arguments is not possible with straight keywords and must be in the form of a container.
//
//          Also note that Filled() could be called to specify a "true", rather thsan Filled(true), whereas the absence
//          of the keyword sets the default (which would normally be false, but it up to the program)
//
// -----------------------------------------------------------
// Two forms of CKwargs: packed-parameters and streamed object
// -----------------------------------------------------------
//
// The form in the examples above is the pack-parameter format, which allows canonical keyword and keyword functions as part
// of the arguments in a function without specifyng them in the protoype.
//
// Another form is a streamed-object format. 
// 
// Both formats have their advantages.  The main advantage of the packed-parameter format is that it provides canonical keyword and keyword
// functions that can be added to a function call, whereas the main advantage of the streamed object type is that it allows for no public
// interface code, and does not require the use of templates. 
// 
// See the Readme.md file at github.com/Sagebox/ckwargs for more information.
// 
// -------------------------------------------------------------------------------
// namespaces vs classes in the main keyword definition scope (i.e. my_keywords.h)
// -------------------------------------------------------------------------------
// 
// See the Readme.md file at github.com/Sagebox/ckwargs on using namespace vs. classes for 
// the keyword class/namespace as shown in the example file my_keywords.h
// 
// The main difference is that with a namespace, the keywords can be brought into a local focus so that a class or namespace prefix
// is not needed.
// 
// This is the differene between:
// 
//      DrawBox(x,y,radius, Filled=true, Color="red", Skew={10,-10})
//      
//      and
//      
//      DrawBox(x,y,radius, kw::Filled=true, kw::Color="red", kw::Skew={10,-10})
// 
// where kw: is the class or namespace of the keyword function locations.
// 

#pragma once

#define keyword_cpp17_support       // uncomment for C++11 and C++14 compatibility
//#include "Sagebox.h"
#include "Sage.h"

#include <cstdlib>
#include <functional>
#ifdef keyword_cpp17_support
#include <optional>
#endif


// -----------------------------------------------------------
// Personalized Includes Needed for Keyword Data Storage Types
// -----------------------------------------------------------
//
// Types required for the _kw_types below

#include <array>        // Included for the "Range" keyword example.  Otherwise not needed.
#include <cstring>

#ifdef _MSC_VER 

// Turn off some MSVC warnings that, while generally useful, also cause inefficient code
// for routines that are focused towards not wasting processor time. 
//
//
// Specifically, warnings about uninitialized values that are purposely left uninitialized, both locally on the stack
// and in class definitions

#pragma warning( push )
#pragma strict_gs_check(off)
#else
    // forceinline for gcc 
    //
    #ifndef __forceinline 
    #define __forceinline __attribute__((always_inline))
#endif
#endif

#include "opt2_keydefs.h"        // Include keyword definitions for ckwargs namespace

// Main Named Parameter namespace -- rename as appropriate

namespace Sage {


namespace kwType
{

    // -------------------------------
    // ckw class -- main keyword class 
    // -------------------------------
    // 
    // ckw is the class returns when a named parameter (i.e. keyword) or named parameter function (i.e. keyfunc) is used.
    // This is basically hidden in the interface, where the namespace (or class) for the keywords themselves is separate.
    //
    // For example, with a namespace 'kw', expressing kw::Range={1,10}  (or kw::Range(1,10) for keyword functions),
    // a ckw class object is returned, but not seen by the function.
    //
    // The function that wishes to use the keywords can simply call 
    // 
    //      auto keys = ckw::FillKeyValues(args...) 
    // 
    // to retrieve the key list with pointers/nullptrs for keywords used,
    // 
    // or
    //  
    //      auto keys = ckw_object.FillkeyValues()  --> when using the streaming form that does not use parameter packed templates.
    //
    class ckw
    {
    public:        
        // Operators for adding keywords in streamed version
        
        ckw & operator << (const ckw & Opt);    // Also required for template parameter packed version

        // These only work for streaming version of keyword functions.
        // For streamed version of keywords, the ',' enclosed by () must be used:
        // 
        //      MyFunction(1234,( AddBorder=true, Range={1,10} )); 
        //
        // For streamed named-parameter functions, it is more flexible, without the () requirement, such as:
        //
        //      MyFunction(1234,AddBorder() << Range(1,10))
        //      MyFunction(1234,AddBorder() + Range(1,10))
        //      MyFunction(1234,AddBorder() | Range(1,10))
        //
        // These are not used for the packed-parameter version and can be removed 
        // if the streaming-type is not being used.

        ckw & operator ,  (const ckw & Opt) { return operator <<(Opt); }
        ckw & operator +  (const ckw & Opt) { return operator <<(Opt); }
        ckw & operator |  (const ckw & Opt) { return operator <<(Opt); }

        // ---------------------
        // Stored keyword values  
        // ---------------------
        //
        // note: By default this is a union, but can be 
        //       declared as a struct to allow more flexibility,
        //       such as passing STL elements such as std::pair, vector,
        //       etc. as well as class object
        //
        //       However, beware that this will cause copying of said objects.
        //
        //       The default union format provides nice protection, because it 
        //       will result in a compiler error for anything that has instantiation, 
        //       copying, move, needs, etc.
        //
        KeyValues keyValues;    
 
        // Basic keyword package, containing the key type and location of its cwk class.
        //
        struct Package
        {
            Keywords key;
            ckw * pData;
        };

        Package package;        // key, location data

        const ckw * pNext = nullptr;    // Next ckw object in the chain
        ckw * pLast       = nullptr;    // Last one we looked at (for packed-parameter compilation)

        // Constructors for packed-parameter usage and user-code constructing keyword
        // transfer code (see my_keywords.h example file).

        ckw();  
        ckw(Keywords key,std::function<void(ckw &)> const & fFunc = nullptr); 

#ifdef __debug_ckwargs__
        static inline int ckwCount = 0;
        int ID;
        static inline int iNumAllocated = 0;

        ~ckw()
        {
            SageDebug::printf("deleted ckw #%d (alloc = %d)\n", ID,--iNumAllocated);
        }
#endif
        // Move constructor should only be used when assigning a keyword.
        //
        ckw(ckw && p2) noexcept;

        // Copy Constructor
        //
        ckw& operator=(const ckw& kw)  noexcept;

        // Fills a structure with pointers (or std::optionals if turned on) for all keywords
        // that were used.  Values are nullptr (or nullopt) if they were not used. 
        //
        // Set Get() functions
        //
        const KeyValuesPtr FillKeyValues() const;
 
        // Get a value if it is defined, or return the default value if it isn't.
        // Use the std::optional version, or just refer to the key directly to test for
        // nullptr or std::optional true/false. 
        //
        template<typename T>
        static __forceinline T Get(const T * const & value,const T & defvalue) 
        {
            return value ? *value : defvalue;
        }

        static __forceinline bool GetBool(const bool * const & value) 
        {
            return value ? *value : false;
        }

        static __forceinline bool GetBool_if(const bool * const & value,bool & bOut) 
        {
            if (value)
            {
                bOut = *value;
                return true;
            }
            return false;        }

        static __forceinline int GetInt(const int * const & value) 
        {
            return value ? *value : 0;
        }

        static __forceinline float GetFloat(const float * const & value) 
        {
            return value ? *value : 0.0f;
        }

        static __forceinline bool GetInt_if(const int * const & value,int & iOut) 
        {
            if (value)
            {
                iOut = *value;
                return true;
            }
            return false;
        }

        static __forceinline const char * GetString(const char * * value) 
        {
            return value ? *value : (const char *) nullptr;
        }

        static __forceinline bool GetString_if(const char * * value,CString & cString) 
        {
            if (value)
            {
                cString = *value;
                return true;
            }
            return false;
        }

        template<typename T>
        static __forceinline T GetNullptr(const T * const & value) 
        {
            return value ? *value : (T) nullptr;
        }


#ifdef keyword_cpp17_support

        // Sets a std::optional to the value of the keyword if it was used, or returns nullopt
        // if not.
        //
        // This function is completely redundant when _kw_useOptional is used, as the directl values
        // are already optionals, and this forces a copy.
        //
        // This routine can be useful when _kw_useOptional is not used to convert to an optional value,
        // but keep in mind that the memory is value is copied when this happens, so size of the value and
        // any object issues, copy constructors, etc. should be kept in mind.
        //
        template<typename T>
        static __forceinline std::optional<T> Get(const T * const & value)
        {
            return (value ? std::optional<T>(*value) : std::nullopt);
        }

#endif
    }; // class ckw


    // This class is for temporary uses to declare standalone keywords or keywords sets on the stack:
    //
    // ckw myKey;
    // myKey << kw::Title("Hello World"); 
    //
    // --> kw::Title() is invalid as the ckw created is out of scope on the next line, where the myKey object has a pointer to it.
    //
    // With ckw2, chains can be created that are presistent and deleted when the ckw2 object is out of scope;
    // 
    // ckw2<20> myKey;     // create stack-based buffer of 20 ckw items
    // 
    // myKey << kw::Title("Hello World") << kw::Realtime();
    // myKey << kw::Color("red"); 
    // 
    // --> All are preistent and can be used when sent to a function with keywords. 
    // 
    // myKey << oldKey 
    // 
    // This is peristent, even when oldKey is a regular ckw type.  Typically incoming keys are const, so "oldKey << myKey" would not work due to const status of oldKey
    //
    // --> Note: if the buffer is exceeded, additions are ignored. 
    //
    template<int size>
    class ckw2
    {
        int m_iCount = 0;
        ckw m_ckw[size];
    public:
        ckw2() = default;
        operator ckw& () { return m_ckw[0]; }
        void Clear() { m_iCount = 0; }
        ckw2& operator <<  (const ckw& Opt)
        {

            if (m_iCount < size)
            {
                // No move constructor in ckw
                m_ckw[m_iCount].package.key = Opt.package.key;
                m_ckw[m_iCount].package.pData = &m_ckw[m_iCount];
                m_ckw[m_iCount].pNext = Opt.pNext;
                m_ckw[m_iCount].pLast = Opt.pLast;
                m_ckw[m_iCount].keyValues = Opt.keyValues;

                if (m_iCount) m_ckw[0] << m_ckw[m_iCount];
                m_iCount++;
            }

            return *this;

        }
        ckw2& operator >>  (const ckw& Opt) { Clear(); return *this << Opt; };
        bool Active() { return m_iCount > 0; }

       ckw2& operator |  (const ckw& Opt)
        {

            if (m_iCount < size)
            {
                // No move constructor in ckw
                m_ckw[m_iCount].package.key = Opt.package.key;
                m_ckw[m_iCount].package.pData = &m_ckw[m_iCount];
                m_ckw[m_iCount].pNext = Opt.pNext;
                m_ckw[m_iCount].pLast = Opt.pLast;
                m_ckw[m_iCount].keyValues = Opt.keyValues;

                if (m_iCount) m_ckw[0] << m_ckw[m_iCount];
                m_iCount++;
            }

            return *this;

        }


        // Copy constructor
        //
        // 1. Copy the values
        // 2. Look for any link in kw[0] that is within itself and 
        // change the offset to 'this' instead
        
        ckw2& operator=(const ckw2& kw)  noexcept
        {
            m_iCount = kw.m_iCount;
                    
            for (int i=0;i<m_iCount;i++)
            {
                m_ckw[i] = kw.m_ckw[i];
                auto iOffset = (unsigned long long) &m_ckw[i]-(unsigned long long) &kw.m_ckw[i];
                if (m_ckw[i].pNext >= &kw.m_ckw[i] && m_ckw[i].pNext < &kw.m_ckw[m_iCount]) * (unsigned long long *) &m_ckw[i].pNext += iOffset;  // Set to local
                if (m_ckw[i].pLast >= &kw.m_ckw[i] && m_ckw[i].pLast < &kw.m_ckw[m_iCount]) * (unsigned long long *) &m_ckw[i].pLast += iOffset;  // Set to local
             }
            return *this;
        }
         ckw2 (const ckw2& kw)  noexcept
         {
            m_iCount = kw.m_iCount;
                  
            for (int i=0;i<m_iCount;i++)
            {
                m_ckw[i] = kw.m_ckw[i];
                auto iOffset = (unsigned long long) &m_ckw[i]-(unsigned long long) &kw.m_ckw[i];
                if (m_ckw[i].pNext >= &kw.m_ckw[i] && m_ckw[i].pNext < &kw.m_ckw[m_iCount]) * (unsigned long long *) &m_ckw[i].pNext += iOffset;  // Set to local
                if (m_ckw[i].pLast >= &kw.m_ckw[i] && m_ckw[i].pLast < &kw.m_ckw[m_iCount]) * (unsigned long long *) &m_ckw[i].pLast += iOffset;  // Set to local
            }
        }

    };



    // ---------=---------------------------------
    // CKwargs Packed-Parameter Fill Keyword Class
    // -------------------------------------------
    //
    // The functions in this class are used to create a ckw class
    // and link all parameters in the parameter pack. 
    //
    // This allows functions to not specify keywords and leave the keyword portion empty.
    //
    class pkw
    {
    private:

        // Parameter Pack Templates for retrieving keyword pointers 
       
        static void __fillkeyvalues(ckw & ckwOrg) { ckwOrg; }

        // Original incoming ckw class is an empty object with no keyword.
        //
        template <class... Args>
        static void __fillkeyvalues(ckw & ckwOrg,const ckw & kwx,const Args&... args)
        {
            ckwOrg << kwx;      // Link the current argument/parameter to the previous one.
            __fillkeyvalues(ckwOrg,args...);
        }

    public:
        // FillKeyValues -- Take a packed parameter package and link the values together as a ckw class,
        // returning a KeyValuePtr object with keyword pointers
        //
        // At this point, the packed-parameters are out of the equation and we can just deal with the keywords.
        //
        // The return value is KeyValuesPtr.  
        // A return cwk object would have made things more consistent with the packed-parameters vs. streamed parameters,
        // but would have caused a copy of the cwk object, so it returns KeyValuesPtr, since that's the
        // first thing the caller is going to do anyway.
        //
        template <class... Args>
        static KeyValuesPtr FillKeyValues(const Args&... args)
        {
            ckw kwx;                            // Get an empty object
            __fillkeyvalues(kwx,args...);       // Compile our ckw object
            return kwx.FillKeyValues();         // Get all keyword information and return it.
        }

        // FillKeyValues() for empty keyword sections (i.e. no keywords specified)
        //
        static KeyValuesPtr FillKeyValues() { ckw kwx; return kwx.FillKeyValues(); }

    }; // class pkw
} // namespace kwType

template<int size>
using SageKeys = kwType::ckw2<size>; 

} // namespace Sage


#ifdef _MSC_VER 
#pragma warning( pop )
#endif
