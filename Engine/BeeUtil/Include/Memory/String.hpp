#pragma once

#include "Memory.hpp"

namespace Bee::Utils::Memory
{
    BEE_API 
    float StoFC(
        const char* pBuff,
        const b_uintmem& uBuffSize,
        b_uintmem& cI);

    BEE_API inline char ToLower(char c);

    template<
        class       T,
        b_uintmem   uFmtSize,
        class...    TArgs>
    void ScanLine(
        const char* pBuff,
        const b_uintmem& uBuffsize,
        T(&pFmt)[uFmtSize],
        TArgs&... args)
    {
        b_uintmem i = 0, k = 0;

        ([&]
         {
             while (i != uFmtSize &&
                    k != uBuffsize)
             {
                 if (pFmt[i] == '%')
                 {
                     if ((++i) >= uFmtSize)
                     {
                         throw Bee::Problems::OutsideOfBuffer(BEE_COLLECT_DATA());
                     }

                     if (pFmt[i] == 'f')
                     {
                         args = StoFC(pBuff, uBuffsize, k);
                         ++i;
                         break;
                     }
                 }

                 ++k;
                 ++i;
             }

         }(), ...);
    }
}
