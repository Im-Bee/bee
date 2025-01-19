#include "BeeUtil.hpp"

using namespace Bee::Utils::Memory;

inline void Bee::Utils::Memory::InMemoryObjMove(      void*      pDest, 
                                                      void*      pSource, 
                                                const b_uintmem& uSourceSize)
{
    ::RtlMoveMemory(pDest, pSource, uSourceSize);
}