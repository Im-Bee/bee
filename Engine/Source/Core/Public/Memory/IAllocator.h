#pragma once

#include "CoreMinimal.h"
#include "AllocatorFlags.h"


namespace Duckers
{


template<class Type>
class IAllocator
{
public:

    Type* Allocate(usize, usize) 
    { throw; }

    void DeAllocate(Type*, usize = 0)
    { throw; }

    Type* ReAllocate(Type*, usize, usize)
    { throw; }

    constexpr static usize MinimumAlloc()
    { return 0; }

};


} // !Duckers
