#pragma once
#include <intrin.h>
#include <functional>
namespace Sage
{

class CLockProcess
{
    bool bResult = false;
    #ifdef _WIN64
        typedef long LockType;
    #else
        typedef char LockType;
    #endif
        volatile LockType ltLock{0}; 
public:
    __forceinline void Unlock() { ltLock = 0; }

#ifdef _WIN64

    __declspec(noinline) bool Lock()
    {

	    __int64 iLockResult;

	
        // Use an atomic operation to try to get the lock.  The result returns the current (i.e. previous) value of the lock,
        // so when 0 is returned this means the value was 0 (i.e. lock was free) when _InterlockedCompareExchange64() 
        // was called.
        //
        // When 1 is returned, this means it was locked (and not data was exhanged)

        do iLockResult = _InterlockedCompareExchange(&ltLock,1,0);
	    while (iLockResult);
        return (bResult = (iLockResult == 0));

    }
    // Proper Use: if (FastLock()) { Locked Code } -- this is to ensure the 
    // optimizer executes the code completely before the Unlock().  Since this may not generate a call, the 
    // optimized may move some or all of the befor the Unlock() after the Unlock().  The if() above prevents this.
    //
    // Example:
    //
    // if (FastLock()) i = ++iLineCount;    // to get a line of data while it is locked. 
    // Unlock();
    //
    // This generate code without a call so in tight cache situations, it can be useful. 
    //
    __forceinline bool FastLock()
    {
	    __int64 iLockResult;

	
        // Use an atomic operation to try to get the lock.  The result returns the current (i.e. previous) value of the lock,
        // so when 0 is returned this means the value was 0 (i.e. lock was free) when _InterlockedCompareExchange64() 
        // was called.
        //
        // When 1 is returned, this means it was locked (and not data was exhanged)

        do iLockResult = _InterlockedCompareExchange(&ltLock,1,0);
	    while (iLockResult);
        return (bResult = (iLockResult == 0));
    }


   // Note: MSVC does not yet have an inline lambda, so this will generate a call when optimized
   //
    __forceinline int Lock(std::function<int()> const & Execute)
    {
        int iValue = 0;
        if (FastLock()) iValue = Execute();
        Unlock();
        return iValue;
    }

    // void() version of Lock(std::function<int()>...); 
    //
    __forceinline void Lockv(std::function<void()> const & Execute)
    {
        if (FastLock()) Execute();
        Unlock();
    }

#else

    __declspec(noinline) bool Lock()
    {

	    LockType iLockResult;

	
        // Use an atomic operation to try to get the lock.  The result returns the current (i.e. previous) value of the lock,
        // so when 0 is returned this means the value was 0 (i.e. lock was free) when _InterlockedCompareExchange64() 
        // was called.
        //
        // When 1 is returned, this means it was locked (and not data was exhanged)

        do iLockResult = _InterlockedCompareExchange8(&ltLock,1,0);
	    while (iLockResult);
        return (bResult = (iLockResult == 0));
   }

    // Proper Use: if (FastLock()) { Locked Code } -- this is to ensure the 
    // optimizer executes the code completely before the Unlock().  Since this may not generate a call, the 
    // optimized may move some or all of the befor the Unlock() after the Unlock().  The if() above prevents this.
    //
    // Example:
    //
    // if (FastLock()) i = ++iLineCount;    // to get a line of data while it is locked. 
    // Unlock();
    //
    // This generate code without a call so in tight cache situations, it can be useful. 
    //
   __forceinline bool FastLock()
    {
	    LockType iLockResult;

        // Use an atomic operation to try to get the lock.  The result returns the current (i.e. previous) value of the lock,
        // so when 0 is returned this means the value was 0 (i.e. lock was free) when _InterlockedCompareExchange64() 
        // was called.
        //
        // When 1 is returned, this means it was locked (and not data was exhanged)

        do iLockResult = _InterlockedCompareExchange8(&ltLock,1,0);
	    while (iLockResult);
        return (bResult = (iLockResult == 0));

   }

   // Note: MSVC does not yet have an inline lambda, so this will generate a call when optimized
   //       The inspected opimized code looks efficient, where it calls the lamdba function directly. 
   //       No doube at some point MSVC will institute inline lambdas
   //
    __forceinline int Lock(std::function<int()> const & Execute)
    {
        int iValue = 0;
        if (FastLock()) iValue = Execute();
        Unlock();
        return iValue;
    }

    // void() version of Lock(std::function<int()>...); 
    //
    __forceinline void Lockv(std::function<void()> const & Execute)
    {
        if (FastLock()) Execute();
        Unlock();
    }

#endif
};

} // namespace Sage