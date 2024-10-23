#pragma once
#include "Sagebox.h"
#include "keywords\opt2_ckwargs.h"
#include "_SageOpt.h"
namespace Sage
{
class kwHelpers
{
public:
    static void opt_to_kw_NewWindow(kwType::KeyValuesPtr & keys,cwfOpt2 & cwOpt3);
    template<typename T>
    static __forceinline int AbsorbFloattoInt(const std::optional<int> & opInt,const std::optional<T> & opFloat,const int iDefault)
    {
        auto iReturn = iDefault;
        if (!opInt && opFloat) return (int) *opFloat; 
        return opInt ? (decltype(iReturn)) *opInt : iDefault; 
    };

    template<typename T>
    static __forceinline std::optional<int>  AbsorbFloattoInt(const std::optional<int> & opInt,const std::optional<T> & opFloat)
    {
        if (!opInt && opFloat) return (int) *opFloat; 
        return opInt ? *opInt : (std::optional<int>) std::nullopt ; 
    };

    static __forceinline int AbsorbFloattoInt(float * fpFloat,int * ipInt,int iDefault)
    {
        if (!ipInt && fpFloat) return (int) *fpFloat; 
        return ipInt ? *ipInt : iDefault; 
    }
    static __forceinline std::optional<int>  AbsorbFloattoInt(float * fpFloat,int * ipInt)
    {
        if (!ipInt && fpFloat) return (int) *fpFloat; 
        return ipInt ? *ipInt : (std::optional<int>) std::nullopt ; 
    }

    template<typename T>
    static __forceinline std::optional<float>  AbsorbIntToFloat(const std::optional<int> & opInt,std::optional<T> & opFloat)
    {
        if (!opFloat && opInt) return (float) *opInt; 
        return opFloat ? *opFloat : (std::optional<float>) std::nullopt ; 
    };

    static __forceinline std::optional<float>  AbsorbIntToFloat(float * fpFloat,int * ipFloat)
    {
        if (!fpFloat && ipFloat) return (float) *ipFloat; 
        return fpFloat ? *fpFloat : (std::optional<float>) std::nullopt ; 
    };
    template<typename T>
    static __forceinline float AbsorbIntToFloat(const std::optional<int> & opInt,std::optional<T> & opFloat,const float fDefault)
    {
        if (!opFloat && opInt) return (float) *opInt; 
        return opFloat ? *opFloat : fDefault;
    };

    static __forceinline std::optional<float> AbsorbIntToFloat(const int * ipValue,const float * fpValue)
    {
        if (!fpValue && ipValue) return (float) *ipValue; 
        return fpValue ? *fpValue : (std::optional<float>) std::nullopt ;
    };

    static __forceinline std::optional<CfPointf> AbsorbIntToFloat(const POINT * ipValue,const CfPointf * fpValue)
    {
        if (!fpValue && ipValue) return (CfPointf) *ipValue; 
        return fpValue ? *fpValue : (std::optional<CfPointf>) std::nullopt ;
    };

    static __forceinline std::optional<CPoint> AbsorbFloatToInt(const CfPointf * fpValue, const POINT * ipValue)
    {
        if (!ipValue && fpValue) return (CPoint) *fpValue; 
        return ipValue ? *ipValue : (std::optional<CPoint>) std::nullopt ;
    };

    static __forceinline std::optional<CfPointf> AbsorbIntoToFloat(const CfPointf * fpValue, const POINT * ipValue)
    {
        if (!fpValue && ipValue) return (CfPointf) *ipValue; 
        return fpValue ? *fpValue : (std::optional<CfPointf>) std::nullopt ;
    };

    [[nodiscard]] static CString GetFontString(kwType::KeyValuesPtr & keys,const char * sDefault = (const char *) nullptr)
    {
        int iFont   = kwType::ckw::Get(keys._Font,0);
        float fFont = kwType::ckw::Get(keys._Fontf,0.0f);
        auto hFont  = kwType::ckw::Get(keys._FontH,(HFONT) 0); 

        const char * sFont = kwType::ckw::Get(keys._FontStr,(decltype(sFont)) nullptr);
        CString csFont; 

        if (!EmptyString(sFont)) return csFont << sFont;
        else if (iFont) return csFont << iFont;
        else if (fFont) return csFont << fFont; 

        // If we don't have a specified font by numeric value or string name, look for a specified hFont we can lookup by HFONT
        // $$ Note --> This is not efficient, in that it looks up the hFont by name just to convert it back to the **same** hFont later
        //             This was done to avoid a large amount of coding changes system-wide.  However, it is kind of dumb that this happens.
        //             The only way to fix it is to remove all font references in kw:: (i.e. _Font, _FontStr, etc. -- i.e. change their names) and 
        //             a) see who get an error
        //             b) fix up all code to **also** look for an HFONT reference.
        //
        //             This code avoids all that (and subsequent testing by just returning the string for the font name for later lookup)
        //
        //             --> This is not addressed, as dealing with fonts has such an overhead, the added overhead for this workaround is trivial.
        //

        if (hFont)
        {
            auto csFont = Sagebox::GetFontName(hFont);
            if (!csFont.isEmpty()) return csFont;
        }

        return csFont << sDefault; 
    }
    static bool GetFontString_if(kwType::KeyValuesPtr & keys,CString & csFont)
    {
        auto csTemp = GetFontString(keys); 
        
        bool bFound = !csTemp.isEmpty();

        if (bFound) csFont = csTemp; 
        return bFound;
    }

    static CString GetValueFontString(kwType::KeyValuesPtr & keys,const char * sDefault = nullptr)
    {
        int iFont = kwType::ckw::Get(keys._ValueFont,0);
        float fFont = kwType::ckw::Get(keys._ValueFontf,0.0f);
        auto hFont  = kwType::ckw::Get(keys._ValueFontH,(HFONT) 0); 

        const char * sFont = kwType::ckw::Get(keys._ValueFontStr,(decltype(sFont)) nullptr);
        CString csFont; 

        if (!EmptyString(sFont))  return csFont << sFont;
        else if (iFont) return csFont << iFont;
        else if (fFont) return csFont << fFont; 

        // If we don't have a specified font by numeric value or string name, look for a specified hFont we can lookup by HFONT
        // $$ Note --> This is not efficient, in that it looks up the hFont by name just to convert it back to the **same** hFont later
        //             This was done to avoid a large amount of coding changes system-wide.  However, it is kind of dumb that this happens.
        //             The only way to fix it is to remove all font references in kw:: (i.e. _Font, _FontStr, etc. -- i.e. change their names) and 
        //             a) see who get an error
        //             b) fix up all code to **also** look for an HFONT reference.
        //
        //             This code avoids all that (and subsequent testing by just returning the string for the font name for later lookup)
        //
        //             --> This is not addressed, as dealing with fonts has such an overhead, the added overhead for this workaround is trivial.
        //

        if (hFont)
        {
            auto csFont = Sagebox::GetFontName(hFont);
            if (!csFont.isEmpty()) return csFont;
        }

        return csFont << sDefault; 
    }
    [[nodiscard]] static CString GetLabelFontString(kwType::KeyValuesPtr & keys,const char * sDefault = nullptr)
    {
        int iFont = kwType::ckw::Get(keys._LabelFont, 0);
        float fFont = kwType::ckw::Get(keys._LabelFontf, 0.0f);
        auto hFont  = kwType::ckw::Get(keys._LabelFontH,(HFONT) 0); 
        
        const char* sFont = kwType::ckw::Get(keys._LabelFontStr, (decltype(sFont)) nullptr);
        CString csFont;

        if (!EmptyString(sFont)) return csFont << sFont;
        else if (iFont) return csFont << iFont;
        else if (fFont) return csFont << fFont;

        // If we don't have a specified font by numeric value or string name, look for a specified hFont we can lookup by HFONT
        // $$ Note --> This is not efficient, in that it looks up the hFont by name just to convert it back to the **same** hFont later
        //             This was done to avoid a large amount of coding changes system-wide.  However, it is kind of dumb that this happens.
        //             The only way to fix it is to remove all font references in kw:: (i.e. _Font, _FontStr, etc. -- i.e. change their names) and 
        //             a) see who get an error
        //             b) fix up all code to **also** look for an HFONT reference.
        //
        //             This code avoids all that (and subsequent testing by just returning the string for the font name for later lookup)
        //
        //             --> This is not addressed, as dealing with fonts has such an overhead, the added overhead for this workaround is trivial.
        //

        if (hFont)
        {
            auto csFont = Sagebox::GetFontName(hFont);
            if (!csFont.isEmpty()) return csFont;
        }

        return csFont << sDefault; 
    }
    static bool GetLabelFontString_if(kwType::KeyValuesPtr & keys,CString & csLabelFont)
    {
        auto csTemp = GetLabelFontString(keys); 
        
        bool bFound = !csTemp.isEmpty();

        if (bFound) csLabelFont = csTemp; 
        return bFound;
    }

    static RgbColor GetColor(RgbColor * rgbColor,const char * * sColor,RgbColor rgbDefault)
    {
        if (rgbColor) return *rgbColor;
        if (sColor && *sColor && **sColor)
        {
            bool bFound = false;
            auto rgbNew = Sagebox::GetColor(*sColor,&bFound);
            if (bFound) return rgbNew.ValidColor() ? rgbNew : rgbDefault;
        }
        return rgbDefault;
    }

    static bool GetColor_if(RgbColor * rgbColor,const char * * sColor,RgbColor & rgbFill)
    {
        if (rgbColor)
        {
            if ((*rgbColor).ValidColor())
            {
                rgbFill = *rgbColor;
                return true;
            }
        }
        if (sColor && *sColor && **sColor)
        {
            bool bFound = false;
            auto rgbNew = Sagebox::GetColor(*sColor,&bFound);
            if (bFound && rgbNew.ValidColor())
            {
                rgbFill = rgbNew;
                return true;
            }
        }
        return false;
    }

   static std::optional<RgbColor> GetColor(RgbColor * rgbColor,const char * * sColor)
    {
        if (rgbColor) return rgbColor->ValidColor() ? *rgbColor : (std::optional<RgbColor>) std::nullopt;
        if (sColor && *sColor && **sColor)
        {
            bool bFound = false;
            auto rgbNew = Sagebox::GetColor(*sColor,&bFound);
            if (bFound) return rgbNew.ValidColor() ? rgbNew : (std::optional<RgbColor>) std::nullopt;;
        }
        return (std::optional<RgbColor>) std::nullopt;;
    }

    static RgbColor GetColor(RgbGradient * rgbColor,const kwType::stColorGradientStr_t * sColor,RgbColor rgbDefault)
    {
        if (rgbColor) return rgbColor->color1.ValidColor() ? rgbColor->color1 : rgbDefault;
        if (sColor && sColor->sColor1 && *sColor->sColor1)
        {
            bool bFound = false;
            auto rgbNew = Sagebox::GetColor(sColor->sColor1,&bFound);
            if (bFound) return rgbNew.ValidColor() ? rgbNew : rgbDefault;
        }
        return rgbDefault;
    }

    static bool GetColor_if(RgbGradient * rgbColor,const kwType::stColorGradientStr_t * sColor,RgbColor & rgbFill)
    {
        if (rgbColor)
        {
            if (rgbColor->color1.ValidColor())
            {
                rgbFill = rgbColor->color1;
                return true;
            }
        }

        if (sColor && sColor->sColor1 && *sColor->sColor1)
        {
            bool bFound = false;
            auto rgbNew = Sagebox::GetColor(sColor->sColor1,&bFound);
            if (bFound && rgbNew.ValidColor())
            {
                rgbFill = rgbNew;
                return true;
            }
        }
        return false;
    }
    static std::optional<RgbColor> GetColor(RgbGradient * rgbColor,const kwType::stColorGradientStr_t * sColor)
    {
        if (rgbColor) return rgbColor->color1.ValidColor() ? rgbColor->color1 : (std::optional<RgbColor>) std::nullopt;
        if (sColor && sColor->sColor1 && *sColor->sColor1)
        {
            bool bFound = false;
            auto rgbNew = Sagebox::GetColor(sColor->sColor1,&bFound);
            if (bFound) return rgbNew.ValidColor() ? rgbNew : (std::optional<RgbColor>) std::nullopt;
        }
        return std::nullopt;
    }

   static RgbColor GetColor2(RgbGradient * rgbColor,const kwType::stColorGradientStr_t * sColor,RgbColor rgbDefault)
    {
        if (rgbColor) return rgbColor->color2.ValidColor() ? rgbColor->color2 : rgbDefault;
        if (sColor && sColor->sColor2 && *sColor->sColor2)
        {
            bool bFound = false;
            auto rgbNew = Sagebox::GetColor(sColor->sColor2,&bFound);
            if (bFound) return rgbNew.ValidColor() ? rgbNew : rgbDefault;
        }
        return rgbDefault;
    }

    static std::optional<RgbGradient> GetColors(RgbGradient * rgbColor,const kwType::stColorGradientStr_t * sColor)
    {
        if (rgbColor)
        {
            RgbColor fgColor = rgbColor->color1.ValidColor() ? rgbColor->color1 : rgbUndefined;
            RgbColor bgColor = rgbColor->color1.ValidColor() ? rgbColor->color2 : fgColor; 

            return fgColor.ValidColor() ? RgbGradient{ fgColor, bgColor } :  (std::optional<RgbGradient>) std::nullopt;
        }
        if (sColor && sColor->sColor1 && *sColor->sColor1)
        {
            auto fgColor = SageBox::GetColor2(sColor->sColor1); 
            auto bgColor = SageBox::GetColor2(sColor->sColor2);

            if (bgColor == RgbVal::Undefined) bgColor = fgColor;

            return fgColor.ValidColor() ? RgbGradient{ fgColor, bgColor } :  (std::optional<RgbGradient>) std::nullopt;
        }
        return std::nullopt;
    }
    static bool GetColors_if(RgbGradient * rgbColor,const kwType::stColorGradientStr_t * sColor,RgbColor & outfgColor,RgbColor & outbgColor)
    {
        if (rgbColor)
        {
            RgbColor fgColor = rgbColor->color1.ValidColor() ? rgbColor->color1 : rgbUndefined;
            RgbColor bgColor = rgbColor->color1.ValidColor() ? rgbColor->color2 : fgColor; 

            if (fgColor.ValidColor())
            {
                outfgColor = fgColor;
                outbgColor = bgColor;
                return true;
            }
            return false;
        }
        if (sColor && sColor->sColor1 && *sColor->sColor1)
        {
            auto fgColor = SageBox::GetColor2(sColor->sColor1); 
            auto bgColor = SageBox::GetColor2(sColor->sColor2);

            if (bgColor == RgbVal::Undefined) bgColor = fgColor;

            if (fgColor.ValidColor())
            {
                outfgColor = fgColor;
                outbgColor = bgColor;
                return true;
            }
            return false;  
        }
        return false;
    }

    static CenterType GetCenterType(kwType::KeyValuesPtr & keys,const CenterType eDefault = CenterType::None,bool bSkipIfExists = false);
    static JustType GetJustType(const char * * sType,const JustType * eJustType,const JustType eDefault = JustType::None,bool bSkipIfExists = false);

    static std::map<std::string,CenterType> map_kwCenterType;
    static std::map<std::string,JustType> map_kwJustType;

}; // class kwHelpers


} // namespace Sage
