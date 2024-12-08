#include "BeeUtil.hpp"

#ifdef _WIN32
#	ifndef WIN_LEAN_AND_MEAN
#		define WIN_LEAN_AND_MEAN
#	endif // !WIN_LEAN_AND_MEAN

#	include <Windows.h>
#endif // _WIN32

using namespace Bee::Utils::Memory;

inline void Bee::Utils::Memory::InMemoryObjMove(void* pDest, void* pSource, const b_uintmem& uSourceSize)
{
    ::RtlMoveMemory(pDest, pSource, uSourceSize);
}