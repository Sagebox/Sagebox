#pragma once

#include "CLockProcess.h"
#include <vector>

namespace Sage
{

    class CPasWindow;
   // CDataStore Class -- used to form essentially smart pointers for each control that are alive
   // until the reference count moves to 0.
   //
   // This allows a control's client to access memory directly to determine if the control is alive, and to bypass 
   // an action if it isn't, rather than looking the control up or relying on C++ smart-pointers that may not be
   // useful for this. 

    class CDataStore
    {
    public:
        struct DataStore_t
        {
           int         iDataStoreID; 
           int         iControlNum;
           void      * pControl;
           long long   iRefCount; 
           bool        bExistent; 
           bool        bControlActive;
           void      * pUserData;
        };
    private:
        int m_iDataStoreCount = 0;
        static inline int m_iDataStoreUserNumber = 1;         // Number go backwards 
        
        std::vector<DataStore_t *> m_vDataStore; 
        CLockProcess m_cDataStoreLock;
        static inline DataStore_t m_stNullDataStore;
    public:
        CPasWindow          * cPasWindow = nullptr;
        DataStore_t         * Register(int iControlID);

        static void InitDataStore(CDataStore * cDataStore);        // Set first data store as a null entry
        static DataStore_t * GetNullDataStore();   // Return first entry as a null, invalid item

        /// <summary>
        /// Decrements the reference count. 
        /// <para></para>
        /// ---> Once Decrement() is called, the Data Store pointer is invalid and cannot be used safely.
        /// <para></para>
        /// ---> (This is because the Data Store may be removed or replaced with another control after Decrement() is called())
        /// </summary>
        /// <param name="iControlNum"></param>
        /// <returns></returns>
        bool                  DecrementID(int iDataStoreID);
        bool                  Decrement(int iControlID);
        bool                  Increment(int iControlID);
        bool                  Invalidate(DataStore_t * stDataStore);
        bool                  DeleteID(DataStore_t * stDataStore);
        bool                  DecrementAndInvalidate(DataStore_t * stDataStore);
        bool                  DecrementID(DataStore_t * stDataStore);
        bool                  Decrement(DataStore_t * stDataStore);
        bool                  Increment(DataStore_t * stDataStore);
        DataStore_t         * Find(int iControlID);
        DataStore_t         * FindID(int iDataStoreID,bool bReturnSafe = false);
        DataStore_t         * FindAndDeleteID(int iDataStoreID,bool bReturnSafe = false);
        bool                  Delete(int iControlID);
        void                  DeleteAll();
        bool                  SetControlInactive(int iControlID);
        int                   CreateUserIndex(); 

        // A way to look at the control active data directly without
        // making a call (hopefully)

        inline bool IsControlInactive(DataStore_t * stDataStore)
        {
            bool bReturn = false;
            if (!stDataStore) return bReturn;     // Something is wrong, but we don't know how to deal with it
            m_cDataStoreLock.Lock();
            bReturn = stDataStore->bControlActive; 
            m_cDataStoreLock.Unlock();
            return bReturn;
        }
        ~CDataStore();
   };

} // namespace Sage
