#pragma once
#include <Windows.h>

namespace Sage
{
class CSageTimer
{
public:
    static constexpr int iDefaultAvgWidth = 50; 
    LARGE_INTEGER liFreq;
    LARGE_INTEGER liStart{};
    LARGE_INTEGER liEnd{};
    long * m_AvgData = nullptr; 
    int m_iAvgCount = 0;
    bool m_bAvgReady  = false;
    int m_iAvgWidth = iDefaultAvgWidth;
    int m_iAvgPlace = 0;
    bool AllocateAvgData(int iSize = -1)  // $$ move to .cpp file
    {
        if (m_AvgData) delete m_AvgData; 
        m_AvgData = nullptr;
        if (iSize < 0) iSize = iDefaultAvgWidth; 
        if (!iSize) return true;
        m_AvgData = new long[iSize]; 
        m_iAvgWidth = iSize;
        m_iAvgCount = 0;
        m_bAvgReady = 0;
        m_iAvgPlace = 0;
        if (m_AvgData) memset(m_AvgData,0,sizeof(m_AvgData[0])*iSize);
        return m_AvgData != nullptr;
    }

public:

    __forceinline void ResetAvg()
    {
        m_iAvgCount = 0;
        m_bAvgReady = false;
        m_iAvgPlace = 0;

    }
    bool InitAvg(int iMaxCount = -1)
    {
        bool bReturn = AllocateAvgData(iMaxCount);
        return iMaxCount ? bReturn : true;  // Always true if we're de-allocating. 

    }
    __forceinline long CalcAvg()
    {
        if (!m_AvgData) return 0;
        int iCount = m_bAvgReady ? m_iAvgWidth : m_iAvgCount;
        if (!iCount) return 0;
        long long llAvg = 0;

        for (int i=0;i<iCount;i++) llAvg += (long long) m_AvgData[i];
        return (long) (llAvg/(long long) iCount); 
    }
    __forceinline double CalcAvgf()
    {
        if (!m_AvgData) return 0;
        int iCount = m_bAvgReady ? m_iAvgWidth : m_iAvgCount;
        long long llAvg = 0;

        for (int i=0;i<iCount++;i++) llAvg += (long long) m_AvgData[i];
        return (double) llAvg/(double) iCount; 
    }

    __forceinline bool AvgReady() { return m_bAvgReady; }
    __forceinline bool AddAvg(long iValue)
    {
        if (!m_AvgData) AllocateAvgData(-1); 
        if (!m_AvgData) return false; 
        m_AvgData[m_iAvgPlace] = iValue;
        if (++m_iAvgPlace >= m_iAvgWidth) m_iAvgPlace = 0;
        if (!m_bAvgReady && ++m_iAvgCount >= m_iAvgWidth) m_bAvgReady = true; 
        return m_bAvgReady;
    }
    __forceinline bool AddAvg(long long iValue) { return AddAvg((long) iValue); }
     __forceinline bool AddAvg(int iValue) { return AddAvg((long) iValue); }


    /// <summary>
    /// Starts timer. Same as Reset. 
    /// <para></para>
    /// Note: Instantaing the CSageTimer class starts the timer.
    /// <para></para>
    /// --> Example: CSageTimer MyTimer;   // Starts timer.  Use ElapsedMs(), ElapsedUs(), etc. to read time since start. 
    /// </summary>
    __forceinline void Start() { QueryPerformanceCounter(&liStart); }

    /// <summary>
    /// Returns elapsed microseconds since timer start or reset.
    /// </summary>
    /// <param name="bReset"></param>
    /// When true, resets the timer.   When false, timer continues from last reset/start.
    /// <returns></returns>
   __forceinline decltype(LARGE_INTEGER::QuadPart) ElapsedUs() { QueryPerformanceCounter(&liEnd); return (((liEnd.QuadPart-liStart.QuadPart)*1000000)/liFreq.QuadPart); }
   __forceinline double ElapsedUsf() { return (double) ElapsedNs()/1000; }

    /// <summary>
    /// Returns elapsed nanoseconds since timer start or reset.
    /// </summary>
    /// <param name="bReset"></param>
    /// When true, resets the timer.   When false, timer continues from last reset/start.
    /// <returns></returns>
    __forceinline decltype(LARGE_INTEGER::QuadPart) ElapsedNs() { QueryPerformanceCounter(&liEnd); return (((liEnd.QuadPart-liStart.QuadPart)*1000000000)/liFreq.QuadPart); }
   __forceinline double ElapsedNsf() { return (double) ElapsedNs(); }

    /// <summary>
    /// Returns elapsed milliseconds since timer start or reset.
    /// </summary>
    /// <param name="bReset"></param>
    /// When true, resets the timer.   When false, timer continues from last reset/start.
    /// <returns></returns>
    __forceinline decltype(LARGE_INTEGER::QuadPart) ElapsedMs() { QueryPerformanceCounter(&liEnd); return (((liEnd.QuadPart-liStart.QuadPart)*1000)/liFreq.QuadPart); };
   __forceinline double ElapsedMsf() { return (double) ElapsedNs()/1000000; }

    /// <summary>
    /// Returns elapsed nanoseconds since timer start or reset.
    /// </summary>
    /// <param name="bReset"></param>
    /// When true, resets the timer.   When false, timer continues from last reset/start.
    /// <returns></returns>
    __forceinline decltype(LARGE_INTEGER::QuadPart) ElapsedNs(bool bReset) { QueryPerformanceCounter(&liEnd); return (((liEnd.QuadPart-liStart.QuadPart)*1000000000)/liFreq.QuadPart);  if (bReset) Reset(); }
    __forceinline double ElapsedNsf(bool bReset) { return (double) ElapsedNs(bReset);  }

    /// <summary>
    /// Returns elapsed microseconds since timer start or reset.
    /// </summary>
    /// <param name="bReset"></param>
    /// When true, resets the timer.   When false, timer continues from last reset/start.
    /// <returns></returns>
    __forceinline decltype(LARGE_INTEGER::QuadPart) ElapsedUs(bool bReset) { QueryPerformanceCounter(&liEnd); return (((liEnd.QuadPart-liStart.QuadPart)*1000000)/liFreq.QuadPart);     if (bReset) Reset(); }
    __forceinline double ElapsedUsf(bool bReset) { return (double) ElapsedNs(bReset)/1000; }

 
    /// <summary>
    /// Returns elapsed milliseconds since timer start or reset.
    /// </summary>
    /// <param name="bReset"></param>
    /// When true, resets the timer.   When false, timer continues from last reset/start.
    /// <returns></returns>
    __forceinline decltype(LARGE_INTEGER::QuadPart) ElapsedMs(bool bReset) { QueryPerformanceCounter(&liEnd); return (((liEnd.QuadPart-liStart.QuadPart)*1000)/liFreq.QuadPart);        if (bReset) Reset(); };
    __forceinline double ElapsedMsf(bool bReset) { return (double) ElapsedMs(bReset); };

    /// <summary>
    /// Resets timer to current time (same as Start())
    /// <para></para>
    /// Note: Instantaing the CSageTimer class starts the timer.
    /// <para></para>
    /// --> Example: CSageTimer MyTimer;   // Starts timer.  Use ElapsedMs(), ElapsedUs(), etc. to read time since start. 
    /// </summary>
    __forceinline void Reset() { Start(); };
    CSageTimer()
    {
        QueryPerformanceFrequency(&liFreq); 
        Start();
    }

};

} // namespace Sage