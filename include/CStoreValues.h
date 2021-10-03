#pragma once

#include "CSageBox.h"

namespace Sage
{
class CStoreValues
{
    static constexpr int kMaxIndexValues = 1000; 
    static constexpr int kMaxAllocValues = 10000; 
    static constexpr int kMaxValueNameSize = 50;

    enum class StoreType
    {
        Value,
        String
    };

    struct stValue_t
    {
        union un
        {
            double          fdValue;
            float           fValue;
            int             iValue;
            unsigned int    uiValue;
            unsigned long long ui64Value;
            long long llValue;
            void * vPointer;
        };
        un Values;
    };
    struct StoreIndex_t
    {
        StoreType eStoreType;
        long long vValue; // N/A if string type
        CString * cString;     // Nullptr if not String Type
    };
    struct Store_t
    {
        char sName[kMaxValueNameSize+1]; 
        int iIndex;             // Index starts at kMaxIndexValues;
        StoreType eStoreType;
        long long vValue; // N/A if string type
        CString * cString;     // Nullptr if not String Type
        bool bActive;
    };

    Mem<StoreIndex_t> m_vIndexValues;   
    Mem<Store_t> m_vAllocValues; 

    int iNumAllocValues = 0;
    bool AddIndexValue(int iIndex,long long uiValue);
    bool AddIndexString(int iIndex,const char * sString);
    bool AddAllocValue(int iIndex,long long uiValue);
    bool AddAllocValue(const char * sName,long long uiValue);
    bool AddAllocString(const char * sName,const char * sString);
    bool AddAllocString(int iIndex,const char * sString);
    Store_t * FindAllocIndex(int iIndex); 
    Store_t * FindAllocName(const char * sName); 

    Store_t * GetNewAllocSlot();
public:


    bool AddValue(int iIndex,long long uiValue);
    bool AddValue(int iIndex,const char * sString);
    bool AddValue(const char * sName,long long uiValue);
    bool AddValue(const char * sName,const char * sString);
    bool DeleteValue(int iIndex); 
    bool DeleteValue(const char * sName); 
    long long GetValue(int iIndex);
    long long GetValue(const char * sName);
    const char * GetString(const char * sName);
    const char * GetString(int iIndex);

    // Passthrough functions to specify different types easily 

#define _CStoreValues_AddValue(_stn)  stValue_t stValue{}; stValue.Values._stn = Value; return AddValue(iIndex,*reinterpret_cast<long long *>(&stValue)); 
#define _CStoreValues_AddValueS(_stn)  stValue_t stValue{}; stValue.Values._stn = Value; return AddValue(sName,*reinterpret_cast<long long *>(&stValue)); 

    bool AddValue(int iIndex,double Value)              { _CStoreValues_AddValue(fdValue); };
    bool AddValue(int iIndex,int Value)                 { _CStoreValues_AddValue(iValue); };
    bool AddValue(int iIndex,unsigned int Value)        { _CStoreValues_AddValue(uiValue); };
    bool AddValue(int iIndex,unsigned long long Value)  { _CStoreValues_AddValue(ui64Value); };
    bool AddValue(int iIndex,void * Value)              { _CStoreValues_AddValue(vPointer); };

    bool AddValue(const char * sName,double Value)              { _CStoreValues_AddValueS(fdValue); };
    bool AddValue(const char * sName,int Value)                 { _CStoreValues_AddValueS(iValue); };
    bool AddValue(const char * sName,unsigned int Value)        { _CStoreValues_AddValueS(uiValue); };
    bool AddValue(const char * sName,unsigned long long Value)  { _CStoreValues_AddValueS(ui64Value); };
    bool AddValue(const char * sName,void * Value)  { _CStoreValues_AddValueS(vPointer); };

#define _CStoreValues_GetValue(_stn) stValue_t stValue;  stValue.Values.llValue = GetValue(iIndex); return stValue.Values._stn;
#define _CStoreValues_GetValueS(_stn) stValue_t stValue;  stValue.Values.llValue = GetValue(sName); return stValue.Values._stn;

    void * GetValuePointer         (int iIndex) { _CStoreValues_GetValue(vPointer); }
    double GetValueDouble           (int iIndex) { _CStoreValues_GetValue(fdValue); }
    int GetValueInt              (int iIndex) { _CStoreValues_GetValue(iValue); }
    unsigned int GetValueUnsignedInt      (int iIndex) { _CStoreValues_GetValue(uiValue); }
    unsigned long long GetValueUnsignedLongLong (int iIndex) { _CStoreValues_GetValue(ui64Value); }

    void * GetValuePointer         (const char * sName) { _CStoreValues_GetValueS(vPointer); }
    double GetValueDouble           (const char * sName) { _CStoreValues_GetValueS(fdValue); }
    int GetValueInt              (const char * sName) { _CStoreValues_GetValueS(iValue); }
    unsigned int GetValueUnsignedInt      (const char * sName) { _CStoreValues_GetValueS(uiValue); }
    unsigned long long GetValueUnsignedLongLong (const char * sName) { _CStoreValues_GetValueS(ui64Value); }

    ~CStoreValues();
    CStoreValues();
};

} // namespace Sage

