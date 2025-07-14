#pragma once

#include "CoreMinimal.h"


namespace Duckers
{


template<class Type>
class DUCKERS_API IAllocator
{
public:

    virtual void Allocate(usize uSizeInBytes) = 0;

    virtual void DeAllocate() = 0;

    virtual void ReSize() = 0;

    virtual Type* Leak() = 0;

    inline virtual Type& Get(usize uIndex) = 0;

    inline virtual usize GetSizeInBytes() = 0;

};


} // !Duckers
