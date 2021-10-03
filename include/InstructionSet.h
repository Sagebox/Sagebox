#pragma once

// This is work in-progress, to be filled out as it gets used more. 

namespace Sage
{
    class CCpuID
    {
    public:

        static bool AVX();
        static bool AVX2();
        static bool AVX512();
        static bool SSE2();
        static bool SSE4();
        static bool AVX512F();  
        static bool AVX512DQ(); 
        static bool AVX512BW(); 
        static bool AVX512PF(); 
        static bool AVX512ER(); 
        static bool AVX512CD(); 
        static bool AVX512VL(); 
   };
}