#pragma once
#include "Sagebox.h"

namespace Sage
{

// Build a list of enum values found in a list that correspond go map entries, i.e.
//
// "Edges = Left,Right,Top"
//
// Where a map defined as std;:map<std::string,ENUM TYPE> 
// that contains "Left","Right", etc. with the corresponding enum.
//
// A vector with the enums found is returned.  If enums keywords are found, an empty vector is returned. 
//
// Example:
//
// auto return_vector = GetMapEnums("Edges",EdgeTypes);
//
// Important note: opt.SetOptString() must be set to the current search string before call is made. 
//
template<typename etype>
std::vector<etype> GetMapEnums(const char * sSearch,std::map<std::string,etype> & inMap)
{
    auto opt = Sagebox::GetSageWindow()->opt;
    std::vector<etype> vReturn; 

    if (EmptyString(sSearch)) return vReturn; 

    auto sTemp = opt.GetOptPointer(sSearch); 

    if (sTemp)
    {
        while(*sTemp)
        {
            sTemp = CString::SkipWhiteSpace(sTemp); 
            auto sDelim = CString::SkipAlphaNumeric(sTemp); 
            CString cs;
            int iLength = (int) (sDelim-sTemp); 
            if (iLength > 0)
            {
                cs.AppendString(sTemp,iLength); 
                sTemp = sDelim;

                auto c = inMap.find(*cs);
                if (c != inMap.end()) vReturn.push_back(c->second); 
            }
          
            sDelim = CString::SkipWhiteSpace(sDelim); 
            if (*sDelim != ',' || iLength <= 0) break;
            sTemp = sDelim;
            if (*sTemp) sTemp++;
        }

    }

    return vReturn;
}

} // namespace Sage