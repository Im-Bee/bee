#pragma once

#include "Memory.hpp"

namespace Bee::Utils::Memory
{
    BEE_API float StoFC(const char*    pBuff,
                        const b_usize& uBuffSize,
                              b_usize& cI);

    BEE_API int StoIC(const char*    pBuff,
                      const b_usize& uBuffSize,
                            b_usize& cI);

    BEE_API inline char ToLower(char c);

    template<class     T,
             b_usize   uFmtSize,
             class...  TArgs>
    void ScanLine(const char*      pBuff,
                  const b_usize&   uBuffsize,
                        T          (&pFmt)[uFmtSize],
                        TArgs&...  args)
    {
        b_usize i = 0, k = 0;

        ([&]
         {
             while (i != uFmtSize && k != uBuffsize)
             {
                 if (pFmt[i] == '%')
                 {
                     if ((++i) >= uFmtSize)
                     {
                         throw ::Bee::Debug::OutsideOfBuffer(BEE_COLLECT_DATA_ON_EXCEPTION());
                     }

                     if (pFmt[i] == 'f')
                     {
                         args = StoFC(pBuff, uBuffsize, k);
                         ++i;
                         break;
                     }

                     if (pFmt[i] == 'd')
                     {
                         args = StoIC(pBuff, uBuffsize, k);
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
